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

#pragma hdrstop

#include "precompiled.h"

//*****************************************************************************

/*
================================================================================================
Contains the vartious ThreadingClass implementations.
================================================================================================
*/

/*
================================================================================================

	idSysThread

================================================================================================
*/

/*
========================
idSysThread::idSysThread
========================
*/
idSysThread::idSysThread() :
	threadHandle( 0 ),
	isWorker( false ),
	isRunning( false ),
	isTerminating( false ),
	moreWorkToDo( false ),
	signalWorkerDone( true )
{
}

/*
========================
idSysThread::~idSysThread
========================
*/
idSysThread::~idSysThread()
{
	StopThread( true );
	if( threadHandle )
	{
		Sys_DestroyThread( threadHandle );
	}
}

/*
========================
idSysThread::StartThread
========================
*/
bool idSysThread::StartThread( const char* name_, core_t core, xthreadPriority priority, int stackSize )
{
	if( isRunning )
	{
		return false;
	}
	
	name = name_;
	
	isTerminating = false;
	
	if( threadHandle )
	{
		Sys_DestroyThread( threadHandle );
	}
	
	threadHandle = Sys_CreateThread( ( xthread_t )ThreadProc, this, priority, name, core, stackSize, false );
	
	isRunning = true;
	return true;
}

/*
========================
idSysThread::StartWorkerThread
========================
*/
bool idSysThread::StartWorkerThread( const char* name_, core_t core, xthreadPriority priority, int stackSize )
{
	if( isRunning )
	{
		return false;
	}
	
	isWorker = true;
	
	bool result = StartThread( name_, core, priority, stackSize );
	
	signalWorkerDone.Wait( idSysSignal::WAIT_INFINITE );
	
	return result;
}

/*
========================
idSysThread::StopThread
========================
*/
void idSysThread::StopThread( bool wait )
{
	if( !isRunning )
	{
		return;
	}
	if( isWorker )
	{
		signalMutex.Lock();
		moreWorkToDo = true;
		signalWorkerDone.Clear();
		isTerminating = true;
		signalMoreWorkToDo.Raise();
		signalMutex.Unlock();
	}
	else
	{
		isTerminating = true;
	}
	if( wait )
	{
		WaitForThread();
	}
}

/*
========================
idSysThread::WaitForThread
========================
*/
void idSysThread::WaitForThread()
{
	if( isWorker )
	{
		signalWorkerDone.Wait( idSysSignal::WAIT_INFINITE );
	}
	else if( isRunning )
	{
		Sys_DestroyThread( threadHandle );
		threadHandle = 0;
	}
}

/*
========================
idSysThread::SignalWork
========================
*/
void idSysThread::SignalWork()
{
	if( isWorker )
	{
		signalMutex.Lock();
		moreWorkToDo = true;
		signalWorkerDone.Clear();
		signalMoreWorkToDo.Raise();
		signalMutex.Unlock();
	}
}

/*
========================
idSysThread::IsWorkDone
========================
*/
bool idSysThread::IsWorkDone()
{
	if( isWorker )
	{
		// a timeout of 0 will return immediately with true if signaled
		if( signalWorkerDone.Wait( 0 ) )
		{
			return true;
		}
	}
	return false;
}

/*
========================
idSysThread::ThreadProc
========================
*/
int idSysThread::ThreadProc( idSysThread* thread )
{
	int retVal = 0;
	
	try
	{
		if( thread->isWorker )
		{
			for( ; ; )
			{
				thread->signalMutex.Lock();
				if( thread->moreWorkToDo )
				{
					thread->moreWorkToDo = false;
					thread->signalMoreWorkToDo.Clear();
					thread->signalMutex.Unlock();
				}
				else
				{
					thread->signalWorkerDone.Raise();
					thread->signalMutex.Unlock();
					thread->signalMoreWorkToDo.Wait( idSysSignal::WAIT_INFINITE );
					continue;
				}
				
				if( thread->isTerminating )
				{
					break;
				}
				
				retVal = thread->Run();
			}
			thread->signalWorkerDone.Raise();
		}
		else
		{
			retVal = thread->Run();
		}
	}
	catch( idException& ex )
	{
		idLib::Warning( "Fatal error in thread %s: %s", thread->GetName(), ex.GetError() );
		
		// We don't handle threads terminating unexpectedly very well, so just terminate the whole process
		exit( 0 );
	}
	
	thread->isRunning = false;
	
	return retVal;
}

/*
========================
idSysThread::Run
========================
*/
int idSysThread::Run()
{
	// The Run() is not pure virtual because on destruction of a derived class
	// the virtual function pointer will be set to nullptr before the idSysThread
	// destructor actually stops the thread.
	return 0;
}

/*
================================================================================================

	test

================================================================================================
*/

/*
================================================
idMyThread test class.
================================================
*/
class idMyThread : public idSysThread
{
public:
	virtual int Run()
	{
		// run threaded code here
		return 0;
	}
	// specify thread data here
};

/*
========================
TestThread
========================
*/
void TestThread()
{
	idMyThread thread;
	thread.StartThread( "myThread", CORE_ANY );
}

/*
========================
TestWorkers
========================
*/
void TestWorkers()
{
	idSysWorkerThreadGroup<idMyThread> workers( "myWorkers", 4 );
	for( ; ; )
	{
		for( int i = 0; i < workers.GetNumThreads(); i++ )
		{
			// workers.GetThread( i )-> // setup work for this thread
		}
		workers.SignalWorkAndWait();
	}
}
