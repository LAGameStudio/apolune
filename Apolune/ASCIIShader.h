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

#include "Crayon.h"
#include "Art.h"
#include "GLWindow.h"
#include "GLSL2Shader.h"
#include "TextureLibrary.h"

ONE(ASCIIProfile,{
 res.Set(1.0,1.0);
})
 Cartesiand res;
  Zp<GLImage> tex0;
 KEYWORDS({
  SUBWORD("res",res)
  else TILEWORD("tex0",tex0)
 })
 WORDKEYS({
  SSUBKEY("res",res)
  IMGKEY("tex0",tex0)
 })
MANY(ASCIIProfile,ASCIIProfileHandle,ASCIIProfileHandles,"ASCII",ASCIIProfiles,{})
 KEYWORDSGroup("ASCII",ASCIIProfile);
 WORDKEYSGroup("ASCII",ASCIIProfile);
DONE(ASCIIProfile);

class ASCIIShader : public GLSLShader {
public:
  Cartesiand res;
  Zp<GLuint> tex0;
 GLuint Ures,Utex0;
 GLint posAttrib,texAttrib;
 GLuint Utrans;
 void Init() {
  OUTPUT("ASCIIShader::Loading\n");
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
   +string("out vec4 outColor;\n")
   +string("in vec2 Texcoord;\n")
   +string("uniform vec2 res;\n")
   +string("uniform sampler2D tex0;\n")
   +string("float character(float n, vec2 p) {\n")
   +string("  p = floor(p*vec2(4.0, -4.0) + 1.5);\n")
   +string("  if (clamp(p.x,0.0, 4.0) == p.x && clamp(p.y, 0.0, 4.0) == p.y) {\n")
   +string("  if (int(mod(n/exp2(p.x + 5.0*p.y), 2.0)) == 1) return 1.0;\n")
   +string(" } else return 0.0;\n")
   +string("}\n")
   +string("void main() {\n")
   +string(" vec2 uv=Texcoord*res;\n")
   +string(" vec3 col = texture2D(tex0,floor(uv/8.0)*8.0/res.xy).rgb; \n")
   +string(" float gray = (col.r + col.g + col.b)/3.0;	\n")
   +string(" float n =  65536.0;              // .\n")
   +string(" if (gray >= 0.2) n = 65600.0;    // :\n")
   +string(" if (gray >= 0.3) n = 332772.0;   // *\n")
   +string(" if (gray >= 0.4) n = 15255086.0; // o \n")
   +string(" if (gray >= 0.6) n = 23385164.0; // &\n")
   +string(" if (gray >= 0.7) n = 15252014.0; // 8\n")
   +string(" if (gray >= 0.8) n = 13199452.0; // @\n")
   +string(" if (gray >= 0.9) n = 11512810.0; // #	\n")
   +string(" vec2 p = mod(uv/4.0, 2.0) - vec2(1.0);\n")
   +string(" col = col*character(n, p);	\n")
   +string(" outColor = vec4(col, 1.0);\n")
   +string("}")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Ures=glGetUniformLocation(program, "res");
  Utex0=glGetUniformLocation(program, "tex0");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Ures,(GLfloat)res.x,(GLfloat)res.y);
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, (*tex0.pointer));

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
 void Set( ASCIIProfile *in ) {
  res.Set(&in->res);
  if ( in->tex0 ) tex0 = &in->tex0->texture;
 }
 void Render( FBOColor *out, ASCIIProfile *in );
};

extern ASCIIShader asciiShader;

void ShadedASCII( FBOColor *out, ASCIIProfile *in );
void ShadedASCII( FBOColor *out, ASCIIProfile *in, GLuint *input );


ONE(P2xProfile,{
 res.Set(1.0,1.0);
 rate=5.0;
})
 Cartesiand res;
 Zdouble rate;
  Zp<GLImage> tex0;
 KEYWORDS({
  SUBWORD("res",res)
  else TILEWORD("tex0",tex0)
  else NUMWORD("rate",rate)
 })
 WORDKEYS({
  SSUBKEY("res",res)
  IMGKEY("tex0",tex0)
 })
MANY(P2xProfile,P2xProfileHandle,P2xProfileHandles,"P2x",P2xProfiles,{})
 KEYWORDSGroup("P2x",P2xProfile);
 WORDKEYSGroup("P2x",P2xProfile);
DONE(P2xProfile);

class P2xShader : public GLSLShader {
public:
 Cartesiand res;
 Zp<GLuint> tex0;
 Zdouble rate;
 GLuint Ures,Utex0,Urate;
 GLint posAttrib,texAttrib;
 GLuint Utrans;
 void Init() {
  OUTPUT("P2xShader::Loading\n");
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
   +string("out vec4 outColor;\n")
   +string("uniform vec2 res;\n")
   +string("uniform sampler2D tex0;\n")
   +string("uniform float rate;\n")
   +string("void main() {\n")
   +string(" vec2 uv=Texcoord*res;\n")
   +string(" vec3 col = texture2D(tex0,floor(uv/rate)*rate/res.xy).rgb; \n")
   +string(" outColor = vec4(col, 1.0);\n")
   +string("}")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Ures=glGetUniformLocation(program, "res");
  Utex0=glGetUniformLocation(program, "tex0");
  Urate=glGetUniformLocation(program, "rate");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Ures,(GLfloat)res.x,(GLfloat)res.y);
  glUniform1f(Urate,(GLfloat)rate);
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, (*tex0.pointer));

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
 void Set( P2xProfile *in ) {
  res.Set(&in->res);
  rate=in->rate;
  if ( in->tex0 ) tex0 = &in->tex0->texture;
 }
 void Render( FBOColor *out, P2xProfile *in );
};

extern P2xShader p2xShader;

void ShadedP2x( FBOColor *out, P2xProfile *in );
void ShadedP2x( FBOColor *out, P2xProfile *in, GLuint *input );