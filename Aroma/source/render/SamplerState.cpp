//===========================================================================
//!
//!	@file		SamplerState.cpp
//!	@brief		サンプラーステート.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#include <aroma/render/SamplerState.h>

namespace aroma {
namespace render {

#define STATE_CASE_SET( _setting, _m_Value )	\
	case Setting::_setting:						\
		if( _m_Value != value )					\
		{										\
			_m_Value = value;					\
			return true;						\
		}										\
		break;									\

//---------------------------------------------------------------------------
//! @brief		ステート設定.
//! @param[in]	value	設定する値.
//!	@retval		true	: 値が変更された.
//! @retval		false	: 値が変更されなかった.
//---------------------------------------------------------------------------
bool SamplerState::Set( Setting state, f32 value )
{
	switch( state )
	{
	STATE_CASE_SET( kMipLODBias, mipLODBias );
	STATE_CASE_SET( kMinLOD, minLOD );
	STATE_CASE_SET( kMaxLOD, maxLOD );

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}

bool SamplerState::Set( Setting state, Filter value )
{
	switch( state )
	{
	STATE_CASE_SET( kFilter, filter );

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}

bool SamplerState::Set( Setting state, TextureAddress value )
{
	switch( state )
	{
	STATE_CASE_SET( kAddressU, addressU );
	STATE_CASE_SET( kAddressV, addressV );
	STATE_CASE_SET( kAddressW, addressW );

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}

bool SamplerState::Set( Setting state, AnisotropicRatio value )
{
	switch( state )
	{
	STATE_CASE_SET( kMaxAnisotropy, maxAnisotropy );

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}

bool SamplerState::Set( Setting state, const data::Color& value )
{
	switch( state )
	{
	STATE_CASE_SET( kBorderColor, borderColor );

	default:
		AROMA_ASSERT( false, "There is no state corresponding to type." );
		break;
	}

	return false;
}


} // namespace render
} // namespace aroma

