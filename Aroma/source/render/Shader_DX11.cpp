//===========================================================================
//!
//!	@file		Shader.cpp
//!	@brief		シェーダー.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//! @todo		TODO: インターフェース化してステージ毎にオブジェクト作成.
//!
//===========================================================================
#include <aroma/render/Shader.h>
#include <aroma/render/Device.h>

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//	コンストラクタ
//---------------------------------------------------------------------------
Shader::Shader()
	: _initialized( false )
	, _device( nullptr )
	, _nativeVS( nullptr )
	, _nativePS( nullptr )
	, _shaderData( nullptr )
	, _shaderSize( 0 )
{
}

//---------------------------------------------------------------------------
//	デストラクタ
//---------------------------------------------------------------------------
Shader::~Shader()
{
	Finalize();
}

//---------------------------------------------------------------------------
//! @brief		初期化 : メモリ上シェーダーバイナリより.
//!	@param[in]	stage			テクスチャステージ.
//!	@param[in]	shaderData		メモリ上のシェーダーバイナリ.
//!	@param[in]	shaderSize		シェーダーバイナリサイズ.
//---------------------------------------------------------------------------
bool Shader::Initialize( Device* device, const Desc& desc )
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
	HRESULT hr;

	switch( desc.stage )
	{
		case Stage::kVertex:
			hr = d3dDevice->CreateVertexShader( desc.shaderData, desc.shaderSize, nullptr, &_nativeVS );
			AROMA_ASSERT( SUCCEEDED( hr ), "Failed to CreateVertexShader." );
			break;
		case Stage::kPixel:
			hr = d3dDevice->CreatePixelShader( desc.shaderData, desc.shaderSize, nullptr, &_nativePS );
			AROMA_ASSERT( SUCCEEDED( hr ), "Failed to CreatePixelShader." );
			break;
	}

	// シェーダーバイナリを保持.
	_shaderData = CpuMemAlloc( desc.shaderSize, 1 );
	memcpy( _shaderData, desc.shaderData, desc.shaderSize );
	_shaderSize = desc.shaderSize;

	_initialized = true;
	return true;
}

//---------------------------------------------------------------------------
//	解放.
//---------------------------------------------------------------------------
void Shader::Finalize()
{
	if (!_initialized) return;

	if( _shaderData )
	{
		CpuMemFree( _shaderData );
		_shaderData = nullptr;
	}
	_shaderSize = 0;

	memory::SafeRelease( _nativeVS );
	memory::SafeRelease( _nativePS );
	memory::SafeRelease( _device );
	_desc.Default();
	_initialized = false;
}

//-----------------------------------------------------------------------
//! @brief		シェーダーバイナリデータ取得.
//-----------------------------------------------------------------------
void* Shader::GetShaderData() const
{
	return _shaderData;
}

//-----------------------------------------------------------------------
//! @brief		シェーダーバイナリデータサイズ取得.
//-----------------------------------------------------------------------
size_t Shader::GetShaderDataSize() const
{
	return _shaderSize;
}

//-----------------------------------------------------------------------
//! @brief		ネイティブAPI頂点シェーダー取得.
//-----------------------------------------------------------------------
ID3D11VertexShader* Shader::GetNativeVertexShader() const
{
	AROMA_ASSERT( _desc.stage == Stage::kVertex, "This Shader is not vertex shader." );
	return _nativeVS;
}

//-----------------------------------------------------------------------
//! @brief		ネイティブAPIピクセルシェーダー取得.
//-----------------------------------------------------------------------
ID3D11PixelShader* Shader::GetNativePixelShader() const
{
	AROMA_ASSERT( _desc.stage == Stage::kPixel, "This Shader is not pixel shader." );
	return _nativePS;
}

} // namespace render
} // namespace aroma
