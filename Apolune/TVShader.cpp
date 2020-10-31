#include "TVShader.h"

TVShader tvShader;

OldTVShader oldtvShader;

void ShadedTV( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 tvShader.pTex0=in;
 out->QuadYInverted(&tvShader);
 out->Unbind();
}

void ShadedOldTV( FBOColor *out, OldTVProfile *in ) {
 out->background.Pick(clear);
 out->Bind();
 oldtvShader.Set(in);
 out->QuadYInverted(&oldtvShader);
 out->Unbind();
}

void OldTVShader::Render( FBOColor *out, OldTVProfile *in ) {
 out->background.Pick(clear);
 out->Bind();
 oldtvShader.Set(in);
 out->QuadYInverted(&oldtvShader);
 out->Unbind();
}


 void TVShader::Init() {
  time=0.0f;
  pTex0=null;
  OUTPUT("TVShader::Loading\n");
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
   string("uniform float time;\n")+
   string("uniform sampler2D tex0;\n")+
   string("void main(void)\n")+
   string("{\n")+
   string("    vec2 q = Texcoord;\n")+
   string("    vec2 uv = 0.5 + (q-0.5)*(0.9 + 0.1*sin(0.2*time));\n")+
   string("    vec3 oricol = texture2D(tex0,vec2(q.x,q.y)).xyz;\n")+
   string("    vec3 col;\n")+
   string("    col.r = texture2D(tex0,vec2(uv.x+0.003+0.003*sin(0.1*time),uv.y)).x;\n")+
   string("    col.g = texture2D(tex0,vec2(uv.x+0.000,uv.y)).y;\n")+
   string("    col.b = texture2D(tex0,vec2(uv.x-0.003-0.003*sin(0.1*time),uv.y)).z;\n")+
   string(" //   col = clamp(col*0.5+0.5*col*col*1.2,0.0,1.0);\n")+
   string("    col *= 0.9+0.1*sin(10.0*time+uv.y);\n")+
   string("    outColor = vec4(col.r,col.g,col.b,1.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 
 void OldTVShader::Init() {
  OUTPUT("OldTVShader::Loading\n");
  string vert=
    string("#version 330\n")
   +string("in vec2 position;\n")
   +string("in vec2 texcoord;\n")
   +string("uniform mat4 trans;\n")
   +string("out vec2 Texcoord;\n")
   +string("\n")
   +string("void main() {\n")
   +string(" Texcoord = texcoord;\n")
   +string(" gl_Position = trans * vec4(position, 0.0, 1.0);\n")
   +string("}")
  ;
  string frag=
    string("#version 330 core\n")
   +string("in vec2 Texcoord;\n")
   +string("out vec4 outColor;\n")
   +string("// change these values to 0.0 to turn off individual effects\n")
   +string("uniform float time;\n")
   +string("uniform float vertJerkOpt;     // = 1.0;\n")
   +string("uniform float vertMovementOpt; // = 1.0;\n")
   +string("uniform float bottomStaticOpt; // = 1.0;\n")
   +string("uniform float scalinesOpt;     // = 1.0;\n")
   +string("uniform float rgbOffsetOpt;    // = 1.0;\n")
   +string("uniform float horzFuzzOpt;     // = 1.0;\n")
   +string("uniform sampler2D tex;\n")
   +string("vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }\n")
   +string("vec2 mod289(vec2 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }\n")
   +string("vec3 permute(vec3 x) { return mod289(((x*34.0)+1.0)*x); }\n")
   +string("float snoise(vec2 v) {\n")
   +string("  const vec4 C = vec4(0.211324865405187,0.366025403784439,-0.577350269189626,0.024390243902439); \n")
   +string("  vec2 i  = floor(v + dot(v, C.yy) );\n")
   +string("  vec2 x0 = v -   i + dot(i, C.xx);\n")
   +string("  vec2 i1;\n")
   +string("  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);\n")
   +string("  vec4 x12 = x0.xyxy + C.xxzz;\n")
   +string("  x12.xy -= i1;\n")
   +string("  i = mod289(i); // Avoid truncation effects in permutation\n")
   +string("  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 )) + i.x + vec3(0.0, i1.x, 1.0 ));\n")
   +string("  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);\n")
   +string("  m = m*m ;\n")
   +string("  m = m*m ;\n")
   +string("  vec3 x = 2.0 * fract(p * C.www) - 1.0;\n")
   +string("  vec3 h = abs(x) - 0.5;\n")
   +string("  vec3 ox = floor(x + 0.5);\n")
   +string("  vec3 a0 = x - ox;\n")
   +string("  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );\n")
   +string("  vec3 g;\n")
   +string("  g.x  = a0.x  * x0.x  + h.x  * x0.y;\n")
   +string("  g.yz = a0.yz * x12.xz + h.yz * x12.yw;\n")
   +string("  return 130.0 * dot(m, g);\n")
   +string("}\n")
   +string("float staticV(vec2 uv) {\n")
   +string("    float staticHeight = snoise(vec2(9.0,time*1.2+3.0))*0.3+5.0;\n")
   +string("    float staticAmount = snoise(vec2(1.0,time*1.2-6.0))*0.1+0.3;\n")
   +string("    float staticStrength = snoise(vec2(-9.75,time*0.6-3.0))*2.0+2.0;\n")
   +string("	return (1.0-step(snoise(vec2(5.0*pow(time,2.0)+pow(uv.x*7.0,1.2),pow((mod(time,100.0)+100.0)*uv.y*0.3+3.0,staticHeight))),staticAmount))*staticStrength;\n")
   +string("}\n")
   +string("void main() {\n")
   +string("	vec2 uv =  Texcoord.xy;\n")
   +string("	float jerkOffset = (1.0-step(snoise(vec2(time*1.3,5.0)),0.8))*0.05;\n")
   +string("	float fuzzOffset = snoise(vec2(time*15.0,uv.y*80.0))*0.003;\n")
   +string("	float largeFuzzOffset = snoise(vec2(time*1.0,uv.y*25.0))*0.004;\n")
   +string(" float vertMovementOn = (1.0-step(snoise(vec2(time*0.2,8.0)),0.4))*vertMovementOpt;\n")
   +string(" float vertJerk = (1.0-step(snoise(vec2(time*1.5,5.0)),0.6))*vertJerkOpt;\n")
   +string(" float vertJerk2 = (1.0-step(snoise(vec2(time*5.5,5.0)),0.2))*vertJerkOpt;\n")
   +string(" float staticVal = 0.0;\n")
   +string("	float xOffset = (fuzzOffset + largeFuzzOffset) * horzFuzzOpt * 0.1;\n")
   +string(" float yOffset = abs(sin(time)*4.0)*vertMovementOn+vertJerk*vertJerk2*0.3;\n")
   +string(" float y = mod(uv.y+yOffset,1.0);\n")
   +string("    for (float y = -1.0; y <= 1.0; y += 1.0) {\n")
   +string("        float maxDist = 5.0/200.0;\n")
   +string("        float dist = y/200.0;\n")
   +string("    	staticVal += staticV(vec2(uv.x,uv.y+dist))*(maxDist-abs(dist))*1.5;\n")
   +string("    }\n")
   +string("    staticVal *= bottomStaticOpt * 0.1;\n")
   +string("	float red 	 = texture2D(	tex, 	vec2(uv.x + xOffset -0.01*rgbOffsetOpt,y)).r+staticVal;\n")
   +string("	float green =	texture2D(	tex, 	vec2(uv.x + xOffset,	                  y)).g+staticVal;\n")
   +string("	float blue 	=	texture2D(	tex, 	vec2(uv.x + xOffset +0.01*rgbOffsetOpt,y)).b+staticVal;\n")
   +string("	vec3 color = vec3(red,green,blue);\n")
   +string("	float scanline = sin(uv.y*800.0)*0.03*scalinesOpt;\n")
   +string("	color -= scanline;\n")
   +string("	outColor = vec4(color,1.0);\n")
   +string("}")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }