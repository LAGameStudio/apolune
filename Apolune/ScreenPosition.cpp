#include "ScreenPosition.h"

#include "Art.h"
#include "UglyFont.h"

void ScreenPosition::DrawDebug() {
 Rectangle(crayons.Pick(hotPink),x,y,x2,y2);
 char buf[256];
 FORMAT(buf,256,"%d,%d -> %d,%d (%dx%d) center: %d,%d\n%1.2f,%1.2f -> %1.2f %1.2f (%1.2fx%1.2f) center: %1.2f,%1.2f \n %1.4f,%1.4f -> %1.4f,%1.4f (%1.4fx%1.4f) center: %1.4f,%1.4f",
  (int)x,(int)y,(int)x2,(int)y2,(int)w,(int)h,(int)cx,(int)cy,
  (float)fx,(float)fy,(float)fx2,(float)fy2,(float)fw,(float)fh,(float)fcx,(float)fcy,
  (double)dx,(double)dy,(double)dx2,(double)dy2,(double)dw,(double)dh,(double)dcx,(double)dcy
 );
 Text( buf, x+10, y+10 );
 FORMAT(buf,256,"X %d,%d",(int)cx,(int)cy);
 Text(buf,crayons.Pick(red255),dcx-3.0,dcy-3.0);
 FORMAT(buf,256,"%d,%d",(int)x,(int)y);
 Text(buf,crayons.Pick(red255),x-10,y-10);
 FORMAT(buf,256,"%d,%d",(int)x2,(int)y2);
 Text(buf,crayons.Pick(red255),x2+10,y2+10);
 FORMAT(buf,256,"%d X %d",(int)w,(int)h);
 Text(buf,crayons.Pick(red255),cx,cy+30);
}