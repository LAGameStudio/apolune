#pragma once

#include "Art2d.h"
#include "GLWindow.h"
#include "FastGUI.h"

class TwoDeeElementEditor : public GLWindow {
public:
 Crayon tint;
 Cartesiand A,B,C,D;
 Cartesiands points;
 Cartesiand paper;
 Zp<TwoDeeStroke> stroke;
 Zp<TwoDeeStrokeCollection> collection;
 TwoDeeStrokeCollections collections;
 Zint stashed_grid,stashed_grid20,stashed_grid_square;
 Zint scrolled,selected;
 Zp<GLImage> plusIcon,checkIcon,discIcon;
 Strings typeOptions;
 void OnLoad() {
  Fullscreen();
  background.Pick(black);
  paper.SetRect(16,16,768,768);
  fast.ResponsiveRatio(display->w,display->h);
  stashed_grid=art.stash.GenGrid(10,10,0.005);
  stashed_grid20=art.stash.GenGrid(20,20,0.005);
  stashed_grid_square=art.stash.GenGrid(1,1,0.005);
  plusIcon=library.Load("data/images/fonts/metal/Chrisdesign_Effect_Letters_alphabet_silver_42.png");
  checkIcon=library.Load("data/images/icon/check.png");
  discIcon=library.Load("data/images/icon/gamesaved.png");
  typeOptions.Add("Line",0);
  typeOptions.Add("Lines",1);
  typeOptions.Add("Curve",2);
  typeOptions.Add("FilledCircle",3);
  typeOptions.Add("Triangle",4);
  typeOptions.Add("Quad",5);
  if ( file_exists("data/out/font.txt") ) {
   collections.fromString(file_as_string("data/out/font.txt").c_str());
  } else if ( file_exists("data/out/font.bin") ) { /* ... */ }
 }
 void Between() {
 }
 Zp<Cartesiand> selectedPoint;
 void RenderCollections() {
  int Lx=16;
  int Ly=16;
  EACH(collections.first,TwoDeeStrokeCollection,c) {
   bool hovering= ( WITHIN(input->mxi,input->myi,Lx,Ly,Lx+64,Ly+64) );
   art.Rectangle(hovering?crayons.Pick(cyan):crayons.Pick(ANSIgrey),additive,Lx,Ly,64,64);
   if ( c->strokes.stashed_id < 0 ) {
    c->strokes.triangles.Clear();
    c->strokes.Get(&c->strokes.triangles);
    c->strokes.stashed_id.value=art.stash.GenTriangles(&c->strokes.triangles);
    OUTPUT("Stashed `%s` ord %d into %d with %d triangles\n",c->name.c_str(),c->ord.value,c->strokes.stashed_id.value,c->strokes.triangles.count.value);
   }
   art.Rectangle(hovering?crayons.Pick(alabaster):crayons.Pick(ANSIgrey),additive,(double)Lx+8.0,(double)Ly,48.0,48.0);
   art.Element(hovering?crayons.Pick(alabaster):crayons.Pick(macNcheese),none,c->strokes.stashed_id,(double)Lx+8.0,(double)Ly,48.0,48.0);
   Lx+=64;
   if ( Lx > display->w - 64 ) {
    Lx=16; Ly+=64;
    if ( Ly > display->h - 128 ) break;
   }
   if ( hovering && input->leftReleased() ) collection=c;
  }
  if ( fast.button(plusIcon,16,display->h-64-16,64,64) ) {
   collection=new TwoDeeStrokeCollection;
   collection->ord=collections.count;
   collections.Append(collection);
   stroke=null;
   return;
  }
 }
 void Render() {
  if ( !collection ) { RenderCollections(); return; }
  art.Rectangle(crayons.Pick(indigo),paper.x,paper.y,paper.w,paper.h);
  art.Element(crayons.Pick(indigo),additive,stashed_grid,paper.x,paper.y,paper.w,paper.h);
  art.Element(crayons.Pick(indigo),additive,stashed_grid20,paper.x,paper.y,paper.w,paper.h);
  art.Element(crayons.Pick(darkBlue),none,stashed_grid_square,paper.x,paper.y,paper.w,paper.h);
  art.Element(crayons.Pick(indigo),additive,stashed_grid,paper.x+paper.w/4.0,paper.y+paper.h/4.0,paper.w/2.0,paper.h/2.0);
  art.Element(crayons.Pick(indigo),additive,stashed_grid_square,paper.x+paper.w/4.0,paper.y+paper.h/4.0,paper.w/2.0,paper.h/2.0);
  art.Rectangle(crayons.Pick(ANSIgrey),additive,paper.x,paper.y+paper.h*0.7,paper.w,1.0);
  art.Rectangle(crayons.Pick(ANSIbackgrey),additive,paper.x,paper.y+paper.h*0.6,paper.w,1.0);
  art.Rectangle(crayons.Pick(ANSIbackgrey),additive,paper.x+paper.w/2.0,paper.y,1.0,paper.h);
  EACH(collection->strokes.first,TwoDeeStroke,s) if ( s != stroke ) {
   EACH(s->lines.first,Cartesiand,a) {
    Cartesiand A(paper.x+a->x*paper.w,paper.y+a->y*paper.h);
    art.Rectangle(crayons.jami(0,96,16,0,255),additive,A.x-3.0,A.y-3.0,6.0,6.0);
    Cartesiand *b=(Cartesiand *) a->next;
    if ( b ) {
     Cartesiand B(paper.x+b->x*paper.w,paper.y+b->y*paper.h);
     art.Rectangle(crayons.jami(0,127,0,0,255),additive,B.x-3.0,B.y-3.0,6.0,6.0);
     DrawLine(crayons.jami(0,0,127,0,255),A.x,A.y,B.x,B.y);
    }
   }
  } else {
   switch ( stroke->type ) {
    case tds_Line: break;
    case tds_Lines: {
     int Lx=(int)(paper.x2+512.0);
     int Ly=(int)paper.y;
     int i=1;
     EACH(stroke->lines.first,Cartesiand,a) {
      if ( WITHIN(input->mxi,input->myi,Lx,Ly,Lx+128,Ly+64) ) {
       selectedPoint=a;
      }
      fast.arrows(I2S(i),a,Lx+32,Ly+32,16,false,0.01);
      i++; Ly+=64;
      if ( Ly > display->w-64 ) {
       Ly=(int)paper.y;
       Lx+=128;
      }
      Cartesiand A(paper.x+a->x*paper.w,paper.y+a->y*paper.h);
      art.Rectangle((selectedPoint == a) ? crayons.Pick(gold) : crayons.jami(0,127,0,0,255),additive,A.x-3.0,A.y-3.0,6.0,6.0);
      Cartesiand *b=(Cartesiand *) a->next;
      if ( b ) {
       Cartesiand B(paper.x+b->x*paper.w,paper.y+b->y*paper.h);
       art.Rectangle(crayons.jami(0,127,0,0,255),additive,B.x-3.0,B.y-3.0,6.0,6.0);
       DrawLine2Color(crayons.Pick(red255),crayons.Pick(green255),A.x,A.y,B.x,B.y);
      }
     }
    } break;
    case tds_Curve:
    case tds_Quad: {
     Cartesiand P;
     P.Set(paper.x+stroke->A.x*paper.w,paper.y+stroke->A.y*paper.h);
     art.Rectangle(crayons.jami(0,0,127,0,255),additive,P.x-3.0,P.y-3.0,6.0,6.0);
     fast.arrows("",&stroke->A,(int)paper.x2+512+32,32+32,16,false,0.01);
     P.Set(paper.x+stroke->B.x*paper.w,paper.y+stroke->B.y*paper.h);
     art.Rectangle(crayons.jami(0,0,127,0,255),additive,P.x-3.0,P.y-3.0,6.0,6.0);
     fast.arrows("",&stroke->B,(int)paper.x2+512+32,32+32+128,16,false,0.01);
     P.Set(paper.x+stroke->C.x*paper.w,paper.y+stroke->C.y*paper.h);
     art.Rectangle(crayons.jami(0,0,127,0,255),additive,P.x-3.0,P.y-3.0,6.0,6.0);
     fast.arrows("",&stroke->C,(int)paper.x2+512+32,32+32+128*2,16,false,0.01);
     P.Set(paper.x+stroke->D.x*paper.w,paper.y+stroke->D.y*paper.h);
     art.Rectangle(crayons.jami(0,0,127,0,255),additive,P.x-3.0,P.y-3.0,6.0,6.0);
     fast.arrows("",&stroke->D,(int)paper.x2+512+32,32+32+128*3,16,false,0.01);
    } break;
    case tds_FilledCircle: break;
    case tds_Triangle: break;
   }
  }
  Cartesiand mouse((double)input->mxi,(double)input->myi);
  Zbool mouseOnPaper=mouse.x >= paper.x && mouse.y >= paper.y && mouse.x <= paper.x2 && mouse.y <= paper.y2;
  if ( mouseOnPaper ) {
   Cartesiand mousePaper((mouse.x-paper.x)/paper.w,(mouse.y-paper.y)/paper.h);
   art.Rectangle(crayons.Pick(ANSIbackgrey),additive,paper.x,(double)input->myi,paper.w,1.0);
   art.Rectangle(crayons.Pick(ANSIbackgrey),additive,(double)input->mxi,paper.y,1.0,paper.h);
   if ( input->leftReleased() && stroke ) {
    if ( input->KeyDown(DX_A) ) stroke->A.Set(mousePaper.x,mousePaper.y);
    else if ( input->KeyDown(DX_B) ) stroke->B.Set(mousePaper.x,mousePaper.y);
    else if ( input->KeyDown(DX_C) ) stroke->C.Set(mousePaper.x,mousePaper.y);
    else if ( input->KeyDown(DX_D) ) stroke->D.Set(mousePaper.x,mousePaper.y);
    else switch ( stroke->type ) {
     case tds_Line: break;
     case tds_Lines: {
      stroke->lines.Append(new Cartesiand(mousePaper.x,mousePaper.y));
     } break;
     case tds_Curve: break;
     case tds_FilledCircle: break;
     case tds_Quad: break;
     case tds_Triangle: break;
    }
   }
  }
  if ( stroke ) {
   stroke->thickness=fast.hslider("thickness",stroke->thickness,0.0,0.5,(int)paper.x,(int)paper.y2+32+64,(int)paper.w,24,true);
   stroke->endPrecision=fast.hslider("end precision",stroke->endPrecision,2,64,(int)paper.x,(int)paper.y2+32+64+32,(int)paper.w,24,true);
   stroke->precision=fast.hslider("segments",stroke->precision,4,64,(int)paper.x,(int)paper.y2+32+64+64,(int)paper.w,24,true);
   if ( fast.button(fast.closeIcon,(int)paper.x2+17,(int)paper.y2,64,64) ) {
    collection->strokes.Remove(stroke);
    delete stroke.pointer;
    stroke=null;
   }
  }
  int picked=fast.list(&collection->strokes,(int)paper.x2+17,(int)paper.y,64,30,&selected.value,&scrolled.value);
  if ( picked != -1 ) {
   selected=picked;
   stroke=(TwoDeeStroke *)collection->strokes.Element(picked);
  }
  if ( fast.button(plusIcon,(int)paper.x,(int)paper.y2+16,64,64) ) {
   stroke=new TwoDeeStroke;
   stroke->type=tds_Lines;
   stroke->thickness=0.175;
   collection->strokes.Append(stroke);
   selected=collection->strokes.count-1;
  }
  if ( fast.button(fast.closeIcon,(int)paper.x+64+16,(int)paper.y2+16,64,64) ) {
   collection->strokes.Clear();
  }
  if ( input->leftReleased() ) {
   if ( collection->strokes.stashed_id >= 0 ) art.stash.Replace(collection->strokes.stashed_id);
   collection->strokes.triangles.Clear();
   collection->strokes.Get(&collection->strokes.triangles);
   collection->strokes.stashed_id.value=art.stash.GenTriangles(&collection->strokes.triangles);
   OUTPUT("Stashed `%s` ord %d into %d with %d triangles\n",
    collection->name.c_str(),collection->ord.value,collection->strokes.stashed_id.value,collection->strokes.triangles.count.value);
  }
  if ( fast.button(discIcon,(int)paper.x+128+32,(int)paper.y2+16,64,64) ) {
   BinaryFile file("data/out/font.bin",true);
   collections.BinaryWrite(&file);
   string_as_file(collections.toString().c_str(),"data/out/font.txt");
   collections.toCode();
  }
  if ( fast.button(fast.leftArrow,(int)paper.x+128+48+64,(int)paper.y2+16,64,64) ) {
   if ( stroke ) {
    if ( stroke->lines.count > 0 ) {
     Cartesiand *c=(Cartesiand *)stroke->lines.last;
     if ( c ) {
      if ( selectedPoint == c ) selectedPoint=null;
      stroke->lines.Remove(c);
      delete c;
     }
    }
   }
  }
  {
   string name=collection->name;
   collection->name=fast.textbox("Name",collection->name,display->w/2,display->h-16-32,10);
   if ( collection->name.length > 0 && str_cmp(name.c_str(),collection->name.c_str()) ) OUTPUT("%s\n",collection->name.c_str());
  }
  collection->ord=fast.integer("ORD",collection->ord.value,0,1024,1,display->w/4,display->h-16-32,5,false);
  if ( fast.button(fast.linesIcon,16,display->h-32-16,32,32) ) {
   collection=null;
   stroke=null;
   return;
  }
  if ( stroke ) stroke->type=(TwoDeeStrokeType)fast.enumerated(&typeOptions,"Type",(int)stroke->type,(int)paper.x+256+64,(int)paper.y2+16,48,24);
  if ( collection->strokes.stashed_id > 0 ) {
   art.Element(crayons.jami(0,32,32,32,255),additive,collection->strokes.stashed_id,paper.x,paper.y,paper.w,paper.h);
   art.Element(crayons.Pick(macNcheese),none,collection->strokes.stashed_id,paper.x2+256.0,paper.y,256.0,256.0);
   art.Element(crayons.Pick(macNcheese),none,collection->strokes.stashed_id,paper.x2+256.0,paper.y+256.0,128.0,128.0);
   art.Element(crayons.Pick(macNcheese),none,collection->strokes.stashed_id,paper.x2+256.0,paper.y+384.0,64.0,64.0);
   art.Element(crayons.Pick(macNcheese),none,collection->strokes.stashed_id,paper.x2+256.0,paper.y+384.0+64.0,32.0,32.0);
   art.Element(crayons.Pick(macNcheese),none,collection->strokes.stashed_id,paper.x2+256.0,paper.y+384.0+64.0+32.0,16.0,24.0);
   art.Element(crayons.Pick(hotPink),none,   collection->strokes.stashed_id,paper.x2+256.0+32.0,paper.y+384.0+64.0+32.0,24.0,24.0);
   art.Element(crayons.Pick(hotPink),none,   collection->strokes.stashed_id,paper.x2+256.0+64.0,paper.y+384.0+64.0+32.0,16.0,16.0);
   art.Element(crayons.Pick(hotPink),none,   collection->strokes.stashed_id,paper.x2+256.0+96.0,paper.y+384.0+64.0+32.0,14.0,18.0);
   art.Element(crayons.Pick(alabaster),none, collection->strokes.stashed_id,paper.x2+256.0+128.0,paper.y+384.0+64.0+32.0,10.0,14.0);
   art.Element(crayons.Pick(alabaster),none, collection->strokes.stashed_id,paper.x2+257.0+128.0+16.0,paper.y+384.0+64.0+32.0+20.0,5.0,7.0);
   art.Element(crayons.Pick(hotPink),none,   collection->strokes.stashed_id,paper.x2+257.0+64.0,paper.y+384.0+64.0+32.0+20.0,16.0,16.0);
   art.Element(crayons.Pick(hotPink),none,   collection->strokes.stashed_id,paper.x2+257.0+96.0,paper.y+384.0+64.0+32.0+20.0,14.0,18.0);
   art.Element(crayons.Pick(alabaster),none, collection->strokes.stashed_id,paper.x2+257.0+128.0,paper.y+384.0+64.0+32.0+20.0,10.0,14.0);
   art.Element(crayons.Pick(alabaster),none, collection->strokes.stashed_id,paper.x2+257.0+128.0+16.0,paper.y+384.0+64.0+32.0+20.0,5.0,7.0);
   for ( int i=0; i<1920/6; i++ ) {  
    art.Element(crayons.Pick(alabaster),none, collection->strokes.stashed_id,i*6,display->h-10,5.0,7.0);
   }
  }
 }
};

