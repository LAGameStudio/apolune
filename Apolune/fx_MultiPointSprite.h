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
#include "MultiPointSprite.h"
#include "fx_FireAndForget.h"
#include "PanningWrappingMultiVBODetail.h"

class fx_MultiPointSprite : public FAFProce55or {
public:
 LookAtPerspective lap;
 MultiColorMultiPointSprites mcmps;
 PanningWrappingMultiVBODetailSets sets;
 fx_MultiPointSprite() : FAFProce55or() {
 }
 void Add( ImmediateModel *model, GLImage *tex, double x, double y, double scx, double scy ) {
  MultiColorMultiPointSprite *mcmp=new MultiColorMultiPointSprite;
  mcmp->fromModel(model,true);
  mcmp->vboVC.Create();
  mcmp->renderSize=64.0f;
  mcmp->rotation.Set(356.0,180.0,0.0);
  mcmp->scale.Set(scx,scy,1.0);
  mcmp->translation.Set(x,y,0.0);
  sets.Append(new PanningWrappingMultiVBODetailSet(mcmp,tex));
 }
 void Add( const char *placer, GLImage *tex, double x, double y, double scx, double scy ) {
  MultiColorMultiPointSprite *mcmp=new MultiColorMultiPointSprite;
  mcmp->fromImage(placer,true);
  mcmp->vboVC.Create();
  mcmp->renderSize=64.0f;
  mcmp->rotation.Set(356.0,180.0,0.0);
  mcmp->scale.Set(scx,scy,1.0);
  mcmp->translation.Set(x,y,0.0);
  sets.Append(new PanningWrappingMultiVBODetailSet(mcmp,tex));
 }
 Vertexd translation;
 void MoveBy( double x, double y ) {
  translation.x+=x;
  translation.y+=y;
  sets.MoveTo(translation.x,translation.y);
 }
 void MoveByd( double x, double y ) {
  translation.x+=x;
  translation.y+=y;
  sets.MoveTo(translation.x,translation.y);
 }
 void MoveByWrap( double x, double y ) {
  translation.x+=x;
  translation.y+=y;
  sets.MoveTo(translation.x,translation.y);
 }
 VertexHandles *FindWithin( double x, double y, double R ) {
  return sets.FindWithin(x,y,R);
 }
 void RemoveWithin( double x, double y, double R ) {
  sets.RemoveWithin(x,y,R);
 }
 void Render( GLWindow *surface ) {
  glPush();
  lap.Apply();
  sets.Render();
  glPop();
 }
 bool Done() {
  return false;
 }
};