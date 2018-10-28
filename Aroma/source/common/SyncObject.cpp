//===========================================================================
//!
//!	@file		SyncObject.cpp
//!	@brief		同期オブジェクト.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/common/SyncObject.h>

namespace aroma
{
//---------------------------------------------------------------------------
//! @brief スピンロック.
//---------------------------------------------------------------------------
SpinLockObject::SpinLockObject()
{
}

SpinLockObject::~SpinLockObject()
{
}

void SpinLockObject::Lock()
{
	while( m_obj.test_and_set( std::memory_order_acquire ) )
	{
		;	// Spin-lock.
	}
}

void SpinLockObject::Unlock()
{
	m_obj.clear( std::memory_order_release );
}

} // namespace aroma
