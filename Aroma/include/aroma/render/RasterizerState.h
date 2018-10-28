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
	//-----------------------------------------------------------------------
	//!	@brief		ラスタライザーステート設定項目.
	//-----------------------------------------------------------------------
	enum class Setting
	{
		kFillMode,					//!< FillMode
		kCullMode,					//!< CullMode
		kFrontCounterClockwise,		//!< bool
		kDepthBias,					//!< s32
		kDepthBiasClamp,			//!< f32
		kSlopeScaledDepthBias,		//!< f32
		kDepthClipEnable,			//!< bool
		kScissorEnable,				//!< bool
		kMultisampleEnable,			//!< bool
		kAntialiasedLineEnable,		//!< bool
	};

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

	//-------------------------------------------------------------------
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

	//---------------------------------------------------------------------------
	//! @brief		ステート設定.
	//! @param[in]	value	設定する値.
	//!	@retval		true	: 値が変更された.
	//! @retval		false	: 値が変更されなかった.
	//---------------------------------------------------------------------------
	bool Set( Setting state, bool value );
	bool Set( Setting state, FillMode value );
	bool Set( Setting state, CullMode value );
	bool Set( Setting state, s32 value );
	bool Set( Setting state, f32 value );
};

} // namespace render
} // namespace aroma

