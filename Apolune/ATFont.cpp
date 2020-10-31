#include "ATFont.h"

Zint ATFontCached_items;

ATexFontLibrary atexfonts;

Zp<ATexFont> defaultATexFont;

Zp<ATexFont> abytes,acalibri,adigicaps,ametalFont,amicroFont,atermFont,aglowTermFont;

#include "TexFont.h"

void LoadATexFonts() {
 atexfonts.LoadFonts();
}

void ATexFontLibrary::LoadFonts() {
 bytes->afont=( abytes=Load("bytes","data/images/atfonts/bytes.txt") );
 calibri->afont=( acalibri=Load("calibri","data/images/atfonts/calibri.txt") );
 digicaps->afont=( adigicaps=Load("digicaps","data/images/atfonts/digicaps.txt") );
// silverFont->afont=( ametalFont=Load("metalfont","data/images/atfonts/metalfont.txt") );
 microFont->afont=( amicroFont=Load("microfont","data/images/atfonts/microfnt.txt") );
 defaultATexFont=( termFont->afont = ( atermFont=Load("term","data/images/atfonts/term.txt") ) );
 glowTermFont->afont = ( aglowTermFont=Load("glowterm","data/images/atfonts/glowterm.txt") );
}

ATexFont *atexFontByName( const char *s ) { return atexfonts.find(s); }


int ATexFontCached::Create( ATexFont *atf, const char *s, double fw, double fh, double cs, double ls ) {
 ATFontCached_items++;
 if ( !atf->image ) return -1;
 content=s;
 if ( content.length < 1 ) return -1;
 this->fw=fw;
 this->fh=fh;
 this->cs=cs;
 this->ls=ls;
 Cartesiands quads;
 Cartesiands texcoords;
 atf->BoxBounds(&bounds,s,fw,fh,cs,ls);
 atf->Calculate(bounds,&quads,&texcoords,s,(double)fw,(double)fh,(double)cs,(double)ls);
 element=art.stash.GenQuads(&quads,&texcoords);
 return element;
}

ATexFontCached *ATexFontCache::Store( ATexFont *atf, int *hc, const char *s, double fw, double fh, double cs, double ls ) {
 *hc=GetHashCode(s,fw,fh,cs,ls);
 ATexFontCached *atfc=new ATexFontCached;
 atfc->Create(atf,s,fw,fh,cs,ls);
 table[(*hc)].Append(atfc);
 return atfc;
}

ATexFontCached *ATexFontCache::GetOrStore( ATexFont *atf, const char *s, double fw, double fh, double cs, double ls ) {
 int hc=0;
 ATexFontCached *atfc=find(&hc,s,fw,fh,cs,ls);
 if ( !atfc ) atfc=Store(atf,&hc,s,fw,fh,cs,ls);
 return atfc;
}

void ATexFont::Write( const char *s, Crayon tint, double x, double y, Blends b, double fontW, double fontH, double lineSpace, double charSpaceKern ) {
 if ( !image ) return;
 if ( s[0] == '\0' ) return;
 ATexFontCached *atfc=cache.GetOrStore( this, s, fontW, fontH, charSpaceKern, lineSpace );
 if ( atfc ) {
  art.Element(image,tint,b,atfc->element,x,y,atfc->bounds.w,atfc->bounds.h);
 }
}

double ATexFont::Width( const char *s, double charSpaceKern, double fontW ) {
 double cw=charSpaceKern*fontW;
 double wx=0.0;
 double longestw=0;
 const char *p=s;
 while ( *p != '\0' ) {
  if ( *p == '\r' ) { }
  else if ( *p == '\n' ) { if ( longestw < wx ) longestw=wx; wx=0.0; }
  else if ( *p == ' ' ) { wx+=spaceWidth * fontW; }
  else { wx+=fontW+cw; }
  p++;
 }
 if ( longestw < wx ) longestw=wx;
 return longestw;
}

std::string ATexFont::FitWidth( const char *s, double maxWidth, double fontW, double fontH, double lineSpace, double charSpaceKern, bool onlyOneLine ) {
 return string("");
}

void ATexFont::GetTextInfo( int *out_lines, int *out_widest, const char *s ) {
 int lines=1;
 int longest=0;
 int columns=0;
 const char *p=s;
 while ( *p != '\0' ) {
  if ( *p == '\r' ) { p++; continue; }
  else if ( *p == '\n' ) {
   lines++;
   if ( longest < columns ) longest=columns;
   columns=0;
   p++;
   continue;
  }
  columns++;
  p++;
 }
 if ( longest < columns ) longest=columns;
 *out_lines=lines;
 *out_widest=longest;
}

void ATexFont::BoxBounds( Cartesiand *out, const char *s, double fontW, double fontH, double lineSpace, double charSpaceKern ) {
 int lines=0,widest=0;
 GetTextInfo(&lines,&widest,s);
 out->SetRect( 0.0, 0.0,
  (double) ( widest * (fontW + charSpaceKern) ),
  (double) ( lines * (fontH + lineSpace) )
 );
}

void ATexFont::Calculate( Cartesiand &bounds, Cartesiands *quads, Cartesiands *tcoords, const char *s, double fw, double fh, double cs, double ls ) {
 const char *p=s;
 double sx=0.0;
 double sy=0.0;
 double cw=fw/bounds.w;  // char width 1.0
 double _cs=cs/bounds.w; // char space 1.0
 double _ls=ls/bounds.h; // line space 1.0
 double lh=fh/bounds.h;  // line height 1.0
 double sw=(spaceWidth*fw)/bounds.w; // scaled space width in 1.0 space
 while ( *p != '\0' ) {
  if ( *p == '\r' ) {}
  else if ( *p == '\n' ) {
   sx=0.0;
   sy += lh+_ls;
  } else if ( *p == ' ' ) {
   sx+=sw;
  } else {
   char seeking=capsonly?UPPER(*p):*p;
   ATexGlyph *g=glyph(seeking);
   if ( g && g->c == seeking ) {
    Cartesiand *quad=new Cartesiand;
    Cartesiand *tc=new Cartesiand;
    quad->SetRect(sx,sy,cw,lh);
    quads->Append(quad);
    tc->SetRect(g->texcoords.x,g->texcoords.y,g->texcoords.w,g->texcoords.h);
    tcoords->Append(tc);
   }
   sx+=cw+_cs;
  }
  p++;
 }
}
