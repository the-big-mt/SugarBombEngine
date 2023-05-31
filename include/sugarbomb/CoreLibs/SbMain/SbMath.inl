/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2023 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#pragma once

//*****************************************************************************

ID_INLINE byte CLAMP_BYTE( int x )
{
	return ( ( x ) < 0 ? ( 0 ) : ( ( x ) > 255 ? 255 : ( byte )( x ) ) );
}

/*
========================
idMath::InvSqrt
========================
*/
ID_INLINE float SbMath::InvSqrt( float x )
{

	return ( x > FLT_SMALLEST_NON_DENORMAL ) ? sqrtf( 1.0f / x ) : INFINITY;
	
}

/*
========================
idMath::InvSqrt16
========================
*/
ID_INLINE float SbMath::InvSqrt16( float x )
{
	return ( x > FLT_SMALLEST_NON_DENORMAL ) ? sqrtf( 1.0f / x ) : INFINITY;
}

/*
========================
idMath::Sqrt
========================
*/
ID_INLINE float SbMath::Sqrt( float x )
{
	return ( x >= 0.0f ) ?  x * InvSqrt( x ) : 0.0f;
}

/*
========================
idMath::Sqrt16
========================
*/
ID_INLINE float SbMath::Sqrt16( float x )
{
	return ( x >= 0.0f ) ?  x * InvSqrt16( x ) : 0.0f;
}

/*
========================
idMath::Frac
========================
*/
ID_INLINE float SbMath::Frac( float f )
{
	return f - floorf( f );
}

/*
========================
idMath::Sin
========================
*/
ID_INLINE float SbMath::Sin( float a )
{
	return sinf( a );
}

/*
========================
idMath::Sin16
========================
*/
ID_INLINE float SbMath::Sin16( float a )
{
	float s;
	
	if( ( a < 0.0f ) || ( a >= TWO_PI ) )
	{
		a -= floorf( a * ONEOVER_TWOPI ) * TWO_PI;
	}
#if 1
	if( a < PI )
	{
		if( a > HALF_PI )
		{
			a = PI - a;
		}
	}
	else
	{
		if( a > PI + HALF_PI )
		{
			a = a - TWO_PI;
		}
		else
		{
			a = PI - a;
		}
	}
#else
	a = PI - a;
	if( fabsf( a ) >= HALF_PI )
	{
		a = ( ( a < 0.0f ) ? -PI : PI ) - a;
	}
#endif
	s = a * a;
	return a * ( ( ( ( ( -2.39e-08f * s + 2.7526e-06f ) * s - 1.98409e-04f ) * s + 8.3333315e-03f ) * s - 1.666666664e-01f ) * s + 1.0f );
}

/*
========================
idMath::Cos
========================
*/
ID_INLINE float SbMath::Cos( float a )
{
	return cosf( a );
}

/*
========================
idMath::Cos16
========================
*/
ID_INLINE float SbMath::Cos16( float a )
{
	float s, d;
	
	if( ( a < 0.0f ) || ( a >= TWO_PI ) )
	{
		a -= floorf( a * ONEOVER_TWOPI ) * TWO_PI;
	}
#if 1
	if( a < PI )
	{
		if( a > HALF_PI )
		{
			a = PI - a;
			d = -1.0f;
		}
		else
		{
			d = 1.0f;
		}
	}
	else
	{
		if( a > PI + HALF_PI )
		{
			a = a - TWO_PI;
			d = 1.0f;
		}
		else
		{
			a = PI - a;
			d = -1.0f;
		}
	}
#else
	a = PI - a;
	if( fabsf( a ) >= HALF_PI )
	{
		a = ( ( a < 0.0f ) ? -PI : PI ) - a;
		d = 1.0f;
	}
	else
	{
		d = -1.0f;
	}
#endif
	s = a * a;
	return d * ( ( ( ( ( -2.605e-07f * s + 2.47609e-05f ) * s - 1.3888397e-03f ) * s + 4.16666418e-02f ) * s - 4.999999963e-01f ) * s + 1.0f );
}

/*
========================
idMath::SinCos
========================
*/
ID_INLINE void SbMath::SinCos( float a, float& s, float& c )
{
#if defined(_MSC_VER) && defined(_M_IX86)
	_asm
	{
		fld		a
		fsincos
		mov		ecx, c
		mov		edx, s
		fstp	dword ptr [ecx]
		fstp	dword ptr [edx]
	}
#else
	// DG: non-MSVC version
	s = sinf( a );
	c = cosf( a );
	// DG end
#endif
}

/*
========================
idMath::SinCos16
========================
*/
ID_INLINE void SbMath::SinCos16( float a, float& s, float& c )
{
	float t, d;
	
	if( ( a < 0.0f ) || ( a >= TWO_PI ) )
	{
		a -= floorf( a * ONEOVER_TWOPI ) * TWO_PI;
	}
#if 1
	if( a < PI )
	{
		if( a > HALF_PI )
		{
			a = PI - a;
			d = -1.0f;
		}
		else
		{
			d = 1.0f;
		}
	}
	else
	{
		if( a > PI + HALF_PI )
		{
			a = a - TWO_PI;
			d = 1.0f;
		}
		else
		{
			a = PI - a;
			d = -1.0f;
		}
	}
#else
	a = PI - a;
	if( fabsf( a ) >= HALF_PI )
	{
		a = ( ( a < 0.0f ) ? -PI : PI ) - a;
		d = 1.0f;
	}
	else
	{
		d = -1.0f;
	}
#endif
	t = a * a;
	s = a * ( ( ( ( ( -2.39e-08f * t + 2.7526e-06f ) * t - 1.98409e-04f ) * t + 8.3333315e-03f ) * t - 1.666666664e-01f ) * t + 1.0f );
	c = d * ( ( ( ( ( -2.605e-07f * t + 2.47609e-05f ) * t - 1.3888397e-03f ) * t + 4.16666418e-02f ) * t - 4.999999963e-01f ) * t + 1.0f );
}

/*
========================
idMath::Tan
========================
*/
ID_INLINE float SbMath::Tan( float a )
{
	return tanf( a );
}

/*
========================
idMath::Tan16
========================
*/
ID_INLINE float SbMath::Tan16( float a )
{
	float s;
	bool reciprocal;
	
	if( ( a < 0.0f ) || ( a >= PI ) )
	{
		a -= floorf( a * ONEOVER_PI ) * PI;
	}
#if 1
	if( a < HALF_PI )
	{
		if( a > ONEFOURTH_PI )
		{
			a = HALF_PI - a;
			reciprocal = true;
		}
		else
		{
			reciprocal = false;
		}
	}
	else
	{
		if( a > HALF_PI + ONEFOURTH_PI )
		{
			a = a - PI;
			reciprocal = false;
		}
		else
		{
			a = HALF_PI - a;
			reciprocal = true;
		}
	}
#else
	a = HALF_PI - a;
	if( fabsf( a ) >= ONEFOURTH_PI )
	{
		a = ( ( a < 0.0f ) ? -HALF_PI : HALF_PI ) - a;
		reciprocal = false;
	}
	else
	{
		reciprocal = true;
	}
#endif
	s = a * a;
	s = a * ( ( ( ( ( ( 9.5168091e-03f * s + 2.900525e-03f ) * s + 2.45650893e-02f ) * s + 5.33740603e-02f ) * s + 1.333923995e-01f ) * s + 3.333314036e-01f ) * s + 1.0f );
	if( reciprocal )
	{
		return 1.0f / s;
	}
	else
	{
		return s;
	}
}

/*
========================
idMath::ASin
========================
*/
ID_INLINE float SbMath::ASin( float a )
{
	if( a <= -1.0f )
	{
		return -HALF_PI;
	}
	if( a >= 1.0f )
	{
		return HALF_PI;
	}
	return asinf( a );
}

/*
========================
idMath::ASin16
========================
*/
ID_INLINE float SbMath::ASin16( float a )
{
	if( a < 0.0f )
	{
		if( a <= -1.0f )
		{
			return -HALF_PI;
		}
		a = fabsf( a );
		return ( ( ( -0.0187293f * a + 0.0742610f ) * a - 0.2121144f ) * a + 1.5707288f ) * SbMath::Sqrt( 1.0f - a ) - HALF_PI;
	}
	else
	{
		if( a >= 1.0f )
		{
			return HALF_PI;
		}
		return HALF_PI - ( ( ( -0.0187293f * a + 0.0742610f ) * a - 0.2121144f ) * a + 1.5707288f ) * SbMath::Sqrt( 1.0f - a );
	}
}

/*
========================
idMath::ACos
========================
*/
ID_INLINE float SbMath::ACos( float a )
{
	if( a <= -1.0f )
	{
		return PI;
	}
	if( a >= 1.0f )
	{
		return 0.0f;
	}
	return acosf( a );
}

/*
========================
idMath::ACos16
========================
*/
ID_INLINE float SbMath::ACos16( float a )
{
	if( a < 0.0f )
	{
		if( a <= -1.0f )
		{
			return PI;
		}
		a = fabsf( a );
		return PI - ( ( ( -0.0187293f * a + 0.0742610f ) * a - 0.2121144f ) * a + 1.5707288f ) * SbMath::Sqrt( 1.0f - a );
	}
	else
	{
		if( a >= 1.0f )
		{
			return 0.0f;
		}
		return ( ( ( -0.0187293f * a + 0.0742610f ) * a - 0.2121144f ) * a + 1.5707288f ) * SbMath::Sqrt( 1.0f - a );
	}
}

/*
========================
idMath::ATan
========================
*/
ID_INLINE float SbMath::ATan( float a )
{
	return atanf( a );
}

/*
========================
idMath::ATan16
========================
*/
ID_INLINE float SbMath::ATan16( float a )
{
	float s;
	if( fabsf( a ) > 1.0f )
	{
		a = 1.0f / a;
		s = a * a;
		s = - ( ( ( ( ( ( ( ( ( 0.0028662257f * s - 0.0161657367f ) * s + 0.0429096138f ) * s - 0.0752896400f )
						  * s + 0.1065626393f ) * s - 0.1420889944f ) * s + 0.1999355085f ) * s - 0.3333314528f ) * s ) + 1.0f ) * a;
		if( a < 0.0f )
		{
			return s - HALF_PI;
		}
		else
		{
			return s + HALF_PI;
		}
	}
	else
	{
		s = a * a;
		return ( ( ( ( ( ( ( ( ( 0.0028662257f * s - 0.0161657367f ) * s + 0.0429096138f ) * s - 0.0752896400f )
						   * s + 0.1065626393f ) * s - 0.1420889944f ) * s + 0.1999355085f ) * s - 0.3333314528f ) * s ) + 1.0f ) * a;
	}
}

/*
========================
idMath::ATan
========================
*/
ID_INLINE float SbMath::ATan( float y, float x )
{
	assert( fabs( y ) > SbMath::FLT_SMALLEST_NON_DENORMAL || fabs( x ) > SbMath::FLT_SMALLEST_NON_DENORMAL );
	return atan2f( y, x );
}

/*
========================
idMath::ATan16
========================
*/
ID_INLINE float SbMath::ATan16( float y, float x )
{
	assert( fabs( y ) > SbMath::FLT_SMALLEST_NON_DENORMAL || fabs( x ) > SbMath::FLT_SMALLEST_NON_DENORMAL );
	
	float a, s;
	if( fabsf( y ) > fabsf( x ) )
	{
		a = x / y;
		s = a * a;
		s = - ( ( ( ( ( ( ( ( ( 0.0028662257f * s - 0.0161657367f ) * s + 0.0429096138f ) * s - 0.0752896400f )
						  * s + 0.1065626393f ) * s - 0.1420889944f ) * s + 0.1999355085f ) * s - 0.3333314528f ) * s ) + 1.0f ) * a;
		if( a < 0.0f )
		{
			return s - HALF_PI;
		}
		else
		{
			return s + HALF_PI;
		}
	}
	else
	{
		a = y / x;
		s = a * a;
		return ( ( ( ( ( ( ( ( ( 0.0028662257f * s - 0.0161657367f ) * s + 0.0429096138f ) * s - 0.0752896400f )
						   * s + 0.1065626393f ) * s - 0.1420889944f ) * s + 0.1999355085f ) * s - 0.3333314528f ) * s ) + 1.0f ) * a;
	}
}

/*
========================
idMath::Pow
========================
*/
ID_INLINE float SbMath::Pow( float x, float y )
{
	return powf( x, y );
}

/*
========================
idMath::Pow16
========================
*/
ID_INLINE float SbMath::Pow16( float x, float y )
{
	return Exp16( y * Log16( x ) );
}

/*
========================
idMath::Exp
========================
*/
ID_INLINE float SbMath::Exp( float f )
{
	return expf( f );
}

/*
========================
idMath::Exp16
========================
*/
ID_INLINE float SbMath::Exp16( float f )
{
	float x = f * 1.44269504088896340f;		// multiply with ( 1 / log( 2 ) )
#if 1
	int i = *reinterpret_cast<int*>( &x );
	int s = ( i >> IEEE_FLT_SIGN_BIT );
	int e = ( ( i >> IEEE_FLT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLT_EXPONENT_BIAS;
	int m = ( i & ( ( 1 << IEEE_FLT_MANTISSA_BITS ) - 1 ) ) | ( 1 << IEEE_FLT_MANTISSA_BITS );
	i = ( ( m >> ( IEEE_FLT_MANTISSA_BITS - e ) ) & ~( e >> INT32_SIGN_BIT ) ) ^ s;
#else
	int i = ( int ) x;
	if( x < 0.0f )
	{
		i--;
	}
#endif
	int exponent = ( i + IEEE_FLT_EXPONENT_BIAS ) << IEEE_FLT_MANTISSA_BITS;
	float y = *reinterpret_cast<float*>( &exponent );
	x -= ( float ) i;
	if( x >= 0.5f )
	{
		x -= 0.5f;
		y *= 1.4142135623730950488f;	// multiply with sqrt( 2 )
	}
	float x2 = x * x;
	float p = x * ( 7.2152891511493f + x2 * 0.0576900723731f );
	float q = 20.8189237930062f + x2;
	x = y * ( q + p ) / ( q - p );
	return x;
}

/*
========================
idMath::Log
========================
*/
ID_INLINE float SbMath::Log( float f )
{
	return logf( f );
}

/*
========================
idMath::Log16
========================
*/
ID_INLINE float SbMath::Log16( float f )
{
	int i = *reinterpret_cast<int*>( &f );
	int exponent = ( ( i >> IEEE_FLT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLT_EXPONENT_BIAS;
	i -= ( exponent + 1 ) << IEEE_FLT_MANTISSA_BITS;	// get value in the range [.5, 1>
	float y = *reinterpret_cast<float*>( &i );
	y *= 1.4142135623730950488f;						// multiply with sqrt( 2 )
	y = ( y - 1.0f ) / ( y + 1.0f );
	float y2 = y * y;
	y = y * ( 2.000000000046727f + y2 * ( 0.666666635059382f + y2 * ( 0.4000059794795f + y2 * ( 0.28525381498f + y2 * 0.2376245609f ) ) ) );
	y += 0.693147180559945f * ( ( float )exponent + 0.5f );
	return y;
}

/*
========================
idMath::IPow
========================
*/
ID_INLINE int SbMath::IPow( int x, int y )
{
	int r;
	for( r = x; y > 1; y-- )
	{
		r *= x;
	}
	return r;
}

/*
========================
idMath::ILog2
========================
*/
ID_INLINE int SbMath::ILog2( float f )
{
	return ( ( ( *reinterpret_cast<int*>( &f ) ) >> IEEE_FLT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLT_EXPONENT_BIAS;
}

/*
========================
idMath::ILog2
========================
*/
ID_INLINE int SbMath::ILog2( int i )
{
	return ILog2( ( float )i );
}

/*
========================
idMath::BitsForFloat
========================
*/
ID_INLINE int SbMath::BitsForFloat( float f )
{
	return ILog2( f ) + 1;
}

/*
========================
idMath::BitsForInteger
========================
*/
ID_INLINE int SbMath::BitsForInteger( int i )
{
	return ILog2( ( float )i ) + 1;
}

/*
========================
idMath::MaskForFloatSign
========================
*/
ID_INLINE int SbMath::MaskForFloatSign( float f )
{
	return ( ( *reinterpret_cast<int*>( &f ) ) >> IEEE_FLT_SIGN_BIT );
}

/*
========================
idMath::MaskForIntegerSign
========================
*/
ID_INLINE int SbMath::MaskForIntegerSign( int i )
{
	return ( i >> INT32_SIGN_BIT );
}

/*
========================
idMath::FloorPowerOfTwo
========================
*/
ID_INLINE int SbMath::FloorPowerOfTwo( int x )
{
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x++;
	return x >> 1;
}

/*
========================
idMath::CeilPowerOfTwo
========================
*/
ID_INLINE int SbMath::CeilPowerOfTwo( int x )
{
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x++;
	return x;
}

/*
========================
idMath::IsPowerOfTwo
========================
*/
ID_INLINE bool SbMath::IsPowerOfTwo( int x )
{
	return ( x & ( x - 1 ) ) == 0 && x > 0;
}

/*
========================
idMath::BitCount
========================
*/
ID_INLINE int SbMath::BitCount( int x )
{
	x -= ( ( x >> 1 ) & 0x55555555 );
	x = ( ( ( x >> 2 ) & 0x33333333 ) + ( x & 0x33333333 ) );
	x = ( ( ( x >> 4 ) + x ) & 0x0f0f0f0f );
	x += ( x >> 8 );
	return ( ( x + ( x >> 16 ) ) & 0x0000003f );
}

/*
========================
idMath::BitReverse
========================
*/
ID_INLINE int SbMath::BitReverse( int x )
{
	x = ( ( ( x >> 1 ) & 0x55555555 ) | ( ( x & 0x55555555 ) << 1 ) );
	x = ( ( ( x >> 2 ) & 0x33333333 ) | ( ( x & 0x33333333 ) << 2 ) );
	x = ( ( ( x >> 4 ) & 0x0f0f0f0f ) | ( ( x & 0x0f0f0f0f ) << 4 ) );
	x = ( ( ( x >> 8 ) & 0x00ff00ff ) | ( ( x & 0x00ff00ff ) << 8 ) );
	return ( ( x >> 16 ) | ( x << 16 ) );
}

/*
========================
idMath::Abs
========================
*/
ID_INLINE int SbMath::Abs( int x )
{
#if 1
	return abs( x );
#else
	int y = x >> INT32_SIGN_BIT;
	return ( ( x ^ y ) - y );
#endif
}

/*
========================
idMath::Fabs
========================
*/
ID_INLINE float SbMath::Fabs( float f )
{
#if 1
	return fabsf( f );
#else
	int tmp = *reinterpret_cast<int*>( &f );
	tmp &= 0x7FFFFFFF;
	return *reinterpret_cast<float*>( &tmp );
#endif
}

/*
========================
idMath::Floor
========================
*/
ID_INLINE float SbMath::Floor( float f )
{
	return floorf( f );
}

/*
========================
idMath::Ceil
========================
*/
ID_INLINE float SbMath::Ceil( float f )
{
	return ceilf( f );
}

/*
========================
idMath::Rint
========================
*/
ID_INLINE float SbMath::Rint( float f )
{
	return floorf( f + 0.5f );
}


/*
========================
idMath::Ftoi
========================
*/
ID_INLINE int SbMath::Ftoi( float f )
{
	// If a converted result is larger than the maximum signed doubleword integer,
	// the floating-point invalid exception is raised, and if this exception is masked,
	// the indefinite integer value (80000000H) is returned.
#if defined(SBE_USE_INTRINSICS)
	__m128 x = _mm_load_ss( &f );
	return _mm_cvttss_si32( x );
#elif 0 // round chop (C/C++ standard)
	int i, s, e, m, shift;
	i = *reinterpret_cast<int*>( &f );
	s = i >> IEEE_FLT_SIGN_BIT;
	e = ( ( i >> IEEE_FLT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLT_EXPONENT_BIAS;
	m = ( i & ( ( 1 << IEEE_FLT_MANTISSA_BITS ) - 1 ) ) | ( 1 << IEEE_FLT_MANTISSA_BITS );
	shift = e - IEEE_FLT_MANTISSA_BITS;
	return ( ( ( ( m >> -shift ) | ( m << shift ) ) & ~( e >> INT32_SIGN_BIT ) ) ^ s ) - s;
#else
	// If a converted result is larger than the maximum signed doubleword integer the result is undefined.
	return C_FLOAT_TO_INT( f );
#endif
}

/*
========================
idMath::Ftoi8
========================
*/
ID_INLINE char SbMath::Ftoi8( float f )
{
#if defined(SBE_USE_INTRINSICS)
	__m128 x = _mm_load_ss( &f );
	x = _mm_max_ss( x, SIMD_SP_min_char );
	x = _mm_min_ss( x, SIMD_SP_max_char );
	return static_cast<char>( _mm_cvttss_si32( x ) );
#else
	// The converted result is clamped to the range [-128,127].
	int i = C_FLOAT_TO_INT( f );
	if( i < -128 )
	{
		return -128;
	}
	else if( i > 127 )
	{
		return 127;
	}
	return static_cast<char>( i );
#endif
}

/*
========================
idMath::Ftoi16
========================
*/
ID_INLINE short SbMath::Ftoi16( float f )
{
#if defined(SBE_USE_INTRINSICS)
	__m128 x = _mm_load_ss( &f );
	x = _mm_max_ss( x, SIMD_SP_min_short );
	x = _mm_min_ss( x, SIMD_SP_max_short );
	return static_cast<short>( _mm_cvttss_si32( x ) );
#else
	// The converted result is clamped to the range [-32768,32767].
	int i = C_FLOAT_TO_INT( f );
	if( i < -32768 )
	{
		return -32768;
	}
	else if( i > 32767 )
	{
		return 32767;
	}
	return static_cast<short>( i );
#endif
}

/*
========================
idMath::Ftoui16
========================
*/
ID_INLINE unsigned short SbMath::Ftoui16( float f )
{
	// TO DO - SSE ??
	
	// The converted result is clamped to the range [-32768,32767].
	int i = C_FLOAT_TO_INT( f );
	if( i < 0 )
	{
		return 0;
	}
	else if( i > 65535 )
	{
		return 65535;
	}
	return static_cast<unsigned short>( i );
}

/*
========================
idMath::Ftob
========================
*/
ID_INLINE byte SbMath::Ftob( float f )
{
	// If a converted result is negative the value (0) is returned and if the
	// converted result is larger than the maximum byte the value (255) is returned.
	
#if defined(SBE_USE_INTRINSICS)
	__m128 x = _mm_load_ss( &f );
	x = _mm_max_ss( x, SIMD_SP_zero );
	x = _mm_min_ss( x, SIMD_SP_255 );
	return static_cast<byte>( _mm_cvttss_si32( x ) );
#else
	// The converted result is clamped to the range [0,255].
	int i = C_FLOAT_TO_INT( f );
	if( i < 0 )
	{
		return 0;
	}
	else if( i > 255 )
	{
		return 255;
	}
	return static_cast<byte>( i );
#endif
}

/*
========================
idMath::ClampChar
========================
*/
ID_INLINE signed char SbMath::ClampChar( int i )
{
	if( i < -128 )
	{
		return -128;
	}
	if( i > 127 )
	{
		return 127;
	}
	return static_cast<signed char>( i );
}

/*
========================
idMath::ClampShort
========================
*/
ID_INLINE signed short SbMath::ClampShort( int i )
{
	if( i < -32768 )
	{
		return -32768;
	}
	if( i > 32767 )
	{
		return 32767;
	}
	return static_cast<signed short>( i );
}

/*
========================
idMath::ClampInt
========================
*/
ID_INLINE int SbMath::ClampInt( int min, int max, int value )
{
	if( value < min )
	{
		return min;
	}
	if( value > max )
	{
		return max;
	}
	return value;
}

/*
========================
idMath::ClampFloat
========================
*/
ID_INLINE float SbMath::ClampFloat( float min, float max, float value )
{
	return Max( min, Min( max, value ) );
}

/*
========================
idMath::AngleNormalize360
========================
*/
ID_INLINE float SbMath::AngleNormalize360( float angle )
{
	if( ( angle >= 360.0f ) || ( angle < 0.0f ) )
	{
		angle -= floorf( angle * ( 1.0f / 360.0f ) ) * 360.0f;
	}
	return angle;
}

/*
========================
idMath::AngleNormalize180
========================
*/
ID_INLINE float SbMath::AngleNormalize180( float angle )
{
	angle = AngleNormalize360( angle );
	if( angle > 180.0f )
	{
		angle -= 360.0f;
	}
	return angle;
}

/*
========================
idMath::AngleDelta
========================
*/
ID_INLINE float SbMath::AngleDelta( float angle1, float angle2 )
{
	return AngleNormalize180( angle1 - angle2 );
}

/*
========================
idMath::FloatHash
========================
*/
ID_INLINE int SbMath::FloatHash( const float* array, const int numFloats )
{
	int i, hash = 0;
	const int* ptr;
	
	ptr = reinterpret_cast<const int*>( array );
	for( i = 0; i < numFloats; i++ )
	{
		hash ^= ptr[i];
	}
	return hash;
}

template< typename T >
ID_INLINE
T Lerp( const T from, const T to, float f )
{
	return from + ( ( to - from ) * f );
}

template<>
ID_INLINE
int Lerp( const int from, const int to, float f )
{
	return SbMath::Ftoi( ( float ) from + ( ( ( float ) to - ( float ) from ) * f ) );
}


/*
========================
LerpToWithScale

Lerps from "cur" to "dest", scaling the delta to change by "scale"
If the delta between "cur" and "dest" is very small, dest is returned to prevent denormals.
========================
*/
inline float SbMath::LerpToWithScale( const float cur, const float dest, const float scale )
{
	float delta = dest - cur;
	if( delta > -1.0e-6f && delta < 1.0e-6f )
	{
		return dest;
	}
	return cur + ( dest - cur ) * scale;
}