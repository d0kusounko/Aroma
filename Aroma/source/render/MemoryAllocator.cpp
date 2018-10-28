//===========================================================================
//!
//!	@file		MemoryAllocator.cpp
//!	@brief		描画システム用メモリアロケーター.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/render/MemoryAllocator.h>

namespace aroma {
namespace render {

namespace
{
	bool				g_initialized = false;
	MemoryAllocatorDesc	g_desc;
}

//---------------------------------------------------------------------------
//! @brief		CPUメモリ確保.
//!
//! @pre		MemoryAllocatorInitialize実行済み.
//---------------------------------------------------------------------------
void* CpuMemAlloc( size_t size, size_t alignment )
{
	AROMA_ASSERT( g_desc.cpuMemAllocator, "Memory allocator has not been initialized yet.\n" );
	return g_desc.cpuMemAllocator->Alloc( size, alignment );
}

//---------------------------------------------------------------------------
//! @brief		CPUメモリ再確保.
//!
//! @pre		MemoryAllocatorInitialize実行済み.
//---------------------------------------------------------------------------
void* CpuMemRealloc( void* addr, size_t newSize )
{
	AROMA_ASSERT( g_desc.cpuMemAllocator, "Memory allocator has not been initialized yet.\n" );
	return g_desc.cpuMemAllocator->Realloc( addr, newSize );
}

//---------------------------------------------------------------------------
//! @brief		CPUメモリ解放.
//!
//! @pre		MemoryAllocatorInitialize実行済み.
//---------------------------------------------------------------------------
void  CpuMemFree( void* addr )
{
	AROMA_ASSERT( g_desc.cpuMemAllocator, "Memory allocator has not been initialized yet.\n" );
	g_desc.cpuMemAllocator->Free( addr );
}

//---------------------------------------------------------------------------
//! @brief		GPUメモリ確保.
//!
//! @pre		MemoryAllocatorInitialize実行済み.
//---------------------------------------------------------------------------
void* GpuMemAlloc( size_t size, size_t alignment )
{
	AROMA_ASSERT( g_desc.gpuMemAllocator, "Memory allocator has not been initialized yet.\n" );
	return g_desc.gpuMemAllocator->Alloc( size, alignment );
}

//---------------------------------------------------------------------------
//! @brief		GPUメモリ再確保.
//!
//! @pre		MemoryAllocatorInitialize実行済み.
//---------------------------------------------------------------------------
void* GpuMemRealloc( void* addr, size_t newSize )
{
	AROMA_ASSERT( g_desc.gpuMemAllocator, "Memory allocator has not been initialized yet.\n" );
	return g_desc.gpuMemAllocator->Realloc( addr, newSize );
}

//---------------------------------------------------------------------------
//! @brief		GPUメモリ解放.
//!
//! @pre		MemoryAllocatorInitialize実行済み.
//---------------------------------------------------------------------------
void  GpuMemFree( void* addr )
{
	AROMA_ASSERT( g_desc.gpuMemAllocator, "Memory allocator has not been initialized yet.\n" );
	g_desc.gpuMemAllocator->Free( addr );
}

//---------------------------------------------------------------------------
//! @brief		メモリアロケーター初期化.
//---------------------------------------------------------------------------
void MemoryAllocatorInitialize( const MemoryAllocatorDesc& desc )
{
	if( g_initialized )
	{
		AROMA_ASSERT( false, "Already initialized.\n" );
		return;
	}
	AROMA_ASSERT( desc.cpuMemAllocator, "Cpu memory allocator must not be nullptr." );
	AROMA_ASSERT( desc.gpuMemAllocator, "Cpu memory allocator must not be nullptr." );

	g_desc			= desc;
	g_initialized	= true;
}

//---------------------------------------------------------------------------
//! @brief		メモリアロケーター終了.
//---------------------------------------------------------------------------
void MemoryAllocatorFinalize()
{
	if( !g_initialized ) return;
	g_desc.Clear();
	g_initialized	= false;
}

} // namespace render
} // namespace aroma