//===========================================================================
//!
//!	@file		Color.h
//!	@brief		カラー.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

namespace aroma {
namespace data {

class ColorU32;

//---------------------------------------------------------------------------
//! @brief		カラー : float4版.
//---------------------------------------------------------------------------
class Color
{
public:
	Color( f32 r = 0.0f, f32 g = 0.0f, f32 b = 0.0f, f32 a = 1.0f );

	Color operator -= ( const Color& rhs );
	Color operator += ( const Color& rhs );
	Color& operator *= ( const f32 scalar );
	Color& operator /= ( const f32 scalar );

	Color operator + ( const Color& rhs ) const;
	Color operator - ( const Color& rhs ) const;
	Color operator * ( const f32 scalar ) const;
	Color operator / ( const f32 scalar ) const;
	
	bool operator == ( const Color& rhs ) const;
	bool operator != ( const Color& rhs ) const;

	void Clamp( f32 min, f32 max );
	void Set( f32 r, f32 g, f32 b, f32 a );
	void Set( const ColorU32& colorU32 );

	union {
		struct {
			f32 r, g, b, a;
		};
		f32 rgba[ 4 ];
	};
};

//---------------------------------------------------------------------------
//! @brief		カラー : Uint32(RGBA8)版.
//---------------------------------------------------------------------------
class ColorU32
{
public:
	ColorU32( u8 r = 0, u8 g = 0, u8 b = 0, u8 a = 1 );
	ColorU32( u32 rgba );

	ColorU32 operator -= ( const ColorU32& rhs );
	ColorU32 operator += ( const ColorU32& rhs );
	ColorU32& operator *= ( const f32 scalar );
	ColorU32& operator /= ( const f32 scalar );

	ColorU32 operator + ( const ColorU32& rhs ) const;
	ColorU32 operator - ( const ColorU32& rhs ) const;
	ColorU32 operator * ( const f32 scalar ) const;
	ColorU32 operator / ( const f32 scalar ) const;
	
	bool operator == ( const ColorU32& rhs ) const;
	bool operator != ( const ColorU32& rhs ) const;

	void Clamp( u8 min, u8 max );
	void Set( u8 r, u8 g, u8 b, u8 a );
	void Set( u32 rgba );
	void Set( const Color& color );

    union {
        struct {
#ifdef AROMA_ENDIAN_LE
			uint8 a, b, g, r;
#else
            uint8 r, g, b, a;
#endif
        };
        uint32 u;
    };
};


inline Color::Color( f32 rr, f32 gg, f32 bb, f32 aa )
	: r( rr )
	, g( gg )
	, b( bb )
	, a( aa )
{
}

inline Color Color::operator -= ( const Color& rhs ) {
	r -= rhs.r;
	g -= rhs.g;
	b -= rhs.b;
	a -= rhs.a;
	return *this;
}

inline Color Color::operator += ( const Color& rhs ) {
	r += rhs.r;
	g += rhs.g;
	b += rhs.b;
	a += rhs.a;
	return *this;
}

inline Color& Color::operator *= ( const f32 scalar ) {
	r *= scalar;
	g *= scalar;
	b *= scalar;
	a *= scalar;
	return *this;
}

inline Color& Color::operator /= ( const f32 scalar ) {
	r /= scalar;
	g /= scalar;
	b /= scalar;
	a /= scalar;
	return *this;
}

inline Color Color::operator + ( const Color& rhs ) const{
	return Color( *this ) += rhs;
}

inline Color Color::operator - ( const Color& rhs ) const{
	return Color( *this ) -= rhs;
}

inline Color Color::operator * ( const f32 scalar ) const{
	return Color( *this ) *= scalar;
}

inline Color Color::operator / ( const f32 scalar ) const{
	return Color( *this ) /= scalar;
}

inline bool Color::operator == ( const Color& rhs ) const {
	return memcmp( this, &rhs, sizeof( Color ) ) == 0;
}
inline bool Color::operator != ( const Color& rhs ) const
{
	return !( *this == rhs );
}

inline void Color::Clamp( f32 min, f32 max )
{
	if( r < min )	r = min;
	if( g < min )	g = min;
	if( b < min )	b = min;
	if( a < min )	a = min;

	if( r > max )	r = max;
	if( g > max )	g = max;
	if( b > max )	b = max;
	if( a > max )	a = max;
}

inline void Color::Set( f32 rr, f32 gg, f32 bb, f32 aa )
{
	r = rr;
	g = gg;
	b = bb;
	a = aa;
}

inline void Color::Set( const ColorU32& colorU32 )
{
	r = static_cast< f32 >( colorU32.r ) / 0xFF;
	g = static_cast< f32 >( colorU32.g ) / 0xFF;
	b = static_cast< f32 >( colorU32.b ) / 0xFF;
	a = static_cast< f32 >( colorU32.a ) / 0xFF;
}

inline ColorU32::ColorU32( u8 rr, u8 gg, u8 bb, u8 aa )
#if AROMA_ENDIAN_LE
	: a(aa)
	, b(bb)
	, g(gg)
	, r(rr)
#else
	: r(rr)
	, g(gg)
	, b(bb)
	, a(aa)
#endif
{
}

inline ColorU32::ColorU32( u32 rgba )
	: u( rgba )
{
}

inline ColorU32 ColorU32::operator -= ( const ColorU32& rhs ) {
	r -= rhs.r;
	g -= rhs.g;
	b -= rhs.b;
	a -= rhs.a;
	return *this;
}

inline ColorU32 ColorU32::operator += ( const ColorU32& rhs ) {
	r += rhs.r;
	g += rhs.g;
	b += rhs.b;
	a += rhs.a;
	return *this;
}

inline ColorU32& ColorU32::operator *= ( const f32 scalar ) {
	r = static_cast< u8 >( r * scalar );
	g = static_cast< u8 >( g * scalar );
	b = static_cast< u8 >( b * scalar );
	a = static_cast< u8 >( a * scalar );
	return *this;
}

inline ColorU32& ColorU32::operator /= ( const f32 scalar ) {
	r = static_cast< u8 >( r / scalar );
	g = static_cast< u8 >( g / scalar );
	b = static_cast< u8 >( b / scalar );
	a = static_cast< u8 >( a / scalar );
	return *this;
}

inline ColorU32 ColorU32::operator + ( const ColorU32& rhs ) const{
	return ColorU32( *this ) += rhs;
}

inline ColorU32 ColorU32::operator - ( const ColorU32& rhs ) const{
	return ColorU32( *this ) -= rhs;
}

inline ColorU32 ColorU32::operator * ( const f32 scalar ) const{
	return ColorU32( *this ) *= scalar;
}

inline ColorU32 ColorU32::operator / ( const f32 scalar ) const{
	return ColorU32( *this ) /= scalar;
}

inline bool ColorU32::operator == ( const ColorU32& rhs ) const {
	return memcmp( this, &rhs, sizeof( ColorU32 ) ) == 0;
}
inline bool ColorU32::operator != ( const ColorU32& rhs ) const
{
	return !( *this == rhs );
}

inline void ColorU32::Clamp( u8 min, u8 max )
{
	if( r < min )	r = min;
	if( g < min )	g = min;
	if( b < min )	b = min;
	if( a < min )	a = min;

	if( r > max )	r = max;
	if( g > max )	g = max;
	if( b > max )	b = max;
	if( a > max )	a = max;
}

inline void ColorU32::Set( u8 rr, u8 gg, u8 bb, u8 aa )
{
	r = rr;
	g = gg;
	b = bb;
	a = aa;
}
inline void ColorU32::Set( u32 rgba )
{
	u = rgba;
}

inline void ColorU32::Set( const Color& color )
{
	r = static_cast< u8 >( color.r * 0xFF );
	g = static_cast< u8 >( color.g * 0xFF );
	b = static_cast< u8 >( color.b * 0xFF );
	a = static_cast< u8 >( color.a * 0xFF );
}

} // namespace data
} // namespace aroma
