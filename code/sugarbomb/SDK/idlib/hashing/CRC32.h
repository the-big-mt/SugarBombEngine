/// @file

//*****************************************************************************

#pragma once

//*****************************************************************************

//namespace BFG
//{

/*
===============================================================================

	Calculates a checksum for a block of data
	using the CRC-32.

===============================================================================
*/

// RB: 64 bit fixes, changed long to int
void CRC32_InitChecksum( unsigned int& crcvalue );
void CRC32_UpdateChecksum( unsigned int& crcvalue, const void* data, int length );
void CRC32_FinishChecksum( unsigned int& crcvalue );
unsigned int CRC32_BlockChecksum( const void* data, int length );
// RB end

//} // namespace BFG