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
	// TODO: RenderStateCacheのkeyと同じ形でビットフィールド化.
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

	//-----------------------------------------------------------------------
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

	bool Set( const BlendState& value );
	bool SetSampleAlphaToCoverage( bool value );
	bool SetBlendEnable( bool value );
	bool SetRGBSource( Blend value );
	bool SetRGBDest( Blend value );
	bool SetRGBBlendOp( BlendOp value );
	bool SetAlphaSource( Blend value );
	bool SetAlphaDest( Blend value );
	bool SetAlphaBlendOp( BlendOp value );
	bool SetWriteMaskR( bool value );
	bool SetWriteMaskG( bool value );
	bool SetWriteMaskB( bool value );
	bool SetWriteMaskA( bool value );
};

} // namespace render
} // namespace aroma

