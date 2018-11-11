//===========================================================================
//!
//!	@file		Device_DX11.cpp
//!	@brief		描画システムデバイス : DirectX11
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#ifdef AROMA_RENDER_DX11

#include <aroma/common/Algorithm.h>
#include <aroma/render/render.h>
#include <aroma/render/Device.h>
#include <aroma/render/CommandList.h>
#include <aroma/render/Buffer.h>
#include <aroma/render/Shader.h>
#include <aroma/render/Texture.h>
#include <aroma/data/DDS.h>

namespace aroma {
namespace render {

//---------------------------------------------------------------------------
//!	@brief		コンストラクタ.
//---------------------------------------------------------------------------
Device::Device()
	: _initialized( false )
	, _renderStateCache( this )
	, _d3dDevice( nullptr )
	, _d3dFactory( nullptr )
	, _d3dImmediateContext( nullptr )
	, _d3dFeatureLevel( D3D_FEATURE_LEVEL_11_1 )
{
}

//---------------------------------------------------------------------------
//!	@brief		デストラクタ.
//---------------------------------------------------------------------------
Device::~Device()
{
	Finalize();
}

//---------------------------------------------------------------------------
//!	@brief		初期化.
//---------------------------------------------------------------------------
void	Device::Initialize()
{
	if( _initialized )
	{
		AROMA_ASSERT( false, _T( "Already initialized.\n" ) );
		Finalize();
	}

	HRESULT hr;

	// デバイスの生成.
	u32	flags = 0;
	if( GetDebugMode() == DebugMode::kEnable )
	{
		flags |= D3D11_CREATE_DEVICE_DEBUG;
	}

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		flags,
		featureLevels,
		AROMA_ARRAY_OF( featureLevels ),
		D3D11_SDK_VERSION,
		&_d3dDevice,
		&_d3dFeatureLevel,
		&_d3dImmediateContext );
	AROMA_ASSERT( SUCCEEDED( hr ), _T( "Failed to D3D11CreateDevice.\n" ) );

	// ファクトリーの生成.
	hr = CreateDXGIFactory( __uuidof( IDXGIFactory ), ( void** )( &_d3dFactory ) );
	AROMA_ASSERT( SUCCEEDED( hr ), _T( "Failed to CreateDXGIFactory.\n" ) );

	_initialized = true;
}

//---------------------------------------------------------------------------
//!	@brief		解放.
//---------------------------------------------------------------------------
void Device::Finalize()
{
	if( !_initialized ) return;

	memory::SafeRelease( _d3dImmediateContext );
	memory::SafeRelease( _d3dFactory );
	memory::SafeRelease( _d3dDevice );

	_initialized = false;
}

//---------------------------------------------------------------------------
//!	@brief		スワップチェイン作成.
//---------------------------------------------------------------------------
SwapChain* Device::CreateSwapChain( const SwapChain::Desc& desc )
{
	SwapChain* swapChain = new render::SwapChain();
	swapChain->Initialize( this, desc );
	return swapChain;
}

//---------------------------------------------------------------------------
//!	@brief		遅延コンテキスト作成.
//---------------------------------------------------------------------------
DeferredContext* Device::CreateDeferredContext( const DeferredContext::Desc& desc )
{
	DeferredContext* context = new render::DeferredContext();
	context->Initialize( this, desc );
	return context;
}

//---------------------------------------------------------------------------
//!	@brief		描画コマンドリスト実行.
//---------------------------------------------------------------------------
void Device::ExecuteCommand( const CommandList* commandList )
{
	_d3dImmediateContext->ExecuteCommandList( commandList->GetNativeCommandList(), FALSE );
}

//---------------------------------------------------------------------------
//! @brief		GPUバッファ作成.
//---------------------------------------------------------------------------
Buffer* Device::CreateBuffer( const Buffer::Desc& desc )
{
	Buffer*	buffer = new Buffer();
	buffer->Initialize( this, desc );
	return buffer;
}

//---------------------------------------------------------------------------
//!	@brief		頂点バッファ作成.
//---------------------------------------------------------------------------
Buffer* Device::CreateVertexBuffer( size_t size, Usage usage, const SubResource* initData, size_t stride, u32 flags )
{
	Buffer::Desc bufDesc;
	bufDesc.size				= size;
	bufDesc.usage				= usage;
	bufDesc.bindFlags			= kBindFlagVertexBuffer;
	if( initData )
	{
		bufDesc.initData		= *initData;
	}
	bufDesc.flags				= flags;
	bufDesc.stride				= stride;

	return CreateBuffer( bufDesc );
}

//---------------------------------------------------------------------------
//! @brief		インデックスバッファ作成.
//---------------------------------------------------------------------------
Buffer* Device::CreateIndexBuffer( size_t size, Usage usage, const SubResource* initData, IndexType indexType, u32 flags )
{
	Buffer::Desc bufDesc;
	bufDesc.size				= size;
	bufDesc.usage				= usage;
	bufDesc.bindFlags			= kBindFlagIndexBuffer;
	if( initData )
	{
		bufDesc.initData		= *initData;
	}
	bufDesc.flags				= flags;
	bufDesc.stride				= GetIndexTypeSize( indexType );

	return CreateBuffer( bufDesc );
}

//---------------------------------------------------------------------------
//!	@brief		定数バッファ作成.
//---------------------------------------------------------------------------
Buffer* Device::CreateConstantBuffer( size_t size, Usage usage, const SubResource* initData, size_t stride, u32 flags )
{
	Buffer::Desc bufDesc;
	bufDesc.size				= size;
	bufDesc.usage				= usage;
	bufDesc.bindFlags			= kBindFlagConstantBuffer;
	if( initData )
	{
		bufDesc.initData		= *initData;
	}
	bufDesc.flags				= flags;
	bufDesc.stride				= stride;

	return CreateBuffer( bufDesc );
}

//---------------------------------------------------------------------------
//! @brief		頂点シェーダー作成.
//---------------------------------------------------------------------------
Shader* Device::CreateVertexShader( const void* data, size_t size )
{
	Shader::Desc shaderDesc;

	shaderDesc.shaderData	= data;
	shaderDesc.shaderSize	= size;
	shaderDesc.stage		= Shader::Stage::kVertex;

	Shader*	vs = new Shader();
	vs->Initialize( this, shaderDesc );
	return vs;
}

//---------------------------------------------------------------------------
//! @brief		ピクセルシェーダー作成.
//---------------------------------------------------------------------------
Shader* Device::CreatePixelShader( const void* data, size_t size )
{
	Shader::Desc shaderDesc;

	shaderDesc.shaderData	= data;
	shaderDesc.shaderSize	= size;
	shaderDesc.stage		= Shader::Stage::kPixel;

	Shader*	vs = new Shader();
	vs->Initialize( this, shaderDesc );
	return vs;
}

//---------------------------------------------------------------------------
//!	@brief		入力レイアウトを作成.
//---------------------------------------------------------------------------
InputLayout* Device::CreateInputLayout( const InputLayout::Desc& desc )
{
	InputLayout*	inputLayout = new InputLayout();
	inputLayout->Initialize( this, desc );
	return inputLayout;
}

//--------------------------------------------------------------------
//! @brief		2Dテクスチャ作成.
//--------------------------------------------------------------------
Texture* Device::CreateTexture2D( const Texture::Desc& desc )
{
	Texture*	texture = new Texture();
	texture->Initialize( this, desc );
	return texture;
}

//--------------------------------------------------------------------
//! @brief		DDSデータより2Dテクスチャ作成.
//--------------------------------------------------------------------
Texture* Device::CreateTexture2DFromDDS( const data::DDSAccessor& dds, Usage usage, u32 bindFlags, u32 flags )
{
	if( !dds.IsValid() )
	{
		AROMA_ASSERT( false, "Invalid DDS data." );
		return nullptr;
	}

	// パラメータ取得.
	u32					arrayCount	= dds.GetArrayCount();
	u32					mipCount	= dds.GetMipMapCount();
	data::PixelFormat	format		= dds.GetPixelFormat();

	// エラーチェック.
	{
		// 2Dテクスチャのみ.
		if( dds.IsCubeMap() || dds.IsVolumeTexture() )
		{
			AROMA_ASSERT( false, "This DDS is not 2D Texture.\n" );
			return nullptr;
		}

		// ミップマップ数がグラフィックスAPIでサポートされていない数の場合エラー.
		if( mipCount > kMipCountMax )
		{
			AROMA_ASSERT( false, "This mipmap count is unsupported." ); 
			return nullptr;
		}
	}

	// 初期データ作成.
	SubResource* initData = new SubResource[ mipCount * arrayCount ];
	AROMA_ASSERT( initData, _T( "Failed to memory allocate.\n" ) ); 
	{
		u32		idx		= 0;
		auto	pImg	= dds.GetImageTop();

		for( u32 iArray = 0; iArray < arrayCount; ++iArray )
		for( u32 iMip = 0; iMip < mipCount; iMip++ )
		{
			u32	mipWidth  = Max( 1ui32, dds.GetWidth()	>> iMip );
			u32	mipHeight = Max( 1ui32, dds.GetHeight()	>> iMip );

			data::SurfaceInfo info;
			data::CalcSurfaceInfo( &info, mipWidth, mipHeight, format );
			initData[ idx ].dataConst	= pImg;
			initData[ idx ].pitch		= info.pitchBytes;
			initData[ idx ].slicePitch	= 0;
			idx++;
			pImg = reinterpret_cast< void* >( reinterpret_cast< uintptr >( pImg ) + info.bytes );
		}
	}

	// 2Dテクスチャ作成.
	Texture::Desc desc;
	dds.GetImageSize( &desc.size );
	desc.mipCount		= mipCount;
	desc.format			= format;
	desc.usage			= usage;
	desc.arrayCount		= arrayCount;
	desc.bindFlags		= bindFlags;
	desc.initDataArray	= initData;
	desc.flags			= flags;

	auto texture = CreateTexture2D( desc );
	memory::SafeDeleteArray( initData );
	return texture;
}

//---------------------------------------------------------------------------
//! @brief		レンダーステートキャッシュの取得.
//---------------------------------------------------------------------------
RenderStateCache* Device::GetRenderStateCache()
{
	return &_renderStateCache;
}

//---------------------------------------------------------------------------
//!	@brief		D3Dデバイスの取得.
//---------------------------------------------------------------------------
ID3D11Device* Device::GetNativeDevice() const
{
	return _d3dDevice;
}

//---------------------------------------------------------------------------
//!	@brief		DXGIファクトリーの取得.
//---------------------------------------------------------------------------
IDXGIFactory* Device::GetDXGIFactory() const
{
	return _d3dFactory;
}

} // namespace render
} // namespace aroma

#endif
