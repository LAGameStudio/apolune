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
#include "Proce55ors.h"

class OrderedProce55Viewer : public GLWindow {
public:
 Proce55ors fx;
 Zpointer<Proce55or> active;
 Zfloat betweenDelay,delay;
 virtual void OnLoad() {
 }
 void Init() {
  active=(Proce55or *) fx.first;
  fx.Go();
 }
 void Rewind() {
  fx.Reset(false);
  Init();
 }
 void Between() {
  if ( !active ) return;
  if ( active->Done() ) {
   active=(Proce55or *) active->next;
   delay=betweenDelay;
   if ( !active ) return;
  }
  if ( delay > 0.0f ) delay-=FRAMETIME;
  else
  active->Between();
 }
 void Render() {
  if ( delay <= 0.0f && active ) {
   active->Render();
  }
 }
 void Add( Proce55or *p ) {
  fx.Add(p,this);
 }
};

