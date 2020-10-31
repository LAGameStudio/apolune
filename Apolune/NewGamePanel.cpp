#include "NewGamePanel.h"

#include "GameMenu.h"
 void NewGamePanel::ReturnToGameMenu() {
  destroyMe=true;
  windows.Add(new GameMenu);
 }