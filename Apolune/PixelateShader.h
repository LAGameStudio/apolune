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

#include "Art.h"
#include "GLWindow.h"
#include "GLSL2Shader.h"
#include "TextureLibrary.h"

class CoverageProfile;
class HQ4XProfile;

void ShadedCoverage( FBOColor *out, CoverageProfile *in );
void ShadedCoverage( FBOColor *out, CoverageProfile *in, FBOColor *in2 );
void ShadedHQ4X( FBOColor *out, HQ4XProfile *in );
void ShadedHQ4X( FBOColor *in, FBOColor *out );
void ShadedHQ4X( GLImage *in, FBOColor *out );
void ShadedPixelate( FBOColor *out, GLuint *source, float sourceW, float sourceH );

class PixelateShader : public GLSLShader {
public:
 float resolution[2];
 GLuint *pTex0;
 GLint Uresolution,Upixel,Utex0;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  resolution[0]=1024.0f;
  resolution[1]=1024.0f;
  pTex0=null;
  OUTPUT("PixelateShader::Loading\n");
  string vert=
    string("#version 330\n")
   +string("in vec2 position;\n")
   +string("in vec2 texcoord;\n")
   +string("uniform mat4 trans;\n")
   +string("out vec2 Texcoord;\n")
   +string("void main() {\n")
   +string(" Texcoord = texcoord;\n")
   +string(" gl_Position = trans * vec4(position, 0.0, 1.0);\n")
   +string("}\n")
  ;
  string frag=
    string("#version 330 core\n")
   +string("in vec2 Texcoord;\n")
   +string("out vec4 outColor;\n")+
   string("uniform sampler2D sceneTex; // 0\n")+
   string("uniform vec2 resolution;\n")+
//   string("uniform vec2 pixel;\n")+
   string("void main()\n")+
   string("{\n")+
   string(" vec2 uv = Texcoord.xy * resolution.xy;\n")+
   string(" outColor = texture2D(sceneTex, uv);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Uresolution=glGetUniformLocation(program, "resolution");
//  Upixel=glGetUniformLocation(program,"pixel");
  Utex0=glGetUniformLocation(program, "sceneTex");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Uresolution, resolution[0], resolution[1]);
//  glUniform2f(Upixel, pixel[0], pixel[1]);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

  glUniformMatrix4fv(Utrans, 1, GL_FALSE, glm::value_ptr(trans));
  glReportError(glGetError());
  glEnableVertexAttribArray(posAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
  glReportError(glGetError());
  glEnableVertexAttribArray(texAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
  glReportError(glGetError());
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
 
};

extern PixelateShader pixelateShader;

ONE(CoverageProfile,{})
  Zp<GLImage> tex;
 Zdouble bitmap_width,bitmap_height,x_scale,y_scale;
 KEYWORDS({
  TILEWORD("tex",tex)
  else KEYWORD("bitmap_width",{bitmap_width=w;})
  else KEYWORD("bitmap_height",{bitmap_height=w;})
  else KEYWORD("x_scale",{x_scale=w;})
  else KEYWORD("y_scale",{y_scale=w;})
 })
 WORDKEYS({
  IMGKEY("tex",tex)
  NUMBKEY("bitmap_width",bitmap_width)
  NUMBKEY("bitmap_height",bitmap_height)
  NUMBKEY("x_scale",x_scale)
  NUMBKEY("y_scale",y_scale)
 })
MANY(CoverageProfile,CoverageProfileHandle,CoverageProfileHandles,"Coverage",CoverageProfiles,{})
 KEYWORDSGroup("Coverage",CoverageProfile);
 WORDKEYSGroup("Coverage",CoverageProfile);
DONE(CoverageProfile);

class CoverageShader : public GLSLShader {
public:
 Zp<GLuint> tex;
 Zdouble bitmap_width,bitmap_height,x_scale,y_scale;
 GLuint Utex,Ubitmap_width,Ubitmap_height,Ux_scale,Uy_scale;
 GLuint Utrans; GLint posAttrib,colAttrib,texAttrib;
 void Init() {
  OUTPUT("CoverageShader::Loading\n");
  string vert=
    string("#version 330\n")
   +string("in vec3 color;")
   +string("in vec2 position;\n")
   +string("in vec2 texcoord;\n")
   +string("uniform mat4 trans;\n")
   +string("out vec2 Texcoord;\n")
   +string("out vec3 Color;\n")
   +string("void main() {\n")
   +string(" Color = color;")
   +string(" Texcoord = texcoord;\n")
   +string(" gl_Position = trans * vec4(position, 0.0, 1.0);\n")
   +string("}\n")
  ;
  string frag=
    string("#version 330 core\n")
   +string("in vec3 Color;\n")
   +string("in vec2 Texcoord;\n")
   +string("out vec4 outColor;\n")
   +string("uniform float bitmap_width;\n")
   +string("uniform float bitmap_height;\n")
   +string("uniform float x_scale;\n")
   +string("uniform float y_scale;\n")
   +string("uniform sampler2D tex;\n")
   +string("\n")
   +string("void main()\n")
   +string("{\n")
   +string("	vec2 sz = vec2(bitmap_width, bitmap_height);\n")
   +string("	vec3 step = vec3(1.0 / x_scale, 1.0 / y_scale, 0);\n")
   +string("	vec2 tex_pixel = sz * Texcoord - step.xy / 2;\n")
   +string("	\n")
   +string("	vec2 corner = floor(tex_pixel) + 1;\n")
   +string("	vec2 frac = min((corner - tex_pixel) * vec2(x_scale, y_scale), vec2(1.0, 1.0));\n")
   +string("	\n")
   +string("	vec4 c1 = texture2D(tex, (floor(tex_pixel + step.zz) + 0.5) / sz);\n")
   +string("	vec4 c2 = texture2D(tex, (floor(tex_pixel + step.xz) + 0.5) / sz);\n")
   +string("	vec4 c3 = texture2D(tex, (floor(tex_pixel + step.zy) + 0.5) / sz);\n")
   +string("	vec4 c4 = texture2D(tex, (floor(tex_pixel + step.xy) + 0.5) / sz);\n")
   +string("	\n")
   +string("	c1 *=        frac.x  *        frac.y;\n")
   +string("	c2 *= (1.0 - frac.x) *        frac.y;\n")
   +string("	c3 *=        frac.x  * (1.0 - frac.y);\n")
   +string("	c4 *= (1.0 - frac.x) * (1.0 - frac.y);\n")
   +string("	\n")
   +string("	outColor = vec4(Color,1.0) * (c1 + c2 + c3 + c4);\n")
   +string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex=glGetUniformLocation(program, "tex");
  Ubitmap_width=glGetUniformLocation(program, "bitmap_width");
  Ubitmap_height=glGetUniformLocation(program, "bitmap_height");
  Ux_scale=glGetUniformLocation(program, "x_scale");
  Uy_scale=glGetUniformLocation(program, "y_scale");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
  colAttrib = glGetAttribLocation(program, "color");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Ubitmap_width,(GLfloat)bitmap_width);
  glUniform1f(Ubitmap_height,(GLfloat)bitmap_height);
  glUniform1f(Ux_scale,(GLfloat)x_scale);
  glUniform1f(Uy_scale,(GLfloat)y_scale);
  glUniform1i(Utex, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, (*tex.pointer));

  glUniformMatrix4fv(Utrans, 1, GL_FALSE, glm::value_ptr(trans));
  glReportError(glGetError());
  glEnableVertexAttribArray(posAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
  glReportError(glGetError());
  glEnableVertexAttribArray(colAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  glReportError(glGetError());
  glEnableVertexAttribArray(texAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
  glReportError(glGetError());
 }
 void Disable() {
  glUseProgram(0);
 }
 void Set( CoverageProfile *in ) {
  tex=&in->tex->texture;
  bitmap_width=in->bitmap_width;
  bitmap_height=in->bitmap_height;
  x_scale=in->x_scale;
  y_scale=in->y_scale;
 }
 void Set( FBOColor *in2, CoverageProfile *in ) {
  tex=&in2->texture;
  bitmap_width=in->bitmap_width;
  bitmap_height=in->bitmap_height;
  x_scale=in->x_scale;
  y_scale=in->y_scale;
 }
 void Render( FBOColor *out, CoverageProfile *in );
};


extern CoverageShader coverageShader;

ONE(HQ4XProfile,{
 resolution.Set(1.0,1.0);
})
  Zp<GLImage> tex;
 Cartesiand resolution;
 KEYWORDS({
  TILEWORD("tex",tex)
  else SUBWORD("resolution",resolution)
 })
 WORDKEYS({
  IMGKEY("tex",tex)
  SSUBKEY("resolution",resolution)
 })
MANY(HQ4XProfile,HQ4XProfileHandle,HQ4XProfileHandles,"HQ4X",HQ4XProfiles,{})
 KEYWORDSGroup("HQ4X",HQ4XProfile);
 WORDKEYSGroup("HQ4X",HQ4XProfile);
DONE(HQ4XProfile);


/*
// hq4x filter
// Ripped from https://github.com/libretro/common-shaders/blob/master/hqx/hq4x.cg

const float SEG = 40.0 / 256.0;
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv =  vec2(SEG,-1.0) * fragCoord.xy / iResolution.xy;
	uv.y++;
	uv.x += SEG*float(int(mod(iGlobalTime * 10.0, 6.0)));
	
	float mx = 1.0; // start smoothing wt.
 	const float k = -1.10; // wt. decrease factor
 	const float max_w = 0.75; // max filter weigth
 	const float min_w = 0.03; // min filter weigth
 	const float lum_add = 0.33; // effects smoothing
	
	vec4 color = texture(iChannel0, uv);
	   vec3 c = color.xyz;
		
	
	   float x = 0.5 * (1.0 / 256.0);
	   float y = 0.5 * (1.0 / 32.0);
	
		const vec3 dt = 1.0*vec3(1.0, 1.0, 1.0);
	
	   vec2 dg1 = vec2( x, y);
	   vec2 dg2 = vec2(-x, y);
	
	   vec2 sd1 = dg1*0.5;
	   vec2 sd2 = dg2*0.5;
	
	   vec2 ddx = vec2(x,0.0);
	   vec2 ddy = vec2(0.0,y);
	
		vec4 t1 = vec4(uv-sd1,uv-ddy);
		vec4 t2 = vec4(uv-sd2,uv+ddx);
		vec4 t3 = vec4(uv+sd1,uv+ddy);
		vec4 t4 = vec4(uv+sd2,uv-ddx);
		vec4 t5 = vec4(uv-dg1,uv-dg2);
		vec4 t6 = vec4(uv+dg1,uv+dg2);
			
	   vec3 i1 = texture(iChannel0, t1.xy).xyz;
	   vec3 i2 = texture(iChannel0, t2.xy).xyz;
	   vec3 i3 = texture(iChannel0, t3.xy).xyz;
	   vec3 i4 = texture(iChannel0, t4.xy).xyz;
	
	   vec3 o1 = texture(iChannel0, t5.xy).xyz;
	   vec3 o3 = texture(iChannel0, t6.xy).xyz;
	   vec3 o2 = texture(iChannel0, t5.zw).xyz;
	   vec3 o4 = texture(iChannel0, t6.zw).xyz;
	
	   vec3 s1 = texture(iChannel0, t1.zw).xyz;
	   vec3 s2 = texture(iChannel0, t2.zw).xyz;
	   vec3 s3 = texture(iChannel0, t3.zw).xyz;
	   vec3 s4 = texture(iChannel0, t4.zw).xyz;
	
	   float ko1 = dot(abs(o1-c),dt);
	   float ko2 = dot(abs(o2-c),dt);
	   float ko3 = dot(abs(o3-c),dt);
	   float ko4 = dot(abs(o4-c),dt);
	
	   float k1=min(dot(abs(i1-i3),dt),max(ko1,ko3));
	   float k2=min(dot(abs(i2-i4),dt),max(ko2,ko4));
	
	   float w1 = k2; if(ko3<ko1) w1*=ko3/ko1;
	   float w2 = k1; if(ko4<ko2) w2*=ko4/ko2;
	   float w3 = k2; if(ko1<ko3) w3*=ko1/ko3;
	   float w4 = k1; if(ko2<ko4) w4*=ko2/ko4;
	
	   c=(w1*o1+w2*o2+w3*o3+w4*o4+0.001*c)/(w1+w2+w3+w4+0.001);
	   w1 = k*dot(abs(i1-c)+abs(i3-c),dt)/(0.125*dot(i1+i3,dt)+lum_add);
	   w2 = k*dot(abs(i2-c)+abs(i4-c),dt)/(0.125*dot(i2+i4,dt)+lum_add);
	   w3 = k*dot(abs(s1-c)+abs(s3-c),dt)/(0.125*dot(s1+s3,dt)+lum_add);
	   w4 = k*dot(abs(s2-c)+abs(s4-c),dt)/(0.125*dot(s2+s4,dt)+lum_add);
	
	   w1 = clamp(w1+mx,min_w,max_w);
	   w2 = clamp(w2+mx,min_w,max_w);
	   w3 = clamp(w3+mx,min_w,max_w);
	   w4 = clamp(w4+mx,min_w,max_w);
	
	color = vec4((w1*(i1+i3)+w2*(i2+i4)+w3*(s1+s3)+w4*(s2+s4)+c)/(2.0*(w1+w2+w3+w4)+1.0), 1.0);
	fragColor = color; //texture(iChannel0, uv);
}
*/

class HQ4XShader : public GLSLShader {
public:
 Zp<GLuint> tex;
 Cartesiand resolution;
 GLuint Utex,Uresolution;
 GLuint Utrans; GLint posAttrib,colAttrib,texAttrib;
 void Init() {
  resolution.Set(1.0,1.0);
  OUTPUT("HQ4XShader::Loading\n");
  string vert=
    string("#version 330\n")
   +string("in vec3 color;")
   +string("in vec2 position;\n")
   +string("in vec2 texcoord;\n")
   +string("uniform mat4 trans;\n")
   +string("out vec2 Texcoord;\n")
   +string("out vec3 Color;\n")
   +string("void main() {\n")
   +string(" Color = color;")
   +string(" Texcoord = texcoord;\n")
   +string(" gl_Position = trans * vec4(position, 0.0, 1.0);\n")
   +string("}\n")
  ;
  string frag=
    string("#version 330 core\n")
   +string("in vec3 Color;\n")
   +string("in vec2 Texcoord;\n")
   +string("out vec4 outColor;\n")+
    string("uniform sampler2D tex;\n")
   +string("uniform vec2 resolution;\n")
   +string("void main() {\n")
   +string(" vec2 uv =  Texcoord.xy / 2.0 - vec2(0.0,0.5);\n")
   +string(" uv.y++;\n")
   +string(" float mx = 1.0; // start smoothing wt.\n")
   +string(" const float k = -1.10; // wt. decrease factor\n")
   +string(" const float max_w = 0.75; // max filter weigth\n")
   +string(" const float min_w = 0.03; // min filter weigth\n")
   +string(" const float lum_add = 0.33; // effects smoothing\n")
   +string(" vec4 color = texture2D(tex, uv);\n")
   +string(" vec3 c = color.xyz;\n")
   +string(" float x = 0.5 * (1.0 / resolution.x);\n")
   +string(" float y = 0.5 * (1.0 / resolution.y);\n")
   +string(" const vec3 dt = vec3(1.0, 1.0, 1.0);\n")
   +string(" vec2 dg1 = vec2( x, y);\n")
   +string(" vec2 dg2 = vec2(-x, y);\n")
   +string(" vec2 sd1 = dg1*0.5;\n")
   +string(" vec2 sd2 = dg2*0.5;\n")
   +string(" vec2 ddx = vec2(x,0.0);\n")
   +string(" vec2 ddy = vec2(0.0,y);\n")
   +string(" vec4 t1 = vec4(uv-sd1,uv-ddy);\n")
   +string(" vec4 t2 = vec4(uv-sd2,uv+ddx);\n")
   +string(" vec4 t3 = vec4(uv+sd1,uv+ddy);\n")
   +string(" vec4 t4 = vec4(uv+sd2,uv-ddx);\n")
   +string(" vec4 t5 = vec4(uv-dg1,uv-dg2);\n")
   +string(" vec4 t6 = vec4(uv+dg1,uv+dg2);\n")
   +string(" vec3 i1 = texture2D(tex, t1.xy).xyz;\n")
   +string(" vec3 i2 = texture2D(tex, t2.xy).xyz;\n")
   +string(" vec3 i3 = texture2D(tex, t3.xy).xyz;\n")
   +string(" vec3 i4 = texture2D(tex, t4.xy).xyz;\n")
   +string(" vec3 o1 = texture2D(tex, t5.xy).xyz;\n")
   +string(" vec3 o3 = texture2D(tex, t6.xy).xyz;\n")
   +string(" vec3 o2 = texture2D(tex, t5.zw).xyz;\n")
   +string(" vec3 o4 = texture2D(tex, t6.zw).xyz;\n")
   +string(" vec3 s1 = texture2D(tex, t1.zw).xyz;\n")
   +string(" vec3 s2 = texture2D(tex, t2.zw).xyz;\n")
   +string(" vec3 s3 = texture2D(tex, t3.zw).xyz;\n")
   +string(" vec3 s4 = texture2D(tex, t4.zw).xyz;\n")
   +string(" float ko1 = dot(abs(o1-c),dt);\n")
   +string(" float ko2 = dot(abs(o2-c),dt);\n")
   +string(" float ko3 = dot(abs(o3-c),dt);\n")
   +string(" float ko4 = dot(abs(o4-c),dt);\n")
   +string(" float k1=min(dot(abs(i1-i3),dt),max(ko1,ko3));\n")
   +string(" float k2=min(dot(abs(i2-i4),dt),max(ko2,ko4));\n")
   +string(" float w1 = k2; if(ko3 < ko1) w1*=ko3/ko1;\n")
   +string(" float w2 = k1; if(ko4 < ko2) w2*=ko4/ko2;\n")
   +string(" float w3 = k2; if(ko1 < ko3) w3*=ko1/ko3;\n")
   +string(" float w4 = k1; if(ko2 < ko4) w4*=ko2/ko4;\n")
   +string(" c=(w1*o1+w2*o2+w3*o3+w4*o4+0.001*c)/(w1+w2+w3+w4+0.001);\n")
   +string(" w1 = k*dot(abs(i1-c)+abs(i3-c),dt)/(0.125*dot(i1+i3,dt)+lum_add);\n")
   +string(" w2 = k*dot(abs(i2-c)+abs(i4-c),dt)/(0.125*dot(i2+i4,dt)+lum_add);\n")
   +string(" w3 = k*dot(abs(s1-c)+abs(s3-c),dt)/(0.125*dot(s1+s3,dt)+lum_add);\n")
   +string(" w4 = k*dot(abs(s2-c)+abs(s4-c),dt)/(0.125*dot(s2+s4,dt)+lum_add);\n")
   +string(" w1 = clamp(w1+mx,min_w,max_w);\n")
   +string(" w2 = clamp(w2+mx,min_w,max_w);\n")
   +string(" w3 = clamp(w3+mx,min_w,max_w);\n")
   +string(" w4 = clamp(w4+mx,min_w,max_w);\n")
   +string(" color = vec4((w1*(i1+i3)+w2*(i2+i4)+w3*(s1+s3)+w4*(s2+s4)+c)/(2.0*(w1+w2+w3+w4)+1.0), 1.0);\n")
   +string(" outColor = color * vec4(Color,1.0);\n")
   +string("}")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex=glGetUniformLocation(program, "tex");
  Uresolution=glGetUniformLocation(program, "resolution");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
  colAttrib = glGetAttribLocation(program, "color");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
  glUniform1i(Utex, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, (*tex.pointer));

  glUniformMatrix4fv(Utrans, 1, GL_FALSE, glm::value_ptr(trans));
  glReportError(glGetError());
  glEnableVertexAttribArray(posAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
  glReportError(glGetError());
  glEnableVertexAttribArray(colAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  glReportError(glGetError());
  glEnableVertexAttribArray(texAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
  glReportError(glGetError());
 }
 void Disable() {
  glUseProgram(0);
 }
 void Set( HQ4XProfile *in ) {
  tex=&in->tex->texture;
  resolution.Set(&in->resolution);
 }
 void Render( FBOColor *out, HQ4XProfile *in );
};


extern HQ4XShader hq4xShader;
