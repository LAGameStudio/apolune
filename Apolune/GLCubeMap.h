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
#include "GLWindow.h"
#include "GLImage.h"
#include "TextureLibrary.h"

#define SQRT_3_3 0.5773502691896258f

#if defined (NEVER)
class GLCubeMap : public LinkedList {
public:
 GLSetup *glSetup;
 float vertices[24] = {	// the idea is that all vectors have a length of 1 so that position can also be used as cubemap texture coords
 	-SQRT_3_3,	-SQRT_3_3,	-SQRT_3_3,
 	SQRT_3_3,	-SQRT_3_3,	-SQRT_3_3,
 	-SQRT_3_3,	SQRT_3_3,	-SQRT_3_3,
 	SQRT_3_3,	SQRT_3_3,	-SQRT_3_3,
 	-SQRT_3_3,	-SQRT_3_3,	SQRT_3_3,
 	SQRT_3_3,	-SQRT_3_3,	SQRT_3_3,
 	-SQRT_3_3,	SQRT_3_3,	SQRT_3_3,
 	SQRT_3_3,	SQRT_3_3,	SQRT_3_3
 };
 GLCubeMap( GLSetup *gl ) {
  glSetup=gl;
  Zero();
  glGenBuffers(2, this->m_bufferIDs);
  glBindBuffer(GL_ARRAY_BUFFER, this->m_bufferIDs[0]);
  float vertices[24] = {	// the idea is that all vectors have a length of 1 so that position can also be used as cubemap texture coords
		-SQRT_3_3,	-SQRT_3_3,	-SQRT_3_3,
		SQRT_3_3,	-SQRT_3_3,	-SQRT_3_3,
		-SQRT_3_3,	SQRT_3_3,	-SQRT_3_3,
		SQRT_3_3,	SQRT_3_3,	-SQRT_3_3,
		-SQRT_3_3,	-SQRT_3_3,	SQRT_3_3,
		SQRT_3_3,	-SQRT_3_3,	SQRT_3_3,
		-SQRT_3_3,	SQRT_3_3,	SQRT_3_3,
		SQRT_3_3,	SQRT_3_3,	SQRT_3_3
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_bufferIDs[1]);
	byte indices[24] = {
		1,			5,			7,			3,	// positive x
		2,			0,			4,			6,	// negative x
		4,			5,			7,			6,	// positive y
		0,			1,			3,			2,	// negative y
		0,			1,			5,			4,	// positive z
		3,			2,			6,			7	// negative z
	};
 }
 void Render() {
  glDisable(GL_DEPTH_TEST);	// skybox should be drawn behind anything else
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_bufferIDs[1]);
  glBindBuffer(GL_ARRAY_BUFFER, this->m_bufferIDs[0]);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  glDrawElements(GL_QUADS, 6 * 4, GL_UNSIGNED_BYTE, 0);
  glEnable(GL_DEPTH_TEST);
 }
 
 void Load( const char *f, const char *b, const char *l, const char *r, const char *u, const char *d ) {
  LoadedTexture *x, *F, *B, *L, *R, *U, *D;
  // front
  x=new LoadedTexture; 
  sprintf_s( x->filename, "%s", f );
  x->image=new GLImage(s);
  x->image->Upload(glSetup,true);//
  x->magFilter=GL_LINEAR_MIPMAP_LINEAR;
  x->minFilter=GL_LINEAR;
  x->wrapS=GL_CLAMP_TO_EDGE;
  x->wrapT=GL_CLAMP_TO_EDGE;
  Append(x);
  F=x;
  // back
  x=new LoadedTexture; 
  sprintf_s( x->filename, "%s", b );
  x->image=new GLImage(s);
  x->image->Upload(glSetup,true);//
  x->magFilter=GL_LINEAR_MIPMAP_LINEAR;
  x->minFilter=GL_LINEAR;
  x->wrapS=GL_CLAMP_TO_EDGE;
  x->wrapT=GL_CLAMP_TO_EDGE;
  Append(x);
  B=x;
  // left
  x=new LoadedTexture; 
  sprintf_s( x->filename, "%s", l );
  x->image=new GLImage(s);
  x->image->Upload(glSetup,true);//
  x->magFilter=GL_LINEAR_MIPMAP_LINEAR;
  x->minFilter=GL_LINEAR;
  x->wrapS=GL_CLAMP_TO_EDGE;
  x->wrapT=GL_CLAMP_TO_EDGE;
  Append(x);
  L=x;
  // right
  x=new LoadedTexture; 
  sprintf_s( x->filename, "%s", r );
  x->image=new GLImage(s);
  x->image->Upload(glSetup,true);//
  x->magFilter=GL_LINEAR_MIPMAP_LINEAR;
  x->minFilter=GL_LINEAR;
  x->wrapS=GL_CLAMP_TO_EDGE;
  x->wrapT=GL_CLAMP_TO_EDGE;
  Append(x);
  R=x;
  // up
  x=new LoadedTexture; 
  sprintf_s( x->filename, "%s", u );
  x->image=new GLImage(s);
  x->image->Upload(glSetup,true);//
  x->magFilter=GL_LINEAR_MIPMAP_LINEAR;
  x->minFilter=GL_LINEAR;
  x->wrapS=GL_CLAMP_TO_EDGE;
  x->wrapT=GL_CLAMP_TO_EDGE;
  Append(x);
  U=x;
  // down
  x=new LoadedTexture; 
  sprintf_s( x->filename, "%s", d );
  x->image=new GLImage(s);
  x->image->Upload(glSetup,true);//
  x->magFilter=GL_LINEAR_MIPMAP_LINEAR;
  x->minFilter=GL_LINEAR;
  x->wrapS=GL_CLAMP_TO_EDGE;
  x->wrapT=GL_CLAMP_TO_EDGE;
  Append(x);
  D=x;

  glGenTextures(1, (GLuint *)(&this->m_id));
  glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_id);
  for (int i = 0; i < 6; i++) {
 	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, info->format, info->width, info->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data + info->ofsCMSides[i]);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 }

 GLCubeMap(void) {}
 CLEARLISTRESET(LoadedTexture);
 ~GLCubeMap(void) { Clear(); }
};

#endif