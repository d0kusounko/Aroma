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
	//-----------------------------------------------------------------------
	//!	@brief		ビューポートシザーステート設定項目.
	//-----------------------------------------------------------------------
	enum class Setting
	{
		kViewport,	//!< Viewport
		kScissor,	//!< ScissorRect
	};

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

	//---------------------------------------------------------------------------
	//! @brief		ステート設定.
	//! @param[in]	value	設定する値.
	//! @param[in]	slot	設定するスロット.
	//!	@retval		true	: 値が変更された.
	//! @retval		false	: 値が変更されなかった.
	//---------------------------------------------------------------------------
	bool Set( u32 slot, Setting state, const Viewport& value );
	bool Set( u32 slot, Setting state, const ScissorRect& value );
};

} // namespace render
} // namespace aroma
