//===========================================================================
//!
//!	@file		Alignment.h
//!	@brief		アラインメント関連.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include "debug/Assert.h"

#ifdef AROMA_WINDOWS

#define AROMA_ALIGN32_BEGIN		__declspec( align(32) )
#define AROMA_ALIGN32_END
#define AROMA_ALIGN16_BEGIN		__declspec( align(16) )
#define AROMA_ALIGN16_END
#define AROMA_ALIGN8_BEGIN		__declspec( align( 8) )
#define AROMA_ALIGN8_END
#define AROMA_ALIGN4_BEGIN		__declspec( align( 4) )
#define AROMA_ALIGN4_END

#elif 0

#define AROMA_ALIGN32_BEGIN
#define AROMA_ALIGN32_END		__attribute__( aligned(32) )
#define AROMA_ALIGN16_BEGIN
#define AROMA_ALIGN16_END		__attribute__( aligned(16) )
#define AROMA_ALIGN8_BEGIN
#define AROMA_ALIGN8_END		__attribute__( aligned( 8) )
#define AROMA_ALIGN4_BEGIN
#define AROMA_ALIGN4_END		__attribute__( aligned( 4) )

#else
#error Undefined platform.
#endif

namespace aroma {

//---------------------------------------------------------------------------
//! @brief	アラインメントサイズ切り上げマクロ.
//---------------------------------------------------------------------------
static inline size_t AlignUp( size_t size, size_t alignment )
{
	AROMA_ASSERT( !(alignment & (alignment-1)), _T( "Alignment size must be a power of 2.\n" ) );
	return (size + (alignment - 1)) & ~(alignment - 1);
}

} // namespace aroma
