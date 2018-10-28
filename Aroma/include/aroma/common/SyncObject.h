//===========================================================================
//!
//!	@file		SyncObject.h
//!	@brief		同期オブジェクト.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include <atomic>

namespace aroma
{

//---------------------------------------------------------------------------
//! @brief		同期オブジェクトインターフェース.
//---------------------------------------------------------------------------
class ISyncObject
{
public:
	ISyncObject() = default;
	virtual ~ISyncObject() = default;
	virtual void Lock()		= 0;
	virtual void Unlock()	= 0;
};

//---------------------------------------------------------------------------
//! @brief		スピンロック.
//---------------------------------------------------------------------------
class SpinLockObject : public ISyncObject
{
public:
	SpinLockObject();
	virtual ~SpinLockObject() override;
	virtual void Lock() override;
	virtual void Unlock() override;

private:
	std::atomic_flag	m_obj;
};

} // namespace aroma
