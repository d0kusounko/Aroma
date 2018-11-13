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
#include "ViewportScissorState.h"
#include "../common/RefObject.h"
#include "../util/NonCopyable.h"
#include "../data/DataDef.h"
#include "../data/Color.h"

namespace aroma {
namespace render {

class Device;
class Texture;
class TextureView;
class RenderTargetView;
class DepthStencilView;
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
	void ClearRenderTarget( RenderTargetView* rtv, const data::Color& color );

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
	inline void VSSetSamplerState( u32 slot, const SamplerState& value );
	inline void VSSetSamplerStateFilter( u32 slot, Filter value );
	inline void VSSetSamplerStateAddressU( u32 slot, TextureAddress value );
	inline void VSSetSamplerStateAddressV( u32 slot, TextureAddress value );
	inline void VSSetSamplerStateAddressW( u32 slot, TextureAddress value );
	inline void VSSetSamplerStateMipLODBias( u32 slot, f32 value );
	inline void VSSetSamplerStateMaxAnisotropy( u32 slot, AnisotropicRatio value );
	inline void VSSetSamplerStateBorderColor( u32 slot, const data::Color& value );
	inline void VSSetSamplerStateMinLOD( u32 slot, f32 value );
	inline void VSSetSamplerStateMaxLOD( u32 slot, f32 value );

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
	inline void PSSetSamplerState( u32 slot, const SamplerState& value );
	inline void PSSetSamplerStateFilter( u32 slot, Filter value );
	inline void PSSetSamplerStateAddressU( u32 slot, TextureAddress value );
	inline void PSSetSamplerStateAddressV( u32 slot, TextureAddress value );
	inline void PSSetSamplerStateAddressW( u32 slot, TextureAddress value );
	inline void PSSetSamplerStateMipLODBias( u32 slot, f32 value );
	inline void PSSetSamplerStateMaxAnisotropy( u32 slot, AnisotropicRatio value );
	inline void PSSetSamplerStateBorderColor( u32 slot, const data::Color& value );
	inline void PSSetSamplerStateMinLOD( u32 slot, f32 value );
	inline void PSSetSamplerStateMaxLOD( u32 slot, f32 value );

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
	//!	@brief		ビューポートシザーステート設定.
	//-----------------------------------------------------------------------
	inline void RSSetViewportScissorState( const ViewportScissorState& value );
	inline void RSSetViewportScissorStateViewport( u32 slot, const Viewport& value );
	inline void RSSetViewportScissorStateScissor( u32 slot, const ScissorRect& value );

	//-----------------------------------------------------------------------
	//!	@brief		ラスタライザーステート設定.
	//-----------------------------------------------------------------------
	inline void RSSetRasterizerState( const RasterizerState& value );
	inline void RSSetRasterizerStateFillMode( FillMode value );
	inline void RSSetRasterizerStateCullMode( CullMode value );
	inline void RSSetRasterizerStateFrontCounterClockwise( bool value );
	inline void RSSetRasterizerStateDepthBias( s32 value );
	inline void RSSetRasterizerStateDepthBiasClamp( f32 value );
	inline void RSSetRasterizerStateSlopeScaledDepthBias( f32 value );
	inline void RSSetRasterizerStateDepthClipEnable( bool value );
	inline void RSSetRasterizerStateScissorEnable( bool value );
	inline void RSSetRasterizerStateMultisampleEnable( bool value );
	inline void RSSetRasterizerStateAntialiasedLineEnable( bool value );
	//! @}

	//=======================================================================
	//!	@name		OM: 出力マージャーステージ.
	//=======================================================================
	//! @{
	//-----------------------------------------------------------------------
	//!	@brief		出力先レンダーターゲット設定.
	//-----------------------------------------------------------------------
	void OMSetRenderTargets( u32 rtvNum, RenderTargetView* const* rtvs, DepthStencilView* dsv );

	//-----------------------------------------------------------------------
	//!	@brief		設定済み出力先レンダーターゲット取得.
	//-----------------------------------------------------------------------
	void OMGetRenderTargets( u32 count, RenderTargetView** outRTVs ) const;

	//-----------------------------------------------------------------------
	//!	@brief		設定済み深度ステンシルターゲット取得.
	//-----------------------------------------------------------------------
	DepthStencilView* OMGetDepthStencilTarget() const;

	//-----------------------------------------------------------------------
	//!	@brief		ブレンドステート設定.
	//-----------------------------------------------------------------------
	inline void OMSetBlendState( const BlendState& state );
	inline void OMSetBlendStateSampleAlphaToCoverage( bool value );
	inline void OMSetBlendStateBlendEnable( bool value );
	inline void OMSetBlendStateRGBSource( Blend value );
	inline void OMSetBlendStateRGBDest( Blend value );
	inline void OMSetBlendStateRGBBlendOp( BlendOp value );
	inline void OMSetBlendStateAlphaSource( Blend value );
	inline void OMSetBlendStateAlphaDest( Blend value );
	inline void OMSetBlendStateAlphaBlendOp( BlendOp value );
	inline void OMSetBlendStateWriteMaskR( bool value );
	inline void OMSetBlendStateWriteMaskG( bool value );
	inline void OMSetBlendStateWriteMaskB( bool value );
	inline void OMSetBlendStateWriteMaskA( bool value );

	//-----------------------------------------------------------------------
	//!	@brief		深度ステンシルステート設定.
	//-----------------------------------------------------------------------
	inline void OMSetDepthStencilState( const DepthStencilState& value );
	inline void OMSetDepthStencilStateDepthEnable( bool value );
	inline void OMSetDepthStencilStateDepthWrite( bool value );
	inline void OMSetDepthStencilStateDepthFunc( ComparisonFunc value );
	inline void OMSetDepthStencilStateStencilEnable( bool value );
	inline void OMSetDepthStencilStateStencilReadMask( u8 value );
	inline void OMSetDepthStencilStateStencilWriteMask( u8 value );
	inline void OMSetDepthStencilStateFrontFaceStencilFailOp( StencilOp value );
	inline void OMSetDepthStencilStateFrontFaceStencilDepthFailOp( StencilOp value );
	inline void OMSetDepthStencilStateFrontFaceStencilPassOp( StencilOp value );
	inline void OMSetDepthStencilStateFrontFaceStencilFunc( ComparisonFunc value );
	inline void OMSetDepthStencilStateBackFaceStencilFailOp( StencilOp value );
	inline void OMSetDepthStencilStateBackFaceStencilDepthFailOp( StencilOp value );
	inline void OMSetDepthStencilStateBackFaceStencilPassOp( StencilOp value );
	inline void OMSetDepthStencilStateBackFaceStencilFunc( ComparisonFunc value );
	inline void OMSetDepthStencilStateStencilRef( u32 value );
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
	ViewportScissorState	_viewportScissorState;

	// OMステージ.
	RenderTargetView*		_renderTargets[ kRenderTargetsSlotMax ];
	DepthStencilView*		_depthStencil;
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
#ifndef __SAMPLER_SLOT_OUT_RANGE_CHECK
#define __SAMPLER_SLOT_OUT_RANGE_CHECK( _slot_ )				\
	if( _slot_ >= kViewportsSlotMax )							\
	{															\
		AROMA_ASSERT( false, _T( "Slot is out of range.\n" ) );	\
		return;													\
	}
#endif

void DeferredContext::VSSetSamplerState( u32 slot, const SamplerState& value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _vsSamplerStates[ slot ].Set( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagVSSamplerState + slot ] = true;
}
void DeferredContext::VSSetSamplerStateFilter( u32 slot, Filter value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _vsSamplerStates[ slot ].SetFilter( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagVSSamplerState + slot ] = true;
}
void DeferredContext::VSSetSamplerStateAddressU( u32 slot, TextureAddress value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _vsSamplerStates[ slot ].SetAddressU( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagVSSamplerState + slot ] = true;
}
void DeferredContext::VSSetSamplerStateAddressV( u32 slot, TextureAddress value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _vsSamplerStates[ slot ].SetAddressV( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagVSSamplerState + slot ] = true;
}
void DeferredContext::VSSetSamplerStateAddressW( u32 slot, TextureAddress value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _vsSamplerStates[ slot ].SetAddressW( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagVSSamplerState + slot ] = true;
}
void DeferredContext::VSSetSamplerStateMipLODBias( u32 slot, f32 value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _vsSamplerStates[ slot ].SetMipLODBias( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagVSSamplerState + slot ] = true;
}
void DeferredContext::VSSetSamplerStateMaxAnisotropy( u32 slot, AnisotropicRatio value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _vsSamplerStates[ slot ].SetMaxAnisotropy( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagVSSamplerState + slot ] = true;
}
void DeferredContext::VSSetSamplerStateBorderColor( u32 slot, const data::Color& value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _vsSamplerStates[ slot ].SetBorderColor( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagVSSamplerState + slot ] = true;
}
void DeferredContext::VSSetSamplerStateMinLOD( u32 slot, f32 value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _vsSamplerStates[ slot ].SetMinLOD( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagVSSamplerState + slot ] = true;
}
void DeferredContext::VSSetSamplerStateMaxLOD( u32 slot, f32 value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _vsSamplerStates[ slot ].SetMaxLOD( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagVSSamplerState + slot ] = true;
}

void DeferredContext::PSSetSamplerState( u32 slot, const SamplerState& value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _psSamplerStates[ slot ].Set( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagPSSamplerState + slot ] = true;
}
void DeferredContext::PSSetSamplerStateFilter( u32 slot, Filter value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _psSamplerStates[ slot ].SetFilter( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagPSSamplerState + slot ] = true;
}
void DeferredContext::PSSetSamplerStateAddressU( u32 slot, TextureAddress value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _psSamplerStates[ slot ].SetAddressU( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagPSSamplerState + slot ] = true;
}
void DeferredContext::PSSetSamplerStateAddressV( u32 slot, TextureAddress value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _psSamplerStates[ slot ].SetAddressV( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagPSSamplerState + slot ] = true;
}
void DeferredContext::PSSetSamplerStateAddressW( u32 slot, TextureAddress value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _psSamplerStates[ slot ].SetAddressW( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagPSSamplerState + slot ] = true;
}
void DeferredContext::PSSetSamplerStateMipLODBias( u32 slot, f32 value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _psSamplerStates[ slot ].SetMipLODBias( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagPSSamplerState + slot ] = true;
}
void DeferredContext::PSSetSamplerStateMaxAnisotropy( u32 slot, AnisotropicRatio value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _psSamplerStates[ slot ].SetMaxAnisotropy( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagPSSamplerState + slot ] = true;
}
void DeferredContext::PSSetSamplerStateBorderColor( u32 slot, const data::Color& value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _psSamplerStates[ slot ].SetBorderColor( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagPSSamplerState + slot ] = true;
}
void DeferredContext::PSSetSamplerStateMinLOD( u32 slot, f32 value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _psSamplerStates[ slot ].SetMinLOD( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagPSSamplerState + slot ] = true;
}
void DeferredContext::PSSetSamplerStateMaxLOD( u32 slot, f32 value )
{
	__SAMPLER_SLOT_OUT_RANGE_CHECK( slot );
	if( _psSamplerStates[ slot ].SetMaxLOD( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagPSSamplerState + slot ] = true;
}

#undef __SAMPLER_SLOT_OUT_RANGE_CHECK

void DeferredContext::RSSetRasterizerState( const RasterizerState& value )
{
	if( _rasterizerState.Set( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagRSRasterizerState ] = true;
}
void DeferredContext::RSSetRasterizerStateFillMode( FillMode value )
{
	if( _rasterizerState.SetFillMode( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagRSRasterizerState ] = true;
}
void DeferredContext::RSSetRasterizerStateCullMode( CullMode value )
{
	if( _rasterizerState.SetCullMode( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagRSRasterizerState ] = true;
}
void DeferredContext::RSSetRasterizerStateFrontCounterClockwise( bool value )
{
	if( _rasterizerState.SetFrontCounterClockwise( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagRSRasterizerState ] = true;
}
void DeferredContext::RSSetRasterizerStateDepthBias( s32 value )
{
	if( _rasterizerState.SetDepthBias( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagRSRasterizerState ] = true;
}
void DeferredContext::RSSetRasterizerStateDepthBiasClamp( f32 value )
{
	if( _rasterizerState.SetDepthBiasClamp( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagRSRasterizerState ] = true;
}
void DeferredContext::RSSetRasterizerStateSlopeScaledDepthBias( f32 value )
{
	if( _rasterizerState.SetSlopeScaledDepthBias( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagRSRasterizerState ] = true;
}
void DeferredContext::RSSetRasterizerStateDepthClipEnable( bool value )
{
	if( _rasterizerState.SetDepthClipEnable( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagRSRasterizerState ] = true;
}
void DeferredContext::RSSetRasterizerStateScissorEnable( bool value )
{
	if( _rasterizerState.SetScissorEnable( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagRSRasterizerState ] = true;
}
void DeferredContext::RSSetRasterizerStateMultisampleEnable( bool value )
{
	if( _rasterizerState.SetMultisampleEnable( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagRSRasterizerState ] = true;
}
void DeferredContext::RSSetRasterizerStateAntialiasedLineEnable( bool value )
{
	if( _rasterizerState.SetAntialiasedLineEnable( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagRSRasterizerState ] = true;
}

void DeferredContext::RSSetViewportScissorState( const ViewportScissorState& value )
{
	if( _viewportScissorState.Set( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagRSViewportScissorState ] = true;
}
void DeferredContext::RSSetViewportScissorStateViewport( u32 slot, const Viewport& value )
{
	if( _viewportScissorState.SetViewport( slot, value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagRSViewportScissorState ] = true;
}
void DeferredContext::RSSetViewportScissorStateScissor( u32 slot, const ScissorRect& value )
{
	if( _viewportScissorState.SetScissor( slot, value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagRSViewportScissorState ] = true;
}

void DeferredContext::OMSetDepthStencilState( const DepthStencilState& value )
{
	if( _depthStencilState.Set( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}
void DeferredContext::OMSetDepthStencilStateDepthEnable( bool value )
{
	if( _depthStencilState.SetDepthEnable( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}
void DeferredContext::OMSetDepthStencilStateDepthWrite( bool value )
{
	if( _depthStencilState.SetDepthWrite( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}
void DeferredContext::OMSetDepthStencilStateDepthFunc( ComparisonFunc value )
{
	if( _depthStencilState.SetDepthFunc( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}
void DeferredContext::OMSetDepthStencilStateStencilEnable( bool value )
{
	if( _depthStencilState.SetStencilEnable( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}
void DeferredContext::OMSetDepthStencilStateStencilReadMask( u8 value )
{
	if( _depthStencilState.SetStencilReadMask( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}
void DeferredContext::OMSetDepthStencilStateStencilWriteMask( u8 value )
{
	if( _depthStencilState.SetStencilWriteMask( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}
void DeferredContext::OMSetDepthStencilStateFrontFaceStencilFailOp( StencilOp value )
{
	if( _depthStencilState.SetFrontFaceStencilFailOp( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}
void DeferredContext::OMSetDepthStencilStateFrontFaceStencilDepthFailOp( StencilOp value )
{
	if( _depthStencilState.SetFrontFaceStencilDepthFailOp( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}
void DeferredContext::OMSetDepthStencilStateFrontFaceStencilPassOp( StencilOp value )
{
	if( _depthStencilState.SetFrontFaceStencilPassOp( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}
void DeferredContext::OMSetDepthStencilStateFrontFaceStencilFunc( ComparisonFunc value )
{
	if( _depthStencilState.SetFrontFaceStencilFunc( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}
void DeferredContext::OMSetDepthStencilStateBackFaceStencilFailOp( StencilOp value )
{
	if( _depthStencilState.SetBackFaceStencilFailOp( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}
void DeferredContext::OMSetDepthStencilStateBackFaceStencilDepthFailOp( StencilOp value )
{
	if( _depthStencilState.SetBackFaceStencilDepthFailOp( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}
void DeferredContext::OMSetDepthStencilStateBackFaceStencilPassOp( StencilOp value )
{
	if( _depthStencilState.SetBackFaceStencilPassOp( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}
void DeferredContext::OMSetDepthStencilStateBackFaceStencilFunc( ComparisonFunc value )
{
	if( _depthStencilState.SetBackFaceStencilFunc( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}
void DeferredContext::OMSetDepthStencilStateStencilRef( u32 value )
{
	if( _depthStencilState.SetStencilRef( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMDepthStencilState ] = true;
}

void DeferredContext::OMSetBlendState( const BlendState& value )
{
	if( _blendState.Set( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMBlendState ] = true;
}
void DeferredContext::OMSetBlendStateSampleAlphaToCoverage( bool value )
{
	if( _blendState.SetSampleAlphaToCoverage( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMBlendState ] = true;
}
void DeferredContext::OMSetBlendStateBlendEnable( bool value )
{
	if( _blendState.SetBlendEnable( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMBlendState ] = true;
}
void DeferredContext::OMSetBlendStateRGBSource( Blend value )
{
	if( _blendState.SetRGBSource( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMBlendState ] = true;
}
void DeferredContext::OMSetBlendStateRGBDest( Blend value )
{
	if( _blendState.SetRGBDest( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMBlendState ] = true;
}
void DeferredContext::OMSetBlendStateRGBBlendOp( BlendOp value )
{
	if( _blendState.SetRGBBlendOp( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMBlendState ] = true;
}
void DeferredContext::OMSetBlendStateAlphaSource( Blend value )
{
	if( _blendState.SetAlphaSource( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMBlendState ] = true;
}
void DeferredContext::OMSetBlendStateAlphaDest( Blend value )
{
	if( _blendState.SetAlphaDest( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMBlendState ] = true;
}
void DeferredContext::OMSetBlendStateAlphaBlendOp( BlendOp value )
{
	if( _blendState.SetAlphaBlendOp( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMBlendState ] = true;
}
void DeferredContext::OMSetBlendStateWriteMaskR( bool value )
{
	if( _blendState.SetWriteMaskR( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMBlendState ] = true;
}
void DeferredContext::OMSetBlendStateWriteMaskG( bool value )
{
	if( _blendState.SetWriteMaskG( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMBlendState ] = true;
}
void DeferredContext::OMSetBlendStateWriteMaskB( bool value )
{
	if( _blendState.SetWriteMaskB( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMBlendState ] = true;
}
void DeferredContext::OMSetBlendStateWriteMaskA( bool value )
{
	if( _blendState.SetWriteMaskA( value ) )
		_pipelineDirtyBits[ kPipelineDirtyBitFlagOMBlendState ] = true;
}


} // namespace render
} // namespace aroma
