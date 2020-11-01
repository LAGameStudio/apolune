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
#include "ATFont.h"


class ATFontTest : public GLWindow {
public:
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
 }
 void Between() {
 }
 void Render() {
  int ty=16;
  abytes->Write("This is a test, this is only a test. $12.123",crayons.Pick(macNcheese),16,ty);
  ty+=(int)abytes->size.h;
  acalibri->Write("This is a test, this is only a test. $120.13",crayons.Pick(black),16,ty);
  acalibri->Write("This is a test, this is only a test. $120.13",crayons.Pick(gray),16+1,ty-1);
  acalibri->Write("This is a test, this is only a test. $120.13",crayons.Pick(alabaster),16+2,ty-2);
  ty+=(int)acalibri->size.h;
  adigicaps->Write("This is a test, this is only a test. $120.12",crayons.Pick(red255),16,ty);
  ty+=(int)adigicaps->size.h;
  ametalFont->Write("This is a test, this is only a test. $120.12",crayons.Pick(gray),16,ty);
  ty+=(int)ametalFont->size.h;
  amicroFont->Write("This is a test, this is only a test. 120.1234567980",crayons.Pick(green255),16,ty);
  ty+=(int)amicroFont->size.h;
  atermFont->Write("This is a test, this is only a test. 120.123",crayons.Pick(alabaster),16,ty);
  ty+=(int)atermFont->size.h;
  aglowTermFont->Write("This is a test, this is only a test.\nYesh, Fresh, into things...! 120.123",crayons.Pick(alabaster),16,ty);
 }
};

