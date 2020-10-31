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
#include "OldVBO.h"
#include "Model.h"
#include "Proce55or.h"
#include "Interface.h"

extern Interface input;

/*
 * Does not yet support "super shaded" models.
 *
 * You must set translate/rotate and scale, as well as factor parameters properly to make this used correctly.
 * See fx_EasyModel for one that creates centered, rotatable models (same file, see below).
 */

enum fx_ModelRenderTypes {
 ThreeD=0,
 SideView=1,
 TopView=2,
 DLSideView=3,
 DLTopView=4,
};

// must become fx_Matter
#if defined(NEVER)
class fx_Model : public Proce55or
{
public:
 VBO *vbo;
 ImmediateModel *model;
 Substance *s;
 PROGRAMMABLE(fx_Model,ModelRenderPtr,modelRenderType,ModelRenderType);
 Vertex scale;
 Vertex translate;
 Vertex rotationDegrees;
 float factor;
 void Nothing() {}
 fx_Model(void) {
  Constructor();
  vbo=null;
  model=null;
  modelRenderType=&fx_Model::Nothing;
  scale.Set(1.0f,1.0f,1.0f);
  s=null;
 }
 void SetTarget( VBO *vbo, fx_ModelRenderTypes r ) {
  this->vbo=vbo;
  switch ( r ) {
   default:
   case ThreeD:
     modelRenderType=&fx_Model::ModelVBO3D;
    break;/*
   case SideView:
     modelRenderType=&fx_Model::ModelVBO2DSide;
    break;
   case TopView:
     modelRenderType=&fx_Model::ModelVBO3DTop;
    break;*/
  }
 }
 void SetTarget( ImmediateModel *model, fx_ModelRenderTypes r ) {
  this->model=model;
 }
 void ModelVBO3D() {
  vbo->m->Apply();
//  vbo->Render();
  vbo->m->Unapply();
 }/*
 void ModelVBO2DTop() {
 }
 void ModelVBO2DSide() {
 }*/
 void ModelImmediate3D() {
  s->m->Apply();
  model->Render3dScaled(factor);
  s->m->Unapply();
 }
 void ModelImmediate2DTop() {
  s->m->Apply();
  model->Render3dScaled(factor);
  s->m->Unapply();
 }
 void ModelImmediate2DSide() {
  s->m->Apply();
  model->Render3dScaled(factor);
  s->m->Unapply();
 }
 void ModelDL3D() {
  s->m->Apply();
  model->dl_scaled.Call();
  s->m->Unapply();
 }
 void ModelDL2DTop() {
  s->m->Apply();
  model->dl_top.Call();
  s->m->Unapply();
 }
 void ModelDL2DSide() {
  s->m->Apply();
  model->dl_right.Call();
  s->m->Unapply();
 }
 void Render( GLWindow *surface ) {
  ModelRenderType();
 }
 void Render(){
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushMatrix();
  glTranslatef(translate.x,translate.y,translate.z);
  glScalef(scale.x,scale.y,scale.z);
  glRotatef(rotationDegrees.x,1.0f,0.0f,0.0f);
  glRotatef(rotationDegrees.y,0.0f,1.0f,0.0f);
  glRotatef(rotationDegrees.z,0.0f,0.0f,1.0f);
  ModelRenderType();
  glPopMatrix();
  glPopAttrib();
 }
};


class fx_EasyModel : public Proce55or
{
public:
 VBO *vbo;
 ImmediateModel *model;
 Substance *s;
 PROGRAMMABLE(fx_EasyModel,ModelRenderPtr,modelRenderType,ModelRenderType);
 Vertex scale;
 Vertex translate;
 Vertex rotationDegrees;
 float factor;
 void Nothing() {}
 fx_EasyModel(void) {
  Constructor();
  vbo=null;
  model=null;
  modelRenderType=&fx_EasyModel::Nothing;
  scale.Set(1.0f,1.0f,1.0f);
  s=null;
 }
 void SetTarget( VBO *vbo, fx_ModelRenderTypes r ) {
  this->vbo=vbo;
  switch ( r ) {
   default:
   case ThreeD:
     modelRenderType=&fx_EasyModel::ModelVBO3D;
    break;/*
   case SideView:
     modelRenderType=&fx_Model::ModelVBO2DSide;
    break;
   case TopView:
     modelRenderType=&fx_Model::ModelVBO3DTop;
    break;*/
  }
 }
 void SetTarget( ImmediateModel *model, fx_ModelRenderTypes r ) {
  this->model=model;
 }
 void ModelVBO3D() {
  vbo->m->Apply();
//  vbo->Render();
  vbo->m->Unapply();
 }/*
 void ModelVBO2DTop() {
 }
 void ModelVBO2DSide() {
 }*/
 void ModelImmediate3D() {
  s->m->Apply();
  model->Render3dScaled(factor);
  s->m->Unapply();
 }
 void ModelImmediate2DTop() {
  s->m->Apply();
  model->Render3dScaled(factor);
  s->m->Unapply();
 }
 void ModelImmediate2DSide() {
  s->m->Apply();
  model->Render3dScaled(factor);
  s->m->Unapply();
 }
 void ModelDL3D() {
  s->m->Apply();
  model->dl_scaled.Call();
  s->m->Unapply();
 }
 void ModelDL2DTop() {
  s->m->Apply();
  model->dl_top.Call();
  s->m->Unapply();
 }
 void ModelDL2DSide() {
  s->m->Apply();
  model->dl_right.Call();
  s->m->Unapply();
 }
 void Render( GLWindow *surface ) {
  ModelRenderType();
 }
 void Render(){
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushMatrix();
  glTranslatef(translate.x,translate.y,translate.z);
  glScalef(scale.x,scale.y,scale.z);
  glRotatef(rotationDegrees.x,1.0f,0.0f,0.0f);
  glRotatef(rotationDegrees.y,0.0f,1.0f,0.0f);
  glRotatef(rotationDegrees.z,0.0f,0.0f,1.0f);
  ModelRenderType();
  glPopMatrix();
  glPopAttrib();
 }
};

// Responds to mouse movements
class fx_ModelInspector : public Proce55or
{
public:
 VBO *vbo;
 ImmediateModel *model;
 Substance *s;
 PROGRAMMABLE(fx_ModelInspector,ModelRenderPtr,modelRenderType,ModelRenderType);
 Vertex scale;
 Vertex translate;
 Vertex rotationDegrees;
 float factor;
 void Nothing() {}
 fx_ModelInspector(void) {
  Constructor();
  vbo=null;
  model=null;
  modelRenderType=&fx_ModelInspector::Nothing;
  scale.Set(1.0f,1.0f,1.0f);
  s=null;
 }
 void SetTarget( VBO *vbo, fx_ModelRenderTypes r ) {
  this->vbo=vbo;
  switch ( r ) {
   default:
   case ThreeD:
     modelRenderType=&fx_ModelInspector::ModelVBO3D;
    break;/*
   case SideView:
     modelRenderType=&fx_Model::ModelVBO2DSide;
    break;
   case TopView:
     modelRenderType=&fx_Model::ModelVBO3DTop;
    break;*/
  }
 }
 void SetTarget( ImmediateModel *model, fx_ModelRenderTypes r ) {
  this->model=model;
 }
 void ModelVBO3D() {
  vbo->m->Apply();
//  vbo->Render();
  vbo->m->Unapply();
 }/*
 void ModelVBO2DTop() {
 }
 void ModelVBO2DSide() {
 }*/
 void ModelImmediate3D() {
  s->m->Apply();
  model->Render3dScaled(factor);
  s->m->Unapply();
 }
 void ModelImmediate2DTop() {
  s->m->Apply();
  model->Render3dScaled(factor);
  s->m->Unapply();
 }
 void ModelImmediate2DSide() {
  s->m->Apply();
  model->Render3dScaled(factor);
  s->m->Unapply();
 }
 void ModelDL3D() {
  s->m->Apply();
  model->dl_scaled.Call();
  s->m->Unapply();
 }
 void ModelDL2DTop() {
  s->m->Apply();
  model->dl_top.Call();
  s->m->Unapply();
 }
 void ModelDL2DSide() {
  s->m->Apply();
  model->dl_right.Call();
  s->m->Unapply();
 }
 void Render( GLWindow *surface ) {
  ModelRenderType();
 }
 void MouseMove() {
  if ( input.left ) {
   rotationDegrees.z=180.0f*iratiof(input.mxi-this->x,w);
  } else
  rotationDegrees.x=180.0f*iratiof(input.mxi-this->x,w);
  rotationDegrees.y=180.0f*iratiof(input.myi-this->y,h);
 }
 void Render(){
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushMatrix();
  glTranslatef(translate.x,translate.y,translate.z);
  glScalef(scale.x,scale.y,scale.z);
  glRotatef(rotationDegrees.x,1.0f,0.0f,0.0f);
  glRotatef(rotationDegrees.y,0.0f,1.0f,0.0f);
  glRotatef(rotationDegrees.z,0.0f,0.0f,1.0f);
  ModelRenderType();
  glPopMatrix();
  glPopAttrib();
 }
};
#endif