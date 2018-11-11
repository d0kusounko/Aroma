//===========================================================================
//!
//!	@file		DepthStencilView.h
//!	@brief		深度ステンシルビュー.
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

//---------------------------------------------------------------------------
//!	@brief	深度ステンシルビュー.
//---------------------------------------------------------------------------
class DepthStencilView : public RefObject, public MemoryAllocator, private util::NonCopyable< DepthStencilView >
{
public:
	//-----------------------------------------------------------------------
	//! @brief		構成設定.
	//-----------------------------------------------------------------------
	struct Desc
	{
		Texture*	texture;	//!< テクスチャ.
		u32			mipLevel;	//!< ミップレベル.

		//-------------------------------------------------------------------
		Desc(){ Default(); }
		void Default()
		{
			texture		= nullptr;
			mipLevel	= 0;
		}
	};

public:
	//-----------------------------------------------------------------------
	//! @brief		コンストラクタ.
	//-----------------------------------------------------------------------
	DepthStencilView();

	//-----------------------------------------------------------------------
	//! @brief		デストラクタ.
	//-----------------------------------------------------------------------
	virtual ~DepthStencilView();

	//-----------------------------------------------------------------------
	//! @brief		初期化.
	//-----------------------------------------------------------------------
	void Initialize( Device* device, const Desc& desc );

	//-----------------------------------------------------------------------
	//! @brief		解放.
	//-----------------------------------------------------------------------
	void Finalize();

	//-----------------------------------------------------------------------
	//! @brief		ネイティブAPI深度ステンシルビュー取得.
	//-----------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
	ID3D11DepthStencilView* GetNativeDepthStencilView() const;
#endif

private:
	bool						_initialized;
	Device*						_device;
	Desc						_desc;
	Texture*					_texture;

#ifdef AROMA_RENDER_DX11
	ID3D11DepthStencilView*		_nativeDSV;
#endif
};

} // namespace render
} // namespace aroma
