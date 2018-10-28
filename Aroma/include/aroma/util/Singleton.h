//===========================================================================
//!
//!	@file		Singleton.h
//!	@brief		シングルトンパターン.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include <functional>
#include <mutex>

namespace aroma {
namespace util {

//---------------------------------------------------------------------------
//!	@brief	シングルトン終了モジュール.
//!
//! @note	システムの終了前にFinalize()を実行して,
//!			全シングルトンモジュールを終了して下さい.
//---------------------------------------------------------------------------
class SingletonFinalizer
{
public:
	using FinalizerFunc = std::function< void() >;

	static void AddFinalizer( FinalizerFunc func );
	static void Finalize();
};

//---------------------------------------------------------------------------
//!	@brief	シングルトンテンプレートクラス.
//!
//! @note	シングルトンにしたいクラスをテンプレートとしてSingletonからアクセスします.
//!
//! @code
//! Foo& foo = aroma::util::Singleton<Foo>::GetInstance();
//! @endcode
//---------------------------------------------------------------------------
template< typename T >
class Singleton final
{
public:
	static T& GetInstance()
	{
		std::call_once( initFlag, Create );
		AROMA_ASSERT( instance, _T( "Instance has not been created yet\n." ) );
		return *instance;
	}

private:
	static void Create()
	{
		instance = new T;
		SingletonFinalizer::AddFinalizer( &Singleton< T >::Destroy );
	}

	static void Destroy()
	{
		delete instance;
		instance = nullptr;
	}

	static std::once_flag initFlag;
	static T* instance;
};

template< typename T > std::once_flag Singleton<T>::initFlag;
template< typename T > T* Singleton<T>::instance = nullptr;


} // namespace aroma
} // namespace util
