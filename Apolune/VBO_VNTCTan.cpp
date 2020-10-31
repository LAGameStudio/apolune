#include "VBO_VNTCTan.h"

#include "PLY.h"
#include "Model.h"

void VBOStaticVNTCTan::fromModel( ImmediateModel *m ) { EACH(m->polygons.first,mPolygon,p) PushTriangle(p); } 
void VBOStaticVNTCTan::fromModel( ImmediateModel *m, int group ) { EACH(m->polygons.first,mPolygon,p) if ( p->group == group ) PushTriangle(p); } 
void VBOStaticVNTCTan::fromPLY( PLY *p ) { fromModel(&p->model); Create(); }
void VBOStaticVNTCTan::fromPLY( PLY *p, bool create ) { fromModel(&p->model); if ( create ) Create(); }
