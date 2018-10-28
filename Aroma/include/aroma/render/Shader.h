//===========================================================================
//!
//!	@file		Shader.h
//!	@brief		シェーダー.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//! @todo		TODO: インターフェース化してステージ毎にオブジェクト作成.
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
//! @brief		シェーダー.
//---------------------------------------------------------------------------
class Shader : public RefObject, public MemoryAllocator, private util::NonCopyable< Shader >
{
public:
	//---------------------------------------------------------------------------
	//! @brief		シェーダーステージ.
	//---------------------------------------------------------------------------
	enum class Stage
	{
		kVertex,	//!< 頂点シェーダー.
		kPixel,		//!< ピクセルシェーダー.
		kNum,
	};

	//-----------------------------------------------------------------------
	//! @brief		構成設定.
	//-----------------------------------------------------------------------
	struct Desc
	{
		Stage		stage;			//!< テクスチャステージ.
		const void* shaderData;		//!< シェーダーバイナリ.
		size_t		shaderSize;		//!< シェーダーバイナリサイズ.
		//-------------------------------------------------------------------
		Desc(){ Default(); }
		void Default()
		{
			stage		= Stage::kVertex;
			shaderData	= nullptr;
			shaderSize	= 0;
		}
	};

public:
	//! コンストラクタ.
	Shader();
	//! デストラクタ.
	virtual ~Shader();

	//-----------------------------------------------------------------------
	//! @brief		初期化.
	//-----------------------------------------------------------------------
	bool Initialize( Device* device, const Desc& desc );

	//-----------------------------------------------------------------------
	//! @brief		解放.
	//-----------------------------------------------------------------------
	void Finalize();

	//-----------------------------------------------------------------------
	//! @brief		シェーダーバイナリデータ取得.
	//-----------------------------------------------------------------------
	void* GetShaderData() const;

	//-----------------------------------------------------------------------
	//! @brief		シェーダーバイナリデータサイズ取得.
	//-----------------------------------------------------------------------
	size_t GetShaderDataSize() const;

	//-----------------------------------------------------------------------
	//! @brief		ネイティブAPI頂点シェーダー取得.
	//-----------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
	ID3D11VertexShader* GetNativeVertexShader() const;
#endif

	//-----------------------------------------------------------------------
	//! @brief		ネイティブAPIピクセルシェーダー取得.
	//-----------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
	ID3D11PixelShader* GetNativePixelShader() const;
#endif

protected:
	bool				_initialized;
	Device*				_device;
	Desc		_desc;
	void*				_shaderData;
	size_t				_shaderSize;


#ifdef AROMA_RENDER_DX11
	ID3D11VertexShader*	_nativeVS;
	ID3D11PixelShader*	_nativePS;
#endif
};

} // namespace render
} // namespace aroma
