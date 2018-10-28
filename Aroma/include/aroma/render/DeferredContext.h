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
	//!	@name		VS: 頂点シェーダーステージ.
	//=======================================================================
	//! @{
	//-----------------------------------------------------------------------
	//!	@brief		頂点シェーダー設定.
	//-----------------------------------------------------------------------
	void VSSetShader( const Shader* vs );
	//! @}

	//=======================================================================
	//!	@name		PS: ピクセルシェーダーステージ.
	//=======================================================================
	//! @{
	//-----------------------------------------------------------------------
	//!	@brief		ピクセルシェーダー設定.
	//-----------------------------------------------------------------------
	void PSSetShader( const Shader* ps );

	//-----------------------------------------------------------------------
	//!	@brief		サンプラーステート設定.
	//-----------------------------------------------------------------------
	template< class T >
	inline void PSSetSamplerState( u32 slot, SamplerState::Setting state, T value );

	//-----------------------------------------------------------------------
	//!	@brief		シェーダーリソース設定.
	//-----------------------------------------------------------------------
	void PSSetShaderResource( u32 slot, const TextureView* srv );

	//-----------------------------------------------------------------------
	//!	@brief		定数バッファ設定.
	//-----------------------------------------------------------------------
	void PSSetConstantBuffer( u32 slot, const Buffer* cb );
	//! @}

	//=======================================================================
	//!	@name		IA: 入力アセンブラーステージ.
	//=======================================================================
	//! @{
	//-----------------------------------------------------------------------
	//!	@brief		入力レイアウト設定.
	//-----------------------------------------------------------------------
	void IASetInputLayout( const InputLayout* inputLayout );

	//-----------------------------------------------------------------------
	//!	@brief		プリミティブタイプ設定.
	//-----------------------------------------------------------------------
	void IASetPrimitiveType( const PrimitiveType primitiveType );

	//-----------------------------------------------------------------------
	//!	@brief		頂点バッファ設定.
	//-----------------------------------------------------------------------
	void IASetVertexBuffers( u32 startStreamIdx, u32 bufferNum, Buffer* const* buffers, const u32* strides, const u32* offsets );

	//-----------------------------------------------------------------------
	//!	@brief		インデックスバッファ設定.
	//-----------------------------------------------------------------------
	void IASetIndexBuffer( const Buffer* indexBuffer, u32 offset );

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
	//! @}
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
		kPipelineDirtyBitFlagBlendState,			//!< ブレンドステート.
		kPipelineDirtyBitFlagRasterizerState,		//!< ラスタライザーステート.
		kPipelineDirtyBitFlagDepthStencilState,		//!< 深度ステンシルステート.
		kPipelineDirtyBitFlagPSSamplerState,		//!< PSステージ : サンプラーステート.
		kPipelineDirtyBitFlagPSSamplerStateEnd = kPipelineDirtyBitFlagPSSamplerState + kSamplerSlotMax - 1,
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
	BlendState				_blendState;
	RasterizerState			_rasterizerState;
	DepthStencilState		_depthStencilState;
	SamplerState			_psSamplerStates[ kSamplerSlotMax ];

#ifdef AROMA_RENDER_DX11
	ID3D11DeviceContext*	_d3dContext;
#endif

	//! @}
};

//---------------------------------------------------------------------------
// Inline Functions
//---------------------------------------------------------------------------
template< class T >
void DeferredContext::OMSetBlendState( BlendState::Setting state, T value )
{
	if( _blendState.Set( state, value ) ) _pipelineDirtyBits[ kPipelineDirtyBitFlagBlendState ] = true;
}

template< class T >
void DeferredContext::RSSetRasterizerState( RasterizerState::Setting state, T value )
{
	if( _rasterizerState.Set( state, value ) ) _pipelineDirtyBits[ kPipelineDirtyBitFlagRasterizerState ] = true;
}

template< class T >
void DeferredContext::OMSetDepthStencilState( DepthStencilState::Setting state, T value )
{
	if( _depthStencilState.Set( state, value ) ) _pipelineDirtyBits[ kPipelineDirtyBitFlagDepthStencilState ] = true;
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
