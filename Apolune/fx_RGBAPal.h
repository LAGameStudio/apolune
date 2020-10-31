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
#include "GLImage.h"

#include "ListItem.h"
#include "LinkedList.h"

#include "Tween.h"
#include "Tweens.h"
#include "Crayon.h"
#include "Proce55or.h"
#include "Art.h"
#include "Audio.h"
#include "fx_TexFont.h"

#include "TextureLibrary.h"

/******************
 * fx_CoolRGBAPal *
 ******************/

extern Interface input;

class fx_CoolRGBAPal4f : public Proce55or {
public:
 Zpointer<GLImage> box,flare,grid;
 Zpointer<float> rgba_array;
 Crayon R,G,B,A,C;
 Crayon red,green,blue,white;
 Zint Rx,Ry,Rx2,Ry2,
      Gx,Gy,Gx2,Gy2,
      Bx,By,Bx2,By2,
      Ax,Ay,Ax2,Ay2,
      Cx,Cy,Cx2,Cy2,Cw,Ch,Cmx,Cmy;
 Zint Tw,Th,Mw,Mh;
 Zint Dx,Dy, mx,my;
 Zbool initialized;
 Zbool hoverRed,hoverGreen,hoverBlue,hoverAlpha,hoverColor,moving,up;
 Zstring label;
 Zint len,labx,laby;
 Crayon labelColor;
 Zpointer<TexFont> font;

 fx_CoolRGBAPal4f() : Proce55or() {
  C=crayons.Pick(alabaster);
  labelColor.Pick(alabaster);
  font=microFont;
 }

 virtual void WhenChanged() { }

 void Setup()
 {
  if ( initialized ) return;
  if ( rgba_array )
  C.Int( (int) ( ((float) rgba_array[0]) * 255.0f ), (int) ( ((float) rgba_array[1]) * 255.0f ), 
         (int) ( ((float) rgba_array[2]) * 255.0f ), (int) ( ((float) rgba_array[3]) * 255.0f ) );
  R.Int( C.r, 0, 0, 255 );
  G.Int( 0, C.g, 0, 255 );
  B.Int( 0, 0, C.b, 255 );
  A.Int( C.a, C.a, C.a, 255 );
  
  Tw=(w-8)/4; Th=h;
  red=red; green=green; blue=blue; white=white;

  if ( !box )   box   = library.find( "data/images/elements/rounder1.png" );
  if ( !flare ) flare = library.find( "data/images/elements/rounder_box.png" );
  if ( !grid )  grid  = library.find( "data/images/elements/smallcube.png" );

  Rx=0;      Ry=0; 
  Gx=Tw*1+2; Gy=0;
  Bx=Tw*2+4; By=0;
  Ax=Tw*3+6; Ay=0;

  Cw=w; Ch=Th*3;
  Cx=x; Cy=y-Ch-4;
  Cx2=x+Cw; 
  Cy2=y-4;

  Rx2=Rx+Tw;  Ry2=Ry+Th;
  Gx2=Gx+Tw;  Gy2=Gy+Th;
  Bx2=Bx+Tw;  By2=By+Th;
  Ax2=Ax+Tw;  Ay2=Ay+Th;

  Rx+=x; Ry+=y;  Rx2+=x; Ry2+=y;
  Gx+=x; Gy+=y;  Gx2+=x; Gy2+=y;
  Bx+=x; By+=y;  Bx2+=x; By2+=y;
  Ax+=x; Ay+=y;  Ax2+=x; Ay2+=y;

  Dx=Dy=0;

  Mw=Cw/2; Mh=Ch/2;
  initialized=true;
  len=(int)label.length;
 }

 void Draw(GLWindow *surface)
 {  
  Variable *v;
  Crayon c;

  WriteTexFont(surface,font,labelColor,additive,labx,laby,8,8,2,label.c_str());

  this->controller.Updatef();
  v=this->controller.variables.Get("fade");

  if ( moving ) {
   UpdateColor( up ? 1 : -1 );
   WhenChanged();
  }

  if ( rgba_array )
   C.Int( (int) ( ((float) rgba_array[0]) * 255.0f ), (int) ( ((float) rgba_array[1]) * 255.0f ), 
          (int) ( ((float) rgba_array[2]) * 255.0f ), (int) ( ((float) rgba_array[3]) * 255.0f ) );

  if ( v && v->tween ) {
   // red
   if ( hoverRed ) {
    c=crayons.Pick(red255);
    c.ScaleByAlphaf( v->f ); 
   } else  c.fromCrayon(crayons.Pick(red255));
   Stretchi(surface,flare,c,v->f,additive,Rx,Ry,Tw,Th);

   // green
   if ( hoverGreen ) {
    c=crayons.Pick(green255);
    c.ScaleByAlphaf( v->f ); 
   } else c.fromCrayon(crayons.Pick(green255));
   Stretchi(surface,flare,c,v->f,additive,Gx,Gy,Tw,Th);

   // blue
   if ( hoverBlue ) {
    c=crayons.Pick(blue255);
    c.ScaleByAlphaf( v->f ); 
   } else c.fromCrayon(crayons.Pick(blue255));
   Stretchi(surface,flare,c,v->f,additive,Bx,By,Tw,Th);

   // alpha
   if ( hoverAlpha ) {
    c=crayons.Pick(alabaster);
    c.ScaleByAlphaf( v->f ); 
   } else c.fromCrayon(crayons.Pick(alabaster));
   Stretchi(surface,flare,c,v->f,additive,Ax,Ay,Tw,Th);
  }

  // Solid boxes
  Stretchi(surface,box,R,1.0f,additive,Rx,Ry,Tw,Th);
  Stretchi(surface,box,G,1.0f,additive,Gx,Gy,Tw,Th);
  Stretchi(surface,box,B,1.0f,additive,Bx,By,Tw,Th);
  Stretchi(surface,box,A,1.0f,additive,Ax,Ay,Tw,Th);

  // Mixed
  Stretchi(surface,grid,C,1.0f,additive,Cx,Cy,Cw,Ch);

  // mixer grid
//  surface.blitTint = { 0, { 255, 255, 255 } }; 
//  Additive(surface,box.bitmap,Cx,Cy,0,0,Cw,Ch,box.bitmap.width,box.bitmap.height);  
 }

 void UpdateColor( int diff ) {
  if ( hoverColor ) {
   C.Int( (int) iwrap(Cmx+C.r,0,255), (int) iwrap(Cmy+C.g,0,255), (int) iwrap((Cmx+Cmy)/2+C.b,0,255), C.a );
   R.Int( C.r,0,0,255 );
   G.Int( 0,C.g,0,255 );
   B.Int( 0,0,C.b,255 ); 
  } else if ( hoverRed ) { 
   int q= (int) ( iwrap((int) C.r +  diff,0,255) );    
   R.Int(q,0,0,255);
   C.Int(q,C.g,C.b,C.a);
  } else if ( hoverGreen ) { 
   int q= (int) ( iwrap((int) C.g +  diff,0,255) );   
   G.Int(0,q,0,255);
   C.Int(C.r,q,C.b,C.a);
  } else if ( hoverBlue )  { 
   int q= (int) ( iwrap((int) C.b +  diff,0,255) );   
   B.Int(0,0,q,255); 
   C.Int(C.r,C.g,q,C.a);
  } else if ( hoverAlpha ) { 
   int q= (int) ( iwrap((int) C.a +  diff,0,255) );   
   A.Int(q,q,q,255); 
   C.Int(C.r,C.g,C.b,q);
  }
  if ( rgba_array ) {
   rgba_array[0] = ( (float) C.r / 255.0f );
   rgba_array[1] = ( (float) C.g / 255.0f );
   rgba_array[2] = ( (float) C.b / 255.0f );
   rgba_array[3] = ( (float) C.a / 255.0f );
  }
 }

 void LeftDown()
 {
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  if(!moving ) {
   hoverRed  =WITHIN(mx,my,Rx,Ry,Rx2,Ry2);
   hoverGreen=WITHIN(mx,my,Gx,Gy,Gx2,Gy2);
   hoverBlue =WITHIN(mx,my,Bx,By,Bx2,By2);
   hoverAlpha=WITHIN(mx,my,Ax,Ay,Ax2,Ay2);
   if ( hoverColor=WITHIN(mx,my,Cx,Cy,Cx2,Cy2) ) { Cmx=mx-Cx+Mw; Cmy=my-Cy+Mh; }
   Dx = mx;
   Dy = my;
   UpdateColor(1);
   WhenChanged();
//   parent->Capture();
   moving=true; up=true;
  }
 }

 void LeftUp()
 {
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  hoverRed  =WITHIN(mx,my,Rx,Ry,Rx2,Ry2);
  hoverGreen=WITHIN(mx,my,Gx,Gy,Gx2,Gy2);
  hoverBlue =WITHIN(mx,my,Bx,By,Bx2,By2);
  hoverAlpha=WITHIN(mx,my,Ax,Ay,Ax2,Ay2);
  if ( hoverColor=WITHIN(mx,my,Cx,Cy,Cx2,Cy2) ) { Cmx=mx-Cx; Cmy=my-Cy; }
  if(moving) {  
//   PARENT.ReleaseCapture();
   moving = false; 
  }
 }


 void RightDown()
 {
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  if(!moving ) {
   hoverRed  =WITHIN(mx,my,Rx,Ry,Rx2,Ry2);
   hoverGreen=WITHIN(mx,my,Gx,Gy,Gx2,Gy2);
   hoverBlue =WITHIN(mx,my,Bx,By,Bx2,By2);
   hoverAlpha=WITHIN(mx,my,Ax,Ay,Ax2,Ay2);
   if ( hoverColor=WITHIN(mx,my,Cx,Cy,Cx2,Cy2) ) { Cmx=mx-Cx; Cmy=my-Cy; }
   Dx = mx;
   Dy = my;
   UpdateColor(-1);
   WhenChanged();
//   PARENT.Capture();
   moving=true; up=false;
  }
 }

 void RightUp()
 {
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  hoverRed  =WITHIN(mx,my,Rx,Ry,Rx2,Ry2);
  hoverGreen=WITHIN(mx,my,Gx,Gy,Gx2,Gy2);
  hoverBlue =WITHIN(mx,my,Bx,By,Bx2,By2);
  hoverAlpha=WITHIN(mx,my,Ax,Ay,Ax2,Ay2);
  if ( hoverColor=WITHIN(mx,my,Cx,Cy,Cx2,Cy2) ) { Cmx=mx-Cx; Cmy=my-Cy; }
  if(moving) {
//   PARENT.ReleaseCapture();
   moving = false;
  }
 }

 void MouseMove()
 {
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  if ( !moving ) {
   hoverRed  =WITHIN(mx,my,Rx,Ry,Rx2,Ry2);
   hoverGreen=WITHIN(mx,my,Gx,Gy,Gx2,Gy2);
   hoverBlue =WITHIN(mx,my,Bx,By,Bx2,By2);
   hoverAlpha=WITHIN(mx,my,Ax,Ay,Ax2,Ay2);
   if ( hoverColor=WITHIN(mx,my,Cx,Cy,Cx2,Cy2) ) { Cmx=mx-Cx; Cmy=my-Cy; }
  } else {
   byte diff=(byte)((int)idistance(mx,my,Dx,Dy)%256);
   Dx=mx; Dy=my;
   UpdateColor( diff );
   WhenChanged();
  }
 }
};

class fx_CoolRGBPal3f : public Proce55or {
public:
 Zpointer<GLImage> box,flare,grid;
 Zpointer<float> rgba_array;
 Crayon R,G,B,C;
 Crayon red,green,blue,white;
 Zint Rx,Ry,Rx2,Ry2,
      Gx,Gy,Gx2,Gy2,
      Bx,By,Bx2,By2,
      Cx,Cy,Cx2,Cy2,Cw,Ch,Cmx,Cmy;
 Zint Tw,Th,Mw,Mh;
 Zint Dx,Dy, mx,my;
 Zbool initialized;
 Zbool hoverRed,hoverGreen,hoverBlue,hoverColor,moving,up;
 Zstring label;
 Zint len,labx,laby;
 Crayon labelColor;
 Zpointer<TexFont> font;

 fx_CoolRGBPal3f() : Proce55or() {
  C=crayons.Pick(alabaster);
  labelColor.Pick(alabaster);
  font=microFont;
 }

 virtual void WhenChanged() { }

 void Setup()
 {
  if ( initialized ) return;
  if ( rgba_array )
  C.Int( (int) ( ((float) rgba_array[0]) * 255.0f ),
         (int) ( ((float) rgba_array[1]) * 255.0f ), 
         (int) ( ((float) rgba_array[2]) * 255.0f ), 255 );
  R.Int( C.r, 0, 0, 255 );
  G.Int( 0, C.g, 0, 255 );
  B.Int( 0, 0, C.b, 255 );
  
  Tw=(w-6)/3; Th=h;
  red=red; green=green; blue=blue; white=white;

  if ( !box )   box   = library.find( "data/images/elements/rounder1.png" );
  if ( !flare ) flare = library.find( "data/images/elements/rounder_box.png" );
  if ( !grid )  grid  = library.find( "data/images/elements/smallcube.png" );

  Rx=0;      Ry=0; 
  Gx=Tw*1+2; Gy=0;
  Bx=Tw*2+4; By=0;

  Cw=w; Ch=Th*3;
  Cx=x; Cy=y-Ch-4;
  Cx2=x+Cw; 
  Cy2=y-4;

  Rx2=Rx+Tw;  Ry2=Ry+Th;
  Gx2=Gx+Tw;  Gy2=Gy+Th;
  Bx2=Bx+Tw;  By2=By+Th;

  Rx+=x; Ry+=y;  Rx2+=x; Ry2+=y;
  Gx+=x; Gy+=y;  Gx2+=x; Gy2+=y;
  Bx+=x; By+=y;  Bx2+=x; By2+=y;

  Dx=Dy=0;

  Mw=Cw/2; Mh=Ch/2;
  initialized=true;
  len=(int)label.length;
 }

 void Draw(GLWindow *surface)
 {  
  Variable *v;
  Crayon c;

  WriteTexFont(surface,font,labelColor,additive,labx,laby,8,8,2,(char *)label.c_str());

  this->controller.Updatef();
  v=this->controller.variables.Get("fade");

  if ( moving ) {
   UpdateColor( up ? 1 : -1 );
   WhenChanged();
  }

  if ( rgba_array )
   C.Int( (int) ( ((float) rgba_array[0]) * 255.0f ),
          (int) ( ((float) rgba_array[1]) * 255.0f ), 
          (int) ( ((float) rgba_array[2]) * 255.0f ), 255 );

  if ( v && v->tween ) {
   // red
   if ( hoverRed ) {
    c=crayons.Pick(red255);
    c.ScaleByAlphaf( v->f ); 
   } else  c.fromCrayon(crayons.Pick(red255));
   Stretchi(surface,flare,c,v->f,additive,Rx,Ry,Tw,Th);

   // green
   if ( hoverGreen ) {
    c=crayons.Pick(green255);
    c.ScaleByAlphaf( v->f ); 
   } else c.fromCrayon(crayons.Pick(green255));
   Stretchi(surface,flare,c,v->f,additive,Gx,Gy,Tw,Th);

   // blue
   if ( hoverBlue ) {
    c=crayons.Pick(blue255);
    c.ScaleByAlphaf( v->f ); 
   } else c.fromCrayon(crayons.Pick(blue255));
   Stretchi(surface,flare,c,v->f,additive,Bx,By,Tw,Th);

  }

  // Solid boxes
  Stretchi(surface,box,R,1.0f,additive,Rx,Ry,Tw,Th);
  Stretchi(surface,box,G,1.0f,additive,Gx,Gy,Tw,Th);
  Stretchi(surface,box,B,1.0f,additive,Bx,By,Tw,Th);

  // Mixed
  Stretchi(surface,grid,C,1.0f,additive,Cx,Cy,Cw,Ch);

  // mixer grid
//  surface.blitTint = { 0, { 255, 255, 255 } }; 
//  Additive(surface,box.bitmap,Cx,Cy,0,0,Cw,Ch,box.bitmap.width,box.bitmap.height);  
 }

 void UpdateColor( int diff ) { 
  if ( hoverColor ) {
   C.Int( (int) iwrap(Cmx+C.r,0,255), 
          (int) iwrap(Cmy+C.g,0,255),
          (int) iwrap((Cmx+Cmy)/2+C.b,0,255), C.a );
   R.Int( C.r,0,0,255 );
   G.Int( 0,C.g,0,255 );
   B.Int( 0,0,C.b,255 ); 
  } else if ( hoverRed ) { 
   int q= (int) ( iwrap((int) C.r +  diff,0,255) );    
   R.Int(q,0,0,255);
   C.Int(q,C.g,C.b,255);
  } else if ( hoverGreen ) { 
   int q= (int) ( iwrap((int) C.g +  diff,0,255) );   
   G.Int(0,q,0,255);
   C.Int(C.r,q,C.b,255);
  } else if ( hoverBlue )  { 
   int q= (int) ( iwrap((int) C.b +  diff,0,255) );   
   B.Int(0,0,q,255); 
   C.Int(C.r,C.g,q,255);
  } 
  if ( rgba_array ) {
   rgba_array[0] = ( (float) C.r / 255.0f );
   rgba_array[1] = ( (float) C.g / 255.0f );
   rgba_array[2] = ( (float) C.b / 255.0f );
   rgba_array[3] = ( (float) C.a / 255.0f );
  }
 }

 void LeftDown() {
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  if(!moving ) {
   hoverRed  =WITHIN(mx,my,Rx,Ry,Rx2,Ry2);
   hoverGreen=WITHIN(mx,my,Gx,Gy,Gx2,Gy2);
   hoverBlue =WITHIN(mx,my,Bx,By,Bx2,By2);
   if ( hoverColor=WITHIN(mx,my,Cx,Cy,Cx2,Cy2) ) { Cmx=mx-Cx+Mw; Cmy=my-Cy+Mh; }
   Dx = mx;
   Dy = my;
   UpdateColor(1);
   WhenChanged();
//   parent->Capture();
   moving=true; up=true;
  }
 }

 void LeftUp() {
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  hoverRed  =WITHIN(mx,my,Rx,Ry,Rx2,Ry2);
  hoverGreen=WITHIN(mx,my,Gx,Gy,Gx2,Gy2);
  hoverBlue =WITHIN(mx,my,Bx,By,Bx2,By2);
  if ( hoverColor=WITHIN(mx,my,Cx,Cy,Cx2,Cy2) ) { Cmx=mx-Cx; Cmy=my-Cy; }
  if(moving) {  
//   PARENT.ReleaseCapture();
   moving = false; 
  }
 }


 void RightDown() {
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  if(!moving ) {
   hoverRed  =WITHIN(mx,my,Rx,Ry,Rx2,Ry2);
   hoverGreen=WITHIN(mx,my,Gx,Gy,Gx2,Gy2);
   hoverBlue =WITHIN(mx,my,Bx,By,Bx2,By2);
   if ( hoverColor=WITHIN(mx,my,Cx,Cy,Cx2,Cy2) ) { Cmx=mx-Cx; Cmy=my-Cy; }
   Dx = mx;
   Dy = my;
   UpdateColor(-1);
   WhenChanged();
//   PARENT.Capture();
   moving=true; up=false;
  }
 }

 void RightUp() {
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  hoverRed  =WITHIN(mx,my,Rx,Ry,Rx2,Ry2);
  hoverGreen=WITHIN(mx,my,Gx,Gy,Gx2,Gy2);
  hoverBlue =WITHIN(mx,my,Bx,By,Bx2,By2);
  if ( hoverColor=WITHIN(mx,my,Cx,Cy,Cx2,Cy2) ) { Cmx=mx-Cx; Cmy=my-Cy; }
  if(moving) {
//   PARENT.ReleaseCapture();
   moving = false;
  }
 }

 void MouseMove() {
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  if ( !moving ) {
   hoverRed  =WITHIN(mx,my,Rx,Ry,Rx2,Ry2);
   hoverGreen=WITHIN(mx,my,Gx,Gy,Gx2,Gy2);
   hoverBlue =WITHIN(mx,my,Bx,By,Bx2,By2);
   if ( hoverColor=WITHIN(mx,my,Cx,Cy,Cx2,Cy2) ) { Cmx=mx-Cx; Cmy=my-Cy; }
  } else {
   byte diff=(byte)((int)idistance(mx,my,Dx,Dy)%256);
   Dx=mx; Dy=my;
   UpdateColor( diff );
   WhenChanged();
  }
 }
};


class fx_SimpleRGBPal3f : public Proce55or {
public:
 Zpointer<GLImage> box,flare;
 Zpointer<float> rgba_array;
 Crayon R,G,B,C;
 Crayon red,green,blue,white;
 Zint Rx,Ry,Rx2,Ry2,
      Gx,Gy,Gx2,Gy2,
      Bx,By,Bx2,By2,
      Cx,Cy,Cx2,Cy2,Cw,Ch,Cmx,Cmy;
 Zint Tw,Th,Mw,Mh;
 Zint Dx,Dy, mx,my;
 Zbool initialized;
 Zbool hoverRed,hoverGreen,hoverBlue,moving,up;
 Zpointer<TexFont> font;

 fx_SimpleRGBPal3f() {
  Constructor();
  C=crayons.Pick(alabaster);
  font=microFont;
 }

 virtual void WhenChanged() { }

 void Setup()
 {
  if ( initialized ) return;
  if ( rgba_array )
  C.Int( (int) ( ((float) rgba_array[0]) * 255.0f ), (int) ( ((float) rgba_array[1]) * 255.0f ), 
         (int) ( ((float) rgba_array[2]) * 255.0f ), 255 );
  R.Int( C.r, 0, 0, 255 );
  G.Int( 0, C.g, 0, 255 );
  B.Int( 0, 0, C.b, 255 );
  
  Tw=(w-6)/3; Th=h;
  red=red; green=green; blue=blue; white=white;

  if ( !box )   box   = library.find( "data/images/elements/rounder1.png" );
  if ( !flare ) flare = library.find( "data/images/elements/rounder_box.png" );

  Rx=0;      Ry=0; 
  Gx=Tw*1+2; Gy=0;
  Bx=Tw*2+4; By=0;

  Cw=w; Ch=Th*3;
  Cx=x; Cy=y-Ch-4;
  Cx2=x+Cw; 
  Cy2=y-4;

  Rx2=Rx+Tw;  Ry2=Ry+Th;
  Gx2=Gx+Tw;  Gy2=Gy+Th;
  Bx2=Bx+Tw;  By2=By+Th;

  Rx+=x; Ry+=y;  Rx2+=x; Ry2+=y;
  Gx+=x; Gy+=y;  Gx2+=x; Gy2+=y;
  Bx+=x; By+=y;  Bx2+=x; By2+=y;

  Dx=Dy=0;

  Mw=Cw/2; Mh=Ch/2;
  initialized=true;
 }
 
 void Render() {
  if ( hidden ) return;
  Variable *v;
  Crayon c;
  
  this->controller.Updatef();
  v=this->controller.variables.Get("fade");

  if ( moving ) {
   UpdateColor( up ? 1 : -1 );
   WhenChanged();
  }

  if ( rgba_array )
   C.Int( (int) ( ((float) rgba_array[0]) * 255.0f ), (int) ( ((float) rgba_array[1]) * 255.0f ), 
          (int) ( ((float) rgba_array[2]) * 255.0f ), 255 );

  if ( v && v->tween ) {
   // red
   if ( hoverRed ) {
    c=crayons.Pick(red255);
    c.ScaleByAlphaf( v->f ); 
   } else  c.fromCrayon(crayons.Pick(red255));
   Stretchi(flare,c,v->f,additive,Rx,Ry,Tw,Th);

   // green
   if ( hoverGreen ) {
    c=crayons.Pick(green255);
    c.ScaleByAlphaf( v->f ); 
   } else c.fromCrayon(crayons.Pick(green255));
   Stretchi(flare,c,v->f,additive,Gx,Gy,Tw,Th);

   // blue
   if ( hoverBlue ) {
    c=crayons.Pick(blue255);
    c.ScaleByAlphaf( v->f ); 
   } else c.fromCrayon(crayons.Pick(blue255));
   Stretchi(flare,c,v->f,additive,Bx,By,Tw,Th);

  }

  // Solid boxes
  Stretchi(box,R,1.0f,additive,Rx,Ry,Tw,Th);
  Stretchi(box,G,1.0f,additive,Gx,Gy,Tw,Th);
  Stretchi(box,B,1.0f,additive,Bx,By,Tw,Th);
 }

 void Render(GLWindow *surface) {
  Draw(surface);
 }
 void Draw(GLWindow *surface)
 {
  if ( hidden ) return;
  Variable *v;
  Crayon c;

  this->controller.Updatef();
  v=this->controller.variables.Get("fade");

  if ( moving ) {
   UpdateColor( up ? 1 : -1 );
   WhenChanged();
  }

  if ( rgba_array )
   C.Int( (int) ( ((float) rgba_array[0]) * 255.0f ), (int) ( ((float) rgba_array[1]) * 255.0f ), 
          (int) ( ((float) rgba_array[2]) * 255.0f ), 255 );

  if ( v && v->tween ) {
   // red
   if ( hoverRed ) {
    c=crayons.Pick(red255);
    c.ScaleByAlphaf( v->f ); 
   } else  c.fromCrayon(crayons.Pick(red255));
   Stretchi(surface,flare,c,v->f,additive,Rx,Ry,Tw,Th);

   // green
   if ( hoverGreen ) {
    c=crayons.Pick(green255);
    c.ScaleByAlphaf( v->f ); 
   } else c.fromCrayon(crayons.Pick(green255));
   Stretchi(surface,flare,c,v->f,additive,Gx,Gy,Tw,Th);

   // blue
   if ( hoverBlue ) {
    c=crayons.Pick(blue255);
    c.ScaleByAlphaf( v->f ); 
   } else c.fromCrayon(crayons.Pick(blue255));
   Stretchi(surface,flare,c,v->f,additive,Bx,By,Tw,Th);

  }

  // Solid boxes
  Stretchi(surface,box,R,1.0f,additive,Rx,Ry,Tw,Th);
  Stretchi(surface,box,G,1.0f,additive,Gx,Gy,Tw,Th);
  Stretchi(surface,box,B,1.0f,additive,Bx,By,Tw,Th);
  
 }

 void UpdateColor( int diff ) { 
  if ( hoverRed ) { 
   int q= (int) ( iwrap((int) C.r +  diff,0,255) );    
   R.Int(q,0,0,255);
   C.Int(q,C.g,C.b,255);
  } else if ( hoverGreen ) { 
   int q= (int) ( iwrap((int) C.g +  diff,0,255) );   
   G.Int(0,q,0,255);
   C.Int(C.r,q,C.b,255);
  } else if ( hoverBlue )  { 
   int q= (int) ( iwrap((int) C.b +  diff,0,255) );   
   B.Int(0,0,q,255); 
   C.Int(C.r,C.g,q,255);
  } 
  if ( rgba_array ) {
   rgba_array[0] = ( (float) C.r / 255.0f );
   rgba_array[1] = ( (float) C.g / 255.0f );
   rgba_array[2] = ( (float) C.b / 255.0f );
   rgba_array[3] = ( (float) C.a / 255.0f );
  }
  WhenChanged();
 }

 void LeftDown()
 {
  if ( hidden ) return;
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  if(!moving ) {
   hoverRed  =WITHIN(mx,my,Rx,Ry,Rx2,Ry2);
   hoverGreen=WITHIN(mx,my,Gx,Gy,Gx2,Gy2);
   hoverBlue =WITHIN(mx,my,Bx,By,Bx2,By2);
   Dx = mx;
   Dy = my;
   UpdateColor(1);
//   parent->Capture();
   moving=true; up=true;
  }
 }

 void LeftUp()
 {
  if ( hidden ) return;
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  hoverRed  =WITHIN(mx,my,Rx,Ry,Rx2,Ry2);
  hoverGreen=WITHIN(mx,my,Gx,Gy,Gx2,Gy2);
  hoverBlue =WITHIN(mx,my,Bx,By,Bx2,By2);
  if(moving) {  
//   PARENT.ReleaseCapture();
   moving = false; 
  }
 }


 void RightDown()
 {
  if ( hidden ) return;
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  if(!moving ) {
   hoverRed  =WITHIN(mx,my,Rx,Ry,Rx2,Ry2);
   hoverGreen=WITHIN(mx,my,Gx,Gy,Gx2,Gy2);
   hoverBlue =WITHIN(mx,my,Bx,By,Bx2,By2);
   Dx = mx;
   Dy = my;
   UpdateColor(-1);
//   PARENT.Capture();
   moving=true; up=false;
  }
 }

 void RightUp()
 {
  if ( hidden ) return;
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  hoverRed  =WITHIN(mx,my,Rx,Ry,Rx2,Ry2);
  hoverGreen=WITHIN(mx,my,Gx,Gy,Gx2,Gy2);
  hoverBlue =WITHIN(mx,my,Bx,By,Bx2,By2);
  if(moving) {
//   PARENT.ReleaseCapture();
   moving = false;
  }
 }

 void MouseMove()
 {
  if ( hidden ) return;
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  if ( !moving ) {
   hoverRed  =WITHIN(mx,my,Rx,Ry,Rx2,Ry2);
   hoverGreen=WITHIN(mx,my,Gx,Gy,Gx2,Gy2);
   hoverBlue =WITHIN(mx,my,Bx,By,Bx2,By2);
  } else {
   byte diff=(byte)((int)idistance(mx,my,Dx,Dy)%256);
   Dx=mx; Dy=my;
   UpdateColor( diff );
  }
 }
};

#include "UglyFont.h"
#include "Cartesian.h"
#include "Art.h"

class fx_UglyRGBPal : public Proce55or {
public:
 Cartesian labelPosition,rPos,gPos,bPos,cPos;
 Zstring label;
 Zp<Crayon> binding;
 Zfloat minimum;
 Zfloat delta;
 Zbool hoverC,hoverR,hoverG,hoverB;
 Zint w4;
 fx_UglyRGBPal() : Proce55or() {
  Reset();
 }
 fx_UglyRGBPal( int x, int y, Crayon *b, string label ) : Proce55or() {
  Bind(b);
  Label(label);
  this->x=x;
  this->y=y;
  Reset();
 }
 void Reset() {
  w=64+32+64;
  h=w/5;
  w4=(w-h)/4;
  x2=w+x;
  y2=h+y;
  labelPosition.SetRect(x.value,y.value-8,6,8);
  cPos.Corners(x,y,x+w4,y2);
  rPos.Corners(x+w4,y+4,x+w4+w4-4,y2-4);
  gPos.Corners(x+w4+w4,y+4,x+w4+w4+w4-4,y2-4);
  bPos.Corners(x+w4+w4+w4,y+4,x+w4+w4+w4+w4-4,y2-4);
  hoverR=hoverG=hoverB=hoverC=false;
  minimum=0.0f;
  Floor();
  delta=1.0f/100.0f;
 }
 void OnSetExtents() {
  h=w/5;
  x2=w+x;
  y2=h+y;
  w4=(w-w/5)/4;
  labelPosition.SetRect(x.value,y.value-8,6,8);
  cPos.Corners(x,y,x+w4,y2);
  rPos.Corners(x+w4,y+4,x+w4+w4-4,y2-4);
  gPos.Corners(x+w4+w4,y+4,x+w4+w4+w4-4,y2-4);
  bPos.Corners(x+w4+w4+w4,y+4,x+w4+w4+w4+w4-4,y2-4);
 }
 void Floor() {
  if ( !binding ) return;
  if ( binding->rf < minimum ) binding->Float(minimum,binding->gf,binding->bf,1.0f );
  if ( binding->gf < minimum ) binding->Float(binding->rf,minimum,binding->bf,1.0f );
  if ( binding->bf < minimum ) binding->Float(binding->rf,binding->gf,minimum,1.0f );
 }
 void RefreshColor() {
  binding->Float(binding->rf,binding->gf,binding->bf,1.0f);
 }
 void Bind( Crayon *b ) {
  binding=b;
 }
 void Label( string L ) {
  this->label=L;
 }
 void Render() {
  Blending(none);
  Area(*binding,&cPos);
  if ( hoverC ) Rectangle( crayons.Pick(alabaster), &cPos );
  Area(crayons.jam(0,binding->rf,0.0f,0.0f,1.0f),&rPos);
  if ( hoverR ) Rectangle( crayons.Pick(alabaster), &rPos );
  Area(crayons.jam(0,0.0f,binding->gf,0.0f,1.0f),&gPos);
  if ( hoverG ) Rectangle( crayons.Pick(alabaster), &gPos );
  Area(crayons.jam(0,0.0f,0.0f,binding->bf,1.0f),&bPos);
  if ( hoverB ) Rectangle( crayons.Pick(alabaster), &bPos );
  glColor3d(1.0,1.0,1.0);
  Text(label,labelPosition.x,labelPosition.y,labelPosition.w,labelPosition.h,false);
 }
 void Render(GLWindow *surface) { Render(); }
 void Between() {
  if ( !binding ) return;
  hoverC= WITHIN(input.mxi,input.myi,cPos.x,cPos.y,cPos.x2,cPos.y2);
  hoverR= WITHIN(input.mxi,input.myi,rPos.x,rPos.y,rPos.x2,rPos.y2);
  hoverG= WITHIN(input.mxi,input.myi,gPos.x,gPos.y,gPos.x2,gPos.y2);
  hoverB= WITHIN(input.mxi,input.myi,bPos.x,bPos.y,bPos.x2,bPos.y2);
  if ( input.left ) {
   if ( hoverC ) {
    binding->Any();
    Floor();
   }
   if ( hoverR ) {
    binding->rf+=delta;
    if ( binding->rf > 1.0f ) binding->rf=minimum;
    RefreshColor();
   }
   if ( hoverG ) {
    binding->gf+=delta;
    if ( binding->gf > 1.0f ) binding->gf=minimum;
    RefreshColor();
   }
   if ( hoverB ) {
    binding->bf+=delta;
    if ( binding->bf > 1.0f ) binding->bf=minimum;
    RefreshColor();
   }
  }
 }
 void ShowHelpInfo();
 void Between( Cartesiand *mouseScreen ) {
  if ( !binding ) return;
  hoverR= WITHIN((int)mouseScreen->x,(int)mouseScreen->y,rPos.x,rPos.y,rPos.x2,rPos.y2);
  hoverG= WITHIN((int)mouseScreen->x,(int)mouseScreen->y,gPos.x,gPos.y,gPos.x2,gPos.y2);
  hoverB= WITHIN((int)mouseScreen->x,(int)mouseScreen->y,bPos.x,bPos.y,bPos.x2,bPos.y2);
  hoverC= WITHIN((int)mouseScreen->x,(int)mouseScreen->y,cPos.x,cPos.y,cPos.x2,cPos.y2);
  if ( ( input.KeyDown(DX_LCONTROL) || input.KeyDown(DX_RCONTROL) )
    && ( hoverR || hoverG || hoverB || hoverC ) ) {
   ShowHelpInfo();
  }
  if ( input.left ) {
   if ( hoverC ) {
    binding->Any();
    Floor();
    RefreshColor();
   }
   if ( hoverR ) {
    binding->rf+=delta;
    if ( binding->rf > 1.0f ) binding->rf=1.0f;
    RefreshColor();
   }
   if ( hoverG ) {
    binding->gf+=delta;
    if ( binding->gf > 1.0f ) binding->gf=1.0f;
    RefreshColor();
   }
   if ( hoverB ) {
    binding->bf+=delta;
    if ( binding->bf > 1.0f ) binding->bf=1.0f;
    RefreshColor();
   }
  } else if ( input.right ) {
   if ( hoverC ) {
    binding->Pick(black);
    Floor();
   }
   if ( hoverR ) {
    binding->rf-=delta;
    if ( binding->rf < minimum ) binding->rf=minimum;
    RefreshColor();
   }
   if ( hoverG ) {
    binding->gf-=delta;
    if ( binding->gf < minimum ) binding->gf=minimum;
    RefreshColor();
   }
   if ( hoverB ) {
    binding->bf-=delta;
    if ( binding->bf < minimum ) binding->bf=minimum;
    RefreshColor();
   }
  } else if ( input.middle ) {
   if ( hoverC ) {
    binding->Pick(alabaster);
    RefreshColor();
   }   
   if ( hoverR ) {
    binding->rf=1.0f;
    RefreshColor();
   }
   if ( hoverG ) {
    binding->gf=1.0f;
    RefreshColor();
   }
   if ( hoverB ) {
    binding->bf=1.0f;
    RefreshColor();
   }
  } else if ( input.wheelUp ) {
   if ( hoverC ) {
    binding->rf+=delta;
    if ( binding->rf > 1.0f ) binding->rf=1.0f;
    binding->gf+=delta;
    if ( binding->gf > 1.0f ) binding->gf=1.0f;
    binding->bf+=delta;
    if ( binding->bf > 1.0f ) binding->bf=1.0f;
    RefreshColor();
   }  
   if ( hoverR ) {
    binding->rf+=delta;
    if ( binding->rf > 1.0f ) binding->rf=1.0f;
    RefreshColor();
   }
   if ( hoverG ) {
    binding->gf+=delta;
    if ( binding->gf > 1.0f ) binding->gf=1.0f;
    RefreshColor();
   }
   if ( hoverB ) {
    binding->bf+=delta;
    if ( binding->bf > 1.0f ) binding->bf=1.0f;
    RefreshColor();
   } 
  } else if ( input.wheelDown ) {
   if ( hoverC ) {
    binding->rf-=delta;
    if ( binding->rf < minimum ) binding->rf=minimum;
    binding->gf-=delta;
    if ( binding->gf < minimum ) binding->gf=minimum;
    binding->bf-=delta;
    if ( binding->bf < minimum ) binding->bf=minimum;
    RefreshColor();
   }  
   if ( hoverR ) {
    binding->rf-=delta;
    if ( binding->rf < minimum ) binding->rf=minimum;
    RefreshColor();
   }
   if ( hoverG ) {
    binding->gf-=delta;
    if ( binding->gf < minimum ) binding->gf=minimum;
    RefreshColor();
   }
   if ( hoverB ) {
    binding->bf-=delta;
    if ( binding->bf < minimum ) binding->bf=minimum;
    RefreshColor();
   } 
  }
 }
};