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

#include "ListItem.h"
#include "LinkedList.h"

#include "Model.h"
#include "OldVBO.h"
#include "Primitive3d.h"
#include "ModernVBO.h"
#include "STLFile.h"
#include "PLY.h"
#include "ColorShader.h"

/*
 Creates collections of VBOs created from models, for displaying in a variety of ways.
*/

// Model Sources (STL,Model,Primitive)
ONE(ModelSource,{})
 Zp<ImmediateModel> modelZp;
 Zp<STLFile> stlZp;
 Zdis<ImmediateModel> modelZdis;
 Zdis<STLFile> stlZdis;
 Zp<PLY> plyZp;
 Zdis<PLY> plyZdis;
MANY(ModelSource,ModelSourceHandle,ModelSourceHandles,"ModelSource",ModelSources,{})
DONE(ModelSource);

// Managed VBO class and renderer
ONE(ModelVBO,{})
 ModelSource source;
 // Original Model Architecture
 VBOStaticVC vboVC;
 VBOStaticVCLines vboVClines;
 VBOStaticVNC vboVNC;
 VBOStaticVNT vboVNT;
 // Unit Versions
 VBOStaticVC unitvboVC;
 VBOStaticVCLines unitvboVClines;
 VBOStaticVNC unitvboVNC;
 VBOStaticVNT unitvboVNT;
 void Prepare( ImmediateModel *m ) {
  source.modelZp=m;
  Complete();
 }
 void Prepare( STLFile *s ) {
  source.stlZp=s;
  Complete();
 }
 void Prepare( PLY *p ) {
  source.plyZp=p;
  Complete();
 }
 void Prepare( const char *fn ) {
  Zstring filename(fn);
  if ( filename.ending(".stl") ) {
   source.stlZdis.Recycle(new STLFile(fn));
  } else if ( filename.ending(".obj") ) {
   ImmediateModel * m=new ImmediateModel;
   m->LoadOBJ(fn);
   source.modelZdis.Recycle(m);
  } else if ( filename.ending(".ply") ) {
   source.plyZdis.Recycle(new PLY(fn));
  } else if ( filename.ending(".txt") ) {
   ImmediateModel * m=new ImmediateModel;
   m->fromString(file_as_string(filename.c_str()).c_str());
   source.modelZdis.Recycle(m);
  }
  Complete();
 }
 void Complete() {
  if ( source.stlZdis || source.stlZp ) {
   STLFile *p=source.stlZdis?source.stlZdis.pointer:source.stlZp.pointer;
   p->toVBOStaticVC(&vboVC);
   p->toVBOStaticVC(&unitvboVC,true,true);
   p->toVBOStaticVNT(&vboVNT);
   p->toVBOStaticVNT(&unitvboVNT,true,true);
   p->toVBOStaticVCLines(&vboVClines);
   p->toVBOStaticVCLines(&unitvboVClines,true,true);
   p->toVBOStaticVNC(&vboVNC);
   p->toVBOStaticVNC(&unitvboVNC,true,true);
  } else if ( source.modelZdis || source.modelZp ) {
   ImmediateModel *p=source.modelZdis?source.modelZdis.pointer:source.modelZp.pointer;
   ImmediateModel m(p); m.Unitize();
   vboVC.fromModel(p);
   unitvboVC.fromModel(&m);
   vboVNC.fromModel(p);
   unitvboVNC.fromModel(&m);
   vboVClines.fromModel(p);
   unitvboVClines.fromModel(&m);
   vboVNT.fromModel(p);
   unitvboVNT.fromModel(&m);
  } else if ( source.plyZdis || source.plyZp ) {
   PLY *p=source.plyZdis?source.plyZdis.pointer:source.plyZp.pointer;
   PLY m(p); m.NormalizeToOne();
   vboVC.fromPLY(p);
   unitvboVC.fromPLY(&m);
   vboVNC.fromPLY(p);
   unitvboVNC.fromPLY(&m);
   vboVClines.fromPLY(p);
   unitvboVClines.fromPLY(&m);
   vboVNT.fromPLY(p);
   unitvboVNT.fromPLY(&m);
  }
  vboVC.Create();
  unitvboVC.Create();
  vboVNC.Create();
  unitvboVNC.Create();
  vboVClines.Create();
  unitvboVClines.Create();
  vboVNT.Create();
  unitvboVNT.Create();
 }
 void Render2d( double x, double y, double w, double h ) {
 }
 void Render2d( Crayon tint, double x, double y, double w, double h, double resx, double resy ) {
  Blending(none);
//  glPush();
//  glLoadIdentity();
//  glTranslated(x,y,0.0);
  //glScaled(w,h,0.0);
  ShadedColor2d(tint,&unitvboVC,x,y,w,h,resx,resy);
//  glPop();
 }
 void Render( GLTransform *in, LookAtPerspective *lap ) {
 }
 void Render( GLTransform *in, LookAtPerspective *lap, Crayon tint ) {
 }
 void Render( Crayon tint ) {
 }
MANY(ModelVBO,ModelVBOHandle,ModelVBOHandles,"ModelVBO",ModelLibrary,{})
 ModelVBO *Add( ImmediateModel *m ) {
  ModelVBO *mv=new ModelVBO;
  mv->Prepare(m);
  Append(mv);
  return mv;
 }
 ModelVBO *Add( STLFile *s ) {
  ModelVBO *mv=new ModelVBO;
  mv->Prepare(s);
  Append(mv);
  return mv;
 }
 ModelVBO *Add( PLY *p ) {
  ModelVBO *mv=new ModelVBO;
  mv->Prepare(p);
  Append(mv);
  return mv;
 }
 ModelVBO *Add( const char *name, ImmediateModel *m ) {
  ModelVBO *mv=new ModelVBO;
  mv->name=name;
  mv->Prepare(m);
  Append(mv);
  return mv;
 }
 ModelVBO *Add( const char *name, STLFile *s ) {
  ModelVBO *mv=new ModelVBO;
  mv->name=name;
  mv->Prepare(s);
  Append(mv);
  return mv;
 }
 ModelVBO *Add( const char *name, PLY *p ) {
  ModelVBO *mv=new ModelVBO;
  mv->name=name;
  mv->Prepare(p);
  Append(mv);
  return mv;
 }
DONE(ModelVBO);

extern ModelLibrary modelLibrary;