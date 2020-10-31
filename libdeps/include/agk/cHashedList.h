// Text Header
#ifndef _H_HLIST_
#define _H_HLIST_

// Common includes
#include "Common.h"

// Namespace
namespace AGK
{
	// An array type structure to hold a list of pointers for constant time access, but using linked lists 
	// when the number of items exceeds iSize for unlimited IDs without pre-assigning a large array.
	// Creates an array of size iSize (default 1024), with each element being a linked list of items.
	// For example ID 1024 and ID 2048 would be stored in the same array index, but linked one after the other.
	template<class T> class cHashedList
	{
		protected:

			class cHashedItem
			{
				public:
					UINT m_iID;
					char *m_szID;
					T *m_pItem;
					cHashedItem *m_pNextItem;

					cHashedItem() { m_iID = 0; m_szID = 0; m_pItem = UNDEF; m_pNextItem = UNDEF; }
					~cHashedItem() { if ( m_szID ) delete [] m_szID; }
			};

			UINT m_iListSize;
			cHashedItem **m_pHashedItems;
			cHashedItem *m_pIter;
			cHashedItem *m_pNextIter; //only needed if we delete the iterator
			UINT m_iLastID;
			bool bDeletePointers;
			UINT m_iItemCount;

			UINT HashIndex( UINT iIndex );
			UINT HashIndex( const char *szIndex );

		public:
			bool m_bIsClearing;

			cHashedList( UINT iSize=1024 );
			~cHashedList();
			void Resize( UINT iSize );

			void AddItem( T* pItem, UINT iID );
			T* GetItem( UINT iID );
			T* GetFirst();
			T* GetNext();
			T* RemoveItem( UINT iID );
			UINT GetFreeID( UINT max=0x7fffffff );
			void ClearAll( );
			UINT GetCount() { return m_iItemCount; }

			void AddItem( T* pItem, const char* szID );
			T* GetItem( const char* szID );
			//T* GetFirstS();
			//T* GetNextS();
			T* RemoveItem( const char* szID );
	};
}

using namespace AGK;

template<class T> cHashedList<T>::cHashedList( UINT iSize )
{
	if ( iSize < 2 ) iSize = 2;
	if ( iSize > 0x7fffffff ) iSize = 0x7fffffff;

	// find the next highest power of 2 
	UINT iNewSize = 1;
	while ( iNewSize < iSize ) iNewSize = iNewSize << 1;
	m_iListSize = iNewSize;

	m_pHashedItems = new cHashedItem*[ m_iListSize ];
	for( UINT i = 0; i < m_iListSize; i++ )
	{
		m_pHashedItems[ i ] = UNDEF;
	}

	m_pIter = UNDEF;
	m_pNextIter = UNDEF;

	m_iLastID = 10000;
	bDeletePointers = false;
	m_iItemCount = 0;
	m_bIsClearing = false;
}

template<class T> void cHashedList<T>::Resize( UINT iSize )
{
	if ( m_pHashedItems ) 
	{
		ClearAll();
		delete [] m_pHashedItems;
	}

	if ( iSize < 2 ) iSize = 2;
	if ( iSize > 0x7fffffff ) iSize = 0x7fffffff;

	// find the next highest power of 2 
	UINT iNewSize = 1;
	while ( iNewSize < iSize ) iNewSize = iNewSize << 1;
	m_iListSize = iNewSize;

	m_pHashedItems = new cHashedItem*[ m_iListSize ];
	for( UINT i = 0; i < m_iListSize; i++ )
	{
		m_pHashedItems[ i ] = UNDEF;
	}

	m_pIter = UNDEF;
	m_pNextIter = UNDEF;

	m_iLastID = 10000;
	bDeletePointers = false;
	m_iItemCount = 0;
	m_bIsClearing = false;
}

template<class T>cHashedList<T>::~cHashedList()
{
	ClearAll( );
	delete [] m_pHashedItems;
}

template<class T>UINT cHashedList<T>::HashIndex( UINT iIndex )
{
	//return iIndex % m_iListSize;
	// if m_iListSize is a power of 2 we can do it in bitwise operators
	return iIndex & (m_iListSize-1);
}

template<class T>UINT cHashedList<T>::HashIndex( const char *szIndex )
{
	UINT iIndex = 0;
	UINT length = (UINT)strlen( szIndex );
	for ( UINT i = 0; i < length; i++ )
	{
		iIndex += szIndex[i]*23*i;
	}
	return iIndex & (m_iListSize-1);
}

template<class T> void cHashedList<T>::AddItem( T* pItem, UINT iID )
{
	if ( GetItem( iID ) ) return;
	UINT index = HashIndex( iID );

	cHashedItem *pNewItem = new cHashedItem();
	pNewItem->m_iID = iID;
	pNewItem->m_pItem = pItem;

	pNewItem->m_pNextItem = m_pHashedItems[ index ];
	m_pHashedItems[ index ] = pNewItem;

	if ( iID > m_iLastID ) m_iLastID = iID;
	if ( m_iLastID > 0x7fffffff ) m_iLastID = 10000;

	m_iItemCount++;
}

template<class T> T* cHashedList<T>::GetItem( UINT iID )
{
	UINT index = HashIndex( iID );
	cHashedItem *pItem = m_pHashedItems[ index ];
	while( pItem )
	{
		if ( pItem->m_iID == iID ) return pItem->m_pItem;
		pItem = pItem->m_pNextItem;
	}

	return UNDEF;
}

template<class T> T* cHashedList<T>::GetFirst()
{
	m_pIter = UNDEF;
	m_pNextIter = UNDEF;
	for ( UINT i = 0; i < m_iListSize; i++ )
	{
		if ( m_pHashedItems[ i ] )
		{
			m_pIter = m_pHashedItems[ i ];
			return m_pIter->m_pItem;
		}
	}

	return UNDEF;
}

template<class T> T* cHashedList<T>::GetNext()
{
	if ( !m_pIter )
	{
		// iterator might have been deleted by RemoveItem(), check for a precalculated next step
		if ( m_pNextIter ) 
		{
			m_pIter = m_pNextIter;
			m_pNextIter = UNDEF;
			return m_pIter->m_pItem;
		}
		else return UNDEF;
	}

	if ( m_pIter->m_pNextItem )
	{
		m_pIter = m_pIter->m_pNextItem;
		return m_pIter->m_pItem;
	}
	
	UINT index = 0;
	if ( m_pIter->m_szID ) index = HashIndex( m_pIter->m_szID );
	else index = HashIndex( m_pIter->m_iID );

	for ( UINT i = index+1; i < m_iListSize; i++ )
	{
		if ( m_pHashedItems[ i ] )
		{
			m_pIter = m_pHashedItems[ i ];
			return m_pIter->m_pItem;
		}
	}

	m_pIter = UNDEF;
	return UNDEF;
}

template<class T> T* cHashedList<T>::RemoveItem( UINT iID )
{
	if ( m_bIsClearing ) return 0;

	UINT index = HashIndex( iID );
	cHashedItem *pItem = m_pHashedItems[ index ];
	cHashedItem *pLast = UNDEF;
	T *pItemContent = UNDEF;
	
	while( pItem )
	{
		// check if this item matches target
		if ( pItem->m_iID == iID )
		{
			// if this item is the current iterator, calculate the next step in advance
			if ( pItem == m_pIter )
			{
				m_pNextIter = UNDEF;

				if ( m_pIter->m_pNextItem )
				{
					m_pNextIter = m_pIter->m_pNextItem;
				}
				else
				{
					UINT index = HashIndex( m_pIter->m_iID );
					for ( UINT i = index+1; i < m_iListSize; i++ )
					{
						if ( m_pHashedItems[ i ] )
						{
							m_pNextIter = m_pHashedItems[ i ];
							break;
						}
					}
				}

				m_pIter = UNDEF;
			}

			// remove the item from the list
			if ( pLast ) pLast->m_pNextItem = pItem->m_pNextItem;
			else m_pHashedItems[ index ] = pItem->m_pNextItem;

			pItemContent = pItem->m_pItem;
			if ( m_iItemCount > 0 ) m_iItemCount--;

			// delete item, but not the contents as it is void, pass it back to be deleted.
			delete pItem;
			return pItemContent;
		}

		// next item
		pLast = pItem;
		pItem = pItem->m_pNextItem;
	}

	return UNDEF;
}

template<class T> UINT cHashedList<T>::GetFreeID( UINT max )
{
	UINT iID = m_iLastID;
	UINT start = iID;

	iID++;
	if ( iID > max ) 
	{
		iID = 1;
		start = max;
	}

	while ( GetItem( iID ) && iID != start )
	{
		iID++;
		if ( iID > max ) iID = 1;
	}

	if ( GetItem( iID ) ) iID = 0;
	return iID;
}

template<class T> void cHashedList<T>::ClearAll( )
{
	for ( UINT i = 0; i < m_iListSize; i++ )
	{
		while( m_pHashedItems[ i ] )
		{
			cHashedItem *pItem = m_pHashedItems[ i ];
			m_pHashedItems[ i ] = m_pHashedItems[ i ]->m_pNextItem;

			if ( bDeletePointers )
			{
				// can't guarantee object has public destructor
				//delete pItem->m_pItem;
			}

			delete pItem;
		}
	}

	m_bIsClearing = false;
	m_iLastID = 10000;
	m_iItemCount = 0;
	m_pIter = UNDEF;
}

// String versions

template<class T> void cHashedList<T>::AddItem( T* pItem, const char *szID )
{
	if ( !szID ) return;
	if ( GetItem( szID ) ) return;
	UINT index = HashIndex( szID );

	cHashedItem *pNewItem = new cHashedItem();
	UINT length = (UINT)strlen( szID );
	pNewItem->m_szID = new char[ length+1 ];
	strcpy( pNewItem->m_szID, szID );
	pNewItem->m_pItem = pItem;

	pNewItem->m_pNextItem = m_pHashedItems[ index ];
	m_pHashedItems[ index ] = pNewItem;

	m_iItemCount++;
}

template<class T> T* cHashedList<T>::GetItem( const char *szID )
{
	if ( !szID ) return UNDEF;
	UINT index = HashIndex( szID );
	cHashedItem *pItem = m_pHashedItems[ index ];
	while( pItem )
	{
		if ( pItem->m_szID && strcmp( szID, pItem->m_szID ) == 0 ) return pItem->m_pItem;
		pItem = pItem->m_pNextItem;
	}

	return UNDEF;
}

template<class T> T* cHashedList<T>::RemoveItem( const char *szID )
{
	UINT index = HashIndex( szID );
	cHashedItem *pItem = m_pHashedItems[ index ];
	cHashedItem *pLast = UNDEF;
	T *pItemContent = UNDEF;
	
	while( pItem )
	{
		// check if this item matches target
		if ( pItem->m_szID && strcmp( szID, pItem->m_szID ) == 0 )
		{
			// if this item is the current iterator, calculate the next step in advance
			if ( pItem == m_pIter )
			{
				m_pNextIter = UNDEF;

				if ( m_pIter->m_pNextItem )
				{
					m_pNextIter = m_pIter->m_pNextItem;
				}
				else
				{
					UINT index = 0;
					if ( m_pIter->m_szID ) index = HashIndex( m_pIter->m_szID );
					else index = HashIndex( m_pIter->m_iID );

					for ( UINT i = index+1; i < m_iListSize; i++ )
					{
						if ( m_pHashedItems[ i ] )
						{
							m_pNextIter = m_pHashedItems[ i ];
							break;
						}
					}
				}

				m_pIter = UNDEF;
			}
			
			// remove the item from the list
			if ( pLast ) pLast->m_pNextItem = pItem->m_pNextItem;
			else m_pHashedItems[ index ] = pItem->m_pNextItem;

			pItemContent = pItem->m_pItem;
			m_iItemCount--;

			// delete item, but not the contents as it is undefined, pass it back to be deleted.
			delete pItem;
			return pItemContent;
		}

		// next item
		pLast = pItem;
		pItem = pItem->m_pNextItem;
	}

	return UNDEF;
}

#endif
