#include "LoadGamePanel.h"

#include "GameMenu.h"

void LoadGamePanel::RenderAfterWindow() {
 if ( listSelected >= 0 && fast.button(digicaps,"Load Selected",fbo->w-768-32,fbo->h-64,768,48) ) {
  audio.manager.CueAndPlay("data/sounds/UI/tinybeep.wav",1.0f,0.8f,false,true);
  Player *p=(Player *)players.Element(players.count-listSelected-1);
  if ( p ) {
   player=p;
   player->ReloadIntoWorld();
   destroyMe=true;
   return;
  }
 }
 if ( fast.button(fast.leftArrow,fbo->w-64-8,8,48,48,false) ) {
  destroyMe=true;
  windows.Add(new GameMenu);
  return;
 }
 textType.Between();
 textType.Render();
 rightText.Between();
 rightText.Render();
 if ( wasSelected ) {
  Blending(none);
  fast.bestrect(crayons.Pick(rgb339),16,64,256,256);
  wasSelected->portrait.Render(16,64,256,256,false,true);
 }
}