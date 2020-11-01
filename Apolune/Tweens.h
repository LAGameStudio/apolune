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
#include <iostream>
#include <string>
using namespace std;
#include "LinkedList.h"
#include "Tween.h"

class Tweens : public LinkedList {
public:
 Tweens(void);
 void Load(void);
 CLEARLISTRESET(Tween);

 void Initialize(void) { 
  FOREACH(Tween,t) {
   t->Generate(); 
   t->PreRender();
  }
 }

 void PreRender(void) {
  FOREACH(Tween,t) {
   t->Generate(); 
   t->PreRender();
  }
 }

 Zp<Tween> one;
 Tween *One() { return one; }

 Tween *find( string n ) {  
  FOREACH(Tween,t) if ( !str_cmp(t->name.c_str(),n.c_str()) ) return t;
  OUTPUT("Tween not found `%s` explicitly, executing fallback match\n",n.c_str());
  return match(n);
 }

 Tween *match( string n ) {
  FOREACH(Tween,t) if ( !str_prefix(n.c_str(),t->name.c_str()) ) return t;
  OUTPUT("Tween not matched `%s` as prefix, searching as partial...\n",n.c_str());
  return partial(n);
 }
 Tween *partial( string n ) {
  FOREACH(Tween,t) if ( !str_infix(n.c_str(),t->name.c_str()) ) return t;
  OUTPUT("Tween not matched `%s` as partial.  Returning null.\n",n.c_str());
  return null;
 }

 Tween *random(void) {
  int c=number_range(0,this->count);
// OUTPUT("Tweens::random chose %d\n", c );
  return (Tween *) (this->Element(c)); 
 }

 Tween *randomNot1(void) {
  if (count == 0) return null;
  while ( 1 ) {
   double reseeder=uniform(true);
   int c=number_range(0,this->count);
   if (c >= this->count) continue;
   Tween *t=(Tween *) (this->Element(c));
   if ( !t ) continue;   
   if ( t!=one ) return t;
  }
  return null;
 }

 Tween *seeded( int seed ) {
  Random *set=(Random *) randoms.first;
  int c=set->range(seed,0,this->count);
// OUTPUT("Tweens::seeded chose %d\n", c );
  return (Tween *) (this->Element(c)); 
 }

 Tween *seededNot1( int seed ) {
  Random *set=(Random *) randoms.first;
  int i=0;
  while ( 1 ) {
   int c=set->range(seed+i,0,this->count);
   Tween *t=(Tween *) (this->Element(c));
   if ( !t ) continue;   
   if ( t!=one ) return t;
   i++;
  }
  return null;
 }

 float tween1024f( float a, float *tween );  // backwards compatibility here...

};

extern Tweens tweens;

#include "ImageProcess.h"

class TweenToTexture : public ProceduralImage {
public:
 Tween *t;
 int line,pixel;
 TweenToTexture() : ProceduralImage() {
  t=null;
  line=0;
  pixel=0;
 }
 void Loop() {
  if ( !t ) t=(Tween *) tweens.first;
  GLubyte value=(GLubyte)(t->tween[pixel]*255.0f);
  bgra_out[0]=
  bgra_out[1]=
  bgra_out[2]=value;
  bgra_out[3]=255;
  pixel++;
  if ( pixel >= 1024 ) {
   line++;
   t=(Tween *)t->next;
   pixel=0;
  }
 }
};

HANDLED(Tween,TweenHandle,TweenHandles,ThweenHandlesHandle,TweenHandlesHandles);

void TweensToTexture();