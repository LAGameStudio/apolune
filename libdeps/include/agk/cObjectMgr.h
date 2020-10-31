#ifndef _H_OBJECTMGR_
#define _H_OBJECTMGR_

// Common includes
#include "Common.h"
#include "cObject3D.h"
#include "AGKShader.h"
#include "cCamera.h"

// Namespace
namespace AGK
{
	class _EXPORT_ cObjectContainer
	{
		protected:
			UINT m_iType;
			void *m_pItem;
					
		public:
			cObjectContainer *m_pNext;

			cObjectContainer() { m_pNext = 0; m_iType = 0; m_pItem = 0; }
			~cObjectContainer() {}
		
			void SetObject( cObject3D *pObject )
			{
				m_iType = 1;
				m_pItem = pObject;
			}

			int GetType() { return m_iType; }

			void GetPosition( float &x, float &y, float &z )
			{
				switch( m_iType )
				{
					case 1: 
					{
						x = ((cObject3D*)m_pItem)->GetX(); 
						y = ((cObject3D*)m_pItem)->GetY(); 
						z = ((cObject3D*)m_pItem)->GetZ(); 
						break;
					}
					default: return;
				}
			}

			int GetDrawMode()
			{
				switch( m_iType )
				{
					case 1: return ((cObject3D*)m_pItem)->GetTransparency(); break;
					default: return 0;
				}
			}

			AGKShader* GetShader()
			{
				switch( m_iType )
				{
					case 1: return ((cObject3D*)m_pItem)->GetShader();
					default: return 0;
				}
			}

			bool GetTransparencyChanged()
			{
				switch( m_iType )
				{
					case 1: return ((cObject3D*)m_pItem)->GetTransparencyChanged(); break;
					default: return false;
				}
			}

			cObject3D* GetObject() { return (cObject3D*)m_pItem; }
	};

	class _EXPORT_ cObjectMgr
	{
		protected:

			int iCurrentCount;
			bool m_bSortDepth;
			
			cObjectContainer *m_pOpaqueObjects;
			cObjectContainer *m_pLastOpaque;
			cObjectContainer *m_pAlphaObjects;
			cObjectContainer **m_pAlphaObjectsArray;
			int m_iNumAlphaObjects;

			UINT m_iLastTotal;
			UINT m_iLastSorted;
			UINT m_iLastDrawn;
			UINT m_iLastDrawCalls;

			AGKShader *m_pCurrentShader;
			static cCamera *g_pCurrentCamera;

			bool AddContainer( cObjectContainer *pNewMember );
			void DrawList( cObjectContainer *pList, int mode );

		public:

			cObjectMgr();
			~cObjectMgr();

			static int ContainerCompare( const void* a, const void* b );

			void ClearAll();
		
			UINT GetTotalCount() { return m_iLastTotal; }
			UINT GetSortedCount() { return m_iLastSorted; }
			UINT GetDrawnCount() { return m_iLastDrawn; }
			UINT GetDrawCalls() { return m_iLastDrawCalls; }
		
			void SetSortDepth( bool sort );
			
			void AddObject( cObject3D* object );
			void RemoveObject( cObject3D* object );

			static void SetCurrentCamera( cCamera* pCamera );
			
			void ResortAll();
			virtual void UpdateAll( float time );
			virtual void DrawAll();
	};
}

#endif
