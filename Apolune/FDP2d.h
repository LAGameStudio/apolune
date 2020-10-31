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

#include "macros.h"
#include "moremath.h"
#include "ListItem.h"
#include "Strings.h"
#include "Crayon.h"

/*
 * "Force Determined Positioning"
 * Place objects so that they can all be viewed!  Use Polynet (equal forces) or Random (mass tree).
 * Iterate > 100 times to determine placement.
 */

class FDPVector2D : public ListItem {
public:
  float x;
  float y;
  FDPVector2D() : ListItem() {
   x=0;
   y=0;
  }
  void Set(float _x,float _y){x=_x;y=_y;}
  FDPVector2D(float _x, float _y) : ListItem() {
    x=_x;
    y=_y;
  }
  void clear(){x=0;y=0;}
  FDPVector2D *add(FDPVector2D v){
    return new FDPVector2D(x+=v.x,y+=v.y);
  }
  FDPVector2D *add(float x, float y){
    return new FDPVector2D(x+=x,y+=y);
  }
  void addSelf(FDPVector2D *v){
    x+=v->x;
    y+=v->y;
  }  
  void addSelf(float _x, float _y){
    x+=_x;
    y+=_y;
  }  
  FDPVector2D *sub(float x, float y){
    return new FDPVector2D(x-=x,y-=y);
  }
  FDPVector2D *sub(FDPVector2D *v){
    return new FDPVector2D(x-v->x,y-v->y);
  }
  void subSelf(FDPVector2D *v){
    x-=v->x;
    y-=v->y;
  }
  void subSelf(float _x, float _y){
    x-=_x;
    y-=_y;
  }
  FDPVector2D *mult(float alpha){
    return new FDPVector2D(x*alpha,y*alpha);
  }
  void multSelf(float alpha){
    x*=alpha;
    y*=alpha;
  }
  FDPVector2D *div(float alpha){
    return new FDPVector2D(x/alpha,y/alpha);
  }
  void divSelf(float alpha){
    x/=alpha;
    y/=alpha;
  }
  float norm(){
    return sqrt(pow(x,2)+pow(y,2));
  }
  FDPVector2D *versor(){
    return new FDPVector2D(x/norm(),y/norm());
  }
  void versorSelf(){
    x/=norm();
    y/=norm();
  }
  FDPVector2D *clone(){
    return new FDPVector2D(x,y);
  }
  string asString(){
   FORMAT(buf,1024,"[%1.2f,%1.2f]", (float) x, (float) y);
   return string(buf);
  }
};

class FDPVector2Ds : public LinkedList { public: CLEARLISTRESET(FDPVector2D); ~FDPVector2Ds() { Clear(); } };

class FDPNode : public ListItem {
public:
  FDPVector2D pos;
  FDPVector2D disp;
  float mass;
  float newmass;
  Crayon mycolor;
  bool trail;
  bool ball;
  FDPNode(float _x, float _y,float _mass){
   pos.Set(_x,_y);
   newmass=mass=_mass;
   mycolor.Int(20+upto(215),20+upto(215),20+upto(215),255);
   ball=true;
   trail=true;
  }
  void incrMass(float nm){
    newmass=mass+nm;
  }
  void setBall(bool ball){
    this->ball=ball;
  }
  void setTrail(bool trail){
    this->trail=trail;
  }
  void Update(){
   pos.addSelf(&disp);
   disp.clear();
   if (mass<newmass) mass+=0.2f;
  }  
  void Constrain(float x0, float x1,float y0, float y1){
    pos.x=min(x1,max(x0,pos.x));
    pos.y=min(y1,max(y0,pos.y));
  }
  string toString(){
    return pos.asString();
  }
};

class FDPNodes : public LinkedList { public: CLEARLISTRESET(FDPNode); ~FDPNodes() { Clear(); } };
enum FDP2dModes { FDPRandom=0, FDPPolynet=1 };

/* Main FDP2d class
 *
 * Usage:
 *  ->Iniitalize( n nodes);
 *  ->AddNode(); n times to add additional nodes prior to iteration / after initialization
 *  ->Iterate(100+);
 */
class FDP2d : public ListItem {
public:
 float width,height;
 int vel;
 FDP2dModes mode;
 FDPNodes nodes;
 float k,k2;
 int t;
 float tMass;
 int curn,Nodes;
 float curMass,maxMass;
 int im;
 FDP2d() : ListItem() {
  mode=FDPRandom;
  tMass=1.0f;
  curn=0;
  curMass=30.0f;
  maxMass=40.0f;
  vel=5;
  width=height=1.0f;
  k=k2=0.0f;
  t=0;
  Nodes=0;
  im=0;
 }
 void Initialize( int Nodes ) {
  this->Nodes=Nodes;
  nodes.Clear();
  switch(mode){
   case FDPRandom:
     this->Nodes=Nodes;
     k=sqrt((UMIN(width,height)/(float)this->Nodes))*0.05f;
     nodes.Append(
      new FDPNode(
       float_range(width/2.0f-width/8.0f,width/2.0f+width/8.0f),
       float_range(height/2.0f-height/8.0f,height/2.0f+height/8.0f),
       4.0f)
      );
    break;
   case FDPPolynet:
     this->Nodes=Nodes;
     k=sqrt((width*height/(float)this->Nodes))*0.5f;
     k2=k*0.2f;
     nodes.Append(
      new FDPNode(
       float_range(width/2.0f-width/8.0f,width/2.0f+width/8.0f),
       float_range(height/2.0f-height/8.0f,height/2.0f+height/8.0f),
       10.0f)
     );
    break;
  }  
  curn=0;
 }
 void AddNode() { this->Nodes++; }
 float fa(float m1, float m2, float z) { return 0.0001f*pow(k-m1-m2-z,2);     }
 float fr(float m1, float m2, float z) { return 0.5f*pow(m1+m2+k,2)/pow(z,2); }
 void Attract( float x, float y ) {
  EACH(nodes.first,FDPNode,f) {
   FDPVector2D *attract=new FDPVector2D(x,y);  
   FDPVector2D *delta=f->pos.sub(attract);
   if (delta->norm()!=0.0f) {
    FDPVector2D *v=delta->versor();
    FDPVector2D *u=v->mult( fa(f->mass,curMass,delta->norm()) );
    f->disp.subSelf(u);
    delete v;
    delete u;
   }
   delete attract;
   delete delta;
  }
 }
 void Repulse( float x, float y ) {
  EACH(nodes.first,FDPNode,f) {
   FDPVector2D *repulse=new FDPVector2D(x,y);  
   FDPVector2D *delta=f->pos.sub(repulse);
   if (delta->norm()<curMass+f->mass+100.0f){
    FDPVector2D *v=delta->versor();
    FDPVector2D *u= v->mult( fr(f->mass,curMass,delta->norm()) );
    f->disp.addSelf(u);
    delete v;
    delete u;
   }
   delete delta;
   delete repulse;
  }
 }
 void Iterations( int n ) {
  for ( int i=0; i<n; i++ ) Iterate();
 }
 void Iterate( int n ) { Iterations(n); }
 void Iterate(){
  if ((t++%vel)==0 && curn<this->Nodes){ 
    curn++;
    int r=(int)(number_range(1,nodes.count-1))-1;
    int s=0;
    bool gen=false;
    if (nodes.count>5 && gen){
      s=(int)(number_range(1,nodes.count-1))-1;
      while(r==s)
        s=(int)(number_range(1,nodes.count-1))-1;
    }
    FDPNode *nr=(FDPNode *)nodes.Element(r);
    FDPNode *ss=(FDPNode *)nodes.Element(s);
    if ( !nr || !ss ) return;
    FDPNode *newn=null;
    switch(mode){
    case FDPRandom:
      newn=new FDPNode(
       nr->pos.x+float_range(nr->mass,nr->mass+10.0f),
       nr->pos.y+float_range(nr->mass,nr->mass+10.0f),
       4
      );
      nodes.Append(newn);
      newn->incrMass(2.0f);
      nr->incrMass(2.0f);
      if (nodes.count>5 && gen){
        newn->incrMass(2);
        ss->incrMass(2);
      }  
     break;
    case FDPPolynet:
      float prob=(float) uniform();
      newn=new FDPNode(
       float_range(1.0f,width),
       float_range(1.0f,height),
       10
      );          
      nodes.Append(newn);
     break;
    }    
  }
  if (tMass<1){
    tMass+=0.1f;
    curMass=sin(PIf*tMass)*600.0f*(1.0f-tMass);
  }
  curMass=max(curMass,maxMass);
  EACH(nodes.first,FDPNode,u) {
   EACH(nodes.first,FDPNode,v) {
    if ( u!=v ) {
     FDPVector2D *delta=v->pos.sub(&u->pos);
     if (delta->norm()!=0.0f) {
      FDPVector2D *dv=delta->versor();
      FDPVector2D *dvm=dv->mult( fr(v->mass,u->mass,delta->norm()) );
      v->disp.addSelf( dvm );
      delete dv;
      delete dvm;
     }
     delete delta;
    }
   }
   u->Update();   
   u->Constrain(0.0f,width,0.0f,height);
  }
 }
};