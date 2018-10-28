//===========================================================================
//!
//!	@file		Algorithm.h
//!	@brief		アルゴリズム.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

namespace aroma
{
//---------------------------------------------------------------------------
//!	@brief	指定された値の小さい方を返却.
//---------------------------------------------------------------------------
template< class T > static inline
constexpr const T& Min( const T& a, const T& b )
{
	return ( a < b ) ? a : b;
}

//---------------------------------------------------------------------------
//!	@brief	指定された値の大きい方を返却.
//---------------------------------------------------------------------------
template< class T > static inline
constexpr const T& Max( const T& a, const T& b )
{
	return ( a > b ) ? a : b;
}

} // namespace aroma
