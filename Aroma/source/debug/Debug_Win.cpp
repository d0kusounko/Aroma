//===========================================================================
//!
//!	@file		Debug_Win.cpp
//!	@brief		デバッグ関連 : Windows.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#ifdef AROMA_WINDOWS
#ifdef AROMA_DEBUG

#include <stdio.h>
#include <stdarg.h>
#include <aroma/debug/Debug.h>

namespace aroma {
namespace debug {

//===========================================================================
//!	@brief		デバッグ文字列出力.
//===========================================================================
void StringOut( CTStr format, ... )
{
	// TODO: ダサい.
	va_list	vaStr;
	static char	tempStr[ 1024 ];

	va_start( vaStr, format );
	vsprintf_s( tempStr, format, vaStr );
	va_end( vaStr );

	OutputDebugString( tempStr );
}

} // namespace debug
} // namespace aroma

#endif
#endif
