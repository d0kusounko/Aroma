﻿//===========================================================================
//!
//!	@file		TextureView_DX11.h
//!	@brief		テクスチャービュー : DirectX11
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/render/TextureView.h>
#include <aroma/render/Device.h>

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//! @brief		コンストラクタ.
//---------------------------------------------------------------------------
TextureView::TextureView()
	: _initialized( false )
	, _device( nullptr )
	, _texture( nullptr )
	, _nativeSRV( nullptr )
{
}

//---------------------------------------------------------------------------
//! @brief		デストラクタ.
//---------------------------------------------------------------------------
TextureView::~TextureView()
{
	Finalize();
}

//---------------------------------------------------------------------------
//! @brief		初期化.
//---------------------------------------------------------------------------
void TextureView::Initialize( Device* device, const Desc& desc )
{
	if( _initialized )
	{
		AROMA_ASSERT( false, _T( "Already initialized.\n" ) );
		Finalize();
	}

	_device = device;
	_device->AddRef();
	_desc = desc;

	_texture = desc.texture;
	_texture->AddRef();
	auto d3dDevice		= _device->GetNativeDevice();
	auto textureDesc	= _texture->GetDesc();

	HRESULT hr;

	// テクスチャービュー作成.
	AROMA_ASSERT( CheckFlags( textureDesc.bindFlags, kBindFlagShaderResource ),
		"There is no kBindFlagShaderResource in texture bind flags." );

	D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc = {};
	switch( textureDesc.format )
	{
		// 深度フォーマットの場合はカラーフォーマットに変換.
		case data::PixelFormat::kD32FloatS8X24Uint:
			d3dSRVDesc.Format	= DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
			break;
		case data::PixelFormat::kD32Float:
			d3dSRVDesc.Format	= DXGI_FORMAT_R32_FLOAT;
			break;
		case data::PixelFormat::kD24UnormS8Uint:
			d3dSRVDesc.Format	= DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			break;
		case data::PixelFormat::kD16Unorm:
			d3dSRVDesc.Format	= DXGI_FORMAT_R16_UNORM;
			break;

		default:
			d3dSRVDesc.Format	= ToNativePixelFormat( textureDesc.format );
			break;
	}

	// TODO: 1D,3D Dimension対応.
	// TODO: Array対応.
	// TODO: CubeMap対応.

	if( textureDesc.multiSampleCount > 1 )
	{
		d3dSRVDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2DMS;
	}
	else
	{
		d3dSRVDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
		d3dSRVDesc.Texture2D.MipLevels			= textureDesc.mipCount;
		d3dSRVDesc.Texture2D.MostDetailedMip	= 0;
	}

	hr = d3dDevice->CreateShaderResourceView( _texture->GetNativeResource(), &d3dSRVDesc, &_nativeSRV );
	AROMA_ASSERT( SUCCEEDED( hr ), _T( "Failed to CreateShaderResourceView.\n" ) );

	_initialized = true;
}

//---------------------------------------------------------------------------
//! @brief		解放.
//---------------------------------------------------------------------------
void TextureView::Finalize()
{
	if( !_initialized ) return;
	memory::SafeRelease( _nativeSRV );
	memory::SafeRelease( _texture );
	memory::SafeRelease( _device );
	_desc.Default();
	_initialized = false;
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIテクスチャービュー取得.
//---------------------------------------------------------------------------
ID3D11ShaderResourceView* TextureView::GetNativeShaderResourceView() const
{
	return _nativeSRV;
}

} // namespace render
} // namespace aroma
