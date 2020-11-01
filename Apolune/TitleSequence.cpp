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
#include "TitleSequence.h"

#include "TitleMenu.h"
#include "Globals.h"
#include "GameOptions.h"

Zpointer<TitleSequence> titles;

 bool TitleSequence::OnComplete()
 {
  this->Rewind(false);
  Presentation *p=(Presentation *) this->children.first;
  if ( p ) p->Rewind(false);
  mainmenu->Repopulate();
  return false;
 }

 void TitleSequence::RightDown() {
  return;
 }
 
 void TitleSequence::OnLoad() { 
  Slide *s;   
//  VisualAidAdditive *v,*o;
//  Proce55or *p;
//  fx_Glow *glow;
//  fx_MovingLine *line;
  int i;//,j,k;
  visible=false;
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(black);
//  Credits *c;
//  AddChild( c=new Credits( display, input, audio ) );
//  if ( options->ShowCredits ) c->visible=true;
//  else c->visible=false;

  xStar.Size(NUM_STARS);
  yStar.Size(NUM_STARS);
  for ( i=0; i<NUM_STARS; i++ ) {
   xStar[i]=(float) ((int) (uniform() * display->wd));
   yStar[i]=(float) ((int) (uniform() * display->hd));
  }

  letterbox=false;
  //debugmode=4;//debugmode=1;//2;//3;

  Add( s=new Slide(900.0f) );

#if defined(SHOW)

//  s->sounds.Add( new SoundEvent("data/sounds/music/jsbach2.wav",null,0,false) );

#endif

  Init(true);
 }