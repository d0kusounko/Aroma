//===========================================================================
//!
//!	@file		Buffer_DX11.cpp
//!	@brief		GPUバッファ : DirectX11
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/render/Buffer.h>
#include <aroma/render/Device.h>
#include <aroma/render/Resource.h>

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//! @brief		コンストラクタ.
//---------------------------------------------------------------------------
Buffer::Buffer()
: _initialized( false )
, _device( nullptr )
{
}

//---------------------------------------------------------------------------
//!	@brief		デストラクタ.
//---------------------------------------------------------------------------
Buffer::~Buffer()
{
	Finalize();
}

//---------------------------------------------------------------------------
//! @brief		初期化.
//---------------------------------------------------------------------------
void Buffer::Initialize( Device* device, const Desc& desc )
{
	if( _initialized )
	{
		AROMA_ASSERT( false, _T( "Already initialized.\n" ) );
		Finalize();
	}

	_device = device;
	_device->AddRef();
	_desc = desc;

	auto d3dDevice	= _device->GetNativeDevice();

	D3D11_BUFFER_DESC d3dDesc = {};
	d3dDesc.ByteWidth			= static_cast< u32 >( desc.size );
	d3dDesc.Usage				= ToNativeUsage( desc.usage );
	d3dDesc.BindFlags			= ToNativeBindFlags( desc.bindFlags );
	d3dDesc.CPUAccessFlags		= ToNativeCpuAccessFlag( GetCpuAccessFlags( desc.usage ) );
	d3dDesc.StructureByteStride	= static_cast< u32 >( desc.stride );

	// 初期データ設定.
	D3D11_SUBRESOURCE_DATA* d3dInitData = nullptr;
	if( desc.initData.dataConst )
	{
		d3dInitData = new D3D11_SUBRESOURCE_DATA;
		desc.initData.ToNativeSubResource( d3dInitData );
	}
	else if( desc.usage == Usage::kImmutable )
	{
		AROMA_ASSERT( false, "Initial data is required when Usage is Immutable." );
	}

	// D3Dバッファ作成.
	HRESULT hr = d3dDevice->CreateBuffer( &d3dDesc, d3dInitData, &_nativeBuffer );
	AROMA_ASSERT( SUCCEEDED( hr ), _T( "Failed to CreateBuffer.\n" ) );
	memory::SafeDelete( d3dInitData );

	_initialized = true;
	return;
}

//---------------------------------------------------------------------------
//!	@brief		解放
//---------------------------------------------------------------------------
void Buffer::Finalize()
{
	if( !_initialized ) return;
	memory::SafeRelease( _nativeBuffer );
	memory::SafeRelease( _device );
	_desc.Default();
	_initialized = false;
}

//---------------------------------------------------------------------------
//! @brief		メモリマッピング.
//---------------------------------------------------------------------------
void* Buffer::Map()
{
    auto d3dDevice = _device->GetNativeDevice();
	ID3D11DeviceContext* d3dDeviceContext;
	d3dDevice->GetImmediateContext( &d3dDeviceContext );

    D3D11_MAPPED_SUBRESOURCE mapped;

	D3D11_MAP	mapType;
	switch( _desc.usage )
	{
		case Usage::kDynamic:
			mapType	= D3D11_MAP_WRITE_DISCARD;
			break;
		case Usage::kStaging:
			mapType = D3D11_MAP_READ_WRITE;
			break;
	}

    auto hr = d3dDeviceContext->Map( _nativeBuffer, 0, mapType, 0, &mapped );
	d3dDeviceContext->Release();

    return mapped.pData;
}

//---------------------------------------------------------------------------
//! @brief		メモリマッピング解除.
//---------------------------------------------------------------------------
void Buffer::Unmap()
{
    auto d3dDevice = _device->GetNativeDevice();
	ID3D11DeviceContext* d3dDeviceContext;
	d3dDevice->GetImmediateContext( &d3dDeviceContext );

	d3dDeviceContext->Unmap( _nativeBuffer, 0 );
	d3dDeviceContext->Release();
}

//---------------------------------------------------------------------------
//!	@brief		構成設定取得.
//---------------------------------------------------------------------------
const Buffer::Desc& Buffer::GetDesc() const
{
	return _desc;
}

//---------------------------------------------------------------------------
//!	@brief		ネイティブAPIバッファーの取得.
//---------------------------------------------------------------------------
ID3D11Buffer* Buffer::GetNativeBuffer() const
{
	return _nativeBuffer;
}


} // namespace render
} // namespace aroma
