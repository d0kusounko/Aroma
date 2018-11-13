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

#ifndef __SLOT_OUT_RANGE_CHECK
#define __SLOT_OUT_RANGE_CHECK( _slot_ )						\
	if( _slot_ >= kViewportsSlotMax )							\
	{															\
		AROMA_ASSERT( false, _T( "Slot is out of range.\n" ) );	\
		return false;											\
	}
#endif

namespace
{
	template< typename T >
	bool __SetValue( T& lhs, const T& rhs )
	{
		if( lhs != rhs )
		{
			lhs = rhs;
			return true;
		}
		return false;
	}
} // namespace

// TODO: 各ステータス値の正規チェックを行う.

bool ViewportScissorState::Set( const ViewportScissorState& value )
{
	bool diff = memcmp( this, &value, sizeof( ViewportScissorState ) ) ? true : false;
	if( diff )
	{
		*this = value;
	}
	return diff;
}

bool ViewportScissorState::SetViewport( u32 slot, const Viewport& value )
{
	__SLOT_OUT_RANGE_CHECK( slot );
	return __SetValue( viewport[ slot ], value );
}
bool ViewportScissorState::SetScissor( u32 slot, const ScissorRect& value )
{
	__SLOT_OUT_RANGE_CHECK( slot );
	return __SetValue( scissor[ slot ], value );
}

} // namespace render
} // namespace aroma



