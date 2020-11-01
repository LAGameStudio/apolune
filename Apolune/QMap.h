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

#include "Strings.h"
#include "TextureLibrary.h"
#include "Cartesian.h"
#include "SoundEffect.h"
#include "Film.h"

class Campaign;

// Basically a specialized dialogue that lets the user place things to click on (map markers)
// on a background image.

ONE(QMapMarker,{
 b=transparency;tint.Pick(alabaster);
 filmDuration=1.0;
 xywh.SetRect(1920/2-1920/4,1080/2-1080/4,1920/2,1080/2);
 tint.Pick(alabaster);
})
 Zstring caption;
 Zstring description;
 Zstring script;  // executed when visited, should include a "Go" clause
 Zstring requires; // tag cloud
 Zstring markCompleted; // quest indicator that marks completion, barring users from returning.
 Zstring human_seed;
 Zstring human_mood;
 Zstring lifeform;
 Cartesian xywh;
 SoundEffect hover;
 Zp<GLImage> image;
 Zp<Film> film;
 Zdouble filmDuration;
 Crayon tint;
 Blends b;
 Zbool showAsButton,captionIsButton,buttonOnHover,enlargeOnHover;
 KEYWORDS({
  TXTWORD("name",name)
  else TXTWORD("caption",caption)
  else TXTWORD("desc",description)
  else TXTWORD("requires",requires)
  else TXTWORD("completed",markCompleted)
  else TXTWORD("script",script)
  else TXTWORD("lifeform",lifeform)
  else TXTWORD("human",human_seed)
  else TXTWORD("mood",human_mood)
  else IMGWORD("image",image)
  else FILMWORD("film",film)
  else NUMWORD("filmDuration",filmDuration)
  else TINTWORD("tint",tint)
  else BLENDWORD("blend",b)
  else SUBWORD("xywh",xywh)
  else TAGWORD("button",showAsButton)
  else TAGWORD("capbutton",captionIsButton)
  else TAGWORD("hovbutton",buttonOnHover)
  else TAGWORD("enlarge",enlargeOnHover)
 })
 WORDKEYS({
  TXTKEY("name",name)
  TXTKEY("caption",caption)
  TXTKEY("desc",description)
  TXTKEY("requires",requires)
  TXTKEY("completed",markCompleted)
  TXTKEY("script",script)
  TXTKEY("lifeform",lifeform)
  TXTKEY("human",human_seed)
  TXTKEY("mood",human_mood)
  IMGKEY("image",image)
  FILMKEY("film",film)
  NUMBKEY("filmDuration",filmDuration)
  TINTKEY("tint",tint)
  BLENDKEY("blend",b)
  SSUBKEY("xywh",xywh)
  TAGKEY("button",showAsButton)
  TAGKEY("capbutton",captionIsButton)
  TAGKEY("hovbutton",buttonOnHover)
  TAGKEY("enlarge",enlargeOnHover)
 })
MANY(QMapMarker,QMapMarkerHandle,QMapMarkerHandles,"V",QMapMarkers,{})
 KEYSWORDSGroups("V",QMapMarker)
DONE(QMapMarker)

ONE(QMap,{
 bgTint.Pick(alabaster);
})
 Zp<Campaign> campaign;
 Zstring area_name,description;
 QMapMarkers markers;
 Zstring script;  // Executed at start
 Zstring presentation; // Background presentation
 Zp<GLImage> background;
 Crayon bgTint;
 KEYWORDS({
  TXTWORD("name",name)
  TXTWORD("area",area_name)
  TXTWORD("desc",description)
  TXTWORD("presi",presentation)
  TXTWORD("script",script)
  SUBWORD("markers",markers)
  IMGWORD("bg",background)
  TINTWORD("bgTint",bgTint)
 })
 WORDKEYS({
  TXTKEY("name",name)
  TXTKEY("area",area_name)
  TXTKEY("desc",description)
  TXTKEY("presi",presentation)
  TXTKEY("script",script)
  SSUBKEY("markers",markers)
  IMGKEY("bg",background)
  TINTKEY("bgTint",bgTint)
 })
MANY(QMap,QMapHandle,QMapHandles,"QMap",QMaps,{})
 KEYSWORDSGroups("QMap",QMap)
 void AssignUnassigned( Campaign *c );
 void Query( Campaign *c, QMapHandles *out );
DONE(QMap)

extern QMaps qmaps;