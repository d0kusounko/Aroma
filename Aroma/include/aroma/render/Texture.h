﻿//===========================================================================
//!
//!	@file		Texture.h
//!	@brief		テクスチャー.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include "Resource.h"

namespace aroma {
namespace render {

class Device;

//---------------------------------------------------------------------------
//!	@brief	テクスチャー.
//---------------------------------------------------------------------------
class Texture : public IResource
{
public:
	//-----------------------------------------------------------------------
	//! @brief		構成設定.
	//-----------------------------------------------------------------------
	struct Desc
	{
		data::ImageSize		size;				//!< 画素数.
		s32					mipCount;			//!< ミップマップ数.
		data::PixelFormat	format;				//!< ピクセルフォーマット.
		Usage				usage;				//!< 用途(メモリ配置).
		s32					arrayCount;			//!< テクスチャ配列個数.
		u32					multiSampleCount;	//!< マルチサンプルカウント.
		u32					bindFlags;			//!< バインドフラグ(BIND_FLAG).
		SubResource*		initDataArray;		//!< 初期データ定義配列.
		u32					flags;				//!< その他オプションフラグ.
		//-------------------------------------------------------------------
		Desc(){ Default(); }
		void Default()
		{
			size.width			= 0;
			size.height			= 0;
			mipCount			= 1;
			format				= data::PixelFormat::kR8G8B8A8Unorm;
			usage				= Usage::kDefault;
			arrayCount			= 1;
			multiSampleCount	= 1;
			bindFlags			= 0;
			initDataArray		= nullptr;
			flags				= 0;
		}
	};

public:
	//-----------------------------------------------------------------------
	//! @brief		コンストラクタ.
	//-----------------------------------------------------------------------
	Texture();

	//-----------------------------------------------------------------------
	//! @brief		デストラクタ.
	//-----------------------------------------------------------------------
	virtual ~Texture();

	//-----------------------------------------------------------------------
	//! @brief		初期化.
	//-----------------------------------------------------------------------
	void Initialize( Device* device, const Desc& desc );

	//-----------------------------------------------------------------------
	//! @brief		ネイティブAPIテクスチャバッファより初期化.
	//-----------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
	void InitializeFromNativeTexture( Device* device, ID3D11Texture2D* nativeTexture );
#endif

	//-----------------------------------------------------------------------
	//! @brief		解放.
	//-----------------------------------------------------------------------
	void Finalize();

	//-----------------------------------------------------------------------
	//! @brief		構成設定取得.
	//-----------------------------------------------------------------------
	const Desc& GetDesc() const;

	//-----------------------------------------------------------------------
	//! @brief		メモリマッピング.
	//-----------------------------------------------------------------------
	void* Map() override;

	//-----------------------------------------------------------------------
	//! @brief		メモリマッピング解除.
	//-----------------------------------------------------------------------
	void Unmap() override;

	//-----------------------------------------------------------------------
	//! @brief		ネイティブAPIリソース取得.
	//-----------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
	ID3D11Resource* GetNativeResource() const;
#endif

private:
	bool							_initialized;
	Device*							_device;
	Desc							_desc;

#ifdef AROMA_RENDER_DX11
	ID3D11Texture2D*				_nativeTexture;
#endif
};

} // namespace render
} // namespace aroma
