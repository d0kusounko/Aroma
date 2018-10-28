//===========================================================================
//!
//!	@file		CommandList_DX11.cpp
//!	@brief		コマンドリスト : DirectX11.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#ifdef AROMA_RENDER_DX11

#include <aroma/render/CommandList.h>
#include <aroma/render/Device.h>

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//!	@brief		コンストラクタ.
//---------------------------------------------------------------------------
CommandList::CommandList()
	: _initialized( false )
	, _device( nullptr )
	, _d3dCommandList( nullptr )
{
}

//---------------------------------------------------------------------------
//!	@brief		デストラクタ.
//---------------------------------------------------------------------------
CommandList::~CommandList()
{
	Finalize();
}


//-----------------------------------------------------------------------
//!	@brief		初期化.
//!
//! @note		ContextのEnd時に生成される時に実行されるため,
//!				基本的にコールする必要はありません.
//!
//!				nativeCommandListに参照カウンタがあればインクリメントします.
//-----------------------------------------------------------------------
void CommandList::Initialize( Device* device, ID3D11CommandList* nativeCommandList )
{
	if( _initialized )
	{
		AROMA_ASSERT( false, _T( "Already initialized.\n" ) );
		Finalize();
	}

	_device = device;
	_device->AddRef();
	_d3dCommandList = nativeCommandList;
	_d3dCommandList->AddRef();

	_initialized = true;
}

//---------------------------------------------------------------------------
//!	@brief		解放.
//---------------------------------------------------------------------------
void CommandList::Finalize()
{
	if( !_initialized ) return;

	memory::SafeRelease( _d3dCommandList );
	memory::SafeRelease( _device );

	_initialized = false;
}

//---------------------------------------------------------------------------
//!	@brief		ネイティブAPIコマンドリスト返却.
//---------------------------------------------------------------------------
ID3D11CommandList* CommandList::GetNativeCommandList() const
{
	return _d3dCommandList;
}

} // namespace render
} // namespace aroma

#endif
