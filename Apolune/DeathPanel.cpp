#include "DeathPanel.h"

#include "GameMenu.h"
#include "LoadGamePanel.h"

void DeathPanel::QuitMenu() {
 destroyMe=true;
 windows.Add(new GameMenu);
}

void DeathPanel::LastCheckpoint() {
 destroyMe=true;
 LoadGamePanel *lgp=new LoadGamePanel;
 lgp->listSelected=players.IndexOf(player);
 windows.Add(lgp);
}