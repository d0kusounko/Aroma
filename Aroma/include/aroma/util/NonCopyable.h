//===========================================================================
//!
//!	@file		NonCopyable.h
//!	@brief		コピー禁止テンプレート.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

namespace aroma {
namespace util {

//---------------------------------------------------------------------------
//!	@brief	コピー禁止クラス.
//!
//! @note	このクラスをprivate継承するとコピー不可なクラスを宣言することができます.
//! @code
//! class Foo : private NonCopyable< Foo >
//! {
//! };
//! @endcode
//---------------------------------------------------------------------------
template< typename T >
class NonCopyable
{
protected:
	NonCopyable() {}
	~NonCopyable() {}

private:
	NonCopyable( const NonCopyable& );
	NonCopyable& operator=( const NonCopyable& );
};

} // namespace util
} // namespace aroma
