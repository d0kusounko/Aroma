//===========================================================================
//!
//!	@file		InputLayout.h
//!	@brief		入力レイアウト.
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

namespace aroma {
namespace render {

class Device;
class Shader;

//---------------------------------------------------------------------------
//!	@brief		入力レイアウト.
//---------------------------------------------------------------------------
class InputLayout : public RefObject, public MemoryAllocator, private util::NonCopyable< InputLayout >
{
public:
	struct ElementDesc;
	struct StreamDesc;

	//-----------------------------------------------------------------------
	//! @brief		入力レイアウト構成設定.
	//-----------------------------------------------------------------------
	struct Desc
	{
		u32				streamNum;				//!< 入力ストリーム数.
		StreamDesc*		streams;				//!< 入力ストリームリスト.
		Shader*			vertexShader;			//!< 入力シグネチャ用頂点シェーダー.
		//-------------------------------------------------------------------
		Desc(){ Default(); }
		void Default()
		{
			streamNum			= 0;
			streams				= nullptr;
			vertexShader		= nullptr;
		}
	};

	//-----------------------------------------------------------------------
	//! @brief		入力ストリーム構成設定.
	//-----------------------------------------------------------------------
	struct StreamDesc
	{
		u32				elementNum;		//!< 入力要素数.
		ElementDesc*	elements;		//!< 入力要素リスト.
		u32				streamIndex;	//!< ストリームインデックス.
		size_t			stride;			//!< ストライド.
		InputClass		inputClass;		//!< 格納データ種別.
	};

	//-----------------------------------------------------------------------
	//! @brief		入力要素構成設定.
	//-----------------------------------------------------------------------
	struct ElementDesc
	{
		const char*			semanticName;	//!< セマンティック
		u32					semanticIndex;	//!< セマンティックインデックス.
		data::PixelFormat	format;			//!< 要素データフォーマット.
		size_t				offset;			//!< オフセット.
	};

public:
	//-----------------------------------------------------------------------
	//! @brief		コンストラクタ.
	//-----------------------------------------------------------------------
	InputLayout();

	//-----------------------------------------------------------------------
	//! @brief		デストラクタ.
	//-----------------------------------------------------------------------
	virtual ~InputLayout();

	//-----------------------------------------------------------------------
	//! @brief		初期化.
	//-----------------------------------------------------------------------
	void Initialize( Device* device, const Desc& desc );

	//-----------------------------------------------------------------------
	//! @brief		解放.
	//-----------------------------------------------------------------------
	void Finalize();

	//-----------------------------------------------------------------------
	//!	@brief		ネイティブAPI入力レイアウトの取得.
	//-----------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
	ID3D11InputLayout* GetNativeInputLayout() const;
#endif

private:
	bool					_initialized;
	Device*					_device;
	Desc					_desc;

#ifdef AROMA_RENDER_DX11
	ID3D11InputLayout*		_nativeInputLayout;
#endif
};

} // namespace render
} // namespace aroma
