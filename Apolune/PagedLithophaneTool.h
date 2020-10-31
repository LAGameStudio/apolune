#pragma once

/************************************************************** Author: H. Elwood Gilliland III
 *  _____  _                 _____       _                    * (c) 2014 PieceMaker Technologies
 * |  _  ||_| ___  ___  ___ |     | ___ | |_  ___  ___        *----------------------------------
 * |   __|| || -_||  _|| -_|| | | || .'|| '_|| -_||  _|       * Paged Lithophane example/test
 * |__|__ |_||___||___||___||_|_|_||__,||_,_||____|_|         * 
 * |_   _| ___  ___ | |_  ___  ___ | | ___  ___ |_| ___  ___  * 
 *   | |  | -_||  _||   ||   || . || || . || . || || -_||_ -| * 
 *   |_|  |___||___||_|_||_|_||___||_||___||_  ||_||___||___| * 
 *                                         |___|              *
 **************************************************************/

#include "GLWindow.h"
#include "PagedLithophane.h"
#include "LookAtPerspective.h"
#include "ParallaxShader.h"
#include "ConvolverShader.h"
#include "TextureLibrary.h"
#include "Cube.h"
#include "FastGUI.h"

class PagedLithophaneTool : public GLWindow {
public:
 Zdisposable<PagedLithophane> litho;
 // Object
 Cartesiand position;
 Cartesiand rotation;
 Zp<GLImage> overlay,cropped,resized,original,lithoized,combined,image;
 ImmediateLighting lights;
 LookAtPerspective lap;
 Cube3d box;
 PolygonList pList;
 Zfloat keyDelay;
 Zbool image_editor;
 Zstring filename,output_file,overlay_file;
 Zdouble imagex, imagey;
 Zuint samplesW, samplesH;
 Zdouble unitW, unitH;
 Zdouble total_thickness;
 Zdouble etch_floor;
 Zdouble crop_left,crop_top,crop_width,crop_height;
 Zint poster_levels;
 Zbool do_not_process_images,shift;
 Crayon milo_lo,milo_hi,replace;
 Zbool weighted_luminance,inverted,invertedtopo;
 Zp<GLWindow> back;
 void OnLoad() {
  ip_Lithoize.Init(); // Must be called before use!
  lap.center.Set(0.0,0.0,0.0);
  lap.eye.Set(0.0,0.0,200.0);
  lap.Near=0.1;
  lap.Far=1000.0;
  background.Pick(indigo);
  Fullscreen();
  image_editor=true;
  overlay_file="data/in/Overlay-Invert.png";
  filename="data/in/IMG_0084.JPG";
  output_file="data/out/test.stl";
  replace.Pick(alabaster);
  ResetLithoSettings();
 }
 void ResetLithoSettings() {
  milo_lo.Float( 0.0f,0.5f, 0.0f,1.0f);
  milo_hi.Float(0.75f,1.0f,0.75f,1.0f);
  crop_top=0.0;
  crop_left=0.0;
  crop_width=1.0;
  crop_height=1.0;
  poster_levels=10;
  imagex=1.0;
  imagey=1.0;
  samplesW=8;
  samplesH=8;
  unitW=50.0;
  unitH=50.0;
  total_thickness=3.0;
  etch_floor=0.4;
 }
 void PrepareImage() {
  OUTPUT("Discarding previous images...\n");
  if ( overlay )   { library.Unload(overlay);   overlay=null;   }
  if ( original )  { library.Unload(original);  original=null;  }
  if ( lithoized ) { library.Unload(lithoized); lithoized=null; }
  if ( cropped )   { library.Unload(cropped);   cropped=null;   }
  if ( resized )   { library.Unload(resized);   resized=null;   }
  if ( combined )  { library.Unload(combined);  combined=null;  }
  if ( image )     { library.Unload(image);     image=null;     }
  OUTPUT("Preparing images...\n");
  if ( file_exists(filename.c_str()) ) {
   if ( do_not_process_images ) {
    image=library.Load(filename.c_str(),true,true);
   } else {
    overlay=library.Load(overlay_file.c_str(),true,true);
    original=library.Load(filename.c_str(),true,true);
    OUTPUT("Lithoizing...\n");
    ip_Lithoize.weightedLum=weighted_luminance;
    ip_Lithoize.levels=poster_levels;
    ip_Lithoize.lo.fromCrayon(milo_lo);
    ip_Lithoize.hi.fromCrayon(milo_hi);
    ip_Lithoize.replace.fromCrayon(replace);
    lithoized=library.Load(original,&ip_Lithoize,true);
    OUTPUT("Cropping...\n");
    ip_Crop.left=(int)((double)lithoized->width*crop_left);
    ip_Crop.top=(int)((double)lithoized->height*crop_top);
    ip_Crop.new_w=(int)((double)lithoized->width*crop_width);
    ip_Crop.new_h=(int)((double)lithoized->height*crop_height);
    cropped=library.Load(lithoized,&ip_Crop,true);
    OUTPUT("Resizing...\n");
    ip_Resizer.new_w=overlay->width;
    ip_Resizer.new_h=overlay->height;
    resized=library.Load(cropped,&ip_Resizer,true);
    if ( inverted ) {
     OUTPUT("Combining...\n");
     ip_Combiner.B=overlay;
     combined=library.Load(resized,&ip_Combiner,true);
     image=library.Load(combined,&ip_Invert,true);
    } else {
     OUTPUT("Combining...\n");
     ip_Combiner.B=overlay;
     image=library.Load(resized,&ip_Combiner,true);
    }
   }
  }
 }
 void PrepareLitho() {
  if ( image ) {
   OUTPUT("Calculating new lithograph...\n");
   litho.Recycle( new PagedLithophane(image,imagex,imagey,samplesW.value,samplesH.value,unitW,unitH,total_thickness,etch_floor,invertedtopo) );
   position.x=-litho->size.x/2.0;
   position.z=-litho->size.y/2.0;
  }
 }
 void SaveLitho() {
  OUTPUT("Saving lithograph...\n");
  litho->topography->stl.ascii=true;
  litho->topography->stl.Write(output_file.c_str());
 }
 void Between() {
  shift = input->KeyDown(DX_LSHIFT) || input->KeyDown(DX_RSHIFT);
  rotation.x=360.0f*(input->myi-display->h2)/display->hf;
  rotation.y=360.0f*(input->mxi-display->w2)/display->wf;
  if ( keyDelay > 0.0f ) keyDelay-=(float)FRAMETIME;
  else {
   if ( input->KeyDown(NumpadPlus)  || input->KeyDown(DX_EQUALS) ) { lap.eye.Set(0.0,0.0,lap.eye.z/2); keyDelay=0.25f; }
   if ( input->KeyDown(NumpadMinus) || input->KeyDown(DX_MINUS) )  { lap.eye.Set(0.0,0.0,lap.eye.z*2); keyDelay=0.25f; }
   if ( input->KeyDown(DX_SPACE) ) { ResetLithoSettings(); }
  }
 }
 void Render() {
  if ( image_editor ) {
   RenderImager();
  } else {
   RenderLitho();
  }
 }
 void RenderImager() {
  if ( image ) Stretchi(
   image,
   crayons.Pick(alabaster),1.0f,none,
   display->w/2-image->width/2,display->h/2-image->height/2,
   image->width,image->height
  );
  if ( overlay ) Stretchi(
   overlay,
   crayons.Pick(alabaster),1.0f,transparency,
   display->w/2-overlay->width/2,display->h-overlay->height/4-16,
   overlay->width/4,overlay->height/4
  );
  if ( original ) Stretchi(
   original,
   crayons.Pick(alabaster),1.0f,none,
   display->w/2,display->h-original->height/4-16,
   original->width/4,original->height/4
  );
  if ( lithoized ) Stretchi(
   lithoized,
   crayons.Pick(alabaster),1.0f,none,
   display->w/2+original->width/4,display->h-lithoized->height/4-16,
   lithoized->width/4,lithoized->height/4
  );
  if ( cropped ) Stretchi(
   cropped,
   crayons.Pick(alabaster),1.0f,none,
   display->w/2+original->width/4+cropped->width/4,display->h-cropped->height/4-16,
   cropped->width/4,cropped->height/4
  );
  if ( resized ) Stretchi(
   resized,
   crayons.Pick(alabaster),1.0f,none,
   display->w/2+lithoized->width/4+original->width/4+cropped->width/4,display->h-resized->height/4-16,
   resized->width/4,resized->height/4
  );
  if ( image && fast.button("Litho>",16,display->h-32-16,96,32) ) {
   image_editor=false;
   PrepareLitho();
  }
  if ( fast.button("<Exit",display->w-128,display->h-196-16,96,32) ) {
   back->visible=true;
   this->destroyMe=true;
   return;
  }
  if ( fast.button("Reload",16,16+32+16,96,32) ) {
   PrepareImage();
  }
  filename=fast.textbox("",filename,16,16,255);
  crop_left  =fast.decimal("Crop: Left",  crop_left,  0.0,0.99, shift ? 0.1 : 0.05,16,16+128,32,24);
  crop_top   =fast.decimal("Crop: Top",   crop_top,   0.0,0.99, shift ? 0.1 : 0.05,16+128,16+128,32,24);
  crop_width =fast.decimal("Crop: Width", crop_width, 0.01,1.0, shift ? 0.1 : 0.05,16,16+128+64,32,24);
  crop_height=fast.decimal("Crop: Height",crop_height,0.01,1.0, shift ? 0.1 : 0.05,16+128,16+128+64,32,24);
  poster_levels=fast.integer("Levels Per Channel",poster_levels,1,32,1,16,16+128+128+128,32,24);
  bool was=do_not_process_images;
  do_not_process_images=fast.toggle("On","Off","Do Not Process",do_not_process_images,16+256+32,16+64,32,32);
  if ( do_not_process_images != was ) PrepareImage();
  was=inverted; 
  inverted=fast.toggle("On","Off","Inverted",inverted,16+256+32+512,16+64,32,32);
  if ( inverted != was ) PrepareImage();
  was=weighted_luminance;
  weighted_luminance=fast.toggle("On","Off","Weighted Luminance",weighted_luminance,16+256+32+512+128,16+64,32,32);
  if ( weighted_luminance != was ) PrepareImage();
  if ( fast.button("Reset Crop",16+512-128,16+128,96,32) ) {
   crop_left=0.0;
   crop_top=0.0;
   crop_width=1.0;
   crop_height=1.0;
  }
  if ( fast.button("Reset Values",16+512,16+128,128,32) ) {
   ResetLithoSettings();
  }
  milo_lo.Redf(  (float)fast.hslider("MILO Low Red",  (double)milo_lo.rf,0.0,(double)milo_hi.rf,  display->w-300,16+64,255,16,true));
  milo_lo.Greenf((float)fast.hslider("         Green",(double)milo_lo.gf,0.0,(double)milo_hi.gf,  display->w-300,16+64+64,255,16,true));
  milo_lo.Bluef( (float)fast.hslider("         Blue", (double)milo_lo.bf,0.0,(double)milo_hi.bf,  display->w-300,16+64+64+64,255,16,true));
  milo_hi.Redf(  (float)fast.hslider("MILO High Red",  (double)milo_hi.rf,(double)milo_lo.rf,1.0, display->w-300,16+64+64+64+16+64,255,16,true));
  milo_hi.Greenf((float)fast.hslider("          Green",(double)milo_hi.gf,(double)milo_lo.gf,1.0, display->w-300,16+64+64+64+16+64+64,255,16,true));
  milo_hi.Bluef( (float)fast.hslider("          Blue", (double)milo_hi.bf,(double)milo_lo.bf,1.0, display->w-300,16+64+64+64+16+64+64+64,255,16,true));
  replace.Redf(  (float)fast.hslider("MILO Replace Red",  (double)replace.rf,0.0,1.0, display->w-300,            16+64+64+64+16+64+64+64+16+64,255,16,true));
  replace.Greenf((float)fast.hslider("             Green",(double)replace.gf,0.0,1.0, display->w-300,            16+64+64+64+16+64+64+64+16+64+64,255,16,true));
  replace.Bluef( (float)fast.hslider("             Blue", (double)replace.bf,0.0,1.0, display->w-300,            16+64+64+64+16+64+64+64+16+64+64+64,255,16,true));
  overlay_file=fast.textbox("Overlay File",overlay_file,16,display->h-32-16-32-32-64,128);
  MultilineText("SHIFT for alternate delta",display->wd-256.0,display->hd-256.0,fast.smallFontW,fast.smallFontH,0,false);
  if ( input->leftReleased() ) PrepareImage();
 }
 void RenderLitho() {
  RenderPreview();
  output_file=fast.textbox("",output_file,16,display->h-32-16-32-32,255);
  if ( fast.button("Save",16,display->h-32-16-32,96,32) ) {
   SaveLitho();
  }
  if ( fast.button("<Image",16+96+16,display->h-32-16,96,32) ) {
   image_editor=true;
  }
  if ( fast.button("Calculate",16,display->h/2,128,32) ) {
   PrepareLitho();
  }
  samplesW=(unsigned int) fast.integer("Samples/Page X",(int)samplesW,2,32,shift?4:2,16,96,32,24);
  samplesH=(unsigned int) fast.integer("Samples/Page Y",(int)samplesH,2,32,shift?4:2,16+128,96,32,24);
  unitW=fast.decimal("Wide (mm)",unitW,10.0,200.0,shift?10.0:1.0,16,256,32,24);
  unitH=fast.decimal("Tall (mm)",unitH,10.0,200.0,shift?10.0:1.0,16+128,256,32,24);
  total_thickness=fast.decimal("Thickness (mm)",total_thickness,0.4,50.0,shift?0.05:0.1,16+128+16+128,256,32,24);
  etch_floor=fast.decimal("Etch Floor (mm)",etch_floor,0.2,30.0,shift?0.05:0.1,16+128+16+128+16+128,256,32,24);
  MultilineText("+ and - to zoom\nSHIFT for alternative delta",display->wd-256.0,display->hd-256.0,fast.smallFontW,fast.smallFontH,4,false);
//  invertedtopo=fast.toggle("On","Off","Invert Topography",invertedtopo,16+256+32+512,16+64,32,32);
 }
 void RenderPreview() {
  Blending(none);
  glPush();
  lap.Apply();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();///
   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);
   glDepthRange(0.1,1000.0);
   glDepthFunc(GL_LEQUAL);
   glRotatef((GLfloat) rotation.x, 1.0, 0.0, 0.0);
   glRotatef((GLfloat) rotation.y, 0.0, 0.0, 1.0);
   glColor3d(0.65,0.1,0.65);
//   glEnable(GL_CULL_FACE);//Enable(GL_CULL_FACE);
//   glCullFace(GL_BACK);
   glPolygonMode(GL_FRONT,GL_FILL);//_AND_BACK,GL_FILL);
   glEnable(GL_POLYGON_SMOOTH);
   litho->RenderWireframe();
//   glDisable(GL_DEPTH_TEST);
  glPopMatrix();
  glPushMatrix();///
   glRotatef((GLfloat) rotation.x, 1.0, 0.0, 0.0);
   glRotatef((GLfloat) rotation.y, 0.0, 0.0, 1.0);
   glLineWidth(4.2f);
   Coordinator();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef((GLfloat) (litho->size.x*0.7), 0.0f,1.0f);
    Text("RIGHT",0,0,2,3,true,0);
   glPopMatrix();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef((GLfloat) -(litho->size.x*0.7), 0.0f,1.0f);
    Text("LEFT",0,0,2,3,true,0);
   glPopMatrix();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, (GLfloat) -(litho->size.y*0.7), 1.0f);
    Text("BACK",0,0,2,3,true,0);
   glPopMatrix();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, (GLfloat) (litho->size.y*0.7), 1.0);
    Text("FRONT",0,0,2,3,true,0);
   glPopMatrix();
   glLineWidth(1.2f);
   glPushMatrix();//
    glTranslatef((GLfloat) -(litho->size.x/2)-10.0f, (GLfloat) -(litho->size.y*0.5), 0.0f);
    GLfloat cm=0.0f;
    for ( unsigned int t=0; t<30; t++ ) {
     cm=t*10.0f;
     glPushMatrix();
      glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
      glTranslatef(0.0f, -cm, 0.0f);
      Text(FORMAT("%-2d cm _____",(int)t).c_str(),0,0,1,1.6,false,0);
     glPopMatrix();
     if ( cm > litho->size.y ) break;
    }
   glPopMatrix();//
   glPushMatrix();//
    glTranslatef( (GLfloat) -(litho->size.x*0.5), (GLfloat) -(litho->size.y*0.5), 0.0f);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f );
    for ( unsigned int t=0; t<30; t++ ) {
     cm=t*10.0f;
     glPushMatrix();
      glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
      glTranslatef(0.0f, -cm, 0.0f);
      Text(FORMAT("____ %-2d cm",(int)t).c_str(),0,0,1,1.6,false,0);
     glPopMatrix();
     if ( cm > litho->size.x ) break;
    }
   glPopMatrix();//
  glPopMatrix();///
  glPop();
  Blending(none);
  glLineWidth(1.2f);
  glColor3d(0.0,1.0,0.0);
  MultilineText(
   FORMAT("Pages: %d by %d\nSamples: %d (%dx%d)",
    litho->topography->pages.w,
    litho->topography->pages.h,
    litho->topography->samples.x*litho->topography->samples.y,
    litho->topography->samples.x,
    litho->topography->samples.y
   ),32,32,10,14,2,false);  
  if ( image ) Stretchi(image,crayons.Pick(alabaster),1.0f,transparency,
   display->w-litho->image->width/2-5,
   display->h-litho->image->height/2-5,
   litho->image->width/2,
   litho->image->height/2
  );
 }
};