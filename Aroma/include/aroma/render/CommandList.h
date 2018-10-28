//===========================================================================
//!
//!	@file		CommandList.h
//!	@brief		コマンドリスト.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include "RenderDef.h"
#include "MemoryAllocator.h"
#include "../common/RefObject.h"
#include "../util/NonCopyable.h"

namespace aroma {
namespace render {

class Device;
class Texture;
class TextureView;

//---------------------------------------------------------------------------
//!	@brief		コマンドリスト.
//---------------------------------------------------------------------------
class CommandList : public RefObject, public MemoryAllocator, private util::NonCopyable< CommandList >
{
public:
	//-----------------------------------------------------------------------
	//!	@brief		コンストラクタ.
	//-----------------------------------------------------------------------
	CommandList();

	//-----------------------------------------------------------------------
	//!	@brief		デストラクタ.
	//-----------------------------------------------------------------------
	virtual ~CommandList();

	//-----------------------------------------------------------------------
	//!	@brief		初期化.
	//!
	//! @note		ContextのEnd時に生成される時に実行されるため,
	//!				基本的にコールする必要はありません.
	//!
	//!				nativeCommandListに参照カウンタがあればインクリメントします.
	//-----------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
	void Initialize( Device* device, ID3D11CommandList* nativeCommandList );
#endif

	//-----------------------------------------------------------------------
	//!	@brief		解放.
	//-----------------------------------------------------------------------
	void Finalize();

	//-----------------------------------------------------------------------
	//!	@brief		ネイティブAPIコマンドリスト返却.
	//-----------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
	ID3D11CommandList* GetNativeCommandList() const;
#endif

private:
	bool					_initialized;
	Device*					_device;

#ifdef AROMA_RENDER_DX11
	ID3D11CommandList*		_d3dCommandList;
#endif
};

} // namespace render
} // namespace aroma
