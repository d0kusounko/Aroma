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

bool RasterizerState::Set( const RasterizerState& value )
{
	bool diff = memcmp( this, &value, sizeof( RasterizerState ) ) ? true : false;
	if( diff )
	{
		*this = value;
	}
	return diff;
}

bool RasterizerState::SetFillMode( FillMode value )
{
	return __SetValue( fillMode, value );
}
bool RasterizerState::SetCullMode( CullMode value )
{
	return __SetValue( cullMode, value );
}
bool RasterizerState::SetFrontCounterClockwise( bool value )
{
	return __SetValue( frontCounterClockwise, value );
}
bool RasterizerState::SetDepthBias( s32 value )
{
	return __SetValue( depthBias, value );
}
bool RasterizerState::SetDepthBiasClamp( f32 value )
{
	return __SetValue( depthBiasClamp, value );
}
bool RasterizerState::SetSlopeScaledDepthBias( f32 value )
{
	return __SetValue( slopeScaledDepthBias, value );
}
bool RasterizerState::SetDepthClipEnable( bool value )
{
	return __SetValue( depthClipEnable, value );
}
bool RasterizerState::SetScissorEnable( bool value )
{
	return __SetValue( scissorEnable, value );
}
bool RasterizerState::SetMultisampleEnable( bool value )
{
	return __SetValue( multisampleEnable, value );
}
bool RasterizerState::SetAntialiasedLineEnable( bool value )
{
	return __SetValue( antialiasedLineEnable, value );
}

} // namespace render
} // namespace aroma

