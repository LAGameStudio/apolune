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

