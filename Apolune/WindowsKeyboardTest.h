/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once

#include "UglyFont.h"
#include "GLWindow.h"
#include "WindowsKeyboard.h"

class WindowsKeyboardTest : public GLWindow {
public:
 char lastKey;
 Zstring value;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
  lastKey='\0';
 }
 void Between() {
  // Classic style
  char key=input->keyboard();
  if ( key != '\0' ) {
   value+=key;
  }
  if ( (input->KeyUp(DX_BACKSPACE) || input->KeyUp(DX_DELETE)) ) {
   value = value.value.substr(0, value.value.size()-1);
  }
  lastKey=(key);
 }
 void Render() {
  crayons.Pick(skyBlue).gl();
  Blending(none);
  glLineWidth(2.0f);
  Text(
   FORMAT("direct: `%s`\n%d lines in history\nbuffer: `%s`\nLines:\n%s",
    value.c_str(),
    (int) windowsKeyboard.lines.count,
    windowsKeyboard.buffer.c_str(),
    windowsKeyboard.lines.Concatenate('\n').c_str()
   ).c_str(),
   32.0,32.0,   16.0,18.0, 4.0, false
  );
  glLineWidth(1.0f);
 }
};

