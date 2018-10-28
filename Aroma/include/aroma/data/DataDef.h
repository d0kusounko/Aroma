//===========================================================================
//!
//!	@file		DataDef.h
//!	@brief		データフォーマット定義.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//!	@todo		TODO:数が多くなってきたらジャンル別に分離.
//!
//===========================================================================
#pragma once

namespace aroma {
namespace data {

//---------------------------------------------------------------------------
//! @brief		データフォーマット識別4バイトコード.
//---------------------------------------------------------------------------
template < s32 c0, s32 c1, s32 c2, s32 c3 >
struct FourCC
{
    static constexpr u32 value = (c3 << 24) | (c2 << 16) | (c1 << 8) | (c0 << 0);
};

//---------------------------------------------------------------------------
//! @brief		ピクセルフォーマット.
//---------------------------------------------------------------------------
enum class PixelFormat {
	kUnknown,
	kR32G32B32A32Typeless,
	kR32G32B32A32Float,
	kR32G32B32A32Uint,
	kR32G32B32A32Sint,
	kR32G32B32Typeless,
	kR32G32B32Float,
	kR32G32B32Uint,
	kR32G32B32Sint,
	kR16G16B16A16Typeless,
	kR16G16B16A16Float,
	kR16G16B16A16Unorm,
	kR16G16B16A16Uint,
	kR16G16B16A16Snorm,
	kR16G16B16A16Sint,
	kR32G32Typeless,
	kR32G32Float,
	kR32G32Uint,
	kR32G32Sint,
	kR32G8X24Typeless,
	kD32FloatS8X24Uint,
	kR32FloatX8X24Typeless,
	kX32TypelessG8X24Uint,
	kR10G10B10A2Typeless,
	kR10G10B10A2Unorm,
	kR10G10B10A2Uint,
	kR11G11B10Float,
	kR8G8B8A8Typeless,
	kR8G8B8A8Unorm,
	kR8G8B8A8UnormSrgb,
	kR8G8B8A8Uint,
	kR8G8B8A8Snorm,
	kR8G8B8A8Sint,
	kR16G16Typeless,
	kR16G16Float,
	kR16G16Unorm,
	kR16G16Uint,
	kR16G16Snorm,
	kR16G16Sint,
	kR32Typeless,
	kD32Float,
	kR32Float,
	kR32Uint,
	kR32Sint,
	kR24G8Typeless,
	kD24UnormS8Uint,
	kR24UnormX8Typeless,
	kX24TypelessG8Uint,
	kR8G8Typeless,
	kR8G8Unorm,
	kR8G8Uint,
	kR8G8Snorm,
	kR8G8Sint,
	kR16Typeless,
	kR16Float,
	kD16Unorm,
	kR16Unorm,
	kR16Uint,
	kR16Snorm,
	kR16Sint,
	kR8Typeless,
	kR8Unorm,
	kR8Uint,
	kR8Snorm,
	kR8Sint,
	kA8Unorm,
	kR1Unorm,
	kR9G9B9E5SharedExp,
	kR8G8B8G8Unorm,
	kG8R8G8B8Unorm,
	kBC1Typeless,
	kBC1Unorm,
	kBC1UnormSrgb,
	kBC2Typeless,
	kBC2Unorm,
	kBC2UnormSrgb,
	kBC3Typeless,
	kBC3Unorm,
	kBC3UnormSrgb,
	kBC4Typeless,
	kBC4Unorm,
	kBC4Snorm,
	kBC5Typeless,
	kBC5Unorm,
	kBC5Snorm,
	kB5G6R5Unorm,
	kB5G5R5A1Unorm,
	kB8G8R8A8Unorm,
	kB8G8R8X8Unorm,
	kR10G10B10XRBiasA2Unorm,
	kB8G8R8A8Typeless,
	kB8G8R8A8UnormSrgb,
	kB8G8R8X8Typeless,
	kB8G8R8X8UnormSrgb,
	kBC6HTypeless,
	kBC6HUF16,
	kBC6HSF16,
	kBC7Typeless,
	kBC7Unorm,
	kBC7UnormSrgb,
	//----
	kNum,
};

//---------------------------------------------------------------------------
//! @brief		画素数.
//---------------------------------------------------------------------------
struct ImageSize
{
	ImageSize() = default;
	ImageSize( u32 w, u32 h ) :
		width( w ), height( h ) {}

	u32 width;
	u32 height;
};

//---------------------------------------------------------------------------
//! @brief		矩形.
//---------------------------------------------------------------------------
struct Rect
{
	u32 x, y, w, h;
};

//---------------------------------------------------------------------------
//! @brief		矩形 : f32版.
//---------------------------------------------------------------------------
struct RectF
{
	f32 x, y, w, h;
};

//---------------------------------------------------------------------------
//! @brief		ピクセルフォーマット別ブロック単位バイトサイズ取得.
//---------------------------------------------------------------------------
void GetPixelBlockSize( ImageSize* outSize, PixelFormat format );

//---------------------------------------------------------------------------
//! @brief		ピクセルフォーマット別１テクセル当たりのビットサイズ取得.
//---------------------------------------------------------------------------
u32 GetBitsPerTextel( PixelFormat format );

//---------------------------------------------------------------------------
//!	@brief		サーフェイス情報.
//---------------------------------------------------------------------------
struct SurfaceInfo
{
	u32				bytes;			//!< 全体のバイトサイズ.
	u32				pitchBytes;		//!< イメージの1行バイトサイズ.
	u32				pitchCount;		//!< 行数(高さ).
	data::ImageSize	blockSize;		//!< ブロック単位のイメージサイズ.
};

//---------------------------------------------------------------------------
//!	@brief		サーフェス情報を作成.
//!
//!	@param[in]	outInfo	サーフェス情報格納先.
//!	@param[in]	w		イメージの幅.
//!	@param[in]	h		イメージの高さ.
//!	@param[in]	format	ピクセルフォーマット.
//---------------------------------------------------------------------------
 void CalcSurfaceInfo( SurfaceInfo* outInfo, s32 w, s32 h, PixelFormat format );

} // namespace data
} // namespace aroma
