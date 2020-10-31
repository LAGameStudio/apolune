#pragma once
#include "Globals.h"
#include "Proce55Viewer2d.h"
#include "Chemicals.h"
#include "fx_UglyText.h"
#include "OrderedProce55Viewer.h"

extern TexFont bytes;

class ElementCard : public Proce55or {
public:
 UniversalElement *element;
 string symbol;
 double quantity;
 int margin;
 float fadeIn, expired;
 Crayon tint,backingTint;
 GLImage *border,*backing;
 Zbool played;
 ElementCard( UniversalElementHandle *u, int x, int y ) : Proce55or() {
  element=u->element;
  symbol=u->element->symbol;
  quantity=u->quantity;
  fadeIn=2.0f;
  margin=8;
  switch ( u->element->group ) {  
   case nonMetallic: tint.Pick(green); break;
   case metal: tint.Pick(gray); break;
   case alkali: tint.Pick(goldenrod); break;
   case alkalineMetal: tint.Pick(cyan); break;
   case metalloid: tint.Pick(ANSIboldgrey); break;
   case nobleGas: tint.Pick(macaroniAndCheese); break;
   case transitionMetal: tint.Pick(ANSIgrey); break;
   case halogen: tint.Pick(orange); break;
   case actinideSeries: tint.Pick(lightCoral3); break;
   case lanthanideSeries: tint.Pick(nadashikoPink2); break;
  }
  backingTint.Pick(black);
  tint.Pick(alabaster);
  this->x=x;
  this->y=y;
  this->w=128;
  this->h=128;
  this->x2=x+w;
  this->y2=y+h;
  backing=library.Load("data/images/elements/transrounded512borderless.png");
  border=library.Load("data/images/elements/rounder_box.png");
  expired=0.0f;
 }
 bool Done() {
   return expired > 4.0f;
 }
 void Between() {
  expired+=FRAMETIME;
 }
 void Render() {
  glColor3d(1.0,1.0,1.0);
  Text("Isolating",x+w/2,y-10,6,7,true);
  char buf[64];
  if ( expired > 3.0f ) {
   float perc=1.0f-(expired-3.0f);
   Crayon drawing;
   drawing.fromCrayon(tint);
   drawing.ScaleByAlphaf(perc);
   backingTint.Alpha(perc);
   QuadiNoAlpha(backing,backingTint,transparency,x,y,w,h);
   QuadiNoAlpha(border,drawing,additive,x,y,w,h);
   WriteTexFont(&bytes,drawing,additive,x+margin+16,y+margin+16,24,24,2,(char *)symbol.c_str());
   Blending(additive);
   glColor4d(drawing.rf,drawing.gf,drawing.bf,drawing.af);
   Text((char *) element->name.c_str(),x+w/2,y+h-margin-16,6,7,true);
   FORMAT(buf,64,"%1.2f Tons",(float)((float)quantity*100.0f*element->density));
   Text(buf,x+w/2,y+h-margin-8-32,6,7,true);
  } else
  if ( expired <= fadeIn ) {
   if ( !played ) {
    audio.manager.CueAndPlayIfNotPlaying(rock_grinding.AnyChar(),float_range(0.25f,1.0f),0.5f,false,true);
    played=true;
   }
   float perc=expired/fadeIn;
   Crayon drawing;
   drawing.fromCrayon(tint);
   drawing.ScaleByAlphaf(perc);
   backingTint.Alpha(perc);
   QuadiNoAlpha(backing,backingTint,transparency,x,y,w,h);
   QuadiNoAlpha(border,drawing,additive,x,y,w,h);
   WriteTexFont(&bytes,drawing,additive,x+margin+16,y+margin+16,24,24,2,(char *)symbol.c_str());
   Blending(additive);
   glColor4d(drawing.rf,drawing.gf,drawing.bf,drawing.af);
   Text((char *) element->name.c_str(),x+w/2,y+h-margin-16,6,7,true);
   FORMAT(buf,64,"%1.2f Tons",(float)((float)quantity*100.0f*element->density));
   Text(buf,x+w/2,y+h-margin-8-32,6,7,true);
  } else {
   QuadiNoAlpha(backing,backingTint,transparency,x,y,w,h);
   QuadiNoAlpha(border,tint,additive,x,y,w,h);
   WriteTexFont(&bytes,tint,additive,x+margin+16,y+margin+16,24,24,2,(char *)symbol.c_str());
   Blending(additive);
   glColor4d(tint.rf,tint.gf,tint.bf,tint.af);
   Text((char *) element->name.c_str(),x+w/2,y+h-margin-16,6,7,true);
   FORMAT(buf,64,"%1.2f Tons",(float)((float)quantity*100.0f*element->density));
   Text(buf,x+w/2,y+h-margin-8-32,6,7,true);
  }
 }
 void Render( GLWindow *surface ) {
  Render();
 }
 void Draw( GLWindow *surface ) {
  Render();
 }
};


class CompoundCard : public Proce55or {
public:
 Compound *compound;
 double quantity;
 int margin;
 float fadeIn, expired;
 Crayon tint,backingTint,compoundTint;
 GLImage *border,*backing,*chemistry;
 Zbool played;
 CompoundCard( CompoundHandle *u, int x, int y ) : Proce55or() {
  compound=u->compound;
  quantity=u->quantity;
  fadeIn=2.0f;
  margin=8;
  switch ( compound->group ) {  
   case inorganic: tint.Pick(green); break;
   case organic: tint.Pick(gray); break;
   case biomolecules: tint.Pick(goldenrod); break;
   case alloys: tint.Pick(cyan); break;
   case minerals: tint.Pick(ANSIboldgrey); break;
   case alchemical: tint.Pick(macaroniAndCheese); break;
   case polyatomicion: tint.Pick(ANSIgrey); break;
   case anion: tint.Pick(orange); break;
   case halide: tint.Pick(lightCoral3); break;
   case organohalide: tint.Pick(nadashikoPink2); break;
  }
  backingTint.Pick(black);
  tint.Pick(alabaster);
  int proposed=(compound->name.length()+4)*8;
  this->w=proposed<128?128:proposed;
  this->x=x-this->w+128;
  this->y=y;
  this->h=128;
  this->x2=x+w;
  this->y2=y+h;
  backing=library.Load("data/images/elements/transrounded512borderless.png");
  border=library.Load("data/images/elements/rounder_box.png");
  chemistry=library.Load("data/images/icons/papapishu_Lab_icon_1.png");
  expired=0.0f;
 }
 bool Done() {
   return expired > 4.0f;
 }
 void Between() {
  expired+=FRAMETIME;
 }
 void Render() {
  glColor3d(1.0,1.0,1.0);
  Text("Refining",x+w/2,y-10,6,7,true);
  char buf[64];
  if ( expired > 3.0f ) {
   float perc=1.0f-(expired-3.0f);
   Crayon drawing;
   drawing.fromCrayon(tint);
   drawing.ScaleByAlphaf(perc);
   backingTint.Alpha(perc);
   compoundTint.fromCrayon(compound->color());
   compoundTint.ScaleByAlphaf(perc);
   QuadiNoAlpha(backing,backingTint,transparency,x,y,w,h);
   QuadiNoAlpha(border,drawing,additive,x,y,w,h);
   QuadiNoAlpha(chemistry,compoundTint,additive,x+w/2-24,y+margin+16,48,48);
   Blending(additive);
   glColor4d(drawing.rf,drawing.gf,drawing.bf,drawing.af);
   Text((char *) compound->name.c_str(),x+w/2,y+h-margin-16,6,7,true);
   FORMAT(buf,64,"%1.2f Tons",(float)((float)quantity*100.0f*compound->atoms.weight));
   Text(buf,x+w/2,y+h-margin-8-32,6,7,true);
  } else
  if ( expired <= fadeIn ) {
   if ( !played ) {
    played=true;
    audio.manager.CueAndPlayIfNotPlaying(rock_grinding.AnyChar(),float_range(0.25f,1.0f),0.5f,false,true);
   }
   float perc=expired/fadeIn;
   Crayon drawing;
   drawing.fromCrayon(tint);
   drawing.ScaleByAlphaf(perc);
   backingTint.Alpha(perc);
   compoundTint.fromCrayon(compound->color());
   compoundTint.ScaleByAlphaf(perc);
   QuadiNoAlpha(backing,backingTint,transparency,x,y,w,h);
   QuadiNoAlpha(border,drawing,additive,x,y,w,h);
   QuadiNoAlpha(chemistry,compoundTint,additive,x+w/2-24,y+margin+16,48,48);
   Blending(additive);
   glColor4d(drawing.rf,drawing.gf,drawing.bf,drawing.af);
   Text((char *) compound->name.c_str(),x+w/2,y+h-margin-16,6,7,true);
   FORMAT(buf,64,"%1.2f Tons",(float)((float)quantity*100.0f*compound->atoms.weight));
   Text(buf,x+w/2,y+h-margin-8-32,6,7,true);
  } else {
   QuadiNoAlpha(backing,backingTint,transparency,x,y,w,h);
   QuadiNoAlpha(border,tint,additive,x,y,w,h);
   QuadiNoAlpha(chemistry,compoundTint,additive,x+w/2-24,y+margin+16,48,48);
   Blending(additive);
   glColor4d(tint.rf,tint.gf,tint.bf,tint.af);
   Text((char *) compound->name.c_str(),x+w/2,y+h-margin-16,6,7,true);
   FORMAT(buf,64,"%1.2f Tons",(float)((float)quantity*100.0f*compound->atoms.weight));
   Text(buf,x+w/2,y+h-margin-8-32,6,7,true);
  }
 }
 void Render( GLWindow *surface ) {
  Render();
 }
 void Draw( GLWindow *surface ) {
  Render();
 }
};

class PeriodicTableOverlay : public OrderedProce55Viewer {
public:
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  Repopulate(null);
 }
 void Repopulate(Chemicals *list) {
  fx.Clear();
  DropChildren();
  if ( !list ) {
   Init();
   return;
  }
  EACH(list->elements.first,UniversalElementHandle,ueh) {
   Add(new ElementCard(ueh,display->w/2+256,display->h/2+256));
  }
  EACH(list->chemicals.first,CompoundHandle,ch) {
   Add(new CompoundCard(ch,display->w/2+256,display->h/2+256));
  }
  Init();
 }
};

