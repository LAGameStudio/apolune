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
#include <Windows.h>

#include "GLImage.h"
#include "GLWindow.h"
#include "LinkedList.h"
#include "DisplayList.h"

#include "TextureLibrary.h"

class DisplayListTest : public DisplayList {
public:
 GLImage *source;
 void Instructions() {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, source->texture);
  glColor4f(1.0f,1.0f,0.0f,1.0f);
  glBegin(GL_QUADS);
  glVertex2i(10,10);
  glVertex2i(512,10);
  glVertex2i(512,512);
  glVertex2i(10,512);
  glEnd();
 }
};

class DisplayListTest2 : public DisplayList {
public:
 void Instructions() {
  glBegin(GL_LINES);
  glColor4f(0.0f,1.0f,0.0f,1.0f);
  glVertex2i(20,20);
  glVertex2i(512,512);
  glEnd();
 }
};

class DisplayListTest3 : public DisplayList {
public:
 void Instructions() {
  glBegin(GL_LINES);
  glColor4f(0.0f,1.0f,0.0f,1.0f);
  glVertex2i(30,30);
  glVertex2i(222,222);
  glEnd();
 }
};

class DisplayListTest4 : public DisplayList {
public:
 void Instructions() {
  glBegin(GL_LINES);
  glColor4f(0.0f,1.0f,1.0f,1.0f);
  glVertex2i(40,40);
  glVertex2i(220,512);
  glEnd();
 }
};


class DisplayListTest5 : public DisplayList {
public:
 void Instructions() {
  glBegin(GL_LINES);
  glColor4f(1.0f,0.0f,1.0f,1.0f);
  glVertex2i(50,50);
  glVertex2i(220,260);
  glEnd();
 }
};

class GLDisplayListTest : public GLWindow
{
public:
 bool dragging;
 int sx,sy;
 DisplayListTest2 single;
 DisplayLists one,many,manyAsOne;
 void OnLoad(void) {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  dragging=false;
  background.Pick(clear);
  single.Upload();
  DisplayListTest *test = new DisplayListTest;
  test->source=library.find("data/images/elements/rounded_1024.png");
  one.Append(new DisplayListTest2);
  one.UploadLists();
  many.Append(test);
  many.Append(new DisplayListTest3);
  many.UploadLists();
  manyAsOne.Append(new DisplayListTest4);
  manyAsOne.Append(new DisplayListTest5);
  manyAsOne.UploadAsOne();
 }
 void Render() {
  single.Call();
  one.Call();
  many.Call();
  manyAsOne.Call();
 }
 ~GLDisplayListTest(void);
};

