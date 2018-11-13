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
//! @brief		深度ステンシルステート.
//---------------------------------------------------------------------------
struct DepthStencilState final
{
	// TODO: RenderStateCacheのkeyと同じ形でビットフィールド化.
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
	u32					stencilRef;						//!< ステンシルテスト実行時の参照値.

	//-----------------------------------------------------------------------
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

		backFaceStencilFailOp		= StencilOp::kKeep;
		backFaceStencilDepthFailOp	= StencilOp::kKeep;
		backFaceStencilPassOp		= StencilOp::kKeep;
		backFaceStencilFunc			= ComparisonFunc::kAlways;

		stencilRef					= 0;
	}

	//-----------------------------------------------------------------------
	//! @brief		各ステート設定.
	//! @param[in]	state	設定するステート項目.
	//! @param[in]	value	設定する値.
	//!	@retval		true	: 値が変更された.
	//! @retval		false	: 値が変更されなかった.
	//-----------------------------------------------------------------------
	bool Set( const DepthStencilState& value );
	bool SetDepthEnable( bool value );
	bool SetDepthWrite( bool value );
	bool SetDepthFunc( ComparisonFunc value );
	bool SetStencilEnable( bool value );
	bool SetStencilReadMask( u8 value );
	bool SetStencilWriteMask( u8 value );
	bool SetFrontFaceStencilFailOp( StencilOp value );
	bool SetFrontFaceStencilDepthFailOp( StencilOp value );
	bool SetFrontFaceStencilPassOp( StencilOp value );
	bool SetFrontFaceStencilFunc( ComparisonFunc value );
	bool SetBackFaceStencilFailOp( StencilOp value );
	bool SetBackFaceStencilDepthFailOp( StencilOp value );
	bool SetBackFaceStencilPassOp( StencilOp value );
	bool SetBackFaceStencilFunc( ComparisonFunc value );
	bool SetStencilRef( u32 value );
};

} // namespace render
} // namespace aroma

