//===========================================================================
//!
//!	@file		RenderStateCache_DX11.cpp
//!	@brief		レンダーステートキャッシュ : DirectX11.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#ifdef AROMA_RENDER_DX11

#include <aroma/render/RenderStateCache.h>
#include <aroma/render/Device.h>
#include <aroma/render/Texture.h>
#include <aroma/render/TextureView.h>
#include <aroma/app/App.h>

namespace aroma {
namespace render {

namespace
{
	constexpr u32 kStatesMax = 4096;
}

//===========================================================================
//!	@name		ブレンドステートハッシュキー.
//===========================================================================
//! @{
//===========================================================================
//!	@brief		コピーコンストラクタ.
//===========================================================================
BlendStateKey::BlendStateKey( const BlendState& state )
{
	Set( state );
}

//===========================================================================
//!	@brief		キー生成.
//===========================================================================
void BlendStateKey::Set( const BlendState& state )
{
	memset( this, 0, sizeof( BlendStateKey ) );
	sampleAlphaToCoverage	= state.sampleAlphaToCoverage ? 1 : 0;
	blendEnable				= state.blendEnable ? 1 : 0;;
	rgbSource				= static_cast< u32 >( state.rgbSource );
	rgbDest					= static_cast< u32 >( state.rgbDest );
	rgbBlendOp				= static_cast< u32 >( state.rgbBlendOp );
	alphaSource				= static_cast< u32 >( state.alphaSource );
	alphaDest				= static_cast< u32 >( state.alphaDest );
	alphaBlendOp			= static_cast< u32 >( state.alphaBlendOp );
	colorMaskR				= state.colorMaskR ? 1 : 0;
	colorMaskG				= state.colorMaskG ? 1 : 0;
	colorMaskB				= state.colorMaskB ? 1 : 0;
	colorMaskA				= state.colorMaskA ? 1 : 0;
}

bool operator==( const BlendStateKey& lhs, const BlendStateKey& rhs )
{
	return memcmp( &lhs, &rhs, sizeof( BlendStateKey ) ) == 0;
}
bool operator!=( const BlendStateKey& lhs, const BlendStateKey& rhs )
{
	return !( lhs == rhs );
}
//! @}

//===========================================================================
//!	@name		ラスタライザーステートステートハッシュキー.
//===========================================================================
//! @{
//===========================================================================
//!	@brief		コピーコンストラクタ.
//===========================================================================
RasterizerStateKey::RasterizerStateKey( const RasterizerState& state )
{
	Set( state );
}

//===========================================================================
//!	@brief		キー生成.
//===========================================================================
void RasterizerStateKey::Set( const RasterizerState& state )
{
	memset( this, 0, sizeof( RasterizerStateKey ) );

	depthBias				= state.depthBias;
	depthBiasClamp			= state.depthBiasClamp;
	slopeScaledDepthBias	= state.slopeScaledDepthBias;

	fillMode				= static_cast< u32 >( state.fillMode );
	cullMode				= static_cast< u32 >( state.cullMode );
	frontCounterClockwise	= state.frontCounterClockwise ? 1 : 0;
	depthClipEnable			= state.depthClipEnable ? 1 : 0;
	scissorEnable			= state.scissorEnable ? 1 : 0;
	multisampleEnable		= state.multisampleEnable ? 1 : 0;
	antialiasedLineEnable	= state.antialiasedLineEnable ? 1 : 0;
}

bool operator==( const RasterizerStateKey& lhs, const RasterizerStateKey& rhs )
{
	return memcmp( &lhs, &rhs, sizeof( RasterizerStateKey ) ) == 0;
}
bool operator!=( const RasterizerStateKey& lhs, const RasterizerStateKey& rhs )
{
	return !( lhs == rhs );
}
//! @}

//===========================================================================
//!	@name		深度ステンシルステートステートハッシュキー.
//===========================================================================
//! @{
//===========================================================================
//!	@name		コピーコンストラクタ.
//===========================================================================
DepthStencilStateKey::DepthStencilStateKey( const DepthStencilState& state )
{
	Set( state );
}

//===========================================================================
//!	@name		キー生成.
//===========================================================================
void DepthStencilStateKey::Set( const DepthStencilState& state )
{
	memset( this, 0, sizeof( DepthStencilStateKey ) );

	depthEnable					= state.depthEnable ? 1 : 0;
	depthWrite					= state.depthWrite ? 1 : 0;
	depthFunc					= static_cast< u32 >( state.depthFunc );
	stencilEnable				= state.stencilEnable ? 1 : 0;
	frontFaceStencilFailOp		= static_cast< u32 >( state.frontFaceStencilFailOp );
	frontFaceStencilDepthFailOp	= static_cast< u32 >( state.frontFaceStencilDepthFailOp );
	frontFaceStencilPassOp		= static_cast< u32 >( state.frontFaceStencilPassOp );
	frontFaceStencilFunc		= static_cast< u32 >( state.frontFaceStencilFunc );
	backFaceStencilFailOp		= static_cast< u32 >( state.backFaceStencilFailOp );
	backFaceStencilDepthFailOp	= static_cast< u32 >( state.backFaceStencilDepthFailOp );
	backFaceStencilPassOp		= static_cast< u32 >( state.backFaceStencilPassOp );
	backFaceStencilFunc			= static_cast< u32 >( state.backFaceStencilFunc );

	stencilReadMask				= static_cast< u32 >( state.stencilReadMask );
	stencilWriteMask			= static_cast< u32 >( state.stencilWriteMask );
}

bool operator==( const DepthStencilStateKey& lhs, const DepthStencilStateKey& rhs )
{
	return memcmp( &lhs, &rhs, sizeof( DepthStencilStateKey ) ) == 0;
}
bool operator!=( const DepthStencilStateKey& lhs, const DepthStencilStateKey& rhs )
{
	return !( lhs == rhs );
}
//! @}

//===========================================================================
//!	@name		サンプラーステートステートハッシュキー.
//===========================================================================
//! @{
//===========================================================================
//!	@name		コピーコンストラクタ.
//===========================================================================
SamplerStateKey::SamplerStateKey( const SamplerState& state )
{
	Set( state );
}

//===========================================================================
//!	@name		キー生成.
//===========================================================================
void SamplerStateKey::Set( const SamplerState& state )
{
	memset( this, 0, sizeof( SamplerStateKey ) );

	mipLODBias					= state.mipLODBias;
	minLOD						= state.minLOD;
	maxLOD						= state.maxLOD;
	borderColor					= state.borderColor;

	filter						= static_cast< u32 >( state.filter );
	addressU					= static_cast< u32 >( state.addressU );
	addressV					= static_cast< u32 >( state.addressV );
	addressW					= static_cast< u32 >( state.addressW );
	maxAnisotropy				= static_cast< u32 >( state.maxAnisotropy );
}

bool operator==( const SamplerStateKey& lhs, const SamplerStateKey& rhs )
{
	return memcmp( &lhs, &rhs, sizeof( SamplerStateKey ) ) == 0;
}
bool operator!=( const SamplerStateKey& lhs, const SamplerStateKey& rhs )
{
	return !( lhs == rhs );
}
//! @}


//===========================================================================
//!	@name		レンダーステートキャッシュ.
//===========================================================================
//! @{
//---------------------------------------------------------------------------
//!	@brief		コンストラクタ.
//---------------------------------------------------------------------------
RenderStateCache::RenderStateCache( Device* device )
	: _device( device )
{
	_blendStateCache.reserve( kStatesMax );
	_rasterizerStateCache.reserve( kStatesMax );
	_depthStencilStateCache.reserve( kStatesMax );
}

//---------------------------------------------------------------------------
//!	@brief		デストラクタ.
//---------------------------------------------------------------------------
RenderStateCache::~RenderStateCache()
{
	for( auto& nativeBlendState : _blendStateCache )
	{
		memory::SafeRelease( nativeBlendState.second );
	}
	for( auto& nativeRasterizerState : _rasterizerStateCache )
	{
		memory::SafeRelease( nativeRasterizerState.second );
	}
	for( auto& nativeDepthStencilState : _depthStencilStateCache )
	{
		memory::SafeRelease( nativeDepthStencilState.second );
	}
	for( auto& nativeSamplerState : _samplerStateCache )
	{
		memory::SafeRelease( nativeSamplerState.second );
	}
}

//---------------------------------------------------------------------------
//!	@brief		ネイティブAPIブレンドステート取得.
//---------------------------------------------------------------------------
NativeBlendState* RenderStateCache::GetNativeBlendState( const BlendStateKey& key )
{
	auto it = _blendStateCache.find( key );
	if( it != _blendStateCache.end() )
	{
		// キャッシュを返却.
		return it->second;
	}

	// キャッシュされていない場合は新規追加.
	auto d3dDevice = _device->GetNativeDevice();

	// TODO: 必要であればレンダーターゲット別独立ブレンド(Independent Blend)対応.
	D3D11_BLEND_DESC d3dBlendDesc = {};
	d3dBlendDesc.AlphaToCoverageEnable					= key.sampleAlphaToCoverage ? TRUE : FALSE;
	d3dBlendDesc.IndependentBlendEnable					= FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable			= key.blendEnable ? TRUE : FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend				= ToNativeBlend( static_cast< Blend >( key.rgbSource ) );
	d3dBlendDesc.RenderTarget[0].DestBlend				= ToNativeBlend( static_cast< Blend >( key.rgbDest ) );
	d3dBlendDesc.RenderTarget[0].BlendOp				= ToNativeBlendOp( static_cast< BlendOp >( key.rgbBlendOp ) );
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha			= ToNativeBlend( static_cast< Blend >( key.alphaSource ) );
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha			= ToNativeBlend( static_cast< Blend >( key.alphaDest ) );
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha			= ToNativeBlendOp( static_cast< BlendOp >( key.alphaBlendOp ) );
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask	|= key.colorMaskR ? D3D11_COLOR_WRITE_ENABLE_RED : 0;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask	|= key.colorMaskG ? D3D11_COLOR_WRITE_ENABLE_GREEN : 0;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask	|= key.colorMaskB ? D3D11_COLOR_WRITE_ENABLE_BLUE : 0;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask	|= key.colorMaskA ? D3D11_COLOR_WRITE_ENABLE_ALPHA : 0;

	ID3D11BlendState*	d3dBlendState;
	f32					blendFactor[ 4 ] = {};
	d3dDevice->CreateBlendState( &d3dBlendDesc, &d3dBlendState );
	_blendStateCache[ key ] = d3dBlendState;
	return d3dBlendState;
}

//---------------------------------------------------------------------------
//!	@brief		ネイティブAPIラスタライザーステート取得.
//---------------------------------------------------------------------------
NativeRasterizerState* RenderStateCache::GetNativeRasterizerState( const RasterizerStateKey& key )
{
	auto it = _rasterizerStateCache.find( key );
	if( it != _rasterizerStateCache.end() )
	{
		// キャッシュを返却.
		return it->second;
	}

	// キャッシュされていない場合は新規追加.
	auto d3dDevice = _device->GetNativeDevice();
	D3D11_RASTERIZER_DESC d3dRasterizerDesc = {};
	d3dRasterizerDesc.FillMode					= ToNativeFillMode( static_cast< FillMode >( key.fillMode ) );
	d3dRasterizerDesc.CullMode					= ToNativeCullMode( static_cast< CullMode >( key.cullMode ) );
	d3dRasterizerDesc.FrontCounterClockwise		= key.frontCounterClockwise ? TRUE : FALSE;
	d3dRasterizerDesc.DepthBias					= key.depthBias;
	d3dRasterizerDesc.DepthBiasClamp			= key.depthBiasClamp;
	d3dRasterizerDesc.SlopeScaledDepthBias		= key.slopeScaledDepthBias;
	d3dRasterizerDesc.DepthClipEnable			= key.depthClipEnable ? TRUE : FALSE;
	d3dRasterizerDesc.ScissorEnable				= key.scissorEnable ? TRUE : FALSE;
	d3dRasterizerDesc.MultisampleEnable			= key.multisampleEnable ? TRUE : FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable		= key.antialiasedLineEnable ? TRUE : FALSE;

	ID3D11RasterizerState*	d3dRasterizerState;
	d3dDevice->CreateRasterizerState( &d3dRasterizerDesc, &d3dRasterizerState );
	_rasterizerStateCache[ key ] = d3dRasterizerState;
	return d3dRasterizerState;
}

//---------------------------------------------------------------------------
//!	@brief		ネイティブAPI深度ステンシルステート取得.
//---------------------------------------------------------------------------
NativeDepthStencilState* RenderStateCache::GetNativeDepthStencilState( const DepthStencilStateKey& key )
{
	auto it = _depthStencilStateCache.find( key );
	if( it != _depthStencilStateCache.end() )
	{
		// キャッシュを返却.
		return it->second;
	}

	// キャッシュされていない場合は新規追加.
	auto d3dDevice = _device->GetNativeDevice();
	D3D11_DEPTH_STENCIL_DESC d3dDepthStencilDesc = {};
	d3dDepthStencilDesc.DepthEnable			= key.depthEnable ? TRUE : FALSE;
	d3dDepthStencilDesc.DepthWriteMask		= key.depthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	d3dDepthStencilDesc.DepthFunc			= ToNativeComparisonFunc( static_cast< ComparisonFunc >( key.depthFunc ) );
	d3dDepthStencilDesc.StencilEnable		= key.depthEnable ? TRUE : FALSE;
	d3dDepthStencilDesc.StencilReadMask		= static_cast< u8 >( key.stencilReadMask );
	d3dDepthStencilDesc.StencilWriteMask	= static_cast< u8 >( key.stencilWriteMask );

	d3dDepthStencilDesc.FrontFace.StencilFailOp			= ToNativeStencilOp( static_cast< StencilOp >( key.frontFaceStencilFailOp ) );
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp	= ToNativeStencilOp( static_cast< StencilOp >( key.frontFaceStencilDepthFailOp ) );
	d3dDepthStencilDesc.FrontFace.StencilPassOp			= ToNativeStencilOp( static_cast< StencilOp >( key.frontFaceStencilPassOp ) );
	d3dDepthStencilDesc.FrontFace.StencilFunc			= ToNativeComparisonFunc( static_cast< ComparisonFunc >( key.frontFaceStencilFunc ) );

	d3dDepthStencilDesc.BackFace.StencilFailOp			= ToNativeStencilOp( static_cast< StencilOp >( key.backFaceStencilFailOp ) );
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp		= ToNativeStencilOp( static_cast< StencilOp >( key.backFaceStencilDepthFailOp ) );
	d3dDepthStencilDesc.BackFace.StencilPassOp			= ToNativeStencilOp( static_cast< StencilOp >( key.backFaceStencilPassOp ) );
	d3dDepthStencilDesc.BackFace.StencilFunc			= ToNativeComparisonFunc( static_cast< ComparisonFunc >( key.backFaceStencilFunc ) );

	ID3D11DepthStencilState*	d3dDepthStencilState;
	d3dDevice->CreateDepthStencilState( &d3dDepthStencilDesc, &d3dDepthStencilState );
	_depthStencilStateCache[ key ] = d3dDepthStencilState;
	return d3dDepthStencilState;
}

//---------------------------------------------------------------------------
//!	@brief		ネイティブAPIサンプラーステート取得.
//---------------------------------------------------------------------------
NativeSamplerState* RenderStateCache::GetNativeSamplerState( const SamplerStateKey& key )
{
	auto it = _samplerStateCache.find( key );
	if( it != _samplerStateCache.end() )
	{
		// キャッシュを返却.
		return it->second;
	}

	// キャッシュされていない場合は新規追加.
	auto d3dDevice = _device->GetNativeDevice();
	D3D11_SAMPLER_DESC d3dSamplerDesc = {};
	d3dSamplerDesc.Filter			= ToNativeFilter( static_cast< Filter >( key.filter ) );
	d3dSamplerDesc.AddressU			= ToNativeTextureAddress( static_cast< TextureAddress >( key.addressU ) );
	d3dSamplerDesc.AddressV			= ToNativeTextureAddress( static_cast< TextureAddress >( key.addressV ) );
	d3dSamplerDesc.AddressW			= ToNativeTextureAddress( static_cast< TextureAddress >( key.addressW ) );
	d3dSamplerDesc.MipLODBias		= key.mipLODBias;
	d3dSamplerDesc.MaxAnisotropy	= ToNativeAnisotropicRatio( static_cast< AnisotropicRatio >( key.maxAnisotropy ) );
	d3dSamplerDesc.ComparisonFunc	= ToNativeComparisonFunc( ComparisonFunc::kNever );
	d3dSamplerDesc.BorderColor[0]	= key.borderColor.r;
	d3dSamplerDesc.BorderColor[1]	= key.borderColor.g;
	d3dSamplerDesc.BorderColor[2]	= key.borderColor.b;
	d3dSamplerDesc.BorderColor[3]	= key.borderColor.a;
	d3dSamplerDesc.MinLOD			= key.minLOD;
	d3dSamplerDesc.MaxLOD			= key.maxLOD;

	ID3D11SamplerState*	d3dSamplerState;
	d3dDevice->CreateSamplerState( &d3dSamplerDesc, &d3dSamplerState );
	_samplerStateCache[ key ] = d3dSamplerState;
	return d3dSamplerState;
}

//! @}

} // namespace render
} // namespace

#endif
