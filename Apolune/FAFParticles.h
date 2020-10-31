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
#include "TextureLibrary.h"
#include "fx_FireAndForget.h"
#include "Spark2d.h"

class FAFParticles : public FAFProce55or {
public:
 Sparksd2d sparks;
 void Setup() {
  x=0;
  y=0;
  w=display.w;
  h=display.h;
 }
 bool Done() {
  return ( sparks.count == 0 );
 }
 void Between() {
  sparks.Between();
 }
 void Render() {
  sparks.Render(parent);
 }
 void Render( GLWindow *surface ) {
  sparks.Render(surface);
 }
 void CreateSpark( int ex, int ey ) {
  Sparkd2d *spark=new Sparkd2d;
  spark->gravity.x=0.0;
  spark->gravity.y=9.6;
  spark->Create_MonocolorImage(
   5, 15, 0.5, 1.5, 0.1, 0.125, 0.25, 3.0, 0.1, 0.25, 50.0, 1, ex,ey,
   crayons.Pick(alabaster), crayons.Pick(orange), crayons.Pick(red255),
   library.Load("data/images/flares/flare.png")
  );
  sparks.Append(spark);
  spark->Init();
 }
 void CreateExplosion(int ex, int ey) {
  Sparkd2d *spark=new Sparkd2d;
  spark->gravity.x=0.0;
  spark->gravity.y=0.0;
  spark->Create_MonocolorImage(
   5, 50, 0.5, 1.5, 0.1, 0.25, 0.5, 1.0, 0.1, 0.25, 50.0, 1, ex,ey,
   crayons.Pick(alabaster), crayons.Pick(orange), crayons.Pick(red255),
   library.Load("data/images/flares/flare.png")
  );
  sparks.Append(spark);
  spark->Init();
 }
  void CreateSparkMed( int ex, int ey ) {
  Sparkd2d *spark=new Sparkd2d;
  spark->gravity.x=0.0;
  spark->gravity.y=9.6;
  spark->Create_MonocolorImage(
   5, 15, 0.5f, 1.5f, 0.01f, 0.0125f, 0.25f, 3.0f, 0.1f, 0.25f, 10.0f, 1, ex,ey,
   crayons.Pick(alabaster), crayons.Pick(orange), crayons.Pick(red255),
   library.Load("data/images/flares/flare.png")
  );
  sparks.Append(spark);
  spark->Init();
 }
 void CreateExplosionMed(int ex, int ey) {
  Sparkd2d *spark=new Sparkd2d;
  spark->gravity.x=0.0;
  spark->gravity.y=0.0;
  spark->Create_MonocolorImage(
   5, 50, 0.5, 1.5, 0.1, 0.25, 0.5, 1.0, 0.1, 0.25, 10.0, 1, ex,ey,
   crayons.Pick(alabaster), crayons.Pick(orange), crayons.Pick(red255),
   library.Load("data/images/flares/flare.png")
  );
  sparks.Append(spark);
  spark->Init();
 }
 void CreateSparkSmall( int ex, int ey ) {
  Sparkd2d *spark=new Sparkd2d;
  spark->gravity.x=0.0;
  spark->gravity.y=9.6;
  spark->Create_MonocolorImage(
   5, 15, 0.5, 1.5, 0.01, 0.0125, 0.25, 3.0, 0.1, 0.25, 10.0, 1, ex,ey,
   crayons.Pick(alabaster), crayons.Pick(orange), crayons.Pick(red255),
   library.Load("data/images/flares/flare.png")
  );
  sparks.Append(spark);
  spark->Init();
 }
 void CreateExplosionSmall(int ex, int ey) {
  Sparkd2d *spark=new Sparkd2d;
  spark->gravity.x=0.0;
  spark->gravity.y=0.0;
  spark->Create_MonocolorImage(
   5, 20, 0.5, 1.5, 0.1, 0.25, 0.5, 1.0, 0.1, 0.25, 5.0, 1, ex,ey,
   crayons.Pick(alabaster), crayons.Pick(orange), crayons.Pick(red255),
   library.Load("data/images/flares/flare.png")
  );
  sparks.Append(spark);
  spark->Init();
 }
 void CreateExplosionTiny(int ex, int ey) {
  Sparkd2d *spark=new Sparkd2d;
  spark->gravity.x=0.0;
  spark->gravity.y=0.0;
  spark->Create_MonocolorImage(
   5, 10, 0.5, 1.5, 0.1, 0.25, 0.5, 0.75, 0.1, 0.25, 2.0, 1, ex,ey,
   crayons.Pick(alabaster), crayons.Pick(orange), crayons.Pick(red255),
   library.Load("data/images/flares/flare.png")
  );
  sparks.Append(spark);
  spark->Init();
 }
};

