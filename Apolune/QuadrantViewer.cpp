#include "QuadrantViewer.h"

GLWindow *quadrantViewers[SECTORS];

extern Audio audio;

void QuadrantViewer::MouseMove() {
 mx=input->mxi-this->x;
 my=input->myi-this->y;
 if ( dragging ) {
  int diffx=mx-lastx;
  int diffy=my-lasty;
  PanBy(diffx,diffy);
  lastx=mx;
  lasty=my;   
 // OUTPUT( "%d,%d .. LastX,LastY: %d,%d MaxX,Y: %d,%d MinX,Y: %d,%d\n", MovedX,MovedY,diffx,diffy,maxx,maxy,minx,miny );
 }

 SolarSystem *old_nearest=nearest;
 SolarSystem *b;  
 float prev_nearest = 100.0f;
 for ( b=(SolarSystem *) sector->systems.first; b!=null; b=(SolarSystem *) (b->next) ) {
  float sx,sy,d;
  sx=b->x*Tx;
  sy=b->y*Ty;
  if ( (d=fdistance(sx,sy,(float) (mx-MovedX), (float) (my-MovedY))) < prev_nearest ) {
   nearest=b;
   prev_nearest=d;
  }
 }
 if ( nearest!=old_nearest && nearest ) audio.manager.CueAndPlayIfNotPlaying("data/sounds/UI/bloop.wav",2.0f,0.25f,false,true);
 if ( prev_nearest > 75.0f ) nearest=null;
}

void QuadrantViewer::ToggleSolarSystem( SolarSystem *b ) {
   Crayon c1,c2;
   Slide *s,*s2;
   VisualAid *va,*ba,*aa;
   fx_Circle2d *path;
   char buf[128];

   if ( !b ) { return; }
   if ( systemViewers.inList(b) ) {
   } else {
    if (systemViewers.count > 20) {
     SystemViewerHandle *s=(SystemViewerHandle *) systemViewers.first;
     systemViewers.Drop(s->a);
    }
    int bx=(int)(b->x*Tx), by=(int)(b->y*Ty);
    SystemViewer *systemViewer=new SystemViewer;
    systemViewer->stayOnTop=true;
    systemViewer->quadrantViewer=this;
    systemViewers.Add(systemViewer);
    this->AddChild(systemViewer);
    systemViewer->SetPositionAndSize(bx+MovedX-256,by+MovedY-((512+40)/2),512,512+40);

    Planet *X;   
    systemViewer->s=b;
    nearest=null;

    s=new Slide( 1.0f );
    systemViewer->Add(s);

    s2=new Slide( 600.0f );
    systemViewer->Add(s2);
    s2->seek=1;

    c1.Int(27, 27, 255, 255); 

    /* Moved
    CoursePlotButton *cpb=new CoursePlotButton;
    cpb->system=b;
    cpb->duration=600.0f;
    cpb->x=systemViewer->w-48;
    cpb->y=systemViewer->h-48;
    s2->fx->Add(cpb,systemViewer);
    */

    va = new VisualAid;
    va->image=library.find( "data/images/elements/tab.png");
    va->sx=0+256;
    va->sy=18+50;
    va->dx=-256+70; //va.dy=0;
    va->dy=-18;
    va->w=0;
    va->w2=512-160;
    va->h=0;
    va->h2=36;
    va->Opacity(0);
    va->Tints( c1, c1, c1 );
    va->duration=0.5f;
    va->delay=0.5f;
    va->in=0.745f;
    va->out=1.0f;
    s->fx->Add(va,systemViewer);

    va = new VisualAid;
    va->image=library.find( "data/images/elements/tab.png");
    va->sx=0+70;
    va->sy=0+50;
    va->w=va->w2=512-140;
    va->h=va->h2=36;
    va->Opacity(0);
    va->Tints( c1, c1, c1 );
    va->duration=600.0f;
    va->delay=0.0f;
    va->out=600.0f;
    s2->fx->Add(va,systemViewer);
        
    systemViewer->title=new fx_UglyTextBox;
    systemViewer->title->duration=600.0f;
    systemViewer->title->delay=0.2f;
    systemViewer->title->x=systemViewer->x+(int)(512/2);
    systemViewer->title->y=systemViewer->y+(int)(2)+70;
    systemViewer->title->bound=&b->name;
    systemViewer->title->SetValue(b->name);
    s2->fx->Add(systemViewer->title,systemViewer);


    systemViewer->caption=new fx_TexFont;
    systemViewer->caption->font=&microFont;
    systemViewer->caption->color.Pick(hotPink);
    systemViewer->caption->fw=7;
    systemViewer->caption->fh=16;
    FORMAT( buf,128, "%d planet%c", (int) b->planets.count, b->planets.count != 1 ? 's' : ' ' );
    systemViewer->caption->duration=600.0f;
    systemViewer->caption->delay=0.2f;
    systemViewer->caption->x=systemViewer->x+(int)(40);
    systemViewer->caption->y=systemViewer->y+(int)(512-40);
    systemViewer->caption->Text(buf,systemViewer);
    s2->fx->Add(systemViewer->caption,systemViewer);

    c1.Int( 27, 27, 96, 255 );

    va = new VisualAid;
    va->image=library.find( "data/images/elements/rounded_512.png");
    va->sx=(0)+256;
    va->sy=(0)+256+40;
    va->dx=-256;
    va->dy=-256;
    va->w=0;
    va->w2=512;
    va->h=0;
    va->h2=512;
    va->Tints( crayons.Pick(clear), c1, c1 );
    va->duration=0.5f;
    va->in=0.245f;
    va->out=1.0f;
    s->fx->Add(va,systemViewer);

    aa = new VisualAid;
    aa->image=library.find( "data/images/elements/rounded_512.png");
    aa->sx=(0);
    aa->sy=(0)+40;
    aa->w=aa->w2=512;
    aa->h=aa->h2=512;
    aa->Tints( c1, c1, c1 );
    aa->duration=1.0f;
    aa->delay=0.5f;
    aa->in=0.0f;
    aa->out=600.0f;
    s->fx->Add(aa,systemViewer);
    
    aa = new VisualAid;
    aa->image=library.find( "data/images/elements/rounded_512.png");
    aa->sx=(0);
    aa->sy=(0)+40;
    aa->w=aa->w2=512;
    aa->h=aa->h2=512;
    aa->Tints( c1, c1, c1 );
    aa->duration=600.0f;
    aa->delay=0.0f;
    aa->in=0.0f;
    aa->out=600.0f;
    s2->fx->Add(aa,systemViewer);

    int cx=aa->sx+aa->w/2;
    int cy=aa->sy+aa->h/2;
    int aw2=256;
    int ar=200;
    float arf=200.0f;

    float farthest=0.0f;
    for ( X=(Planet *) (b->planets.first); X; X=(Planet *) (X->next) ) {
     float fd=fdistance(0.0f,0.0f,X->x,X->y);
     if ( fd > farthest ) farthest=fd;
    }

    for ( X=(Planet *) (b->planets.first); X; X=(Planet *) (X->next) ) {
     float fd2=fdistance(0.0f,0.0f,X->x,X->y);
     float orbitRatio=(float)fd2/(float) farthest;
     int bw,bh,vx,vy;
     float pa,ra2;

     // Planetary width
     bw=URANGE(20,(int)(40.0f*X->radius),50);
     bh=URANGE(20,(int)(40.0f*X->radius),50);
     // Planetary radius
//     ra2 =  orbitRatio*ra2;
     ra2=orbitRatio*arf;
     // Location of planet
     pa=(float) (uniform()*TWO_PI);
     vx=-bw/2 + cx + ((int) ( ra2 * ( (float) cos(pa) ) ) );
     vy=-bh/2 + cy + ((int) ( ra2 * ( (float) sin(pa) ) ) );
     // Radius between center point (px,y) and 

     // Fade-In

     path=new fx_Circle2d;
     path->radius=ra2;
     path->segments=36;
     path->x=cx;
     path->y=cy;
     path->h=path->w=(int) (path->radius*2);
     path->tint.Pick(red255);
     path->tint.ScaleByAlphaf(0.5f);
     path->duration=1.0f;
     path->delay=0.5f * orbitRatio;
     s->fx->Add(path,systemViewer);

     // Persistent

     path=new fx_Circle2d;
     path->radius=ra2;
     path->segments=36;
     path->x=cx;
     path->y=cy;
     path->h=path->w=(int) (path->radius*2);
     path->tint.Pick(red255);
     path->tint.ScaleByAlphaf(0.5f);
     path->duration=600.0f;
     path->delay=0.0f;
     s2->fx->Add(path,systemViewer);

     // Fade In
     
     ba = new VisualAid; // Planet
     ba->image=library.find( "data/images/elements/filledcircle.png");
     ba->w=ba->w2=bw;
     ba->h=ba->h2=bh;
     ba->sx=vx;
     ba->sy=vy;
     ba->delay=0.4f*orbitRatio;
     if ( ba->delay < 0.2f ) ba->delay=0.2f;
     ba->in=orbitRatio*0.5f; 
     ba->out=1.0f;
     ba->duration=1.0f;
     ba->Tints( crayons.Pick(clear), crayons.Pick(red255), crayons.Pick(red255) );
     s->fx->Add(ba,systemViewer);

     // Persistent

     ba = new VisualAid; // Planet
     ba->image=library.find( "data/images/elements/filledcircle.png");
     ba->w=ba->w2=bw;
     ba->h=ba->h2=bh;
     ba->sx=vx;
     ba->sy=vy;
     ba->delay=0.0f;
     ba->in=0.0f;
     ba->out=600.0f;
     ba->duration=600.0f;
     ba->Tints( crayons.Pick(clear), crayons.Pick(red255), crayons.Pick(red255) );
     s2->fx->Add(ba,systemViewer);
    }

    systemViewer->Init(false);
//    audio.manager.CueAndPlay("data/sounds/logosounds/smallsoudn.wav",1.0f,1.0f,false,true);
    audio.manager.CueAndPlay("data/sounds/UI/download1.wav",1.0f,1.0f,false,true);
   }
 }

#include "Player.h"
#include "StarShip.h"
#include "HUD.h"

void QuadrantViewer::LeftDown() {
 lastx=input->mxi-this->x;
 lasty=input->myi-this->y;
 if ( nearest ) {
  bool viewer=false;
  EACH(systemViewers.first,SystemViewerHandle,sh) if ( sh->a->s==nearest ) {
   viewer=true;
   break;
  }
  if ( !viewer )
   // Use nearest
   ToggleSolarSystem ( nearest );
   else {
    windows.stopDrill=true;
    if ( players.active->profile.active->vessel 
     && players.active->profile.active->vesselType == spacecraft ) {
    Starship *ship=(Starship *) players.active->profile.active->vessel.pointer;
    ship->course=nearest;
    audio.manager.CueAndPlayIfNotPlaying("data/sounds/fx/beep1.wav",1.0f,1.0f,false,true);  
    visible=false;
    windows.send=hud;
    hud->visible=true;
    hud->eh->hidden=false;
   }
  }
  dragging=false;
 } else {   dragging=true;  }
}