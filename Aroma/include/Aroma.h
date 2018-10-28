//===========================================================================
//!
//!	@file		Aroma.h
//!	@brief		Cross-platform video game framework "Aroma".
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

//---------------------------------------------------------------------------
//!	@name	Includes
//---------------------------------------------------------------------------
//! @{
// Precompiled header
#include "aroma/Pch.h"

// common includes
#include "aroma/common/Macro.h"
#include "aroma/common/RefObject.h"
#include "aroma/common/ScopedPtr.h"
#include "aroma/common/Algorithm.h"
#include "aroma/common/BitFlag.h"

// app includes
#include "aroma/app/App.h"

// debug includes
#include "aroma/debug/Assert.h"
#include "aroma/debug/Debug.h"

// data includes
#include "aroma/data/DataDef.h"
#include "aroma/data/Color.h"
#include "aroma/data/DDS.h"
#include "aroma/data/FixedArray.h"

// file includes
#include "aroma/file/FileIO.h"

// util includes
#include "aroma/util/NonCopyable.h"
#include "aroma/util/Singleton.h"

// render includes
#include "aroma/render/Render.h"
#include "aroma/render/RenderDef.h"
#include "aroma/render/MemoryAllocator.h"
#include "aroma/render/Device.h"
#include "aroma/render/BlendState.h"
#include "aroma/render/RenderStateCache.h"
#include "aroma/render/SwapChain.h"
#include "aroma/render/Texture.h"
#include "aroma/render/TextureView.h"
#include "aroma/render/DeferredContext.h"
#include "aroma/render/CommandList.h"
#include "aroma/render/Buffer.h"
#include "aroma/render/Shader.h"
#include "aroma/render/InputLayout.h"
//! @}
