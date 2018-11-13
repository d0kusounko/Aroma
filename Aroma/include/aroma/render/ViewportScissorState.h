//===========================================================================
//!
//!	@file		ViewportScissorState.h
//!	@brief		ビューポートシザーステート.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include "RenderDef.h"

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//! @brief		ビューポートシザーステート.
//---------------------------------------------------------------------------
struct ViewportScissorState final
{
	Viewport	viewport[ kViewportsSlotMax ];
	ScissorRect	scissor[ kViewportsSlotMax ];

	//-------------------------------------------------------------------
	ViewportScissorState(){ Default(); }
	void Default()
	{
		for( u32 i = 0; i < kViewportsSlotMax; ++i )
		{
			viewport[ i ].Default();
			scissor[ i ].Default();
		}
	}

	//-----------------------------------------------------------------------
	//! @brief		各ステート設定.
	//! @param[in]	slot	設定するスロット.
	//! @param[in]	value	設定する値.
	//!	@retval		true	: 値が変更された.
	//! @retval		false	: 値が変更されなかった.
	//-----------------------------------------------------------------------
	bool Set( const ViewportScissorState& value );
	bool SetViewport( u32 slot, const Viewport& value );
	bool SetScissor( u32 slot, const ScissorRect& value );
};

} // namespace render
} // namespace aroma
