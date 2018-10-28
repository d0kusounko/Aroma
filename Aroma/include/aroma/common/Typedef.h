//===========================================================================
//!
//!	@file		Typedef.h
//!	@brief		型定義.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include <stdint.h>
#include <tchar.h>
#include <climits>
#include <float.h>

namespace aroma
{
//===========================================================================
//!	@name	型定義.
//===========================================================================
//!@{
using sint8		= int8_t;
using uint8		= uint8_t;
using sint16	= int16_t;
using uint16	= uint16_t;
using sint32	= int32_t;
using uint32	= uint32_t;
using sint64	= int64_t;
using uint64	= uint64_t;
using float32	= float;
using float64	= double;

using s8		= sint8;
using u8		= uint8;
using s16		= sint16;
using u16		= uint16;
using s32		= sint32;
using u32		= uint32;
using s64		= sint64;
using u64		= uint64;
using f32		= float32;
using f64		= float64;

using intptr	= intptr_t;
using uintptr	= uintptr_t;

#ifdef AROMA_UNICODE
using TChar		= wchar_t;
using TStr		= wchar_t*;
using CTStr		= const wchar_t*;
#else
using TChar		= char;
using TStr		= char*;
using CTStr		= const char*;
#endif
//!@}

} // namespace aroma

//---------------------------------------------------------------------------
//!	@name		マクロ定義.
//---------------------------------------------------------------------------
//! @
#define AROMA_SINT8_MAX ( 127i8 )							//!< s8 最大値.
#define AROMA_SINT8_MIN ( -127i8 - 1 )						//!< s8 最低値.
#define AROMA_UINT8_MAX ( 0xffui8 )							//!< u8 最大値.
#define AROMA_UINT8_MIN ( 0ui8 )							//!< u8 最低値.

#define AROMA_SINT16_MAX ( 32767i16 )						//!< s16 最大値.
#define AROMA_SINT16_MIN ( -32767i16 - 1 )					//!< s16 最低値.
#define AROMA_UINT16_MAX ( 0xffffui16 )						//!< u16 最大値.
#define AROMA_UINT16_MIN ( 0ui16 )							//!< u16 最低値.

#define AROMA_SINT32_MAX ( 2147483647i32 )					//!< s32 最大値.
#define AROMA_SINT32_MIN ( -2147483647i32 - 1 )				//!< s32 最低値.
#define AROMA_UINT32_MAX ( 0xffffffffui32 )					//!< u32 最大値.
#define AROMA_UINT32_MIN ( 0ui32 )							//!< u32 最低値.

#define AROMA_SINT64_MAX ( 9223372036854775807i64 )			//!< s32 最大値.
#define AROMA_SINT64_MIN ( -9223372036854775807i64 - 1 )	//!< s32 最低値.
#define AROMA_UINT64_MAX ( 0xffffffffffffffffui64 )			//!< u32 最大値.
#define AROMA_UINT64_MIN ( 0ui64 )							//!< u32 最低値.

#define	AROMA_FLT32_MAX	( 3.402823466e+38F )				//!< f32 最大値.
#define	AROMA_FLT32_MIN	( 1.175494351e-38F )				//!< f32 最低値.
#define	AROMA_FLT64_MAX	( 1.7976931348623158e+308 )			//!< f64 最大値.
#define	AROMA_FLT64_MIN	( 2.2250738585072014e-308 )			//!< f64 最低値.

//---------------------------------------------------------------------------
//! @brief		静的配列の要素数取得マクロ.
//---------------------------------------------------------------------------
template< size_t size > struct aroma_elementsof_t { typedef aroma::sint8	type[size]; };
template< typename element_type, size_t size > typename aroma_elementsof_t<size>::type& AROMA_ARRAY_OF(element_type(&)[size]);
#define AROMA_ARRAY_OF( _array ) sizeof( AROMA_ARRAY_OF( _array ) )


//! @}
