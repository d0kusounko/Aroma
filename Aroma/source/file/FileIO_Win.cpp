//===========================================================================
//!
//!	@file		FileIO.cpp
//! @brief		ファイルI/O : Windows.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#ifdef AROMA_WINDOWS

#include <aroma/file/FileIO.h>

namespace aroma {
namespace file {

//---------------------------------------------------------------------------
//	デフォルトコンストラクタ.
//---------------------------------------------------------------------------
File::File()
	: _opened( false )
	, _openFlags( 0 )
	, _shareFlags( 0 )
	, _filePath( nullptr )
	, _fileHandle( INVALID_HANDLE_VALUE )
{
}

//---------------------------------------------------------------------------
//	デストラクタ.
//---------------------------------------------------------------------------
File::~File()
{
	Close();
}

//---------------------------------------------------------------------------
//	ファイルオープン.
//---------------------------------------------------------------------------
bool File::Open( CTStr filePath, u32 openflags, u32 shareFlags )
{
	if( _opened )
	{
		AROMA_ASSERT( false, _T( "Already opened.\n" ) );
		Close();
	}
	_openFlags	= openflags;
	_shareFlags	= shareFlags;
	_filePath	= data::StrCopy( filePath );

	DWORD desiredAccess = 0;
	if( openflags & kOpenModeFlagRead )		desiredAccess |= GENERIC_READ;
	if( openflags & kOpenModeFlagWrite )	desiredAccess |= GENERIC_WRITE;

	DWORD shareMode = 0;
	if( shareFlags & kShareFlagDelete )		shareMode |= FILE_SHARE_DELETE;
	if( shareFlags & kShareFlagRead )		shareMode |= FILE_SHARE_READ;
	if( shareFlags & kShareFlagWrite )		shareMode |= FILE_SHARE_WRITE;

	DWORD creationDisposition = OPEN_EXISTING;
	if( openflags & kOpenModeFlagWrite ) creationDisposition = OPEN_ALWAYS;

	_fileHandle = CreateFile( filePath, desiredAccess, shareMode, nullptr, creationDisposition, FILE_FLAG_RANDOM_ACCESS, nullptr );
	if( _fileHandle == INVALID_HANDLE_VALUE )
	{
		// ファイルオープン失敗.
		return false;
	}

	_opened = true;
	return true;
}

//---------------------------------------------------------------------------
//	ファイルクローズ.
//---------------------------------------------------------------------------
void File::Close()
{
	if( !_opened ) return;

	BOOL result = CloseHandle( _fileHandle );
	AROMA_ASSERT( result, _T( "Failed to CloseHandle.\n" ) );
	_fileHandle = INVALID_HANDLE_VALUE;
	memory::SafeFree( _filePath );
	_opened = false;
}

//---------------------------------------------------------------------------
//	ファイルサイズ取得.
//---------------------------------------------------------------------------
bool File::GetFileSize( size_t* outSize )
{
	// Get file size
	LARGE_INTEGER fileSize;
	BOOL result = GetFileSizeEx( _fileHandle, &fileSize );
	if( result == FALSE )
	{
		// ファイルサイズ取得失敗.
		return false;
	}
	*outSize = static_cast< size_t >( fileSize.QuadPart );
	return true;
}

//---------------------------------------------------------------------------
//	ファイルデータ読み込み.
//---------------------------------------------------------------------------
bool File::ReadFile( void* outBuf, size_t readBytes, size_t* readedBytes )
{
	if( !( _openFlags & kOpenModeFlagRead ) )
	{
		AROMA_ASSERT( false, _T( "FileIO not READ mode.\n" ) );
		return false;
	}

	DWORD tempReadedBytes;
	BOOL result = ::ReadFile( _fileHandle, outBuf, static_cast< DWORD >( readBytes ), &tempReadedBytes, nullptr );
	if( result == FALSE )
	{
		// ファイルデータ読み込み失敗.
		return false;
	}

	// 読み取ったバイト数を格納.
	if( readedBytes )
	{
		(*readedBytes) = tempReadedBytes;
	}

	return true;
}

//---------------------------------------------------------------------------
//	ファイルデータ書き込み.
//---------------------------------------------------------------------------
bool File::WriteFile( void* outBuf, size_t writeBytes )
{
	// TODO: 実装.
	AROMA_ASSERT( false, _T( "Unimplemented.\n" ) );
	return true;
}


} // namespace file
} // namespace aroma

#endif

