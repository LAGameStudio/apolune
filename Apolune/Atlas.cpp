#include "Atlas.h"
#include "Art2d.h"

AltasLibrary atlasLibrary;

AtlasFonts atlasFonts;

 void AtlasFrame::GenGeo( bool mirrored, GLImage *source, Indexed<int> *elements, int k ) {
  double textureW=(double)source->width;
  double textureH=(double)source->height;
  if ( texturePacker ) {
   if ( mirrored ) {
    if ( trimmed ) {
     if ( rotated ) {
      (*elements)[k]=art.stash.GenQuadRotated(
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y2/sourceSize.h,
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y2/sourceSize.h,
       extents.x/textureW,       extents.y/textureH,
       extents.x2/textureW,      extents.y2/textureH
      );
      (*elements)[k*2]=art.stash.GenQuadRotated(
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y2/sourceSize.h,
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y2/sourceSize.h,
       -extents.x/textureW,      extents.y/textureH,
       -extents.x2/textureW,     extents.y2/textureH
      );
      (*elements)[k*3]=art.stash.GenQuadRotated(
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y2/sourceSize.h,
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y2/sourceSize.h,
       -extents.x/textureW,      -extents.y/textureH,
       -extents.x2/textureW,     -extents.y2/textureH
      );
      (*elements)[k*4]=art.stash.GenQuadRotated(
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y2/sourceSize.h,
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y2/sourceSize.h,
       extents.x/textureW,      -extents.y/textureH,
       extents.x2/textureW,     -extents.y2/textureH
      );
     } else { // not rotated, tp
      (*elements)[k]=art.stash.GenQuad(
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y2/sourceSize.h,
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y2/sourceSize.h,
       extents.x/textureW,       extents.y/textureH,
       extents.x2/textureW,      extents.y2/textureH
      );
      (*elements)[k*2]=art.stash.GenQuad(
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y2/sourceSize.h,
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y2/sourceSize.h,
       -extents.x/textureW,      extents.y/textureH,
       -extents.x2/textureW,     extents.y2/textureH
      );
      (*elements)[k*3]=art.stash.GenQuad(
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y2/sourceSize.h,
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y2/sourceSize.h,
       -extents.x/textureW,      -extents.y/textureH,
       -extents.x2/textureW,     -extents.y2/textureH
      );
      (*elements)[k*4]=art.stash.GenQuad(
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y2/sourceSize.h,
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y2/sourceSize.h,
       extents.x/textureW,      -extents.y/textureH,
       extents.x2/textureW,     -extents.y2/textureH
      );
     }
    } else { // not trimmed, tp
     if ( rotated ) {
      (*elements)[k]=art.stash.GenQuadRotated(
       0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
       extents.x/textureW,       extents.y/textureH,
       extents.x2/textureW,      extents.y2/textureH
      );
      (*elements)[k*2]=art.stash.GenQuadRotated(
       0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
       -extents.x/textureW,       extents.y/textureH,
       -extents.x2/textureW,      extents.y2/textureH
      );
      (*elements)[k*3]=art.stash.GenQuadRotated(
       0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
       -extents.x/textureW,       -extents.y/textureH,
       -extents.x2/textureW,      -extents.y2/textureH
      );
      (*elements)[k*4]=art.stash.GenQuadRotated(
       0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
       extents.x/textureW,       -extents.y/textureH,
       extents.x2/textureW,      -extents.y2/textureH
      );
     } else { // not rotated, tp
      (*elements)[k]=art.stash.GenQuad(
       0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
       extents.x/textureW,       extents.y/textureH,
       extents.x2/textureW,      extents.y2/textureH
      );
      (*elements)[k*2]=art.stash.GenQuad(
       0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
       -extents.x/textureW,       extents.y/textureH,
       -extents.x2/textureW,      extents.y2/textureH
      );
      (*elements)[k*3]=art.stash.GenQuad(
       0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
       -extents.x/textureW,       -extents.y/textureH,
       -extents.x2/textureW,      -extents.y2/textureH
      );
      (*elements)[k*4]=art.stash.GenQuad(
       0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
       extents.x/textureW,      -extents.y/textureH,
       extents.x2/textureW,     -extents.y2/textureH
      );
     }
    }
   } else { // not mirrored, tp
    if ( trimmed ) {
     if ( rotated ) {
      (*elements)[k]=art.stash.GenQuadRotated(
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y2/sourceSize.h,
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y2/sourceSize.h,
       extents.x/textureW,
       extents.y/textureH,
       extents.x2/textureW,
       extents.y2/textureH
      );
     } else { // not rotated, tp
      (*elements)[k]=art.stash.GenQuad(
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y/sourceSize.h,
       spriteSourceSize.x2/sourceSize.w,   spriteSourceSize.y2/sourceSize.h,
       spriteSourceSize.x/sourceSize.w,    spriteSourceSize.y2/sourceSize.h,
       extents.x/textureW,
       extents.y/textureH,
       extents.x2/textureW,
       extents.y2/textureH
      );
     }
    } else { // not trimmed, tp
     if ( rotated ) {
      (*elements)[k]=art.stash.GenQuadRotated(
       0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
       extents.x/textureW,       extents.y/textureH,
       extents.x2/textureW,      extents.y2/textureH
      );
     } else { // not rotated, tp
      (*elements)[k]=art.stash.GenQuad(
       0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
       extents.x/textureW,       extents.y/textureH,
       extents.x2/textureW,      extents.y2/textureH
      );
     }
    }
   }
  } else { // grid-based, handled elsewhere
   if ( mirrored ) {
   } else {
   }
  }
 }

void Atlas::GenElements() {
 if ( dontGenElements ) return;
 if ( uniformGrid ) {
  int number=rows*columns;
  this->elements.Size((size_t)(number*(mirrored?4:1)));
  int k=0;
  for ( int j=0; j<rows; j++ ) for ( int i=0; i<columns; i++ ) {
   Cartesiand *tc=&coords(i,j);
   if ( mirrored ) {
    elements[k]  =art.stash.GenQuad(tc->x,    tc->y,    tc->x2,tc->y2);
    elements[k*2]=art.stash.GenQuad(tc->x+1.0,tc->y,    tc->x2,tc->y2);
    elements[k*3]=art.stash.GenQuad(tc->x+1.0,tc->y+1.0,tc->x2,tc->y2);
    elements[k*4]=art.stash.GenQuad(tc->x,    tc->y+1.0,tc->x2,tc->y2);
   } else {
    elements[k]=art.stash.GenQuad(tc->x,tc->y,tc->x2,tc->y2);
   }
   k++;
  }
 } else {
  this->elements.Size(coords.length*(size_t)(mirrored?4:1));
  for ( int k=0; k<(int)elements.length; k++ ) {
   Cartesiand *tc=&coords(k);
   if ( mirrored ) {
    elements[k]  =art.stash.GenQuad(tc->x,    tc->y,    tc->x2,tc->y2);
    elements[k*2]=art.stash.GenQuad(tc->x+1.0,tc->y,    tc->x2,tc->y2);
    elements[k*3]=art.stash.GenQuad(tc->x+1.0,tc->y+1.0,tc->x2,tc->y2);
    elements[k*4]=art.stash.GenQuad(tc->x,    tc->y+1.0,tc->x2,tc->y2);
   } else {
    elements[k]=art.stash.GenQuad(tc->x,tc->y,tc->x2,tc->y2);
   }
  }
 }
}

void LoadAtlasFonts() {
 atlasFonts.Load("data/images/atlasfonts/atlasfonts.txt");
}