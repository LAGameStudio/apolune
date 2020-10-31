// Sprite Header
#ifndef _H_NETWORK_
#define _H_NETWORK_


#define AGK_NET_SEND_BUFFER_SIZE 1400
#define AGK_NET_PACKET_SIZE 1400

// Common includes
#include "Common.h"
#include "cHashedList.h"
#include "uString.h"
#include "Thread.h"
#include "NetworkCommon.h"

// Namespace
namespace AGK
{
        /*
        class cLock;
        //class cNetCon;
	class AGKSocket;
	class cNetworkListener;
	class Broadcaster;
	class BroadcastListener;
	class cFile;
	*/

	class cAutoLock
	{
		protected:
			cLock *m_pLock;

		public:
			cAutoLock( cLock *lock )
			{
				if ( !lock ) return;
				m_pLock = lock;
				m_pLock->Acquire();
			}

			~cAutoLock()
			{
				m_pLock->Release();
			}
	};

	class cAutoSLock
	{
		protected:
			cSpinLock *m_pLock;

		public:
			cAutoSLock( cSpinLock *lock )
			{
				if ( !lock ) return;
				m_pLock = lock;
				m_pLock->Acquire();
			}

			~cAutoSLock()
			{
				m_pLock->Release();
			}
	};

	class cFileSender : public AGKThread
	{
		protected:
			AGKSocket *m_pConnection;
			char m_szIP[ 40 ];
			UINT m_iPort;
			UINT m_iTimeout;
			//bool m_bClose;
			cLock *m_kNameLock;
			bool m_bCloseAfter;

			uString* m_pFiles;
			UINT m_iSize;

			volatile UINT m_iCurrentFileSize;
			volatile UINT m_iTotalSending;
			volatile UINT m_iTotalSent;
			UINT m_iAdded;
			volatile UINT m_iCurrentFile;
			
			UINT Run();

		public:
			cFileSender *m_pNext;

			cFileSender( UINT numFiles );
			~cFileSender();

			bool ConnectTo( const char *szIP, UINT port, UINT timeout=3000 );
			void SetReceiver( AGKSocket *pReceiver, bool closeafter=true );
			bool AddFile( const char *szFilename );
			float GetProgress();
			int GetCurrentFile( uString &s );
			void Reset( UINT numFiles );
			bool IsFinished() { return !IsRunning(); }
			void Stop();
	};

	class cFileBroadcaster : public AGKThread
	{
		protected:
			UINT Run();

			UINT m_iBrodcastPort;
			UINT m_iReturnPort;
			Broadcaster m_cBroadcaster;
			cNetworkListener m_cListener;

			cFileSender *m_pSenders;
			cLock *m_kSenderLock;

			uString* m_pFiles;
			UINT m_iNumFiles;
			UINT m_iAdded;

			volatile UINT m_iCompleted;

		public:
			cFileBroadcaster( UINT numFiles, const char* szGameName, UINT returnPort=45633, UINT brodcastPort=45632 );
			~cFileBroadcaster();

			// fills arrays with the latest data, each array must be at least max in size.
			int GetProgress( UINT max, uString *pDevices, uString *pCurrentFile, float *pProgress );
			UINT GetNumClients();
			UINT GetNumCompleted() { return m_iCompleted; }

			bool AddFile( const char *szFilename );

			void Start();
			void Stop();
			void StopBroadcasting();
			void ResumeBroadcasting();
	};

	class cFileReceiver : public AGKThread
	{
		protected:
			AGKSocket* m_pConnection;
			char m_szIP[ 40 ];
			UINT m_iPort;
			UINT m_iTimeout;
			//bool m_bClose;
			volatile bool m_bSuccess;
			bool m_bCloseAfter;

			volatile UINT m_iTotalReceiving;
			volatile UINT m_iTotalReceived;

			char m_buffer[ 1500 ];

			UINT Run();

		public:
			cFileReceiver( );
			~cFileReceiver();

			bool ConnectTo( const char *szIP, UINT port, UINT timeout=3000 );
			void SetSender( AGKSocket *pSender, bool closeafter=true );
			void Reset( );
			bool IsFinished() { return !IsRunning(); }
			bool IsSuccess() { return m_bSuccess; }	
			float GetProgress();
			void Stop();
	};

	class cNetworkVariable
	{
		public:
			uString m_sName;
			bool m_bChanged;
			bool m_bNew;
			UINT m_iMode; // 0=normal, 1=reset, 2=unique
			UINT m_iType;
			bool m_bReset;
			union 
			{
				float f;
				int i;
			} m_value;

			cNetworkVariable() { m_bChanged = true; m_bNew = true; m_iMode = 0; m_value.i = 0; m_iType = 0; m_bReset = false; }
			~cNetworkVariable() { }
	};

	class cNetworkClient
	{
		protected:
			cHashedList<cNetworkVariable> m_pHashedVars;
			cNetworkVariable **m_pVars;
			UINT m_iNumVars;
			UINT m_iArraySize;
			cLock *m_kVarLock;
			
		public:
			uString m_sName;
			UINT m_iID;
			volatile bool m_bKick;
			int m_iUserValue[ 5 ];
			float m_fPingStart;
			char m_iPingResponded;
			float m_fPing;
			bool m_bDisconnected;
			bool m_bDelete;

			cNetworkClient();
			~cNetworkClient();

			cLock* GetVarLock() { return m_kVarLock; }
			UINT GetNumVars() { return m_iNumVars; }
			cNetworkVariable** GetVars() { return m_pVars; }

			int GetVariableI( const char *name, bool bDontReset=false );
			float GetVariableF( const char *name, bool bDontReset=false );
			UINT GetVariableType( const char *name );
			UINT GetVariableExists( const char *name );

			//int GetVariableI( UINT index );
			//float GetVariableF( UINT index );
			UINT GetVariableType( UINT index );

			void SetVariableI( const char *name, int i, int mode=0 );
			void SetVariableF( const char *name, float f, int mode=0 );

			void SetVariableI( UINT index, int i, int mode=0 );
			void SetVariableF( UINT index, float f, int mode=0 );
	};

	class cNetworkMessage : public AGKPacket
	{
		protected:
			friend class cNetwork;

			UINT m_iToClientID;
			UINT m_iFromClientID;
			cNetworkMessage *m_pNext;

		public:
			uString sFromIP;
			
			cNetworkMessage() { m_iToClientID = 0; m_iFromClientID = 0; m_pNext = 0; }
			~cNetworkMessage() { };

			UINT GetSenderID() { return m_iFromClientID; }
	};

	class cNetwork : public AGKThread
	{
		protected:
			uString m_sNetworkName;
			uString m_sMyName;
			char m_szServerIP[ 40 ];
			UINT m_iServerPort;

			Broadcaster *m_pBroadcaster;				//server only
			cNetworkListener *m_pListener;				//server only
			AGKSocket ** volatile m_ppClientSock;		//server only

			BroadcastListener *m_pBroadcastListener;	//client only
			AGKSocket *m_pServerSock;					//client only
			char m_iKeepAliveResponded;					//client only
			
			cNetworkClient** volatile m_ppClients; 
			volatile UINT m_iNumClients;
			UINT m_iArraySize;
			bool m_bServer;
			cLock *m_kClientLock;
			UINT m_iNextClientID;
			volatile UINT m_iMyClientID;
			UINT m_iClientIter;
			cHashedList<UINT> m_cClientRef;
			UINT m_iLatency;
			float m_fLastKeepAlive;

			cLock *m_kCleanUpLock;

			cLock *m_kMessageLock;
			cNetworkMessage* volatile m_pSendMessages;
			cNetworkMessage* volatile m_pSendTail;
			cNetworkMessage* volatile m_pRecvMessages;
			cNetworkMessage* volatile m_pRecvTail;

			UINT Run();

			void JoinCommon();

			void NewClient( AGKSocket *pNew );
			void CheckMessagesServer();
			void SendChangesServer();
			void SendDisconnects();
			void UpdateServer();

			void ConnectClient();
			void CheckMessagesClient();
			void SendChangesClient();
			void UpdateClient();

		public:
			cNetwork();
			~cNetwork();
			void Reset();
			void Stop();

			void HostNetwork( const char *szNetworkName, const char *szMyName, UINT port );
			void NoMoreConnections();

			void JoinNetwork( const char *szNetworkName, const char *szMyName );
			void JoinNetwork( const char *szIP, UINT port, const char *szMyName );

			bool IsActive() { return IsRunning(); }

			void CloseNetwork();
			void SetLatency( UINT latency );

			UINT GetMyID() { return m_iMyClientID; }
			UINT GetNumClients();
			UINT GetNumActiveClients();
			UINT GetFirstClient();
			UINT GetNextClient();

			UINT GetClientDisconnected( UINT client );
			void DeleteDisconnectedClient( UINT client );
			int GetClientName( UINT client, uString &sName );
			float GetClientPing( UINT client );
			UINT GetServerID( ) { return 1; } // for now server is always client ID 1
			
			void SetLocalVariableI( const char *name, int i, int mode=0 );
			void SetLocalVariableF( const char *name, float f, int mode=0 );
			
			int GetClientVariableI( UINT client, const char *name );
			float GetClientVariableF( UINT client, const char *name );
			
			void SendMessage( UINT toClient, cNetworkMessage* pMsg );
			cNetworkMessage* GetMessage( );
			
			void SetClientUserData( UINT client, UINT index, int value );
			int GetClientUserData( UINT client, UINT index );
			
			void Update();
	};
}

#endif
