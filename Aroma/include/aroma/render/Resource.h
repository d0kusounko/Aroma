//===========================================================================
//!
//!	@file		Resource.h
//!	@brief		GPUリソースインターフェース.
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

//---------------------------------------------------------------------------
//!	@brief	バッファのサブリソースメモリ配置情報.
//---------------------------------------------------------------------------
struct SubResource
{
	union {
		void*		data;			//! データの先頭アドレス.
		const void*	dataConst;		//! データの先頭アドレス const版.
	};
	size_t			pitch;			//!< データピッチ.
	size_t			slicePitch;		//!< デプススライスピッチ.

	//-------------------------------------------------------------------
	SubResource(){ Clear(); }
	void Clear()
	{
		data		= nullptr;
		pitch		= 0;
		slicePitch	= 0;
	}

	//---------------------------------------------------------------------------
	//!	@brief	ネイティブAPIサブリソースに変換.
	//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
	void ToNativeSubResource( D3D11_SUBRESOURCE_DATA* outSubResource ) const;
#endif
};

//---------------------------------------------------------------------------
//!	@brief	GPUリソースインターフェース.
//---------------------------------------------------------------------------
class IResource : public RefObject, public MemoryAllocator, private util::NonCopyable< IResource >
{
public:
	IResource();
	virtual ~IResource();

	//-----------------------------------------------------------------------
    //! @brief		メモリマッピング.
	//-----------------------------------------------------------------------
    virtual void* Map() = 0;

	//-----------------------------------------------------------------------
    //! @brief		メモリマッピング解除.
	//-----------------------------------------------------------------------
    virtual void Unmap() = 0;

protected:
	Device*	_device;
};

} // namespace render
} // namespace aroma
