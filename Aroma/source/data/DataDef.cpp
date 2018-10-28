//===========================================================================
//!
//!	@file		DataDef.cpp
//!	@brief		データフォーマット定義.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//!	@todo		TODO:数が多くなってきたらジャンル別に分離.
//!
//===========================================================================
#include <aroma/data/DataDef.h>
#include <aroma/common/Algorithm.h>

namespace aroma {
namespace data {

//---------------------------------------------------------------------------
//	ピクセルフォーマット別ブロック単位サイズ取得.
//---------------------------------------------------------------------------
void GetPixelBlockSize( ImageSize* outSize, PixelFormat format )
{
	AROMA_ASSERT( outSize, _T( "outSize is nullptr not allowed.\n" ) );

	outSize->width	= 1;
	outSize->height	= 1;

	switch( format ) {
	case PixelFormat::kR8G8B8G8Unorm:
	case PixelFormat::kG8R8G8B8Unorm:
		*outSize = ImageSize( 2, 1 );		// YUY2
		break;
	case PixelFormat::kBC1Typeless:
	case PixelFormat::kBC1Unorm:			// DXT1
	case PixelFormat::kBC1UnormSrgb:
	case PixelFormat::kBC2Typeless:
	case PixelFormat::kBC2Unorm:			// DXT3
	case PixelFormat::kBC2UnormSrgb:
	case PixelFormat::kBC3Typeless:
	case PixelFormat::kBC3Unorm:			// DXT5
	case PixelFormat::kBC3UnormSrgb:
	case PixelFormat::kBC4Typeless:
	case PixelFormat::kBC4Unorm:			// ATI1N/DXT5A
	case PixelFormat::kBC4Snorm:
	case PixelFormat::kBC5Typeless:
	case PixelFormat::kBC5Unorm:			// ATI2N/3Dc/DXN
	case PixelFormat::kBC5Snorm:
	case PixelFormat::kBC6HTypeless:
	case PixelFormat::kBC6HUF16:
	case PixelFormat::kBC6HSF16:
	case PixelFormat::kBC7Typeless:
	case PixelFormat::kBC7Unorm:
	case PixelFormat::kBC7UnormSrgb:
		// DXT圧縮形式は4x4単位で扱う.
		*outSize = ImageSize( 4, 4 );
		break;
	default:
		*outSize = ImageSize( 1, 1 );
		break;
	}
}

//---------------------------------------------------------------------------
//	ピクセルフォーマット別１テクセル当たりのビットサイズ取得.
//---------------------------------------------------------------------------
u32 GetBitsPerTextel( PixelFormat format )
{
	u32	bits = 1;
	switch( format )
	{
	case PixelFormat::kR1Unorm:
		bits = 1;
		break;

	case PixelFormat::kBC1Typeless:
	case PixelFormat::kBC1Unorm:
	case PixelFormat::kBC1UnormSrgb:
	case PixelFormat::kBC4Typeless:
	case PixelFormat::kBC4Unorm:
	case PixelFormat::kBC4Snorm:
		bits = 4;
		break;

	case PixelFormat::kR8Typeless:
	case PixelFormat::kR8Unorm:
	case PixelFormat::kR8Uint:
	case PixelFormat::kR8Snorm:
	case PixelFormat::kR8Sint:
	case PixelFormat::kA8Unorm:
	case PixelFormat::kBC2Typeless:
	case PixelFormat::kBC2Unorm:
	case PixelFormat::kBC2UnormSrgb:
	case PixelFormat::kBC3Typeless:
	case PixelFormat::kBC3Unorm:
	case PixelFormat::kBC3UnormSrgb:
	case PixelFormat::kBC5Typeless:
	case PixelFormat::kBC5Unorm:
	case PixelFormat::kBC5Snorm:
	case PixelFormat::kBC6HTypeless:
	case PixelFormat::kBC6HUF16:
	case PixelFormat::kBC6HSF16:
	case PixelFormat::kBC7Typeless:
	case PixelFormat::kBC7Unorm:
	case PixelFormat::kBC7UnormSrgb:
		bits = 8;
		break;

	case PixelFormat::kR8G8Typeless:
	case PixelFormat::kR8G8Unorm:
	case PixelFormat::kR8G8Uint:
	case PixelFormat::kR8G8Snorm:
	case PixelFormat::kR8G8Sint:
	case PixelFormat::kR16Typeless:
	case PixelFormat::kR16Float:
	case PixelFormat::kD16Unorm:
	case PixelFormat::kR16Unorm:
	case PixelFormat::kR16Uint:
	case PixelFormat::kR16Snorm:
	case PixelFormat::kR16Sint:
	case PixelFormat::kB5G6R5Unorm:
	case PixelFormat::kB5G5R5A1Unorm:
		bits = 16;
		break;

	case PixelFormat::kR10G10B10A2Typeless:
	case PixelFormat::kR10G10B10A2Unorm:
	case PixelFormat::kR10G10B10A2Uint:
	case PixelFormat::kR11G11B10Float:
	case PixelFormat::kR8G8B8A8Typeless:
	case PixelFormat::kR8G8B8A8Unorm:
	case PixelFormat::kR8G8B8A8UnormSrgb:
	case PixelFormat::kR8G8B8A8Uint:
	case PixelFormat::kR8G8B8A8Snorm:
	case PixelFormat::kR8G8B8A8Sint:
	case PixelFormat::kR16G16Typeless:
	case PixelFormat::kR16G16Float:
	case PixelFormat::kR16G16Unorm:
	case PixelFormat::kR16G16Uint:
	case PixelFormat::kR16G16Snorm:
	case PixelFormat::kR16G16Sint:
	case PixelFormat::kR32Typeless:
	case PixelFormat::kD32Float:
	case PixelFormat::kR32Float:
	case PixelFormat::kR32Uint:
	case PixelFormat::kR32Sint:
	case PixelFormat::kR24G8Typeless:
	case PixelFormat::kD24UnormS8Uint:
	case PixelFormat::kR24UnormX8Typeless:
	case PixelFormat::kX24TypelessG8Uint:
	case PixelFormat::kR9G9B9E5SharedExp:
	case PixelFormat::kR8G8B8G8Unorm:
	case PixelFormat::kG8R8G8B8Unorm:
	case PixelFormat::kB8G8R8A8Unorm:
	case PixelFormat::kB8G8R8X8Unorm:
	case PixelFormat::kR10G10B10XRBiasA2Unorm:
	case PixelFormat::kB8G8R8A8Typeless:
	case PixelFormat::kB8G8R8A8UnormSrgb:
	case PixelFormat::kB8G8R8X8Typeless:
	case PixelFormat::kB8G8R8X8UnormSrgb:
		bits = 32;
		break;

	case PixelFormat::kR16G16B16A16Typeless:
	case PixelFormat::kR16G16B16A16Float:
	case PixelFormat::kR16G16B16A16Unorm:
	case PixelFormat::kR16G16B16A16Uint:
	case PixelFormat::kR16G16B16A16Snorm:
	case PixelFormat::kR16G16B16A16Sint:
	case PixelFormat::kR32G32Typeless:
	case PixelFormat::kR32G32Float:
	case PixelFormat::kR32G32Uint:
	case PixelFormat::kR32G32Sint:
	case PixelFormat::kR32G8X24Typeless:
	case PixelFormat::kD32FloatS8X24Uint:
	case PixelFormat::kR32FloatX8X24Typeless:
	case PixelFormat::kX32TypelessG8X24Uint:
		bits = 64;
		break;

	case PixelFormat::kR32G32B32Typeless:
	case PixelFormat::kR32G32B32Float:
	case PixelFormat::kR32G32B32Uint:
	case PixelFormat::kR32G32B32Sint:
		bits = 96;
		break;

	case PixelFormat::kR32G32B32A32Typeless:
	case PixelFormat::kR32G32B32A32Float:
	case PixelFormat::kR32G32B32A32Uint:
	case PixelFormat::kR32G32B32A32Sint:
		bits = 128;
		break;

	default:
	case PixelFormat::kUnknown:
		bits = 0;
		break;
	}
	return bits;
}

//---------------------------------------------------------------------------
//	サーフェス情報を作成.
//---------------------------------------------------------------------------
void CalcSurfaceInfo( SurfaceInfo* outInfo, s32 w, s32 h, PixelFormat format )
{
	AROMA_ASSERT( outInfo, _T( "outInfo is nullptr not allowed.\n" ) );

	w = Max( w, 1 );
	h = Max( h, 1 );

	ImageSize	blockSize;
	GetPixelBlockSize( &blockSize, format );
	u32			bitPerTexel = GetBitsPerTextel( format );

	// ピクセルブロック境界に合わせたサイズ.
	u32	alignedWidth  = static_cast< u32 >( AlignUp( w, blockSize.width ) );
	u32	alignedHeight = static_cast< u32 >( AlignUp( h, blockSize.height ) );

	// サーフェス情報を作成.
	outInfo->blockSize	= blockSize;
	outInfo->pitchBytes	= alignedWidth  * blockSize.width * bitPerTexel / 8;
	outInfo->pitchCount	= alignedHeight / blockSize.height;
	outInfo->bytes		= outInfo->pitchBytes * outInfo->pitchCount;
}


} // namespace data
} // namespace aroma
