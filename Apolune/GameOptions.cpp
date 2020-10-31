#include "GameOptions.h"

GameOptions options;
Zp<GameOptions> gameOptions;

void GameOptions::SetGlobalPointer() {
 gameOptions.pointer=&options;
}

#include "Player.h"
#include "GameMenu.h"
void GameOptionsPanel::ReturnToGameMenu() {
//    players.Save();   Don't call here, saves elsewhere. 
 back_to->destroyMe=true;
 destroyMe=true;
 activeEngineWindow=null;
 options.Save();
 vgui.SaveState(statefile.c_str());
 // Instead of saving, load the last saved version to nulify progress in ReturnToGameMenu
 players.Clear();
 player=null;
 windows.DestroyAll();
 windows.Add(new GameMenu);
}

void GameOptionsPanel::ResumeGame() {
 back_to->visible=true;
 destroyMe=true;
 options.Save();
 vgui.SaveState(statefile.c_str());
}