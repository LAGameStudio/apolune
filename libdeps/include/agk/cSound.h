// Sound Header
#ifndef _H_SOUND_
#define _H_SOUND_

// Common includes
#include "Common.h"
#include "uString.h"

#define MAX_SOUND_FILES 300
#define MAX_MUSIC_FILES 50

// Namespace
namespace AGK
{
	typedef struct tWaveFormat
	{
		unsigned short  wFormatTag;         /* format type */
		unsigned short  nChannels;          /* number of channels (i.e. mono, stereo...) */
		unsigned int    nSamplesPerSec;     /* sample rate */
		unsigned int    nAvgBytesPerSec;    /* for buffer estimation */
		unsigned short  nBlockAlign;        /* block size of data */
		unsigned short  wBitsPerSample;     /* number of bits per sample of mono data */
		unsigned short  cbSize;             /* the count in bytes of the size of extra data */
	} AGKWAVEFORMATEX;

	class cMusicMgr
	{
		protected:
			class cMusic
			{
				public:
					UINT m_iID;
					uString m_sFile;
					cMusic *m_pNextMusic;
					int m_iVolume;
			};

			cMusic* m_pMusicFiles[ MAX_MUSIC_FILES ];
			static cMusic *m_pCurrentlyPlaying;
			UINT m_iStartID;
			UINT m_iEndID;
			bool m_bLoop;
			int m_iMasterVolume;
			UINT m_iLastID;

			//cMusic* GetMusic( int iID );

			static void PlatformAddFile( cMusic *pMusic );
			bool PlatformPrepare( UINT iID );

		public:
			cMusicMgr();
			~cMusicMgr();

			void AddFile( UINT iID, const uString &szFile );
			UINT AddFile( const uString &szFile );
			void SetFileVolume( UINT iID, int volume ); 
			void Play( UINT iID=1, bool bLoop=true, UINT iStartID=1, UINT iEndID=MAX_MUSIC_FILES-1 );
			void Pause();
			void Resume();
			void Stop();
			void RemoveFile( UINT iID );
			void ClearAll();
			void HandleEvent();
			UINT CurrentlyPlaying();
			void SetMasterVolume( int vol );
			int GetMasterVolume() { return m_iMasterVolume; }
			UINT Exists( UINT iID );

			float GetDuration( UINT ID );
			void Seek( float seconds, int mode );
			float GetPosition();
	};

	// to be define by the platform, requires device specific pointers for playing
	class cSoundInst;

	class cSoundMgr
	{
		public:

			class cSoundFile
			{
				public:
					UINT m_iID;
					uString m_sFile;
					unsigned char *m_pRawData;
					UINT m_uDataSize;
					AGKWAVEFORMATEX m_fmt;
					int m_iMax;
					UINT m_iInstances;

					cSoundFile();
					~cSoundFile();
			};

		protected:

			// between 0 and 100
			static int m_iGlobalVolume;

			static cSoundFile* m_pSoundFiles[ MAX_SOUND_FILES ];
			// cSoundInst is platform specific
			static cSoundInst *m_pSounds;
			// keep a list of used instances that have stopped so we don't have to reallocate the memory later
			static cSoundInst *m_pUsedSounds;

			static UINT m_iLastID;
			static float m_fMinPlaybackRate;
			static float m_fMaxPlaybackRate;
			static float m_fStepPlaybackRate;

			static void PlatformInit();
			static void PlatformUpdateVolume( );
			static void PlatformUpdate();
			static void PlatformDelete();
			static void PlatformAddFile( cSoundFile *pSound );

			static UINT PlatformCreateInstance( cSoundFile *pSound, int iVol=100, int iLoop=0, int iPriority=0 );
			static void PlatformStopInstances( UINT iID=0 );
			//static void PlatformPauseInstances( UINT iID=0 );
			//static void PlatformResumeInstances( UINT iID=0 );

			cSoundMgr() {}

		public:

			static cSoundInst* GetCurrentSounds() { return m_pSounds; }

			static void Init();
			static void SetVolume( int vol );
			static void UpdateSounds();
			static int GetVolume() { return m_iGlobalVolume; }
			static float GetMinRate() { return m_fMinPlaybackRate; }
			static float GetMaxRate() { return m_fMaxPlaybackRate; }

			static void AddFile( UINT iID, const uString &szFile, int iMaxInstances=-1 );
			static UINT AddFile( const uString &szFile, int iMaxInstances=-1 );
			static void AddFile( UINT iID, const unsigned char *pData, UINT size, int iMaxInstances=-1 );
			static UINT AddFile( const unsigned char *pData, UINT size, int iMaxInstances=-1 );

			static UINT PlayInstance( UINT iID, int iVol=100, int iLoop=0, int iPriority=0 );
			//static void PauseAll( UINT iID=0 );
			//static void ResumeAll( UINT iID=0 );
			static cSoundFile* GetFile( UINT iID );
			static void SaveFile( UINT iID, const uString &szFile );

			static void SetInstanceVolume( UINT instance, int vol );
			static void SetInstanceRate( UINT instance, float rate );
			static void SetInstanceBalance( UINT instance, float balance );
			static int GetInstanceVolume( UINT instance );
			static float GetInstanceRate( UINT instance );
			static int GetInstancePlaying( UINT instance );
			static int GetInstanceLoopCount( UINT instance );
			static void StopInstance( UINT instance );

			static void StopAll( UINT iID=0 );
			static void DeleteFile( UINT iID=0 );
			static UINT CountInstances( UINT iID );
			static UINT Exists( UINT iID );
	};
}

#endif
