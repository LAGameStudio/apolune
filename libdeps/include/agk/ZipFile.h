#ifndef _H_ZIP_FILE_
#define _H_ZIP_FILE_

#include "Common.h"

namespace AGK
{
	class _EXPORT_ ZipFile
	{
		protected:
			void* m_zf;

		public:
			ZipFile();
			~ZipFile();

			void Create( const char* filename );
			bool AddEntry( const char* realPath, const char* zipPath );
			bool AddEntry( const char* realPath, const char* zipPath, int compressionlevel );
			void Close();

			static bool ExtractAll( const char* filename, const char* extractPath );

	};
}

#endif