#include "VBO_VNTTan.h"

#include "PLY.h"
#include "Model.h"

void VBOStaticVNTTan::fromModel( ImmediateModel *m ) { EACH(m->polygons.first,mPolygon,p) PushTriangle(p); } 
void VBOStaticVNTTan::fromModel( ImmediateModel *m, int group ) { EACH(m->polygons.first,mPolygon,p) if ( p->group == group ) PushTriangle(p); } 
void VBOStaticVNTTan::fromPLY( PLY *p ) { fromModel(&p->model); Create(); }
void VBOStaticVNTTan::fromPLY( PLY *p, bool create ) { fromModel(&p->model); if ( create ) Create(); }
