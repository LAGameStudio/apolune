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
#include "LightManager2d.h"
#include "GLWindow.h"

class LightManager2dTest : public GLWindow {
public:
 LightManager2d lightmap;
 Light2d *mouse;
 GLImage *test;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(red255);
  lightmap.ambient.Float(0.0f,0.0f,0.0f,1.0f);
  lightmap.walls.Add(Vertexd(7.0,30.0),Vertexd(57.0,55.0));
  lightmap.walls.Add(Vertexd(150.0,30.0),Vertexd(127.0,255.0));
  lightmap.walls.Add(Vertexd(520.0,30.0),Vertexd(1127.0,255.0));
  lightmap.walls.Add(Vertexd(222,30.0),Vertexd(333,255.0));
  lightmap.walls.Add(Vertexd(566,30.0),Vertexd(1227.0,255.0));
  lightmap.walls.Add(Vertexd(160.0,530.0),Vertexd(1900.0,455.0));
  lightmap.walls.Add(Vertexd(520.0,630.0),Vertexd(1127.0,855.0));
  lightmap.walls.Add(Vertexd(100.0,630.0),Vertexd(100.0,100.0));
  lightmap.walls.Add(Vertexd(200.0,630.0),Vertexd(200.0,200.0));
  lightmap.walls.Add(Vertexd(300.0,630.0),Vertexd(300.0,300.0));
  lightmap.walls.Add(Vertexd(400.0,630.0),Vertexd(450.0,630.0));
  lightmap.walls.Add(Vertexd(500.0,630.0),Vertexd(505.0,630.0));
  lightmap.walls.Add(Vertexd(200.0,630.0),Vertexd(215.0,630.0));
  lightmap.walls.Add(Vertexd(220.0,630.0),Vertexd(230.0,630.0));
  lightmap.walls.Add(Vertexd(240.0,630.0),Vertexd(250.0,630.0));
  lightmap.walls.Add(Vertexd(260.0,630.0),Vertexd(270.0,630.0));
  lightmap.walls.Add(Vertexd(280.0,630.0),Vertexd(290.0,630.0));
  lightmap.walls.Circle(800.0,600.0,20.0,16);
  lightmap.walls.Rectangle(1000.0,600.0,40.0,40.0);
  mouse=lightmap.Add(Vertexd(10.0,40.0),1.0,420.0,64.0,crayons.Pick(orangeRed));
  mouse->add=true;
  mouse->duration=0.0;
  mouse->SetColor(mouse->color,1.0);
  mouse->flare=library.Load("data/images/flares/neoflare20.png");
  lightmap.Add(Vertexd(800.0,800.0),1.0,10000.0,64.0,crayons.Pick(blue));
  lightmap.Add(Vertexd(1700.0,800.0),1.0,10.0,64.0,crayons.Pick(red255));
  lightmap.Add(Vertexd(-10.0,40.0),1.0,420.0,64.0,crayons.Pick(blue));
//  lightmap.Shaded(Vertexd(1022.0,40.0),1.0,820.0,64.0,crayons.Pick(green));
//  lightmap.Shaded(Vertexd(410.0,420.0),1.0,420.0,64.0,crayons.Pick(yellow));
  lightmap.Add(Vertexd(610.0,420.0),0.5,420.0,64.0,crayons.Pick(yellow));
  lightmap.Spot(Vertexd(250.0,600.0),1.0,600.0,32.0,crayons.Pick(alabaster),75.0,200.0);
  lightmap.Spot(Vertexd(450.0,600.0),1.0,600.0,32.0,crayons.Pick(alabaster),75.0,200.0);
  lightmap.Spot(Vertexd(450.0,300.0),1.0,600.0,32.0,crayons.Pick(alabaster),75.0,200.0);
  lightmap.Spot(Vertexd(850.0,300.0),1.0,600.0,32.0,crayons.Pick(alabaster),75.0,200.0,20.0);
  test=library.Load("data/images/textures/1024/1024copper.png");//encyclopedia/Nasa_space_elev.jpg");//NASA_70s_colony_3lrg.jpg"); //rounder_minigrid.png");
 }
 void Between() {
  int mx,my;
  MousePosition(&mx,&my);
  mouse->MoveTo((double)mx,(double)(my));
  lightmap.Between();
 }
 void Render() {
  QuadiNoAlpha(test,crayons.Pick(alabaster),none,0,0,w,h);
  lightmap.Render();
 }
 void Debug() {
  lightmap.walls.Debug();
  lightmap.shadowless.Debug();
  lightmap.shadedLights.Debug();
  lightmap.casters.Debug();
  lightmap.statics.Debug();
 }
};
