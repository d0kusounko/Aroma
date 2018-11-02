//===========================================================================
//!
//!	@file		ViewportScissorState.cpp
//!	@brief		ビューポートシザーステート.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/render/ViewportScissorState.h>

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
//	ステート設定.
//---------------------------------------------------------------------------
bool ViewportScissorState::Set( u32 slot, Setting state, const Viewport& value )
{
	if( slot >= kViewportsSlotMax )
	{
		AROMA_ASSERT( false, _T( "Slot is out of range.\n" ) );
		return false;
	}

	switch( state )
	{
	STATE_CASE_SET( kViewport, viewport[ slot ] );

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}

bool ViewportScissorState::Set( u32 slot, Setting state, const ScissorRect& value )
{
	if( slot >= kViewportsSlotMax )
	{
		AROMA_ASSERT( false, _T( "Slot is out of range.\n" ) );
		return false;
	}

	switch( state )
	{
	STATE_CASE_SET( kScissor, scissor[ slot ] );

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}


} // namespace render
} // namespace aroma

