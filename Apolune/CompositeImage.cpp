#include "CompositeImage.h"


CompositeModes CompositeModeOptions( string k ) {
 if ( is_integer((char *)k.c_str()) ) return (CompositeModes)atoi((char *)k.c_str());
 if ( !str_cmp((char *)k.c_str(),"masked") ) return _CompositeMask;
 if ( !str_cmp((char *)k.c_str(),"overlay") ) return _CompositeOverlay;
 if ( !str_cmp((char *)k.c_str(),"overlaymasked") ) return _CompositeOverlayMask;
 OUTPUT("CompositeModeOptions: Invalid mode named `%s` for composite image was ignored.\n",(char *)k.c_str());
 return _CompositeDefault;
}