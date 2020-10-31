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

#include "Art.h"
#include "GLWindow.h"
#include "GLSL2Shader.h"
#include "GLMCamera.h"
#include "VBO.h"

// Based on GLM-related tutorials #13 opengl-tutorial.org
// Uses older-style TC/N/V vbo
class PhongShader : public GLSLShader {
public:
 GLMCamera camera;
 Vertex lightPosition;
 float lightPower;
 Crayon lightColor;
 GLuint
  Ulight,UlightPower,UlightColor,
  Umvp,Uv,Um,Umv3x3; // color, gloss, normal
	GLuint
  UAposition,
  UAnormal,
  UAtangent,
  UAbitangent;
 Zp<VBOStaticVNTTanBi> vbo;
 GLint posAttrib,texAttrib;
 void Init() {
  Ulight=UlightPower=UlightColor=Umvp=Uv=Um=Umv3x3=0;
  UAposition=UAnormal=UAtangent=UAbitangent=0;
  lightPower=10.0f;
  lightColor.Pick(alabaster);
  lightPosition.Set(0.0f,0.0f,3.0f);
  OUTPUT("PhongShader:Loading\n");
  string vert=
   string("#version 120\n")+
   string("attribute vec3 position;\n")+
   string("attribute vec3 normal;\n")+
   string("attribute vec3 tangent;\n")+
   string("attribute vec3 bitangent;\n")+
   string("uniform mat4 MVP;\n")+
   string("uniform mat4 V;\n")+
   string("uniform mat4 M;\n")+
   string("uniform mat3 MV3x3;\n")+
   string("varying vec3 Norm;\n")+
   string("varying vec3 Position_worldspace;\n")+
   string("varying vec3 EyeDirection_cameraspace;\n")+
   string("uniform vec3 LightPosition_worldspace;\n")+
   string("varying vec3 LightDirection_cameraspace;\n")+
   string("varying vec3 LightDirection_tangentspace;\n")+
   string("varying vec3 EyeDirection_tangentspace;\n")+
   string("void main() {\n")+
   string(" gl_Position =  MVP * vec4(position,1);\n")+
   string(" Position_worldspace = (M * vec4(position,1)).xyz;\n")+
   string(" vec3 vertexPosition_cameraspace = ( V * M * vec4(position,1)).xyz;\n")+
   string(" EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;\n")+
   string(" vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;\n")+
   string(" LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;\n")+
   string(" vec3 T = MV3x3 * tangent;\n")+
   string(" vec3 B = MV3x3 * bitangent;\n")+
   string(" vec3 N = MV3x3 * normal;\n")+
   string(" mat3 TBN = transpose(mat3(	T,	B,	N	));\n")+
   string(" LightDirection_tangentspace = TBN * LightDirection_cameraspace;\n")+
   string(" EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;\n")+
   string("}\n")
  ;
  string frag=
   string("#version 120\n")+
   string("uniform	vec3 LightColor;\n")+
   string("uniform	float LightPower;\n")+
   string("varying vec3 Norm;\n")+
   string("varying vec3 Position_worldspace;\n")+
   string("varying vec3 EyeDirection_cameraspace;\n")+
   string("uniform vec3 LightPosition_worldspace;\n")+
   string("varying vec3 LightDirection_cameraspace;\n")+
   string("varying vec3 LightDirection_tangentspace;\n")+
   string("varying vec3 EyeDirection_tangentspace;\n")+
   string("void main() {\n")+
   string(" vec3 A  = vec3(0.1,0.1,0.1);\n")+
   string(" vec3 L = normalize(LightDirection_tangentspace);\n")+
   string(" vec3 E = normalize(EyeDirection_tangentspace);\n")+
   string(" vec3 R = reflect(-L,Norm);\n")+
   string(" float cosAlpha = clamp( dot( E,R ), 0,1 );\n")+
   string(" float cosTheta = clamp( dot( Norm,L ), 0,1 );\n")+
   string(" vec3 Intensity = LightColor * LightPower;\n")+
   string(" float distance = length( LightPosition_worldspace - Position_worldspace );\n")+
   string(" float distanceSquared=distance*distance;\n")+
   string(" vec3 D = Intensity * cosTheta / (distanceSquared);\n")+
   string(" vec3 S = Intensity * pow(cosAlpha,5) / (distanceSquared);\n")+
   string(" vec3 final= A + D + (0.01*S);\n")+
   string(" gl_FragColor = vec4( final, 1.0 );\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  // Single Light
  Uniform(&Ulight,"LightPosition_worldspace");
  Uniform(&UlightPower,"LightPower");
  Uniform(&UlightColor,"LightColor");
  // Matrices
  Uniform(&Umvp,  "MVP");
  Uniform(&Uv,    "V");
  Uniform(&Um,    "M");
  Uniform(&Umv3x3,"MV3x3");
  // Attribute handles for VBO
 	Attribute(&UAposition, "position");
 	Attribute(&UAnormal,   "normal");
 	Attribute(&UAtangent,  "tangent");
 	Attribute(&UAbitangent,"bitangent");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform3f(Ulight, lightPosition.x,lightPosition.y,lightPosition.z);
  glUniform1f(UlightPower, lightPower);
  glUniform3f(UlightColor, lightColor.rf, lightColor.gf, lightColor.bf);
		glUniformMatrix4fv(Umvp,   1, GL_FALSE, &camera.MVP[0][0]);
		glUniformMatrix4fv(Um,     1, GL_FALSE, &camera.ModelMatrix[0][0]);
		glUniformMatrix4fv(Uv,     1, GL_FALSE, &camera.ViewMatrix[0][0]);
		glUniformMatrix3fv(Umv3x3, 1, GL_FALSE, &camera.ModelView3x3Matrix[0][0]);
  vbo->position=&UAposition;
  vbo->tangent=&UAtangent;
  vbo->normal=&UAnormal;
  vbo->bitangent=&UAbitangent;
  vbo->Render3();
 }
 void Disable() {
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);
 }
};

extern PhongShader phongShader;

class ParallaxPointShader : public GLSLShader {
public:
 float lightRadius,scale,bias;
 GLuint *color,*normal,*height;
 GLuint Ucolor,Unormal,Uheight,UlightRadius,Uscale,Ubias;
 GLint posAttrib,texAttrib;
 void Init() {
  color=normal=height=null;
  lightRadius=7.5f;
  scale=0.04f;
  bias=-0.03f;
  OUTPUT("ParallaxPointShader::Loading\n");
  string vert=
   string("#version 110\n")+
   string("uniform float lightRadius;\n")+
   string("varying vec3 lightDir;\n")+
   string("varying vec3 viewDir;\n")+
   string("void main() {\n")+
   string(" gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n")+
   string(" gl_TexCoord[0] = gl_MultiTexCoord0;\n")+
   string(" vec3 vertexPos = vec3(gl_ModelViewMatrix * gl_Vertex);\n")+
   string(" vec3 n = normalize(gl_NormalMatrix * gl_Normal);\n")+
   string(" vec3 t = normalize(gl_NormalMatrix * gl_MultiTexCoord1.xyz);\n")+
   string(" vec3 b = cross(n, t) * gl_MultiTexCoord1.w;\n")+
   string(" mat3 tbnMatrix = mat3(t.x, b.x, n.x, t.y, b.y, n.y, t.z, b.z, n.z);\n")+
   string(" lightDir = (gl_LightSource[0].position.xyz - vertexPos) / lightRadius;\n")+
   string(" lightDir = tbnMatrix * lightDir;\n")+
   string(" viewDir = -vertexPos;\n")+
   string(" viewDir = tbnMatrix * viewDir;\n")+
   string("}\n")
  ;
  string frag=
   string("#version 110\n")+
   string("uniform sampler2D colorMap;\n")+
   string("uniform sampler2D normalMap;\n")+
   string("uniform sampler2D heightMap;\n")+
   string("uniform float scale;\n")+
   string("uniform float bias;\n")+
   string("varying vec3 lightDir;\n")+
   string("varying vec3 viewDir;\n")+
   string("void main() {\n")+
   string(" vec2 newTexCoord;\n")+
   string(" vec3 v = normalize(viewDir);\n")+
   string(" float height = texture2D(heightMap, gl_TexCoord[0].st).r;\n")+
   string(" height = height * scale + bias;\n")+
   string(" newTexCoord = gl_TexCoord[0].st + (height * v.xy);\n")+
   string(" vec3 l = lightDir;\n")+
   string(" float atten = max(0.0, 1.0 - dot(l, l));\n")+
   string(" l = normalize(l);\n")+
   string(" vec3 n = normalize(texture2D(normalMap, newTexCoord).rgb * 2.0 - 1.0);\n")+
   string(" vec3 h = normalize(l + v);\n")+
   string(" float nDotL = max(0.0, dot(n, l));\n")+
   string(" float nDotH = max(0.0, dot(n, h));\n")+
   string(" float power = (nDotL == 0.0) ? 0.0 : pow(nDotH, gl_FrontMaterial.shininess);\n")+
   string(" vec4 ambient = gl_FrontLightProduct[0].ambient * atten;\n")+
   string(" vec4 diffuse = gl_FrontLightProduct[0].diffuse * nDotL * atten;\n")+
   string(" vec4 specular = gl_FrontLightProduct[0].specular * power * atten;\n")+
   string(" vec4 color = gl_FrontLightModelProduct.sceneColor + ambient + diffuse + specular;\n")+
   string(" gl_FragColor = color * texture2D(colorMap, newTexCoord);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUIniforms() {
  Ucolor=glGetUniformLocation(program,"colorMap");
  Unormal=glGetUniformLocation(program,"normalMap");
  Uheight=glGetUniformLocation(program,"heightMap");
  UlightRadius=glGetUniformLocation(program,"lightRadius");
  Ubias=glGetUniformLocation(program,"bias");
  Uscale=glGetUniformLocation(program,"scale");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform1f(Uscale,scale);
  glUniform1f(Ubias,bias);
  glUniform1f(UlightRadius,lightRadius);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Ucolor, 0);   glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *color);
  glUniform1i(Unormal, 0);  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *normal);
  glUniform1i(Uheight, 0);  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *height);
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern ParallaxPointShader parallaxpointShader;



/**
 SteepParallax.glsl.vrt
 Morgan McGuire 2005 morgan@cs.brown.edu
 (incomplete)
 */
class SteepParallaxShader : public GLSLShader {
public:
/** Color texture (with alpha) */
/** xyz = normal, w = bump height */
 GLuint *color,*normalheight;
 float eye[3];
 float bumpiness;
/** Multiplier for bump map.  Typically on the range [0, 0.05]
  This increases with texture scale and bump height. */
    // We are at height bumpScale.  March forward until we hit a hair or the 
    // base surface.  Instead of dropping down discrete y-voxels we should be
    // marching in texels and dropping our y-value accordingly (TODO: fix)
 float divisions; // Number of height divisions
 GLint posAttrib,texAttrib;
 void Init() {
  bumpiness=0.025f;
  divisions=5.0f;
  string vert=string("#version 120\n")+
   string("uniform vec3 wsEyePos;")+
   string("uniform sampler2D texture;\n")+
   string("uniform sampler2D normalBumpMap;\n")+
   string("uniform float bumpScale;\n")+
   string("uniform float numSteps;\n")+
   string("uniform float a;\n")+
   string("varying vec3 tsL;\n")+
   string("varying vec3 _tsE;\n")+
   string("varying vec2 texCoord;\n")+
   string("varying vec4 tan_X, tan_Y, tan_Z, tan_W;\n")+
   string("void main(void) {\n")+
   string(" float height = 1.0;\n")+
   string("    float numSteps = 5;\n")+
   string("    /** Texture coordinate marched forward to intersection point */\n")+
   string("    vec2 offsetCoord = texCoord.xy;\n")+
   string("    vec4 NB = texture2D(normalBumpMap, offsetCoord);\n")+
   string("    vec3 tsE = normalize(_tsE);\n")+
   string("    // Increase steps at oblique angles / Note: tsE.z = N dot V\n")+
   string("    numSteps = mix(numSteps*2, numSteps, tsE.z);\n")+
   string("    // We have to negate tsE because we're walking away from the eye.\n")+
   string("    //vec2 delta = vec2(-_tsE.x, _tsE.y) * bumpScale / (_tsE.z * numSteps);\n")+
   string("    float step;\n")+
   string("    vec2 delta;\n")+
   string("    // Constant in z\n")+
   string("    step = 1.0 / numSteps;\n")+
   string("    delta = vec2(-_tsE.x, _tsE.y) * bumpScale / (_tsE.z * numSteps);\n")+
   string("        // Can also step along constant in xy; the results are essentially the same in each case.\n")+
   string("        // delta = 1.0 / (25.6 * numSteps) * vec2(-tsE.x, tsE.y);\n")+
   string("        // step = tsE.z * bumpScale * (25.6 * numSteps) / (length(tsE.xy) * 400);\n")+
   string("    while (NB.a < height) {\n")+
   string("     height -= step;\n")+
   string("     offsetCoord += delta;\n")+
   string("     NB = texture2D(normalBumpMap, offsetCoord);\n")+
   string("    }\n")+
   string("    height = NB.a;\n")+
   string("    const vec3 color = texture2D(texture, offsetCoord).rgb;\n")+
   string("    tsL = normalize(tsL);\n")+
   string("    vec3 tsN = NB.xyz * 2 - 1;\n")+
   string("    // Smooth normals locally along gradient to avoid making slices visible.\n")+
   string("    // The magnitude of the step should be a function of the resolution and\n")+
   string("    // of the bump scale and number of steps.\n")+
   string("//    tsN = normalize(texture2D(normalBumpMap, offsetCoord + vec2(tsN.x, -tsN.y) * mipScale).xyz * 2 - 1 + tsN);\n")+
   string("    const vec3 tsH = normalize(tsL + normalize(_tsE));\n")+
   string("    const float NdotL = max(0, dot(tsN, tsL));\n")+
   string("    const float NdotH = max(0, dot(tsN, tsH));\n")+
   string("    float spec = NdotH * NdotH;\n")+
   string("    vec3 lightColor = vec3(1.5, 1.5, 1) * 0.9;\n")+
   string("    vec3 ambient = vec3(0.4,0.4,0.6) * 1.4;\n")+
   string("    float selfShadow = 0;\n")+
   string("    // Don't bother checking for self-shadowing if we're on the back side of an object.\n")+
   string("    if (NdotL > 0) {\n")+
   string("        // Trace a shadow ray along the light vector.\n")+
   string("        const int numShadowSteps = mix(60,5,tsL.z);\n")+
   string("        step = 1.0 / numShadowSteps;\n")+
   string("        delta = vec2(tsL.x, -tsL.y) * bumpScale / (numShadowSteps * tsL.z);\n")+
   string("            // We start one iteration out to avoid shadow acne \n")+
   string("            // (could start bumped a little without going\n")+
   string("            // a whole iteration).\n")+
   string("            height = NB.a + step * 0.1;\n")+
   string("            while ((NB.a < height) && (height < 1)) {\n")+
   string("                height += step;\n")+
   string("                offsetCoord += delta;\n")+
   string("                NB = texture2D(normalBumpMap, offsetCoord);\n")+
   string("            }\n")+
   string("            // We are in shadow if we left the loop because we hit a point\n")+
   string("            selfShadow = (NB.a < height);\n")+
   string("            // Shadows will make the whole scene darker, so up the light contribution\n")+
   string("            lightColor = lightColor * 1.2;\n")+
   string("        }\n")+
   string("   }\n")+
   string("   gl_FragColor.rgb = color * ambient + color * NdotL * selfShadow * lightColor;\n")+
   string("}\n")
  ;
 }
};

class BasicTextureShader : public GLSLShader {
public:
 GLMCamera camera;
 Vertex lightPosition;
 GLuint *diffuse,*specular,*normal;
 GLuint
  Ulight,Umvp,Uv,Um,Umv3x3,Udiffuse,Uspecular,Unormal; // color, gloss, normal
	GLuint
  UAposition,
  UAtexcoord,
  UAnormal,
  UAtangent,
  UAbitangent;
 GLint posAttrib,texAttrib;
 Zp<VBOStaticVNTTanBi> vbo;
 void Init() {
  diffuse=specular=normal=null;
  Ulight=Umvp=Uv=Um=Umv3x3=Udiffuse=Uspecular=Unormal=0;
  UAposition=UAtexcoord=UAnormal=UAtangent=UAbitangent=0;
  lightPosition.Set(0.0f,0.0f,3.0f);
  OUTPUT("BasicTextureShader:Loading\n");
  string vert=
   string("#version 130\n")+
   string("in vec3 position;\n")+
   string("in vec2 texcoord;\n")+
   string("uniform mat4 MVP;\n")+
   string("out vec2 UV;\n")+
   string("void main() {\n")+
   string("	gl_Position = MVP * vec4(position,1);\n")+
   string("	UV = texcoord;\n")+
   string("}\n")
  ;
  string frag=
   string("#version 120\n")+
   string("uniform sampler2D DiffuseMap;\n")+
   string("in vec2 UV;\n")+
   string("void main() {\n")+
   string(" vec4 d=texture2D(DiffuseMap,UV.xy);\n")+
   string(" gl_FragColor = vec4(d.r,d.g,UV.x,1.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  // Matrices
  Uniform(&Umvp,  "MVP");
  // Texture Sources
  Uniform(&Udiffuse,  "DiffuseMap");
  // Attribute handles for VBO
 	Attribute(&UAposition, "position");
 	Attribute(&UAtexcoord, "texcoord");
 }
 void UpdateUniforms() {
  glUseProgram(program);
		glUniformMatrix4fv(Umvp,   1, GL_FALSE, &camera.MVP[0][0]);
		glUniform1i(Udiffuse, 0);  glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, *diffuse);	
  vbo->position=&UAposition;
  vbo->texcoord=&UAtexcoord;
  vbo->Render();
 }
 void Disable() {
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);
 }
};

extern BasicTextureShader basictextureShader;

// Source: GLM-related tutorials #13 opengl-tutorial.org
class NormalMappingShader : public GLSLShader {
public:
 GLMCamera camera;
 Vertex lightPosition;
 float lightPower;
 Crayon lightColor;
 GLuint *diffuse,*specular,*normal;
 GLuint
  Ulight,UlightPower,UlightColor,Uoffset,
  Umvp,Uv,Um,Umv3x3,Udiffuse,Uspecular,Unormal; // color, gloss, normal
	GLuint
  UAposition,
  UAtexcoord,
  UAnormal,
  UAtangent,
  UAbitangent;
 Vertex offset;
 Zp<VBOStaticVNTTanBi> vbo;
 GLint posAttrib,texAttrib;
 void Init() {
  diffuse=specular=normal=null;
  Ulight=UlightPower=UlightColor=Uoffset=Umvp=Uv=Um=Umv3x3=Udiffuse=Uspecular=Unormal=0;
  UAposition=UAtexcoord=UAnormal=UAtangent=UAbitangent=0;
  lightPower=10.0f;
  lightColor.Pick(alabaster);
  lightPosition.Set(0.0f,0.0f,3.0f);
  OUTPUT("NormalMappingShader:Loading\n");
  string vert=
   string("#version 120\n")+
   string("attribute vec3 position;\n")+
   string("attribute vec2 texcoord;\n")+
   string("attribute vec3 normal;\n")+
   string("attribute vec3 tangent;\n")+
   string("attribute vec3 bitangent;\n")+
   string("uniform mat4 MVP;\n")+
   string("uniform mat4 V;\n")+
   string("uniform mat4 M;\n")+
   string("uniform mat3 MV3x3;\n")+
   string("varying vec2 UV;\n")+
   string("varying vec3 Position_worldspace;\n")+
   string("varying vec3 EyeDirection_cameraspace;\n")+
   string("uniform vec3 LightPosition_worldspace;\n")+
   string("varying vec3 LightDirection_cameraspace;\n")+
   string("varying vec3 LightDirection_tangentspace;\n")+
   string("varying vec3 EyeDirection_tangentspace;\n")+
   string("uniform vec3 ObjectOffset_worldspace;\n")+
   string("void main() {\n")+
   string(" gl_Position =  MVP * vec4(position+ObjectOffset_worldspace,1);\n")+
   string(" Position_worldspace = (M * vec4(position+ObjectOffset_worldspace,1)).xyz;\n")+
   string(" vec3 vertexPosition_cameraspace = ( V * M * vec4(position+ObjectOffset_worldspace,1)).xyz;\n")+
   string(" EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;\n")+
   string(" vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;\n")+
   string(" LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;\n")+
   string(" UV = texcoord;\n")+
   string(" vec3 T = MV3x3 * tangent;\n")+
   string(" vec3 B = MV3x3 * bitangent;\n")+
   string(" vec3 N = MV3x3 * normal;\n")+
   string(" mat3 TBN = transpose(mat3(	T,	B,	N	));\n")+
   string(" LightDirection_tangentspace = TBN * LightDirection_cameraspace;\n")+
   string(" EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;\n")+
   string("}\n")
  ;
  string frag=
   string("#version 120\n")+
   string("uniform sampler2D DiffuseMap;\n")+
   string("uniform sampler2D NormalMap;\n")+
   string("uniform sampler2D SpecularMap;\n")+
   string("uniform	vec3 LightColor;\n")+
   string("uniform	float LightPower;\n")+
   string("varying vec2 UV;\n")+
   string("varying vec3 Position_worldspace;\n")+
   string("varying vec3 EyeDirection_cameraspace;\n")+
   string("uniform vec3 LightPosition_worldspace;\n")+
   string("varying vec3 LightDirection_cameraspace;\n")+
   string("varying vec3 LightDirection_tangentspace;\n")+
   string("varying vec3 EyeDirection_tangentspace;\n")+
   string("uniform vec3 ObjectOffset_worldspace;\n")+
   string("void main() {\n")+
   string(" vec3 Diff  = texture2D( DiffuseMap,  UV ).rgb;\n")+
   string(" vec3 Spec  = texture2D( SpecularMap, UV ).rgb * 0.3;\n")+
   string(" vec3 Norm  = texture2D( NormalMap,   UV ).rgb;\n")+
   string(" vec3 A  = vec3(0.1,0.1,0.1) * Diff;\n")+
   string(" vec3 N = normalize(Norm*2.0 - 1.0);\n")+
   string(" vec3 L = normalize(LightDirection_tangentspace);\n")+
   string(" vec3 E = normalize(EyeDirection_tangentspace);\n")+
   string(" vec3 R = reflect(-L,N);\n")+
   string(" float cosAlpha = clamp( dot( E,R ), 0,1 );\n")+
   string(" float cosTheta = clamp( dot( N,L ), 0,1 );\n")+
   string(" vec3 Intensity = LightColor * LightPower;\n")+
   string(" float distance = length( LightPosition_worldspace - Position_worldspace );\n")+
   string(" float distanceSquared=distance*distance;\n")+
   string(" vec3 D = Diff * Intensity * cosTheta / (distanceSquared);\n")+
   string(" vec3 S = Spec * Intensity * pow(cosAlpha,5) / (distanceSquared);\n")+
   string(" vec3 final= A + D + (0.01*S);\n")+
   string(" gl_FragColor = vec4( final, 1.0 );\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  // Single Light
  Uniform(&Ulight,"LightPosition_worldspace");
  Uniform(&UlightPower,"LightPower");
  Uniform(&UlightColor,"LightColor");
  // Matrices
  Uniform(&Umvp,  "MVP");
  Uniform(&Uv,    "V");
  Uniform(&Um,    "M");
  Uniform(&Umv3x3,"MV3x3");
  // Texture Sources
  Uniform(&Udiffuse,  "DiffuseMap");
  Uniform(&Uspecular,"SpecularMap");
  Uniform(&Unormal,   "NormalMap");
  // Attribute handles for VBO
 	Attribute(&UAposition, "position");
 	Attribute(&UAnormal,   "normal");
 	Attribute(&UAtexcoord, "texcoord");
 	Attribute(&UAtangent,  "tangent");
 	Attribute(&UAbitangent,"bitangent");
  // Positioning
  Uniform(&Uoffset,"ObjectOffset_worldspace");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform3f(Ulight, lightPosition.x,lightPosition.y,lightPosition.z);
  glUniform1f(UlightPower, lightPower);
  glUniform3f(UlightColor, lightColor.rf, lightColor.gf, lightColor.bf);
  glUniform3f(Uoffset, offset.x,offset.y,offset.z);
		glUniformMatrix4fv(Umvp,   1, GL_FALSE, &camera.MVP[0][0]);
		glUniformMatrix4fv(Um,     1, GL_FALSE, &camera.ModelMatrix[0][0]);
		glUniformMatrix4fv(Uv,     1, GL_FALSE, &camera.ViewMatrix[0][0]);
		glUniformMatrix3fv(Umv3x3, 1, GL_FALSE, &camera.ModelView3x3Matrix[0][0]);
		glUniform1i(Udiffuse, 0);  glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, *diffuse);	
		glUniform1i(Unormal,  1);  glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, *normal);		
		glUniform1i(Uspecular, 2); glActiveTexture(GL_TEXTURE2);	glBindTexture(GL_TEXTURE_2D, *specular);
  vbo->position=&UAposition;
  vbo->texcoord=&UAtexcoord;
  vbo->normal=&UAnormal;
  vbo->tangent=&UAtangent;
  vbo->bitangent=&UAbitangent;
  vbo->Render();
 }
 void Disable() {
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);
 }
};

extern NormalMappingShader normalmappingShader;


// Same as above but respects alpha.
class NormalMappingAlphaShader : public GLSLShader {
public:
 GLMCamera camera;
 Vertex lightPosition;
 float lightPower;
 Crayon lightColor;
 GLuint *diffuse,*specular,*normal;
 GLuint
  Ulight,UlightPower,UlightColor,
  Umvp,Uv,Um,Umv3x3,Udiffuse,Uspecular,Unormal; // color, gloss, normal
	GLuint
  UAposition,
  UAtexcoord,
  UAnormal,
  UAtangent,
  UAbitangent;
 Zp<VBOStaticVNTTanBi> vbo;
 GLint posAttrib,texAttrib;
 void Init() {
  diffuse=specular=normal=null;
  Ulight=UlightPower=UlightColor=Umvp=Uv=Um=Umv3x3=Udiffuse=Uspecular=Unormal=0;
  UAposition=UAtexcoord=UAnormal=UAtangent=UAbitangent=0;
  lightPower=10.0f;
  lightColor.Pick(alabaster);
  lightPosition.Set(0.0f,0.0f,3.0f);
  OUTPUT("NormalMappingShader:Loading\n");
  string vert=
   string("#version 120\n")+
   string("attribute vec3 position;\n")+
   string("attribute vec2 texcoord;\n")+
   string("attribute vec3 normal;\n")+
   string("attribute vec3 tangent;\n")+
   string("attribute vec3 bitangent;\n")+
   string("uniform mat4 MVP;\n")+
   string("uniform mat4 V;\n")+
   string("uniform mat4 M;\n")+
   string("uniform mat3 MV3x3;\n")+
   string("varying vec2 UV;\n")+
   string("varying vec3 Position_worldspace;\n")+
   string("varying vec3 EyeDirection_cameraspace;\n")+
   string("uniform vec3 LightPosition_worldspace;\n")+
   string("varying vec3 LightDirection_cameraspace;\n")+
   string("varying vec3 LightDirection_tangentspace;\n")+
   string("varying vec3 EyeDirection_tangentspace;\n")+
   string("void main() {\n")+
   string(" gl_Position =  MVP * vec4(position,1);\n")+
   string(" Position_worldspace = (M * vec4(position,1)).xyz;\n")+
   string(" vec3 vertexPosition_cameraspace = ( V * M * vec4(position,1)).xyz;\n")+
   string(" EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;\n")+
   string(" vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;\n")+
   string(" LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;\n")+
   string(" UV = texcoord;\n")+
   string(" vec3 T = MV3x3 * tangent;\n")+
   string(" vec3 B = MV3x3 * bitangent;\n")+
   string(" vec3 N = MV3x3 * normal;\n")+
   string(" mat3 TBN = transpose(mat3(	T,	B,	N	));\n")+
   string(" LightDirection_tangentspace = TBN * LightDirection_cameraspace;\n")+
   string(" EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;\n")+
   string("}\n")
  ;
  string frag=
   string("#version 120\n")+
   string("uniform sampler2D DiffuseMap;\n")+
   string("uniform sampler2D NormalMap;\n")+
   string("uniform sampler2D SpecularMap;\n")+
   string("uniform	vec3 LightColor;\n")+
   string("uniform	float LightPower;\n")+
   string("varying vec2 UV;\n")+
   string("varying vec3 Position_worldspace;\n")+
   string("varying vec3 EyeDirection_cameraspace;\n")+
   string("uniform vec3 LightPosition_worldspace;\n")+
   string("varying vec3 LightDirection_cameraspace;\n")+
   string("varying vec3 LightDirection_tangentspace;\n")+
   string("varying vec3 EyeDirection_tangentspace;\n")+
   string("void main() {\n")+
   string(" vec4 DM  = texture2D( DiffuseMap,  UV );\n")+
   string(" vec3 Diff = DM.rgb;\n")+
   string(" vec3 Spec  = texture2D( SpecularMap, UV ).rgb * 0.3;\n")+
   string(" vec3 Norm  = texture2D( NormalMap,   UV ).rgb;\n")+
   string(" vec3 A  = vec3(0.1,0.1,0.1) * Diff;\n")+
   string(" vec3 N = normalize(Norm*2.0 - 1.0);\n")+
   string(" vec3 L = normalize(LightDirection_tangentspace);\n")+
   string(" vec3 E = normalize(EyeDirection_tangentspace);\n")+
   string(" vec3 R = reflect(-L,N);\n")+
   string(" float cosAlpha = clamp( dot( E,R ), 0,1 );\n")+
   string(" float cosTheta = clamp( dot( N,L ), 0,1 );\n")+
   string(" vec3 Intensity = LightColor * LightPower;\n")+
   string(" float distance = length( LightPosition_worldspace - Position_worldspace );\n")+
   string(" float distanceSquared=distance*distance;\n")+
   string(" vec3 D = Diff * Intensity * cosTheta / (distanceSquared);\n")+
   string(" vec3 S = Spec * Intensity * pow(cosAlpha,5) / (distanceSquared);\n")+
   string(" vec3 final= A + D + (0.01*S);\n")+
   string(" gl_FragColor = vec4( final, DM.a );\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  // Single Light
  Uniform(&Ulight,"LightPosition_worldspace");
  Uniform(&UlightPower,"LightPower");
  Uniform(&UlightColor,"LightColor");
  // Matrices
  Uniform(&Umvp,  "MVP");
  Uniform(&Uv,    "V");
  Uniform(&Um,    "M");
  Uniform(&Umv3x3,"MV3x3");
  // Texture Sources
  Uniform(&Udiffuse,  "DiffuseMap");
  Uniform(&Uspecular,"SpecularMap");
  Uniform(&Unormal,   "NormalMap");
  // Attribute handles for VBO
 	Attribute(&UAposition, "position");
 	Attribute(&UAnormal,   "normal");
 	Attribute(&UAtexcoord, "texcoord");
 	Attribute(&UAtangent,  "tangent");
 	Attribute(&UAbitangent,"bitangent");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform3f(Ulight, lightPosition.x,lightPosition.y,lightPosition.z);
  glUniform1f(UlightPower, lightPower);
  glUniform3f(UlightColor, lightColor.rf, lightColor.gf, lightColor.bf);
		glUniformMatrix4fv(Umvp,   1, GL_FALSE, &camera.MVP[0][0]);
		glUniformMatrix4fv(Um,     1, GL_FALSE, &camera.ModelMatrix[0][0]);
		glUniformMatrix4fv(Uv,     1, GL_FALSE, &camera.ViewMatrix[0][0]);
		glUniformMatrix3fv(Umv3x3, 1, GL_FALSE, &camera.ModelView3x3Matrix[0][0]);
		glUniform1i(Udiffuse, 0);  glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, *diffuse);	
		glUniform1i(Unormal,  1);  glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, *normal);		
		glUniform1i(Uspecular, 2); glActiveTexture(GL_TEXTURE2);	glBindTexture(GL_TEXTURE_2D, *specular);
  vbo->position=&UAposition;
  vbo->texcoord=&UAtexcoord;
  vbo->normal=&UAnormal;
  vbo->tangent=&UAtangent;
  vbo->bitangent=&UAbitangent;
  vbo->Render();
 }
 void Disable() {
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);
 }
};

extern NormalMappingAlphaShader normalmappingalphaShader;


// Same as above but obscures light by the stencil provided.
// Stencil uses black for transparent and white for obscuring
// Used by planetary surface renderer
// Respects alpha channel
class NormalMappingShadowProjectionShader : public GLSLShader {
public:
 GLMCamera camera;
 Vertex lightPosition;
 float lightPower;
 Crayon lightColor;
 GLuint *diffuse,*specular,*normal,*projected;
 GLuint
  Ulight,UlightPower,UlightColor,UprojectOpacity,
  Umvp,Uv,Um,Umv3x3,
  Udiffuse,Uspecular,Unormal,Uprojected; // color, gloss, normal, projected
	GLuint
  UAposition,
  UAtexcoord,
  UAnormal,
  UAtangent,
  UAbitangent;
 Zp<VBOStaticVNTTanBi> vbo;
 GLint posAttrib,texAttrib;
 void Init() {
  diffuse=specular=normal=null;
  Ulight=UlightPower=UlightColor=Umvp=Uv=Um=Umv3x3=Udiffuse=Uspecular=Unormal=0;
  UAposition=UAtexcoord=UAnormal=UAtangent=UAbitangent=0;
  lightPower=10.0f;
  lightColor.Pick(alabaster);
  lightPosition.Set(0.0f,0.0f,3.0f);
  OUTPUT("NormalMappingShadowProjectionShader:Loading\n");
  string vert=
   string("#version 120\n")+
   string("attribute vec3 position;\n")+
   string("attribute vec2 texcoord;\n")+
   string("attribute vec3 normal;\n")+
   string("attribute vec3 tangent;\n")+
   string("attribute vec3 bitangent;\n")+
   string("uniform mat4 MVP;\n")+
   string("uniform mat4 V;\n")+
   string("uniform mat4 M;\n")+
   string("uniform mat3 MV3x3;\n")+
   string("varying vec2 UV;\n")+
   string("varying vec3 Position_worldspace;\n")+
   string("varying vec3 EyeDirection_cameraspace;\n")+
   string("uniform vec3 LightPosition_worldspace;\n")+
   string("varying vec3 LightDirection_cameraspace;\n")+
   string("varying vec3 LightDirection_tangentspace;\n")+
   string("varying vec3 EyeDirection_tangentspace;\n")+
   string("void main() {\n")+
   string(" gl_Position =  MVP * vec4(position,1);\n")+
   string(" Position_worldspace = (M * vec4(position,1)).xyz;\n")+
   string(" vec3 vertexPosition_cameraspace = ( V * M * vec4(position,1)).xyz;\n")+
   string(" EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;\n")+
   string(" vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;\n")+
   string(" LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;\n")+
   string(" UV = texcoord;\n")+
   string(" vec3 T = MV3x3 * tangent;\n")+
   string(" vec3 B = MV3x3 * bitangent;\n")+
   string(" vec3 N = MV3x3 * normal;\n")+
   string(" mat3 TBN = transpose(mat3(	T,	B,	N	));\n")+
   string(" LightDirection_tangentspace = TBN * LightDirection_cameraspace;\n")+
   string(" EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;\n")+
   string("}\n")
  ;
  string frag=
   string("#version 120\n")+
   string("uniform sampler2D DiffuseMap;\n")+
   string("uniform sampler2D NormalMap;\n")+
   string("uniform sampler2D SpecularMap;\n")+
   string("uniform	vec3 LightColor;\n")+
   string("uniform	float LightPower;\n")+
   string("varying vec2 UV;\n")+
   string("varying vec3 Position_worldspace;\n")+
   string("varying vec3 EyeDirection_cameraspace;\n")+
   string("uniform vec3 LightPosition_worldspace;\n")+
   string("varying vec3 LightDirection_cameraspace;\n")+
   string("varying vec3 LightDirection_tangentspace;\n")+
   string("varying vec3 EyeDirection_tangentspace;\n")+
   string("void main() {\n")+
   string(" vec3 Diff  = texture2D( DiffuseMap,  UV ).rgb;\n")+
   string(" vec3 Spec  = texture2D( SpecularMap, UV ).rgb * 0.3;\n")+
   string(" vec3 Norm  = texture2D( NormalMap,   UV ).rgb;\n")+
   string(" vec3 Proj  = texture2D( Project, UV ).rgb;\n")+
   string(" vec3 A  = vec3(0.1,0.1,0.1) * Diff;\n")+
   string(" vec3 N = normalize(Norm*2.0 - 1.0);\n")+
   string(" vec3 L = normalize(LightDirection_tangentspace);\n")+
   string(" vec3 E = normalize(EyeDirection_tangentspace);\n")+
   string(" vec3 R = reflect(-L,N);\n")+
   string(" float cosAlpha = clamp( dot( E,R ), 0,1 );\n")+
   string(" float cosTheta = clamp( dot( N,L ), 0,1 );\n")+
   string(" vec3 Intensity = LightColor * LightPower;\n")+
   string(" float distance = length( LightPosition_worldspace - Position_worldspace );\n")+
   string(" float distanceSquared=distance*distance;\n")+
   string(" vec3 D = Diff * Intensity * cosTheta / (distanceSquared);\n")+
   string(" vec3 S = Spec * Intensity * pow(cosAlpha,5) / (distanceSquared);\n")+
   string(" vec3 final= A + D + (0.01*S);\n")+
   string(" gl_FragColor = vec4( final, 1.0 );\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  // Single Light
  Uniform(&Ulight,"LightPosition_worldspace");
  Uniform(&UlightPower,"LightPower");
  Uniform(&UlightColor,"LightColor");
  // Matrices
  Uniform(&Umvp,  "MVP");
  Uniform(&Uv,    "V");
  Uniform(&Um,    "M");
  Uniform(&Umv3x3,"MV3x3");
  // Texture Sources
  Uniform(&Udiffuse,  "DiffuseMap");
  Uniform(&Uspecular,"SpecularMap");
  Uniform(&Unormal,   "NormalMap");
  // Attribute handles for VBO
 	Attribute(&UAposition, "position");
 	Attribute(&UAnormal,   "normal");
 	Attribute(&UAtexcoord, "texcoord");
 	Attribute(&UAtangent,  "tangent");
 	Attribute(&UAbitangent,"bitangent");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform3f(Ulight, lightPosition.x,lightPosition.y,lightPosition.z);
  glUniform1f(UlightPower, lightPower);
  glUniform3f(UlightColor, lightColor.rf, lightColor.gf, lightColor.bf);
		glUniformMatrix4fv(Umvp,   1, GL_FALSE, &camera.MVP[0][0]);
		glUniformMatrix4fv(Um,     1, GL_FALSE, &camera.ModelMatrix[0][0]);
		glUniformMatrix4fv(Uv,     1, GL_FALSE, &camera.ViewMatrix[0][0]);
		glUniformMatrix3fv(Umv3x3, 1, GL_FALSE, &camera.ModelView3x3Matrix[0][0]);
		glUniform1i(Udiffuse, 0);  glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, *diffuse);	
		glUniform1i(Unormal,  1);  glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, *normal);		
		glUniform1i(Uspecular, 2); glActiveTexture(GL_TEXTURE2);	glBindTexture(GL_TEXTURE_2D, *specular);
  vbo->position=&UAposition;
  vbo->texcoord=&UAtexcoord;
  vbo->normal=&UAnormal;
  vbo->tangent=&UAtangent;
  vbo->bitangent=&UAbitangent;
  vbo->Render();
 }
 void Disable() {
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);
 }
};

//extern NormalMappingShadowProjectionShader normalmappingshadowprojectionShader;

// Source: GLM-related tutorials #13 opengl-tutorial.org
class NormalMapping3LightShader : public GLSLShader {
public:
 GLMCamera camera;
 Vertex lightPosition1,lightPosition2,lightPosition3;
 float lightPower1,lightPower2,lightPower3;
 Crayon lightColor1,lightColor2,lightColor3;
 GLuint *diffuse,*specular,*normal;
 GLuint
  Ulight1,UlightPower1,UlightColor1,
  Ulight2,UlightPower2,UlightColor2,
  Ulight3,UlightPower3,UlightColor3,
  Umvp,Uv,Um,Umv3x3,Udiffuse,Uspecular,Unormal; // color, gloss, normal
 GLint posAttrib,texAttrib;
	GLuint
  UAposition,
  UAtexcoord,
  UAnormal,
  UAtangent,
  UAbitangent;
 Zp<VBOStaticVNTTanBi> vbo;
 void Init() {
  diffuse=specular=normal=null;
  Ulight1=UlightPower1=UlightColor1=
   Ulight2=UlightPower2=UlightColor2=
   Ulight3=UlightPower3=UlightColor3=
   Umvp=Uv=Um=Umv3x3=Udiffuse=Uspecular=Unormal=0;
  UAposition=UAtexcoord=UAnormal=UAtangent=UAbitangent=0;
  lightPower1=10.0f;
  lightColor1.Pick(alabaster);
  lightPosition1.Set(0.0f,0.0f,3.0f);
  lightPower2=10.0f;
  lightColor2.Pick(alabaster);
  lightPosition2.Set(-3.0f,-3.0f,3.0f);
  lightPower3=10.0f;
  lightColor3.Pick(alabaster);
  lightPosition3.Set(3.0f,3.0f,3.0f);
  OUTPUT("NormalMappingShader:Loading\n");
  string vert=
   string("#version 120\n")+
   string("attribute vec3 position;\n")+
   string("attribute vec2 texcoord;\n")+
   string("attribute vec3 normal;\n")+
   string("attribute vec3 tangent;\n")+
   string("attribute vec3 bitangent;\n")+
   string("uniform mat4 MVP;\n")+
   string("uniform mat4 V;\n")+
   string("uniform mat4 M;\n")+
   string("uniform mat3 MV3x3;\n")+
   string("varying vec2 UV;\n")+
   string("varying vec3 Position_worldspace;\n")+
   string("varying vec3 EyeDirection_cameraspace;\n")+
   string("uniform vec3 LightPosition1_worldspace;\n")+
   string("varying vec3 LightDirection1_cameraspace;\n")+
   string("varying vec3 LightDirection1_tangentspace;\n")+
   string("uniform vec3 LightPosition2_worldspace;\n")+
   string("varying vec3 LightDirection2_cameraspace;\n")+
   string("varying vec3 LightDirection2_tangentspace;\n")+
   string("uniform vec3 LightPosition3_worldspace;\n")+
   string("varying vec3 LightDirection3_cameraspace;\n")+
   string("varying vec3 LightDirection3_tangentspace;\n")+
   string("varying vec3 EyeDirection_tangentspace;\n")+
   string("void main() {\n")+
   string(" gl_Position =  MVP * vec4(position,1);\n")+
   string(" Position_worldspace = (M * vec4(position,1)).xyz;\n")+
   string(" vec3 vertexPosition_cameraspace = ( V * M * vec4(position,1)).xyz;\n")+
   string(" EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;\n")+
   string(" vec3 LightPosition1_cameraspace = ( V * vec4(LightPosition1_worldspace,1)).xyz;\n")+
   string(" LightDirection1_cameraspace = LightPosition1_cameraspace + EyeDirection_cameraspace;\n")+
   string(" vec3 LightPosition2_cameraspace = ( V * vec4(LightPosition2_worldspace,1)).xyz;\n")+
   string(" LightDirection2_cameraspace = LightPosition2_cameraspace + EyeDirection_cameraspace;\n")+
   string(" vec3 LightPosition3_cameraspace = ( V * vec4(LightPosition3_worldspace,1)).xyz;\n")+
   string(" LightDirection3_cameraspace = LightPosition3_cameraspace + EyeDirection_cameraspace;\n")+
   string(" UV = texcoord;\n")+
   string(" vec3 T = MV3x3 * tangent;\n")+
   string(" vec3 B = MV3x3 * bitangent;\n")+
   string(" vec3 N = MV3x3 * normal;\n")+
   string(" mat3 TBN = transpose(mat3(	T,	B,	N	));\n")+
   string(" LightDirection1_tangentspace = TBN * LightDirection1_cameraspace;\n")+
   string(" LightDirection2_tangentspace = TBN * LightDirection2_cameraspace;\n")+
   string(" LightDirection3_tangentspace = TBN * LightDirection3_cameraspace;\n")+
   string(" EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;\n")+
   string("}\n")
  ;
  string frag=
   string("#version 120\n")+
   string("uniform sampler2D DiffuseMap;\n")+
   string("uniform sampler2D NormalMap;\n")+
   string("uniform sampler2D SpecularMap;\n")+
   string("uniform	vec3 LightColor1;\n")+
   string("uniform	float LightPower1;\n")+
   string("uniform	vec3 LightColor2;\n")+
   string("uniform	float LightPower2;\n")+
   string("uniform	vec3 LightColor3;\n")+
   string("uniform	float LightPower3;\n")+
   string("varying vec2 UV;\n")+
   string("varying vec3 Position_worldspace;\n")+
   string("varying vec3 EyeDirection_cameraspace;\n")+
   string("uniform vec3 LightPosition1_worldspace;\n")+
   string("varying vec3 LightDirection1_cameraspace;\n")+
   string("varying vec3 LightDirection1_tangentspace;\n")+
   string("uniform vec3 LightPosition2_worldspace;\n")+
   string("varying vec3 LightDirection2_cameraspace;\n")+
   string("varying vec3 LightDirection2_tangentspace;\n")+
   string("uniform vec3 LightPosition3_worldspace;\n")+
   string("varying vec3 LightDirection3_cameraspace;\n")+
   string("varying vec3 LightDirection3_tangentspace;\n")+
   string("varying vec3 EyeDirection_tangentspace;\n")+
   string("void main() {\n")+
   string(" vec3 Diff  = texture2D( DiffuseMap,  UV ).rgb;\n")+
   string(" vec3 Spec  = texture2D( SpecularMap, UV ).rgb * 0.3;\n")+
   string(" vec3 Norm  = texture2D( NormalMap,   UV ).rgb;\n")+
   string(" vec3 A  = vec3(0.1,0.1,0.1) * Diff;\n")+
   string(" vec3 N = normalize(Norm*2.0 - 1.0);\n")+
   string(" vec3 E = normalize(EyeDirection_tangentspace);\n")+
   // Light 1
   string(" vec3 L1 = normalize(LightDirection1_tangentspace);\n")+
   string(" vec3 R1 = reflect(-L1,N);\n")+
   string(" float cosAlpha1 = clamp( dot( E,R1 ), 0,1 );\n")+
   string(" float cosTheta1 = clamp( dot( N,L1 ), 0,1 );\n")+
   string(" vec3 Intensity1 = LightColor1 * LightPower1;\n")+
   string(" float distance1 = length( LightPosition1_worldspace - Position_worldspace );\n")+
   string(" float distanceSquared1=distance1*distance1;\n")+
   string(" vec3 D1 = Diff * Intensity1 * cosTheta1 / (distanceSquared1);\n")+
   string(" vec3 S1 = Spec * Intensity1 * pow(cosAlpha1,5) / (distanceSquared1);\n")+
   string(" vec3 final1= A + D1 + (0.01*S1);\n")+
   // Light 2
   string(" vec3 L2 = normalize(LightDirection2_tangentspace);\n")+
   string(" vec3 R2 = reflect(-L2,N);\n")+
   string(" float cosAlpha2 = clamp( dot( E,R2 ), 0,1 );\n")+
   string(" float cosTheta2 = clamp( dot( N,L2 ), 0,1 );\n")+
   string(" vec3 Intensity2 = LightColor2 * LightPower2;\n")+
   string(" float distance2 = length( LightPosition2_worldspace - Position_worldspace );\n")+
   string(" float distanceSquared2=distance2*distance2;\n")+
   string(" vec3 D2 = Diff * Intensity2 * cosTheta2 / (distanceSquared2);\n")+
   string(" vec3 S2 = Spec * Intensity2 * pow(cosAlpha2,5) / (distanceSquared2);\n")+
   string(" vec3 final2= A + D2 + (0.01*S2);\n")+
   // Light 3
   string(" vec3 L3 = normalize(LightDirection3_tangentspace);\n")+
   string(" vec3 R3 = reflect(-L3,N);\n")+
   string(" float cosAlpha3 = clamp( dot( E,R3 ), 0,1 );\n")+
   string(" float cosTheta3 = clamp( dot( N,L3 ), 0,1 );\n")+
   string(" vec3 Intensity3 = LightColor3 * LightPower3;\n")+
   string(" float distance3 = length( LightPosition3_worldspace - Position_worldspace );\n")+
   string(" float distanceSquared3=distance3*distance3;\n")+
   string(" vec3 D3 = Diff * Intensity3 * cosTheta3 / (distanceSquared3);\n")+
   string(" vec3 S3 = Spec * Intensity3 * pow(cosAlpha3,5) / (distanceSquared3);\n")+
   string(" vec3 final3= A + D3 + (0.01*S3);\n")+
   // Combine
   string(" gl_FragColor = vec4( final1+final2+final3, 1.0 );\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  // Light0
  Uniform(&Ulight1,"LightPosition1_worldspace");
  Uniform(&UlightPower1,"LightPower1");
  Uniform(&UlightColor1,"LightColor1");
  // Light1
  Uniform(&Ulight2,"LightPosition2_worldspace");
  Uniform(&UlightPower2,"LightPower2");
  Uniform(&UlightColor2,"LightColor2");
  // Light2
  Uniform(&Ulight3,"LightPosition3_worldspace");
  Uniform(&UlightPower3,"LightPower3");
  Uniform(&UlightColor3,"LightColor3");
  // Matrices
  Uniform(&Umvp,  "MVP");
  Uniform(&Uv,    "V");
  Uniform(&Um,    "M");
  Uniform(&Umv3x3,"MV3x3");
  // Texture Sources
  Uniform(&Udiffuse,  "DiffuseMap");
  Uniform(&Uspecular,"SpecularMap");
  Uniform(&Unormal,   "NormalMap");
  // Attribute handles for VBO
 	Attribute(&UAposition, "position");
 	Attribute(&UAnormal,   "normal");
 	Attribute(&UAtexcoord, "texcoord");
 	Attribute(&UAtangent,  "tangent");
 	Attribute(&UAbitangent,"bitangent");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform3f(Ulight1, lightPosition1.x,lightPosition1.y,lightPosition1.z);
  glUniform1f(UlightPower1, lightPower1);
  glUniform3f(UlightColor1, lightColor1.rf, lightColor1.gf, lightColor1.bf);
  glUniform3f(Ulight2, lightPosition2.x,lightPosition2.y,lightPosition2.z);
  glUniform1f(UlightPower2, lightPower2);
  glUniform3f(UlightColor2, lightColor2.rf, lightColor2.gf, lightColor2.bf);
  glUniform3f(Ulight3, lightPosition3.x,lightPosition3.y,lightPosition3.z);
  glUniform1f(UlightPower3, lightPower3);
  glUniform3f(UlightColor3, lightColor3.rf, lightColor3.gf, lightColor3.bf);
		glUniformMatrix4fv(Umvp,   1, GL_FALSE, &camera.MVP[0][0]);
		glUniformMatrix4fv(Um,     1, GL_FALSE, &camera.ModelMatrix[0][0]);
		glUniformMatrix4fv(Uv,     1, GL_FALSE, &camera.ViewMatrix[0][0]);
		glUniformMatrix3fv(Umv3x3, 1, GL_FALSE, &camera.ModelView3x3Matrix[0][0]);
		glUniform1i(Udiffuse, 0);  glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, *diffuse);	
		glUniform1i(Unormal,  1);  glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, *normal);		
		glUniform1i(Uspecular, 2); glActiveTexture(GL_TEXTURE2);	glBindTexture(GL_TEXTURE_2D, *specular);
  vbo->position=&UAposition;
  vbo->texcoord=&UAtexcoord;
  vbo->normal=&UAnormal;
  vbo->tangent=&UAtangent;
  vbo->bitangent=&UAbitangent;
  vbo->Render();
 }
 void Disable() {
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);
 }
};

extern NormalMapping3LightShader normalmapping3lightShader;

// Source: GLM-related tutorials #13 opengl-tutorial.org
class NormalMappingAlpha3LightShader : public GLSLShader {
public:
 GLMCamera camera;
 Vertex lightPosition1,lightPosition2,lightPosition3;
 float lightPower1,lightPower2,lightPower3;
 Crayon lightColor1,lightColor2,lightColor3;
 GLuint *diffuse,*specular,*normal;
 GLuint
  Ulight1,UlightPower1,UlightColor1,
  Ulight2,UlightPower2,UlightColor2,
  Ulight3,UlightPower3,UlightColor3,
  Umvp,Uv,Um,Umv3x3,Udiffuse,Uspecular,Unormal; // color, gloss, normal
	GLuint
  UAposition,
  UAtexcoord,
  UAnormal,
  UAtangent,
  UAbitangent;
 Zp<VBOStaticVNTTanBi> vbo;
 GLint posAttrib,texAttrib;
 void Init() {
  diffuse=specular=normal=null;
  Ulight1=UlightPower1=UlightColor1=
   Ulight2=UlightPower2=UlightColor2=
   Ulight3=UlightPower3=UlightColor3=
   Umvp=Uv=Um=Umv3x3=Udiffuse=Uspecular=Unormal=0;
  UAposition=UAtexcoord=UAnormal=UAtangent=UAbitangent=0;
  lightPower1=10.0f;
  lightColor1.Pick(alabaster);
  lightPosition1.Set(0.0f,0.0f,3.0f);
  lightPower2=10.0f;
  lightColor2.Pick(alabaster);
  lightPosition2.Set(-3.0f,-3.0f,3.0f);
  lightPower3=10.0f;
  lightColor3.Pick(alabaster);
  lightPosition3.Set(3.0f,3.0f,3.0f);
  OUTPUT("NormalMappingShader:Loading\n");
  string vert=
   string("#version 120\n")+
   string("attribute vec3 position;\n")+
   string("attribute vec2 texcoord;\n")+
   string("attribute vec3 normal;\n")+
   string("attribute vec3 tangent;\n")+
   string("attribute vec3 bitangent;\n")+
   string("uniform mat4 MVP;\n")+
   string("uniform mat4 V;\n")+
   string("uniform mat4 M;\n")+
   string("uniform mat3 MV3x3;\n")+
   string("varying vec2 UV;\n")+
   string("varying vec3 Position_worldspace;\n")+
   string("varying vec3 EyeDirection_cameraspace;\n")+
   string("uniform vec3 LightPosition1_worldspace;\n")+
   string("varying vec3 LightDirection1_cameraspace;\n")+
   string("varying vec3 LightDirection1_tangentspace;\n")+
   string("uniform vec3 LightPosition2_worldspace;\n")+
   string("varying vec3 LightDirection2_cameraspace;\n")+
   string("varying vec3 LightDirection2_tangentspace;\n")+
   string("uniform vec3 LightPosition3_worldspace;\n")+
   string("varying vec3 LightDirection3_cameraspace;\n")+
   string("varying vec3 LightDirection3_tangentspace;\n")+
   string("varying vec3 EyeDirection_tangentspace;\n")+
   string("void main() {\n")+
   string(" gl_Position =  MVP * vec4(position,1);\n")+
   string(" Position_worldspace = (M * vec4(position,1)).xyz;\n")+
   string(" vec3 vertexPosition_cameraspace = ( V * M * vec4(position,1)).xyz;\n")+
   string(" EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;\n")+
   string(" vec3 LightPosition1_cameraspace = ( V * vec4(LightPosition1_worldspace,1)).xyz;\n")+
   string(" LightDirection1_cameraspace = LightPosition1_cameraspace + EyeDirection_cameraspace;\n")+
   string(" vec3 LightPosition2_cameraspace = ( V * vec4(LightPosition2_worldspace,1)).xyz;\n")+
   string(" LightDirection2_cameraspace = LightPosition2_cameraspace + EyeDirection_cameraspace;\n")+
   string(" vec3 LightPosition3_cameraspace = ( V * vec4(LightPosition3_worldspace,1)).xyz;\n")+
   string(" LightDirection3_cameraspace = LightPosition3_cameraspace + EyeDirection_cameraspace;\n")+
   string(" UV = texcoord;\n")+
   string(" vec3 T = MV3x3 * tangent;\n")+
   string(" vec3 B = MV3x3 * bitangent;\n")+
   string(" vec3 N = MV3x3 * normal;\n")+
   string(" mat3 TBN = transpose(mat3(	T,	B,	N	));\n")+
   string(" LightDirection1_tangentspace = TBN * LightDirection1_cameraspace;\n")+
   string(" LightDirection2_tangentspace = TBN * LightDirection2_cameraspace;\n")+
   string(" LightDirection3_tangentspace = TBN * LightDirection3_cameraspace;\n")+
   string(" EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;\n")+
   string("}\n")
  ;
  string frag=
   string("#version 120\n")+
   string("uniform sampler2D DiffuseMap;\n")+
   string("uniform sampler2D NormalMap;\n")+
   string("uniform sampler2D SpecularMap;\n")+
   string("uniform	vec3 LightColor1;\n")+
   string("uniform	float LightPower1;\n")+
   string("uniform	vec3 LightColor2;\n")+
   string("uniform	float LightPower2;\n")+
   string("uniform	vec3 LightColor3;\n")+
   string("uniform	float LightPower3;\n")+
   string("varying vec2 UV;\n")+
   string("varying vec3 Position_worldspace;\n")+
   string("varying vec3 EyeDirection_cameraspace;\n")+
   string("uniform vec3 LightPosition1_worldspace;\n")+
   string("varying vec3 LightDirection1_cameraspace;\n")+
   string("varying vec3 LightDirection1_tangentspace;\n")+
   string("uniform vec3 LightPosition2_worldspace;\n")+
   string("varying vec3 LightDirection2_cameraspace;\n")+
   string("varying vec3 LightDirection2_tangentspace;\n")+
   string("uniform vec3 LightPosition3_worldspace;\n")+
   string("varying vec3 LightDirection3_cameraspace;\n")+
   string("varying vec3 LightDirection3_tangentspace;\n")+
   string("varying vec3 EyeDirection_tangentspace;\n")+
   string("void main() {\n")+
   string(" vec4 DM  = texture2D( DiffuseMap, UV );\n")+
   string(" vec3 Diff = DM.rgb;\n")+
   string(" vec3 Spec  = texture2D( SpecularMap, UV ).rgb * 0.3;\n")+
   string(" vec3 Norm  = texture2D( NormalMap,   UV ).rgb;\n")+
   string(" vec3 N = normalize(Norm*2.0 - 1.0);\n")+
   string(" vec3 E = normalize(EyeDirection_tangentspace);\n")+
   // Light 1
   string(" vec3 L1 = normalize(LightDirection1_tangentspace);\n")+
   string(" vec3 R1 = reflect(-L1,N);\n")+
   string(" float cosAlpha1 = clamp( dot( E,R1 ), 0,1 );\n")+
   string(" float cosTheta1 = clamp( dot( N,L1 ), 0,1 );\n")+
   string(" vec3 Intensity1 = LightColor1 * LightPower1;\n")+
   string(" float distance1 = length( LightPosition1_worldspace - Position_worldspace );\n")+
   string(" float distanceSquared1=distance1*distance1;\n")+
   string(" vec3 D1 = Diff * Intensity1 * cosTheta1 / (distanceSquared1);\n")+
   string(" vec3 S1 = Spec * Intensity1 * pow(cosAlpha1,5) / (distanceSquared1);\n")+
   string(" vec3 final1= D1 + (0.01*S1);\n")+
   // Light 2
   string(" vec3 L2 = normalize(LightDirection2_tangentspace);\n")+
   string(" vec3 R2 = reflect(-L2,N);\n")+
   string(" float cosAlpha2 = clamp( dot( E,R2 ), 0,1 );\n")+
   string(" float cosTheta2 = clamp( dot( N,L2 ), 0,1 );\n")+
   string(" vec3 Intensity2 = LightColor2 * LightPower2;\n")+
   string(" float distance2 = length( LightPosition2_worldspace - Position_worldspace );\n")+
   string(" float distanceSquared2=distance2*distance2;\n")+
   string(" vec3 D2 = Diff * Intensity2 * cosTheta2 / (distanceSquared2);\n")+
   string(" vec3 S2 = Spec * Intensity2 * pow(cosAlpha2,5) / (distanceSquared2);\n")+
   string(" vec3 final2= D2 + (0.01*S2);\n")+
   // Light 3
   string(" vec3 L3 = normalize(LightDirection3_tangentspace);\n")+
   string(" vec3 R3 = reflect(-L3,N);\n")+
   string(" float cosAlpha3 = clamp( dot( E,R3 ), 0,1 );\n")+
   string(" float cosTheta3 = clamp( dot( N,L3 ), 0,1 );\n")+
   string(" vec3 Intensity3 = LightColor3 * LightPower3;\n")+
   string(" float distance3 = length( LightPosition3_worldspace - Position_worldspace );\n")+
   string(" float distanceSquared3=distance3*distance3;\n")+
   string(" vec3 D3 = Diff * Intensity3 * cosTheta3 / (distanceSquared3);\n")+
   string(" vec3 S3 = Spec * Intensity3 * pow(cosAlpha3,5) / (distanceSquared3);\n")+
   string(" vec3 final3= D3 + (0.01*S3);\n")+
   // Combine
   string(" gl_FragColor = vec4( final1+final2+final3, DM.a );\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  // Light0
  Uniform(&Ulight1,"LightPosition1_worldspace");
  Uniform(&UlightPower1,"LightPower1");
  Uniform(&UlightColor1,"LightColor1");
  // Light1
  Uniform(&Ulight2,"LightPosition2_worldspace");
  Uniform(&UlightPower2,"LightPower2");
  Uniform(&UlightColor2,"LightColor2");
  // Light2
  Uniform(&Ulight3,"LightPosition3_worldspace");
  Uniform(&UlightPower3,"LightPower3");
  Uniform(&UlightColor3,"LightColor3");
  // Matrices
  Uniform(&Umvp,  "MVP");
  Uniform(&Uv,    "V");
  Uniform(&Um,    "M");
  Uniform(&Umv3x3,"MV3x3");
  // Texture Sources
  Uniform(&Udiffuse,  "DiffuseMap");
  Uniform(&Uspecular,"SpecularMap");
  Uniform(&Unormal,   "NormalMap");
  // Attribute handles for VBO
 	Attribute(&UAposition, "position");
 	Attribute(&UAnormal,   "normal");
 	Attribute(&UAtexcoord, "texcoord");
 	Attribute(&UAtangent,  "tangent");
 	Attribute(&UAbitangent,"bitangent");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform3f(Ulight1, lightPosition1.x,lightPosition1.y,lightPosition1.z);
  glUniform1f(UlightPower1, lightPower1);
  glUniform3f(UlightColor1, lightColor1.rf, lightColor1.gf, lightColor1.bf);
  glUniform3f(Ulight2, lightPosition2.x,lightPosition2.y,lightPosition2.z);
  glUniform1f(UlightPower2, lightPower2);
  glUniform3f(UlightColor2, lightColor2.rf, lightColor2.gf, lightColor2.bf);
  glUniform3f(Ulight3, lightPosition3.x,lightPosition3.y,lightPosition3.z);
  glUniform1f(UlightPower3, lightPower3);
  glUniform3f(UlightColor3, lightColor3.rf, lightColor3.gf, lightColor3.bf);
		glUniformMatrix4fv(Umvp,   1, GL_FALSE, &camera.MVP[0][0]);
		glUniformMatrix4fv(Um,     1, GL_FALSE, &camera.ModelMatrix[0][0]);
		glUniformMatrix4fv(Uv,     1, GL_FALSE, &camera.ViewMatrix[0][0]);
		glUniformMatrix3fv(Umv3x3, 1, GL_FALSE, &camera.ModelView3x3Matrix[0][0]);
		glUniform1i(Udiffuse, 0);  glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, *diffuse);	
		glUniform1i(Unormal,  1);  glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, *normal);		
		glUniform1i(Uspecular, 2); glActiveTexture(GL_TEXTURE2);	glBindTexture(GL_TEXTURE_2D, *specular);
  vbo->position=&UAposition;
  vbo->texcoord=&UAtexcoord;
  vbo->normal=&UAnormal;
  vbo->tangent=&UAtangent;
  vbo->bitangent=&UAbitangent;
  vbo->Render();
 }
 void Disable() {
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);
 }
};

extern NormalMappingAlpha3LightShader normalmappingalpha3lightShader;

// Source: GLM-related tutorials #13 opengl-tutorial.org
// Same as above but allows the user to provide an additive lightmap
class NormalMappingAlpha3LightALMShader : public GLSLShader {
public:
 GLMCamera camera;
 Vertex lightPosition1,lightPosition2,lightPosition3;
 float lightPower1,lightPower2,lightPower3;
 Crayon lightColor1,lightColor2,lightColor3;
 GLuint *diffuse,*specular,*normal,*alm;
 GLuint
  Ulight1,UlightPower1,UlightColor1,
  Ulight2,UlightPower2,UlightColor2,
  Ulight3,UlightPower3,UlightColor3,
  Umvp,Uv,Um,Umv3x3,Udiffuse,Uspecular,Unormal,Uadditive; // color, gloss, normal, alm
	GLuint
  UAposition,
  UAtexcoord,
  UAnormal,
  UAtangent,
  UAbitangent;
 Zp<VBOStaticVNTTanBi> vbo;
 GLint posAttrib,texAttrib;
 void Init() {
  diffuse=specular=normal=null;
  Ulight1=UlightPower1=UlightColor1=
   Ulight2=UlightPower2=UlightColor2=
   Ulight3=UlightPower3=UlightColor3=
   Umvp=Uv=Um=Umv3x3=Udiffuse=Uspecular=Unormal=0;
  UAposition=UAtexcoord=UAnormal=UAtangent=UAbitangent=0;
  lightPower1=10.0f;
  lightColor1.Pick(alabaster);
  lightPosition1.Set(0.0f,0.0f,3.0f);
  lightPower2=10.0f;
  lightColor2.Pick(alabaster);
  lightPosition2.Set(-3.0f,-3.0f,3.0f);
  lightPower3=10.0f;
  lightColor3.Pick(alabaster);
  lightPosition3.Set(3.0f,3.0f,3.0f);
  OUTPUT("NormalMappingShader:Loading\n");
  string vert=
   string("#version 120\n")+
   string("attribute vec3 position;\n")+
   string("attribute vec2 texcoord;\n")+
   string("attribute vec3 normal;\n")+
   string("attribute vec3 tangent;\n")+
   string("attribute vec3 bitangent;\n")+
   string("uniform mat4 MVP;\n")+
   string("uniform mat4 V;\n")+
   string("uniform mat4 M;\n")+
   string("uniform mat3 MV3x3;\n")+
   string("varying vec2 UV;\n")+
   string("varying vec3 Position_worldspace;\n")+
   string("varying vec3 EyeDirection_cameraspace;\n")+
   string("uniform vec3 LightPosition1_worldspace;\n")+
   string("varying vec3 LightDirection1_cameraspace;\n")+
   string("varying vec3 LightDirection1_tangentspace;\n")+
   string("uniform vec3 LightPosition2_worldspace;\n")+
   string("varying vec3 LightDirection2_cameraspace;\n")+
   string("varying vec3 LightDirection2_tangentspace;\n")+
   string("uniform vec3 LightPosition3_worldspace;\n")+
   string("varying vec3 LightDirection3_cameraspace;\n")+
   string("varying vec3 LightDirection3_tangentspace;\n")+
   string("varying vec3 EyeDirection_tangentspace;\n")+
   string("void main() {\n")+
   string(" gl_Position =  MVP * vec4(position,1);\n")+
   string(" Position_worldspace = (M * vec4(position,1)).xyz;\n")+
   string(" vec3 vertexPosition_cameraspace = ( V * M * vec4(position,1)).xyz;\n")+
   string(" EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;\n")+
   string(" vec3 LightPosition1_cameraspace = ( V * vec4(LightPosition1_worldspace,1)).xyz;\n")+
   string(" LightDirection1_cameraspace = LightPosition1_cameraspace + EyeDirection_cameraspace;\n")+
   string(" vec3 LightPosition2_cameraspace = ( V * vec4(LightPosition2_worldspace,1)).xyz;\n")+
   string(" LightDirection2_cameraspace = LightPosition2_cameraspace + EyeDirection_cameraspace;\n")+
   string(" vec3 LightPosition3_cameraspace = ( V * vec4(LightPosition3_worldspace,1)).xyz;\n")+
   string(" LightDirection3_cameraspace = LightPosition3_cameraspace + EyeDirection_cameraspace;\n")+
   string(" UV = texcoord;\n")+
   string(" vec3 T = MV3x3 * tangent;\n")+
   string(" vec3 B = MV3x3 * bitangent;\n")+
   string(" vec3 N = MV3x3 * normal;\n")+
   string(" mat3 TBN = transpose(mat3(	T,	B,	N	));\n")+
   string(" LightDirection1_tangentspace = TBN * LightDirection1_cameraspace;\n")+
   string(" LightDirection2_tangentspace = TBN * LightDirection2_cameraspace;\n")+
   string(" LightDirection3_tangentspace = TBN * LightDirection3_cameraspace;\n")+
   string(" EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;\n")+
   string("}\n")
  ;
  string frag=
   string("#version 120\n")+
   string("uniform sampler2D DiffuseMap;\n")+
   string("uniform sampler2D NormalMap;\n")+
   string("uniform sampler2D SpecularMap;\n")+
   string("uniform sampler2D AdditiveMap;\n")+
   string("uniform	vec3 LightColor1;\n")+
   string("uniform	float LightPower1;\n")+
   string("uniform	vec3 LightColor2;\n")+
   string("uniform	float LightPower2;\n")+
   string("uniform	vec3 LightColor3;\n")+
   string("uniform	float LightPower3;\n")+
   string("varying vec2 UV;\n")+
   string("varying vec3 Position_worldspace;\n")+
   string("varying vec3 EyeDirection_cameraspace;\n")+
   string("uniform vec3 LightPosition1_worldspace;\n")+
   string("varying vec3 LightDirection1_cameraspace;\n")+
   string("varying vec3 LightDirection1_tangentspace;\n")+
   string("uniform vec3 LightPosition2_worldspace;\n")+
   string("varying vec3 LightDirection2_cameraspace;\n")+
   string("varying vec3 LightDirection2_tangentspace;\n")+
   string("uniform vec3 LightPosition3_worldspace;\n")+
   string("varying vec3 LightDirection3_cameraspace;\n")+
   string("varying vec3 LightDirection3_tangentspace;\n")+
   string("varying vec3 EyeDirection_tangentspace;\n")+
   string("void main() {\n")+
   string(" vec4 DM  = texture2D( DiffuseMap, UV );\n")+
   string(" vec3 ALM  = texture2D( AdditiveMap, UV ).rgb;\n")+
   string(" vec3 Diff = DM.rgb;\n")+
   string(" vec3 Spec  = texture2D( SpecularMap, UV ).rgb * 0.3;\n")+
   string(" vec3 Norm  = texture2D( NormalMap,   UV ).rgb;\n")+
   string(" vec3 N = normalize(Norm*2.0 - 1.0);\n")+
   string(" vec3 E = normalize(EyeDirection_tangentspace);\n")+
   // Light 1
   string(" vec3 L1 = normalize(LightDirection1_tangentspace);\n")+
   string(" vec3 R1 = reflect(-L1,N);\n")+
   string(" float cosAlpha1 = clamp( dot( E,R1 ), 0,1 );\n")+
   string(" float cosTheta1 = clamp( dot( N,L1 ), 0,1 );\n")+
   string(" vec3 Intensity1 = LightColor1 * LightPower1 + ALM;\n")+
   string(" float distance1 = length( LightPosition1_worldspace - Position_worldspace );\n")+
   string(" float distanceSquared1=distance1*distance1;\n")+
   string(" vec3 D1 = Diff * Intensity1 * cosTheta1 / (distanceSquared1);\n")+
   string(" vec3 S1 = Spec * Intensity1 * pow(cosAlpha1,5) / (distanceSquared1);\n")+
   string(" vec3 final1= D1 + (0.01*S1);\n")+
   // Light 2
   string(" vec3 L2 = normalize(LightDirection2_tangentspace);\n")+
   string(" vec3 R2 = reflect(-L2,N);\n")+
   string(" float cosAlpha2 = clamp( dot( E,R2 ), 0,1 );\n")+
   string(" float cosTheta2 = clamp( dot( N,L2 ), 0,1 );\n")+
   string(" vec3 Intensity2 = LightColor2 * LightPower2 + ALM;\n")+
   string(" float distance2 = length( LightPosition2_worldspace - Position_worldspace );\n")+
   string(" float distanceSquared2=distance2*distance2;\n")+
   string(" vec3 D2 = Diff * Intensity2 * cosTheta2 / (distanceSquared2);\n")+
   string(" vec3 S2 = Spec * Intensity2 * pow(cosAlpha2,5) / (distanceSquared2);\n")+
   string(" vec3 final2= D2 + (0.01*S2);\n")+
   // Light 3
   string(" vec3 L3 = normalize(LightDirection3_tangentspace);\n")+
   string(" vec3 R3 = reflect(-L3,N);\n")+
   string(" float cosAlpha3 = clamp( dot( E,R3 ), 0,1 );\n")+
   string(" float cosTheta3 = clamp( dot( N,L3 ), 0,1 );\n")+
   string(" vec3 Intensity3 = LightColor3 * LightPower3 + ALM;\n")+
   string(" float distance3 = length( LightPosition3_worldspace - Position_worldspace );\n")+
   string(" float distanceSquared3=distance3*distance3;\n")+
   string(" vec3 D3 = Diff * Intensity3 * cosTheta3 / (distanceSquared3);\n")+
   string(" vec3 S3 = Spec * Intensity3 * pow(cosAlpha3,5) / (distanceSquared3);\n")+
   string(" vec3 final3= D3 + (0.01*S3);\n")+
   // Combine
   string(" gl_FragColor = vec4( final1+final2+final3, DM.a );\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  // Light0
  Uniform(&Ulight1,"LightPosition1_worldspace");
  Uniform(&UlightPower1,"LightPower1");
  Uniform(&UlightColor1,"LightColor1");
  // Light1
  Uniform(&Ulight2,"LightPosition2_worldspace");
  Uniform(&UlightPower2,"LightPower2");
  Uniform(&UlightColor2,"LightColor2");
  // Light2
  Uniform(&Ulight3,"LightPosition3_worldspace");
  Uniform(&UlightPower3,"LightPower3");
  Uniform(&UlightColor3,"LightColor3");
  // Matrices
  Uniform(&Umvp,  "MVP");
  Uniform(&Uv,    "V");
  Uniform(&Um,    "M");
  Uniform(&Umv3x3,"MV3x3");
  // Texture Sources
  Uniform(&Udiffuse,  "DiffuseMap");
  Uniform(&Uspecular, "SpecularMap");
  Uniform(&Unormal,   "NormalMap");
  Uniform(&Uadditive, "AdditiveMap");
  // Attribute handles for VBO
 	Attribute(&UAposition, "position");
 	Attribute(&UAnormal,   "normal");
 	Attribute(&UAtexcoord, "texcoord");
 	Attribute(&UAtangent,  "tangent");
 	Attribute(&UAbitangent,"bitangent");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform3f(Ulight1, lightPosition1.x,lightPosition1.y,lightPosition1.z);
  glUniform1f(UlightPower1, lightPower1);
  glUniform3f(UlightColor1, lightColor1.rf, lightColor1.gf, lightColor1.bf);
  glUniform3f(Ulight2, lightPosition2.x,lightPosition2.y,lightPosition2.z);
  glUniform1f(UlightPower2, lightPower2);
  glUniform3f(UlightColor2, lightColor2.rf, lightColor2.gf, lightColor2.bf);
  glUniform3f(Ulight3, lightPosition3.x,lightPosition3.y,lightPosition3.z);
  glUniform1f(UlightPower3, lightPower3);
  glUniform3f(UlightColor3, lightColor3.rf, lightColor3.gf, lightColor3.bf);
		glUniformMatrix4fv(Umvp,   1, GL_FALSE, &camera.MVP[0][0]);
		glUniformMatrix4fv(Um,     1, GL_FALSE, &camera.ModelMatrix[0][0]);
		glUniformMatrix4fv(Uv,     1, GL_FALSE, &camera.ViewMatrix[0][0]);
		glUniformMatrix3fv(Umv3x3, 1, GL_FALSE, &camera.ModelView3x3Matrix[0][0]);
		glUniform1i(Udiffuse, 0);  glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, *diffuse);	
		glUniform1i(Unormal,  1);  glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, *normal);		
		glUniform1i(Uspecular, 2); glActiveTexture(GL_TEXTURE2);	glBindTexture(GL_TEXTURE_2D, *specular);
		glUniform1i(Uadditive, 3); glActiveTexture(GL_TEXTURE3);	glBindTexture(GL_TEXTURE_2D, *alm);
  vbo->position=&UAposition;
  vbo->texcoord=&UAtexcoord;
  vbo->normal=&UAnormal;
  vbo->tangent=&UAtangent;
  vbo->bitangent=&UAbitangent;
  vbo->Render();
 }
 void Disable() {
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);
 }
};

extern NormalMappingAlpha3LightALMShader normalmappingalpha3lightalmShader;
