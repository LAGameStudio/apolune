#ifndef _H_FILE_
#define _H_FILE_

// Common includes
#include "Common.h"
#include "uString.h"

#define MAX_FILES 0xffffffff
#define AGK_FILE_READ_BUFFER 256

// Namespace
namespace AGK
{
	class cFile
	{
		protected:
			FILE *pFile;
			void *pFilePtr;
			int mode; //0=read, 1=write
			char readbuf[AGK_FILE_READ_BUFFER];

		public:
			cFile();
			~cFile();

			static void DeleteFile( const char *szFilename );
			static bool Exists( const char *szFilename );
			static bool ExistsWrite( const char *szFilename );
			static bool ExistsRead( const char *szFilename );
			static bool GetModified( const char *szFilename, int &time );
			static void SetModified( const char *szFilename, int time );
			static UINT GetFileSize( const char *szFilename );

			bool OpenToWrite( const char *szFilename, bool append=false );
			bool OpenToRead( const char *szFilename );

			bool IsOpen();
			void Close();
			void Rewind();
			bool IsEOF();
			UINT GetSize();
			UINT GetPos();
			void Seek( UINT pos );

			void WriteByte( unsigned char b );
			void WriteInteger( int i );
			void WriteFloat( float f );
			void WriteString( const char *str );
			void WriteData( const char *str, UINT bytes );
			void WriteLine( const char *str );

			unsigned char ReadByte( );
			int ReadInteger( );
			float ReadFloat( );
			int ReadString( uString &str );
			int ReadData( char *str, UINT length );
			int ReadLine( uString &str );
			int ReadLineFast( uString &str );
	};
}

#endif