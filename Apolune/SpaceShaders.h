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
#include "Vertex.h"
#include "GLSetup.h"
#include "GLSL2Shader.h"
#include "FBO.h"
#include "TextureLibrary.h"

class WaterAndGroundProfile;    void ShadedWaterAndGround( FBOColor *out, WaterAndGroundProfile *in );
class EmissionCloudsProfile;    void ShadedEmissionClouds( FBOColor *out, EmissionCloudsProfile *in );
class ElectroCloudsProfile;     void ShadedElectroClouds( FBOColor *out, ElectroCloudsProfile *in );
class NebulazProfile;           void ShadedNebulaz( FBOColor *out, NebulazProfile *in );
class ToonTroidProfile;         void ShadedToonTroid( FBOColor *out, ToonTroidProfile *in );
class TexturedPlanetProfile;    void ShadedTexturedPlanet( FBOColor *out, TexturedPlanetProfile *in );
class EarthlikeProfile;         void ShadedEarthlike( FBOColor *out, EarthlikeProfile *in );
class AccretionProfile;         void ShadedAccretion( FBOColor *out, AccretionProfile *in );
class BlackHoleProfile;         void ShadedBlackHole( FBOColor *out, BlackHoleProfile *in );
class RingedJovianProfile;      void ShadedRingedJovian( FBOColor *out, RingedJovianProfile *in );
class ElectroPlanetProfile;     void ShadedElectroPlanet( FBOColor *out, ElectroPlanetProfile *in );
class AtmosphericPlanetProfile; void ShadedAtmosphericPlanet( FBOColor *out, AtmosphericPlanetProfile *in );
class Earthlike2Profile;        void ShadedEarthlike2( FBOColor *out, Earthlike2Profile *in );
class RingedJovian2Profile;     void ShadedRingedJovian2( FBOColor *out, RingedJovian2Profile *in );
class LunarPlanetProfile;       void ShadedLunarPlanet( FBOColor *out, LunarPlanetProfile *in );
class FlaringStarProfile;       void ShadedFlaringStar( FBOColor *out, FlaringStarProfile *in );
class ElectronCloudsProfile;    void ShadedElectronClouds( FBOColor *out, ElectronCloudsProfile *in );
class ClassicPlanetProfile;     void ShadedClassicPlanet( FBOColor *out, ClassicPlanetProfile *in );


// ----------------------------------------------------
ONE(WaterAndGroundProfile,{
 waveParams.Set(0.5,4.0,6.0); waveParams.w=5.0;
 waterLight.Set(15.0,120.0);
 particleAmount=70.0;
 waterColor1.Double(0.43,0.6,0.66,1.0);
 waterColor2.Double(0.06,0.07,0.11,1.0);
 waterSpec.Set(1.3,1.3,0.9);
 light.Set(0.0,20.0,10.0);
 LEVEL.Set(0.525,0.54,0.5); LEVEL.w=0.1;
 noiseTex=library.Load("data/images/textures/noise4.png",tllto_Tiling);
 groundTex=library.Load("data/images/textures/stone.png",tllto_Tiling);
 landColor.Double(0.7,0.55,0.4,1.0);
})
 Cartesiand waveParams,waterLight,waterSpec,light,pos2d,LEVEL;
 Zdouble particleAmount,time;
 Crayon waterColor1,waterColor2,landColor;
  Zp<GLImage> noiseTex,groundTex;
 void Set( WaterAndGroundProfile *in ) {
  waveParams.Set(&in->waveParams);
  waterLight.Set(&in->waterLight);
  particleAmount=in->particleAmount;
  waterColor1.fromCrayon(in->waterColor1);
  waterColor2.fromCrayon(in->waterColor2);
  landColor.fromCrayon(in->landColor);
  waterSpec.Set(&in->waterSpec);
  light.Set(&in->light);
  LEVEL.Set(&in->LEVEL);
  noiseTex=in->noiseTex;
  groundTex=in->groundTex;
 }
 BINARY_IN({
  B_IN_SSUB(waveParams)  B_IN_SSUB(waterLight)    B_IN_SSUB(waterSpec)
  B_IN_SSUB(light)       B_IN_SSUB(pos2d)         B_IN_SSUB(LEVEL)
  B_IN_NUM(particleAmount) B_IN_NUM(time)
  B_IN_TINT(waterColor1) B_IN_TINT(waterColor2)   B_IN_TINT(landColor)
  B_IN_TILE(noiseTex)    B_IN_TILE(groundTex)
 })
 BINARY_OUT({
  B_OUT_SSUB(waveParams)  B_OUT_SSUB(waterLight)    B_OUT_SSUB(waterSpec)
  B_OUT_SSUB(light)       B_OUT_SSUB(pos2d)         B_OUT_SSUB(LEVEL)
  B_OUT_NUM(particleAmount) B_OUT_NUM(time)
  B_OUT_TINT(waterColor1) B_OUT_TINT(waterColor2)   B_OUT_TINT(landColor)
  B_OUT_TILE(noiseTex)    B_OUT_TILE(groundTex)
 })
 KEYWORDS({
  SUBWORD("waveParams",waveParams)
  else SUBWORD("waterLight",waterLight)
  else SUBWORD("waterSpec",waterSpec)
  else SUBWORD("light",light)
  else SUBWORD("pos2d",pos2d)
  else SUBWORD("LEVEL",LEVEL)
  else NUMWORD("particleAmount",particleAmount)
  else NUMWORD("time",time)
  else TINTWORD("waterColor1",waterColor1)
  else TINTWORD("waterColor2",waterColor2)
  else TINTWORD("landColor",landColor)
  else TILEWORD("noiseTex",noiseTex)
  else TILEWORD("groundTex",groundTex)
 })
 WORDKEYS({
  SSUBKEY("waveParams",waveParams)
  SSUBKEY("waterLight",waterLight)
  SSUBKEY("waterSpec",waterSpec)
  SSUBKEY("light",light)
  SSUBKEY("pos2d",pos2d)
  SSUBKEY("LEVEL",LEVEL)
  NUMBKEY("particleAmount",particleAmount)
  NUMBKEY("time",time)
  TINTKEY("waterColor1",waterColor1)
  TINTKEY("waterColor2",waterColor2)
  TINTKEY("landColor",landColor)
  IMGKEY("noiseTex",noiseTex)
  IMGKEY("groundTex",groundTex)
 })
MANY(WaterAndGroundProfile,WaterAndGroundProfileHandle,WaterAndGroundProfileHandles,"WaterAndGround",WaterAndGroundProfiles,{})
 KEYWORDSGroup("WaterAndGround",WaterAndGroundProfile);
 WORDKEYSGroup("WaterAndGround",WaterAndGroundProfile);
 BINARY_INOUT(WaterAndGroundProfile);
DONE(WaterAndGroundProfile);

class WaterAndGroundShader : public GLSLShader {
public:
  Cartesiand waveParams,waterLight,waterSpec,light,pos2d,LEVEL;
  Zdouble particleAmount,time;
  Crayon waterColor1,waterColor2,landColor;
  Zp<GLuint> noiseTex,groundTex;
 GLuint UwaveParams,UwaterLight,UwaterSpec,Ulight,Upos2d,ULEVEL,UparticleAmount,Utime,UwaterColor1,UwaterColor2,UnoiseTex,UgroundTex,UlandColor;
GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  UwaveParams=glGetUniformLocation(program, "waveParams");
  UwaterLight=glGetUniformLocation(program, "waterLight");
  UwaterSpec=glGetUniformLocation(program, "waterSpec");
  Ulight=glGetUniformLocation(program, "light");
  Upos2d=glGetUniformLocation(program, "pos2d");
  ULEVEL=glGetUniformLocation(program, "LEVEL");
  UparticleAmount=glGetUniformLocation(program, "particleAmount");
  Utime=glGetUniformLocation(program, "time");
  UwaterColor1=glGetUniformLocation(program, "waterColor1");
  UwaterColor2=glGetUniformLocation(program, "waterColor2");
  UlandColor=glGetUniformLocation(program, "landColor");
  UnoiseTex=glGetUniformLocation(program, "noiseTex");
  UgroundTex=glGetUniformLocation(program, "groundTex");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform4f(UwaveParams,(GLfloat)waveParams.x,(GLfloat)waveParams.y,(GLfloat)waveParams.z,(GLfloat)waveParams.w);
  glUniform2f(UwaterLight,(GLfloat)waterLight.x,(GLfloat)waterLight.y);
  glUniform3f(UwaterSpec,(GLfloat)waterSpec.x,(GLfloat)waterSpec.y,(GLfloat)waterSpec.z);
  glUniform3f(Ulight,(GLfloat)light.x,(GLfloat)light.y,(GLfloat)light.z);
  glUniform2f(Upos2d,
   (GLfloat)(pos2d.x),
   (GLfloat)(pos2d.y)
  );
  glUniform4f(ULEVEL,(GLfloat)LEVEL.x,(GLfloat)LEVEL.y,(GLfloat)LEVEL.z,(GLfloat)LEVEL.w);
  glUniform3f(UwaterColor1,(GLfloat)waterColor1.rf,(GLfloat)waterColor1.gf,(GLfloat)waterColor1.bf);
  glUniform3f(UwaterColor2,(GLfloat)waterColor2.rf,(GLfloat)waterColor2.gf,(GLfloat)waterColor2.bf);
  glUniform3f(UlandColor,(GLfloat)landColor.rf,(GLfloat)landColor.gf,(GLfloat)landColor.bf);
  glUniform1f(UparticleAmount,(GLfloat)particleAmount);
  glUniform1f(Utime,(GLfloat)time);
  glEnable(GL_TEXTURE_2D);
  glUniform1i(UnoiseTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *noiseTex);
  glUniform1i(UgroundTex, 1);  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *groundTex);

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
  glDisable(GL_TEXTURE_2D);
  glUseProgram(0);
 }
 void Set( WaterAndGroundProfile *in ) {
  waveParams.Set(&in->waveParams);
  waterLight.Set(&in->waterLight);
  waterSpec.Set(&in->waterSpec);
  light.Set(&in->light);
  pos2d.Set(&in->pos2d);
  LEVEL.Set(&in->LEVEL);
  particleAmount=in->particleAmount;
  time=in->time;
  waterColor1.fromCrayon(in->waterColor1);
  waterColor2.fromCrayon(in->waterColor2);
  landColor.fromCrayon(in->landColor);
  noiseTex=&in->noiseTex->texture;
  groundTex=&in->groundTex->texture;
 }
};




// ----------------------------------------------------

ONE(EmissionCloudsProfile,{
 noiseTex=library.Load("data/images/noise3.png",tllto_Tiling);
})
  Zp<GLImage> noiseTex;
  Cartesiand pos2d;
  Zdouble time;
  void Set( EmissionCloudsProfile *in ) {
   noiseTex=in->noiseTex;
   pos2d.Set(&in->pos2d);
   time=in->time;
  }
 BINARY_IN({
  B_IN_TILE(noiseTex)
  B_IN_SSUB(pos2d)
  B_IN_NUM(time)
 })
 BINARY_OUT({
  B_OUT_TILE(noiseTex)
  B_OUT_SSUB(pos2d)
  B_OUT_NUM(time)
 })
 KEYWORDS({
  SUBWORD("pos2d",pos2d)
  else TILEWORD("noise",noiseTex)
  else KEYWORD("time",{time=w;})
 })
 WORDKEYS({
  SSUBKEY("pos2d",pos2d)
  NUMBKEY("time",time)
  IMGKEY("noise",noiseTex)
 })
MANY(EmissionCloudsProfile,EmissionCloudsProfileHandle,EmissionCloudsProfileHandles,"EmissionClouds",EmissionCloudsProfiles,{})
 KEYWORDSGroup("EmissionClouds",EmissionCloudsProfile);
 WORDKEYSGroup("EmissionClouds",EmissionCloudsProfile);
 BINARY_INOUT(EmissionCloudsProfile)
DONE(EmissionCloudsProfile);

class EmissionCloudsShader : public GLSLShader {
public:
 Cartesiand pos2d;
 Zdouble time;
 Zp<GLuint> noiseTex;
 GLuint Upos2d,Utime,UnoiseTex;
GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Upos2d=glGetUniformLocation(program, "pos2d");
  Utime=glGetUniformLocation(program, "time");
  UnoiseTex=glGetUniformLocation(program, "noiseTex");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Upos2d,(GLfloat)pos2d.x,(GLfloat)pos2d.y);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1i(UnoiseTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *noiseTex);

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
 void Set( EmissionCloudsProfile *in ) {
  pos2d.Set(&in->pos2d);
  time=in->time;
  noiseTex=&in->noiseTex->texture;
 }
};




// ----------------------------------------------------

ONE(ElectroCloudsProfile,{
 toffs = -154.0;
 complexity = 10;
 noiseTex=library.Load("data/images/textures/noise2.jpg",tllto_Tiling);
 tint.Pick(alabaster);
})
 Zp<GLImage> noiseTex;
 Zdouble time,toffs,complexity;
 Cartesiand position;
 Crayon tint;
 void Set( ElectroCloudsProfile *in ) {
  noiseTex=in->noiseTex;
  time=in->time;
  toffs=in->toffs;
  complexity=in->complexity;
  position.Set(&in->position);
  tint.fromCrayon(in->tint);
 }
 void Randomize(int seed) {
  toffs = URANGE(-288.0, sin(rad2deg(deg2rad((double)seed + 1234.56789 + (double)seed / 100000.0)))*288.0, 288.0);
  complexity = 1+upto(9, seed);
  tint.random(seed);
 }
 BINARY_IN({
  B_IN_TILE(noiseTex)
  B_IN_NUM(time)
  B_IN_NUM(toffs)
  B_IN_NUM(complexity)
  B_IN_SSUB(position)
  B_IN_TINT(tint)
 })
 BINARY_OUT({
  B_OUT_TILE(noiseTex)
  B_OUT_NUM(time)
  B_OUT_NUM(toffs)
  B_OUT_NUM(complexity)
  B_OUT_SSUB(position)
  B_OUT_TINT(tint)
 })
 KEYWORDS({
  KEYWORD("time",{time=w;})
  else KEYWORD("toffs",{toffs=w;})
  else KEYWORD("complexity",{complexity=w;})
  else SUBWORD("position",position)
  else TINTWORD("tint",tint)
 })
 WORDKEYS({
  NUMBKEY("time",time)
  NUMBKEY("toffs",toffs)
  NUMBKEY("complexity",complexity)
  SSUBKEY("position",position)
  TINTKEY("tint",tint)
 })
MANY(ElectroCloudsProfile,ElectroCloudsProfileHandle,ElectroCloudsProfileHandles,"ElectroClouds",ElectroCloudsProfiles,{})
 KEYWORDSGroup("ElectroClouds",ElectroCloudsProfile);
 WORDKEYSGroup("ElectroClouds",ElectroCloudsProfile);
 BINARY_INOUT(ElectroCloudsProfile)
DONE(ElectroCloudsProfile);

class ElectroCloudsShader : public GLSLShader {
public:
 Zp<GLuint> noiseTex;
 Zdouble time,toffs,complexity;
 Cartesiand pos;
 Crayon tint;
 GLuint Utime,Utoffs,Ucomplexity,Upos,Utint,UnoiseTex;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Utime=glGetUniformLocation(program, "time");
  Utoffs=glGetUniformLocation(program, "toffs");
  Ucomplexity=glGetUniformLocation(program, "complexity");
  Upos=glGetUniformLocation(program, "pos");
  Utint=glGetUniformLocation(program, "tint");
  UnoiseTex=glGetUniformLocation(program, "noiseTex");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Upos,(GLfloat)pos.x,(GLfloat)pos.y);
  glUniform3f(Utint,(GLfloat)tint.r,(GLfloat)tint.g,(GLfloat)tint.b);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(Utoffs,(GLfloat)toffs);
  glUniform1f(Ucomplexity,(GLfloat)complexity);
  glUniform1i(UnoiseTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *noiseTex);

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
 void Set( ElectroCloudsProfile *in ) {
  time=in->time;
  toffs=in->toffs;
  complexity=in->complexity;
  pos.Set(&in->position);
  tint.fromCrayon(in->tint);
  noiseTex=&in->noiseTex->texture;
 }
};


// ----------------------------------------------------

ONE(NebulazProfile,{
 structure_size=1.9;
 background_radiation1=0.2;
 noiseTex=library.Load("data/images/noise4.png",tllto_Tiling);
})
 Zdouble time;
 Zdouble structure_size,background_radiation1;
 Cartesiand position;
 Zp<GLImage> noiseTex;
 Crayon blueColor,yellowColor,redColor,orangeColor,brownColor,blackColor,coolestColor,middleColor,hottestColor;
 void Set( NebulazProfile *in ) {
  time=in->time;
  structure_size=in->structure_size;
  background_radiation1=in->background_radiation1;
  position.Set(&in->position);
  noiseTex=in->noiseTex;
 }
 BINARY_IN({
  B_IN_NUM(time)
  B_IN_NUM(structure_size)
  B_IN_NUM(background_radiation1)
  B_IN_SSUB(position)
  B_IN_TILE(noiseTex)
  B_IN_TINT(blueColor)
  B_IN_TINT(yellowColor)
  B_IN_TINT(redColor)
  B_IN_TINT(orangeColor)
  B_IN_TINT(brownColor)
  B_IN_TINT(blackColor)
  B_IN_TINT(coolestColor)
  B_IN_TINT(middleColor)
  B_IN_TINT(hottestColor)
 })
 BINARY_OUT({
  B_OUT_NUM(time)
  B_OUT_NUM(structure_size)
  B_OUT_NUM(background_radiation1)
  B_OUT_SSUB(position)
  B_OUT_TILE(noiseTex)
  B_OUT_TINT(blueColor)
  B_OUT_TINT(yellowColor)
  B_OUT_TINT(redColor)
  B_OUT_TINT(orangeColor)
  B_OUT_TINT(brownColor)
  B_OUT_TINT(blackColor)
  B_OUT_TINT(coolestColor)
  B_OUT_TINT(middleColor)
  B_OUT_TINT(hottestColor)
 })
 KEYWORDS({
  SUBWORD("position",position)
  else TILEWORD("noiseTex",noiseTex)
  else TINTWORD("blueColor",blueColor)
  else TINTWORD("yellowColor",yellowColor)
  else TINTWORD("redColor",redColor)
  else TINTWORD("orangeColor",orangeColor)
  else TINTWORD("brownColor",brownColor)
  else TINTWORD("blackColor",blackColor)
  else TINTWORD("coolestColor",coolestColor)
  else TINTWORD("middleColor",middleColor)
  else TINTWORD("hottestColor",hottestColor)
 })
 WORDKEYS({
  SSUBKEY("position",position)
  IMGKEY("noiseTex",noiseTex)
  TINTKEY("blueColor",blueColor)
  TINTKEY("yellowColor",yellowColor)
  TINTKEY("redColor",redColor)
  TINTKEY("orangeColor",orangeColor)
  TINTKEY("brownColor",brownColor)
  TINTKEY("blackColor",blackColor)
  TINTKEY("coolestColor",coolestColor)
  TINTKEY("middleColor",middleColor)
  TINTKEY("hottestColor",hottestColor)
 })
MANY(NebulazProfile,NebulazProfileHandle,NebulazProfileHandles,"Nebulaz",NebulazProfiles,{})
 KEYWORDSGroup("Nebulaz",NebulazProfile);
 WORDKEYSGroup("Nebulaz",NebulazProfile);
 BINARY_INOUT(NebulazProfile)
DONE(NebulazProfile);

class NebulazShader : public GLSLShader {
public:
 Zdouble time;
 Cartesiand position;
 Zp<GLuint> noiseTex;
 Zdouble structure_size,background_radiation1;
 Crayon blueColor,yellowColor,redColor,orangeColor,brownColor,blackColor,coolestColor,middleColor,hottestColor;
 GLuint Utime,Upos,UnoiseTex,UblueColor,UyellowColor,UredColor,UorangeColor,UbrownColor,UblackColor,UcoolestColor,UmiddleColor,UhottestColor,Ustructure_size,Ubackground_radiation1;
GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Upos=glGetUniformLocation(program, "pos");
  UnoiseTex=glGetUniformLocation(program, "noiseTex");
  UblueColor=glGetUniformLocation(program, "blueColor");
  UyellowColor=glGetUniformLocation(program, "yellowColor");
  UredColor=glGetUniformLocation(program, "redColor");
  UorangeColor=glGetUniformLocation(program, "orangeColor");
  UbrownColor=glGetUniformLocation(program, "brownColor");
  UblackColor=glGetUniformLocation(program, "blackColor");
  UcoolestColor=glGetUniformLocation(program, "coolestColor");
  UmiddleColor=glGetUniformLocation(program, "middleColor");
  UhottestColor=glGetUniformLocation(program, "hottestColor");
  Ustructure_size=glGetUniformLocation(program, "structure_size");
  Ubackground_radiation1=glGetUniformLocation(program, "background_radiation1");
  Utime=glGetUniformLocation(program, "time");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Upos,(GLfloat)position.x,(GLfloat)position.y);
  glUniform3f(UblueColor,(GLfloat)blueColor.rf,(GLfloat)blueColor.gf,(GLfloat)blueColor.bf);
  glUniform3f(UyellowColor,(GLfloat)yellowColor.rf,(GLfloat)yellowColor.gf,(GLfloat)yellowColor.bf);
  glUniform3f(UredColor,(GLfloat)redColor.rf,(GLfloat)redColor.gf,(GLfloat)redColor.bf);
  glUniform3f(UorangeColor,(GLfloat)orangeColor.rf,(GLfloat)orangeColor.gf,(GLfloat)orangeColor.bf);
  glUniform3f(UbrownColor,(GLfloat)brownColor.rf,(GLfloat)brownColor.gf,(GLfloat)brownColor.bf);
  glUniform3f(UblackColor,(GLfloat)blackColor.rf,(GLfloat)blackColor.gf,(GLfloat)blackColor.bf);
  glUniform3f(UcoolestColor,(GLfloat)coolestColor.rf,(GLfloat)coolestColor.gf,(GLfloat)coolestColor.bf);
  glUniform3f(UmiddleColor,(GLfloat)middleColor.rf,(GLfloat)middleColor.gf,(GLfloat)middleColor.bf);
  glUniform3f(UhottestColor,(GLfloat)hottestColor.rf,(GLfloat)hottestColor.gf,(GLfloat)hottestColor.bf);
  glUniform1i(UnoiseTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *noiseTex);
  glUniform1f(Ustructure_size,(GLfloat)structure_size);
  glUniform1f(Ubackground_radiation1,(GLfloat)background_radiation1);
  glUniform1f(Utime,(GLfloat)time);

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
 void Set( NebulazProfile *in ) {
  position.Set(&in->position);
  noiseTex=&in->noiseTex->texture;
  blueColor.fromCrayon(in->blueColor);
  yellowColor.fromCrayon(in->yellowColor);
  redColor.fromCrayon(in->redColor);
  orangeColor.fromCrayon(in->orangeColor);
  brownColor.fromCrayon(in->brownColor);
  blackColor.fromCrayon(in->blackColor);
  coolestColor.fromCrayon(in->coolestColor);
  middleColor.fromCrayon(in->middleColor);
  hottestColor.fromCrayon(in->hottestColor);
  structure_size=in->structure_size;
  background_radiation1=in->background_radiation1;
  time=in->time;
 }
};



// ----------------------------------------------------
ONE(ToonTroidProfile,{
 noiseTex=library.Load("data/images/textures/noise4.png",tllto_Tiling);
 sway=0.0;      // = 2.12
 L.Set(10.25,0.33,-0.7);
 backlight=0.15;
 diffuseColor.Double( 1.0, 0.9, 0.7, 1.0 );
 ambientColor.Double(0.1, 0.11, 0.13, 1.0 );
 lightComponents.Set( 0.1, 0.2, 0.5 ); lightComponents.w=0.2;
 emmissive=0.1;
 roughness = 0.001;
 dimensions.Set( 0.54030230586, 0.8414709848, -0.8414709848 ); dimensions.w= 0.54030230586;
 craterDims.Set( 0.25, 0.05, 0.5 ); craterDims.w=0.15;
 craterParams.Set( 0.15, 0.6, 5.0 ); craterParams.w= 0.0;
 surfaceParams.Set( 0.005, 0.1, 2.0 ); surfaceParams.w= 20.0;
 kaleidoscope.Set( 0.9, 0.6, 0.2 ); kaleidoscope.w=0.15; //   ... make z a huge number for odd effects
 scale.Set(1.0,1.0,1.0);
 cam.Set(-2.0,0.0,0.0);
 tintColor.Double(1.0,1.0,1.0,1.0);
})
 Zp<GLImage> noiseTex;
 Cartesiand scale,L,lightComponents,rotation,dimensions,craterDims,craterParams,surfaceParams,kaleidoscope,cam;
 Zdouble time,sway,backlight,emmissive,roughness;
 Crayon diffuseColor,ambientColor,tintColor;
 void Randomize(int seed) {
  backlight = 0.15 + iratiod(seed % 100, 100) * 0.25;
  diffuseColor.random(seed + 1234);
  ambientColor.random(seed);
  ambientColor.MultiplyClampRGB(0.13);
  //lightComponents.Set(iratiod(upto(100, seed), 100) * 0.1 + 0.05, iratiod(upto(100, seed + 1), 100)*0.2 + 0.1, iratiod(upto(100, seed + 3), 100)*0.5 + 0.25);
  //lightComponents.w = iratiod(upto(100, seed + 4), 100) * 0.2 + 0.1;
  roughness = 0.001+ 0.01 * iratiod(upto(200, seed + 6), 100);
  craterDims.Set(iratiod(upto(100, seed), 100) * 0.25 + 0.0125, iratiod(upto(100, seed + 1), 100)*0.05 + 0.025, iratiod(upto(100, seed + 3), 100)*0.5 + 0.25);
  craterParams.Set(iratiod(upto(100, seed), 100) * 0.15 + 0.075, iratiod(upto(100, seed + 1), 100)*0.6 + 0.3, iratiod(upto(100, seed + 3), 100)*5.0 + 2.5);
  surfaceParams.Set(iratiod(upto(100, seed), 100) * 0.001 + 0.004, iratiod(upto(100, seed + 1), 100)*0.1 + 0.1, iratiod(upto(100, seed + 3), 100)*2.0 + 1.25);
  //kaleidoscope.Set(iratiod(upto(100, seed), 100) * 0.9 + 0.9, iratiod(upto(100, seed + 1), 100)*0.6 + 0.6, seed % 100 < 30 ? iratiod(upto(100,seed+1337),100)*100.0 : iratiod(upto(100, seed + 3), 100)*0.5 + 0.25);
  //kaleidoscope.w = iratiod(upto(100, seed + 4), 100) * 0.2 + 0.1;
  tintColor.random(seed+666);
  tintColor.MultiplyClampRGB(2.1);
 }
 BINARY_IN({
  B_IN_TILE(noiseTex)
  B_IN_SSUB(scale)
  B_IN_SSUB(L)
  B_IN_SSUB(lightComponents)
  B_IN_SSUB(rotation)
  B_IN_SSUB(dimensions)
  B_IN_SSUB(craterDims)
  B_IN_SSUB(craterParams)
  B_IN_SSUB(surfaceParams)
  B_IN_SSUB(kaleidoscope)
  B_IN_SSUB(cam)
  B_IN_NUM(time)
  B_IN_NUM(sway)
  B_IN_NUM(backlight)
  B_IN_NUM(emmissive)
  B_IN_NUM(roughness)
  B_IN_TINT(diffuseColor)
  B_IN_TINT(ambientColor)
  B_IN_TINT(tintColor)
 })
 BINARY_OUT({
  B_OUT_TILE(noiseTex)
  B_OUT_SSUB(scale)
  B_OUT_SSUB(L)
  B_OUT_SSUB(lightComponents)
  B_OUT_SSUB(rotation)
  B_OUT_SSUB(dimensions)
  B_OUT_SSUB(craterDims)
  B_OUT_SSUB(craterParams)
  B_OUT_SSUB(surfaceParams)
  B_OUT_SSUB(kaleidoscope)
  B_OUT_SSUB(cam)
  B_OUT_NUM(time)
  B_OUT_NUM(sway)
  B_OUT_NUM(backlight)
  B_OUT_NUM(emmissive)
  B_OUT_NUM(roughness)
  B_OUT_TINT(diffuseColor)
  B_OUT_TINT(ambientColor)
  B_OUT_TINT(tintColor)
 })
 KEYWORDS({
  SUBWORD("scale",scale)
  else SUBWORD("L",L)
  else SUBWORD("lightComponents",lightComponents)
  else SUBWORD("rotation",rotation)
  else SUBWORD("dimensions",dimensions)
  else SUBWORD("cam",cam)
  else SUBWORD("craterDims",craterDims)
  else SUBWORD("craterParams",craterParams)
  else SUBWORD("surfaceParams",surfaceParams)
  else SUBWORD("kaleidoscope",kaleidoscope)
  else KEYWORD("time",{time=w;})
  else KEYWORD("sway",{sway=w;})
  else KEYWORD("backlight",{backlight=w;})
  else KEYWORD("emmissive",{emmissive=w;})
  else KEYWORD("roughness",{roughness=w;})
  else TINTWORD("diffuseColor",diffuseColor)
  else TINTWORD("ambientColor",ambientColor)
  else TINTWORD("tintColor",tintColor)
  else TILEWORD("noiseTex",noiseTex)
 })
 WORDKEYS({
  SSUBKEY("scale",scale)
  SSUBKEY("L",L)
  SSUBKEY("lightComponents",lightComponents)
  SSUBKEY("rotation",rotation)
  SSUBKEY("dimensions",dimensions)
  SSUBKEY("cam",cam)
  SSUBKEY("craterDims",craterDims)
  SSUBKEY("craterParams",craterParams)
  SSUBKEY("surfaceParams",surfaceParams)
  SSUBKEY("kaleidoscope",kaleidoscope)
  NUMBKEY("time",time)
  NUMBKEY("sway",sway)
  NUMBKEY("backlight",backlight)
  NUMBKEY("emmissive",emmissive)
  NUMBKEY("roughness",roughness)
  TINTKEY("diffuseColor",diffuseColor)
  TINTKEY("ambientColor",ambientColor)
  TINTKEY("tintColor",tintColor)
  IMGKEY("noisetex",noiseTex)
 })
MANY(ToonTroidProfile,ToonTroidProfileHandle,ToonTroidProfileHandles,"ToonTroid",ToonTroidProfiles,{})
 KEYWORDSGroup("ToonTroid",ToonTroidProfile);
 WORDKEYSGroup("ToonTroid",ToonTroidProfile);
 BINARY_INOUT(ToonTroidProfile);
DONE(ToonTroidProfile);

class ToonTroidShader : public GLSLShader {
public:
 Zp<GLuint> noiseTex;
 Cartesiand scale,L,lightComponents,rotation,dimensions,craterDims,craterParams,surfaceParams,kaleidoscope,cam;
 Zdouble time,sway,backlight,emmissive,roughness;
 Crayon diffuseColor,ambientColor,tintColor;
 GLuint Ucam,Uscale,UL,UlightComponents,Urotation,Udimensions,UcraterDims,UcraterParams,UsurfaceParams,Ukaleidoscope,Utime,Usway,Ubacklight,Uemmissive,Uroughness,UdiffuseColor,UambientColor,UnoiseTex,UtintColor;
GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Uscale=glGetUniformLocation(program, "scale");
  UL=glGetUniformLocation(program, "L");
  UlightComponents=glGetUniformLocation(program, "lightComponents");
  Urotation=glGetUniformLocation(program, "rotation");
  Udimensions=glGetUniformLocation(program, "dimensions");
  UcraterDims=glGetUniformLocation(program, "craterDims");
  UcraterParams=glGetUniformLocation(program, "craterParams");
  UsurfaceParams=glGetUniformLocation(program, "surfaceParams");
  Ukaleidoscope=glGetUniformLocation(program, "kaleidoscope");
  Utime=glGetUniformLocation(program, "time");
  Usway=glGetUniformLocation(program, "sway");
  Ubacklight=glGetUniformLocation(program, "backlight");
  Uemmissive=glGetUniformLocation(program, "emmissive");
  Uroughness=glGetUniformLocation(program, "roughness");
  UdiffuseColor=glGetUniformLocation(program, "diffuseColor");
  UambientColor=glGetUniformLocation(program, "ambientColor");
  Ucam=glGetUniformLocation(program,"cam");
  UtintColor=glGetUniformLocation(program,"tintColor");
  UnoiseTex=glGetUniformLocation(program,"noiseTex");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Uscale,(GLfloat)scale.x,(GLfloat)scale.y);
  glUniform3f(UL,(GLfloat)L.x,(GLfloat)L.y,(GLfloat)L.z);
  glUniform4f(UlightComponents,(GLfloat)lightComponents.x,(GLfloat)lightComponents.y,(GLfloat)lightComponents.z,(GLfloat)lightComponents.w);
  glUniform2f(Urotation,(GLfloat)rotation.x,(GLfloat)rotation.y);
  glUniform2f(Udimensions,(GLfloat)dimensions.x,(GLfloat)dimensions.y);
  glUniform4f(UcraterDims,(GLfloat)craterDims.x,(GLfloat)craterDims.y,(GLfloat)craterDims.z,(GLfloat)craterDims.w);
  glUniform4f(UcraterParams,(GLfloat)craterParams.x,(GLfloat)craterParams.y,(GLfloat)craterParams.z,(GLfloat)craterParams.w);
  glUniform4f(UsurfaceParams,(GLfloat)surfaceParams.x,(GLfloat)surfaceParams.y,(GLfloat)surfaceParams.z,(GLfloat)surfaceParams.w);
  glUniform4f(Ukaleidoscope,(GLfloat)kaleidoscope.x,(GLfloat)kaleidoscope.y,(GLfloat)kaleidoscope.z,(GLfloat)kaleidoscope.w);
  glUniform3f(UdiffuseColor,(GLfloat)diffuseColor.rf,(GLfloat)diffuseColor.gf,(GLfloat)diffuseColor.bf);
  glUniform3f(UambientColor,(GLfloat)ambientColor.rf,(GLfloat)ambientColor.gf,(GLfloat)ambientColor.bf);
  glUniform3f(UtintColor,(GLfloat)tintColor.rf,(GLfloat)tintColor.gf,(GLfloat)tintColor.bf);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(Usway,(GLfloat)sway);
  glUniform1f(Ubacklight,(GLfloat)backlight);
  glUniform1f(Uemmissive,(GLfloat)emmissive);
  glUniform1f(Uroughness,(GLfloat)roughness);
  glUniform3f(Ucam,(GLfloat) cam.x, (GLfloat) cam.y, (GLfloat) cam.z );
  glUniform1i(UnoiseTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *noiseTex);

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
 void Set( ToonTroidProfile *in ) {
  scale.Set(&in->scale);
  L.Set(&in->L);
  lightComponents.Set(&in->lightComponents);
  rotation.Set(&in->rotation);
  dimensions.Set(&in->dimensions);
  craterDims.Set(&in->craterDims);
  craterParams.Set(&in->craterParams);
  surfaceParams.Set(&in->surfaceParams);
  kaleidoscope.Set(&in->kaleidoscope);
  time=in->time;
  sway=in->sway;
  backlight=in->backlight;
  emmissive=in->emmissive;
  roughness=in->roughness;
  diffuseColor.fromCrayon(in->diffuseColor);
  ambientColor.fromCrayon(in->ambientColor);
  tintColor.fromCrayon(in->tintColor);
  noiseTex=&in->noiseTex->texture;
  cam.Set(&cam);
 }
};

// ----------------------------------------------------
// Planetary Shaders

// ----------------------------------------------------

ONE(TexturedPlanetProfile,{
 LIGHT_COLOR.Double(252.0/255.0,212.0/255.0,128.0/255.0,1.0);
 LIGHT_AMB_COLOR.Double(0.3,0.3,0.3,1.0);
 LIGHT_DIFF_COLOR.Double(1.0,1.0,1.0,1.0);
 LIGHT_SPEC_COLOR.Double(0.25,0.25,0.25,1.0);
 LIGHT_DIR.Set(0.45,0.3,0.45);
 tilt=0.75;
 noiseTex=library.Load("data/images/textures/noise9.png",tllto_Tiling);
 surfaceTex=library.Load("data/images/textures/pitted.png",tllto_Tiling);
})
  Cartesiand position,LIGHT_DIR;
  Crayon LIGHT_COLOR,LIGHT_AMB_COLOR,LIGHT_DIFF_COLOR,LIGHT_SPEC_COLOR;
  Zdouble tilt;
  Zp<GLImage> noiseTex,surfaceTex;
  void Randomize(int seed) {
   LIGHT_COLOR.Brights(seed);
   if (seed % 3 == 1) LIGHT_DIFF_COLOR.Double(1.0, 1.0, 1.0, 1.0);
   else LIGHT_DIFF_COLOR.random(seed+1);
   LIGHT_SPEC_COLOR.random(seed+2);
   LIGHT_SPEC_COLOR.MultiplyClampRGB(0.4, 0.4, 0.4);
   LIGHT_AMB_COLOR.random(seed+3);
   LIGHT_AMB_COLOR.MultiplyClampRGB(0.4, 0.4, 0.4);
  }
 BINARY_IN({
  B_IN_SSUB(position)
  B_IN_SSUB(LIGHT_DIR)
  B_IN_TINT(LIGHT_COLOR)
  B_IN_TINT(LIGHT_AMB_COLOR)
  B_IN_TINT(LIGHT_DIFF_COLOR)
  B_IN_TINT(LIGHT_SPEC_COLOR)
  B_IN_NUM(tilt)
  B_IN_TILE(noiseTex)
  B_IN_TILE(surfaceTex)
 })
 BINARY_OUT({
  B_OUT_SSUB(position)
  B_OUT_SSUB(LIGHT_DIR)
  B_OUT_TINT(LIGHT_COLOR)
  B_OUT_TINT(LIGHT_AMB_COLOR)
  B_OUT_TINT(LIGHT_DIFF_COLOR)
  B_OUT_TINT(LIGHT_SPEC_COLOR)
  B_OUT_NUM(tilt)
  B_IN_TILE(noiseTex)
  B_IN_TILE(surfaceTex)
 })
 KEYWORDS({
  SUBWORD("position",position)
  else SUBWORD("LIGHT_DIR",LIGHT_DIR)
  else TINTWORD("LIGHT_COLOR",LIGHT_COLOR)
  else TINTWORD("LIGHT_AMB_COLOR",LIGHT_AMB_COLOR)
  else TINTWORD("LIGHT_DIFF_COLOR",LIGHT_DIFF_COLOR)
  else TINTWORD("LIGHT_SPEC_COLOR",LIGHT_SPEC_COLOR)
  else KEYWORD("tilt",{tilt=w;})
  else TILEWORD("noiseTex",noiseTex)
  else TILEWORD("surfaceTex",surfaceTex)
 })
 WORDKEYS({
  SSUBKEY("position",position)
  SSUBKEY("LIGHT_DIR",LIGHT_DIR)
  TINTKEY("LIGHT_COLOR",LIGHT_COLOR)
  TINTKEY("LIGHT_AMB_COLOR",LIGHT_AMB_COLOR)
  TINTKEY("LIGHT_DIFF_COLOR",LIGHT_DIFF_COLOR)
  TINTKEY("LIGHT_SPEC_COLOR",LIGHT_SPEC_COLOR)
  NUMBKEY("tilt",tilt)
  IMGKEY("noiseTex",noiseTex)
  IMGKEY("surfaceTex",surfaceTex)
 })
MANY(TexturedPlanetProfile,TexturedPlanetProfileHandle,TexturedPlanetProfileHandles,"TexturedPlanet",TexturedPlanetProfiles,{})
 KEYWORDSGroup("TexturedPlanet",TexturedPlanetProfile);
 WORDKEYSGroup("TexturedPlanet",TexturedPlanetProfile);
 BINARY_INOUT(TexturedPlanetProfile)
DONE(TexturedPlanetProfile);

class TexturedPlanetShader : public GLSLShader {
public:
 Cartesiand position,LIGHT_DIR;
 Crayon LIGHT_COLOR,LIGHT_AMB_COLOR,LIGHT_DIFF_COLOR,LIGHT_SPEC_COLOR;
 Zdouble tilt;
 Zp<GLuint> noiseTex,surfaceTex;
 GLuint Upos,ULIGHT_DIR,ULIGHT_COLOR,ULIGHT_AMB_COLOR,ULIGHT_DIFF_COLOR,ULIGHT_SPEC_COLOR,Utilt,UnoiseTex,UsurfaceTex;
GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Upos=glGetUniformLocation(program, "pos2d");
  ULIGHT_DIR=glGetUniformLocation(program, "LIGHT_DIR");
  ULIGHT_COLOR=glGetUniformLocation(program, "LIGHT_COLOR");
  ULIGHT_AMB_COLOR=glGetUniformLocation(program, "LIGHT_AMB_COLOR");
  ULIGHT_DIFF_COLOR=glGetUniformLocation(program, "LIGHT_DIFF_COLOR");
  ULIGHT_SPEC_COLOR=glGetUniformLocation(program, "LIGHT_SPEC_COLOR");
  Utilt=glGetUniformLocation(program, "tilt");
  UnoiseTex=glGetUniformLocation(program, "noiseTex");
  UsurfaceTex=glGetUniformLocation(program, "surfaceTex");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Upos,(GLfloat)position.x,(GLfloat)position.y);
  glUniform3f(ULIGHT_DIR,(GLfloat)LIGHT_DIR.x,(GLfloat)LIGHT_DIR.y,(GLfloat)LIGHT_DIR.z);
  glUniform3f(ULIGHT_COLOR,(GLfloat)LIGHT_COLOR.rf,(GLfloat)LIGHT_COLOR.gf,(GLfloat)LIGHT_COLOR.bf);
  glUniform3f(ULIGHT_AMB_COLOR,(GLfloat)LIGHT_AMB_COLOR.rf,(GLfloat)LIGHT_AMB_COLOR.gf,(GLfloat)LIGHT_AMB_COLOR.bf);
  glUniform3f(ULIGHT_DIFF_COLOR,(GLfloat)LIGHT_DIFF_COLOR.rf,(GLfloat)LIGHT_DIFF_COLOR.gf,(GLfloat)LIGHT_DIFF_COLOR.bf);
  glUniform3f(ULIGHT_SPEC_COLOR,(GLfloat)LIGHT_SPEC_COLOR.rf,(GLfloat)LIGHT_SPEC_COLOR.gf,(GLfloat)LIGHT_SPEC_COLOR.bf);
  glUniform1f(Utilt,(GLfloat)tilt);
  glEnable(GL_TEXTURE_2D);
  glUniform1i(UnoiseTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *noiseTex);
  glUniform1i(UnoiseTex, 1);  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *surfaceTex);


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
  glDisable(GL_TEXTURE_2D);
  glUseProgram(0);
 }
 void Set( TexturedPlanetProfile *in ) {
  position.Set(&in->position);
  LIGHT_DIR.Set(&in->LIGHT_DIR);
  LIGHT_COLOR.fromCrayon(in->LIGHT_COLOR);
  LIGHT_AMB_COLOR.fromCrayon(in->LIGHT_AMB_COLOR);
  LIGHT_DIFF_COLOR.fromCrayon(in->LIGHT_DIFF_COLOR);
  LIGHT_SPEC_COLOR.fromCrayon(in->LIGHT_SPEC_COLOR);
  tilt=in->tilt;
  noiseTex=&in->noiseTex->texture;
  surfaceTex=&in->surfaceTex->texture;
 }
};





//---------------------------

ONE(EarthlikeProfile,{
 noiseOffset.Set(0.0, 0.0);
 noiseScale.Set(16.0, 16.0);
 noiseScale2.Set(200.0, 200.0);
 noiseScale3.Set(50.0, 50.0);
 cloudNoise.Set(10.0, 30.0);
 oceanColor.Double(13.0 / 255.0, 55.0 / 255.0, 79.0 / 255.0, 1.0);
 iceColor.Double(250.0 / 255.0, 250.0 / 255.0, 250.0 / 255.0, 1.0);
 coldColor.Double(53.0 / 255.0, 102.0 / 255.0, 100.0 / 255.0, 1.0);
 temperateColor.Double(79.0 / 255.0, 109.0 / 255.0, 68.0 / 255.0, 1.0);
 warmColor.Double(119.0 / 255.0, 141.0 / 255.0, 82.0 / 255.0, 1.0);
 hotColor.Double(223.0 / 255.0, 193.0 / 255.0, 148.0 / 255.0, 1.0);
 cloudiness=0.5;
 timescale=0.01;
})
 Cartesiand pos2d,noiseOffset,noiseScale,noiseScale2,noiseScale3,cloudNoise;
 Zdouble time,timescale,cloudiness;
 Crayon oceanColor,iceColor,coldColor,temperateColor,warmColor,hotColor;
 void Randomize( int seed ) {
  oceanColor.random(seed+66);
  iceColor.random(seed+51);
  temperateColor.random(seed+42);
  warmColor.random(seed+33);
  hotColor.random(seed+24);
  coldColor.random(seed+15);
  noiseOffset.Set(iratiod(upto(100, seed),100), iratiod(upto(100, seed + 100),100));
  cloudiness = iratiod(upto(100, seed + 1234), 100) * 0.8;
 }
 BINARY_IN({
  B_IN_SSUB(pos2d)
  B_IN_SSUB(noiseOffset)
  B_IN_SSUB(noiseScale)
  B_IN_SSUB(noiseScale2)
  B_IN_SSUB(noiseScale3)
  B_IN_SSUB(cloudNoise)
  B_IN_NUM(time)
  B_IN_NUM(timescale)
  B_IN_NUM(cloudiness)
  B_IN_TINT(oceanColor)
  B_IN_TINT(iceColor)
  B_IN_TINT(coldColor)
  B_IN_TINT(temperateColor)
  B_IN_TINT(warmColor)
  B_IN_TINT(hotColor)
 })
 BINARY_OUT({
  B_OUT_SSUB(pos2d)
  B_OUT_SSUB(noiseOffset)
  B_OUT_SSUB(noiseScale)
  B_OUT_SSUB(noiseScale2)
  B_OUT_SSUB(noiseScale3)
  B_OUT_SSUB(cloudNoise)
  B_OUT_NUM(time)
  B_OUT_NUM(timescale)
  B_OUT_NUM(cloudiness)
  B_OUT_TINT(oceanColor)
  B_OUT_TINT(iceColor)
  B_OUT_TINT(coldColor)
  B_OUT_TINT(temperateColor)
  B_OUT_TINT(warmColor)
  B_OUT_TINT(hotColor)
 })
 KEYWORDS({
  SUBWORD("pos2d",pos2d)
  else SUBWORD("noiseOffset",noiseOffset)
  else SUBWORD("noiseScale",noiseScale)
  else SUBWORD("noiseScale2",noiseScale2)
  else SUBWORD("noiseScale3",noiseScale3)
  else SUBWORD("cloudNoise",cloudNoise)
  else KEYWORD("time",{time=w;})
  else KEYWORD("time",{timescale=w;})
  else KEYWORD("cloudiness",{cloudiness=w;})
  else TINTWORD("oceanColor",oceanColor)
  else TINTWORD("iceColor",iceColor)
  else TINTWORD("coldColor",coldColor)
  else TINTWORD("temperateColor",temperateColor)
  else TINTWORD("warmColor",warmColor)
  else TINTWORD("hotColor",hotColor)
 })
 WORDKEYS({
  SSUBKEY("pos2d",pos2d)
  SSUBKEY("noiseOffset",noiseOffset)
  SSUBKEY("noiseScale",noiseScale)
  SSUBKEY("noiseScale2",noiseScale2)
  SSUBKEY("noiseScale3",noiseScale3)
  SSUBKEY("cloudNoise",cloudNoise)
  NUMBKEY("time",time)
  NUMBKEY("time",timescale)
  NUMBKEY("cloudiness",cloudiness)
  TINTKEY("oceanColor",oceanColor)
  TINTKEY("iceColor",iceColor)
  TINTKEY("coldColor",coldColor)
  TINTKEY("temperateColor",temperateColor)
  TINTKEY("warmColor",warmColor)
  TINTKEY("hotColor",hotColor)
 })
MANY(EarthlikeProfile,EarthlikeProfileHandle,EarthlikeProfileHandles,"Earthlike",EarthlikeProfiles,{})
 KEYWORDSGroup("Earthlike",EarthlikeProfile);
 WORDKEYSGroup("Earthlike",EarthlikeProfile);
 BINARY_INOUT(EarthlikeProfile)
DONE(EarthlikeProfile);

class EarthlikeShader : public GLSLShader {
public:
 Cartesiand pos2d,noiseOffset,noiseScale,noiseScale2,noiseScale3,cloudNoise;
 Zdouble time,timescale,cloudiness;
 Crayon oceanColor,iceColor,coldColor,temperateColor,warmColor,hotColor;
 GLuint Upos2d,UnoiseOffset,UnoiseScale,UnoiseScale2,UnoiseScale3,UcloudNoise,Utime,Utimescale,Ucloudiness,UoceanColor,UiceColor,UcoldColor,UtemperateColor,UwarmColor,UhotColor;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Upos2d=glGetUniformLocation(program, "pos2d");
  UnoiseOffset=glGetUniformLocation(program, "noiseOffset");
  UnoiseScale=glGetUniformLocation(program, "noiseScale");
  UnoiseScale2=glGetUniformLocation(program, "noiseScale2");
  UnoiseScale3=glGetUniformLocation(program, "noiseScale3");
  UcloudNoise=glGetUniformLocation(program, "cloudNoise");
  Utime=glGetUniformLocation(program, "time");
  Utimescale=glGetUniformLocation(program, "timescale");
  Ucloudiness=glGetUniformLocation(program, "cloudiness");
  UoceanColor=glGetUniformLocation(program, "oceanColor");
  UiceColor=glGetUniformLocation(program, "iceColor");
  UcoldColor=glGetUniformLocation(program, "coldColor");
  UtemperateColor=glGetUniformLocation(program, "temperateColor");
  UwarmColor=glGetUniformLocation(program, "warmColor");
  UhotColor=glGetUniformLocation(program, "hotColor");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Upos2d,(GLfloat)pos2d.x,(GLfloat)pos2d.y);
  glUniform2f(UnoiseOffset,(GLfloat)noiseOffset.x,(GLfloat)noiseOffset.y);
  glUniform2f(UnoiseScale,(GLfloat)noiseScale.x,(GLfloat)noiseScale.y);
  glUniform2f(UnoiseScale2,(GLfloat)noiseScale2.x,(GLfloat)noiseScale2.y);
  glUniform2f(UnoiseScale3,(GLfloat)noiseScale3.x,(GLfloat)noiseScale3.y);
  glUniform2f(UcloudNoise,(GLfloat)cloudNoise.x,(GLfloat)cloudNoise.y);
  glUniform3f(UoceanColor,(GLfloat)oceanColor.rf,(GLfloat)oceanColor.gf,(GLfloat)oceanColor.bf);
  glUniform3f(UiceColor,(GLfloat)iceColor.rf,(GLfloat)iceColor.gf,(GLfloat)iceColor.bf);
  glUniform3f(UcoldColor,(GLfloat)coldColor.rf,(GLfloat)coldColor.gf,(GLfloat)coldColor.bf);
  glUniform3f(UtemperateColor,(GLfloat)temperateColor.rf,(GLfloat)temperateColor.gf,(GLfloat)temperateColor.bf);
  glUniform3f(UwarmColor,(GLfloat)warmColor.rf,(GLfloat)warmColor.gf,(GLfloat)warmColor.bf);
  glUniform3f(UhotColor,(GLfloat)hotColor.rf,(GLfloat)hotColor.gf,(GLfloat)hotColor.bf);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(Ucloudiness,(GLfloat)cloudiness);

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
  glUseProgram(0);
 }
 void Set( EarthlikeProfile *in ) {
  pos2d.Set(&in->pos2d);
  noiseOffset.Set(&in->noiseOffset);
  noiseScale.Set(&in->noiseScale);
  noiseScale2.Set(&in->noiseScale2);
  noiseScale3.Set(&in->noiseScale3);
  cloudNoise.Set(&in->cloudNoise);
  time=in->time;
  cloudiness=in->cloudiness;
  oceanColor.fromCrayon(in->oceanColor);
  iceColor.fromCrayon(in->iceColor);
  coldColor.fromCrayon(in->coldColor);
  temperateColor.fromCrayon(in->temperateColor);
  warmColor.fromCrayon(in->warmColor);
  hotColor.fromCrayon(in->hotColor);
  timescale=in->timescale;
 }
};

// ----------------------------------------------------



// ----------------------------------------------------

ONE(AccretionProfile,{
 tex1=library.Load("data/images/textures/moldy.png",tllto_Tiling);
 tex2=library.Load("data/images/textures/noise3.png",tllto_Tiling);
 tex3=library.Load("data/images/textures/noise6.png",tllto_Tiling);
})
 Cartesiand resolution,pos2d,scale;
 Zp<GLImage> tex1,tex2,tex3;
 Zdouble time;
 BINARY_IN({
  B_IN_SSUB(pos2d)
  B_IN_SSUB(scale)
  B_IN_TILE(tex1)
  B_IN_TILE(tex2)
  B_IN_TILE(tex3)
  B_IN_NUM(time)
 })
 BINARY_OUT({
  B_OUT_SSUB(pos2d)
  B_OUT_SSUB(scale)
  B_OUT_TILE(tex1)
  B_OUT_TILE(tex2)
  B_OUT_TILE(tex3)
  B_OUT_NUM(time)
 })
 KEYWORDS({
  SUBWORD("pos2d",pos2d)
  else SUBWORD("scale",scale)
  else TILEWORD("tex1",tex1)
  else TILEWORD("tex2",tex2)
  else TILEWORD("tex3",tex3)
  else KEYWORD("time",{time=w;})
 })
 WORDKEYS({
  SSUBKEY("pos2d",pos2d)
  SSUBKEY("scale",scale)
  IMGKEY("tex1",tex1)
  IMGKEY("tex2",tex2)
  IMGKEY("tex3",tex3)
  NUMBKEY("time",time)
 })
MANY(AccretionProfile,AccretionProfileHandle,AccretionProfileHandles,"Accretion",AccretionProfiles,{})
 KEYWORDSGroup("Accretion",AccretionProfile);
 WORDKEYSGroup("Accretion",AccretionProfile);
 BINARY_INOUT(AccretionProfile);
DONE(AccretionProfile);

class AccretionShader : public GLSLShader {
public:
 Cartesiand pos2d,scale;
 Zp<GLuint> tex1,tex2,tex3;
 Zdouble time;
 GLuint Upos2d,Uscale,Utex1,Utex2,Utex3,Utime;
GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Upos2d=glGetUniformLocation(program, "pos2d");
  Uscale=glGetUniformLocation(program, "scale");
  Utex1=glGetUniformLocation(program, "tex1");
  Utex2=glGetUniformLocation(program, "tex2");
  Utex3=glGetUniformLocation(program, "tex3");
  Utime=glGetUniformLocation(program, "time");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Upos2d,(GLfloat)pos2d.x,(GLfloat)pos2d.y);
  glUniform3f(Uscale,(GLfloat)scale.x,(GLfloat)scale.y,(GLfloat)scale.z);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1i(Utex1, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *tex1);
  glUniform1i(Utex2, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *tex2);
  glUniform1i(Utex3, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *tex3);
  
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
 void Set( AccretionProfile *in ) {
  pos2d.Set(&in->pos2d);
  scale.Set(&in->scale);
  tex1=&in->tex1->texture;
  tex2=&in->tex2->texture;
  tex3=&in->tex3->texture;
  time=in->time;
 }
};


// ----------------------------------------------------

ONE(BlackHoleProfile,{
 color1.Double(0.5,0.35,0.1,1.0);
 color2.Double(1.0,0.8,0.6,1.0);
 color3.Double(1.0,0.9,0.7,1.0);
 blackholeSize.Set(0.3,5.0);
 attenuate.Set(0.1,0.75,0.1);
 tex2=library.Load("data/images/textures/noise1.png",tllto_Tiling);
})
 Zdouble time;
 Crayon color1,color2,color3;
 Cartesiand orientation,blackholeSize,attenuate;
 Zp<GLImage> tex2;
 BINARY_IN({
  B_IN_NUM(time)
  B_IN_TINT(color1)
  B_IN_TINT(color2)
  B_IN_TINT(color3)
  B_IN_SSUB(orientation)
  B_IN_SSUB(blackholeSize)
  B_IN_SSUB(attenuate)
  B_IN_TILE(tex2)
 })
 BINARY_OUT({
  B_OUT_NUM(time)
  B_OUT_TINT(color1)
  B_OUT_TINT(color2)
  B_OUT_TINT(color3)
  B_OUT_SSUB(orientation)
  B_OUT_SSUB(blackholeSize)
  B_OUT_SSUB(attenuate)
  B_OUT_TILE(tex2)
 })
 KEYWORDS({
  NUMWORD("time",time)
  else TINTWORD("color1",color1)
  else TINTWORD("color2",color2)
  else TINTWORD("color3",color3)
  else SUBWORD("orientation",orientation)
  else SUBWORD("blackholeSize",blackholeSize)
  else SUBWORD("attenuate",attenuate)
  else TILEWORD("tex2",tex2)
 })
 WORDKEYS({
  TINTKEY("color1",color1)
  TINTKEY("color2",color2)
  TINTKEY("color3",color3)
  SSUBKEY("orientation",orientation)
  SSUBKEY("blackholeSize",blackholeSize)
  SSUBKEY("attenuate",attenuate)
  IMGKEY("tex2",tex2)
 })
MANY(BlackHoleProfile,BlackHoleProfileHandle,BlackHoleProfileHandles,"BlackHole",BlackHoleProfiles,{})
 KEYWORDSGroup("BlackHole",BlackHoleProfile);
 WORDKEYSGroup("BlackHole",BlackHoleProfile);
 BINARY_INOUT(BlackHoleProfile);
DONE(BlackHoleProfile);

class BlackHoleShader : public GLSLShader {
public:
 Zdouble time;
 Crayon color1,color2,color3;
 Cartesiand orientation,blackholeSize,attenuate;
 Zp<GLuint> tex2;
 GLuint Ucolor1,Ucolor2,Ucolor3,Uorientation,UblackholeSize,Uattenuate,Utex2,Utime;
GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Ucolor1=glGetUniformLocation(program, "color1");
  Ucolor2=glGetUniformLocation(program, "color2");
  Ucolor3=glGetUniformLocation(program, "color3");
  Uorientation=glGetUniformLocation(program, "orientation");
  UblackholeSize=glGetUniformLocation(program, "blackholeSize");
  Uattenuate=glGetUniformLocation(program, "attenuate");
  Utex2=glGetUniformLocation(program, "tex2");
  Utime=glGetUniformLocation(program,"time");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Uorientation,(GLfloat)orientation.x,(GLfloat)orientation.y);
  glUniform2f(UblackholeSize,(GLfloat)blackholeSize.x,(GLfloat)blackholeSize.y);
  glUniform3f(Uattenuate,(GLfloat)attenuate.x,(GLfloat)attenuate.y,(GLfloat)attenuate.z);
  glUniform3f(Ucolor1,(GLfloat)color1.rf,(GLfloat)color1.gf,(GLfloat)color1.bf);
  glUniform3f(Ucolor2,(GLfloat)color2.rf,(GLfloat)color2.gf,(GLfloat)color2.bf);
  glUniform3f(Ucolor3,(GLfloat)color3.rf,(GLfloat)color3.gf,(GLfloat)color3.bf);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1i(Utex2, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *tex2);
  
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
 void Set( BlackHoleProfile *in ) {
  color1.fromCrayon(in->color1);
  color2.fromCrayon(in->color2);
  color3.fromCrayon(in->color3);
  orientation.Set(&in->orientation);
  blackholeSize.Set(&in->blackholeSize);
  attenuate.Set(&in->attenuate);
  tex2=&in->tex2->texture;
  time=in->time;
 }
};




// ----------------------------------------------------
ONE(RingedJovianProfile,{
 ringColor.Double(0.95,0.8,0.7,1.0);
 planetColor.Double(0.9,0.75,0.6,1.0);
 surfaceParams.Set(1.2,2.0,0.05); surfaceParams.w=0.6;
 ringParams.Set(0.6,2.0,8.1);
 sunpos.Set(50,10,20);
})
 Crayon ringColor,planetColor;
 Cartesiand surfaceParams,ringParams,sunpos;
 Zdouble time;
 void Randomize(int seed) {
  ringColor.random(seed);
  planetColor.random(seed + 1492);
  surfaceParams.Set(1.2, 2.0, 0.05); surfaceParams.w = 0.6;
  surfaceParams.Multiply(iratiod(upto(100, seed + 23), 50));
  surfaceParams.w *=iratiod(upto(100), 100);
  surfaceParams.w += 0.1;
 }
 BINARY_IN({
  B_IN_TINT(ringColor)
  B_IN_TINT(planetColor)
  B_IN_SSUB(surfaceParams)
  B_IN_SSUB(ringParams)
  B_IN_SSUB(sunpos)
  B_IN_NUM(time)
 })
 BINARY_OUT({
  B_OUT_TINT(ringColor)
  B_OUT_TINT(planetColor)
  B_OUT_SSUB(surfaceParams)
  B_OUT_SSUB(ringParams)
  B_OUT_SSUB(sunpos)
  B_OUT_NUM(time)
 })
 KEYWORDS({
  TINTWORD("ringColor",ringColor)
  else TINTWORD("planetColor",planetColor)
  else SUBWORD("surfaceParams",surfaceParams)
  else SUBWORD("ringParams",ringParams)
  else SUBWORD("sunpos",sunpos)
  else KEYWORD("time",{time=w;})
 })
 WORDKEYS({
  TINTKEY("ringColor",ringColor)
  TINTKEY("planetColor",planetColor)
  SSUBKEY("surfaceParams",surfaceParams)
  SSUBKEY("ringParams",ringParams)
  SSUBKEY("sunpos",sunpos)
  NUMBKEY("time",time)
 })
MANY(RingedJovianProfile,RingedJovianProfileHandle,RingedJovianProfileHandles,"RingedJovian",RingedJovianProfiles,{})
 KEYWORDSGroup("RingedJovian",RingedJovianProfile);
 WORDKEYSGroup("RingedJovian",RingedJovianProfile);
 BINARY_INOUT(RingedJovianProfile)
DONE(RingedJovianProfile);

class RingedJovianShader : public GLSLShader {
public:
 Crayon ringColor,planetColor;
 Cartesiand surfaceParams,ringParams,sunpos;
 Zdouble time;
 GLuint UringColor,UplanetColor,UsurfaceParams,UringParams,Usunpos,Utime;
GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  UringColor=glGetUniformLocation(program, "ringColor");
  UplanetColor=glGetUniformLocation(program, "planetColor");
  UsurfaceParams=glGetUniformLocation(program, "surfaceParams");
  UringParams=glGetUniformLocation(program, "ringParams");
  Usunpos=glGetUniformLocation(program, "sunpos");
  Utime=glGetUniformLocation(program, "time");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform4f(UsurfaceParams,(GLfloat)surfaceParams.x,(GLfloat)surfaceParams.y,(GLfloat)surfaceParams.z,(GLfloat)surfaceParams.w);
  glUniform3f(UringParams,(GLfloat)ringParams.x,(GLfloat)ringParams.y,(GLfloat)ringParams.z);
  glUniform3f(Usunpos,(GLfloat)sunpos.x,(GLfloat)sunpos.y,(GLfloat)sunpos.z);
  glUniform3f(UringColor,(GLfloat)ringColor.rf,(GLfloat)ringColor.gf,(GLfloat)ringColor.bf);
  glUniform3f(UplanetColor,(GLfloat)planetColor.rf,(GLfloat)planetColor.gf,(GLfloat)planetColor.bf);
  glUniform1f(Utime,(GLfloat)time);
  
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
  glUseProgram(0);
 }
 void Set( RingedJovianProfile *in ) {
  ringColor.fromCrayon(in->ringColor);
  planetColor.fromCrayon(in->planetColor);
  surfaceParams.Set(&in->surfaceParams);
  ringParams.Set(&in->ringParams);
  sunpos.Set(&in->sunpos);
  time=in->time;
 }
};



// ----------------------------------------------------
ONE(ElectroPlanetProfile,{
 surfaceTex=library.Load("data/images/textures/moldy.png",tllto_Tiling);
 sampleTex=library.Load("data/images/textures/noise1.png",tllto_Tiling);
 position.Set(0.5,0.5);
 oColor.Double( 0.2, 0.25, 0.3, 1.0 );
 oRColor.Double( 0.6, 0.75, 0.6, 1.0 );
 glowAmount=2.2;
})
 Crayon oColor,oRColor;
 Zp<GLImage> surfaceTex,sampleTex;
 Cartesiand position;
 Zdouble time,glowAmount;
 BINARY_IN({
  B_IN_TINT(oColor)
  B_IN_TINT(oRColor)
  B_IN_TILE(surfaceTex)
  B_IN_TILE(sampleTex)
  B_IN_SSUB(position)
  B_IN_NUM(time)
  B_IN_NUM(glowAmount)
 })
 BINARY_OUT({
  B_OUT_TINT(oColor)
  B_OUT_TINT(oRColor)
  B_OUT_TILE(surfaceTex)
  B_OUT_TILE(sampleTex)
  B_OUT_SSUB(position)
  B_OUT_NUM(time)
  B_OUT_NUM(glowAmount)
 })
 KEYWORDS({
  TINTWORD("oColor",oColor)
  else TINTWORD("oRColor",oRColor)
  else TILEWORD("surfaceTex",surfaceTex)
  else TILEWORD("sampleTex",sampleTex)
  else SUBWORD("position",position)
  else KEYWORD("time",{time=w;})
  else KEYWORD("glowAmount",{glowAmount=w;})
 })
 WORDKEYS({
  TINTKEY("oColor",oColor)
  TINTKEY("oRColor",oRColor)
  IMGKEY("surfaceTex",surfaceTex)
  IMGKEY("sampleTex",sampleTex)
  SSUBKEY("position",position)
  NUMBKEY("time",time)
  NUMBKEY("glowAmount",glowAmount)
 })
MANY(ElectroPlanetProfile,ElectroPlanetProfileHandle,ElectroPlanetProfileHandles,"ElectroPlanet",ElectroPlanetProfiles,{})
 KEYWORDSGroup("ElectroPlanet",ElectroPlanetProfile);
 WORDKEYSGroup("ElectroPlanet",ElectroPlanetProfile);
DONE(ElectroPlanetProfile);

class ElectroPlanetShader : public GLSLShader {
public:
 Crayon oColor,oRColor;
 Zp<GLuint> surfaceTex,sampleTex;
 Cartesiand position;
 Zdouble time,glowAmount;
 GLuint UoColor,UoRColor,UsurfaceTex,UsampleTex,Upos,Utime,UglowAmount;
GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  UoColor=glGetUniformLocation(program, "oColor");
  UoRColor=glGetUniformLocation(program, "oRColor");
  UsurfaceTex=glGetUniformLocation(program, "surfaceTex");
  UsampleTex=glGetUniformLocation(program, "sampleTex");
  Upos=glGetUniformLocation(program, "pos");
  Utime=glGetUniformLocation(program, "time");
  UglowAmount=glGetUniformLocation(program, "glowAmount");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Upos,(GLfloat)position.x,(GLfloat)position.y);
  glUniform3f(UoColor,(GLfloat)oColor.rf,(GLfloat)oColor.gf,(GLfloat)oColor.bf);
  glUniform3f(UoRColor,(GLfloat)oRColor.rf,(GLfloat)oRColor.gf,(GLfloat)oRColor.bf);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(UglowAmount,(GLfloat)glowAmount);
  glUniform1i(UsurfaceTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *surfaceTex);
  glUniform1i(UsampleTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *sampleTex);
  
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
 void Set( ElectroPlanetProfile *in ) {
  oColor.fromCrayon(in->oColor);
  oRColor.fromCrayon(in->oRColor);
  surfaceTex=&in->surfaceTex->texture;
  sampleTex=&in->sampleTex->texture;
  position.Set(&in->position);
  time=in->time;
  glowAmount=in->glowAmount;
 }
};



// ----------------------------------------------------

ONE(AtmosphericPlanetProfile,{
 oceanBlueColor.Double(0.0,0.031,0.11,1.0);
 snowColor.Double(0.85/1.85,0.85/1.85,0.85/1.85,1.0);
 desertColor.Double(1.5/13.0,1.3/13.0,1.0/13.0,1.0);
 grassColor.Double(1.0/12.0,1.3/12.0,0.5/12.0,1.0);
 landRatios.Set(1.85,13.0,12.0);
 cloudColor.Double(1.0,1.0,1.0,1.0);
 lightIntensity=4.0;
 landHeight= 4.0;
 cloudComplexity=2.01;
 opacityGain=1.4;
 brightnessGain=0.7;
 haze=2.0;
 baseAtmColor.Double(0.0*1.5,0.041*1.5,0.11*1.5,1.0);
 glowIntensity=5.5;
 glowCutoff=16.0;
 position.Set(0.5,0.5);
})
 Cartesiand position,landRatios;
 Zdouble time,lightIntensity,landHeight,cloudComplexity,opacityGain,brightnessGain,haze,glowIntensity,glowCutoff;
 Crayon oceanBlueColor,snowColor,desertColor,grassColor,cloudColor,baseAtmColor;
 BINARY_IN({
  B_IN_SSUB(position)
  B_IN_SSUB(landRatios)
  B_IN_NUM(time)
  B_IN_NUM(lightIntensity)
  B_IN_NUM(landHeight)
  B_IN_NUM(cloudComplexity)
  B_IN_NUM(opacityGain)
  B_IN_NUM(brightnessGain)
  B_IN_NUM(haze)
  B_IN_NUM(glowIntensity)
  B_IN_NUM(glowCutoff)
  B_IN_TINT(oceanBlueColor)
  B_IN_TINT(snowColor)
  B_IN_TINT(desertColor)
  B_IN_TINT(grassColor)
  B_IN_TINT(cloudColor)
  B_IN_TINT(baseAtmColor)
 })
 BINARY_OUT({
  B_OUT_SSUB(position)
  B_OUT_SSUB(landRatios)
  B_OUT_NUM(time)
  B_OUT_NUM(lightIntensity)
  B_OUT_NUM(landHeight)
  B_OUT_NUM(cloudComplexity)
  B_OUT_NUM(opacityGain)
  B_OUT_NUM(brightnessGain)
  B_OUT_NUM(haze)
  B_OUT_NUM(glowIntensity)
  B_OUT_NUM(glowCutoff)
  B_OUT_TINT(oceanBlueColor)
  B_OUT_TINT(snowColor)
  B_OUT_TINT(desertColor)
  B_OUT_TINT(grassColor)
  B_OUT_TINT(cloudColor)
  B_OUT_TINT(baseAtmColor)
 })
 KEYWORDS({
  SUBWORD("position",position)
  else SUBWORD("landRatios",landRatios)
  else KEYWORD("time",{time=w;})
  else KEYWORD("lightIntensity",{lightIntensity=w;})
  else KEYWORD("landHeight",{landHeight=w;})
  else KEYWORD("cloudComplexity",{cloudComplexity=w;})
  else KEYWORD("opacityGain",{opacityGain=w;})
  else KEYWORD("brightnessGain",{brightnessGain=w;})
  else KEYWORD("haze",{haze=w;})
  else KEYWORD("glowIntensity",{glowIntensity=w;})
  else KEYWORD("glowCutoff",{glowCutoff=w;})
  else TINTWORD("oceanBlueColor",oceanBlueColor)
  else TINTWORD("snowColor",snowColor)
  else TINTWORD("desertColor",desertColor)
  else TINTWORD("grassColor",grassColor)
  else TINTWORD("cloudColor",cloudColor)
  else TINTWORD("baseAtmColor",baseAtmColor)
 })
 WORDKEYS({
  SSUBKEY("position",position)
  SSUBKEY("landRatios",landRatios)
  NUMBKEY("time",time)
  NUMBKEY("lightIntensity",lightIntensity)
  NUMBKEY("landHeight",landHeight)
  NUMBKEY("cloudComplexity",cloudComplexity)
  NUMBKEY("opacityGain",opacityGain)
  NUMBKEY("brightnessGain",brightnessGain)
  NUMBKEY("haze",haze)
  NUMBKEY("glowIntensity",glowIntensity)
  NUMBKEY("glowCutoff",glowCutoff)
  TINTKEY("oceanBlueColor",oceanBlueColor)
  TINTKEY("snowColor",snowColor)
  TINTKEY("desertColor",desertColor)
  TINTKEY("grassColor",grassColor)
  TINTKEY("cloudColor",cloudColor)
  TINTKEY("baseAtmColor",baseAtmColor)
 })
MANY(AtmosphericPlanetProfile,AtmosphericPlanetProfileHandle,AtmosphericPlanetProfileHandles,"AtmosphericPlanet",AtmosphericPlanetProfiles,{})
 KEYWORDSGroup("AtmosphericPlanet",AtmosphericPlanetProfile);
 WORDKEYSGroup("AtmosphericPlanet",AtmosphericPlanetProfile);
DONE(AtmosphericPlanetProfile);

class AtmosphericPlanetShader : public GLSLShader {
public:
 Cartesiand position,landRatios;
 Zdouble time,lightIntensity,landHeight,cloudComplexity,opacityGain,brightnessGain,haze,glowIntensity,glowCutoff;
 Crayon oceanBlueColor,snowColor,desertColor,grassColor,cloudColor,baseAtmColor;
 GLuint Upos,UlandRatios,Utime,UlightIntensity,UlandHeight,UcloudComplexity,UopacityGain,UbrightnessGain,Uhaze,UglowIntensity,UglowCutoff,UoceanBlueColor,UsnowColor,UdesertColor,UgrassColor,UcloudColor,UbaseAtmColor;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Upos=glGetUniformLocation(program, "pos");
  UlandRatios=glGetUniformLocation(program, "landRatios");
  Utime=glGetUniformLocation(program, "time");
  UlightIntensity=glGetUniformLocation(program, "lightIntensity");
  UlandHeight=glGetUniformLocation(program, "landHeight");
  UcloudComplexity=glGetUniformLocation(program, "cloudComplexity");
  UopacityGain=glGetUniformLocation(program, "opacityGain");
  UbrightnessGain=glGetUniformLocation(program, "brightnessGain");
  Uhaze=glGetUniformLocation(program, "haze");
  UglowIntensity=glGetUniformLocation(program, "glowIntensity");
  UglowCutoff=glGetUniformLocation(program, "glowCutoff");
  UoceanBlueColor=glGetUniformLocation(program, "oceanBlueColor");
  UsnowColor=glGetUniformLocation(program, "snowColor");
  UdesertColor=glGetUniformLocation(program, "desertColor");
  UgrassColor=glGetUniformLocation(program, "grassColor");
  UcloudColor=glGetUniformLocation(program, "cloudColor");
  UbaseAtmColor=glGetUniformLocation(program, "baseAtmColor");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Upos,(GLfloat)position.x,(GLfloat)position.y);
  glUniform3f(UlandRatios,(GLfloat)landRatios.x,(GLfloat)landRatios.y,(GLfloat)landRatios.z);
  glUniform3f(UoceanBlueColor,(GLfloat)oceanBlueColor.rf,(GLfloat)oceanBlueColor.gf,(GLfloat)oceanBlueColor.bf);
  glUniform3f(UsnowColor,(GLfloat)snowColor.rf,(GLfloat)snowColor.gf,(GLfloat)snowColor.bf);
  glUniform3f(UdesertColor,(GLfloat)desertColor.rf,(GLfloat)desertColor.gf,(GLfloat)desertColor.bf);
  glUniform3f(UgrassColor,(GLfloat)grassColor.rf,(GLfloat)grassColor.gf,(GLfloat)grassColor.bf);
  glUniform3f(UcloudColor,(GLfloat)cloudColor.rf,(GLfloat)cloudColor.gf,(GLfloat)cloudColor.bf);
  glUniform3f(UbaseAtmColor,(GLfloat)baseAtmColor.rf,(GLfloat)baseAtmColor.gf,(GLfloat)baseAtmColor.bf);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(UlightIntensity,(GLfloat)lightIntensity);
  glUniform1f(UlandHeight,(GLfloat)landHeight);
  glUniform1f(UcloudComplexity,(GLfloat)cloudComplexity);
  glUniform1f(UopacityGain,(GLfloat)opacityGain);
  glUniform1f(UbrightnessGain,(GLfloat)brightnessGain);
  glUniform1f(Uhaze,(GLfloat)haze);
  glUniform1f(UglowIntensity,(GLfloat)glowIntensity);
  glUniform1f(UglowCutoff,(GLfloat)glowCutoff);
  
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
  glUseProgram(0);
 }
 void Set( AtmosphericPlanetProfile *in ) {
  position.Set(&in->position);
  landRatios.Set(&in->landRatios);
  time=in->time;
  lightIntensity=in->lightIntensity;
  landHeight=in->landHeight;
  cloudComplexity=in->cloudComplexity;
  opacityGain=in->opacityGain;
  brightnessGain=in->brightnessGain;
  haze=in->haze;
  glowIntensity=in->glowIntensity;
  glowCutoff=in->glowCutoff;
  oceanBlueColor.fromCrayon(in->oceanBlueColor);
  snowColor.fromCrayon(in->snowColor);
  desertColor.fromCrayon(in->desertColor);
  grassColor.fromCrayon(in->grassColor);
  cloudColor.fromCrayon(in->cloudColor);
  baseAtmColor.fromCrayon(in->baseAtmColor);
 }
};



// ----------------------------------------------------------

ONE(Earthlike2Profile,{
 planetParams.Set(0.3,2.5,0.55); planetParams.w=0.45;
 planetParams2.Set(0.5,15.5,0.4); planetParams2.w=0.45;
 planetParams3.Set(0.5,0.75,0.55); planetParams3.w=0.8;
 surfaceThresholdColor.Double(0.2,0.9,0.5,1.0);
 secondaryMaterialColor.Double(0.3,0.3,0.3,1.0);
 specular=0.03;
 specularPos.Set(0.9,0.3,1.0);
 ambientColor.Double(0.1,0.1,0.1,1.0);
 atmosphereDiffuseColor.Double(0.4,0.0,1.0,1.0);
 atmosphereSpecularColor.Double(0.7,0.9,1.0,1.0);
 darkDiffuseColor.Double(0.2,0.1,0.05,1.0);
 darkSpecularColor.Double(0.2,0.1,0.05,1.0);
 oceanColor.Double(0.2,0.3,0.4,1.0);
 diffusion=0.75;
 zoom=2.5;
 cloudset=2.0;
 landset=0.5;
 tex=library.Load("data/images/textures/rubble.png",tllto_Tiling);
 tex2=library.Load("data/images/textures/noise8.png",tllto_Tiling);
})
 Zp<GLImage> tex,tex2;
 Zdouble time,specular,diffusion,zoom,cloudset,landset;
 Cartesiand rotation,planetParams,planetParams2,planetParams3,specularPos;
 Crayon surfaceThresholdColor,secondaryMaterialColor,ambientColor,atmosphereDiffuseColor,atmosphereSpecularColor,darkDiffuseColor,darkSpecularColor,oceanColor;
 void Randomize(int seed) {
  double a = iratiod(upto(1000, seed), 1000);      a =0.1 + a / 2.0 - a * 0.3;
  double b = iratiod(upto(1000, seed + 1), 1000);  b =0.1 + b / 2.0 - b * 2.5;
  double c = iratiod(upto(1000, seed + 2), 1000);  c =0.1 + c / 2.0 - c * 0.55;
  double d = iratiod(upto(1000, seed + 3), 1000);  d =0.1 + d / 2.0 - d * 0.45;
  double e = iratiod(upto(1000, seed + 4), 1000);  e =0.1 + e / 2.0 - e * 0.5;
  double f = iratiod(upto(1000, seed + 5), 1000);  f =5.0 + f / 2.0 - f * 15.5;
  double g = iratiod(upto(1000, seed + 6), 1000);  g =0.1 + g / 2.0 - g * 0.4;
  double h = iratiod(upto(1000, seed + 7), 1000);  h =0.1 + h / 2.0 - h * 0.45;
  double i = iratiod(upto(1000, seed + 6), 1000);  i =0.1 + i / 2.0 - i * 0.5;
  double j = iratiod(upto(1000, seed + 9), 1000);  j =0.1 + j / 2.0 - j * 0.75;
  double k = iratiod(upto(1000, seed + 10), 1000); k =0.1 + k / 2.0 - k * 0.55;
  double l = iratiod(upto(1000, seed + 11), 1000); l =0.1 + l / 2.0 - l * 0.8;
  planetParams.Set(0.3+a, 2.5+b, 0.55+c); planetParams.w = 0.45+d;
  planetParams2.Set(0.5+e, 15.5+f, 0.4+g); planetParams2.w = 0.45+h;
  planetParams3.Set(0.5+i, 0.75+j, 0.55+k); planetParams3.w = 0.8+l;
  diffusion = 0.35 + (a + b + c + d + e + f) / 6.0; if (diffusion > 0.9) diffusion = 0.9;
  surfaceThresholdColor.random(seed + 1243);
  secondaryMaterialColor.random(seed + 5123);
  ambientColor.random(seed + 643);
  atmosphereDiffuseColor.random(seed + 845);
  atmosphereSpecularColor.random(seed + 124);
  darkDiffuseColor.random(seed + 754);
  darkSpecularColor.random(seed + 865);
  oceanColor.random(seed + 532);
  surfaceThresholdColor.MultiplyClampRGB(0.2, 0.9, 0.5);
  secondaryMaterialColor.MultiplyClampRGB(0.3, 0.3, 0.3);
  ambientColor.MultiplyClampRGB(0.1, 0.1, 0.1);
  atmosphereDiffuseColor.MultiplyClampRGB(0.4, 0.0, 1.0);
  atmosphereSpecularColor.MultiplyClampRGB(0.7, 0.9, 1.0);
  darkDiffuseColor.MultiplyClampRGB(0.2, 0.1, 0.05);
  darkSpecularColor.MultiplyClampRGB(0.2, 0.1, 0.05);
  oceanColor.MultiplyClampRGB(0.2, 0.3, 0.4);
  surfaceThresholdColor.MultiplyClampRGB(2.0,2.0,2.0);
  secondaryMaterialColor.MultiplyClampRGB(2.0, 2.0, 2.0);
  ambientColor.MultiplyClampRGB(2.0, 2.0, 2.0);
  atmosphereDiffuseColor.MultiplyClampRGB(2.0, 2.0, 2.0);
  atmosphereSpecularColor.MultiplyClampRGB(2.0, 2.0, 2.0);
  darkDiffuseColor.MultiplyClampRGB(2.0, 2.0, 2.0);
  darkSpecularColor.MultiplyClampRGB(2.0, 2.0, 2.0);
  oceanColor.MultiplyClampRGB(2.0, 2.0, 2.0);
 }
 BINARY_IN({
  B_IN_TILE(tex)
  B_IN_TILE(tex2)
  B_IN_NUM(time)
  B_IN_NUM(specular)
  B_IN_NUM(diffusion)
  B_IN_NUM(zoom)
  B_IN_NUM(cloudset)
  B_IN_NUM(landset)
  B_IN_SSUB(rotation)
  B_IN_SSUB(planetParams)
  B_IN_SSUB(planetParams2)
  B_IN_SSUB(planetParams3)
  B_IN_SSUB(specularPos)
  B_IN_TINT(surfaceThresholdColor)
  B_IN_TINT(secondaryMaterialColor)
  B_IN_TINT(ambientColor)
  B_IN_TINT(atmosphereDiffuseColor)
  B_IN_TINT(atmosphereSpecularColor)
  B_IN_TINT(darkDiffuseColor)
  B_IN_TINT(darkSpecularColor)
  B_IN_TINT(oceanColor)
 })
 BINARY_OUT({
  B_OUT_TILE(tex)
  B_OUT_TILE(tex2)
  B_OUT_NUM(time)
  B_OUT_NUM(specular)
  B_OUT_NUM(diffusion)
  B_OUT_NUM(zoom)
  B_OUT_NUM(cloudset)
  B_OUT_NUM(landset)
  B_OUT_SSUB(rotation)
  B_OUT_SSUB(planetParams)
  B_OUT_SSUB(planetParams2)
  B_OUT_SSUB(planetParams3)
  B_OUT_SSUB(specularPos)
  B_OUT_TINT(surfaceThresholdColor)
  B_OUT_TINT(secondaryMaterialColor)
  B_OUT_TINT(ambientColor)
  B_OUT_TINT(atmosphereDiffuseColor)
  B_OUT_TINT(atmosphereSpecularColor)
  B_OUT_TINT(darkDiffuseColor)
  B_OUT_TINT(darkSpecularColor)
  B_OUT_TINT(oceanColor)
 })
 KEYWORDS({
  TILEWORD("tex",tex)
  else TILEWORD("tex2",tex2)
  else KEYWORD("time",{time=w;})
  else KEYWORD("specular",{specular=w;})
  else KEYWORD("diffusion",{diffusion=w;})
  else KEYWORD("zoom",{zoom=w;})
  else KEYWORD("cloudset",{cloudset=w;})
  else KEYWORD("landset",{landset=w;})
  else SUBWORD("rotation",rotation)
  else SUBWORD("planetParams",planetParams)
  else SUBWORD("planetParams2",planetParams2)
  else SUBWORD("planetParams3",planetParams3)
  else SUBWORD("specularPos",specularPos)
  else TINTWORD("surfaceThresholdColor",surfaceThresholdColor)
  else TINTWORD("secondaryMaterialColor",secondaryMaterialColor)
  else TINTWORD("ambientColor",ambientColor)
  else TINTWORD("atmosphereDiffuseColor",atmosphereDiffuseColor)
  else TINTWORD("atmosphereSpecularColor",atmosphereSpecularColor)
  else TINTWORD("darkDiffuseColor",darkDiffuseColor)
  else TINTWORD("darkSpecularColor",darkSpecularColor)
  else TINTWORD("oceanColor",oceanColor)
 })
 WORDKEYS({
  IMGKEY("tex",tex)
  IMGKEY("tex2",tex2)
  NUMBKEY("time",time)
  NUMBKEY("specular",specular)
  NUMBKEY("diffusion",diffusion)
  NUMBKEY("zoom",zoom)
  NUMBKEY("cloudset",cloudset)
  NUMBKEY("landset",landset)
  SSUBKEY("rotation",rotation)
  SSUBKEY("planetParams",planetParams)
  SSUBKEY("planetParams2",planetParams2)
  SSUBKEY("planetParams3",planetParams3)
  SSUBKEY("specularPos",specularPos)
  TINTKEY("surfaceThresholdColor",surfaceThresholdColor)
  TINTKEY("secondaryMaterialColor",secondaryMaterialColor)
  TINTKEY("ambientColor",ambientColor)
  TINTKEY("atmosphereDiffuseColor",atmosphereDiffuseColor)
  TINTKEY("atmosphereSpecularColor",atmosphereSpecularColor)
  TINTKEY("darkDiffuseColor",darkDiffuseColor)
  TINTKEY("darkSpecularColor",darkSpecularColor)
  TINTKEY("oceanColor",oceanColor)
 })
MANY(Earthlike2Profile,Earthlike2ProfileHandle,Earthlike2ProfileHandles,"Earthlike2",Earthlike2Profiles,{})
 KEYWORDSGroup("Earthlike2",Earthlike2Profile);
 WORDKEYSGroup("Earthlike2",Earthlike2Profile);
 BINARY_INOUT(Earthlike2Profile)
DONE(Earthlike2Profile);

class Earthlike2Shader : public GLSLShader {
public:
 Zp<GLuint> tex,tex2;
 Zdouble time,specular,diffusion,zoom,cloudset,landset;
 Cartesiand rotation,planetParams,planetParams2,planetParams3,specularPos;
 Crayon surfaceThresholdColor,secondaryMaterialColor,ambientColor,atmosphereDiffuseColor,atmosphereSpecularColor,darkDiffuseColor,darkSpecularColor,oceanColor;
 GLuint Utex,Utex2,Utime,Uspecular,Udiffusion,Uzoom,Ucloudset,Ulandset,Urotation,UplanetParams,UplanetParams2,UplanetParams3,UspecularPos,UsurfaceThresholdColor,UsecondaryMaterialColor,UambientColor,UatmosphereDiffuseColor,UatmosphereSpecularColor,UdarkDiffuseColor,UdarkSpecularColor,UoceanColor;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Utex=glGetUniformLocation(program, "tex");
  Utex2=glGetUniformLocation(program, "tex2");
  Utime=glGetUniformLocation(program, "time");
  Uspecular=glGetUniformLocation(program, "specular");
  Udiffusion=glGetUniformLocation(program, "diffusion");
  Uzoom=glGetUniformLocation(program, "zoom");
  Ucloudset=glGetUniformLocation(program, "cloudset");
  Ulandset=glGetUniformLocation(program, "landset");
  Urotation=glGetUniformLocation(program, "rotation");
  UplanetParams=glGetUniformLocation(program, "planetParams");
  UplanetParams2=glGetUniformLocation(program, "planetParams2");
  UplanetParams3=glGetUniformLocation(program, "planetParams3");
  UspecularPos=glGetUniformLocation(program, "specularPos");
  UsurfaceThresholdColor=glGetUniformLocation(program, "surfaceThresholdColor");
  UsecondaryMaterialColor=glGetUniformLocation(program, "secondaryMaterialColor");
  UambientColor=glGetUniformLocation(program, "ambientColor");
  UatmosphereDiffuseColor=glGetUniformLocation(program, "atmosphereDiffuseColor");
  UatmosphereSpecularColor=glGetUniformLocation(program, "atmosphereSpecularColor");
  UdarkDiffuseColor=glGetUniformLocation(program, "darkDiffuseColor");
  UdarkSpecularColor=glGetUniformLocation(program, "darkSpecularColor");
  UoceanColor=glGetUniformLocation(program, "oceanColor");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Urotation,(GLfloat)rotation.x,(GLfloat)rotation.y);
  glUniform4f(UplanetParams,(GLfloat)planetParams.x,(GLfloat)planetParams.y,(GLfloat)planetParams.z,(GLfloat)planetParams.w);
  glUniform4f(UplanetParams2,(GLfloat)planetParams2.x,(GLfloat)planetParams2.y,(GLfloat)planetParams2.z,(GLfloat)planetParams2.w);
  glUniform4f(UplanetParams3,(GLfloat)planetParams3.x,(GLfloat)planetParams3.y,(GLfloat)planetParams3.z,(GLfloat)planetParams3.w);
  glUniform3f(UspecularPos,(GLfloat)specularPos.x,(GLfloat)specularPos.y,(GLfloat)specularPos.z);
  glUniform3f(UsurfaceThresholdColor,(GLfloat)surfaceThresholdColor.rf,(GLfloat)surfaceThresholdColor.gf,(GLfloat)surfaceThresholdColor.bf);
  glUniform3f(UsecondaryMaterialColor,(GLfloat)secondaryMaterialColor.rf,(GLfloat)secondaryMaterialColor.gf,(GLfloat)secondaryMaterialColor.bf);
  glUniform3f(UambientColor,(GLfloat)ambientColor.rf,(GLfloat)ambientColor.gf,(GLfloat)ambientColor.bf);
  glUniform3f(UatmosphereDiffuseColor,(GLfloat)atmosphereDiffuseColor.rf,(GLfloat)atmosphereDiffuseColor.gf,(GLfloat)atmosphereDiffuseColor.bf);
  glUniform3f(UatmosphereSpecularColor,(GLfloat)atmosphereSpecularColor.rf,(GLfloat)atmosphereSpecularColor.gf,(GLfloat)atmosphereSpecularColor.bf);
  glUniform3f(UdarkDiffuseColor,(GLfloat)darkDiffuseColor.rf,(GLfloat)darkDiffuseColor.gf,(GLfloat)darkDiffuseColor.bf);
  glUniform3f(UdarkSpecularColor,(GLfloat)darkSpecularColor.rf,(GLfloat)darkSpecularColor.gf,(GLfloat)darkSpecularColor.bf);
  glUniform3f(UoceanColor,(GLfloat)oceanColor.rf,(GLfloat)oceanColor.gf,(GLfloat)oceanColor.bf);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(Uspecular,(GLfloat)specular);
  glUniform1f(Udiffusion,(GLfloat)diffusion);
  glUniform1f(Uzoom,(GLfloat)zoom);
  glUniform1f(Ucloudset,(GLfloat)cloudset);
  glUniform1f(Ulandset,(GLfloat)landset);
  glUniform1i(Utex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *tex);
  glUniform1i(Utex2, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *tex2);
  
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
 void Set( Earthlike2Profile *in ) {
  tex=&in->tex->texture;
  tex2=&in->tex2->texture;
  time=in->time;
  specular=in->specular;
  diffusion=in->diffusion;
  zoom=in->zoom;
  cloudset=in->cloudset;
  landset=in->landset;
  rotation.Set(&in->rotation);
  planetParams.Set(&in->planetParams);
  planetParams2.Set(&in->planetParams2);
  planetParams3.Set(&in->planetParams3);
  specularPos.Set(&in->specularPos);
  surfaceThresholdColor.fromCrayon(in->surfaceThresholdColor);
  secondaryMaterialColor.fromCrayon(in->secondaryMaterialColor);
  ambientColor.fromCrayon(in->ambientColor);
  atmosphereDiffuseColor.fromCrayon(in->atmosphereDiffuseColor);
  atmosphereSpecularColor.fromCrayon(in->atmosphereSpecularColor);
  darkDiffuseColor.fromCrayon(in->darkDiffuseColor);
  darkSpecularColor.fromCrayon(in->darkSpecularColor);
  oceanColor.fromCrayon(in->oceanColor);
 }
};



// --------------------------------------------------------------

ONE(RingedJovian2Profile,{
 noiseTex=library.Load("data/images/textures/grain1.png",tllto_Tiling);
 cameraPos.Set(32.0,1.5,1); //vec3( 16.0*cos(0.2+0.5*.1*time*1.5), 1.5, 16.0*sin(0.1+0.5*0.11*time*1.5) ); }\n")
 lightPos.Set(0.3,0.9,0.7);
 bumpiness=-21.0;
 color1.Double(0.8,0.6,0.0,1.0);
 color2.Double(0.7,0.7,1.0,1.0);
 albedoColor.Double(0.0,0.5,0.8,1.0);
 ringColor1.Double(1.0,0.3,0.0,1.0);
 ringColor2.Double(1.0,0.2,0.0,1.0);
 centerGlowColor.Double(1,0.8,0.5,1.0);
 amb=2.5;
 diffuseColor.Double(0.8,0.8,0.8,1.0);
 ringParams.Set(1.2,0.9,573.62); ringParams.w=0.4;
 ringRadRatio=2.1;
 bandFreq=8.0;
 roll=-200.0;
})
 Zdouble time,bumpiness,amb,ringRadRatio,bandFreq,rotation,roll;
 Zp<GLImage> noiseTex;
 Cartesiand cameraPos,lightPos,ringParams;
 Crayon color1,color2,albedoColor,ringColor1,ringColor2,centerGlowColor,diffuseColor;
 void Randomize(int seed) {
  color1.random(seed + 12);
  color2.random(seed + 142);
  albedoColor.random(seed + 15);
  ringColor1.random(seed + 173);
  ringColor2.random(seed + 21345);
  centerGlowColor.random(seed + 123);
  diffuseColor.random(seed);
  bumpiness = iratiod(upto(100, seed + 123456),100)*100.0 - 50.0;
  //bandFreq = iratiod(upto(100, seed + 23456), 100)*30.0+1.0;
  //ringRadRatio = iratiod(upto(100, seed + 3456), 100)*5.0 + 1.0;
 }
 BINARY_IN({
  B_IN_NUM(time)
  B_IN_NUM(bumpiness)
  B_IN_NUM(amb)
  B_IN_NUM(ringRadRatio)
  B_IN_NUM(bandFreq)
  B_IN_NUM(rotation)
  B_IN_NUM(roll)
  B_IN_TILE(noiseTex)
  B_IN_SSUB(cameraPos)
  B_IN_SSUB(lightPos)
  B_IN_SSUB(ringParams)
  B_IN_TINT(color1)
  B_IN_TINT(color2)
  B_IN_TINT(albedoColor)
  B_IN_TINT(ringColor1)
  B_IN_TINT(ringColor2)
  B_IN_TINT(centerGlowColor)
  B_IN_TINT(diffuseColor)
 })
 BINARY_OUT({
  B_OUT_NUM(time)
  B_OUT_NUM(bumpiness)
  B_OUT_NUM(amb)
  B_OUT_NUM(ringRadRatio)
  B_OUT_NUM(bandFreq)
  B_OUT_NUM(rotation)
  B_OUT_NUM(roll)
  B_OUT_TILE(noiseTex)
  B_OUT_SSUB(cameraPos)
  B_OUT_SSUB(lightPos)
  B_OUT_SSUB(ringParams)
  B_OUT_TINT(color1)
  B_OUT_TINT(color2)
  B_OUT_TINT(albedoColor)
  B_OUT_TINT(ringColor1)
  B_OUT_TINT(ringColor2)
  B_OUT_TINT(centerGlowColor)
  B_OUT_TINT(diffuseColor)
 })
 KEYWORDS({
  KEYWORD("time",{time=w;})
  else KEYWORD("bumpiness",{bumpiness=w;})
  else KEYWORD("ringRadRatio",{ringRadRatio=w;})
  else KEYWORD("amb",{amb=w;})
  else KEYWORD("roll",{roll=w;})
  else KEYWORD("rotation",{rotation=w;})
  else KEYWORD("bandFreq",{bandFreq=w;})
  else TILEWORD("noiseTex",noiseTex)
  else SUBWORD("cameraPos",cameraPos)
  else SUBWORD("lightPos",lightPos)
  else SUBWORD("ringParams",ringParams)
  else TINTWORD("color1",color1)
  else TINTWORD("color2",color2)
  else TINTWORD("albedoColor",albedoColor)
  else TINTWORD("ringColor1",ringColor1)
  else TINTWORD("ringColor2",ringColor2)
  else TINTWORD("centerGlowColor",centerGlowColor)
  else TINTWORD("diffuseColor",diffuseColor)
 })
 WORDKEYS({
  NUMBKEY("time",time)
  NUMBKEY("bumpiness",bumpiness)
  NUMBKEY("amb",amb)
  NUMBKEY("rotation",rotation)
  NUMBKEY("roll",roll)
  NUMBKEY("ringRadRatio",ringRadRatio)
  NUMBKEY("bandFreq",bandFreq)
  IMGKEY("noiseTex",noiseTex)
  SSUBKEY("cameraPos",cameraPos)
  SSUBKEY("lightPos",lightPos)
  SSUBKEY("ringParams",ringParams)
  TINTKEY("color1",color1)
  TINTKEY("color2",color2)
  TINTKEY("albedoColor",albedoColor)
  TINTKEY("ringColor1",ringColor1)
  TINTKEY("ringColor2",ringColor2)
  TINTKEY("centerGlowColor",centerGlowColor)
  TINTKEY("diffuseColor",diffuseColor)
 })
MANY(RingedJovian2Profile,RingedJovian2ProfileHandle,RingedJovian2ProfileHandles,"RingedJovian2",RingedJovian2Profiles,{})
 KEYWORDSGroup("RingedJovian2",RingedJovian2Profile);
 WORDKEYSGroup("RingedJovian2",RingedJovian2Profile);
 BINARY_INOUT(RingedJovian2Profile)
DONE(RingedJovian2Profile);

class RingedJovian2Shader : public GLSLShader {
public:
 Zdouble time,bumpiness,amb,ringRadRatio,bandFreq,rotation,roll;
 Zp<GLuint> noiseTex;
 Cartesiand cameraPos,lightPos,ringParams;
 Crayon color1,color2,albedoColor,ringColor1,ringColor2,centerGlowColor,diffuseColor;
 GLuint Utime,Ubumpiness,Uamb,UringRadRatio,UnoiseTex,UcameraPos,UlightPos,UringParams,Ucolor1,Ucolor2,UalbedoColor,UringColor1,UringColor2,UcenterGlowColor,UdiffuseColor,UbandFreq,Urotation,Uroll;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Utime=glGetUniformLocation(program, "time");
  Ubumpiness=glGetUniformLocation(program, "bumpiness");
  Uamb=glGetUniformLocation(program, "amb");
  UringRadRatio=glGetUniformLocation(program, "ringRadRatio");
  UbandFreq=glGetUniformLocation(program, "bandFreq");
  Urotation=glGetUniformLocation(program, "rotation");
  UcameraPos=glGetUniformLocation(program, "cameraPos");
  UlightPos=glGetUniformLocation(program, "lightPos");
  UringParams=glGetUniformLocation(program, "ringParams");
  Ucolor1=glGetUniformLocation(program, "color1");
  Ucolor2=glGetUniformLocation(program, "color2");
  UalbedoColor=glGetUniformLocation(program, "albedoColor");
  UringColor1=glGetUniformLocation(program, "ringColor1");
  UringColor2=glGetUniformLocation(program, "ringColor2");
  UcenterGlowColor=glGetUniformLocation(program, "centerGlowColor");
  UdiffuseColor=glGetUniformLocation(program, "diffuseColor");
  Uroll=glGetUniformLocation(program,"roll");
  UnoiseTex=glGetUniformLocation(program, "noiseTex");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform3f(UcameraPos,(GLfloat)cameraPos.x,(GLfloat)cameraPos.y,(GLfloat)cameraPos.z);
  glUniform3f(UlightPos,(GLfloat)lightPos.x,(GLfloat)lightPos.y,(GLfloat)lightPos.z);
  glUniform4f(UringParams,(GLfloat)ringParams.x,(GLfloat)ringParams.y,(GLfloat)ringParams.z,(GLfloat)ringParams.w);
  glUniform3f(Ucolor1,(GLfloat)color1.rf,(GLfloat)color1.gf,(GLfloat)color1.bf);
  glUniform3f(Ucolor2,(GLfloat)color2.rf,(GLfloat)color2.gf,(GLfloat)color2.bf);
  glUniform3f(UalbedoColor,(GLfloat)albedoColor.rf,(GLfloat)albedoColor.gf,(GLfloat)albedoColor.bf);
  glUniform3f(UringColor1,(GLfloat)ringColor1.rf,(GLfloat)ringColor1.gf,(GLfloat)ringColor1.bf);
  glUniform3f(UringColor2,(GLfloat)ringColor2.rf,(GLfloat)ringColor2.gf,(GLfloat)ringColor2.bf);
  glUniform3f(UcenterGlowColor,(GLfloat)centerGlowColor.rf,(GLfloat)centerGlowColor.gf,(GLfloat)centerGlowColor.bf);
  glUniform3f(UdiffuseColor,(GLfloat)diffuseColor.rf,(GLfloat)diffuseColor.gf,(GLfloat)diffuseColor.bf);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(Ubumpiness,(GLfloat)bumpiness);
  glUniform1f(Uamb,(GLfloat)amb);
  glUniform1f(UringRadRatio,(GLfloat)ringRadRatio);
  glUniform1f(UbandFreq,(GLfloat)bandFreq);
  glUniform1f(Urotation,(GLfloat)rotation);
  glUniform1f(Uroll,(GLfloat)roll);
  glEnable(GL_TEXTURE_2D);
  glUniform1i(UnoiseTex, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *noiseTex);
  
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
  glBindTexture(GL_TEXTURE_2D,0);
  glDisable(GL_TEXTURE_2D);
  glUseProgram(0);
 }
 void Set( RingedJovian2Profile *in ) {
  time=in->time;
  bumpiness=in->bumpiness;
  amb=in->amb;
  ringRadRatio=in->ringRadRatio;
  bandFreq=in->bandFreq;
  rotation=in->rotation;
  roll=in->roll;
  cameraPos.Set(&in->cameraPos);
  lightPos.Set(&in->lightPos);
  ringParams.Set(&in->ringParams);
  color1.fromCrayon(in->color1);
  color2.fromCrayon(in->color2);
  albedoColor.fromCrayon(in->albedoColor);
  ringColor1.fromCrayon(in->ringColor1);
  ringColor2.fromCrayon(in->ringColor2);
  centerGlowColor.fromCrayon(in->centerGlowColor);
  diffuseColor.fromCrayon(in->diffuseColor);
  noiseTex=&in->noiseTex->texture;
 }
};



// ----------------------------------------------

ONE(LunarPlanetProfile,{
 tex=library.Load("data/images/textures/tile37.png",tllto_Tiling);
 offSet.Set(0.6,0.0);
 texScale.Set(1.0,1.0);
 origin.Set(0.0,0.0,-2.5);
 baseColor.Double(0.7,0.8,1.0,1.0);
 multiplyColor.Double(0.9,0.55,0.9,1.0);
 heightFactor=0.6;
 scaleFactor=1.0;
 rotationSpeed=-0.05;
 zScale=3.5;
 smoothness=0.01;
 ambientColor.Double(0.2, 0.0, 0.05, 1.0);
 lightPos.Set(1.0, 0.0, -1.0);
})
 Zdouble time,heightFactor,scaleFactor,rotationSpeed,zScale,smoothness;
 Cartesiand position,offSet,texScale,origin,lightPos;
 Crayon baseColor,multiplyColor,ambientColor;
 Zp<GLImage> tex;
 void Randomize(int seed) {
  baseColor.random(seed);
  multiplyColor.random(seed + 353);
  ambientColor.random(seed + 123456);
  ambientColor.MultiplyClampRGB(0.2);
  heightFactor = iratiod(upto(100, seed), 100);
  rotationSpeed = -0.05 * (iratiod(upto(100, seed + 123), 100) - 0.5);
  smoothness = 0.02  * (iratiod(upto(100, seed + 1234), 100) - 0.5);
 }
 BINARY_IN({
  B_IN_NUM(time)
  B_IN_NUM(heightFactor)
  B_IN_NUM(scaleFactor)
  B_IN_NUM(rotationSpeed)
  B_IN_NUM(zScale)
  B_IN_NUM(smoothness)
  B_IN_SSUB(position)
  B_IN_SSUB(offSet)
  B_IN_SSUB(texScale)
  B_IN_SSUB(origin)
  B_IN_SSUB(lightPos)
  B_IN_TINT(baseColor)
  B_IN_TINT(multiplyColor)
  B_IN_TINT(ambientColor)
  B_IN_IMAGE(tex)
 })
 BINARY_OUT({
  B_OUT_NUM(time)
  B_OUT_NUM(heightFactor)
  B_OUT_NUM(scaleFactor)
  B_OUT_NUM(rotationSpeed)
  B_OUT_NUM(zScale)
  B_OUT_NUM(smoothness)
  B_OUT_SSUB(position)
  B_OUT_SSUB(offSet)
  B_OUT_SSUB(texScale)
  B_OUT_SSUB(origin)
  B_OUT_SSUB(lightPos)
  B_OUT_TINT(baseColor)
  B_OUT_TINT(multiplyColor)
  B_OUT_TINT(ambientColor)
  B_OUT_IMAGE(tex)
 })
 KEYWORDS({
  KEYWORD("time",{time=w;})
  else KEYWORD("heightFactor",{heightFactor=w;})
  else KEYWORD("scaleFactor",{scaleFactor=w;})
  else KEYWORD("rotationSpeed",{rotationSpeed=w;})
  else KEYWORD("zScale",{zScale=w;})
  else KEYWORD("smoothness",{smoothness=w;})
  else SUBWORD("position",position)
  else SUBWORD("offSet",offSet)
  else SUBWORD("texScale",texScale)
  else SUBWORD("origin",origin)
  else SUBWORD("lightPos",lightPos)
  else TINTWORD("baseColor",baseColor)
  else TINTWORD("multiplyColor",multiplyColor)
  else TINTWORD("ambientColor",ambientColor)
  else TILEWORD("tex",tex)
 })
 WORDKEYS({
  NUMBKEY("time",time)
  NUMBKEY("heightFactor",heightFactor)
  NUMBKEY("scaleFactor",scaleFactor)
  NUMBKEY("rotationSpeed",rotationSpeed)
  NUMBKEY("zScale",zScale)
  NUMBKEY("smoothness",smoothness)
  SSUBKEY("position",position)
  SSUBKEY("offSet",offSet)
  SSUBKEY("texScale",texScale)
  SSUBKEY("origin",origin)
  SSUBKEY("lightPos",lightPos)
  TINTKEY("baseColor",baseColor)
  TINTKEY("multiplyColor",multiplyColor)
  TINTKEY("ambientColor",ambientColor)
  IMGKEY("tex",tex)
 })
MANY(LunarPlanetProfile,LunarPlanetProfileHandle,LunarPlanetProfileHandles,"LunarPlanet",LunarPlanetProfiles,{})
 KEYWORDSGroup("LunarPlanet",LunarPlanetProfile);
 WORDKEYSGroup("LunarPlanet",LunarPlanetProfile);
 BINARY_INOUT(LunarPlanetProfile)
DONE(LunarPlanetProfile);

class LunarPlanetShader : public GLSLShader {
public:
 Zdouble time,heightFactor,scaleFactor,rotationSpeed,zScale,smoothness;
 Cartesiand position,offSet,texScale,origin,lightPos;
 Crayon baseColor,multiplyColor,ambientColor;
 Zp<GLuint> tex;
 GLuint Utime,UheightFactor,UscaleFactor,UrotationSpeed,UzScale,Usmoothness,Upos,UoffSet,UtexScale,Uorigin,UlightPos,UbaseColor,UmultiplyColor,UambientColor,Utex;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Utime=glGetUniformLocation(program, "time");
  UheightFactor=glGetUniformLocation(program, "heightFactor");
  UscaleFactor=glGetUniformLocation(program, "scaleFactor");
  UrotationSpeed=glGetUniformLocation(program, "rotationSpeed");
  UzScale=glGetUniformLocation(program, "zScale");
  Usmoothness=glGetUniformLocation(program, "smoothness");
  Upos=glGetUniformLocation(program, "position");
  UoffSet=glGetUniformLocation(program, "offSet");
  UtexScale=glGetUniformLocation(program, "texScale");
  Uorigin=glGetUniformLocation(program, "origin");
  UlightPos=glGetUniformLocation(program, "lightPos");
  UbaseColor=glGetUniformLocation(program, "baseColor");
  UmultiplyColor=glGetUniformLocation(program, "multiplyColor");
  UambientColor=glGetUniformLocation(program, "ambientColor");
  Utex=glGetUniformLocation(program, "tex");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Upos,(GLfloat)position.x,(GLfloat)position.y);
  glUniform2f(UoffSet,(GLfloat)offSet.x,(GLfloat)offSet.y);
  glUniform2f(UtexScale,(GLfloat)texScale.x,(GLfloat)texScale.y);
  glUniform3f(Uorigin,(GLfloat)origin.x,(GLfloat)origin.y,(GLfloat)origin.z);
  glUniform3f(UlightPos,(GLfloat)lightPos.x,(GLfloat)lightPos.y,(GLfloat)lightPos.z);
  glUniform4f(UbaseColor,(GLfloat)baseColor.rf,(GLfloat)baseColor.gf,(GLfloat)baseColor.bf,(GLfloat)baseColor.af);
  glUniform4f(UmultiplyColor,(GLfloat)multiplyColor.rf,(GLfloat)multiplyColor.gf,(GLfloat)multiplyColor.bf,(GLfloat)multiplyColor.af);
  glUniform4f(UambientColor,(GLfloat)ambientColor.rf,(GLfloat)ambientColor.gf,(GLfloat)ambientColor.bf,(GLfloat)ambientColor.af);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(UheightFactor,(GLfloat)heightFactor);
  glUniform1f(UscaleFactor,(GLfloat)scaleFactor);
  glUniform1f(UrotationSpeed,(GLfloat)rotationSpeed);
  glUniform1f(UzScale,(GLfloat)zScale);
  glUniform1f(Usmoothness,(GLfloat)smoothness);
  glUniform1i(Utex, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, (*tex.pointer));
  
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
  glUseProgram(0);
 }
 void Set( LunarPlanetProfile *in ) {
  time=in->time;
  heightFactor=in->heightFactor;
  scaleFactor=in->scaleFactor;
  rotationSpeed=in->rotationSpeed;
  zScale=in->zScale;
  smoothness=in->smoothness;
  position.Set(&in->position);
  offSet.Set(&in->offSet);
  texScale.Set(&in->texScale);
  origin.Set(&in->origin);
  lightPos.Set(&in->lightPos);
  baseColor.fromCrayon(in->baseColor);
  multiplyColor.fromCrayon(in->multiplyColor);
  ambientColor.fromCrayon(in->ambientColor);
  tex=&in->tex->texture;
 }
 void Render( FBOColor *out, LunarPlanetProfile *in );
};



// ----------------------------------------------------

ONE(FlaringStarProfile,{
 scaleStar=1.5;
 scaleClamp=0.5;
 invCircleWidth=0.25;
 rays_innerR=0.96;
 rays_outerR=2.0;
 position.Set(0.5,0.5);
 rotation.Set(0.1,0.0);
 convectiveColor.Double(0.5,0.0,0.0,1.0);
 surfaceStaticColor.Double(0.0,1.0,1.0,1.0);
 surfaceStaticColor2.Double(1.0,1.0,1.0,1.0);
 surfaceColor1.Double(1.0,0.0,0.0,1.0);
 surfaceColor2.Double(1.0,1.0,0.0,1.0);
 tintColor.Double(1.0,1.0,1.0,1.0);
 coronaColor.Double(1.0,1.0,1.0,1.0);
 plasmaColor.Double(1.0,0.6,0.1,1.0);
 windColor1.Double(1.0,0.0,0.0,1.0);
 windColor2.Double(0.0,0.0,1.0,1.0);
 windColor3.Double(0.9,1.0,0.1,1.0);
})
 Zdouble time,scaleStar,scaleClamp,invCircleWidth,rays_innerR,rays_outerR;
 Cartesiand position,rotation;
 Crayon convectiveColor,surfaceStaticColor,surfaceStaticColor2,surfaceColor1,surfaceColor2,tintColor,coronaColor,plasmaColor,windColor1,windColor2,windColor3;
 void Randomize(int seed) {
  convectiveColor.random(seed+123);
  surfaceStaticColor.random(seed+321);
  surfaceStaticColor2.random(seed+213);
  surfaceColor1.random(seed+212);
  surfaceColor2.random(seed+234);
  tintColor.random(seed+532);
  coronaColor.random(seed+459);
  plasmaColor.random(seed+301);
  windColor1.random(seed+202);
  windColor2.random(seed+101);
  windColor3.random(seed);
  //convectiveColor.Lerp(iratiod(upto(1000,seed+1),1000),convectiveColor,crayons.jam(0, 0.5f, 0.0f, 0.0f, 1.0f));
  //surfaceStaticColor.Lerp(iratiod(upto(1000, seed + 2), 1000), surfaceStaticColor, crayons.jam(0, 0.0f, 1.0f, 1.0f, 1.0f));
  //surfaceStaticColor2.Lerp(iratiod(upto(1000, seed + 3), 1000), surfaceStaticColor2, crayons.jam(0, 1.0f, 1.0f, 1.0f, 1.0f));
  //surfaceColor1.Lerp(iratiod(upto(1000, seed + 4), 1000), surfaceColor1, crayons.jam(0, 1.0f, 1.0f, 1.0f, 1.0f));
  //surfaceColor2.Lerp(iratiod(upto(1000, seed + 5), 1000), surfaceColor2, crayons.jam(0, 0.5f, 0.5f, 0.5f, 1.0f));
  //tintColor.Lerp(iratiod(upto(1000, seed + 6), 1000), tintColor, crayons.Pick(alabaster));
  //coronaColor.Lerp(iratiod(upto(1000, seed + 17), 1000) - 0.5 + 0.5, coronaColor, crayons.Pick(alabaster));
  //plasmaColor.Lerp(iratiod(upto(1000, seed + 18), 1000) - 0.5 + 0.5, plasmaColor, crayons.Pick(alabaster));
 }
 BINARY_IN({
  B_IN_NUM(time)
  B_IN_NUM(scaleStar)
  B_IN_NUM(scaleClamp)
  B_IN_NUM(invCircleWidth)
  B_IN_NUM(rays_innerR)
  B_IN_NUM(rays_outerR)
  B_IN_SSUB(position)
  B_IN_SSUB(rotation)
  B_IN_TINT(convectiveColor)
  B_IN_TINT(surfaceStaticColor)
  B_IN_TINT(surfaceStaticColor2)
  B_IN_TINT(surfaceColor1)
  B_IN_TINT(surfaceColor2)
  B_IN_TINT(tintColor)
  B_IN_TINT(coronaColor)
  B_IN_TINT(plasmaColor)
  B_IN_TINT(windColor1)
  B_IN_TINT(windColor2)
  B_IN_TINT(windColor3)
 })
 BINARY_OUT({
  B_OUT_NUM(time)
  B_OUT_NUM(scaleStar)
  B_OUT_NUM(scaleClamp)
  B_OUT_NUM(invCircleWidth)
  B_OUT_NUM(rays_innerR)
  B_OUT_NUM(rays_outerR)
  B_OUT_SSUB(position)
  B_OUT_SSUB(rotation)
  B_OUT_TINT(convectiveColor)
  B_OUT_TINT(surfaceStaticColor)
  B_OUT_TINT(surfaceStaticColor2)
  B_OUT_TINT(surfaceColor1)
  B_OUT_TINT(surfaceColor2)
  B_OUT_TINT(tintColor)
  B_OUT_TINT(coronaColor)
  B_OUT_TINT(plasmaColor)
  B_OUT_TINT(windColor1)
  B_OUT_TINT(windColor2)
  B_OUT_TINT(windColor3)
 })
 KEYWORDS({
  KEYWORD("time",{time=w;})
  else KEYWORD("scaleStar",{scaleStar=w;})
  else KEYWORD("scaleClamp",{scaleClamp=w;})
  else KEYWORD("invCircleWidth",{invCircleWidth=w;})
  else KEYWORD("rays_innerR",{rays_innerR=w;})
  else KEYWORD("rays_outerR",{rays_outerR=w;})
  else SUBWORD("position",position)
  else SUBWORD("rotation",rotation)
  else TINTWORD("convectiveColor",convectiveColor)
  else TINTWORD("surfaceStaticColor",surfaceStaticColor)
  else TINTWORD("surfaceStaticColor2",surfaceStaticColor2)
  else TINTWORD("surfaceColor1",surfaceColor1)
  else TINTWORD("surfaceColor2",surfaceColor2)
  else TINTWORD("tintColor",tintColor)
  else TINTWORD("coronaColor",coronaColor)
  else TINTWORD("plasmaColor",plasmaColor)
  else TINTWORD("windColor1",windColor1)
  else TINTWORD("windColor2",windColor2)
  else TINTWORD("windColor3",windColor3)
 })
 WORDKEYS({
  NUMBKEY("time",time)
  NUMBKEY("scaleStar",scaleStar)
  NUMBKEY("scaleClamp",scaleClamp)
  NUMBKEY("invCircleWidth",invCircleWidth)
  NUMBKEY("rays_innerR",rays_innerR)
  NUMBKEY("rays_outerR",rays_outerR)
  SSUBKEY("position",position)
  SSUBKEY("rotation",rotation)
  TINTKEY("convectiveColor",convectiveColor)
  TINTKEY("surfaceStaticColor",surfaceStaticColor)
  TINTKEY("surfaceStaticColor2",surfaceStaticColor2)
  TINTKEY("surfaceColor1",surfaceColor1)
  TINTKEY("surfaceColor2",surfaceColor2)
  TINTKEY("tintColor",tintColor)
  TINTKEY("coronaColor",coronaColor)
  TINTKEY("plasmaColor",plasmaColor)
  TINTKEY("windColor1",windColor1)
  TINTKEY("windColor2",windColor2)
  TINTKEY("windColor3",windColor3)
 })
MANY(FlaringStarProfile,FlaringStarProfileHandle,FlaringStarProfileHandles,"FlaringStar",FlaringStarProfiles,{})
 KEYWORDSGroup("FlaringStar",FlaringStarProfile);
 WORDKEYSGroup("FlaringStar",FlaringStarProfile);
 BINARY_INOUT(FlaringStarProfile);
DONE(FlaringStarProfile);

class FlaringStarShader : public GLSLShader {
public:
 Zdouble time,scaleStar,scaleClamp,invCircleWidth,rays_innerR,rays_outerR;
 Cartesiand position,rotation;
 Crayon convectiveColor,surfaceStaticColor,surfaceStaticColor2,surfaceColor1,surfaceColor2,tintColor,coronaColor,plasmaColor,windColor1,windColor2,windColor3;
 GLuint Utime,UscaleStar,UscaleClamp,UinvCircleWidth,Urays_innerR,Urays_outerR,Upos,Urotation,UconvectiveColor,UsurfaceStaticColor,UsurfaceStaticColor2,UsurfaceColor1,UsurfaceColor2,UtintColor,UcoronaColor,UplasmaColor,UwindColor1,UwindColor2,UwindColor3;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Utime=glGetUniformLocation(program, "time");
  UscaleStar=glGetUniformLocation(program, "scaleStar");
  UscaleClamp=glGetUniformLocation(program, "scaleClamp");
  UinvCircleWidth=glGetUniformLocation(program, "invCircleWidth");
  Urays_innerR=glGetUniformLocation(program, "rays_innerR");
  Urays_outerR=glGetUniformLocation(program, "rays_outerR");
  Upos=glGetUniformLocation(program, "pos");
  Urotation=glGetUniformLocation(program, "rotation");
  UconvectiveColor=glGetUniformLocation(program, "convectiveColor");
  UsurfaceStaticColor=glGetUniformLocation(program, "surfaceStaticColor");
  UsurfaceStaticColor2=glGetUniformLocation(program, "surfaceStaticColor2");
  UsurfaceColor1=glGetUniformLocation(program, "surfaceColor1");
  UsurfaceColor2=glGetUniformLocation(program, "surfaceColor2");
  UtintColor=glGetUniformLocation(program, "tintColor");
  UcoronaColor=glGetUniformLocation(program, "coronaColor");
  UplasmaColor=glGetUniformLocation(program, "plasmaColor");
  UwindColor1=glGetUniformLocation(program, "windColor1");
  UwindColor2=glGetUniformLocation(program, "windColor2");
  UwindColor3=glGetUniformLocation(program, "windColor3");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Upos,(GLfloat)position.x,(GLfloat)position.y);
  glUniform2f(Urotation,(GLfloat)rotation.x,(GLfloat)rotation.y);
  glUniform3f(UconvectiveColor,(GLfloat)convectiveColor.rf,(GLfloat)convectiveColor.gf,(GLfloat)convectiveColor.bf);
  glUniform3f(UsurfaceStaticColor,(GLfloat)surfaceStaticColor.rf,(GLfloat)surfaceStaticColor.gf,(GLfloat)surfaceStaticColor.bf);
  glUniform3f(UsurfaceStaticColor2,(GLfloat)surfaceStaticColor2.rf,(GLfloat)surfaceStaticColor2.gf,(GLfloat)surfaceStaticColor2.bf);
  glUniform3f(UsurfaceColor1,(GLfloat)surfaceColor1.rf,(GLfloat)surfaceColor1.gf,(GLfloat)surfaceColor1.bf);
  glUniform3f(UsurfaceColor2,(GLfloat)surfaceColor2.rf,(GLfloat)surfaceColor2.gf,(GLfloat)surfaceColor2.bf);
  glUniform3f(UtintColor,(GLfloat)tintColor.rf,(GLfloat)tintColor.gf,(GLfloat)tintColor.bf);
  glUniform3f(UcoronaColor,(GLfloat)coronaColor.rf,(GLfloat)coronaColor.gf,(GLfloat)coronaColor.bf);
  glUniform3f(UplasmaColor,(GLfloat)plasmaColor.rf,(GLfloat)plasmaColor.gf,(GLfloat)plasmaColor.bf);
  glUniform3f(UwindColor1,(GLfloat)windColor1.rf,(GLfloat)windColor1.gf,(GLfloat)windColor1.bf);
  glUniform3f(UwindColor2,(GLfloat)windColor2.rf,(GLfloat)windColor2.gf,(GLfloat)windColor2.bf);
  glUniform3f(UwindColor3,(GLfloat)windColor3.rf,(GLfloat)windColor3.gf,(GLfloat)windColor3.bf);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(UscaleStar,(GLfloat)scaleStar);
  glUniform1f(UscaleClamp,(GLfloat)scaleClamp);
  glUniform1f(UinvCircleWidth,(GLfloat)invCircleWidth);
  glUniform1f(Urays_innerR,(GLfloat)rays_innerR);
  glUniform1f(Urays_outerR,(GLfloat)rays_outerR);
  
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
  glUseProgram(0);
 }
 void Set( FlaringStarProfile *in ) {
  time=in->time;
  scaleStar=in->scaleStar;
  scaleClamp=in->scaleClamp;
  invCircleWidth=in->invCircleWidth;
  rays_innerR=in->rays_innerR;
  rays_outerR=in->rays_outerR;
  position.Set(&in->position);
  rotation.Set(&in->rotation);
  convectiveColor.fromCrayon(in->convectiveColor);
  surfaceStaticColor.fromCrayon(in->surfaceStaticColor);
  surfaceStaticColor2.fromCrayon(in->surfaceStaticColor2);
  surfaceColor1.fromCrayon(in->surfaceColor1);
  surfaceColor2.fromCrayon(in->surfaceColor2);
  tintColor.fromCrayon(in->tintColor);
  coronaColor.fromCrayon(in->coronaColor);
  plasmaColor.fromCrayon(in->plasmaColor);
  windColor1.fromCrayon(in->windColor1);
  windColor2.fromCrayon(in->windColor2);
  windColor3.fromCrayon(in->windColor3);
 }
 void Render( FBOColor *out, FlaringStarProfile *in );
};



// ----------------------------------------------------



ONE(ElectronCloudsProfile,{
 iterations=4;
 formuparam2=0.89;
 volsteps=10;
 stepsize=0.190;
 zoom=3.9;
 tile=0.450;
 speed2=0.001;
 brightness=0.2;
 darkmatter=0.4;
 distfading=0.56;
 saturation=0.4;
 transverseSpeed=0.001;
 cloud=0.2;
 col1.Pick(black);
 col2.Pick(black);
})
 Zint iterations,volsteps;
 Zdouble formuparam2,stepsize,zoom,tile,speed2,brightness,darkmatter,distfading,saturation,transverseSpeed,cloud,time;
 Crayon col1,col2;
 Cartesiand pos2d,move2d;
 void Randomize(int seed) {
  col1.random(seed);
  col2.random(seed + 1);
  darkmatter = 0.4 * iratiod(upto(200, seed + 3), 100);
  distfading = iratiod(upto(200, seed + 3), 200);
  saturation = 0.4 * iratiod(upto(200, seed + 3), 100);
 }
 BINARY_IN({
  B_IN_NUM(iterations)
  B_IN_NUM(volsteps)
  B_IN_NUM(formuparam2)
  B_IN_NUM(stepsize)
  B_IN_NUM(zoom)
  B_IN_NUM(tile)
  B_IN_NUM(speed2)
  B_IN_NUM(brightness)
  B_IN_NUM(darkmatter)
  B_IN_NUM(distfading)
  B_IN_NUM(saturation)
  B_IN_NUM(transverseSpeed)
  B_IN_NUM(cloud)
  B_IN_NUM(time)
  B_IN_TINT(col1)
  B_IN_TINT(col2)
  B_IN_SSUB(pos2d)
  B_IN_SSUB(move2d)
 })
 BINARY_OUT({
  B_OUT_NUM(iterations)
  B_OUT_NUM(volsteps)
  B_OUT_NUM(formuparam2)
  B_OUT_NUM(stepsize)
  B_OUT_NUM(zoom)
  B_OUT_NUM(tile)
  B_OUT_NUM(speed2)
  B_OUT_NUM(brightness)
  B_OUT_NUM(darkmatter)
  B_OUT_NUM(distfading)
  B_OUT_NUM(saturation)
  B_OUT_NUM(transverseSpeed)
  B_OUT_NUM(cloud)
  B_OUT_NUM(time)
  B_OUT_TINT(col1)
  B_OUT_TINT(col2)
  B_OUT_SSUB(pos2d)
  B_OUT_SSUB(move2d)
 })
 void Set( ElectronCloudsProfile *in ) {
  iterations=in->iterations;
  formuparam2=in->formuparam2;
  volsteps=in->volsteps;
  stepsize=in->stepsize;
  zoom=in->zoom;
  tile=in->tile;
  speed2=in->speed2;
  brightness=in->brightness;
  darkmatter=in->darkmatter;
  distfading=in->distfading;
  saturation=in->saturation;
  transverseSpeed=in->transverseSpeed;
  cloud=in->cloud;
  time=in->time;
  col1.fromCrayon(in->col1);
  col2.fromCrayon(in->col2);
  pos2d.Set(&in->pos2d);
  move2d.Set(&in->move2d);
 }
 KEYWORDS({
  KEYWORD("iterations",{iterations=w;})
  else KEYWORD("formuparam2",{formuparam2=w;})
  else KEYWORD("volsteps",{volsteps=w;})
  else KEYWORD("stepsize",{stepsize=w;})
  else KEYWORD("zoom",{zoom=w;})
  else KEYWORD("tile",{tile=w;})
  else KEYWORD("speed2",{speed2=w;})
  else KEYWORD("brightness",{brightness=w;})
  else KEYWORD("darkmatter",{darkmatter=w;})
  else KEYWORD("distfading",{distfading=w;})
  else KEYWORD("saturation",{saturation=w;})
  else KEYWORD("transverseSpeed",{transverseSpeed=w;})
  else KEYWORD("cloud",{cloud=w;})
  else KEYWORD("time",{time=w;})
  else TINTWORD("col1",col1)
  else TINTWORD("col2",col2)
  else SUBWORD("pos2d",pos2d)
  else SUBWORD("move2d",move2d)
 })
 WORDKEYS({
  NUMBKEY("iterations",iterations)
  NUMBKEY("formuparam2",formuparam2)
  NUMBKEY("volsteps",volsteps)
  NUMBKEY("stepsize",stepsize)
  NUMBKEY("zoom",zoom)
  NUMBKEY("tile",tile)
  NUMBKEY("speed2",speed2)
  NUMBKEY("brightness",brightness)
  NUMBKEY("darkmatter",darkmatter)
  NUMBKEY("distfading",distfading)
  NUMBKEY("saturation",saturation)
  NUMBKEY("transverseSpeed",transverseSpeed)
  NUMBKEY("cloud",cloud)
  NUMBKEY("time",time)
  TINTKEY("col1",col1)
  TINTKEY("col2",col2)
  SSUBKEY("pos2d",pos2d)
  SSUBKEY("move2d",move2d)
 })
MANY(ElectronCloudsProfile,ElectronCloudsProfileHandle,ElectronCloudsProfileHandles,"ElectronClouds",ElectronCloudsProfiles,{})
 KEYWORDSGroup("ElectronClouds",ElectronCloudsProfile);
 WORDKEYSGroup("ElectronClouds",ElectronCloudsProfile);
DONE(ElectronCloudsProfile);

class ElectronCloudsShader : public GLSLShader {
public:
 Zint iterations,volsteps;
 Zdouble formuparam2,stepsize,zoom,tile,speed2,brightness,darkmatter,distfading,saturation,transverseSpeed,cloud,time;
 Crayon col1,col2;
 Cartesiand move2d,pos2d;
 GLuint Umove2d,Uiterations,Uformuparam2,Uvolsteps,Ustepsize,Uzoom,Utile,Uspeed2,Ubrightness,Udarkmatter,Udistfading,Usaturation,UtransverseSpeed,Ucloud,Utime,Ucol1,Ucol2,Upos2d;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Uiterations=glGetUniformLocation(program, "iterations");
  Uformuparam2=glGetUniformLocation(program, "formuparam2");
  Uvolsteps=glGetUniformLocation(program, "volsteps");
  Ustepsize=glGetUniformLocation(program, "stepsize");
  Uzoom=glGetUniformLocation(program, "zoom");
  Utile=glGetUniformLocation(program, "tile");
  Uspeed2=glGetUniformLocation(program, "speed2");
  Ubrightness=glGetUniformLocation(program, "brightness");
  Udarkmatter=glGetUniformLocation(program, "darkmatter");
  Udistfading=glGetUniformLocation(program, "distfading");
  Usaturation=glGetUniformLocation(program, "saturation");
  UtransverseSpeed=glGetUniformLocation(program, "transverseSpeed");
  Ucloud=glGetUniformLocation(program, "cloud");
  Utime=glGetUniformLocation(program, "time");
  Ucol1=glGetUniformLocation(program, "col1");
  Ucol2=glGetUniformLocation(program, "col2");
  Upos2d=glGetUniformLocation(program, "pos2d");
  Umove2d=glGetUniformLocation(program, "move2d");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1i(Uiterations,(GLint)(int)iterations);
  glUniform1i(Uvolsteps,(GLint)(int)volsteps);
  glUniform4f(Ucol1,(GLfloat)col1.r,(GLfloat)col1.g,(GLfloat)col1.b,(GLfloat)col1.a);
  glUniform4f(Ucol2,(GLfloat)col2.r,(GLfloat)col2.g,(GLfloat)col2.b,(GLfloat)col2.a);
  glUniform2f(Upos2d,(GLfloat)pos2d.x,(GLfloat)pos2d.y);
  glUniform2f(Umove2d,(GLfloat)move2d.x,(GLfloat)move2d.y);
  glUniform1f(Uformuparam2,(GLfloat)formuparam2);
  glUniform1f(Ustepsize,(GLfloat)stepsize);
  glUniform1f(Uzoom,(GLfloat)zoom);
  glUniform1f(Utile,(GLfloat)tile);
  glUniform1f(Uspeed2,(GLfloat)speed2);
  glUniform1f(Ubrightness,(GLfloat)brightness);
  glUniform1f(Udarkmatter,(GLfloat)darkmatter);
  glUniform1f(Udistfading,(GLfloat)distfading);
  glUniform1f(Usaturation,(GLfloat)saturation);
  glUniform1f(UtransverseSpeed,(GLfloat)transverseSpeed);
  glUniform1f(Ucloud,(GLfloat)cloud);
  glUniform1f(Utime,(GLfloat)time);
  
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
  glUseProgram(0);
 }
 void Set( ElectronCloudsProfile *in ) {
  iterations=in->iterations;
  volsteps=in->volsteps;
  formuparam2=in->formuparam2;
  stepsize=in->stepsize;
  zoom=in->zoom;
  tile=in->tile;
  speed2=in->speed2;
  brightness=in->brightness;
  darkmatter=in->darkmatter;
  distfading=in->distfading;
  saturation=in->saturation;
  transverseSpeed=in->transverseSpeed;
  cloud=in->cloud;
  time=in->time;
  col1.fromCrayon(in->col1);
  col2.fromCrayon(in->col2);
  pos2d.Set(&in->pos2d);
 }
 void Render( FBOColor *out, ElectronCloudsProfile *in );
};


ONE(ClassicPlanetProfile,{
  tex1=library.Load("data/images/textures/tile12.png",tllto_Tiling);
  tex2=library.Load("data/images/textures/tile13.png",tllto_Tiling);
  tex3=library.Load("data/images/textures/tile14.png",tllto_Tiling);
  tex4=library.Load("data/images/textures/tile15.png",tllto_Tiling);
  sF.Set(1.0,100.0,90.0); sF.w=45.0;
  nT.Set(2.0,3.0,4.0); nT.w=5.0;
  striations.Set(0.0,0.8);
  noiseform.Set(3.0,2.0,3.0); noiseform.w=2.0;
  nf2.Set(4.0,8.0,0.66); nf2.w=0.34;
  nf3.Set(0.0,41.0,289.0); nf3.w=330.0;
  nf5.Set(10.0,0.5);
  edgeGlow=0.001;
  noisescale=0.005;
  colMixer.Set(0.5,0.5,0.5); colMixer.w=0.5;
  colorA.Double(0.15,0.05,1.0,1.0);
  colorB.Double(1.0,0.1,0.02,1.0);
  e.Set(0.1001,0.0);
  nf4.Set(12.0,48.0);
})
 Cartesiand pos2d,sF,nT,striations,noiseform,nf2,nf3,nf5,colMixer,e,nf4;
 Zp<GLImage> tex1,tex2,tex3,tex4;
 Zdouble seed,edgeGlow,noisescale;
 Crayon colorA,colorB;
 void Randomize(int seed) {
//  colorA.random(seed);
//  colorB.random(seed + 1);
//  colMixer.Set(iratiod(upto(100, seed + 2), 90) + 0.1, iratiod(upto(100, seed + 4), 90) + 0.1, iratiod(upto(100, seed + 3), 90) + 0.1);
//  striations.Set(iratiod(upto(100, seed + 12), 200), iratiod(upto(100, seed + 6), 100)*7.0+1.0);
 }
 BINARY_IN({
  B_IN_SSUB(pos2d)
  B_IN_SSUB(sF)
  B_IN_SSUB(nT)
  B_IN_SSUB(striations)
  B_IN_SSUB(noiseform)
  B_IN_SSUB(nf2)
  B_IN_SSUB(nf3)
  B_IN_SSUB(nf5)
  B_IN_SSUB(colMixer)
  B_IN_SSUB(e)
  B_IN_SSUB(nf4)
  B_IN_IMAGE(tex1)
  B_IN_IMAGE(tex2)
  B_IN_IMAGE(tex3)
  B_IN_IMAGE(tex4)
  B_IN_NUM(seed)
  B_IN_NUM(edgeGlow)
  B_IN_NUM(noisescale)
  B_IN_TINT(colorA)
  B_IN_TINT(colorB)
 })
 BINARY_OUT({
  B_OUT_SSUB(pos2d)
  B_OUT_SSUB(sF)
  B_OUT_SSUB(nT)
  B_OUT_SSUB(striations)
  B_OUT_SSUB(noiseform)
  B_OUT_SSUB(nf2)
  B_OUT_SSUB(nf3)
  B_OUT_SSUB(nf5)
  B_OUT_SSUB(colMixer)
  B_OUT_SSUB(e)
  B_OUT_SSUB(nf4)
  B_OUT_IMAGE(tex1)
  B_OUT_IMAGE(tex2)
  B_OUT_IMAGE(tex3)
  B_OUT_IMAGE(tex4)
  B_OUT_NUM(seed)
  B_OUT_NUM(edgeGlow)
  B_OUT_NUM(noisescale)
  B_OUT_TINT(colorA)
  B_OUT_TINT(colorB)
 })
 KEYWORDS({
  SUBWORD("pos2d",pos2d)
  else SUBWORD("sF",sF)
  else SUBWORD("nT",nT)
  else SUBWORD("striations",striations)
  else SUBWORD("noiseform",noiseform)
  else SUBWORD("nf2",nf2)
  else SUBWORD("nf3",nf3)
  else SUBWORD("nf5",nf5)
  else SUBWORD("colMixer",colMixer)
  else SUBWORD("e",e)
  else SUBWORD("nf4",nf4)
  else TILEWORD("tex1",tex1)
  else TILEWORD("tex2",tex2)
  else TILEWORD("tex3",tex3)
  else TILEWORD("tex4",tex4)
  else KEYWORD("seed",{seed=w;})
  else KEYWORD("edgeGlow",{edgeGlow=w;})
  else KEYWORD("noisescale",{noisescale=w;})
  else TINTWORD("colorA",colorA)
  else TINTWORD("colorB",colorB)
 })
 WORDKEYS({
  SSUBKEY("pos2d",pos2d)
  SSUBKEY("sF",sF)
  SSUBKEY("nT",nT)
  SSUBKEY("striations",striations)
  SSUBKEY("noiseform",noiseform)
  SSUBKEY("nf2",nf2)
  SSUBKEY("nf3",nf3)
  SSUBKEY("nf5",nf5)
  SSUBKEY("colMixer",colMixer)
  SSUBKEY("e",e)
  SSUBKEY("nf4",nf4)
  IMGKEY("tex1",tex1)
  IMGKEY("tex2",tex2)
  IMGKEY("tex3",tex3)
  IMGKEY("tex4",tex4)
  NUMBKEY("seed",seed)
  NUMBKEY("edgeGlow",edgeGlow)
  NUMBKEY("noisescale",noisescale)
  TINTKEY("colorA",colorA)
  TINTKEY("colorB",colorB)
 })
MANY(ClassicPlanetProfile,ClassicPlanetProfileHandle,ClassicPlanetProfileHandles,"ClassicPlanet",ClassicPlanetProfiles,{})
 KEYWORDSGroup("ClassicPlanet",ClassicPlanetProfile);
 WORDKEYSGroup("ClassicPlanet",ClassicPlanetProfile);
 BINARY_INOUT(ClassicPlanetProfile)
DONE(ClassicPlanetProfile);

class ClassicPlanetShader : public GLSLShader {
public:
 Cartesiand pos2d,sF,nT,striations,noiseform,nf2,nf3,nf5,colMixer,e,nf4;
 Zp<GLuint> tex1,tex2,tex3,tex4;
 Zdouble seed,edgeGlow,noisescale;
 Crayon colorA,colorB;
 GLuint Upos2d,UsF,UnT,Ustriations,Unoiseform,Unf2,Unf3,Unf5,UcolMixer,Ue,Unf4,Utex1,Utex2,Utex3,Utex4,Useed,UedgeGlow,Unoisescale,UcolorA,UcolorB;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Upos2d=glGetUniformLocation(program, "pos2d");
  UsF=glGetUniformLocation(program, "sF");
  UnT=glGetUniformLocation(program, "nT");
  Ustriations=glGetUniformLocation(program, "striations");
  Unoiseform=glGetUniformLocation(program, "noiseform");
  Unf2=glGetUniformLocation(program, "nf2");
  Unf3=glGetUniformLocation(program, "nf3");
  Unf5=glGetUniformLocation(program, "nf5");
  UcolMixer=glGetUniformLocation(program, "colMixer");
  Ue=glGetUniformLocation(program, "e");
  Unf4=glGetUniformLocation(program, "nf4");
  Utex1=glGetUniformLocation(program, "tex1");
  Utex2=glGetUniformLocation(program, "tex2");
  Utex3=glGetUniformLocation(program, "tex3");
  Utex4=glGetUniformLocation(program, "tex4");
  Useed=glGetUniformLocation(program, "seed");
  UedgeGlow=glGetUniformLocation(program, "edgeGlow");
  Unoisescale=glGetUniformLocation(program, "noisescale");
  UcolorA=glGetUniformLocation(program, "colorA");
  UcolorB=glGetUniformLocation(program, "colorB");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Upos2d,(GLfloat)pos2d.x,(GLfloat)pos2d.y);
  glUniform4f(UsF,(GLfloat)sF.x,(GLfloat)sF.y,(GLfloat)sF.z,(GLfloat)sF.w);
  glUniform4f(UnT,(GLfloat)nT.x,(GLfloat)nT.y,(GLfloat)nT.z,(GLfloat)nT.w);
  glUniform2f(Ustriations,(GLfloat)striations.x,(GLfloat)striations.y);
  glUniform4f(Unoiseform,(GLfloat)noiseform.x,(GLfloat)noiseform.y,(GLfloat)noiseform.z,(GLfloat)noiseform.w);
  glUniform4f(Unf2,(GLfloat)nf2.x,(GLfloat)nf2.y,(GLfloat)nf2.z,(GLfloat)nf2.w);
  glUniform4f(Unf3,(GLfloat)nf3.x,(GLfloat)nf3.y,(GLfloat)nf3.z,(GLfloat)nf3.w);
  glUniform2f(Unf5,(GLfloat)nf5.x,(GLfloat)nf5.y);
  glUniform4f(UcolMixer,(GLfloat)colMixer.x,(GLfloat)colMixer.y,(GLfloat)colMixer.z,(GLfloat)colMixer.w);
  glUniform2f(Ue,(GLfloat)e.x,(GLfloat)e.y);
  glUniform2f(Unf4,(GLfloat)nf4.x,(GLfloat)nf4.y);
  glUniform3f(UcolorA,(GLfloat)colorA.rf,(GLfloat)colorA.gf,(GLfloat)colorA.bf);
  glUniform3f(UcolorB,(GLfloat)colorB.rf,(GLfloat)colorB.gf,(GLfloat)colorB.bf);
  glUniform1f(Useed,(GLfloat)seed);
  glUniform1f(UedgeGlow,(GLfloat)edgeGlow);
  glUniform1f(Unoisescale,(GLfloat)noisescale);
  glUniform1i(Utex1, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, (*tex1.pointer));
  glUniform1i(Utex2, 1);  glActiveTexture(GL_TEXTURE1);  glBindTexture(GL_TEXTURE_2D, (*tex2.pointer));
  glUniform1i(Utex3, 2);  glActiveTexture(GL_TEXTURE2);  glBindTexture(GL_TEXTURE_2D, (*tex3.pointer));
  glUniform1i(Utex4, 3);  glActiveTexture(GL_TEXTURE3);  glBindTexture(GL_TEXTURE_2D, (*tex4.pointer));
  glUniform1i(Utex1, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, (*tex1.pointer));
  glUniform1i(Utex2, 1);  glActiveTexture(GL_TEXTURE1);  glBindTexture(GL_TEXTURE_2D, (*tex2.pointer));
  glUniform1i(Utex3, 2);  glActiveTexture(GL_TEXTURE2);  glBindTexture(GL_TEXTURE_2D, (*tex3.pointer));
  glUniform1i(Utex4, 3);  glActiveTexture(GL_TEXTURE3);  glBindTexture(GL_TEXTURE_2D, (*tex4.pointer));
  
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
 void Set( ClassicPlanetProfile *in ) {
  pos2d.Set(&in->pos2d);
  sF.Set(&in->sF);
  nT.Set(&in->nT);
  striations.Set(&in->striations);
  noiseform.Set(&in->noiseform);
  nf2.Set(&in->nf2);
  nf3.Set(&in->nf3);
  nf5.Set(&in->nf5);
  colMixer.Set(&in->colMixer);
  e.Set(&in->e);
  nf4.Set(&in->nf4);
  tex1=&in->tex1->texture;
  tex2=&in->tex2->texture;
  tex3=&in->tex3->texture;
  tex4=&in->tex4->texture;
  seed=in->seed;
  edgeGlow=in->edgeGlow;
  noisescale=in->noisescale;
  colorA.fromCrayon(in->colorA);
  colorB.fromCrayon(in->colorB);
 }
 void Render( FBOColor *out, ClassicPlanetProfile *in );
};


//\=\\\\\\\\\\\\\\\\\\\\=\\\\\\\\\\

extern WaterAndGroundShader waterandgroundShader;
extern EmissionCloudsShader emissioncloudsShader;
extern ElectroCloudsShader electrocloudsShader;
extern NebulazShader nebulazShader;
extern ToonTroidShader toontroidShader;
extern TexturedPlanetShader texturedplanetShader;
extern EarthlikeShader earthlikeShader;
extern AccretionShader accretionShader;
extern BlackHoleShader blackholeShader;
extern RingedJovianShader ringedjovianShader;
extern ElectroPlanetShader electroplanetShader;
extern AtmosphericPlanetShader atmosphericplanetShader;
extern Earthlike2Shader earthlike2Shader;
extern RingedJovian2Shader ringedjovian2Shader;
extern LunarPlanetShader lunarplanetShader;
extern FlaringStarShader flaringstarShader;
extern ElectronCloudsShader electroncloudsShader;
extern ClassicPlanetShader classicplanetShader;