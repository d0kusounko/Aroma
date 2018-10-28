//===========================================================================
//!
//!	@file		DeferredContext_DX11.cpp
//!	@brief		遅延コンテキスト : DirectX11.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#ifdef AROMA_RENDER_DX11

#include <aroma/render/DeferredContext.h>
#include <aroma/render/Render.h>
#include <aroma/render/Device.h>
#include <aroma/render/Texture.h>
#include <aroma/render/TextureView.h>
#include <aroma/render/CommandList.h>
#include <aroma/render/Shader.h>
#include <aroma/render/RenderStateCache.h>

namespace aroma {
namespace render {

#define BEGIN_ERROR_CHECK()												\
	if( !_begin )														\
	{																	\
		AROMA_ASSERT( false, _T( "Command recording has not began." ) );\
		return;															\
	}																	\

//---------------------------------------------------------------------------
//!	@brief		コンストラクタ.
//---------------------------------------------------------------------------
DeferredContext::DeferredContext()
	: _initialized( false )
	, _device( nullptr )
	, _d3dContext( nullptr )
	, _begin( false )
{
}

//---------------------------------------------------------------------------
//!	@brief		デストラクタ.
//---------------------------------------------------------------------------
DeferredContext::~DeferredContext()
{
	Finalize();
}

//---------------------------------------------------------------------------
//!	@brief		初期化.
//---------------------------------------------------------------------------
void DeferredContext::Initialize( Device* device, const Desc& desc )
{
	if( _initialized )
	{
		AROMA_ASSERT( false, _T( "Already initialized.\n" ) );
		Finalize();
	}

	_device = device;
	_device->AddRef();
	_desc = desc;

	auto d3dDevice = _device->GetNativeDevice();
	HRESULT hr;

	hr = d3dDevice->CreateDeferredContext( 0, &_d3dContext );
	AROMA_ASSERT( SUCCEEDED( hr ), _T( "Failed to CreateDeferredContext.\n" ) );

	_initialized = true;
	return;
}

//---------------------------------------------------------------------------
//!	@brief		解放.
//---------------------------------------------------------------------------
void DeferredContext::Finalize()
{
	if( !_initialized ) return;

	_blendState.Default();
	_rasterizerState.Default();
	_depthStencilState.Default();
	for( auto& state : _psSamplerStates )
	{
		state.Default();
	}
	memory::SafeRelease( _d3dContext );
	memory::SafeRelease( _device );
	_desc.Default();

	_initialized = false;
}

//---------------------------------------------------------------------------
//!	@brief		コマンド記録開始.
//---------------------------------------------------------------------------
void DeferredContext::Begin()
{
	if( _begin )
	{
		AROMA_ASSERT( false, _T( "Command recording has already begun." ) );
		return;
	};

	// 描画パイプラインの復元.
	for( u32 i = 0; i < kPipelineDirtyBitFlagNum; ++i )
	{
		_pipelineDirtyBits[ i ] = true;
	}

	_begin = true;
}

//---------------------------------------------------------------------------
//!	@brief		コマンド記録終了.
//---------------------------------------------------------------------------
void DeferredContext::End( CommandList** outCommandList )
{
	BEGIN_ERROR_CHECK();

	if( outCommandList )
	{
		// D3Dコマンドリスト生成.
		ID3D11CommandList* d3dCommandList;
		HRESULT hr = _d3dContext->FinishCommandList( FALSE, &d3dCommandList );
		AROMA_ASSERT( SUCCEEDED( hr ), _T( "Failed to FinishCommandList.\n" ) );

		// Aromaコマンドリストを生成してインスタンス返却.
		CommandList* commandList = new render::CommandList();
		commandList->Initialize( _device, d3dCommandList );
		d3dCommandList->Release();
		(*outCommandList) = commandList;
	}

	_begin = false;
}

//===========================================================================
//!	@name		コマンド群.
//===========================================================================
//! @{

//---------------------------------------------------------------------------
//!	@brief		レンダーターゲットを指定カラーでクリア.
//---------------------------------------------------------------------------
void DeferredContext::ClearRenderTarget( TextureView* rtv, const data::Color& color )
{
	BEGIN_ERROR_CHECK();
	_d3dContext->ClearRenderTargetView( rtv->GetNativeRenderTargetView(), color.rgba );
}

//---------------------------------------------------------------------------
//! @brief		描画.
//!	@param[in]	vertexNum			描画する頂点数.
//!	@param[in]	startVertexIndex	先頭の頂点番号.
//--------------------------------------------------------------------------
void DeferredContext::Draw( u32 vertexNum, u32 startVertexIndex )
{
	BEGIN_ERROR_CHECK();
	SyncDrawPipeline();
	_d3dContext->Draw( vertexNum, startVertexIndex );
}

//---------------------------------------------------------------------------
//! @brief		インデックス付き描画.
//!	@param[in]	indexNum			描画するインデックス数.
//!	@param[in]	startIndex			先頭の頂点インデックス番号.
//!	@param[in]	baseVertexIndex		頂点バッファから頂点を読み取る前に各インデックスに加算する値.
//---------------------------------------------------------------------------
void DeferredContext::DrawIndexed( u32 indexNum, u32 startIndex, u32 baseVertexIndex )
{
	BEGIN_ERROR_CHECK();
	SyncDrawPipeline();
	_d3dContext->DrawIndexed( indexNum, startIndex, baseVertexIndex );
}

//===========================================================================
//!	@name		VS: 頂点シェーダーステージ.
//===========================================================================
//! @{
//---------------------------------------------------------------------------
//!	@brief		頂点シェーダー設定.
//---------------------------------------------------------------------------
void DeferredContext::VSSetShader( const Shader* vs )
{
	BEGIN_ERROR_CHECK();
	_d3dContext->VSSetShader( vs->GetNativeVertexShader(), nullptr, 0 );
}
//! @}

//===========================================================================
//!	@name		PS: ピクセルシェーダーステージ.
//===========================================================================
//! @{
//---------------------------------------------------------------------------
//!	@brief		ピクセルシェーダー設定.
//---------------------------------------------------------------------------
void DeferredContext::PSSetShader( const Shader* ps )
{
	BEGIN_ERROR_CHECK();
	_d3dContext->PSSetShader( ps->GetNativePixelShader(), nullptr, 0 );
}

//---------------------------------------------------------------------------
//!	@brief		シェーダーリソース設定.
//---------------------------------------------------------------------------
void DeferredContext::PSSetShaderResource( u32 slot, const TextureView* srv )
{
	BEGIN_ERROR_CHECK();
	ID3D11ShaderResourceView* const srvs[] = { srv->GetNativeShaderResourceView() };
	_d3dContext->PSSetShaderResources( slot, 1, srvs );
}

//---------------------------------------------------------------------------
//!	@brief		シェーダーリソース設定.
//---------------------------------------------------------------------------
void DeferredContext::PSSetConstantBuffer( u32 slot, const Buffer* cb )
{
	BEGIN_ERROR_CHECK();
	ID3D11Buffer* const cbs[] = { cb->GetNativeBuffer() };
	_d3dContext->PSSetConstantBuffers( slot, 1, cbs );
}
//! @}

//===========================================================================
//!	@name		IA: 入力アセンブラーステージ.
//===========================================================================
//! @{
//---------------------------------------------------------------------------
//!	@brief		入力レイアウト設定.
//---------------------------------------------------------------------------
void DeferredContext::IASetInputLayout( const InputLayout* inputLayout )
{
	BEGIN_ERROR_CHECK();
	_d3dContext->IASetInputLayout( inputLayout->GetNativeInputLayout() );
}

//---------------------------------------------------------------------------
//!	@brief		プリミティブタイプ設定.
//---------------------------------------------------------------------------
void DeferredContext::IASetPrimitiveType( const PrimitiveType primitiveType )
{
	BEGIN_ERROR_CHECK();
	_d3dContext->IASetPrimitiveTopology( ToNativePrimitiveType( primitiveType ) );
}

//-----------------------------------------------------------------------
//!	@brief		インデックスバッファ設定.
//-----------------------------------------------------------------------
void DeferredContext::IASetIndexBuffer( const Buffer* indexBuffer, u32 offset )
{
	BEGIN_ERROR_CHECK();

	DXGI_FORMAT d3dFortmat = DXGI_FORMAT_UNKNOWN;
	switch( GetIndexTypeFromBufferStride( indexBuffer->GetDesc().stride ) )
	{
		case IndexType::k16:
			d3dFortmat = DXGI_FORMAT_R16_UINT;
			break;
		case IndexType::k32:
			d3dFortmat = DXGI_FORMAT_R32_UINT;
			break;
		default:
			AROMA_ASSERT( false, _T( "Undefined index type.\n" ) );
			break;
	}

	_d3dContext->IASetIndexBuffer( indexBuffer->GetNativeBuffer(), d3dFortmat, offset );
}

//-----------------------------------------------------------------------
//!	@brief		頂点バッファ設定.
//-----------------------------------------------------------------------
void DeferredContext::IASetVertexBuffers( u32 startStreamIdx, u32 bufferNum, Buffer* const* buffers, const u32* strides, const u32* offsets )
{
	BEGIN_ERROR_CHECK();
	if( bufferNum == 0 ) return;

	ID3D11Buffer**	d3dBuffers = new ID3D11Buffer*[ bufferNum ];
	for( u32 i = 0; i < bufferNum; ++i )
	{
		d3dBuffers[ i ] = buffers[ i ]->GetNativeBuffer();
	}
	_d3dContext->IASetVertexBuffers( startStreamIdx, bufferNum, d3dBuffers, strides, offsets );
	memory::SafeDeleteArray( d3dBuffers );
}

//! @}

//===========================================================================
//!	@name		RS: ラスタライザーステージ.
//===========================================================================
//! @{
//---------------------------------------------------------------------------
//!	@brief		ビューポート設定.
//---------------------------------------------------------------------------
void DeferredContext::RSSetViewports( u32 viewportNum, const Viewport* viewports )
{
	BEGIN_ERROR_CHECK();

	if( viewportNum == 0 ) return;

	D3D11_VIEWPORT* d3dViewports = new D3D11_VIEWPORT[ viewportNum ];
	for( u32 i = 0; i < viewportNum; ++i )
	{
		d3dViewports[ i ].TopLeftX 	= viewports[ i ].x;
		d3dViewports[ i ].TopLeftY 	= viewports[ i ].y;
		d3dViewports[ i ].Width 	= viewports[ i ].w;
		d3dViewports[ i ].Height 	= viewports[ i ].h;
		d3dViewports[ i ].MinDepth 	= viewports[ i ].minDepth;
		d3dViewports[ i ].MaxDepth 	= viewports[ i ].maxDepth;
	}
	_d3dContext->RSSetViewports( viewportNum, d3dViewports );
	memory::SafeDeleteArray( d3dViewports );
}

//---------------------------------------------------------------------------
//!	@brief		シザー設定.
//---------------------------------------------------------------------------
void DeferredContext::RSSetScissors( u32 scissorsNum, const ScissorRect* scisscorRects )
{
	BEGIN_ERROR_CHECK();

	if( scissorsNum == 0 ) return;

	D3D11_RECT* rects = new D3D11_RECT[ scissorsNum ];
	for( u32 i = 0; i < scissorsNum; ++i )
	{
		rects[ i ].left		= scisscorRects[ i ].x;
		rects[ i ].top		= scisscorRects[ i ].y;
		rects[ i ].right	= scisscorRects[ i ].x + scisscorRects[ i ].w;
		rects[ i ].bottom	= scisscorRects[ i ].y + scisscorRects[ i ].h;
	}
	_d3dContext->RSSetScissorRects( scissorsNum, rects );
	memory::SafeDeleteArray( rects );
}
//! @}

//===========================================================================
//!	@name		OM: 出力マージャーステージ.
//===========================================================================
//! @{
//---------------------------------------------------------------------------
//!	@brief		出力先レンダーターゲット設定.
//---------------------------------------------------------------------------
void DeferredContext::OMSetRenderTargets( u32 rtvNum, TextureView* const* rtvs, TextureView* dsv )
{
	BEGIN_ERROR_CHECK();

	AROMA_ASSERT( rtvNum >= 1, _T( "The number of render targets to be set must be 1 or more." ) );
	AROMA_ASSERT( rtvs, _T( "Be sure to specify the render target list." ) );

	// レンダーターゲットビュー.
	ID3D11RenderTargetView** d3dRTVs = new ID3D11RenderTargetView*[ rtvNum ];
	bool existRTV = false;
	for( u32 i = 0; i < rtvNum; ++i )
	{
		auto d3dRTV = rtvs[ i ]->GetNativeRenderTargetView();
		if( d3dRTV )
		{
			d3dRTVs[ i ] = d3dRTV;
			existRTV = true;
		}
	}
	AROMA_ASSERT( existRTV, _T( "Need at least one render target to set." ) );

	// 深度ステンシルビュー.
	ID3D11DepthStencilView* d3dDSV = nullptr;
	if( dsv ) d3dDSV = dsv->GetNativeDepthStencilView();

	// 設定.
	_d3dContext->OMSetRenderTargets( rtvNum, d3dRTVs, d3dDSV );
	memory::SafeDeleteArray( d3dRTVs );
}
//! @}

//! @}

//---------------------------------------------------------------------------
//!	@brief		ネイティブAPI遅延コンテキストの取得.
//---------------------------------------------------------------------------
ID3D11DeviceContext* DeferredContext::GetNativeContext() const
{
	return _d3dContext;
}

//---------------------------------------------------------------------------
//!	@brief		描画パイプラインを構築.
//---------------------------------------------------------------------------
void DeferredContext::SyncDrawPipeline()
{
	auto renderStateCache = _device->GetRenderStateCache();

	// ブレンドステート.
	if( _pipelineDirtyBits[ kPipelineDirtyBitFlagBlendState ] )
	{
		_pipelineDirtyBits[ kPipelineDirtyBitFlagBlendState ] = false;

		BlendStateKey key( _blendState );
		auto	d3dBlendState = renderStateCache->GetNativeBlendState( key );
		f32		blendFactor[ 4 ] = {};
		_d3dContext->OMSetBlendState( d3dBlendState, blendFactor, 0xffffffff );
	}

	// ラスタライザーステート.
	if( _pipelineDirtyBits[ kPipelineDirtyBitFlagRasterizerState ] )
	{
		_pipelineDirtyBits[ kPipelineDirtyBitFlagRasterizerState ] = false;

		RasterizerStateKey key( _rasterizerState );
		auto	d3dRasterizerState = renderStateCache->GetNativeRasterizerState( key );
		_d3dContext->RSSetState( d3dRasterizerState );
	}

	// 深度ステンシルステート.
	if( _pipelineDirtyBits[ kPipelineDirtyBitFlagDepthStencilState ] )
	{
		_pipelineDirtyBits[ kPipelineDirtyBitFlagDepthStencilState ] = false;

		DepthStencilStateKey key( _depthStencilState );
		auto	d3dDepthStencilState = renderStateCache->GetNativeDepthStencilState( key );
		// TODO: 0は仮, 参照ステンシル値を設定.
		_d3dContext->OMSetDepthStencilState( d3dDepthStencilState, 0 );
	}

	// PSステージ : サンプラーステート.
	for( u32 i = 0; i < kSamplerSlotMax; i++ )
	{
		u32 flagIdx = kPipelineDirtyBitFlagPSSamplerState + i;

		if( _pipelineDirtyBits[ flagIdx ] )
		{
			_pipelineDirtyBits[ flagIdx ] = false;

			SamplerStateKey key( _psSamplerStates[ i ] );
			auto	d3dSamplerState = renderStateCache->GetNativeSamplerState( key );
			_d3dContext->PSSetSamplers( i, 1, &d3dSamplerState );
		}
	}
}

} // namespace render
} // namespace aroma

#endif
