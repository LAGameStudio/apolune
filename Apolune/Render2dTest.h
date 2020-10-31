#pragma once
#include "GLWindow.h"
#include "Model.h"
#include "FBO.h"
#include "TextureLibrary.h"
#include "TexFont.h"

class Render2dTest : public GLWindow {
public:
 Zp<GLImage> tex;
 Zp<FBOColorDepthStencil> fbo;
 Zdouble spin,spun;
 ImmediateModel quad1,quad2;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);

  fbo=&fboCDS1024;
  tex=library.Load("data/images/icons/commodity.png");
  spin=input->mx;

  Vertex *v;
  // AB   BAC BCD
  // CD
  mPolygon *tri1=new mPolygon; 
  quad1.addPolygon(tri1);
  tri1->draw_method=GL_TRIANGLES;
  v=new Vertex; v->Set(128,0);   v->TexCoord(0,1); v->Color(crayons.Pick(hotPink));   tri1->addVertex(v); //B
  v=new Vertex; v->Set(0,0);     v->TexCoord(0,0); v->Color(crayons.Pick(red255));    tri1->addVertex(v); //A
  v=new Vertex; v->Set(0,128);   v->TexCoord(1,0); v->Color(crayons.Pick(green255));  tri1->addVertex(v); //C
  tri1->TriNormal();

  mPolygon *tri2=new mPolygon;
  quad1.addPolygon(tri2);
  tri2->draw_method=GL_TRIANGLES;
  v=new Vertex; v->Set(128,0);   v->TexCoord(0,1); v->Color(crayons.Pick(hotPink));   tri2->addVertex(v); //B
  v=new Vertex; v->Set(0,128);   v->TexCoord(1,0); v->Color(crayons.Pick(green255));  tri2->addVertex(v); //C
  v=new Vertex; v->Set(128,128); v->TexCoord(1,1); v->Color(crayons.Pick(blue255));   tri2->addVertex(v); //D
  tri2->TriNormal();

  quad2.Duplicate(&quad1);
  quad2.Rotate(0.0f,0.0f,45.0f);
  quad2.calculateNormals();
 }
 void Between() {
  spun+=spin;
  spin=input->mx;
 }
 void Render() {
  fbo->blend=none;
  fbo->background.Pick(darkSalmon);
  fbo->Bind();
  ///
  quad1.Render2dVNTC(tex,0,0,1,1);
  crayons.Pick(orange).gl();
  quad1.Render2dVC(200,200,1,1);
  crayons.Pick(alabaster).gl(); MultilineText("VC",200.0,200.0,8.0,10.0,2.0,true);
  quad1.Render2dVTC(tex,400,0,1,1);
  crayons.Pick(alabaster).gl(); MultilineText("VTC",400.0,0.0,8.0,10.0,2.0,true);
  quad1.Render2dVNTC(tex,700,500,2,2);
  crayons.Pick(alabaster).gl(); MultilineText("VNTC",0.0,500.0,8.0,10.0,2.0,true);
  glPush(false);
  glRotated(33.0,0.0,0.0,1.0);
//  glRotated(spun.value,1.0,0.0,0.0);
  quad1.Render2dVC(200,300,1,1);
  Rectangle(crayons.Pick(green255),200,300,328,428);
  crayons.Pick(alabaster).gl(); MultilineText("VC",200.0,300.0,8.0,10.0,2.0,true);
  quad1.Render2dVTC(tex,400,100,1,1);
  Rectangle(crayons.Pick(green255),400,100,528,228);
  crayons.Pick(alabaster).gl(); MultilineText("VTC",400.0,100.0,8.0,10.0,2.0,true);
  Text(nexaBold,crayons.Pick(hotPink),transparency,32,32,10,14,2,2,"Pop Art");
  RoundRect(crayons.Pick(blue255),crayons.Pick(red255),crayons.Pick(orange),150,150,25,25,8,3);
  glPop();
  RoundRect(crayons.Pick(green255),crayons.Pick(vermillion),
   crayons.Pick(orange),50,50,input->mxi,input->myi,32,8,2.0f);
  ///
  quad2.Render2dVNTC(tex,display->wd-128.0,display->hd-128.0,1.0,1.0);
  crayons.Pick(magenta).gl(); MultilineText("q2 VNTC",display->wd-128.0,display->hd-128.0,8.0,10.0,2.0,true);
  crayons.Pick(orange).gl();
  quad2.Render2dVC(200.0,600.0,1.0,0.5);
  crayons.Pick(magenta).gl(); MultilineText("q2 VC",200.0,600.0,8.0,10.0,2.0,true);
  quad2.Render2dVTC(tex,400,600,1,2);
  crayons.Pick(magenta).gl(); MultilineText("q2 VTC",400.0,600.0,8.0,10.0,2.0,true);
  quad2.Render2dVNTC(tex,600,500,2,1);
  crayons.Pick(magenta).gl(); MultilineText("q2 VNTC",600.0,500.0,8.0,10.0,2.0,true);
  Text(nexaBold,crayons.Pick(hotPink),transparency,32,32,10,14,2,2,"Pop Art");
  fbo->Unbind();
  fbo->RenderUpsideDown(32,32,display->w-64,display->h-64); //128,128,128+512,128+512);
 }
};

