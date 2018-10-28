//===========================================================================
//!
//!	@file		FileIO.h
//!	@brief		ファイルI/O.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include "../common/RefObject.h"
#include "../util/NonCopyable.h"

namespace aroma {
namespace file {

//---------------------------------------------------------------------------
//!	@brief		ファイルオープンモードフラグ.
//---------------------------------------------------------------------------
enum OpenModeFlag : u32
{
	kOpenModeFlagRead	= 1ui32 << 0,	//!< 読み込み許可.
	kOpenModeFlagWrite	= 1ui32 << 1,	//!< 書き込み許可.
	// TODO: kOpenModeAppend	= 1ui32 << 2,	//!< 追記書き込み許可.
};

//---------------------------------------------------------------------------
//!	@brief		ファイル共有フラグ.
//!
//! @details
//!		ファイルオープン後に別プロセスにて,
//!		同ファイルにアクセスする際の共有許可フラグです.
//---------------------------------------------------------------------------
enum ShareFlag : u32
{
	kShareFlagDelete	= 1ui32 << 0,	//!< 削除許可.
	kShareFlagRead		= 1ui32 << 1,	//!< 読み込み許可.
	kShareFlagWrite		= 1ui32 << 2,	//!< 書き込み許可.
};

//---------------------------------------------------------------------------
//!	@brief		ファイル入出力操作クラス.
//---------------------------------------------------------------------------
class File final: public RefObject, private util::NonCopyable< File >
{
public:
	File();
	~File();

	//-----------------------------------------------------------------------
	//!	@brief		ファイルオープン.
	//!
	//! @param[in]	filePath	ファイルのパス.
	//! @param[in]	openflags	オープンモードフラグ(OpenModeFlagの組み合わせ).
	//! @param[in]	shareFlags	共有フラグ(ShareFlagの組み合わせ).
	//!
	//! @retval		true	: オープン成功.
	//!	@retval		false	: オープン失敗.
	//-----------------------------------------------------------------------
	bool Open( CTStr filePath, u32 openflags, u32 shareFlags );

	//-----------------------------------------------------------------------
	//!	@brief		ファイルクローズ.
	//!
	//! @details
	//!		デストラクタ時に自動的にコールされます.
	//-----------------------------------------------------------------------
	void Close();

	//-----------------------------------------------------------------------
	//!	@brief		ファイルサイズ取得.
	//!
	//!	@param[out]	outSize		ファイルサイズ格納先.
	//!
	//! @pre		ファイルがオープン状態.
	//!
	//! @retval		true	: ファイルサイズ取得成功.
	//! @retval		false	: ファイルサイズ取得失敗.
	//-----------------------------------------------------------------------
	bool GetFileSize( size_t* outSize );

	//-----------------------------------------------------------------------
	//!	@brief		ファイルデータ読み込み.
	//!
	//! @param[out]	outBuf		読み込みデータ格納先.
	//! @param[in]	readBytes	読み込み最大データバイトサイズ.
	//! @param[in]	readedBytes	読み込んだデータバイトサイズ格納先(nullptr指定可).
	//!
	//! @pre		ファイルがオープン状態かつ読み込み許可有り.
	//!
	//! @retval		true	: データ読み込み成功.
	//! @retval		false	: データ読み込み失敗.
	//-----------------------------------------------------------------------
	bool ReadFile( void* outBuf, size_t readBytes, size_t* readedBytes );

	//-----------------------------------------------------------------------
	//!	@brief		ファイルデータ書き込み.
	//!
	//! @param[in]	buf			書き込みデータバッファ.
	//! @param[in]	writeBytes	書き込みデータサイズ.
	//!
	//! @pre		ファイルがオープン状態かつ書き込み許可有り.
	//!
	//! @retval		true	: データ書き込み成功.
	//! @retval		false	: データ書き込み失敗.
	//-----------------------------------------------------------------------
	bool WriteFile( void* outBuf, size_t writeBytes );

private:
	bool	_opened;
	u32		_openFlags;
	u32		_shareFlags;
	TStr	_filePath;

#ifdef AROMA_WINDOWS
	HANDLE	_fileHandle;
#endif
};


} // namespace file
} // namespace aroma
