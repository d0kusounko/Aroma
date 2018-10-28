//===========================================================================
//!
//!	@file		DDS.cpp
//!	@brief		DDS(Direct Draw Surface).
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/data/DDS.h>

namespace aroma {
namespace data {


namespace
{
bool IsValidForNormal( const DDSHeader& ddsHeader );
bool IsValidForDX10( const DDSHeaderDX10& ddsHeaderDX10 );
PixelFormat ToAromaPixelFormat( DDSDXGIFormat dxgiFormat );
}

//---------------------------------------------------------------------------
//	コンストラクタ.
//---------------------------------------------------------------------------
DDSAccessor::DDSAccessor()
	: _ddsData( nullptr )
	, _ddsHeader( nullptr )
	, _ddsHeaderDX10( nullptr )
{
}

//---------------------------------------------------------------------------
//	DDSデータ設定コンストラクタ.
//---------------------------------------------------------------------------
DDSAccessor::DDSAccessor( void* ddsData )
{
	Set( ddsData );
}

//---------------------------------------------------------------------------
//	DDSデータ設定.
//---------------------------------------------------------------------------
bool DDSAccessor::Set( void* ddsData )
{
	_ddsData		= ddsData;
	_ddsHeader		= static_cast< const DDSHeader* >( _ddsData );
	_ddsHeaderDX10	= nullptr;

	if( !IsValidForNormal( *_ddsHeader ) )
	{
		// 不正データ.
		return false;
	}

	// DX10拡張チェック.
	if( ( _ddsHeader->dwPixelFormat.dwPfFlags & kDDSPixelFormatFlagFourCC ) &&
		( _ddsHeader->dwPixelFormat.dwFourCC == DDSFormatFourCC::kDX10 ) )
	{
		_ddsHeaderDX10 = reinterpret_cast< const DDSHeaderDX10* >(
			reinterpret_cast< uintptr >( _ddsHeader ) + kDDSHeaderDX10Size );
	}

	if( _ddsHeaderDX10 )
	{
		// DX10拡張ヘッダーの不正チェック.
		if( !IsValidForDX10( *_ddsHeaderDX10 ) )
		{
			return false;
		}
	}

	return true;
}

//---------------------------------------------------------------------------
//	設定済みDDSデータ正規判定.
//---------------------------------------------------------------------------
bool DDSAccessor::IsValid() const
{
	if( !GetHeader() ) return false;
	if( !IsValidForNormal( *GetHeader() ) ) return false;
	if( GetHeaderDX10() )
	{
		if( !IsValidForDX10( *GetHeaderDX10() ) ) return false;
	}
	return true;
}

//---------------------------------------------------------------------------
//	DDSヘッダーを取得.
//---------------------------------------------------------------------------
const DDSHeader* DDSAccessor::GetHeader() const
{
	return _ddsHeader;
}

//---------------------------------------------------------------------------
//	DX10拡張DDSヘッダーを取得.
//---------------------------------------------------------------------------
const DDSHeaderDX10* DDSAccessor::GetHeaderDX10() const
{
	return _ddsHeaderDX10;
}


//---------------------------------------------------------------------------
//	イメージの幅取得.
//---------------------------------------------------------------------------
u32 DDSAccessor::GetWidth() const
{
	return GetHeader()->dwWidth;
}

//---------------------------------------------------------------------------
//	イメージの高さ取得.
//---------------------------------------------------------------------------
u32 DDSAccessor::GetHeight() const
{
	return GetHeader()->dwHeight;
}

//---------------------------------------------------------------------------
//	イメージ画素数取得.
//---------------------------------------------------------------------------
void DDSAccessor::GetImageSize( ImageSize* outImageSize ) const
{
	AROMA_ASSERT( outImageSize, _T( "nullptr not allowed.\n" ) );

	outImageSize->width		= GetWidth();
	outImageSize->height	= GetHeight();
}

//-------------------------------------------------------------
//!	イメージデータの先頭アドレスを取得.
//-------------------------------------------------------------
const void*	DDSAccessor::GetImageTop() const
{
	uintptr	addr = reinterpret_cast< uintptr >( _ddsData );
	addr += sizeof( DDSHeader );
	addr += GetHeaderDX10() ? sizeof( DDSHeaderDX10 ) : 0;
	return reinterpret_cast< void* >( addr );
}

//-----------------------------------------------------------------------
//	ピクセルフォーマット取得.
//
//	TODO: 各種フォーマット対応.
//-----------------------------------------------------------------------
PixelFormat DDSAccessor::GetPixelFormat() const
{
	// DX10拡張有り.
	if( GetHeaderDX10() )
	{
		return ToAromaPixelFormat( GetHeaderDX10()->dxgiFormat );
	}

	// DX10拡張なし.
	PixelFormat	format	= PixelFormat::kUnknown;
	const auto& pf		= GetHeader()->dwPixelFormat;

	if( CheckFlags( pf.dwPfFlags, kDDSPixelFormatFlagFourCC ) )
	{
		// FourCCから取得.
		switch( pf.dwFourCC )
		{
		case DDSFormatFourCC::kDXT1:
			format	= PixelFormat::kBC1Unorm;
			break;
		case DDSFormatFourCC::kDXT2:
			format	= PixelFormat::kBC1Unorm;
			break;
		case DDSFormatFourCC::kDXT3:
			break;
		case DDSFormatFourCC::kDXT4:
			format	= PixelFormat::kBC2Unorm;
			break;
		case DDSFormatFourCC::kDXT5:
			format	= PixelFormat::kBC3Unorm;
			break;
		case DDSFormatFourCC::kR8G8_B8G8:
			break;
		case DDSFormatFourCC::kG8R8_G8B8:
			break;
		case DDSFormatFourCC::kYUY2:
			break;
		case DDSFormatFourCC::kUYVY:
			break;
		case DDSFormatFourCC::kMUTI2_ARGB8:
			break;
		}
	}
	else
	{
		// TODO: 作成.
	}

	AROMA_ASSERT( format != PixelFormat::kUnknown, _T( "Unsupport Format.\n" ) );
	return format;
}

//---------------------------------------------------------------------------
//	ミップマップ数取得.
//---------------------------------------------------------------------------
u32 DDSAccessor::GetMipMapCount() const
{
	u32 mipMapCount = GetHeader()->dwMipMapCount;
	if( mipMapCount <= 0 ) mipMapCount = 1;
	return mipMapCount;
}

//---------------------------------------------------------------------------
//	テクスチャ配列数取得.
//---------------------------------------------------------------------------
u32 DDSAccessor::GetArrayCount() const
{
	u32 arrayCount = 1;
	if( GetHeaderDX10() )
	{
		arrayCount = GetHeaderDX10()->arraySize;
	}
	return arrayCount;
}

//---------------------------------------------------------------------------
//	キューブ環境マップ判定.
//---------------------------------------------------------------------------
bool DDSAccessor::IsCubeMap() const
{
	// TODO: 作成.
	return false;
}
	
//---------------------------------------------------------------------------
//	ボリュームテクスチャ判定.
//---------------------------------------------------------------------------
bool DDSAccessor::IsVolumeTexture() const
{
	// TODO: 作成.
	return false;
}

namespace
{
//---------------------------------------------------------------------------
//	DDSデータ正規判定.
//---------------------------------------------------------------------------
bool IsValidForNormal( const DDSHeader& ddsHeader )
{
	// マジック値 FourCC.
	if( ddsHeader.dwMagic != kDDSMagicID )
	{
		return false;
	}

	// ヘッダーサイズ.
	if( ddsHeader.dwSize != kDDSHeaderSize )
	{
		return false;
	}

	// ピクセルフォーマットサイズ.
	if( ddsHeader.dwPixelFormat.dwPfSize != kDDSPixelFormatSize )
	{
		return false;
	}

	// 必須フラグ.
	if( !CheckFlags( ddsHeader.dwFlags, kDDSHeaderFlagTexture ) )
	{
		return false;
	}
	if( !CheckFlags( ddsHeader.dwCaps, kDDSCapFlagTexture ) )
	{
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------
//	DX10拡張DDSヘッダー部正規判定.
//---------------------------------------------------------------------------
bool IsValidForDX10( const DDSHeaderDX10& ddsHeaderDX10 )
{
	// TODO: 作成.
	return true;
}

//---------------------------------------------------------------------------
//	Aroma ピクセルフォーマットに変換.
//---------------------------------------------------------------------------
PixelFormat ToAromaPixelFormat( DDSDXGIFormat dxgiFormat )
{
	AROMA_ASSERT( false, _T( "Unsupport Format.\n" ) );
	return PixelFormat::kUnknown;
}


} // namespace

} // namespace data
} // namespace aroma
