//===========================================================================
//!
//!	@file		Pch.h
//!	@brief		Precompiled header
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

//---------------------------------------------------------------------------
// OS Definitions
//---------------------------------------------------------------------------
#ifndef AROMA_WINDOWS
#if defined( _WIN32 ) || defined( _WIN64 )
#define AROMA_WINDOWS 1
#endif
#endif

//---------------------------------------------------------------------------
// Debug Definition
//---------------------------------------------------------------------------
#ifndef AROMA_DEBUG
#if defined( DEBUG ) || defined( _DEBUG )
#define AROMA_DEBUG 1
#endif
#endif

//---------------------------------------------------------------------------
// Charactor set Definitions
//---------------------------------------------------------------------------
#ifndef AROMA_UNICODE
#if defined( UNICODE ) || defined( _UNICODE )
#define AROMA_UNICODE 1
#endif
#endif

//---------------------------------------------------------------------------
// OS API Includes
//---------------------------------------------------------------------------
#ifdef AROMA_WINDOWS
#include <windows.h>
#endif

//---------------------------------------------------------------------------
// Aroma Includes
//---------------------------------------------------------------------------
#include "common/Typedef.h"
#include "common/Endian.h"
#include "common/Alignment.h"
#include "common/BitFlag.h"
#include "debug/Assert.h"
#include "debug/Debug.h"
#include "memory/Allocator.h"
#include "data/String.h"

//---------------------------------------------------------------------------
// Check Preprocessor Definitions
//---------------------------------------------------------------------------
#ifdef AROMA_RENDER_DX11
#else
#error No graphic API specified. \
Please apply the configuration file "Aroma.props".
#endif
