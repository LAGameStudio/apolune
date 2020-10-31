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

#pragma once
#include <Windows.h>
#include "GLSetup.h"

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"

#include "GLBlend.h"
#include "Crayon.h"
#include "Art.h"

#include "TextureLibrary.h"

#include "GLSL2Shader.h"
#include "fx_Shader2d.h"

#include "Shader.h"
#include "Lights.h"

#include "dhpowareMath.h"

/*
 It is important to know that only the DIFFUSE colour's alpha value
 actually determines the transparency of the polygon. If you have
 taken my advice and used glColorMaterial to cause glColor to drive
 the ambient and diffuse components then this seems perfectly natural...
 but people sometimes want to use the glColor to drive one of the other
 material components and are then very confused about their inability
 to set the transparency using glColor. If that happens to you -
 remember to use glMaterial to set the diffuse colour - and hence the
 alpha for the polygon overall.
 */

//class ShaderProgram;
//class LightList;
//class Lights;

class VBO;
class gCamera;
class Scene;

enum MaterialTypes {
 WorldShaded=0, // Uses the "WorldShader" class
 PhongTexture=1, // Uses the "phong" GL2-style shader with Phong lighting
 Shaded=2, // GLSL2 fx_Shader class (shader amalgam)
 TextureMapped=3, // GL_SMOOTH GL_LIGHTING ImmediateMode class
 VertexColor=4,
 VertexMaterial=5, // Also known as a "Color-driven Material"
 FlatShaded=6,
 SolidShaded=7, // Lightless emissive
 WireFrame=8,
 WireVertexShaded=9,
 TextureVertexShaded=10
};

enum MaterialCategory {
 SolidColor=0,
 GlassAlpha=1,
 Textured=2,
 VertexColored=3,
 ShadedSurface=4,
 WireFramed=5
};

// A default fixed-function, shader-ish Material class.
class Material : public ListItem {
public:
 string name;
 MaterialCategory category;
 MaterialTypes type;
 Blends blend;
 Strings options;
 int lights;

 // Color/lighting attributes
 Crayon ambient_color;
 Crayon diffuse_color;
 Crayon specular_color;
 Crayon emission_color;
 GLfloat shininess;

 // Flat
 Crayon color;
 
 // Simple Texturing
 GLImage *texture;

 // "Simple" GL2 shaders that come packaged with their own uniform managers, in a manager
 fx_Shader2d shader;

 // "Super" Shaders
 string vertex, fragment, geometry, tesselation;
 string eq,ll;
 ShaderProgram *program;

 void Constructor() {
  lights=1;
  next=prev=null;
  name=string("");
  vertex=string("");
  fragment=string("");
  geometry=string("");
  tesselation=string("");
  ambient_color.Float(0.1f,0.1f,0.1f,1.0f);
  diffuse_color.Float(1.0f,1.0f,1.0f,1.0f);
  specular_color.Float(1.0f,1.0f,1.0f,1.0f);
  emission_color.Float(1.0f,1.0f,1.0f,1.0f);
  shininess=60.0f;
  program=null;
  eq=string("");
  ll=string("");
  blend=none;
  type=VertexColor;
  apply=&Material::VertexApply;
  unapply=&Material::VertexUnapply;
  texture=null;
  category=SolidColor;
 }

 bool NeedsColorComponent() {
  return
      ( type == VertexColor
     || type == WireVertexShaded
     || type == TextureVertexShaded ) ;
 }

 void AssignShader( const char *prefix );
 void AssignShader( const char *prefix, int num_lights );
 void AssignShader( const char *prefix, int num_lights, const char *equation, const char *lightloop );
 void AssignShader( const char *v, const char *f );
 void DetachShader();


 // This is our standard application for non-super-shaded
 void Nothing() {}
 PROGRAMMABLE(Material,ApplyPtr,apply,Apply);
 PROGRAMMABLE(Material,UnapplyPtr,unapply,Unapply);

 void ShadedApply() {
  shader.Update(); // Put here to ensure settings are set right before render. Calls "each"
  GL_Report( "Material:fx_Shader2d::Render (entering)" );
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  Blending(blend); // Note: uses the material not the shader settings
  shader.active->UpdateUniforms();
 }

 void ShadedUnapply() {
  shader.active->disableShader();
  glPopAttrib();
  GL_Report( "Material:fx_Shader2d::Render" );
 }

 void TexturedApply() {
//  glPushAttrib(GL_ALL_ATTRIB_BITS);
  Blending(blend);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture->texture);
 }

 void TexturedUnapply() {
  GL_Report( "Material::TexturedUnapply() Pre" );
//  glDisable(GL_TEXTURE_2D);
//  glDisable(GL_BLEND);
//  glPopAttrib();
  GL_Report( "Material::TexturedUnapply() Post" );
 }

 // Vertex shaded
 void VertexApply() {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  Blending(blend);
  //glDisable(GL_LIGHTING);
 }

 void VertexUnapply() {
  glPopAttrib();
 }

 void VertexMaterialApply() {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  Blending(blend);
  glColor4fv(color.floats);
  glEnable(GL_COLOR_MATERIAL);
  glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emission_color.floats ) ;
  glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT,  ambient_color.floats ) ;
  glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,  diffuse_color.floats ) ;
  glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specular_color.floats ) ;
 }

 void VertexMaterialUnapply() {
  glPopAttrib();
 }

 // Flat shaded
 void FlatApply() {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  Blending(blend);
  glColor4fv(color.floats);
 }

 void FlatUnapply() {
  glPopAttrib();
 }

 // Solid emissive shaded
 void SolidApply() {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  Blending(blend);
  glDisable(GL_LIGHTING);
  glColor4fv(color.floats);
 }

 void SolidUnapply() {
  glPopAttrib();
 }

 // Wireframe
 void WireApply() {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPolygonMode(GL_FRONT,GL_LINE);
 }

 void WireUnapply() {
  glPopAttrib();
 }

 void WireVertexApply() {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPolygonMode(GL_FRONT,GL_LINE);
 }

 void WireVertexUnapply() {
  glPopAttrib();
 }

 // Textured by vertex
 void TextureVertexApply() {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  Blending(blend);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture->texture);
 }

 void TextureVertexUnapply() {
  GL_Report( "Material::TexturedUnapply() Pre" );
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  glPopAttrib();
  GL_Report( "Material::TexturedUnapply() Post" );
 }

 void SetName( const char *n ) {
  name=string(n);
 }

 void SetType( MaterialTypes t ) {
  switch ( t ) {
   case WorldShaded: // Occurs in the big Apply function below instead
     apply=&Material::Nothing;
     unapply=&Material::Nothing;
     category=ShadedSurface;
    break;
   case Shaded:
     apply=&Material::ShadedApply;
     unapply=&Material::ShadedUnapply;
     category=ShadedSurface;
    break;
   case TextureMapped:
     apply=&Material::TexturedApply;
     unapply=&Material::TexturedUnapply;
     category=Textured;
    break;
   case VertexColor:
     apply=&Material::VertexApply;
     unapply=&Material::VertexUnapply;
     category=VertexColored;
    break;
   case VertexMaterial:
     apply=&Material::VertexMaterialApply;
     unapply=&Material::VertexMaterialUnapply;
     category=VertexColored;
    break;
   case FlatShaded:
     apply=&Material::FlatApply;
     unapply=&Material::FlatUnapply;
     category=SolidColor;
    break;
   case SolidShaded:
     apply=&Material::SolidApply;
     unapply=&Material::SolidUnapply;
     category=SolidColor;
    break;
   case WireFrame:
     apply=&Material::WireApply;
     unapply=&Material::WireUnapply;
     category=SolidColor;
    break;
   case WireVertexShaded:
     apply=&Material::WireVertexApply;
     unapply=&Material::WireVertexUnapply;
     category=WireFramed;
    break;
   case TextureVertexShaded:
     apply=&Material::TextureVertexApply;
     unapply=&Material::TextureVertexUnapply;
     category=VertexColored;
    break;
  }
 }

 // This is our "apply" for SuperShading
 // Scene: Make sure to use InitUniforms after calling this.
 virtual void Apply( 
   float dt,
   float rotationDegrees,
   Vertex *offset,
   Vertex *position,
   Vertex *scale,
   Vertex *rotation,
   Vector3 rotationAxis,
   Quaternionf * q,
   VBO *vbo,
   gCamera *camera,
   Scene *scene );

 virtual void InitUniforms() {}
 virtual void UpdateUniforms() {}
 virtual void UpdateLightList( LightList *lux ) {}
 virtual void UpdateLights( Lights *lux ) {}

 virtual void ShaderDebugger( GLWindow *win ) {}

 virtual Material *Duplicate() { return null; }

  Material() { Constructor(); }
 ~Material() { }
};

// "Singleton"
class Materials : public LinkedList {
public:
 /* Test Materials */
 Material defaultMaterial;
 Material colorMaterial;
 Material flatMaterial;
 Material solidMaterial;
 Material vertexMaterial;
 Material wireMaterial;
 Material wireVertexMaterial;

 Materials() : LinkedList() {
  colorMaterial.SetType( VertexMaterial );
  colorMaterial.diffuse_color.Pick(green);
  colorMaterial.SetName( "Color Material" );
  solidMaterial.SetType( SolidShaded );
  solidMaterial.SetName( "Solid Material" );
  flatMaterial.SetType( FlatShaded );
  flatMaterial.SetName( "Flat Material" );
  vertexMaterial.SetType( VertexColor );
  vertexMaterial.SetName( "Vertex Material" );
  wireMaterial.SetType( WireFrame );
  wireMaterial.SetName( "Wire Material" );
  wireVertexMaterial.SetType( WireVertexShaded );
  wireVertexMaterial.SetName( "Wire Vertex Material" );
 }
 // Create A Default Material
 Material *Add( const char *name, MaterialTypes m ) {
  Material *mat = new Material;
  Append(mat);
  switch ( m ) {
  case VertexMaterial:
    mat->SetType( VertexMaterial );
   break;
  case SolidShaded:
    mat->SetType( SolidShaded );
   break;
  case VertexColor:
    mat->SetType( VertexColor );
   break;
  case WireFrame:
    mat->SetType( WireFrame );
   break;
  case WireVertexShaded:
    mat->SetType( WireVertexShaded );
   break;
  }
  return mat;
 }
 // Create A Solid Material
 Material *Add( const char *name, Crayon c ) {
  Material *m=new Material;
  m->SetType( SolidShaded );
  m->SetName( name );
  m->color.fromCrayon(c);
  Append(m);
  return m;
 }
 // Create a Flat Material
 Material *Flat( const char *name, Crayon c ) {
  Material *m=new Material;
  m->SetType( FlatShaded );
  m->SetName( name );
  m->color.fromCrayon(c);
  Append(m);
  return m;
 }
 // Create a Flat Material
 Material *Wire( const char *name, Crayon c ) {
  Material *m=new Material;
  m->SetType( WireFrame );
  m->SetName( name );
  m->color.fromCrayon(c);
  Append(m);
  return m;
 }
 // Create a Vertex Material
 Material *Add( const char *name, Crayon e, Crayon d, Crayon a, Crayon s ) {
  Material *m=new Material;
  m->SetType( VertexMaterial );
  m->SetName( name );
  m->emission_color.fromCrayon(e);
  m->diffuse_color.fromCrayon(d);
  m->ambient_color.fromCrayon(a);
  m->specular_color.fromCrayon(s);
  Append(m);
  return m;
 }
 // Create a Vertex Material
 Material *Alpha( const char *name, Crayon e, Crayon d, Crayon a, Crayon s ) {
  Material *m=new Material;
  m->SetType( VertexMaterial );
  m->SetName( name );
  m->emission_color.fromCrayon(e);
  m->diffuse_color.fromCrayon(d);
  m->ambient_color.fromCrayon(a);
  m->specular_color.fromCrayon(s);
  m->blend=transparency;
  Append(m);
  return m;
 }
 // Constrained by the method used while loading,
 // prefers wrapping textures one or more axis.
 Material *Add(MaterialCategory category, const char *name, GLImage *i ) {
  Material *m=new Material;
  m->SetName(name);
  m->SetType(TextureMapped);
  m->texture=i;
  Append(m);
  return m;
 }
 Material *RandomTestMaterial() {
  switch ( number_range(0,100)%7 ) {
   case 0: return &colorMaterial;
   case 1: return &solidMaterial;
   case 2: return &flatMaterial;
   case 3: return &vertexMaterial;
   case 4: return &wireMaterial;
   case 5: return &wireVertexMaterial;
  default: return &defaultMaterial;
  }
 }
 Material *Random() {
  return (Material *) Element(upto(count));
 }
 Material *Random( MaterialCategory category ) {
  Material *m;
  m= (Material *) Element(upto(count));
  while ( m->category != category ) m= (Material *) Element(upto(count));
  return m;
 }
 Material *find( const char *prefix, int num_lights ) {
  for ( Material *m=(Material *) first; m; m=(Material *) m->next )
   if ( m->lights == num_lights && !str_cmp(m->name.c_str(),prefix) ) return m;
  OUTPUT( "Materials:find Material not found: %s %d\n", prefix, (int) num_lights );
  return &defaultMaterial;
 }
 Material *find( const char *prefix, int num_lights, const char *equation, const char *lightloop ) {
  for ( Material *m=(Material *) first; m; m=(Material *) m->next )
   if ( m->lights == num_lights && !str_cmp(m->name.c_str(),prefix) && m->eq==string(equation) && m->ll==string(lightloop) ) return m;
  OUTPUT( "Materials:find Material not found: %s %d %s %s\n", 
   prefix, (int) num_lights, equation, lightloop );
  return &defaultMaterial;
 }
 Material *find( const char *prefix ) { return find( prefix, 1 ); }
 void Update() {
  for ( Material *m=(Material*)first; m; m=(Material*)m->next ) {
   m->UpdateUniforms();
  }
 }
 CLEARLISTRESET(Material);
 ~Materials() { Clear(); }
};

HANDLED(Material,MaterialHandle,MaterialHandles,MaterialHandlesHandle,MaterialsHandlesHandles);

extern Materials materials;