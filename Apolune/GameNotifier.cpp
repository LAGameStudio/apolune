#include "GameNotifier.h"

#include "Campaign.h"
#include "Player.h"

Zint stashed_Icon, stashed_OuterBox;


 void GameNotifier::GainedXP( int amount ) {
  if ( amount == 0 ) return;
  if ( amount > 0 ) {
   GameNotification *gn=new GameNotification;
   gn->icon=library.Load("data/images/icon/gain.png");
   gn->sfx=("data/sounds/UI/softRandomize47.wav");
   gn->text1="You gained";
   gn->tint1.Pick(skyBlue);
   gn->text2=FORMAT("%d XP",amount);
   gn->tint2.Pick(cyan);
   notes.Append(gn);
  } else if ( amount < 0 ) {
   GameNotification *gn=new GameNotification;
   gn->icon=library.Load("data/images/icon/loss.png");
   gn->sfx=("data/sounds/UI/Randomize14.wav");
   gn->text1="You lost";
   gn->tint1.Pick(macNcheese);
   gn->text2=FORMAT("%d XP",abs(amount));
   gn->tint2.Pick(gold);
   notes.Append(gn);
  }
  if ( player->exp >= 1000 ) {
   LevelUp();
  }
 }

void GameNotifier::LevelUp() {
 GameNotification *gn=new GameNotification;
 gn->icon=library.Load("data/images/icons/mouth.png");
 gn->sfx=("data/sounds/UI/levelup.wav");
 gn->text1="LEVEL UP";
 gn->tint1.Int(255,96,96,255);
 gn->text2=FORMAT("You are now level %d",(int)(player->level+1));
 gn->tint2.Pick(gold);
 notes.Append(gn);
 player->LevelUp();
}

void GameNotifier::Paid( int amount ) {
 if ( amount > 0 ) {
  GameNotification *gn=new GameNotification;
  gn->icon=library.Load("data/images/icon/paid.png");
  gn->text1=FORMAT("%d credits",amount);
  gn->tint1.Pick(alabaster);
  gn->text2="gained";
  gn->tint2.Pick(green255);
  gn->sfx=("data/sounds/Legacy/sfx_coindrop.wav");
  gn->oldMoney=(player->credits-(double)amount);
  gn->newMoney=(player->credits);
  gn->moneyCounter.TrackValue(&gn->oldMoney.value);
  gn->oldMoney=gn->newMoney;
  gn->showMoney=true;
  notes.Append(gn);
 }
}

void GameNotifier::Charged( int amount ) {
 if ( amount > 0 ) {
  GameNotification *gn=new GameNotification;
  gn->icon=library.Load("data/images/icon/charged.png");
  gn->text1="You paid";
  gn->tint1.Pick(magenta);
  gn->text2=FORMAT("%d credits",amount);
  gn->sfx=("data/sounds/UI/sfx_chaching.wav");
  gn->tint2.Int(255,64,64,255);
  notes.Append(gn);
 }
}



void GameNotification::RenderFromCampaign() {
 if ( vb && vb->hull ) {
  vb->hull->RenderBody(vb->tint,iconBox.CX(),iconBox.CY(),-120.0,1.0);
 }
 if ( vwb ) {
  vwb->RenderBarrel(vwb->barrelTint,iconBox.CX(),iconBox.CY(),-120.0,0.0f);
  vwb->RenderBody(vwb->bodyTint,iconBox.CX(),iconBox.CY(),-120.0);
 }
}