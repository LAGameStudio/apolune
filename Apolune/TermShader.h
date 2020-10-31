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

class TermShader : public GLSLShader {
public:
 void Init() {
  string frag=
   string("#define DETECT_BLINK 1.5\n")+
   string("#define DETECT_BORDER 4.0\n")+
   string("#define DETECT_CROSS 8.0\n")+
   string("#define DETECT_SEQUENCE 10.0\n")+
   string("float hash(int n) { return fract(sin(float(n))*43758.5453); }\n")+
   string("float roundBox(vec2 p, vec2 b, float r) {	return length(max(abs(p)-b,0.0))-r; }\n")+
   string("float signal(float value) {	value -= floor(value);	if (value < 0.5) return 0.0;	return 1.0; }\n")+
   string("vec3 getPixel(vec2 p) {	p.y *= iResolution.y / iResolution.x;	\n")+
   string("	float border = roundBox(p, vec2(0.92, 0.42), 0.0);\n")+
   string("	if (border > 0.02) return vec3(0.0);\n")+
   string("	if (border > 0.01) return vec3(1.0);\n")+
   string("	if (border > 0.00) return vec3(0.0);\n")+
   string("	p *= 0.425;\n")+
   string("	vec2 trans = vec2(iGlobalTime)*vec2(0.0331,0.0051);\n")+
   string("	int detectID = int(iGlobalTime / DETECT_SEQUENCE);\n")+
   string("	float detectTime = mod(iGlobalTime, DETECT_SEQUENCE);\n")+
   string("	vec2 center = vec2(hash(detectID), hash(detectID*1500))*2.0-1.0;\n")+
   string("	center.x *= .3; center.y *= .1;\n")+
   string("	center -= vec2(mod(trans.x, 0.8)-.4, mod(trans.y,.4)-.2);\n")+
   string("	if (detectTime < DETECT_BLINK) {\n")+
   string("		if (signal(detectTime*5.0) < 0.5) {\n")+
   string("			float detectBorder = roundBox(p-center, vec2(0.001, 0.001), 0.0);\n")+
   string("			if (detectBorder < 0.005) {\n")+
   string("				return vec3(1.0);\n")+
   string("			}\n")+
   string("		}\n")+
   string("	} else if (detectTime < DETECT_BORDER) {\n")+
   string("		detectTime = min(detectTime, DETECT_BORDER);\n")+
   string("		float size = (detectTime-DETECT_BLINK)/(DETECT_BORDER-DETECT_BLINK)*0.02;\n")+
   string("		float detectBorder = abs(roundBox(p-center, vec2(size, size), 0.01));\n")+
   string("		if (detectBorder < 0.002) {\n")+
   string("			return vec3(1.0);\n")+
   string("		}\n")+
   string("	} else if (detectTime < DETECT_CROSS) {\n")+
   string("		float detectBorder = roundBox(p-center, vec2(0.02, 0.02), 0.01);\n")+
   string("		if (detectBorder > 0.002) {\n")+
   string("			detectBorder = roundBox(p-center, vec2(100.0, 0.002), 0.0);\n")+
   string("			if (detectBorder < 0.0001) return vec3(1.0);\n")+
   string("			detectBorder = roundBox(p-center, vec2(0.002, 100.0), 0.0);\n")+
   string("			if (detectBorder < 0.0001) return vec3(1.0);\n")+
   string("		}\n")+
   string("		detectBorder = abs(roundBox(p-center, vec2(0.02, 0.02), 0.01));\n")+
   string("		if (detectBorder < 0.002) {\n")+
   string("			return vec3(1.0);\n")+
   string("		}\n")+
   string("	}	\n")+
   string("	p += trans;\n")+
   string(" vec3 uv = vec3(texture2D(iChannel1, p));\n")+
   string("	uv.x *= cos(uv.y+iGlobalTime)*.35124;\n")+
   string("	uv.y /= cos(uv.x*sqrt(uv.y))*.23312/abs(uv.y);\n")+
   string(" vec3 co = vec3(texture2D(iChannel1, uv.xy));\n")+
   string(" return co*.8;\n")+
   string("}\n")+
   string("// pixel coordinate to screen value, with scanline and glitches etc\n")+
   string("vec3 getScreen(vec2 p) {	\n")+
   string("	float boundValue = roundBox(p, vec2(0.72, 0.6), 0.1)*5.0;\n")+
   string("	boundValue = 1.0 - clamp(boundValue, 0.0, 1.0);\n")+
   string("	float scanline = sin(p.y*3.1415*110.0)*.5+.5;\n")+
   string("	scanline = sqrt(scanline);\n")+
   string("	float glitch = sin(18.245*iGlobalTime)*cos(11.323*iGlobalTime)*sin(4.313*iGlobalTime);\n")+
   string("	glitch *= glitch;\n")+
   string("	p.x += sin(p.y*19.1)*glitch*.01;\n")+
   string("	p.x += sin(p.y*459.1)*glitch*glitch*.02;\n")+
   string("	vec3 color = getPixel(p);\n")+
   string("	float factor = scanline*boundValue;\n")+
   string("	return color*factor*.6+vec3(0, factor*color.g*.8, 0);\n")+
   string("}\n")+
   string("vec3 getScreenBorder(vec2 p) {\n")+
   string("	float boundOut = roundBox(p, vec2(1.15, 1.15), 0.0)*8.0;\n")+
   string("	boundOut = clamp(boundOut, 0.5, 1.0);\n")+
   string("	vec3 cin = vec3(texture2D(iChannel0, p*.88));\n")+
   string("	cin = vec3(cin.r*.02, cin.g*.18, cin.b*.03);\n")+
   string("	float light = sin((atan(p.x, p.y)+0.2)*2.0);\n")+
   string("	cin += light*.10+0.13;\n")+
   string("	float boundIn = roundBox(p, vec2(0.99, 0.9), 0.05)*150.0;\n")+
   string("	boundIn = clamp(boundIn, 0.0, 1.0);\n")+
   string("	float hazeV = roundBox(p, vec2(0.83, 0.73), 0.03)*3.5;\n")+
   string("	hazeV = 1.0 - clamp(hazeV, 0.8, 1.0);\n")+
   string("	hazeV += (sin(iGlobalTime*2.3) + sin(iGlobalTime*20.912) * .4)*.06;\n")+
   string("	vec3 haze = vec3(0.0, hazeV*.14, 0.0);\n")+
   string("	return mix(cin*boundIn, vec3(0), boundOut) + haze;	\n")+
   string("}\n")+
   string("void mainImage( out vec4 fragColor, in vec2 fragCoord ) {\n")+
   string("	vec2 p = (fragCoord.xy / iResolution.xy) * 2.0 - 1.0;\n")+
   string("	p += p*dot(p, p)*0.22;\n")+
   string("	vec2 delta = vec2(0.97, 0.04) - (fragCoord.xy / iResolution.xy);\n")+
   string("	delta.y *= iResolution.y / iResolution.x;\n")+
   string("	float ledv = clamp(1.5 - length(delta)*150.0, 0.0, 1.0);\n")+
   string("	ledv *= sin(2.0*iGlobalTime)*.5+.5;\n")+
   string("	vec3 led = vec3(ledv*.4, ledv, ledv*.4);\n")+
   string("	fragColor = vec4(getScreenBorder(p)+getScreen(p)+led, 1);\n")+
   string("}\n")
  ;
 }
 void InitUniforms() {
 }
 void UpdateUniforms() {
 }
};

