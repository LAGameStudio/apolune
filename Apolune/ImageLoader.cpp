#include "ImageLoader.h"

#include "FBO.h"

ImageLoader imageLoader;

CPUImage::CPUImage( FBO *f, int fx, int fy, int fw, int fh ) {
 GLubyte *data=f->ReadPixels(gl_color0,fx,fy,fw,fh);
 Crayon c;
 this->Size(fw,fh);
 for ( int _x=fx; _x<(fx+fw); _x++ ) {
  for ( int _y=fy; _y<(fy+fh); _y++ ) {
//   f->ReadPixel(gl_color0,_x,_y);
   c.Float((float)data[(_x+_y*fw)*4+0]/255.0f,(float)data[(_x+_y*fw)*4+1]/255.0f,(float)data[(_x+_y*fw)*4+2]/255.0f,(float)data[(_x+_y*fw)*4+3]/255.0f);
   this->SetPixel((unsigned)_x,(unsigned)_y,&c);
  }
 }
 delete[] data;
}

#include "Vertexd.h"

CPUImage *CPUImage::HeightToNormalMap() {
  CPUImage *out=new CPUImage(w,h);
  if ( h == 1 ) {
   out->Copy(this);
   return out;
  }
  Crayon p6,p7,p8,
         p3,p4,p5,
         p0,p1,p2;
  double s0=0.0,s1=0.0,s2=0.0,s3=0.0,s4=0.0,s5=0.0,s6=0.0,s7=0.0,s8=0.0;
  for ( int i=0; i<w; i++ ) {
   for ( int j=0; j<h; j++ ) {
    if ( WITHIN(i,  j,  0,0,w,h) ) GetPixel(i,  j,  &p4); else { /* last value */ }
    if ( WITHIN(i-1,j-1,0,0,w,h) ) GetPixel(i-1,j-1,&p0); else p0.fromCrayon(p4);
    if ( WITHIN(i,  j-1,0,0,w,h) ) GetPixel(i,  j-1,&p1); else p1.fromCrayon(p4);
    if ( WITHIN(i+1,j-1,0,0,w,h) ) GetPixel(i+1,j-1,&p2); else p2.fromCrayon(p4);
    if ( WITHIN(i-1,j,  0,0,w,h) ) GetPixel(i-1,j,  &p3); else p3.fromCrayon(p4);
    if ( WITHIN(i+1,j,  0,0,w,h) ) GetPixel(i+1,j,  &p5); else p5.fromCrayon(p4);
    if ( WITHIN(i-1,j+1,0,0,w,h) ) GetPixel(i-1,j+1,&p6); else p6.fromCrayon(p4);
    if ( WITHIN(i,  j+1,0,0,w,h) ) GetPixel(i,  j+1,&p7); else p7.fromCrayon(p4);
    if ( WITHIN(i+1,j+1,0,0,w,h) ) GetPixel(i+1,j+1,&p8); else p8.fromCrayon(p4);
    s0=(p0.rd+p0.gd+p0.bd)/3.0;
    s1=(p1.rd+p1.gd+p1.bd)/3.0;
    s2=(p2.rd+p2.gd+p2.bd)/3.0;
    s3=(p3.rd+p3.gd+p3.bd)/3.0;
    s4=(p4.rd+p4.gd+p4.bd)/3.0;
    s5=(p5.rd+p5.gd+p5.bd)/3.0;
    s6=(p6.rd+p6.gd+p6.bd)/3.0;
    s7=(p7.rd+p7.gd+p7.bd)/3.0;
    s8=(p8.rd+p8.gd+p8.bd)/3.0;
    Crayon pix;
    pix.Double(
     -(s2-s0+2.0*(s5-s3)+s8-s6),
     -(s6-s0+2.0*(s7-s1)+s8-s2),
     1.0,
     p4.a
    );
    out->SetPixel(i,h-j-1,&pix);
   }
  }
  return out;
}