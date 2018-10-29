//===========================================================================
//!
//!	@file		Debug.h
//!	@brief		デバッグ関連.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

namespace aroma {
namespace debug {

#ifdef AROMA_DEBUG
void StringOut( CTStr format, ... );
#endif

} // namespace debug
} // namespace aroma

//!--------------------------------------------------------------------------
//!	@brief		デバッグ文字列出力.
//!--------------------------------------------------------------------------
#ifdef AROMA_DEBUG
#define AROMA_DEBUG_OUT( ... )	aroma::debug::StringOut( __VA_ARGS__ )
#else
#define AROMA_DEBUG_OUT( ... )
#endif
