#include "Trigger.h"

bool operator == ( Line& a, Line& b ) { return ( b.x==a.x && b.y==a.y && b.x2 == a.x2 && b.y2 == a.y2 ); }

void Circle::Approximate( int precision=16 ) {
 lines.Recycle();
 if ( precision < 3 ) {
  lines->Add(this,3);
  precision=3;
 } else {
  lines->Add(this,precision);
  this->precision=precision;
 }
}

bool Circle::Collides( Circle *c, Line *out ) {
 if ( R == 0.0 ) {
  out->x=out->y=out->x2=out->y2;
  return within(x,y);
 } else if ( c->R == 0.0 ) {
  out->x=out->y=out->x2=out->y2;
  return within(c->x,c->y);
 }
 double a, dx, dy, d, h, rx, ry;
 double x2, y2;
 dx = c->x - x;
 dy = c->y - y;
 /* Determine the straight-line distance between the centers. */
 //d = sqrt((dy*dy) + (dx*dx));
 d = hypot(dx,dy); // Suggested by Keith Briggs
 if (d > (R + c->R)) return false;
 else if (d < fabs(R - c->R)) return false;
 /* 'point 2' is the point where the line through the circle
  * intersection points crosses the line between the circle
  * centers.  
  */
 double sqR=R*R;
 a = ((sqR) - (c->R*c->R) + (d*d)) / (2.0 * d) ;
 x2 = x + (dx * a/d);
 y2 = y + (dy * a/d);
 h = sqrt((sqR) - (a*a));
 rx = -dy * (h/d);
 ry = dx * (h/d);
 /* Determine the absolute intersection points. */
 out->x = x2 + rx;
 out->x2 = x2 - rx;
 out->y = y2 + ry;
 out->y2 = y2 - ry;
 return 1;
}

bool Circle::Collides( Box *b, CoplanarLinesResults *out, int precision=16 ) {
 out->Clear();
 if ( !lines.pointer || lines->count != precision ) Approximate(precision);
 if ( !b->lines.pointer ) b->CalculateDimensions();
 return b->lines->Collides(lines,out);
}

bool Circle::Collides( Vertices *v, bool connectLastToFirst, CoplanarLinesResults *out, int precision=16 ) {
 out->Clear();
 if ( !lines.pointer || lines->count != precision ) Approximate(precision);
 Lines polygon;
 polygon.Add(v,connectLastToFirst);
 return lines->Collides(&polygon,out);
}

bool Circle::Collides( Lines *in, CoplanarLinesResults *out, int precision=16 ) {
 out->Clear();
 if ( !lines.pointer || lines->count != precision ) Approximate(precision);
 return lines->Collides(in,out);
}

bool Circle::within( Circle *c ) {
 if ( ddistance(c->x,c->y,x,y) <= abs(c->R-R) ) return true;
 return false;
}

// Returns the magnitude of an overlap where - means circle distance from edges,
// positive indicates full or partial overlap
double Circle::overlap( Circle *c ) {
 double d=ddistance(c->x,c->y,x,y);
 double combinedR=c->R+R;
 if (d > combinedR ) return combinedR-d;
 return (d-abs(c->R-R))/d;
}

void Box::CalculateDimensions() {
 double lx=minX();
 double ly=minY();
 double lz=minZ();
 double lx2=maxX();
 double ly2=maxY();
 double lz2=maxZ();
 if ( lx < 0 && lx2 < 0 ) {
  w=abs(lx)-abs(lx2);
 } else if ( lx < 0 && lx2 >= 0 ) {
  w=abs(lx)+lx2;
 } else {
  w=lx2-lx;
 }
 if ( ly < 0 && ly2 < 0 ) {
  h=abs(ly)-abs(ly2);
 } else if ( ly < 0 && ly2 >= 0 ) {
  h=abs(ly)+ly2;
 } else {
  h=ly2-ly;
 }
 if ( lz < 0 && lz2 < 0 ) {
  d=abs(lz)-abs(lz2);
 } else if ( lz < 0 && lz2 >= 0 ) {
  d=abs(lz)+lz2;
 } else {
  d=lz2-lz;
 }
 area=w*h;
 volume=w*h*d;
 lines.Recycle();
 left=lines->Add(x,y,x,y2);
 right=lines->Add(x2,y,x2,y2);
 top=lines->Add(x,y,x2,y);
 bottom=lines->Add(x,y2,x2,y2);
 centroid.Set((x+x2)/2.0,(y+y2)/2.0);
}

bool Box::Collides( Box *b, CoplanarLinesResults *out, Box *over ) {
 out->Clear();
 if ( !b->lines.pointer ) b->CalculateDimensions();
 if ( !lines.pointer ) CalculateDimensions();
 Box *Abox=null,*Bbox=null;
 // Abox should be the bigger box
 if ( this->area > b->area ) {
  Abox=this;
  Bbox=b;
 } else {
  Abox=b;
  Bbox=this;
 }
 if ( !Bbox->overlaps(Abox) ) return false;
 bool colliding=Bbox->lines->Collides( Abox->lines, out );
 if ( colliding ) {
  if ( out->count == 1 ) {
   OUTPUT("Box::Collides(box) reports an invalid case where boxes collided on less than two points.\n");
   return true;
  } else if ( out->count == 2 ) {
   CoplanarLinesResult *A=(CoplanarLinesResult *) out->first;
   CoplanarLinesResult *B=(CoplanarLinesResult *) out->last;
   // There's one case where if a,b are on a vertical or horizontal, we must calculate the
   // overlapping box.
   if ( A->x==B->x) { // Both points fall on the x or x2 side
    if ( Bbox->x2 < Abox->maxX() ) // We're colliding on left side of b
    over->Set(Bbox->x2,A->y,A->x,B->y);
    else over->Set(Bbox->x,A->y,B->x,B->y);
   } else if ( A->y==B->y ) { // Both points fall on the y or y2 side
    if ( Bbox->y2 < Abox->maxY() )
    over->Set(A->x,A->y,B->x,Bbox->y2);
    else over->Set(A->x,Bbox->y,B->x,B->y);
   } else
   over->Set(A->x,A->y,B->x,B->y); // thus the name...
  } else if ( out->count > 2 ) {
   // If out->count > 2 the boxes share common sides, so we must find greatest and least points.
   Vertexd min,max;
   out->Min(&min);
   out->Max(&max);
   over->Set(min.x,min.y,max.x,max.y);
  }
 }
 if ( out->count == 0 ) {
  if ( area < b->area ) over->Set(this);
  else over->Set(b);
 }
 // out->count could be 0 because we are within the box
 return colliding;
}

// Warning: incomplete
bool Box::Collides3d( Box *b, CoplanarLinesResults *out, Box *over ) {
 out->Clear();
 if ( !overlaps3d(b) && !b->overlaps(this) ) return null;
 if ( !b->lines.pointer ) b->CalculateDimensions();
 if ( !lines.pointer ) CalculateDimensions();
 ///...///
 if ( out->count == 0 ) {
  if ( volume < b->volume ) over->Set(this);
  else over->Set(b);
 }
 return false;
}

bool Box::Collides( Lines *in, CoplanarLinesResults *out ) {
 out->Clear();
 if ( !lines.pointer ) CalculateDimensions();
 return ( lines->Collides(in,out) );
}

bool Line::Collides( Circle *c, Line *out, int precision=16 ) {
 if ( !c->lines || c->precision != precision ) c->Approximate(precision);
 Lines lines;
 lines.Add(c,precision);
 CoplanarLinesResults results;
 bool result=lines.Collides(this,&results);
 if ( result ) {
  if ( results.count == 1 ) {
   CoplanarLinesResult *a=(CoplanarLinesResult *) results.first;
   out->Set(a->x,a->y,a->x,a->y);
   return true;
  } else if ( results.count == 2 ) {
   CoplanarLinesResult *a=(CoplanarLinesResult *) results.first;
   CoplanarLinesResult *b=(CoplanarLinesResult *) results.last;
   out->Set(a->x,a->y,b->x,b->y);
   return true;
  }
 } else {
  out->Set(0.0,0.0,0.0,0.0,0.0,0.0);
 }
 return result;
}

bool Line::Collides( Vertices *points, CoplanarLinesResults *out, bool connectLastToFirst=true ) {
 out->Clear();
 if ( points->count == 0 ) return false;
 Lines lines;
 lines.Add(points,connectLastToFirst);
 return lines.Collides(this,out);
}

bool Line::Collides( Lines *lines, CoplanarLinesResults *out ) {
 return lines->Collides(this,out);
}

bool Line::Collides( Box *b, BoxCollisionResults *out ) {
 out->Reset();
 out->startsInside=b->within(x,y);
 out->endsInside=b->within(x2,y2);
 // Convert box B to Lines
 if ( b->lines->count == 0 ) b->CalculateDimensions();
 out->collided=b->lines->Collides(this,&out->results);
 if ( out->collided ) {
  out->collisions=out->results.count;
  EACH(out->results.first,CoplanarLinesResult,p) {
   if ( p->segment == *(b->top) ) {
    out->top=true;
   } else if ( p->segment == *(b->bottom) ) {
    out->bottom=true;
   } else if ( p->segment == *(b->left) ) {
    out->left=true;
   } else if ( p->segment == *(b->right) ) {
    out->right=true;
   }
  }
  if ( out->results.count == 2 ) {
   CoplanarLinesResult *a=(CoplanarLinesResult *) out->results.first;
   CoplanarLinesResult *b=(CoplanarLinesResult *) out->results.last;
   out->x=a->x;
   out->y=a->y;
   out->x2=b->x;
   out->y2=b->y;
   return true;
  } else if ( out->results.count == 1 ) {
   CoplanarLinesResult *a=(CoplanarLinesResult *) out->results.first;
   out->x=a->x;
   out->y=a->y;
   return true;
  }
  return false;
 }
 return out->collided;
}


bool Line::within( Box *b ) {
 return b->within(x,y) && b->within(x2,y2);
}

bool Line::within( Circle *c ) {
 return c->within(x,y) && c->within(x2,y2);
}

bool Line::within3d( Box *b ) {
 return b->within(x,y,z) && b->within(x2,y2,z2);
}

bool Line::within3d( Circle *c ) {
 return c->within(x,y,z) && c->within(x2,y2,z2);
}

bool Lines::Collides( Line *line, CoplanarLinesResults *out ) {
 out->Clear();
 out->sx=line->x;
 out->sy=line->y;
 Vertexd collision;
 FOREACH(Line,L) if ( L->Collides(line,&collision) ) out->Add(L,&collision);
 out->SortDistance();
 return (out->count > 0);
}

bool Lines::Collides( Line *line, CoplanarLinesResults *out, bool reverseSort ) {
 out->Clear();
 out->sx=line->x2;
 out->sy=line->y2;
 Vertexd collision;
 FOREACH(Line,L) if ( L->Collides(line,&collision) ) out->Add(L,&collision);
 out->SortDistance();
 return (out->count > 0);
}

void Lines::Add( Box *b ) {
 Add(b->x,b->y,b->x2,b->y);
 Add(b->x,b->y2,b->x2,b->y2);
 Add(b->x,b->y,b->x,b->y2);
 Add(b->x2,b->y,b->x2,b->y2);
}

bool Lines::Collides( Lines *lines, CoplanarLinesResults *out ) {
 Vertexd collision;
 FOREACH(Line,L) EACH(lines->first,Line,M) if ( L->Collides(M,&collision) ) {
  out->Add(M,&collision);
 }
 out->SortDistance();
 return (out->count > 0);
}