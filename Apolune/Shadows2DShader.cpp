#include "Shadows2DShader.h"

Light2DShader light2dShader;
Lights2DShader lights2dShader;
LightsNoRadius2DShader lightsnoradius2dShader;
LightsNoColor2DShader lightsnocolor2dShader;
LightsPulsing2DShader lightspulsing2dShader;
Shadows2DShader shadows2dShader;
Shadow2DShader shadow2dShader;
MultiShadow2DShader multishadow2dShader;

void Shadow2DShader::Render( FBOColor *out, Shadow2DProfile *in ) {
 out->Bind();
 shadow2dShader.Set(in);
 out->QuadYInverted(&shadow2dShader);
 out->Unbind();
}

void ShadedShadow2D( FBOColor *out, Shadow2DProfile *in ) {
 out->Bind();
 shadow2dShader.Set(in);
 out->QuadYInverted(&shadow2dShader);
 out->Unbind();
}

void ShadedShadow2D( FBOColor *out, Shadow2DProfile *in, GLuint *color, GLuint *addlight, GLuint *light, GLuint *emap, GLuint *back, GLuint *height ) {
 out->Bind();
 shadow2dShader.pos.Set(&in->pos);
 shadow2dShader.ambience.fromCrayon(in->ambience);
 shadow2dShader.colormap.pointer=color;
 shadow2dShader.heightmap.pointer=height;
 shadow2dShader.lightmap.pointer=light;
 shadow2dShader.emap.pointer=emap;
 shadow2dShader.addlightmap.pointer=addlight;
 shadow2dShader.backlight.pointer=back;
 out->QuadYInverted(&shadow2dShader);
 out->Unbind();
}

//----

void MultiShadow2DShader::Init() {
 OUTPUT("MultiShadow2DShader::Loading\n");
 string vert=
   string("#version 150\n")
  +string("uniform mat4 trans;\n")
  +string("in vec2 position;\n")
  +string("in vec2 texcoord;\n")
  +string("out vec2 tc;\n")
  +string("\n")
  +string("void main() {\n")
  +string(" tc = texcoord;\n")
  +string(" gl_Position = trans * vec4(position, 0.0, 1.0);\n")
  +string("}")
 ;
 string frag=
   string("#version 150 core\n")
  +string("out vec4 outColor;\n")
  +string("in vec2 tc;\n")
  +string("const int num = 256;\n")
  +string("const float recip = 1.0/256.0;\n")
  +string("\n")
  +string("uniform vec3 pos;\n")
  +string("uniform vec3 pos1;\n")
  +string("uniform vec3 pos2;\n")
  +string("uniform vec3 pos3;\n")
  +string("uniform vec3 pos4;\n")
  +string("uniform vec3 pos5;\n")
  +string("uniform vec3 pos6;\n")
  +string("uniform vec3 pos7;\n")
  +string("uniform vec3 ambience;\n")
  +string("uniform int count;\n")
  +string("uniform sampler2D colormap;\n")
  +string("uniform sampler2D lightmap;\n")
  +string("uniform sampler2D emap;\n")
  +string("uniform sampler2D addlightmap;\n")
  +string("uniform sampler2D backlight;\n")
  +string("uniform sampler2D heightmap;\n")
  +string("float shade( vec3 p, float height )\n")
  +string("{\n")
  +string("    float shadow=0.0;\n")
  +string("    for(int Z=0;Z<num;Z+=2)\n")
  +string("        {\n")
  +string("            float Zr=float(Z)*recip;\n")
  +string("            shadow += clamp(texture(heightmap,tc * (1.0-Zr)\n")
  +string("            + (Zr*p.xy*0.5)).r - Zr - height,0.0,1.0);\n")
  +string("        }\n")
  +string("    return shadow;\n") // clamp(shadow,0.0,1.0);\n")
  +string("}\n")
  +string("vec4 image() {\n")
  +string("    float height = texture(heightmap,tc).r-(1.0-pos.z);\n")
  +string("    vec4 BS=texture(backlight,tc);")
  +string("    float BL=(BS.r+BS.g+BS.b)/3.0;\n")
  +string("    vec4 CM=texture(colormap,tc);\n")
  +string("    vec4 LM=texture(lightmap,tc);\n")
  +string("    float LI=(LM.r+LM.g+LM.b)/3.0;\n")
  +string("    vec4 ALM=texture(addlightmap,tc);\n")
  +string("    float ALI=1.0-(ALM.r+ALM.g+ALM.b)/3.0;\n")
  +string("    vec4 EM=texture(emap,tc);\n")
  +string("    vec4 AM=vec4((ambience)*texture(colormap,tc).rgb,1.0);\n")
  +string(" float shadow=0.0;\n")
  +string(" if ( count > 0 ) shadow+=shade(pos, height); \n")
  +string(" if ( count > 1 ) shadow+=shade(pos1,height);\n")
  +string(" if ( count > 2 ) shadow+=shade(pos2,height);\n")
  +string(" if ( count > 3 ) shadow+=shade(pos3,height);\n")
  +string(" if ( count > 4 ) shadow+=shade(pos4,height);\n")
  +string(" if ( count > 5 ) shadow+=shade(pos5,height);\n")
  +string(" if ( count > 6 ) shadow+=shade(pos6,height);\n")
  +string(" if ( count > 7 ) shadow+=shade(pos7,height);\n")
  +string(" return clamp(EM + CM * LM * vec4((1.0-ambience),1.0) + AM - clamp(vec4(vec3(shadow/(float(count)*4.0)),1.0),0.0,1.0)*.2,0.0,1.0);\n")
  +string("}\n")
  +string("void main() {\n")
  +string(" if ( tc.x < 0.5 && tc.y > 0.5 ) outColor=image(); else outColor=vec4(0);\n")
  +string("}\n")
 ;
 Strings binds; binds.Add("outColor");
 loadShaders(&binds,vert.c_str(),frag.c_str());
 InitUniforms();
}

#include "Art2d.h"

void MultiShadow2DShader::Render( FBOColor *out, MultiShadow2DProfile *in ) {
 out->Bind();
 multishadow2dShader.Set(in);
 out->QuadYInverted(&multishadow2dShader);
 out->Unbind();
}

void MultiShadedShadow2D( FBOColor *out, MultiShadow2DProfile *in ) {
 out->Bind();
 multishadow2dShader.Set(in);
 out->QuadYInverted(&multishadow2dShader);
 out->Unbind();
}

void MultiShadedShadow2D( FBOColor *out, MultiShadow2DProfile *in, GLuint *color, GLuint *addlight, GLuint *light, GLuint *emap, GLuint *back, GLuint *height ) {
 multishadow2dShader.pos.Set(&in->pos);
 multishadow2dShader.pos1.Set(&in->pos1);
 multishadow2dShader.pos2.Set(&in->pos2);
 multishadow2dShader.pos3.Set(&in->pos3);
 multishadow2dShader.pos4.Set(&in->pos4);
 multishadow2dShader.pos5.Set(&in->pos5);
 multishadow2dShader.pos6.Set(&in->pos6);
 multishadow2dShader.pos7.Set(&in->pos7);
 multishadow2dShader.ambience.fromCrayon(in->ambience);
 multishadow2dShader.activeLights=in->activeLights;
 multishadow2dShader.colormap.pointer=color;
 multishadow2dShader.heightmap.pointer=height;
 multishadow2dShader.lightmap.pointer=light;
 multishadow2dShader.emap.pointer=emap;
 multishadow2dShader.addlightmap.pointer=addlight;
 multishadow2dShader.backlight.pointer=back;
 out->background.Pick(black);
 out->Bind();
 out->QuadYInverted(&multishadow2dShader);
 out->Unbind();
}
