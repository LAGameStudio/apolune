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

