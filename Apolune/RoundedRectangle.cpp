#include "RoundedRectangle.h"
#include "Trigger.h"

RoundedRectangles roundrects; // Global, may need to be cleared sometimes...

void RoundedRectangle::Calculate() {
 if ( borderLine.polygons.count > 0 ) borderLine.Clear();
 borderLine.addPolygon(borderPoly=new mPolygon);
 borderPoly->draw_method=GL_LINE_LOOP;
 Vertex *bp;

 if ( precision < 2 ) precision=2;
 Circle circle;
 circle.R=(double)radius;
 Vertexd point;

 //Build the middle section quad
 Vertex TL(rect.x,rect.y);               TL.Color(inner);
 Vertex TR(rect.x+rect.w,rect.y);        TR.Color(inner);
 Vertex BR(rect.x+rect.w,rect.y+rect.h); BR.Color(inner);
 Vertex BL(rect.x,rect.y+rect.h);        BL.Color(inner);
 interior.PushQuad(&TL,&TR,&BR,&BL); 
 
 //Calculate the bottom border quad
 Vertex bottomTR((float)(rect.x+rect.w),(float)(rect.y+rect.h),0.0f);         bottomTR.Color(inner);
 Vertex bottomBR((float)(rect.x+rect.w),(float)(rect.y+rect.h+radius),0.0f);  bottomBR.Color(outer);
 Vertex bottomBL((float)rect.x,(float)(rect.y+rect.h+radius),0.0f);           bottomBL.Color(outer);
 Vertex bottomTL((float)rect.x,(float)(rect.y+rect.h),0.0f);                  bottomTL.Color(inner);  
 interior.PushQuad(&bottomTL,&bottomTR,&bottomBR,&bottomBL);
 
 //Calculate the left border quad
 Vertex leftTR((float)rect.x,(float)rect.y,0.0f);                             leftTR.Color(inner);
 Vertex leftBR((float)rect.x,(float)(rect.y+rect.h),0.0f);                    leftBR.Color(inner);
 Vertex leftBL((float)(rect.x-radius),(float)(rect.y+rect.h),0.0f);           leftBL.Color(outer);
 Vertex leftTL((float)(rect.x-radius),(float)rect.y,0.0f);                    leftTL.Color(outer); 
 interior.PushQuad(&leftTL,&leftTR,&leftBR,&leftBL);

 //Calculate the top border quad
 Vertex topTL((float)rect.x,(float)(rect.y-radius),0.0f);                     topTL.Color(outer);
 Vertex topTR((float)(rect.w+rect.x),(float)(rect.y-radius),0.0f);            topTR.Color(outer);
 Vertex topBR((float)(rect.w+rect.x),(float)rect.y,0.0f);                     topBR.Color(inner);
 Vertex topBL((float)rect.x,(float)rect.y,0.0f);                              topBL.Color(inner);
 interior.PushQuad(&topTL,&topTR,&topBR,&topBL);

 //Calculate the right border
 Vertex rightTR((float)(rect.x+rect.w+radius),(float)rect.y,0.0f);            rightTR.Color(outer);
 Vertex rightBR((float)(rect.x+rect.w+radius),(float)(rect.y+rect.h),0.0f);   rightBR.Color(outer);
 Vertex rightBL((float)(rect.x+rect.w),(float)(rect.y+rect.h),0.0f);          rightBL.Color(inner);
 Vertex rightTL((float)(rect.x+rect.w),(float)rect.y,0.0f);                   rightTL.Color(inner);
 interior.PushQuad(&rightTL,&rightTR,&rightBR,&rightBL);

 Vertex *v;
 int totalP=precision*4;
 Zdisposable<Vertex> lastv;
 
 //Calculate the bottom right corner
 int i = 0;
 lastv=null;
 for (int j = i; j<=i+(precision); j++ ) {
  circle.Point(iratiod(j,totalP),&point);
  v=new Vertex( BR.x+ (float) point.x , BR.y+ (float) point.y );
  v->TexCoord( (float)point.x / (float)radius, (float)point.y / (float)radius );
  v->Color(outer);
  bp = new Vertex(v); bp->Color(border); borderPoly->addVertex(bp);
  if ( lastv ) interior.PushTriangle(lastv,&BR,v);
  lastv.Recycle(v);
 }
 lastv.Deinstantiate();
 i+=precision.value;

 bp = new Vertex(&bottomBR); bp->Color(border); borderPoly->addVertex(bp);
 bp = new Vertex(&bottomBL); bp->Color(border); borderPoly->addVertex(bp);

 //Calculate the bottom left corner
 lastv=null;
 for ( int j=i; j<=i+precision; j++ ) {
  circle.Point(iratiod(j,totalP),&point);
  v=new Vertex( BL.x+ (float) point.x , BL.y+ (float) point.y );
  v->TexCoord( (float)point.x / (float)radius, (float)point.y / (float)radius );
  v->Color(outer);
  bp = new Vertex(v); bp->Color(border); borderPoly->addVertex(bp);
  if ( lastv ) interior.PushTriangle(lastv,&BL,v);
  lastv.Recycle(v);
 }
 lastv.Deinstantiate();
 i+=precision.value;

 bp = new Vertex(&leftBL); bp->Color(border); borderPoly->addVertex(bp);
 bp = new Vertex(&leftTL); bp->Color(border); borderPoly->addVertex(bp);

 //Calculate the top left corner
 for ( int j=i; j<=i+precision; j++ ) {
  circle.Point(iratiod(j,totalP),&point);
  v=new Vertex( TL.x+ (float) point.x, TL.y+ (float) point.y );
  v->TexCoord( (float)point.x / (float)radius, (float)point.y / (float)radius );
  v->Color(outer);
  bp = new Vertex(v); bp->Color(border); borderPoly->addVertex(bp);
  if ( lastv ) interior.PushTriangle(lastv,&TL,v);
  lastv.Recycle(v);
 }
 lastv.Deinstantiate();
 i+=precision.value;

 bp = new Vertex(&topTL); bp->Color(border); borderPoly->addVertex(bp);
 bp = new Vertex(&topTR); bp->Color(border); borderPoly->addVertex(bp);

 //Calculate the top right corner
 for ( int j=i; j<=i+precision; j++ ) {
  circle.Point(iratiod(j,totalP),&point);
  v=new Vertex( TR.x+ (float) point.x , TR.y+ (float) point.y );
  v->TexCoord( (float)point.x / (float)radius,  (float)point.y / (float)radius );
  v->Color(outer);
  bp = new Vertex(v); bp->Color(border); borderPoly->addVertex(bp);
  if ( lastv ) interior.PushTriangle(lastv,&TR,v);
  lastv.Recycle(v);
 }
 lastv.Deinstantiate();

 bp = new Vertex(&rightTR); bp->Color(border); borderPoly->addVertex(bp);
 bp = new Vertex(&rightBR); bp->Color(border); borderPoly->addVertex(bp);

 interior.Create();
 borderVBO.Polygon(borderPoly);
 borderVBO.Create();
}
void RoundedRectangle::Render() {
 interior.Render();
 if ( lineWidth > 0.0f ) {
  glLineWidth(this->lineWidth.value);
  borderVBO.Render();
  glLineWidth(1.0f);
 }
}
void RoundedRectangle::RenderBorder() {
 if ( lineWidth > 0.0f ) {
  glLineWidth(this->lineWidth.value);
  borderVBO.Render();
  glLineWidth(1.0f);
 }
}
