//===========================================================================
//!
//!	@file		InputLayout_DX11.cpp
//!	@brief		入力レイアウト. : DirectX11
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/render/InputLayout.h>
#include <aroma/render/Device.h>
#include <aroma/render/Shader.h>

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//	コンストラクタ.
//---------------------------------------------------------------------------
InputLayout::InputLayout()
: _initialized( false )
, _device( nullptr )
, _nativeInputLayout( nullptr )
{
}

//---------------------------------------------------------------------------
//	デストラクタ.
//---------------------------------------------------------------------------
InputLayout::~InputLayout()
{
	Finalize();
}

//---------------------------------------------------------------------------
//	初期化.
//---------------------------------------------------------------------------
void InputLayout::Initialize( Device* device, const Desc& desc )
{
	if( _initialized )
	{
		AROMA_ASSERT( false, _T( "Already initialized.\n" ) );
		Finalize();
	}

	_device = device;
	_device->AddRef();
	_desc = desc;	// TODO: StreamDesc, ElementDesc コピー.

	auto d3dDevice		= _device->GetNativeDevice();
	D3D11_INPUT_ELEMENT_DESC d3dElementDesc[ D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT ] = {};
	u32 d3dElmNum = 0;

	for( u32 strm_i = 0; strm_i < desc.streamNum; ++strm_i )
	{
		// Stream
		const auto& strm = desc.streams[ strm_i ];

	    for( u32 elm_i = 0; elm_i < strm.elementNum; ++elm_i )
	    {
			// Element
			const auto& elm = strm.elements[ elm_i ];

			// D3D Desc設定.
			auto& editDesc = d3dElementDesc[ d3dElmNum ];

			editDesc.SemanticName			= elm.semanticName;
			editDesc.SemanticIndex			= elm.semanticIndex;
			editDesc.Format					= ToNativePixelFormat( elm.format );
			editDesc.InputSlot				= strm.streamIndex;
			editDesc.AlignedByteOffset		= static_cast< u32 >( elm.offset );
			editDesc.InputSlotClass			= ToNativeInputClass( strm.inputClass );
			editDesc.InstanceDataStepRate	= 0;	//!< TODO: InputClass::kPerInstance の場合対応.
			d3dElmNum++;
	    }
	}

	HRESULT hr = d3dDevice->CreateInputLayout(
		d3dElementDesc,
		d3dElmNum,
		desc.vertexShader->GetShaderData(),
		desc.vertexShader->GetShaderDataSize(),
		&_nativeInputLayout );
	AROMA_ASSERT( SUCCEEDED( hr ), _T( "Failed to CreateInputLayout.\n" ) );

	_initialized = true;
}

//---------------------------------------------------------------------------
//	解放.
//---------------------------------------------------------------------------
void InputLayout::Finalize()
{
	if( !_initialized ) return;
	memory::SafeRelease( _nativeInputLayout );
	memory::SafeRelease( _device );
	_desc.Default();
	_initialized = false;
}

//-----------------------------------------------------------------------
//! @brief		ストリーム数取得.
//-----------------------------------------------------------------------
u32 InputLayout::GetStreamCount()
{
	return _desc.streamNum;
}

//---------------------------------------------------------------------------
//	ネイティブAPI入力レイアウトの取得.
//---------------------------------------------------------------------------
ID3D11InputLayout* InputLayout::GetNativeInputLayout() const
{
	return _nativeInputLayout;
}


} // namespace render
} // namespace aroma
