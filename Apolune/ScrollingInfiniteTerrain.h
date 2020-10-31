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
#include "InfiniteTerrain.h"
#include "Globals.h"

enum MovementDirections {
 move_right=1,
 move_left=2,
 move_forward=4,
 move_back=8,
 move_up=16,
 move_down=32
};

class ScrollingInfiniteTerrainMesh : public ListItem {
public:
 Zpointer<InfiniteTerrain> terrain;
 Zpointer<GLImage> ground;
 ImmediateLighting lights;
 Zbool threading;
 ScrollingInfiniteTerrainMesh() : ListItem() {
  threading=false;
  terrain = new InfiniteTerrain( threading, 2048.0f, 128, 2, 32 );
  ground=library.Load("data/images/textures/tiny/gold.png",tllto_Tiling);
  lights.light[7].position.Set( -1.0f, 0.5f, 0.2f, 0.0f );
  lights.light[7].on=true;
 }
 void Render( SimpleHCamera *hcamera ) {
  lights.On();
//  glDisable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,ground->texture);
  terrain->Render( &hcamera->position ); 
  glDisable(GL_TEXTURE_2D);
  lights.Off();
 }
 void Reset() {
  terrain.Delete();
  terrain=new InfiniteTerrain( threading, 2048.0f, 128, 2, 32 );
 }
 ~ScrollingInfiniteTerrainMesh() {
  terrain.Delete();
 }
};

class ScrollingInfiniteTerrainMeshes : public LinkedList {
public:
 ScrollingInfiniteTerrainMesh *New() {
  ScrollingInfiniteTerrainMesh *result=new ScrollingInfiniteTerrainMesh;
  Append(result);
  return result;
 }
 void Render( SimpleHCamera *hcamera ) {
  FOREACH(ScrollingInfiniteTerrainMesh,m) m->Render(hcamera);
 }
 CLEARLISTRESET(ScrollingInfiniteTerrainMesh);
};

class ScrollingInfiniteTerrain {
public:
 SimpleHCamera hcamera;
 ScrollingInfiniteTerrainMeshes meshes;
 Zpointer<FBOColorDepth> fbo;
 Zpointer<ScrollingInfiniteTerrainMesh> mesh;
 Crayon fog;
 Zbool foggy;
 ScrollingInfiniteTerrain() {
  fbo=&fboScreen2;
  foggy=true;
  fog.Int(0, 0, 0, 0);
  hcamera.position.x=-1000000.0f;
  hcamera.position.y=-300.0f;
  hcamera.position.z=0.0;//1048.0f;
  hcamera.Wx=-PIf/16.0f;
  hcamera.Wy=PIf;
 }
 void Advance( double factor, MovementDirections movement ) {
  float sqrLen;
  HVertex dir(0.0,0.0,0.0);
  HVertex dx(hcamera.modelView[0], hcamera.modelView[4], hcamera.modelView[8]);
  HVertex dy(hcamera.modelView[1], hcamera.modelView[5], hcamera.modelView[9]);
  HVertex dz(hcamera.modelView[2], hcamera.modelView[6], hcamera.modelView[10]);
  if ( IS_SET(movement,move_up) ) dir -= dy;
  if ( IS_SET(movement,move_down) ) dir += dy;
  if ( IS_SET(movement,move_left) ) dir -= dx;
  if ( IS_SET(movement,move_right) ) dir += dx;
  if ( IS_SET(movement,move_forward) ) dir -= dz;
  if ( IS_SET(movement,move_back) ) dir += dz;
  if ((sqrLen = lengthSqr(dir)) != 0){
  	dir *= rsqrtf(sqrLen);
  }
  hcamera.processMovement(hcamera.position + FRAMETIME * hcamera.speed * factor * dir);
 }
 void RenderComposite() {
  fbo->background.Pick(clear);
  fbo->Bind();
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(hcamera.projection);
  hcamera.modelView.loadRotateZXY(-(float)hcamera.Wx, -(float)hcamera.Wy, -(float)hcamera.Wz);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(hcamera.modelView);
  glClearColor(0.0,0.0,0.0,0.0);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
 	glDepthFunc(GL_LEQUAL);
  glDisable(GL_DEPTH_TEST);
  hcamera.modelView.translate(-hcamera.position);
  glLoadMatrixf(hcamera.modelView);
  // <- Draw Skybox...
  Blending(transparency);
  glEnable(GL_DEPTH_TEST);  
  if ( foggy ) {
   glFogfv(GL_FOG_COLOR, fog.floats);
   glFogi(GL_FOG_MODE, GL_EXP2);
   glFogf(GL_FOG_DENSITY, 0.0005f);
//   glFogf(GL_FOG_START, 1);
    //glFogf(GL_FOG_END, viewDistance);
   glEnable(GL_FOG);
  }
  Blending(transparency);
  meshes.Render(&hcamera);
//  glEnable(GL_CULL_FACE);
  fbo->Unbind();
 }
 void Render(int x, int y, int x2, int y2) {
  RenderComposite();
  fbo->Render(transparency,x,y,x2,y2);
 }
 void Debug() {
  FORMAT(buf,64,"Pos: %1.2f  %1.2f  %1.2f\nRot: %1.2f  %1.2f  %1.2f",
   (float) hcamera.position.x,
   (float) hcamera.position.y,
   (float) hcamera.position.z,
   (float) hcamera.Wx,
   (float) hcamera.Wy,
   (float) hcamera.Wz
  );
  glColor3d(0.0,0.0,0.0);
  Text(buf,10,20,8,8,2,1.0);
  glColor3d(1.0,1.0,0.0);
  Text(buf,10+1,20-1,8,8,2,1.0);
 }
 void GenerateJovian( int seed ) {
 }
 void Generate( int seed ) {
  int totalMeshes=1+upto(5,seed);
  if ( totalMeshes < 2 && seed%2==1 ) totalMeshes*=2;
  fog.random(seed);
  Random *set=randoms.GetSet(seed);
  for ( int i=0; i<totalMeshes; i++ ) {
   int phone=set->arnd(i+seed);
   set=randoms.GetSet(phone);
   mesh=meshes.New();
   mesh->ground=planet_terrain_textures.Any(phone);
   int dialup=upto(44,seed+i+phone);
   phone+=dialup;
   float f=set->aunique(dialup,phone,seed+i);
   if ( f < 0.1f ) {
    OUTPUT("ITE_TweenABCD added.\n");
    ITE_TweenABCD *eq=new ITE_TweenABCD(tweens.seeded(seed+i),tweens.seeded(phone),tweens.seeded(i+phone),tweens.seeded(seed));
    eq->factor=set->range(set->arnd(seed+i,phone),0.1,0.9);
    eq->scale=set->range(set->arnd(phone,seed),16.0,512.0);
    mesh->terrain->generator.Append(eq);
   } else if ( f < 0.15f ) {
    OUTPUT("ITE_TweenX added.\n");
    ITE_TweenX *eq=new ITE_TweenX(tweens.seeded(phone));
    eq->scale=set->range(set->arnd(phone,seed),16.0,512.0);
    mesh->terrain->generator.Append(eq);
   } else if ( f < 0.2f ) {
    OUTPUT("ITE_TweenZ added.\n");
    ITE_TweenZ *eq=new ITE_TweenZ(tweens.seeded(phone));
    eq->scale=set->range(set->arnd(phone,seed),16.0,512.0);
    mesh->terrain->generator.Append(eq);
   } else if ( f < 0.25f ) {
    OUTPUT("ITE_TweenXpZ added.\n");
    ITE_TweenXpZ *eq=new ITE_TweenXpZ(tweens.seeded(phone));
    eq->scale=set->range(set->arnd(phone,seed),16.0,512.0);
    eq->xScale=set->range(set->arnd(i,phone),0.1,2.0);
    eq->zScale=set->range(set->arnd(i,phone),0.1,2.0);
    mesh->terrain->generator.Append(eq);
   } else if ( f < 0.3f ) {
    OUTPUT("ITE_AbsSinCos added.\n");
    ITE_AbsSinCos *eq=new ITE_AbsSinCos;
    eq->scale=set->range(set->arnd(phone,seed),8.0,512.0);
    mesh->terrain->generator.Append(eq);
   } else if ( f < 0.35f ) {
    OUTPUT("ITE_AbsSinCosScaled added.\n");
    ITE_AbsSinCosScaled *eq=new ITE_AbsSinCosScaled;
    eq->scale=set->range(set->arnd(phone,seed),8.0,512.0);
    mesh->terrain->generator.Append(eq);
   } else if ( f < 0.4f ) {
    OUTPUT("ITE_Noise2 added.\n");
    ITE_Noise2 *eq=new ITE_Noise2;
    eq->scale=set->range(set->arnd(phone,seed),8.0,512.0);
    if ( set->aunique(seed,phone,i) > 0.25f ) {
     int depth=phone%10000; if ( depth == 0 ) depth=1;
     eq->a=(double)set->aunique(i,phone,seed)/(double)depth;
    }
    if ( set->aunique(i,phone,seed) > 0.25f ) {
     int depth=abs(seed-phone)%10000; if ( depth == 0 ) depth=1;
     eq->b=(double)set->aunique(i,phone,seed)/(double)depth;
    }
    if ( set->aunique(i,phone,i) > 0.25f ) {
     int depth=abs(seed-phone*i)%10000; if ( depth == 0 ) depth=1;
     eq->c=(double)set->aunique(i,phone,seed)/(double)depth;
    }
    if ( set->aunique(seed,i,phone) > 0.25f ) {
     int depth=(seed+phone*i)%10000; if ( depth == 0 ) depth=1;
     eq->d=(double)set->aunique(i,phone,seed)/(double)depth;
    }
    mesh->terrain->generator.Append(eq);
   } else if ( f < 0.5f ) {
    OUTPUT("ITE_AbsRandomSetRubix added.\n");
    ITE_AbsRandomSetRubix *eq=new ITE_AbsRandomSetRubix(phone+seed+i);
    eq->scale=set->range(set->arnd(phone,seed),8.0,512.0);
    mesh->terrain->generator.Append(eq);
   } else if ( f < 0.55f ) {
    OUTPUT("ITE_AbsRandomSetRand added.\n");
    ITE_AbsRandomSetRand *eq=new ITE_AbsRandomSetRand(i+phone+seed);
    eq->scale=set->range(set->arnd(phone,seed),8.0,512.0);
    mesh->terrain->generator.Append(eq);
   } else if ( f < 0.95f ) {
    OUTPUT("ITE_HeightBrush added.\n");
    ITE_HeightBrush *eq=new ITE_HeightBrush((HeightBrush *) heightbrushes.Any(seed+phone+i));
    mesh->terrain->generator.Append(eq);
   } else if ( f < 1.0f ) {
    OUTPUT("ITE_Flat added.\n");
    ITE_Flat *eq=new ITE_Flat(set->range(phone+seed+i,0.0,32.0));
    mesh->terrain->generator.Append(eq);
   }
  }
  OUTPUT("ScrollingInfiniteTerrain:Generate() generated %d\n", (int) mesh->terrain->generator.count );
 }
 Zfloat keyDelay;
 void DebugKeys() {
  if ( !mesh ) mesh=meshes.New();
  if ( keyDelay > 0.0f ) { keyDelay-=FRAMETIME; return; }
  if ( input.KeyDown(DIK_N) ) {
   mesh=meshes.New();
   keyDelay=0.25f;
  }
  if ( input.KeyDown(DX_BACKSPACE) ) meshes.Clear();
  if ( input.KeyDown(DX_RETURN) ) {
   meshes.Clear();
   Generate(number_range(0,100000));
   keyDelay=0.25f;
  }
  if ( input.KeyDown(DIK_8) ) {
   if ( !input.KeyDown(DX_LSHIFT) ) mesh->Reset();
   mesh->terrain->generator.Append(new ITE_TweenABCD(tweens.random(),tweens.random(),tweens.random(),tweens.random()));
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DIK_9) ) {
   if ( !input.KeyDown(DX_LSHIFT) ) mesh->Reset();
   mesh->terrain->generator.Append(new ITE_TweenX(tweens.random()));
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DIK_0) ) {
   if ( !input.KeyDown(DX_LSHIFT) ) mesh->Reset();
   mesh->terrain->generator.Append(new ITE_TweenZ(tweens.random()));
   keyDelay=0.25f;
  }
  if ( input.KeyDown(DX_1) ) {
   if ( !input.KeyDown(DX_LSHIFT) ) mesh->Reset();
   mesh->terrain->generator.Append(new ITE_TweenXZ(tweens.random(),tweens.random()));
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DX_2) ) {
   if ( !input.KeyDown(DX_LSHIFT) ) mesh->Reset();
   mesh->terrain->generator.Append(new ITE_AbsSinCos);
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DX_3) ) {
   if ( !input.KeyDown(DX_LSHIFT) ) mesh->Reset();
   mesh->terrain->generator.Append(new ITE_AbsSinCosScaled);
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DX_4) ) {
   if ( !input.KeyDown(DX_LSHIFT) ) mesh->Reset();
   mesh->terrain->generator.Append(new ITE_Noise2);
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DX_5) ) {
   if ( !input.KeyDown(DX_LSHIFT) ) mesh->Reset();
   mesh->terrain->generator.Append(new ITE_AbsRandomsUnique);
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DX_6) ) {
   if ( !input.KeyDown(DX_LSHIFT) ) mesh->Reset();
   mesh->terrain->generator.Append(new ITE_AbsRandomSetRubix(number_range(0,12345)));
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DX_7) ) {
   if ( !input.KeyDown(DX_LSHIFT) ) mesh->Reset();
   mesh->terrain->generator.Append(new ITE_AbsRandomSetRand(number_range(0,12345)));
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DX_K) ) {
   if ( !input.KeyDown(DX_LSHIFT) ) mesh->Reset();
   mesh->terrain->generator.Append(new ITE_HeightBrush((HeightBrush *) heightbrushes.Any()));
   keyDelay=0.25f;
  }
  if ( input.KeyDown(DIK_A) ) {
   mesh->ground=library.Load("data/images/textures/tiny/grid1.png");
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DIK_B) ) {
   mesh->ground=library.Load("data/images/textures/tiny/wrap2.png");
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DIK_C) ) {
   mesh->ground=library.Load("data/images/textures/tiny/foil1.png");
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DIK_D) ) {
   mesh->ground=library.Load("data/images/textures/wrap1024/magma.png");
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DIK_E) ) {
   mesh->ground=library.Load("data/images/textures/wrap1024/neural.png");
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DIK_F) ) {
   mesh->ground=library.Load("data/images/textures/wrap1024/ground.png");
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DIK_G) ) {
   mesh->ground=library.Load("data/images/textures/256/bubbles.png");
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DIK_H) ) {
   mesh->ground=library.Load("data/images/textures/wrap1024/comb.png");
   keyDelay=0.25f;
  } else
  if ( input.KeyDown(DIK_I) ) {
   mesh->ground=library.Load("data/images/textures/odd/orangeish.png");
   keyDelay=0.25f;
  }
 }
};

