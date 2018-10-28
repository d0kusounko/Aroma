//===========================================================================
//!
//!	@file		Endian.h
//!	@brief		エンディアン関連.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

//---------------------------------------------------------------------------
//! @brief プラットフォーム別バイトオーダー定義.
//---------------------------------------------------------------------------
#if defined( AROMA_WINDOWS )
#define AROMA_ENDIAN_LE 1	//!< // リトルエンディアン.
#elif 0
#define AROMA_ENDIAN_BE 1	//!< // ビッグエンディアン.
#else
#error Undefined platform.
#endif

namespace aroma
{
//---------------------------------------------------------------------------
//! @brief リトルエンディアンデータアクセサー.
//---------------------------------------------------------------------------
template< class T >
struct LE
{
	LE() = default;
	LE( const T value )
	{
		operator = ( value );
	}

	T operator = ( const T value )
	{
#if AROMA_ENDIAN_LE
		_data = value;
#else
		_data = SwapEndian( value );
#endif

		return value;
	}

	//! キャスト.
	operator T() const
	{
#if AROMA_ENDIAN_LE
		return _data;
#else
		return SwapEndian( _data );
#endif
	}

	T _data;
};

//---------------------------------------------------------------------------
//! @brief ビッグエンディアンデータアクセサー.
//---------------------------------------------------------------------------
template< class T >
struct BE
{
	BE() {}
	BE( const T value )
	{
		operator = ( value );
	}

	T operator = ( const T value )
	{
#if AROMA_ENDIAN_BE
		_data = value;
#else
		_data = SwapEndian( value );
#endif
		return value;
	}

	//! キャスト.
	operator T() const
	{
#if AROMA_ENDIAN_BE
		return _data;
#else
		return SwapEndian( _data );
#endif
	}

	T _data;
};

//---------------------------------------------------------------------------
//! @brief 16bitデータのエンディアン変換.
//---------------------------------------------------------------------------
static inline u16 SwapEndian16( u16 data )
{
	u32 _data = data;
	return static_cast< u16 >(
		((_data >> 8) & 0x00ffui16) +
		((_data << 8) & 0xff00ui16)
	);
}

//---------------------------------------------------------------------------
//! @brief 32bitデータのエンディアン変換.
//---------------------------------------------------------------------------
static inline u32 SwapEndian32( u32 data )
{
	return
		((data >> 24)	& 0x000000ffui32) +
		((data >> 8)	& 0x0000ff00ui32) +
		((data << 8)	& 0x00ff0000ui32) +
		((data << 24)	& 0xff000000ui32);
}

//---------------------------------------------------------------------------
//! @brief 64bitデータのエンディアン変換.
//---------------------------------------------------------------------------
static inline u64 SwapEndian64( u64 data )
{
	return
		((data >> 56)	& 0x00000000000000ffui64) +
		((data >> 40)	& 0x000000000000ff00ui64) +
		((data >> 24)	& 0x0000000000ff0000ui64) +
		((data >> 8)	& 0x00000000ff000000ui64) +
		((data << 8)	& 0x000000ff00000000ui64) +
		((data << 24)	& 0x0000ff0000000000ui64) +
		((data << 40)	& 0x00ff000000000000ui64) +
		((data << 56)	& 0xff00000000000000ui64);
}

//===========================================================================
//! @name	データ別エンディアン変換関数群.
//===========================================================================
//! @{
static inline u8	SwapEndian( u8& data )
{
	return data;
}
static inline u16	SwapEndian( const u16& data )
{
	return SwapEndian16( data );
}
static inline u32	SwapEndian( const u32& data )
{
	return SwapEndian32( data );
}
static inline u64	SwapEndian( const u64& data )
{
	return SwapEndian64( data );
}
static inline s8	SwapEndian( const s8& data )
{
	return data;
}
static inline s16	SwapEndian( const s16& data )
{
	u16 temp  = SwapEndian16( *( reinterpret_cast< const u16* >( &data ) ) );
	return *( reinterpret_cast< const s16* >( &temp ) );
}
static inline s32	SwapEndian( const s32& data )
{
	u32 temp  = SwapEndian32( *( reinterpret_cast< const u32* >( &data ) ) );
	return *( reinterpret_cast< const s32* >( &temp ) );
}
static inline s64	SwapEndian( const s64& data )
{
	u64 temp  = SwapEndian64( *( reinterpret_cast< const u64* >( &data ) ) );
	return *( reinterpret_cast< const s64* >( &temp ) );
}
static inline f32	SwapEndian( const f32& data )
{
	u32 temp  = SwapEndian32( *( reinterpret_cast< const u32* >( &data ) ) );
	return *( reinterpret_cast< const f32* >( &temp ) );
}
static inline f64	SwapEndian( const f64& data )
{
	u64 temp  = SwapEndian64( *( reinterpret_cast< const u64* >( &data ) ) );
	return *( reinterpret_cast< const f64* >( &temp ) );
}
//! @}

} // namespace aroma
