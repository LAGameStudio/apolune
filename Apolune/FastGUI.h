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
/**************************************************************
  Author: H. Elwood Gilliland III
  "Fast implementation" of a
  graphical user interface for
  touch screens using WinTouch with
  mouse fallback
 **************************************************************/
#pragma once

#include "Art.h"
#include "Graphics.h"
#include "Cartesian.h"
#include "TextureLibrary.h"
#include "Interface.h"
#include "TTFEx.h"
#include "TexFont.h"
#include "ATFont.h"
#include "CDateTime.h"
#include "Audio.h"
#include "RoundedRectangle.h"
#include "ModelLibrary.h"
#include "Tweens.h"
#include "Film.h"
#include "Delay.h"
#include "Art2d.h"

#include "fx_RGBAPal.h"

#define gui fast

#define FAST_GUI_LOG_PATH "data/logs/"

#define FAST_BUTTON_TIMEOUT 0.23

class FullscreenStringEditor : public GLWindow {
public:
 Zstring label;
 Zp<Zstring> out;
 Zp<FBOColorDepth> fbo;
 Zp<TexFont> font,labelFont;
 Blends fontBlend,labelFontBlend;
 Zp<GLWindow> back_to;
 Zbool limited;
 FullscreenStringEditor( GLWindow *back_to ) : GLWindow() {
  this->fbo=&fbo1080p3;
  this->font=termFont;
  this->labelFont=digicaps;
  this->back_to=back_to;
  fontBlend=transparency;
  labelFontBlend=additive;
 }
 void OnLoad() {
  Fullscreen();
  background.Int(0,32,0,255);
 }
 void Between() {}
 void Render();
};

class FullscreenStringTerminalEditorChar {
public:
 Zstring stored;
 Zbool empty;
 FullscreenStringTerminalEditorChar() { empty=true; }
 void Set( char c ) { stored=""; stored+=c; empty=false; }
 void Unset() { stored=""; empty=true; }
 char Get() { if ( empty ) return '\0'; return stored.At(0); }
};

class FullscreenStringTerminalEditor : public GLWindow {
public:
 Zstring label;
 Zp<Zstring> out;
 Zp<FBOColorDepth> fbo;
 Zp<TexFont> font,labelFont;
 Blends fontBlend,labelFontBlend;
 Zp<GLWindow> back_to;
 Zbool limited;
 char lastKey;
 Crayon cursorColor,textColor;
 Zbool cursorBlink;
 Delay cursorBlinkRate,keyRepeatRate,arrowKeyRepeatRate;
 Zdouble time;
 Strings lines;
 Zp<String> selectStartLine,selectEndLine;
 Zint selectStartOffset,selectEndOffset;
 Zstring clipboard;
 Zbool helpF1;
 Zstring helpText;
 FullscreenStringTerminalEditor( GLWindow *back_to ) : GLWindow() {
  this->fbo=&fbo1080p3;
  this->font=termFont;
  this->labelFont=digicaps;
  this->back_to=back_to;
  fontBlend=transparency;
  labelFontBlend=additive;
  lastKey='\0';
  cursorColor.Int(96,255,0,255);
  textColor.Int(128,255,64,255);
  cursorBlinkRate.Set(0.8);
  keyRepeatRate.Set(0.01);
  arrowKeyRepeatRate.Set(0.5);
  cursorBlinkRate.Reset();
  keyRepeatRate.Reset();
  arrowKeyRepeatRate.Reset();
  helpText=file_as_string("data/fasthelp.txt");
 }
 void OnLoad() {
  Fullscreen();
  background.Int(0,32,0,255);
  ImportFromString();
 }
 void Between() { helpF1=input->KeyDown(DX_F1)?true:false; }
 void Render();
 void ImportFromString() {
  lines.Clear();
  lines.SplitPush((*out.pointer).value.c_str(),'\n');
  ClearSelection();
 }
 Cartesian scrolled;
 Cartesian cursorPosition;
 Cartesian eos;
 void ReplaceSelection() {
 }
 void DeleteSelection() {
 }
 void ClearSelection() {
  selectStartLine=null;
  selectStartOffset=0;
  selectEndLine=null;
  selectEndOffset=0;
 }
 void StartSelect() {
  selectEndLine=null;
 }
 void EndSelect() {
 }
 int GetLastCharPosition( int j ) { String *line=(String *) lines.Element(j); return line ? ((int)(line->s.length()-1)) : 0; }
 int GetLastLine() { return lines.count-1; }
 bool AtBeginningOfLine() { return cursorPosition.x == 0; }
 bool AtFirstLine() { return cursorPosition.y == 0; }
 bool AtEndOfLine() { return cursorPosition.x == GetLastCharPosition(cursorPosition.y)+1; }
 bool GoPreviousLine() { return cursorPosition.x == -1; }
 bool GoNextLine() { return cursorPosition.x == GetLastCharPosition(cursorPosition.y)+2; }
 bool OnNewestLine() { return cursorPosition.y == GetLastLine()+1; }
 void CheckCursorBounds() {
  // Moving up from top line
  if ( cursorPosition.y < 0 ) { cursorPosition.x=0; cursorPosition.y=0; return; }
  int lastLine=GetLastLine();
  // Moving down from bottom line
  if ( cursorPosition.y > lastLine+1 ) { cursorPosition.y=lastLine+1; }
  // Move left from 
  if ( GoPreviousLine() ) {
   if ( !AtFirstLine() ) {
    cursorPosition.y-=1;
    cursorPosition.x=GetLastCharPosition(cursorPosition.y)+1;
   } else cursorPosition.x=0;
  } else if ( GoNextLine() ) {
   if ( !OnNewestLine() ) {
    cursorPosition.y+=1;
    cursorPosition.x=0;
   }
  }
  if ( OnNewestLine() ) cursorPosition.x=0;
 }
 void Put(int i, int j) {
  cursorPosition.Set(i,j);
 }
 string Get( int i, int j ) {
  String *line=(String *)lines.Element(j);
  if ( !line ) return string("");
  if ( (int)line->s.length() <= i ) return string("");
  Zstring r=line->s;
  return r(i,1);
 }
 string GetAtCursor() { return Get(cursorPosition.x,cursorPosition.y); }
 string GetBeforeCursor() {
  Zstring result;
  int j=0;
  String *line=(String *)lines.first;
  while ( line ) {
   if ( j<cursorPosition.y ) { result+=line->s+string("\n"); } else break;
   line=(String *)line->next;
   j++;
  }
  line=(String *)lines.Element(cursorPosition.y);
  if ( line ) {
   const char *p=line->s.c_str();
   int i=0;
   while ( *p ) {
    if ( i >= cursorPosition.x ) { break; }
    result+=(*p);
    p++;
    i++;
   }
  }
  return result;
 }
 string GetAfterCursor() {
  Zstring result;
  if ( AtEndOfLine() ) result+="\n";
  String *thisLine=(String *)lines.Element(cursorPosition.y);
  if ( thisLine ) {
   const char *p=thisLine->s.c_str();
   int i=0;
   bool found=false;
   while ( *p != '\0' ) {
    if ( i > cursorPosition.x ) { found=true; result+=(*p); }
    p++;
    i++;
   }
   if ( !found ) {} else result+=string("\n");
   String *line=(String *)thisLine->next;
   while ( line ) {
    result+=line->s+string("\n");
    line=(String *)line->next;
   }
  }
  return result;
 }
 void Backspacer() {
  if ( AtBeginningOfLine() && AtFirstLine() ) return;
  else if ( AtBeginningOfLine() && OnNewestLine() ) {
   cursorPosition.y--;
   cursorPosition.x=GetLastCharPosition(cursorPosition.y)+1;
   CheckCursorBounds();
   return;
  } else if ( AtBeginningOfLine() ) {
   String *thisLine=(String *) lines.Element(cursorPosition.y);
   String *previousLine=(String *) lines.Element(cursorPosition.y-1);
   if ( !thisLine || !previousLine ) return;
   cursorPosition.y--;
   cursorPosition.x=GetLastCharPosition(cursorPosition.y)+1;
   Zstring result;
   EACH(lines.first,String,line) {
    result+=line->s;
    if ( line != previousLine ) result+=string("\n");
   }
   (*out.pointer)=result;
   ImportFromString();
   CheckCursorBounds();
  } else {
   Zstring result=GetBeforeCursor();
   if ( result.length == 0 ) return;
   result=result(0,result.length-1); // trim
   result+=GetAtCursor();
   result+=GetAfterCursor();
   (*out.pointer)=result;
   ImportFromString();
   cursorPosition.x-=1;
   CheckCursorBounds();
  }
 }
 void Deleter() {
  Zstring result=GetBeforeCursor();
  result+=GetAfterCursor();
  (*out.pointer)=result;
  ImportFromString();
  CheckCursorBounds();
 }
 void Newliner() {
  if ( OnNewestLine() ) {
   lines.Add("");
   cursorPosition.x=0;
   cursorPosition.y+=1;
  } else {
   Inserter('\n');
   cursorPosition.x=0;
  }
  CheckCursorBounds();
 }
 void Inserter( char k ) {
  Zstring result=GetBeforeCursor();
  result+=k; // insert character
  result+=GetAtCursor();
  result+=GetAfterCursor();
  (*out.pointer)=result;
  ImportFromString();
  cursorPosition.x+=1;
  CheckCursorBounds();
 }
 void Editor() {
  cursorBlinkRate.Between();
  keyRepeatRate.Between();
  arrowKeyRepeatRate.Between();
  if ( input->KeyHit(DX_UP)    ) { cursorPosition.y-=1; arrowKeyRepeatRate.Reset(); }
  if ( input->KeyDown(DX_UP)    && !arrowKeyRepeatRate ) { cursorPosition.y-=1; if ( !arrowKeyRepeatRate ) arrowKeyRepeatRate.Reset(); }
  if ( input->KeyHit(DX_LEFT)  ) { cursorPosition.x-=1; arrowKeyRepeatRate.Reset(); }
  if ( input->KeyDown(DX_LEFT)  && !arrowKeyRepeatRate ) { cursorPosition.x-=1; if ( !arrowKeyRepeatRate ) arrowKeyRepeatRate.Reset(); }
  if ( input->KeyHit(DX_RIGHT) ) { cursorPosition.x+=1; arrowKeyRepeatRate.Reset(); }
  if ( input->KeyDown(DX_RIGHT) && !arrowKeyRepeatRate ) { cursorPosition.x+=1; if ( !arrowKeyRepeatRate ) arrowKeyRepeatRate.Reset(); }
  if ( input->KeyHit(DX_DOWN)  ) { cursorPosition.y+=1; arrowKeyRepeatRate.Reset(); }
  if ( input->KeyDown(DX_DOWN)  && !arrowKeyRepeatRate ) { cursorPosition.y+=1; if ( !arrowKeyRepeatRate ) arrowKeyRepeatRate.Reset(); }
  CheckCursorBounds();
  if ( input->KeyHit(DX_BACKSPACE) ) Backspacer();
  else if ( input->KeyHit(DX_DELETE) ) Deleter();
  else if ( input->KeyHit(DX_ENTER) ) {
   Newliner();
  } else {
   char k=input->keyboard();
   if ( k != '\0' ) {
    if ( !keyRepeatRate ) {
     if ( LOWER(k) != lastKey && k != '\0' && k != '{' && k != '}' ) {
      Inserter(k);
      //lastKey=LOWER(k);
     }
     keyRepeatRate.Reset();
    }
   } else keyRepeatRate.instant=0.0;
  }
  // Render
  Cartesian mouseScreen((int)(input->mxd*(double)fbo->w),(int)(input->myd*(double)fbo->h));
  Cartesian draw(16,16);
  Crayon cursorTint(cursorColor);
  time+=FRAMETIME;
  cursorTint.ScaleByAlpha(255.0*fmod(time,0.5)/0.5);
  String *line=(String *)lines.first;
  int j=0;
  while ( line ) {
   if ( j < scrolled.y ) { line=(String *) line->next; j++; continue; }
   const char *p=line->s.c_str();
   int i=0;
   while ( *p ) {
    if ( i < scrolled.x ) { p++; i++; continue; }
    if ( WITHIN(mouseScreen.x,mouseScreen.y,draw.x,draw.y,draw.x+16-4-1,draw.y+20) ) {
     glLineWidth(2.0);
     Rectangle(cursorColor,draw.x,draw.y,draw.x+16,draw.y+20);;
     glLineWidth(1.0);
     if ( input->left ) {
      Put(i,j);
     }
    }
    string s=Get(i,j);
    bool mt= s.length() == 0;
    if ( !mt ) {
     GLImage *g=font->glyph(*(s.c_str()));
     if ( g ) StretchiNoAlpha(g,textColor,fontBlend,draw.x,draw.y,16,20);
    }
    draw.x+=16+2-4;
    if ( draw.x > w-128 ) break;
    p++;
    i++;
   }
   glLineWidth(2.0);
   Crayon lineEnd(textColor);
   lineEnd.ScaleByAlphaf(0.2f);
   Rectangle(lineEnd,draw.x,draw.y+16,draw.x+8,draw.y+20);
   glLineWidth(1.0);
   draw.x=16;
   draw.y+=20+2;
   if ( draw.y > h-16-20) break;
   line=(String *)line->next;
   j++;
  }
  draw.Set(16+(cursorPosition.x-scrolled.x)*(16+2-4),16+(cursorPosition.y-scrolled.y)*(20+2));
  glLineWidth(2.0);
  Area(cursorTint,draw.x,draw.y,draw.x+3,draw.y+20);
  glLineWidth(1.0);
 }
};

class FullscreenStringClassEditor : public GLWindow {
public:
 Zstring label;
 Zp<String> out;
 Zp<FBOColorDepth> fbo;
 Zp<TexFont> font,labelFont;
 Blends fontBlend,labelFontBlend;
 Zp<GLWindow> back_to;
 Zbool limited;
 FullscreenStringClassEditor( GLWindow *back_to ) : GLWindow() {
  this->fbo=&fbo1080p3;
  this->font=termFont;
  this->labelFont=digicaps;
  this->back_to=back_to;
  fontBlend=transparency;
  labelFontBlend=additive;
 }
 void OnLoad() {
  Fullscreen();
  background.Int(0,32,0,255);
 }
 void Between() {}
 void Render();
};


class FullscreenStringClassTerminalEditor : public GLWindow {
public:
 Zstring label;
 Zp<String> out;
 Zp<FBOColorDepth> fbo;
 Zp<TexFont> font,labelFont;
 Blends fontBlend,labelFontBlend;
 Zp<GLWindow> back_to;
 Zbool limited;
 char lastKey;
 Crayon cursorColor,textColor;
 Zbool cursorBlink;
 Delay cursorBlinkRate,keyRepeatRate,arrowKeyRepeatRate;
 Zdouble time;
 Strings lines;
 Zp<String> selectStartLine,selectEndLine;
 Zint selectStartOffset,selectEndOffset;
 Zstring clipboard;
 Zbool helpF1;
 Zstring helpText;
 FullscreenStringClassTerminalEditor( GLWindow *back_to ) : GLWindow() {
  this->fbo=&fbo1080p3;
  this->font=termFont;
  this->labelFont=digicaps;
  this->back_to=back_to;
  fontBlend=transparency;
  labelFontBlend=additive;
  lastKey='\0';
  cursorColor.Int(96,255,0,255);
  textColor.Int(128,255,64,255);
  cursorBlinkRate.Set(0.8);
  keyRepeatRate.Set(0.01);
  arrowKeyRepeatRate.Set(0.5);
  cursorBlinkRate.Reset();
  keyRepeatRate.Reset();
  arrowKeyRepeatRate.Reset();
  helpText=file_as_string("data/fasthelp.txt");
 }
 void OnLoad() {
  Fullscreen();
  background.Int(0,32,0,255);
  ImportFromString();
 }
 void Between() { helpF1=input->KeyDown(DX_F1)?true:false; }
 void Render();
 void ImportFromString() {
  lines.Clear();
  lines.SplitPush(out->c_str(),'\n');
  ClearSelection();
 }
 Cartesian scrolled;
 Cartesian cursorPosition;
 Cartesian eos;
 void ReplaceSelection() {
 }
 void DeleteSelection() {
 }
 void ClearSelection() {
  selectStartLine=null;
  selectStartOffset=0;
  selectEndLine=null;
  selectEndOffset=0;
 }
 void StartSelect() {
  selectEndLine=null;
 }
 void EndSelect() {
 }
 int GetLastCharPosition( int j ) { String *line=(String *) lines.Element(j); return line ? ((int)(line->s.length()-1)) : 0; }
 int GetLastLine() { return lines.count-1; }
 bool AtBeginningOfLine() { return cursorPosition.x == 0; }
 bool AtFirstLine() { return cursorPosition.y == 0; }
 bool AtEndOfLine() { return cursorPosition.x == GetLastCharPosition(cursorPosition.y)+1; }
 bool GoPreviousLine() { return cursorPosition.x == -1; }
 bool GoNextLine() { return cursorPosition.x == GetLastCharPosition(cursorPosition.y)+2; }
 bool OnNewestLine() { return cursorPosition.y == GetLastLine()+1; }
 void CheckCursorBounds() {
  // Moving up from top line
  if ( cursorPosition.y < 0 ) { cursorPosition.x=0; cursorPosition.y=0; return; }
  int lastLine=GetLastLine();
  // Moving down from bottom line
  if ( cursorPosition.y > lastLine+1 ) { cursorPosition.y=lastLine+1; }
  // Move left from 
  if ( GoPreviousLine() ) {
   if ( !AtFirstLine() ) {
    cursorPosition.y-=1;
    cursorPosition.x=GetLastCharPosition(cursorPosition.y)+1;
   } else cursorPosition.x=0;
  } else if ( GoNextLine() ) {
   if ( !OnNewestLine() ) {
    cursorPosition.y+=1;
    cursorPosition.x=0;
   }
  }
  if ( OnNewestLine() ) cursorPosition.x=0;
 }
 void Put(int i, int j) {
  cursorPosition.Set(i,j);
 }
 string Get( int i, int j ) {
  String *line=(String *)lines.Element(j);
  if ( !line ) return string("");
  if ( (int)line->s.length() <= i ) return string("");
  Zstring r=line->s;
  return r(i,1);
 }
 string GetAtCursor() { return Get(cursorPosition.x,cursorPosition.y); }
 string GetBeforeCursor() {
  Zstring result;
  int j=0;
  String *line=(String *)lines.first;
  while ( line ) {
   if ( j<cursorPosition.y ) { result+=line->s+string("\n"); } else break;
   line=(String *)line->next;
   j++;
  }
  line=(String *)lines.Element(cursorPosition.y);
  if ( line ) {
   const char *p=line->s.c_str();
   int i=0;
   while ( *p ) {
    if ( i >= cursorPosition.x ) { break; }
    result+=(*p);
    p++;
    i++;
   }
  }
  return result;
 }
 string GetAfterCursor() {
  Zstring result;
  if ( AtEndOfLine() ) result+="\n";
  String *thisLine=(String *)lines.Element(cursorPosition.y);
  if ( thisLine ) {
   const char *p=thisLine->s.c_str();
   int i=0;
   bool found=false;
   while ( *p != '\0' ) {
    if ( i > cursorPosition.x ) { found=true; result+=(*p); }
    p++;
    i++;
   }
   if ( !found ) {} else result+=string("\n");
   String *line=(String *)thisLine->next;
   while ( line ) {
    result+=line->s+string("\n");
    line=(String *)line->next;
   }
  }
  return result;
 }
 void Backspacer() {
  if ( AtBeginningOfLine() && AtFirstLine() ) return;
  else if ( AtBeginningOfLine() && OnNewestLine() ) {
   cursorPosition.y--;
   cursorPosition.x=GetLastCharPosition(cursorPosition.y)+1;
   CheckCursorBounds();
   return;
  } else if ( AtBeginningOfLine() ) {
   String *thisLine=(String *) lines.Element(cursorPosition.y);
   String *previousLine=(String *) lines.Element(cursorPosition.y-1);
   if ( !thisLine || !previousLine ) return;
   cursorPosition.y--;
   cursorPosition.x=GetLastCharPosition(cursorPosition.y)+1;
   Zstring result;
   EACH(lines.first,String,line) {
    result+=line->s;
    if ( line != previousLine ) result+=string("\n");
   }
   out->s=string(result.c_str());
   ImportFromString();
   CheckCursorBounds();
  } else {
   Zstring result=GetBeforeCursor();
   if ( result.length == 0 ) return;
   result=result(0,result.length-1); // trim
   result+=GetAtCursor();
   result+=GetAfterCursor();
   out->s=string(result.c_str());
   ImportFromString();
   cursorPosition.x-=1;
   CheckCursorBounds();
  }
 }
 void Deleter() {
  Zstring result=GetBeforeCursor();
  result+=GetAfterCursor();
  out->s=string(result.c_str());
  ImportFromString();
  CheckCursorBounds();
 }
 void Newliner() {
  if ( OnNewestLine() ) {
   lines.Add("");
   cursorPosition.x=0;
   cursorPosition.y+=1;
  } else {
   Inserter('\n');
   cursorPosition.x=0;
  }
  CheckCursorBounds();
 }
 void Inserter( char k ) {
  Zstring result=GetBeforeCursor();
  result+=k; // insert character
  result+=GetAtCursor();
  result+=GetAfterCursor();
  out->s=string(result.c_str());
  ImportFromString();
  cursorPosition.x+=1;
  CheckCursorBounds();
 }
 void Editor() {
  cursorBlinkRate.Between();
  keyRepeatRate.Between();
  arrowKeyRepeatRate.Between();
  if ( input->KeyHit(DX_UP)    ) { cursorPosition.y-=1; arrowKeyRepeatRate.Reset(); }
  if ( input->KeyDown(DX_UP)    && !arrowKeyRepeatRate ) { cursorPosition.y-=1; if ( !arrowKeyRepeatRate ) arrowKeyRepeatRate.Reset(); }
  if ( input->KeyHit(DX_LEFT)  ) { cursorPosition.x-=1; arrowKeyRepeatRate.Reset(); }
  if ( input->KeyDown(DX_LEFT)  && !arrowKeyRepeatRate ) { cursorPosition.x-=1; if ( !arrowKeyRepeatRate ) arrowKeyRepeatRate.Reset(); }
  if ( input->KeyHit(DX_RIGHT) ) { cursorPosition.x+=1; arrowKeyRepeatRate.Reset(); }
  if ( input->KeyDown(DX_RIGHT) && !arrowKeyRepeatRate ) { cursorPosition.x+=1; if ( !arrowKeyRepeatRate ) arrowKeyRepeatRate.Reset(); }
  if ( input->KeyHit(DX_DOWN)  ) { cursorPosition.y+=1; arrowKeyRepeatRate.Reset(); }
  if ( input->KeyDown(DX_DOWN)  && !arrowKeyRepeatRate ) { cursorPosition.y+=1; if ( !arrowKeyRepeatRate ) arrowKeyRepeatRate.Reset(); }
  CheckCursorBounds();
  if ( input->KeyHit(DX_BACKSPACE) ) Backspacer();
  else if ( input->KeyHit(DX_DELETE) ) Deleter();
  else if ( input->KeyHit(DX_ENTER) ) {
   Newliner();
  } else {
   char k=input->keyboard();
   if ( k != '\0' ) {
    if ( !keyRepeatRate ) {
     if ( LOWER(k) != lastKey && k != '\0' && k != '{' && k != '}' ) {
      Inserter(k);
      //lastKey=LOWER(k);
     }
     keyRepeatRate.Reset();
    }
   } else keyRepeatRate.instant=0.0;
  }
  // Render
  Cartesian mouseScreen((int)(input->mxd*(double)fbo->w),(int)(input->myd*(double)fbo->h));
  Cartesian draw(16,16);
  Crayon cursorTint(cursorColor);
  time+=FRAMETIME;
  cursorTint.ScaleByAlpha(255.0*fmod(time,0.5)/0.5);
  String *line=(String *)lines.first;
  int j=0;
  while ( line ) {
   if ( j < scrolled.y ) { line=(String *) line->next; j++; continue; }
   const char *p=line->s.c_str();
   int i=0;
   while ( *p ) {
    if ( i < scrolled.x ) { p++; i++; continue; }
    if ( WITHIN(mouseScreen.x,mouseScreen.y,draw.x,draw.y,draw.x+16-4-1,draw.y+20) ) {
     glLineWidth(2.0);
     Rectangle(cursorColor,draw.x,draw.y,draw.x+16,draw.y+20);;
     glLineWidth(1.0);
     if ( input->left ) {
      Put(i,j);
     }
    }
    string s=Get(i,j);
    bool mt= s.length() == 0;
    if ( !mt ) {
     GLImage *g=font->glyph(*(s.c_str()));
     if ( g ) StretchiNoAlpha(g,textColor,fontBlend,draw.x,draw.y,16,20);
    }
    draw.x+=16+2-4;
    if ( draw.x > w-128 ) break;
    p++;
    i++;
   }
   glLineWidth(2.0);
   Crayon lineEnd(textColor);
   lineEnd.ScaleByAlphaf(0.2f);
   Rectangle(lineEnd,draw.x,draw.y+16,draw.x+8,draw.y+20);
   glLineWidth(1.0);
   draw.x=16;
   draw.y+=20+2;
   if ( draw.y > h-16-20) break;
   line=(String *)line->next;
   j++;
  }
  draw.Set(16+(cursorPosition.x-scrolled.x)*(16+2-4),16+(cursorPosition.y-scrolled.y)*(20+2));
  glLineWidth(2.0);
  Area(cursorTint,draw.x,draw.y,draw.x+3,draw.y+20);
  glLineWidth(1.0);
 }
};

enum FastGUIStates {
 fg_none=0,
 fg_hovering=1,
 fg_clicking=2,
 fg_cancelling=4,
 fg_clicked=8,
 fg_cancelled=16,
 fg_unfocused=32
};

enum FastGuiBoxTypes {
 fg_C, // Rounded
 fg_D, // Rounded More (Classic TV)
 fg_E, // Rounded Variant// Rounded
 fg_F, // Rounded More Variant// Rounded More
 fg_G, // Thin Top/Bottom, Thick Sides
 fg_H, // Ultrathin Top, Thin Sides
 fg_I, // Double Stroke Border, Ultrathin 2X
 fg_J, // Rounded
 fg_K, // Rounded with central rectangle
 fg_L, // Rounded with top rectangle
 fg_M  // Rounded with bottom rectangle
};

struct FastExtents {
 public:
  int x,y,w,h;
  FastExtents() { x=0; y=0; w=0; h=0; }
  FastExtents( Cartesian *c ) { Set(c); }
  FastExtents( Cartesiand *c ) { Set(c); }
  FastExtents( ScreenPosition *c ) { Set(c); }
  FastExtents( int x, int y, int w, int h ) {
   this->x=x; this->y=y; this->w=w; this->h=h;
  }
  void Set( Cartesian *c ) { x=c->x; y=c->y; w=c->w; h=c->h; }
  void Set( Cartesiand *c ) { x=(int)c->x; y=(int)c->y; w=(int)c->w; h=(int)c->h; }
  void Set( ScreenPosition *c ) { x=c->x; y=c->y; w=c->w; h=c->h; }
};

struct FastGUIState {
public:
 Crayon hover,highlight,color,tint,selected,activated,deactivated,excited,indicated;
 Crayon rr_hover_inner,rr_hover_outer,rr_hover_border,rr_hover_text;     // When the mouse is hovering
 Crayon rr_select_inner,rr_select_outer,rr_select_border,rr_select_text; // When hovering and clicking
 Crayon rr_active_inner,rr_active_outer,rr_active_border,rr_active_text; // When "activated" because it was toggled on
 Crayon rr_active_select_inner,rr_active_select_outer,rr_active_select_border,rr_active_select_text; // both of last two above
 Crayon rr_hover_active_inner,rr_hover_active_outer,rr_hover_active_border,rr_hover_active_text; // hovering while active
 Crayon rr_inner,rr_outer,rr_border,rr_text; // Normal mode
 Coord2d point;
 Coord2d fontSize,smallFontSize;
 Dim2i tfont;
 Zp<TexFont> font;
 Blends tblend;
 Zint fontW,fontH,smallFontW,smallFontH;
 Zint charSpace,lineSpace,fontCharSpace,fontLineSpace; // for TexFont
 Cartesiand responsiveRatio,reciprocalRatio,b100;
 Cartesiand scope; // Scopes the mouse to a certain area (when scaling to a region of the screen), may not work well with responsiveRatio
 Coord2d scopeRatio,screenRatio; // For scope.
 Crayon bTintA,bTintB,bTintC,bTintD,border,filmPreviewBacking,imagePreviewBacking;
 Zint rect_radius,rect_precision;
 Zfloat rect_borderwidth;
 Cartesiand vdisplay,mouse;
};

class VirtualWindow;
class VirtualGUI;
class FileDialog;
class FastGUI {
public:
 Zpointer<GLImage> rounded,upArrow,downArrow,leftArrow,rightArrow,invisPixel,whitePixel,
  soundIcon,particlesIcon,curveIcon,quadIcon,animIcon,linesIcon,filmIcon,textIcon,
  rotateLeft,rotateRight,raiseZ,lowerZ,nearY,farY,raiseX,lowerX,unknown,
  closeIcon,rightArrowTrans;
 Zpointer<VirtualGUI> vwindows;
 Zpointer<VirtualWindow> member;
 FastGUIState savedState;
 Crayon hover,highlight,color,tint,selected,activated,deactivated,excited,indicated;
 Crayon rr_hover_inner,rr_hover_outer,rr_hover_border,rr_hover_text;     // When the mouse is hovering
 Crayon rr_select_inner,rr_select_outer,rr_select_border,rr_select_text; // When hovering and clicking
 Crayon rr_active_inner,rr_active_outer,rr_active_border,rr_active_text; // When "activated" because it was toggled on
 Crayon rr_active_select_inner,rr_active_select_outer,rr_active_select_border,rr_active_select_text; // both of last two above
 Crayon rr_hover_active_inner,rr_hover_active_outer,rr_hover_active_border,rr_hover_active_text; // hovering while active
 Crayon rr_inner,rr_outer,rr_border,rr_text; // Normal mode
 Coord2d point;
 Cartesian lastSize,lastDimensions;
 Coord2d fontSize,smallFontSize;
 Dim2i tfont;
 Zp<TexFont> font;
 Zp<ATexFont> afont;
 Blends tblend;
 Zint fontW,fontH,smallFontW,smallFontH;
 Zint fontCharSpace,fontLineSpace,charSpace,lineSpace; // for TexFont
 Zbyte last;
 Zbool hovering,clicking,cancelling,clicked,cancelled,hovered,rgbPalHovering,cPalHovering;
 char lastKey;
 Zdouble ignoreNextRelease;
 Zstring logfilename,logpath; // Logs events / button presses, stored in FAST_GUI_LOG_PATH
 Zstring logfile;
 Cartesiand responsiveRatio,reciprocalRatio,b100;
 Cartesiand scope; // Scopes the mouse to a certain area (when scaling to a region of the screen), may not work well with responsiveRatio
 Coord2d scopeRatio,screenRatio; // For scope.
 Crayon bTintA,bTintB,bTintC,bTintD,border,filmPreviewBacking,imagePreviewBacking;
 Zint rect_radius,rect_precision;
 Zfloat rect_borderwidth;
 RoundedRectangles rrects;
 Cartesiand vdisplay,mouse;
 Zp<FileDialog> fd;
 Zstring fName;
 TexFontHandles tfonts;
 Zint stashed_wideRect,stashed_squareRect,stashed_ButtonRect,stashed_ThickSquare,stashed_FilledSquare,stashed_TallRect,stashed_BarGraphRect,stashed_VBarGraphRect;
 FastGUI() {
  logpath=FAST_GUI_LOG_PATH;
  last=fg_none;
  lastKey='\0';
 }
 // Only called once...
 void Init() {
  ResetResponsiveRatio();
  Unscope();
  OUTPUT("Initializing FastGUI...");
  DefaultState();
  tfonts.Clear();
  tfonts.Add(alarmClockNumbers.pointer);
  tfonts.Add(neonNumbers.pointer);
  tfonts.Add(goldNeonNumbers.pointer);
  tfonts.Add(redNeonNumbers.pointer);
  tfonts.Add(microFont.pointer);
  tfonts.Add(digicaps.pointer);
  tfonts.Add(silverFont.pointer);
  tfonts.Add(calibri.pointer);
  tfonts.Add(bytes.pointer);
  tfonts.Add(nexaBold.pointer);
  tfonts.Add(termFont.pointer);
  tfonts.Add(glowTermFont.pointer);
  this->afont=atermFont;
  rounded=library.Load("data/images/elements/rounded_trans.png");
  upArrow=library.Load("data/images/elements/uparrow.png");
  downArrow=library.Load("data/images/elements/downarrow.png");
  leftArrow=library.Load("data/images/elements/leftarrow.png");
  rightArrow=library.Load("data/images/elements/rightarrow.png");
  whitePixel=library.Load("data/images/elements/white.png");
  invisPixel=library.Load("data/images/elements/invis.png");
  soundIcon=library.Load("data/images/icons/sound.png");
  unknown=library.Load("data/images/icons/random.png");
  particlesIcon=library.Load("data/images/icons/particles.png");
  curveIcon=library.Load("data/images/icons/curves.png");
  animIcon=library.Load("data/images/icons/animation.png");
  filmIcon=library.Load("data/images/icons/reel.png");
  linesIcon=library.Load("data/images/icons/lines2.png");
  rotateLeft=library.Load("data/images/icons/rotateLeft.png");
  rotateRight=library.Load("data/images/icons/rotateRight.png");
  quadIcon=library.Load("data/images/icons/quaded.png");
  raiseX=library.Load("data/images/icons/raiseX.png");
  lowerX=library.Load("data/images/icons/lowerX.png");
  farY=library.Load("data/images/icons/farY.png");
  nearY=library.Load("data/images/icons/nearY.png");
  raiseZ=library.Load("data/images/icons/raiseZ.png");
  lowerZ=library.Load("data/images/icons/lowerZ.png");
  textIcon=library.Load("data/images/icons/texttool.png");
  closeIcon=library.Load("data/images/icon/x.png");
  rightArrowTrans=library.Load("data/images/elements/right.png");
  OUTPUT("complete.\n");
  // Setup logging
  SYSTEMTIME systemTime;
  GetSystemTime(&systemTime);
  CDateTime now(systemTime);
  logfilename=(
   FORMAT("%sdaily-%d-%d-%d.txt",logpath.c_str(),
   (int)now.GetDay(),
   (int)now.GetMonth(),
   (int)now.GetYear()
   )
  );
  if ( file_exists(logfilename.c_str()) ) {
   logfile.Load(logfilename.c_str());
   log(FORMAT("--- Log Reloaded: `%s`", logfilename.c_str()).c_str());
  } else {
   log( "Log Initialized" );
  }
  stashed_wideRect=art.stash.GenRoundRect(0.2,0.1);
  stashed_squareRect=art.stash.GenRoundRect(0.2,0.2);
  stashed_ButtonRect=art.stash.GenRoundRect(0.2,0.075);
  stashed_ThickSquare=art.stash.GenGrid(1,1,0.3);
  stashed_FilledSquare=art.stash.GenRoundRect(0.01,0.01);
  stashed_TallRect=art.stash.GenRoundRect(0.1,0.2);
  stashed_BarGraphRect=art.stash.GenRoundRect(0.1,0.05);
  stashed_VBarGraphRect=art.stash.GenRoundRect(0.05,0.1);
 }
 void DefaultState() {
  tblend=transparency;
  Fonts(9.0,12.0,6.0,8.0);
  charSpace=2;
  lineSpace=1;
  fontCharSpace=1;
  fontLineSpace=2;
  tfont.Set(32,42);
  rect_radius=8;
  rect_precision=6;
  rect_borderwidth=4.0f;//2.45f;
  vdisplay.SetRect(0.0,0.0,1920.0,1080.0);
  _rgbPal.minimum=0.0f;
  font=termFont;
  filmPreviewBacking.x_Black();
  imagePreviewBacking.x_Black();
  highlight.Pick(alabaster);
  color.Int( 40, 75, 255, 255 );
  selected.Pick(hotPink);
  activated.Int( 255, 215, 54, 255 );
  deactivated.Float(0.25,0.25,0.25,1.0);
  indicated.Pick(gold); indicated.Multiply(0.8f); indicated.Red(indicated.r+20);
  excited.Int( 196, 64, 48, 255 );
  tint.Pick(macNcheese);
  hover.x_Orange();
  bTintA.fromCrayon(skyBlue);
  bTintB.fromCrayon(magenta);
  bTintC.fromCrayon(macNcheese);
  bTintD.fromCrayon(blue255);
  border.Pick(indigo);
  rr_hover_inner.Pick(ANSImutedblue);
  rr_hover_outer.Pick(ANSIboldblue);
  rr_hover_border.Pick(ANSIcyan);
  rr_hover_text.Pick(alabaster);
  rr_select_inner.Pick(ANSImutedred);
  rr_select_outer.Pick(macNcheese);
  rr_select_border.Pick(gold);
  rr_select_text.Pick(black);
  rr_active_inner.Pick(c64blue);
  rr_active_outer.Pick(blue255);
  rr_active_border.Pick(ANSIboldblue);
  rr_active_text.Pick(gold);
  rr_active_select_inner.Pick(ANSIbackred);
  rr_active_select_outer.Pick(ANSIboldred);
  rr_active_select_border.Pick(ANSIboldred);
  rr_active_select_text.Pick(gold);
  rr_hover_active_inner.Pick(alabaster);
  rr_hover_active_outer.Pick(ANSIcyan);
  rr_hover_active_border.Pick(ANSIboldblue);
  rr_hover_active_text.Pick(black);
  rr_inner.Pick(magenta);
  rr_outer.Pick(ANSIboldblue);
  rr_border.Pick(black);
  rr_text.Pick(alabaster);  
 }
 void Unscope() { scope.SetRect(0,0,display.w,display.h); }
 // Used to limit active area.
 // Input terms are from the virtual screen, not the actual screen.
 void ScopeTo( int x, int y, int w, int h ) {
  ScopeTo((double)x,(double)y,(double)w,(double)h);
 }
 // Used to limit active area.
 // Input terms are from the virtual screen, not the actual screen.
 void ScopeTo( double x, double y, double w, double h ) {
  scope.SetRect(x,y,w,h);
  screenRatio.x=scope.w/vdisplay.w;
  screenRatio.y=scope.h/vdisplay.h;
 }
 void ResetResponsiveRatio() {
  ResponsiveRatio(display.wd,display.hd);
 }
 void ResponsiveRatio( double virtual_w, double virtual_h ) {
  vdisplay.SetRect(0,0,virtual_w,virtual_h);
  responsiveRatio.SetRect(virtual_w/display.wd,virtual_h/display.hd,virtual_w,virtual_h);
  reciprocalRatio.SetRect(display.wd/virtual_w,display.hd/virtual_h,virtual_h,virtual_w);
  b100.Set(virtual_w/100.0,virtual_h/100.0);
  Fonts(setFontSizes.x,setFontSizes.y,setFontSizes.w,setFontSizes.h);
  ScopeTo(0.0,0.0,virtual_w,virtual_h);
 }
 FastExtents P( int x100, int y100, int w100, int h100 ) {
  return FastExtents((int)b100.x*x100,(int)b100.y*y100, (int)b100.x*w100, (int)b100.y*h100);
 }
 Cartesiand setFontSizes;
 void Fonts( double fx, double fy, double sfx, double sfy ) {
  setFontSizes.SetRect(fx,fy,sfx,sfy);
  fontSize.x=UMAX(5.0,fx*responsiveRatio.x);
  fontSize.y=UMAX(7.0,fy*responsiveRatio.y);
  fontW=(int)( (fontSize.x+0.5) * responsiveRatio.x );
  fontH=(int)( (fontSize.y+0.5) * responsiveRatio.y );
  smallFontSize.x=UMAX(8.0,sfx * responsiveRatio.x);
  smallFontSize.y=UMAX(10.0,sfy * responsiveRatio.y);
  smallFontW=(int) ( (smallFontSize.x+0.5) * responsiveRatio.x );
  smallFontH=(int) ( (smallFontSize.y+0.5) * responsiveRatio.y );
 }
 void SetLastState( bool hovering, bool clicking, bool cancelling, bool clicked, bool cancelled ) {
  last=fg_none;
  if ( hovering   ) last.Set(fg_hovering,true);
  if ( clicking   ) last.Set(fg_clicking,true);
  if ( cancelling ) last.Set(fg_cancelling,true);
  if ( clicked    ) last.Set(fg_clicked,true);
  if ( cancelled  ) last.Set(fg_cancelled,true);
 }
 void GetState(int x, int y, int w, int h, bool dontIgnoreNextRelease=false);
 void log( const char *content ) {
  SYSTEMTIME systemTime;
  GetSystemTime(&systemTime);
  CDateTime now(systemTime);
  logfile+=FORMAT("%d-%d-%d,%d:%d:%d,%d,%s\n",
   (int) now.GetMonth(),
   (int) now.GetDay(),
   (int) now.GetYear(),
   (int) now.GetHour(),
   (int) now.GetMinute(),
   (int) now.GetSecond(),
   (int) now.GetSecsSinceMillenium(),
   content
  );
  logfile.Save(logfilename.c_str());
  OUTPUT("Fast.log: %s\n",content);
 }
 double number_delta( double source=1.0 ) {  // Returns a numeric delta in a standard way (for use with decimal() and number() widgets)
  bool alt=(input.KeyDown(DX_LALT)>0 || input.KeyDown(DX_RALT)>0);
  bool control=(input.KeyDown(DX_LCTRL)>0 || input.KeyDown(DX_RCTRL)>0);
  bool shift=(input.KeyDown(DX_LSHIFT)>0 || input.KeyDown(DX_RSHIFT)>0);
  bool zee=(input.KeyDown(DX_Z)>0);
  double result=source;
  if ( alt ) result*=5.0;
  if ( control && !shift && !zee ) result *=10.0;
  else if ( !control && !zee && shift ) result *=100.0;
  else if ( !zee && control && shift ) result /=10.0;
  else if ( zee && control && shift ) result /=1000.0;
  else if ( zee && control ) result /=100.0;
  else if ( zee && shift ) result /=2.0;
  else if ( zee ) result *=2.0;
  return result;
 }
 int integer_delta( int source=1 ) {  // Returns a numeric delta in a standard way (for use with decimal() and number() widgets)
  bool alt=(input.KeyDown(DX_LALT)>0 || input.KeyDown(DX_RALT)>0);
  bool control=(input.KeyDown(DX_LCTRL)>0 || input.KeyDown(DX_RCTRL)>0);
  bool shift=(input.KeyDown(DX_LSHIFT)>0 || input.KeyDown(DX_RSHIFT)>0);
  bool zee=(input.KeyDown(DX_Z)>0);
  int result=source;
  if ( alt ) result*=5;
  if ( control && !shift && !zee ) result *=10;
  else if ( !control && !zee && shift ) result *=100;
  else if ( !zee && control && shift ) result /=10;
  else if ( zee && control && shift ) result /=1000;
  else if ( zee && control ) result /=100;
  else if ( zee && shift ) result /=2;
  else if ( zee ) result *=2;
  return result;
 }
 void display_delta( double delta ) {
  double vpointx=(double)lastDimensions.x+point.x*(double)lastDimensions.w+16.0;
  double vpointy=(double)lastDimensions.y+point.y*(double)lastDimensions.h+16.0;
  boxedtext(FORMAT("%c%1.4f",delta>0.0?'+':'-',abs(delta)).c_str(),(int)vpointx,(int)vpointy);
 }
 void display_delta( int delta ) {
  double vpointx=(double)lastDimensions.x+point.x*(double)lastDimensions.w+16.0;
  double vpointy=(double)lastDimensions.y+point.y*(double)lastDimensions.h+16.0;
  boxedtext(FORMAT("%c%d",delta>0?'+':'-',abs(delta)).c_str(),(int)vpointx,(int)vpointy);
 }
 int bestrectforaspect( double w, double h );
 void bestrect( Crayon color, int x, int y, int w, int h, double corner=8.0 ) {
  art.BevelRect(color,corner,x,y,w,h);
 }
 void bestrect( Crayon color, Blends blend, int x, int y, int w, int h, double corner=8.0 ) {
  art.BevelRect(color,blend,corner,x,y,w,h);
 }
 void bestrect( Crayon color, double x, double y, double w, double h, double corner=8.0 ) {
  art.BevelRect(color,corner,x,y,w,h);
 }
 void bestrect( Crayon color, Blends blend, double x, double y, double w, double h, double corner=8.0 ) {
  art.BevelRect(color,blend,corner,x,y,w,h);
 }
 void bestrect( GLImage *t, Crayon color, Blends blend, int x, int y, int w, int h ) {
  art.Element(t,color,blend,bestrectforaspect(w,h),(double)x,(double)y,(double)w,(double)h);
 }
 void bestrect( GLImage *t, Crayon outer, Crayon inner, Blends blend, double x, double y, double w, double h ) {
  art.Element(t,color,blend,bestrectforaspect(w,h),x,y,w,h);
 }
 void bestrect( Crayon outer, Crayon inner, int x, int y, int w, int h, int border=5 ) {
  art.Element2(outer,inner,transparency,bestrectforaspect(w,h),(double)x,(double)y,(double)w,(double)h,(double)border);
 }
 void bestrect( Crayon outer, Crayon inner, double x, double y, double w, double h, double corner=8.0, double border=5.0 ) {
  art.Element2(outer,inner,transparency,bestrectforaspect(w,h),x,y,w,h,border);
 }
 void bestrect( Crayon outer, Crayon inner, Blends blend, int x, int y, int w, int h, int border=5 ) {
  art.Element2(outer,inner,blend,bestrectforaspect(w,h),(double)x,(double)y,(double)w,(double)h,(double)border);
 }
 void bestrect( Crayon outer, Crayon inner, Blends blend, double x, double y, double w, double h, double border=5.0 ) {
  art.Element2(outer,inner,blend,bestrectforaspect(w,h),x,y,w,h,border);
 }
 void box( FastExtents e );
 void box( int x, int y, int w, int h );
 void box( FastGuiBoxTypes fg, FastExtents e );
 void box( FastGuiBoxTypes fg, int x, int y, int w, int h );
 void box( Crayon c, FastGuiBoxTypes fg, FastExtents e );
 void box( Crayon c, FastGuiBoxTypes fg, int x, int y, int w, int h, Blends blend=none );
 void box( Crayon c, FastGuiBoxTypes fg, double x, double y, double w, double h, Blends blend=none );
 void bargraph( Crayon empty, Crayon full, double filled, int x, int y, int w, int h, bool disabled=false );
 void bargraphv( Crayon empty, Crayon full, double filled, int x, int y, int w, int h, bool disabled=false );
 void bargraph( const char *label, Crayon empty, Crayon full, double filled, int x, int y, int w, int h, bool disabled=false );
 void bargraphv( const char *label, Crayon empty, Crayon full, double filled, int x, int y, int w, int h, bool disabled=false );
 void bargraphcap( const char *label, Crayon empty, Crayon full, double filled, int x, int y, int w, int h, bool disabled=false );
 bool bbutton( FastGuiBoxTypes fg, Crayon c, int x, int y, int w, int h, bool selection=false );
 bool bbutton( FastGuiBoxTypes fg, int x, int y, int w, int h, bool selection=false );
 bool bbutton( FastGuiBoxTypes fg, const char *label, int x, int y, int w, int h );
 ////
 bool rrbutton( GLImage *label, FastExtents e );
 bool rrbutton( GLImage *label, int x, int y, int w, int h );
 bool rrbutton( GLImage *label, FastExtents e, bool selection );
 bool rrbutton( GLImage *label, int x, int y, int w, int h, bool selection );
 bool rrbutton( const char *label, FastExtents e );
 bool rrbutton( const char *label, int x, int y, int w, int h );
 bool rrbutton( const char *label, FastExtents e, bool selection );
 bool rrbutton( const char *label, int x, int y, int w, int h, bool selection );
 bool rrbutton( const char *label, FastExtents e, bool selection, int indent );
 bool rrbutton( const char *label, int x, int y, int w, int h, bool selection, int indent );
 bool rrsmbutton( const char *label, FastExtents e );
 bool rrsmbutton( const char *label, int x, int y, int w, int h );
 bool rrsmbutton( const char *label, FastExtents e, bool selection );
 bool rrsmbutton( const char *label, int x, int y, int w, int h, bool selection );
 ////
 bool hoverarea( int x, int y, int w, int h );
 bool buttonarea( int x, int y, int w, int h );
 bool hoverarea( double x, double y, double w, double h )  { return hoverarea( (int) x, (int) y, (int) w, (int) h ); }
 bool buttonarea( double x, double y, double w, double h ) { return buttonarea( (int) x, (int) y, (int) w, (int) h ); }
 bool button( GLImage *label, FastExtents e );
 bool button( GLImage *label, int x, int y, int w, int h );
 bool button( GLImage *label, FastExtents e, bool selection );
 bool button( GLImage *label, int x, int y, int w, int h, bool selection );
 bool button( FastExtents e );
 bool button( int x, int y, int w, int h );
 bool button( const char *label, FastExtents e );
 bool button( const char *label, int x, int y, int w, int h );
 bool tbutton( const char *label, FastExtents e, bool selection );
 bool tbutton( const char *label, int x, int y, int w, int h, bool selection );
 bool button( const char *label, FastExtents e, bool selection );
 bool button( const char *label, int x, int y, int w, int h, bool selection );
 bool button( const char *label, FastExtents e, bool selection, int indent );
 bool button( const char *label, int x, int y, int w, int h, bool selection, int indent );
 bool smbutton( const char *label, FastExtents e );
 bool smbutton( const char *label, int x, int y, int w, int h );
 bool smbutton( const char *label, FastExtents e, bool selection );
 bool smbutton( const char *label, int x, int y, int w, int h, bool selection );
 bool button(  GLImage *backing, const char *label, FastExtents e, bool selection );
 bool button( GLImage *backing, const char *label, int x, int y, int w, int h, bool selection );
 bool button( TexFont *font, const char *label, FastExtents e, bool selection=false );
 bool button( TexFont *font, const char *label, int x, int y, int w, int h, bool selection=false, int charspace=-1024 );
 bool button( TexFont *font, const char *label, int cx, int cy );
 bool ljustbutton( TexFont *font, const char *label, int x, int y, int w, int h, bool selection=false, int charspace=-1024 );
 bool button( TexFont *font, const char *label, Crayon c, FastExtents e, bool selection=false );
 bool button( TexFont *font, const char *label, Crayon c, int x, int y, int w, int h, bool selection=false );
 void dbutton( GLImage *label, FastExtents e );
 void dbutton( GLImage *label, int x, int y, int w, int h );
 void dbutton( TexFont *font, const char *label, FastExtents e );
 void dbutton( TexFont *font, const char *label, int x, int y, int w, int h );
 bool dbbutton( TexFont *font, const char *label, FastExtents e );
 bool dbbutton( TexFont *font, const char *label, int x, int y, int w, int h );
 bool button( TexFont *font, const char *label, FastExtents e, int fw, int fh, bool selection=false );
 bool button( TexFont *font, const char *label, int x, int y, int w, int h, int fw, int fh, bool selection=false );
 void dbutton( TexFont *font, const char *label, FastExtents e, int fw, int fh );
 void dbutton( TexFont *font, const char *label, int x, int y, int w, int h, int fw, int fh );
 bool button( GLImage *backing, TexFont *font, const char *label, FastExtents e, bool selection=false, Blends b=transparency );
 bool button( GLImage *backing, TexFont *font, const char *label, int x, int y, int w, int h, bool selection=false, Blends b=transparency );
 bool button( GLImage *backing, TexFont *font, const char *label, FastExtents e, int limitedw, bool selection=false, Blends b=transparency );
 bool button( GLImage *backing, TexFont *font, const char *label, int x, int y, int w, int h, int limitedw, bool selection=false, Blends b=transparency );
 bool button( GLImage *icon, GLImage *backing, FastExtents e, Crayon b, Crayon i, Crayon d, bool disabled=false, Blends blend=transparency  );
 bool button( GLImage *icon, GLImage *backing, int x, int y, int w, int h, Crayon b, Crayon i, Crayon d, bool disabled=false, Blends blend=transparency  );
 bool backlessbutton( const char *label, FastExtents e );
 bool backlessbutton( const char *label, int x, int y, int w, int h );
 bool squarebutton( GLImage *icon, GLImage *backing, FastExtents e, Crayon b, Crayon i, Crayon d, bool disabled=false, Blends blend=transparency  );
 bool squarebutton( GLImage *icon, GLImage *backing, int x, int y, int w, int h, Crayon b, Crayon i, Crayon d, bool disabled=false, Blends blend=transparency  );
 bool squaredbutton( GLImage *icon, GLImage *backing, FastExtents e, Crayon b, Crayon i, Crayon d, bool disabled=false, Blends blend=transparency  );
 bool squaredbutton( GLImage *icon, GLImage *backing, int x, int y, int w, int h, Crayon b, Crayon i, Crayon d, bool disabled=false, Blends blend=transparency  );
 bool button( GLImage *backing, TexFont *font, const char *label, FastExtents e, int fw, int fh, bool selection, Blends b=transparency  );
 bool button( GLImage *backing, TexFont *font, const char *label, int x, int y, int w, int h, int fw, int fh, bool selection=false, Blends b=transparency );
 ////
 bool toggle( const char *label, bool value, FastExtents e, Blends b=transparency );
 bool toggle( const char *label, bool value, int  x, int y, int w, int h, Blends b=transparency );
 bool toggle( const char *labelTop, const char *labelBottom, const char *label, bool value, FastExtents e );
 bool toggle( const char *labelTop, const char *labelBottom, const char *label, bool value, int  x, int y, int w, int h );
 ////
 int numeric( const char *label, int value, int lowest, int highest, FastExtents e, int characters=5 );
 int numeric( const char *label, int value, int lowest, int highest, int x, int y, int w, int h, int characters=5 );
 int integer( const char *label, int value, int lowest, int highest, int delta, FastExtents e, int characters=5, bool deltaOk=true );
 int integer( const char *label, int value, int lowest, int highest, int delta, int x, int y, int w, int h, int characters=5, bool deltaOk=true );
 int integerNL( const char *label, int value, int delta, FastExtents e, int characters=5, bool deltaOk=true );
 int integerNL( const char *label, int value, int delta, int x, int y, int w=32, int h=16, int characters=5, bool deltaOk=true );
 int integerL( const char *label, int value, int lowest, int delta, FastExtents e, int characters=5, bool deltaOk=true );
 int integerL( const char *label, int value, int lowest, int delta, int x, int y, int w=32, int h=16, int characters=5, bool deltaOk=true );
 double decimal( const char *label, double value, double lowest, double highest, double delta, FastExtents e, int characters=7, bool deltaOk=true );
 double decimal( const char *label, double value, double lowest, double highest, double delta, int x, int y, int w, int h, int characters=7, bool deltaOk=true );
 double precise( const char *label, double value, double lowest, double highest, double delta, FastExtents e, int characters=7, bool deltaOk=true );
 double precise( const char *label, double value, double lowest, double highest, double delta, int x, int y, int w, int h, int characters=7, bool deltaOk=true );
 ////
 int enumerated( Strings *values, const char *label, int value, FastExtents e, int characters=5 );
 int enumerated( Strings *values, const char *label, int value, int x, int y, int w, int h, int characters=5 );
 int enumerated( Strings *values, const char *label, int value, FastExtents e, having values_integer, int characters=5 );
 int enumerated( Strings *values, const char *label, int value, int x, int y, int w, int h, having values_integer, int characters=5 );
 int enumerated( Strings *values, const char *label, int value, int lowest, int highest, FastExtents e, int characters=5 );
 int enumerated( Strings *values, const char *label, int value, int lowest, int highest, int x, int y, int w, int h, int characters=5 );
 String *enumerated( Strings *values, const char *label, String *value, FastExtents e, int characters=5 );
 String *enumerated( Strings *values, const char *label, String *value, int x, int y, int w, int h, int characters=5 );
 Blends blend( const char *label, Blends value, int x, int y, int w, int h, int characters=5 );
 ////
 void hrange( const char *label, double *value1, double *value2, double lowest, double highest, int x, int y, int w, int h=16, bool simple=false );
 ////
 double hslider( const char *label, double value, double lowest, double highest, int x, int y, int w, int h=16, bool simple=false);
 int hslider( const char *label, int value, int lowest, int highest, int x, int y, int w, int h=16, bool simple=false);
 double vslider( const char *label, double value, double lowest, double highest, int x, int y, int w, int h, bool simple = false, bool superSimple = false );
 double vslider( having upIsDown, const char *label, double value, double lowest, double highest, int x, int y, int w, int h, bool simple = false, bool superSimple = false );
 int menu(TexFont *font,Strings *options,int x, int y, int cw=256, int oh=32, int itemSpacing=8, const char *sound="data/sounds/UI/tinybeep.wav", float pitch=1.0f, float gain=0.75f );
 void tandem(const char *label1, const char *label2, int x, int y, int cw, int lines, Strings* left, Integers * leftSelected, int * leftScroll, Strings * right, Integers * rightSelected, int * rightScroll, bool uniqueOnly=true, bool useButton = true);
 ////
 int list(LinkedList * values, int x, int y, int cw, int lines, int *scroll); int list(LinkedList * values, int x, int y, int cw, int lines, int *selected, int *scroll );
 int list(const char * label, Strings * values, int x, int y, int cw, int lines, int *scroll);
 int list(const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int *scroll);
 int list(TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int *scroll,int linespace=4,int charspace=2, bool showRectangle=false, int limitString=-1 );
 void reorder( LinkedList * values, int x, int y, int cw, int lines, Zp<ListItem> *selected, int *scroll );
 int reorder(TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int *scroll,int linespace=4,int charspace=2, bool showRectangle=false, int limitString=-1 );
 // selectable list of strings that contain valid double values
 void decimalstringlist(TexFont *font, const char * label, Strings * values, int x, int y, int w, int lines, int *scroll, double lowest, double highest, double delta, int linespace=4, int charspace=2, bool canAdd=true, int limitString=20, const char *placeholder="" );
 int list(TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int *scroll,bool tailEnd,int charLimit=-1,int linespace=4,int charspace=2);
 // scrollable clickable left justified button list
 int ljustlist( TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *scroll, int scrollw=10, int linespace=4, int charspace=-2, bool no_rectangle=true);
 // scrollable clickable left justified button list
 int ljustlist( TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int *scroll, int scrollw=10, int linespace=4, int charspace=-2, bool no_rectangle=true);
 // clickable, scrollable center justified button list
 int list( TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *scroll,int linespace=4,int charspace=2, bool no_rectangle=false);
 // scrollable clickable centered list with title using a texfont with custom backing
 int list( GLImage *backing, TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *scroll, int linespace=4,int charspace=2);
 // selectable scrollable clickable centered list with title using a texfont with custom backing
 int list( GLImage *backing, TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int *scroll, int linespace=4,int charspace=2);
 // multi-selectable scrollable clickable centered list with title using a texfont with custom backing
 bool list( GLImage *backing, TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, Numbers *selected, int *scroll, int linespace=4,int charspace=2);
 // multi-selectable scrollable clickable centered list with title using a texfont
 bool list( TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, Numbers *selected, int *scroll, int linespace=4,int charspace=2);
 ////
 int tabs(const char * label, Strings * values, int x, int y, int cw, int *selected, int *scroll,int linespace=4,int charspace=2, int limitString=-1 );
 int tabs(TexFont *font, const char * label, Strings * values, int x, int y, int cw, int *selected, int *scroll,int linespace=4,int charspace=2, int limitString=-1 );
 int togglelist( TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *scroll);
 int hlist(const char * label, Strings * values, int x, int y, int cw, int lines, int *selected=null, int *scroll=null,int linespace=4,int charspace=2, bool showRectangle=false );
 int hlist(TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int *scroll,int linespace=4,int charspace=2, bool showRectangle=false );
 int tabs(  Strings *options, int selected, int x, int y, int cw, int bh );
 int tabs( TexFont *f, Strings *options, int selected, int x, int y, int cw, int bh, int spacing=16 );
 int quicktabs( TexFont *font, const char *label, LinkedList *list, int x, int y, int cw, ListItem *selected, int linespace=4, int charspace=2, int limitString=3, bool canAdd=true );
 int quicktabs( const char *label, LinkedList *list, int x, int y, int cw, ListItem *selected, int linespace=4, int charspace=2, int limitString=3, bool canAdd=true );
 int quicklist( TexFont *font, const char *label, LinkedList *L, int x, int y, int cw, ListItem *selected, int lines, int *scroll=null, int linespace=4, int charspace=2, bool canAdd=true );
 void order(const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int * scroll);
 ////
 string textbox( const char *label, string value, FastExtents e, int characters, bool limited=false, int lines=1, int lineSpace=2, int border=4 );
 string textbox( const char *label, string value, int x, int y, int characters, bool limited=false, int lines=1, int lineSpace=2, int border=4 );
 string textbox( TexFont *font, TexFont *labelFont, const char *label, string value, FastExtents e, int characters, bool limited=false, int lines=1, int lineSpace=2, int border=4, bool alwaysKeys=false, int kern=0, Zstring *goFullscreen=null, GLWindow *back_to=null, bool subcaption=false, bool textCloserMode=false, bool useTerminal=true );
 string textbox( TexFont *font, Blends fontBlend, TexFont *labelFont, Blends labelFontBlend, const char *label, string value, int x, int y, int characters, bool limited=false, int lines=1, int lineSpace=2, int border=4, bool alwaysKeys=false, int kern=0, Zstring *goFullscreen=null, GLWindow *back_to=null, bool subcaption=false, bool textCloserMode=false, bool useTerminal=true );
 string stringbox( TexFont *font, Blends fontBlend, TexFont *labelFont, Blends labelFontBlend, const char *label, string value, int x, int y, int characters, bool limited=false, int lines=1, int lineSpace=2, int border=4, bool alwaysKeys=false, int kern=0, String *goFullscreen=null, GLWindow *back_to=null, bool subcaption=false, bool textCloserMode=false );
 ////
 void rect( int rectType, FastExtents e );
 void rect( int rectType, int x, int y, int w, int h );
 void rect( Crayon c, int rectType, FastExtents e );
 void rect( Crayon c, int rectType, int x, int y, int w, int h );
 void rrect( Crayon border, Crayon inner, Crayon outer, FastExtents e );
 void rrect( Crayon border, Crayon inner, Crayon outer, int x, int y, int w, int h );
 //// Text drawing.
 void textsize( Dim2i *out, TexFont *font, const char *txt );
 void textsize( Dim2i *out, TexFont *font, const char *txt, int charspace );
 void textsize( Dim2i *out, TexFont *font, const char *txt, int charspace, int linespace );
 void textsize( Dim2d *out, const char *txt, double fw, double fh, double linespace );
 void textsize( Dim2d *out, const char *txt, double fw, double fh );
 void textsize( Dim2d *out, const char *txt );
 TexFont *fontSelect( TexFont *value, int x, int y, int w, int h );
 ////
 void text( const char *label, int x, int y, bool centered=false, Crayon c=crayons.Pick(alabaster), Blends b=transparency );
 void boxedtext( const char *label, int x, int y, bool centered=false, Crayon c=crayons.Pick(gray), Crayon bord=crayons.Pick(darkIndigo), Crayon back=crayons.Pick(black), int padding=4, bool above=false );
 void boxedtextrjust( const char *label, int x, int y, bool centered=false, Crayon c=crayons.Pick(gray), Crayon bord=crayons.Pick(darkIndigo), Crayon back=crayons.Pick(black) );
 int types( int typing, TexFont *font, const char *text, Crayon tint, int x, int y, int fw, int fh, int cs=0, int ls=3 );
 void text( TexFont *font, const char *label, FastExtents e, int cs=0, int ls=3, Crayon c=crayons.Pick(alabaster), Blends b=transparency );
 void text( TexFont *font, const char *label, int x, int y, int fw, int fh, int cs=0, int ls=3, Crayon c=crayons.Pick(alabaster), Blends b=transparency );
 void text( TexFont *font, const char *label, int x, int y, Crayon c=crayons.Pick(alabaster), Blends b=transparency, int cs=0, int ls=3 );
 void ljust( TexFont *font, const char *label, int x, int y, int fw, int fh, int cs=0, int ls=3, Crayon c=crayons.Pick(alabaster), Blends b=transparency );
 void ljust( TexFont *font, const char *label, int x, int y, int cs=0, int ls=3, Crayon c=crayons.Pick(alabaster), Blends b=transparency );
 ////
 void image( GLImage *i, int x, int y, int w, int h, Crayon t, Blends b=transparency );
 ////
 GLImage *imagepick( GLImage *value, const char *label, FileDialog *picker, Zstring *bound, Crayon imageTint, int x, int y, int w, int h, bool wrapImages=false, bool allowNull=false );
 GLImage *imagepick( TexFont *font, GLImage *value, const char *label, FileDialog *picker, Zstring *bound, Crayon imageTint, int x, int y, int w, int h, bool wrapImages=false, bool allowNull=false );
 GLImage *imagepick( GLImage *value, const char *label, FileDialog *picker, Crayon imageTint, int x, int y, int w, int h, bool wrapImages=false, bool allowNull=false );
 GLImage *imagepick( TexFont *font, GLImage *value, const char *label, FileDialog *picker, Crayon imageTint, int x, int y, int w, int h, bool wrapImages=false, bool allowNull=false );
 ////
 int arrows( int x, int y, int bw, bool disabled=false, bool showDelta=false, int delta=1 );
 int arrows( const char *label, int x, int y, int bw, bool disabled=false, bool showDelta=false, double delta=1.0 );
 void arrows( const char *label, Cartesian *xy, int x, int y, int bw, bool disabled=false, int delta=1 );
 void arrows( const char *label, Cartesiand *xy, int x, int y, int bw, bool disabled=false, double delta=1.0 );
 ////
 Zint colorPickerFrame;
 Crayon pick( const char *label, Crayon value, int x, int y );
 int colorPicker( int value, int x, int y, int cellSize=8, bool Alpha=false, GLfloat rectThickness=1.0f );
 Crayon colorPicker( Crayon cvalue, int x, int y, int cellSize=8, bool Alpha=false, GLfloat rectThickness=1.0f );
 int c16x16Picker( int value, int x, int y, int cellSize=8, bool Alpha=false, GLfloat rectThickness=1.0f );
 int c8x16Picker( int value, int x, int y, int cellSize=8, bool Alpha=false, GLfloat rectThickness=1.0f );
 int c4x16Picker( int value, int x, int y, int cellSize=8, bool Alpha=false, GLfloat rectThickness=1.0f );
 int c8x8Picker( int value, int x, int y, int cellSize=8, bool Alpha=false, GLfloat rectThickness=1.0f );
 fx_UglyRGBPal _rgbPal;
 Crayon colorPickerTiny( Crayon tint, int x, int y, int w, int h );
 // Tween selector
 Tween *tween( Tween *selected, int x, int y, int w, int h );
 ////
 void xy( Cartesiand *inOut, int x, int y, double xymin=-10000, double xymax=10000, const char *label="" );
 void xy( Cartesian *inOut, int x, int y, int xymin=-10000, int xymax=10000, const char *label="" );
 void xy( having precision, Cartesiand *inOut, int x, int y, double xymin=-10000, double xymax=10000, const char *label="" );
 void xy( ScreenPosition *inOut, int x, int y, double xymin=-10000, double xymax=10000, const char *label="" );
 void wh( Cartesiand *inOut, int x, int y, double xymin=-10000, double xymax=10000, const char *label="" );
 void wh( Cartesian *inOut, int x, int y, int xymin=-10000, int xymax=10000, const char *label="" );
 void wh( having precision, Cartesiand *inOut, int x, int y, double xymin=-10000, double xymax=10000, const char *label="" );
 void wh( ScreenPosition *inOut, int x, int y, double xymin=-10000, double xymax=10000, const char *label="" );
 void xywhi( Cartesiand *inOut, int x, int y, int xymin=-10000, int xymax=10000, const char *label="" );
 void xyz( Cartesiand *inOut, int x, int y, double xymin=-10000, double xymax=10000, const char *label="" );
 void xywh( Cartesiand *inOut, int x, int y, double xymin=-10000, double xymax=10000, const char *label="" );
 void xywh( Cartesiand *inOut, having differentRangeWH, int x, int y, double xymin=-10000, double xymax=10000, double whmin=0.0, double whmax=10000, const char *label="" );
 void xywh( Cartesiand *inOut, having scaleWH, int x, int y, double xymin=-10000, double xymax=10000, double whmin=0.0, double whmax=10.0, double whDeltaFactor=0.01, const char *label="" );
 void xyzw( Cartesiand *inOut, int x, int y, double xymin=-10000, double xymax=10000, const char *label="" );
 void xyz( having preciseDecimal, Cartesiand *inOut, int x, int y, double xymin=-10000, double xymax=10000, const char *label="" );
 void xywh( having preciseDecimal, Cartesiand *inOut, int x, int y, double xymin=-10000, double xymax=10000, const char *label="" );
 void xyzw( having preciseDecimal, Cartesiand *inOut, int x, int y, double xymin=-10000, double xymax=10000, const char *label="" );
 void xywh( Cartesian *inOut, int x, int y, int xymin=-10000, int xymax=10000, const char *label="" );
 void xywh( ScreenPosition *inOut, int x, int y, double xymin=-10000, double xymax=10000, const char *label="" );
 void xyAngle( double *xx, double *yy, int x, int y, int maxX=16, int maxY=16 );
 double angleDegrees( double in, int x, int y, int r=16 );
 // File dialog
 bool fileDialog(FileDialog *in, int x, int y, int w=64, int h=16);
 GLImage *imagePicker(FileDialog *in, GLImage *value,int x, int y, int w=64, int h=64, int bh=16,bool xToNull=false);
 GLImage *imagePicker(FileDialog *in, GLImage *value,Crayon tint,int x, int y, int w=64, int h=64, int bh=16,bool xToNull=false);
 GLImage *imagePicker(FileDialog *in, GLImage *value,Crayon tint,Blends b,int x, int y, int w=64, int h=64, int bh=16,bool xToNull=false);
 ////
 Film *FilmPicker( std::string *va, std::string *fi, double *instant, int *scrolled, int x, int y, int w=384, int h=256, const char *label="FILM Vault" );
 Film *FilmPicker( std::string *va, std::string *fi, int *scrolled, int x, int y, int w=384, int h=256 );
 Film *FilmPicker( TexFont *font, std::string *va, std::string *fi, double *instant, int *scrolled, int x, int y, int w=384, int h=256, int linespace=4,int charspace=2,bool showrectangle=false );
 // WIP
 void paragraph( TexFont *font, const char *label, int x, int y, int fw, int fh, int cs=0, int ls=3, Crayon c=crayons.Pick(alabaster), Blends b=transparency ) {
  int gw=WidthTexFont(font,fw,fontCharSpace.value,label);
  Text(font, c, b,
   (x)-gw/2, (y)-(int)((float)fh*(font->baselineOffset))/2,
   fw,fh,ls,cs,label
  );
 }
 bool CTRL()  { return input.KeyDown(DX_LCONTROL) || input.KeyDown(DX_RCONTROL); }
 bool ALT()   { return input.KeyDown(DX_LALT)     || input.KeyDown(DX_RALT);     }
 bool SHIFT() { return input.KeyDown(DX_LSHIFT)   || input.KeyDown(DX_RSHIFT);   }
 // Call only once per frame no matter how many uses of fastGUI concurrently
 Zdouble instant;
 void FrameStart() {
  hovered=false;
  cPalHovering=false;
  rgbPalHovering=false;
  (instant)+=FRAMETIME;
  if ( ignoreNextRelease > 0.0 ) {
   ignoreNextRelease-=FRAMETIME;
   if ( ignoreNextRelease < 0.0 ) ignoreNextRelease=0.0;
  }
 }
 void Clear() {
  rrects.Clear();
 }
 
 void SaveState() {
  savedState.hover                       = hover                      ;
  savedState.highlight                   = highlight                  ;
  savedState.color                       = color                      ;
  savedState.tint                        = tint                       ;
  savedState.selected                    = selected                   ;
  savedState.activated                   = activated                  ;
  savedState.deactivated                 = deactivated                ;
  savedState.excited                     = excited                    ;
  savedState.indicated                   = indicated                  ;
  savedState.rr_hover_inner              = rr_hover_inner             ;
  savedState.rr_hover_outer              = rr_hover_outer             ;
  savedState.rr_hover_border             = rr_hover_border            ;
  savedState.rr_hover_text               = rr_hover_text              ;
  savedState.rr_select_inner             = rr_select_inner            ;
  savedState.rr_select_outer             = rr_select_outer            ;
  savedState.rr_select_border            = rr_select_border           ;
  savedState.rr_select_text              = rr_select_text             ;
  savedState.rr_active_inner             = rr_active_inner            ;
  savedState.rr_active_outer             = rr_active_outer            ;
  savedState.rr_active_border            = rr_active_border           ;
  savedState.rr_active_text              = rr_active_text             ;
  savedState.rr_active_select_inner      = rr_active_select_inner     ;
  savedState.rr_active_select_outer      = rr_active_select_outer     ;
  savedState.rr_active_select_border     = rr_active_select_border    ;
  savedState.rr_active_select_text       = rr_active_select_text      ;
  savedState.rr_hover_active_inner       = rr_hover_active_inner      ;
  savedState.rr_hover_active_outer       = rr_hover_active_outer      ;
  savedState.rr_hover_active_border      = rr_hover_active_border     ;
  savedState.rr_hover_active_text        = rr_hover_active_text       ;
  savedState.rr_inner                    = rr_inner                   ;
  savedState.rr_outer                    = rr_outer                   ;
  savedState.rr_border                   = rr_border                  ;
  savedState.rr_text                     = rr_text                    ;
  savedState.point.x    = point.x;
  savedState.point.y    = point.y;
  savedState.fontSize.x = fontSize.x;
  savedState.fontSize.y = fontSize.y;
  savedState.tfont.w    = tfont.w;
  savedState.tfont.h    = tfont.h;
  savedState.font       = font;
  savedState.tblend     = tblend;
  savedState.fontW      = fontW;
  savedState.fontH      = fontH;
  savedState.smallFontW = smallFontW;
  savedState.smallFontH = smallFontH;
  savedState.charSpace  = charSpace;
  savedState.lineSpace  = lineSpace;
  savedState.fontCharSpace  = fontCharSpace;
  savedState.fontLineSpace  = fontLineSpace;
  savedState.responsiveRatio.Set(&responsiveRatio);
  savedState.reciprocalRatio.Set(&reciprocalRatio);
  savedState.b100.Set(&b100);
  savedState.scope.Set(&scope);
  savedState.scopeRatio.x  = scopeRatio.x;
  savedState.scopeRatio.y  = scopeRatio.y;
  savedState.screenRatio.x = screenRatio.x;
  savedState.screenRatio.y = screenRatio.y;
  savedState.rect_radius      = rect_radius;
  savedState.rect_precision   = rect_precision;
  savedState.rect_borderwidth = rect_borderwidth;
  savedState.vdisplay.Set(&vdisplay);
  savedState.mouse.Set(&mouse);
 }
 void RecallState() {
  hover                       = savedState.hover                      ;
  highlight                   = savedState.highlight                  ;
  color                       = savedState.color                      ;
  tint                        = savedState.tint                       ;
  selected                    = savedState.selected                   ;
  activated                   = savedState.activated                  ;
  deactivated                 = savedState.deactivated                ;
  excited                     = savedState.excited                    ;
  indicated                   = savedState.indicated                  ;
  rr_hover_inner              = savedState.rr_hover_inner             ;
  rr_hover_outer              = savedState.rr_hover_outer             ;
  rr_hover_border             = savedState.rr_hover_border            ;
  rr_hover_text               = savedState.rr_hover_text              ;
  rr_select_inner             = savedState.rr_select_inner            ;
  rr_select_outer             = savedState.rr_select_outer            ;
  rr_select_border            = savedState.rr_select_border           ;
  rr_select_text              = savedState.rr_select_text             ;
  rr_active_inner             = savedState.rr_active_inner            ;
  rr_active_outer             = savedState.rr_active_outer            ;
  rr_active_border            = savedState.rr_active_border           ;
  rr_active_text              = savedState.rr_active_text             ;
  rr_active_select_inner      = savedState.rr_active_select_inner     ;
  rr_active_select_outer      = savedState.rr_active_select_outer     ;
  rr_active_select_border     = savedState.rr_active_select_border    ;
  rr_active_select_text       = savedState.rr_active_select_text      ;
  rr_hover_active_inner       = savedState.rr_hover_active_inner      ;
  rr_hover_active_outer       = savedState.rr_hover_active_outer      ;
  rr_hover_active_border      = savedState.rr_hover_active_border     ;
  rr_hover_active_text        = savedState.rr_hover_active_text       ;
  rr_inner                    = savedState.rr_inner                   ;
  rr_outer                    = savedState.rr_outer                   ;
  rr_border                   = savedState.rr_border                  ;
  rr_text                     = savedState.rr_text                    ;
  point.x    = savedState.point.x;
  point.y    = savedState.point.y;
  fontSize.x = savedState.fontSize.x;
  fontSize.y = savedState.fontSize.y;
  tfont.w    = savedState.tfont.w;
  tfont.h    = savedState.tfont.h;
  font       = savedState.font;
  tblend     = savedState.tblend;
  fontW      = savedState.fontW;
  fontH      = savedState.fontH;
  smallFontW = savedState.smallFontW;
  smallFontH = savedState.smallFontH;
  charSpace  = savedState.charSpace;
  lineSpace  = savedState.lineSpace;
  responsiveRatio.Set(&savedState.responsiveRatio);
  reciprocalRatio.Set(&savedState.reciprocalRatio);
  b100.Set(&savedState.b100);
  scope.Set(&savedState.scope);
  scopeRatio.x  = savedState.scopeRatio.x;
  scopeRatio.y  = savedState.scopeRatio.y;
  screenRatio.x = savedState.screenRatio.x;
  screenRatio.y = savedState.screenRatio.y;
  rect_radius      = savedState.rect_radius;
  rect_precision   = savedState.rect_precision;
  rect_borderwidth = savedState.rect_borderwidth;
  vdisplay.Set(&savedState.vdisplay);
  mouse.Set(&savedState.mouse);
 }
};

extern FastGUI fast;