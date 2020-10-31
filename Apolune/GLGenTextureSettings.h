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
#include "GLSetup.h"
#if defined(NEVER)
class GLGenTextureSettings
{
public:
 bool S,T;
 GLGenTextureSettings() {
  S=true;
  T=true;
 }
 void Defaults( int idx ) {
  switch ( idx ) {
  case 0:
  /* Marble teapot */
  glBindTexture (GL_TEXTURE_2D, textures[0]);
  glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGenfv (GL_S, GL_EYE_PLANE, plane_xy);
  glTexGenfv (GL_T, GL_EYE_PLANE, plane_yz);
  glPushMatrix ();
  glTranslatef (-1, -1, 0);
  Teapot ();
  glPopMatrix ();

  /* Chess teapot, texture funny projection */
  glBindTexture (GL_TEXTURE_2D, textures[1]);
  glPushMatrix ();
  glLoadIdentity ();
  glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGenfv (GL_S, GL_EYE_PLANE, plane_yz);
  glTexGenfv (GL_T, GL_EYE_PLANE, plane_yz);
  glPopMatrix ();
  glPushMatrix ();
  glTranslatef (0, -1, 0);
  Teapot ();
  glPopMatrix ();

  /*/ Chess teapot, texture projected vertically */
  glBindTexture (GL_TEXTURE_2D, textures[1]);
  glTexGenfv (GL_S, GL_EYE_PLANE, plane_xy);
  glTexGenfv (GL_T, GL_EYE_PLANE, plane_yz);
  glPushMatrix ();
  glTranslatef (1, -1, 0);
  Teapot ();
  glPopMatrix ();

  /* Chrome teapot */
  glBindTexture (GL_TEXTURE_2D, textures[2]);
  glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glPushMatrix ();
  glTranslatef (-1, 0, 0);
  Teapot ();
  glPopMatrix ();
 }
 void Generate() {
  glGenTextures (1, texture);
  if ( S ) glEnable (GL_TEXTURE_GEN_S);
  if ( T ) glEnable (GL_TEXTURE_GEN_T);
  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  
  glEnable (GL_TEXTURE_GEN_S);
  glEnable (GL_TEXTURE_GEN_T);
  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

 }
};

void DefaultGenSettings() {
 


}
#endif