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
/*******************************************************************************************************************************\
| 88888888888          88                  LOST ASTRONAUT'S                 * Lost Astronaut Game Development Framework         |
| 88                   ""                                                   * Copyright (c) 2007-2017 H. Elwood Gilliland III   |
| 88aaaaa  8b,dPPYba,  88  8b,dPPYba,    ,adPPYb,d8   ,adPPYba,  ,adPPYba,  ****************************************************|
| 88UUUUU  88P'   "Y8  88  88P'   `"8a  a8"    `Y88  a8P_____88  I8[    ""  * This software is copyrighted software.            |
| 88       88          88  88       88  8b       88  8PP"""""""   `"Y8ba,   * Use of this code is given only with permission    |
| 88       88          88  88       88  "8a,   ,d88  "8b,   ,aa  aa    ]8I  * to parties who have been granted such permission  |
| 88       88          88  88       88   `"YbbdP"Y8   `"Ybbd8"'  `"YbbdP"'  * by its author, Herbert Elwood Gilliland III.      |
|    O  F      T H E     E M P I R E     aa,    ,88                         * All rights reserved.                              |
|_______________________________________  "Y8bbdP"   __________________________________________________________________________*/
#pragma once

#include "TextureLibrary.h"
#include "Stats.h"
#include "GameObject.h"
#include "SoundEffect.h"
#include "SpaceShaders.h"

class Campaign;

enum PlanetProfileDesignation {
 ppd_Asteroid=0,
 ppd_Planetoid=1,
 ppd_GasJovian=2,
 ppd_SpongeJovian=3,
 ppd_Terrestrial=4,
 ppd_Bioplanet=5,
 ppd_Anomaly=6,
 ppd_Star=7
};

enum PlanetProfileRenderSizes {
 pprs_512=0,
 pprs_1024=1,
 //pprs_2048=2
};

enum PlanetProfileShaderOptions {
 ppso_RingedJovian=0,
 ppso_RingedJovian2=1,
 ppso_Atmospheric=2,
 ppso_Earthlike=3,
 ppso_Earthlike2=4,
 ppso_Cratered=5,
 ppso_Textured=6,
 ppso_ElectroReactive=7,
 ppso_Lunar=8,
 ppso_Stellar=9,
 ppso_Accretion=10
};

ONE(PlanetProfile,{
 size=pprs_512;
 shader=ppso_Earthlike;
 fbo=&fboCDS512;
 designation=ppd_Anomaly;
})
 PlanetProfileRenderSizes size;
 PlanetProfileShaderOptions shader;
 RingedJovianProfile ringedJovian;
 RingedJovian2Profile ringedJovian2;
 AtmosphericPlanetProfile atmospheric;
 EarthlikeProfile earthlike;
 Earthlike2Profile earthlike2;
 ToonTroidProfile cratered;
 TexturedPlanetProfile textured;
 ElectroPlanetProfile electro;
 LunarPlanetProfile lunar;
 FlaringStarProfile stellar;
 AccretionProfile accretion;
 Zp<GLImage> background;
 Zdouble bgScale;
 Zp<GLImage> overlay;
 Zdouble ovScale;
 Zbool stellarScale;  // means the ship cannot approach without damage.
 Zp<Campaign> campaign; // Source campaign for keeping items relative
 ///
 Zp<FBOColor> fbo;
 Zdouble time;
 PlanetProfileDesignation designation;
 KEYWORDS({
  TXTWORD("name",name)
  else KEYWORD("size",{size=(PlanetProfileRenderSizes)atoi(w);})
  else KEYWORD("shader",{shader=(PlanetProfileShaderOptions)atoi(w);})
  else KEYWORD("d",{designation=(PlanetProfileDesignation)atoi(w);})
  else SSUBWORD("rj",ringedJovian)
  else SSUBWORD("rj2",ringedJovian2)
  else SSUBWORD("atmos",atmospheric)
  else SSUBWORD("el",earthlike)
  else SSUBWORD("el2",earthlike2)
  else SSUBWORD("cr",cratered)
  else SSUBWORD("te",textured)
  else SSUBWORD("ele",electro)
  else SSUBWORD("lu",lunar)
  else SSUBWORD("st",stellar)
  else SSUBWORD("ac",accretion)
  else IMGWORD("bg",background)
  else IMGWORD("ov",overlay)
  else KEYWORD("bgscale",{bgScale=w;})
  else KEYWORD("ovscale",{ovScale=w;})
  else TAGWORD("stellarScale",stellarScale)
 })
 WORDKEYS({
  TXTKEY("name",name)
  ENUMKEY("size",size)
  ENUMKEY("shader",shader)
  ENUMKEY("d",designation)
  SSUBKEY("rj",ringedJovian)
  SSUBKEY("rj2",ringedJovian2)
  SSUBKEY("atmos",atmospheric)
  SSUBKEY("el",earthlike)
  SSUBKEY("el2",earthlike2)
  SSUBKEY("cr",cratered)
  SSUBKEY("te",textured)
  SSUBKEY("ele",electro)
  SSUBKEY("lu",lunar)
  SSUBKEY("st",stellar)
  SSUBKEY("ac",accretion)
  IMGKEY("bg",background)
  IMGKEY("ov",overlay)
  NUMBKEY("bgscale",bgScale)
  NUMBKEY("ovscale",ovScale)
  TAGKEY("stellarScale",stellarScale)
 })
 BINARY_IN({
  B_IN_STRING(name)
  B_IN_ENUM(PlanetProfileRenderSizes,size)
  B_IN_ENUM(PlanetProfileShaderOptions,shader)
  B_IN_ENUM(PlanetProfileDesignation,designation)
  B_IN_SSUB(ringedJovian)
  B_IN_SSUB(ringedJovian2)
  B_IN_SSUB(atmospheric)
  B_IN_SSUB(earthlike)
  B_IN_SSUB(earthlike2)
  B_IN_SSUB(cratered)
  B_IN_SSUB(textured)
  B_IN_SSUB(electro)
  B_IN_SSUB(lunar)
  B_IN_SSUB(stellar)
  B_IN_SSUB(accretion)
  B_IN_IMAGE(background)
  B_IN_IMAGE(overlay)
  B_IN_NUM(bgScale)
  B_IN_NUM(ovScale)
 })
 BINARY_OUT({
  B_OUT_STRING(name)
  B_OUT_ENUM(PlanetProfileRenderSizes,size)
  B_OUT_ENUM(PlanetProfileShaderOptions,shader)
  B_OUT_ENUM(PlanetProfileDesignation,designation)
  B_OUT_SSUB(ringedJovian)
  B_OUT_SSUB(ringedJovian2)
  B_OUT_SSUB(atmospheric)
  B_OUT_SSUB(earthlike)
  B_OUT_SSUB(earthlike2)
  B_OUT_SSUB(cratered)
  B_OUT_SSUB(textured)
  B_OUT_SSUB(electro)
  B_OUT_SSUB(lunar)
  B_OUT_SSUB(stellar)
  B_OUT_SSUB(accretion)
  B_OUT_IMAGE(background)
  B_OUT_IMAGE(overlay)
  B_OUT_NUM(bgScale)
  B_OUT_NUM(ovScale)
 })
 void SetTime( double t ) {
  time=t;
  ringedJovian.time=time;
  ringedJovian2.time=time;
  atmospheric.time=time;
  earthlike.time=time;
  earthlike2.time=time;
  cratered.time=time;
  textured.position.x=time;
  electro.time=time;
  lunar.time=time;
  stellar.time=time;
  accretion.time=time;
 }
 void Between() {
  time+=FRAMETIME;
  SetTime(time);
 }
 int GetSize() {
  switch ( size ) {
    case pprs_512: return 512;
   case pprs_1024: return 1024;
          default: return 1024;
  }
 }
 void Render() {
  switch ( size ) {
    case pprs_512: fbo=&fboCDS512; break;
   case pprs_1024: fbo=&fboCDS1024; break;
 //  case pprs_2048: fbo=&fboCDS2048; break; 
          default: break;
  }
  fbo->background.Pick(clear);
  fbo->blend=transparency;
  switch ( shader ) {
      case ppso_RingedJovian: ShadedRingedJovian(fbo,&ringedJovian); break;
     case ppso_RingedJovian2: ShadedRingedJovian2(fbo,&ringedJovian2); break;
       case ppso_Atmospheric: ShadedAtmosphericPlanet(fbo,&atmospheric); break;
         case ppso_Earthlike: ShadedEarthlike(fbo,&earthlike); break;
        case ppso_Earthlike2: ShadedEarthlike2(fbo,&earthlike2); break;
          case ppso_Cratered: ShadedToonTroid(fbo,&cratered); break;
          case ppso_Textured: ShadedTexturedPlanet(fbo,&textured); break;
   case ppso_ElectroReactive: ShadedElectroPlanet(fbo,&electro); break;
             case ppso_Lunar: ShadedLunarPlanet(fbo,&lunar); break;
           case ppso_Stellar: stellar.rotation.x=time/100.0; ShadedFlaringStar(fbo,&stellar); break;
         case ppso_Accretion: ShadedAccretion(fbo,&accretion); break;
   default: break;
  }
 }
MANY(PlanetProfile,PlanetProfileHandle,PlanetProfileHandles,"PlanetProfile",PlanetProfiles,{})
 KEYWORDSGroup("PlanetProfile",PlanetProfile);
 WORDKEYSGroup("PlanetProfile",PlanetProfile);
 void BinaryReadIterative( BinaryFile *open, int *remaining ) {
  OUTPUT("b_in[%s][<--",(typeid(*this).name()) ); 
  {
   if ( (*remaining) > 0 ) {
    PlanetProfile *pp=new PlanetProfile;
    Append(pp);
    pp->BinaryRead(open);
    (*remaining)--;
    return;
   }
  }
  OUTPUT("-->]\n"); 
 }
 BINARY_INOUT(PlanetProfile)
 void AssignUnassigned( Campaign *c );
 void Query( Campaign *c, PlanetProfileHandles *out );
 static void DeployTypesSizes( Strings *s ) {  
  s->Add("512",0);
  s->Add("1024",1);
  //s->Add("2048",2);
 }
 static void DeployTypesShaders( Strings *s ) {  
  s->Add("Ringed Jovian",0);
  s->Add("Jovian Variant",1);
  s->Add("Atmospheric Terrestrial",2);
  s->Add("Earthlike",3);
  s->Add("Terrestrial",4);
  s->Add("Cratered",5);
  s->Add("Textured",6);
  s->Add("Electro-Reactive",7);
  s->Add("Lunar",8);
  s->Add("Stellar",9);
  s->Add("Accretion",10);
 }
 static void DeployTypesDesignation( Strings *s ) {  
  s->Add("Asteroid",0);       // ppd_Asteroid=0,
  s->Add("Planetoid",1);      // ppd_Planetoid=1,
  s->Add("Gas Giant",2);      // ppd_GasJovian=2,
  s->Add("Sponge Jovian",3);  // ppd_SpongeJovian=3,
  s->Add("Terrestrial",4);    // ppd_Terrestrial=4,
  s->Add("Bioplanet",5);      // ppd_Bioplanet=5,
  s->Add("Anomaly",6);        // ppd_Anomaly=6,
  s->Add("Star",7);           // ppd_Star=7
 }
DONE(PlanetProfile);

extern PlanetProfiles planetProfiles;