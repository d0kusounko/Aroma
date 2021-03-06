﻿//===========================================================================
//!
//!	@file		TextureView.h
//!	@brief		テクスチャビュー.
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
//!	@brief	テクスチャビュー.
//---------------------------------------------------------------------------
class TextureView : public RefObject, public MemoryAllocator, private util::NonCopyable< TextureView >
{
public:
	//-----------------------------------------------------------------------
	//! @brief		構成設定.
	//-----------------------------------------------------------------------
	struct Desc
	{
		Texture* texture;	//!< テクスチャー.

		//-------------------------------------------------------------------
		Desc(){ Default(); }
		void Default()
		{
			texture = nullptr;
		}
	};

public:
	//-----------------------------------------------------------------------
	//! @brief		コンストラクタ.
	//-----------------------------------------------------------------------
	TextureView();

	//-----------------------------------------------------------------------
	//! @brief		デストラクタ.
	//-----------------------------------------------------------------------
	virtual ~TextureView();

	//-----------------------------------------------------------------------
	//! @brief		初期化.
	//-----------------------------------------------------------------------
	void Initialize( Device* device, const Desc& desc );

	//-----------------------------------------------------------------------
	//! @brief		解放.
	//-----------------------------------------------------------------------
	void Finalize();

	//-----------------------------------------------------------------------
	//! @brief		ネイティブAPIテクスチャビュー取得.
	//-----------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
	ID3D11ShaderResourceView* GetNativeShaderResourceView() const;
#endif

private:
	bool						_initialized;
	Device*						_device;
	Desc						_desc;
	Texture*					_texture;

#ifdef AROMA_RENDER_DX11
	ID3D11ShaderResourceView*	_nativeSRV;
#endif
};

} // namespace render
} // namespace aroma
