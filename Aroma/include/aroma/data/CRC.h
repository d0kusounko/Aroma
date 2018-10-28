//===========================================================================
//!
//!	@file		CRC.h
//!	@brief		巡回冗長検査.
//!
//!	@author		Copyright (C) DebugCurry. All rights reserved.
//!	@author		d0
//!
//===========================================================================
#pragma once

namespace aroma {
namespace data {

//---------------------------------------------------------------------------
//! @brief		CRC取得モジュール.
//---------------------------------------------------------------------------
class CRC
{
public:
	//-----------------------------------------------------------------------
	//! @brief	CRC32取得.
	//-----------------------------------------------------------------------
	static inline u32 GetCRC(const void* pbuf,u32 bytes );

private:
	static const u32 _CRCtable[ 256 ];
	static const u32 _CRC32Ctable[ 256 ];
	static const u32 _CRC32Ktable[ 256 ];
};

//---------------------------------------------------------------------------
//! @brief	CRC32取得.
//---------------------------------------------------------------------------
u32 CRC::GetCRC( const void *pbuf, u32 bytes )
{
	u32 crc32 = 0xffffffff;

	if(bytes & 3){
		const u8* str = static_cast<const u8*>(pbuf);

		for(u32 n=0; n < bytes; n++){
			crc32 = (crc32 >> 8) ^ _CRCtable[(crc32 & 0xff) ^ str[n]];
		}
		return(crc32);
	}
	else{
		const u32* str = static_cast<const u32*>(pbuf);
		u32 words = bytes/4;
		for(u32 n=0; n < words; n++){
			u32 v = str[n];
#if AROMA_ENDIAN_LE
			v = SwapEndian32( v );
#endif
			crc32 = (crc32 >> 8) ^ _CRCtable[(crc32 & 0xff) ^ ((v>>24)&0xff)];
			crc32 = (crc32 >> 8) ^ _CRCtable[(crc32 & 0xff) ^ ((v>>16)&0xff)];
			crc32 = (crc32 >> 8) ^ _CRCtable[(crc32 & 0xff) ^ ((v>> 8)&0xff)];
			crc32 = (crc32 >> 8) ^ _CRCtable[(crc32 & 0xff) ^ ((v    )&0xff)];
		}

		return(crc32);
	}
}

} // namespace data
} // namespace aroma
