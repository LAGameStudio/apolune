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

#include "WorldShader.h"
#include "GLWindow.h"

/// Incomplete.
class WorldShaderTest : public GLWindow {
public:
 WorldShader *ws1,*ws2,*ws3,*ws4;
 VBO *vbo;
 void OnLoad() {
  ws1=new WorldShader;
  ws1->Load("data/materials/TileWork/TileWork.txt",0 );

  ws2=new WorldShader;
  ws2->Load("data/materials/TileWork/GlowingTiles.txt",0 );

  ws3=new WorldShader;
  ws3->Load("data/materials/TileWork/NoEnvMapTileWork.txt",0);

  ws4=new WorldShader;
  ws4->Load("data/materials/TileWork/WhiteLuminance.txt",0);
 }
 void Render() {
  Vertex offset;
 }
};