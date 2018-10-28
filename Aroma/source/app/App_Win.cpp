//===========================================================================
//!
//!	@file		App_Win.cpp
//!	@brief		アプリケーション関連 : Windows
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#ifdef AROMA_WINDOWS

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <aroma/app/App.h>
#include <aroma/debug/Debug.h>
#include <aroma/debug/Assert.h>

namespace aroma
{
namespace app
{

namespace
{
	// TODO: 仮のウィンドウプロシージャ. 外部より設定できるようにする.
	LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wp, LPARAM lp )
	{
		switch( msg )
		{
			case WM_CREATE:
			break;

		case WM_DESTROY:
			{ PostQuitMessage(0); }
			break;

		case WM_CLOSE:
			break;

		case WM_CHAR:
			break;

		case WM_SIZE:
			break;

		default:
			break;
		}
		return DefWindowProc(hWnd, msg, wp, lp);
	}

	//-----------------------------------------------------------------------
	//!	@brief ネイティブAPIウィンドウハンドルをAroma用に変換.
	//-----------------------------------------------------------------------
	WindowHandle ToAromaWindowHandle( HWND wHnd )
	{
		return static_cast< WindowHandle >( wHnd );
	}

} // namespace


//---------------------------------------------------------------------------
//!	@brief ウィンドウの作成.
//---------------------------------------------------------------------------
WindowHandle CreateAppWindow( const CreateAppWindowConfig& config )
{
	// インスタンスハンドルを取得.
	HINSTANCE hInst = GetModuleHandle( nullptr );
	if( !hInst )
	{
		AROMA_ASSERT( false, "Failed to get module handle." );
		return kWindowHandleInvalid;
	}

	// 拡張ウィンドウクラスの設定.
	WNDCLASSEX wc;
	wc.cbSize           = sizeof( WNDCLASSEX );
	wc.style            = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc      = WndProc;	// TODO: 外部より設定できるように変更.
	wc.cbClsExtra       = 0;
	wc.cbWndExtra       = 0;
	wc.hInstance        = hInst;
	wc.hIcon            = LoadIcon( hInst, IDI_APPLICATION );
	wc.hCursor          = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground    = (HBRUSH)( COLOR_WINDOW + 1 );
	wc.lpszMenuName     = NULL;
	wc.lpszClassName    = config.windowTitle;
	wc.hIconSm          = LoadIcon( hInst, IDI_APPLICATION );

	// ウィンドウクラスを登録します.
	if ( !RegisterClassEx( &wc ) )
	{
		AROMA_ASSERT( false, "Failed to Register window class." );
		return kWindowHandleInvalid;
	}

	// 矩形の設定.
	RECT rc = { 0, 0, static_cast<LONG>(config.size.width), static_cast<LONG>(config.size.height) };

	// 指定されたクライアント領域を確保するために必要なウィンドウ座標を計算します.
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME;
	AdjustWindowRect( &rc, style, FALSE );

	// ウィンドウ作成.
	HWND hWnd = CreateWindow(
		config.windowTitle,
		config.windowTitle,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		( rc.right - rc.left ),
		( rc.bottom - rc.top ),
		NULL,
		NULL,
		hInst,
		nullptr // TODO:
	);

	// 生成チェック.
	if ( !hWnd )
	{
		AROMA_ASSERT( false, "Failed to get module handle." );
		return kWindowHandleInvalid;
	}

	// ウィンドウ表示.
	ShowWindow( hWnd, SW_SHOWNORMAL );
	UpdateWindow( hWnd );

	// フォーカス設定.
	SetFocus( hWnd );

	return ToAromaWindowHandle( hWnd );
}

//---------------------------------------------------------------------------
//!	@brief ネイティブAPIウィンドウハンドルの取得.
//---------------------------------------------------------------------------
HWND ToNativeWindowHandle( WindowHandle wHnd )
{
	return static_cast< HWND >( wHnd );
}

//---------------------------------------------------------------------------
//!	@brief ウィンドウのタイトル設定.
//---------------------------------------------------------------------------
void SetWindowTitle( WindowHandle wHnd, CTStr title )
{
	auto nativeWindowHandle = ToNativeWindowHandle( wHnd );
	SetWindowText( nativeWindowHandle, title );
}

//---------------------------------------------------------------------------
//!	@brief 処理の強制停止.
//---------------------------------------------------------------------------
void Panic( CTStr file, u32 line, CTStr format, ... )
{
	// TODO: ダサい.
	va_list	vaStr;
	static char	tempStr[ 1024 ];

	va_start( vaStr, format );
	vsprintf_s( tempStr, format, vaStr );
	va_end( vaStr );

	AROMA_DEBUG_OUT( _T( "App Panic : file[%s] line[%d]\n%s\n" ), file, line, tempStr );
	assert( false );
}

} // namespace app
} // namespace aroma

#endif