////===========================================================================
//!
//!	@file		Window_Win.cpp
//!	@brief		アプリケーションウィンドウ : Windows.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//=============================================================================
#ifdef AROMA_WINDOWS

#include <aroma/app/Window.h>

namespace aroma {
namespace app {

WindowManager::WindowMap WindowManager::windowMap;

namespace
{
//---------------------------------------------------------------------------
//	ウィンドウメッセージプロシージャ.
//---------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wp, LPARAM lp )
{
	auto window = WindowManager::GetWindow( hWnd );
	switch( msg )
	{
		// M2_TODO: ウィンドウ作成時.
		case WM_CREATE:
		{
			break;
		}

		// 消去時.
		case WM_DESTROY:
		{
			if( window )
			{
				auto callback = window->GetMessageCallback( WindowMessage::kDestroy );
				if( callback )
				{
					// コールバック.
					callback->func( nullptr, callback->userParam );
				}

				if( CheckFlags( window->GetFlags(), kWindowFlagDestroyPostQuit ) )
				{
					// アプリ終了メッセージを送出.
					PostQuitMessage( 0 );
				}
			}
			break;
		}

		// 閉じようとした.
		case WM_CLOSE:
		{
			if( window )
			{
				auto callback = window->GetMessageCallback( WindowMessage::kClose );
				if( callback )
				{
					// コールバック.
					callback->func( nullptr, callback->userParam );
				}
			
				if( !CheckFlags( window->GetFlags(), kWindowFlagCloseMessageNotDestory ) )
				{
					// ウィンドウを削除.
					window->Destory();
				}
			}
			return 0;	// 自動的に削除しないようにするためreturn
		}
		
		// 座標移動時.
		case WM_MOVE:
		{
			if( window )
			{
				auto callback = window->GetMessageCallback( WindowMessage::kMove );
				if( callback )
				{
					// 固有パラメータ作成.
					WindowMessageCallbackParamMove param;
					param.x	= lp & 0xFFFF;			// 移動後座標X.
					param.y	= (lp >> 16) & 0xFFFF;	// 移動後座標Y.

					// コールバック.
					callback->func( &param, callback->userParam );
				}
			}
			break;
		}

		// サイズ変更.
		case WM_SIZE:
		{
			if( window )
			{
				auto callback = window->GetMessageCallback( WindowMessage::kSize );
				if( callback )
				{
					// 固有パラメータ作成.
					WindowMessageCallbackParamSize param;

					switch( wp )	// サイズ変更のタイプ.
					{
					case SIZE_MINIMIZED:
						param.action	= WindowSizeAction::kMinimized;
						break;
					case SIZE_MAXIMIZED:
						param.action	= WindowSizeAction::kMaximized;
						break;

					// MEMO: SIZE_MAXSHOW, SIZE_MAXHIDE の来る条件がよくわからないので,
					//		 もし来た場合は通常処理としてますが, 必要になった場合は,
					//		 WindowSizeAction にアクションを追加してちゃんと対応して下さい.
					default:
						param.action	= WindowSizeAction::kNormal;
						break;
					}
					param.size.width	= lp & 0xFFFF;			// 変更後Xサイズ.
					param.size.height	= (lp >> 16) & 0xFFFF;	// 変更後Yサイズ.

					// コールバック.
					callback->func( &param, callback->userParam );
				}
			}
			break;
		}
	}
	return DefWindowProc( hWnd, msg, wp, lp );
}
} // namespace

//---------------------------------------------------------------------------
//	ウィンドウの作成.
//---------------------------------------------------------------------------
Window* Window::Create( const CreateConfig& config )
{
	// インスタンスハンドル取得.
	HINSTANCE hInst = GetModuleHandle( nullptr );
	if( !hInst )
	{
		AROMA_ASSERT( false, "Failed to get Instance handle." );
		return nullptr;
	}

	// ウィンドウクラス登録.
	{
		WNDCLASSEX wcDesc;
		wcDesc.cbSize           = sizeof( WNDCLASSEX );
		wcDesc.style            = CS_HREDRAW | CS_VREDRAW;
		wcDesc.lpfnWndProc      = WndProc;
		wcDesc.cbClsExtra       = 0;
		wcDesc.cbWndExtra       = 0;
		wcDesc.hInstance        = hInst;
		wcDesc.hIcon            = LoadIcon( hInst, IDI_APPLICATION );
		wcDesc.hCursor          = LoadCursor( NULL, IDC_ARROW );
		wcDesc.hbrBackground    = (HBRUSH)( COLOR_WINDOW + 1 );
		wcDesc.lpszMenuName     = NULL;
		wcDesc.lpszClassName    = config.windowTitle;
		wcDesc.hIconSm          = LoadIcon( hInst, IDI_APPLICATION );

		if ( !RegisterClassEx( &wcDesc ) )
		{
			AROMA_ASSERT( false, "Failed to Register window class." );
			return nullptr;
		}
	}

	// 矩形の設定.
	RECT rc = { 0, 0, static_cast<LONG>(config.size.width), static_cast<LONG>(config.size.height) };

	// 指定されたクライアント領域を確保するために必要なウィンドウ座標を計算.
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
		nullptr
	);

	// 生成チェック.
	if ( !hWnd )
	{
		AROMA_ASSERT( false, "Failed to get module handle." );
		return nullptr;
	}
	
	// ウィンドウクラス作成.
	Window* window = new Window( hWnd );
	{
		// パラメータ保存.
		for( u32 i = 0; i < ( u32 )WindowMessage::kNum; ++i )
		{
			window->_callbacks[ i ] = config.callbacks[ i ];
		}
		window->_flags = config.flags;
	}

	// ウィンドウ表示.
	ShowWindow( hWnd, SW_SHOWNORMAL );
	SetFocus( hWnd );
	// UpdateWindow( hWnd );

	return window;
}

//---------------------------------------------------------------------------
//	コンストラクタ.
//---------------------------------------------------------------------------
Window::Window( HWND wHnd )
	: _nativeWindowHandle( wHnd )
	, _flags( 0 )
{
	// ウィンドウマネージャーに登録.
	WindowManager::RegisterWindow( _nativeWindowHandle, this );
}

//---------------------------------------------------------------------------
//	デストラクタ.
//---------------------------------------------------------------------------
Window::~Window()
{
	Destory();
}

//---------------------------------------------------------------------------
//	ウィンドウを削除.
//---------------------------------------------------------------------------
void Window::Destory()
{
	if( _nativeWindowHandle )
	{
		DestroyWindow( _nativeWindowHandle );
		UpdateWindow( _nativeWindowHandle );

		// ウィンドウマネージャーから解除.
		WindowManager::UnregisterWindow( _nativeWindowHandle );

		_nativeWindowHandle = NULL;
	}
}

//---------------------------------------------------------------------------
//	ウィンドウのタイトル設定.
//---------------------------------------------------------------------------
void Window::SetWindowTitle( CTStr title )
{
	SetWindowText( _nativeWindowHandle, title );
}

//---------------------------------------------------------------------------
//	ウィンドウメッセージ別コールバック取得.
//---------------------------------------------------------------------------
const WindowMessageCallback* Window::GetMessageCallback( WindowMessage msg ) const
{
	auto* callback = &_callbacks[ static_cast< u32 >( msg ) ];
	if( callback->func )
	{
		return callback;
	}
	return nullptr;
}

//---------------------------------------------------------------------------
//	ウィンドウコールバック設定フラグ取得.
//---------------------------------------------------------------------------
u32 Window::GetFlags() const
{
	return _flags;
}

//---------------------------------------------------------------------------
//	ネイティブAPIウィンドウハンドルの取得.
//---------------------------------------------------------------------------
HWND Window::GetNativeHandle()
{
	return _nativeWindowHandle;
}

//---------------------------------------------------------------------------
//	ウィンドウマネージャーにウィンドウ登録.
//---------------------------------------------------------------------------
void WindowManager::RegisterWindow( HWND wHnd, Window* window )
{
	AROMA_ASSERT( windowMap.count( wHnd ) == 0, _T( "Duplicate window handles.\n" ) );
	AROMA_ASSERT( window, _T( "Window must not be nullptr.\n" ) );
	windowMap[ wHnd ]  = window;
}

//---------------------------------------------------------------------------
//	ウィンドウマネージャーに登録されたウィンドウ解除.
//---------------------------------------------------------------------------
void WindowManager::UnregisterWindow( HWND wHnd )
{
	windowMap.erase( wHnd );
}

//---------------------------------------------------------------------------
//	ウィンドウマネージャーに登録されたウィンドウの取得.
//---------------------------------------------------------------------------
Window* WindowManager::GetWindow( HWND wHnd )
{
	auto it = windowMap.find( wHnd );
	if( it != windowMap.end() )
	{
		return it->second;
	}
	return nullptr;
}

} // namespace app
} // namespace aroma

#endif