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
#include "TexFont.h"

class TexFontTest : public GLWindow
{
public:
 Crayon drawing;
 Zpointer<TexFont> active;
 Zfloat colorchangedelay;
 Blends blend;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  active=digicaps;
  colorchangedelay=10.0f;
  blend=additive;
  drawing.Pick(alabaster);
 }
 void Between() {
  if ( colorchangedelay > 0.0f ) colorchangedelay-=FRAMETIME;
  else {
   drawing.Any();
   colorchangedelay=10.0f;
  }
 }
 void Render() {
  WriteTexFont(active,drawing,blend,50,50,32,32,5,
   "ABCDEFGHI\n"
   "JK LMNOPQ\n"
   "RSTUVWXYZ\n"
   "$0123456789\n"
   "#1234ABCD\n"
   "!@#$%^&*()_+|\n"
   "\\=-0/'\"`.,?\n"
   "><[]{}()"
  );
  WriteTexFont(active,drawing,blend,600,50,16,16,5,
   "ABCDEFGHI\n"
   "JK LMNOPQ\n"
   "RSTUVWXYZ\n"
   "$0123456789\n"
   "#1234ABCD\n"
   "!@#$%^&*()_+|\n"
   "\\=-0/'\"`.,?\n"
   "><[]{}()"
  );

  WriteTexFont(active,drawing,blend,300,300,8,8,5,
   "ABCDEFGHI\n"
   "JK LMNOPQ\n"
   "RSTUVWXYZ\n"
   "$0123456789\n"
   "#1234ABCD\n"
   "!@#$%^&*()_+|\n"
   "\\=-0/'\"`.,?\n"
   "><[]{}()"
  );  

  WriteTexFont(active,drawing,blend,300,450,7,10,5,
   "ABCDEFGHI\n"
   "JK LMNOPQ\n"
   "RSTUVWXYZ\n"
   "$0123456789\n"
   "#1234ABCD\n"
   "!@#$%^&*()_+|\n"
   "\\=-0/'\"`.,?\n"
   "><[]{}()"
  );

  WriteTexFont(active,drawing,blend,50,600,18,24,5,
   "ABCDEFGHI\n"
   "JK LMNOPQ\n"
   "RSTUVWXYZ\n"
   "$0123456789\n"
   "#1234ABCD\n"
   "!@#$%^&*()_+|\n"
   "\\=-0/'\"`.,?\n"
   "><[]{}()"
  );
    
  WriteTexFont(active,drawing,blend,600,300,64,64,5,
   "ABCDEFGHI\n"
   "JK LMNOPQ\n"
   "RSTUVWXYZ\n"
   "$0123456789\n"
   "#1234ABCD\n"
   "!@#$%^&*()_+|\n"
   "\\=-0/'\"`.,?\n"
   "><[]{}()"
  );
  
    
  WriteTexFont(active,drawing,blend,0,900,128,128,5,
   "ABCDEFGHI\n"
   "JK LMNOPQ\n"
   "RSTUVWXYZ\n"
   "$0123456789\n"
   "#1234ABCD\n"
   "!@#$%^&*()_+|\n"
   "\\=-0/'\"`.,?\n"
   "><[]{}()"
  );

 }
};

