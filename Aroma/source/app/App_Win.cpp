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

namespace aroma {
namespace app {

namespace
{
bool g_isQuit = false;

void SetQuit( bool flag );
} // namespace

//---------------------------------------------------------------------------
//	メッセージループ処理.
//---------------------------------------------------------------------------
void ProcessMessage()
{
	if( IsQuit() ) return;

	MSG  msg;
	while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
	{
		PeekMessage( &msg, NULL, 0, 0, PM_REMOVE );
		if( msg.message == WM_QUIT )
		{
			SetQuit( true );
			break;
		}
		else
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}
}

//---------------------------------------------------------------------------
//!	@brief		アプリケーションの終了判定.
//---------------------------------------------------------------------------
bool IsQuit()
{
	return g_isQuit;
}

//---------------------------------------------------------------------------
//	処理の強制停止.
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


namespace
{
	void SetQuit( bool flag )
	{
		g_isQuit = flag;
	}
} // namespace

} // namespace app
} // namespace aroma

#endif