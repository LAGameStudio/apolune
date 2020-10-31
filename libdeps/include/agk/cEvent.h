// Event Header
#ifndef _H_EVENT_
#define _H_EVENT_

// Common includes
#include "Common.h"

#define EVENT_LIST_SIZE 100

// Namespace
namespace AGK
{
	class cEvent
	{
		public:
			int m_iID;
			bool m_bRead;
			int m_iData1;
			int m_iData2;
			int m_iData3;
			int m_iData4;
			cEvent *m_pNextEvent;
			
			static cEvent* g_pIter;
			static cEvent* g_pEvents[ EVENT_LIST_SIZE ];
			
			cEvent() { Reset(); }
			~cEvent() {}
			
			void Reset();
			void SetData1( int data );
			void SetData2( int data );
			void SetData3( int data );
			void SetData4( int data );
			int GetData1();
			int GetData2();
			int GetData3();
			int GetData4();

			static int Count( );
			static cEvent* GetFirstEvent( );
			static cEvent* GetNextEvent( );

			static void UpdateEvents();
			static cEvent* NewEvent( int iNewID );
			static cEvent* GetEvent( int iIndex );
	};
}

#endif