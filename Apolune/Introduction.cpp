/***********************************************************************************************
 * ____ _  _ ___  _ ____ ____    _ _  _    ___ _  _ ____    ____ _  _ _   _ (c)2009-2011       *
 * |___ |\/| |__] | |__/ |___    | |\ |     |  |__| |___    [__  |_/   \_/  Lost Astronaut     *
 * |___ |  | |    | |  \ |___    | | \|     |  |  | |___    ___] | \_   |   Studios + Ganos.biz*
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/
#include "Introduction.h"

#include "GLWindow.h"
#include "TitleMenu.h"
#include "TitleSequence.h"
#include "GameOptions.h"

bool Introduction::OnComplete()
 {
  if ( current ) current->sounds.Stop();
  current=null;
  mainmenu->cursor=false;
  mainmenu->visible=true;
  mainmenu->Repopulate();
  windows.send=mainmenu;
  titles->visible=true;
  Presentation *p=(Presentation *) titles->children.first;
  if ( p ) {
   if ( p->paused ) p->Pause(); // unpauses child of title (credits)
   p->visible=true;
  }
  if ( titles->paused ) titles->Pause(); //unpauses
  visible=false;
  Pause();
  return true;
 }