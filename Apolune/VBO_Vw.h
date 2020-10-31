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
#include "ModernVBO.h"

struct VBO_Vw {
 GLfloat pos[4];
 VBO_Vw() {
  pos[0]=pos[1]=pos[2]=pos[3]=0.0f;
 }
};

// http://www.arcsynthesis.org/gltut/Basics/Tut01%20Following%20the%20Data.html
class VBOStaticVw : public ModernVBO<VBO_Vw> {
public:
 VBOStaticVw() : ModernVBO() {
  draw_method=GL_POINTS;
 }
 virtual void fromModel( ImmediateModel *m );
 virtual void fromModel( ImmediateModel *m, int group );
 virtual void fromPLY( PLY *p );
 virtual void fromPLY( PLY *p, bool create ); 
 virtual void Create() {
  if ( data.size() == 0 ) return;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) (stride*data.size()), &data[0], usage);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  uploaded=true;
  OUTPUT("VBOStaticVw::Create() glGenBuffers created: VBO %d of size %dx%d (%d).\n",
   (int) vbo, (int) stride, (int) data.size(), (int) (stride*data.size()), (int) ibo, (int) indices.size() );
 }
 virtual void Bind() {
  glBindBuffer(GL_ARRAY_BUFFER,vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,0);
 }
 virtual void Render() {
  Enable();
  Bind();
//  glDrawRangeElements(GL_TRIANGLES, 0, data.size(), data.size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
//  glDrawElements(draw_method,count,preferred_type_enum,BUFFER_OFFSET(0));
  glDrawArrays(GL_TRIANGLES,0,(GLsizei) data.size());
  Unbind();
  Disable();
 }
 virtual void Unbind() {
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER,0);
 }
 void Set( VBO_Vw *target, Vertex *source ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  target->pos[3]=1.0f;
 }
 void Get( VBO_Vw *target, Vertex *source ) {
  source->x=(GLfloat) target->pos[0];
  source->y=(GLfloat) target->pos[1];
  source->z=(GLfloat) target->pos[2];
  source->w=(GLfloat) target->pos[3];
 }
};
