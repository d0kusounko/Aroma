//===========================================================================
//!
//!	@file		Singleton.cpp
//!	@brief		シングルトンパターン.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/util/Singleton.h>
#include <aroma/debug/Assert.h>

namespace aroma {
namespace util {

namespace
{
	// TODO: 外部で個数設定できるようにする.
	constexpr u32						kFinalizersMax		= 256;
	s32									gFinalizersNum		= 0;
	SingletonFinalizer::FinalizerFunc	gFinalizers[ kFinalizersMax ];
}

void SingletonFinalizer::AddFinalizer( FinalizerFunc func )
{
	AROMA_ASSERT( gFinalizersNum < kFinalizersMax, "Please Increase kFinalizersMax." );
	gFinalizers[ gFinalizersNum ] = func;
	gFinalizersNum++;
}

void SingletonFinalizer::Finalize()
{
	// 作成されたシングルトンの逆順に終了.
	for( s32 i = gFinalizersNum - 1; i >= 0; --i )
	{
		( gFinalizers[ i ] )();
	}
	gFinalizersNum		= 0;
}


} // namespace aroma
} // namespace util
