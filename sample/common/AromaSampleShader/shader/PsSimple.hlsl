//===========================================================================
//!
//!	@file		PsSimple.js
//!	@brief		ピクセルシェーダー.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author 	d0
//!
//! @note		fxc.exeでコンパイルさせるため文字コードはUTF-8 BOM無しです.
//!
//===========================================================================

struct PsInput
{
	float4	position	: SV_POSITION0;
	float4	color		: COLOR0;
	float2	texCoord	: TEXCOORD0;
};

cbuffer SimpleConstantBuffer : register( b0 )
{
	float mipLevel;
}

Texture2D		g_tex01		: register( t0 );
SamplerState	g_sampler01	: register( s0 );

void main(
	in	const	PsInput	input,
	out 		float4	outColor : SV_TARGET )
{
	outColor = g_tex01.SampleLevel( g_sampler01, input.texCoord, mipLevel ) * input.color;
}
