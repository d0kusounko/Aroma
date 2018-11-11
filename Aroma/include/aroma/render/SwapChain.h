//===========================================================================
//!
//!	@file		SwapChain.h
//!	@brief		スワップチェイン.
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
#include "../data/DataDef.h"
#include "../app/Window.h"

namespace aroma {
namespace render {

class Device;
class Texture;
class RenderTargetView;

//---------------------------------------------------------------------------
//!	@brief		スワップチェイン.
//---------------------------------------------------------------------------
class SwapChain : public RefObject, public MemoryAllocator, private util::NonCopyable< SwapChain >
{
public:
	//-----------------------------------------------------------------------
	//! @brief		構成設定.
	//-----------------------------------------------------------------------
	struct Desc
	{
		data::ImageSize		size;			//!< バッファ画素数.
		data::PixelFormat	format;			//!< バッファフォーマット.
		u32					bufferCount;	//!< バッファ数.
		app::Window*		window;			//!< 表示先ウィンドウ.
		//-------------------------------------------------------------------
		Desc(){ Default(); }
		void Default()
		{
			size.width		= 0;
			size.height		= 0;
			format			= data::PixelFormat::kR8G8B8A8Unorm;
			bufferCount		= 2;
			window			= nullptr;
		}
	};

public:
	//-----------------------------------------------------------------------
	//!	@brief		コンストラクタ.
	//-----------------------------------------------------------------------
	SwapChain();

	//-----------------------------------------------------------------------
	//!	@brief		デストラクタ.
	//-----------------------------------------------------------------------
	virtual ~SwapChain();

	//-----------------------------------------------------------------------
	//!	@brief		初期化.
	//-----------------------------------------------------------------------
	void Initialize( Device* device, const Desc& desc );

	//-----------------------------------------------------------------------
	//!	@brief		解放.
	//-----------------------------------------------------------------------
	void Finalize();

	//-----------------------------------------------------------------------
	//! @brief		バッファを画面に出力.
	//-----------------------------------------------------------------------
	void Present( u32 syncInterval = 1 );

	//-----------------------------------------------------------------------
	//! @brief		構成設定取得.
	//-----------------------------------------------------------------------
	const Desc& GetDesc() const;

	//-----------------------------------------------------------------------
	//! @brief		現在のバッファのインデックス取得.
	//-----------------------------------------------------------------------
	u32 GetCurrentBufferIndex() const;

	//-----------------------------------------------------------------------
	//! @brief		 バッファ取得.
	//-----------------------------------------------------------------------
	void GetBuffer( u32 index, Texture** outResource ) const;

	//-----------------------------------------------------------------------
	//! @brief		 バッファビュー取得.
	//-----------------------------------------------------------------------
	void GetBufferView( u32 index, RenderTargetView** outResource ) const;

private:
	bool					_initialized;
	Device*					_device;
	Desc					_desc;
	Texture**				_buffers;
	RenderTargetView**		_bufferRTVs;
	u32						_bufferIndex;

#ifdef AROMA_RENDER_DX11
	IDXGISwapChain*			_d3dSwapChain;
#endif
};

} // namespace render
} // namespace aroma
