//===========================================================================
//!
//!	@file		RenderDef.h
//!	@brief		描画システム共通定義.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include "../data/DataDef.h"

//---------------------------------------------------------------------------
//! @name		グラフィックスAPIヘッダーインクルード.
//---------------------------------------------------------------------------
//! @{
#ifdef AROMA_RENDER_DX11
#include <d3d11.h>
#endif
//! @}

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//! @name		デバイス関連定義.
//---------------------------------------------------------------------------
//! @{
#ifdef AROMA_RENDER_DX11
//! レンダーターゲット設定スロット最大数.
//! d3d11.h : D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT
constexpr u32	kRenderTargetsSlotMax = 8;

//! ビューポート設定スロット最大数.
//! d3d11.h : D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE
constexpr u32	kViewportsSlotMax = 16;

//! 入力ストリーム最大数.
//! d3d11.h : D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT
constexpr u32	kInputStreamsMax = 32;

//! 入力ストリーム毎の要素最大数.
//! d3d11.h : D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT
constexpr u32	kInputElementsMax = 32;

//! シェーダーリソース設定スロット最大数.
//! d3d11.h : D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT
constexpr u32	kShaderResourceSlotMax = 128;

//! サンプラー設定スロット最大数.
//! d3d11.h : D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT
constexpr u32	kSamplerSlotMax = 16;

//! 定数バッファ設定スロット最大数.
//! d3d11.h : D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT
constexpr u32	kShaderUniformBufferSlotMax = 14;

//! テクスチャ数ミップレベル最大数.
//! d3d11.h : D3D11_REQ_MIP_LEVELS
constexpr u32	kMipCountMax = 15;

#else
#error Please define.
#endif
//! @}

//---------------------------------------------------------------------------
//! @brief		バッファー用途.
//!
//! @note		各モードのアクセス許可表.
//!		+-------+-------+---------+-------+-------+
//!		|       |Default|Immutable|Dynamic|Staging|
//!		+-------+-------+---------+-------+-------+
//!		| GPU-R |   o   |    o    |   o   |   o   |
//!		| GPU-W |   o   |    x    |   x   |   o   |
//!		| CPU-R |   x   |    x    |   x   |   o   |
//!		| CPU-W |   x   |    x    |   o   |   o   |
//!		+-------+-------+---------+-------+-------+
//---------------------------------------------------------------------------
enum class Usage
{
	kDefault,	//!< GPU読み書き可, CPUアクセス不可.
	kImmutable,	//!< GPU読み取りのみ可, CPUアクセス不可. 作成時に初期化の必要あり.
	kDynamic,	//!< GPU読み取りのみ可, CPU書き込みのみ可. CPU側で動的に書き換え.
	kStaging,	//!< GPUからCPUへのデータ転送(コピー)をサポート.
	kNum,
};

//---------------------------------------------------------------------------
//!	@brief		CPUアクセスフラグ.
//---------------------------------------------------------------------------
enum CpuAccessFlag : u32
{
	kCpuAccessFlagRead	= Bit32(0),	//!< リソースをマップしてCPUで読み取り可能.
	kCpuAccessFlagWrite	= Bit32(1),	//!< リソースをマップしてCPUで書き込み可能.
};

//---------------------------------------------------------------------------
//!	@brief		バッファーのバインドパイプラインフラグ.
//---------------------------------------------------------------------------
enum BindFlag : u32
{
	kBindFlagVertexBuffer			= Bit32(0),	//!< 頂点バッファ.
	kBindFlagIndexBuffer			= Bit32(1),	//!< インデックスバッファ.
	kBindFlagConstantBuffer			= Bit32(2),	//!< 定数バッファ.
	kBindFlagShaderResource			= Bit32(3),	//!< シェーダーリソース.
	kBindFlagStreamOutput			= Bit32(4),	//!< ストリーム出力.
	kBindFlagRenderTarget			= Bit32(5),	//!< 描画ターゲット.
	kBindFlagDepthStencil			= Bit32(6),	//!< 深度ステンシル.
	kBindFlagUnorderedAccess		= Bit32(7),	//!< アンオーダードアクセスリソース(UAV).
};

//---------------------------------------------------------------------------
//! @brief		インデックスの形式.
//---------------------------------------------------------------------------
enum class IndexType
{
	kUndefined,		//!< 未定義.
	k16,			//!< 16bit インデックス.
	k32,			//!< 32bit インデックス.
	kNum,
};

//---------------------------------------------------------------------------
//! @brief		入力スロットに格納されているデータの種類.
//---------------------------------------------------------------------------
enum class InputClass
{
	kPerVertex,		//!< 頂点単位データ.
	kPerInstance,	//!< インスタンス単位データ.
	kNum,
};

//---------------------------------------------------------------------------
//! @brief		プリミティブの種類.
//---------------------------------------------------------------------------
enum class PrimitiveType
{
	kUndefined,			//!< 未定義.
	kPointList,			//!< 点.
	kLineList,			//!< ライン.
	kLineStrip,			//!< ラインストリップ.
	kTriangleList,		//!< 三角形.
	kTriangleStrip,		//!< 三角形ストリップ.
	kLineListAdj,		//!< ライン : 隣接情報あり.
	kLineStripAdj,		//!< ラインストリップ : 隣接情報あり.
	kTriangleListAdj,	//!< 三角形 : 隣接情報あり.
	kTriangleStripAdj,	//!< 三角形ストリップ : 隣接情報あり.
	kNum,
};

//---------------------------------------------------------------------------
//! @brief		ビューポート.
//---------------------------------------------------------------------------
struct Viewport
{
	f32 x;
	f32 y;
	f32 w;
	f32 h;
	f32 minDepth;
	f32 maxDepth;

	Viewport()
	{ Default(); }
	Viewport(f32 x, f32 y, f32 w, f32 h, f32 minZ = 0.0f, f32 maxZ = 1.0f) :
		x(x), y(y), w(w), h(h), minDepth(minZ), maxDepth(maxZ)
	{}

	void Default()
	{
		x			= 0.0f;
		y			= 0.0f;
		w			= 0.0f;
		h			= 0.0f;
		minDepth	= 0.0f;
		maxDepth	= 1.0f;
	}

	bool operator==( const Viewport& rhs )
	{
		return memcmp( this, &rhs, sizeof( Viewport ) ) == 0;
	}
	bool operator!=( const Viewport& rhs )
	{
		return !(*this == rhs);
	}
};

//---------------------------------------------------------------------------
//! @brief		シザー矩形.
//---------------------------------------------------------------------------
struct ScissorRect : public data::Rect
{
	ScissorRect()
	{ Default(); }
	void Default()
	{
		x	= 0;
		y	= 0;
		w	= 0;
		h	= 0;
	}

	bool operator==( const ScissorRect& rhs )
	{
		return memcmp( this, &rhs, sizeof( ScissorRect ) ) == 0;;
	}
	bool operator!=( const ScissorRect& rhs )
	{
		return !(*this == rhs);
	}
};

//---------------------------------------------------------------------------
//! @brief		ブレンド係数.
//---------------------------------------------------------------------------
enum class Blend
{
	kZero			= 0,	//!< 
	kOne			= 1,	//!< 
	kSrcCol			= 2,	//!< 
	kInvSrcCol		= 3,	//!< 
	kSrcAlp			= 4,	//!< 
	kInvSrcAlp		= 5,	//!< 
	kDstAlp			= 6,	//!< 
	kInvDstAlp		= 7,	//!< 
	kDstCol			= 8,	//!< 
	kInvDstCol		= 9,	//!< 
	kSrcAlpSAT		= 10,	//!< 
	kBldFct			= 11,	//!< 
	kInvBldFct		= 12,	//!< 
	kSrcOneCol		= 13,	//!< 
	kInvSrcOneCol	= 14,	//!< 
	kSrcOneAlp		= 15,	//!< 
	kInvSrcOneAlp	= 16,	//!< 
	kNum,
};

//---------------------------------------------------------------------------
//! @brief		ブレンドオペレーター.
//---------------------------------------------------------------------------
enum class BlendOp
{
	kAdd			= 0,	//!<
	kSub			= 1,	//!<
	kRevSub			= 2,	//!<
	kMin			= 3,	//!<
	kMax			= 4,	//!<
	kNum,
};

//---------------------------------------------------------------------------
//! @brief		三角形描画モード.
//---------------------------------------------------------------------------
enum class FillMode
{
	kWireframe	= 0,	//!< ワイヤーフレーム.
	kSolid		= 1,	//!< 塗りつぶし.
	kNum,
};

//---------------------------------------------------------------------------
//! @brief		三角形カリングモード.
//---------------------------------------------------------------------------
enum class CullMode
{
	kNone		= 0,	//!< 常に描画.
	kFront		= 1,	//!< 前向きを描画しない.
	kBack		= 2,	//!< 後ろ向きを描画しない.
	kNum,
};

//---------------------------------------------------------------------------
//! @brief		比較関数.
//---------------------------------------------------------------------------
enum class ComparisonFunc
{
	kNever			= 0,	//!< 常に不合格.
	kLess			= 1,	//!< (値) <  refのとき.
	kEqual			= 2,	//!< (値) == refのとき.
	kLessEqual		= 3,	//!< (値) <= refのとき.
	kGreater		= 4,	//!< (値) >  refのとき.
	kNotEqual		= 5,	//!< (値) != refのとき.
	kGreaterEqual	= 6,	//!< (値) >= refのとき.
	kAlways			= 7,	//!< 常に合格.
	kNum,
};

//---------------------------------------------------------------------------
//! @brief		ステンシルオペレーター.
//---------------------------------------------------------------------------
enum class StencilOp
{
	kKeep		= 0,		//!< 変化なし.
	kZero		= 1,		//!< 0クリア.
	kRep		= 2,		//!< 参照値に設定.
	kIncSat		= 3,		//!< インクリメントしてクランプ.
	kDecSat		= 4,		//!< デクリメントしてクランプ.
	kInv		= 5,		//!< 反転.
	kInc		= 6,		//!< インクリメント.
	kDec		= 7,		//!< デクリメント.
	kNum
};

//---------------------------------------------------------------------------
//! @brief		サンプリングフィルター.
//!
//! @details
//!		[Min]			縮小サンプリング.
//!		[Mag]			拡大サンプリング.
//!		[Mip]			ミップレベルサンプリング.
//!
//!		[Point]			ポイントサンプリング.
//!		[Linear]		線形補間.
//!		[Anisotropic]	異方性補間.
//---------------------------------------------------------------------------
enum class Filter
{
	kMinMagMipPoint				= 0,
	kMinMagPointMipLinear		= 1,
	kMinPointMagLinearMipPoint	= 2,
	kMinPointMagMipLinear		= 3,
	kMinLinearMagMipPoint		= 4,
	kMinLinearMagPointMipLinear	= 5,
	kMinMagLinearMipPoint		= 6,
	kMinMagMipLinear			= 7,
	kAnisotropic				= 8,
	kNum
};

//---------------------------------------------------------------------------
//! @brief		境界外テクスチャー座標解決メソッド.
//---------------------------------------------------------------------------
enum class TextureAddress
{
	kWrap		= 0,	//!< 繰り返し.
	kMirror		= 1,	//!< 反転繰り返し.
	kClamp		= 2,	//!< 0.0 - 1.0範囲で切り詰め.
	kBorder		= 3,	//!< サンプラーかシェーダーコードで指定した境界色.
	kMirrorOnce	= 4,	//!< 一度反転, 以降繰り返し無しで切り詰め.
	kNum
};

//---------------------------------------------------------------------------
//! @brief		異方性フィルタリングサンプル数.
//---------------------------------------------------------------------------
enum class AnisotropicRatio
{
	k1X		= 0,		//!< 1x
	k2X		= 1,		//!< 2x
	k4X		= 2,		//!< 4x
	k8X		= 3,		//!< 8x
	k16X	= 4,		//!< 16x
	kNum
};

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIピクセルフォーマット取得.
//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
DXGI_FORMAT ToNativePixelFormat( data::PixelFormat aromaFormat );
#endif

//---------------------------------------------------------------------------
//! @brief		Aromaピクセルフォーマット取得.
//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
data::PixelFormat ToAromaPixelFormat( DXGI_FORMAT nativeFormat );
#endif

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIバッファ用途取得.
//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
D3D11_USAGE ToNativeUsage( Usage aromaUsage );
#endif

//---------------------------------------------------------------------------
//! @brief		Aromaバッファ用途取得.
//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
Usage ToAromaUsage( D3D11_USAGE nativeUsage );
#endif

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIバッファバインドフラグ取得.
//---------------------------------------------------------------------------
u32 ToNativeBindFlags( u32 aromaFlags );

//---------------------------------------------------------------------------
//! @brief		Aromaバッファバインドフラグ取得.
//---------------------------------------------------------------------------
u32 ToAromaBindFlags( u32 nativeFlags );

//---------------------------------------------------------------------------
//! @brief		UsageよりCPUアクセスフラグ取得.
//---------------------------------------------------------------------------
u32 GetCpuAccessFlags( Usage aromaUsage );

//---------------------------------------------------------------------------
//! @brief		ネイティブAPI CPUアクセスフラグ取得.
//---------------------------------------------------------------------------
u32 ToNativeCpuAccessFlag( u32 aromaCpuAccessFlag );

//---------------------------------------------------------------------------
//! @brief		インデックスの形式よりサイズ取得.
//---------------------------------------------------------------------------
size_t GetIndexTypeSize( IndexType aromaIndexType );

//---------------------------------------------------------------------------
//! @brief		バッファのストライドよりインデックス形式取得.
//---------------------------------------------------------------------------
IndexType GetIndexTypeFromBufferStride( size_t stride );

//---------------------------------------------------------------------------
//! @brief		ネイティブAPI入力スロット格納データ種別取得.
//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
D3D11_INPUT_CLASSIFICATION ToNativeInputClass( InputClass aromaInputClass );
#endif

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIプリミティブの種類取得.
//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
D3D11_PRIMITIVE_TOPOLOGY ToNativePrimitiveType( PrimitiveType aromaPrimitiveType );
#endif

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIブレンド係数取得.
//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
D3D11_BLEND ToNativeBlend( Blend aromaBlend );
#endif

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIブレンドオペレーター取得.
//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
D3D11_BLEND_OP ToNativeBlendOp( BlendOp aromaBlendOp );
#endif

//---------------------------------------------------------------------------
//! @brief		ネイティブAPI三角形描画モード取得.
//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
D3D11_FILL_MODE ToNativeFillMode( FillMode aromaFillMode );
#endif

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIカリングモード取得.
//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
D3D11_CULL_MODE ToNativeCullMode( CullMode aromaCullMode );
#endif

//---------------------------------------------------------------------------
//! @brief		ネイティブAPI比較関数取得.
//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
D3D11_COMPARISON_FUNC ToNativeComparisonFunc( ComparisonFunc aromaComparisonFunc );
#endif

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIステンシルオペレーター取得.
//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
D3D11_STENCIL_OP ToNativeStencilOp( StencilOp aromaStencilOp );
#endif

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIサンプリングフィルター取得.
//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
D3D11_FILTER ToNativeFilter( Filter aromaFilter );
#endif

//---------------------------------------------------------------------------
//! @brief		ネイティブAPI境界外テクスチャー座標解決メソッド取得.
//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
D3D11_TEXTURE_ADDRESS_MODE ToNativeTextureAddress( TextureAddress aromaTextureAddress );
#endif

//---------------------------------------------------------------------------
//! @brief		ネイティブAPI異方性フィルタリングサンプル数取得.
//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
u32 ToNativeAnisotropicRatio( AnisotropicRatio aromaAnisotropicRatio );
#endif

//---------------------------------------------------------------------------
//! @brief		異方性フィルタリングサンプル数値取得.
//---------------------------------------------------------------------------
u32 GetAnisotropicRatioValue( AnisotropicRatio aromaAnisotropicRatio );

//---------------------------------------------------------------------------
//! @brief		数値より異方性フィルタリングサンプル数定義取得.
//---------------------------------------------------------------------------
AnisotropicRatio GetAnisotropicRatio( u32 value );

} // namespace render
} // namespace aroma

