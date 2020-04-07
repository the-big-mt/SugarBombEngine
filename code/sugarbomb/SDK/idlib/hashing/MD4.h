/// @file

//*****************************************************************************

#pragma once

//*****************************************************************************

//namespace BFG
//{

/*
===============================================================================

	Calculates a checksum for a block of data
	using the MD4 message-digest algorithm.

===============================================================================
*/

// RB: 64 bit fix, changed long to int
unsigned int MD4_BlockChecksum( const void* data, int length );
// RB end

//} // namespace BFG