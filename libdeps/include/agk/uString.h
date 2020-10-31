// String Header
#ifndef _H_USTRING_
#define _H_USTRING_

#ifndef AGK_COMPILER
	#include "Common.h"
#endif

namespace AGK
{
	class uString
	{
		protected:
			char* m_pData;
			unsigned int m_iLength; //length of the current string, cached for speed
			unsigned int m_iTotalLength; //length of the array holding the string, might have room for more data
			// a 64bit interpreter would depend on this struct being a multiple of 8 bytes in size for use in AGK types
		
		public:
			uString();
			uString( const uString &szString );
			uString( const char* szString, unsigned int initialLength=0 ); //can define a longer array than the data given
			~uString();

			void Reset();
			void Init();

			void CheckSize( unsigned int size, bool preserve );

			void SetStr( const char* szString );
			inline void SetStr( const uString &szString ) { SetStr( szString.GetStr() ); }
			void SetStrN( const char* szString, unsigned int length );
			inline const char* GetStr() const { if ( !m_pData || m_iLength == 0 ) return ""; else return m_pData; }
			inline unsigned int GetLength() const { return m_iLength; }
			void ClearTemp(); // call if you want to empty the string but will likely be reusing its capacity soon

			uString& Prepend( const char* szString );
			inline uString& Prepend( const uString &szString ) { return Prepend( szString.GetStr() ); }

			uString& Append( char c );
			uString& Append( int num );
			uString& Append( unsigned int num );
			uString& Append( float num );
			uString& Append( const char* szString );
			uString& AppendN( const char* szString, unsigned int length ); // adds a null terminator to the internal string when done

			uString& InsertCharAt( unsigned int index, char c );
			uString& DeleteCharAt( unsigned int index );

			uString& Format( const char* szString, ... ); //same format as sprintf

			uString& Strip( const char* tokens ); //removes all characters in "tokens" from the string
            uString& Strip( char token ); //removes all instances of "token" from the string
            uString& StripUTF8(); // remove all multi-byte UTF8 characters
			uString& Trim( const char* tokens ); //removes all characters in "tokens" from the ends of the string
			uString& Replace( char find, char replace ); //replaces all instances of "find" with "replace"
			uString& ReplaceStr( const char* find, const char* replace ); //replaces all instances of "find" with "replace"
			uString& Upper( ); //converts to upper case
			uString& Lower( ); //converts to lower case

			char CharAt( unsigned int index ) const;
			int Find( char cFind ) const; //finds the first instance of the character and returns its index
			int RevFind( char cFind ) const; //finds the last instance of the character and returns its index
			void Trunc( char cFind ); //removes all characters after the last occurance of cFind, including that character
			void Trunc2( int count ); //removes the last count characters from the string
			int FindStr( const char* str ) const;
			int Count( char find ); // counts the occurances of the given character

			int ToInt() const;
			float ToFloat() const;

			// "delimit" is a list of characters that split words
			int CountTokens( const char* delimit );
			char* GetToken( const char* delimit, int index );
			int GetToken( const char* delimit, int index, uString &out );

			// "delimit" is a single character that splits sections
			int CountTokens2( char delimit );
			//int GetToken2( char delimit, int index, uString &out );
			int SplitTokens2( char delimit, uString* &out );
			
			inline int CompareTo( const uString &pTo ) const { return CompareTo( pTo.GetStr() ); }
			int CompareTo( const char* szTo ) const;
        
            inline int CompareToN( const uString &pTo, int max ) const { return CompareToN( pTo.GetStr(), max ); }
            int CompareToN( const char* szTo, int max ) const;
        
            inline int CompareCaseTo( const uString &pTo ) const { return CompareCaseTo( pTo.GetStr() ); }
            int CompareCaseTo( const char* szTo ) const;
        
            inline int CompareCaseToN( const uString &pTo, int max ) const { return CompareCaseToN( pTo.GetStr(), max ); }
            int CompareCaseToN( const char* szTo, int max ) const;

			inline uString& operator =( const char* str ) { this->SetStr(str); return *this; }
			inline uString& operator =( const uString& str ) { this->SetStr(str); return *this; }
			inline uString& operator +=( const char* str ) { this->Append(str); return *this; }
			inline uString& operator +=( unsigned int u ) { this->Append(u); return *this; }
			inline uString& operator +=( int i ) { this->Append(i); return *this; }
			inline uString& operator +=( float f ) { this->Append(f); return *this; }
			inline uString& operator +=( const uString& str ) { if ( &str!=this ) this->Append(str); return *this; }
			inline bool operator ==( const uString& str ) const { return this->CompareTo(str) == 0; }
			inline bool operator !=( const uString& str ) const { return this->CompareTo(str) != 0; }
			inline operator const char *() const { if ( !m_pData || m_iLength == 0 ) return ""; else return m_pData; }
		
			void SubString( uString &output, int start, int length=-1 ) const;
	};
}

#endif
