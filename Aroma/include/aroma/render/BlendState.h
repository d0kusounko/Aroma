//===========================================================================
//!
//!	@file		BlendState.h
//!	@brief		ブレンドステート.
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
//! @brief		ブレンドステート.
//!
//! @todo		TODO: 必要であればレンダーターゲット別独立ブレンド(Independent Blend)対応.
//---------------------------------------------------------------------------
struct BlendState final
{
	//-----------------------------------------------------------------------
	//!	@brief		ブレンドステート設定項目.
	//-----------------------------------------------------------------------
	enum class Setting
	{
		kSampleAlphaToCoverage,		//!< bool
		kBlendEnable,				//!< bool
		kRGBSource,					//!< Blend
		kRGBDest,					//!< Blend
		kRGBBlendOp,				//!< BlendOp
		kAlphaSource,				//!< Blend
		kAlphaDest,					//!< Blend
		kAlphaBlendOp,				//!< BlendOp
		kWriteMaskR,				//!< bool
		kWriteMaskG,				//!< bool
		kWriteMaskB,				//!< bool
		kWriteMaskA,				//!< bool
	};

	bool	sampleAlphaToCoverage;
	bool	blendEnable;
	Blend	rgbSource;
	Blend	rgbDest;
	BlendOp	rgbBlendOp;
	Blend	alphaSource;
	Blend	alphaDest;
	BlendOp	alphaBlendOp;
	bool	colorMaskR;
	bool	colorMaskG;
	bool	colorMaskB;
	bool	colorMaskA;

	//-------------------------------------------------------------------
	BlendState(){ Default(); }
	void Default()
	{
		sampleAlphaToCoverage	= false;
		blendEnable				= false;
		rgbSource				= Blend::kOne;
		rgbDest					= Blend::kZero;
		rgbBlendOp				= BlendOp::kAdd;
		alphaSource				= Blend::kOne;
		alphaDest				= Blend::kZero;
		alphaBlendOp			= BlendOp::kAdd;
		colorMaskR				= true;
		colorMaskG				= true;
		colorMaskB				= true;
		colorMaskA				= true;
	}

	//---------------------------------------------------------------------------
	//! @brief		ステート設定.
	//! @param[in]	value	設定する値.
	//!	@retval		true	: 値が変更された.
	//! @retval		false	: 値が変更されなかった.
	//---------------------------------------------------------------------------
	bool Set( Setting state, bool value );
	bool Set( Setting state, Blend value );
	bool Set( Setting state, BlendOp value );
};

} // namespace render
} // namespace aroma

