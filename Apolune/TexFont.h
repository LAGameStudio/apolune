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
#pragma warning (disable:4267) // conversion warning

#include <Windows.h>
#include <stdio.h>
#include <string>

#include "macros.h"

#include "ListItem.h"
#include "LinkedList.h"
#include "Strings.h"

#include "GLImage.h"
#include "GLWindow.h"

#include "Art.h"
#include "TextureLibrary.h"

/* Instructions on creating one of these in Photoshop:

1) After filling with black, create a layer with overlayed text in white (or a grayscale
   or even full color image with blending) and choose a left margin such that each character
   has a small margin to the side of the image.
   NOTE: Can be transparent now.

2) Create an Action in photoshop called Duplicate that contains
   simply a recorded Layer duplication.  When recording the macro,
   type a single letter such as D or O for the name of the duplicated layer.
   Stop recording the macro.  Repeatedly invoke action approximately 88 times.

3) Change names to letters selected from this list: ABCDEFGHJKLMNOPQRSTUVWXYZabcdefghijklmnop0123456789
and then single word names for the other punctuation marks, like "pipe", "comma", "at", "dollar" etc.
In the case of lower case characters, add an underline (_) prefix like _a

4) Use Export Layers to Files to export without file numbers and voila you have a set of PNGs
   that can be folded into a font.  Trim each file to a precise width, leaving the full height
   of the original dimensions (or export trimmed and then correct the height compensating for
   glyph WinDescent).  Make sure your "space.png" is a sane width.

5) Create data/images/fonts/fontfilename.txt to map images to glyph identities relative
glyph { ' ' data/images/fonts/calibri/space.png }
glyph { 'A' data/images/fonts/calibri/A.png }
.
.
.
*/

class TexGlyph {
public:
 Zpointer<GLImage> glyph;
 TexGlyph() {
 }
};

class ATexFont;
ONE(TexFont,{
 glyphs.Size(256);
 baselineOffset=0.75f;
})
 Zp<ATexFont> afont;
 Indexed<TexGlyph> glyphs;
 Zbool relative, nearest, fixed;
 Zfloat widest;
 Zfloat baselineOffset;
 Blends blend;
 Cartesian fixedSize;
 TexFont( string s ) : ListItem() {
  name=s;
  glyphs.Size(256);
  baselineOffset=0.75f;
  blend=additive;
 }
 GLImage *glyph( char c ) {
  return glyphs[URANGE(0,(int) c,(int)glyphs.length-1)].glyph;
 }
 KEYWORDS_POST({
  KEYWORD("name",{name=w;})
  else TAGWORD("relative",relative)
  else TAGWORD("nearest",nearest)
  else NUMWORD("baseline",baselineOffset)
  else BLENDWORD("blend",blend)
  else SUBWORD("fixed",fixedSize)
  else KEYWORD("glyph",{
   Zstring interior(w);
   const char *v=interior.Next_case();
   char t=(*v);
   v=interior.Next();
   OUTPUT("\nGLYPH: %c %s",t,v);
   if ( nearest ) {
    Push(t,library.Load(v,GL_NEAREST,GL_NEAREST,GL_REPEAT,GL_REPEAT));
   } else Push(t,library.Load(v));
  })
  NEXTWORD;
 },{
  if ( fixedSize.x != 0 && fixedSize.y != 0 ) fixed=true;
 })
 WORDKEYS({
 })
 void Load( const char *font ) {
  OUTPUT("TexFont: Loading font `%s`.\n", font);
  string file=file_as_string(font);
  fromString(file.c_str());
  OUTPUT("\n");
 }
 void Push( char c, GLImage *b ) { 
  if ( !b ) { OUTPUT("Error: No image provided for glyph `%c`, ignoring.\n", c ); return; }
  if ( glyphs[(unsigned int) c].glyph != null )
   OUTPUT( "\n =-> Error: '%c' in TexFont '%s' already in use, discarding second requested definition.\n", c, name.c_str() );
  else glyphs[(unsigned int) c].glyph = b;
  if ( relative && widest < b->width ) widest=(float) (b->width);
 }
 void Write(GLWindow *win, char *t, Crayon c, Blends blend, float W, float H, int x, int y) {
  int i,len=strlen(t);
  char *p=t;
  char ch='\0'; // Is this initialized and used properly?
  int G;
  GLImage *g=glyph('A');
  int h=(int) ((float)g->height*H);
  int w;
  if ( relative ) {
   for ( i=0; i<len; i++ ) {
    G=(int) ch;
    if ( glyphs[G].glyph ) {
     w=(int) (glyphs[G].glyph->width*W);
     Stretchi(win,glyph(*p),c,1.0f,blend,x,y,w,h);
     t++;
    }
   }
  } else {
   w=(int) ((float)g->width*W);
   for ( x=0; x<len; x++ ) {
    Stretchi(win,glyph(*p),c,1.0f,blend,x,y,w,h);
    t++;
   }
  }
 }
 void Write(GLWindow *win, char *t, Crayon c, Blends blend, float S, int x, int y) {
  int i,len=strlen(t);
  char *p=t;
  int G;
  GLImage *g=glyph('A');
  if ( !g ) return;
  int h=(int) ((float)g->height*S);
  int w;
  if ( relative ) {
   for ( i=0; i<len; i++ ) {
    G=(int) *p;
    if ( glyphs[G].glyph ) {
     w=(int) (glyphs[G].glyph->width*W);
     Stretchi(win,glyph(*p),c,1.0f,blend,x,y,w,h);
     p++;
    }
   }
  } else {
   w=(int) ((float)g->width*S);
   for ( i=0; i<len; i++ ) {
    Stretchi(win,glyph(*p),c,1.0f,blend,x,y,w,h);
    p++;
   }
  }
 }
MANY(TexFont,TexFontHandle,TexFontHandles,"TexFont",TexFontLibrary,{})
 KEYSWORDSGroups("TexFont",TexFont)
 void LoadFonts();
 TexFont *Load(const char *filename) { return Load(filename,filename,false); }
 TexFont *Load(const char *filename,bool relative) { return Load(filename,filename,relative); }
 TexFont *Load(const char *name,const char *filename) { return Load(name,filename,false); }
 TexFont *Load(const char *name,const char *filename,bool relative) {
  TexFont *f=new TexFont;
  f->name=name;
  f->relative=true;
  f->fromString(file_as_string(filename).c_str());
  Append(f);
  return f;
 }
 TexFont *find(const char *name) {
  FOREACH(TexFont,f) if ( !str_cmp(f->name.c_str(),name) ) return f;
  return null;
 }
DONE(TexFont);

extern TexFontLibrary texfonts;

void PrintTexFont( GLWindow *surface, TexFont *tf, Crayon tint, Blends blend, int x, int y, int fw, int fh, int ls, Strings *t );
void PrintTexFont( TexFont *tf, Crayon tint, Blends blend, int x, int y, int fw, int fh, int ls, Strings *t );
void WriteTexFont( GLWindow *surface, TexFont *tf, Crayon tint, Blends blend, int x, int y, int fw, int fh, int ls, const char *t, int kern=0 );
void WriteTexFont( TexFont *tf, Crayon tint, float alpha, Blends blend, int x, int y, int fw, int fh, int ls, const char *t, int kern );
void WriteTexFontColorCodes( TexFont *tf, Crayon tint, float alpha, Blends blend, int x, int y, int fw, int fh, int ls, const char *t, int kern );
string TexFontColorCodesWordWrap( const char *txt, int linelen );
void WriteTexFontWonky( TexFont *tf, Crayon tint, float alpha, float Wonkyness, int wonkseed, Blends blend, int x, int y, int fw, int fh, int ls, const char *t, int kern );
void WriteTexFont( TexFont *tf, Crayon tint, Blends blend, int x, int y, int fw, int fh, int ls, const char *t, int kern=0 );
int WidthTexFont( TexFont *tf, int fw, int kern, const char *t );
string TexFitWidth( TexFont *tf, int charSpace, int w, string in );
string TexFitWidth( TexFont *tf, int charSpace, int w, int fw, string in );
void TexFontBoxBounds( Cartesian *out, TexFont *tf, int lineSpace, int charSpaceKern, int fontW, int fontH, const char *s, char forcednewline='\\' );
void TexFontColorCodesBoxBounds( Cartesian *out, TexFont *tf, int lineSpace, int charSpaceKern, int fontW, int fontH, const char *s, char forcednewline='\\' );

void Text( TexFont *tf, Crayon tint, Blends blend, int x, int y, int fw, int fh, int ls, int cs, const char *t );
void Text( TexFont *tf, Crayon tint, Blends blend, int x, int y, int w, int fw, int fh, int ls, int cs, const char *t );
void Text( TexFont *tf, Crayon tint, Blends blend, int x, int y, int w, int lines, int fw, int fh, int ls, int cs, const char *t );

extern Zp<TexFont> defaultTexFont;

extern Zp<TexFont> goldFont,silverFont,calibri,bytes,digicaps,
        microFont,neonNumbers,redNeonNumbers,
        goldNeonNumbers,alarmClockNumbers,nexaBold,termFont,glowTermFont;

void LoadTexFonts();

TexFont *texFontByName( const char *s );