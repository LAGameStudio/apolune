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

/**************************************************************
  Author: H. Elwood Gilliland III
  Test case for FastGUI
 **************************************************************/

#include "GLWindow.h"
#include "FastGUI.h"

class FastGUITest : public GLWindow {
public:
 Zbool togglable,selected;
 Zdouble tickDelay;
 Zuint tick;
 Zstring text;
 void OnLoad() {
  background.Pick(indigo);
  Fullscreen();
  fast.tfont.w=64;
  fast.tfont.h=72;
  fast.charSpace=-8;
 }
 void Between() {
  tickDelay-=FRAMETIME;
  if ( tickDelay <= 0.0 ) {
   tickDelay=2.0f;
   tick += 1;
   if ( tick.value > 10 ) tick=0;
  }
 }
 void Render() {
  crayons.Pick(hotPink).gl();
#if defined(NEVER)
  fast.mC.Render2dVC(1024.0f,16.0f, 24.0f,24.0f);
  fast.mD.Render2dVC(1024.0f,64.0f, 24.0f,24.0f);
  fast.mE.Render2dVC(1024.0f,128.0f,24.0f,24.0f);
  fast.mF.Render2dVC(1024.0f,200.0f,24.0f,24.0f);
  fast.mG.Render2dVC(1024.0f,256.0f,24.0f,24.0f);
  fast.mH.Render2dVC(1024.0f,300.0f,24.0f,24.0f);
  fast.mI.Render2dVC(1024.0f,350.0f,24.0f,24.0f);
  fast.mJ.Render2dVC(1024.0f,400.0f,24.0f,24.0f);
  fast.mK.Render2dVC(1024.0f,450.0f,24.0f,24.0f);
  fast.mL.Render2dVC(1024.0f,500.0f,24.0f,24.0f);
  fast.mM.Render2dVC(1024.0f,550.0f,24.0f,24.0f);
  
  fast.mC.Render2dVC(1124.0f, 16.0f,12.0f,12.0f);
  fast.mD.Render2dVC(1124.0f, 64.0f,12.0f,12.0f);
  fast.mE.Render2dVC(1124.0f,128.0f,12.0f,12.0f);
  fast.mF.Render2dVC(1124.0f,200.0f,12.0f,12.0f);
  fast.mG.Render2dVC(1124.0f,256.0f,12.0f,12.0f);
  fast.mH.Render2dVC(1124.0f,300.0f,12.0f,12.0f);
  fast.mI.Render2dVC(1124.0f,350.0f,12.0f,12.0f);
  fast.mJ.Render2dVC(1124.0f,400.0f,12.0f,12.0f);
  fast.mK.Render2dVC(1124.0f,450.0f,12.0f,12.0f);
  fast.mL.Render2dVC(1124.0f,500.0f,12.0f,12.0f);
  fast.mM.Render2dVC(1124.0f,550.0f,12.0f,12.0f);
  
  fast.mC.Render2dVC(1324.0f, 16.0f,16.0f,16.0f);
  fast.mD.Render2dVC(1324.0f, 64.0f,16.0f,16.0f);
  fast.mE.Render2dVC(1324.0f,128.0f,16.0f,16.0f);
  fast.mF.Render2dVC(1324.0f,200.0f,16.0f,16.0f);
  fast.mG.Render2dVC(1324.0f,256.0f,16.0f,16.0f);
  fast.mH.Render2dVC(1324.0f,300.0f,16.0f,16.0f);
  fast.mI.Render2dVC(1324.0f,350.0f,16.0f,16.0f);
  fast.mJ.Render2dVC(1324.0f,400.0f,16.0f,16.0f);
  fast.mK.Render2dVC(1324.0f,450.0f,16.0f,16.0f);
  fast.mL.Render2dVC(1324.0f,500.0f,16.0f,16.0f);
  fast.mM.Render2dVC(1324.0f,550.0f,16.0f,16.0f);

  fast.mC.Render2dVC(1224.0f, 16.0f,48.0f,48.0f);
  fast.mD.Render2dVC(1224.0f, 64.0f,48.0f,48.0f);
  fast.mE.Render2dVC(1224.0f,128.0f,48.0f,48.0f);
  fast.mF.Render2dVC(1224.0f,200.0f,48.0f,48.0f);
  fast.mG.Render2dVC(1224.0f,256.0f,48.0f,48.0f);
  fast.mH.Render2dVC(1224.0f,300.0f,48.0f,48.0f);
  fast.mI.Render2dVC(1224.0f,350.0f,48.0f,48.0f);
  fast.mJ.Render2dVC(1224.0f,400.0f,48.0f,48.0f);
  fast.mK.Render2dVC(1224.0f,450.0f,48.0f,48.0f);
  fast.mL.Render2dVC(1224.0f,500.0f,48.0f,48.0f);
  fast.mM.Render2dVC(1224.0f,550.0f,48.0f,48.0f);
  
  fast.mC.Render2dVC(824.0f,  16.0f, 100.0f,100.0f);
  fast.mD.Render2dVC(724.0f, 116.0f, 100.0f,100.0f);
  fast.mE.Render2dVC(824.0f, 228.0f, 100.0f,100.0f);
  fast.mF.Render2dVC(724.0f, 300.0f, 100.0f,100.0f);
  fast.mG.Render2dVC(824.0f, 456.0f, 100.0f,100.0f);
  fast.mH.Render2dVC(724.0f, 500.0f, 100.0f,100.0f);
  fast.mI.Render2dVC(824.0f, 650.0f, 100.0f,100.0f);
  fast.mJ.Render2dVC(724.0f, 700.0f, 100.0f,100.0f);
  fast.mK.Render2dVC(824.0f, 850.0f, 100.0f,100.0f);
  fast.mL.Render2dVC(724.0f, 900.0f, 100.0f,100.0f);
  fast.mM.Render2dVC(824.0f, 150.0f, 100.0f,100.0f);
  
  fast.mC.Render2dVC(724.0f,  16.0f, 50.0f,100.0f);
  fast.mD.Render2dVC(624.0f, 116.0f, 50.0f,100.0f);
  fast.mE.Render2dVC(724.0f, 228.0f, 50.0f,100.0f);
  fast.mF.Render2dVC(624.0f, 300.0f, 50.0f,100.0f);
  fast.mG.Render2dVC(724.0f, 456.0f, 50.0f,100.0f);
  fast.mH.Render2dVC(624.0f, 500.0f, 50.0f,100.0f);
  fast.mI.Render2dVC(724.0f, 650.0f, 50.0f,100.0f);
  fast.mJ.Render2dVC(624.0f, 700.0f, 50.0f,100.0f);
  fast.mK.Render2dVC(724.0f, 850.0f, 50.0f,100.0f);
  fast.mL.Render2dVC(624.0f, 900.0f, 50.0f,100.0f);
  fast.mM.Render2dVC(724.0f, 150.0f, 50.0f,100.0f);

  crayons.Pick(yellow).gl();
  switch ( tick.value ) {
   default:
    case 0: fast.mC.Render2dVC( 8, 8, 384, 512 ); break;
    case 1: fast.mD.Render2dVC( 8, 8, 384, 512 ); break;
    case 2: fast.mE.Render2dVC( 8, 8, 384, 512 ); break;
    case 3: fast.mF.Render2dVC( 8, 8, 384, 512 ); break;
    case 4: fast.mG.Render2dVC( 8, 8, 384, 512 ); break;
    case 5: fast.mH.Render2dVC( 8, 8, 384, 512 ); break;
    case 6: fast.mI.Render2dVC( 8, 8, 384, 512 ); break;
    case 7: fast.mJ.Render2dVC( 8, 8, 384, 512 ); break;
    case 8: fast.mK.Render2dVC( 8, 8, 384, 512 ); break;
    case 9: fast.mL.Render2dVC( 8, 8, 384, 512 ); break;
   case 10: fast.mM.Render2dVC( 8, 8, 384, 512 ); break;
  }
#endif // Legacy

  if ( fast.button("Test A Button", 16, 16, 128, 24 ) ) {
   OUTPUT("Test button A pressed.\n");
  }
  togglable=fast.toggle("Toggle Something",togglable.value,128,128,24,24);
  togglable=fast.toggle("A","B","Toggle Something Else",togglable.value,128,128+48,24,24);
  glLineWidth(2);
  if ( fast.button("Test B Button", 256, 512, 128, 24 ) ) {
   OUTPUT("Test button B pressed.\n");
   selected=false;
  }
  glLineWidth(1);
  if ( fast.button("Test C Button", 256, 512+128, 256, 32, selected ) ) {
   OUTPUT("Test button C pressed.\n");
  }

  if ( fast.button(nexaBold,"Test D Button", 256, 512+32, 256, 64 ) ) {
   OUTPUT("Test button D pressed.\n");
   selected=true;
  }

  text=fast.textbox("Enter your text:",text.value, 128, 800, 40 );

  glLineWidth(2);
  text=fast.textbox("Enter your text:",text.value, 128, 848, 40 );
  glLineWidth(1);
 }
};

