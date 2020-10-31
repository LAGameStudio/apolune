/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once

#include <Windows.h>

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "Display.h"
#include "Interface.h"
#include "WebPage.h"

#include "Globals.h"

#if defined(USE_CURL)

extern char web_buffer[65536];
extern bool WaitingForState;
enum States { 
 precursor=-1,
 initial=0,
 authenticated=1,
 bad_auth=2,
 character_select=3,
 player_is_playing=4
};
extern States CLIENT;
extern States WAS;

class AuthRequest : public WebPage {
public:
 AuthRequest() { }
 bool OnResult()
 {
   const char *a;
   char buf[1024];
   if ( lines.count == 0 ) return true;
   a=one_argcase(data.memory,buf);
   if ( !str_cmp( buf, "AUTH" ) ) {
   OUTPUT( "Authenticated.\n" );
    web_buffer[0]='\0';
    FORMAT( web_buffer,65536, "%s", a );
    CLIENT=authenticated;
    WaitingForState=false;       
    return false;
   }
   return true;
 }

 void Result();
};

#endif