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

#include "VideoStream.h"
#include "GLSetup.h"

class Video {
public:
 Zdisposable<VideoStream> stream;
 Zint width, height;
 Zstring filename;
 Zbool valid;
 GLenum error;
 GLuint _name;
 void * pixels;
 Video(const char *filename, int width, int height) {
  stream.Instantiate();
  stream->Create(filename, width, height);
  this->width = width;
  this->height = height;
  this->filename = filename;
  this->pixels = (uint8_t*)calloc(1, 3 * width * height);
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &_name);
  glBindTexture(GL_TEXTURE_2D, _name);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  Update();
  error = glGetError();
  if (error != GL_NO_ERROR) valid = false;
 }
 ~Video() {
  glDeleteTextures(1, &_name);
 }
 void Bind(GLenum texture) {
  glActiveTexture(texture);
  glBindTexture(GL_TEXTURE_2D, _name);
 }
 void Update() {
  if (!valid) return;
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _name);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) (int) width, (GLsizei) (int) height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
 }
 void Between() {
  if (stream->NextFrame(pixels) == 1) Update();
 }

};

