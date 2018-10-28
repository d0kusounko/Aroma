//===========================================================================
//!
//!	@file		String_Win.h
//!	@brief		文字列操作 : Windows.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#ifdef AROMA_WINDOWS

#include <aroma/data/String.h>
#include <string.h>

namespace aroma {
namespace data {

//---------------------------------------------------------------------------
// 文字列のコピー.
//---------------------------------------------------------------------------
TStr StrCopy( CTStr srcStr )
{
#ifdef AROMA_UNICODE
	TStr retStr = _wcsdup( srcStr );
#else
	TStr retStr = _strdup( srcStr );
#endif
	AROMA_ASSERT( retStr, "Failed to memory allocation." );
	return retStr;
}

} // namespace data
} // namespace

#endif
