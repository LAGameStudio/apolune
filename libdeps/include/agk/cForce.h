// Sprite Header
#ifndef _H_FORCE_
#define _H_FORCE_

// Common includes
#include "Common.h"

#define MAX_FORCES 0x7fffffff

// Namespace
namespace AGK
{
	class _EXPORT_ cForce
	{
		public:
			
			float		m_fX;
			float		m_fY;
			float		m_fRadius;
			float		m_fPower;
			float		m_fLimit;
			bool		m_bFade;

			cForce();
			cForce( float x, float y, float power, float limit=0, float radius=-1, bool fade=false );
			~cForce();
	};
}

#endif