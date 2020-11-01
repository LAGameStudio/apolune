/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once

#include "GLImage.h"
#include "Shader.h"
#include "Lights.h"
#include "Material.h"
#include "TextureLibrary.h"
#include "OldVBO.h"
#include "Camera.h"
#include "Scene.h"
 
#include "TexFont.h"

//#define DEBUG_WORLDSHADER 1

extern GLSetup gl;
extern ShaderLibrary shaders;

// The "world shader" is a special layered texture written explicitly for the 
// purpose of providing a wide range of features that are homogenous and
// programmable.
//
// The following class implements this shader's maintenance class, which wraps
// around the shader stored at WORLD_SHADER_LOCATION
//
// To add new materials to the framework, you should use a similar method to
// this class, but this class is very much written specifically for the
// features outlined in the world shader that comes with the framework.
//
// Use of this and other Material child-classes impact Scene and VBO classes.
//
// The following class establishes a "material description file", which is an ascii
// text file, provided to the Load() member function, which contains a series of
// keywords and values, one complete set per line, which meets the criteria set forth
// inside the Load() member function.

#define WORLD_SHADER_LOCATION "data/shaders/worldshader"

class WorldShader : public Material {
public:
 GLImage *pNullColorMapTexture;
 GLImage *pColorMapTexture;
 GLImage *pGlossMapTexture;
 GLImage *pNormalMapTexture;
 GLImage *pHeightMapTexture;
 GLImage *pLuminanceMapTexture;
 GLImage *pEnvMapTexture;
 GLImage *pAdditiveMapTexture;
 GLImage *pSubtractiveMapTexture;
 GLImage *pAdditiveMap2Texture;
 GLImage *pSubtractiveMap2Texture;
 
	float ParallaxScaleBias[2];
 float TexAmp;
 float EnvClamp;
 float MixRatio;
 float Add1;
 float Add2;
 float Sub1;
 float Sub2;
 float Goochness;
 Crayon SurfaceColor;
 Crayon WarmColor;
 Crayon CoolColor;
 float DiffuseWarm;
 float DiffuseCool;
 float OverGoochness;
 Crayon OverSurfaceColor;
 Crayon OverWarmColor;
 Crayon OverCoolColor;
 float OverDiffuseWarm;
 float OverDiffuseCool;
 Crayon StaticColor;
 float Luminance;
 Crayon LuminanceTint;
 bool colorMapDisabled;

 WorldShader() { Constructor(); Init(); this->type=WorldShaded; }
 ~WorldShader() {}

 void Init() {
  Constructor();

  pNullColorMapTexture
  =pColorMapTexture
  =pGlossMapTexture
  =pNormalMapTexture
  =pHeightMapTexture
  =pLuminanceMapTexture
  =pEnvMapTexture
  =pAdditiveMapTexture
  =pSubtractiveMapTexture
  =pAdditiveMap2Texture
  =pSubtractiveMap2Texture=library.find("data/images/elements/black.png");

  colorMapDisabled=false;
  ParallaxScaleBias[0]=0.04f; ParallaxScaleBias[1]=-0.03f;
  TexAmp=1.0f;
  EnvClamp=1.0f;
  MixRatio=0.5f;
  Add1=1.0f;
  Add2=1.0f;
  Sub1=1.0f;
  Sub2=1.0f;
  Goochness=0.0f;
  SurfaceColor.Float( 0.75f, 0.75f, 0.75f, 1.0f );
  WarmColor.Float( 0.6f, 0.0f, 0.0f, 1.0f  );
  CoolColor.Float( 0.0f, 0.0f, 0.6f, 1.0f  );
  DiffuseWarm=0.45f;
  DiffuseCool=0.45f;
  OverGoochness=0.0f;
  OverSurfaceColor.Float( 0.6f, 0.6f, 0.6f, 1.0f );
  OverWarmColor.Float( 6.0f, 0.0f, 0.0f, 1.0f );
  OverCoolColor.Float( 0.0f, 0.0f, 0.6f, 1.0f );
  OverDiffuseWarm=0.45f;
  OverDiffuseCool=0.45f;
  StaticColor.Float( 0.0f, 0.5f, 0.0f, 1.0f );
  Luminance=1.0f;
  LuminanceTint.Float( 1.0f, 1.0f, 1.0f, 1.0f );
  lights=1;
 }

 void SetNumberOfLights( int nlights ) {
  AssignShader( WORLD_SHADER_LOCATION, lights=nlights );
  UpdateUniforms();
 }

 void Load( const char *mtl, int num_lights ) {
  lights=num_lights;
  Load(mtl);
 }

 void Load( const char *mtl ) {
  static char arg[1024];
  Crayon c;
  Strings *file=new Strings(mtl);

  arg[0]='\0';
	 if (!(pNullColorMapTexture = library.find("data/images/elements/white.png"))) {
   OUTPUT("WorldShader:Load is unable to load texture: data/images/elements/white.png");
   return;
  }

  for ( String *line=(String *) (file->first); line; line=(String *) (line->next) ) {
   const char *p=one_argument(line->s.c_str(),arg);
   if ( arg[0] == '\0' || *p=='\0' ) continue;
   if ( !str_cmp(arg,"ColorMap") ) {
    p=one_argument(p,arg);
    pColorMapTexture = library.find(arg);
   } else
   if ( !str_cmp(arg,"GlossMap") ) {
    p=one_argument(p,arg);
    pGlossMapTexture = library.find(arg);
   } else
   if ( !str_cmp(arg,"NormalMap") ) {
    p=one_argument(p,arg);
    pNormalMapTexture = library.find(arg);
   } else
   if ( !str_cmp(arg,"HeightMap") ) {
    p=one_argument(p,arg);
    pHeightMapTexture = library.find(arg);
   } else
   if ( !str_cmp(arg,"LuminanceMap") ) {
    p=one_argument(p,arg);
    pLuminanceMapTexture = library.find(arg);
   } else
   if ( !str_cmp(arg,"EnvMap") ) {
    p=one_argument(p,arg);
    pEnvMapTexture = library.find(arg,GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT);
   } else
   if ( !str_cmp(arg,"AdditiveMap") ) {
    p=one_argument(p,arg);
    pAdditiveMapTexture = library.find(arg);
   } else
   if ( !str_cmp(arg,"SubtractiveMap") ) {
    p=one_argument(p,arg);
    pSubtractiveMapTexture = library.find(arg);
   } else
   if ( !str_cmp(arg,"AdditiveMap2") ) {
    p=one_argument(p,arg);
    pAdditiveMap2Texture = library.find(arg);
   } else
   if ( !str_cmp(arg,"SubtractiveMap2") ) {
    p=one_argument(p,arg);
    pSubtractiveMap2Texture = library.find(arg);
   } 
   else
   if ( !str_cmp(arg,"ParallaxScaleBias") ) {
    p=ParseFloat(p,&ParallaxScaleBias[0]);
    p=ParseFloat(p,&ParallaxScaleBias[1]);
   }
   else if ( !str_cmp(arg,"TexAmp"           ) ) { p=ParseFloat(p,&TexAmp);          }
   else if ( !str_cmp(arg,"EnvClamp"         ) ) { p=ParseFloat(p,&EnvClamp);        }
   else if ( !str_cmp(arg,"MixRatio"         ) ) { p=ParseFloat(p,&MixRatio);        }
   else if ( !str_cmp(arg,"Add1"             ) ) { p=ParseFloat(p,&Add1);            }
   else if ( !str_cmp(arg,"Add2"             ) ) { p=ParseFloat(p,&Add2);            }
   else if ( !str_cmp(arg,"Sub1"             ) ) { p=ParseFloat(p,&Sub1);            }
   else if ( !str_cmp(arg,"Sub2"             ) ) { p=ParseFloat(p,&Sub2);            }
   else if ( !str_cmp(arg,"Add1"             ) ) { p=ParseFloat(p,&Add1);            }
   else if ( !str_cmp(arg,"Goochness"        ) ) { p=ParseFloat(p,&Goochness);       }
   else if ( !str_cmp(arg,"SurfaceColor"     ) ) { p=SurfaceColor.Parse(p,3);        }
   else if ( !str_cmp(arg,"WarmColor"        ) ) { p=WarmColor.Parse(p,3);           }
   else if ( !str_cmp(arg,"CoolColor"        ) ) { p=CoolColor.Parse(p,3);           }
   else if ( !str_cmp(arg,"DiffuseWarm"      ) ) { p=ParseFloat(p,&DiffuseWarm);     }
   else if ( !str_cmp(arg,"DiffuseCool"      ) ) { p=ParseFloat(p,&DiffuseCool);     }
   else if ( !str_cmp(arg,"OverGoochness"    ) ) { p=ParseFloat(p,&OverGoochness);   }
   else if ( !str_cmp(arg,"OverSurfaceColor" ) ) { p=OverSurfaceColor.Parse(p,3);    }
   else if ( !str_cmp(arg,"OverWarmColor"    ) ) { p=OverWarmColor.Parse(p,3);       }
   else if ( !str_cmp(arg,"OverCoolColor"    ) ) { p=OverCoolColor.Parse(p,3);       }
   else if ( !str_cmp(arg,"OverDiffuseWarm"  ) ) { p=ParseFloat(p,&OverDiffuseWarm); }
   else if ( !str_cmp(arg,"OverDiffuseCool"  ) ) { p=ParseFloat(p,&OverDiffuseCool); }
   else if ( !str_cmp(arg,"StaticColor"      ) ) { p=StaticColor.Parse(p,4);         }
   else if ( !str_cmp(arg,"Luminance"        ) ) { p=ParseFloat(p,&Luminance);       }
   else if ( !str_cmp(arg,"LuminanceTint"    ) ) { p=LuminanceTint.Parse(p,4);       }
   else if ( !str_cmp(arg,"diffuse"          ) ) { p=diffuse_color.Parse(p,4);       }
   else if ( !str_cmp(arg,"ambient"          ) ) { p=ambient_color.Parse(p,4);       }
   else if ( !str_cmp(arg,"specular"         ) ) { p=specular_color.Parse(p,4);      }
   else if ( !str_cmp(arg,"shininess"        ) ) { p=ParseFloat(p,&shininess);       }
  }// END FILE PARSING
  delete file;

  AssignShader( "data/shaders/worldshader", lights );
 }

 // Uniform locations
 GLint UColorMapSampler, UGlossMapSampler, UNormalMapSampler,
  UHeightMapSampler, ULuminanceMapSampler, UEnvMapSampler,
  UAdditiveMapSampler, USubtractiveMapSampler, UAdditiveMap2Sampler,  USubtractiveMap2Sampler,
  UTexAmp, UEnvClamp, UMixRatio, UAdd1, UAdd2, USub1, USub2, 
  ULuminance, ULuminanceTint, UGoochness, USurfaceColor, UWarmColor, UCoolColor, UDiffuseWarm, UDiffuseCool,
  UOverGoochness, UOverSurfaceColor, UOverWarmColor, UOverCoolColor, UOverDiffuseWarm, UOverDiffuseCool,
  UMaterial1ambient, UMaterial1diffuse, UMaterial1specular, UMaterial1shininess;

 // Initializes or assigns existing uniforms.
 virtual void InitUniforms() {
  UColorMapSampler=glGetUniformLocation(program->handle, "ColorMapSampler");
  UGlossMapSampler=glGetUniformLocation(program->handle, "GlossMapSampler");
  UNormalMapSampler=glGetUniformLocation(program->handle, "NormalMapSampler");
  UHeightMapSampler=glGetUniformLocation(program->handle, "HeightMapSampler");
  ULuminanceMapSampler=glGetUniformLocation(program->handle, "LuminanceMapSampler");
  UEnvMapSampler=glGetUniformLocation(program->handle, "EnvMapSampler");
  UAdditiveMapSampler=glGetUniformLocation(program->handle, "AdditiveMapSampler");
  USubtractiveMapSampler=glGetUniformLocation(program->handle, "SubtractiveMapSampler");
  UAdditiveMap2Sampler=glGetUniformLocation(program->handle, "AdditiveMap2Sampler");
  USubtractiveMap2Sampler=glGetUniformLocation(program->handle, "SubtractiveMap2Sampler");
  
  UTexAmp=glGetUniformLocation(program->handle, "TexAmp");
  UEnvClamp=glGetUniformLocation(program->handle, "EnvClamp");
  UMixRatio=glGetUniformLocation(program->handle, "MixRatio");
  UAdd1=glGetUniformLocation(program->handle, "Add1");
  UAdd2=glGetUniformLocation(program->handle, "Add2");
  USub1=glGetUniformLocation(program->handle, "Sub1");
  USub2=glGetUniformLocation(program->handle, "Sub2");
  ULuminance=glGetUniformLocation(program->handle, "Luminance");
  ULuminanceTint=glGetUniformLocation(program->handle, "LuminanceTint");
  
  UGoochness=glGetUniformLocation(program->handle, "Goochness");
  USurfaceColor=glGetUniformLocation(program->handle, "SurfaceColor");
  UWarmColor=glGetUniformLocation(program->handle, "WarmColor");
  UCoolColor=glGetUniformLocation(program->handle, "CoolColor");
  UDiffuseWarm=glGetUniformLocation(program->handle, "DiffuseWarm");
  UDiffuseCool=glGetUniformLocation(program->handle, "DiffuseCool");
  
  UOverGoochness=glGetUniformLocation(program->handle, "OverGoochness");
  UOverSurfaceColor=glGetUniformLocation(program->handle, "OverSurfaceColor");
  UOverWarmColor=glGetUniformLocation(program->handle, "OverWarmColor");
  UOverCoolColor=glGetUniformLocation(program->handle, "OverCoolColor");
  UOverDiffuseWarm=glGetUniformLocation(program->handle, "OverDiffuseWarm");
  UOverDiffuseCool=glGetUniformLocation(program->handle, "OverDiffuseCool");

  UMaterial1ambient=glGetUniformLocation(program->handle, "Material1.ambient");
  UMaterial1diffuse=glGetUniformLocation(program->handle, "Material1.diffuse");
  UMaterial1specular=glGetUniformLocation(program->handle, "Material1.specular");
  UMaterial1shininess=glGetUniformLocation(program->handle, "Material1.shininess");
 }

 virtual void AttachVBO( VBO16 *v ) {
  v->Bind();
   
  // Map the generic vertex attributes used by the cube's shader program to
  // the cube's vertex data stored in the vertex buffer object.
  
  GLint variable = glGetAttribLocation(program->handle, "VertexPosition");
  glVertexAttribPointer(variable, 3, GL_FLOAT, GL_FALSE, sizeof(VBO_Vertex16), BUFFER_OFFSET(0));
  glEnableVertexAttribArray(variable);
  
  variable = glGetAttribLocation(program->handle, "VertexNormal");
  glVertexAttribPointer(variable, 3,   GL_FLOAT, GL_FALSE, sizeof(VBO_Vertex16), BUFFER_OFFSET(sizeof(GLfloat) * 3));
  glEnableVertexAttribArray(variable);                   
  
  variable = glGetAttribLocation(program->handle, "VertexTexCoord");
  glVertexAttribPointer(variable, 2, GL_FLOAT, GL_FALSE, sizeof(VBO_Vertex16), BUFFER_OFFSET(sizeof(GLfloat) * (3+3)));
  glEnableVertexAttribArray(variable);             
  
  variable = glGetAttribLocation(program->handle, "VertexColor");
  glVertexAttribPointer(variable, 4,    GL_FLOAT, GL_FALSE, sizeof(VBO_Vertex16), BUFFER_OFFSET(sizeof(GLfloat) * (6+2)) );
  glEnableVertexAttribArray(variable);
  
  variable = glGetAttribLocation(program->handle, "VertexTangent");
  glVertexAttribPointer(variable, 4,  GL_FLOAT, GL_FALSE, sizeof(VBO_Vertex16), BUFFER_OFFSET(sizeof(GLfloat) * (6+2+4)));
  glEnableVertexAttribArray(variable);

  v->Unbind();
 }

 // Assigns current uniform state.
 virtual void UpdateUniforms() {
  
  glUseProgram(program->handle);

  // Assign texture IDs used by glActiveTexture (see Apply)
  glUniform1i(UColorMapSampler,        0);
  glUniform1i(UGlossMapSampler,        1);
  glUniform1i(UNormalMapSampler,       2);
  glUniform1i(UHeightMapSampler,       3);
  glUniform1i(ULuminanceMapSampler,    4);
  glUniform1i(UEnvMapSampler,          5);
  glUniform1i(UAdditiveMapSampler,     6);
  glUniform1i(USubtractiveMapSampler,  7);
  glUniform1i(UAdditiveMap2Sampler,    8);
  glUniform1i(USubtractiveMap2Sampler, 9);

  glUniform1f(UTexAmp,        TexAmp);
  glUniform1f(UEnvClamp,      EnvClamp);
  glUniform1f(UMixRatio,      MixRatio);
  glUniform1f(UAdd1,          Add1);
  glUniform1f(UAdd2,          Add2);
  glUniform1f(USub1,          Sub1);
  glUniform1f(USub2,          Sub2);
  glUniform1f(ULuminance,     Luminance);
  glUniform4f(ULuminanceTint, LuminanceTint.rf, LuminanceTint.gf, LuminanceTint.bf, LuminanceTint.af );

  glUniform1f(UGoochness,     Goochness);
  glUniform3f(USurfaceColor,  SurfaceColor.rf, SurfaceColor.gf, SurfaceColor.bf );
  glUniform3f(UWarmColor,     WarmColor.rf, WarmColor.gf, WarmColor.bf );
  glUniform3f(UCoolColor,     CoolColor.rf, CoolColor.gf, CoolColor.bf );
  glUniform1f(UDiffuseWarm,   DiffuseWarm);
  glUniform1f(UDiffuseCool,   DiffuseCool);

  glUniform1f(UOverGoochness,     Goochness);
  glUniform3f(UOverSurfaceColor,  SurfaceColor.rf, SurfaceColor.gf, SurfaceColor.bf );
  glUniform3f(UOverWarmColor,     WarmColor.rf, WarmColor.gf, WarmColor.bf );
  glUniform3f(UOverCoolColor,     CoolColor.rf, CoolColor.gf, CoolColor.bf );
  glUniform1f(UOverDiffuseWarm,   DiffuseWarm);
  glUniform1f(UOverDiffuseCool,   DiffuseCool);
  
   // Setup a basic Phong material.
  
  glUniform4f(UMaterial1ambient,   ambient_color.rf,  ambient_color.gf,  ambient_color.bf,  ambient_color.af );
  glUniform4f(UMaterial1diffuse,   diffuse_color.rf,  diffuse_color.gf,  diffuse_color.bf,  diffuse_color.af );
  glUniform4f(UMaterial1specular,  specular_color.rf, specular_color.gf, specular_color.bf, specular_color.af );
  glUniform1f(UMaterial1shininess, shininess );
 }

 // Called after/around UpdateUniforms(), and when light values change, assume you've already called MultiLightObjectAtVertex(centroid)
 // call glUseProgram() around this.
 virtual void UpdateLights( LightList *lux ) {
  static char lightParam[128];
  LightHandle *LH;
  Light *L;
  LH=(LightHandle *) lux->first;
  if ( !LH ) return;
  L=LH->light;
  for ( int i=0; i<lights; i++ ) {
   FORMAT( lightParam,128, "Light_%d.position", (int) i );     glUniform3f(glGetUniformLocation(program->handle, lightParam ), L->position.x, L->position.y, L->position.z );
   FORMAT( lightParam,128, "Light_%d.spot", (int) i );         glUniform1i(glGetUniformLocation(program->handle, lightParam ), L->type == spot_light ? 1 : 0 );
   FORMAT( lightParam,128, "Light_%d.point", (int) i );        glUniform1i(glGetUniformLocation(program->handle, lightParam ), L->type == point_light ? 1 : 0 );
   FORMAT( lightParam,128, "Light_%d.lit", (int) i );          glUniform1i(glGetUniformLocation(program->handle, lightParam ), L->lit ? 1 : 0 );
   FORMAT( lightParam,128, "Light_%d.radius", (int) i );       glUniform1f(glGetUniformLocation(program->handle, lightParam ), L->radius );
   FORMAT( lightParam,128, "Light_%d.cosInnerCone", (int) i ); glUniform1f(glGetUniformLocation(program->handle, lightParam ), L->cosInnerCone );
   FORMAT( lightParam,128, "Light_%d.cosOuterCone", (int) i ); glUniform1f(glGetUniformLocation(program->handle, lightParam ), L->cosOuterCone );
   FORMAT( lightParam,128, "Light_%d.direction", (int) i );    glUniform3f(glGetUniformLocation(program->handle, lightParam ), L->direction.x, L->direction.y, L->direction.z );
   FORMAT( lightParam,128, "Light_%d.ambient", (int) i );      glUniform4f(glGetUniformLocation(program->handle, lightParam ), L->ambient.rf, L->ambient.gf, L->ambient.bf, L->ambient.af );
   FORMAT( lightParam,128, "Light_%d.diffuse", (int) i );      glUniform4f(glGetUniformLocation(program->handle, lightParam ), L->diffuse.rf, L->diffuse.gf, L->diffuse.bf, L->ambient.af );
   FORMAT( lightParam,128, "Light_%d.specular", (int) i );     glUniform4f(glGetUniformLocation(program->handle, lightParam ), L->specular.rf, L->specular.gf, L->specular.bf, L->specular.af );
   LH=(LightHandle *) LH->next;
   if ( !LH ) return;
   L=LH->light;
  }
 }

 // Called after/around UpdateUniforms(), and when light values change, did you call MultiLightObjectAtVertex(centroid) first?
 // call glUseProgram() around this.
 virtual void UpdateLights( Lights *lux ) {
  static char lightParam[128];
  Light *L;
  L=(Light *) lux->first;
  if ( !L ) return;
  for ( int i=0; i<lights; i++ ) {
   FORMAT( lightParam,128, "Light_%d.position", (int) i );
   glUniform3f(glGetUniformLocation(program->handle, lightParam), L->position.x, L->position.y, L->position.z );
   FORMAT( lightParam,128, "Light_%d.spot", (int) i );
   glUniform1i(glGetUniformLocation(program->handle, lightParam), L->type == spot_light ? 1 : 0 );
   FORMAT( lightParam,128, "Light_%d.point", (int) i );         
   glUniform1i(glGetUniformLocation(program->handle, lightParam), L->type == point_light ? 1 : 0 );
   FORMAT( lightParam,128, "Light_%d.lit", (int) i );           
   glUniform1i(glGetUniformLocation(program->handle, lightParam), L->lit ? 1 : 0 );
   FORMAT( lightParam,128, "Light_%d.radius", (int) i );        
   glUniform1f(glGetUniformLocation(program->handle, lightParam), L->radius );
   FORMAT( lightParam,128, "Light_%d.cosInnerCone", (int) i ); 
   glUniform1f(glGetUniformLocation(program->handle, lightParam), L->cosInnerCone );
   FORMAT( lightParam,128, "Light_%d.cosOuterCone", (int) i );
   glUniform1f(glGetUniformLocation(program->handle, lightParam), L->cosOuterCone );
   FORMAT( lightParam,128, "Light_%d.direction", (int) i );
   glUniform3f(glGetUniformLocation(program->handle, lightParam), L->direction.x, L->direction.y, L->direction.z );
   FORMAT( lightParam,128, "Light_%d.ambient", (int) i );
   glUniform4f(glGetUniformLocation(program->handle, lightParam), L->ambient.rf,  L->ambient.gf,  L->ambient.bf,  L->ambient.af );
   FORMAT( lightParam,128, "Light_%d.diffuse", (int) i );
   glUniform4f(glGetUniformLocation(program->handle, lightParam), L->diffuse.rf,  L->diffuse.gf,  L->diffuse.bf,  L->diffuse.af );
   FORMAT( lightParam,128, "Light_%d.specular", (int) i );
   glUniform4f(glGetUniformLocation(program->handle, lightParam), L->specular.rf, L->specular.gf, L->specular.bf, L->specular.af );
   /*
   log.printf( "Light_%d.position %f %f %f %f", i, L->position.x, L->position.y, L->position.z );
   log.printf( "Light_%d.spot %d",              i, L->type == spot_light ? 1 : 0 );
   log.printf( "Light_%d.point %d",             i, L->type == point_light ? 1 : 0 );
   log.printf( "Light_%d.lit %d",               i, L->lit ? 1 : 0 );
   log.printf( "Light_%d.radius %f",            i, L->radius );
   log.printf( "Light_%d.cosInnerCone %f",      i, L->cosInnerCone );
   log.printf( "Light_%d.cosOuterCone %f",      i, L->cosOuterCone );
   log.printf( "Light_%d.diffuse %f %f %f %f",  i, L->diffuse.rf,  L->diffuse.gf,  L->diffuse.bf,  L->diffuse.af );
   log.printf( "Light_%d.direction %f %f %f",   i, L->direction.x, L->direction.y, L->direction.z );
   log.printf( "Light_%d.ambient %f %f %f %f",  i, L->ambient.rf,  L->ambient.gf,  L->ambient.bf,  L->ambient.af );
   log.printf( "Light_%d.specular %f %f %f %f", i, L->specular.rf, L->specular.gf, L->specular.bf, L->specular.af );
    */
   L=(Light *) L->next;
   if ( !L ) return;
  }
 }

 // Called after/around UpdateUniforms(), and when light values change, did you call MultiLightObjectAtVertex(centroid) first?
 Vertex g;
 virtual void Apply( float dt, float rotationDegrees, Vertex *offset, Vertex *position, Vertex *scale, Vertex *rotation, Vector3 rotationAxis, Quaternionf * q, VBO *vbo, gCamera *camera, Scene *scene ) {
  glGetError();

	 glUseProgram(program->handle);

  g.Set( 0.0f, 0.0f, 0.0f );
  g.Add( position, offset );
//  g.Add( &g, &(vbo16->centroid) );

  scene->lights.MultiLightObjectAtVertex(&g);
  UpdateLights( &(scene->lights) );

 	q->toAxisAngle(rotationAxis, rotationDegrees);
  Matrix4 modelMatrix = Matrix4::createRotate(rotationAxis, rotationDegrees);

  glUniform3f(glGetUniformLocation(program->handle,"RotationAngles"), rotation->x, rotation->y, rotation->z );
  glUniform3f(glGetUniformLocation(program->handle,"VerticesOffset"), position->x, position->y, position->z );
  glUniform3f(glGetUniformLocation(program->handle,"CumulativeOffset"), offset->x, offset->y, offset->z );
  glUniform3f(glGetUniformLocation(program->handle,"VerticesScale"), scale->x, scale->y, scale->z );

  Matrix4 *temp=camera->viewMatrix.asMatrix4();

	 glUniformMatrix4fv(glGetUniformLocation(program->handle, "ProjectionMatrix"), 1, GL_FALSE, &scene->projectionMatrix[0][0]);
#if defined(DEBUG_WORLDSHADER)
  string output=scene->projectionMatrix.Concatenate( "ProjectionMatrix" ); log.Push(output);
#endif
	 glUniformMatrix4fv(glGetUniformLocation(program->handle, "ViewMatrix"), 1, GL_FALSE, &(*temp)[0][0] ); // needs type optimization on this line
#if defined(DEBUG_WORLDSHADER)
   output=temp->Concatenate( "ViewMatrix" ); log.Push(output);
#endif
  glUniformMatrix4fv(glGetUniformLocation(program->handle, "ModelMatrix"), 1, GL_FALSE, &modelMatrix[0][0] ); //&v->modelMatrix[0][0]);
#if defined(DEBUG_WORLDSHADER)
   output=modelMatrix.Concatenate( "ModelMatrix" ); log.Push(output);
#endif
	 glUniformMatrix3fv(glGetUniformLocation(program->handle, "NormalMatrix"), 1, GL_FALSE, vbo->calcNormalMatrix(*temp)); // needs type optimization on this line
     delete temp;
	 glUniform2fv(glGetUniformLocation(program->handle, "ParallaxScaleBias"), 1, ParallaxScaleBias);

	 glActiveTexture(GL_TEXTURE0);
	 if (colorMapDisabled)	 	glBindTexture(GL_TEXTURE_2D, pNullColorMapTexture->texture);
	 else                   	glBindTexture(GL_TEXTURE_2D, pColorMapTexture->texture);
  
     glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, pGlossMapTexture->texture);
	 glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, pNormalMapTexture->texture);
	 glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, pHeightMapTexture->texture);
	 glActiveTexture(GL_TEXTURE4);	 glBindTexture(GL_TEXTURE_2D, pLuminanceMapTexture->texture);
	 glActiveTexture(GL_TEXTURE5);	 glBindTexture(GL_TEXTURE_2D, pEnvMapTexture->texture);
	 glActiveTexture(GL_TEXTURE6);	 glBindTexture(GL_TEXTURE_2D, pAdditiveMapTexture->texture);
	 glActiveTexture(GL_TEXTURE7);	 glBindTexture(GL_TEXTURE_2D, pSubtractiveMapTexture->texture);
	 glActiveTexture(GL_TEXTURE8);	 glBindTexture(GL_TEXTURE_2D, pAdditiveMap2Texture->texture);
	 glActiveTexture(GL_TEXTURE9);	 glBindTexture(GL_TEXTURE_2D, pSubtractiveMap2Texture->texture);

	 glBindVertexArray(vbo->vertexArrayObject);
	 glDrawArrays(GL_TRIANGLES, 0, vbo->sizeVertices);
	 glBindVertexArray(0);

     glReportError( glGetError() );
  
	 glBindTexture(GL_TEXTURE_2D, 0);
	 glUseProgram(0);
 }

#if defined(DEBUG_WORLDSHADER)
 Strings log;
#endif

 void ShaderDebugger(GLWindow *win) {
#if defined(DEBUG_WORLDSHADER)
  Stretchi(win,pColorMapTexture,       crayons.Pick(alabaster),1.0f,additive,  0,  0,128,128);
  Stretchi(win,pGlossMapTexture,       crayons.Pick(alabaster),1.0f,additive,128,  0,128,128);
  Stretchi(win,pNormalMapTexture,      crayons.Pick(alabaster),1.0f,additive,256,  0,128,128);
  Stretchi(win,pHeightMapTexture,      crayons.Pick(alabaster),1.0f,additive,512,  0,128,128);
  Stretchi(win,pLuminanceMapTexture,   crayons.Pick(alabaster),1.0f,additive,  0,128,128,128);
  Stretchi(win,pEnvMapTexture,         crayons.Pick(alabaster),1.0f,additive,128,128,128,128);
  Stretchi(win,pAdditiveMapTexture,    crayons.Pick(alabaster),1.0f,additive,256,128,128,128);
  Stretchi(win,pSubtractiveMapTexture, crayons.Pick(alabaster),1.0f,additive,512,128,128,128);
  Stretchi(win,pAdditiveMap2Texture,   crayons.Pick(alabaster),1.0f,additive,  0,256,128,128);
  Stretchi(win,pSubtractiveMap2Texture,crayons.Pick(alabaster),1.0f,additive,128,256,128,128);

  PrintTexFont( win, microFont, crayons.Pick(green255), additive, 5, 384, 10, 12, 2, &log);
  log.Clear();
#endif
 }
};