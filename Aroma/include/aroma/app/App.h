//===========================================================================
//!
//!	@file		App.h
//!	@brief		アプリケーション関連.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//! @note		ここに存在する要素は肥大化したら細分化するかも.
//!
//===========================================================================
#pragma once

#include "../data/DataDef.h"

namespace aroma {
namespace app {

//! CreateAppWindow用ウィンドウ作成設定.
struct CreateAppWindowConfig
{
	CreateAppWindowConfig()
		: windowTitle( nullptr )
	{
	}

	CTStr			windowTitle;
	data::ImageSize	size;
};

//! ウィンドウハンドル.
using WindowHandle = void*;

//! 無効なウィンドウハンドル.
constexpr WindowHandle kWindowHandleInvalid = nullptr;

//---------------------------------------------------------------------------
//!	@brief ウィンドウの作成.
//---------------------------------------------------------------------------
WindowHandle CreateAppWindow( const CreateAppWindowConfig& config );

//---------------------------------------------------------------------------
//!	@brief ネイティブAPIウィンドウハンドルの取得.
//---------------------------------------------------------------------------
#ifdef AROMA_WINDOWS
HWND ToNativeWindowHandle( WindowHandle wHnd );
#endif

//---------------------------------------------------------------------------
//!	@brief ウィンドウのタイトル設定.
//---------------------------------------------------------------------------
void SetWindowTitle( WindowHandle wHnd, CTStr title );

//---------------------------------------------------------------------------
//!	@brief アプリケーションの強制停止.
//---------------------------------------------------------------------------
void Panic( CTStr file, u32 line, CTStr msg, ... );

} // namespace app
} // namespace aroma