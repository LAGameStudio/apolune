#pragma once
#include "GLWindow.h"
#include "GLImage.h"

#include "Crayon.h"
#include "Art.h"
#include "Tweens.h"
#include "Presentation.h"

#include "fx_MixedBag.h"
#include "fx_Glow.h"
#include "fx_Buttons.h"
#include "fx_TexFont.h"
#include "WebPage.h"
#include "WebAPI.h"
#include "Globals.h"
#include "HUD.h"
#include "Engine.h"
#include "LoadingSequence.h"

#include "TitleSequence.h"

#include "fx_List.h"
#include "fx_FireAndForget.h"
#include "CommandLine.h"

#include "DemoMessage.h"

class DeferredHUDLoader : public LoadingSequence {
public:
 Zpointer<SolarSystem> solarsystem;
 Zpointer<SolarSystemEngine> engine;
 Zpointer<GLImage> splash;
 DeferredHUDLoader() : LoadingSequence() {
  splash=library.find("data/images/splash.png");
 }
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(black);
 }
 void Render() {
  QuadiNoAlpha(splash,crayons.Pick(alabaster),none,0,0,display->w,display->h);
 }
 void Load() {  
  CreateHUD();
  hud->visible=true;
  windows.send=hud;
  UnfocusAllUglyTextboxes();
//  windows.Add(demomessage=new DemoMessage);
//  windows.BringToFront(demomessage);
 }
};

class PlayerProfileList : public fx_List<Players,Player> {
public:
 Crayon color, drawing, colorHi, drawingHi,hover,select;
 PlayerProfileList( int x, int y, int w, int h, int ItemHeight, int Margin )
 : fx_List( x, y, w, h, ItemHeight, Margin ) {
  duration=10.0f;
  drawing.Pick(green);
  color.Pick(cyan);
  drawingHi.Pick(orange);
  colorHi.Pick(blue);
  hover.Pick(blue);
  itemBorder.Pick(clear);
  select.Float(0.2f,0.3f,0.9f,1.0f);
  backing=library.Load("data/images/elements/transrounded512borderless.png");
  backingBlend=transparency;
  backingTint.Float(0.0f,0.0f,0.2f,1.0f);
  SetList(&players);
 }
 void RenderItem( Player *player, Cartesian *extents, bool isSelected, bool isHovering ) {
  bool active=isSelected||isHovering;
  if ( active ) {
   QuadiNoAlpha(backing,isSelected?select:hover,transparency,extents->x,extents->y,extents->w,extents->h);
  }
  WriteTexFont(parent,&bytes,(active?drawing:drawingHi),additive,extents->x+16,extents->y+8,18,28,0,(char *)player->profile.username.c_str());
  char buf[64];
  FORMAT(buf,64,"%d character%s",
   (int) player->profile.characters.count,
   player->profile.characters.count==1?"":"s");
  WriteTexFont(parent,&bytes,(active?color:colorHi),additive,extents->x+16,extents->y+32,12,22,0,buf);
 }
 void RenderEmptyList() {
  WriteTexFont(parent,&bytes,drawing,additive,x+32,y+32,12,20,0,"No account profiles yet");
 }
 void OnSelect();
};

class NewPlayerProfileUsername : public fx_UglyTextBox {
public:
 Zpointer<fx_UglyTextBox> pwd;
 NewPlayerProfileUsername( fx_UglyTextBox *p ) : fx_UglyTextBox() {
  maxChars=60;
  x=display.w2-256+8+256;
  y=display.h2+256+8;
  w=128;
  h=128;
  x2=x+w;
  y2=y+h;
  SetStyle(uglyShadowed);
  SetValue(string(""));
  pwd=p;
 }
 void WhenComplete( bool EnterPressed ) {
  pwd->Focus();
  pwd->enterDelay=0.5f;
 }
};

class NewPlayerProfilePassword : public fx_UglyTextBox {
public:
 Zpointer<fx_UglyButton> login;
 NewPlayerProfilePassword() : fx_UglyTextBox() {
  password=true;
  maxChars=60;
  x=display.w2-256+8+256;
  y=display.h2+256+8+16;
  w=128;
  h=128;
  x2=x+w;
  y2=y+h;
  SetStyle(uglyShadowed);
  SetValue(string(""));
 }
 void WhenComplete( bool EnterPressed ) {
  login->WhenClicked();
 }
};

class NewPlayerProfile;
class NewPlayerProfileDoneButton : public fx_UglyButton {
public:
 Zpointer<NewPlayerProfile> npp;
 Zpointer<NewPlayerProfileUsername> username;
 Zpointer<NewPlayerProfilePassword> password;
 NewPlayerProfileDoneButton( int x, int y, NewPlayerProfile *npp, NewPlayerProfileUsername *u, NewPlayerProfilePassword *pwd );
 void WhenClicked();
};

class NewPlayerProfile : public Proce55Viewer2d {
public:
 Zpointer<NewPlayerProfileUsername> username;
 Zpointer<NewPlayerProfilePassword> password;
 Zpointer<NewPlayerProfileDoneButton> done;
 NewPlayerProfile() : Proce55Viewer2d() {}
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  visible=false;
  fx_UglyText *label;
  Add(password=new NewPlayerProfilePassword);
  Add(username=new NewPlayerProfileUsername(password));
  done=new NewPlayerProfileDoneButton(display->w2+256-128-16,display->h2+256+8,this,username,password);
  Add(done);
  password->login=done;
  username->Focus();
  Add(label=new fx_UglyText(display->w2-256+8+8,display->h2+256+8,string("Username:")) );
  label->centered=false;
  Add(label=new fx_UglyText(display->w2-256+8+8,display->h2+256+8+16,string("Password (optional):")) );
  label->centered=false;
  Init();
 }
};

class OldPlayerProfilePassword : public fx_UglyTextBox {
public:
 Zpointer<fx_UglyButton> oppdb;
 OldPlayerProfilePassword() : fx_UglyTextBox() {
  password=true;
  maxChars=60;
  x=display.w2-256+8+128;
  y=display.h2+256+8+32;
  w=128;
  h=128;
  x2=x+w;
  y2=y+h;
  SetValue(string(""));
 }
 void WhenComplete( bool EnterPressed ) { oppdb->WhenClicked(); }
};

class OldPlayerProfile;
class OldPlayerProfilePassword;
class OldPlayerProfileDoneButton : public fx_UglyButton {
public:
 Zpointer<OldPlayerProfile> opp;
 Zpointer<OldPlayerProfilePassword> password;
 OldPlayerProfileDoneButton( int x, int y, OldPlayerProfile *opp, OldPlayerProfilePassword *pwd );
 void WhenClicked();
};

class OldPlayerProfile : public Proce55Viewer2d {
public:
 Zpointer<OldPlayerProfilePassword> password;
 OldPlayerProfile( OldPlayerProfilePassword *oldpw ) : Proce55Viewer2d() {
  password=oldpw;
 }
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  visible=false;
  fx_UglyText *label;
  Add(label=new fx_UglyText(display->w2-256+8+8,display->h2+256+8,string("Enter password:"),ugly3d) );
  Add(password=new OldPlayerProfilePassword);
  Add(password->oppdb=new OldPlayerProfileDoneButton(display->w2-256+8,display->h2+256+8+16,this,password));
  password->Focus();
  Init();
 }
};

class NewPlayerProfileButton : public fx_UglyButton {
public:
 Zpointer<NewPlayerProfile> child;
 NewPlayerProfileButton( int x, int y, NewPlayerProfile *npp ) : fx_UglyButton() {
  normal   = crayons.Pick(chocolate);
  t_normal = crayons.Pick(alabaster);
  f_normal = crayons.Pick(chocolate);
  click    = crayons.Pick(red255);
  t_click  = crayons.Pick(alabaster);
  f_click  = crayons.Pick(red255);
  hover    = crayons.Pick(green);
  t_hover  = crayons.Pick(alabaster);
  f_hover  = crayons.Pick(green);
  backing = library.find("data/images/elements/buttonglow_1024_1.png");
  glow_c  = library.find("data/images/elements/buttonglow_1024_4.png");
  glow_h  = library.find("data/images/elements/buttonglow_1024_4.png");
  glow_n  = library.find("data/images/elements/buttonglow_1024_0.png");
  SetLabel("New Account");
  delay=0.0f;
  duration=20.0f;
  SetSounds("data/sounds/UI/bleep.wav","data/sounds/UI/bonk.wav");
  w=128;
  h=32;
  w2=128;
  h2=32;
  this->x=x;
  this->y=y;
  this->x2=x+w;
  this->y2=y+h;
  this->child=npp;
 }
 void WhenClicked() {
  child->visible=true;
  this->hidden=true;
 }
};

class PlayerProfileSelect : public Proce55Viewer2d {
public:
 Zpointer<NewPlayerProfile> child;
 Cartesian rect;
 Zpointer<GLImage> backing;
 Crayon tint;
 Zbool username_too_short;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  visible=false;
  rect.SetRect(display->w2-256-8,display->h2-256-8,512+16,512+64+16);
  backing=library.Load("data/images/elements/transrounded512borderless.png");
  tint.Pick(black);
  AddChild(child=new NewPlayerProfile);
  Add(new PlayerProfileList(rect.x+16,rect.y+32,512-32,512-64,64,5));
  Add(new NewPlayerProfileButton(display->w2+256-128-16,display->h2+256+8,child));
  Init();
 }
 void PreProcessing() {
  QuadiNoAlpha(backing,tint,transparency,rect.x,rect.y,rect.w,rect.h);
  if ( username_too_short ) {
   glColor3d(1.0,0.0,0.0);
   Text("Usernames must be at least 3 characters.",x+10.0,display->h2+256+8,7.0,10.0,false);
  }
 }
};

class RadioTypeSelect : public fx_IconSelect {
public:
 Zpointer<Character> c;
 void Customize() {
  click=crayons.Pick(red255);
  hover=crayons.Pick(green);
  normal=crayons.Pick(black);
  t_click.Int( 26, 13, 7, 0 );
  t_hover=crayons.Pick(red255);
  t_normal=crayons.Pick(gray);
  f_click=crayons.Pick(red255);
  f_hover=crayons.Pick(blue);
  f_normal.Int( 0, 0, 78, 255 );
  delay=0.0f;
  duration=20.0f;
  backing = library.find( "data/images/elements/rounded.png"),
  glow_c  = library.find( "data/images/elements/rounded.png"),
  glow_h  = library.find( "data/images/elements/rounded.png"),
  glow_n  = library.find( "data/images/elements/rounded.png");
  w=128;
  h=128;
  items.Push("data/images/icons/male.png",  0);  
  items.Push("data/images/icons/female.png",1);
  items.Push("data/images/icons/robot.png", 2);
  items.Push("data/images/icons/alien.png", 3);
  Select(0);
 }
 void WhenClicked();
};

class HomeworldSelect : public fx_IconSelect {
public:
 Zpointer<Character> c;
 void Customize() {
  click=crayons.Pick(red255);
  hover=crayons.Pick(green);
  normal=crayons.Pick(black);  
  t_click.Int( 26, 13, 7, 0 );
  t_hover=crayons.Pick(red255);
  t_normal=crayons.Pick(gray); 
  f_click=crayons.Pick(red255);
  f_hover=crayons.Pick(blue);
  f_normal.Int( 0, 0, 78, 255 );
  delay=0.0f;
  duration=20.0f;
  backing = library.find( "data/images/elements/rounded.png");
  glow_c  = library.find( "data/images/elements/rounded.png");
  glow_h  = library.find( "data/images/elements/rounded.png");
  glow_n  = library.find( "data/images/elements/rounded.png");
 }
 void WhenClicked();
};

class NewCharacterSubmitButton : public fx_UglyButton {
public:
 NewCharacterSubmitButton( int x, int y );
 void WhenClicked();
/*
 * Removes the tildes from a string.
 * Used for player-entered strings that go into disk files.
 */
 void remove_character( string *n, char c) {
  string r=string("");
  const char *str=n->c_str();
  char buf[2]; buf[1]='\0';
  for ( ; *str != '\0'; str++ ) if ( *str != c ) { buf[0]=*str; r=r+string(buf); }
  *n=r;
 }
};

class NewCharacterNameBox : public fx_UglyTextBox {
public:
 NewCharacterNameBox() : fx_UglyTextBox() {
  maxChars=60;
  x=display.w2-256+16+256;
  y=display.h2-256+32;
  w=128;
  h=128;
  x2=x+w;
  y2=y+h;
  SetValue(string(""));
 }
};

class NewCharacter : public Proce55Viewer2d {
public:
 Zpointer<Character> character;
 Zpointer<RadioTypeSelect> rts;
 Zpointer<HomeworldSelect> hs;
 Zpointer<NewCharacterNameBox> nb;
 Zpointer<NewCharacterSubmitButton> ncsb;
 NewCharacter() : Proce55Viewer2d() {}
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  visible=false;
  Repopulate();
 }
 void OnUnload() {
  if ( character ) {
   delete character;
   character=null;
  }
 }
 void Repopulate() {
  fx.Clear();

  if ( !character ) {
   character=new Character;
   character->V=radio.V=&voiceMaleV;
   character->B=radio.B=&voiceMaleB;   
   character->locale=universe->systemByNumber( EARTH_SYSTEM );
  }
  
  fx_UglyText *label=new fx_UglyText(display->w2-256+8+8,display->h2-256+8+32,string("Character name:\n\nFirst, First Last,\nor Title First Last"));
  label->centered=false;
  Add(label);

  Add(nb=new NewCharacterNameBox);

  label=new fx_UglyText(display->w2-256+8+8,display->h2-256+112+32,string("Choose your voice:\n\nPick male, female,\nrobot or alien"));
  label->centered=false;
  Add(label);

  rts =new RadioTypeSelect;
  rts->x=display->w2+32;
  rts->y=display->h2-256+112;
  rts->c=character;
  Add(rts);

//  s.fx.Add( fx_WriteText { x=display->w/2+16, y=96, color=white, label=CopyString( "Homeworld" ), duration=20.0f } );
  label=new fx_UglyText(display->w2-256+8+8,display->h2-256+112+128+32,string("Choose your homeword:\n\nBe from Earth or beyond."));
  label->centered=false;
  Add(label);

  hs=new HomeworldSelect;
  hs->x=display->w2+32;
  hs->y=display->h2-256+112+128+16;
  hs->w=128;
  hs->h=128;
  hs->c=character;
  hs->items.Push("data/images/icons/random.png", 0);  
  hs->items.Push("data/images/icons/earth.png",  1);
  hs->Select(1);
  Add(hs);
 
  
//  s->fx->Add( message=fx_WriteText { x=24, y=72, color=black, label=CopyString( "" ), duration=20.0f } );
  ncsb=new NewCharacterSubmitButton(display->w2+256-128-16,display->h2+256+8);
  ncsb->parent=this;
  ncsb->duration=20.0f;
  Add( ncsb );

  Init();
 }
};


class NewCharacterButton : public fx_UglyButton {
public:
 NewCharacterButton( int x, int y );
 void WhenClicked();
};


class CharacterList : public fx_List<Characters,Character> {
public:
 Crayon color, drawing, colorHi, drawingHi,hover,select;
 CharacterList( int x, int y, int w, int h, int ItemHeight, int Margin )
 : fx_List( x, y, w, h, ItemHeight, Margin ) {
  duration=10.0f;
  drawing.Pick(green);
  color.Pick(cyan);
  drawingHi.Pick(orange);
  colorHi.Pick(blue);
  hover.Pick(blue);
  itemBorder.Pick(clear);
  backing=library.Load("data/images/elements/transrounded512borderless.png");
  backingBlend=transparency;
  backingTint.Float(0.0f,0.0f,0.2f,1.0f);
 }
 void RenderItem( Character *c, Cartesian *extents, bool isSelected, bool isHovering ) {
  bool active=isSelected||isHovering;
  if ( active ) {
   QuadiNoAlpha(backing,isSelected?select:hover,transparency,extents->x,extents->y,extents->w,extents->h);
  }
  WriteTexFont(parent,&bytes,(active?drawing:drawingHi),additive,extents->x+16,extents->y+8,18,28,0,(char *)c->name.c_str());
  char buf[64];
  FORMAT( buf,64, "Location: Star System #%d", (int) c->locale->system_catalog_no );
  WriteTexFont(parent,&bytes,(active?color:colorHi),additive,extents->x+16,extents->y+32,12,22,0,buf);
 }
 void RenderEmptyList() {
  WriteTexFont(parent,&bytes,drawing,additive,x+32,y+32,12,20,0,"No characters yet");
 }
 void OnSelect();
};

class CharacterSelect : public Proce55Viewer2d {
public:
 Zpointer<NewCharacterButton> ncb;
 Zpointer<NewCharacter> nc;
 Zpointer<CharacterList> list;
 Cartesian rect;
 Zpointer<GLImage> backing;
 Crayon tint;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  visible=false;
  rect.SetRect(display->w2-256-8,display->h2-256-8,512+16,512+64+16);
  backing=library.Load("data/images/elements/transrounded512borderless.png");
  tint.Pick(black);
  AddChild(nc=new NewCharacter);
  Add(list=new CharacterList(rect.x+16,rect.y+32,512-32,512-64,64,5));
  Add(ncb=new NewCharacterButton(display->w2+256-128-16,display->h2+256+8));
  Init();
 }
 void PreProcessing() {
  QuadiNoAlpha(backing,tint,transparency,rect.x,rect.y,rect.w,rect.h);
 }
};