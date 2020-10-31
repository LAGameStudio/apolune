/***********************************************************************************************
 * ____ _  _ ___  _ ____ ____    _ _  _    ___ _  _ ____    ____ _  _ _   _ (c)2009-2011       *
 * |___ |\/| |__] | |__/ |___    | |\ |     |  |__| |___    [__  |_/   \_/  Lost Astronaut     *
 * |___ |  | |    | |  \ |___    | | \|     |  |  | |___    ___] | \_   |   Studios + Ganos.biz*
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#pragma once

#include <Windows.h>
#include <math.h>
#include "moremath.h"

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "Strings.h"
#include "GLWindow.h"
#include "GLImage.h"

#include "Art.h"
#include "VisualAid.h"
#include "Proce55Viewer2d.h"
#include "fx_TexFont.h"
#include "fx_Glow.h"

#include "Universe.h"

#include "SystemViewer.h"

extern GLWindow *quadrantViewers[SECTORS];

char *system_catalog_name( SolarSystem *s );

extern TexFont bytes;
extern TexFont microFont;
extern GLWindowManager windows;

class StarMap;

extern char *quadrantsX[10];
extern char *quadrantsY[10];

extern char *starmap_flares[16];

class QuadrantViewerScreen : public Presentation {
public:
 void OnLoad() {
  visible=false;
  background.Int(0,0,0,127);
  Anchor(0.0f,0.0f,0.0f,0.0f);
 }
};

class QuadrantViewer : public Proce55Viewer2d {
public:
 GLImage *crosshair, *indicator;
 bool dragging;
 int MovedX,MovedY,lastx,lasty,mx,my,cx,cy,maxx,minx,maxy,miny;
 Sector *sector;
 GLWindow *map;
 SolarSystem *nearest;
 SystemViewers systemViewers;
 int Tx,Ty, // Total size of the presentation, as the presentation extends beyond the screen.
     Sw,Sh; // Size of a single quadrant
 Crayon c;
 
 void ResetPositions() {
  PanBy(-MovedX,-MovedY);
//  MovedX=MovedY=0;
 }

 void PanBy( int x, int y ) {
  if ( x+MovedX > maxx ) x=0;
  if ( x+MovedX < minx ) x=0;
  if ( y+MovedY > maxy ) y=0;
  if ( y+MovedY < miny ) y=0;

  MoveBy(x,y);
  MovedX+=x;
  MovedY+=y;
  
  SystemViewerHandle *p;
  int limx=this->x-512,limy=this->y-(512+40);
  for ( p=(SystemViewerHandle *)systemViewers.first; p; p=(SystemViewerHandle *) (p->next) ) {
   p->a->MoveBy(x,y);
   p->a->title->MoveBy(x,y);
   p->a->caption->MoveBy(x,y);
   // Turn off SystemViewers that are not visible on the screen, turn on ones that are.
   p->a->visible=
    !( p->a->x < limx || p->a->y < limy || p->a->y > this->y2 || p->a->x > this->x2 );
  }
 }

 void PrintDebugInfo() {
  Proce55or *p=(Proce55or *) this->fx.first;
  Strings s;
  s.printf( "Moved x,y: %d,%d   Sw,Sh: %d,%d Tx,Ty: %d,%d", MovedX, MovedY, Sw,Sh, Tx,Ty );
  s.printf( "Mx,y: %d,%d MinX,MinY:  %d,%d  MaxX,MaxY: %d,%d", mx,my,minx,miny,maxx,maxy );
  s.printf( "Cx,y: %d,%d", cx,cy );
  s.printf( "x,y: %d,%d w/h: %d/%d", x,y,w,h );
  s.printf( "Px,y: %d,%d", p->x, p->y );
  string content=s.Concatenate('\n');
  WriteTexFont(this,&microFont,crayons.Pick(green),additive,x,y,7,16,5,(char *) content.c_str());
 }

 void OnLoad() {
  int star,i,j;
  Crayon c1;
  SolarSystem *b;
  Star *c;
  VisualAid2 *va;
  fx_TexFont *text;
  char buf[256];

  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Int(0,0,64,255);
  crosshair = library.find("data/images/elements/crosshair1.png");
  indicator = library.find("data/images/elements/rounded_1024.png");
  dragging=false;
  MovedX=MovedY=mx=my=cx=cy=maxx=minx=maxy=miny=0;
  nearest=null;

  Sw=w;
  Sh=h;
  Tx=10*Sw;
  Ty=10*Sh;

  visible=false;

  minx = -((int) Tx+256);
  miny = -((int) Ty+256);
  maxx = 256;
  maxy = 256;

  duration=10.0f;

  c1.Pick(blue);
  va=new VisualAid2; // background (top)
  va->image=library.find( "data/images/encyclopedia/star-fields-2x.png");
  va->Tint( c1 );
  //va->Opacity(0);
  va->duration=10.0f;
  va->sx=0;
  va->sy=-Ty-10;
  va->w=va->w2=Tx;
  va->h=va->h2=Ty;
  va->in=1.0f;
  va->out=10.0f;
  Add(va);

  c1.Pick(blue);
  va=new VisualAid2; // background (left, top)
  va->image=library.find( "data/images/encyclopedia/star-fields-2x.png");
  va->Tint( c1 );
  //va->Opacity(0);
  va->duration=10.0f;
  va->sx=-Tx-1;
  va->sy=-Ty-10;
  va->w=va->w2=Tx;
  va->h=va->h2=Ty;
  va->in=1.0f;
  va->out=10.0f;
  Add(va);

  c1.Pick(blue);
  va=new VisualAid2; // background (right, top)
  va->image=library.find( "data/images/encyclopedia/star-fields-2x.png");
  va->Tint( c1 );
  //va->Opacity(0);
  va->duration=10.0f;
  va->sx=Tx;
  va->sy=-Ty-10;
  va->w=va->w2=Tx;
  va->h=va->h2=Ty;
  va->in=1.0f;
  va->out=10.0f;
  Add(va);

  c1.Pick(blue);
  va=new VisualAid2; // background (bottom)
  va->image=library.find( "data/images/encyclopedia/star-fields-2x.png");
  va->Tint( c1 );
  //va->Opacity(0);
  va->duration=10.0f;
  va->sx=0;
  va->sy=Ty+10+1;
  va->w=va->w2=Tx;
  va->h=va->h2=Ty;
  va->in=1.0f;
  va->out=10.0f;
  Add(va);

  c1.Pick(blue);
  va=new VisualAid2; // background (bottom, left)
  va->image=library.find( "data/images/encyclopedia/star-fields-2x.png");
  va->Tint( c1 );
  //va->Opacity(0);
  va->duration=10.0f;
  va->sx=-Tx;
  va->sy=Ty+10+1;
  va->w=va->w2=Tx;
  va->h=va->h2=Ty;
  va->in=1.0f;
  va->out=10.0f;
  Add(va);

  c1.Pick(blue);
  va=new VisualAid2; // background (bottom, right)
  va->image=library.find( "data/images/encyclopedia/star-fields-2x.png");
  va->Tint( c1 );
  //va->Opacity(0);
  va->duration=10.0f;
  va->sx=Tx;
  va->sy=Ty+10+1;
  va->w=va->w2=Tx;
  va->h=va->h2=Ty;
  va->in=1.0f;
  va->out=10.0f;
  Add(va);

  c1.Pick(blue);
  va=new VisualAid2; // background (left)
  va->image=library.find( "data/images/encyclopedia/star-fields-2x.png");
  va->Tint( c1 );
  //va->Opacity(0);
  va->duration=10.0f;
  va->sx=-Tx-10;
  va->sy=-10;
  va->w=va->w2=Tx;
  va->h=va->h2=Ty+20;
  va->in=1.0f;
  va->out=10.0f;
  Add(va);


  c1.Pick(blue);
  va=new VisualAid2; // background (right)
  va->image=library.find( "data/images/encyclopedia/star-fields-2x.png");
  va->Tint( c1 );
  //va->Opacity(0);
  va->duration=10.0f;
  va->sx=Tx+10;
  va->sy=-10;
  va->w=va->w2=Tx;
  va->h=va->h2=Ty+20;
  va->in=1.0f;
  va->out=10.0f;
  Add(va);


  // Place quadrant grid backdrop
  for ( i=0; i<10; i++ ) for ( j=0; j<10; j++ ) {
   c1.Int( 0,0,127,127 );
   va=new VisualAid2; // center
   va->image=library.find( "data/images/elements/white.png");
   va->Tint( c1 );
   //va->Opacity(0);
   va->duration=10.0f;
   va->sx=Sw*i+10-5;
   va->sy=Sh*j+10-5;
   va->w=va->w2=Sw-20;
   va->h=va->h2=Sh-20;
   va->in=1.0f;
   va->out=10.0f;
   Add(va);
   
   text= new fx_TexFont;
   FORMAT( buf,256, "%s%s", quadrantsX[i], quadrantsY[j] );
   text->font=&bytes;
   text->duration=10.0f;
   text->fw=200;
   text->fh=200;
   text->color.Int( 0, 32, 127, 96 );
   text->x=Sw*i+40;
   text->y=Sh*j+20;
   text->Text(buf,this);
   Add(text);
  }

  GLImage *circ=library.find("data/images/elements/filledcircle.png");

  // Draw Quadrants
  for ( b=(SolarSystem *) (sector->systems.first); b; b=(SolarSystem *)(b->next) ) {
   int sx,sy,planets,asteroidsd2;
   star=0;
   planets=0;
   sx=(int)(b->x*((float) Tx));
   sy=(int)(b->y*((float) Ty));
   planets=b->planets.count;
   asteroidsd2=(planets*10)/2;
   if ( asteroidsd2 < 5 ) asteroidsd2=5;
   if ( b->stars.count == 0 ) { // 0 stars
    va = new VisualAid2;
    va->sx=sx-asteroidsd2;
    va->sy=sy-asteroidsd2+20;
    va->image=b->nebulaImage;
    va->Tint(b->tint);
    va->duration=10.0f;
    va->width=va->w=va->w2=asteroidsd2*2;
    va->height=va->h=va->h2=asteroidsd2*2;
    va->out=10.0f;
    Add(va);
   } else for ( c=(Star *) (b->stars.first); c; c=(Star *)(c->next) ) { 

    if ( b->stars.count > 1 ) { // Mark a multi-star system with a faint circle
    va = new VisualAid2;
    va->sx=sx-asteroidsd2*3;
    va->sy=sy-asteroidsd2*3+20;
    va->image=circ;
    va->Tint(crayons.jam(0,0.1f,0.1f,0.1f,0.1f));
    va->duration=10.0f;
    va->width=va->w=va->w2=asteroidsd2*6;
    va->height=va->h=va->h2=asteroidsd2*6;
    va->out=10.0f;
    Add(va);   
    }

    fx_Glow2 *glow,*glow2;
    int bdbdbd=(int) (c->radius*(w/20));
    int bdbdbd2=(int) (c->radius*(h/20));
    int bd3=bdbdbd/2;
    int bd4=bdbdbd2/2;
    int bd5=(int) (c->fx * 5.0f * (float) bd3 );
    int bd6=(int) (c->fy * 5.0f * (float) bd4 );
    float perc3=(float) abs(sin(1.0f/(float) c->star_catalog_no));
    glow=new fx_Glow2;
    glow->x=(int) (sx-bd3+bd5);
    glow->y=(int) (sy-bd4+bd6);
    glow->flare= anomaly_images.image((int)c->type);
    if ( glow->flare == NULL ) OUTPUT( "Error!  Quadrantviewer on system %d could not find %s (type was %d)\n", (int) b->system_catalog_no, anomaly_images.filename((int)c->type), (int) c->type );
    glow->tint=c->corona;
    glow->w=(bdbdbd);
    glow->w2=(bdbdbd);
    glow->h=(bdbdbd);
    glow->h2=(bdbdbd);
               //   delay=float_range(0.0f,1.0f);
    glow->duration=10.0f;
    glow->maxOp=127.0f;
    glow->controller.instant= (float) uniform();
    glow->controller.period= (float) (10.0 * ( (double) (c->fx+c->fy+c->fz)/3.0) );
    glow->controller.variables.NewfT("Z",127.0f,255.0f,float_range(0.25f,1.0f),tweens.find("Humanized Triples"));
    Add(glow);
    
    glow2=new fx_Glow2;
    glow2->x=glow->x;
    glow2->y=glow->y;
    glow2->flare= library.find( "data/images/flares/flare.png");
    glow2->tint=c->photosphere;
    glow2->w=(bdbdbd);
    glow2->w2=(bdbdbd);
    glow2->h=(bdbdbd);
    glow2->h2=(bdbdbd);
              //    delay=s.fx.delay;
    glow2->duration=10.0f;
    glow2->maxOp=127.0f;
    glow2->controller.instant= (float) uniform();
    glow2->controller.period= (c->period*2.0f);
    glow2->controller.variables.NewfT("Z",127.0f,255.0f,float_range(0.25f,1.0f),tweens.find("Humanized Interspliced Triples"));
    Add(glow2);
    
    if ( c->period2 != 1.0f ) {
     glow2=new fx_Glow2;
     glow2->x=glow->x+bdbdbd/6;
     glow2->y=glow->y+bdbdbd/6;
     glow2->flare= anomaly_images.image((int)c->type);
     if ( glow2->flare == NULL ) OUTPUT( "Error!  Quadrantviewer on system %d could not find %s (type was %d)\n", (int) b->system_catalog_no, anomaly_images.filename((int) c->type), (int)c->type );
     glow2->tint=c->photosphere;
     glow2->w=(bdbdbd/2);
     glow2->w2=(bdbdbd/2);
     glow2->h=(bdbdbd/2);
     glow2->h2=(bdbdbd/2);
              //    delay=s.fx.delay;
     glow2->duration=10.0f;
     glow2->maxOp=127.0f;
     glow2->controller.instant= (float) uniform();
     glow2->controller.period= (float) (c->period2);
     glow2->controller.variables.NewfT("Z",127.0f,255.0f,float_range(0.25f,1.0f),tweens.random());
     Add(glow2);
    }
    
    if ( c->period3 != 1.0f ) {
     glow2=new fx_Glow2;
     glow2->x=glow->x;
     glow2->y=glow->y;
     glow2->flare= anomaly_images.image((int)c->type);
     if ( glow2->flare == NULL ) OUTPUT( "Error!  Quadrantviewer on system %d could not find %s (type was %d)\n", (int) b->system_catalog_no, anomaly_images.filename((int)c->type), (int)c->type );
     glow2->tint=c->corona;
     glow2->w=(bdbdbd);
     glow2->w2=(bdbdbd);
     glow2->h=(bdbdbd);
     glow2->h2=(bdbdbd);
              //    delay=s.fx.delay;
     glow2->duration=10.0f;
     glow2->maxOp=127.0f;
     glow2->controller.instant= (float) uniform();
     glow2->controller.period= (float) (c->period3);
     glow2->controller.variables.NewfT("Z",127.0f,255.0f,float_range(0.25f,1.0f),tweens.random());
     Add(glow2);
    }
    star++;
   }
  }

  Init();
 }

 void RightDown() {
  visible=false;
  map->visible=true;
  windows.send=(GLWindow *) map;
 }
 void LeftDown();
 void LeftUp() {
  dragging=false;
 }

 void MouseMove();

 void PostProcessing() {
  char buf[64];
  int ylimit = display->h-20;
  c.Int( 125, 167, 217, 255 );
  FORMAT(buf,64, "Sector %s", sector->number );
  WriteTexFont(this,&microFont,crayons.Pick(alabaster),additive,30,ylimit+3,7,16,2,buf);

  if ( nearest ) {
   Crayon C;
   int tx,ty,sx,sy,dx=0,dy=0;
   c.Int( 255, 0, 0, 127 );
   tx=(int)(nearest->x*Tx-25);
   ty=(int)(nearest->y*Ty-15);
   sx=tx+MovedX;
   sy=ty+MovedY;
   bool viewer=false;
   EACH(systemViewers.first,SystemViewerHandle,sh) if ( sh->a->s==nearest ) {
    viewer=true;
    break;
   }
   if ( viewer ) 
    StretchiNoAlpha(this,crosshair,c,additive,tx+MovedX,ty+MovedY,50,50);
   else
    StretchiNoAlpha(this,indicator,c,additive,tx+MovedX,ty+MovedY,50,50);
   if ( nearest->stars.count > 1 ) {
    int ox=(int) (nearest->x*Tx)+MovedX;
    int oy=(int) (nearest->y*Ty)+MovedY+10;
    Blending(additive);
    for ( Star *s=(Star *) (nearest->stars.first); s; s=(Star *)(s->next) ) {
     int ox2=ox+(int) (s->fx * 5.0f * (s->radius*(w/20)/2) );
     int oy2=oy+(int) (s->fy * 5.0f * (s->radius*(h/20)/2) );
     DrawLine2Color( this, c, crayons.Pick(clear), ox, oy, ox2, oy2+1 );
     DrawLine2Color( this, c, crayons.Pick(clear), ox, oy, ox2, oy2-1 );
     DrawLine2Color( this, c, crayons.Pick(clear), ox, oy, ox2, oy2 );
     DrawLine2Color( this, c, crayons.Pick(clear), ox, oy, ox2+1, oy2 );
     DrawLine2Color( this, c, crayons.Pick(clear), ox, oy, ox2-1, oy2 );
    }
    Blending(none);
   }                
   FORMAT( buf,64, "%s\n", system_catalog_name(nearest) );
   WriteTexFont(this,&microFont,crayons.Pick(red255),additive,sx,sy+50,7,16,2,buf);
   if ( nearest->stars.count == 0 ) {
    String *neb=nebula_type_strings.find((int)nearest->nebula);
    WriteTexFont(this,&microFont,crayons.Pick(red255),additive,sx,sy+50+16,7,16,2,neb->c_str());
   }
   else {
    FORMAT( buf,64, "%d star%c", (int)nearest->stars.count, nearest->stars.count != 1 ? 's' : ' ' );
    WriteTexFont(this,&microFont,crayons.Pick(red255),additive,sx,sy+50+16,7,16,2,buf);
   }
   for ( Star *c= (Star *) nearest->stars.first; c; c=(Star *) (c->next) ) {
   switch ( c->MKspectral ) {
    case 'O': C.Pick(cadetBlue); break;
    case 'B': C.Pick(deepSkyBlue); break;
    case 'A': C.Pick(steelBlue); break;
    case 'F': C.Pick(slateBlue); break;
    case 'G': C.Pick(yellow); break;
    case 'K': C.Pick(orange); break;
    case 'M': C.Pick(bloodRed); break;
    case 'W': C.Pick(skyBlue); break;
    case 'L': C.Pick(rosyBrown); break;
    case 'T': C.Pick(violet); break;
    case 'Y': C.Pick(brown); break;
    case 'C': C.Pick(darkRed); break;
    case 'S': C.Pick(chartreuse); break;
    case 'D': C.Pick(lightSkyBlue); break;
    case 'P': C.Pick(beige); break;
    case 'Q': C.Pick(bisque); break;
    case 'R': C.Pick(orangeRed); break;
    case 'H': C.Pick(fireBrick); break;
   }
//  sprintf(buf, "%c%s", c.MKspectral, (c.next ? "," : "") );
   FORMAT(buf,64, "%c", c->MKspectral );
   WriteTexFont(this,&microFont,C,additive,sx+dx,sy+50+16+16+dy,7,16,2,buf);
   dx+=15;//   dy+=15;
  }
  }
  //PrintDebugInfo();
 }

 void ToggleSolarSystem( SolarSystem *b );

};