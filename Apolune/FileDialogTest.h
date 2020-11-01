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

#include "GLWindow.h"
#include "FileDialog.h"
#include "Graphics.h"

class FileDialogTest : public GLWindow {
public:
 FileDialog fd;
 void OnLoad() {
  Fullscreen();
  background.Pick(clear);
  fd.description="This is just a test of the selection of a file.\nThis is only a test.";
  fd.path=specialPaths.data_folder;
  fd.background.x_DarkSlateBlue();
 }
 void Render() {
  vg.Start();
  fd.Render();
  if ( fd.changed ) {
   OUTPUT("Resulting location selected: ``%s``\n", fd.result.c_str());
   fd.changed=false;
  }
  Text(fd.result.c_str(),(double)fd.w+4.0,4.0+14.0,10.0,14.0,2.0,false);
  vg.End();
 }
};

