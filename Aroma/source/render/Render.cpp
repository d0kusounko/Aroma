//===========================================================================
//!
//!	@file		Render.cpp
//!	@brief		描画システム.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/render/render.h>
#include <aroma/render/MemoryAllocator.h>

namespace aroma {
namespace render {

namespace
{
	bool		g_initialized;
	Desc		g_desc;
}

//---------------------------------------------------------------------------
//! @brief		描画システム初期化.
//---------------------------------------------------------------------------
void Initialize( const Desc& desc )
{
	if( g_initialized )
	{
		AROMA_ASSERT( false, "Already initialized.\n" );
		return;
	}
	AROMA_DEBUG_OUT( "[Aroma] Render system initialize.\n" );
	g_desc			= desc;

	// メモリアロケーター初期化.
	MemoryAllocatorInitialize( desc.allocator );

	g_initialized	= true;
}

//---------------------------------------------------------------------------
//! @brief		描画システム終了.
//---------------------------------------------------------------------------
void Finalize()
{
	if( !g_initialized ) return;

	AROMA_DEBUG_OUT( "[Aroma] Render system finalize.\n" );
	g_initialized	= false;
}

//---------------------------------------------------------------------------
//! @brief		デバッグモードの取得.
//---------------------------------------------------------------------------
DebugMode GetDebugMode()
{
	return g_desc.debugMode;
}


} // namespace render
} // namespace aroma