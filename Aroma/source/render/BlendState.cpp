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

bool BlendState::Set( const BlendState& value )
{
	bool diff = memcmp( this, &value, sizeof( BlendState ) ) ? true : false;
	if( diff )
	{
		*this = value;
	}
	return diff;
}

bool BlendState::SetSampleAlphaToCoverage( bool value )
{
	return __SetValue( sampleAlphaToCoverage, value );
}
bool BlendState::SetBlendEnable( bool value )
{
	return __SetValue( blendEnable, value );
}
bool BlendState::SetRGBSource( Blend value )
{
	return __SetValue( rgbSource, value );
}
bool BlendState::SetRGBDest( Blend value )
{
	return __SetValue( rgbDest, value );
}
bool BlendState::SetRGBBlendOp( BlendOp value )
{
	return __SetValue( rgbBlendOp, value );
}
bool BlendState::SetAlphaSource( Blend value )
{
	return __SetValue( alphaSource, value );
}
bool BlendState::SetAlphaDest( Blend value )
{
	return __SetValue( alphaDest, value );
}
bool BlendState::SetAlphaBlendOp( BlendOp value )
{
	return __SetValue( alphaBlendOp, value );
}
bool BlendState::SetWriteMaskR( bool value )
{
	return __SetValue( colorMaskR, value );
}
bool BlendState::SetWriteMaskG( bool value )
{
	return __SetValue( colorMaskG, value );
}
bool BlendState::SetWriteMaskB( bool value )
{
	return __SetValue( colorMaskB, value );
}
bool BlendState::SetWriteMaskA( bool value )
{
	return __SetValue( colorMaskA, value );
}

} // namespace render
} // namespace aroma

