//===========================================================================
//!
//!	@file		RasterizerState.h
//!	@brief		ラスタライザーステート.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include "RenderDef.h"

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//! @brief		ラスタライザーステート.
//---------------------------------------------------------------------------
struct RasterizerState final
{
	// TODO: RenderStateCacheのkeyと同じ形でビットフィールド化.
	FillMode	fillMode;				//!< 描画モード.
	CullMode	cullMode;				//!< カリングモード.
	bool		frontCounterClockwise;	//!< 左回り前向き判定.
	s32			depthBias;				//!< 深度バイアス.
	f32			depthBiasClamp;			//!< 最大深度バイアス.
	f32			slopeScaledDepthBias;	//!< スロープに対するスカラ.
	bool		depthClipEnable;		//!< 距離クリッピング有効.
	bool		scissorEnable;			//!< シザー矩形有効.
	bool		multisampleEnable;		//!< MSAA有効.
	bool		antialiasedLineEnable;	//!< 線のアンチエイリアシング有効.

	//-----------------------------------------------------------------------
	RasterizerState(){ Default(); }
	void Default()
	{
		fillMode				= FillMode::kSolid;
		cullMode				= CullMode::kNone;
		frontCounterClockwise	= true;
		depthBias				= 0;
		depthBiasClamp			= 0.0f;
		slopeScaledDepthBias	= 0.0f;
		depthClipEnable			= true;
		scissorEnable			= false;
		multisampleEnable		= false;
		antialiasedLineEnable	= false;
	}

	//-----------------------------------------------------------------------
	//! @brief		各ステート設定.
	//! @param[in]	state	設定するステート項目.
	//! @param[in]	value	設定する値.
	//!	@retval		true	: 値が変更された.
	//! @retval		false	: 値が変更されなかった.
	//-----------------------------------------------------------------------
	bool Set( const RasterizerState& value );
	bool SetFillMode( FillMode value );
	bool SetCullMode( CullMode value );
	bool SetFrontCounterClockwise( bool value );
	bool SetDepthBias( s32 value );
	bool SetDepthBiasClamp( f32 value );
	bool SetSlopeScaledDepthBias( f32 value );
	bool SetDepthClipEnable( bool value );
	bool SetScissorEnable( bool value );
	bool SetMultisampleEnable( bool value );
	bool SetAntialiasedLineEnable( bool value );
};




} // namespace render
} // namespace aroma

