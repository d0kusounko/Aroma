#include <Aroma.h>
#include <functional>
#include <stdlib.h>
#include <time.h>

namespace VsSimple
{
#include <VsSimple.ash>
}
namespace PsSimple
{
#include <PsSimple.ash>
}

using namespace aroma;

#ifdef AROMA_WINDOWS

namespace
{
	const data::ImageSize		kSampleScreenSize( 1280, 720 );
	constexpr u32				kSwapChainBufferNum	= 3;
	constexpr u32				kBufferingCount = 2;

	//! CPUメモリアロケーター.
	class SampleCpuAllocator : public memory::IAllocator
	{
	public:
		void* Alloc( size_t size, size_t alignment ) noexcept override
		{
			return _aligned_malloc( size, alignment );
		}

		void* Realloc( void* ptr, size_t newSize ) noexcept override
		{
			return realloc( ptr, newSize );
		}

		void Free( void* ptr ) noexcept override
		{
			_aligned_free( ptr );
		}
	} s_sampleCpuMemAllocator;

	//! GPUメモリアロケーター.
	class SampleGpuAllocator : public memory::IAllocator
	{
	public:
		void* Alloc( size_t size, size_t alignment ) noexcept override
		{
			return _aligned_malloc( size, alignment );
		}

		void* Realloc( void* ptr, size_t newSize ) noexcept override
		{
			return realloc( ptr, newSize );
		}

		void Free( void* ptr ) noexcept override
		{
			free( ptr );
		}
	} s_sampleGpuMemAllocator;

	// 頂点.
	struct Vertex
	{
		f32 x, y, z;
		data::Color color;
		f32 u, v;
	};

	//! ピクセルシェーダ : 定数バッファ.
	struct PSConstantBuffer
	{
		f32	mip;
		f32 pad[ 3 ];
	};

	// 2Dスプライト.
	struct Sprite
	{
		render::Texture*		tex;
		render::TextureView*	texView;
		render::Buffer*			vtxBuffer[ kBufferingCount ];
		bool					visible;
		data::RectF				rect;
		data::Color				color;

		Sprite()
			: tex( nullptr )
			, texView( nullptr )
			, visible( false )
		{
			for( u32 i = 0; i < kBufferingCount; ++i )
			{
				vtxBuffer[ i ] = nullptr;
			}
		}

		~Sprite()
		{
			Release();
		}

		void Release()
		{
			for( u32 i = 0; i < kBufferingCount; ++i )
			{
				memory::SafeRelease( vtxBuffer[ i ] );
			}
			memory::SafeRelease( texView );
			memory::SafeRelease( tex );
		}
	};

	enum class SampleSprite
	{
		kDX11Logo,
		kHirokun,
		kDoudesuka,
		kNum
	};

	app::Window*				g_window			= nullptr;
	u32							g_bufferingIndex	= 0;
	render::Device*				g_device			= nullptr;
	render::SwapChain*			g_swapChain			= nullptr;
	render::DeferredContext*	g_context			= nullptr;
	render::Shader*				g_vertexShader		= nullptr;
	render::Shader*				g_pixelShader		= nullptr;
	render::InputLayout*		g_inputLayout		= nullptr;
	render::Buffer*				g_indexBuffer		= nullptr;
	render::RenderTargetView*	g_backBufferView[ kSwapChainBufferNum ] = {};
	render::Buffer*				g_constBuffer[ kBufferingCount ] = {};
	f32							g_mipLevel			= 0.0f;
	data::Color					g_bgColor			= { 1.0f, 1.0f, 1.0f, 1.0f };
	
	Sprite*						g_sprite[ ( u32 )SampleSprite::kNum ];

	using UpdateFunc = std::function< void() >;
	UpdateFunc					g_updateFunc		= nullptr;
	s32							g_updateFuncFrame	= 0;

	void Update( u32 frame );
	void Draw();
	void DrawSprite( render::DeferredContext* context, Sprite* sprite );
	void CreateSprite( Sprite** outSprite, CTStr ddsPath );

	void UpdateInitialize();
	void UpdateLogoAppear();
	void UpdateBlackBG();
	void UpdateHirokunAppear();
	void UpdateHirokunShake();
	void UpdateDoudesukaAppear();
	void UpdateDoudesukaShake();
}

// TODO: ウィンドウ座標移動コールバックテスト　あとで消す.
void TestCallback( const void* messageParam, void* userParam )
{
	auto param = static_cast< const app::WindowMessageCallbackParamMove* >( messageParam );
	if( param )
	{
		AROMA_DEBUG_OUT( "x = %d, y = %d\n", param->x, param->y );
	}
}

//---------------------------------------------------------------------------
//!	@brief		メインエントリ.
//---------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	srand( ( u32 )time( nullptr) );

	// ウィンドウの作成.
	{
		app::Window::CreateConfig config;
		config.windowTitle	= _T( "AromaSample Render Initialize" );
		config.size			= kSampleScreenSize;
		config.callbacks[ ( u32 )app::WindowMessage::kMove ].func = TestCallback;
		OnFlags( config.flags, app::kWindowFlagDestroyPostQuit );
		g_window			= app::Window::Create( config );
	}

	// TODO: あとで消す.
	// ダミーウィンドウ作成.
	app::Window* testWindow = nullptr;
	{
		app::Window::CreateConfig config;
		config.windowTitle	= _T( "Dummy" );
		config.size.width	= 320;
		config.size.height	= 160;
		config.callbacks[ ( u32 )app::WindowMessage::kSize ].func = TestCallback;
		OnFlags( config.flags, app::kWindowFlagCloseMessageNotDestory );
		testWindow			= app::Window::Create( config );
	}

	// 描画システム初期化.
	{
		render::Desc desc;
#if AROMA_DEBUG
		desc.debugMode					= render::DebugMode::kEnable;
#endif
		desc.allocator.cpuMemAllocator	= &s_sampleCpuMemAllocator;
		desc.allocator.gpuMemAllocator	= &s_sampleGpuMemAllocator;

		render::Initialize( desc );
	}

	// デバイス.
	{
		g_device = new render::Device();
		g_device->Initialize();
	}

	// スワップチェイン.
	{
		render::SwapChain::Desc desc;
		desc.size.width		= kSampleScreenSize.width;
		desc.size.height	= kSampleScreenSize.height;
		desc.bufferCount	= kSwapChainBufferNum;
		desc.window			= g_window;
		g_swapChain = g_device->CreateSwapChain( desc );

		for( u32 i = 0; i < kSwapChainBufferNum; ++i )
		{
			g_swapChain->GetBufferView( i, &g_backBufferView[ i ] );
		}
	}

	// 遅延コンテキスト.
	{
		render::DeferredContext::Desc desc;
		g_context = g_device->CreateDeferredContext( desc );
	}

	// インデックスバッファ.
	{
		constexpr u32 Indices[] =
		{
			0, 1, 2, 3
		};

		render::SubResource initData;
		initData.dataConst = Indices;
		g_indexBuffer = g_device->CreateIndexBuffer(
			sizeof(Indices), render::Usage::kImmutable, &initData,
			render::IndexType::k32, 0 );
	}

	// 定数バッファ.
	for( u32 i = 0; i < kBufferingCount; ++i )
	{
		g_constBuffer[ i ] = g_device->CreateConstantBuffer(
			sizeof( PSConstantBuffer ), render::Usage::kDynamic, nullptr,
			sizeof( PSConstantBuffer ), 0 );
	}

	// 頂点シェーダー.
	g_vertexShader	= g_device->CreateVertexShader( VsSimple::g_main, sizeof( VsSimple::g_main ) );

	// ピクセルシェーダー.
	g_pixelShader	= g_device->CreatePixelShader( PsSimple::g_main, sizeof( PsSimple::g_main ) );

	// 入力レイアウト.
	{
		// 入力要素.
		render::InputLayout::ElementDesc elm[] =
		{
			{ "POSITION",	0,	data::PixelFormat::kR32G32B32Float,	0 },
			{ "COLOR",		0,	data::PixelFormat::kR32G32B32A32Float, 12 },
			{ "TEXCOORD",	0,	data::PixelFormat::kR32G32Float, 28 },
		};

		// 入力ストリーム.
		render::InputLayout::StreamDesc strm[] =
		{
			{ AROMA_ARRAY_OF( elm ), elm, 0, sizeof( Vertex ), render::InputClass::kPerVertex },
		};

		// 入力レイアウト.
		render::InputLayout::Desc desc;
		desc.streamNum		= AROMA_ARRAY_OF( strm );
		desc.streams		= strm;
		desc.vertexShader	= g_vertexShader;
		g_inputLayout		= g_device->CreateInputLayout( desc );
	}

	// スプライト.
	{
		// DirectX11logo
		CreateSprite( &g_sprite[ ( u32 )SampleSprite::kDX11Logo ], _T( "resource/sample01.dds" ) );
		g_sprite[ ( u32 )SampleSprite::kDX11Logo ]->rect.x = 0.0f;
		g_sprite[ ( u32 )SampleSprite::kDX11Logo ]->rect.y = 0.0f;
		g_sprite[ ( u32 )SampleSprite::kDX11Logo ]->rect.w = 2.0f;
		g_sprite[ ( u32 )SampleSprite::kDX11Logo ]->rect.h = 1.0f;
		g_sprite[ ( u32 )SampleSprite::kDX11Logo ]->visible = true;

		// ヒロくん.
		CreateSprite( &g_sprite[ ( u32 )SampleSprite::kHirokun ], _T( "resource/sample02.dds" ) );
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->rect.x = 0.0f;
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->rect.y = 0.6f;
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->rect.w = 1.2f;
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->rect.h = 0.6f;
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->visible = false;

		// どうですか.
		CreateSprite( &g_sprite[ ( u32 )SampleSprite::kDoudesuka ], _T( "resource/sample03.dds" ) );
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->rect.x = 0.0f;
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->rect.y = -0.6f;
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->rect.w = 1.2f;
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->rect.h = 0.6f;
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->visible = false;
	}

	u32 frame = 0;
	g_updateFunc = UpdateInitialize;

    while( true )
	{
		app::ProcessMessage();
		if( app::IsQuit() )
		{
			break;
		}

		AROMA_DEBUG_OUT( "Frame = %u\n", frame );

		// 更新.
		Update( frame );

		// 描画.
		Draw();

		frame++;
	}

	// 終了.
	for( u32 i = 0; i < ( u32 )SampleSprite::kNum; ++i )
	{
		memory::SafeDelete( g_sprite[ i ] );
	}
	memory::SafeRelease( g_inputLayout );
	memory::SafeRelease( g_vertexShader );
	memory::SafeRelease( g_pixelShader );
	for( u32 i = 0; i < kBufferingCount; ++i )
	{
		memory::SafeRelease( g_constBuffer[ i ] );
	}
	memory::SafeRelease( g_indexBuffer );
	for( u32 i = 0; i < AROMA_ARRAY_OF( g_backBufferView ); ++i )
	{
		memory::SafeRelease( g_backBufferView[ i ] );
	}
	memory::SafeRelease( g_context );
	memory::SafeRelease( g_swapChain );
	memory::SafeRelease( g_device );
	render::Finalize();
	
	memory::SafeRelease( testWindow );	// TODO: あとで消す.
	memory::SafeRelease( g_window );

	return 0;
}

namespace
{

void Update( u32 frame )
{
	g_updateFunc();
}

void UpdateInitialize()
{
	g_updateFuncFrame	= 0;
	g_mipLevel			= 5.0f;
	g_updateFunc		= UpdateLogoAppear;
}

void UpdateLogoAppear()
{
	g_mipLevel -= 0.01f;
	if( g_mipLevel <= 0.0f )
	{
		g_mipLevel = 0.0f;

		g_updateFuncFrame++;
		if( g_updateFuncFrame >= 180 )
		{
			g_updateFuncFrame	= 0;
			g_updateFunc		= UpdateBlackBG;
		}
	}
}

void UpdateBlackBG()
{
	g_bgColor -= data::Color( 0.005f, 0.005f, 0.005f, 0.0f );
	if( g_bgColor.r <= 0.0f )
	{
		g_bgColor.Set( 0.0f, 0.0f, 0.0f, 1.0f );
		g_updateFunc = UpdateHirokunAppear;

		// ウィンドウタイトル変更.
		g_window->SetWindowTitle( _T( "おわいハウスへおいでよ　家賃１万円でしかもDirectX11までできてしまう" ) );
	}
}

void UpdateHirokunAppear()
{
	constexpr f32 beginA = 0.0f;
	constexpr f32 beginW = 1.2f * 15;
	constexpr f32 beginH = 0.6f * 15;
	constexpr f32 endA = 1.0f;
	constexpr f32 endW = 1.2f;
	constexpr f32 endH = 0.6f;
	constexpr s32 animFrame = 180;

	if( g_updateFuncFrame == 0 )
	{
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->color.a	= beginA;
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->rect.w	= beginW;
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->rect.h	= beginH;
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->visible = true;
	}
	else if( g_updateFuncFrame < animFrame )
	{
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->color.a = 
			beginA + ( endA - beginA ) * ( ( f32 )g_updateFuncFrame / ( f32 )animFrame );
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->rect.w = 
			beginW + ( endW - beginW ) * ( ( f32 )g_updateFuncFrame / ( f32 )animFrame );
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->rect.h = 
			beginH + ( endH - beginH ) * ( ( f32 )g_updateFuncFrame / ( f32 )animFrame );
	}
	else
	{
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->color.a = endA;
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->rect.w = endW;
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->rect.h = endH;

		g_updateFuncFrame = 0;
		g_updateFunc = UpdateHirokunShake;
		return;
	}
	g_updateFuncFrame++;
}


void UpdateHirokunShake()
{
	constexpr f32 centerX		= 0.0f;
	constexpr f32 centerY		= 0.6f;
	constexpr f32 ShakeRange	= 0.1f;
	constexpr s32 animFrame		= 60;

	if( g_updateFuncFrame < animFrame )
	{
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->rect.x = centerX + ( ( f32 )rand() / RAND_MAX * ShakeRange - ( ShakeRange / 2 ));
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->rect.y = centerY + ( ( f32 )rand() / RAND_MAX * ShakeRange - ( ShakeRange / 2 ));
	}
	else
	{
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->rect.x = centerX;
		g_sprite[ ( u32 )SampleSprite::kHirokun ]->rect.y = centerY;

		g_updateFuncFrame = 0;
		g_updateFunc = UpdateDoudesukaAppear;
		return;
	}
	g_updateFuncFrame++;
}

void UpdateDoudesukaAppear()
{
	constexpr f32 beginA = 0.0f;
	constexpr f32 beginW = 1.2f * 15;
	constexpr f32 beginH = 0.6f * 15;
	constexpr f32 endA = 1.0f;
	constexpr f32 endW = 1.2f;
	constexpr f32 endH = 0.6f;
	constexpr s32 animFrame = 180;

	if( g_updateFuncFrame == 0 )
	{
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->color.a	= beginA;
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->rect.w	= beginW;
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->rect.h	= beginH;
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->visible = true;
	}
	else if( g_updateFuncFrame < animFrame )
	{
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->color.a = 
			beginA + ( endA - beginA ) * ( ( f32 )g_updateFuncFrame / ( f32 )animFrame );
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->rect.w = 
			beginW + ( endW - beginW ) * ( ( f32 )g_updateFuncFrame / ( f32 )animFrame );
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->rect.h = 
			beginH + ( endH - beginH ) * ( ( f32 )g_updateFuncFrame / ( f32 )animFrame );
	}
	else
	{
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->color.a = endA;
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->rect.w = endW;
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->rect.h = endH;

		g_updateFuncFrame = 0;
		g_updateFunc = UpdateDoudesukaShake;
	}
	g_updateFuncFrame++;
}


void UpdateDoudesukaShake()
{
	constexpr f32 centerX		= 0.0f;
	constexpr f32 centerY		= -0.6f;
	constexpr f32 ShakeRange	= 0.1f;
	constexpr s32 animFrame		= 60;

	if( g_updateFuncFrame < animFrame )
	{
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->rect.x = centerX + ( ( f32 )rand() / RAND_MAX * ShakeRange - ( ShakeRange / 2 ));
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->rect.y = centerY + ( ( f32 )rand() / RAND_MAX * ShakeRange - ( ShakeRange / 2 ));
	}
	else
	{
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->rect.x = centerX;
		g_sprite[ ( u32 )SampleSprite::kDoudesuka ]->rect.y = centerY;
	}
	g_updateFuncFrame++;
}

void Draw()
{
	// 描画コマンド作成.
	g_context->Begin();
	{
		auto currentBackBuffer = g_backBufferView[ g_swapChain->GetCurrentBufferIndex() ];

		// ビューポート.
		render::Viewport viewport;
		viewport.x			= 0.0f;
		viewport.y			= 0.0f;
		viewport.w			= static_cast< f32 >( kSampleScreenSize.width );
		viewport.h			= static_cast< f32 >( kSampleScreenSize.height );
		viewport.minDepth	= 0.0f;
		viewport.maxDepth	= 1.0f;
		g_context->RSSetViewportScissorState( 0, render::ViewportScissorState::Setting::kViewport, viewport );

		// シザー.
		render::ScissorRect scissor;
		scissor.x = 0;
		scissor.y = 0;
		scissor.w = kSampleScreenSize.width;
		scissor.h = kSampleScreenSize.height;
		g_context->RSSetViewportScissorState( 0, render::ViewportScissorState::Setting::kScissor, scissor );
			
		// レンダーターゲット設定.
		g_context->OMSetRenderTargets( 1, &currentBackBuffer, nullptr );
			
		// 画面クリア.
		g_context->ClearRenderTarget( currentBackBuffer, g_bgColor );

		// PSステージ : 定数バッファ更新.
		void* mapped = g_constBuffer[ g_bufferingIndex ]->Map();
		{
			PSConstantBuffer constBuf;
			constBuf.mip = g_mipLevel;
			memcpy( mapped, &constBuf, sizeof( PSConstantBuffer ) );
		}
		g_constBuffer[ g_bufferingIndex ]->Unmap();
		g_context->PSSetConstantBuffer( 0, g_constBuffer[ g_bufferingIndex ] );

		// スプライト描画.
		for( auto& sprite : g_sprite )
		{
			if( !sprite ) continue;
			if( sprite->visible ) DrawSprite( g_context, sprite );
		}
	}
	render::CommandList* commandList;
	g_context->End( &commandList );

	// 描画コマンド実行.
	g_device->ExecuteCommand( commandList );
	memory::SafeRelease( commandList );

	// 画面に出力.
	g_swapChain->Present( 1 );

	// ダブルバッファをフリップ.
	g_bufferingIndex = ( g_bufferingIndex + 1 ) % kBufferingCount;
}

void DrawSprite( render::DeferredContext* context, Sprite* sprite )
{
	// ブレンドステート.
	context->OMSetBlendState( render::BlendState::Setting::kSampleAlphaToCoverage,	false );
	context->OMSetBlendState( render::BlendState::Setting::kBlendEnable,			true );
	context->OMSetBlendState( render::BlendState::Setting::kRGBSource,				render::Blend::kSrcAlp );
	context->OMSetBlendState( render::BlendState::Setting::kRGBDest,				render::Blend::kInvSrcAlp );
	context->OMSetBlendState( render::BlendState::Setting::kRGBBlendOp,				render::BlendOp::kAdd );
	context->OMSetBlendState( render::BlendState::Setting::kAlphaSource,			render::Blend::kSrcAlp );
	context->OMSetBlendState( render::BlendState::Setting::kAlphaDest,				render::Blend::kInvSrcAlp );
	context->OMSetBlendState( render::BlendState::Setting::kAlphaBlendOp,			render::BlendOp::kAdd );
	context->OMSetBlendState( render::BlendState::Setting::kWriteMaskR,				true );
	context->OMSetBlendState( render::BlendState::Setting::kWriteMaskG,				true );
	context->OMSetBlendState( render::BlendState::Setting::kWriteMaskB,				true );
	context->OMSetBlendState( render::BlendState::Setting::kWriteMaskA,				true );

	// ラスタライザーステート.
	context->RSSetRasterizerState( render::RasterizerState::Setting::kFillMode, render::FillMode::kSolid );
	context->RSSetRasterizerState( render::RasterizerState::Setting::kCullMode, render::CullMode::kNone );
	context->RSSetRasterizerState( render::RasterizerState::Setting::kFrontCounterClockwise, true );
	context->RSSetRasterizerState( render::RasterizerState::Setting::kDepthBias, 0 );
	context->RSSetRasterizerState( render::RasterizerState::Setting::kDepthBiasClamp, 0.f );
	context->RSSetRasterizerState( render::RasterizerState::Setting::kSlopeScaledDepthBias, 0.f );
	context->RSSetRasterizerState( render::RasterizerState::Setting::kDepthClipEnable, true );
	context->RSSetRasterizerState( render::RasterizerState::Setting::kScissorEnable, false );
	context->RSSetRasterizerState( render::RasterizerState::Setting::kMultisampleEnable, false );
	context->RSSetRasterizerState( render::RasterizerState::Setting::kAntialiasedLineEnable, false );

	// 深度ステンシルステート.
	context->OMSetDepthStencilState( render::DepthStencilState::Setting::kDepthEnable, false );

	// ポリゴン描画.
	{
		// シェーダー設定.
		context->VSSetShader( g_vertexShader );
		context->PSSetShader( g_pixelShader );

		// 入力レイアウト設定.
		context->IASetInputLayout( g_inputLayout );

		// プリミティブタイプ設定.
		context->IASetPrimitiveType( render::PrimitiveType::kTriangleStrip );

		// 頂点バッファ設定.
		void* mapped = sprite->vtxBuffer[ g_bufferingIndex ]->Map();
		{
			Vertex vtx[ 4 ] =
			{
				{ sprite->rect.x - ( sprite->rect.w / 2.f ), sprite->rect.y + ( sprite->rect.h / 2.f ), 0.0f, sprite->color, 0.0f, 0.0f },
				{ sprite->rect.x - ( sprite->rect.w / 2.f ), sprite->rect.y - ( sprite->rect.h / 2.f ), 0.0f, sprite->color, 0.0f, 1.0f },
				{ sprite->rect.x + ( sprite->rect.w / 2.f ), sprite->rect.y + ( sprite->rect.h / 2.f ), 0.0f, sprite->color, 1.0f, 0.0f },
				{ sprite->rect.x + ( sprite->rect.w / 2.f ), sprite->rect.y - ( sprite->rect.h / 2.f ), 0.0f, sprite->color, 1.0f, 1.0f }
			};
			memcpy( mapped, vtx, sizeof( vtx ) );
		}
		sprite->vtxBuffer[ g_bufferingIndex ]->Unmap();
		context->IASetVertexBuffer( 0, sprite->vtxBuffer[ g_bufferingIndex ], sizeof( Vertex ), 0 );

		// テクスチャ設定.
		{
			// テクスチャ設定.
			context->PSSetShaderResource( 0, sprite->texView );

			// サンプラーステート.
			context->PSSetSamplerState( 0, render::SamplerState::Setting::kFilter, render::Filter::kMinMagMipLinear );
		}

		// 描画.
		context->IASetIndexBuffer( g_indexBuffer, 0 );
		context->DrawIndexed( 4, 0, 0 );
	}
}

void CreateSprite( Sprite** outSprite, CTStr ddsPath )
{
	*outSprite = new Sprite;
	{
		// DDSテクスチャファイル読み込み.
		file::File* ddsFile	= new file::File;
		void*	ddsBuf		= nullptr;
		size_t	ddsBufSize	= 0;

		bool result = ddsFile->Open( ddsPath, file::kOpenModeFlagRead, file::kShareFlagRead );
		AROMA_ASSERT( result, _T( "Failed to file open.\n" ) );
		result = ddsFile->GetFileSize( &ddsBufSize );
		AROMA_ASSERT( result, _T( "Failed to GetFileSize.\n" ) );
		ddsBuf = s_sampleCpuMemAllocator.Alloc( ddsBufSize, 1 );
		result = ddsFile->ReadFile( ddsBuf, ddsBufSize, nullptr );
		AROMA_ASSERT( result, _T( "Failed to ReadFile.\n" ) );
		memory::SafeDelete( ddsFile );

		// DDSからテクスチャリソース作成.
		(*outSprite)->tex = g_device->CreateTexture2DFromDDS( data::DDSAccessor( ddsBuf ), render::Usage::kImmutable, render::kBindFlagShaderResource, 0 );
		s_sampleCpuMemAllocator.Free( ddsBuf );
		{
			render::TextureView::Desc	desc;
			desc.texture	= (*outSprite)->tex;

			(*outSprite)->texView = new render::TextureView;
			(*outSprite)->texView->Initialize( g_device, desc );
		}

		// 頂点バッファ作成.
		for( u32 i = 0; i < kBufferingCount; ++i )
		{
			(*outSprite)->vtxBuffer[ i ] = g_device->CreateVertexBuffer(
				sizeof( Vertex ) * 4, render::Usage::kDynamic, nullptr,
				sizeof( Vertex ), 0 );
		}

		// 初期パラメータ.
		(*outSprite)->rect.x	= 0.0f;
		(*outSprite)->rect.y	= 0.0f;
		(*outSprite)->rect.w	= 1.0f;
		(*outSprite)->rect.h	= 1.0f;
		(*outSprite)->color.Set( 1.0f, 1.0f, 1.0f, 1.0f );
		(*outSprite)->visible	= false;
	}
}

} // namespace

#endif
