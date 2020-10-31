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
#include "Art.h"
#include "GLWindow.h"
#include "UglyFont.h"

class TouchTest : public GLWindow {
 Crayon touchColor,areaColor;
 Zint releases;
 void OnLoad() {
  Anchor(0.05f,0.05f,0.05f,0.05f);
  background.Int(0,0,64,255);
  touchColor.Int(255,0,0,255);
  areaColor.Int(255,0,0,255);
 }
 void Between() {
 }
 void RenderTouch ( int i, Touch *t ) {
  glColor3d(0.6,0.6,0.6);
  DrawLine(touchColor,0,0,t->tx,t->ty);
  DrawLine(areaColor,0,0,t->cx,t->cy);
  string out=t->asString();
  Text(FORMAT("Touch %c%d: %s", '#', i, (char *) out.c_str() ),512,512+i*18*2,16,16,false);
 }
 void RenderMouse() {
  DrawLine(touchColor,input->mxi,0,input->mxi,display->h);
  DrawLine(touchColor,0,input->myi,display->w,input->myi);
  Text(
   FORMAT("Mouse: %d,%d Left: %d / %d Right: %d / %d Touches: %d\nReleases: %d",input->mxi,input->myi,(int)input->left,(int)input->wasLeft,(int)input->right,(int)input->wasRight,(int)input->touches.count,(int) releases).c_str(),
   input->mxi+5,input->myi+5, 16,16,false,0
  );
  for ( int i=0; i<MAXPOINTS; i++ ) {
   Text(
    FORMAT("Points[%d] = %d,%d id %d",(int)i,(int)touchpoints[i][0],(int)touchpoints[i][1],(int)tidLookup[i]).c_str(),
    32,32+i*30, 16,16,false,0
    );
  }
 }
 void Render() {
  if ( input->leftReleased() ) releases++;
  if ( input->touches.count > 0 ) {
   int i=0;
   EACH(input->touches.first,Touch,t) RenderTouch(i++,t);
  }
  RenderMouse();
  glColor3d(0.6,0.6,0.6);
  Text(
   FORMAT("Mouse X,Y: %d,%d",(int)input->mxi,(int)input->myi).c_str(),
   128,128,16,16,false,0);
 }
};
