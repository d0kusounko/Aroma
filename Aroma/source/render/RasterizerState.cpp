//===========================================================================
//!
//!	@file		RasterizerState.cpp
//!	@brief		ラスタライザーステート.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/render/RasterizerState.h>

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//! @brief		ステート設定.
//! @param[in]	value	設定する値.
//!	@retval		true	: 値が変更された.
//! @retval		false	: 値が変更されなかった.
//---------------------------------------------------------------------------
bool RasterizerState::Set( Setting state, bool value )
{
	switch( state )
	{
	case Setting::kFrontCounterClockwise:
		if( frontCounterClockwise != value )
		{
			frontCounterClockwise = value;
			return true;
		}
		break;
	case Setting::kDepthClipEnable:
		if( depthClipEnable != value )
		{
			depthClipEnable = value;
			return true;
		}
		break;
	case Setting::kScissorEnable:
		if( scissorEnable != value )
		{
			scissorEnable = value;
			return true;
		}
		break;
	case Setting::kMultisampleEnable:
		if( multisampleEnable != value )
		{
			multisampleEnable = value;
			return true;
		}
		break;
	case Setting::kAntialiasedLineEnable:
		if( antialiasedLineEnable != value )
		{
			antialiasedLineEnable = value;
			return true;
		}
		break;

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}

bool RasterizerState::Set( Setting state, FillMode value )
{
	switch( state )
	{
	case Setting::kFillMode:
		if( fillMode != value )
		{
			fillMode = value;
			return true;
		}
		break;

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}

bool RasterizerState::Set( Setting state, CullMode value )
{
	switch( state )
	{
	case Setting::kCullMode:
		if( cullMode != value )
		{
			cullMode = value;
			return true;
		}
		break;

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}

bool RasterizerState::Set( Setting state, s32 value )
{
	switch( state )
	{
	case Setting::kDepthBias:
		if( depthBias != value )
		{
			depthBias = value;
			return true;
		}
		break;

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}

bool RasterizerState::Set( Setting state, f32 value )
{
	switch( state )
	{
	case Setting::kDepthBiasClamp:
		if( depthBiasClamp != value )
		{
			depthBiasClamp = value;
			return true;
		}
		break;
	case Setting::kSlopeScaledDepthBias:
		if( slopeScaledDepthBias != value )
		{
			slopeScaledDepthBias = value;
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

