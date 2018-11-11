//===========================================================================
//!
//!	@file		DepthStencilView_DX11.h
//!	@brief		深度ステンシルビュー : DirectX11
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/render/DepthStencilView.h>
#include <aroma/render/Texture.h>
#include <aroma/render/Device.h>

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//! @brief		コンストラクタ.
//---------------------------------------------------------------------------
DepthStencilView::DepthStencilView()
	: _initialized( false )
	, _device( nullptr )
	, _texture( nullptr )
	, _nativeDSV( nullptr )
{
}

//---------------------------------------------------------------------------
//! @brief		デストラクタ.
//---------------------------------------------------------------------------
DepthStencilView::~DepthStencilView()
{
	Finalize();
}

//---------------------------------------------------------------------------
//! @brief		初期化.
//---------------------------------------------------------------------------
void DepthStencilView::Initialize( Device* device, const Desc& desc )
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

	// 深度ステンシルビュー作成.
	AROMA_ASSERT( CheckFlags( textureDesc.bindFlags, kBindFlagDepthStencil ),
		"There is no kBindFlagDepthStencil in texture bind flags." );

	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDSVDesc = {};
	d3dDSVDesc.Format	= ToNativePixelFormat( textureDesc.format );

	// TODO: 1D,3D Dimension対応.
	// TODO: Array対応.
	// TODO: CubeMap対応.

	if( textureDesc.multiSampleCount > 1 )
	{
		d3dDSVDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2DMS;
	}
	else
	{
		d3dDSVDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
		d3dDSVDesc.Texture2D.MipSlice	= desc.mipLevel;
	}

	hr = d3dDevice->CreateDepthStencilView( _texture->GetNativeResource(), &d3dDSVDesc, &_nativeDSV );
	AROMA_ASSERT( SUCCEEDED( hr ), _T( "Failed to CreateDepthStencilView.\n" ) );


	_initialized = true;
}

//---------------------------------------------------------------------------
//! @brief		解放.
//---------------------------------------------------------------------------
void DepthStencilView::Finalize()
{
	if( !_initialized ) return;
	memory::SafeRelease( _nativeDSV );
	memory::SafeRelease( _texture );
	memory::SafeRelease( _device );
	_desc.Default();
	_initialized = false;
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPI深度ステンシルビュー取得.
//---------------------------------------------------------------------------
ID3D11DepthStencilView* DepthStencilView::GetNativeDepthStencilView() const
{
	return _nativeDSV;
}

} // namespace render
} // namespace aroma
