#include "PixelUtils.h"

#include "GLImage.h"

inline void G_Pixel( Bitmap s, int x, int y, ColorRGBA c ) {
 ((ColorRGBA *)s.picture)[s.stride * y + x] = c;
}

inline void G_PixelRGBA( Bitmap s, int x, int y, int r, int g, int b, int a ) {
 ColorRGBA c { (byte) r, (byte) b, (byte) g, (byte) a };
 ((ColorRGBA *)s.picture)[s.stride * y + x] = c;
}

inline void G_PixelRGB( Bitmap s, int x, int y, int r, int g, int b ) {
 ColorRGBA c { (byte) r, (byte) g, (byte) b, 255 };
 ((ColorRGBA *)s.picture)[s.stride * y + x] = c;
}

void G_Draw2StepLine32( Bitmap s, int x0, int yy0, int x1, int yy1, ColorRGBA c ) {
 int dy = yy1 - yy0;
 int dx = x1 - x0;
 int stepx, stepy; int i;

 if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
 if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }

 G_Pixel(s, x0, yy0,c);
 G_Pixel(s, x1, yy1,c);
 if (dx > dy) {
  int length = (dx - 1) >> 2;
  int extras = (dx - 1) & 3;
  int incr2 = (dy << 2) - (dx << 1);

  if (incr2 < 0) {
   int c = dy << 1;
   int incr1 = c << 1;
   int d =  incr1 - dx;
   for (i = 0; i < length; i++) {
    x0 += stepx;
    x1 -= stepx;
    if (d < 0) {                                // Pattern:
     G_Pixel(s, x0, yy0, c);                    //
     G_Pixel(s, x0 += stepx, yy0,c);            //  x o o
     G_Pixel(s, x1, yy1,c);                     //
     G_Pixel(s, x1 -= stepx, yy1,c);
     d += incr1;
    } else {
     if (d < c) {                               // Pattern:
      G_Pixel(s, x0, yy0,c);                    //      o
      G_Pixel(s, x0 += stepx, yy0 += stepy,c);  //  x o
      G_Pixel(s, x1, yy1,c);                    //
      G_Pixel(s, x1 -= stepx, yy1 -= stepy,c);
     } else {
      G_Pixel(s, x0, yy0 += stepy,c);           // Pattern:
      G_Pixel(s, x0 += stepx, yy0,c);           //    o o 
      G_Pixel(s, x1, yy1 -= stepy,c);           //  x
      G_Pixel(s, x1 -= stepx, yy1,c);           //
     }
     d += incr2;
    }
   }

   if (extras > 0) {
    if (d < 0) {
     G_Pixel(s, x0 += stepx, yy0,c);
     if (extras > 1) G_Pixel(s, x0 += stepx, yy0,c);
     if (extras > 2) G_Pixel(s, x1 -= stepx, yy1,c);
    } else
     if (d < c) {
      G_Pixel(s, x0 += stepx, yy0,c);
      if (extras > 1) G_Pixel(s, x0 += stepx, yy0 += stepy,c);
      if (extras > 2) G_Pixel(s, x1 -= stepx, yy1,c);
     } else {
      G_Pixel(s, x0 += stepx, yy0 += stepy,c);
      if (extras > 1) G_Pixel(s, x0 += stepx, yy0,c);
      if (extras > 2) G_Pixel(s, x1 -= stepx, yy1 -= stepy,c);
     }
   }
  } else {
   int c = (dy - dx) << 1;
   int incr1 = c << 1;
   int d =  incr1 + dx;
   for (i = 0; i < length; i++) {
    x0 += stepx;
    x1 -= stepx;
    if (d > 0) {
     G_Pixel(s, x0, yy0 += stepy,c);                  // Pattern:
     G_Pixel(s, x0 += stepx, yy0 += stepy,c);         //      o
     G_Pixel(s, x1, yy1 -= stepy,c);                  //    o
     G_Pixel(s, x1 -= stepx, yy1 -= stepy,c);	      //  x
     d += incr1;
    } else {
     if (d < c) {
      G_Pixel(s, x0, yy0,c);                          // Pattern:
      G_Pixel(s, x0 += stepx, yy0 += stepy,c);        //      o
      G_Pixel(s, x1, yy1,c);                          //  x o
      G_Pixel(s, x1 -= stepx, yy1 -= stepy,c);        //
     } else {
      G_Pixel(s, x0, yy0 += stepy,c);                 // Pattern:
      G_Pixel(s, x0 += stepx, yy0,c);                 //    o o
      G_Pixel(s, x1, yy1 -= stepy,c);                 //  x
      G_Pixel(s, x1 -= stepx, yy1,c);                 //
     }
     d += incr2;
    }
   }
    
   if (extras > 0) {
    if (d > 0) {
     G_Pixel(s, x0 += stepx, yy0 += stepy,c);
     if (extras > 1) G_Pixel(s, x0 += stepx, yy0 += stepy,c);
     if (extras > 2) G_Pixel(s, x1 -= stepx, yy1 -= stepy,c);
    } else if (d < c) {
     G_Pixel(s, x0 += stepx, yy0,c);
     if (extras > 1) G_Pixel(s, x0 += stepx, yy0 += stepy,c);
     if (extras > 2) G_Pixel(s, x1 -= stepx, yy1,c);
    } else {
     G_Pixel(s, x0 += stepx, yy0 += stepy,c);
     if (extras > 1) G_Pixel(s, x0 += stepx, yy0,c);
     if (extras > 2) {
      if (d > c) G_Pixel(s, x1 -= stepx, yy1 -= stepy,c);
      else       G_Pixel(s, x1 -= stepx, yy1,c);
     }
    }
   }
  }
 } else {
  int length = (dy - 1) >> 2;
  int extras = (dy - 1) & 3;
  int incr2 = (dx << 2) - (dy << 1);
  if (incr2 < 0) {
   int c = dx << 1;
   int incr1 = c << 1;
   int d =  incr1 - dy;
   for (i = 0; i < length; i++) {
    yy0 += stepy;
    yy1 -= stepy;
    if (d < 0) {
     G_Pixel(s, x0, yy0,c);
     G_Pixel(s, x0, yy0 += stepy,c);
     G_Pixel(s, x1, yy1,c);
     G_Pixel(s, x1, yy1 -= stepy,c);
     d += incr1;
    } else {
     if (d < c) {
      G_Pixel(s, x0, yy0,c);
      G_Pixel(s, x0 += stepx, yy0 += stepy,c);
      G_Pixel(s, x1, yy1,c);
      G_Pixel(s, x1 -= stepx, yy1 -= stepy,c);
     } else {
      G_Pixel(s, x0 += stepx, yy0,c);
      G_Pixel(s, x0, yy0 += stepy,c);
      G_Pixel(s, x1 -= stepx, yy1,c);
      G_Pixel(s, x1, yy1 -= stepy,c);
     }
     d += incr2;
    }
   }
   if (extras > 0) {
    if (d < 0) {
     G_Pixel(s, x0, yy0 += stepy,c);
     if (extras > 1) G_Pixel(s, x0, yy0 += stepy,c);
     if (extras > 2) G_Pixel(s, x1, yy1 -= stepy,c);
    } else if (d < c) {
     G_Pixel(s, stepx, yy0 += stepy,c);
     if (extras > 1) G_Pixel(s, x0 += stepx, yy0 += stepy,c);
     if (extras > 2) G_Pixel(s, x1, yy1 -= stepy,c);
    } else {
     G_Pixel(s, x0 += stepx, yy0 += stepy,c);
     if (extras > 1) G_Pixel(s, x0, yy0 += stepy,c);
     if (extras > 2) G_Pixel(s, x1 -= stepx, yy1 -= stepy,c);
    }
   }
  } else {
   int c = (dx - dy) << 1;
   int incr1 = c << 1;
   int d =  incr1 + dy;
   for (i = 0; i < length; i++) {
    yy0 += stepy;
    yy1 -= stepy;
    if (d > 0) {
     G_Pixel(s, x0 += stepx, yy0,c);
     G_Pixel(s, x0 += stepx, yy0 += stepy,c);
     G_Pixel(s, x1 -= stepy, yy1,c);
     G_Pixel(s, x1 -= stepx, yy1 -= stepy,c);
     d += incr1;
    } else {
     if (d < c) {
      G_Pixel(s, x0, yy0,c);
      G_Pixel(s, x0 += stepx, yy0 += stepy,c);
      G_Pixel(s, x1, yy1,c);
      G_Pixel(s, x1 -= stepx, yy1 -= stepy,c);
     } else {
      G_Pixel(s, x0 += stepx, yy0,c);
      G_Pixel(s, x0, yy0 += stepy,c);
      G_Pixel(s, x1 -= stepx, yy1,c);
      G_Pixel(s, x1, yy1 -= stepy,c);
     }
     d += incr2;
    }
   }
   if (extras > 0) {
    if (d > 0) {
     G_Pixel(s, x0 += stepx, yy0 += stepy,c);
     if (extras > 1) G_Pixel(s, x0 += stepx, yy0 += stepy,c);
     if (extras > 2) G_Pixel(s, x1 -= stepx, yy1 -= stepy,c);
    } else if (d < c) {
     G_Pixel(s, x0, yy0 += stepy,c);
     if (extras > 1) G_Pixel(s, x0 += stepx, yy0 += stepy,c);
     if (extras > 2) G_Pixel(s, x1, yy1 -= stepy,c);
    } else {
     G_Pixel(s, x0 += stepx, yy0 += stepy,c);
     if (extras > 1) G_Pixel(s, x0, yy0 += stepy,c);
     if (extras > 2) {
      if (d > c) G_Pixel(s, x1 -= stepx, yy1 -= stepy,c);
      else       G_Pixel(s, x1, yy1 -= stepy,c);
     }
    }
   }
  }
 }
}


void arrowhead_up( Bitmap s, int sx, int sy, int h, ColorRGBA c ) {
 int xx;
 for ( xx=h; xx>=0; xx-- ) hline(s, URANGE(0,sx-xx,s.width), URANGE(0,sy+xx,s.height), URANGE(0,sx+xx,s.width), c);   
}

void arrowhead_down( Bitmap s, int sx, int sy, int h, ColorRGBA c ) {
 int xx;
 for ( xx=0; xx<h; xx++ ) hline(s, URANGE(0,sx-(h-xx),s.width), URANGE(0,sy+xx,s.height), URANGE(0,sx+(h-xx),s.width), c);   
}

void arrowhead_left( Bitmap s, int sx, int sy, int w, ColorRGBA c ) {
 int xx;
 for ( xx=w; xx>=0; xx-- ) vline(s, URANGE(0,sx+xx,s.width), URANGE(0,sy-xx,s.height), URANGE(0,sy+xx,s.height), c);   
}

void arrowhead_right( Bitmap s, int sx, int sy, int w, ColorRGBA c ) {
 int xx;
 for ( xx=0; xx<w; xx++ ) vline(s, URANGE(0,sx+xx,s.width), URANGE(0,sy-(w-xx),s.height), URANGE(0,sy+(w-xx),s.height), c);   
}

/*
 * Draws a horizontal line of a single color.
 * Expects sx2 >= sx
 */
void hline(Bitmap s, int sx, int sy, int sx2, ColorRGBA c ) {
 int x;
 int wm1=s.width-1; int hm1=s.height-1;
 sy=s.stride * IWRAP(sy,0,hm1);
 for ( x=sx; x<=sx2; x++ ) ((ColorRGBA *)s.picture)[sy + IWRAP(x,0,wm1)] = c;
}

/*
 * Draws a vertical line of a single color.
 * Expects sy2 >= sy
 */
void vline(Bitmap s, int sx, int sy, int sy2, ColorRGBA c ) {
 int y;
 int wm1=s.width-1; int hm1=s.height-1;
 sx=IWRAP(sx,0,wm1);
 for ( y=sy; y<=sy2; y++ ) ((ColorRGBA *)s.picture)[IWRAP(y,0,hm1)*s.stride+sx]=c;
}

/* wrapped 8-way symmetric circle drawing */
void circle(Bitmap s, int xCenter, int yCenter, int radius, ColorRGBA c ) {
 int x, y, r2;
 int r,g,b,a;
 r=c.r; g=c.g; b=c.b; a=c.a;
 r2 = radius * radius;
 G_Pixel(s, xCenter, yCenter + radius, c);
 G_Pixel(s, xCenter, yCenter - radius, c);
 G_Pixel(s, xCenter + radius, yCenter, c);
 G_Pixel(s, xCenter - radius, yCenter, c);

 y = radius;
 x = 1;
 y = (int) (sqrt(r2 - 1) + 0.5);
 while (x < y) {
  int X_xx,X_xy,X_xmx,X_xmy;
  int Y_yx,Y_yy,Y_ymx,Y_ymy;
  X_xx =IWRAP(xCenter+x,0,s.width-1);
  X_xy =IWRAP(xCenter+y,0,s.width-1);
  X_xmx=IWRAP(xCenter-x,0,s.width-1);
  X_xmy=IWRAP(xCenter-y,0,s.width-1);
  Y_yx =IWRAP(yCenter+x,0,s.height-1);
  Y_yy =IWRAP(yCenter+y,0,s.height-1);
  Y_ymx=IWRAP(yCenter-x,0,s.height-1);
  Y_ymy=IWRAP(yCenter-y,0,s.height-1);
  G_Pixel(s, X_xx,  Y_yy,  c);
  G_Pixel(s, X_xx,  Y_ymy, c);
  G_Pixel(s, X_xmx, Y_yy,  c);
  G_Pixel(s, X_xmx, Y_ymy, c);
  G_Pixel(s, X_xy,  Y_yx,  c);
  G_Pixel(s, X_xy,  Y_ymx, c);
  G_Pixel(s, X_xmy, Y_yx,  c);
  G_Pixel(s, X_xmy, Y_ymx, c);
  if ( x < s.width ) x += 1;
  y = Min((int)(sqrt(r2 - x*x) + 0.5),s.height);
 }

 if (x == y) {
  int X_xx,X_xy,X_xmx,X_xmy;
  int Y_yx,Y_yy,Y_ymx,Y_ymy;
  X_xx =IWRAP(xCenter+x,0,s.width-1);
  X_xmx=IWRAP(xCenter-x,0,s.width-1);
  Y_yy =IWRAP(yCenter+y,0,s.height-1);
  Y_ymy=IWRAP(yCenter-y,0,s.height-1);
  G_Pixel(s, X_xx,  Y_yy,  c);
  G_Pixel(s, X_xx,  Y_ymy, c);
  G_Pixel(s, X_xmx, Y_yy,  c);
  G_Pixel(s, X_xmx, Y_ymy, c);
 }
}

/* 8-way symmetric circle drawing (filled)
 * uses hline (above)
 * slightly inefficient (rewrites a couple of regions)
 */
void circleFilled(Bitmap s, int xCenter, int yCenter, int radius, ColorRGBA c) {
 int x, y, r2;
 int wm1=s.width-1;
 int hm1=s.height-1;

 r2 = radius * radius;
        
 vline(s, xCenter, yCenter - radius, yCenter + radius, c);
 // hline(s, xCenter - radius, xCenter + radius, yCenter, c);
 y = radius;
 x = 1;
 y = (int) (sqrt(r2 - 1) + 0.5);
 while (x < y) {
  int X_xmx = Max(xCenter-x,0);
  int Y_ymy = Max(yCenter-y,0); 
  int yy=yCenter+y;
  int yx=yCenter+x;
  vline(s, Min(xCenter + x,wm1), Y_ymy,                yy, c);
  vline(s, X_xmx,                 Max(yCenter - y, 0), yy, c);
  vline(s, Min(xCenter + y,wm1), Y_ymy,                yx, c);
  vline(s, Max(xCenter - y,  0), Max(yCenter - x, 0), yx, c);
  x += 1;
  y = (int) (sqrt(r2 - x*x) + 0.5);
 }

 if (x == y) {
  int yy=yCenter+y;
  int Y_ymy=Max(yCenter-y,0);
  vline(s, Min(xCenter + x,wm1), Y_ymy, yy, c);
  vline(s, Max(xCenter - x,  0), Y_ymy, yy, c);
 }
}

#if defined(NEVER)
/* Draw rectangle with alpha enabled from RGBA color. */

void G_FillRectRGBA(AG_Widget * dst, Sint16 x1, Sint16 yy1, Sint16 x2, Sint16 y2, 
     Uint8 r, Uint8 g, Uint8 b, Uint8 a, AG_BlendFn blendFn )
{
    Uint8 c[4];

    c[0]=r;
    c[1]=g;
    c[2]=b;
    c[3]=a;

   AG_DrawRectBlended( dst, AG_RECT(x1,yy1,x2,y2), c, blendFn);
//   for ( y=yy1; y<=y2; y++ )
//   for ( x=x1; x<=x2; x++ ) AG_WidgetBlendPixelRGBA(dst,x,y,c,blendFn);
}

/* Draw rectangle with alpha enabled from RGBA color. */

void G_SurfaceFillRectRGBA(Bitmap  s, int x1, int yy1, int x2, int y2, 
     Uint8 r, Uint8 g, Uint8 b, Uint8 a, AG_BlendFn blendFn )
{
   int x,y;
   for ( x=x1; x<x2; x++ ) 
    for ( y=yy1; y<y2; y++ ) 
    AG_BLEND_RGBA2( s, x,y, r,g,b,a, blendFn );    
}


/* Draw horizontal line with alpha enabled from RGBA color */

int HLineAlpha(SDL_Surface * dst, Sint16 x1, Sint16 x2, Sint16 y, Uint32 color)
{
    return (filledRectAlpha(dst, x1, y, x2, y, color));
}


/* Draw vertical line with alpha enabled from RGBA color */

int VLineAlpha(SDL_Surface * dst, Sint16 x, Sint16 yy1, Sint16 y2, Uint32 color)
{
    return (filledRectAlpha(dst, x, yy1, x, y2, color));
}


/* Pixel - using alpha weight on color for AA-drawing */

int pixelColorWeight(SDL_Surface * dst, Sint16 x, Sint16 y, Uint32 color, Uint32 weight)
{
    Uint32 a;

    /*
     * Get alpha 
     */
    a = (color & (Uint32) 0x000000ff);

    /*
     * Modify Alpha by weight 
     */
    a = ((a * weight) >> 8);

    return (pixelColor(dst, x, y, (color & (Uint32) 0xffffff00) | (Uint32) a));
}

/* AA Line */

#define AAlevels 256
#define AAbits 8

/* 

This implementation of the Wu antialiasing code is based on Mike Abrash's
DDJ article which was reprinted as Chapter 42 of his Graphics Programming
Black Book, but has been optimized to work with SDL and utilizes 32-bit
fixed-point arithmetic. (A. Schiffler).

*/

int aalineColorInt(Bitmap dst, Sint16 x1, int yy1, int x2, int y2, ColorRGBA color, int draw_endpoint)
{
 signed short xx0, yy0, xx1, yyy1, intshift, erracc, erradj, erracctmp, wgt, wgtcompmask, dx, dy, tmp, xdir, y0p1, x0pxdir;

    /*
     * Clip line and test if we have to draw 
     */
 if (!(clipLine(dst, &x1, &yy1, &x2, &y2))) return (0);

    /*
     * Keep on working with 32bit numbers 
     */
 xx0 = x1;
 yy0 = yy1;
 xx1 = x2;
 yyy1 = y2;

    /*
     * Reorder points if required 
     */
 if (yy0 > yyy1) {
  tmp = yy0;
  yy0 = yyy1;
  yyy1 = tmp;
  tmp = xx0;
  xx0 = xx1;
  xx1 = tmp;
 }

    /*
     * Calculate distance 
     */
 dx = xx1 - xx0;
 dy = yyy1 - yy0;

    /*
     * Adjust for negative dx and set xdir 
     */
 if (dx >= 0) xdir = 1;
 else {
  xdir = -1;
  dx = (-dx);
 }

    /*
     * Check for special cases 
     */
 if (dx == 0) {
	/*
	 * Vertical line 
	 */
  return (vlineColor(dst, x1, yy1, y2, color));
 } else if (dy == 0) {
	/*
	 * Horizontal line 
	 */
  return (hlineColor(dst, x1, x2, yy1, color));
 } else if (dx == dy) {
	/*
	 * Diagonal line 
	 */
  return (lineColor(dst, x1, yy1, x2, y2, color));
 }

    /*
     * Line is not horizontal, vertical or diagonal 
     */
 result = 0;

    /*
     * Zero accumulator 
     */
 erracc = 0;

    /*
     * # of bits by which to shift erracc to get intensity level 
     */
 intshift = 32 - AAbits;
    /*
     * Mask used to flip all bits in an intensity weighting 
     */it
 wgtcompmask = AAlevels - 1;

    /*
     * Draw the initial pixel in the foreground color 
     */
 result |= pixelColorNolock(dst, x1, yy1, color);

    /*
     * x-major or y-major? 
     */
 if (dy > dx) {

	/*
	 * y-major.  Calculate 16-bit fixed point fractional part of a pixel that
	 * X advances every time Y advances 1 pixel, truncating the result so that
	 * we won't overrun the endpoint along the X axis 
	 */
	/*
	 * Not-so-portable version: erradj = ((Uint64)dx << 32) / (Uint64)dy; 
	 */
	erradj = ((dx << 16) / dy) << 16;

	/*
	 * draw all pixels other than the first and last 
	 */
	x0pxdir = xx0 + xdir;
	while (--dy) {
	    erracctmp = erracc;
	    erracc += erradj;
	    if (erracc <= erracctmp) {
		/*
		 * rollover in error accumulator, x coord advances 
		 */
		xx0 = x0pxdir;
		x0pxdir += xdir;
	    }
	    yy0++;		/* y-major so always advance Y */

	    /*
	     * the AAbits most significant bits of erracc give us the intensity
	     * weighting for this pixel, and the complement of the weighting for
	     * the paired pixel. 
	     */
	    wgt = (erracc >> intshift) & 255;
	    G_Pixel (dst, xx0, yy0, color, 255 - wgt);
	    G_Pixel (dst, x0pxdir, yy0, color, wgt);
	}

 } else {

	/*
	 * x-major line.  Calculate 16-bit fixed-point fractional part of a pixel
	 * that Y advances each time X advances 1 pixel, truncating the result so
	 * that we won't overrun the endpoint along the X axis. 
	 */
	/*
	 * Not-so-portable version: erradj = ((Uint64)dy << 32) / (Uint64)dx; 
	 */
	erradj = ((dy << 16) / dx) << 16;

	/*
	 * draw all pixels other than the first and last 
	 */
	y0p1 = yy0 + 1;
	while (--dx) {

	    erracctmp = erracc;
	    erracc += erradj;
	    if (erracc <= erracctmp) {
		/*
		 * Accumulator turned over, advance y 
		 */
		yy0 = y0p1;
		y0p1++;
	    }
	    xx0 += xdir;	/* x-major so always advance X */
	    /*
	     * the AAbits most significant bits of erracc give us the intensity
	     * weighting for this pixel, and the complement of the weighting for
	     * the paired pixel. 
	     */
	    wgt = (erracc >> intshift) & 255;
	    G_Pixel (dst, xx0, yy0, color, 255 - wgt);
	    G_Pixel (dst, xx0, y0p1, color, wgt);
	}
 }

    /*
     * Do we have to draw the endpoint 
     */
 if (draw_endpoint) {
	/*
	 * Draw final pixel, always exactly intersected by the line and doesn't
	 * need to be weighted. 
	 */
	G_Pixel (dst, x2, y2, color);
 }

 return (result);
}

int aalineColor(Bitmap dst, Sint16 x1, Sint16 yy1, Sint16 x2, Sint16 y2, ColorRGBA color)
{    return aalineColorInt(dst, x1, yy1, x2, y2, color, 1);  }

int aalineRGBA(Bitmap dst, Sint16 x1, Sint16 yy1, Sint16 x2, Sint16 y2, int r, int g, int b, int a)
{    return aalineColorInt(dst, x1, yy1, x2, y2, ColorRGBA { (byte) r, (byte) g, (byte) b, (byte) a } );  }


/* ----- AA Circle */

/* AA circle is based on AAellipse  */

int aacircleColor(Bitmap dst, Sint16 x, Sint16 y, Sint16 r, Uint32 color)
{    return aaellipseColor(dst, x, y, r, r, color);  }

int aacircleRGBA(Bitmap dst, int x, int y, int rad, int r, int g, int b, int a)
{
    /*
     * Draw 
     */
    return aaellipseColor(dst, x, y, rad, rad, ColorRGBA { (byte) r, (byte) g, (byte) b, (byte) a } );
}


/* ----- Ellipse */

/* Note: Based on algorithm from sge library with multiple-hline draw removal */
/* and other speedup changes. */

int ellipseColor(Bitmap dst, int x, int y, int rx, int ry, ColorRGBA color)
{
    signed short left, right, top, bottom;
    signed short x1, yy1, x2, y2;
    int ix, iy;
    signed short h, i, j, k;
    int oh, oi, oj, ok;
    int xmh, xph, ypk, ymk;
    int xmi, xpi, ymj, ypj;
    int xmj, xpj, ymi, ypi;
    int xmk, xpk, ymh, yph;
    /*
     * Sanity check radii 
     */
    if ((rx < 0) || (ry < 0)) return (-1);

    /*
     * Special case for rx=0 - draw a vline 
     */
    if (rx == 0) return (vline(dst, x, y - ry, y + ry, color));
    /*
     * Special case for ry=0 - draw a hline 
     */
    if (ry == 0) return (hline(dst, x - rx, x + rx, y, color));

    /*
     * Get clipping boundary 
     */
    left = dst.clip_rect.x;
    right = dst.clip_rect.x + dst.clip_rect.w - 1;
    top = dst.clip_rect.y;
    bottom = dst.clip_rect.y + dst.clip_rect.h - 1;

    /*
     * Test if bounding box of ellipse is visible 
     */
    x1 = x - rx;
    x2 = x + rx;
    yy1 = y - ry;
    y2 = y + ry;
    if ((x1<left) && (x2<left)) return(0);
    if ((x1>right) && (x2>right)) return(0);
    if ((yy1<top) && (y2<top)) return(0);
    if ((yy1>bottom) && (y2>bottom)) return(0);

    /*
     * Init vars 
     */
    oh = oi = oj = ok = 0xFFFF;

    /*
     * Draw 
     */
    result = 0;

	if (rx > ry) {
	    ix = 0;
	    iy = rx * 64;

	    do {
		h = (ix + 32) >> 6;
		i = (iy + 32) >> 6;
		j = (h * ry) / rx;
		k = (i * ry) / rx;

		if (((ok != k) && (oj != k)) || ((oj != j) && (ok != j)) || (k != j)) {
		    xph = x + h;
		    xmh = x - h;
		    if (k > 0) {
			ypk = y + k;
			ymk = y - k;
			G_Pixel(dst, xmh, ypk, color);
			G_Pixel(dst, xph, ypk, color);
			G_Pixel(dst, xmh, ymk, color);
			G_Pixel(dst, xph, ymk, color);
		    } else {
			G_Pixel(dst, xmh, y, color);
			G_Pixel(dst, xph, y, color);
		    }
		    ok = k;
		    xpi = x + i;
		    xmi = x - i;
		    if (j > 0) {
			ypj = y + j;
			ymj = y - j;
			G_Pixel(dst, xmi, ypj, color);
			G_Pixel(dst, xpi, ypj, color);
			G_Pixel(dst, xmi, ymj, color);
			G_Pixel(dst, xpi, ymj, color);
		    } else {
			G_Pixel(dst, xmi, y, color);
			G_Pixel(dst, xpi, y, color);
		    }
		    oj = j;
		}

		ix = ix + iy / rx;
		iy = iy - ix / rx;

	    } while (i > h);
	} else {
	    ix = 0;
	    iy = ry * 64;

	    do {
		h = (ix + 32) >> 6;
		i = (iy + 32) >> 6;
		j = (h * rx) / ry;
		k = (i * rx) / ry;

		if (((oi != i) && (oh != i)) || ((oh != h) && (oi != h) && (i != h))) {
		    xmj = x - j;
		    xpj = x + j;
		    if (i > 0) {
			ypi = y + i;
			ymi = y - i;
			G_Pixel(dst, xmj, ypi, color);
			G_Pixel(dst, xpj, ypi, color);
			G_Pixel(dst, xmj, ymi, color);
			G_Pixel(dst, xpj, ymi, color);
		    } else {
			G_Pixel(dst, xmj, y, color);
			G_Pixel(dst, xpj, y, color);
		    }
		    oi = i;
		    xmk = x - k;
		    xpk = x + k;
		    if (h > 0) {
			yph = y + h;
			ymh = y - h;
			G_Pixel(dst, xmk, yph, color);
			G_Pixel(dst, xpk, yph, color);
			G_Pixel(dst, xmk, ymh, color);
			G_Pixel(dst, xpk, ymh, color);
		    } else {
			G_Pixel(dst, xmk, y, color);
			G_Pixel(dst, xpk, y, color);
		    }
		    oh = h;
		}

		ix = ix + iy / ry;
		iy = iy - ix / ry;

	    } while (i > h);
	}

    } else {

	if (rx > ry) {
	    ix = 0;
	    iy = rx * 64;

	    do {
		h = (ix + 32) >> 6;
		i = (iy + 32) >> 6;
		j = (h * ry) / rx;
		k = (i * ry) / rx;

		if (((ok != k) && (oj != k)) || ((oj != j) && (ok != j)) || (k != j)) {
		    xph = x + h;
		    xmh = x - h;
		    if (k > 0) {
			ypk = y + k;
			ymk = y - k;
			G_Pixel(dst, xmh, ypk, color);
			G_Pixel(dst, xph, ypk, color);
			G_Pixel(dst, xmh, ymk, color);
			G_Pixel(dst, xph, ymk, color);
		    } else {
			G_Pixel(dst, xmh, y, color);
			G_Pixel(dst, xph, y, color);
		    }
		    ok = k;
		    xpi = x + i;
		    xmi = x - i;
		    if (j > 0) {
			ypj = y + j;
			ymj = y - j;
			G_Pixel(dst, xmi, ypj, color);
			G_Pixel(dst, xpi, ypj, color);
			G_Pixel(dst, xmi, ymj, color);
			G_Pixel(dst, xpi, ymj, color);
		    } else {
			G_Pixel(dst, xmi, y, color);
			G_Pixel(dst, xpi, y, color);
		    }
		    oj = j;
		}

		ix = ix + iy / rx;
		iy = iy - ix / rx;

	    } while (i > h);
	} else {
	    ix = 0;
	    iy = ry * 64;

	    do {
		h = (ix + 32) >> 6;
		i = (iy + 32) >> 6;
		j = (h * rx) / ry;
		k = (i * rx) / ry;

		if (((oi != i) && (oh != i)) || ((oh != h) && (oi != h) && (i != h))) {
		    xmj = x - j;
		    xpj = x + j;
		    if (i > 0) {
			ypi = y + i;
			ymi = y - i;
			G_Pixel(dst, xmj, ypi, color);
			G_Pixel(dst, xpj, ypi, color);
			G_Pixel(dst, xmj, ymi, color);
			G_Pixel(dst, xpj, ymi, color);
		    } else {
			G_Pixel(dst, xmj, y, color);
			G_Pixel(dst, xpj, y, color);
		    }
		    oi = i;
		    xmk = x - k;
		    xpk = x + k;
		    if (h > 0) {
			yph = y + h;
			ymh = y - h;
			G_Pixel(dst, xmk, yph, color);
			G_Pixel(dst, xpk, yph, color);
			G_Pixel(dst, xmk, ymh, color);
			G_Pixel(dst, xpk, ymh, color);
		    } else {
			G_Pixel(dst, xmk, y, color);
			G_Pixel(dst, xpk, y, color);
		    }
		    oh = h;
		}

		ix = ix + iy / ry;
		iy = iy - ix / ry;

	    } while (i > h);
	}

   return true;
}

int ellipseRGBA(Bitmap dst, int x, int y, int rx, int ry, int r, int g, int b, int a)
{
    /*
     * Draw 
     */
    return ellipseColor(dst, x, y, rx, ry, ColorRGBA { (byte) r, (byte) g, (byte) b, (byte) a });
}

/* ----- AA Ellipse */

/* Based on code from Anders Lindstroem, based on code from SGE, based on code from TwinLib */

int aaellipseColor(Bitmap dst, int xc, int yc, int rx, int ry, ColorRGBA color)
{
    signed short left, right, top, bottom, x1,yy1,x2,y2;
    int i;
    int a2, b2, ds, dt, dxt, t, s, d;
    signed short x, y, xs, ys, dyt, od, xx, yy, xc2, yc2;
    float cp;
    double sab;
    byte weight, iweight;
    int result;

    /*
     * Sanity check radii 
     */
    if ((rx < 0) || (ry < 0)) {
	return (-1);
    }

    /*
     * Special case for rx=0 - draw a vline 
     */
    if (rx == 0) {
	return (vlineColor(dst, xc, yc - ry, yc + ry, color));
    }
    /*
     * Special case for ry=0 - draw a hline 
     */
    if (ry == 0) {
	return (hlineColor(dst, xc - rx, xc + rx, yc, color));
    }

    /*
     * Get clipping boundary 
     */
    left = dst.clip_rect.x;
    right = dst.clip_rect.x + dst.clip_rect.w - 1;
    top = dst.clip_rect.y;
    bottom = dst.clip_rect.y + dst.clip_rect.h - 1;

    /*
     * Test if bounding box of ellipse is visible 
     */
    x1 = xc - rx;
    x2 = xc + rx;
    yy1 = yc - ry;
    y2 = yc + ry;
    if ((x1<left) && (x2<left)) {
     return(0);
    } 
    if ((x1>right) && (x2>right)) {
     return(0);
    } 
    if ((yy1<top) && (y2<top)) {
     return(0);
    } 
    if ((yy1>bottom) && (y2>bottom)) {
     return(0);
    } 
    
    /* Variable setup */
    a2 = rx * rx;
    b2 = ry * ry;

    ds = 2 * a2;
    dt = 2 * b2;

    xc2 = 2 * xc;
    yc2 = 2 * yc;

    sab = sqrt(a2 + b2);
    od = lrint(sab*0.01) + 1; /* introduce some overdraw */
    dxt = lrint((double)a2 / sab) + od;

    t = 0;
    s = -2 * a2 * ry;
    d = 0;

    x = xc;
    y = yc - ry;

    /* Draw */
    result = 0;

    /* "End points" */
    G_Pixel(dst, x, y, color);
    G_Pixel(dst, xc2 - x, y, color);
    G_Pixel(dst, x, yc2 - y, color);
    G_Pixel(dst, xc2 - x, yc2 - y, color);

    for (i = 1; i <= dxt; i++) {
	x--;
	d += t - b2;

	if (d >= 0)
	    ys = y - 1;
	else if ((d - s - a2) > 0) {
	    if ((2 * d - s - a2) >= 0)
		ys = y + 1;
	    else {
		ys = y;
		y++;
		d -= s + a2;
		s += ds;
	    }
	} else {
	    y++;
	    ys = y + 1;
	    d -= s + a2;
	    s += ds;
	}

	t -= dt;

	/* Calculate alpha */
	if (s != 0.0) {
	    cp = (float) abs(d) / (float) abs(s);
	    if (cp > 1.0) {
		cp = 1.0;
	    }
	} else {
	    cp = 1.0;
	}

	/* Calculate weights */
	weight = (Uint8) (cp * 255);
	iweight = 255 - weight;

	/* Upper half */
	xx = xc2 - x;
	G_PixelWeighted(dst, x, y, color, iweight);
	G_PixelWeighted(dst, xx, y, color, iweight);

	G_PixelWeighted(dst, x, ys, color, weight);
	G_PixelWeighted(dst, xx, ys, color, weight);

	/* Lower half */
	yy = yc2 - y;
	G_PixelWeighted(dst, x, yy, color, iweight);
	G_PixelWeighted(dst, xx, yy, color, iweight);

	yy = yc2 - ys;
	G_PixelWeighted(dst, x, yy, color, weight);
	G_PixelWeighted(dst, xx, yy, color, weight);
    }

    /* Replaces original approximation code dyt = abs(y - yc); */
    dyt = lrint((double)b2 / sab ) + od;    
    
    for (i = 1; i <= dyt; i++) {
	y++;
	d -= s + a2;

	if (d <= 0)
	    xs = x + 1;
	else if ((d + t - b2) < 0) {
	    if ((2 * d + t - b2) <= 0)
		xs = x - 1;
	    else {
		xs = x;
		x--;
		d += t - b2;
		t -= dt;
	    }
	} else {
	    x--;
	    xs = x - 1;
	    d += t - b2;
	    t -= dt;
	}

	s += ds;

	/* Calculate alpha */
	if (t != 0.0) {
	    cp = (float) abs(d) / (float) abs(t);
	    if (cp > 1.0) {
		cp = 1.0;
	    }
	} else {
	    cp = 1.0;
	}

	/* Calculate weight */
	weight = (Uint8) (cp * 255);
	iweight = 255 - weight;

	/* Left half */
	xx = xc2 - x;
	yy = yc2 - y;
	G_PixelWeighted(dst, x, y, color, iweight);
	G_PixelWeighted(dst, xx, y, color, iweight);

	G_PixelWeighted(dst, x, yy, color, iweight);
	G_PixelWeighted(dst, xx, yy, color, iweight);

	/* Right half */
	xx = 2 * xc - xs;
	G_PixelWeighted(dst, xs, y, color, weight);
	G_PixelWeighted(dst, xx, y, color, weight);

	G_PixelWeighted(dst, xs, yy, color, weight);
	G_PixelWeighted(dst, xx, yy, color, weight);

    }
}

int aaellipseRGBA(Bitmap dst, int x, int y, int rx, int ry, int r, int g, int b, int a)
{
    /*
     * Draw 
     */
    return aaellipseColor(dst, x, y, rx, ry, ColorRGBA { (byte) r, (byte) g, (byte) b, (byte) a });
}

/* ---- Filled Ellipse */

/* Note: */
/* Based on algorithm from sge library with multiple-hline draw removal */
/* and other speedup changes. */

bool filledEllipseColor(Bitmap dst, int x, int y, int rx, int ry, ColorRGBA color)
{
    Sint16 left, right, top, bottom;
    int result;
    Sint16 x1, yy1, x2, y2;
    int ix, iy;
    int h, i, j, k;
    int oh, oi, oj, ok;
    int xmh, xph;
    int xmi, xpi;
    int xmj, xpj;
    int xmk, xpk;

    /*
     * Sanity check radii 
     */
    if ((rx < 0) || (ry < 0)) return false;    

    /*
     * Special case for rx=0 - draw a vline 
     */
    if (rx == 0) {
	  vline(dst, x, y - ry, y + ry, color);
     return true;
    }

    /*
     * Special case for ry=0 - draw a hline 
     */
    if (ry == 0) {
	  hline(dst, x - rx, x + rx, y, color);
     return true;
    }

    /*
     * Get clipping boundary 
     */
    left = 0;      right = dst.width - 1;
    top = 1;       bottom = dst.width - 1;

    /*
     * Test if bounding box of ellipse is visible 
     */
    x1 = x - rx;
    x2 = x + rx;
    yy1 = y - ry;
    y2 = y + ry;
    if ((x1<left) && (x2<left)) return false;
    if ((x1>right) && (x2>right)) return false;
    if ((yy1<top) && (y2<top)) return false;
    if ((yy1>bottom) && (y2>bottom)) return false;

    /*
     * Init vars 
     */
    oh = oi = oj = ok = 0xFFFF;

    /*
     * Draw 
     */
    result = 0;
    if (rx > ry) {
	ix = 0;
	iy = rx * 64;

	do {
	    h = (ix + 32) >> 6;
	    i = (iy + 32) >> 6;
	    j = (h * ry) / rx;
	    k = (i * ry) / rx;

	    if ((ok != k) && (oj != k)) {
		xph = x + h;
		xmh = x - h;
		if (k > 0) {
		    hline(dst, xmh, xph, y + k, color);
		    hline(dst, xmh, xph, y - k, color);
		} else {
		    hline(dst, xmh, xph, y, color);
		}
		ok = k;
	    }
	    if ((oj != j) && (ok != j) && (k != j)) {
		xmi = x - i;
		xpi = x + i;
		if (j > 0) {
		    hline(dst, xmi, xpi, y + j, color);
		    hline(dst, xmi, xpi, y - j, color);
		} else {
		    hline(dst, xmi, xpi, y, color);
		}
		oj = j;
	    }

	    ix = ix + iy / rx;
	    iy = iy - ix / rx;

	} while (i > h);
    } else {
	ix = 0;
	iy = ry * 64;

	do {
	    h = (ix + 32) >> 6;
	    i = (iy + 32) >> 6;
	    j = (h * rx) / ry;
	    k = (i * rx) / ry;

	    if ((oi != i) && (oh != i)) {
		xmj = x - j;
		xpj = x + j;
		if (i > 0) {
		    hline(dst, xmj, xpj, y + i, color);
		    hline(dst, xmj, xpj, y - i, color);
		} else {
		    hline(dst, xmj, xpj, y, color);
		}
		oi = i;
	    }
	    if ((oh != h) && (oi != h) && (i != h)) {
		xmk = x - k;
		xpk = x + k;
		if (h > 0) {
		    hline(dst, xmk, xpk, y + h, color);
		    hline(dst, xmk, xpk, y - h, color);
		} else {
		    hline(dst, xmk, xpk, y, color);
		}
		oh = h;
	    }

	    ix = ix + iy / ry;
	    iy = iy - ix / ry;

	} while (i > h);
 }
}


int filledEllipseRGBA(Bitmap dst, int x, int y, int rx, int ry, int r, int g, int b, int a)
{
    /*
     * Draw 
     */
    return filledEllipseColor(dst, x, y, rx, ry, ColorRGBA { (byte) r, (byte) g, (byte) b, (byte) a });
}

/* ----- filled pie */

/* Low-speed float pie-calc implementation by drawing polygons/lines. */

int doPieColor(SDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, Uint32 color, Uint8 filled)
{
    Sint16 left, right, top, bottom;
    Sint16 x1, yy1, x2, y2;
    int result;
    double angle, start_angle, end_angle;
    double deltaAngle;
    double dr;
    int posX, posY;
    int numpoints, i;
    Sint16 *vx, *vy;

    /*
     * Sanity check radii 
     */
    if (rad < 0) {
	return (-1);
    }

    /*
     * Fixup angles
     */
    start = start % 360;
    end = end % 360;

    /*
     * Special case for rad=0 - draw a point 
     */
    if (rad == 0) {
	return (pixelColor(dst, x, y, color));
    }

    /*
     * Get clipping boundary 
     */
    left = dst.clip_rect.x;
    right = dst.clip_rect.x + dst.clip_rect.w - 1;
    top = dst.clip_rect.y;
    bottom = dst.clip_rect.y + dst.clip_rect.h - 1;

    /*
     * Test if bounding box of pie's circle is visible 
     */
    x1 = x - rad;
    x2 = x + rad;
    yy1 = y - rad;
    y2 = y + rad;
    if ((x1<left) && (x2<left)) {
     return(0);
    } 
    if ((x1>right) && (x2>right)) {
     return(0);
    } 
    if ((yy1<top) && (y2<top)) {
     return(0);
    } 
    if ((yy1>bottom) && (y2>bottom)) {
     return(0);
    } 

    /*
     * Variable setup 
     */
    dr = (double) rad;
    deltaAngle = 3.0 / dr;
    start_angle = (double) start *(2.0 * M_PI / 360.0);
    end_angle = (double) end *(2.0 * M_PI / 360.0);
    if (start > end) {
	end_angle += (2.0 * M_PI);
    }

    /* Count points (rather than calculate it) */
    numpoints = 1;
    angle = start_angle;
    while (angle <= end_angle) {
	angle += deltaAngle;
	numpoints++;
    }

    /* Check size of array */
    if (numpoints == 1) {
	return (pixelColor(dst, x, y, color));
    } else if (numpoints == 2) {
	posX = x + (int) (dr * cos(start_angle));
	posY = y + (int) (dr * sin(start_angle));
	return (lineColor(dst, x, y, posX, posY, color));
    }

    /* Allocate vertex array */
    vx = vy = (Sint16 *) malloc(2 * sizeof(Uint16) * numpoints);
    if (vx == null) {
	return (-1);
    }
    vy += numpoints;

    /* Center */
    vx[0] = x;
    vy[0] = y;

    /* Calculate and store vertices */
    i = 1;
    angle = start_angle;
    while (angle <= end_angle) {
	vx[i] = x + (int) (dr * cos(angle));
	vy[i] = y + (int) (dr * sin(angle));
	angle += deltaAngle;
	i++;
    }

    /* Draw */
    if (filled) {
     result = filledPolygonColor(dst, vx, vy, numpoints, color);
    } else {
     result = polygonColor(dst, vx, vy, numpoints, color);
    }

    /* Free vertex array */
    free(vx);

    return (result);
}

int pieColor(SDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, 
		Sint16 start, Sint16 end, Uint32 color) 
{
    return (doPieColor(dst, x, y, rad, start, end, color, 0));

}

int pieRGBA(SDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad,
	    Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return (doPieColor(dst, x, y, rad, start, end,
			   ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a, 0));

}

int filledPieColor(SDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, Uint32 color)
{
    return (doPieColor(dst, x, y, rad, start, end, color, 1));
}

int filledPieRGBA(SDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad,
		  Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return (doPieColor(dst, x, y, rad, start, end,
			   ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a, 1));
}

/* Trigon */

int trigonColor(Bitmap dst, int x1, int yy1, int x2, int y2, int x3, int y3, ColorRGBA color)
{
 int vx[3]; 
 int vy[3];
 
 vx[0]=x1;
 vx[1]=x2;
 vx[2]=x3;
 vy[0]=yy1;
 vy[1]=y2;
 vy[2]=y3;
 
 return polygonColor(dst,vx,vy,3,color);
}

int trigonRGBA(Bitmap dst, int x1, int yy1, int x2, int y2, int x3, int y3,
				 int r, int g, int b, int a)
{
 int vx[3]; 
 int vy[3];
 
 vx[0]=x1;
 vx[1]=x2;
 vx[2]=x3;
 vy[0]=yy1;
 vy[1]=y2;
 vy[2]=y3;
 
 return polygonRGBA(dst,vx,vy,3,r,g,b,a);
}				 

/* AA-Trigon */

int aatrigonColor(Bitmap dst, int x1, int yy1, int x2, int y2, int x3, int y3, ColorRGBA color)
{
 int vx[3]; 
 int vy[3];
 
 vx[0]=x1;
 vx[1]=x2;
 vx[2]=x3;
 vy[0]=yy1;
 vy[1]=y2;
 vy[2]=y3;
 
 return aapolygonColor(dst,vx,vy,3,color);
}

int aatrigonRGBA(SDL_Surface * dst,  Sint16 x1, Sint16 yy1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
				   int r, int g, int b, int a)
{
 int vx[3]; 
 int vy[3];
 
 vx[0]=x1;
 vx[1]=x2;
 vx[2]=x3;
 vy[0]=yy1;
 vy[1]=y2;
 vy[2]=y3;
 
 return aapolygonRGBA(dst,vx,vy,3, ColorRGBA { (byte) r, (byte) g, (byte) b, (byte) a } );
}				   

/* Filled Trigon */

int filledTrigonColor(SDL_Surface * dst, Sint16 x1, Sint16 yy1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color)
{
 Sint16 vx[3]; 
 Sint16 vy[3];
 
 vx[0]=x1;
 vx[1]=x2;
 vx[2]=x3;
 vy[0]=yy1;
 vy[1]=y2;
 vy[2]=y3;
 
 return(filledPolygonColor(dst,vx,vy,3,color));
}

int filledTrigonRGBA(SDL_Surface * dst, Sint16 x1, Sint16 yy1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
				       Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
 Sint16 vx[3]; 
 Sint16 vy[3];
 
 vx[0]=x1;
 vx[1]=x2;
 vx[2]=x3;
 vy[0]=yy1;
 vy[1]=y2;
 vy[2]=y3;
 
 return(filledPolygonRGBA(dst,vx,vy,3,r,g,b,a));
}

/* ---- Polygon */

int polygonColor(SDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color)
{
    int result;
    int i;
    const Sint16 *x1, *yy1, *x2, *y2;

    /*
     * Sanity check 
     */
    if (n < 3) {
	return (-1);
    }

    /*
     * Pointer setup 
     */
    x1 = x2 = vx;
    yy1 = y2 = vy;
    x2++;
    y2++;

    /*
     * Draw 
     */
    result = 0;
    for (i = 1; i < n; i++) {
	result |= lineColor(dst, *x1, *yy1, *x2, *y2, color);
	x1 = x2;
	yy1 = y2;
	x2++;
	y2++;
    }
    result |= lineColor(dst, *x1, *yy1, *vx, *vy, color);

    return (result);
}

int polygonRGBA(SDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    /*
     * Draw 
     */
    return (polygonColor(dst, vx, vy, n, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/* ---- AA-Polygon */

int aapolygonColor(SDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color)
{
    int result;
    int i;
    const Sint16 *x1, *yy1, *x2, *y2;

    /*
     * Sanity check 
     */
    if (n < 3) {
	return (-1);
    }

    /*
     * Pointer setup 
     */
    x1 = x2 = vx;
    yy1 = y2 = vy;
    x2++;
    y2++;

    /*
     * Draw 
     */
    result = 0;
    for (i = 1; i < n; i++) {
	result |= aalineColorInt(dst, *x1, *yy1, *x2, *y2, color, 0);
	x1 = x2;
	yy1 = y2;
	x2++;
	y2++;
    }
    result |= aalineColorInt(dst, *x1, *yy1, *vx, *vy, color, 0);

    return (result);
}

int aapolygonRGBA(SDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    /*
     * Draw 
     */
    return (aapolygonColor(dst, vx, vy, n, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/* ---- Filled Polygon */

int gfxPrimitivesCompareInt(const void *a, const void *b);

static int *gfxPrimitivesPolyInts = null;
static int gfxPrimitivesPolyAllocated = 0;

int filledPolygonColor(SDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color)
{
    int result;
    int i;
    int y, xa, xb;
    int miny, maxy;
    int x1, yy1;
    int x2, y2;
    int ind1, ind2;
    int ints;

    /*
     * Sanity check 
     */
    if (n < 3) {
	return -1;
    }

    /*
     * Allocate temp array, only grow array 
     */
    if (!gfxPrimitivesPolyAllocated) {
	gfxPrimitivesPolyInts = (int *) malloc(sizeof(int) * n);
        if ( gfxPrimitivesPolyInts == null ) { malloc_stats(); printf( "Out of memory error!\n" ); exit(1); }
	gfxPrimitivesPolyAllocated = n;
    } else {
	if (gfxPrimitivesPolyAllocated < n) {
	    gfxPrimitivesPolyInts = (int *) realloc(gfxPrimitivesPolyInts, sizeof(int) * n);
	    gfxPrimitivesPolyAllocated = n;
	}
    }

    /*
     * Determine Y maxima 
     */
    miny = vy[0];
    maxy = vy[0];
    for (i = 1; (i < n); i++) {
	if (vy[i] < miny) {
	    miny = vy[i];
	} else if (vy[i] > maxy) {
	    maxy = vy[i];
	}
    }

    /*
     * Draw, scanning y 
     */
    result = 0;
    for (y = miny; (y <= maxy); y++) {
	ints = 0;
	for (i = 0; (i < n); i++) {
	    if (!i) {
		ind1 = n - 1;
		ind2 = 0;
	    } else {
		ind1 = i - 1;
		ind2 = i;
	    }
	    yy1 = vy[ind1];
	    y2 = vy[ind2];
	    if (yy1 < y2) {
		x1 = vx[ind1];
		x2 = vx[ind2];
	    } else if (yy1 > y2) {
		y2 = vy[ind1];
		yy1 = vy[ind2];
		x2 = vx[ind1];
		x1 = vx[ind2];
	    } else {
		continue;
	    }
	    if ( ((y >= yy1) && (y < y2)) || ((y == maxy) && (y > yy1) && (y <= y2)) ) {
		gfxPrimitivesPolyInts[ints++] = ((65536 * (y - yy1)) / (y2 - yy1)) * (x2 - x1) + (65536 * x1);
	    } 
	    
	}
	
	qsort(gfxPrimitivesPolyInts, ints, sizeof(int), gfxPrimitivesCompareInt);

	for (i = 0; (i < ints); i += 2) {
	    xa = gfxPrimitivesPolyInts[i] + 1;
	    xa = (xa >> 16) + ((xa & 32768) >> 15);
	    xb = gfxPrimitivesPolyInts[i+1] - 1;
	    xb = (xb >> 16) + ((xb & 32768) >> 15);
	    result |= hlineColor(dst, xa, xb, y, color);
	}
    }

    return (result);
}

int filledPolygonRGBA(SDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    /*
     * Draw 
     */
    return (filledPolygonColor
	    (dst, vx, vy, n, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}



int gfxPrimitivesCompareInt(const void *a, const void *b)
{
    return (*(const int *) a) - (*(const int *) b);
}

/* ---- Bezier curve */

/*
 Calculate bezier interpolator of data array with ndata values at position 't'
*/

double evaluateBezier (double *data, int ndata, double t) 
{
 double mu, result;
 int n,k,kn,nn,nkn;
 double blend,muk,munk;
     
 /* Sanity check bounds */
 if (t<0.0) {
  return(data[0]);
 }
 if (t>=(double)ndata) {
  return(data[ndata-1]);
 }
  
 /* Adjust t to the range 0.0 to 1.0 */ 
 mu=t/(double)ndata;
 
 /* Calculate interpolate */
 n=ndata-1;
 result=0.0;
 muk = 1;
 munk = pow(1-mu,(double)n);
 for (k=0;k<=n;k++) {
  nn = n;
  kn = k;
  nkn = n - k;
  blend = muk * munk;
  muk *= mu;
  munk /= (1-mu);
  while (nn >= 1) {
   blend *= nn;
   nn--;
   if (kn > 1) {
    blend /= (double)kn;
    kn--;
   }
   if (nkn > 1) {
    blend /= (double)nkn;
    nkn--;
   }
  }
  result += data[k] * blend;
 }
                                                          
 return(result);
}

int bezierColor(SDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, int s, Uint32 color)
{
    int result;
    int i;
    double *x, *y, t, stepsize;
    Sint16 x1, yy1, x2, y2;

    /*
     * Sanity check 
     */
    if (n < 3) {
	return (-1);
    }
    if (s < 2) {
        return (-1);
    }
    
    /*
     * Variable setup 
     */
    stepsize=(double)1.0/(double)s;
    
    /* Transfer vertices into float arrays */
    if ((x=(double *)malloc(sizeof(double)*(n+1)))==null) {
     return(-1);
    }
    if ((y=(double *)malloc(sizeof(double)*(n+1)))==null) {
     free(x);
     return(-1);
    }    
    for (i=0; i<n; i++) {
     x[i]=vx[i];
     y[i]=vy[i];
    }      
    x[n]=vx[0];
    y[n]=vy[0];
    
    /*
     * Draw 
     */
    result = 0;
    t=0.0;
    x1=evaluateBezier(x,n+1,t);
    yy1=evaluateBezier(y,n+1,t);
    for (i = 0; i <= (n*s); i++) {
	t += stepsize;
	x2=(Sint16)evaluateBezier(x,n,t);
	y2=(Sint16)evaluateBezier(y,n,t);
	result |= lineColor(dst, x1, yy1, x2, y2, color);
	x1 = x2;
	yy1 = y2;
    }

    /* Clean up temporary array */
    free(x);
    free(y);
    
    return (result);
}

int bezierRGBA(SDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, int s, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    /*
     * Draw 
     */
    return (bezierColor(dst, vx, vy, n, s, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}


// Original gifted by Mario Klingmann
// Make sure surface is unlocked
Bitmap G_fastBlur( Bitmap img, int radius ) {
   int w, h, wm, hm, wh, ddiv, worh;
   int rsum,gsum,bsum,x,y,i,p,p1,p2,yp,yi,yw;
   int *r,*g,*b, *vmax, *vmin, *dv;
   long rsize,gsize,bsize,vminsize,vmaxsize,dvsize;
   
  if (radius<1){    return img;  }  
  
  w=img.w;      h=img.h;
  wm=w-1;        hm=h-1;
  wh=w*h;        ddiv=radius+radius+1;
  
  rsize=gsize=bsize=sizeof(int)*wh;
  vminsize=vmaxsize=sizeof(int)*(worh=UMAX(w,h));
  dvsize=256*ddiv;  
  r = malloc(rsize);
  if ( r == null ) { malloc_stats(); printf( "Out of memory error!\n" ); exit(1); }
  g = malloc(gsize);
  if ( g == null ) { malloc_stats(); printf( "Out of memory error!\n" ); exit(1); }
  b = malloc(bsize);
  if ( b == null ) { malloc_stats(); printf( "Out of memory error!\n" ); exit(1); }
  vmin = malloc(vminsize);
  if ( vmin == null ) { malloc_stats(); printf( "Out of memory error!\n" ); exit(1); }
  vmax = malloc(vmaxsize);
  if ( vmax == null ) { malloc_stats(); printf( "Out of memory error!\n" ); exit(1); }
  dv = malloc(sizeof(int)*dvsize); 
  if ( dv == null ) { malloc_stats(); printf( "Out of memory error!\n" ); exit(1); }
  
  for (i=0;i<dvsize;i++){     dv[i]=i/ddiv;   }
  
  yw=yi=0;
 
  for (y=0;y<h;y++){
    rsum=gsum=bsum=0;
    for(i=-radius;i<=radius;i++){
      p=getpixelxy(img,yi+UMIN(wm,UMAX(0,i)));
      rsum+=RED(p,img.format);   //         rsum+=(p & 0xff0000)>>16;
      gsum+=GREEN(p,img.format); //         gsum+=(p & 0x00ff00)>>8;
      bsum+=BLUE(p,img.format);  //         bsum+= p & 0x0000ff;

   }
    for (x=0;x<w;x++){
    
      r[yi]=dv[rsum];      g[yi]=dv[gsum];      b[yi]=dv[bsum];

      if(y==0){
        vmin[x]=Min(x+radius+1,wm);
        vmax[x]=Max(0,x-radius);
       } 
       p1=getpixelxy(img,yw+vmin[x]);
       p2=getpixelxy(img,yw+vmax[x]);

      rsum+=RED(p1,img.format)-RED(p2,img.format);
            //((p1 & 0xff0000)-(p2 & 0xff0000))>>16;
      gsum+=GREEN(p1,img.format)-GREEN(p2,img.format);
            //((p1 & 0x00ff00)-(p2 & 0x00ff00))>>8;
      bsum+=BLUE(p1,img.format)-BLUE(p2,img.format);
            //(p1 & 0x0000ff)-(p2 & 0x0000ff);
      yi++;
    }
    yw+=w;
  }
  
  for (x=0;x<w;x++){
    rsum=gsum=bsum=0;
    yp=-radius*w;
    for(i=-radius;i<=radius;i++){
      yi=Max(0,yp)+x;
      rsum+=r[yi];      gsum+=g[yi];      bsum+=b[yi];
      yp+=w;
    }
    yi=x;
    for (y=0;y<h;y++){
      putpixel(img,x,y,COLOR(dv[rsum],dv[gsum],dv[bsum],255,img.format));
    //  0xff000000 | (dv[rsum]<<16) | (dv[gsum]<<8) | dv[bsum];
      if(x==0){
        vmin[y]=UMIN(y+radius+1,hm)*w;
        vmax[y]=UMAX(y-radius,0)*w;
      } 
      p1=x+vmin[y];
      p2=x+vmax[y];

      rsum+=r[p1]-r[p2];
      gsum+=g[p1]-g[p2];
      bsum+=b[p1]-b[p2];

      yi+=w;
    }
  }
  
  free(r);
  free(g);
  free(b);
  free(vmin);
  free(vmax);
  free(dv);
  return img;
}