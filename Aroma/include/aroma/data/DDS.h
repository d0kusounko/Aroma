//===========================================================================
//!
//!	@file		DDS.h
//!	@brief		DDS(Direct Draw Surface).
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include "DataDef.h"
#include "Color.h"

namespace aroma {
namespace data {

//-----------------------------------------------------------------------
//!	@name	定数.
//-----------------------------------------------------------------------
//! @{

//! DDS FourCC.
constexpr u32		kDDSMagicID			= FourCC< 'D','D','S',' ' >::value;

// ! FourCCを抜いたDDSヘッダーサイズ.
constexpr size_t	kDDSHeaderSize		= 124;

// ! DX10拡張DDSヘッダーサイズ.
constexpr size_t	kDDSHeaderDX10Size	= 20;

//! DDSピクセルフォーマットサイズ.
constexpr size_t	kDDSPixelFormatSize	= 32;

//---------------------------------------------------------------------------
//! @brief	DDSヘッダーフラグ.
//!
//! @details
//!		DDSHeader の dwFlags に使用されます.
//---------------------------------------------------------------------------
enum DDSHeaderFlag
{
	//! DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT を含んだ必須フラグ.
	kDDSHeaderFlagTexture	= 0x00001007U,

	//! DDSD_MIPMAPCOUNT
	//! ミップマップ有効.
	kDDSHeaderFlagMipMap	= 0x00020000U,

	//! DDSD_DEPTH
	//! ボリュームテクスチャ.
	kDDSHeaderFlagVolume	= 0x00800000U,

	//! DDSD_PITCH
	//! dwPitchOrLinearSizeがイメージの走査線あたりのバイト数を表す.
	kDDSHeaderFlagPitch		= 0x00000008U,

	//! DDSD_LINEARSIZE
	//! dwPitchOrLinearSizeがイメージの総バイト数を表す.
	kDDSHeaderFlagLinerSize	= 0x00080000U,
};

//---------------------------------------------------------------------------
//! @brief	DDSピクセルフォーマットフラグ.
//!
//! @details
//!		DDSPixelFormat の dwPfFlags に使用されます.
//---------------------------------------------------------------------------
enum DDSPixelFormatFlag
{
	//! DDPF_FOURCC
	//! 圧縮フォーマットを示す4文字コードを持つ.
	kDDSPixelFormatFlagFourCC		= 0x00000004U,

	//! DDPF_RGB
	//! RGBフォーマット.
	kDDSPixelFormatFlagRGB			= 0x00000040U,

	//! DDPF_RGB | DDPF_ALPHAPIXELS
	//! RGBAフォーマット.
	kDDSPixelFormatFlagRGBA			= 0x00000041U,

	//! DDPF_LUMINANCE
	//! 単一チャンネルのカラー非圧縮データ.
	//! dwRGBBitCountに輝度チャンネルのビット数が格納され,
	//! dwRBitMaskにチャンネルマスクが格納される.
	kDDSPixelFormatFlagLuminance	= 0x00020000U,

	//! DDPF_ALPHA
	//! アルファチャンネルの非圧縮データ.
	//! dwRGBBitCountにアルファチャンネルのビット数が格納され,
	//! dwABitMaskに有効なデータが格納される.
	kDDSPixelFormatFlagAlpha		= 0x00000002U,
};

//---------------------------------------------------------------------------
//! @brief	DDSサーフェイス詳細フラグ.
//!
//! @details
//!		DDSHeader の dwCaps に使用されます.
//---------------------------------------------------------------------------
enum DDSCapFlag
{
	//! DDSCAPS_TEXTURE
	//! 必須フラグ.
	kDDSCapFlagTexture	= 0x00001000U,

	//! DDSCAPS_COMPLEX | DDSCAPS_MIPMAP
	//! ミップマップが2個以上.
	kDDSCapFlagMipMap	= 0x00400008U,

	//! DDSCAPS_COMPLEX
	//! 複数サーフェイス.
	kDDSCapFlagCubeMap	= 0x00000008U,
};

//---------------------------------------------------------------------------
//! @brief	DDSキューブ環境マップ/ボリュームテクスチャ詳細フラグ.
//!
//! @details
//!		DDSHeader の dwCaps2 に使用されます.
//---------------------------------------------------------------------------
enum DDSCap2Flag
{
	//! DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
	//! キューブ環境マップの正のX面が有効.
	kDDSCap2FlagCubeMapPositiveX	= 0x00000600U,

	//! DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
	//! キューブ環境マップの負のX面が有効.
	kDDSCap2FlagCubeMapNegativeX	= 0x00000a00U,

	//! DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
	//! キューブ環境マップの正のY面が有効.
	kDDSCap2FlagCubeMapPositiveY	= 0x00001200U,

	//! DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
	//! キューブ環境マップの負のY面が有効.
	kDDSCap2FlagCubeMapNegativeY	= 0x00002200U,

	//! DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
	//! キューブ環境マップの正のZ面が有効.
	kDDSCap2FlagCubeMapPositiveZ	= 0x00004200U,

	//! DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ
	//! キューブ環境マップの負のZ面が有効.
	kDDSCap2FlagCubeMapNegativeZ	= 0x00008200U,

	//! キューブ環境マップの6つ全ての面が有効.
	kDDSCap2FlagCubeMapAllFaces		= kDDSCap2FlagCubeMapPositiveX |
									  kDDSCap2FlagCubeMapNegativeX |
									  kDDSCap2FlagCubeMapPositiveY |
									  kDDSCap2FlagCubeMapNegativeY |
									  kDDSCap2FlagCubeMapPositiveZ |
									  kDDSCap2FlagCubeMapNegativeZ,

	//! DDSCAPS2_VOLUME
	//! ボリュームテクスチャ.
	kDDSCap2FlagVolume		= 0x00200000U,
};

//---------------------------------------------------------------------------
//! @brief		DDSファイルのフォーマットID
//!
//! @details
//!		値は DirectX9 D3DFORMAT定数に準拠.
//---------------------------------------------------------------------------
enum class DDSDXGIFormat : u32
{
	kR8G8B8					= 20,
	kA8R8G8B8				= 21,
	kX8R8G8B8				= 22,
	kR5G6B5					= 23,
	kX1R5G5B5				= 24,
	kA1R5G5B5				= 25,
	kA4R4G4B4				= 26,
	kR3G3B2					= 27,
	kA8						= 28,
	kA8R3G3B2				= 29,
	kX4R4G4B4				= 30,
	kA2B10G10R10			= 31,
	kA8B8G8R8				= 32,
	kX8B8G8R8				= 33,
	kG16R16					= 34,
	kA2R10G10B10			= 35,
	kA16B16G16R16			= 36,

	kA8P8					= 40,
	kP8						= 41,

	kL8						= 50,
	kA8L8					= 51,
	kA4L4					= 52,

	kV8U8					= 60,
	kL6V5U5					= 61,
	kX8L8V8U8				= 62,
	kQ8W8V8U8				= 63,
	kV16U16					= 64,
	kA2W10V10U10			= 67,

	kD16_LOCKABLE			= 70,
	kD32					= 71,
	kD15S1					= 73,
	kD24S8					= 75,
	kD24X8					= 77,
	kD24X4S4				= 79,
	kD16					= 80,

	kD32F_LOCKABLE			= 82,
	kD24FS8					= 83,

	kD32_LOCKABLE			= 84,
	kS8_LOCKABLE			= 85,

	kL16					= 81,

	kVERTEXDATA				= 100,
	kINDEX16				= 101,
	kINDEX32				= 102,

	kQ16W16V16U16			= 110,


	kR16F					= 111,
	kG16R16F				= 112,
	kA16B16G16R16F			= 113,

	kR32F					= 114,
	kG32R32F				= 115,
	kA32B32G32R32F			= 116,

	kCxV8U8					= 117,

	kA1						= 118,
	kA2B10G10R10_XR_BIAS	= 119,
	kBINARYBUFFER			= 199,
};

//---------------------------------------------------------------------------
//! @brief		DDSファイルのフォーマットFourCC
//!
//! @details
//!		値は DirectX9 D3DFORMAT定数に準拠.
//---------------------------------------------------------------------------
enum class DDSFormatFourCC : u32
{
	kDX10					= FourCC< 'D','X','1','0' >::value,
	kDXT1					= FourCC< 'D','X','T','1' >::value,
	kDXT2					= FourCC< 'D','X','T','2' >::value,
	kDXT3					= FourCC< 'D','X','T','3' >::value,
	kDXT4					= FourCC< 'D','X','T','4' >::value,
	kDXT5					= FourCC< 'D','X','T','5' >::value,
	kR8G8_B8G8				= FourCC< 'R','G','B','G' >::value,
	kG8R8_G8B8				= FourCC< 'G','R','G','B' >::value,
	kYUY2					= FourCC< 'Y','U','Y','2' >::value,
	kUYVY					= FourCC< 'U','Y','V','Y' >::value,
	kMUTI2_ARGB8			= FourCC< 'M','E','T','1' >::value,
};

//---------------------------------------------------------------------------
//! @brief	DDSリソースタイプ.
//!
//! @details
//!		DDSHeaderDX10 の resourceType に使用されます.
//---------------------------------------------------------------------------
enum class DDSResourceType : u32
{
	kUnknown	= 0,
	kBuffer		= 1,
	kTexture1D	= 2,
	kTexture2D	= 3,
	kTexture3D	= 4,
};

//! @}

//---------------------------------------------------------------------------
//! @brief	DDSピクセルフォーマット.
//---------------------------------------------------------------------------
AROMA_ALIGN4_BEGIN struct DDSPixelFormat
{
	LE<u32>				dwPfSize;				//!< ピクセルフォーマット(当構造体)サイズ : 32Byte固定.
	LE<u32>				dwPfFlags;				//!< ピクセルフォーマットフラグ : DDSPixelFormatFlagの組み合わせ.
	LE<DDSFormatFourCC>	dwFourCC;				//!< 圧縮フォーマットの4文字のコード.
	LE<u32>				dwRGBBitCount;			//!< 1ピクセルのbit数.
	LE<u32>				dwRBitMask;				//!< RGBフォーマット時のRチャンネルマスク.
	LE<u32>				dwGBitMask;				//!< RGBフォーマット時のGチャンネルマスク.
	LE<u32>				dwBBitMask;				//!< RGBフォーマット時のBチャンネルマスク.
	LE<u32>				dwABitMask;				//!< RGBフォーマット時のAチャンネルマスク.

} AROMA_ALIGN4_END;

//---------------------------------------------------------------------------
//! @brief	DDSファイルヘッダー.
//---------------------------------------------------------------------------
AROMA_ALIGN4_BEGIN struct DDSHeader
{
	LE<u32>			dwMagic;				//!< マジック値 : 0x20534444 ' SDD'.
	LE<u32>			dwSize;					//!< dwMagicを除いた当ヘッダーのサイズ : 124Byte固定.
	LE<u32>			dwFlags;				//!< ヘッダーフラグ : DDSHeaderFlag の組み合わせ.
	LE<u32>			dwHeight;				//!< イメージの幅.
	LE<u32>			dwWidth;				//!< イメージの高さ.
	LE<u32>			dwPitchOrLinearSize;	//!< １ライン、または１面分のByteサイズ.
	LE<u32>			dwDepth;				//!< イメージの深度.
	LE<u32>			dwMipMapCount;			//!< ミップマップ数.
	LE<u32>			dwReserved1[11];		//!< 予約領域.
	DDSPixelFormat	dwPixelFormat;			//!< ピクセルフォーマット
	LE<u32>			dwCaps;					//!< サーフェスフラグ : DDSCapFlag の組み合わせ.
	LE<u32>			dwCaps2;				//!< キューブマップフラグ : DDSCap2Flag 組み合わせ.
	LE<u32>			dwCaps3;				//!< 未使用.
	LE<u32>			dwCaps4;				//!< 未使用.
	LE<u32>			dwReserved2;			//!< 予約領域.

} AROMA_ALIGN4_END;

//---------------------------------------------------------------------------
//! @brief	DDSファイルヘッダー DX10拡張分.
//---------------------------------------------------------------------------
AROMA_ALIGN4_BEGIN struct DDSHeaderDX10
{
	LE<DDSDXGIFormat>	dxgiFormat;		//!< DDSファイルフォーマットID.
	LE<DDSResourceType>	resourceType;	//!< リソースの種類.
	LE<u32>				miscFlags;		//!< 未使用.
	LE<u32>				arraySize;		//!< テクスチャ配列数.
	LE<u32>				miscFlags2;		//!< 未使用.
} AROMA_ALIGN4_END;

//---------------------------------------------------------------------------
//! @brief	DDSアクセスヘルパー.
//---------------------------------------------------------------------------
class DDSAccessor
{
public:
	DDSAccessor();

	//-----------------------------------------------------------------------
	//! @brief		DDSデータ設定コンストラクタ.
	//-----------------------------------------------------------------------
	explicit DDSAccessor( void* ddsData );

	//-----------------------------------------------------------------------
	//! @brief		DDSデータ設定.
	//-----------------------------------------------------------------------
	bool Set( void* ddsData );

	//-----------------------------------------------------------------------
	//! @brief		設定済みDDSデータ正規判定.
	//!
	//!	@retval	true	正常.
	//!	@retval	false	不正.
	//-----------------------------------------------------------------------
	bool IsValid() const;

	//-----------------------------------------------------------------------
	//!	@brief		DDSヘッダーを取得.
	//-----------------------------------------------------------------------
	const DDSHeader* GetHeader() const;

	//-----------------------------------------------------------------------
	//!	@brief		DX10拡張DDSヘッダーを取得.
	//-----------------------------------------------------------------------
	const DDSHeaderDX10* GetHeaderDX10() const;

	//-----------------------------------------------------------------------
	//!	@brief		イメージの幅取得.
	//-----------------------------------------------------------------------
	u32 GetWidth() const;

	//-----------------------------------------------------------------------
	//!	@brief		イメージの高さ取得.
	//-----------------------------------------------------------------------
	u32 GetHeight() const;

	//-----------------------------------------------------------------------
	//!	@brief		イメージ画素数取得.
	//-----------------------------------------------------------------------
	void GetImageSize( ImageSize* outImageSize ) const;

	//-----------------------------------------------------------------------
	//!	@brief		イメージデータの先頭アドレスを取得.
	//-----------------------------------------------------------------------
	const void* GetImageTop() const;

	//-----------------------------------------------------------------------
	//!	@brief		ピクセルフォーマット取得.
	//-----------------------------------------------------------------------
	PixelFormat GetPixelFormat() const;

	//-----------------------------------------------------------------------
	//!	@brief		ミップマップ数取得.
	//-----------------------------------------------------------------------
	u32 GetMipMapCount() const;

	//-----------------------------------------------------------------------
	//!	@brief		テクスチャ配列数取得.
	//-----------------------------------------------------------------------
	u32 GetArrayCount() const;

	//-----------------------------------------------------------------------
	//!	@brief		キューブ環境マップ判定.
	//-----------------------------------------------------------------------
	bool IsCubeMap() const;

	//-----------------------------------------------------------------------
	//!	@brief		ボリュームテクスチャ判定.
	//-----------------------------------------------------------------------
	bool IsVolumeTexture() const;

	//@}

private:
	const void*				_ddsData;
	const DDSHeader*		_ddsHeader;
	const DDSHeaderDX10*	_ddsHeaderDX10;
};

AROMA_STATIC_ASSERT(sizeof(DDSHeader) == kDDSHeaderSize + 4, _T( "DDSHeader size mismatch.\n" ) );
AROMA_STATIC_ASSERT(sizeof(DDSHeaderDX10) == kDDSHeaderDX10Size, _T( "DDSHeaderDXT10 size mismatch.\n") );
AROMA_STATIC_ASSERT(sizeof(DDSPixelFormat) == kDDSPixelFormatSize, _T( "DDSPixelFormat size mismatch.\n") );

} // namespace data
} // namespace aroma
