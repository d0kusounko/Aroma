//===========================================================================
//!
//!	@file		Device.h
//!	@brief		描画システムデバイス.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include "../common/RefObject.h"
#include "../util/NonCopyable.h"
#include "RenderDef.h"
#include "MemoryAllocator.h"
#include "SwapChain.h"
#include "DeferredContext.h"
#include "Buffer.h"
#include "InputLayout.h"
#include "RenderStateCache.h"
#include "Texture.h"

namespace aroma {

namespace data {
class DDSAccessor;
}

namespace render {

struct SubResource;

//---------------------------------------------------------------------------
//!	@brief		描画システムデバイス.
//---------------------------------------------------------------------------
class Device : public RefObject, public MemoryAllocator, private util::NonCopyable< Device >
{
public:
	//---------------------------------------------------------------------------
	//!	@brief		コンストラクタ.
	//---------------------------------------------------------------------------
	Device();
	//---------------------------------------------------------------------------
	//!	@brief		デストラクタ.
	//---------------------------------------------------------------------------
	virtual ~Device();

	//---------------------------------------------------------------------------
	//!	@brief		初期化.
	//!
	//! @pre		aroma::render::Initialize()実行済み.
	//---------------------------------------------------------------------------
	void Initialize();

	//---------------------------------------------------------------------------
	//!	@brief		終了.
	//---------------------------------------------------------------------------
	void Finalize();

	//---------------------------------------------------------------------------
	//!	@brief		スワップチェイン作成.
	//---------------------------------------------------------------------------
	SwapChain* CreateSwapChain( const SwapChain::Desc& desc );

	//---------------------------------------------------------------------------
	//!	@brief		遅延コンテキスト作成.
	//---------------------------------------------------------------------------
	DeferredContext* CreateDeferredContext( const DeferredContext::Desc& desc );

	//---------------------------------------------------------------------------
	//!	@brief		描画コマンドリスト実行.
	//---------------------------------------------------------------------------
	void ExecuteCommand( const CommandList* commandList );

	//---------------------------------------------------------------------------
	//! @brief		GPUバッファ作成.
	//---------------------------------------------------------------------------
	Buffer* CreateBuffer( const Buffer::Desc& desc );

	//---------------------------------------------------------------------------
	//! @brief		頂点バッファ作成.
	//---------------------------------------------------------------------------
	Buffer* CreateVertexBuffer( size_t size, Usage usage, const SubResource* initData, size_t stride, u32 flags );

	//---------------------------------------------------------------------------
	//! @brief		インデックスバッファ作成.
	//---------------------------------------------------------------------------
	Buffer* CreateIndexBuffer( size_t size, Usage usage, const SubResource* initData, IndexType indexType, u32 flags );

	//---------------------------------------------------------------------------
	//! @brief		定数バッファ作成.
	//---------------------------------------------------------------------------
	Buffer* CreateConstantBuffer( size_t size, Usage usage, const SubResource* initData, size_t stride, u32 flags );

	//---------------------------------------------------------------------------
	//! @brief		頂点シェーダー作成.
	//---------------------------------------------------------------------------
	Shader* CreateVertexShader( const void* data, size_t size );

	//---------------------------------------------------------------------------
	//! @brief		ピクセルシェーダー作成.
	//---------------------------------------------------------------------------
	Shader* CreatePixelShader( const void* data, size_t size );

	//---------------------------------------------------------------------------
	//!	@brief		入力レイアウトを作成.
	//---------------------------------------------------------------------------
	InputLayout* CreateInputLayout( const InputLayout::Desc& desc );

	//--------------------------------------------------------------------
	//! @brief		2Dテクスチャ作成.
	//--------------------------------------------------------------------
	Texture* CreateTexture2D( const Texture::Desc& desc );

	//--------------------------------------------------------------------
	//! @brief		DDSデータより2Dテクスチャ作成.
	//--------------------------------------------------------------------
	Texture* CreateTexture2DFromDDS( const data::DDSAccessor& dds, Usage usage, u32 bindFlags, u32 flags );

	//---------------------------------------------------------------------------
	//! @brief		レンダーステートキャッシュの取得.
	//---------------------------------------------------------------------------
	RenderStateCache* GetRenderStateCache();

	//---------------------------------------------------------------------------
	//!	@brief		ネイティブAPIデバイスの取得.
	//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
	ID3D11Device* GetNativeDevice() const;
#endif

#ifdef AROMA_RENDER_DX11
	//---------------------------------------------------------------------------
	//!	@brief		DXGIファクトリーの取得.
	//---------------------------------------------------------------------------
	IDXGIFactory* GetDXGIFactory() const;
#endif

private:
	bool					_initialized;
	RenderStateCache		_renderStateCache;

#ifdef AROMA_RENDER_DX11
	ID3D11Device*			_d3dDevice;
	IDXGIFactory*			_d3dFactory;
	ID3D11DeviceContext*	_d3dImmediateContext;
	D3D_FEATURE_LEVEL		_d3dFeatureLevel;
#endif
};

} // namespace render
} // namespace aroma
