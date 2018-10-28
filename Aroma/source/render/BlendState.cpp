//===========================================================================
//!
//!	@file		BlendState.cpp
//!	@brief		ブレンドステート.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/render/BlendState.h>

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//! @brief		ステート設定.
//! @param[in]	value	設定する値.
//!	@retval		true	: 値が変更された.
//! @retval		false	: 値が変更されなかった.
//---------------------------------------------------------------------------
bool BlendState::Set( Setting state, bool value )
{
	switch( state )
	{
	case Setting::kSampleAlphaToCoverage:
		if( sampleAlphaToCoverage != value )
		{
			sampleAlphaToCoverage = value;
			return true;
		}
		break;
	case Setting::kBlendEnable:
		if( blendEnable != value )
		{
			blendEnable = value;
			return true;
		}
		break;
	case Setting::kWriteMaskR:
		if( colorMaskR != value )
		{
			colorMaskR = value;
			return true;
		}
		break;
	case Setting::kWriteMaskG:
		if( colorMaskG != value )
		{
			colorMaskG = value;
			return true;
		}
		break;
	case Setting::kWriteMaskB:
		if( colorMaskB != value )
		{
			colorMaskB = value;
			return true;
		}
		break;
	case Setting::kWriteMaskA:
		if( colorMaskA != value )
		{
			colorMaskA = value;
			return true;
		}
		break;

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}

bool BlendState::Set( Setting state, Blend value )
{
	switch( state )
	{
	case Setting::kRGBSource:
		if( rgbSource != value )
		{
			rgbSource = value;
			return true;
		}
		break;
	case Setting::kRGBDest:
		if( rgbDest != value )
		{
			rgbDest = value;
			return true;
		}
		break;
	case Setting::kAlphaSource:
		if( alphaSource != value )
		{
			alphaSource = value;
			return true;
		}
		break;
	case Setting::kAlphaDest:
		if( alphaDest != value )
		{
			alphaDest = value;
			return true;
		}
		break;

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}

bool BlendState::Set( Setting state, BlendOp value )
{
	switch( state )
	{
	case Setting::kRGBBlendOp:
		if( rgbBlendOp != value )
		{
			rgbBlendOp = value;
			return true;
		}
		break;
	case Setting::kAlphaBlendOp:
		if( alphaBlendOp != value )
		{
			alphaBlendOp = value;
			return true;
		}
		break;

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}


} // namespace render
} // namespace aroma

