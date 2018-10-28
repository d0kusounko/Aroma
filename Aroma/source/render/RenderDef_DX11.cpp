////===========================================================================
//!
//!	@file		RenderDef.h
//!	@brief		描画システム共通定義 : DirectX11.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#ifdef AROMA_RENDER_DX11

#include <aroma/render/RenderDef.h>

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIピクセルフォーマット取得.
//---------------------------------------------------------------------------
DXGI_FORMAT ToNativePixelFormat( data::PixelFormat format )
{
	constexpr DXGI_FORMAT	nativeFormat[] =
	{
		DXGI_FORMAT_UNKNOWN,						// PixelFormat::kUnknown
		DXGI_FORMAT_R32G32B32A32_TYPELESS,			// PixelFormat::kR32G32B32A32Typeless
		DXGI_FORMAT_R32G32B32A32_FLOAT,				// PixelFormat::kR32G32B32A32Float
		DXGI_FORMAT_R32G32B32A32_UINT,				// PixelFormat::kR32G32B32A32Uint
		DXGI_FORMAT_R32G32B32A32_SINT,				// PixelFormat::kR32G32B32A32Sint
		DXGI_FORMAT_R32G32B32_TYPELESS,				// PixelFormat::kR32G32B32Typeless
		DXGI_FORMAT_R32G32B32_FLOAT,				// PixelFormat::kR32G32B32Float
		DXGI_FORMAT_R32G32B32_UINT,					// PixelFormat::kR32G32B32Uint
		DXGI_FORMAT_R32G32B32_SINT,					// PixelFormat::kR32G32B32Sint
		DXGI_FORMAT_R16G16B16A16_TYPELESS,			// PixelFormat::kR16G16B16A16Typeless
		DXGI_FORMAT_R16G16B16A16_FLOAT,				// PixelFormat::kR16G16B16A16Float
		DXGI_FORMAT_R16G16B16A16_UNORM,				// PixelFormat::kR16G16B16A16Unorm
		DXGI_FORMAT_R16G16B16A16_UINT,				// PixelFormat::kR16G16B16A16Uint
		DXGI_FORMAT_R16G16B16A16_SNORM,				// PixelFormat::kR16G16B16A16Snorm
		DXGI_FORMAT_R16G16B16A16_SINT,				// PixelFormat::kR16G16B16A16Sint
		DXGI_FORMAT_R32G32_TYPELESS,				// PixelFormat::kR32G32Typeless
		DXGI_FORMAT_R32G32_FLOAT,					// PixelFormat::kR32G32Float
		DXGI_FORMAT_R32G32_UINT,					// PixelFormat::kR32G32Uint
		DXGI_FORMAT_R32G32_SINT,					// PixelFormat::kR32G32Sint
		DXGI_FORMAT_R32G8X24_TYPELESS,				// PixelFormat::kR32G8X24Typeless
		DXGI_FORMAT_D32_FLOAT_S8X24_UINT,			// PixelFormat::kD32FloatS8X24Uint
		DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS,		// PixelFormat::kR32FloatX8X24Typeless
		DXGI_FORMAT_X32_TYPELESS_G8X24_UINT,		// PixelFormat::kX32TypelessG8X24Uint
		DXGI_FORMAT_R10G10B10A2_TYPELESS,			// PixelFormat::kR10G10B10A2Typeless
		DXGI_FORMAT_R10G10B10A2_UNORM,				// PixelFormat::kR10G10B10A2Unorm
		DXGI_FORMAT_R10G10B10A2_UINT,				// PixelFormat::kR10G10B10A2Uint
		DXGI_FORMAT_R11G11B10_FLOAT,				// PixelFormat::kR11G11B10Float
		DXGI_FORMAT_R8G8B8A8_TYPELESS,				// PixelFormat::kR8G8B8A8Typeless
		DXGI_FORMAT_R8G8B8A8_UNORM,					// PixelFormat::kR8G8B8A8Unorm
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,			// PixelFormat::kR8G8B8A8UnormSrgb
		DXGI_FORMAT_R8G8B8A8_UINT,					// PixelFormat::kR8G8B8A8Uint
		DXGI_FORMAT_R8G8B8A8_SNORM,					// PixelFormat::kR8G8B8A8Snorm
		DXGI_FORMAT_R8G8B8A8_SINT,					// PixelFormat::kR8G8B8A8Sint
		DXGI_FORMAT_R16G16_TYPELESS,				// PixelFormat::kR16G16Typeless
		DXGI_FORMAT_R16G16_FLOAT,					// PixelFormat::kR16G16Float
		DXGI_FORMAT_R16G16_UNORM,					// PixelFormat::kR16G16Unorm
		DXGI_FORMAT_R16G16_UINT,					// PixelFormat::kR16G16Uint
		DXGI_FORMAT_R16G16_SNORM,					// PixelFormat::kR16G16Snorm
		DXGI_FORMAT_R16G16_SINT,					// PixelFormat::kR16G16Sint
		DXGI_FORMAT_R32_TYPELESS,					// PixelFormat::kR32Typeless
		DXGI_FORMAT_D32_FLOAT,						// PixelFormat::kD32Float
		DXGI_FORMAT_R32_FLOAT,						// PixelFormat::kR32Float
		DXGI_FORMAT_R32_UINT,						// PixelFormat::kR32Uint
		DXGI_FORMAT_R32_SINT,						// PixelFormat::kR32Sint
		DXGI_FORMAT_R24G8_TYPELESS,					// PixelFormat::kR24G8Typeless
		DXGI_FORMAT_D24_UNORM_S8_UINT,				// PixelFormat::kD24UnormS8Uint
		DXGI_FORMAT_R24_UNORM_X8_TYPELESS,			// PixelFormat::kR24UnormX8Typeless
		DXGI_FORMAT_X24_TYPELESS_G8_UINT,			// PixelFormat::kX24TypelessG8Uint
		DXGI_FORMAT_R8G8_TYPELESS,					// PixelFormat::kR8G8Typeless
		DXGI_FORMAT_R8G8_UNORM,						// PixelFormat::kR8G8Unorm
		DXGI_FORMAT_R8G8_UINT,						// PixelFormat::kR8G8Uint
		DXGI_FORMAT_R8G8_SNORM,						// PixelFormat::kR8G8Snorm
		DXGI_FORMAT_R8G8_SINT,						// PixelFormat::kR8G8Sint
		DXGI_FORMAT_R16_TYPELESS,					// PixelFormat::kR16Typeless
		DXGI_FORMAT_R16_FLOAT,						// PixelFormat::kR16Float
		DXGI_FORMAT_D16_UNORM,						// PixelFormat::kD16Unorm
		DXGI_FORMAT_R16_UNORM,						// PixelFormat::kR16Unorm
		DXGI_FORMAT_R16_UINT,						// PixelFormat::kR16Uint
		DXGI_FORMAT_R16_SNORM,						// PixelFormat::kR16Snorm
		DXGI_FORMAT_R16_SINT,						// PixelFormat::kR16Sint
		DXGI_FORMAT_R8_TYPELESS,					// PixelFormat::kR8Typeless
		DXGI_FORMAT_R8_UNORM,						// PixelFormat::kR8Unorm
		DXGI_FORMAT_R8_UINT,						// PixelFormat::kR8Uint
		DXGI_FORMAT_R8_SNORM,						// PixelFormat::kR8Snorm
		DXGI_FORMAT_R8_SINT,						// PixelFormat::kR8Sint
		DXGI_FORMAT_A8_UNORM,						// PixelFormat::kA8Unorm
		DXGI_FORMAT_R1_UNORM,						// PixelFormat::kR1Unorm
		DXGI_FORMAT_R9G9B9E5_SHAREDEXP,				// PixelFormat::kR9G9B9E5SharedExp
		DXGI_FORMAT_R8G8_B8G8_UNORM,				// PixelFormat::kR8G8B8G8Unorm
		DXGI_FORMAT_G8R8_G8B8_UNORM,				// PixelFormat::kG8R8G8B8Unorm
		DXGI_FORMAT_BC1_TYPELESS,					// PixelFormat::kBC1Typeless
		DXGI_FORMAT_BC1_UNORM,						// PixelFormat::kBC1Unorm
		DXGI_FORMAT_BC1_UNORM_SRGB,					// PixelFormat::kBC1UnormSrgb
		DXGI_FORMAT_BC2_TYPELESS,					// PixelFormat::kBC2Typeless
		DXGI_FORMAT_BC2_UNORM,						// PixelFormat::kBC2Unorm
		DXGI_FORMAT_BC2_UNORM_SRGB,					// PixelFormat::kBC2UnormSrgb
		DXGI_FORMAT_BC3_TYPELESS,					// PixelFormat::kBC3Typeless
		DXGI_FORMAT_BC3_UNORM,						// PixelFormat::kBC3Unorm
		DXGI_FORMAT_BC3_UNORM_SRGB,					// PixelFormat::kBC3UnormSrgb
		DXGI_FORMAT_BC4_TYPELESS,					// PixelFormat::kBC4Typeless
		DXGI_FORMAT_BC4_UNORM,						// PixelFormat::kBC4Unorm
		DXGI_FORMAT_BC4_SNORM,						// PixelFormat::kBC4Snorm
		DXGI_FORMAT_BC5_TYPELESS,					// PixelFormat::kBC5Typeless
		DXGI_FORMAT_BC5_UNORM,						// PixelFormat::kBC5Unorm
		DXGI_FORMAT_BC5_SNORM,						// PixelFormat::kBC5Snorm
		DXGI_FORMAT_B5G6R5_UNORM,					// PixelFormat::kB5G6R5Unorm
		DXGI_FORMAT_B5G5R5A1_UNORM,					// PixelFormat::kB5G5R5A1Unorm
		DXGI_FORMAT_B8G8R8A8_UNORM,					// PixelFormat::kB8G8R8A8Unorm
		DXGI_FORMAT_B8G8R8X8_UNORM,					// PixelFormat::kB8G8R8X8Unorm
		DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM,		// PixelFormat::kR10G10B10XRBIASA2Unorm
		DXGI_FORMAT_B8G8R8A8_TYPELESS,				// PixelFormat::kB8G8R8A8Typeless
		DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,			// PixelFormat::kB8G8R8A8UnormSrgb
		DXGI_FORMAT_B8G8R8X8_TYPELESS,				// PixelFormat::kB8G8R8X8Typeless
		DXGI_FORMAT_B8G8R8X8_UNORM_SRGB,			// PixelFormat::kB8G8R8X8UnormSrgb
		DXGI_FORMAT_BC6H_TYPELESS,					// PixelFormat::kBC6HTypeless
		DXGI_FORMAT_BC6H_UF16,						// PixelFormat::kBC6HUF16
		DXGI_FORMAT_BC6H_SF16,						// PixelFormat::kBC6HSF16
		DXGI_FORMAT_BC7_TYPELESS,					// PixelFormat::kBC7Typeless
		DXGI_FORMAT_BC7_UNORM,						// PixelFormat::kBC7Unorm
		DXGI_FORMAT_BC7_UNORM_SRGB,					// PixelFormat::kBC7UnormSrgb
	};
	AROMA_STATIC_ASSERT( AROMA_ARRAY_OF( nativeFormat ) == ( u32 )data::PixelFormat::kNum, _T( "Array length mismatch." ) );

	// 未定義フォーマットチェック.
	AROMA_ASSERT( nativeFormat[ ( u32 )format ] != DXGI_FORMAT_UNKNOWN, "Undefined format." );

	return nativeFormat[ ( u32 )format ];
}

//---------------------------------------------------------------------------
//! @brief		Aromaピクセルフォーマット取得.
//---------------------------------------------------------------------------
data::PixelFormat ToAromaPixelFormat( DXGI_FORMAT format )
{
	constexpr data::PixelFormat	aromaFormat[] =
	{
		data::PixelFormat::kUnknown,				// DXGI_FORMAT_UNKNOWN
		data::PixelFormat::kR32G32B32A32Typeless,	// DXGI_FORMAT_R32G32B32A32_TYPELESS
		data::PixelFormat::kR32G32B32A32Float,		// DXGI_FORMAT_R32G32B32A32_FLOAT
		data::PixelFormat::kR32G32B32A32Uint,		// DXGI_FORMAT_R32G32B32A32_UINT
		data::PixelFormat::kR32G32B32A32Sint,		// DXGI_FORMAT_R32G32B32A32_SINT
		data::PixelFormat::kR32G32B32Typeless,		// DXGI_FORMAT_R32G32B32_TYPELESS
		data::PixelFormat::kR32G32B32Float,			// DXGI_FORMAT_R32G32B32_FLOAT
		data::PixelFormat::kR32G32B32Uint,			// DXGI_FORMAT_R32G32B32_UINT
		data::PixelFormat::kR32G32B32Sint,			// DXGI_FORMAT_R32G32B32_SINT
		data::PixelFormat::kR16G16B16A16Typeless,	// DXGI_FORMAT_R16G16B16A16_TYPELESS
		data::PixelFormat::kR16G16B16A16Float,		// DXGI_FORMAT_R16G16B16A16_FLOAT
		data::PixelFormat::kR16G16B16A16Unorm,		// DXGI_FORMAT_R16G16B16A16_UNORM
		data::PixelFormat::kR16G16B16A16Uint,		// DXGI_FORMAT_R16G16B16A16_UINT
		data::PixelFormat::kR16G16B16A16Snorm,		// DXGI_FORMAT_R16G16B16A16_SNORM
		data::PixelFormat::kR16G16B16A16Sint,		// DXGI_FORMAT_R16G16B16A16_SINT
		data::PixelFormat::kR32G32Typeless,			// DXGI_FORMAT_R32G32_TYPELESS
		data::PixelFormat::kR32G32Float,			// DXGI_FORMAT_R32G32_FLOAT
		data::PixelFormat::kR32G32Uint,				// DXGI_FORMAT_R32G32_UINT
		data::PixelFormat::kR32G32Sint,				// DXGI_FORMAT_R32G32_SINT
		data::PixelFormat::kR32G8X24Typeless,		// DXGI_FORMAT_R32G8X24_TYPELESS
		data::PixelFormat::kD32FloatS8X24Uint,		// DXGI_FORMAT_D32_FLOAT_S8X24_UINT
		data::PixelFormat::kR32FloatX8X24Typeless,	// DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS
		data::PixelFormat::kX32TypelessG8X24Uint,	// DXGI_FORMAT_X32_TYPELESS_G8X24_UINT
		data::PixelFormat::kR10G10B10A2Typeless,	// DXGI_FORMAT_R10G10B10A2_TYPELESS
		data::PixelFormat::kR10G10B10A2Unorm,		// DXGI_FORMAT_R10G10B10A2_UNORM
		data::PixelFormat::kR10G10B10A2Uint,		// DXGI_FORMAT_R10G10B10A2_UINT
		data::PixelFormat::kR11G11B10Float,			// DXGI_FORMAT_R11G11B10_FLOAT
		data::PixelFormat::kR8G8B8A8Typeless,		// DXGI_FORMAT_R8G8B8A8_TYPELESS
		data::PixelFormat::kR8G8B8A8Unorm,			// DXGI_FORMAT_R8G8B8A8_UNORM
		data::PixelFormat::kR8G8B8A8UnormSrgb,		// DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
		data::PixelFormat::kR8G8B8A8Uint,			// DXGI_FORMAT_R8G8B8A8_UINT
		data::PixelFormat::kR8G8B8A8Snorm,			// DXGI_FORMAT_R8G8B8A8_SNORM
		data::PixelFormat::kR8G8B8A8Sint,			// DXGI_FORMAT_R8G8B8A8_SINT
		data::PixelFormat::kR16G16Typeless,			// DXGI_FORMAT_R16G16_TYPELESS
		data::PixelFormat::kR16G16Float,			// DXGI_FORMAT_R16G16_FLOAT
		data::PixelFormat::kR16G16Unorm,			// DXGI_FORMAT_R16G16_UNORM
		data::PixelFormat::kR16G16Uint,				// DXGI_FORMAT_R16G16_UINT
		data::PixelFormat::kR16G16Snorm,			// DXGI_FORMAT_R16G16_SNORM
		data::PixelFormat::kR16G16Sint,				// DXGI_FORMAT_R16G16_SINT
		data::PixelFormat::kR32Typeless,			// DXGI_FORMAT_R32_TYPELESS
		data::PixelFormat::kD32Float,				// DXGI_FORMAT_D32_FLOAT
		data::PixelFormat::kR32Float,				// DXGI_FORMAT_R32_FLOAT
		data::PixelFormat::kR32Uint,				// DXGI_FORMAT_R32_UINT
		data::PixelFormat::kR32Sint,				// DXGI_FORMAT_R32_SINT
		data::PixelFormat::kR24G8Typeless,			// DXGI_FORMAT_R24G8_TYPELESS
		data::PixelFormat::kD24UnormS8Uint,			// DXGI_FORMAT_D24_UNORM_S8_UINT
		data::PixelFormat::kR24UnormX8Typeless,		// DXGI_FORMAT_R24_UNORM_X8_TYPELESS
		data::PixelFormat::kX24TypelessG8Uint,		// DXGI_FORMAT_X24_TYPELESS_G8_UINT
		data::PixelFormat::kR8G8Typeless,			// DXGI_FORMAT_R8G8_TYPELESS
		data::PixelFormat::kR8G8Unorm,				// DXGI_FORMAT_R8G8_UNORM
		data::PixelFormat::kR8G8Uint,				// DXGI_FORMAT_R8G8_UINT
		data::PixelFormat::kR8G8Snorm,				// DXGI_FORMAT_R8G8_SNORM
		data::PixelFormat::kR8G8Sint,				// DXGI_FORMAT_R8G8_SINT
		data::PixelFormat::kR16Typeless,			// DXGI_FORMAT_R16_TYPELESS
		data::PixelFormat::kR16Float,				// DXGI_FORMAT_R16_FLOAT
		data::PixelFormat::kD16Unorm,				// DXGI_FORMAT_D16_UNORM
		data::PixelFormat::kR16Unorm,				// DXGI_FORMAT_R16_UNORM
		data::PixelFormat::kR16Uint,				// DXGI_FORMAT_R16_UINT
		data::PixelFormat::kR16Snorm,				// DXGI_FORMAT_R16_SNORM
		data::PixelFormat::kR16Sint,				// DXGI_FORMAT_R16_SINT
		data::PixelFormat::kR8Typeless,				// DXGI_FORMAT_R8_TYPELESS
		data::PixelFormat::kR8Unorm,				// DXGI_FORMAT_R8_UNORM
		data::PixelFormat::kR8Uint,					// DXGI_FORMAT_R8_UINT
		data::PixelFormat::kR8Snorm,				// DXGI_FORMAT_R8_SNORM
		data::PixelFormat::kR8Sint,					// DXGI_FORMAT_R8_SINT
		data::PixelFormat::kA8Unorm,				// DXGI_FORMAT_A8_UNORM
		data::PixelFormat::kR1Unorm,				// DXGI_FORMAT_R1_UNORM
		data::PixelFormat::kR9G9B9E5SharedExp,		// DXGI_FORMAT_R9G9B9E5_SHAREDEXP
		data::PixelFormat::kR8G8B8G8Unorm,			// DXGI_FORMAT_R8G8_B8G8_UNORM
		data::PixelFormat::kG8R8G8B8Unorm,			// DXGI_FORMAT_G8R8_G8B8_UNORM
		data::PixelFormat::kBC1Typeless,			// DXGI_FORMAT_BC1_TYPELESS
		data::PixelFormat::kBC1Unorm,				// DXGI_FORMAT_BC1_UNORM
		data::PixelFormat::kBC1UnormSrgb,			// DXGI_FORMAT_BC1_UNORM_SRGB
		data::PixelFormat::kBC2Typeless,			// DXGI_FORMAT_BC2_TYPELESS
		data::PixelFormat::kBC2Unorm,				// DXGI_FORMAT_BC2_UNORM
		data::PixelFormat::kBC2UnormSrgb,			// DXGI_FORMAT_BC2_UNORM_SRGB
		data::PixelFormat::kBC3Typeless,			// DXGI_FORMAT_BC3_TYPELESS
		data::PixelFormat::kBC3Unorm,				// DXGI_FORMAT_BC3_UNORM
		data::PixelFormat::kBC3UnormSrgb,			// DXGI_FORMAT_BC3_UNORM_SRGB
		data::PixelFormat::kBC4Typeless,			// DXGI_FORMAT_BC4_TYPELESS
		data::PixelFormat::kBC4Unorm,				// DXGI_FORMAT_BC4_UNORM
		data::PixelFormat::kBC4Snorm,				// DXGI_FORMAT_BC4_SNORM
		data::PixelFormat::kBC5Typeless,			// DXGI_FORMAT_BC5_TYPELESS
		data::PixelFormat::kBC5Unorm,				// DXGI_FORMAT_BC5_UNORM
		data::PixelFormat::kBC5Snorm,				// DXGI_FORMAT_BC5_SNORM
		data::PixelFormat::kB5G6R5Unorm,			// DXGI_FORMAT_B5G6R5_UNORM
		data::PixelFormat::kB5G5R5A1Unorm,			// DXGI_FORMAT_B5G5R5A1_UNORM
		data::PixelFormat::kB8G8R8A8Unorm,			// DXGI_FORMAT_B8G8R8A8_UNORM
		data::PixelFormat::kB8G8R8X8Unorm,			// DXGI_FORMAT_B8G8R8X8_UNORM
		data::PixelFormat::kR10G10B10XRBiasA2Unorm,	// DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM
		data::PixelFormat::kB8G8R8A8Typeless,		// DXGI_FORMAT_B8G8R8A8_TYPELESS
		data::PixelFormat::kB8G8R8A8UnormSrgb,		// DXGI_FORMAT_B8G8R8A8_UNORM_SRGB
		data::PixelFormat::kB8G8R8X8Typeless,		// DXGI_FORMAT_B8G8R8X8_TYPELESS
		data::PixelFormat::kB8G8R8X8UnormSrgb,		// DXGI_FORMAT_B8G8R8X8_UNORM_SRGB
		data::PixelFormat::kBC6HTypeless,			// DXGI_FORMAT_BC6H_TYPELESS
		data::PixelFormat::kBC6HUF16,				// DXGI_FORMAT_BC6H_UF16
		data::PixelFormat::kBC6HSF16,				// DXGI_FORMAT_BC6H_SF16
		data::PixelFormat::kBC7Typeless,			// DXGI_FORMAT_BC7_TYPELESS
		data::PixelFormat::kBC7Unorm,				// DXGI_FORMAT_BC7_UNORM
		data::PixelFormat::kBC7UnormSrgb,			// DXGI_FORMAT_BC7_UNORM_SRGB
	};
	// 未定義フォーマットチェック.
	AROMA_ASSERT( ( u32 )format >= 0 && ( u32 )format < AROMA_ARRAY_OF( aromaFormat ), "Undefined format." );
	AROMA_ASSERT( aromaFormat[ ( u32 )format ] != data::PixelFormat::kUnknown, "Undefined format." );

	return aromaFormat[ ( u32 )format ];
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIバッファ用途取得.
//---------------------------------------------------------------------------
D3D11_USAGE ToNativeUsage( Usage usage )
{
	constexpr D3D11_USAGE	nativeUsage[] =
	{
		D3D11_USAGE_DEFAULT,			// Usage::kDefault
		D3D11_USAGE_IMMUTABLE,			// Usage::kImmutable
		D3D11_USAGE_DYNAMIC,			// Usage::kDynamic
		D3D11_USAGE_STAGING,			// Usage::kStaging
	};
	AROMA_STATIC_ASSERT( AROMA_ARRAY_OF( nativeUsage ) == ( u32 )Usage::kNum, _T( "Array length mismatch." ) );

	return nativeUsage[ ( u32 )usage ];
}

//---------------------------------------------------------------------------
//! @brief		Aromaバッファ用途取得.
//---------------------------------------------------------------------------
Usage ToAromaUsage( D3D11_USAGE usage )
{
	constexpr Usage	aromaUsage[] =
	{
		Usage::kDefault,		// D3D11_USAGE_DEFAULT
		Usage::kImmutable,		// D3D11_USAGE_IMMUTABLE
		Usage::kDynamic,		// D3D11_USAGE_DYNAMIC
		Usage::kStaging,		// D3D11_USAGE_STAGING
	};
	// 未定義フォーマットチェック.
	AROMA_ASSERT( ( u32 )usage >= 0 && ( u32 )usage < AROMA_ARRAY_OF( aromaUsage ), "Undefined usage." );

	return aromaUsage[ ( u32 )usage ];
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIバッファバインドフラグ取得.
//---------------------------------------------------------------------------
u32 ToNativeBindFlags( u32 aromaFlags )
{
	u32	nativeFlags = 0;
	if( aromaFlags & kBindFlagVertexBuffer )		nativeFlags |= D3D11_BIND_VERTEX_BUFFER;
	if( aromaFlags & kBindFlagIndexBuffer )			nativeFlags |= D3D11_BIND_INDEX_BUFFER;
	if( aromaFlags & kBindFlagConstantBuffer )		nativeFlags |= D3D11_BIND_CONSTANT_BUFFER;
	if( aromaFlags & kBindFlagShaderResource )		nativeFlags |= D3D11_BIND_SHADER_RESOURCE;
	if( aromaFlags & kBindFlagStreamOutput )		nativeFlags |= D3D11_BIND_STREAM_OUTPUT;
	if( aromaFlags & kBindFlagRenderTarget )		nativeFlags |= D3D11_BIND_RENDER_TARGET;
	if( aromaFlags & kBindFlagDepthStencil )		nativeFlags |= D3D11_BIND_DEPTH_STENCIL;
	if( aromaFlags & kBindFlagUnorderedAccess )		nativeFlags |= D3D11_BIND_UNORDERED_ACCESS;
	return nativeFlags;
}

//---------------------------------------------------------------------------
//! @brief		Aromaバッファバインドフラグ取得.
//---------------------------------------------------------------------------
u32 ToAromaBindFlags( u32 nativeFlags )
{
	u32	aromaFlags = 0;
	if( nativeFlags & ( u32 )D3D11_BIND_VERTEX_BUFFER )		aromaFlags |= kBindFlagVertexBuffer;
	if( nativeFlags & ( u32 )D3D11_BIND_INDEX_BUFFER )		aromaFlags |= kBindFlagIndexBuffer;
	if( nativeFlags & ( u32 )D3D11_BIND_CONSTANT_BUFFER )	aromaFlags |= kBindFlagConstantBuffer;
	if( nativeFlags & ( u32 )D3D11_BIND_SHADER_RESOURCE )	aromaFlags |= kBindFlagShaderResource;
	if( nativeFlags & ( u32 )D3D11_BIND_STREAM_OUTPUT )		aromaFlags |= kBindFlagStreamOutput;
	if( nativeFlags & ( u32 )D3D11_BIND_RENDER_TARGET )		aromaFlags |= kBindFlagRenderTarget;
	if( nativeFlags & ( u32 )D3D11_BIND_DEPTH_STENCIL )		aromaFlags |= kBindFlagDepthStencil;
	if( nativeFlags & ( u32 )D3D11_BIND_UNORDERED_ACCESS )	aromaFlags |= kBindFlagUnorderedAccess;
	return aromaFlags;
}

//---------------------------------------------------------------------------
//! @brief		UsageよりCPUアクセスフラグ取得.
//---------------------------------------------------------------------------
u32 GetCpuAccessFlag( Usage aromaUsage )
{
	constexpr u32 cpuAccessFlags[] =
	{
		0,												// Usage::kDefault
		0,												// Usage::kImmutable
		kCpuAccessFlagWrite,							// Usage::kDynamic
		kCpuAccessFlagWrite | kCpuAccessFlagRead,		// Usage::kStaging
	};
	AROMA_STATIC_ASSERT( AROMA_ARRAY_OF( cpuAccessFlags ) == ( u32 )Usage::kNum, _T( "Array length mismatch." ) );

	return cpuAccessFlags[ ( u32 )aromaUsage ];
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPI CPUアクセスフラグ取得.
//---------------------------------------------------------------------------
u32 ToNativeCpuAccessFlag( u32 aromaFlags )
{
	u32	nativeFlags = 0;
	if( aromaFlags & kCpuAccessFlagRead )			nativeFlags |= D3D11_CPU_ACCESS_READ;
	if( aromaFlags & kCpuAccessFlagWrite )			nativeFlags |= D3D11_CPU_ACCESS_WRITE;
	return nativeFlags;
}

//---------------------------------------------------------------------------
//! @brief		インデックスの形式よりサイズ取得.
//---------------------------------------------------------------------------
size_t GetIndexTypeSize( IndexType indexType )
{
	constexpr size_t indexTypeSizes[] =
	{
		0,					// IndexType::kUndefined
		sizeof( u16 ),		// IndexType::k16
		sizeof( u32 ),		// IndexType::k32
	};
	AROMA_STATIC_ASSERT( AROMA_ARRAY_OF( indexTypeSizes ) == ( u32 )IndexType::kNum, _T( "Array length mismatch." ));

	return indexTypeSizes[ ( u32 )indexType ];
}

//---------------------------------------------------------------------------
//! @brief		バッファのストライドよりインデックス形式取得.
//---------------------------------------------------------------------------
IndexType GetIndexTypeFromBufferStride( size_t stride )
{
	switch( stride )
	{
		case sizeof( u16 ):
			return IndexType::k16;
		case sizeof( u32 ):
			return IndexType::k32;
	}

	AROMA_ASSERT( false, _T( "The index type corresponding to this stride is not defined.\n" ) );
	return IndexType::kUndefined;
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPI入力スロット格納データ種別取得.
//---------------------------------------------------------------------------
D3D11_INPUT_CLASSIFICATION ToNativeInputClass( InputClass aromaInputClass )
{
	constexpr D3D11_INPUT_CLASSIFICATION intputClasses[] =
	{
		D3D11_INPUT_PER_VERTEX_DATA,		// InputClass::kPerVertex
		D3D11_INPUT_PER_INSTANCE_DATA,		// InputClass::kPerInstance
	};
	AROMA_STATIC_ASSERT( AROMA_ARRAY_OF( intputClasses ) == ( u32 )InputClass::kNum, _T( "Array length mismatch." ));

	return intputClasses[ ( u32 )aromaInputClass ];
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIプリミティブの種類取得.
//---------------------------------------------------------------------------
D3D11_PRIMITIVE_TOPOLOGY ToNativePrimitiveType( PrimitiveType aromaPrimitiveType )
{
	constexpr D3D11_PRIMITIVE_TOPOLOGY primitiveTypes[] =
	{
		D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,			// kUndefined
		D3D_PRIMITIVE_TOPOLOGY_POINTLIST,			// kPointList
		D3D_PRIMITIVE_TOPOLOGY_LINELIST,			// kLineList
		D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,			// kLineStrip
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,		// kTriangleList
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,		// kTriangleStrip
		D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,		// kLineListAdj
		D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,		// kLineStripAdj
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,	// kTriangleListAdj
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ,	// kTriangleStripAdj
	};
	AROMA_STATIC_ASSERT( AROMA_ARRAY_OF( primitiveTypes ) == ( u32 )PrimitiveType::kNum, _T( "Array length mismatch." ));
	return primitiveTypes[ ( u32 )aromaPrimitiveType ];
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIブレンド係数取得.
//---------------------------------------------------------------------------
D3D11_BLEND ToNativeBlend( Blend aromaBlend )
{
	constexpr D3D11_BLEND nativeBlends[] =
	{
		D3D11_BLEND_ZERO,				// Blend::kZero
		D3D11_BLEND_ONE,				// Blend::kOne
		D3D11_BLEND_SRC_COLOR,			// Blend::kSrcCol
		D3D11_BLEND_INV_SRC_COLOR,		// Blend::kInvSrcCol
		D3D11_BLEND_SRC_ALPHA,			// Blend::kSrcAlp
		D3D11_BLEND_INV_SRC_ALPHA,		// Blend::kInvSrcAlp
		D3D11_BLEND_DEST_ALPHA,			// Blend::kDstAlp
		D3D11_BLEND_INV_DEST_ALPHA,		// Blend::kInvDstAlp
		D3D11_BLEND_DEST_COLOR,			// Blend::kDstCol
		D3D11_BLEND_INV_DEST_COLOR,		// Blend::kInvDstCol
		D3D11_BLEND_SRC_ALPHA_SAT,		// Blend::kSrcAlpSAT
		D3D11_BLEND_BLEND_FACTOR,		// Blend::kBldFct
		D3D11_BLEND_INV_BLEND_FACTOR,	// Blend::kInvBldFct
		D3D11_BLEND_SRC1_COLOR,			// Blend::kSrcOneCol
		D3D11_BLEND_INV_SRC1_COLOR,		// Blend::kInvSrcOneCol
		D3D11_BLEND_SRC1_ALPHA,			// Blend::kSrcOneAlp
		D3D11_BLEND_INV_SRC1_ALPHA,		// Blend::kInvSrcOneAlp
	};
	AROMA_STATIC_ASSERT( AROMA_ARRAY_OF( nativeBlends ) == ( u32 )Blend::kNum, _T( "Array length mismatch." ));
	return nativeBlends[ ( u32 )aromaBlend ];
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIブレンドオペレーション取得.
//---------------------------------------------------------------------------
D3D11_BLEND_OP ToNativeBlendOp( BlendOp aromaBlendOp )
{
	constexpr D3D11_BLEND_OP nativeBlendOps[] =
	{
		D3D11_BLEND_OP_ADD,				// BlendOp::kAdd
		D3D11_BLEND_OP_SUBTRACT,		// BlendOp::kSub
		D3D11_BLEND_OP_REV_SUBTRACT,	// BlendOp::kRevSub
		D3D11_BLEND_OP_MIN,				// BlendOp::kMin
		D3D11_BLEND_OP_MAX,				// BlendOp::kMax
	};
	AROMA_STATIC_ASSERT( AROMA_ARRAY_OF( nativeBlendOps ) == ( u32 )BlendOp::kNum, _T( "Array length mismatch." ));
	return nativeBlendOps[ ( u32 )aromaBlendOp ];
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPI三角形描画モード取得.
//---------------------------------------------------------------------------
D3D11_FILL_MODE ToNativeFillMode( FillMode aromaFillMode )
{
	constexpr D3D11_FILL_MODE nativeFillModes[] =
	{
		D3D11_FILL_WIREFRAME,	// FillMode::kWireframe
		D3D11_FILL_SOLID,		// FillMode::kSolid
	};
	AROMA_STATIC_ASSERT( AROMA_ARRAY_OF( nativeFillModes ) == ( u32 )FillMode::kNum, _T( "Array length mismatch." ));
	return nativeFillModes[ ( u32 )aromaFillMode ];
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIカリングモード取得.
//---------------------------------------------------------------------------
D3D11_CULL_MODE ToNativeCullMode( CullMode aromaCullMode )
{
	constexpr D3D11_CULL_MODE nativeCullModes[] =
	{
		D3D11_CULL_NONE,		// CullMode::kNone
		D3D11_CULL_FRONT,		// CullMode::kFront
		D3D11_CULL_BACK,		// CullMode::kBack
	};
	AROMA_STATIC_ASSERT( AROMA_ARRAY_OF( nativeCullModes ) == ( u32 )CullMode::kNum, _T( "Array length mismatch." ));
	return nativeCullModes[ ( u32 )aromaCullMode ];
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPI比較関数取得.
//---------------------------------------------------------------------------
D3D11_COMPARISON_FUNC ToNativeComparisonFunc( ComparisonFunc aromaComparisonFunc )
{
	constexpr D3D11_COMPARISON_FUNC nativeComparisonFuncs[] =
	{
		D3D11_COMPARISON_NEVER,			// kNever
		D3D11_COMPARISON_LESS,			// kLess
		D3D11_COMPARISON_EQUAL,			// kEqual
		D3D11_COMPARISON_LESS_EQUAL,	// kLessEqual
		D3D11_COMPARISON_GREATER,		// kGreater
		D3D11_COMPARISON_NOT_EQUAL,		// kNotEqual
		D3D11_COMPARISON_GREATER_EQUAL,	// kGreaterEqual
		D3D11_COMPARISON_ALWAYS,		// kAlways
	};
	AROMA_STATIC_ASSERT( AROMA_ARRAY_OF( nativeComparisonFuncs ) == ( u32 )ComparisonFunc::kNum, _T( "Array length mismatch." ));
	return nativeComparisonFuncs[ ( u32 )aromaComparisonFunc ];
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIステンシルオペレーター取得.
//---------------------------------------------------------------------------
D3D11_STENCIL_OP ToNativeStencilOp( StencilOp aromaStencilOp )
{
	constexpr D3D11_STENCIL_OP nativeStencilOps[] =
	{
		D3D11_STENCIL_OP_KEEP,			// kKeep
		D3D11_STENCIL_OP_ZERO,			// kZero
		D3D11_STENCIL_OP_REPLACE,		// kRep
		D3D11_STENCIL_OP_INCR_SAT,		// kIncSat
		D3D11_STENCIL_OP_DECR_SAT,		// kDecSat
		D3D11_STENCIL_OP_INVERT,		// kInv
		D3D11_STENCIL_OP_INCR,			// kInc
		D3D11_STENCIL_OP_DECR,			// kDec
	};
	AROMA_STATIC_ASSERT( AROMA_ARRAY_OF( nativeStencilOps ) == ( u32 )StencilOp::kNum, _T( "Array length mismatch." ));
	return nativeStencilOps[ ( u32 )aromaStencilOp ];
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIサンプリングフィルター取得.
//---------------------------------------------------------------------------
D3D11_FILTER ToNativeFilter( Filter aromaFilter )
{
	constexpr D3D11_FILTER nativeFilters[] =
	{
		D3D11_FILTER_MIN_MAG_MIP_POINT,						//!< Filter::kMinMagMipPoint
		D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,				//!< Filter::kMinMagPointMipLinear
		D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,		//!< Filter::kMinPointMagLinearMipPoint
		D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,				//!< Filter::kMinPointMagMipLinear
		D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,				//!< Filter::kMinLinearMagMipPoint
		D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,		//!< Filter::kMinLinearMagPointMipLinear
		D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,				//!< Filter::kMinMagLinearMipPoint
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,					//!< Filter::kMinMagMipLinear
		D3D11_FILTER_ANISOTROPIC,							//!< Filter::kAnisotropic
	};
	AROMA_STATIC_ASSERT( AROMA_ARRAY_OF( nativeFilters ) == ( u32 )Filter::kNum, _T( "Array length mismatch." ));
	return nativeFilters[ ( u32 )aromaFilter ];
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPI境界外テクスチャー座標解決メソッド取得.
//---------------------------------------------------------------------------
D3D11_TEXTURE_ADDRESS_MODE ToNativeTextureAddress( TextureAddress aromaTextureAddress )
{
	constexpr D3D11_TEXTURE_ADDRESS_MODE nativeTextureAddresses[] =
	{
        D3D11_TEXTURE_ADDRESS_WRAP,			//!< kWrap
        D3D11_TEXTURE_ADDRESS_MIRROR,		//!< kMirror
        D3D11_TEXTURE_ADDRESS_CLAMP,		//!< kClamp
        D3D11_TEXTURE_ADDRESS_BORDER,		//!< kBorder
        D3D11_TEXTURE_ADDRESS_MIRROR_ONCE,	//!< kMirrorOnce
	};
	AROMA_STATIC_ASSERT( AROMA_ARRAY_OF( nativeTextureAddresses ) == ( u32 )TextureAddress::kNum, _T( "Array length mismatch." ));
	return nativeTextureAddresses[ ( u32 )aromaTextureAddress ];
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPI異方性フィルタリングサンプル数取得.
//---------------------------------------------------------------------------
u32 ToNativeAnisotropicRatio( AnisotropicRatio aromaAnisotropicRatio )
{
	constexpr u32 nativeAnisotropicRatios[] =
	{
		1,	//!< k1X
		2,	//!< k2X
		4,	//!< k4X
		8,	//!< k8X
		16,	//!< k16X
	};
	AROMA_STATIC_ASSERT( AROMA_ARRAY_OF( nativeAnisotropicRatios ) == ( u32 )AnisotropicRatio::kNum, _T( "Array length mismatch." ));
	return nativeAnisotropicRatios[ ( u32 )aromaAnisotropicRatio ];
}

//---------------------------------------------------------------------------
//! @brief		異方性フィルタリングサンプル数値取得.
//---------------------------------------------------------------------------
u32 GetAnisotropicRatioValue( AnisotropicRatio aromaAnisotropicRatio )
{
	constexpr u32 anisotropicRatioValues[] =
	{
		1,	//!< k1X
		2,	//!< k2X
		4,	//!< k4X
		8,	//!< k8X
		16,	//!< k16X
	};
	AROMA_STATIC_ASSERT( AROMA_ARRAY_OF( anisotropicRatioValues ) == ( u32 )AnisotropicRatio::kNum, _T( "Array length mismatch." ));
	return anisotropicRatioValues[ ( u32 )aromaAnisotropicRatio ];
}

//---------------------------------------------------------------------------
//! @brief		数値より異方性フィルタリングサンプル数定義取得.
//---------------------------------------------------------------------------
AnisotropicRatio GetAnisotropicRatio( u32 value )
{
	switch( value )
	{
		case 1:
			return AnisotropicRatio::k1X;
		case 2:
			return AnisotropicRatio::k2X;
		case 4:
			return AnisotropicRatio::k4X;
		case 8:
			return AnisotropicRatio::k8X;
		case 16:
			return AnisotropicRatio::k16X;
	};

	AROMA_ASSERT( false, _T( "The anisotropic ratio value corresponding is not defined.\n" ) );
	return AnisotropicRatio::k1X;
}

} // namespace render
} // namespace aroma

#endif
