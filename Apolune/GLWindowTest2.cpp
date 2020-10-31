
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include <Windows.h>
#include <math.h>

#include "GLWindow.h"
#include "GLImage.h"

#include "GLWindowTest2.h"
#include "Art.h"

GLWindowTest2::GLWindowTest2(void)
{
 color.floats[0]=1.0f;
 color.floats[3]=1.0f;
}


GLWindowTest2::~GLWindowTest2(void)
{
}

void GLWindowTest2::Render(void) {
 Crayon color2;
 color2.floats[3]=0.5f;
 color2.floats[1]=1.0f;
 Rectangle(this,color,20,20,30,30);
 Area(this,color,5,5,10,10);
 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 Area(this,color2,7,7,15,15);
 glDisable(GL_BLEND);
}

void GLWindowTest2::MouseMove(void) {
 float X=((float)(input->mx-x)/(float)w);
 float Y=((float)(input->my-y)/(float)h);
 background.Float(X,Y,1.0f,(X+Y)/2.0f);
}

void GLWindowTest2::MouseEnter(void) {
 background.Float(0.5f,0.5f,0.5f,0.5f);
}

void GLWindowTest2::MouseLeave(void) {
 background.Float(1.0f,1.0f,1.0f,1.0f);
}