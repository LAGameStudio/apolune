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
/* Some parts herein are the property of Khronos Group */

#pragma once

#include <windows.h>
#include <stdio.h>
#include <string>
#include "Strings.h"
#include "Cartesian.h"
#include "Indexed.h"

//#define GL_DEBUG 1
// The above line locally enables debug strings and is here specifically
// due to glPop() hacky frowned upon but necessary implementation.
// Note: glPop() swallows internal errors.  These may be indicators of a
// problem in OpenGL code being run before glPop() is called.  To see
// this oddity, define REPORT_ALL_ERRORS.
// Affects all files and GL_Report etc listed in GLWindow.h
//#define REPORT_ALL_ERRORS 1

#if defined(GL_DEBUG)
extern char gl_info[1024];
extern char old_gl_info[1024];
#endif

#define wglxGetProcAddress wglGetProcAddress

#if !defined(using_gl)
#define using_gl 1
#define WGL_WGLEXT_PROTOTYPES 1
#if !defined(dont_use_glew)
#define GLEW_STATIC
#include "glew.h"
#undef GLEWAPI
#define GLEW_STATIC
#include "wglew.h"
#else
#define use_dhpowareGL 1
#endif
//#include <gl/gl.h>
//#include <gl/glu.h>
#endif

#if defined(use_dhpowareGL)
#include "dhpowareGL3.h"
#endif

#define WGL_WGLEXT_PROTOTYPES 1
#include "wglext.h"		//WGL extensions
#include "glext.h"		 //GL extensions

#include "glsw.h"       //Shader Wrangler

#include "macros.h"

// Declarations We'll Use
#define WGL_SAMPLE_BUFFERS_ARB	0x2041
#define WGL_SAMPLES_ARB		0x2042

// GL_EXT_packed_depth_stencil
#ifndef GL_DEPTH_STENCIL_EXT
#define GL_DEPTH_STENCIL_EXT 0x84F9
#endif
#ifndef GL_DEPTH24_STENCIL8_EXT
#define GL_DEPTH24_STENCIL8_EXT 0x88F0
#endif

#define GL_CLAMP_TO_EDGE 0x812F

#define BEST_DEGREE_OF_ANISOTROPY  -1
#define BEST_ANTI_ALIASING_SAMPLES -1

// GL_ARB_point_parameters
//PFNGLPOINTPARAMETERFARBPROC  glPointParameterfARB  = NULL;
//PFNGLPOINTPARAMETERFVARBPROC glPointParameterfvARB = NULL;

enum GLVendors { undetermined=-1, ati=0, nvidia=1, intel=2 /* apple? droid? */ };

/*
 * The following code must be here for higher level classes (GLWindow, Art.h). 
 */
enum ColorChannel { RED=0, GREEN=1, BLUE=2, ALPHA=3 };


enum BlendOpSeparationModes {
 _BlendOpDefault=0,
 _BlendOpBlend=1,
 _BlendOpEquation=2,
 _BlendOpBlendEquation=3
};

struct BlendOp {
public:
 Zint id;
 Zstring name,keyword;
 GLenum s,d,sa,da,e1,e2;
 BlendOpSeparationModes mode;
 PROGRAMMABLE(BlendOp,OpAPIPtr,opapi,OpAPI);
 BlendOp() {
  id=-1;
  sa=s=GL_ONE;
  da=d=GL_ZERO;
  e2=e1=GL_FUNC_ADD;
  opapi=&BlendOp::OpDefault;
  mode=_BlendOpDefault;
 }
 void SetMode( BlendOpSeparationModes m ) {
  mode=m;
  switch (mode) {
         case _BlendOpDefault: opapi=&BlendOp::OpDefault; break;
           case _BlendOpBlend: opapi=&BlendOp::OpBlend;  break;
        case _BlendOpEquation: opapi=&BlendOp::OpEquation; break;
   case _BlendOpBlendEquation: opapi=&BlendOp::OpBlendEquation;  break;
  }
 }
 void OpDefault() {
  glEnable(GL_BLEND);
  glBlendFunc(s,d);
  glBlendEquation(e1);
 }
 void OpBlend() {
  glEnable(GL_BLEND);
  glBlendFuncSeparate(s,d,sa,da);
  glBlendEquation(e1);
 }
 void OpEquation() {
  glEnable(GL_BLEND);
  glBlendFunc(s,d);
  glBlendEquationSeparate(e1,e2);
 }
 void OpBlendEquation() {
  glEnable(GL_BLEND);
  glBlendFuncSeparate(s,d,sa,da);
  glBlendEquationSeparate(e1,e2);
 }
 void Enable() { OpAPI(); }
 string toString() { return keyword.value; }
};

class BlendModes {
public:
 Indexed<BlendOp> modes;
 void Init();
 BlendOp *Define( string keyword, GLenum s, GLenum d, GLenum e ) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->e1=e;
  b->keyword=keyword;
  return b;
 }
 BlendOp *Define( string keyword, GLenum s, GLenum d ) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->keyword=keyword;
  return b;
 }
 BlendOp *Define( string keyword, GLenum s, GLenum d, GLenum e1, GLenum e2, bool fuck ) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->e1=e1;
  b->e2=e2;
  b->SetMode(_BlendOpEquation);
  b->keyword=keyword;
  return b;
 }
 BlendOp *Define( string keyword, GLenum s, GLenum d, GLenum sa, GLenum da, GLenum e ) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->sa=sa;
  b->da=da;
  b->e1=e;
  b->keyword=keyword;
  b->SetMode(_BlendOpBlend);
  return b;
 }
 BlendOp *Define( string keyword, GLenum s, GLenum d, GLenum sa, GLenum da, GLenum e1, GLenum e2) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->sa=sa;
  b->da=da;
  b->e1=e1;
  b->e2=e2;
  b->keyword=keyword;
  b->SetMode(_BlendOpBlendEquation);
  return b;
 }
 BlendOp *Define( int id, string keyword, GLenum s, GLenum d ) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->keyword=keyword;
  b->id=id;
  return b;
 }
 BlendOp *Define( int id, string keyword, GLenum s, GLenum d, GLenum e ) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->e1=e;
  b->keyword=keyword;
  b->id=id;
  return b;
 }
 BlendOp *Define( int id, string keyword, GLenum s, GLenum d, GLenum e1, GLenum e2, bool fuck ) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->e1=e1;
  b->e2=e2;
  b->SetMode(_BlendOpEquation);
  b->keyword=keyword;
  b->id=id;
  return b;
 }
 BlendOp *Define( int id, string keyword, GLenum s, GLenum d, GLenum sa, GLenum da, GLenum e ) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->sa=sa;
  b->da=da;
  b->e1=e;
  b->keyword=keyword;
  b->SetMode(_BlendOpBlend);
  b->id=id;
  return b;
 }
 BlendOp *Define( int id, string keyword, GLenum s, GLenum d, GLenum sa, GLenum da, GLenum e1, GLenum e2) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->sa=sa;
  b->da=da;
  b->e1=e1;
  b->e2=e2;
  b->keyword=keyword;
  b->SetMode(_BlendOpBlendEquation);
  b->id=id;
  return b;
 }
 BlendOp *Define( string name, string keyword, GLenum s, GLenum d, GLenum e ) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->e1=e;
  b->keyword=keyword;
  b->name=name;
  return b;
 }
 BlendOp *Define( string name, string keyword, GLenum s, GLenum d ) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->keyword=keyword;
  b->name=name;
  return b;
 }
 BlendOp *Define( string name, string keyword, GLenum s, GLenum d, GLenum e1, GLenum e2, bool fuck ) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->e1=e1;
  b->e2=e2;
  b->SetMode(_BlendOpEquation);
  b->keyword=keyword;
  b->name=name;
  return b;
 }
 BlendOp *Define( string name, string keyword, GLenum s, GLenum d, GLenum sa, GLenum da, GLenum e ) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->sa=sa;
  b->da=da;
  b->e1=e;
  b->keyword=keyword;
  b->name=name;
  b->SetMode(_BlendOpBlend);
  return b;
 }
 BlendOp *Define( string name, string keyword, GLenum s, GLenum d, GLenum sa, GLenum da, GLenum e1, GLenum e2) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->sa=sa;
  b->da=da;
  b->e1=e1;
  b->e2=e2;
  b->keyword=keyword;
  b->name=name;
  b->SetMode(_BlendOpBlendEquation);
  return b;
 }
 BlendOp *Define( int id, string name, string keyword, GLenum s, GLenum d ) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->keyword=keyword;
  b->name=name;
  b->id=id;
  return b;
 }
 BlendOp *Define( int id, string name, string keyword, GLenum s, GLenum d, GLenum e ) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->e1=e;
  b->keyword=keyword;
  b->name=name;
  b->id=id;
  return b;
 }
 BlendOp *Define( int id, string name, string keyword, GLenum s, GLenum d, GLenum e1, GLenum e2, bool fuck ) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->e1=e1;
  b->e2=e2;
  b->SetMode(_BlendOpEquation);
  b->keyword=keyword;
  b->name=name;
  b->id=id;
  return b;
 }
 BlendOp *Define( int id, string name, string keyword, GLenum s, GLenum d, GLenum sa, GLenum da, GLenum e ) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->sa=sa;
  b->da=da;
  b->e1=e;
  b->SetMode(_BlendOpBlend);
  b->keyword=keyword;
  b->name=name;
  b->id=id;
  return b;
 }
 BlendOp *Define( int id, string name, string keyword, GLenum s, GLenum d, GLenum sa, GLenum da, GLenum e1, GLenum e2) {
  Zp<BlendOp> b=modes.Increase();
  b->s=s;
  b->d=d;
  b->sa=sa;
  b->da=da;
  b->e1=e1;
  b->e2=e2;
  b->SetMode(_BlendOpBlendEquation);
  b->keyword=keyword;
  b->name=name;
  b->id=id;
  return b;
 }
 BlendOp *find( int id ) {
  for ( unsigned int i=0; i<modes.length; i++ ) if ( modes[i].id == id ) return &modes[i];
  return &modes[0];
 }
 BlendOp *find( string name ) { return find(name.c_str()); }
 BlendOp *find( const char *s ) {
  for ( unsigned int i=0; i<modes.length; i++ ) if ( !str_cmp(modes[i].name,s) ) return &modes[i];
  return &modes[0];
 }
};

extern BlendModes blends;

void Blend( int id );
void Blend( BlendOp *b );
BlendOp *Blend( string name );
void Blend( bool on_off );

// Legacy blending system.
enum Blendings {
 _additive=0,
 _solarize=1,
 _mask=2,
 _multiply=3,
 _none=4,
 _transparency=5,
 _overlay=6,
 _screened=7,
 _saturate=8,
 _mask1=9,
 _subtractive=10,
 _translucency=11,
 _transluminant=12,
 _silhouette=13,
 _subtractAlpha=14,
 _transparentAlpha=15
};

#define FIRST_BLEND (_additive)
#define LAST_BLEND (_transparentAlpha)

class Blends {
public:
 Blendings blend;
 Blends() { blend=_none; }
 Blends( Blendings b ) : blend(b) {}
 Blends& operator= ( const Blends& b ) {
  blend=b.blend;
  return *this;
 }
 Blends& operator= ( Blendings b ) {
  blend=b;
  return *this;
 }
 Blends& operator= ( int b ) {
  blend=(Blendings) b;
  return *this;
 } 
 bool operator != ( const Blends& b ) { return (this->blend != b.blend); }
 Blends& operator= ( const string& s ) {
  const char *q=s.c_str();
  if ( is_integer(q) ) blend= (Blendings) atoi(q);
  else if ( !str_prefix(q,"additive") )      blend=_additive;
  else if ( !str_prefix(q,"solarize") )      blend=_solarize;
  else if ( !str_prefix(q,"mask") )          blend=_mask;
  else if ( !str_prefix(q,"multiply") )      blend=_multiply;
  else if ( !str_prefix(q,"none") )          blend=_none;
  else if ( !str_prefix(q,"transparency") )  blend=_transparency;
  else if ( !str_prefix(q,"overlay") )       blend=_overlay;
  else if ( !str_prefix(q,"screened") )      blend=_screened;
  else if ( !str_prefix(q,"saturate") )      blend=_saturate;
  else if ( !str_prefix(q,"mask1") )         blend=_mask1;
  else if ( !str_prefix(q,"subtractive") )   blend=_subtractive;
  else if ( !str_prefix(q,"translucency") )  blend=_translucency;
  else if ( !str_prefix(q,"transluminant") ) blend=_transluminant;
  else if ( !str_prefix(q,"silhouette") )    blend=_silhouette;
  else if ( !str_prefix(q,"subtractalpha") ) blend=_subtractAlpha;
  else if ( !str_prefix(q,"transparentalpha") ) blend=_transparentAlpha;
  else {
   OUTPUT("blendStringOptions: Invalid blending mode `%s`, defaulting to `none`.");
   blend=_none;
  }
  return *this;
 }
 void fromString( const char *s ) { (*this) = string(s); }
 operator int() { return (int) blend; }
 operator string() { return toString(); }
 string toString() {
  switch ( blend ) {
        case _additive: return string("additive");
        case _solarize: return string("solarize");
            case _mask: return string("mask");
        case _multiply: return string("multiply");
            case _none: return string("none");
    case _transparency: return string("transparency");
         case _overlay: return string("overlay");
        case _screened: return string("screened");
        case _saturate: return string("saturate");
           case _mask1: return string("mask1");
     case _subtractive: return string("subtractive");
    case _translucency: return string("translucency");
   case _transluminant: return string("transluminant");
      case _silhouette: return string("silhouette");
   case _subtractAlpha: return string("subtractAlpha");
case _transparentAlpha: return string("transparentAlpha");
   default: 
    OUTPUT("blendsToString: Value passed (%d) not a valid blending mode; defaulting to `none`.\n", (int) blend );
    return string("none");
  }
 }
 void toStrings( Strings *out ) {
  out->Add("additive",(int)_additive);
  out->Add("solarize",(int)_solarize);
  out->Add("mask",(int)_mask);
  out->Add("multiply",(int)_multiply);
  out->Add("none",(int)_none);
  out->Add("transparency",(int)_transparency);
  out->Add("overlay",(int)_overlay);
  out->Add("screened",(int)_screened);
  out->Add("saturate",(int)_saturate);
  out->Add("mask1",(int)_mask1);
  out->Add("subtractive",(int)_subtractive);
  out->Add("translucency",(int)_translucency);
  out->Add("transluminant",(int)_transluminant);
  out->Add("silhouette",(int)_silhouette);
  out->Add("subtractAlpha",(int)_subtractAlpha);
  out->Add("transparentAlpha",(int)_transparentAlpha);
 }
 void next() {
  if ( blend == LAST_BLEND ) blend = FIRST_BLEND;
  else blend=(Blendings)((int)blend+1);
 }
 void previous() {
  if ( blend == FIRST_BLEND ) blend = LAST_BLEND;
  else blend=(Blendings)((int)blend-1);
 }
 void Additive()         { blend=_additive;         }
 void Solarize()         { blend=_solarize;         }
 void Mask()             { blend=_mask;             }
 void Multiply()         { blend=_multiply;         }
 void None()             { blend=_none;             }
 void Transparency()     { blend=_transparency;     }
 void Overlay()          { blend=_overlay;          }
 void Screened()         { blend=_screened;         }
 void Saturate()         { blend=_saturate;         }
 void Mask1()            { blend=_mask1;            }
 void Subtractive()      { blend=_subtractive;      }
 void Translucency()     { blend=_translucency;     }
 void Transluminant()    { blend=_transluminant;    }
 void Silhouette()       { blend=_silhouette;       }
 void SubtractAlpha()    { blend=_subtractAlpha;    }
 void TransparentAlpha() { blend=_transparentAlpha; }
};

extern const Blends
 additive,
 solarize,
 mask,
 multiply,
 none,
 transparency,
 overlay,
 screened,
 saturate,
 mask1,
 subtractive,
 translucency,
 transluminant,
 silhouette,
 subtractAlpha,
 transparentAlpha;

bool operator== ( const Blends& a, const Blends& b );
bool operator== ( Blendings a, const Blends& b );
bool operator== ( const Blends& a, Blendings b );

enum BlendOps {
 zero=GL_ZERO, one=GL_ONE,
 srcColor=GL_SRC_COLOR,
 diffSrcColor=GL_ONE_MINUS_SRC_COLOR,
 destColor=GL_DST_COLOR, 
 mDestColor=GL_ONE_MINUS_DST_COLOR,
 srcAlpha=GL_SRC_ALPHA,
 mOneSrcAlpha=GL_ONE_MINUS_SRC_ALPHA,
 dstAlpha=GL_DST_ALPHA,
 dstAlpha2=GL_ONE_MINUS_DST_ALPHA,
 constantColor=GL_CONSTANT_COLOR,
 mConstantColor=GL_ONE_MINUS_CONSTANT_COLOR,
 constantAlpha=GL_CONSTANT_ALPHA, 
 mConstantAlpha=GL_ONE_MINUS_CONSTANT_ALPHA,
 srcAlphaSaturate=GL_SRC_ALPHA_SATURATE
};

class GLSetup
{
public:
 std::string vendor;
 std::string renderer;
 std::string version;
 std::string gluVersion;
 std::string gluExtensions;

 GLVendors vendortype;
 bool isATI,isNVIDIA,isINTEL;

 Strings supported;

 // Display-level features
 bool WGL_EXT_swap_control_supported;
 bool vSync;

 // Multisampling
 bool	arbMultisampleSupported;
 int	arbMultisampleFormat;

 // FBO and ancillary supported features
 // Best
 bool ARB_FBO, ARB_FBO_multisample, ARB_FBO_blit, ARB_FBO_npot, ARB_FBO_rect, ARB_packed_depth_stencil, ARB_draw_buffers;
 // Old fallback extension
 bool EXT_FBO, EXT_FBO_multisample, EXT_FBO_blit, EXT_FBO_npot, EXT_FBO_rect, EXT_packed_depth_stencil;
 bool EXTX_mixed_framebuffer_formats;
 int fboMaxSamples,fboMaxColorAttachments;

 // Deprecated point sprite systems
 bool GL_ARB_POINT_SPRITES;

 // VBO and Shading
 bool vboSupported, hasShaders, hasGeometryShaders, hasTesselationShaders, canInstance;

 // Features that require multiple subsystems
 bool deferredShading, perPixelLighting;

 // GL Parameters
 float glMaxSize;

 // Anisotropic Filtering
 int maxDegreeOfAnisotropy;
 int degreeOfAnisotropy;

 // Display Specifics
 int redBits;
 int greenBits;
 int blueBits;
 int alphaBits;
 int depthBits;
 int stencilBits;
 int maxTextureSize;
 int maxLights;
 int maxAttribStacks;
 int maxModelViewStacks;
 int maxProjectionStacks;
 int maxClipPlanes;
 int maxTextureStacks;
 int maxVertexUniforms;
 int maxGeometryUniforms;
 int maxFragmentUniforms;
 int maxDrawBuffers;
 int maxTessUniforms;
 int
  maxVertexTextureImageUnits, maxVertexOutputComponents, maxVertexOutputVertices,
  maxGeometryOutputVertices, maxGeometryTotalOutputComponents, maxGeometryTextureImageUnits,
  maxGeometryInputComponents, maxGeometryOutputComponents, 
  maxGeometryUniformComponents, maxVaryingComponents, maxPatchVertices;
 int maxW,maxH,aliasedPointLow,aliasedPointHigh, defaultPatchInner,defaultPatchOuter;
 int maxTessGenLevel, maxTessControlUniformComponents, maxTessEvaluationUniformComponents,
  maxTessControlTextureImageUnits, maxTessEvaluationTextureImageUnits,
  maxTessControlOutputComponents, maxTessPatchComponents,
  maxTessControlTotalOutputComponents,maxTessControlInputComponents,
  maxTessEvaluationOutputComponents,
  maxTessControlUniformBlocks,maxTessEvaluationUniformBlocks,
  maxTessEvaluationInputComponents,
  maxCombinedTessControlUniformComponents,
  maxCombinedTessEvaluationUniformComponents;
 //char *alphaTestFunc;
 float alphaTestRef;
 float minPointSize,maxPointSize;
 int numCompressedTextureFormats;
 Indexed<int> formats;
 int implementationColorReadFormatOES;
 int implementationColorReadTypeOES;
 int maxModelviewStackDepth,maxProjectionStackDepth;
 int maxTextureStackDepth,maxTextureUnits,maxCombinedTextureImageUnits;
 int pointSizeMax,pointSizeMin,stencilRef,subpixelBits;
 
 // GL Versions
 int majorGL, minorGL, majorGLSL, minorGLSL;

 // Requirements test
 Zbool cardMeetsRequirements;
 Zdouble GLRequired, GLSLRequired;

 // Command line settings
 bool noMipmapping, noFullscreen, borderless, resizable, escTrapdoor;
 Zstring winTitle;

 /*some day soon, client side this.. bleah
  Integer states;
 MatrixStack matrices;
 int state;
 void PushMatrix() {

 }
 void PopMatrix() {
 }

 void PushAttrib( int bit ) {
  
 }
 void PopAttrib() {
 }*/

 GLSetup(void) : redBits(0), greenBits(0), blueBits(0), alphaBits(0), depthBits(0),
               stencilBits(0), maxTextureSize(0), maxLights(0), maxAttribStacks(0),
               maxModelViewStacks(0), maxClipPlanes(0), maxTextureStacks(0) {
#if defined(GL_DEBUG)
  gl_info[0]='\0';
  old_gl_info[0]='\0';
#endif
  winTitle="OpenGL";
  WGL_EXT_swap_control_supported=false;
  vSync=false;
  arbMultisampleSupported=false;
  arbMultisampleFormat=0;
  EXT_FBO=EXT_FBO_multisample=EXT_FBO_blit
   =EXT_FBO_npot=EXT_FBO_rect=EXT_packed_depth_stencil
   =ARB_draw_buffers=GL_ARB_POINT_SPRITES
   =EXTX_mixed_framebuffer_formats=false;
  fboMaxColorAttachments=fboMaxSamples=0;
  noMipmapping=noFullscreen=borderless=escTrapdoor=false;
  vboSupported=false;
  hasShaders=hasGeometryShaders=hasTesselationShaders=false;
  deferredShading=false;
  perPixelLighting=false;
  canInstance=false;
  glMaxSize=0;
  majorGL=minorGL=majorGLSL=minorGLSL=0;
  isATI=isNVIDIA=isINTEL=false;
  vendortype=undetermined;
  maxW=maxH=aliasedPointLow=aliasedPointHigh=0;
   //char *alphaTestFunc;
  alphaTestRef=0.0f;
  numCompressedTextureFormats=0;
  implementationColorReadFormatOES=implementationColorReadTypeOES
   =maxModelviewStackDepth=maxProjectionStackDepth=maxTextureStackDepth
   =maxTextureUnits=pointSizeMax=pointSizeMin=stencilRef=subpixelBits=0;
  ARB_FBO=false;
  ARB_FBO_multisample=false;
  ARB_FBO_blit=false;
  ARB_FBO_npot=false;
  ARB_FBO_rect=false;
  ARB_packed_depth_stencil=false;
  maxDegreeOfAnisotropy=0;
  degreeOfAnisotropy=0;
  maxProjectionStacks=0;
  maxVertexUniforms=0;
  maxGeometryUniforms=0;
  maxFragmentUniforms=0;
  maxTessUniforms=0;
  maxVertexTextureImageUnits=0;
  maxVertexOutputComponents=0;
  maxVertexOutputVertices=0;
  maxGeometryOutputVertices=0;
  maxGeometryTotalOutputComponents=0;
  maxGeometryTextureImageUnits=0;
  maxGeometryInputComponents=0;
  maxGeometryOutputComponents=0;
  maxGeometryUniformComponents=0;
  maxVaryingComponents=0;
  maxPatchVertices=0;
  defaultPatchInner=0;
  defaultPatchOuter=0;
  maxTessGenLevel=0;
  maxTessControlUniformComponents=0;
  maxTessEvaluationUniformComponents=0;
  maxTessControlTextureImageUnits=0;
  maxTessEvaluationTextureImageUnits=0;
  maxTessControlOutputComponents=0;
  maxTessEvaluationTextureImageUnits=0;
  maxTessControlOutputComponents=0;
  maxTessPatchComponents=0;
  maxTessControlTotalOutputComponents=0;
  maxTessControlInputComponents=0;
  maxTessEvaluationOutputComponents=0;
  maxTessControlUniformBlocks=0;
  maxTessEvaluationUniformBlocks=0;
  maxTessEvaluationInputComponents=0;
  maxCombinedTessControlUniformComponents=0;
  maxCombinedTessEvaluationUniformComponents=0;
  minPointSize=0;
  maxPointSize=0;
  maxCombinedTextureImageUnits=0;
  GLRequired=3.3;
  GLSLRequired=3.2;
 }
 ~GLSetup(void) {}

 void FindSupportedExtensions() {
  char *s = NULL;

 	// Try Standard Opengl Extensions String
 	s = (char*)glGetString(GL_EXTENSIONS);
 
  const char *p=s;
  char buf[128];
  while ( p && *p != '\0' ) {
   p=one_argcase(p,buf);
   supported.Push(buf);
  }

  gluVersion=string((const char*)gluGetString(GLU_VERSION));
  gluExtensions=string((const char*)gluGetString(GLU_EXTENSIONS));

  OUTPUT("GLU Version: %s\nGLU Extensions:\n%s---\n",gluVersion.c_str(),gluExtensions.c_str());

  // While there may be duplicates, at least we have the full suite.
 	PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");
 	if (wglGetExtString)	s = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());

  p=s;
  while ( p && *p != '\0' ) {
   p=one_argcase(p,buf);
   supported.Push(buf);
  }
//#if defined(GL_DEBUG)
  OUTPUT( "GL and WGL Extensions: \n " );
  supported.show( ",\n ", ".\n" );
//#endif
 }

 void GetContextInfo() {
  blends.Init();
  int dims[2];
  glGetIntegerv(GL_ALIASED_POINT_SIZE_RANGE,dims);
  aliasedPointLow=dims[0];
  aliasedPointHigh=dims[1];
  //glGet
  glGetFloatv(GL_ALPHA_TEST_REF,&alphaTestRef);
  glGetFloatv(GL_POINT_SIZE_MIN,&minPointSize);
  glGetFloatv(GL_POINT_SIZE_MAX,&maxPointSize);
  glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS,&numCompressedTextureFormats);
  formats.Size(numCompressedTextureFormats);
  for ( int i=0; i<numCompressedTextureFormats; i++ ) formats[i]=0;
  glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS,formats.list);
//  glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES,&implementationColorReadFormatOES);
//  glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_TYPE_OES,&implementationColorReadTypeOES);
  glGetIntegerv(GL_MAX_CLIP_PLANES,&maxClipPlanes);
  glGetIntegerv(GL_MAX_LIGHTS,&maxLights);
  glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH,&maxModelviewStackDepth);
  glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH,&maxProjectionStackDepth);
  glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maxTextureSize);
  glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH,&maxTextureStackDepth);
  glGetIntegerv(GL_MAX_TEXTURE_UNITS,&maxTextureUnits);
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&maxCombinedTextureImageUnits);
  glGetIntegerv(GL_MAX_VIEWPORT_DIMS,dims);
  glGetIntegerv(GL_MAX_DRAW_BUFFERS,&maxDrawBuffers);
  /* GLSL limit detections (implementation specific):
   * Source: http://www.opengl.org/wiki/GLSL_Uniform#Implementation_limits 
   * You should assume that each separate uniform takes up 4 components,
   * much like it would in D3D. That means a "uniform float" is 4 components,
   * a mat2x4 is 16 components (each row is 4 components), but a mat4x2 is 8 components.
   * ATI/AMD note: The ATI max component values are wrong. They are the actual number of components divided by 4.
   */
  glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS,&maxVertexUniforms);
  glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &maxVertexOutputComponents);
  glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS,&maxGeometryUniforms);
  glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,&maxFragmentUniforms);
  glGetIntegerv(GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS,&maxTessUniforms);
  glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES, &maxGeometryOutputVertices);
  glGetIntegerv(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS, &maxGeometryTotalOutputComponents);
  glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxVertexTextureImageUnits);
  glGetIntegerv(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS, &maxGeometryTextureImageUnits);
  glGetIntegerv(GL_MAX_GEOMETRY_INPUT_COMPONENTS, &maxGeometryInputComponents);
  glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS, &maxGeometryOutputComponents);
  glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &maxGeometryUniformComponents);
  glGetIntegerv(GL_MAX_VARYING_COMPONENTS, &maxVaryingComponents);
  glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVertices);
  glGetIntegerv(GL_PATCH_DEFAULT_INNER_LEVEL,&defaultPatchInner);
  glGetIntegerv(GL_PATCH_DEFAULT_OUTER_LEVEL,&defaultPatchOuter);  
  glGetIntegerv(GL_MAX_PATCH_VERTICES,&maxPatchVertices);
  glGetIntegerv(GL_MAX_TESS_GEN_LEVEL,&maxTessGenLevel);
  glGetIntegerv(GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS,&maxTessControlUniformComponents);
  glGetIntegerv(GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS,&maxTessEvaluationUniformComponents);
  glGetIntegerv(GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS,&maxTessControlTextureImageUnits);
  glGetIntegerv(GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS,&maxTessEvaluationTextureImageUnits);
  glGetIntegerv(GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS,&maxTessControlOutputComponents);
  glGetIntegerv(GL_MAX_TESS_PATCH_COMPONENTS,&maxTessPatchComponents);
  glGetIntegerv(GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS,&maxTessEvaluationOutputComponents);
  glGetIntegerv(GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS,&maxTessControlUniformBlocks);
  glGetIntegerv(GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS,&maxTessEvaluationUniformBlocks);
  glGetIntegerv(GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS,&maxTessControlTotalOutputComponents);
  glGetIntegerv(GL_MAX_TESS_CONTROL_INPUT_COMPONENTS,&maxTessControlInputComponents);
  glGetIntegerv(GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS,&maxCombinedTessEvaluationUniformComponents);
  glGetIntegerv(GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS,&maxTessEvaluationInputComponents);
  glGetIntegerv(GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS,&maxCombinedTessControlUniformComponents);
  OUTPUT("GLSL uniform limits: vert %d, frag %d, geometric %d, tess %d\n",
   maxVertexUniforms, maxFragmentUniforms, maxGeometryUniforms, maxTessUniforms );
  maxW=dims[0];
  maxH=dims[1];
  OUTPUT("GL_MAX_VIEWPORT_DIMS reports %dx%d\n", maxW,maxH);
  glGetIntegerv(GL_STENCIL_REF,&stencilRef);
  glGetIntegerv(GL_SUBPIXEL_BITS,&subpixelBits);
  glGetError(); // Ignore GL_INVALID_ENUM because OES functions may not be widely available.
 }

 void GetViewport( Cartesian *viewport ) {
  int dims[4];
  glGetIntegerv(GL_VIEWPORT,dims);
  viewport->x=dims[0];
  viewport->y=dims[1];
  viewport->w=dims[2];
  viewport->h=dims[3];
 }

 void GetScissorBox( Cartesian *scissors ) {
  int dims[4];
  glGetIntegerv(GL_SCISSOR_BOX,dims);
  scissors->x=dims[0];
  scissors->y=dims[1];
  scissors->w=dims[2];
  scissors->h=dims[3];
 }

 bool hasExtension(char *extension) {	return ( supported.find(extension) != NULL ); }

 bool InitMultisample(HINSTANCE hInstance,HWND hWnd,PIXELFORMATDESCRIPTOR pfd)
 {  
 	// See If The String Exists In WGL!
 	if (!hasExtension("WGL_ARB_multisample"))
 	{
 		arbMultisampleSupported=false;
 		return false;
 	}
 
 	// Get Our Pixel Format
 	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
 
 	if (!wglChoosePixelFormatARB)
 	{
 		// We Didn't Find Support For Multisampling, Set Our Flag And Exit Out.
 		arbMultisampleSupported=false;
 	} else {  
  	HDC hDC = GetDC(hWnd);
  	int pixelFormat;
  	UINT numFormats;
  	float fAttributes[] = {0,0};
  	int iAttributes[] = { WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
  		WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
  		WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
  		WGL_COLOR_BITS_ARB,24,
  		WGL_ALPHA_BITS_ARB,8,
  		WGL_DEPTH_BITS_ARB,16,
  		WGL_STENCIL_BITS_ARB,0,
  		WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
  		WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
  		WGL_SAMPLES_ARB, 4,						// Check For 4x Multisampling
  		0,0};
  
  	// First We Check To See If We Can Get A Pixel Format For 4 Samples
   
  	// if We Returned True, And Our Format Count Is Greater Than 1
  	if ((wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats) ? true : false) && numFormats >= 1)
  	{
  		arbMultisampleSupported	= true;
  		arbMultisampleFormat	= pixelFormat;	
  	} else {
   	// Our Pixel Format With 4 Samples Failed, Test For 2 Samples
   	iAttributes[19] = 2;
   	if ( (wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats) ? true : false) && numFormats >= 1)
   	{
   		arbMultisampleSupported	= true;
   		arbMultisampleFormat	= pixelFormat;	 
   		return true;
   	}
   }
  }	  
 	return  true;
 }

 bool InitPointSprites() {
  
    //
    // If the required extensions are present, get the addresses of  
    // functions that we wish to use...
    //
  
    if( !hasExtension( "GL_ARB_point_parameters" ) )
    {
        OUTPUT("GL_ARB_point_parameters extension was not found.\n" );
        return false;
    }
    else
    {
        glPointParameterfARB  = (PFNGLPOINTPARAMETERFARBPROC)wglGetProcAddress("glPointParameterfARB");
        glPointParameterfvARB = (PFNGLPOINTPARAMETERFVARBPROC)wglGetProcAddress("glPointParameterfvARB");

        if( !glPointParameterfARB || !glPointParameterfvARB )
        {
            OUTPUT("One or more GL_ARB_point_parameters functions were not found.\n");
            return false;
        }
    }

    OUTPUT( "Point sprites are supported.\n" );
    // Query for the max point size supported by the hardware
    glGetFloatv( GL_POINT_SIZE_MAX_ARB, &glMaxSize );
    glPointSize( glMaxSize );
    return true;
 }

 bool InitDisplayLists() {
  
    //
    // If the required extensions are present, get the addresses of  
    // functions that we wish to use...
    //
  
    if( !hasExtension( "GL_ARB_point_parameters" ) )
    {
        OUTPUT("GL_ARB_point_parameters extension was not found.\n" );
        return false;
    }
    else
    {
        glPointParameterfARB  = (PFNGLPOINTPARAMETERFARBPROC)wglGetProcAddress("glPointParameterfARB");
        glPointParameterfvARB = (PFNGLPOINTPARAMETERFVARBPROC)wglGetProcAddress("glPointParameterfvARB");

        if( !glPointParameterfARB || !glPointParameterfvARB )
        {
            OUTPUT("One or more GL_ARB_point_parameters functions were not found.\n");
            return false;
        }
    }

    OUTPUT( "Point sprites are supported.\n" );
    // Query for the max point size supported by the hardware
    glGetFloatv( GL_POINT_SIZE_MAX_ARB, &glMaxSize );
    glPointSize( glMaxSize );
    return true;
 }

 /*
 bool InitCUDA()
 {
 	int deviceCount;
 	cudaGetDeviceCount(&deviceCount);								//Get number of CUDA devices
 	if(deviceCount == 0)
 	{
 		printf("No CUDA enabled device found.\n");
 		return false;
 	}
 	else
 	{
 		int device = cutGetMaxGflopsDeviceId();						//Get device we are going to use
 		cudaDeviceProp deviceProperties;
         cudaGetDeviceProperties(&deviceProperties, device);			//Get it's properties
 		OUTPUT("Running On:\n Device %d: %s\n\n", device, deviceProperties.name);
	 }

 	return true;
 }*/

 bool InitVBO() {
  
  // check VBO is supported by your video card
  if(hasExtension("GL_ARB_vertex_buffer_object"))
  {
   // get pointers to GL functions
   glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
   glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
   glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
   glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
   glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
   glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
   glMapBufferARB = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
   glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");

   // check once again VBO extension
   if(glGenBuffersARB && glBindBufferARB && glBufferDataARB && glBufferSubDataARB &&
      glMapBufferARB && glUnmapBufferARB && glDeleteBuffersARB && glGetBufferParameterivARB)
   {
       vboSupported = true;
       OUTPUT("Video card supports GL_ARB_vertex_buffer_object.\n");
   }
   else
   {
       vboSupported = false;
       OUTPUT("Video card does NOT support GL_ARB_vertex_buffer_object.\n");
   }
  }
  else
  {
      vboSupported = false;
      OUTPUT("Video card does NOT support GL_ARB_vertex_buffer_object.\n");
  }
  return vboSupported;
 }

 void InitFBO() {  
  if ( ARB_FBO=hasExtension("GL_ARB_framebuffer_object") ) {
  //glGenFramebuffersARB         = (PFNGLGENFRAMEBUFFERSPROC)         wglGetProcAddress("glGenFramebuffersARB");
  //glBindFramebufferARB         = (PFNGLBINDFRAMEBUFFERPROC)         wglGetProcAddress("glBindFramebufferARB");
  //glGenRenderbuffersARB        = (PFNGLGENFRAMEBUFFERSPROC)         wglGetProcAddress("glGenRenderbuffersARB");
  //glBindRenderbufferARB        = (PFNGLBINDRENDERBUFFERPROC)        wglGetProcAddress("glBindRenderbufferARB");
  //glRenderbufferStorageARB     = (PFNGLRENDERBUFFERSTORAGEPROC)     wglGetProcAddress("glRenderbufferStorageARB");
  //glFramebufferRenderbufferARB = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) wglGetProcAddress("glFramebufferRenderbufferARB");
  //glFramebufferTexture1DARB    = (PFNGLFRAMEBUFFERTEXTURE1DPROC)    wglGetProcAddress("glFramebufferTexture1DARB");
  //glFramebufferTexture2DARB    = (PFNGLFRAMEBUFFERTEXTURE2DPROC)    wglGetProcAddress("glFramebufferTexture2DARB");
  //glFramebufferTexture3DARB    = (PFNGLFRAMEBUFFERTEXTURE3DPROC)    wglGetProcAddress("glFramebufferTexture3DARB");
  //glFramebufferTextureLayerARB = (PFNGLFRAMEBUFFERTEXTURELAYERPROC) wglGetProcAddress("glFramebufferTextureLayerARB");
   OUTPUT("GL_ARB_framebuffer_object is present, and:\n" );
   if ( !(ARB_FBO_multisample=hasExtension("GL_ARB_multisample")) )
    OUTPUT("GL_ARB_multisample is not supported.\nMultisampling disabled.\n");
   else
   if ( ARB_FBO_blit=hasExtension("GL_ARB_framebuffer_blit") ) {
    glGetIntegerv(GL_MAX_SAMPLES, &fboMaxSamples);
    OUTPUT("GL_ARB_framebuffer_multisample supports %d samples.\n", fboMaxSamples );
   } else
    OUTPUT("GL_ARB_framebuffer_blit is not supported.\nMultisampling disabled.\n");
   if ( ARB_FBO_npot=hasExtension("GL_ARB_texture_non_power_of_two") ) OUTPUT( "NPOT textures are supported.\n" );
   else OUTPUT("FBO must be square, POT.\n");
   if ( ARB_packed_depth_stencil=hasExtension("ARB_packed_depth_stencil") ) OUTPUT( "GL_ARB_packed_depth_stencil is supported.\n");
   if ( ARB_FBO_rect=hasExtension("GL_ARB_texture_rectangle") ) OUTPUT("GL_ARB_texture_rectangle is supported.\n" );
   glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &fboMaxColorAttachments);
   OUTPUT( "FBOs are supported with %d samples and %d attachments\n", fboMaxSamples, fboMaxColorAttachments );
   if ( ARB_draw_buffers=hasExtension("GL_ARB_draw_buffers") ) OUTPUT( "Multiple render targets are supported.\n");
   if ( EXTX_mixed_framebuffer_formats=hasExtension("GL_EXTX_mixed_framebuffer_formats") ) OUTPUT("Mixed format FBOs are allowed (slow?).\n");
  }

  if ( EXT_FBO=hasExtension("GL_EXT_framebuffer_object") ) {
  //glGenFramebuffersEXT         = (PFNGLGENFRAMEBUFFERSEXTPROC)         wglGetProcAddress("glGenFramebuffersEXT");
  //glBindFramebufferEXT         = (PFNGLBINDFRAMEBUFFEREXTPROC)         wglGetProcAddress("glBindFramebufferEXT");
  //glGenRenderbuffersEXT        = (PFNGLGENFRAMEBUFFERSEXTPROC)         wglGetProcAddress("glGenRenderbuffersEXT");
  //glBindRenderbufferEXT        = (PFNGLBINDRENDERBUFFEREXTPROC)        wglGetProcAddress("glBindRenderbufferEXT");
  //glRenderbufferStorageEXT     = (PFNGLRENDERBUFFERSTORAGEEXTPROC)     wglGetProcAddress("glRenderbufferStorageEXT");
  //glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC) wglGetProcAddress("glFramebufferRenderbufferEXT");
  //glFramebufferTexture1DEXT    = (PFNGLFRAMEBUFFERTEXTURE1DEXTPROC)    wglGetProcAddress("glFramebufferTexture1DEXT");
  //glFramebufferTexture2DEXT    = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)    wglGetProcAddress("glFramebufferTexture2DEXT");
  //glFramebufferTexture3DEXT    = (PFNGLFRAMEBUFFERTEXTURE3DEXTPROC)    wglGetProcAddress("glFramebufferTexture3DEXT");
  //glFramebufferTextureLayerEXT = (PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC) wglGetProcAddress("glFramebufferTextureLayerEXT");
   OUTPUT("GL_EXT_framebuffer_object is present, and:\n" );
   if ( !(EXT_FBO_multisample=hasExtension("GL_EXT_framebuffer_multisample")) )
    OUTPUT("GL_EXT_framebuffer_multisample is not supported.\nMultisampling disabled.\n");
   else
   if ( EXT_FBO_blit=hasExtension("GL_EXT_framebuffer_blit") ) {
    glGetIntegerv(GL_MAX_SAMPLES_EXT, &fboMaxSamples);
    OUTPUT("GL_EXT_framebuffer_multisample supports %d samples.\n", fboMaxSamples );
   } else
    OUTPUT("GL_EXT_framebuffer_blit is not supported.\nMultisampling disabled.\n");
   if ( EXT_FBO_npot=hasExtension("GL_ARB_texture_non_power_of_two") ) OUTPUT( "NPOT textures are supported.\n" );
   else OUTPUT("FBO must be square, POT.\n");
   if ( EXT_packed_depth_stencil=hasExtension("EXT_packed_depth_stencil") ) OUTPUT( "GL_EXT_packed_depth_stencil is supported.\n");
   if ( EXT_FBO_rect=hasExtension("GL_ARB_texture_rectangle") ) OUTPUT("GL_ARB_texture_rectangle is supported.\n" );
   glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &fboMaxColorAttachments);
   OUTPUT( "FBOs are supported with %d samples and %d attachments\n", fboMaxSamples, fboMaxColorAttachments );
   if ( ARB_draw_buffers=hasExtension("GL_ARB_draw_buffers") ) OUTPUT( "Multiple render targets are supported.\n");
   if ( EXTX_mixed_framebuffer_formats=hasExtension("GL_EXTX_mixed_framebuffer_formats") ) OUTPUT("Mixed format FBOs are allowed (slow?).\n");
  }
 }

 void InitShaders() {
  if (GLEW_VERSION_2_0) {
   hasShaders=true;
   OUTPUT("GLSL is supported, proceeding\n");
  } else {
   if ( hasExtension("GL_ARB_shader_objects")
     && hasExtension("GL_ARB_shading_language_100")
     && hasExtension("GL_ARB_vertex_shader")
     && hasExtension("GL_ARB_fragment_shader") ) {
    hasShaders=true;
    OUTPUT("GL_ARB_shader_objects, shading_language_100, vertex_shader, fragment_shader found\n");
  	OUTPUT("GLSL is supported, proceeding\n");
   } else OUTPUT("GLSL is not supported. Exit\n");
  }
  if ( hasShaders ) {
   if ( hasExtension("GL_ARB_geometry_shader4")
     || hasExtension("GL_EXT_geometry_shader4")
     || hasExtension("GL_NV_geometry_shader4") ) {
    OUTPUT("GLSL geometry shaders are supported.\n");
    hasGeometryShaders=true;
   }
   if ( hasExtension("GL_ARB_tessellation_shader") ) {
    OUTPUT("GLSL tessellation shaders are supported.");
    hasTesselationShaders=true;
   }
  }
 }

 void InitDeferredShading() {
  if ( hasExtension("GL_ARB_fragment_program")
    && hasExtension("GL_ARB_vertex_buffer_object")
    && hasExtension("GL_ARB_vertex_program")
    && hasExtension("GL_NV_float_buffer")
    && hasExtension("GL_NV_fragment_program")
    && hasExtension("GL_NV_texture_rectangle")
    && hasExtension("WGL_ARB_pbuffer")
    && hasExtension("WGL_ARB_pixel_format") ) {
   deferredShading=true;
   OUTPUT("Deferred shading supported.\n");
  } else
  OUTPUT("Deferred shading not supported.\n");
 }

 void InitPerPixelLighting() {
  if ( hasExtension("GL_EXT_compiled_vertex_array")
    && hasExtension("GL_EXT_texture_edge_clamp")
    && hasExtension("GL_ARB_multitexture")
    && hasExtension("GL_NV_register_combiners")
    && hasExtension("GL_NV_vertex_program") ) {
   GLint maxTextureUnitsARB;
  	glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &maxTextureUnitsARB);
  	if(maxTextureUnitsARB<4) 
		 OUTPUT("Per-pixel lighting requires at least 4 texture units, %d found.\n", maxTextureUnitsARB);
   else {
    perPixelLighting=true;
    OUTPUT("Per-pixel lighting supported.\n");
   }
  } else
  OUTPUT( "Per-pixel lighting not supported.\n");
 }

 void InitGeometryInstancing() {
  if ( hasExtension("GL_ARB_draw_instanced") || hasExtension("GL_EXT_draw_instanced") ) canInstance=true;
 }

 bool Init( HINSTANCE hinstance, int color_bits ) {
  HWND hwnd = CreateWindow("static", null, 0,0,0,0,0,null,null,null,null);
  HDC hdc = GetDC(hwnd);
  HGLRC glrc;
  PIXELFORMATDESCRIPTOR pfd;
  int format;

  if ( !hdc ) return false;

  memset(&pfd,0,sizeof(PIXELFORMATDESCRIPTOR));
  pfd.nSize= (short) sizeof(pfd);
  pfd.nVersion=1;
  pfd.dwFlags=PFD_DRAW_TO_WINDOW /*PFD_DRAW_TO_BITMAP*/ | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = (BYTE) color_bits;
  pfd.cDepthBits = 24;
  pfd.iLayerType = PFD_MAIN_PLANE;
  format = ChoosePixelFormat(hdc, &pfd);
  DescribePixelFormat(hdc, format, sizeof(pfd), &pfd);
  
  if ( pfd.cColorBits <= 8 ) return false;

  SetPixelFormat(hdc, format, &pfd);
  glrc = wglCreateContext(hdc);

  if ( !glrc ) return false;

  wglMakeCurrent( hdc, glrc );

  GLenum err=glewInit();
  if(err!=GLEW_OK)
  {
   //Problem: glewInit failed, something is seriously wrong
   OUTPUT("Error: %s", (const char *) glewGetErrorString(err));
   return false;
  }

  FindSupportedExtensions();
  GetContextInfo();

  this->InitFBO();

  WGL_EXT_swap_control_supported=hasExtension("WGL_EXT_swap_control");
  if (WGL_EXT_swap_control_supported){
   wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglxGetProcAddress("wglSwapIntervalEXT");
   wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC) wglxGetProcAddress("wglGetSwapIntervalEXT");
  }

  this->InitMultisample( hinstance, hwnd, pfd );
  this->InitVBO();
  //this->InitCUDA();

  // Used by Fluid2d/3d
  glswInit();
  glswAddPath("../", ".glsl");
  glswAddPath("./", ".glsl");
  glswAddPath("data/shaders/", ".glsl");
  glswAddDirective("*", "#version 150");

  this->InitShaders();
  this->InitDeferredShading();
  this->InitPerPixelLighting();
  GL_ARB_POINT_SPRITES=this->InitPointSprites();
  getInfo();

  wglMakeCurrent(null, null);
  wglDeleteContext( glrc );
  ReleaseDC(hwnd,hdc);
  DestroyWindow(hwnd);
  
  CheckRequirements();
  return true;

 }

 // If we don't meet our minimums, head toward exit.
 void CheckRequirements() {
  double GLVer=(double)majorGL + (double)minorGL/10.0;
  double GLSLVer=(double)majorGLSL + (double)minorGLSL/10.0;
  if ( GLVer < GLRequired || GLSLVer < GLRequired ) {
#if defined(WIN32)
   int result = MessageBox(NULL,
    "Your graphics card does not meet the minimum required version of OpenGL. The application must exit.", 
    "OpenGL Version Mismatch",
    MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
   switch ( result ) { case IDCONTINUE: exit(1); break; default: exit(1); break; }
#endif
  }
 }

 ///////////////////////////////////////////////////////////////////////////////
// extract openGL info
// This function must be called once after GL rendering context opened.
///////////////////////////////////////////////////////////////////////////////
 bool getInfo()
 {
    char* str = 0;
    char* tok = 0;

    // get vendor string
    str = (char*)glGetString(GL_VENDOR);
    if(str) this->vendor = str;                  // check NULL return value
    else return false;

    if ( !str_infix("nvidia",str) ) {
     isNVIDIA=true;
     OUTPUT("NVIDIA graphics card detected.\n");
    }
    else if ( !str_infix("ati",str) ) {
     isATI=true;
     OUTPUT("ATI graphics card detected.\n");
    } else
    if ( !str_infix("intel",str) ) {
     isINTEL=true;
     OUTPUT("INTEL graphics card detected.\n");
    } else
    OUTPUT("Could not determine card manufacturer.\n" );

    OUTPUT( "%s ", str );

    // get renderer string
    str = (char*)glGetString(GL_RENDERER);
    if(str) this->renderer = str;                // check NULL return value
    else return false;

    OUTPUT( "%s ", str );

    // get version string
    str = (char*)glGetString(GL_VERSION);
    if(str) this->version = str;                 // check NULL return value
    else return false;

    OUTPUT( "%s\n", str );

    GetGL_and_GLSLVersion();
    OUTPUT( "OpenGL ver %d.%d / GLSL ver %d.%d\n", majorGL, minorGL, majorGLSL, minorGLSL );

    glGetIntegerv(GL_RED_BITS, &this->redBits);
    glGetIntegerv(GL_GREEN_BITS, &this->greenBits);
    glGetIntegerv(GL_BLUE_BITS, &this->blueBits);
    glGetIntegerv(GL_ALPHA_BITS, &this->alphaBits);
    glGetIntegerv(GL_DEPTH_BITS, &this->depthBits);
    glGetIntegerv(GL_STENCIL_BITS, &this->stencilBits);
    glGetIntegerv(GL_MAX_LIGHTS, &this->maxLights);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &this->maxTextureSize);
    glGetIntegerv(GL_MAX_CLIP_PLANES, &this->maxClipPlanes);
    glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &this->maxModelViewStacks);
    glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &this->maxProjectionStacks);
    glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, &this->maxAttribStacks);
    glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, &this->maxTextureStacks);
   	glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &this->maxDegreeOfAnisotropy);
    return true;
 }

 void GetGL_and_GLSLVersion() {
  if (!majorGLSL && !minorGLSL) {
   if (!majorGL && !minorGL) {
    const char *pszVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    if (pszVersion) sscanf_s(pszVersion, "%d.%d", &majorGL, &minorGL);
   }
   if (majorGL >= 2) {
    const char *pszShaderVersion = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    if (pszShaderVersion) sscanf_s(pszShaderVersion, "%d.%d", &majorGLSL, &minorGLSL);
   } else {
    const char *pszExtension = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));
    if (pszExtension) {
     if (strstr(pszExtension, "GL_ARB_shading_language_100")) {
      majorGLSL = 1;
      minorGLSL = 0;
     }
    }
   }
  }
 }


};

extern GLSetup gl;

// GL hacks below this line
extern int Zeroi;
extern char gl_info[1024];
extern char old_gl_info[1024];

void glPush();
void glPush( bool dont_load_identity );
void glPop();
void Point3dto2d( double obX, double obY, double obZ, double *tx, double *ty, double *tz );
void Point3dto2d( int x, int y, int w, int h,  double obX, double obY, double obZ, double *tx, double *ty, double *tz );
void glToScreen( double *x, double *y, double *z, double *w );
void glToScreen( int vx, int vy, int vw, int vh, double *x, double *y, double *z, double *w );

#include "ZIndexed.h"
struct Crayon;
class GLTexImage {
public:
 ZIndexed<GLubyte> pixel;
 GLint texture, width, height, format, level, direction, stride;
 Zbool valid;
 GLTexImage(GLint texture_2d_id, GLint level=0, GLint format=GL_RGBA, GLint stride=4, GLint direction=1) {
  this->format = format;
  this->direction = direction;
  this->stride = stride;
  this->level = level;
  height = 0;
  width = 0;
  texture = texture_2d_id;
  glBindTexture(GL_TEXTURE_2D, texture);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, &width);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, &height);
  if (direction != 0 && stride != 0) {
   pixel.Size(width*height*stride);
   glGetTexImage(GL_TEXTURE_2D, level, GL_RGBA, GL_UNSIGNED_BYTE, pixel.list);
   valid = true;
  }
  glBindTexture(GL_TEXTURE_2D, 0);
 }
 // Offset from Center
 Crayon Probe(int x, int y);
 // Offset from top left
 Crayon Probe(int x, int y, having fromTopLeft);
 // Offset from top left
 Crayon Probe(int x, int y,bool flippedx, bool flippedy);
};


inline void Blending( Blends blend );

void FullscreenQuad(void);
void FullscreenQuadXInverted(void);
void FullscreenQuadYInverted(void);
void FullscreenQuadXYInverted(void);

#if !defined(GL_DEBUG)
#define GL_Assert(s) { /* s */ }
#define GL_Assert_Param(s,t) { /* s, t */ }
#define glReportError( v ) { /* v */ }
#define GL_Report(n) { /* n */ }
#define GL_Revert() { }
#define GL_Error { }
#else
#define GL_Assert(x) FORMAT( gl_info,1024, x )
#define GL_Assert_Param(x,y) FORMAT( gl_info,1024, x,y )
void glReportError( GLenum err );
#define GL_Report(n) { FORMAT( old_gl_info,1024, "%s", gl_info ); FORMAT( gl_info,1024, n ); glReportError( glGetError() ); }
#define GL_Revert() FORMAT( gl_info,1024, "%s", old_gl_info )
#define GL_Error glReportError(glGetError())
#endif

/*
void OpenGLApp::screenShot(){
	char path[MAX_PATH];
	SHGetSpecialFolderPath(NULL, path, CSIDL_DESKTOPDIRECTORY, FALSE);

	FILE *file;
	int pos = strlen(path);
#ifndef NO_PNG
	strcpy(path + pos, "\\ScreenShot00.png");
#else
	strcpy(path + pos, "\\ScreenShot00.tga");
#endif // NO_PNG
	pos += 11;
	
	int i = 0;
	do {
		path[pos]     = '0' + (i / 10);
		path[pos + 1] = '0' + (i % 10);
		
		if (file = fopen(path, "r")){
			fclose(file);
		} else {
			CRect rect;
			m_pMainWnd->GetClientRect(rect);
			int w = rect.right - rect.left;
			int h = rect.bottom - rect.top;

			unsigned char *pixels  = new unsigned char[w * h * 3];
			unsigned char *flipped = new unsigned char[w * h * 3];;

			glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, flipped);
			for (int y = 0; y < h; y++){
				memcpy(pixels + y * w * 3, flipped + (h - y - 1) * w * 3, w * 3);				
			}
			delete flipped;

			Image img;

			img.loadFromMemory(pixels, w, h, FORMAT_RGB8, true);
			img.saveImage(path, true);

			break;
		}
		i++;
	} while (i < 100);

}
*/
