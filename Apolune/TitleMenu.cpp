#pragma once
#include "TitleMenu.h"

#include "Globals.h"
#include "GameOptions.h"

#include "TouchTest.h"

Zpointer<MainMenu> mainmenu;

extern Display display;
extern Interface input;
extern GLWindowManager windows;

bool menu_will_exit=false;

bool MainMenuActual::OnComplete()
{
 current=null;
 Rewind(true);
 visible=false;
 if ( manager->send == this ) manager->send = mainmenu;
 return false;
}

void LoginButton::WhenClicked() {
 mainmenu->stayClosed=true;
 mainmenu->menu->Seek(3);
 windows.Add(new TouchTest);
} 

 void OptionsButton::WhenClicked() {
 } 

 void ExitButton::WhenClicked() {
  mainmenu->menu->Seek(3);
  menu_will_exit=true;
  exit(1); // hacky.. why didn't this work in the OnComplete?
 }

 void MainMenuActual::OnLoad()
 {
  Anchor( 0.60f, 0.30f, 0.375f, 0.4f /*375*/ );
  background.Pick(clear);

  Slide *s=new Slide(1.25f);
  Add(s);

  VisualAid *v;
  int i;
  
  // Menu Open Animation
  v=new VisualAid;
  v->image = library.find( "data/images/elements/rounded_less_1024.png");
  v->duration=0.25f;
  v->in=0.25f;
  v->w=0; v->w2=this->w;
  v->h=0; v->h2=10;
  v->sx=this->w/2;
  v->sy=this->h/2;
  v->dx=-v->sx;
  v->dy=-5;
  v->Opacity(0);
  s->fx->Add(v,this);

  v=new VisualAid;
  v->image = library.find( "data/images/flares/ball8.png");
  v->duration=0.46f;
  v->in=0.22f; v->out=0.34f;
  v->w=20; v->w2=40;
  v->h=20; v->h2=40;
  v->sx=this->w/2;
  v->sy=this->h/2;
  v->dx=-20;
  v->dy=-20;
  v->Opacity(0);
  v->Tints(crayons.Pick(black),crayons.Pick(chocolate),crayons.Pick(black));
  s->fx->Add(v,this);

  v=new VisualAid;
  v->image = library.find( "data/images/elements/rounded_less_1024.png");
  v->delay=0.25f;
  v->duration=0.5f;
  v->in=1.0f;
  v->w=v->w2=this->w;
  v->h=10;
  v->h2=this->h;
  v->sx=0;
  v->sy=this->h/2-5;
  v->dx=-v->sx;
  v->dy=-v->sy;
  v->Opacity(0);
  v->Tints(crayons.Pick(alabaster),crayons.Pick(chocolate),crayons.Pick(black));
  s->fx->Add(v,this);   
 
  // Background is now 'static'
  v=new VisualAid;
  v->image = library.find( "data/images/elements/rounded_less_1024.png");
  v->delay=0.75f;
  v->duration=0.5f;
  v->in=1.0f;
  v->w=v->w2=this->w;
  v->h=v->h2=this->h;
  v->sx=v->sy=0;
  v->dx=v->dy=0;
  v->Opacity(0);
  v->Tints(crayons.Pick(chocolate),crayons.Pick(chocolate),crayons.Pick(black));
  s->fx->Add(v,this);
  
  // Button backing growth
  for ( i=0; i <3; i++ ) {
   int buttonHeight=(this->h/3);
   v=new VisualAid;
   v->image = library.find( "data/images/elements/buttonglow_1024_1.png");
   v->delay=0.75f;
   v->duration=0.5f;
   v->in=0.5f;
   v->w=0; v->w2=(this->w);
   v->h=0; v->h2=(this->h/3);
   v->sx=this->w/2;
   v->sy=(buttonHeight*i)+(buttonHeight/2);
   v->dx=-v->sx;
   v->dy=-buttonHeight/2;
   v->Opacity(0);
   v->Tints(crayons.Pick(black),crayons.Pick(chocolate),crayons.Pick(black));
   s->fx->Add(v,this);      
  }        

  s = new Slide(30.0f);
  s->seek=3;
  s->rewind=true;
  s->unpause=true;
  Add(s);

  // "Opened" Menu Background Area
  v=new VisualAid;
  v->image = library.find( "data/images/elements/rounded_less_1024.png");
  v->delay=0.0f;
  v->duration=60.0f;                                                                                                                            
  v->in=0.0f; v->out=58.0f;
  v->Opacity(0);
  v->Tints(crayons.Pick(chocolate),crayons.Pick(chocolate),crayons.Pick(black));
  v->w=v->w2=this->w;
  v->h=v->h2=this->h;           
  s->fx->Add(v,this);

  Tween
   *pulsing=tweens.find("Humanized Gradual Rise"),                                        //
   *gyrating=tweens.find("Humanized In Out Slow"),                       ///   <Capn        //
   *flashing=tweens.find("Humanized Wobble Rise");                     //////                 //

  for ( i=0; i <3; i++ ) {
   fx_CoolButton *b;
   int buttonHeight=(this->h/3);
   switch ( i ) {
     case 0: /* Login */
         b=(fx_CoolButton *) (new LoginButton);
      break;
     case 1: /* Options */
         b=(fx_CoolButton *) (new OptionsButton);
      break;
     case 2: /* Exit */
         b=(fx_CoolButton *) (new ExitButton);
      break;
   }
   b->normal=crayons.Pick(chocolate);
   b->t_normal=crayons.Pick(alabaster);
   b->f_normal=crayons.Pick(chocolate);
   b->click=crayons.Pick(red255);
   b->t_click=crayons.Pick(alabaster);
   b->f_click=crayons.Pick(red255);
   b->hover=crayons.Pick(green);
   b->t_hover=crayons.Pick(alabaster);
   b->f_hover=crayons.Pick(green);
   b->backing = library.find("data/images/elements/buttonglow_1024_1.png");
   b->glow_c  = library.find("data/images/elements/buttonglow_1024_3.png");
   b->glow_h  = library.find("data/images/elements/buttonglow_1024_4.png");
   b->glow_n  = library.find("data/images/elements/buttonglow_1024_0.png");
//   b->buttontext =buttontexts.image(i);// library.find(  buttontexts[i]);                      //
   b->controller.variables.NewfT("chi",  0.0f, 255.0f, 0.0625f,flashing);
   b->controller.variables.NewfT("clo",  0.0f, 255.0f, 0.0625f,flashing);                  // OMFG ASCII Space ship!
   b->controller.variables.NewfT("ct",   0.0f, 255.0f, 0.0625f,flashing);
   b->controller.variables.NewfT("hhi",127.0f, 255.0f, 0.0625f,pulsing); 
   b->controller.variables.NewfT("hlo",  0.0f, 255.0f, 1.0f,gyrating);
   b->controller.variables.NewfT("ht",   0.0f, 255.0f, 1.0f,gyrating);
   b->controller.variables.NewfT("hi",   0.0f, 255.0f, 0.0f,null); //////////////////////
   b->controller.variables.NewfT("lo",   0.0f, 255.0f, 1.0f,null); 
   b->controller.variables.NewfT("t",    0.0f, 255.0f, 0.0f,null); ///////////////////
   b->delay=0.0f;
   b->duration=60.0f;
   b->w=(this->w);
   b->h=buttonHeight;
   b->x=0;
   b->y=buttonHeight*i;          
   s->fx->Add(b,this);
  } 

  // Fading out
  s=new Slide( 2.0f );
  Add(s);
  // Background is now 'static'
  v=new VisualAid;
  v->image = library.find( "data/images/elements/rounded_less_1024.png");
  v->delay=0.0f;
  v->duration=0.5f;
  v->in=0.0f; v->out=0.0f;
  v->w=v->w2=this->w;
  v->h=v->h2=this->h;
  v->sx=v->sy=0;
  v->dx=v->dy=0;
  v->Opacity(0);
  v->Tints(crayons.Pick(alabaster),crayons.Pick(chocolate),crayons.Pick(black));
  s->fx->Add(v,this);
  
  for ( i=0; i <3; i++ ) {
   int buttonHeight=(this->h/3);
   v=new VisualAid;
   v->image = library.find( "data/images/elements/buttonglow_1024_1.png");
   v->delay=0.0f;
   v->duration=0.5f;
   v->in=0.5f;
   v->w2=0; v->w=(this->w);
   v->h2=0; v->h=(this->h/3);
   v->sx=0;
   v->sy=(buttonHeight*i);
   v->dx=this->w/2;
   v->dy=buttonHeight/2;
   v->Opacity(0);
   v->Tints(crayons.Pick(black),crayons.Pick(chocolate),crayons.Pick(black));
   s->fx->Add(v,this);      
  }

  Init(false);
 }