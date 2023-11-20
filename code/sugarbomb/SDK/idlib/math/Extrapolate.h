/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2019-2020 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus
subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license 
header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the 
SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#pragma once

//*****************************************************************************

/*
==============================================================================================

	Extrapolation

==============================================================================================
*/

typedef enum
{
	EXTRAPOLATION_NONE			= 0x01,	// no extrapolation, covered distance = duration * 0.001 * ( baseSpeed )
	EXTRAPOLATION_LINEAR		= 0x02,	// linear extrapolation, covered distance = duration * 0.001 * ( baseSpeed + speed )
	EXTRAPOLATION_ACCELLINEAR	= 0x04,	// linear acceleration, covered distance = duration * 0.001 * ( baseSpeed + 0.5 * speed )
	EXTRAPOLATION_DECELLINEAR	= 0x08,	// linear deceleration, covered distance = duration * 0.001 * ( baseSpeed + 0.5 * speed )
	EXTRAPOLATION_ACCELSINE		= 0x10,	// sinusoidal acceleration, covered distance = duration * 0.001 * ( baseSpeed + sqrt( 0.5 ) * speed )
	EXTRAPOLATION_DECELSINE		= 0x20,	// sinusoidal deceleration, covered distance = duration * 0.001 * ( baseSpeed + sqrt( 0.5 ) * speed )
	EXTRAPOLATION_NOSTOP		= 0x40	// do not stop at startTime + duration
} extrapolation_t;

template< class type >
class idExtrapolate
{
public:
	idExtrapolate();
	
	void				Init( const int startTime, const int duration, const type& startValue, const type& baseSpeed, const type& speed, const extrapolation_t extrapolationType );
	type				GetCurrentValue( int time ) const;
	type				GetCurrentSpeed( int time ) const;
	bool				IsDone( int time ) const
	{
		return ( !( extrapolationType & EXTRAPOLATION_NOSTOP ) && time >= startTime + duration );
	}
	void				SetStartTime( int time )
	{
		startTime = time;
	}
	int					GetStartTime() const
	{
		return startTime;
	}
	int					GetEndTime() const
	{
		return ( !( extrapolationType & EXTRAPOLATION_NOSTOP ) && duration > 0 ) ? startTime + duration : 0;
	}
	int					GetDuration() const
	{
		return duration;
	}
	void				SetStartValue( const type& value )
	{
		startValue = value;
	}
	const type& 		GetStartValue() const
	{
		return startValue;
	}
	const type& 		GetBaseSpeed() const
	{
		return baseSpeed;
	}
	const type& 		GetSpeed() const
	{
		return speed;
	}
	extrapolation_t		GetExtrapolationType() const
	{
		return extrapolationType;
	}
	
private:
	extrapolation_t		extrapolationType;
	int					startTime;
	int					duration;
	type				startValue;
	type				baseSpeed;
	type				speed;
};

/*
====================
idExtrapolate::idExtrapolate
====================
*/
template< class type >
ID_INLINE idExtrapolate<type>::idExtrapolate()
{
	extrapolationType = EXTRAPOLATION_NONE;
	startTime = duration = 0.0f;
	memset( &startValue, 0, sizeof( startValue ) );
	memset( &baseSpeed, 0, sizeof( baseSpeed ) );
	memset( &speed, 0, sizeof( speed ) );
}

/*
====================
idExtrapolate::Init
====================
*/
template< class type >
ID_INLINE void idExtrapolate<type>::Init( const int startTime, const int duration, const type& startValue, const type& baseSpeed, const type& speed, const extrapolation_t extrapolationType )
{
	this->extrapolationType = extrapolationType;
	this->startTime = startTime;
	this->duration = duration;
	this->startValue = startValue;
	this->baseSpeed = baseSpeed;
	this->speed = speed;
}

/*
====================
idExtrapolate::GetCurrentValue
====================
*/
template< class type >
ID_INLINE type idExtrapolate<type>::GetCurrentValue( int time ) const
{
	if( time < startTime )
	{
		return startValue;
	}
	
	if( !( extrapolationType &	EXTRAPOLATION_NOSTOP ) && ( time > startTime + duration ) )
	{
		time = startTime + duration;
	}
	
	switch( extrapolationType & ~EXTRAPOLATION_NOSTOP )
	{
		case EXTRAPOLATION_NONE:
		{
			const float deltaTime = ( time - startTime ) * 0.001f;
			return startValue + deltaTime * baseSpeed;
		}
		case EXTRAPOLATION_LINEAR:
		{
			const float deltaTime = ( time - startTime ) * 0.001f;
			return startValue + deltaTime * ( baseSpeed + speed );
		}
		case EXTRAPOLATION_ACCELLINEAR:
		{
			if( duration == 0 )
			{
				return startValue;
			}
			else
			{
				const float deltaTime = ( time - startTime ) / ( float )duration;
				const float s = ( 0.5f * deltaTime * deltaTime ) * ( ( float )duration * 0.001f );
				return startValue + deltaTime * baseSpeed + s * speed;
			}
		}
		case EXTRAPOLATION_DECELLINEAR:
		{
			if( duration == 0 )
			{
				return startValue;
			}
			else
			{
				const float deltaTime = ( time - startTime ) / ( float )duration;
				const float s = ( deltaTime - ( 0.5f * deltaTime * deltaTime ) ) * ( ( float )duration * 0.001f );
				return startValue + deltaTime * baseSpeed + s * speed;
			}
		}
		case EXTRAPOLATION_ACCELSINE:
		{
			if( duration == 0 )
			{
				return startValue;
			}
			else
			{
				const float deltaTime = ( time - startTime ) / ( float )duration;
				const float s = ( 1.0f - idMath::Cos( deltaTime * idMath::HALF_PI ) ) * ( float )duration * 0.001f * idMath::SQRT_1OVER2;
				return startValue + deltaTime * baseSpeed + s * speed;
			}
		}
		case EXTRAPOLATION_DECELSINE:
		{
			if( duration == 0 )
			{
				return startValue;
			}
			else
			{
				const float deltaTime = ( time - startTime ) / ( float )duration;
				const float s = idMath::Sin( deltaTime * idMath::HALF_PI ) * ( float )duration * 0.001f * idMath::SQRT_1OVER2;
				return startValue + deltaTime * baseSpeed + s * speed;
			}
		}
	}
	return startValue;
}

/*
====================
idExtrapolate::GetCurrentSpeed
====================
*/
template< class type >
ID_INLINE type idExtrapolate<type>::GetCurrentSpeed( int time ) const
{
	if( time < startTime || duration == 0 )
	{
		return ( startValue - startValue ); //-V501
	}
	
	if( !( extrapolationType &	EXTRAPOLATION_NOSTOP ) && ( time > startTime + duration ) )
	{
		return ( startValue - startValue ); //-V501
	}
	
	switch( extrapolationType & ~EXTRAPOLATION_NOSTOP )
	{
		case EXTRAPOLATION_NONE:
		{
			return baseSpeed;
		}
		case EXTRAPOLATION_LINEAR:
		{
			return baseSpeed + speed;
		}
		case EXTRAPOLATION_ACCELLINEAR:
		{
			const float deltaTime = ( time - startTime ) / ( float )duration;
			const float s = deltaTime;
			return baseSpeed + s * speed;
		}
		case EXTRAPOLATION_DECELLINEAR:
		{
			const float deltaTime = ( time - startTime ) / ( float )duration;
			const float s = 1.0f - deltaTime;
			return baseSpeed + s * speed;
		}
		case EXTRAPOLATION_ACCELSINE:
		{
			const float deltaTime = ( time - startTime ) / ( float )duration;
			const float s = idMath::Sin( deltaTime * idMath::HALF_PI );
			return baseSpeed + s * speed;
		}
		case EXTRAPOLATION_DECELSINE:
		{
			const float deltaTime = ( time - startTime ) / ( float )duration;
			const float s = idMath::Cos( deltaTime * idMath::HALF_PI );
			return baseSpeed + s * speed;
		}
		default:
		{
			return baseSpeed;
		}
	}
}