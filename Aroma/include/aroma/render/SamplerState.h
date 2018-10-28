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
	//-----------------------------------------------------------------------
	//!	@brief		サンプラーステート設定項目.
	//-----------------------------------------------------------------------
	enum class Setting
	{
		kFilter,			//!< Filter
		kAddressU,			//!< TextureAddress
		kAddressV,			//!< TextureAddress
		kAddressW,			//!< TextureAddress
		kMipLODBias,		//!< f32
		kMaxAnisotropy,		//!< AnisotropicRatio
		kBorderColor,		//!< data::Color
		kMinLOD,			//!< f32
		kMaxLOD,			//!< f32
	};

	Filter					filter;
	TextureAddress			addressU;
	TextureAddress			addressV;
	TextureAddress			addressW;
	f32						mipLODBias;
	AnisotropicRatio		maxAnisotropy;
	data::Color				borderColor;
	f32						minLOD;
	f32						maxLOD;

	//-------------------------------------------------------------------
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

	//---------------------------------------------------------------------------
	//! @brief		ステート設定.
	//! @param[in]	value	設定する値.
	//!	@retval		true	: 値が変更された.
	//! @retval		false	: 値が変更されなかった.
	//---------------------------------------------------------------------------
	bool Set( Setting state, f32 value );
	bool Set( Setting state, Filter value );
	bool Set( Setting state, TextureAddress value );
	bool Set( Setting state, AnisotropicRatio value );
	bool Set( Setting state, const data::Color& value );
};

} // namespace render
} // namespace aroma

