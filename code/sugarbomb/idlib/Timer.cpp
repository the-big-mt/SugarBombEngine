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

//*****************************************************************************

#include "precompiled.h"

#pragma hdrstop

#include "idlib/Timer.h"
//#include "idlib/Heap.h"
//#include "idlib/Lib.h"
//#include "idlib/Str.h"
//#include "idlib/containers/StrList.h"
//#include "idlib/containers/List.h"
//#include "idlib/sys/sys_assert.h"

//#include "CoreLibs/SbSystem/ISystem.hpp"

//*****************************************************************************

//namespace sbe
//{

double idTimer::base = -1.0;


/*
=================
idTimer::InitBaseClockTicks
=================
*/
void idTimer::InitBaseClockTicks() const
{
	idTimer timer;
	double ct, b;
	int i;
	
	base = 0.0;
	b = -1.0;
	for( i = 0; i < 1000; i++ )
	{
		timer.Clear();
		timer.Start();
		timer.Stop();
		ct = timer.ClockTicks();
		if( b < 0.0 || ct < b )
		{
			b = ct;
		}
	}
	base = b;
}


/*
=================
idTimerReport::idTimerReport
=================
*/
idTimerReport::idTimerReport()
{
}

/*
=================
idTimerReport::SetReportName
=================
*/
void idTimerReport::SetReportName( const char* name )
{
	reportName = ( name ) ? name : "Timer Report";
}

/*
=================
idTimerReport::~idTimerReport
=================
*/
idTimerReport::~idTimerReport()
{
	Clear();
}

/*
=================
idTimerReport::AddReport
=================
*/
int idTimerReport::AddReport( const char* name )
{
	if( name && *name )
	{
		names.Append( name );
		return timers.Append( new( TAG_IDLIB ) idTimer() );
	}
	return -1;
}

/*
=================
idTimerReport::Clear
=================
*/
void idTimerReport::Clear()
{
	timers.DeleteContents( true );
	names.Clear();
	reportName.Clear();
}

/*
=================
idTimerReport::Reset
=================
*/
void idTimerReport::Reset()
{
	assert( timers.Num() == names.Num() );
	for( int i = 0; i < timers.Num(); i++ )
	{
		timers[i]->Clear();
	}
}

/*
=================
idTimerReport::AddTime
=================
*/
void idTimerReport::AddTime( const char* name, idTimer* time )
{
	assert( timers.Num() == names.Num() );
	int i;
	for( i = 0; i < names.Num(); i++ )
	{
		if( names[i].Icmp( name ) == 0 )
		{
			*timers[i] += *time;
			break;
		}
	}
	if( i == names.Num() )
	{
		int index = AddReport( name );
		if( index >= 0 )
		{
			timers[index]->Clear();
			*timers[index] += *time;
		}
	}
}

/*
=================
idTimerReport::PrintReport
=================
*/
void idTimerReport::PrintReport()
{
	assert( timers.Num() == names.Num() );
	idLib::sys->Printf( "Timing Report for %s\n", reportName.c_str() );
	idLib::sys->Printf( "-------------------------------\n" );
	float total = 0.0f;
	for( int i = 0; i < names.Num(); i++ )
	{
		idLib::sys->Printf( "%s consumed %5.2f seconds\n", names[i].c_str(), timers[i]->Milliseconds() * 0.001f );
		total += timers[i]->Milliseconds();
	}
	idLib::sys->Printf( "Total time for report %s was %5.2f\n\n", reportName.c_str(), total * 0.001f );
}

//} // namespace sbe