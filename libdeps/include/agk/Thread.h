#ifndef AGKTHREAD_H
#define AGKTHREAD_H

#include "Common.h"

namespace AGK
{
	class AGKThread
	{
		friend class BadaThread;
		//friend class iOSThread;

		private:

			void* pThread;
			UINT iThreadID;
			volatile bool m_bRunning;

		public:
			inline static UINT STDCALL EntryPoint( void *pParams )
			{
				if ( !pParams ) return 0;
				AGKThread* pThis = (AGKThread*) pParams;
				UINT result = 0;
				if ( !pThis->m_bTerminate ) 
				{
					result = pThis->Run();
					pThis->Finally();
				}
				return result;
			}

			// when a thread stops for whatever reason this function shuld be called
			void Finally()
			{
				m_bRunning = false;
			}

		protected:

			// variables to tell the thread to stop, it must watch these and stop appropriately
			volatile bool m_bTerminate;
			void* m_pStop; 

			// this must be overridden by the inheriting class, contains all code that the thread will run.
			virtual UINT Run( ) = 0;

		private:
			void PlatformInit();
			void PlatformStart();
			void PlatformStop();
			void PlatformSleepSafe( UINT milliseconds );
			void PlatformJoin();
			void PlatformTerminate();
			void PlatformCleanUp();
			
		public:

			AGKThread( )
			{
				pThread = 0;
				iThreadID = 0;
				m_bRunning = false;
				m_bTerminate = false;
				m_pStop = 0;
				PlatformInit(); 
			}

			virtual ~AGKThread( )
			{
				// tell it to stop
				Stop();

				// give some time?
				Join();
				//Sleep( 100 );

				// force it to stop
				PlatformTerminate();
				PlatformCleanUp();
			}

			bool IsRunning() { return m_bRunning; }

			// starts the thread and calls Run() (if not already running)
			virtual void Start( )
			{
				if ( m_bRunning ) return;
				m_bTerminate = false;
				m_bRunning = true;
				PlatformStart();
			}

			// tells the thread to stop and returns immediately, the thread is not guaranteed to stop
			virtual void Stop()
			{
				m_bTerminate = true;
				PlatformStop();
			}

			// sleep for a specified time but will wake early if the thread is told to stop, check m_bTerminate after this command
			void SleepSafe( UINT milliseconds )
			{
				PlatformSleepSafe( milliseconds );
			}

			// waits for the thread to stop then returns, can wait forever. If thread is already stopped or 
			// not yet started returns immediately
			void Join( )
			{
				if ( !IsRunning() ) return;
				PlatformJoin();
			}

			/*
			// forces the thread to stop immediately, if it held any locks they will not be released and
			// may cause a crash
			void Terminate()
			{
				PlatformTerminate();
				Finally();
			}
			*/
	};
}

#endif
