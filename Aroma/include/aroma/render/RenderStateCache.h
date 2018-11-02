//===========================================================================
//!
//!	@file		RenderStateCache.h
//!	@brief		レンダーステートキャッシュ.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include <unordered_map>
#include "RenderDef.h"
#include "MemoryAllocator.h"
#include "BlendState.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"
#include "SamplerState.h"
#include "ViewportScissorState.h"
#include "../util/NonCopyable.h"
#include "../data/CRC.h"

namespace aroma {
namespace render {

#ifdef AROMA_RENDER_DX11

struct D3D11_VIEWPORT_SCISSOR
{
	D3D11_VIEWPORT	viewport[ kViewportsSlotMax ];
	D3D11_RECT		scissor[ kViewportsSlotMax ];
};

using NativeBlendState				= ID3D11BlendState;
using NativeRasterizerState			= ID3D11RasterizerState;
using NativeDepthStencilState		= ID3D11DepthStencilState;
using NativeSamplerState			= ID3D11SamplerState;
using NativeViewportScissorState	= D3D11_VIEWPORT_SCISSOR;
#endif

class Device;

//---------------------------------------------------------------------------
//!	@brief		ブレンドステートハッシュキー.
//---------------------------------------------------------------------------
struct BlendStateKey final
{
	// 32Bits : 0
	u32	sampleAlphaToCoverage	: 1;	//!< boolean
	u32	blendEnable				: 1;	//!< boolean
	u32	rgbSource				: 5;	//!< Blend
	u32	rgbDest					: 5;	//!< Blend
	u32	rgbBlendOp				: 3;	//!< BlendOp
	u32	alphaSource				: 5;	//!< Blend
	u32	alphaDest				: 5;	//!< Blend
	u32	alphaBlendOp			: 3;	//!< BlendOp
	u32	colorMaskR				: 1;	//!< boolean
	u32	colorMaskG				: 1;	//!< boolean
	u32	colorMaskB				: 1;	//!< boolean
	u32	colorMaskA				: 1;	//!< boolean

	BlendStateKey( const BlendState& state );
	void Set( const BlendState& state );
};
bool operator==( const BlendStateKey& lhs, const BlendStateKey& rhs );
bool operator!=( const BlendStateKey& lhs, const BlendStateKey& rhs );

//---------------------------------------------------------------------------
//!	@brief		ラスタライザーステートハッシュキー.
//---------------------------------------------------------------------------
struct RasterizerStateKey final
{
	s32 depthBias;						//!< s32
	f32 depthBiasClamp;					//!< f32
	f32 slopeScaledDepthBias;			//!< f32

	// 32Bits: 0
	u32 fillMode				: 1;	//!< FillMode
	u32 cullMode				: 2;	//!< CullMode
	u32 frontCounterClockwise	: 1;	//!< bool
	u32 depthClipEnable			: 1;	//!< bool
	u32 scissorEnable			: 1;	//!< bool
	u32 multisampleEnable		: 1;	//!< bool
	u32 antialiasedLineEnable	: 1;	//!< bool

	RasterizerStateKey( const RasterizerState& state );
	void Set( const RasterizerState& state );
};
bool operator==( const RasterizerStateKey& lhs, const RasterizerStateKey& rhs );
bool operator!=( const RasterizerStateKey& lhs, const RasterizerStateKey& rhs );

//---------------------------------------------------------------------------
//!	@brief		深度ステンシルステートハッシュキー.
//---------------------------------------------------------------------------
struct DepthStencilStateKey final
{
	// 32Bits: 0
	u32 depthEnable					: 1;	//!< bool
	u32 depthWrite					: 1;	//!< bool
	u32 depthFunc					: 3;	//!< ComparisonFunc
	u32 stencilEnable				: 1;	//!< bool
	u32 frontFaceStencilFailOp		: 3;	//!< StencilOp
	u32 frontFaceStencilDepthFailOp	: 3;	//!< StencilOp
	u32 frontFaceStencilPassOp		: 3;	//!< StencilOp
	u32 frontFaceStencilFunc		: 3;	//!< ComparisonFunc
	u32 backFaceStencilFailOp		: 3;	//!< StencilOp
	u32 backFaceStencilDepthFailOp	: 3;	//!< StencilOp
	u32 backFaceStencilPassOp		: 3;	//!< StencilOp
	u32 backFaceStencilFunc			: 3;	//!< ComparisonFunc
	u32 pad0						: 2;

	// 32Bits: 1
	u32 stencilReadMask				: 8;	//!< u8
	u32 stencilWriteMask			: 8;	//!< u8

	DepthStencilStateKey( const DepthStencilState& state );
	void Set( const DepthStencilState& state );
};
bool operator==( const DepthStencilStateKey& lhs, const DepthStencilStateKey& rhs );
bool operator!=( const DepthStencilStateKey& lhs, const DepthStencilStateKey& rhs );

//---------------------------------------------------------------------------
//!	@brief		サンプラーステートハッシュキー.
//---------------------------------------------------------------------------
struct SamplerStateKey final
{
	f32			mipLODBias;		//!< f32
	f32			minLOD;			//!< f32
	f32			maxLOD;			//!< f32
	data::Color	borderColor;	//!< data::Color

	// 32Bits: 0
	u32	filter			: 4;	//!< Filter
	u32	addressU		: 3;	//!< TextureAddress
	u32	addressV		: 3;	//!< TextureAddress
	u32	addressW		: 3;	//!< TextureAddress
	u32	maxAnisotropy	: 3;	//!< AnisotropicRatio

	SamplerStateKey( const SamplerState& state );
	void Set( const SamplerState& state );
};
bool operator==( const SamplerStateKey& lhs, const SamplerStateKey& rhs );
bool operator!=( const SamplerStateKey& lhs, const SamplerStateKey& rhs );

//---------------------------------------------------------------------------
//!	@brief		ビューポートシザーステートハッシュキー.
//---------------------------------------------------------------------------
struct ViewportScissorStateKey final
{
	Viewport	viewport[ kViewportsSlotMax ];
	ScissorRect	scissor[ kViewportsSlotMax ];

	ViewportScissorStateKey( const ViewportScissorState& state );
	void Set( const ViewportScissorState& state );
};
bool operator==( const ViewportScissorStateKey& lhs, const ViewportScissorStateKey& rhs );
bool operator!=( const ViewportScissorStateKey& lhs, const ViewportScissorStateKey& rhs );

} // namespace render
} // namespace aroma

//===========================================================================
//!	@name		レンダーステートキャッシュ用ハッシュ.
//===========================================================================
//! @{
namespace std {
//---------------------------------------------------------------------------
//!	@brief		ブレンドステートハッシュ.
//---------------------------------------------------------------------------
template <>
struct hash< aroma::render::BlendStateKey >
{
	size_t operator()( const aroma::render::BlendStateKey& key ) const noexcept
	{
		return aroma::data::CRC::GetCRC( &key, sizeof( key ) );
	}
};

//---------------------------------------------------------------------------
//!	@brief		ラスタライザーステートハッシュ.
//---------------------------------------------------------------------------
template <>
struct hash< aroma::render::RasterizerStateKey >
{
	size_t operator()( const aroma::render::RasterizerStateKey& key ) const noexcept
	{
		return aroma::data::CRC::GetCRC( &key, sizeof( key ) );
	}
};

//---------------------------------------------------------------------------
//!	@brief		深度ステンシルステートハッシュ.
//---------------------------------------------------------------------------
template <>
struct hash< aroma::render::DepthStencilStateKey >
{
	size_t operator()( const aroma::render::DepthStencilStateKey& key ) const noexcept
	{
		return aroma::data::CRC::GetCRC( &key, sizeof( key ) );
	}
};

//---------------------------------------------------------------------------
//!	@brief		サンプラーステートハッシュ.
//---------------------------------------------------------------------------
template <>
struct hash< aroma::render::SamplerStateKey >
{
	size_t operator()( const aroma::render::SamplerStateKey& key ) const noexcept
	{
		return aroma::data::CRC::GetCRC( &key, sizeof( key ) );
	}
};

//---------------------------------------------------------------------------
//!	@brief		ビューポートシザーステートハッシュ.
//---------------------------------------------------------------------------
template <>
struct hash< aroma::render::ViewportScissorStateKey >
{
	size_t operator()( const aroma::render::ViewportScissorStateKey& key ) const noexcept
	{
		return aroma::data::CRC::GetCRC( &key, sizeof( key ) );
	}
};

}	// namespace std
//! @}

namespace aroma {
namespace render {
//---------------------------------------------------------------------------
//!	@brief		レンダーステートキャッシュ.
//---------------------------------------------------------------------------
class RenderStateCache final : public MemoryAllocator, private util::NonCopyable< RenderStateCache >
{
public:
	//-----------------------------------------------------------------------
	//!	@brief		コンストラクタ.
	//-----------------------------------------------------------------------
	explicit RenderStateCache( Device* device );

	//-----------------------------------------------------------------------
	//!	@brief		デストラクタ.
	//-----------------------------------------------------------------------
	virtual ~RenderStateCache();

	//-----------------------------------------------------------------------
	//!	@brief		ネイティブAPIブレンドステート取得.
	//-----------------------------------------------------------------------
	NativeBlendState* GetNativeBlendState( const BlendStateKey& key );

	//-----------------------------------------------------------------------
	//!	@brief		ネイティブAPIラスタライザーステート取得.
	//-----------------------------------------------------------------------
	NativeRasterizerState* GetNativeRasterizerState( const RasterizerStateKey& key );

	//-----------------------------------------------------------------------
	//!	@brief		ネイティブAPI深度ステンシルステート取得.
	//-----------------------------------------------------------------------
	NativeDepthStencilState* GetNativeDepthStencilState( const DepthStencilStateKey& key );

	//-----------------------------------------------------------------------
	//!	@brief		ネイティブAPIサンプラーステート取得.
	//-----------------------------------------------------------------------
	NativeSamplerState* GetNativeSamplerState( const SamplerStateKey& key );

	//-----------------------------------------------------------------------
	//!	@brief		ネイティブAPIビューポートシザーステート取得.
	//-----------------------------------------------------------------------
	NativeViewportScissorState* GetNativeViewportScissorState( const ViewportScissorStateKey& key );

private:
	RenderStateCache();

	Device* _device;

	// TODO: パフォーマンス向上のためコンテナをMRU化.
	using BlendStateCache = std::unordered_map< BlendStateKey, NativeBlendState* >;
	BlendStateCache _blendStateCache;

	using RasterizerStateCache = std::unordered_map< RasterizerStateKey, NativeRasterizerState* >;
	RasterizerStateCache _rasterizerStateCache;

	using DepthStencilStateCache = std::unordered_map< DepthStencilStateKey, NativeDepthStencilState* >;
	DepthStencilStateCache _depthStencilStateCache;

	using SamplerStateCache = std::unordered_map< SamplerStateKey, NativeSamplerState* >;
	SamplerStateCache _samplerStateCache;

	using ViewportScissorStateCache = std::unordered_map< ViewportScissorStateKey, NativeViewportScissorState* >;
	ViewportScissorStateCache _viewportScissorStateCache;
};

} // namespace render
} // namespace aroma
