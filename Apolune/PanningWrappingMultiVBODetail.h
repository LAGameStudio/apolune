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
#include "VBO.h"
#include "Image2Model.h"
#include "MultiPointSprite.h"

class PanningWrappingMultiVBODetailSet : public ListItem {
public:
 Vertexd translation,scale,rotation;
 Zp<GLImage> sprite;
 PanningWrappingMultiVBODetailSet( MultiColorMultiPointSprite *mcmps, const char *imageFilename ) {
  this->mcmps=mcmps;
  sprite=library.Load(imageFilename);
 }
 PanningWrappingMultiVBODetailSet( MultiColorMultiPointSprite *mcmps, GLImage *image ) {
  this->mcmps=mcmps;
  sprite=image;
 }
 Zp<MultiColorMultiPointSprite> mcmps;
 void Render() {
  mcmps->sprite=sprite;
  mcmps->translation.Set(&translation);
  mcmps->scale.Set(&scale);
  mcmps->rotation.Set(&rotation);
  mcmps->Render();
 }
 void MoveBy( double x, double y ) {
  translation.Add(x,y,0.0);
 }
 void MoveBy( double x, double y, ScreenPosition *wrap ) {
  translation.Add(x,y,0.0);
  if ( translation.x > wrap->dx2 ) translation.x-=wrap->dw;
  if ( translation.x < wrap->x ) translation.x+=wrap->dw;
  if ( translation.y > wrap->dy2 ) translation.y-=wrap->dh;
  if ( translation.y < wrap->y ) translation.y+=wrap->dh;
 }
 void FindWithin( VertexHandles *out, double x, double y, double R ) {
  mcmps->FindWithin(x,y,R,out);
 }
 void RemoveWithin( double x, double y, double R ) {
  mcmps->RemoveWithin(x,y,R);
 }
 void FindWithinBuffer( VertexHandles *out, double x, double y, double R ) {
  mcmps->FindWithinBuffer(x,y,R,out);
 }
 void RemoveWithinBuffer( double x, double y, double R ) {
  mcmps->RemoveWithinBuffer(x,y,R);
 }
 void FindWithinModel( VertexHandles *out, double x, double y, double R ) {
  mcmps->FindWithinModel(x,y,R,out);
 }
 void RemoveWithinModel( double x, double y, double R ) {
  mcmps->RemoveWithinModel(x,y,R);
 }
};

class PanningWrappingMultiVBODetailSets : public LinkedList {
public:
 ScreenPosition wrap;
 VertexHandles result;
 VertexHandles *FindWithin( double x, double y, double r ) {
  result.Clear();
  FOREACH(PanningWrappingMultiVBODetailSet,s) s->FindWithin(&result,x,y,R);
  return &result;
 }
 Vertices RemoveWithin( double x, double y, double r ) {
  FOREACH(PanningWrappingMultiVBODetailSet,s) s->RemoveWithin(x,y,R);
 }
 VertexHandles *FindWithinModel( double x, double y, double r ) {
  result.Clear();
  FOREACH(PanningWrappingMultiVBODetailSet,s) s->FindWithinModel(&result,x,y,R);
  return &result;
 }
 Vertices RemoveWithinModel( double x, double y, double r ) {
  FOREACH(PanningWrappingMultiVBODetailSet,s) s->RemoveWithinModel(x,y,R);
 }
 VertexHandles *FindWithinBuffer( double x, double y, double r ) {
  result.Clear();
  FOREACH(PanningWrappingMultiVBODetailSet,s) s->FindWithinBuffer(&result,x,y,R);
  return &result;
 }
 Vertices RemoveWithinBuffer( double x, double y, double r ) {
  FOREACH(PanningWrappingMultiVBODetailSet,s) s->RemoveWithinBuffer(x,y,R);
 }
 void MoveBy( double x, double y ) {
  FOREACH(PanningWrappingMultiVBODetailSet,s) s->MoveBy(x,y);
 }
 void MoveByWrap( double x, double y ) {
  FOREACH(PanningWrappingMultiVBODetailSet,s) s->MoveBy(x,y,&wrap);
 }
 void MoveTo( double x, double y ) {
  FOREACH(PanningWrappingMultiVBODetailSet,s) s->translation.Set(x,y,s->translation.z);
 }
 void MoveTo( double x, double y, double z ) {
  FOREACH(PanningWrappingMultiVBODetailSet,s) s->translation.Set(x,y,z);
 }
 void RotateBy( double x, double y, double z ) {
  FOREACH(PanningWrappingMultiVBODetailSet,s) s->rotation.Add(x,y,z);
 }
 void RotateTo( double x, double y, double z ) {
  FOREACH(PanningWrappingMultiVBODetailSet,s) s->rotation.Set(x,y,z);
 }
 void ScaleBy( double x, double y, double z ) {
  FOREACH(PanningWrappingMultiVBODetailSet,s) s->scale.Add(x,y,z);
 }
 void ScaleTo( double x, double y ) {
  FOREACH(PanningWrappingMultiVBODetailSet,s) s->scale.Set(x,y,s->scale.z);
 }
 void ScaleTo( double x, double y, double z ) {
  FOREACH(PanningWrappingMultiVBODetailSet,s) s->scale.Set(x,y,z);
 }

 void Render() {
  FOREACH(PanningWrappingMultiVBODetailSet,s) s->Render();
 }
};

#include "UglyFont.h"

class PWMVDTest : public GLWindow {
 LookAtPerspective lap;
 PanningWrappingMultiVBODetailSets sets;
 MultiColorMultiPointSprite  mcmps1;
 MultiColorMultiPointSprite  mcmps2;
 MultiColorMultiPointSprite  mcmps3;
 void MoveBy( double x, double y ) {
 }
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  mcmps1.fromImage("data/images/flora/large/blotched.png",true);
  mcmps1.vboVC.Create();
  mcmps1.renderSize=64.0f;
  sets.Append(new PanningWrappingMultiVBODetailSet(&mcmps1,"data/images/flora/blueflower.png"));
  mcmps2.fromImage("data/images/flora/large/skull1.png",true);
  mcmps2.vboVC.Create();
  mcmps2.renderSize=32;
  sets.Append(new PanningWrappingMultiVBODetailSet(&mcmps2,"data/images/flora/Plant1.png"));
  mcmps3.fromImage("data/images/flora/large/scattering2.png",true);
  mcmps3.vboVC.Create();
  mcmps3.renderSize=32.0f;
  sets.Append(new PanningWrappingMultiVBODetailSet(&mcmps3,"data/images/flora/greenlily.png"));
  lap.eye.Set(0,0,1.2);
  lap.Near=0.0001;
  lap.Far=10.0;
 }
 Vertexd rotation,translation,scale;
 void Between() {
  if ( input->left ) {
   int mx,my;
   MousePosition(&mx,&my);
   float dx,dy;
   dx=iratiof(mx,display->w)*360.0f;
   dy=iratiof(my,display->h)*360.0f;
   sets.RotateTo(dx,dy,0.0);
   rotation.Set(dx,dy,0.0);
  }
  if ( input->right ) {
   int mx,my;
   MousePosition(&mx,&my);
   float dx,dy;
   dx=iratiof(mx,display->w)*10.0f;
   dy=iratiof(my,display->h)*10.0f;
   sets.ScaleTo(dx,dy);
   scale.Set(dx,dy,1.0);
  }
  if ( input->middle ) {
   int mx,my;
   MousePosition(&mx,&my);
   float dx,dy;
   dx=iratiof(mx,display->w)*10.0f;
   dy=iratiof(my,display->h)*10.0f;
   sets.MoveTo((double)dx,(double)dy);
   translation.Set(dx,dy,0.0);
  }
  if ( input->KeyDown(DX_RIGHT) ) translation.x+=FRAMETIME;
  if ( input->KeyDown(DX_LEFT) ) translation.x-=FRAMETIME;
  if ( input->KeyDown(DX_UP) ) translation.y-=FRAMETIME;
  if ( input->KeyDown(DX_DOWN) ) translation.y+=FRAMETIME;
  sets.MoveTo(translation.x,translation.y,translation.z);
 }
 void Render() {
  glPush();
  lap.Apply();
  sets.Render();
  glPop();
  Debug();
 }
 void Debug() {
  Blending(none);
  glColor3d(1.0,0.4,0.3);
  FORMAT(buf,128,"Rotation: %f %f %f",   (float)rotation.x,(float)rotation.y,(float)rotation.z);
  Text(buf,10.0,display->hd-80.0,8.0,10.0,false);
  FORMAT(buf,128,"Translation: %f %f %f",(float)translation.x,(float)translation.y,(float)translation.z);
  Text(buf,10.0,display->hd-60.0,8.0,10.0,false);
  FORMAT(buf,128,"Scale: %f %f %f",      (float)scale.x,(float)scale.y,(float)scale.z);
  Text(buf,10.0,display->hd-40.0,8.0,10.0,false);
 }
};


class WrappingPWMVD {
public:
 LookAtPerspective lap;
 PanningWrappingMultiVBODetailSets sets;
 MultiColorMultiPointSprites mcmps;
 Vertexd rotation,translation,scale;
 WrappingPWMVD() {
  lap.eye.Set(0,0,1.2);
  lap.Near=0.0001;
  lap.Far=10.0;
  scale.Set(1.0,1.0,1.0);
 }
 MultiColorMultiPointSprite *Add( ImmediateModel *model, GLImage *tex, int mangle, double mdx, double mdy, double x, double y, double rs ) {
  MultiColorMultiPointSprite *mcmp=new MultiColorMultiPointSprite;
  mcmp->fromModel(model,true,mangle,(float)mdx,(float)mdy);
  mcmp->vboVC.Create();
  mcmp->renderSize=(float)rs;
  mcmp->rotation.Set(356.0,180.0,0.0);
//  mcmp->scale.Set(scx,scy,1.0);
//  mcmp->translation.Set(x,y,0.0);
  mcmps.Append(mcmp);
  sets.Append(new PanningWrappingMultiVBODetailSet(mcmp,tex));
  return mcmp;
 }
 MultiColorMultiPointSprite *Add( const char *placer, GLImage *tex, double x, double y, double scx, double scy ) {
  MultiColorMultiPointSprite *mcmp=new MultiColorMultiPointSprite;
  mcmp->fromImage(placer,true);
  mcmp->vboVC.Create();
  mcmp->renderSize=64.0f;
  mcmp->rotation.Set(356.0,180.0,0.0);
//  mcmp->scale.Set(scx,scy,1.0);
//  mcmp->translation.Set(x,y,0.0);
  mcmps.Append(mcmp);
  sets.Append(new PanningWrappingMultiVBODetailSet(mcmp,tex));
  return mcmp;
 }
 void MoveBy( double x, double y ) {
  translation.x+=-x;
  translation.y+=y;
  sets.MoveTo(translation.x,translation.y,translation.z);
 }
 void MoveByWrap( double x, double y ) {
  sets.MoveByWrap(-x,y);
 }
 void Render() {
  glPush();
  lap.Apply();
  sets.Render();
  glPop();
 }
 void Clear() {
  mcmps.Clear();
  sets.Clear();
 }
};