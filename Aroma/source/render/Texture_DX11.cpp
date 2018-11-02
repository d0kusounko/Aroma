//===========================================================================
//!
//!	@file		Texture_DX11.h
//!	@brief		テクスチャ : DirectX11
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#ifdef AROMA_RENDER_DX11

#include <aroma/render/Texture.h>
#include <aroma/render/Device.h>

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//! @brief		コンストラクタ.
//---------------------------------------------------------------------------
Texture::Texture()
	: _initialized( false )
	, _device( nullptr )
	, _nativeTexture( nullptr )
{
}

//---------------------------------------------------------------------------
//! @brief		デストラクタ.
//---------------------------------------------------------------------------
Texture::~Texture()
{
	Finalize();
}

//---------------------------------------------------------------------------
//! @brief		初期化.
//---------------------------------------------------------------------------
void Texture::Initialize( Device* device, const Desc& desc )
{
	if( _initialized )
	{
		AROMA_ASSERT( false, _T( "Already initialized.\n" ) );
		Finalize();
	}
	// TODO: descの不正値チェック.
	AROMA_ASSERT( desc.mipLevel > 0, "Invalid desc value.\n" );

	_device = device;
	_device->AddRef();
	_desc = desc;

	auto d3dDevice		= _device->GetNativeDevice();

	D3D11_TEXTURE2D_DESC d3dDesc = {};
	d3dDesc.Width				= desc.size.width;
	d3dDesc.Height				= desc.size.height;
	d3dDesc.ArraySize			= desc.arrayCount;
	d3dDesc.MipLevels			= desc.mipLevel;
	d3dDesc.BindFlags			= ToNativeBindFlags( desc.bindFlags );
	d3dDesc.CPUAccessFlags		= ToNativeCpuAccessFlag( GetCpuAccessFlags( desc.usage ) );
	d3dDesc.Format				= ToNativePixelFormat( desc.format );
	d3dDesc.SampleDesc.Count	= 1;
	d3dDesc.SampleDesc.Quality	= 0;

	D3D11_SUBRESOURCE_DATA* subResource = nullptr;
	if( desc.initDataArray )
	{
		// 初期データ設定.
		u32 dataNum = desc.mipLevel * desc.arrayCount;
		subResource = new D3D11_SUBRESOURCE_DATA[ desc.mipLevel * desc.arrayCount ];
		for( u32 i = 0; i < dataNum; ++i )
		{
			subResource[ i ].pSysMem			= desc.initDataArray[ i ].dataConst;
			subResource[ i ].SysMemPitch		= static_cast< u32 >( desc.initDataArray[ i ].pitch );
			subResource[ i ].SysMemSlicePitch	= static_cast< u32 >( desc.initDataArray[ i ].slicePitch );
		}
	}
	HRESULT hr = d3dDevice->CreateTexture2D( &d3dDesc, subResource, &_nativeTexture );
	memory::SafeDeleteArray( subResource );

	_initialized = true;
}

//---------------------------------------------------------------------------
//! @brief		ネイティブAPIテクスチャバッファより初期化.
//---------------------------------------------------------------------------
void Texture::InitializeFromNativeTexture( Device* device, ID3D11Texture2D* nativeTexture )
{
	if( _initialized )
	{
		AROMA_ASSERT( false, _T( "Already initialized.\n" ) );
		Finalize();
	}

	_device = device;
	_device->AddRef();
	_nativeTexture	= nativeTexture;
	_nativeTexture->AddRef();

	// ネイティブのDescをもとにAroma Descをできるだけ似た形で作成.
	{
		D3D11_TEXTURE2D_DESC d3dDesc;
		nativeTexture->GetDesc( &d3dDesc );

		_desc.Default();
		_desc.size.width		= d3dDesc.Width;
		_desc.size.height		= d3dDesc.Height;
		_desc.mipLevel			= d3dDesc.MipLevels;
		_desc.format			= ToAromaPixelFormat( d3dDesc.Format );
		_desc.usage				= ToAromaUsage( d3dDesc.Usage );
		_desc.arrayCount		= d3dDesc.ArraySize;
		_desc.multiSampleCount	= d3dDesc.SampleDesc.Count;
		_desc.bindFlags			= ToAromaBindFlags( d3dDesc.BindFlags );
		_desc.initDataArray		= nullptr;
		_desc.flags				= 0;
	}

	_initialized = true;
}



//---------------------------------------------------------------------------
//! @brief		解放.
//---------------------------------------------------------------------------
void Texture::Finalize()
{
	if( !_initialized ) return;
	memory::SafeRelease( _nativeTexture );
	memory::SafeRelease( _device );
	_desc.Default();
	_initialized = false;
}


//-----------------------------------------------------------------------
//! @brief		構成設定取得.
//-----------------------------------------------------------------------
const Texture::Desc& Texture::GetDesc() const
{
	return _desc;
}

//-----------------------------------------------------------------------
//! @brief		ネイティブAPIリソース取得.
//-----------------------------------------------------------------------
ID3D11Resource* Texture::GetNativeResource() const
{
	return _nativeTexture;
}

//-----------------------------------------------------------------------
//! @brief		メモリマッピング.
//-----------------------------------------------------------------------
void* Texture::Map()
{
	// TODO: 作成.
	AROMA_ASSERT( false, "TODO" );
	return nullptr;
}
	
//-----------------------------------------------------------------------
//! @brief		メモリマッピング解除.
//-----------------------------------------------------------------------
void Texture::Unmap()
{
	// TODO: 作成.
	AROMA_ASSERT( false, "TODO" );
}

} // namespace render
} // namespace aroma

#endif
