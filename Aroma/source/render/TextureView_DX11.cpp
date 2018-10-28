//===========================================================================
//!
//!	@file		TextureView_DX11.h
//!	@brief		テクスチャビュー : DirectX11
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/render/TextureView.h>
#include <aroma/render/Texture.h>
#include <aroma/render/Device.h>

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//! @brief		コンストラクタ.
//---------------------------------------------------------------------------
TextureView::TextureView()
	: _initialized( false )
	, _device( nullptr )
	, _nativeSRV( nullptr )
	, _nativeRTV( nullptr )
	, _nativeDSV( nullptr )
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

	// シェーダーリソースビュー作成.
	if( ( u32 )textureDesc.bindFlags & kBindFlagShaderResource )
	{
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
			d3dSRVDesc.Texture2D.MipLevels			= textureDesc.mipLevel;
			d3dSRVDesc.Texture2D.MostDetailedMip	= 0;
		}

		hr = d3dDevice->CreateShaderResourceView( _texture->GetNativeResource(), &d3dSRVDesc, &_nativeSRV );
		AROMA_ASSERT( SUCCEEDED( hr ), _T( "Failed to CreateShaderResourceView.\n" ) );
	}

	// レンダーターゲットビュー作成.
	if( ( u32 )textureDesc.bindFlags & kBindFlagRenderTarget )
	{
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
			d3dRTVDesc.ViewDimension       = D3D11_RTV_DIMENSION_TEXTURE2D;
			d3dRTVDesc.Texture2D.MipSlice  = 0;	// TOOD: MipSliceをDescより設定.
		}
		hr = d3dDevice->CreateRenderTargetView( _texture->GetNativeResource(), &d3dRTVDesc, &_nativeRTV );
		AROMA_ASSERT( SUCCEEDED( hr ), _T( "Failed to CreateRenderTargetView.\n" ) );
	}

	// 深度ステンシルビュー作成.
	if( ( u32 )textureDesc.bindFlags & kBindFlagDepthStencil )
	{
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
			d3dDSVDesc.Texture2D.MipSlice	= 0;	// TOOD: MipSliceをDescより設定.
		}

		hr = d3dDevice->CreateDepthStencilView( _texture->GetNativeResource(), &d3dDSVDesc, &_nativeDSV );
		AROMA_ASSERT( SUCCEEDED( hr ), _T( "Failed to CreateDepthStencilView.\n" ) );
	}

	_initialized = true;
}

//---------------------------------------------------------------------------
//! @brief		解放.
//---------------------------------------------------------------------------
void TextureView::Finalize()
{
	if( !_initialized ) return;
	memory::SafeRelease( _nativeSRV );
	memory::SafeRelease( _nativeRTV );
	memory::SafeRelease( _nativeDSV );
	memory::SafeRelease( _texture );
	memory::SafeRelease( _device );
	_desc.Default();
	_initialized = false;
}



//---------------------------------------------------------------------------
//! @brief		ネイティブAPIシェーダーリソースビュー取得.
//---------------------------------------------------------------------------
ID3D11ShaderResourceView* TextureView::GetNativeShaderResourceView() const
{
	return _nativeSRV;
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIレンダーターゲットビュー取得.
//---------------------------------------------------------------------------
ID3D11RenderTargetView*	TextureView::GetNativeRenderTargetView() const
{
	return _nativeRTV;
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPI深度ステンシルビュー取得.
//---------------------------------------------------------------------------
ID3D11DepthStencilView* TextureView::GetNativeDepthStencilView() const
{
	return _nativeDSV;
}

} // namespace render
} // namespace aroma
