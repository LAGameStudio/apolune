/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once
#include "VBO_VC.h"

class VBODynamicVC : public ModernVBO<VBO_VC> {
public:
 VBODynamicVC() : ModernVBO() {
  draw_method=GL_POINTS;
  usage=GL_DYNAMIC_DRAW;
 }
 virtual void fromModel( ImmediateModel *m );
 virtual void fromModel( ImmediateModel *m, int group );
 virtual void fromPLY( PLY *p );
 virtual void fromPLY( PLY *p, bool create ); 
 void PushVertexColor( Vertexd *a, Crayon color ) {
  size_t end=data.size();
  data.resize(end+1);
  indices.resize(end+1);
  CopyData(a,&data[end], color.rf,  color.gf,  color.bf, color.af);
  indices[end]=(preferred_type)end;
  count++;
 }
 void PushVertexColor( Vertexd *a, float r, float g, float b, float af ) {
  size_t end=data.size();
  data.resize(end+1);
  indices.resize(end+1);
  CopyData(a,&data[end], r,  g,  b, af);
  indices[end]=(preferred_type)end;
  count++;
 }
 void BindToStreams() {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(VBO_VC), BUFFER_OFFSET(0));
  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(4, GL_FLOAT, sizeof(VBO_VC), BUFFER_OFFSET(4*3));
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
 } 
 void CopyData( Vertexd *source, VBO_VC *target, float r, float g, float b, float a ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  target->color[0]=(GLfloat) r;
  target->color[1]=(GLfloat) g;
  target->color[2]=(GLfloat) b;
  target->color[3]=(GLfloat) a;
 }
 void CopyData( Vertexd *source, VBO_VC *target ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  target->color[0]=target->color[1]=target->color[2]=target->color[3]=1.0f;
 }
 void CopyData( Vertex *source, VBO_VC *target ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  target->color[0]=(GLfloat) source->rgba[0];
  target->color[1]=(GLfloat) source->rgba[1];
  target->color[2]=(GLfloat) source->rgba[2];
  target->color[3]=(GLfloat) source->rgba[3];
 }
 void RevCopyData( Vertex *source, VBO_VC *target ) {
  target->pos[0]  =(GLfloat) source->x;
  target->pos[1]  =(GLfloat) source->y;
  target->pos[2]  =(GLfloat) source->z;
  target->color[0]=(GLfloat) source->rgba[0];
  target->color[1]=(GLfloat) source->rgba[1];
  target->color[2]=(GLfloat) source->rgba[2];
  target->color[3]=(GLfloat) source->rgba[3];
 }
};