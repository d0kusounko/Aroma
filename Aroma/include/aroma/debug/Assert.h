//===========================================================================
//!
//!	@file		Assert.h
//!	@brief		アサート処理.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include "../app/App.h"

namespace aroma
{
} // namespace aroma

//---------------------------------------------------------------------------
//!	@brief アサートマクロ.
//---------------------------------------------------------------------------
#ifdef AROMA_DEBUG
#define AROMA_ASSERT( exp, ...) if( !( exp ) ) { aroma::app::Panic( __FILE__, __LINE__, __VA_ARGS__ ); }
#else
#define AROMA_ASSERT( exp, ... )
#endif

//---------------------------------------------------------------------------
//!	@brief 静的アサートマクロ.
//---------------------------------------------------------------------------
#define AROMA_STATIC_ASSERT( exp, ... ) static_assert( exp, __VA_ARGS__ )
