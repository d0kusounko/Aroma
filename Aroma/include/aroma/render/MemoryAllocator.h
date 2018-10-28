//===========================================================================
//!
//!	@file		MemoryAllocator.h
//!	@brief		描画システム用メモリアロケーター.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include "../memory/Allocator.h"

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//! @brief		メモリアロケーター構成設定.
//---------------------------------------------------------------------------
struct MemoryAllocatorDesc
{
	memory::IAllocator*	cpuMemAllocator;	//!< CPUメモリアロケーター.
	memory::IAllocator*	gpuMemAllocator;	//!< GPUメモリアロケーター.
	//-------------------------------------------------------------------
	MemoryAllocatorDesc(){ Clear(); }
	void Clear()
	{
		cpuMemAllocator	= nullptr;
		gpuMemAllocator = nullptr;
	}
};

//---------------------------------------------------------------------------
//! @brief		CPUメモリ確保.
//!
//! @pre		MemoryAllocatorInitialize実行済み.
//---------------------------------------------------------------------------
void* CpuMemAlloc( size_t size, size_t alignment );

//---------------------------------------------------------------------------
//! @brief		CPUメモリ再確保.
//!
//! @pre		MemoryAllocatorInitialize実行済み.
//---------------------------------------------------------------------------
void* CpuMemRealloc( void* addr, size_t newSize );

//---------------------------------------------------------------------------
//! @brief		CPUメモリ解放.
//!
//! @pre		MemoryAllocatorInitialize実行済み.
//---------------------------------------------------------------------------
void  CpuMemFree( void* addr );

//---------------------------------------------------------------------------
//! @brief		GPUメモリ確保.
//!
//! @pre		MemoryAllocatorInitialize実行済み.
//---------------------------------------------------------------------------
void* GpuMemAlloc( size_t size, size_t alignment );

//---------------------------------------------------------------------------
//! @brief		GPUメモリ再確保.
//!
//! @pre		MemoryAllocatorInitialize実行済み.
//---------------------------------------------------------------------------
void* GpuMemRealloc( void* addr, size_t newSize );

//---------------------------------------------------------------------------
//! @brief		GPUメモリ解放.
//!
//! @pre		MemoryAllocatorInitialize実行済み.
//---------------------------------------------------------------------------
void  GpuMemFree( void* addr );

//---------------------------------------------------------------------------
//! @brief		メモリアロケーター初期化.
//!
//!	@note		aroma::render::Initialize内で実行されるため,
//!				基本的にコールする必要はありません.
//---------------------------------------------------------------------------
void MemoryAllocatorInitialize( const MemoryAllocatorDesc& desc );

//---------------------------------------------------------------------------
//! @brief		メモリアロケーター終了.
//!
//!	@note		aroma::render::Initialize内で実行されるため,
//!				基本的にコールする必要はありません.
//---------------------------------------------------------------------------
void MemoryAllocatorFinalize();

//---------------------------------------------------------------------------
//! @brief		描画システム用メモリアロケータークラス.
//!
//! @note		当クラスを継承することで,
//!				クラススコープでのnew/delete演算子のオーバーロードを提供します.
//!				基本的にaroma::render空間で定義される描画システムオブジェクトは,
//!				当クラスを継承しているため,
//!				MermoryAllocatorInitialize実行時に指定したアロケーターを使用して,
//!				メモリ確保されます.
//---------------------------------------------------------------------------
struct MemoryAllocator
{
	//-----------------------------------------------------------------------
	//! @name	usual new/delete 演算子.
	//-----------------------------------------------------------------------
	//! @{
	static void* operator new ( size_t size ) noexcept
	{
		return CpuMemAlloc( size, 1 );
	}

	static void* operator new[] ( size_t size ) noexcept
	{
		return CpuMemAlloc( size, 1 );
	}

	static void operator delete ( void* ptr ) noexcept
	{
		CpuMemFree( ptr );
	}

	static void operator delete[] ( void* ptr ) noexcept
	{
		CpuMemFree( ptr );
	}
	//! @}

	//-----------------------------------------------------------------------
	//! @name	placement new/delete 演算子.
	//-----------------------------------------------------------------------
	//! @{
	static void* operator new ( size_t, void* addr ) noexcept
	{
		return addr;
	}

	static void* operator new[] ( size_t, void* addr ) noexcept
	{
		return addr;
	}

	static void operator delete ( void*, void* ) noexcept
	{}

	static void operator delete[] ( void*, void* ) noexcept
	{}
	//! @}

};

} // namespace render
} // namespace aroma