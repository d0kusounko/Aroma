//===========================================================================
//!
//!	@file		Window.h
//!	@brief		アプリケーションウィンドウ.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include "../common/RefObject.h"
#include "../util/NonCopyable.h"
#include <functional>
#include <unordered_map>

namespace aroma {
namespace app {

class WindowManager;

//---------------------------------------------------------------------------
//! @brief	ウィンドウメッセージコールバック関数.
//!
//! @details
//!		Windowクラスが WindowMessage で定義されたメッセージを受け取ったときに,
//!		コールバックされる関数の定義です.
//!		引数 messageParam はWindowMessage別の固有パラメータが格納され,
//!		パラメータが無いときは nullptr となります.
//!		引数 userParam は WindowProcCallback の userParam に設定した任意データが格納されます.
//---------------------------------------------------------------------------
using WindowMessageCallbackFunc  = std::function< void( const void* messageParam, void* userParam ) >;

//---------------------------------------------------------------------------
//!	@brief		ウィンドウメッセージ.
//---------------------------------------------------------------------------
enum class WindowMessage
{
	kDestroy,	//!< ウィンドウ消去.
	kClose,		//!< 閉じようとした.
	kSize,		//!< サイズ変更.
	kNum,
};

//---------------------------------------------------------------------------
//!	@brief		ウィンドウサイズ変更アクション.
//---------------------------------------------------------------------------
enum class WindowSizeAction
{
	kNormal,	//!< 通常サイズ変更された.
	kMinimized,	//!< 最小化された.
	kMaximized,	//!< 最大化された.
	kNum,
};

//---------------------------------------------------------------------------
//!	@brief		ウィンドウ設定フラグ.
//---------------------------------------------------------------------------
enum WindowFlag : u32
{
	//! ウィンドウ消去時にアプリ終了のメッセージを送出.
	kWindowFlagDestroyPostQuit			= Bit32( 0 ),

	//! 閉じるメッセージを受信してもウィンドウを消去しない.
	kWindowFlagCloseMessageNotDestory	= Bit32( 1 ),
};

//---------------------------------------------------------------------------
//!	@brief		ウィンドウメッセージコールバック.
//---------------------------------------------------------------------------
struct WindowMessageCallback
{
	WindowMessageCallbackFunc	func;		//!< コールバック関数.
	void*						userParam;	//!< コールバック時に呼ばれる任意データ.

	WindowMessageCallback& operator=( const WindowMessageCallback& rhs )
	{
		func		= rhs.func;
		userParam	= rhs.userParam;
		return *this;
	}
};


//---------------------------------------------------------------------------
//!	@name		ウィンドウメッセージ別コールバックパラメータ.
//---------------------------------------------------------------------------
// @{
//! サイズ変更時パラメータ.
struct WindowMessageCallbackParamSize
{
	WindowSizeAction	action;	//!< サイズ変更アクション.
	data::ImageSize		size;	//!< 変更後サイズ.
};
// @}

//---------------------------------------------------------------------------
//!	@brief		アプリケーションウィンドウクラス.
//---------------------------------------------------------------------------
class Window final : public RefObject, private util::NonCopyable< Window >
{
	friend WindowManager;

public:
	//-----------------------------------------------------------------------
	//!	@brief		ウィンドウ作成設定.
	//!
	//! @param		windowTitle	ウィンドウタイトル.
	//! @param		size		ウィンドウサイズ.
	//! @param		callbacks	メッセージコールバック.
	//! @param		flags		設定フラグ : WindowFlag の組み合わせ.
	//-----------------------------------------------------------------------
	struct CreateConfig
	{
		CTStr					windowTitle;
		data::ImageSize			size;
		WindowMessageCallback	callbacks[ ( u32 )( WindowMessage::kNum ) ];
		u32						flags;
		//--------------------------
		CreateConfig()
			: windowTitle( nullptr )
			, flags( 0 )
		{
			for( auto& callback : callbacks )
			{
				callback.func		= nullptr;
				callback.userParam	= nullptr;
			}
		}
	};

	//-----------------------------------------------------------------------
	//!	@brief		ウィンドウの作成.
	//-----------------------------------------------------------------------
	static Window* Create( const CreateConfig& config );
	
	//---------------------------------------------------------------------------
	//!	@brief		ウィンドウを削除.
	//---------------------------------------------------------------------------
	void Destory();

	//---------------------------------------------------------------------------
	//!	@brief		ウィンドウのタイトル設定.
	//---------------------------------------------------------------------------
	void SetWindowTitle( CTStr title );

	//---------------------------------------------------------------------------
	//!	@brief		ウィンドウメッセージ別コールバック取得.
	//---------------------------------------------------------------------------
	const WindowMessageCallback* GetMessageCallback( WindowMessage msg ) const;

	//---------------------------------------------------------------------------
	//!	@brief		ウィンドウ設定フラグ取得.
	//---------------------------------------------------------------------------
	u32 GetFlags() const;

	//---------------------------------------------------------------------------
	//!	@brief		ネイティブAPIウィンドウハンドルの取得.
	//---------------------------------------------------------------------------
#ifdef AROMA_WINDOWS
	HWND GetNativeHandle();
#endif

private:
	Window();
#ifdef AROMA_WINDOWS
	Window( HWND wHnd );
#endif
	~Window();

#ifdef AROMA_WINDOWS
	HWND _nativeWindowHandle;
#endif
	WindowMessageCallback	_callbacks[ static_cast< u32 >( WindowMessage::kNum ) ];
	u32						_flags;
};

class WindowManager final : private util::NonCopyable< Window >
{
	friend Window;
public:
#ifdef AROMA_WINDOWS
	static Window* GetWindow( HWND wHnd );
#endif

private:
#ifdef AROMA_WINDOWS
	static void RegisterWindow( HWND wHnd, Window* window );
	static void UnregisterWindow( HWND wHnd );

	using WindowMap = std::unordered_map< HWND, Window* >;
#endif
	static WindowMap windowMap;
};

} // namespace app
} // namespace aroma