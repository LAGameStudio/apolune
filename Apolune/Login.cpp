#include "Login.h"

#include "Crayon.h"
#include "TitleSequence.h"
#include "TitleMenu.h"
#include "HUD.h"
#include "Engine.h"
#include "Player.h"
#include "Universe.h"

extern int failures;

//extern GameOptions options;
extern Radio radio;
extern RadioTransmissions voiceMaleV;
extern RadioTransmissions voiceMaleB;
extern RadioTransmissions voiceFemaleV;
extern RadioTransmissions voiceFemaleB;
extern RadioTransmissions voiceRobomaleV;
extern RadioTransmissions voiceRobomaleB;
extern RadioTransmissions voiceAlienV;
extern RadioTransmissions voiceAlienB;

Zpointer<CharacterSelect> characterselect;
Zpointer<PlayerProfileSelect> playerprofileselect;

void PlayerProfileList::OnSelect() {
 players.active=selected;
 playerprofileselect->visible=false;
 if ( !characterselect ) {
  characterselect=new CharacterSelect;
  windows.Add(characterselect);
 }
 characterselect->list->SetList(&selected->profile.characters);
 characterselect->list->selectDelay=0.5f;
 characterselect->visible=true; 
 windows.BringToFront(characterselect);
}

NewPlayerProfileDoneButton::NewPlayerProfileDoneButton
 ( int x, int y,
   NewPlayerProfile *npp,
   NewPlayerProfileUsername *u, 
   NewPlayerProfilePassword *pwd )
 : fx_UglyButton() {
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
  SetLabel("Login");
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
  username=u;
  password=pwd;
  this->npp=npp;
 }

void NewPlayerProfileDoneButton::WhenClicked() {
 if ( username->out.length < 3 ) {
  playerprofileselect->username_too_short=true;
  return;
 }
 const char *t=username->out.c_str();
 int badChars=0;
 while ( *t != '\0' ) {
  if ( isspace(*t) ) badChars++;
  t++;
 }
 if ( username->out.length-badChars < 3 ) {
  playerprofileselect->username_too_short=true;
  return;
 }
 Player *player=new Player;
 player->profile.username=username->out;
 player->profile.password=password->out;
 players.Append(player);
 players.active=player;
 this->hidden=true;
 playerprofileselect->visible=false;
 playerprofileselect->username_too_short=false;
 if ( !characterselect ) {
  characterselect=new CharacterSelect;
  windows.Add(characterselect);
 }
 characterselect->visible=true;
 characterselect->nc->visible=true;
 characterselect->list->hidden=true;
 characterselect->ncb->hidden=true;
 characterselect->nc->nb->Focus();
 windows.BringToFront(characterselect);
}

OldPlayerProfileDoneButton::OldPlayerProfileDoneButton
 ( int x, int y,
   OldPlayerProfile *opp,
   OldPlayerProfilePassword *pwd )
 : fx_UglyButton() {
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
  password=pwd;
  this->opp=opp;
 }

void OldPlayerProfileDoneButton::WhenClicked() {
 if ( contains(players.active->profile.password.c_str(),password->out.c_str()) || CmdLine.Option("-crack") ) {
  playerprofileselect->visible=false;
  if ( !characterselect ) {
   characterselect=new CharacterSelect;
   windows.Add(characterselect);
  }
 }
 windows.BringToFront(characterselect);
}

NewCharacterSubmitButton::NewCharacterSubmitButton( int x, int y ) : fx_UglyButton() {
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
  SetLabel("Create");
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
 }

 void NewCharacterSubmitButton::WhenClicked() {
  NewCharacter *nc=(NewCharacter *) parent.pointer;
  nc->character->name=characterselect->nc->nb->out;
  if ( nc->character->name.length < 3 ) {
   return;
  }
  remove_character( &nc->character->name, '*' );
  remove_character( &nc->character->name, '"' );
  remove_character( &nc->character->name, '&' );
  remove_character( &nc->character->name, ';' );
  remove_character( &nc->character->name, ':' );
  remove_character( &nc->character->name, '{' );
  remove_character( &nc->character->name, '}' );
  remove_character( &nc->character->name, '[' );
  remove_character( &nc->character->name, ']' );
  remove_character( &nc->character->name, '%' );
  remove_character( &nc->character->name, '$' );
  remove_character( &nc->character->name, '#' );
  remove_character( &nc->character->name, '@' );
  remove_character( &nc->character->name, '!' );
  remove_character( &nc->character->name, '~' );
  remove_character( &nc->character->name, '_' );
  remove_character( &nc->character->name, '\n' );
  remove_character( &nc->character->name, '\r' );
  remove_character( &nc->character->name, '\t' );
  if ( !nc->character->locale )
   nc->character->locale = 
    characterselect->nc->hs->selected ? universe->systemByNumber( EARTH_SYSTEM ) 
                                      : universe->systemByNumber( number_range(0,EARTH_SYSTEM) );
  players.active->profile.characters.Append(nc->character);
  players.active->profile.active=nc->character;
  if ( hud ) hud->mc->TrackValue(&nc->character->money.value);
  players.Save();
  OUTPUT( "Selected new character named '%s'\n", nc->character->name.c_str() );
//  nc->character=null;
//  nc->Repopulate();
  characterselect->visible=false;  
  windows.Add(new DeferredHUDLoader);
 }
 
void RadioTypeSelect::WhenClicked() {
 switch ( selected->v.value ) {
  case 0: c->V=radio.V=&voiceMaleV;     c->B=radio.B=&voiceMaleB;    
          audio.manager.CueAndPlay( "data/sounds/radio/male/Salutations/hello.wav", 1.0f, 1.0f, false, true );
   break;
  case 1: c->V=radio.V=&voiceFemaleV;   c->B=radio.B=&voiceFemaleB;   
          audio.manager.CueAndPlay( "data/sounds/radio/female/Salutations/hello.wav", 1.0f, 1.0f, false, true );
   break;
  case 2: c->V=radio.V=&voiceRobomaleV; c->B=radio.B=&voiceRobomaleB; 
          audio.manager.CueAndPlay( "data/sounds/radio/robomale/Salutations/hello.wav", 1.0f, 1.0f, false, true );
   break;
  case 3: c->V=radio.V=&voiceAlienV;    c->B=radio.B=&voiceAlienB; 
          audio.manager.CueAndPlay( "data/sounds/radio/alien/goodbye.wav", 1.0f, 1.0f, false, true );
   break;
 }
}

void HomeworldSelect::WhenClicked() {
 switch ( selected->v.value ) {
  case 0: c->locale = universe->systemByNumber( number_range( 0, 10000 ) );
   break;
  case 1: c->locale = universe->systemByNumber( EARTH_SYSTEM );
   break;
 }
}


NewCharacterButton::NewCharacterButton ( int x, int y ) : fx_UglyButton() {
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
  SetLabel("New Character");
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
 }

void NewCharacterButton::WhenClicked() {
 characterselect->nc->visible=true;
 characterselect->nc->nb->Focus();
 characterselect->list->hidden=true;
 this->hidden=true;
}

void CharacterList::OnSelect() {
 OUTPUT( "Selected character named '%s'\n", selected->name.c_str() );
 players.active->profile.active=selected;
if ( hud ) hud->mc->TrackValue(&selected->money.value);
 mainmenu->Repopulate();
 mainmenu->visible=false;
 if ( mainmenu->menu ) mainmenu->menu->visible=false;
 titles->Rewind(true);
 titles->visible=false;
 characterselect->visible=false;
 windows.Add(new DeferredHUDLoader);
}