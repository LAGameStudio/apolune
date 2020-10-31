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
#include "Cartesian.h"
#include "Display.h"

// Note: Stored clip stack values must use an inverted Y value and not the 
// direct GLWindow value for Y.
class GLClipStack : public Cartesians {
public:
 Cartesians recycling;
 Zbool enabled;
 GLClipStack() : Cartesians() {}
 void Enable() {
  glEnable(GL_SCISSOR_TEST);
  enabled=true;
 }
 void Push( Zint x, Zint y, Zint w, Zint h ) {
  Push((int)x,(int)y,(int)w,(int)h);
 }
 void Push( int x, int y, int w, int h ) {
  Cartesian *c=null;
  if ( recycling.first ) {
   c=(Cartesian *) recycling.first;
   recycling.Remove(c);
  } else c=new Cartesian;
  c->x=x;
  c->y=y;
  c->w=w;
  c->h=h;
  Append(c);
  if ( !enabled ) Enable();        // v- wtf, this is not a solution here.. basically sometimes (on FBOs) we have to invert Y for some reason :(
  glScissor(x,( count > 1 ? display.h-(y+h+1) : y ),w,h+1);
 }
 void Pop() {
  if ( !last ) {
   OUTPUT("GLClipStack:Pop() tried to pop when nothing was poppable.  Ignoring.\n");
   return;
  }
  Cartesian *c=(Cartesian *) last;
  Remove(last);
  recycling.Append(c);
  if ( !last ) Disable();
  else {
   Cartesian *top=(Cartesian *) last;   // v- wtf
   glScissor(top->x, ( count > 1 ? display.h-(top->y+top->h+1) : top->y ),top->w,top->h+1);
  }
 }
 void Disable() { glDisable(GL_SCISSOR_TEST); enabled=false; }
 CLEARLISTRESET(Cartesian);
 ~GLClipStack() { Clear(); }
};

extern GLClipStack scissors;