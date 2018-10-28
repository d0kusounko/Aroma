//===========================================================================
//!
//!	@file		Resource_DX11.cpp
//!	@brief		GPUリソース : DirectX11
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/render/Resource.h>

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//!	@brief	ネイティブAPIサブリソースに変換.
//---------------------------------------------------------------------------
void SubResource::ToNativeSubResource( D3D11_SUBRESOURCE_DATA* outSubResource ) const
{
	outSubResource->pSysMem				= dataConst;
	outSubResource->SysMemPitch			= static_cast< u32 >( pitch );
	outSubResource->SysMemSlicePitch	= static_cast< u32 >( slicePitch );
}

//---------------------------------------------------------------------------
//!	@brief		コンストラクタ.
//---------------------------------------------------------------------------
IResource::IResource()
	: _device( nullptr )
{
}

//---------------------------------------------------------------------------
//!	@brief		デストラクタ.
//---------------------------------------------------------------------------
IResource::~IResource()
{
}

} // namespace render
} // namespace aroma
