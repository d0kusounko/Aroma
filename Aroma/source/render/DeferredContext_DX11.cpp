//===========================================================================
//!
//!	@file		DeferredContext_DX11.cpp
//! @brief		遅延コンテキスト : DirectX11.
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
//	コンストラクタ.
//---------------------------------------------------------------------------
DeferredContext::DeferredContext()
	: _initialized( false )
	, _device( nullptr )
	, _d3dContext( nullptr )
	, _begin( false )
	, _indexBuffer( nullptr )
	, _indexBufferOffset( 0 )
	, _primitiveType( PrimitiveType::kUndefined )
	, _inputLayout( nullptr )
	, _vsShader( nullptr )
	, _psShader( nullptr )
	, _depthStencil( nullptr )
{
	memory::Clear( _vertexBuffers );
	memory::Clear( _vertexBufferStrides );
	memory::Clear( _vertexBufferOffsets );
	memory::Clear( _vsShaderResources );
	memory::Clear( _vsConstantBuffers );
	memory::Clear( _psShaderResources );
	memory::Clear( _psConstantBuffers );
	memory::Clear( _renderTargets );
}

//---------------------------------------------------------------------------
//	デストラクタ.
//---------------------------------------------------------------------------
DeferredContext::~DeferredContext()
{
	Finalize();
}

//---------------------------------------------------------------------------
//	初期化.
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
//	解放.
//---------------------------------------------------------------------------
void DeferredContext::Finalize()
{
	if( !_initialized ) return;

	// IAステージ.
	for( auto& vtxBuf : _vertexBuffers )
	{
		memory::SafeRelease( vtxBuf );
	}
	memory::SafeRelease( _indexBuffer );
	memory::SafeRelease( _inputLayout );

	// VSステージ.
	memory::SafeRelease( _vsShader );
	for( auto& srv : _vsShaderResources )
	{
		memory::SafeRelease( srv );
	}
	for( auto& cb : _vsConstantBuffers )
	{
		memory::SafeRelease( cb );
	}

	// PSステージ.
	memory::SafeRelease( _psShader );
	for( auto& srv : _psShaderResources )
	{
		memory::SafeRelease( srv );
	}
	for( auto& cb : _psConstantBuffers )
	{
		memory::SafeRelease( cb );
	}

	// OMステージ.
	for( auto& rtv : _renderTargets )
	{
		memory::SafeRelease( rtv );
	}
	memory::SafeRelease( _depthStencil );

	// デバイス.
	memory::SafeRelease( _d3dContext );
	memory::SafeRelease( _device );
	_desc.Default();

	_initialized = false;
}

//---------------------------------------------------------------------------
//	コマンド記録開始.
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
//	コマンド記録終了.
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
//	レンダーターゲットを指定カラーでクリア.
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
//	IA: 入力アセンブラーステージ.
//===========================================================================
//---------------------------------------------------------------------------
//	入力レイアウト設定.
//---------------------------------------------------------------------------
void DeferredContext::IASetInputLayout( InputLayout* inputLayout )
{
	if( _inputLayout != inputLayout )
	{
		memory::SafeRelease( _inputLayout );
		_inputLayout = inputLayout;
		_inputLayout->AddRef();

		_pipelineDirtyBits[ kPipelineDirtyBitFlagIAInputLayout ] = true;
	}
}

//---------------------------------------------------------------------------
//	プリミティブタイプ設定.
//---------------------------------------------------------------------------
void DeferredContext::IASetPrimitiveType( PrimitiveType primitiveType )
{
	if( _primitiveType != primitiveType )
	{
		_primitiveType = primitiveType;
		_pipelineDirtyBits[ kPipelineDirtyBitFlagIAPrimitiveType ] = true;
	}
}

//---------------------------------------------------------------------------
//	頂点バッファ設定.
//---------------------------------------------------------------------------
void DeferredContext::IASetVertexBuffer( u32 slot, Buffer* vb, u32 stride, u32 offset )
{
	if( slot >= kInputStreamsMax )
	{
		AROMA_ASSERT( false, _T( "Slot is out of range.\n" ) );
		return;
	}

	// 頂点バッファ.
	if( _vertexBuffers[ slot ] != vb )
	{
		memory::SafeRelease( _vertexBuffers[ slot ] );
		_vertexBuffers[ slot ] = vb;
		_vertexBuffers[ slot ]->AddRef();

		_pipelineDirtyBits[ kPipelineDirtyBitFlagIAVertexBuffer + slot ] = true;
	}

	// ストライド.
	if( _vertexBufferStrides[ slot ] != stride )
	{
		_vertexBufferStrides[ slot ] = stride;
		_pipelineDirtyBits[ kPipelineDirtyBitFlagIAVertexBuffer + slot ] = true;
	}

	// オフセット.
	if( _vertexBufferOffsets[ slot ] != offset )
	{
		_vertexBufferOffsets[ slot ] = offset;
		_pipelineDirtyBits[ kPipelineDirtyBitFlagIAVertexBuffer + slot ] = true;
	}
}

//---------------------------------------------------------------------------
//	インデックスバッファ設定.
//---------------------------------------------------------------------------
void DeferredContext::IASetIndexBuffer( Buffer* indexBuffer, u32 offset )
{
	if( _indexBuffer != indexBuffer )
	{
		memory::SafeRelease( _indexBuffer );
		_indexBuffer = indexBuffer;
		_indexBuffer->AddRef();

		_pipelineDirtyBits[ kPipelineDirtyBitFlagIAIndexBuffer ] = true;
	}

	if( _indexBufferOffset != offset )
	{
		_indexBufferOffset = offset;
		_pipelineDirtyBits[ kPipelineDirtyBitFlagIAIndexBuffer ] = true;
	}
}


//===========================================================================
//	VS: 頂点シェーダーステージ.
//===========================================================================
//---------------------------------------------------------------------------
//	頂点シェーダー設定.
//---------------------------------------------------------------------------
void DeferredContext::VSSetShader( Shader* vs )
{
	if( _vsShader != vs )
	{
		memory::SafeRelease( _vsShader );
		_vsShader = vs;
		_vsShader->AddRef();

		_pipelineDirtyBits[ kPipelineDirtyBitFlagVSShader ] = true;
	}
}

//---------------------------------------------------------------------------
//	シェーダーリソース設定.
//---------------------------------------------------------------------------
void DeferredContext::VSSetShaderResource( u32 slot, TextureView* srv )
{
	if( slot >= kShaderResourceSlotMax )
	{
		AROMA_ASSERT( false, _T( "Slot is out of range.\n" ) );
		return;
	}

	auto& contextSRV = _vsShaderResources[ slot ];
	if( contextSRV != srv )
	{
		memory::SafeRelease( contextSRV );
		contextSRV = srv;
		contextSRV->AddRef();

		_pipelineDirtyBits[ kPipelineDirtyBitFlagVSShaderResource + slot ] = true;
	}
}

//---------------------------------------------------------------------------
//	定数バッファ設定.
//---------------------------------------------------------------------------
void DeferredContext::VSSetConstantBuffer( u32 slot, Buffer* cb )
{
	if( slot >= kShaderUniformBufferSlotMax )
	{
		AROMA_ASSERT( false, _T( "Slot is out of range.\n" ) );
		return;
	}

	auto& constantBuffer = _vsConstantBuffers[ slot ];
	if( constantBuffer != cb )
	{
		memory::SafeRelease( constantBuffer );
		constantBuffer = cb;
		constantBuffer->AddRef();

		_pipelineDirtyBits[ kPipelineDirtyBitFlagVSConstantBuffer + slot ] = true;
	}
}

//=======================================================================
//	PS: ピクセルシェーダーステージ.
//=======================================================================
//-----------------------------------------------------------------------
//	ピクセルシェーダー設定.
//-----------------------------------------------------------------------
void DeferredContext::PSSetShader( Shader* ps )
{
	if( _psShader != ps )
	{
		memory::SafeRelease( _psShader );
		_psShader = ps;
		_psShader->AddRef();

		_pipelineDirtyBits[ kPipelineDirtyBitFlagPSShader ] = true;
	}
}

//-----------------------------------------------------------------------
//	シェーダーリソース設定.
//-----------------------------------------------------------------------
void DeferredContext::PSSetShaderResource( u32 slot, TextureView* srv )
{
	if( slot >= kShaderResourceSlotMax )
	{
		AROMA_ASSERT( false, _T( "Slot is out of range.\n" ) );
		return;
	}

	auto& contextSRV = _psShaderResources[ slot ];
	if( contextSRV != srv )
	{
		memory::SafeRelease( contextSRV );
		contextSRV = srv;
		contextSRV->AddRef();

		_pipelineDirtyBits[ kPipelineDirtyBitFlagPSShaderResource + slot ] = true;
	}
}

//---------------------------------------------------------------------------
//	定数バッファ設定.
//---------------------------------------------------------------------------
void DeferredContext::PSSetConstantBuffer( u32 slot, Buffer* cb )
{
	if( slot >= kShaderUniformBufferSlotMax )
	{
		AROMA_ASSERT( false, _T( "Slot is out of range.\n" ) );
		return;
	}

	auto& constantBuffer = _psConstantBuffers[ slot ];
	if( constantBuffer != cb )
	{
		memory::SafeRelease( constantBuffer );
		constantBuffer = cb;
		constantBuffer->AddRef();

		_pipelineDirtyBits[ kPipelineDirtyBitFlagPSConstantBuffer + slot ] = true;
	}
}

//===========================================================================
//	OM: 出力マージャーステージ.
//===========================================================================
//---------------------------------------------------------------------------
//	出力先レンダーターゲット設定.
//---------------------------------------------------------------------------
void DeferredContext::OMSetRenderTargets( u32 rtvNum, TextureView* const* rtvs, TextureView* dsv )
{
	AROMA_ASSERT( rtvNum <= kRenderTargetsSlotMax, _T( "rtvNum is out of range.\n" ) );
	AROMA_ASSERT( rtvNum >= 1, _T( "The number of render targets to be set must be 1 or more." ) );
	AROMA_ASSERT( rtvs, _T( "Be sure to specify the render target list." ) );

	for( u32 i = 0; i < rtvNum; ++i )
	{
		auto& rtv = _renderTargets[ i ];
		if( rtv != rtvs[ i ] )
		{
			memory::SafeRelease( rtv );
			rtv = rtvs[ i ];
			if( rtv ) rtv->AddRef();

			_pipelineDirtyBits[ kPipelineDirtyBitFlagOMRenderTarget ] = true;
		}
	}

	if( _depthStencil != dsv )
	{
		memory::SafeRelease( _depthStencil );
		_depthStencil = dsv;
		if( _depthStencil ) _depthStencil->AddRef();

		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMRenderTarget ] = true;
	}
}

//---------------------------------------------------------------------------
//	設定済み出力先レンダーターゲット取得.
//---------------------------------------------------------------------------
void DeferredContext::OMGetRenderTargets( u32 count, TextureView** outRTVs ) const
{
	if( count > kRenderTargetsSlotMax )
	{
		AROMA_ASSERT( false, _T( "count is out of range.\n" ) );
		count = kRenderTargetsSlotMax;
	}
	for( u32 i = 0; i < count; ++i )
	{
		outRTVs[ i ] = _renderTargets[ i ];
	}
}

//---------------------------------------------------------------------------
//	設定済み深度ステンシルターゲット取得.
//---------------------------------------------------------------------------
TextureView* DeferredContext::OMGetDepthStencilTarget() const
{
	return _depthStencil;
}

//===========================================================================
//!	@name		RS: ラスタライザーステージ.
//===========================================================================
//! @{
//---------------------------------------------------------------------------
//	ビューポート設定.
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
//	シザー設定.
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

//! @}

//---------------------------------------------------------------------------
//	ネイティブAPI遅延コンテキストの取得.
//---------------------------------------------------------------------------
ID3D11DeviceContext* DeferredContext::GetNativeContext() const
{
	return _d3dContext;
}

//---------------------------------------------------------------------------
//	描画パイプラインを構築.
//---------------------------------------------------------------------------
void DeferredContext::SyncDrawPipeline()
{
#ifdef AROMA_DEBUG
	//-----------------------------------------------------------------------
	// 警告表示.
	//-----------------------------------------------------------------------
	// IAステージ.
	for( u32 i = 0; i < _inputLayout->GetStreamCount(); ++i )
	{
		if( _vertexBuffers[ i ] == nullptr )
		{
			// 要求されている頂点ストリームインデックスに頂点バッファが設定されていません.
			// 動作が不定になる可能性があるので要求された頂点ストリーム数を満たすバッファを設定して下さい.
			AROMA_ASSERT( false, _T( "[Warning] Vertex stream index %d has no vertex buffer set.\n" ), i );
		}
	}
#endif

	auto renderStateCache = _device->GetRenderStateCache();

	//-----------------------------------------------------------------------
	// IAステージ.
	//-----------------------------------------------------------------------
	// 入力レイアウト.
	if( _pipelineDirtyBits[ kPipelineDirtyBitFlagIAInputLayout ] )
	{
		_pipelineDirtyBits[ kPipelineDirtyBitFlagIAInputLayout ] = false;
		_d3dContext->IASetInputLayout( _inputLayout->GetNativeInputLayout() );
	}

	// プリミティブタイプ.
	if( _pipelineDirtyBits[ kPipelineDirtyBitFlagIAPrimitiveType ] )
	{
		_pipelineDirtyBits[ kPipelineDirtyBitFlagIAPrimitiveType ] = false;
		_d3dContext->IASetPrimitiveTopology( ToNativePrimitiveType( _primitiveType ) );
	}

	// 頂点バッファ.
	for( u32 i = 0; i < kInputStreamsMax; i++ )
	{
		u32 flagIdx	= kPipelineDirtyBitFlagIAVertexBuffer + i;
		if( _pipelineDirtyBits[ flagIdx ] )
		{
			_pipelineDirtyBits[ flagIdx ] = false;
			auto& vertexBuffer = _vertexBuffers[ i ];
			if( vertexBuffer )
			{
				ID3D11Buffer*	d3dBuffers[] = { vertexBuffer->GetNativeBuffer() };
				u32				strides[] = { _vertexBufferStrides[ i ] };
				u32				offset[] = { _vertexBufferOffsets[ i ] };
				_d3dContext->IASetVertexBuffers( i, 1, d3dBuffers, strides, offset );
			}
		}
	}

	// インデックスバッファ.
	if( _pipelineDirtyBits[ kPipelineDirtyBitFlagIAIndexBuffer ] )
	{
		_pipelineDirtyBits[ kPipelineDirtyBitFlagIAIndexBuffer ] = false;

		if( _indexBuffer )
		{
			DXGI_FORMAT d3dFortmat = DXGI_FORMAT_UNKNOWN;
			switch( GetIndexTypeFromBufferStride( _indexBuffer->GetDesc().stride ) )
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

			_d3dContext->IASetIndexBuffer( _indexBuffer->GetNativeBuffer(), d3dFortmat, _indexBufferOffset );
		}
	}

	//-----------------------------------------------------------------------
	// VSステージ.
	//-----------------------------------------------------------------------
	// ピクセルシェーダー.
	if( _pipelineDirtyBits[ kPipelineDirtyBitFlagVSShader ] )
	{
		_pipelineDirtyBits[ kPipelineDirtyBitFlagVSShader ] = false;
		_d3dContext->VSSetShader( _vsShader->GetNativeVertexShader(), nullptr, 0 );
	}

	// シェーダーリソース.
	for( u32 i = 0; i < kShaderResourceSlotMax; i++ )
	{
		u32 flagIdx = kPipelineDirtyBitFlagVSShaderResource + i;

		if( _pipelineDirtyBits[ flagIdx ] )
		{
			_pipelineDirtyBits[ flagIdx ] = false;

			if( !_vsShaderResources[ i ] ) continue;

			ID3D11ShaderResourceView* const srvs[] = { _vsShaderResources[ i ]->GetNativeShaderResourceView() };
			_d3dContext->VSSetShaderResources( i, 1, srvs );
		}
	}

	// サンプラーステート.
	for( u32 i = 0; i < kSamplerSlotMax; i++ )
	{
		u32 flagIdx = kPipelineDirtyBitFlagVSSamplerState + i;

		if( _pipelineDirtyBits[ flagIdx ] )
		{
			_pipelineDirtyBits[ flagIdx ] = false;

			SamplerStateKey key( _vsSamplerStates[ i ] );
			auto	d3dSamplerState = renderStateCache->GetNativeSamplerState( key );
			_d3dContext->VSSetSamplers( i, 1, &d3dSamplerState );
		}
	}

	// 定数バッファ.
	for( u32 i = 0; i < kShaderUniformBufferSlotMax; i++ )
	{
		u32 flagIdx	= kPipelineDirtyBitFlagVSConstantBuffer + i;
		if( _pipelineDirtyBits[ flagIdx ] )
		{
			_pipelineDirtyBits[ flagIdx ] = false;

			if( !_vsConstantBuffers[ i ] ) continue;

			ID3D11Buffer* const cbs[] = { _vsConstantBuffers[ i ]->GetNativeBuffer() };
			_d3dContext->VSSetConstantBuffers( i, 1, cbs );
		}
	}

	//-----------------------------------------------------------------------
	// PSステージ.
	//-----------------------------------------------------------------------
	// ピクセルシェーダー.
	if( _pipelineDirtyBits[ kPipelineDirtyBitFlagPSShader ] )
	{
		_pipelineDirtyBits[ kPipelineDirtyBitFlagPSShader ] = false;
		_d3dContext->PSSetShader( _psShader->GetNativePixelShader(), nullptr, 0 );
	}

	// シェーダーリソース.
	for( u32 i = 0; i < kShaderResourceSlotMax; i++ )
	{
		u32 flagIdx = kPipelineDirtyBitFlagPSShaderResource + i;

		if( _pipelineDirtyBits[ flagIdx ] )
		{
			_pipelineDirtyBits[ flagIdx ] = false;

			if( !_psShaderResources[ i ] ) continue;

			ID3D11ShaderResourceView* const srvs[] = { _psShaderResources[ i ]->GetNativeShaderResourceView() };
			_d3dContext->PSSetShaderResources( i, 1, srvs );
		}
	}

	// サンプラーステート.
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

	// 定数バッファ.
	for( u32 i = 0; i < kShaderUniformBufferSlotMax; i++ )
	{
		u32 flagIdx	= kPipelineDirtyBitFlagPSConstantBuffer + i;
		if( _pipelineDirtyBits[ flagIdx ] )
		{
			_pipelineDirtyBits[ flagIdx ] = false;

			if( !_psConstantBuffers[ i ] ) continue;

			ID3D11Buffer* const cbs[] = { _psConstantBuffers[ i ]->GetNativeBuffer() };
			_d3dContext->PSSetConstantBuffers( i, 1, cbs );
		}
	}

	//-----------------------------------------------------------------------
	// RSステージ.
	//-----------------------------------------------------------------------
	// ラスタライザーステート.
	if( _pipelineDirtyBits[ kPipelineDirtyBitFlagRSRasterizerState ] )
	{
		_pipelineDirtyBits[ kPipelineDirtyBitFlagRSRasterizerState ] = false;

		RasterizerStateKey key( _rasterizerState );
		auto	d3dRasterizerState = renderStateCache->GetNativeRasterizerState( key );
		_d3dContext->RSSetState( d3dRasterizerState );
	}

	//-----------------------------------------------------------------------
	// OMステージ.
	//-----------------------------------------------------------------------
	// レンダーターゲット.
	if( _pipelineDirtyBits[ kPipelineDirtyBitFlagOMRenderTarget ] )
	{
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMRenderTarget ] = false;

		ID3D11RenderTargetView* d3dRTVs[ kRenderTargetsSlotMax ] = {};
		ID3D11DepthStencilView*	d3dDSV = nullptr;

		for( u32 i = 0; i < kRenderTargetsSlotMax; ++i )
		{
			auto& rtv = _renderTargets[ i ];
			if( rtv )
			{
				d3dRTVs[ i ] = rtv->GetNativeRenderTargetView();
			}
		}

		if( _depthStencil )
		{
			d3dDSV = _depthStencil->GetNativeDepthStencilView();
		}

		_d3dContext->OMSetRenderTargets( kRenderTargetsSlotMax, d3dRTVs, d3dDSV );
	}

	// ブレンドステート.
	if( _pipelineDirtyBits[ kPipelineDirtyBitFlagOMBlendState ] )
	{
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMBlendState ] = false;

		BlendStateKey key( _blendState );
		auto	d3dBlendState = renderStateCache->GetNativeBlendState( key );
		f32		blendFactor[ 4 ] = {};
		_d3dContext->OMSetBlendState( d3dBlendState, blendFactor, 0xffffffff );
	}

	// 深度ステンシルステート.
	if( _pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] )
	{
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = false;

		DepthStencilStateKey key( _depthStencilState );
		auto	d3dDepthStencilState = renderStateCache->GetNativeDepthStencilState( key );
		// TODO: 0は仮, 参照ステンシル値を設定.
		_d3dContext->OMSetDepthStencilState( d3dDepthStencilState, 0 );
	}
}

} // namespace render
} // namespace aroma

#endif
