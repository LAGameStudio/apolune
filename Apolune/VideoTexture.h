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

