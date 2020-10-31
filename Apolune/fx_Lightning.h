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
#include "Art.h"
#include "Film.h"
#include "GLPrimitives.h"
#include "Proce55or.h"
#include "Vertex.h"
#include "Tweens.h"

extern Tweens tweens;

class LightningBolts2d;

class LightningBolt2d : public ListItem {
public:
 Cartesians path;
 Zpointer<LightningBolts2d> branches;
 Cartesian start,delta;
 Zpointer<Cartesian> end;
 Zfloat branch, boltWidth,boltWidth2;
 Zint nodes,nodes2;
 // s=starting position, d=deltaxy branchChance 0.0-1.0f, never 1.0f, n=node granularity 
 LightningBolt2d( Cartesian *s, Cartesian *d, float branchChance, int n, float w );
 void Create();
 GLQuads outer,inner;
 void Compile() {
  EACH(path.first,Cartesian,c) {
   Cartesian *next=(Cartesian *) c->next;
   Cartesian *prev=(Cartesian *) c->prev;
   if ( !next ) break;
   else
   if ( !prev ) { // starting triangle
    GLQuad *p=new GLQuad;
    GLQuad *q=new GLQuad;
    GLQuad *r=new GLQuad;
    p->a.Setf(c->fx,c->fy);
    p->b.Setf(c->fx+0.00001f,c->fy);
    p->c.Setf(next->fx+next->fh,next->fy);
    p->d.Setf(next->fx-next->fh,next->fy);
    q->a.Setf(c->fx-c->fw,c->fy);
    q->b.Setf(c->fx+c->fw,c->fy);
    q->c.Setf(next->fx+next->fw,next->fy);
    q->d.Setf(next->fx-next->fw,next->fy);
    r->a.Setf(c->fx,c->fy);
    r->b.Setf(c->fx+0.00001f,c->fy);
    r->c.Setf(next->fx+next->fh/2.0f,next->fy);
    r->d.Setf(next->fx-next->fh/2.0f,next->fy);
    outer.Append(q);
    inner.Append(p);
   } else
   if ( !next->next ) { // end
    GLQuad *p=new GLQuad;
    GLQuad *q=new GLQuad;
    GLQuad *r=new GLQuad;
    p->a.Setf(c->fx-c->fh,c->fy);
    p->b.Setf(c->fx+c->fh,c->fy);
    p->c.Setf(next->fx+next->fh,next->fy);
    p->d.Setf(next->fx-next->fh,next->fy);
    q->a.Setf(c->fx-c->fw,c->fy);
    q->b.Setf(c->fx+c->fw,c->fy);
    q->c.Setf(next->fx+0.00001f,next->fy);
    q->d.Setf(next->fx,next->fy);
    outer.Append(q);
    inner.Append(p);
   } else {
    GLQuad *p=new GLQuad;
    GLQuad *q=new GLQuad;
    GLQuad *r=new GLQuad;
    p->a.Setf(c->fx-c->fh,c->fy);
    p->b.Setf(c->fx+c->fh,c->fy);
    p->c.Setf(next->fx+next->fh,next->fy);
    p->d.Setf(next->fx-next->fh,next->fy);
    q->a.Setf(c->fx-c->fw,c->fy);
    q->b.Setf(c->fx+c->fw,c->fy);
    q->c.Setf(next->fx+next->fw,next->fy);
    q->d.Setf(next->fx-next->fw,next->fy);
    outer.Append(q);
    inner.Append(p);
   }
  }
 }
 void Clean();
 void Creep();
 void MoveBy( int x, int y );
};

class LightningBolts2d : public LinkedList {
public:
 CLEARLISTRESET(LightningBolt2d);
 Vertexd translate,scale,rotation;
 Zfloat angle;
 LightningBolts2d() : LinkedList() {
  scale.Set(1.0,1.0,1.0);
 }
 void Strike( Cartesian *s, Cartesian *d, float branchChance, int n, float w ) {
  angle=(float) (TWO_PI * uniform());
  Append( new LightningBolt2d(s,d,branchChance,n,w) );
 }
 void Draw();
 void Creep() {
  FOREACH(LightningBolt2d,bolt) bolt->Creep();
 }
 void MoveBy( int x, int y ) {
  FOREACH(LightningBolt2d,bolt) bolt->MoveBy(x,y);
 }
};

class fx_Lightning2d : public Proce55or {
public:
 Cartesian startExtents;
 LightningBolts2d lightning;
 Blends blend;
 Crayon tint,color,contactTint;
 Zpointer<Film> contact;
 Zpointer<GLImage> paint;
 Zpointer<Tween> flash;
 Zfloat thickness,angle;
 Zint grains;
 Cartesian delta;
 Zfloat waitLength;
 Zbool fromTop,startContact,endContact;
 Zbool immovableY;
 Zfloat wait,branchProbability,contactSize;
 Zfloat decay,persist,persist2,progress,expired;
 fx_Lightning2d() : Proce55or() {
  tint.Pick(cyan);
  contactTint.Pick(electricBlue);
  branchProbability=0.1f;
  thickness=20.0f;
  blend=additive;
  waitLength=0.25f;
  decay=0.25f;
  persist=0.125f;
  persist2=persist*2.0f;
  fromTop=true;
  grains=20; // if you want you can calculate grains to a specific height by using delta as a factor
  startExtents.Setf(50.0f,50.0f);
  contactSize=128.0f;
  delta.Setf(20.0f,15.0f);
  FilmLibrary *f=vault.find("Sparks");
  contact=f->films.find("Fuse");
  paint=library.Load("data/images/flares/bolt.png",tllto_TilingX);
  flash=tweens.find("Humanized Sputter");
 }
 /*bool Done() {
  return (decay<=0.0f);
 }*/
 void MoveBy( int xx, int yy ) {
  int dy=immovableY ? 0 : yy ;
  lightning.MoveBy( xx, dy );
  x+=xx;
  y+=dy;
  x2+=xx;
  y2+=dy;
 }
 void Setup() {
  x2=x+w;
  y2=y+h;
  Reset();
 }
 void Reset() {
  decay=persist2;
  progress=0.0f;
  Cartesian s;
  s.Setf((float) x+float_range(0.0f,(float) startExtents.fx),(float) y+float_range(0.0f,(float) startExtents.fy));
  lightning.Clear();
  lightning.Strike(&s,&delta,branchProbability,grains,thickness);
 }
 bool Done() {
  return expired>duration;
 }
 void Between() {
  expired+=FRAMETIME;
  if ( wait > 0.0f ) { wait-=FRAMETIME; return; }
  decay-=FRAMETIME;
  if ( decay<= 0.0f ) {
   wait=(float) uniform()*waitLength;
   Reset();
   return;
  }
  if ( progress > persist ) progress-=FRAMETIME;
  else progress+=FRAMETIME;
  lightning.Creep();
 }
 void Render() {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  if ( wait > 0.0f || hidden || decay < 0.0f ) return;
  color.fromCrayon(tint);
  color.ScaleByAlphaf(flash->tweenf(CLAMP(progress/persist2,1.0f)));
  Blending(blend);
  glColor4fv(color.floats);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,paint->texture);
  lightning.Draw();
  glPopAttrib();
  if ( startContact ) {
   LightningBolt2d *first=(LightningBolt2d *) lightning.first;
   GLImage *frame=contact->frame(CLAMP(1.0f-decay/persist2,1.0f));
   Cartesian *c=(Cartesian *) first->path.first;
   Cartesian d;
   d.x=(int) (c->fx-contactSize/2.0f);
   d.y=(int) (c->fy-contactSize/2.0f);
   StretchiNoAlpha(this->parent,frame,contactTint,additive,d.x,d.y,(int)contactSize,(int)contactSize);
  }
  if ( endContact ) {
   LightningBolt2d *first=(LightningBolt2d *) lightning.first;
   GLImage *frame=contact->frame(CLAMP(1.0f-decay/persist2,1.0f));
   Cartesian *c=(Cartesian *) first->path.last;
   Cartesian d;
   d.x=(int) (c->fx-contactSize/2.0f);
   d.y=(int) (c->fy-contactSize/2.0f);
   StretchiNoAlpha(this->parent,frame,contactTint,additive,d.x,d.y,(int)contactSize,(int)contactSize);
  }
 }
 void Render(GLWindow *surface) {
  Render();
 }
};


class fx_LightningField : public Proce55or {
public:
 GLImage *zap;
 float hLine;
 
};