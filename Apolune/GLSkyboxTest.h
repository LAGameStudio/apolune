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
#include "TexFont.h"
#include "Skybox.h"

class GLSkyboxTest : public GLWindow
{
public:
// CubicSkyboxDefinition skybox,skybox2,skybox3;
 CubicSkyboxDefinitions skyboxes;
 void OnLoad() {
  background.Pick(black);
  Anchor(0.0f,0.0f,0.0f,0.0f);
  /*
  skyboxes.New( crayons.Any(), additive, 
   library.Load("data/images/skyboxes/blocks/bg1/bg1_front5.png"),
   library.Load("data/images/skyboxes/blocks/bg1/bg1_right1.png"),
   library.Load("data/images/skyboxes/blocks/bg1/bg1_back6.png"),
   library.Load("data/images/skyboxes/blocks/bg1/bg1_left2.png"),
   library.Load("data/images/skyboxes/blocks/bg1/bg1_top3.png"),
   library.Load("data/images/skyboxes/blocks/bg1/bg1_bottom4.png")
  );
  skyboxes.New( crayons.Any(), additive, 
   library.Load("data/images/skyboxes/blocks/neardust1/neardust1_front5.png"),
   library.Load("data/images/skyboxes/blocks/neardust1/neardust1_right1.png"),
   library.Load("data/images/skyboxes/blocks/neardust1/neardust1_back6.png"),
   library.Load("data/images/skyboxes/blocks/neardust1/neardust1_left2.png"),
   library.Load("data/images/skyboxes/blocks/neardust1/neardust1_top3.png"),
   library.Load("data/images/skyboxes/blocks/neardust1/neardust1_bottom4.png")
  );
  skyboxes.New( crayons.Any(), additive,
   library.Load("data/images/skyboxes/blocks/locals1/locals1_front5.png"),
   library.Load("data/images/skyboxes/blocks/locals1/locals1_right1.png"),
   library.Load("data/images/skyboxes/blocks/locals1/locals1_back6.png"),
   library.Load("data/images/skyboxes/blocks/locals1/locals1_left2.png"),
   library.Load("data/images/skyboxes/blocks/locals1/locals1_top3.png"),
   library.Load("data/images/skyboxes/blocks/locals1/locals1_bottom4.png")
  );*/
  skyboxes.factor=0.1f;
  skyboxes.Append( skyboxesFar.CubicSkybox( additive, crayons.Any() ) );
  int i=0, total=1+upto(5);
  for ( i=0; i<total; i++ ) 
   skyboxes.Append( skyboxesMid.CubicSkybox( additive, crayons.Any() ) );
  total=1+upto(5);
  for ( i=0; i<total; i++ ) 
   skyboxes.Append( skyboxesNear.CubicSkybox( additive, crayons.Any() ) );
//  skyboxes.NormalizeFactors();
  skyboxes.NormalizeRadii();
 }
 void Between() {
  float fx=0.0f,fy=0.0f;
  if ( input->KeyDown(DX_LEFT) )  { fx=-1.0f;  }
  if ( input->KeyDown(DX_RIGHT) ) { fx=1.0f;   }
  if ( input->KeyDown(DX_UP) )    { fy=1.0f;   }
  if ( input->KeyDown(DX_DOWN) )  { fy=-1.0f;  }
//  if ( fx != 0 || fy != 0 ) skyboxes.RotateByFactor(fx,fy);
  if ( fx != 0 || fy != 0 ) skyboxes.RotateBySame(fx,fy);
 }
 /*
 void OnLoad() {
  background.Pick(black);
  Anchor(0.0f,0.0f,0.0f,0.0f);
  skybox.Set( crayons.Pick(alabaster),
   additive,
   library.Load("data/images/skyboxes/blocks/bg1/bg1_front5.png"),
   library.Load("data/images/skyboxes/blocks/bg1/bg1_right1.png"),
   library.Load("data/images/skyboxes/blocks/bg1/bg1_back6.png"),
   library.Load("data/images/skyboxes/blocks/bg1/bg1_left2.png"),
   library.Load("data/images/skyboxes/blocks/bg1/bg1_top3.png"),
   library.Load("data/images/skyboxes/blocks/bg1/bg1_bottom4.png")
  );
  //skybox.blend=additive;
  skybox2.Set( crayons.Pick(alabaster),
   additive,
   library.Load("data/images/skyboxes/blocks/neardust1/neardust1_front5.png"),
   library.Load("data/images/skyboxes/blocks/neardust1/neardust1_right1.png"),
   library.Load("data/images/skyboxes/blocks/neardust1/neardust1_back6.png"),
   library.Load("data/images/skyboxes/blocks/neardust1/neardust1_left2.png"),
   library.Load("data/images/skyboxes/blocks/neardust1/neardust1_top3.png"),
   library.Load("data/images/skyboxes/blocks/neardust1/neardust1_bottom4.png")
  );
  //skybox2.blend=additive;
  skybox3.Set( crayons.Pick(alabaster),
   additive,
   library.Load("data/images/skyboxes/blocks/locals1/locals1_front5.png"),
   library.Load("data/images/skyboxes/blocks/locals1/locals1_right1.png"),
   library.Load("data/images/skyboxes/blocks/locals1/locals1_back6.png"),
   library.Load("data/images/skyboxes/blocks/locals1/locals1_left2.png"),
   library.Load("data/images/skyboxes/blocks/locals1/locals1_top3.png"),
   library.Load("data/images/skyboxes/blocks/locals1/locals1_bottom4.png")
  );
  //skybox3.blend=additive;
 }
 void Between() {
  //skybox.Between();
  if ( input->KeyDown(DX_LEFT) ) {
   skybox.rotation.fx+=0.3f;
   skybox2.rotation.fx+=0.2f;
   skybox3.rotation.fx+=0.1f;
  }
  if ( input->KeyDown(DX_RIGHT) ) {
   skybox.rotation.fx-=0.3f;
   skybox2.rotation.fx-=0.2f;
   skybox3.rotation.fx-=0.1f;
  }
  if ( input->KeyDown(DX_UP) ) {
   skybox.rotation.fy+=0.3f;
   skybox2.rotation.fy+=0.2f;
   skybox3.rotation.fy+=0.1f;
  }
  if ( input->KeyDown(DX_DOWN) ) {
   skybox.rotation.fy-=0.3f;
   skybox2.rotation.fy-=0.2f;
   skybox3.rotation.fy-=0.1f;
  }
 }
 */
 void Render() {
  skyboxes.Render();
  //skybox.Render();//0,0,0,100,100,100);
  //skybox2.Render();
  //skybox3.Render();
  char buf[32];
  FORMAT( buf,32, "Boxes: %d", (int) skyboxes.count );
  WriteTexFont(this,microFont,crayons.Pick(green),none,5,5,7,16,2,buf);
 }
};

