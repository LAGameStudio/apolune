#include "VBO_V.h"

#include "PLY.h"
#include "Model.h"

void VBOStaticV::fromModel( ImmediateModel *m ) { EACH(m->polygons.first,mPolygon,p) PushTriangle(p); }
void VBOStaticV::fromModel( ImmediateModel *m, int group ) { EACH(m->polygons.first,mPolygon,p) if ( p->group == group ) PushTriangle(p); } 
void VBOStaticV::fromPLY( PLY *p ) { fromModel(&p->model); Create(); }
void VBOStaticV::fromPLY( PLY *p, bool create ) { fromModel(&p->model); if ( create ) Create(); }
