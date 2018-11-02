//===========================================================================
//!
//!	@file		RefObject.cpp
//!	@brief		参照カウンター付きベースオブジェクト.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/common/RefObject.h>

namespace aroma
{
//---------------------------------------------------------------------------
//	コンストラクタ.
//---------------------------------------------------------------------------
RefObject::RefObject()
	: _refCount(1)
{
#ifdef AROMA_DEBUG
	RefObjectManager::AddObj( this );
#endif
}

//---------------------------------------------------------------------------
//	デストラクタ.
//---------------------------------------------------------------------------
RefObject::~RefObject()
{
#ifdef AROMA_DEBUG
	RefObjectManager::DelObj( this );
#endif
}

//-----------------------------------------------------------------------
//!	@brief		解放（参照カウンターを1減少).
//!
//!	@return		減少後の参照カウント値.
//! @note		カウンターが0になったら解放処理を行います.
//-----------------------------------------------------------------------
s32	RefObject::Release()
{
	s32	refCountOld;
	s32	refCount;
	do {
		refCountOld = _refCount.load();
		refCount    = refCountOld - 1;
	} while( !_refCount.compare_exchange_weak( refCountOld, refCount ) );

	AROMA_ASSERT( refCount >= 0, _T( "Reference counter is invalid." ) );

	if( refCount <= 0 )
	{
		delete this;
	}
	return refCount;
}

//---------------------------------------------------------------------------
//!	@brief		参照カウンタを1増加.
//!
//!	@return		増加後の参照カウント値.
//---------------------------------------------------------------------------
s32 RefObject::AddRef()
{
	s32 refCountOld;
	s32 refCount;
	do {
		refCountOld = _refCount.load();
		refCount    = refCountOld + 1;
	} while( !_refCount.compare_exchange_weak( refCountOld, refCount ) );

	return refCount;
}

//-----------------------------------------------------------------------
//!	@brief		参照カウントを取得.
//!
//!	@return		参照カウント数.
//-----------------------------------------------------------------------
s32 RefObject::GetCount() const
{
	return _refCount.load();
}

#ifdef AROMA_DEBUG
//===========================================================================
//	RefObjectManager
//===========================================================================
std::list< RefObject* > RefObjectManager::_objs;

//---------------------------------------------------------------------------
//	全RefObjectインスタンスの参照カウントをデバッグ出力.
//---------------------------------------------------------------------------
void RefObjectManager::Dump()
{
	for( const auto& obj : _objs )
	{
		AROMA_DEBUG_OUT( "[RefObject] %p : %d Reference\n", obj, obj->GetCount() );
	}
}

//---------------------------------------------------------------------------
//	オブジェクトリストに追加.
//---------------------------------------------------------------------------
void RefObjectManager::AddObj( RefObject* obj )
{
	_objs.push_back( obj );
}

//---------------------------------------------------------------------------
//	オブジェクトリストから削除.
//---------------------------------------------------------------------------
void RefObjectManager::DelObj( RefObject* obj )
{
	_objs.remove( obj );
}
#endif

} // namespace aroma
