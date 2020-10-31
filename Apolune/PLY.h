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
#include "moremath.h"
#include "Strings.h"
#include "Model.h"
#include "OldVBO.h"

/* A simple PLY utility library for reading PLYs exported from Blender */

struct PLYvert {
 Zfloat x,y,z,nx,ny,nz;
 const char *read( const char *a ) {
  char in[32];
  const char *p=a;
  p=one_argument(p,in); x=(float) atof(in);
  p=one_argument(p,in); y=(float) atof(in);
  p=one_argument(p,in); z=(float) atof(in);
  p=one_argument(p,in); nx=(float) atof(in);
  p=one_argument(p,in); ny=(float) atof(in);
  p=one_argument(p,in); nz=(float) atof(in);   
  return p;
 }
 PLYvert& operator= ( const PLYvert &b ) {
  x=b.x;
  y=b.y;
  z=b.z;
  nx=b.nx;
  ny=b.ny;
  nz=b.nz;
  return *this;
 }
 
};

// Same as TriNormal?
struct PLYnormal {
 Zfloat x,y,z;
 PLYnormal& operator= ( const PLYnormal &b ) {
  x=b.x;
  y=b.y;
  z=b.z;
  return *this;
 }
};

struct PLYface {
 Indexed<Zint> indices;
 PLYface() { }
 const char *read( const char *a ) {
  char in[32];
  const char *p=a;
  p=one_argument(p,in);
  int size=atoi(in);
  indices.Size(size);
  for ( int i=0; i<size; i++ ) {
   p=one_argument(p,in);
   indices[i]=atoi(in);
  }
  return p;
 }
 PLYface& operator= ( PLYface &b ) {
  indices.Size(b.indices.length);
  for ( int i=0; i<(int)b.indices.length; i++ ) indices(i)=b.indices(i);
  return *this;
 }
};

class PLY : public ListItem {
public:
 Zstring name,uvmap;
 Zbool verified;
 Zint Triangles, Quads, Points, Faces; 
 Indexed<PLYvert> verts;
 Indexed<PLYface> faces;
 ImmediateModel model;
 PLY( PLY *in ) {
  name=in->name;
  uvmap=in->uvmap;
  verified=in->verified;
  Triangles=in->Triangles;
  Quads=in->Quads;
  Points=in->Points;
  Faces=in->Faces;
  verts.Size(Points);
  for ( int i=0; i < Points; i++ ) verts[i]=in->verts[i];
  faces.Size(Faces);
  for ( int i=0; i < Faces; i++ ) faces[i]=in->faces[i];
  in->model.CopyInto(&model);
 }
 PLY( const char * filename ) {
  OUTPUT("PLY: `%s` Loading: ",filename);
  name=string(filename);
  Triangles=Quads=Points=Faces=0;
  verified=false;
  char in[1024];
  string file=file_as_string(filename);
  string header;
  const char *p=file.c_str();
  p=one_argument(p,in);
  if ( !str_cmp(in,"ply") ) verified=true;
  else return;
  p=gather_to(p,"end_header",&header);
  // Parse the header
  const char *r=header.c_str();
  while ( *r != '\0' ) {
   string line;
   r=gather_to(r,"\n",&line);
   const char *s=line.c_str();
   while ( *s != '\0' ) {
    s=one_argument(s,in);
    // Ignore lines starting with ply, format (assuming ascii), comment, property or end_header
    if ( !str_cmp(in,"ply") || !str_cmp(in,"property") || !str_cmp(in,"end_header") ) break;
    else if ( !str_cmp(in,"comment") ) { // take the name of the object from the first word of the content
     s=one_argcase(s,in);
     name=string(in);
     OUTPUT("'%s', ", in);
     s=one_argcase(s,in);
     uvmap=string(in); // stores the name or keyword for uv-mapping step
    }
    else if ( !str_cmp(in,"element") ) { // Decipher properties of interest (face, vertex) ignores point descriptor
     s=one_argument(s,in);
     if ( !str_cmp(in,"vertex") ) {
      s=one_argument(s,in);
      Points=atoi(in);
      OUTPUT("%d points, ", (int) Points );
     } else
     if ( !str_cmp(in,"face") ) {
      s=one_argument(s,in);
      Faces=atoi(in);
      OUTPUT("%d faces, ", (int) Faces );
     }
    }
   }
  }
  // Read verts
  verts.Size(Points);
  for ( int i=0; i < Points; i++ ) p=verts[i].read(p);
  // Read faces
  faces.Size(Faces);
  for ( int i=0; i < Faces; i++ ) p=faces[i].read(p);

  Quads=0;
  Triangles=0;
  for ( int i=0; i < Faces; i++ ) if ( faces[i].indices.length == 4 ) Quads++; else Triangles++;
  OUTPUT("%d quads and %d triangles.\n", (int) Quads, (int) Triangles );
  NormalizeToOne();
  CopyToModelAndCreateTriangleIndices();
  MakeUVMap((char *) uvmap.c_str());
  model.CompileLists(1.0f);
  model.BoxBounds();
  model.RoughVolume();
 }

 void NormalizeToOne() {
  double largest=1.0;
  for ( int i=0; i< Points; i++ ) {
   if ( abs(verts[i].x) > largest ) largest=abs(verts[i].x);
   if ( abs(verts[i].y) > largest ) largest=abs(verts[i].y);
   if ( abs(verts[i].z) > largest ) largest=abs(verts[i].z);
  }
  double ratio=1.0;
  if ( largest > 1.0 ) {
   if ( largest < 2.0 ) {
    ratio=1.0-(largest-1.0);
   } else {
    ratio=1.0/largest;
   }
  }
  if ( ratio < 1.0 && largest > 1.001 ) {
   for ( int i=0; i< Points; i++ ) {
    verts[i].x*=(float)ratio;
    verts[i].y*=(float)ratio;
    verts[i].z*=(float)ratio;
   }
   OUTPUT("Model was too big.  Normalized to -1 to 1.  Largest was %1.3f, ratio was %1.3f\n", (float)largest, (float)ratio);
  }
 }

 void MakeUVMap(const char *type) {
  // check if it is a keyword, otherwise it is a filename
  if ( !str_cmp( type, "none" ) ) return;
  if ( !str_cmp( type, "stoop" ) ) { // Stoop
   OUTPUT("Stoop mapping.\n");
   model.StoopMap();
   return;
  } else
  if ( !str_cmp( type, "poots" ) ) { // opposite Poots
   OUTPUT("Poots mapping.\n");
   model.PootsMap();
   return;
  } else
  if ( !str_cmp( type, "cylindrical" ) ) { // Object should be mapped as a vertical cylinder
   OUTPUT("Cylindrical mapping.\n");
   model.CylindricalMap();
   return;
  } else
  if ( !str_cmp( type, "normal" ) ) { // Object should have one texture assigned to each surface aligned to normal and scaled to area.
   OUTPUT("Normal UV mapping.\n");
   model.NormalUVMap();
   return;
  } else
  if ( !str_cmp( type, "spherical" ) ) {
   OUTPUT("Spherical mapping.\n");
   model.SphereMap();
   return;
  } else
  if ( !str_cmp( type, "cubemap" ) || strlen(type)==0 ) {
   OUTPUT("Cube mapping.\n");
   model.CubeMap();
   return;
  }
  // if file does not exist, default to Stoop map...
 }

 // Imports a PLY from disk into the attached Immediate Model,
 // creating a "Generic Matter Primitive" which can be duplicated.
 // During import, it calculates a "Normalized CubeMap Texture Coordination"
 // Note this permits quads!
 void CopyToModelAndCreateTriangleIndices() {
  int index=0;
  // Import PLY verts by face into VBO storage class, then upload to GPU
  for ( int i=0; i<Faces; i++ ) {
   switch ( faces[i].indices.length ) {
    case 3: 
            { 
   mPolygon *p=new mPolygon;
   p->draw_method=GL_TRIANGLES; 
   Vertex a,b,c;
   a.x=verts[faces[i].indices[0]].x;
   a.y=verts[faces[i].indices[0]].y;
   a.z=verts[faces[i].indices[0]].z;
   a.w=1.0f;
   a.normal.Instantiate();
   a.normal->x=verts[faces[i].indices[0]].nx;
   a.normal->y=verts[faces[i].indices[0]].ny;
   a.normal->z=verts[faces[i].indices[0]].nz;
   p->points.Append(a.Copy());
   b.x=verts[faces[i].indices[1]].x;
   b.y=verts[faces[i].indices[1]].y;
   b.z=verts[faces[i].indices[1]].z;
   b.w=1.0f;
   b.normal.Instantiate();
   b.normal->x=verts[faces[i].indices[1]].nx;
   b.normal->y=verts[faces[i].indices[1]].ny;
   b.normal->z=verts[faces[i].indices[1]].nz;
   p->points.Append(b.Copy());
   c.x=verts[faces[i].indices[2]].x;
   c.y=verts[faces[i].indices[2]].y;
   c.z=verts[faces[i].indices[2]].z;
   c.w=1.0f;
   c.normal.Instantiate();
   c.normal->x=verts[faces[i].indices[2]].nx;
   c.normal->y=verts[faces[i].indices[2]].ny;
   c.normal->z=verts[faces[i].indices[2]].nz;
   p->points.Append(c.Copy());
   p->TriNormal();
   model.polygons.Append(p);
   model.triangles.Add(index,index+1,index+2);
   index+=3;
            }
     break;
    case 4: 
            {
   mPolygon *t1=new mPolygon, *t2=new mPolygon;
   Vertex a,b,c,d;
   a.x=verts[faces[i].indices[0]].x;
   a.y=verts[faces[i].indices[0]].y;
   a.z=verts[faces[i].indices[0]].z;
   a.normal.Instantiate();
   a.normal->x=verts[faces[i].indices[0]].nx;
   a.normal->y=verts[faces[i].indices[0]].ny;
   a.normal->z=verts[faces[i].indices[0]].nz;
   b.x=verts[faces[i].indices[1]].x;
   b.y=verts[faces[i].indices[1]].y;
   b.z=verts[faces[i].indices[1]].z;
   b.normal.Instantiate();
   b.normal->x=verts[faces[i].indices[1]].nx;
   b.normal->y=verts[faces[i].indices[1]].ny;
   b.normal->z=verts[faces[i].indices[1]].nz;
   c.x=verts[faces[i].indices[2]].x;
   c.y=verts[faces[i].indices[2]].y;
   c.z=verts[faces[i].indices[2]].z;
   c.normal.Instantiate();
   c.normal->x=verts[faces[i].indices[2]].nx;
   c.normal->y=verts[faces[i].indices[2]].ny;
   c.normal->z=verts[faces[i].indices[2]].nz;
   d.x=verts[faces[i].indices[3]].x;
   d.y=verts[faces[i].indices[3]].y;
   d.z=verts[faces[i].indices[3]].z;
   d.normal.Instantiate();
   d.normal->x=verts[faces[i].indices[3]].nx;
   d.normal->y=verts[faces[i].indices[3]].ny;
   d.normal->z=verts[faces[i].indices[3]].nz;
   t1->draw_method=GL_TRIANGLES;
   t1->points.Append(a.Copy());
   t1->points.Append(b.Copy());  //    t2       t1
   t1->points.Append(c.Copy());  //  d   a         a
   t1->TriNormal();              //  1   0         0
   t2->draw_method=GL_TRIANGLES; //  2         2   1
   t2->points.Append(a.Copy());  //  c         c   b
   t2->points.Append(d.Copy());  
   t2->points.Append(c.Copy());
   t2->TriNormal();
   model.polygons.Append(t1);
   model.polygons.Append(t2);
   model.triangles.Add(index,index+1,index+2);
   index+=3;
   model.triangles.Add(index,index+3,index+2);
   index+=3;
           }
     break;
   }
  }
 }

 // Flip normals stored in the vert list 
 void negateNormals() {
  int i;
  for (i = 0; i < Points; i++) {
   verts[i].nx *= -1;
   verts[i].ny *= -1;
   verts[i].nz *= -1;
  }
 }

 PLYnormal* calculateNormal( PLYvert *coord1, PLYvert *coord2, PLYvert *coord3 ) {
  /* calculate Vector1 and Vector2 */
  float va[3], vb[3], vr[3], val;
  va[0] = coord1->x - coord2->x; va[1] = coord1->y - coord2->y; va[2] = coord1->z - coord2->z;
  vb[0] = coord1->x - coord3->x; vb[1] = coord1->y - coord3->y; vb[2] = coord1->z - coord3->z;
  /* cross product */
  vr[0] = va[1] * vb[2] - vb[1] * va[2];
  vr[1] = vb[0] * va[2] - va[0] * vb[2];
  vr[2] = va[0] * vb[1] - vb[0] * va[1];
  /* normalization factor */
  val = sqrt( vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2] );
  PLYnormal *norm=new PLYnormal;
  norm->x = vr[0]/val;
  norm->y = vr[1]/val;
  norm->z = vr[2]/val; 
  return norm;
 }
};

class PLYs : public LinkedList {
public:
 PLY *Random(int seed) { return (PLY *) Element(seed%count); }
 PLY *Random() {
  int r=upto(count);
  return (PLY *) Element(r);
 }
 void Load( const char *filename ) {
  Append((ListItem *) new PLY(filename));
 }
 PLY *find( const char *name ) {
  FOREACH(PLY,p) if ( !str_cmp((char *) p->name.c_str(),name) ) return p;
  OUTPUT("PLYs:find(`%s`) could not find the primitive\n", name);
  return null;
 }
};

extern PLYs lowPLYprimitives;
extern PLYs highPLYprimitives;

HANDLED(PLY,PLYHandle,PLYHandles,PLYHandlesHandle,PLyHandlesHandles);

#if defined(NEVER)

/******************************************************************************
Create a tetrahedron.
******************************************************************************/

tetra()
{
  static float v[4][3] = {
    -1, -1, -1,
     1,  1, -1,
     1, -1,  1,
    -1,  1,  1,
  };
  static int f[4][3] = {
    1, 2, 3,
    1, 0, 2,
    3, 2, 0,
    0, 1, 3,
  };
  int i;

  nverts = 4;
  nfaces = 4;

  for (i = 0; i < nverts; i++) {
    vlist[i].x = v[i][0];
    vlist[i].y = v[i][1];
    vlist[i].z = v[i][2];
  }

  for (i = 0; i < nfaces; i++) {
    flist[i].nverts = 3;
    flist[i].verts = (int *) malloc (sizeof(int) * 3);
    flist[i].verts[0] = f[i][0];
    flist[i].verts[1] = f[i][1];
    flist[i].verts[2] = f[i][2];
  }
}


/******************************************************************************
Create an octahedron.
******************************************************************************/

octa()
{
  static float v[6][3] = {
     1,  0,  0,
     0, -1,  0,
    -1,  0,  0,
     0,  1,  0,
     0,  0,  1,
     0,  0, -1,
  };
  static int f[8][3] = {
    4, 0, 1,
    4, 1, 2,
    4, 2, 3,
    4, 3, 0,
    5, 1, 0,
    5, 2, 1,
    5, 3, 2,
    5, 0, 3,
  };
  int i;

  nverts = 6;
  nfaces = 8;

  for (i = 0; i < nverts; i++) {
    vlist[i].x = v[i][0];
    vlist[i].y = v[i][1];
    vlist[i].z = v[i][2];
  }

  for (i = 0; i < nfaces; i++) {
    flist[i].nverts = 3;
    flist[i].verts = (int *) malloc (sizeof(int) * 3);
    flist[i].verts[0] = f[i][0];
    flist[i].verts[1] = f[i][1];
    flist[i].verts[2] = f[i][2];
  }
}


/******************************************************************************
Create a cube.
******************************************************************************/

cube()
{
  static float v[8][3] = {
    -1, -1, -1,
     1, -1, -1,
     1,  1, -1,
    -1,  1, -1,
    -1, -1,  1,
     1, -1,  1,
     1,  1,  1,
    -1,  1,  1,
  };
  static int f[6][4] = {
    0, 1, 2, 3,
    5, 4, 7, 6,
    6, 2, 1, 5,
    3, 7, 4, 0,
    7, 3, 2, 6,
    5, 1, 0, 4,
  };
  int i;

  nverts = 8;
  nfaces = 6;

  for (i = 0; i < nverts; i++) {
    vlist[i].x = v[i][0];
    vlist[i].y = v[i][1];
    vlist[i].z = v[i][2];
  }

  for (i = 0; i < nfaces; i++) {
    flist[i].nverts = 4;
    flist[i].verts = (int *) malloc (sizeof(int) * 4);
    flist[i].verts[0] = f[i][0];
    flist[i].verts[1] = f[i][1];
    flist[i].verts[2] = f[i][2];
    flist[i].verts[3] = f[i][3];
  }
}


/******************************************************************************
Create an icosahedron.
******************************************************************************/

icosahedron()
{
  static float v[12][3] = {
    0, -2,  1,
    1,  0,  2,
    1,  0, -2,
    -1,  0, -2,
    -1,  0,  2,
    -2,  1,  0,
    2,  1,  0,
    2, -1,  0,
    -2, -1,  0,
    0, -2, -1,
    0,  2, -1,
    0,  2,  1,
  };
  static int f[20][3] = {
    6, 2, 1, 
    2, 7, 1, 
    5, 4, 3, 
    8, 3, 4, 
    11, 5, 6, 
    10, 6, 5, 
    2, 10, 9, 
    3, 9, 10, 
    9, 8, 7, 
    0, 7, 8, 
    1, 0, 11, 
    4, 11, 0, 
    10, 2, 6, 
    11, 6, 1, 
    10, 5, 3, 
    11, 4, 5, 
    9, 7, 2, 
    0, 1, 7, 
    8, 9, 3, 
    0, 8, 4, 
  };
  int i;
  float *p;
  float a,b;
  float len;

  a = 0.5 * (1 + sqrt(5));
  b = 1;

  len = sqrt(a*a + b*b);
  a /= len;
  b /= len;

  for (i = 0, p = (float *) v; i < 12 * 3; i++, p++) {
    if (fabs(*p) == 1)
      *p *= a;
    if (fabs(*p) == 2)
      *p *= b / 2.0;
  }

  nverts = 12;
  nfaces = 20;

  for (i = 0; i < nverts; i++) {
    vlist[i].x = v[i][0];
    vlist[i].y = v[i][1];
    vlist[i].z = v[i][2];
  }

  for (i = 0; i < nfaces; i++) {
    flist[i].nverts = 3;
    flist[i].verts = (int *) malloc (sizeof(int) * 3);
    flist[i].verts[0] = f[i][0];
    flist[i].verts[1] = f[i][1];
    flist[i].verts[2] = f[i][2];
  }
}


/******************************************************************************
Create a dodecahedron.
******************************************************************************/

dodecahedron()
{
  static float v[20][3] = {
    -2, -2, 2,
    3, 1, 0,
    3, -1, 0,
    -3, 1, 0,
    -3, -1, 0,
    0, 3, 1,
    0, 3, -1,
    1, 0, -3,
    -1, 0, -3,
    0, -3, -1,
    0, -3, 1,
    1, 0, 3,
    -1, 0, 3,
    2, 2, -2,
    2, 2, 2,
    -2, 2, -2,
    -2, 2, 2,
    2, -2, -2,
    2, -2, 2,
    -2, -2, -2,
  };
  static int f[12][5] = {
    1, 2, 18, 11, 14, 
    1, 13, 7, 17, 2, 
    3, 4, 19, 8, 15, 
    3, 16, 12, 0, 4, 
    3, 15, 6, 5, 16, 
    1, 14, 5, 6, 13, 
    2, 17, 9, 10, 18, 
    4, 0, 10, 9, 19, 
    7, 8, 19, 9, 17, 
    6, 15, 8, 7, 13, 
    5, 14, 11, 12, 16, 
    10, 0, 12, 11, 18, 
  };
  int i;
  float *p;
  float a,b,c;
  float len;

  a = 1 + 0.5 * (1 + sqrt(5));
  b = 1;
  c = 0.5 * (1 + sqrt(5));

  len = sqrt(a*a + b*b);
  a /= len;
  b /= len;
  c /= len;

  for (i = 0, p = (float *) v; i < 20 * 3; i++, p++) {
    if (fabs(*p) == 1)
      *p *= b;
    if (fabs(*p) == 2)
      *p *= c / 2.0;
    if (fabs(*p) == 3)
      *p *= a / 3.0;
  }

  nverts = 20;
  nfaces = 12;

  for (i = 0; i < nverts; i++) {
    vlist[i].x = v[i][0];
    vlist[i].y = v[i][1];
    vlist[i].z = v[i][2];
  }

  for (i = 0; i < nfaces; i++) {
    flist[i].nverts = 5;
    flist[i].verts = (int *) malloc (sizeof(int) * 5);
    flist[i].verts[0] = f[i][0];
    flist[i].verts[1] = f[i][1];
    flist[i].verts[2] = f[i][2];
    flist[i].verts[3] = f[i][3];
    flist[i].verts[4] = f[i][4];
  }
}
#endif