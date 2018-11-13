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

bool DepthStencilState::Set( const DepthStencilState& value )
{
	bool diff = memcmp( this, &value, sizeof( DepthStencilState ) ) ? true : false;
	if( diff )
	{
		*this = value;
	}
	return diff;
}

bool DepthStencilState::SetDepthEnable( bool value )
{
	return __SetValue( depthEnable, value );
}
bool DepthStencilState::SetDepthWrite( bool value )
{
	return __SetValue( depthWrite, value );
}
bool DepthStencilState::SetDepthFunc( ComparisonFunc value )
{
	return __SetValue( depthFunc, value );
}
bool DepthStencilState::SetStencilEnable( bool value )
{
	return __SetValue( stencilEnable, value );
}
bool DepthStencilState::SetStencilReadMask( u8 value )
{
	return __SetValue( stencilReadMask, value );
}
bool DepthStencilState::SetStencilWriteMask( u8 value )
{
	return __SetValue( stencilWriteMask, value );
}
bool DepthStencilState::SetFrontFaceStencilFailOp( StencilOp value )
{
	return __SetValue( frontFaceStencilFailOp, value );
}
bool DepthStencilState::SetFrontFaceStencilDepthFailOp( StencilOp value )
{
	return __SetValue( frontFaceStencilDepthFailOp, value );
}
bool DepthStencilState::SetFrontFaceStencilPassOp( StencilOp value )
{
	return __SetValue( frontFaceStencilPassOp, value );
}
bool DepthStencilState::SetFrontFaceStencilFunc( ComparisonFunc value )
{
	return __SetValue( frontFaceStencilFunc, value );
}
bool DepthStencilState::SetBackFaceStencilFailOp( StencilOp value )
{
	return __SetValue( backFaceStencilFailOp, value );
}
bool DepthStencilState::SetBackFaceStencilDepthFailOp( StencilOp value )
{
	return __SetValue( backFaceStencilDepthFailOp, value );
}
bool DepthStencilState::SetBackFaceStencilPassOp( StencilOp value )
{
	return __SetValue( backFaceStencilPassOp, value );
}
bool DepthStencilState::SetBackFaceStencilFunc( ComparisonFunc value )
{
	return __SetValue( backFaceStencilFunc, value );
}
bool DepthStencilState::SetStencilRef( u32 value )
{
	return __SetValue( stencilRef, value );
}

} // namespace render
} // namespace aroma

