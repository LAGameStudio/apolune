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

