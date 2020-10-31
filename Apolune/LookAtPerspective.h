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
#include "Interface.h"
#include "Vertex.h"

extern Interface input;

extern GLfloat n[6][3];
extern GLint faces[6][4];
extern GLfloat v[8][3];

struct SimpleCube {
 GLfloat v[8][3];
 SimpleCube() {
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;
 }
 void Render();
};

struct LookAtPerspective {
public:
 double halfFOV,aspect,Near,Far;
 Cartesian screen; 
 Vertexd eye, center, up;
 bool ortho;
 Cartesiand extents;
 LookAtPerspective() {
  ortho=false;
  halfFOV=40.0;
  aspect=4.0/3.0;
  Near=1.0;
  Far=10.0;
  eye.Set(0.0,0.0,5.0);
  center.Set(0.0,0.0,-5.0);
  up.Set(0.0,1.0,0.0);
 }
 void Reset() {
  screen.SetRect(0,0,0,0);
  extents.SetRect(0,0,0,0);
  ortho=false;
  halfFOV=40.0;
  aspect=4.0/3.0;
  Near=1.0;
  Far=10.0;
  eye.Set(0.0,0.0,5.0);
  center.Set(0.0,0.0,-5.0);
  up.Set(0.0,1.0,0.0);
 }
 void BinaryRead( BinaryFile *open ) {
  open->read(&halfFOV);
  open->read(&aspect);
  open->read(&Near);
  open->read(&Far);
  screen.BinaryRead(open);
  eye.BinaryRead(open);
  center.BinaryRead(open);
  up.BinaryRead(open);
  int temp; open->read(&temp); ortho=(temp!=0);
  extents.BinaryRead(open);
 }
 void BinaryWrite( BinaryFile *open ) {
  open->write(&halfFOV);
  open->write(&aspect);
  open->write(&Near);
  open->write(&Far);
  screen.BinaryWrite(open);
  eye.BinaryWrite(open);
  center.BinaryWrite(open);
  up.BinaryWrite(open);
  int temp=ortho?1:0; open->write(&temp);
  extents.BinaryWrite(open);
 }
 SKEYSTART
  SKEY("halffov", { SPARAM(halfFOV,double,atof); })
  SKEY("aspect",  { SPARAM(aspect,double,atof); })
  SKEY("near",    { SPARAM(Near,double,atof); })
  SKEY("far",     { SPARAM(Far,double,atof); })
  SKEY("screen",  { SKEYPAIR; screen.fromString((char *) key.c_str()); })
  SKEY("eye",     { SKEYPAIR; eye.fromString((char *) key.c_str()); })
  SKEY("center",  { SKEYPAIR; center.fromString((char *) key.c_str()); })
  SKEY("up",      { SKEYPAIR; up.fromString((char *) key.c_str()); })
  SKEY("extents", { SKEYPAIR; extents.fromString((char *) key.c_str()); })
 SKEYEND("LookAtPerspective");
 string toString() {
  incdent();
  string out=string("");
  if ( halfFOV != 40.0 )   { FORMAT(buf,80,"halffov %1.5f ", (float) halfFOV); out+=indention+string(buf)+string("\n"); }
  if ( aspect != 4.0/3.0 ) { FORMAT(buf,80,"aspect %1.5f ", (float) aspect); out+=indention+string(buf)+string("\n"); }
  if ( Near != 1.0 ) { FORMAT(buf,80,"near %1.5f ", (float) Near ); out+=indention+string(buf)+string("\n"); }
  if ( Far != 10.0 ) { FORMAT(buf,80,"far %1.5f ", (float) Far); out+=indention+string(buf)+string("\n"); }
  out+=indention+string("screen {\n")+screen.toString()+indention+string("}\n");
  out+=indention+string("eye {\n")+eye.toString()+indention+string("}\n");
  out+=indention+string("center {\n")+center.toString()+indention+string("}\n");
  out+=indention+string("up {\n")+up.toString()+indention+string("}\n");
  decdent();
  return out;
 }
 void Set( LookAtPerspective *lap ) {
  halfFOV=lap->halfFOV;
  aspect=lap->aspect;
  Near=lap->Near;
  Far=lap->Far;
  screen.Set(&lap->screen);
  eye.Set(&lap->eye);
  center.Set(&lap->center);
  up.Set(&lap->up);
  ortho=lap->ortho;
  extents.Set(&lap->extents);
 }
 void Screen( int x, int y, int w, int h ) {
  aspect=iratiod(w,h);
 }
 void Apply() {
  /* Setup the view. */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if ( !ortho )
  gluPerspective(
    halfFOV, /* degrees */
    aspect,
    Near, 
    Far);
  else glOrtho( extents.x, extents.y, extents.x2, extents.y2, Near, Far );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(
   eye.x,eye.y,eye.z,
   center.x,center.y,center.z,
   up.x,up.y,up.z
  );
 }
 void MouseSimpleRotation() {
  glRotated(input.mxd * 360.0,1.0,0.0,0.0);
  glRotated(input.myd * 360.0,0.0,0.0,1.0);
 } 
 Zdouble debugDelay;
 void DebugKeys() {
  if ( debugDelay <= 0.0 ) {
	  if ( input.KeyDown(DX_LEFT) ) {
		  
	  } else if ( input.KeyDown(DX_RIGHT) ) {
	  } else if ( input.KeyDown(DX_UP) ) {
	  } else if ( input.KeyDown(DX_DOWN) ) {
	  } else if ( input.KeyDown(DX_HOME) ) {
	  } else if ( input.KeyDown(DX_END) ) {
	  }
  } else {
   debugDelay-=FRAMETIME;
  }
 }
};

struct Coord4 {
 float coord[4];
 Coord4() { Set(0.0f,0.0f,0.0f,0.0f); }
 void Set( float a, float b, float c, float d ) {
  coord[0]=a;
  coord[1]=b;
  coord[2]=c;
  coord[3]=d;
 }
 SKEYSTART
  SPARAM(coord[0],float,atof);
  SPARAM(coord[1],float,atof);
  SPARAM(coord[2],float,atof);
  SPARAM(coord[3],float,atof);
 SKEYEND("Coord4");
 string toString() {
  FORMAT(buf,80,"%1.5f %1.5f %1.5f %1.5f", 
   (float) coord[0], (float) coord[1], (float) coord[2], (float) coord[3] );
  return string(buf);
 }
};

struct ImmediateLight {
 Coord4 ambient,diffuse,specular,position,direction;
 float
  cutoff,exponent,
  constant,linear,quadratic;
 bool on,spot;
 ImmediateLight() {
  ambient.Set(0.0f,0.0f,0.0f,1.0f); // OGL Default
  diffuse.Set(1.0f,1.0f,1.0f,1.0f);
  specular.Set(1.0f,1.0f,1.0f,1.0f);
  cutoff=180.0f; // "point" light
  exponent=0.0f;
  constant=1.0f;
  linear=0.0f;
  quadratic=0.0f;
  on=false;
  spot=false;
 }
 SKEYSTART
  SKEY("ambient",  { SKEYPAIR; ambient.fromString((char *) key.c_str()); })
  SKEY("diffuse",  { SKEYPAIR; ambient.fromString((char *) key.c_str()); })
  SKEY("specular", { SKEYPAIR; ambient.fromString((char *) key.c_str()); })
  SKEY("position", { SKEYPAIR; ambient.fromString((char *) key.c_str()); })
  SKEY("cutoff",   { SKEYPAIR; cutoff=(float)atof(key.c_str()); })
  SKEY("exponent", { SKEYPAIR; exponent=(float)atof(key.c_str()); })
  SKEY("constant", { SKEYPAIR; constant=(float)atof(key.c_str()); })
  SKEY("linear",   { SKEYPAIR; linear=(float)atof(key.c_str()); })
  SKEY("quadratic",{ SKEYPAIR; quadratic=(float)atof(key.c_str()); })
  SKEY("on", { on=true; })
  SKEY("spot", { spot=true; });
 SKEYEND("ImmediateLight");
 string toString() {
  string out=string("");
  incdent();
  if ( ambient.coord[0] != 0.0f || ambient.coord[1] != 0.0f || ambient.coord[2] != 0.0f || ambient.coord[3] != 1.0f )
   out+=indention+string("ambient {")+ambient.toString()+string("}\n");
  if ( diffuse.coord[0] != 1.0f || diffuse.coord[1] != 1.0f || diffuse.coord[2] != 1.0f || diffuse.coord[3] != 1.0f )
   out+=indention+string("diffuse {")+diffuse.toString()+string("}\n");
  if ( specular.coord[0] != 1.0f || specular.coord[1] != 1.0f || specular.coord[2] != 1.0f || specular.coord[3] != 1.0f )
   out+=indention+string("specular {")+specular.toString()+string("}\n");
  if ( position.coord[0] != 0.0f || position.coord[1] != 0.0f || position.coord[2] != 0.0f || position.coord[3] != 0.0f )
   out+=indention+string("position {")+position.toString()+string("}\n");
  if ( direction.coord[0] != 0.0f || direction.coord[1] != 0.0f || direction.coord[2] != 0.0f || direction.coord[3] != 0.0f )
   out+=indention+string("direction {")+direction.toString()+string("}\n");
  if ( cutoff != 180.0 ) out+=FORMAT(buf,128,"cutoff %f\n", cutoff);
  if ( exponent != 0.0 ) out+=FORMAT(buf,128,"exponent %f\n", exponent);
  if ( constant != 1.0f ) out+=FORMAT(buf,128,"constant %f\n", constant);
  if ( linear != 0.0f ) out+=FORMAT(buf,128,"linear %f\n", linear);
  if ( quadratic != 0.0f ) out+=FORMAT(buf,128,"quadratic %f\n", quadratic);
  if ( on ) out+=indention+string("on\n");
  if ( spot ) out+=indention+string("spot\n");
  decdent();
  return out;
 }
};

/*
 * color-index mode and ambient_and_diffuse are not supported.
 */
struct GLMaterial : public ListItem {
 string name;
 Coord4 ambient,diffuse,specular,emission;
 float shininess[1];
 Zdisposable<GLMaterial> back;
 GLMaterial() : ListItem() {
  name=string("");
  ambient.Set(0.2f,0.2f,0.2f,1.0f);
  diffuse.Set(0.8f,0.8f,0.8f,1.0f);
  specular.Set(0.0f,0.0f,0.0f,1.0f);
  shininess[0]=0.0f;
 }
 void TwoSided() { back.Instantiate(); }
 void Shininess( float val ) {
  shininess[0]=val;
  if ( shininess[0] < 0.0f ) shininess[0]=0.0f;
  if ( shininess[0] > 128.0f ) shininess[0]=128.0f;
 }
 void Apply() {
  if ( back ) {
   glMaterialfv(GL_FRONT, GL_AMBIENT, ambient.coord );
   glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse.coord );
   glMaterialfv(GL_FRONT, GL_SPECULAR, specular.coord );
   glMaterialfv(GL_FRONT, GL_EMISSION, emission.coord );
   glMaterialfv(GL_FRONT, GL_SHININESS, shininess );
   glMaterialfv(GL_BACK, GL_AMBIENT, back->ambient.coord );
   glMaterialfv(GL_BACK, GL_DIFFUSE, back->diffuse.coord );
   glMaterialfv(GL_BACK, GL_SPECULAR, back->specular.coord );
   glMaterialfv(GL_BACK, GL_EMISSION, back->emission.coord );
   glMaterialfv(GL_BACK, GL_SHININESS, back->shininess );
  } else {
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient.coord );
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse.coord );
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular.coord );
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission.coord );
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess );
  }
 }
};

struct GLMaterials : public LinkedList {
 GLMaterial *find( const char *name ) {
  FOREACH(GLMaterial,m) if ( !str_cmp(m->name.c_str(),name) ) return m;
  return null;
 }
};

struct ImmediateLighting {
 Indexed<ImmediateLight> light;
 Coord4 ambient;
 Zbool smooth,twoSided,local,depth;
 SKEYSTART
  SKEY("ambient", { SKEYPAIR; ambient.fromString((char *) key.c_str()); })
  SKEY("light0",{ SKEYPAIR; light[0].fromString((char *) key.c_str()); })
  SKEY("light1",{ SKEYPAIR; light[1].fromString((char *) key.c_str()); })
  SKEY("light2",{ SKEYPAIR; light[2].fromString((char *) key.c_str()); })
  SKEY("light3",{ SKEYPAIR; light[3].fromString((char *) key.c_str()); })
  SKEY("light4",{ SKEYPAIR; light[4].fromString((char *) key.c_str()); })
  SKEY("light5",{ SKEYPAIR; light[5].fromString((char *) key.c_str()); })
  SKEY("light6",{ SKEYPAIR; light[6].fromString((char *) key.c_str()); })
  SKEY("light7",{ SKEYPAIR; light[7].fromString((char *) key.c_str()); })
  SKEY("smooth",{ smooth=true; });
  SKEY("2sided",{ twoSided=true; });
  SKEY("local", { local=true; });
  SKEY("depth", { depth=true; });
 SKEYEND("ImmediateLighting");
 string toString() {
  string out=string("");
  incdent();
  if ( ambient.coord[0] != 0.2f || ambient.coord[1] != 0.2f || ambient.coord[2] != 0.2f || ambient.coord[3] != 1.0f )
   out+=indention+string("ambient {")+ambient.toString()+string("}\n");
  if ( smooth ) { out+=indention+string("smooth\n"); }
  if ( twoSided ) { out+=indention+string("2sided\n"); }
  if ( local ) { out+=indention+string("local\n"); }
  if ( depth ) { out+=indention+string("depth\n"); }
  for ( int i=0; i<8; i++ ) {
   FORMAT(buf,16,"light%d {", (int) i);
   out+=indention+string(buf)+light[i].toString()+string("}\n");
  }
  decdent();
  return out;
 }
 ImmediateLighting() {
  ambient.Set(0.2f,0.2f,0.2f,1.0f);
  light.Size(8);
  smooth=true;
  depth=true;
  SampleLighting();
 }
 void Dark() {
  for ( int i=0; i<8; i++ ) light[i].on=false;
 }
 void SampleLighting() {
  light[0].diffuse.Set(1.0f, 0.9f, 0.8f, 1.0f);
  light[0].position.Set( 1.0f,  1.0f, -1.0f, 0.0f); 
  light[0].on=true;
  light[1].diffuse.Set(0.8f, 0.0f, 0.8f, 1.0f);
  light[1].position.Set(-1.0f,  1.0f,  1.0f, 0.0f); 
  light[1].on=true;
  light[2].diffuse.Set(1.0f, 0.3f, 0.2f, 1.0f);
  light[2].position.Set( 1.0f, -1.0f,  1.0f, 0.0f); 
  light[2].on=true;
  light[3].diffuse.Set(0.0f, 0.3f, 1.0f, 1.0f);
  light[3].position.Set( 0.0f,  0.0f,  5.0f, 0.0f); // at default eye (above)
  light[3].on=true;
  light[4].diffuse.Set(0.5f, 0.0f, 0.4f, 1.0f);
  light[4].position.Set( 0.0f,  -5.0f,  0.0f, 0.0f);
  light[4].on=true;
 }
 void On() {
  if ( smooth ) glShadeModel(GL_SMOOTH); else glShadeModel(GL_FLAT);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, (local?GL_TRUE:GL_FALSE) );
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, (twoSided?GL_TRUE:GL_FALSE) );
  glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  if ( depth ) glEnable(GL_DEPTH_TEST); /*else glDisable(GL_DEPTH_TEST); <- done elsewhere */
  glEnable(GL_LIGHTING);
  for ( int i=0; i<8; i++ ) if ( light[i].on ) {
   glLightfv(GL_LIGHT0+i, GL_DIFFUSE, light[i].diffuse.coord);
   glLightfv(GL_LIGHT0+i, GL_AMBIENT, light[i].ambient.coord);
   glLightfv(GL_LIGHT0+i, GL_SPECULAR, light[i].specular.coord);
   if ( light[i].spot ) {
    glLightfv(GL_LIGHT0+i, GL_SPOT_DIRECTION, light[i].direction.coord);
   }
   glLightfv(GL_LIGHT0+i, GL_POSITION, light[i].position.coord);
   glLightf(GL_LIGHT0+i, GL_SPOT_CUTOFF, light[i].cutoff);
   glLightf(GL_LIGHT0+i, GL_SPOT_EXPONENT, light[i].exponent);
   glLightf(GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, light[i].constant );
   glLightf(GL_LIGHT0+i, GL_LINEAR_ATTENUATION, light[i].linear );
   glLightf(GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, light[i].quadratic );
   glEnable(GL_LIGHT0+i); // Red book chapter 5 "Enable Lighting"
  } else glDisable(GL_LIGHT0+i);
 }
 void Off() {
  if ( depth ) glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  for ( int i=0; i<8; i++ ) if ( light[i].on ) glDisable(GL_LIGHT0+i);
 }
 void Randomize( int seed, int lamps ) {
  Random *set=randoms.GetSet(seed);
  Crayon color;
  for ( int i=0; i < 8; i++ ) light[i].on=false;
  for ( int i=0; i < lamps; i++ ) {
   color.Brights(seed+i); 
   light[i].diffuse.Set(color.rf, color.gf, color.bf, 1.0f);
   float scaleX=5.0f-10.0f*set->unique(i+seed,i,i+seed);
   float scaleY=5.0f-10.0f*set->unique(i+seed,seed+i,i+seed);
   float scaleZ=5.0f-10.0f*set->unique(i,i+seed,i+seed);
   light[i].position.Set(
    scaleX*set->unique(seed,i,i),
    scaleY*set->unique(i,seed,i),
    scaleZ*set->unique(i,i,seed),
    0.0f
   ); 
   light[i].on=true;
  }
 }
};
