#pragma once
#include "LinkedList.h"
#include "GLWindow.h"
#include "Proce55or.h"
#include "TextureLibrary.h"
#include "Art.h"
#include "Polygon.h"
#include "Model.h"
#include "InputControls.h"
#include "FastGUI.h"
#include "ImageProcess.h"
#include "ImageProcessingStack.h"

#define CANVAS_MIN_DIST  30

class CollagePaletteItem : public ListItem {
public:
 Zp<GLImage> image, icon;
 ScreenPosition area;
 Crayon tint;
 Cartesian dragOffset;
 ic_Blends blendType;
 Blends blend;
 CollagePaletteItem(const char * s) : ListItem() {
  image = library.Load(s,true,true);
  tint = crayons.Pick(alabaster);
  Blend(none);
 }
 CollagePaletteItem(const char * s, const char * i) : ListItem() {
  image = library.Load(s,true,true);
  icon = library.Load(i);
  Blend(none);
 }
 CollagePaletteItem(const char * s, const char * i, int x, int y, int w, int h, Crayon t) : ListItem() {
  image = library.Load(s,true,true);
  icon = library.Load(i);
  area.Set(x,y,w,h);
  tint = t;
  Blend(none);
 }
 CollagePaletteItem(const char * s, int x, int y, int w, int h, Crayon t) : ListItem() {
  image = library.Load(s,true,true);
  area.Set(x,y,w,h);
  tint = t;
  Blend(none);
 }
 CollagePaletteItem(const char * s, int x, int y, int w, int h, Crayon t, Blends b) : ListItem() {
  image = library.Load(s,true,true);
  area.Set(x,y,w,h);
  tint = t;
  Blend(b);
 }
 void Blend( Blends b ) {
  blend = b;
  switch ( blend ) {
       case _additive: blendType = ic_Add; break;
       case _multiply: blendType = ic_Multiply; break;
   case _transparency: blendType = ic_Transparency; break;
   default: blendType = ic_Overwrite; break;
  }
 }
 void Tint(Crayon t) {
  tint = t;
 }
};

class CollagePaletteItems : public LinkedList {
public:
 void Load(const char * s) {}
 void Add (const char * s) {
  CollagePaletteItem * lipi = new CollagePaletteItem(s);
  Append(lipi);
 }
 void Add (const char * s, const char * i, int x, int y, int w, int h, Crayon t) {
  CollagePaletteItem * lipi = new CollagePaletteItem(s,i,x,y,w,h,t);
  Append(lipi);
 }
 void Add (const char * s, int x, int y, int w, int h, Crayon t) {
  CollagePaletteItem * lipi = new CollagePaletteItem(s,x,y,w,h,t);
  Append(lipi);
 }
 void Add (CollagePaletteItem i) {
  CollagePaletteItem *p = new CollagePaletteItem(
   i.image->filename.c_str(),
   i.icon->filename.c_str(),
   i.area.x,i.area.y,i.area.w,i.area.h,i.tint
  );
  Append(p);
 }
 CLEARLISTRESET(CollagePaletteItem);
};

class CollageCanvasImagePlacement : public ListItem {
public:
 Zp<CollagePaletteItem> image;
 Vertexd positionRatio, scale, originalScale;
 Vertexd extents;
 Vertexd canvasExtents;
 Vertexd TL, TR, BL, BR;
 Cartesiand CP;
 Zdouble xPos, yPos;
 Vertexd mousePosWhenClicked;
 Vertexd mousePosWhenDragging;
 Zbool selected, hovering;
 Zp<mPolygon> tri1, tri2;
 ImmediateModel quad, translatedQuad;
 Zdouble angle;
 ic_Blends blendtype;
 Cartesiand clickedLine;
 CollageCanvasImagePlacement() : ListItem() {
  scale.Set(1.0,1.0);
  blendtype = ic_Add;
 }
 void CalculateExtents(double cw, double ch, double fboW, double fboH) {
  canvasExtents.Set(cw,ch);
  scale.x = CLAMPTO(scale.x,0.1f,4.0);
  scale.y = CLAMPTO(scale.y,0.1,4.0);
  while (angle > 360.0) angle -= 360.0;
  while (angle < 0.0  ) angle += 360.0;

  extents.x = (double)(image->image->width)*canvasExtents.x*scale.x;
  extents.y = (double)(image->image->height)*canvasExtents.y*scale.y;
  quad.Clear();
  Vertexd A,B,C,D;
  A.Set(-extents.x/2,extents.y/2);
  B.Set(extents.x/2,extents.y/2);
  C.Set(extents.x/2,-extents.y/2);
  D.Set(-extents.x/2,-extents.y/2);

  tri1 = new mPolygon;
  tri1->draw_method = GL_TRIANGLES;
  Vertex * v;
  v = new Vertex(D.x,D.y,D.z); v->Color(crayons.Pick(alabaster)); v->TexCoord(0.0f,0.0f); tri1->TriNormal(); tri1->addVertex(v);
  v = new Vertex(B.x,B.y,B.z); v->Color(crayons.Pick(alabaster)); v->TexCoord(1.0f,1.0f); tri1->TriNormal(); tri1->addVertex(v);
  v = new Vertex(A.x,A.y,A.z); v->Color(crayons.Pick(alabaster)); v->TexCoord(1.0f,0.0f); tri1->TriNormal(); tri1->addVertex(v);
  quad.addPolygon(tri1);

  tri2 = new mPolygon;
  tri2->draw_method = GL_TRIANGLES;
  v = new Vertex(D.x,D.y,D.z); v->Color(crayons.Pick(alabaster)); v->TexCoord(0.0f,0.0f); tri2->TriNormal(); tri2->addVertex(v);
  v = new Vertex(C.x,C.y,C.z); v->Color(crayons.Pick(alabaster)); v->TexCoord(0.0f,1.0f); tri2->TriNormal(); tri2->addVertex(v);
  v = new Vertex(B.x,B.y,B.z); v->Color(crayons.Pick(alabaster)); v->TexCoord(1.0f,1.0f); tri2->TriNormal(); tri2->addVertex(v);
  quad.addPolygon(tri2);

  xPos += mousePosWhenDragging.x;
  yPos += mousePosWhenDragging.y;

  translatedQuad.Clear();
  translatedQuad.Duplicate(&quad);

  Vertexd translate(xPos+((double)(image->image->width)*canvasExtents.x*1.0)/2,
  yPos+((double)(image->image->height)*canvasExtents.y*1.0)/2,0.0);
  Vertexd rotate(0.0,0.0,angle);
  Vertexd scale(scale.x,scale.y,0.0f);
  translatedQuad.ScaleRotateMove(&scale,&rotate,&translate);

  mPolygon * t1 = (mPolygon*)translatedQuad.polygons.first;
  mPolygon * t2 = (mPolygon*)translatedQuad.polygons.last;

  TL.Set((Vertex*)t1->points.first);
  BL.Set((Vertex*)t1->points.last);
  BR.Set((Vertex*)t1->points.first->next);
  TR.Set((Vertex*)t2->points.first->next);
  CP.Set((TL.x +BR.x)/2.0,(TL.y +BR.y)/2.0);
 }
 void Render(int w, int h) {
  if (hovering) {
   DrawLine(crayons.Pick(orange),TR.x,TR.y,TL.x,TL.y);
   DrawLine(crayons.Pick(orange),TR.x,TR.y,BR.x,BR.y);
   DrawLine(crayons.Pick(orange),BL.x,BL.y,BR.x,BR.y);
   DrawLine(crayons.Pick(orange),TL.x,TL.y,BL.x,BL.y);
  }
  if (selected) {
   DrawLine(crayons.Pick(cyan),TL.x,TL.y,TR.x,TR.y);
   DrawLine(crayons.Pick(cyan),TR.x,TR.y,BR.x,BR.y);
   DrawLine(crayons.Pick(cyan),BL.x,BL.y,BR.x,BR.y);
   DrawLine(crayons.Pick(cyan),TL.x,TL.y,BL.x,BL.y);
  }
  hovering = false;
  selected = false;
  switch(blendtype) {
   case ic_Add: {
    Blending(additive);
    translatedQuad.Render2dVTC(image->image,0.0f,0.0f,1.0f,1.0f);
    break;
   }
   case ic_Sub: {
    Blending(subtractive);
    translatedQuad.Render2dVTC(image->image,0.0f,0.0f,1.0f,1.0f);
    break;
   }
   case ic_Transparency: {
    Blending(transparency);
    translatedQuad.Render2dVTC(image->image,0.0f,0.0f,1.0f,1.0f);
    break;
   }
   case ic_Multiply: {
    Blending(multiply);
    translatedQuad.Render2dVTC(image->image,0.0f,0.0f,1.0f,1.0f);
    break;
   }
   default: {
    Stretchi(image->image,image->tint,1.0,image->blend,
    (int)(positionRatio.x*(double)w),(int)(positionRatio.y*(double)h),
    (int)(extents.x),
    (int)(extents.y)
    );
    break;
   }
  }
 }
};

class CollageCanvasImagePlacements : public LinkedList {
public:
 void Add(CollagePaletteItem * image, double xRatio, double yRatio, double cw, double ch, double fboW, double fboH) {
  CollageCanvasImagePlacement * placement = new CollageCanvasImagePlacement;
  placement->image = image;
  placement->positionRatio.x = xRatio;
  placement->positionRatio.y = yRatio;
  placement->xPos = fboW*xRatio;
  placement->yPos = fboH*yRatio;
  placement->CalculateExtents(cw,ch,fboW,fboH);
  placement->blendtype = image->blendType;
  Append(placement);
 }
 CLEARLISTRESET(CollageCanvasImagePlacement);
};


class CollageCanvasTextGlyph : public ListItem {
public:
 Zpointer<GLImage> image;
 ImmediateModel quad, translatedQuad;
 Vertex CanvasTL, CanvasTR, CanvasBL, CanvasBR;
 VBOStaticVNT vbo;
 GLTransform transform; 
 Vertex TL, TR, BR, BL, CP;
 void CalculateCanvasPos(double rotation, Vertexd scale, Vertexd translation) {
  transform.position(translation.x,translation.y,0.0);
  transform.rotation(0.0,0.0,rotation);
  transform.scale(scale.x,scale.y,0.0);
  CP.Set( 
   ((TL.x+BR.x) /2) + translation.x,
   ((TL.y+BR.y) /2) + translation.y
  );
 }
 void CalculateRelativePos(double offset, int wordLength2) {
  double halfHeight = (double)image->height/2;
  TL.Set(-wordLength2+offset,-halfHeight,0.0);         TL.Color(crayons.Pick(alabaster)); TL.TexCoord(0.0,0.0); 
  TR.Set((double)(TL.x+image->width),-halfHeight,0.0); TR.Color(crayons.Pick(alabaster)); TR.TexCoord(0.0,1.0);
  BR.Set((double)(TL.x+image->width),halfHeight,0.0);  BR.Color(crayons.Pick(alabaster)); BR.TexCoord(1.0,1.0);
  BL.Set(-wordLength2+offset,halfHeight,0.0);          BL.Color(crayons.Pick(alabaster)); BL.TexCoord(1.0,0.0);
  vbo.PushQuad(&TL,&TR,&BR,&BL); 
  vbo.Create();
 }
 void Render() {
  Blending(additive);
  glEnable(GL_TEXTURE_2D);
  crayons.Pick(alabaster).gl();
  image->Use();
  glPush(false);
  transform.Translate();
  transform.Rotate2d();
  transform.Scale();
  vbo.Render();
  glPop();
  glDisable(GL_TEXTURE_2D);
 }
};

class CollageCanvasTextGlyphs : public LinkedList {
public: 
 Vertexd scale;
 Vertexd translation;
 Zdouble rotation;
 Zp<mPolygon> tri1, tri2;
 ImmediateModel quad;
 Vertexd TL, TR, BL, BR;
 Cartesiand CP;
 Cartesiand clickedLine;
 Vertexd mousePosWhenClicked;
 void Add(const char * t, TexFont * tf) {
  Clear();
  const char *p=t;
  Zint totalLength;
  while ( *p != '\0' ) {
   CollageCanvasTextGlyph * gly = new CollageCanvasTextGlyph;
   gly->image = tf->glyph(*p);
   totalLength+=gly->image->width;
   Append(gly);
   p++;
  }
  if ( totalLength == 0 ) return;
  CalculateRelativePos(totalLength);
  CalculateCanvasPos();
 }
 void Render() {
  if(count>0) {
   EACH(first,CollageCanvasTextGlyph,glyph) glyph->Render();
   DrawLine(crayons.Pick(cyan),TL.x,TL.y,TR.x,TR.y);
   DrawLine(crayons.Pick(cyan),TR.x,TR.y,BR.x,BR.y);
   DrawLine(crayons.Pick(cyan),BL.x,BL.y,BR.x,BR.y);
   DrawLine(crayons.Pick(cyan),TL.x,TL.y,BL.x,BL.y);
  }
 }
 void CalculateCanvasPos() {
  EACH(first,CollageCanvasTextGlyph,glyph) glyph->CalculateCanvasPos(rotation,scale,translation);
  CalculateOuterRectangle();
 }
 void CalculateRelativePos(int wordLength) {
  int CharOffset = 0;
  wordLength/=2;
  EACH(first,CollageCanvasTextGlyph,glyph) {
   glyph->CalculateRelativePos((double)CharOffset,wordLength);
   CharOffset+=(glyph->image->width.value);
  }
 }
 void CalculateOuterRectangle() {
  CollageCanvasTextGlyph * firstGlyph = (CollageCanvasTextGlyph*)this->first;
  CollageCanvasTextGlyph * lastGlyph  = (CollageCanvasTextGlyph*)this->last;
  quad.Clear();

  tri1 = new mPolygon;
  tri1->draw_method = GL_TRIANGLES;
  Vertex * v;

  v = new Vertex(firstGlyph->BL.x,firstGlyph->BL.y,firstGlyph->BL.z); v->Color(crayons.Pick(alabaster)); v->TexCoord(0.0f,0.0f); tri1->TriNormal(); tri1->addVertex(v);
  v = new Vertex(lastGlyph->TR.x, lastGlyph->TR.y, lastGlyph->TR.z);  v->Color(crayons.Pick(alabaster)); v->TexCoord(1.0f,1.0f); tri1->TriNormal(); tri1->addVertex(v);
  v = new Vertex(firstGlyph->TL.x,firstGlyph->TL.y,firstGlyph->TL.z); v->Color(crayons.Pick(alabaster)); v->TexCoord(1.0f,0.0f); tri1->TriNormal(); tri1->addVertex(v);
  quad.addPolygon(tri1);

  tri2 = new mPolygon;
  tri2->draw_method = GL_TRIANGLES;
  v = new Vertex(firstGlyph->BL.x,firstGlyph->BL.y,firstGlyph->BL.z); v->Color(crayons.Pick(alabaster)); v->TexCoord(0.0f,0.0f); tri2->TriNormal(); tri2->addVertex(v);
  v = new Vertex(lastGlyph->BR.x, lastGlyph->BR.y, lastGlyph->BR.z);  v->Color(crayons.Pick(alabaster)); v->TexCoord(0.0f,1.0f); tri2->TriNormal(); tri2->addVertex(v);
  v = new Vertex(lastGlyph->TR.x, lastGlyph->TR.y, lastGlyph->TR.z);  v->Color(crayons.Pick(alabaster)); v->TexCoord(1.0f,1.0f); tri2->TriNormal(); tri2->addVertex(v);
  quad.addPolygon(tri2);

  Vertexd translate(translation.x,translation.y,0.0);
  Vertexd rotate(0.0,0.0,rotation);
  Vertexd scale(scale.x,scale.y,0.0f);
  quad.ScaleRotateMove(&scale,&rotate,&translate);

  mPolygon * t1 = (mPolygon*)quad.polygons.first;
  mPolygon * t2 = (mPolygon*)quad.polygons.last;

  TL.Set((Vertex*)t1->points.first);
  BL.Set((Vertex*)t1->points.last);
  BR.Set((Vertex*)t1->points.first->next);
  TR.Set((Vertex*)t2->points.first->next);
  CP.Set((TL.x +BR.x)/2.0,(TL.y +BR.y)/2.0);
 }
};

class CollagePalette;
class CollageCanvas : public Proce55or {
public:
 Zp<FBOColorDepth> fbo;
 Zp<CollageCanvasImagePlacement> selected;
 Zp<CollagePalette> palette;
 Zint scaleBoxSize;
 Zbool scaling, dragging, rotating, hasPaletteItem;
 Vertexd dragOffset;
 CollageCanvasImagePlacements placements;
 Zint winH;
 Vertexd canvasFBORatio;
 Vertexd fboCanvasRatio;
 Vertexd dragRatio;
 Vertexd mousePos;
 Strings fontSizes;
 Zint activeFontSize;
 CollageCanvasTextGlyphs glyph;
 std::string text;
 Vertexd originalScale;     
 ImageProcessingStack stack;
 Zp<GLImage> processedImage;

 CollageCanvas() : Proce55or() {
  fbo = &fboCDS1024;
  scaleBoxSize = 40;
  glyph.scale.Set(1.0,1.0,1.0);
  stack.canvasToImageScale.Set((double)(stack.outputSize.x)/(double)(fbo->w),(double)(stack.outputSize.y)/(double)(fbo->h));
 }

 void OnSetExtents() {
  canvasFBORatio.Set(iratiod(w,fbo->w),iratiod(h,fbo->h));
  fboCanvasRatio.Set(iratiod(fbo->w,w),iratiod(fbo->h,h));
 }
 bool TryToAdd(CollagePaletteItem * toAdd) {    
  int imageX = input.mxi-toAdd->dragOffset.x;
  int imageY = input.myi-toAdd->dragOffset.y;
  int imageW = toAdd->area.w;
  int imageH = toAdd->area.h;
  if ( within(imageX,imageY) || within(imageX+imageW,imageY) || within(imageX,imageY+imageH) || within(imageX+imageW,imageY+imageH)) {
   placements.Add(toAdd,iratiod(imageX-x,w),iratiod(imageY-y,h),fboCanvasRatio.x,fboCanvasRatio.y,fbo->w,fbo->h);
   selected = (CollageCanvasImagePlacement*)placements.last;
  }
  return false;
 }
 Zdouble oldAngle, currAngle, diffAngle, clickedAngle;
 Cartesiand centerNow;
 Zbool shift, ctrl;
 Zbool movingText, scalingRotatingText;

 void HandleSelected() {
  if ( !hasPaletteItem ) {
   if ( selected ) {
    if( input.KeyDown(DX_SPACE) ) {
     selected->angle = 0.0;
     selected->scale.Set(1.0,1.0);
     selected->CalculateExtents(fboCanvasRatio.x,fboCanvasRatio.y,fbo->w,fbo->h);
    }
    selected->mousePosWhenDragging.Set(0.0,0.0);
    if ( WITHIN(input.mxi,input.myi,x,y,x+w,y+h) ) {
     mPolygon * t1 = (mPolygon*)selected->translatedQuad.polygons.first;
     mPolygon * t2 = (mPolygon*)selected->translatedQuad.polygons.last;
     if ( t1->pinpoly2d((float)(mousePos.x*fboCanvasRatio.x),(float)(mousePos.y*fboCanvasRatio.y))
       || t2->pinpoly2d((float)(mousePos.x*fboCanvasRatio.x),(float)(mousePos.y*fboCanvasRatio.y))) {
      double distToTL = ddistance(mousePos.x*fboCanvasRatio.x,mousePos.y*fboCanvasRatio.y,selected->TL.x,selected->TL.y);
      double distToBR = ddistance(mousePos.x*fboCanvasRatio.x,mousePos.y*fboCanvasRatio.y,selected->BR.x,selected->BR.y);
      double distToBL = ddistance(mousePos.x*fboCanvasRatio.x,mousePos.y*fboCanvasRatio.y,selected->BL.x,selected->BL.y);
      double distToTR = ddistance(mousePos.x*fboCanvasRatio.x,mousePos.y*fboCanvasRatio.y,selected->TR.x,selected->TR.y);
      if ( ( distToTL<CANVAS_MIN_DIST 
          || distToTR<CANVAS_MIN_DIST
          || distToBL<CANVAS_MIN_DIST
          || distToBR<CANVAS_MIN_DIST )
        && input.left && !dragging && !scaling && !rotating ) {
       rotating = true;
       selected->clickedLine.SetCorners(
        selected->CP.x,selected->CP.y,
        mousePos.x*fboCanvasRatio.x,mousePos.y*fboCanvasRatio.y
       );
       oldAngle = selected->angle;
      } else if ( input.left && !dragging && !rotating && !scaling ) {
       dragging = true;
      }
     }
     if ( !input.left ) dragging = rotating = scaling = false;
    }
    if ( rotating || scaling ) {
     clickedAngle = norm_deg(rad2deg(selected->clickedLine.LineAngle())); 
     centerNow.SetCorners(selected->CP.x,selected->CP.y,mousePos.x*fboCanvasRatio.x,mousePos.y*fboCanvasRatio.y);
     currAngle = norm_deg(rad2deg(centerNow.LineAngle()));
     diffAngle = currAngle-clickedAngle;
     selected->angle = oldAngle + diffAngle;
     selected->CalculateExtents(fboCanvasRatio.x,fboCanvasRatio.y,fbo->w,fbo->h);
     double origLineLength = selected->clickedLine.LineMagnitude();
     double currLineLength = centerNow.LineMagnitude();
     if ( shift ) selected->scale.x = currLineLength/origLineLength;
     else if ( ctrl ) selected->scale.y = currLineLength/origLineLength;
     else {
      selected->scale.x = currLineLength/origLineLength;
      selected->scale.y = currLineLength/origLineLength;
     }
     selected->CalculateExtents(fboCanvasRatio.x,fboCanvasRatio.y,fbo->w,fbo->h);
    }
    if ( dragging ) {
     selected->mousePosWhenDragging.Set(
      (mousePos.x*fboCanvasRatio.x)-((input.oldmxi-x)*fboCanvasRatio.x),
      (mousePos.y*fboCanvasRatio.y)-((input.oldmyi-y)*fboCanvasRatio.y)
     );
     selected->CalculateExtents(fboCanvasRatio.x,fboCanvasRatio.y,fbo->w,fbo->h);
    }
    if ( dragging && !input.left ) placements.Remove(selected);
   }
  }
 };

 void HandleText() {
  if ( glyph.count>0 ) {
   mPolygon * t1 = (mPolygon*)glyph.quad.polygons.first;
   mPolygon * t2 = (mPolygon*)glyph.quad.polygons.last;
   if ( t1->pinpoly2d((float)(mousePos.x*fboCanvasRatio.x),(float)(mousePos.y*fboCanvasRatio.y))
     || t2->pinpoly2d((float)(mousePos.x*fboCanvasRatio.x),(float)(mousePos.y*fboCanvasRatio.y)) ) {
    double distToTL = ddistance(mousePos.x*fboCanvasRatio.x,mousePos.y*fboCanvasRatio.y,glyph.TL.x,glyph.TL.y);
    double distToBR = ddistance(mousePos.x*fboCanvasRatio.x,mousePos.y*fboCanvasRatio.y,glyph.BR.x,glyph.BR.y);
    double distToBL = ddistance(mousePos.x*fboCanvasRatio.x,mousePos.y*fboCanvasRatio.y,glyph.BL.x,glyph.BL.y);
    double distToTR = ddistance(mousePos.x*fboCanvasRatio.x,mousePos.y*fboCanvasRatio.y,glyph.TR.x,glyph.TR.y);
    if( ( distToTL<CANVAS_MIN_DIST
       || distToTR<CANVAS_MIN_DIST
       || distToBL<CANVAS_MIN_DIST
       || distToBR<CANVAS_MIN_DIST )
       && input.left && !dragging && !scaling && !rotating ) {
     scalingRotatingText = true;
     glyph.clickedLine.SetCorners(
      glyph.CP.x,glyph.CP.y,
      mousePos.x*fboCanvasRatio.x,mousePos.y*fboCanvasRatio.y
     );
     oldAngle = glyph.rotation;
     originalScale = glyph.scale;
    }
    if ( input.left && !scalingRotatingText ) movingText = true;
    glyph.mousePosWhenClicked.Set(mousePos.x*fboCanvasRatio.x,mousePos.y*fboCanvasRatio.y);
   }
   if ( scalingRotatingText ) {
    clickedAngle = norm_deg(rad2deg(glyph.clickedLine.LineAngle())); 
    centerNow.SetCorners(glyph.CP.x,glyph.CP.y,mousePos.x*fboCanvasRatio.x,mousePos.y*fboCanvasRatio.y);
    currAngle = norm_deg(rad2deg(centerNow.LineAngle()));
    diffAngle = currAngle-clickedAngle;
    glyph.rotation = oldAngle + diffAngle;
    double origLineLength = glyph.clickedLine.LineMagnitude();
    double currLineLength = centerNow.LineMagnitude();
    if ( shift ) glyph.scale.x = originalScale.x* currLineLength/origLineLength;
    else if ( ctrl ) glyph.scale.y = originalScale.y*  currLineLength/origLineLength;
    else {
     glyph.scale.x = originalScale.x * (currLineLength/origLineLength);
     glyph.scale.y = originalScale.y * (currLineLength/origLineLength);
    }
    glyph.CalculateCanvasPos();
   }
   if ( !input.left ) {
    movingText = false;
    scalingRotatingText = false;
   }
   if ( movingText ) {
    glyph.translation.x = mousePos.x*fboCanvasRatio.x; 
    glyph.translation.y = mousePos.y*fboCanvasRatio.y;        
    glyph.CalculateCanvasPos();
   }
  }
 }

 void Between() {
  mousePos.Set(
   (double)(input.mxi-x),
   (double)(input.myi-y)
  );
  shift=input.KeyDown(LShift) || input.KeyDown(RShift);
  ctrl=input.KeyDown(LCtrl) || input.KeyDown(RCtrl);
  if( !movingText && !scalingRotatingText ) HandleSelected();
  if( !scaling && !rotating && !dragging && !hasPaletteItem ) HandleText();
 }

 void Render() {
  fbo->background.Pick(black);
  fbo->blend = none;
  fbo->tint.Pick(alabaster);
  fbo->Bind();
  glyph.Render();
  EACH(placements.first,CollageCanvasImagePlacement,i) {
   mPolygon * t1 = (mPolygon*)i->translatedQuad.polygons.first;
   mPolygon * t2 = (mPolygon*)i->translatedQuad.polygons.last;
   if ( t1->pinpoly2d((float)(mousePos.x*fboCanvasRatio.x),(float)(mousePos.y*fboCanvasRatio.y))
     || t2->pinpoly2d((float)(mousePos.x*fboCanvasRatio.x),(float)(mousePos.y*fboCanvasRatio.y)) ) {
    i->hovering = true;
    if( input.left && !dragging && !rotating && !scaling && !hasPaletteItem ) {
     selected = i;
    }
   }
   if ( i == selected ) {
    selected->selected = true;
    selected->hovering = false;
   }
   i->Render(fbo->w,fbo->h);
  }
  fbo->Unbind();
  fbo->RenderUpsideDown((int)x,(int)y,(int)(x+w),(int)(y+h));
  Rectangle(crayons.Pick(hotPink),(int)x,(int)y,(int)(x+w),(int)(y+h));
  crayons.Pick(orange).gl();
  if ( selected ) {
   MultilineText(
    FORMAT("Click: %1.2f, Curr: %1.2f, Image: %1.2f, Diff: %1.2f\n Center: %s", 
    (double)clickedAngle,(double)currAngle, (double)(selected->angle), (double)diffAngle, centerNow.toString().c_str()
    ).c_str(),
    (double)x,display.hd - display.hd/4.0,8.0,12.0,2.0,false
   );
  }
  RenderTextControls();
  if(fast.button("Make Image",0,560,128,48)) {
   stack.Clear();
   EACH(placements.first,CollageCanvasImagePlacement,i) {
    stack.Add(i->image->image,i->CP.x/(double)fbo->w,i->CP.y/(double)fbo->h,i->angle,i->scale.x,i->scale.y);
   }
   EACH(glyph.first,CollageCanvasTextGlyph,g) {
    stack.Add(g->image,g->CP.x/(double)fbo->w,g->CP.y/(double)fbo->h,glyph.rotation,glyph.scale.x,glyph.scale.y);
   }
   processedImage = stack.Process();
  }
  if ( fast.button("Get Image",0,660,128,48) ) {
   fbo->BindWithoutClearing();
   CPUImage *saving=fbo->Snapshot();
   fbo->Unbind();
   GLImage *g=library.Load("temporary",saving);
   GLImage *flipped=library.Load(g,&ip_YInvert,true);
   flipped->image->Save("data/out/lastimage.png");
   library.Remove(g);
   library.Remove(flipped);
  }
  if ( processedImage )
   Stretchi(processedImage,crayons.pick(alabaster),1.0,additive,0,600,500,500);
 // if(was!=activeFontSize)
   //Recompute text data

  //MultilineText(
  // FORMAT("input: %d,%d\n"
  // "Translated: %1.2f, %1.2f",
  // input.mxi-x,input.myi-y,
  // (double)(input.mxi-x)*fboCanvasRatio.x,(double)(input.myi-y)*fboCanvasRatio.y
  // ).c_str(),
  // 32.0,display.hd - display.hd/4.0,8.0,12.0,2.0,false
  //);
 }

  void RenderTextControls() {
   text = fast.textbox("",text,x.value,h+64,10,true);
   if(fast.button("Add Text",x.value+128,h+64,64,64)) {
    if(glyph.count==0)
     glyph.translation.Set(fbo->w/2,fbo->h/2);
    glyph.Add(text.c_str(),nexaBold);
   }
 }
};

class CollagePalette : public Proce55or {
public:
 Zint columns;
 Zint edgeGap;
 Zp<CollagePaletteItem> dragging;
 CollagePaletteItems images;
 Zp<CollageCanvas> canvas;
 Vertexd dragRatio;
 CollagePalette() : Proce55or() {
  columns = 3;
  edgeGap = 5;
 }
 void Add(const char * s) {
  images.Add(s);
  Update();
 }
 void Update() { 
  Cartesian imageExtents(w/columns,w/columns);
  Cartesian xy(x,y);
  int loopCount = 0;
  EACH(images.first,CollagePaletteItem,i) {
   i->area.x = this->x + edgeGap + (loopCount/columns*this->w/columns);
   i->area.y = this->y + edgeGap + (loopCount%columns*(this->h/columns));
   i->area.w = this->w/columns - edgeGap - edgeGap;
   i->area.h = this->h/columns - edgeGap - edgeGap;
   loopCount+=1;
  }
 }
 void Between() {
  if ( dragging ) canvas->hasPaletteItem = true;
  if (input.left) {
   EACH(images.first,CollagePaletteItem,i) {
    if (
      WITHIN(input.mxi,input.myi,i->area.x,i->area.y,i->area.w+i->area.x,i->area.h+i->area.y) 
      && !canvas->movingText
      && !canvas->scalingRotatingText
      && !canvas->dragging
      && !canvas->rotating
      && !canvas->scaling
      && !dragging
     ) {
     dragging = i;
     dragRatio.Set(iratiod(input.mxi-dragging->area.x,dragging->area.x),iratiod(input.myi-dragging->area.y,dragging->area.y));
     dragging->dragOffset.x = (int)(dragRatio.x*(double)dragging->area.x);
     dragging->dragOffset.y = (int)(dragRatio.y*(double)dragging->area.y);
    }
   }
  }
  if (input.leftReleased()) {
   if(dragging!=null) {
    canvas->hasPaletteItem = false;
    canvas->TryToAdd(dragging.pointer);
    dragging = null;
   }
  }
 }
 void Render() {
  Rectangle(crayons.Pick(green255),(int)x,(int)y,(int)(x+w),(int)(y+h));
  EACH(images.first,CollagePaletteItem,i) {
   Stretchi(i->image,i->tint,1.0f,none,i->area.x,i->area.y,i->area.w,i->area.h);
  }
  if (dragging) {
   Stretchi(
    dragging->image,dragging->tint,1.0f,none,
    input.mxi-dragging->dragOffset.x,
    input.myi-dragging->dragOffset.y,
    dragging->image->width,dragging->image->height
   );
  }  
 }
};

class ImageCollageTest : public GLWindow {
public:
 CollagePalette palette;
 CollageCanvas canvas;
 VBOStaticVNT vbo;
 Zp<GLImage> image;
 
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
  palette.canvas = &canvas;
  palette.SetExtents(32,32,256,512);
  palette.Add("data/images/elements/unlocked.png");
  palette.Add("data/images/elements/uparrow.png");
  palette.Add("data/images/elements/downarrow.png");
  palette.Add("data/images/elements/leftarrow.png");
  palette.Add("data/images/elements/rightarrow.png");
  palette.Add("data/images/elements/rounder_minigrid.png");
  palette.Add("data/images/icons/commodity.png");
  palette.Add("data/images/elements/rounded_less_512.png");
  image = library.Load("data/images/elements/unlocked.png");
  canvas.winH = display->h;
  canvas.SetExtents(32+256+32,32,512,512);
 }

 void Between() {
  palette.Between();
  canvas.Between();
 }

 void Render() {
  palette.Render();
  canvas.Render();
 }
};