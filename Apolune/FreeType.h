/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once

#include "GLWindow.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
/* Based on	a quick and simple opengl font library that uses GNU freetype2, written
 *	and distributed as part of a tutorial for nehe.gamedev.net.	Sven Olsen, 2003
 */
#include "Freetype.h"
#include "Model.h"

#include "Triangulate.h"

//#include "TTF.h"
//using namespace TTF;

#define BEZIER_STEPS ( (const unsigned int) 5 )

class FreetypeContour : public ListItem {
public:
 FT_Outline outline;
 Vertices points,front,back;
 Zbool clockwise;
 FreetypeContour( FT_Vector *points, char *tags, short n, FT_Outline source ) : ListItem() {
  outline=source;
  Vertex P;
  Vertex C( (float) points[(n - 1) % n].x, (float) points[(n - 1) % n].y, 0.0f );
  Vertex N( (float) points[0].x,           (float) points[0].y,           0.0f );
  Vertex a;
  Vertex b(&N); b.Subtract(&C);
  double olddir, dir = atan2(b.y, b.x);
  double angle = 0.0;
  // See http://freetype.sourceforge.net/freetype2/docs/glyphs/glyphs-6.html
  // for a full description of FreeType tags.
  for(unsigned int i = 0; i < (unsigned int) n; i++) {
   P.Set(&C);
   C.Set(&N);
   N.Set( (float) points[(i + 1) % n].x, (float) points[(i + 1) % n].y, 0.0f );
   b.Set(&N); b.Subtract(&C);
   olddir = dir;
   dir = atan2(b.y,b.x);
   // Compute our path's new direction.
   double t = dir - olddir;
   if (t < -PI) t += 2 * PI;
   if (t >  PI) t -= 2 * PI;
   angle += t;
   // Only process point tags we know.
   if ( n < 2 || FT_CURVE_TAG(tags[i]) == FT_Curve_Tag_On ) this->points.Add(C.x,C.y,C.z);
   else if ( FT_CURVE_TAG(tags[i]) == FT_Curve_Tag_Conic ) {
    Vertex P2(&P);
    Vertex N2(&N);
    // Previous point is either the real previous point (an "on"
    // point), or the midpoint between the current one and the
    // previous "conic off" point.
    if ( FT_CURVE_TAG(tags[(i - 1 + n) % n]) == FT_Curve_Tag_Conic ) {
     P2.Set( (C.x+P.x)*0.5f, (C.y+P.y)*0.5f, 0.0f );
     this->points.Add(P2.x,P2.y,P2.z);
    }
    // Next point is either the real next point or the midpoint.
    if ( FT_CURVE_TAG(tags[(i + 1) % n]) == FT_Curve_Tag_Conic )
     N2.Set( (C.x+N.x)*0.5f, (C.y+N.y)*0.5f, 0.0f );

    Quadratic(&P2,&C,&N2);
   } else if ( FT_CURVE_TAG(tags[i]) == FT_Curve_Tag_Cubic
           && FT_CURVE_TAG(tags[(i + 1) % n]) == FT_Curve_Tag_Cubic ) {
    Vertex D (
     (float) points[(i + 2) % n].x,
     (float) points[(i + 2) % n].y,
     0.0f
    );
    Cubic(&P,&C,&N,&D);
   }
  }
  // If final angle is positive (+2PI), it's an anti-clockwise contour,
  // otherwise (-2PI) it's clockwise.
  clockwise = (angle < 0.0);
 }

 // From FTGL FTContour
 void SetParity( int parity ) {
  if ( ((parity & 1) && clockwise) || (!(parity & 1) && !clockwise) ) {
   points.ReverseOrder(); 
   clockwise = !clockwise;
  }
 }
 const double *Front( unsigned int index ) {
  static double f[3];
  Vertex *v=(Vertex *) front.Element(index);
  f[0]=v->x;
  f[1]=v->y;
  f[2]=v->z;
  return f;
 }
 const double *Back( unsigned int index ) {
  static double b[3];
  Vertex *v=(Vertex *) back.Element(index);
  b[0]=v->x;
  b[1]=v->y;
  b[2]=v->z;
  return b;
 }
 const double *Point( unsigned int index ) {
  static double p[3];
  Vertex *v=(Vertex *) points.Element(index);
  p[0]=v->x;
  p[1]=v->y;
  p[2]=v->z;
  return p;
 }
 void Quadratic( Vertex *a, Vertex *b, Vertex *c ) {
  for(unsigned int i = 1; i < BEZIER_STEPS; i++) {
   float t = static_cast<float>(i) / BEZIER_STEPS;
   Vertex U(
    (1.0f - t) * a->x + t * b->x, (1.0f - t) * a->y + t * b->y, (1.0f - t) * a->z + t * b->z
   );
   Vertex V(
    (1.0f - t) * b->x + t * c->x, (1.0f - t) * b->y + t * c->y, (1.0f - t) * b->z + t * c->z
   );
   Vertex *K = new Vertex(
    (1.0f - t) * U.x + t * V.x, (1.0f - t) * U.y + t * V.y, (1.0f - t) * U.z + t * V.z
   );
   this->points.Append(K);
  }
 }
 void Cubic( Vertex *a, Vertex *b, Vertex *c, Vertex *d ) {
  for(unsigned int i = 0; i < BEZIER_STEPS; i++) {
   float t = static_cast<float>(i) / BEZIER_STEPS;
   Vertex U(
    (1.0f - t) * a->x + t * b->x, (1.0f - t) * a->y + t * b->y, (1.0f - t) * a->z + t * b->z
   );
   Vertex V(
    (1.0f - t) * b->x + t * c->x, (1.0f - t) * b->y + t * c->y, (1.0f - t) * b->z + t * c->z
   );
   Vertex W(
    (1.0f - t) * c->x + t * d->x, (1.0f - t) * c->y + t * d->y, (1.0f - t) * c->z + t * d->z
   );
   Vertex M(
    (1.0f - t) * U.x + t * V.x, (1.0f - t) * U.y + t * V.y, (1.0f - t) * U.z + t * V.z
    );
   Vertex N(
    (1.0f - t) * V.x + t * W.x, (1.0f - t) * V.y + t * W.y, (1.0f - t) * V.z + t * W.z
   );
   Vertex *K=new Vertex(
    (1.0f - t) * M.x + t * N.x, (1.0f - t) * M.y + t * N.y, (1.0f - t) * M.z + t * N.z
   );
   this->points.Append(K);
  }
 }
};

class FreetypeContours : public LinkedList {
public:
 CLEARLISTRESET(FreetypeContour);
 //                                         M         This function is a bit tricky. 
 //                            - - - - - - X          Given a path ABC, it returns the
 //                             ^         / '         coordinates of the outset point
 //                             | 64.0   /   '        facing B on the left at a distance
 //  X---->-----X     ==>    X--v-------X     '       point of 64.0
 // A          B \          A          B \   .>'
 //               \                       \<'  64.0
 //                \                       \     '
 //               C X                     C X
 void ComputeOuterPoint(Vertex *a, Vertex *b, Vertex *c, Vertex *out ) {
  Vertex ba(a);
  ba.Subtract(b);
  ba.normalize();
  Vertex bc(c);
  bc.Subtract(b);
  Vertex t(
   bc.x * -ba.x + bc.y * -ba.y,
   bc.z * ba.y + bc.y * -ba.x,
   0.0f
  );
  double norm = sqrt( t.x * t.x + t.y * t.y );
  double dist = 64.0 * sqrt( (norm - t.x) / (norm + t.y) );
  t.x=(float) (t.y < 0.0f ? dist : -dist);
  t.y=64.0f;
  out->Set( t.x * -ba.x + t.y * ba.y, t.x * -ba.y + t.y * -ba.x, 0.0f );
 }
 void Calculate( float front, float back, float depth ) {
  FOREACH(FreetypeContour,c) {
   
  }
 }
 Vertices *points() {
  Vertices *out=new Vertices;
  FOREACH(FreetypeContour,c) {
   out->CopyAppend(&c->points);
  }
  return out;
 }
};

class FreetypeGlyph : public ListItem {
public:
 Zstring name;
 Vertices outlines;
 Cartesian bounds;
 FT_Face face;
 FT_ULong index;
 char character;
 FT_Glyph glyph;
 FT_GlyphSlot slot;
 FT_Outline outline;
 FT_Error error;
 FreetypeContours inner,outer; // "inner" = hole, "outer" = exterior
 ImmediateModel filled,extruded;
 Zfloat front,back,depth;
 FreetypeGlyph( float front, float back, float depth, FT_Face f, FT_ULong i, FT_GlyphSlot gs, FT_Glyph g, char ch='?' ) {
  Set(front,back,depth,f,i,gs,g,ch);
 }
 void Set( float front, float back, float depth, FT_Face f, FT_ULong i, FT_GlyphSlot gs, FT_Glyph g, char ch='?' ) {
  character=ch;
  this->front=front;
  this->back=back;
  this->depth=depth;
  face=f;
  index=i;
  glyph=g;
  static char buf[STRING_SIZE];
  if ( FT_Get_Glyph_Name(face,index,&buf,STRING_SIZE) > 0 ) name=string(buf);
  slot=gs;
  outline=gs->outline;
  Recompute();
 }
 // Populate Vertices with resulting contour
 void Recompute() {
 	FT_Orientation orientation = FT_Outline_Get_Orientation(&outline);
  Clear();
  short startIndex=0;
  for ( unsigned int i=0; i < (unsigned int) outline.n_contours; i++ ) {
   FT_Vector *pointList= &outline.points[startIndex];
   short endIndex = outline.contours[i];
   char *tagList = &outline.tags[startIndex];
   short contourLength=(endIndex-startIndex) + 1;
   FreetypeContour *c=new FreetypeContour(pointList,tagList,contourLength,outline);
   if ( FT_ORIENTATION_TRUETYPE ) {
    if ( c->clockwise ) outer.Append(c);
    else inner.Append(c);
   } else if ( FT_ORIENTATION_POSTSCRIPT ) {
    if ( c->clockwise ) outer.Append(c);
    else inner.Append(c);
   } else {
    OUTPUT("FreetypeGlyph orientation could not be determined.\n");
    delete c;
   }
   if ( c->points.first ) {
    outlines.CopyAppendIfUnique(&c->points);
    //Vertex *end=((Vertex *)c->points.first)->Copy();
    //outlines.Append(end); // Duplicate first point to create a closed loop
    //end->flags.Toggle(vertex_contour_end,true);
   }
   startIndex = endIndex + 1;
  }
  EACH(outer.first,FreetypeContour,c) {
   Vertex *left=c->points.leastX();
   int parity=0;
   EACH(outer.first,FreetypeContour,c2) if ( c2 != c ) {
    EACH(c2->points.first,Vertex,v) {
     Vertex *v2 = (Vertex *) ( v->next ? v->next : c2->points.first );
     if ( (v->y < left->y && v2->y < left->y)
       || (v->y >= left->y && v2->y >= left->y)
       || (v->x > left->x && v2->x > left->x) )
      continue;
     else if ( v->x < left->x && v2->x < left->x ) parity++;
     else {
      Vertex a(v); a.Subtract(left);
      Vertex b(v2); b.Subtract(left);
      if ( b.x * a.y > b.y * a.x ) parity++;
     }
    }
   }
   c->SetParity(parity);
  }
  // Calculate the related point values (outset points, front and back faces)
  inner.Calculate(front,back,depth);
  outer.Calculate(front,back,depth);
  Tesselate();
  outlines.Bounds();
 }
 void Clear() {
  outer.Clear();
  inner.Clear();
  filled.polygons.Clear();
  extruded.polygons.Clear();
 }
 Vertices result1,result2;
 Polygons result3,result4;
 void Tesselate() {
  /*
    // Debug info
  OUTPUT( "Glyph: \n" );
  OUTPUT( "Outside contours (length=%d)\n", (int) outer.count );
  OUTPUT( "Inside contours: (length=%d)\n", (int) inner.count );
  EACH(outer.first,FreetypeContour,c) {
   ModelAssemblerNode outerContour(ma_contour,&c->points);
   sgCContour *sgc=(sgCContour *)outerContour.object.pointer;
   if ( sgc->IsSelfIntersecting() ) {
    OUTPUT("Self-intersecting outer contour found in glyph. Skipping.\n");
    continue;
   }
   if ( !sgc->IsClosed() ) {
    OUTPUT("Outer contour is not closed in glyph. Skipping.\n");
    continue;
   }
   ModelAssemblerNodes holeContours;
   EACH(inner.first,FreetypeContour,d) {
    ModelAssemblerNode *contour=new ModelAssemblerNode(ma_contour,&d->points);
    sgCContour *sgc2=(sgCContour *)contour->object.pointer;
    if ( sgc->IsFirstObjectInsideSecondObject(*sgc2,*sgc) ) {
     holeContours.Append(contour);
    } else {
     OUTPUT("Interior contour was not inside exterior contour. Disposing.\n");
     delete contour;
    }
   }
   modelAssembler.ContourToGlyph(&outerContour,&holeContours,&result3);
  }
  */
  //if ( !outer.first ) return;
  //Vertices outside;
  //EACH(outer.first,FreetypeContour,c) {
  // outside.CopyAppend(&c->points);
  //}
  //
  //// Use sgCore to tesselate Delauney
  //VerticesHandles holes;
  //EACH(inner.first,FreetypeContour,c) {
  // if ( c == inner.first ) continue;
  // if ( c->points.count > 1 ) {
  //  holes.Add(&c->points);
  // }
  //}
  //// Debug info
  //OUTPUT( "Glyph: \n" );
  //OUTPUT( "Outside contour (length=%d): %s\n", (int) outside.count, outside.toString().c_str() );
  //EACH(holes.first,VerticesHandle,vh) {
  // OUTPUT("Hole contour: (length=%d): %s\n", (int) vh->p->count, vh->p->toString().c_str() );
  //}
  //OUTPUT("Total #of holes: %d\n", (int) holes.count );
  //modelAssembler.ContourToGlyph(&outside,&holes,&result3); //issues?

  // Use Bourke's superTriangle

  // Use TTF's Triangulator class
  //Triangulator t;


  return;
 }
 // Renders the outlines
 void Render( Crayon color, float ox, float oy, float sx, float sy ) {
  outlines.DrawInvertY( color, ox, oy, sx, sy );
 }
};

class FreetypeGlyphs : public LinkedList {
public:
 CLEARLISTRESET(FreetypeGlyph);
 FreetypeGlyph *byChar( char c ) {
  FOREACH(FreetypeGlyph,g) if ( g->character == c || g->index == c ) return g;
  return null;
 }
 FreetypeGlyph *byIndex( FT_ULong idx ) {
  FOREACH(FreetypeGlyph,g) if ( g->index == idx ) return g;
  return null;
 }
 void Add( float front, float back, float depth, FT_Face f, FT_ULong i, FT_GlyphSlot gs, FT_Glyph g, char ch='?' ) {
  Append(new FreetypeGlyph(front, back, depth, f,i,gs,g,ch));
 }
};

class FreetypeFont : public ListItem {
public:
 Zstring filename;
 FT_Face face;
 FreetypeGlyphs glyphs;

 Zfloat front,back,depth;
 // Front and back are the "outset" values, these values
 // are used for instance from the side:
 //      __f
 //     /  \    <- where f outset < b outset and b outset > 0
 //    .____.b

 //Create a display list coresponding to the given character.
 bool Glyph( FT_Face face, char ch ) { return ProcessGlyph(ch,ch); }
 bool ProcessGlyph( FT_ULong idx, char ch='?' ) {
  // The first thing we do is get FreeType to render our character
	 // into a bitmap.  This actually requires a couple of FreeType commands:
  // Load the Glyph for our character.
  FT_ULong index=FT_Get_Char_Index( face, idx );
  if ( ch == '?' ) ch=(char) (int) idx;
	 if ( FT_Load_Glyph( face, index, FT_LOAD_DEFAULT ) ) {
   OUTPUT("FT_Load_Glyph on face `%s` failed on unicode index %l ch=%c", filename.c_str(), (char)ch, ch );
   return false; // throw std::runtime_error("FT_Load_Glyph failed");
  }
 	//Move the face's glyph into a Glyph object.
  FT_Glyph glyph;
  if ( FT_Get_Glyph( face->glyph, &glyph ) ) {
   OUTPUT("FT_Get_Glyph failed");
   return false; //  throw std::runtime_error("FT_Get_Glyph failed");
  }
  glyphs.Add(front,back,depth,face,index,face->glyph,glyph,ch);
  return true;
 }
 FreetypeGlyph *Glyph( FT_ULong search ) {
  return glyphs.byIndex( FT_Get_Char_Index( face, search ) );
 }
 void ProcessGlyphs( ) {
  Zstring s=string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890=@&+$#()\"'%@~!,.?");
  const char *p=s.c_str();
  while ( *p != '\0' ) {
   bool result=ProcessGlyph( *p );
   if ( !result ) {
    OUTPUT("No glyph to load for %c\n", *p);
   }
   p++;
  }
 }
 // Renders 2d to the screen
 void Render( const char *phrase, Crayon color, float ox, float oy, float sx, float sy, float lineHeight, float charSpace=2.0f, float spaceWidth=16.0f ) {
  float x=ox, y=oy;
  const char *p=phrase;
  while ( *p != '\0' ) {
   if ( *p == ' ' ) {
    x+=spaceWidth;
   } else if ( *p == '\n' ) {
    x=ox;
    y+=lineHeight;
   } else {
    FreetypeGlyph *g=Glyph( *p );
    if ( g ) {
     g->Render( color, x,y,sx,sy );
     x+=(float) g->outlines.bounds.w * sx;
    }
    x+=charSpace;
   }
   p++;
  }
 }
};

class FreetypeFonts : public LinkedList {
public:
 FreetypeFont *Increase() { FreetypeFont *f=new FreetypeFont; Append(f); return f; }
 CLEARLISTRESET(FreetypeFont);
};

class Freetype {
public:
	FT_Library library;
 FreetypeFonts fonts;
 Zuint resolution;

 //Create and initilize a freetype font library.
 Freetype() {
	 if (FT_Init_FreeType( &library )) {
   OUTPUT("Freetype init failed.\n");
   return; //		throw std::runtime_error("FT_Init_FreeType failed");
  }
  resolution=6;
  // Call Load() in your GameInit after GL is loaded due to GLU Tesselation requirements.
 }

 void Load( const char *filename ) {
  if ( !file_exists(filename) ) {
   OUTPUT("Freetype:Load(`%s`) list file not found\n", filename);
   return;
  }
  Zstring file=file_as_string(filename);
  char *werd=file.next();
  while ( *werd != '\0' ) {
   if ( file_exists(werd) ) {
    OUTPUT("Freetype:LoadFont(`%s`) ", werd);
    LoadFont(werd,resolution);
    OUTPUT("\n");
   } else {
   OUTPUT("Freetype:LoadFont(`%s`) file not found\n", werd);
   return;
   }
   werd=file.next();
  }
 }

 //This function gets the first power of 2 >= the int that we pass it.
 inline int next_p2 ( int a )
 {
 	int rval=1;
 	while(rval<a) rval<<=1;
 	return rval;
 }

 void LoadFont( const char * fname, unsigned int res=1 ) {
	 //This is where we load in the font information from the file.
	 //Of all the places where the code might die, this is the most likely,
	 //as FT_New_Face will die if the font file does not exist or is somehow broken.
  FreetypeFont *font=fonts.Increase();
  FT_Face *face=&(font->face);

	 if (FT_New_Face( library, fname, 0, face )) {
   OUTPUT("Freetype could not load `%s`\n", fname);
	 	//throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");
   return;
  }

  if ( (*face)->charmap == NULL ) {
   OUTPUT("Freetype:LoadFont(`%s`) file does not contain unicode charmap\n", fname);
   return;
  }

	 //For some twisted reason, Freetype measures font size
	 //in terms of 1/64ths of pixels.  Thus, to make a font
	 //h pixels high, we need to request a size of h*64.
	 //(h << 6 is just a prettier way of writting h*64)
 	FT_Set_Char_Size( *face, res << 6, res << 6, 96, 96);
  font->ProcessGlyphs();
 }

 ~Freetype() {
 }

};

extern Freetype freetype;