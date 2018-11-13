//===========================================================================
//!
//!	@file		SamplerState.h
//!	@brief		サンプラーステート.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

#include "../data/Color.h"
#include "RenderDef.h"

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//! @brief		サンプラーステート.
//---------------------------------------------------------------------------
struct SamplerState final
{
	// TODO: RenderStateCacheのkeyと同じ形でビットフィールド化.
	Filter					filter;
	TextureAddress			addressU;
	TextureAddress			addressV;
	TextureAddress			addressW;
	f32						mipLODBias;
	AnisotropicRatio		maxAnisotropy;
	data::Color				borderColor;
	f32						minLOD;
	f32						maxLOD;

	//-----------------------------------------------------------------------
	SamplerState(){ Default(); }
	void Default()
	{
		filter			= Filter::kMinMagMipLinear;
		addressU		= TextureAddress::kClamp;
		addressV		= TextureAddress::kClamp;
		addressW		= TextureAddress::kClamp;
		mipLODBias		= 0.0f;
		maxAnisotropy	= AnisotropicRatio::k1X;
		borderColor		= data::Color( 0.f, 0.f, 0.f, 1.f );
		minLOD			= 0.0f;
		maxLOD			= AROMA_FLT32_MAX;
	}

	//-----------------------------------------------------------------------
	//! @brief		各ステート設定.
	//! @param[in]	value	設定する値.
	//!	@retval		true	: 値が変更された.
	//! @retval		false	: 値が変更されなかった.
	//-----------------------------------------------------------------------
	bool Set( const SamplerState& value );
	bool SetFilter( Filter value );
	bool SetAddressU( TextureAddress value );
	bool SetAddressV( TextureAddress value );
	bool SetAddressW( TextureAddress value );
	bool SetMipLODBias( f32 value );
	bool SetMaxAnisotropy( AnisotropicRatio value );
	bool SetBorderColor( const data::Color& value );
	bool SetMinLOD( f32 value );
	bool SetMaxLOD( f32 value );
};

} // namespace render
} // namespace aroma

