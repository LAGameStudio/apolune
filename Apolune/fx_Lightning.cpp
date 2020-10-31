#include "fx_Lightning.h"

LightningBolt2d::LightningBolt2d( Cartesian *s, Cartesian *d, float branchChance, int n, float w ) : ListItem() {
  branches=new LightningBolts2d;
  start.fx=s->fx;
  start.fy=s->fy;
  delta.fx=d->fx;
  delta.fy=d->fy;
  branch=branchChance;
  nodes=n;
  nodes2=n/2;
  boltWidth=w;
  boltWidth2=w/2.0f;
  if ( branchChance >= 0.9f ) branchChance=0.5f;
  Create();
  Compile();
 }

void LightningBolt2d::Create() {
  path.Clear();
  float X=0.0f,Y=0.0f;
  for ( int i=0; i < nodes; i++ ) {
   Cartesian *c=new Cartesian;
   c->fx=start.fx+X;
   c->fy=start.fy+Y;
   c->fw=boltWidth+ (boltWidth2* ( i > nodes2 ? 1.0f/iratiof(i-nodes2,nodes2) : iratiof(i,nodes2) ) );
   c->fh=c->fw/2.0f;
   X+=delta.fx-(float)(uniform() * (double) delta.fx*2.0);
   Y+=1.0f+(float)(uniform() * (double) delta.fy);
   path.Append(c);
   if ( i==nodes-1 ) end=c;
   else if ( (float) uniform() < branch ) {
    if ( nodes2 < 2 || boltWidth/2.0f < 1.0f ) continue;
    Cartesian d2;
    d2.Set(delta.fx/2.0f,delta.fy/2.0f);
    branches->Strike( c, &d2, branch/2.0f, nodes/2, boltWidth/2.0f );
   }
  }
 }

void LightningBolt2d::Clean() {
 delete branches;
}

void LightningBolt2d::Creep() {
 outer.Clear();
 inner.Clear();
 EACH(path.first,Cartesian,c) c->fx=c->fx+(boltWidth2-(float)uniform()*boltWidth);
 EACH(branches->first,LightningBolt2d,bolt) bolt->Creep();
 Compile();
}

void LightningBolt2d::MoveBy( int x, int y ) {
 start.fx+=x;
 start.fy+=y;
 EACH(path.first,Cartesian,c) { c->fx+=x; c->fy+=y; }
 EACH(branches->first,LightningBolt2d,bolt) bolt->MoveBy(x,y);
 Compile();
}

void LightningBolts2d::Draw() {
 glMatrixMode(GL_MODELVIEW);
 glPushMatrix();
 glTranslated(translate.x,translate.y,translate.z);
 glScaled(scale.x,scale.y,scale.z);
 glRotated(rotation.x,1.0,0.0,0.0);
 glRotated(rotation.y,0.0,1.0,0.0);
 glRotated(rotation.z,0.0,0.0,1.0);
 glBegin(GL_QUADS);
 FOREACH(LightningBolt2d,bolt){
  EACH(bolt->branches->first,LightningBolt2d,branch) {
   branch->outer.DrawNoBeginEnd();
   branch->inner.DrawNoBeginEnd();
  }
  bolt->outer.DrawNoBeginEnd();
  bolt->inner.DrawNoBeginEnd();
 }
 glEnd();
 glPopMatrix();
}