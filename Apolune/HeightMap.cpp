#include "HeightMap.h"

#include <math.h>
#include "moremath.h"
#include "Vertex.h"
#include "Model.h"
#include "Indexed.h"

float Displace(float num)
{
  float max = num / (float)(SRES + SRES) * 3;
  return ((float) random(1.0) - 0.5f) * max;
}

void DivideAndConquer(float x, float y, int hw, float c1, float c2, float c3, float c4, float *Map)
{
  float Edge1, Edge2, Edge3, Edge4, Middle;
  int HW = hw / 2;

  if (hw > 2)
  {
    Middle = (c1 + c2 + c3 + c4) / 4 + Displace(W + H);
    Edge1 = (c1 + c2) / 2; Edge2 = (c2 + c3) / 2; Edge3 = (c3 + c4) / 2; Edge4 = (c4 + c1) / 2;

    //Make sure that the midpoint doesn't accidentally "randomly displaced" past the boundaries!
    if (Middle < 0)
    {
      Middle = 0;
    }
    else if (Middle > 1.0f)
    {
      Middle = 1.0f;
    }

    //Do the operation over again for each of the four new grids.
    DivideAndConquer(x,      y,      HW, c1,     Edge1,  Middle, Edge4,  Map);
    DivideAndConquer(x + HW, y,      HW, Edge1,  c2,     Edge2,  Middle, Map);
    DivideAndConquer(x + HW, y + HW, HW, Middle, Edge2,  c3,     Edge3,  Map);
    DivideAndConquer(x,      y + HW, HW, Edge4,  Middle, Edge3,  c4,     Map);
  }
  else	//This is the "base case," where each grid piece is less than the size of a pixel.
  {
    //The four corners of the grid piece will be averaged and drawn as a single pixel.
    float c = (c1 + c2 + c3 + c4) / 4;
    Map[(int)x+(int)y*SRES]=c;

        /* comment out if not 1:1 aspect */
    Map[(int)x+   ((int)y+1)*SRES] =Map[(int)x+(int)y*SRES];
    Map[(int)x+1+  (int)y   *SRES] =Map[(int)x+(int)y*SRES];
    Map[(int)x+1+ ((int)y+1)*SRES] =Map[(int)x+(int)y*SRES]; 

 }
}

void plasmize(int hw, int seed, float *Map)
{
  float c1, c2, c3, c4;
  c4=c3=c2=c1= (float) random(seed);
  DivideAndConquer(0.0f,0.0f,hw,c1,c2,c3,c4,Map);
}

void smoothize( float *Map, int radius, int hw ) {
 Indexed<Zfloat> D;
 D.Size(hw*hw);
 for (int y = 0; y < hw; ++y) {
  for (int x = 0; x < hw; ++x) {
   float p,t = 0.0f;
   for (int ky = -radius; ky <= radius; ++ky) for (int kx = -radius; kx <= radius; ++kx) t += Map[iwrap((x + kx),0,511)+iwrap(y + ky,0,511)*hw];
   D[x+y*hw] = t / (p=radius * 2.0f + 1.0f)*p;
  }
 }
 for ( int x=0; x<hw; x++ ) for ( int y=0; y<hw; y++ ) Map[x+y*hw]=D[x+y*hw];
}

C2Smap C2S[SRES*SRES];

// C2Smap:
// float x,y,z;
// float lambda,phi;

void pregenerateCubeToSphereMap() {
 float R=1.0f;
 // For a 512x512 map, generate spherical coordinates for each
 // sphere will have 8 bands per hemisphere, at 32 segments each, plus a polar coordinate calculated from an average of the bands
 for ( int x=0; x<SRES; x++ ) {
  for ( int y=0; y<SRES; y++ ) {
   int coord=x+y*SRES;
   C2S[coord].lambda=((float) (x-(SRES/2)) / (SRES/2))*(float) PI;
   C2S[coord].phi   =((float) (y-(SRES/2)) / (SRES/2))*(float) PI;
   C2S[coord].x = R * cos (C2S[coord].lambda) * cos (C2S[coord].phi);
   C2S[coord].y = R * sin (C2S[coord].lambda) * cos (C2S[coord].phi);
   C2S[coord].z = R * sin (C2S[coord].phi);
  }
 }
}

// Create a sphere model
void HeightMap::toSphere( float radius ) {
#if defined(NEVER) 
 Vertex *sphereMap=new Vertex[SRES*SRES];
 if ( sphered ) delete sphered;
 sphered=new Model;
 for ( int x=0; x<SRES; x++ ) {
  for ( int y=0; y<SRES; y++ ) {
   int coord=x+y*SRES;
   sphereMap[coord].x=radius * (C2S[coord].x + (map[coord] * cos (C2S[coord].lambda) * cos (C2S[coord].phi)));
   sphereMap[coord].y=radius * (C2S[coord].y + (map[coord] * sin (C2S[coord].lambda) * cos (C2S[coord].phi)));
   sphereMap[coord].z=radius * (C2S[coord].z + (map[coord] * sin (C2S[coord].phi)));
   sphereMap[coord].q=(GLfloat) (C2S[coord].lambda/(float) PI);
   sphereMap[coord].r=(GLfloat) (C2S[coord].phi/(float) PI);
// for testing
   sphereMap[coord].rgba[0] = (float) x/(float)SRES;
   sphereMap[coord].rgba[1] = (float) y/(float)SRES;
   sphereMap[coord].rgba[2] = (((float)x+(float)y)/2.0f)/(float)SRES;
   sphereMap[coord].rgba[3] = 1.0f;
  }
 }
 for ( int x=0; x<SRES-1; x++) {
  for ( int y=0; y<SRES-1; y++ ) {
   int coord=x+y*SRES;
   mPolygon *p=new mPolygon;
   p->draw_method=GL_POLYGON;
   p->addVertex(sphereMap[coord].Copy());
   p->addVertex(sphereMap[coord+1].Copy());
   p->addVertex(sphereMap[coord+SRES+1].Copy());
   p->addVertex(sphereMap[coord+SRES].Copy());
   sphered->addPolygon(p);
  }
 }
 delete[] sphereMap;
#endif
}

void HeightMap::Generate( int s ) {
 if ( sphered ) delete sphered;
}
