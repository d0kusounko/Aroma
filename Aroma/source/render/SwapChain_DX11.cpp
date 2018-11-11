//===========================================================================
//!
//!	@file		SwapChain_DX11.cpp
//!	@brief		スワップチェイン : DirectX11.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#ifdef AROMA_RENDER_DX11

#include <aroma/render/SwapChain.h>
#include <aroma/render/Device.h>
#include <aroma/render/Texture.h>
#include <aroma/render/RenderTargetView.h>
#include <aroma/app/App.h>

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//!	@brief		コンストラクタ.
//---------------------------------------------------------------------------
SwapChain::SwapChain()
	: _initialized( false )
	, _device( nullptr )
	, _buffers( nullptr )
	, _bufferRTVs( nullptr )
	, _bufferIndex( 0 )
	, _d3dSwapChain( nullptr )
{
}

//---------------------------------------------------------------------------
//!	@brief		デストラクタ.
//---------------------------------------------------------------------------
SwapChain::~SwapChain()
{
	Finalize();
}

//---------------------------------------------------------------------------
//!	@brief		初期化.
//---------------------------------------------------------------------------
void SwapChain::Initialize( Device* device, const Desc& desc )
{
	if( _initialized )
	{
		AROMA_ASSERT( false, _T( "Already initialized.\n" ) );
		Finalize();
	}

	_device = device;
	_device->AddRef();
	_desc = desc;
	_desc.window->AddRef();

	auto d3dDevice	= _device->GetNativeDevice();
	auto d3dFactory	= _device->GetDXGIFactory();
	HRESULT hr;

    // スワップチェイン生成.
    {
        DXGI_SWAP_CHAIN_DESC d3dDesc = {};
        d3dDesc.BufferDesc.Width	= desc.size.width;
        d3dDesc.BufferDesc.Height	= desc.size.height;
        d3dDesc.BufferDesc.Format	= ToNativePixelFormat( desc.format );
        d3dDesc.SampleDesc.Count	= 1;
        d3dDesc.SampleDesc.Quality	= 0;
        d3dDesc.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
        d3dDesc.BufferCount			= desc.bufferCount;
        d3dDesc.Windowed			= TRUE;	// TODO: フルスクリーン対応.
        d3dDesc.OutputWindow		= desc.window->GetNativeHandle();
        // d3dDesc.Flags			= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        d3dDesc.Flags				= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// TODO: フルスクリーン対応.
		d3dDesc.SwapEffect			= DXGI_SWAP_EFFECT_DISCARD;

        hr = d3dFactory->CreateSwapChain( d3dDevice, &d3dDesc, &_d3dSwapChain );
		AROMA_ASSERT( SUCCEEDED( hr ), _T( "Failed to CreateSwapChain.\n" ) );
    }

	// スワップチェインのバッファよりAromaテクスチャバッファを作成.
	{
		_buffers	= new Texture*[ desc.bufferCount ];
		_bufferRTVs	= new RenderTargetView*[ desc.bufferCount ];

		// DXGI_SWAP_EFFECT_DISCARD なので, 0番目のバッファを使い回す.
		ID3D11Texture2D* d3dBuffer;
		hr = _d3dSwapChain->GetBuffer( 0, IID_PPV_ARGS( &d3dBuffer ) );
		AROMA_ASSERT( SUCCEEDED( hr ), _T( "Failed to GetBuffer.\n" ) );

		for( u32 i = 0; i < desc.bufferCount; ++i )
		{
			_buffers[ i ]		= nullptr;
			_bufferRTVs[ i ]	= nullptr;

			// 取得したネイティブAPIバッファからAromaテクスチャバッファ作成.
			{
				_buffers[ i ] = new Texture();
				_buffers[ i ]->InitializeFromNativeTexture( _device, d3dBuffer );
			}

			// レンダーターゲットビュー作成.
			{
				_bufferRTVs[ i ] = new RenderTargetView();
				RenderTargetView::Desc desc;
				desc.texture	= _buffers[ i ];
				desc.mipLevel	= 0;
				_bufferRTVs[ i ]->Initialize( _device, desc );
			}
		}
		memory::SafeRelease( d3dBuffer );
	}
	_bufferIndex = 0;

	_initialized = true;
	return;
}

//-----------------------------------------------------------------------
//!	@brief		解放.
//-----------------------------------------------------------------------
void SwapChain::Finalize()
{
	if( !_initialized ) return;

	for( u32 i = 0; i < _desc.bufferCount; ++i )
	{
		memory::SafeRelease( _bufferRTVs[ i ] );
		memory::SafeRelease( _buffers[ i ] );
	}
	memory::SafeDelete( _bufferRTVs );
	memory::SafeDelete( _buffers );
	memory::SafeRelease( _d3dSwapChain );
	memory::SafeRelease( _device );
	memory::SafeRelease( _desc.window );
	_desc.Default();

	_initialized = false;
}


//---------------------------------------------------------------------------
// @brief		バッファを画面に出力.
//---------------------------------------------------------------------------
void SwapChain::Present( u32 syncInterval )
{
	_d3dSwapChain->Present( syncInterval, 0 );
	_bufferIndex = (_bufferIndex + 1 ) % _desc.bufferCount;	// バッファフリップ.
}

//-----------------------------------------------------------------------
//! @brief		構成設定取得.
//-----------------------------------------------------------------------
const SwapChain::Desc& SwapChain::GetDesc() const
{
	return _desc;
}

//-----------------------------------------------------------------------
//! @brief		現在のバッファのインデックス取得.
//-----------------------------------------------------------------------
u32 SwapChain::GetCurrentBufferIndex() const
{
	return _bufferIndex;
}

//-----------------------------------------------------------------------
//! @brief		 バッファ取得.
//-----------------------------------------------------------------------
void SwapChain::GetBuffer( u32 index, Texture** outResource ) const
{
	AROMA_ASSERT( index < _desc.bufferCount, "index is out of range" );

	(*outResource) = _buffers[ index ];
	(*outResource)->AddRef();
}

//-----------------------------------------------------------------------
//! @brief		 バッファビュー取得.
//-----------------------------------------------------------------------
void SwapChain::GetBufferView( u32 index, RenderTargetView** outResource ) const
{
	AROMA_ASSERT( index < _desc.bufferCount, "index is out of range" );

	(*outResource) = _bufferRTVs[ index ];
	(*outResource)->AddRef();
}

} // namespace render
} // namespace aroma

#endif
