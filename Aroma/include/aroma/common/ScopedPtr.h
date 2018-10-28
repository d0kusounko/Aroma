//===========================================================================
//!
//!	@file		ScopedPtr.h
//!	@brief		スコープ内ポインター.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include "../util/NonCopyable.h"

namespace aroma {
//---------------------------------------------------------------------------
//!	@brief	スコープ内ポインタークラス.
//!
//! @note	動的に確保されたオブジェクトのポインタを格納します.
//!			格納されたオブジェクトはスコープを外れるかReset()を呼び出すことで破棄されます.
//!
//! @code
//!	class Foo
//!	{
//!	private:
//!		class Impl;
//!		ScopedPtr< Impl > m_pimpl;
//!	};
//! @endcode
//---------------------------------------------------------------------------
template< typename T >
class ScopedPtr : private util::NonCopyable< ScopedPtr< T > >
{
	T* ptr;

public:
	explicit ScopedPtr( T* p = nullptr )
	{
		ptr = p;
	}
	~ScopedPtr() {
		if ( ptr != nullptr )
		{
			delete ptr;
		}
	}

	void Reset( T* p = nullptr )
	{
		AROMA_ASSERT( ( p == nullptr ) || ( p != ptr ), _T( "Same pointer is not allowed." ) );

		if ( ptr != nullptr ) {
			delete ptr;
		}
		ptr = p;
	}

	T* Get() const {
		return ptr;
	}

	T& operator*() const
	{
		return *ptr;
	}
	T* operator->() const
	{
		return ptr;
	}
	bool operator==( const T* p ) const
	{
		return ptr == p;
	}
	bool operator!=( const T* p ) const
	{
		return ptr != p;
	}
};

} // namespace aroma
