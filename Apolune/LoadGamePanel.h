#pragma once

#include "GLWindow.h"
#include "Player.h"
#include "fx_Teletype.h"
#include "GameDereference.h"

class LoadGamePanel : public MoveableWindowManager {
public:
 Zp<GLWindow> back_to;
 Delay saveStateInterval;
 Zp<GameOptionsEditor> oe;
 Strings list;
 Zint listSelected,listScrolled;
 Zp<Player> wasSelected;
 fx_Teletype textType,rightText;
 LoadGamePanel() : MoveableWindowManager() {
  autoarrange=false;
  vgui.drawWindowControls=false;
  list.Clear();
  EACH(players.first,Player,p) {
   list.Add(
    FORMAT("%-30s  Level %3d  XP %10d",
     (p->firstName+string(" ")+p->lastName).c_str(),
     (int)p->level, (int)p->exp
    )
   );
  }
  list.ReverseOrder();
  wasSelected=(Player *)players.last;
  Repopulate();
  textType.SetExtents(16+256+16,64,w-32-256-16,256);
  textType.delay=1;
  textType.stayOnDone=true;
  rightText.SetExtents(16+256+16+640,64,w-32,256);
  rightText.delay=5;
  rightText.stayOnDone=true;
 }
 void Repopulate() {
  if ( !wasSelected ) {
   textType.WriteText("");
   rightText.WriteText("");
   return;
  }
  textType.delay=1;
  rightText.delay=5;
  textType.WriteText(FORMAT("NAME: %s %s ^\\RANK: %s ^\\XP: %d^\\^\\%s^\\",
    wasSelected->firstName.c_str(),wasSelected->lastName.c_str(),wasSelected->rank.c_str(),
    wasSelected->exp.value,
    wasSelected->vehicle?wasSelected->vehicle->ship_name.c_str():""
   ).c_str(), false
  );
  GameDereference gd;
  gd.Reference(wasSelected->reloadHere.c_str());
  if ( gd.location )
  rightText.WriteText(FORMAT("LOCATION:^\\%s^\\SD %d-%d-%d",
    gd.location->named.c_str(),
    (int)wasSelected->stardateMonth, (int)wasSelected->stardateDay, (int)wasSelected->stardateYear
   ).c_str(), false
  );
  else if ( gd.interior )
  rightText.WriteText(FORMAT("LOCATION:^\\%s^\\SD %d-%d-%d",
    gd.interior->caption.c_str(),
    (int)wasSelected->stardateMonth, (int)wasSelected->stardateDay, (int)wasSelected->stardateYear
   ).c_str(), false
  );
 }
 void RenderBackground() {
  fast.ResponsiveRatio(fbo->w,fbo->h);
  fast.ljust(digicaps,"Load Checkpoint",16,32,32,32,2,3,crayons.Pick(aliceBlue),additive);
  int selected=fast.list(glowTermFont,"",&list,10,64+12*24-8,fbo->w-64,14,&listSelected.value,&listScrolled.value,0,0);
  if ( selected != -1 ) {
   wasSelected=(Player *)players.Element(players.count-listSelected-1);
   Repopulate();
  }
 }
 void RenderAfterWindow();
};