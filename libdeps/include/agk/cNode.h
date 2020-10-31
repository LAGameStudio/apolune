#ifndef _H_NODE_
#define _H_NODE_

//#include "Common.h"

namespace AGK
{
	class cNode
	{
		protected:

			cNode *pSibling;
			cNode *pChild;
			cNode *pParent;
			
		public:
			enum eNodeType
			{
				eNode,
				eSprite,
				eStaticShape,
				eSound
			};

			cNode();
			virtual ~cNode();

			virtual eNodeType GetType() { return eNode; }
	};
}

#endif