//===========================================================================
//!
//!	@file		DepthStencilState.cpp
//!	@brief		深度ステンシルステート.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/render/DepthStencilState.h>

namespace aroma {
namespace render {

#define STATE_CASE_SET( _setting, _m_Value )	\
	case Setting::_setting:						\
		if( _m_Value != value )					\
		{										\
			_m_Value = value;					\
			return true;						\
		}										\
		break;									\

//---------------------------------------------------------------------------
//! @brief		ステート設定.
//! @param[in]	value	設定する値.
//!	@retval		true	: 値が変更された.
//! @retval		false	: 値が変更されなかった.
//---------------------------------------------------------------------------
bool DepthStencilState::Set( Setting state, bool value )
{
	switch( state )
	{
	STATE_CASE_SET( kDepthEnable, depthEnable );
	STATE_CASE_SET( kDepthWrite, depthWrite );
	STATE_CASE_SET( kStencilEnable, stencilEnable );

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}

bool DepthStencilState::Set( Setting state, ComparisonFunc value )
{
	switch( state )
	{
	STATE_CASE_SET( kDepthFunc, depthFunc );
	STATE_CASE_SET( kFrontFaceStencilFunc, frontFaceStencilFunc );
	STATE_CASE_SET( kBackFaceStencilFunc, backFaceStencilFunc );

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}

bool DepthStencilState::Set( Setting state, u8 value )
{
	switch( state )
	{
	STATE_CASE_SET( kStencilReadMask, stencilReadMask );
	STATE_CASE_SET( kStencilWriteMask, stencilWriteMask );

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}

bool DepthStencilState::Set( Setting state, StencilOp value )
{
	switch( state )
	{
	STATE_CASE_SET( kFrontFaceStencilFailOp, frontFaceStencilFailOp );
	STATE_CASE_SET( kFrontFaceStencilDepthFailOp, frontFaceStencilDepthFailOp );
	STATE_CASE_SET( kFrontFaceStencilPassOp, frontFaceStencilPassOp );
	STATE_CASE_SET( kBackFaceStencilFailOp, backFaceStencilFailOp );
	STATE_CASE_SET( kBackFaceStencilDepthFailOp, backFaceStencilDepthFailOp );
	STATE_CASE_SET( kBackFaceStencilPassOp, backFaceStencilPassOp );

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}


} // namespace render
} // namespace aroma

