//===========================================================================
//!
//!	@file		BitFlag.h
//!	@brief		ビットフラグ.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include "../common/Typedef.h"

namespace aroma {
//---------------------------------------------------------------------------
//!	@brief	指定位置ビットが立った整数値を作成.
//---------------------------------------------------------------------------
static inline constexpr u32 Bit32( u32 n ){ return ( 1ui32 << n ); }
static inline constexpr u64 Bit64( u32 n ){ return ( 1ui64 << n ); }

//---------------------------------------------------------------------------
//!	@brief	ビットフラグをオン.
//---------------------------------------------------------------------------
static inline void OnFlags( u32& flags, u32 bits ) { flags |= bits; }
static inline void OnFlags( u64& flags, u64 bits ) { flags |= bits; }

//---------------------------------------------------------------------------
//!	@brief	ビットフラグをオフ.
//---------------------------------------------------------------------------
static inline void OffFlags( u32& flags, u32 bits ) { flags &= ~bits; }
static inline void OffFlags( u64& flags, u64 bits ) { flags &= ~bits; }

//---------------------------------------------------------------------------
//!	@brief	ビットフラグを切り替え.
//---------------------------------------------------------------------------
static inline void ToggleFlags( u32& flags, u32 bits ) { flags ^= bits; }
static inline void ToggleFlags( u64& flags, u64 bits ) { flags ^= bits; }

//---------------------------------------------------------------------------
//!	@brief	ビットフラグチェック.
//!
//! @retval	true	: フラグオン.
//! @retval	false	: フラグオフ.
//!
//! @details
//!		指定した集合フラグのビットが全部オンならチェックはtrueとなります.
//---------------------------------------------------------------------------
static inline bool CheckFlags( u32 inFlags, u32 flags )
{
	return ( inFlags & flags ) == flags;
}
static inline constexpr bool CheckFlags( u64 inFlags, u64 flags )
{
	return ( inFlags & flags ) == flags;
}

} // namespace aroma
