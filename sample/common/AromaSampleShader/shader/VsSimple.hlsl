//===========================================================================
//!
//!	@file		VsSimple.js
//!	@brief		頂点シェーダー.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author 	d0
//!
//! @note		fxc.exeでコンパイルさせるため文字コードはUTF-8 BOM無しです.
//!
//===========================================================================
struct VsInput
{
	float3	position	: POSITION0;
	float4	color		: COLOR0;
	float2	texCoord	: TEXCOORD0;
};

struct VsOutput {
	float4	position	: SV_POSITION0;
	float4	color		: COLOR0;
	float2	texCoord	: TEXCOORD0;
};

VsOutput main( in const VsInput input )
{
	VsOutput	output;
	output.position		= float4( input.position, 1.0f );
	output.color		= input.color;
	output.texCoord		= input.texCoord;

	return output;
}