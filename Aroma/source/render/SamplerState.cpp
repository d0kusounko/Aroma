//===========================================================================
//!
//!	@file		SamplerState.cpp
//!	@brief		サンプラーステート.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/render/SamplerState.h>

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

bool SamplerState::Set( const SamplerState& state )
{
	bool diff = memcmp( this, &state, sizeof( SamplerState ) ) ? true : false;
	if( diff )
	{
		*this = state;
	}
	return diff;
}

bool SamplerState::SetFilter( Filter value )
{
	return __SetValue( filter, value );
}
bool SamplerState::SetAddressU( TextureAddress value )
{
	return __SetValue( addressU, value );
}
bool SamplerState::SetAddressV( TextureAddress value )
{
	return __SetValue( addressV, value );
}
bool SamplerState::SetAddressW( TextureAddress value )
{
	return __SetValue( addressW, value );
}
bool SamplerState::SetMipLODBias( f32 value )
{
	return __SetValue( mipLODBias, value );
}
bool SamplerState::SetMaxAnisotropy( AnisotropicRatio value )
{
	return __SetValue( maxAnisotropy, value );
}
bool SamplerState::SetBorderColor( const data::Color& value )
{
	return __SetValue( borderColor, value );
}
bool SamplerState::SetMinLOD( f32 value )
{
	return __SetValue( minLOD, value );
}
bool SamplerState::SetMaxLOD( f32 value )
{
	return __SetValue( maxLOD, value );
}

} // namespace render
} // namespace aroma

