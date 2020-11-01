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
#include "ZeroTypes.h"
#include "GLWindow.h"
#include "TextureLibrary.h"

#include "Graphics.h"

class AssetLoader : public GLWindow {
public:
 Zp<GLImage> backing;
 Strings campaign_files;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
  backing=library.Load("data/sprites/screens/Background_AssetLoadingScreen.png");
  Strings campaigns;
  campaigns.Load("data/campaigns.txt");
 }
 void Between() {
 }
 void Render() {
  vg.Start();
  vg.Stretchi(backing,crayons.Pick(alabaster),1.0f,none,0,0,vg.w,vg.h);
  vg.End();
 }
};

