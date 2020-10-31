#ifndef _H_JSONELEMENT
#define _H_JSONELEMENT

namespace AGK
{
	class JSONElement
	{
		public:
			JSONElement *m_pTempNext;

			JSONElement() { m_pTempNext = 0; }
			virtual ~JSONElement() {};

			virtual int GetType() { return 0; } // 0=Undefined, 1=Object, 2=Array, 3=String, 4=Number, 5=Bool
			
			static JSONElement* LoadJSONFromFile( const char* filename );
			static JSONElement* LoadJSONFromData( const char* data );
			static int ParseString( const char* data );
			static int ParseNumber( const char* data );

	};

	class JSONKeyPair
	{
		public:
			uString m_sName;
			JSONElement *m_pElement;
			JSONKeyPair *m_pTempNext;

			JSONKeyPair() { m_pElement = 0; m_pTempNext = 0; }
			~JSONKeyPair() { if ( m_pElement ) delete m_pElement; }

			int ParsePair( const char* data );
			int FindPairEnd( const char* data );
	};

	class JSONObject : public JSONElement
	{
		public:
			UINT m_iNumPairs;
			JSONKeyPair **m_pPairs;

			JSONObject() { m_iNumPairs = 0; m_pPairs = 0; }
			~JSONObject() 
			{ 
				if ( m_pPairs ) 
				{
					for ( UINT i = 0; i < m_iNumPairs; i++ ) if ( m_pPairs[ i ] ) delete m_pPairs[ i ];
					
					delete [] m_pPairs; 
				}
			}

			int ParseObject( const char* data );

			int GetType() { return 1; } // 0=Undefined, 1=Object, 2=Array, 3=String, 4=Number, 5=Bool
			JSONElement* GetElement( const char* szKey );
	};

	class JSONArray : public JSONElement
	{
		public:
			UINT m_iNumElements;
			JSONElement **m_pElements;

			JSONArray() { m_iNumElements = 0; m_pElements = 0; }
			~JSONArray() 
			{ 
				if ( m_pElements ) 
				{
					for ( UINT i = 0; i < m_iNumElements; i++ ) if ( m_pElements[ i ] ) delete m_pElements[ i ];

					delete [] m_pElements; 
				}
			}

			int ParseArray( const char* data );

			int GetType() { return 2; } // 0=Undefined, 1=Object, 2=Array, 3=String, 4=Number, 5=Bool
			JSONElement* GetElement( int index );

			int FindArrayEnd( const char* data );
	};

	class JSONString : public JSONElement
	{
		public:
			uString m_sValue;

			JSONString() {}
			~JSONString() {}

			int GetType() { return 3; } // 0=Undefined, 1=Object, 2=Array, 3=String, 4=Number, 5=Bool
	};

	class JSONNumber : public JSONElement
	{
		public:
			float m_fValue;

			JSONNumber() { m_fValue = 0; }
			~JSONNumber() {}

			int GetType() { return 4; } // 0=Undefined, 1=Object, 2=Array, 3=String, 4=Number, 5=Bool
	};

	class JSONBool : public JSONElement
	{
		public:
			bool m_bValue;

			JSONBool() { m_bValue = false; }
			~JSONBool() {}

			int GetType() { return 5; } // 0=Undefined, 1=Object, 2=Array, 3=String, 4=Number, 5=Bool
	};

	class JSONNull : public JSONElement
	{
		public:

			JSONNull() {}
			~JSONNull() {}

			int GetType() { return 0; } // 0=Undefined, 1=Object, 2=Array, 3=String, 4=Number, 5=Bool
	};
}

#endif