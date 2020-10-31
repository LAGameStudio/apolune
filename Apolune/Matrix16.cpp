#include "Matrix16.h"

#include "Vertex.h"

void Euler::ofLine( void *s, void *e ) {
 static Vertex v;
 v.Set( (Vertex *) e);
 v.subtract( (Vertex *) s);
 yaw= atan2(v.x, v.y);
 pitch = atan2( v.z, sqrt(SQ(v.x)) + SQ(v.y) );
 roll=0.0f;
}