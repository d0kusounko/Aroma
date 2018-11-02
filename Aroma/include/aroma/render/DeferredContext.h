//===========================================================================
//!
//!	@file		DeferredContext.h
//!	@brief		遅延コンテキスト.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include <bitset>
#include "RenderDef.h"
#include "MemoryAllocator.h"
#include "BlendState.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"
#include "SamplerState.h"
#include "../common/RefObject.h"
#include "../util/NonCopyable.h"
#include "../data/DataDef.h"
#include "../data/Color.h"

namespace aroma {
namespace render {

class Device;
class Texture;
class TextureView;
class CommandList;
class Shader;
class InputLayout;
class Buffer;

//---------------------------------------------------------------------------
//!	@brief		遅延コンテキスト.
//---------------------------------------------------------------------------
class DeferredContext final: public RefObject, public MemoryAllocator, private util::NonCopyable< DeferredContext >
{
public:
	//-----------------------------------------------------------------------
	//! @brief		構成設定.
	//-----------------------------------------------------------------------
	struct Desc
	{
		//-------------------------------------------------------------------
		Desc(){ Default(); }
		void Default()
		{
		}
	};

public:
	//-----------------------------------------------------------------------
	//!	@brief		コンストラクタ.
	//-----------------------------------------------------------------------
	DeferredContext();

	//-----------------------------------------------------------------------
	//!	@brief		デストラクタ.
	//-----------------------------------------------------------------------
	virtual ~DeferredContext();

	//-----------------------------------------------------------------------
	//!	@brief		初期化.
	//-----------------------------------------------------------------------
	void Initialize( Device* device, const Desc& desc );

	//-----------------------------------------------------------------------
	//!	@brief		解放.
	//-----------------------------------------------------------------------
	void Finalize();

	//-----------------------------------------------------------------------
	//!	@brief		コマンド記録開始.
	//-----------------------------------------------------------------------
	void Begin();

	//-----------------------------------------------------------------------
	//!	@brief		コマンド記録終了.
	//!
	//! @note		コマンドリストのインスタンスを生成して返却します.
	//-----------------------------------------------------------------------
	void End( CommandList** outCommandList );

	//=======================================================================
	//!	@name		コマンド群.
	//=======================================================================
	//! @{
	//-----------------------------------------------------------------------
	//!	@brief		レンダーターゲットを指定カラーでクリア.
	//-----------------------------------------------------------------------
	void ClearRenderTarget( TextureView* rtv, const data::Color& color );

	//-----------------------------------------------------------------------
	//! @brief		描画.
	//!	@param[in]	vertexNum			描画する頂点数.
	//!	@param[in]	startVertexIndex	先頭の頂点番号.
	//-----------------------------------------------------------------------
	void Draw( u32 vertexNum, u32 startVertexIndex );

	//-----------------------------------------------------------------------
	//! @brief		インデックス付き描画.
	//!	@param[in]	indexNum			描画するインデックス数.
	//!	@param[in]	startIndex			先頭の頂点インデックス番号.
	//!	@param[in]	baseVertexIndex		頂点バッファから頂点を読み取る前に各インデックスに加算する値.
	//-----------------------------------------------------------------------
	void DrawIndexed( u32 indexNum, u32 startIndex, u32 baseVertexIndex = 0 );

	//=======================================================================
	//!	@name		IA: 入力アセンブラーステージ.
	//=======================================================================
	//! @{
	//-----------------------------------------------------------------------
	//!	@brief		入力レイアウト設定.
	//-----------------------------------------------------------------------
	void IASetInputLayout( InputLayout* inputLayout );

	//-----------------------------------------------------------------------
	//!	@brief		プリミティブタイプ設定.
	//-----------------------------------------------------------------------
	void IASetPrimitiveType( PrimitiveType primitiveType );

	//-----------------------------------------------------------------------
	//!	@brief		頂点バッファ設定.
	//-----------------------------------------------------------------------
	void IASetVertexBuffer( u32 slot, Buffer* vb, u32 stride, u32 offset );

	//-----------------------------------------------------------------------
	//!	@brief		インデックスバッファ設定.
	//-----------------------------------------------------------------------
	void IASetIndexBuffer( Buffer* indexBuffer, u32 offset );
	//! @}

	//=======================================================================
	//!	@name		VS: 頂点シェーダーステージ.
	//=======================================================================
	//! @{
	//-----------------------------------------------------------------------
	//!	@brief		頂点シェーダー設定.
	//-----------------------------------------------------------------------
	void VSSetShader( Shader* vs );

	//-----------------------------------------------------------------------
	//!	@brief		シェーダーリソース設定.
	//-----------------------------------------------------------------------
	void VSSetShaderResource( u32 slot, TextureView* srv );

	//-----------------------------------------------------------------------
	//!	@brief		サンプラーステート設定.
	//-----------------------------------------------------------------------
	template< class T >
	inline void VSSetSamplerState( u32 slot, SamplerState::Setting state, T value );

	//-----------------------------------------------------------------------
	//!	@brief		定数バッファ設定.
	//-----------------------------------------------------------------------
	void VSSetConstantBuffer( u32 slot, Buffer* cb );
	//! @}

	//=======================================================================
	//!	@name		PS: ピクセルシェーダーステージ.
	//=======================================================================
	//! @{
	//-----------------------------------------------------------------------
	//!	@brief		ピクセルシェーダー設定.
	//-----------------------------------------------------------------------
	void PSSetShader( Shader* ps );

	//-----------------------------------------------------------------------
	//!	@brief		シェーダーリソース設定.
	//-----------------------------------------------------------------------
	void PSSetShaderResource( u32 slot, TextureView* srv );

	//-----------------------------------------------------------------------
	//!	@brief		サンプラーステート設定.
	//-----------------------------------------------------------------------
	template< class T >
	inline void PSSetSamplerState( u32 slot, SamplerState::Setting state, T value );

	//-----------------------------------------------------------------------
	//!	@brief		定数バッファ設定.
	//-----------------------------------------------------------------------
	void PSSetConstantBuffer( u32 slot, Buffer* cb );
	//! @}

	//=======================================================================
	//!	@name		RS: ラスタライザーステージ.
	//=======================================================================
	//! @{
	//-----------------------------------------------------------------------
	//!	@brief		ビューポート設定.
	//-----------------------------------------------------------------------
	void RSSetViewports( u32 viewportNum, const Viewport* viewports );

	//-----------------------------------------------------------------------
	//!	@brief		シザー設定.
	//-----------------------------------------------------------------------
	void RSSetScissors( u32 scissorsNum, const ScissorRect* scisscorRects );

	//-----------------------------------------------------------------------
	//!	@brief		ラスタライザーステート設定.
	//-----------------------------------------------------------------------
	template< class T >
	inline void RSSetRasterizerState( RasterizerState::Setting state, T value );
	//! @}

	//=======================================================================
	//!	@name		OM: 出力マージャーステージ.
	//=======================================================================
	//! @{
	//-----------------------------------------------------------------------
	//!	@brief		出力先レンダーターゲット設定.
	//-----------------------------------------------------------------------
	void OMSetRenderTargets( u32 rtvNum, TextureView* const* rtvs, TextureView* dsv );
	
	//-----------------------------------------------------------------------
	//!	@brief		設定済み出力先レンダーターゲット取得.
	//-----------------------------------------------------------------------
	void OMGetRenderTargets( u32 count, TextureView** outRTVs ) const;

	//-----------------------------------------------------------------------
	//!	@brief		設定済み深度ステンシルターゲット取得.
	//-----------------------------------------------------------------------
	TextureView* OMGetDepthStencilTarget() const;

	//-----------------------------------------------------------------------
	//!	@brief		ブレンドステート設定.
	//-----------------------------------------------------------------------
	template< class T >
	inline void OMSetBlendState( BlendState::Setting state, T value );

	//-----------------------------------------------------------------------
	//!	@brief		深度ステンシルステート設定.
	//-----------------------------------------------------------------------
	template< class T >
	inline void OMSetDepthStencilState( DepthStencilState::Setting state, T value );
	//! @}

	//! @}

	//-----------------------------------------------------------------------
	//!	@brief		ネイティブAPI遅延コンテキストの取得.
	//-----------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
	ID3D11DeviceContext* GetNativeContext() const;
#endif

private:
	//-----------------------------------------------------------------------
	//! @brief		描画パイプライン構築ダーティフラグ.
	//-----------------------------------------------------------------------
	enum PipelineDirtyBitFlag
	{
		kPipelineDirtyBitFlagIAInputLayout,			//!< IAステージ : 入力レイアウト.
		kPipelineDirtyBitFlagIAPrimitiveType,		//!< IAステージ : プリミティブタイプ.
		kPipelineDirtyBitFlagIAVertexBuffer,		//!< IAステージ : 頂点バッファ.
		kPipelineDirtyBitFlagIAVertexBufferEnd = kPipelineDirtyBitFlagIAVertexBuffer + kInputStreamsMax - 1,
		kPipelineDirtyBitFlagIAIndexBuffer,			//!< IAステージ : インデックスバッファ.

		kPipelineDirtyBitFlagVSShader,				//!< VSステージ : シェーダー.
		kPipelineDirtyBitFlagVSShaderResource,		//!< VSステージ : シェーダーリソース.
		kPipelineDirtyBitFlagVSShaderResourceEnd = kPipelineDirtyBitFlagVSShaderResource + kShaderResourceSlotMax - 1,
		kPipelineDirtyBitFlagVSSamplerState,		//!< VSステージ : サンプラーステート.
		kPipelineDirtyBitFlagVSSamplerStateEnd = kPipelineDirtyBitFlagVSSamplerState + kSamplerSlotMax - 1,
		kPipelineDirtyBitFlagVSConstantBuffer,		//!< VSステージ : 定数バッファ.
		kPipelineDirtyBitFlagVSConstantBufferEnd = kPipelineDirtyBitFlagVSConstantBuffer + kShaderUniformBufferSlotMax - 1,

		kPipelineDirtyBitFlagPSShader,				//!< PSステージ : シェーダー.
		kPipelineDirtyBitFlagPSShaderResource,		//!< PSステージ : シェーダーリソース.
		kPipelineDirtyBitFlagPSShaderResourceEnd = kPipelineDirtyBitFlagPSShaderResource + kShaderResourceSlotMax - 1,
		kPipelineDirtyBitFlagPSSamplerState,		//!< PSステージ : サンプラーステート.
		kPipelineDirtyBitFlagPSSamplerStateEnd = kPipelineDirtyBitFlagPSSamplerState + kSamplerSlotMax - 1,
		kPipelineDirtyBitFlagPSConstantBuffer,		//!< PSステージ : 定数バッファ.
		kPipelineDirtyBitFlagPSConstantBufferEnd = kPipelineDirtyBitFlagPSConstantBuffer + kShaderUniformBufferSlotMax - 1,

		kPipelineDirtyBitFlagRSRasterizerState,		//!< RSステージ: ラスタライザーステート.
		kPipelineDirtyBitFlagRSViewportScissorState,//!< RSステージ: ビューポートシザーステート.

		kPipelineDirtyBitFlagOMRenderTarget,		//!< OMステージ: レンダーターゲット.
		kPipelineDirtyBitFlagOMBlendState,			//!< OMステージ: ブレンドステート.
		kPipelineDirtyBitFlagOMDepthStencilState,	//!< OMステージ: 深度ステンシルステート.

		kPipelineDirtyBitFlagNum,
	};

	using PiplineDirtyBits = std::bitset< kPipelineDirtyBitFlagNum >;

	//-----------------------------------------------------------------------
	//!	@brief		描画パイプラインを構築.
	//-----------------------------------------------------------------------
	void SyncDrawPipeline();

	//-----------------------------------------------------------------------
	//!	@name		メンバ変数.
	//-----------------------------------------------------------------------
	//! @{
	bool					_initialized;
	Device*					_device;
	Desc					_desc;
	bool					_begin;
	PiplineDirtyBits		_pipelineDirtyBits;

	// IAステージ.
	Buffer*					_vertexBuffers[ kInputStreamsMax ];
	u32						_vertexBufferStrides[ kInputStreamsMax ];
	u32						_vertexBufferOffsets[ kInputStreamsMax ];
	Buffer*					_indexBuffer;
	u32						_indexBufferOffset;
	PrimitiveType			_primitiveType;
	InputLayout*			_inputLayout;

	// VSステージ.
	Shader*					_vsShader;
	TextureView* 			_vsShaderResources[ kShaderResourceSlotMax ];
	Buffer*					_vsConstantBuffers[ kShaderUniformBufferSlotMax ];
	SamplerState			_vsSamplerStates[ kSamplerSlotMax ];

	// PSステージ.
	Shader*					_psShader;
	TextureView* 			_psShaderResources[ kShaderResourceSlotMax ];
	Buffer*					_psConstantBuffers[ kShaderUniformBufferSlotMax ];
	SamplerState			_psSamplerStates[ kSamplerSlotMax ];

	// RSステージ.
	RasterizerState			_rasterizerState;
	// ViewportScissorState	_viewportScissorState;

	// OMステージ.
	TextureView*			_renderTargets[ kRenderTargetsSlotMax ];
	TextureView*			_depthStencil;
	BlendState				_blendState;
	DepthStencilState		_depthStencilState;

#ifdef AROMA_RENDER_DX11
	ID3D11DeviceContext*	_d3dContext;
#endif

	//! @}
};

//---------------------------------------------------------------------------
// Inline Functions
//---------------------------------------------------------------------------
template< class T >
void DeferredContext::RSSetRasterizerState( RasterizerState::Setting state, T value )
{
	if( _rasterizerState.Set( state, value ) ) _pipelineDirtyBits[ kPipelineDirtyBitFlagRSRasterizerState ] = true;
}

template< class T >
void DeferredContext::OMSetBlendState( BlendState::Setting state, T value )
{
	if( _blendState.Set( state, value ) ) _pipelineDirtyBits[ kPipelineDirtyBitFlagOMBlendState ] = true;
}

template< class T >
void DeferredContext::OMSetDepthStencilState( DepthStencilState::Setting state, T value )
{
	if( _depthStencilState.Set( state, value ) ) _pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}

template< class T >
void DeferredContext::VSSetSamplerState( u32 slot, SamplerState::Setting state, T value )
{
	if( slot >= kSamplerSlotMax )
	{
		AROMA_ASSERT( false, _T( "Slot is out of range.\n" ) );
		return;
	}
	if( _vsSamplerStates[ slot ].Set( state, value ) ) _pipelineDirtyBits[ kPipelineDirtyBitFlagVSSamplerState + slot ] = true;
}

template< class T >
void DeferredContext::PSSetSamplerState( u32 slot, SamplerState::Setting state, T value )
{
	if( slot >= kSamplerSlotMax )
	{
		AROMA_ASSERT( false, _T( "Slot is out of range.\n" ) );
		return;
	}
	if( _psSamplerStates[ slot ].Set( state, value ) ) _pipelineDirtyBits[ kPipelineDirtyBitFlagPSSamplerState + slot ] = true;
}

} // namespace render
} // namespace aroma
