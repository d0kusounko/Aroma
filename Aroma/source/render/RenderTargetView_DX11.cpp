//===========================================================================
//!
//!	@file		RenderTargetView_DX11.h
//!	@brief		レンダーターゲットビュー : DirectX11
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/render/RenderTargetView.h>
#include <aroma/render/Texture.h>
#include <aroma/render/Device.h>

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//! @brief		コンストラクタ.
//---------------------------------------------------------------------------
RenderTargetView::RenderTargetView()
	: _initialized( false )
	, _device( nullptr )
	, _texture( nullptr )
	, _nativeRTV( nullptr )
{
}

//---------------------------------------------------------------------------
//! @brief		デストラクタ.
//---------------------------------------------------------------------------
RenderTargetView::~RenderTargetView()
{
	Finalize();
}

//---------------------------------------------------------------------------
//! @brief		初期化.
//---------------------------------------------------------------------------
void RenderTargetView::Initialize( Device* device, const Desc& desc )
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

	// レンダーターゲットビュー作成.
	AROMA_ASSERT( CheckFlags( textureDesc.bindFlags, kBindFlagRenderTarget ),
		"There is no kBindFlagRenderTarget in texture bind flags." );

	D3D11_RENDER_TARGET_VIEW_DESC d3dRTVDesc = {};
	d3dRTVDesc.Format	= ToNativePixelFormat( textureDesc.format );

	// TODO: 1D,3D Dimension対応.
	// TODO: Array対応.
	// TODO: CubeMap対応.

	if( textureDesc.multiSampleCount > 1 )
	{
		d3dRTVDesc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2DMS;
	}
	else
	{
		d3dRTVDesc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
		d3dRTVDesc.Texture2D.MipSlice	= desc.mipLevel;
	}
	hr = d3dDevice->CreateRenderTargetView( _texture->GetNativeResource(), &d3dRTVDesc, &_nativeRTV );
	AROMA_ASSERT( SUCCEEDED( hr ), _T( "Failed to CreateRenderTargetView.\n" ) );

	_initialized = true;
}

//---------------------------------------------------------------------------
//! @brief		解放.
//---------------------------------------------------------------------------
void RenderTargetView::Finalize()
{
	if( !_initialized ) return;
	memory::SafeRelease( _nativeRTV );
	memory::SafeRelease( _texture );
	memory::SafeRelease( _device );
	_desc.Default();
	_initialized = false;
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIレンダーターゲットビュー取得.
//---------------------------------------------------------------------------
ID3D11RenderTargetView*	RenderTargetView::GetNativeRenderTargetView() const
{
	return _nativeRTV;
}

} // namespace render
} // namespace aroma
