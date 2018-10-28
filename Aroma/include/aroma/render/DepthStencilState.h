//===========================================================================
//!
//!	@file		DepthStencilState.h
//!	@brief		深度ステンシルステート.
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
//! @brief		深度ステンシルステートオペレーター.
//---------------------------------------------------------------------------
struct DepthStencilStateOp final
{
	//-------------------------------------------------------------------
	DepthStencilStateOp(){ Default(); }
	void Default()
	{
	}
};

//---------------------------------------------------------------------------
//! @brief		深度ステンシルステート.
//---------------------------------------------------------------------------
struct DepthStencilState final
{
	//-----------------------------------------------------------------------
	//!	@brief		深度ステンシルステート設定項目.
	//-----------------------------------------------------------------------
	enum class Setting
	{
		kDepthEnable,					//!< bool
		kDepthWrite,					//!< bool
		kDepthFunc,						//!< ComparisonFunc
		kStencilEnable,					//!< bool
		kStencilReadMask,				//!< u8
		kStencilWriteMask,				//!< u8
		kFrontFace,						//!< DepthStencilStateOp
		kFrontFaceStencilFailOp,		//!< StencilOp
		kFrontFaceStencilDepthFailOp,	//!< StencilOp
		kFrontFaceStencilPassOp,		//!< StencilOp
		kFrontFaceStencilFunc,			//!< ComparisonFunc
		kBackFaceStencilFailOp,			//!< StencilOp
		kBackFaceStencilDepthFailOp,	//!< StencilOp
		kBackFaceStencilPassOp,			//!< StencilOp
		kBackFaceStencilFunc,			//!< ComparisonFunc
	};

	bool				depthEnable;					//!< 深度テスト有効.
	bool				depthWrite; 					//!< 深度書き込み有効.
	ComparisonFunc		depthFunc;						//!< 深度比較関数.
	bool				stencilEnable;					//!< ステンシルテスト有効.
	u8					stencilReadMask;				//!< 深度ステンシル読み取りマスク値.
	u8					stencilWriteMask;				//!< 深度ステンシル書き込みマスク値.

	StencilOp			frontFaceStencilFailOp;			//!< 前向きサーフェス: ステンシルテスト不合格時実行処理.
	StencilOp			frontFaceStencilDepthFailOp;	//!< 前向きサーフェス: 深度テスト不合格時実行処理.
	StencilOp			frontFaceStencilPassOp;			//!< 前向きサーフェス: 両テスト合格時実行処理.
	ComparisonFunc		frontFaceStencilFunc;			//!< 前向きサーフェス: ステンシル比較関数.

	StencilOp			backFaceStencilFailOp;			//!< 後ろ向きサーフェス: ステンシルテスト不合格時実行処理.
	StencilOp			backFaceStencilDepthFailOp;		//!< 後ろ向きサーフェス: 深度テスト不合格時実行処理.
	StencilOp			backFaceStencilPassOp;			//!< 後ろ向きサーフェス: 両テスト合格時実行処理.
	ComparisonFunc		backFaceStencilFunc;			//!< 後ろ向きサーフェス: ステンシル比較関数.

	//-------------------------------------------------------------------
	DepthStencilState(){ Default(); }
	void Default()
	{
		depthEnable			= true;
		depthWrite			= true;
		depthFunc			= ComparisonFunc::kLess;
		stencilEnable		= false;
		stencilReadMask		= 0xff;
		stencilWriteMask	= 0xff;

		frontFaceStencilFailOp		= StencilOp::kKeep;
		frontFaceStencilDepthFailOp	= StencilOp::kKeep;
		frontFaceStencilPassOp		= StencilOp::kKeep;
		frontFaceStencilFunc		= ComparisonFunc::kAlways;

		backFaceStencilFailOp		= StencilOp::kKeep;;
		backFaceStencilDepthFailOp	= StencilOp::kKeep;;
		backFaceStencilPassOp		= StencilOp::kKeep;;
		backFaceStencilFunc			= ComparisonFunc::kAlways;;
	}

	//---------------------------------------------------------------------------
	//! @brief		ステート設定.
	//! @param[in]	value	設定する値.
	//!	@retval		true	: 値が変更された.
	//! @retval		false	: 値が変更されなかった.
	//---------------------------------------------------------------------------
	bool Set( Setting state, bool value );
	bool Set( Setting state, ComparisonFunc value );
	bool Set( Setting state, u8 value );
	bool Set( Setting state, StencilOp value );
};

} // namespace render
} // namespace aroma

