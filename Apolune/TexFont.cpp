#include "TexFont.h"

#include "ATFont.h"
#include "FastGUI.h"

void TexFontLibrary::LoadFonts() {
// alarmClockNumbers=Load("alarmclock","data/images/fonts/alarmclock.txt");
// neonNumbers=Load("neonnumbers","data/images/fonts/neonnumbers.txt");
// goldNeonNumbers=Load("goldneon","data/images/fonts/goldneonnumbers.txt");
 redNeonNumbers=Load("redneon","data/images/fonts/redneonnumbers.txt");
 defaultTexFont=(microFont=Load("microfont","data/images/fonts/microfont.txt",true));
 digicaps=Load("digicaps","data/images/fonts/digicaps.txt");
// silverFont=Load("silverfont","data/images/fonts/silverfont.txt");
 calibri=Load("calibri","data/images/fonts/calibri.txt",true);
 bytes=Load("bytes","data/images/fonts/bytes.txt",true);
// nexaBold=Load("nexa-bold","data/images/fonts/nexabold.txt",true);
 termFont=Load("term","data/images/fonts/term.txt");
 glowTermFont=Load("glowterm","data/images/fonts/glowterm.txt");
}

void PrintTexFont( GLWindow *surface, TexFont *tf, Crayon tint, Blends blend, int x, int y, int fw, int fh, int ls, Strings *t ) {
 String *s=(String *) t->first;
 int i=0;
 while ( s ) {
  WriteTexFont( surface, tf, tint, blend, x,y+i*(fh+ls),fw,fh,ls, (char *) s->s.c_str() );
  i+=linefeeds((char *)s->s.c_str())+1;
  s=(String *) s->next;
 }
}

void PrintTexFont( TexFont *tf, Crayon tint, Blends blend, int x, int y, int fw, int fh, int ls, Strings *t ) {
 String *s=(String *) t->first;
 int i=0;
 while ( s ) {
  WriteTexFont( tf, tint, blend, x,y+i*(fh+ls),fw,fh,ls, (char *) s->s.c_str() );
  i+=linefeeds((char *)s->s.c_str())+1;
  s=(String *) s->next;
 }
}

void WriteTexFont( GLWindow *surface, TexFont *tf, Crayon tint, Blends blend, int x, int y, int fw, int fh, int ls, const char *t, int kern ) {
 int line=0;
 int sx=surface->x+x;
 int sy=surface->y+y;
 int ox=sx;
 int oy=sy;
 if ( !t ) return;
 const char *p=t;
 GLImage *space=tf->glyph(' ');
 Blending(blend);
 while ( *p != '\0' ) {
  if ( *p=='\n' ) {
   line++;
   ox=sx;
   oy=ls*line+sy+line*fh;
   p++;
   continue;
  } else if ( *p==' ' && !space ) {
   ox+=(fw/2)+kern;
   p++;
   continue;
  }
  GLImage *glyph=tf->glyph(*p);   
  if ( !glyph ) {
   p++;
   continue;
  }
  art.Stretch(glyph,tint,ox,oy,fw,fh);
  ox+=fw+kern;
  p++;
 }
}



void WriteTexFont( TexFont *tf, Crayon tint, float alpha, Blends blend, int x, int y, int fw, int fh, int ls, const char *t, int kern ) {
 if ( !tf ) return;
 int line=0;
 int sx=x;
 int sy=y;
 int ox=sx;
 int oy=sy;
 if ( !t ) return;
 const char *p=t;
 Crayon myTint(tint);
 myTint.Alpha(alpha);
 GLImage *space=tf->glyph(' ');
 Blending(blend);
 while ( *p != '\0' ) {
  if ( *p=='\n' ) {
   line++;
   ox=sx;
   oy=ls*line+sy+line*fh;
   p++;
   continue;
  } else if ( *p==' ' && !space ) {
   ox+=(space?space->width:(fw/2))+kern;
   p++;
   continue;
  }
  GLImage *glyph=tf->glyph(*p);   
  if ( !glyph ) {
   p++;
   continue;
  }
  art.Stretch(glyph,myTint,ox,oy,fw,fh);
  ox+=fw+kern;
  p++;
 }
}

void WriteTexFontColorCodes( TexFont *tf, Crayon tint, float alpha, Blends blend, int x, int y, int fw, int fh, int ls, const char *t, int kern ) {
 Crayon ink(tint);
 if ( !tf ) return;
 int line=0;
 int sx=x;
 int sy=y;
 int ox=sx;
 int oy=sy;
 if ( !t ) return;
 const char *p=t;
 GLImage *space=tf->glyph(' ');
 while ( *p != '\0' ) {
  if ( *p == '^' ) {
   p++;
   if ( *p == '\0' ) break;
   bool continuation=false;
   bool stop=false;
   switch ( UPPER(*p) ) {
    default:break;
    case '^':
    {
  GLImage *glyph=tf->glyph('^');   
  if ( !glyph ) {
   p++;
   continue;
  }
  Crayon myTint(ink);
  myTint.Alpha(alpha);
  art.Stretch(glyph,myTint,ox,oy,fw,fh);
  ox+=fw+kern;
  p++;
  continuation=true;
    }
     break;
    case 'X': ink.fromCrayon(tint); continuation=true; break;
    case '!': ink.Multiply(1.5f); continuation=true; break;
    case '@': ink.Multiply(0.75f); continuation=true; break;
    case '#': ink.Multiply(0.5f); continuation=true; break;
    case '$': ink.Multiply(2.0f); continuation=true; break;
    case '%': ink.Multiply(1.1f); continuation=true; break;
    case '&': {
     p++;
     if ( *p == '\0' ) stop=true;
     else {
      Crayon ink2;
      switch ( UPPER(*p) ) {
    case '|': ink2.Pick(omahaOrange); break;     case '0': ink2.Pick(black); break;
    case '1': ink2.Pick(ANSIblue); break;        case '2': ink2.Pick(ANSIgreen); break;
    case '3': ink2.Pick(ANSIred); break;         case '4': ink2.Pick(ANSIamber); break;
    case '5': ink2.Pick(ANSImagenta); break;     case '6': ink2.Pick(ANSIcyan); break;
    case '7': ink2.Pick(ANSIgrey); break;        case '8': ink2.Pick(ANSIboldgrey); break;
    case '9': ink2.Pick(ANSIboldblue); break;    case 'A': ink2.Pick(ANSIboldgreen); break;
    case 'B': ink2.Pick(ANSIboldred); break;     case 'C': ink2.Pick(gold); break;
    case 'D': ink2.Pick(ANSIboldmagenta); break; case 'E': ink2.Pick(cyan); break;
    case 'F': ink2.Pick(alabaster); break;       case 'G': ink2.Int(64,255,32,255); break;
    case 'H': ink2.Pick(hotPink); break;         case 'I': ink2.Pick(eggplant); break;
    case 'J': ink2.Pick(jazzberryJam); break;    case 'K': ink2.Pick(crimsonGlory);  break;
    case 'L': ink2.Int(96,255,32,255);  break;   case 'M': ink2.Pick(macNcheese); break;
    case 'N': ink2.Pick(nadashikoPink);  break;  case 'O': ink2.Pick(orangeRed); break;
    case 'P': ink2.Pick(purple);  break;         case 'Q': ink2.Pick(bole);  break;
    case 'R': ink2.Pick(redGlass); break;        case 'S': ink2.Pick(saddleBrown); break;
    case 'T': ink2.Pick(teal); break;            case 'U': ink2.Pick(schoolBusYellow);  break;
    case 'V': ink2.Pick(vermilion);  break;      case 'W': ink2.Pick(black); break;
    case 'Y': ink2.Pick(peachYellow);  break;    case 'Z': ink2.Pick(vividTangerine); break;
      }
      ink.Mixed(0.5,ink,ink2);
      continuation=true;
     }
    } break;
    case '*': ink.Pick(red255); continuation=true; break;
    case '(': ink.Pick(green255); continuation=true; break;
    case ')': ink.Pick(blue255); continuation=true; break;
    case '_': ink.Alpha(ink.af*0.5f); continuation=true; break;
    case '-': ink.Float(UMAX(ink.rf-0.1f,0.0f),UMAX(ink.gf-0.1f,0.0f),UMAX(ink.bf-0.1f,0.0f),1.0f); continuation=true; break;
    case '+': ink.Float(CLAMP(ink.rf+0.1f,1.0f),CLAMP(ink.gf+0.1f,1.0f),CLAMP(ink.bf+0.1f,1.0f),1.0f); continuation=true; break;
    case '=': ink.fromCrayon(tint); continuation=true; break;
    case '\\': {
   line++;
   ox=sx;
   oy=ls*line+sy+line*fh;
   p++;
   continuation=true;
    } break;
    case '|': ink.Pick(omahaOrange); continuation=true; break;
    case '0': ink.Pick(black); continuation=true; break;
    case '1': ink.Pick(ANSIblue); continuation=true; break;
    case '2': ink.Pick(ANSIgreen); continuation=true; break;
    case '3': ink.Pick(ANSIred); continuation=true; break;
    case '4': ink.Pick(ANSIamber); continuation=true; break;
    case '5': ink.Pick(ANSImagenta); continuation=true; break;
    case '6': ink.Pick(ANSIcyan); continuation=true; break;
    case '7': ink.Pick(ANSIgrey); continuation=true; break;
    case '8': ink.Pick(ANSIboldgrey); continuation=true; break;
    case '9': ink.Pick(ANSIboldblue); continuation=true; break;
    case 'A': ink.Pick(ANSIboldgreen); continuation=true; break;
    case 'B': ink.Pick(ANSIboldred); continuation=true; break;
    case 'C': ink.Pick(gold); continuation=true; break;
    case 'D': ink.Pick(ANSIboldmagenta); continuation=true; break;
    case 'E': ink.Pick(cyan); continuation=true; break;
    case 'F': ink.Pick(alabaster); continuation=true; break;
    case 'G': ink.Int(64,255,32,255); continuation=true; break;
    case 'H': ink.Pick(hotPink); continuation=true; break;
    case 'I': ink.Float(1.0f-ink.rf,1.0f-ink.gf,1.0f-ink.bf,ink.af); continuation=true; break;
    case 'J': ink.Pick(jazzberryJam); continuation=true; break;
    case 'K': ink.Pick(crimsonGlory); continuation=true; break;
    case 'L': ink.Int(96,255,32,255); continuation=true; break;
    case 'M': ink.Pick(macNcheese); continuation=true; break;
    case 'N': ink.Pick(nadashikoPink); continuation=true; break;
    case 'O': ink.Pick(orangeRed); continuation=true; break;
    case 'P': ink.Pick(purple); continuation=true; break;
    case 'Q': ink.Pick(bole); continuation=true; break;
    case 'R': ink.Pick(redGlass); continuation=true; break;
    case 'S': ink.Pick(saddleBrown); continuation=true; break;
    case 'T': ink.Pick(teal); continuation=true; break;
    case 'U': ink.Pick(schoolBusYellow); continuation=true; break;
    case 'V': ink.Pick(vermilion); continuation=true; break;
    case 'W': ink.Pick(black); continuation=true; break;
    case 'Y': ink.Pick(peachYellow); continuation=true; break;
    case 'Z': ink.Pick(vividTangerine); continuation=true; break;
   }
   if ( stop ) break;
   if ( continuation ) { p++; continue; }
  } else if ( *p=='\n' ) {
   line++;
   ox=sx;
   oy=ls*line+sy+line*fh;
   p++;
   continue;
  } else if ( *p==' ' && !space ) {
   ox+=(space?space->width:(fw/2))+kern;
   p++;
   continue;
  }
  GLImage *glyph=tf->glyph(*p);   
  if ( !glyph ) {
   p++;
   continue;
  }
  Crayon myTint(ink);
  myTint.Alpha(alpha);
  art.Stretch(glyph,myTint,ox,oy,fw,fh);
  ox+=fw+kern;
  p++;
 }
}

string TexFontColorCodesWordWrap( const char *txt, int linelen ) {
  Zstring out;
  Strings lines;
  lines.SplitBy(txt,"^\\");
  EACH(lines.first,String,line) {
   Strings words;
   words.SplitPushCaseIgnoreQuotes(line->c_str());
   Zint thislinelen;
   EACH(words.first,String,s) {
    Zstring w=s->s;
    int wordlen=(int)s->s.length()-w.count("^&") - w.count('^')*2;
    if ( (int)thislinelen + 1 + wordlen > linelen ) {
     out+=string("\n");
     out+=s->s+string(" ");
     thislinelen=wordlen+1;
    } else {
     out+=s->s+string(" ");
     thislinelen+=wordlen+1;
    }
   }
   out+=string("\n");
  }
  return out.value;
 }

void WriteTexFontWonky( TexFont *tf, Crayon tint, float alpha, float Wonkyness, int wonkseed, Blends blend, int x, int y, int fw, int fh, int ls, const char *t, int kern ) {
 if ( !tf ) return;
 int line=0;
 int sx=x;
 int sy=y;
 int ox=sx;
 int oy=sy;
 if ( !t ) return;
 const char *p=t;
 Crayon myTint(tint);
 myTint.Alpha(alpha);
 GLImage *space=tf->glyph(' ');
 int seed=wonkseed;
 while ( *p != '\0' ) {
  if ( *p=='\n' ) {
   line++;
   ox=sx;
   oy=ls*line+sy+line*fh;
   p++;
   continue;
  } else if ( *p==' ' && !space ) {
   ox+=(space?space->width:(fw/2))+kern;
   p++;
   continue;
  }
  GLImage *glyph=tf->glyph(*p);   
  if ( !glyph ) {
   p++;
   continue;
  }
  seed++;
  double amt=pseudo(seed);
  double angle=Wonkyness*2.0*amt-Wonkyness;
  art.Pivot(glyph,myTint,blend,(double)ox,(double)oy,(double)(fh/2),(double)(fw/2),angle,0.0,0.0,false,false);
  ox+=fw+kern;
  p++;
 }
}

void WriteTexFont( TexFont *tf, Crayon tint, Blends blend, int x, int y, int fw, int fh, int ls, const char *t, int kern ) {
 if ( !tf ) return;
 if ( tf->afont ) {
  tf->afont->Write(t,tint,x,y,blend,fw,fh,ls,kern);
  return;
 }
 int line=0;
 int sx=x;
 int sy=y;
 int ox=sx;
 int oy=sy;
 if ( !t ) return;
 const char *p=t;
 Blending(blend);
 GLImage *space=tf->glyph(' ');
 while ( *p != '\0' ) {
  if ( *p=='\n' ) {
   line++;
   ox=sx;
   oy=ls*line+sy+line*fh;
   p++;
   continue;
  } else if ( *p==' ' && !space ) {
   ox+=(space?space->width:(fw/2))+kern;
   p++;
   continue;
  }
  GLImage *glyph=tf->glyph(*p);   
  if ( !glyph ) {
   p++;
   continue;
  }
  art.Stretch(glyph,tint,(double)ox,(double)oy,(double)fw,(double)fh);
  ox+=fw+kern;
  p++;
 }
}


int WidthTexFont( TexFont *tf, int fw, int cs, const char *t ) {
 if ( !tf ) return 0;
 if ( tf->afont ) {
  return (int) tf->afont->Width(t,iratiod(cs,fw),(double)fw);
 }
 int total=0;
 int ltotal=0;
 if ( !t ) return 0;
 const char *p=t;
 GLImage *glyph=null;
 GLImage *space=tf->glyph(' ');
 while ( *p != '\0' ) {
  if ( *p == '\n' ) {
   if ( ltotal > total ) total=ltotal;
   ltotal=0;
  } else if ( *p == '\r' ) {
  } else if ( *p == ' ' && !space ) {
   ltotal+=(space?space->width:fw/2)+cs;
  } else if ( glyph=tf->glyph(*p) ) {
   ltotal+=(int)((float)fw*((float)glyph->width/tf->widest))+cs;
  } else {
   if ( glyph=tf->glyph('?') ) {
    ltotal+=(int)((float)fw*((float)glyph->width/tf->widest))+cs;
   }
  }
  p++;
 }
 if ( ltotal > total ) total=ltotal;
 return total;
}


string TexFitWidth( TexFont *tf, int charSpace, int w, string in ) {
 Zstring content(in);
 return content.WordWrapNewlines( w/((int)(tf->widest+0.5f)+charSpace) );
}

string TexFitWidth( TexFont *tf, int charSpace, int w, int fw, string in ) {
 Zstring content(in);
 return content.WordWrapNewlines( w/(int)(fw+charSpace) );
}

void TexFontBoxBounds( Cartesian *out, TexFont *tf, int lineSpace, int charSpaceKern, int fontW, int fontH, const char *s, char forcednewline ) {
 if ( tf->afont ) {
  Cartesiand dout;
  tf->afont->BoxBounds(&dout,s,fontW,fontH,(int)iratiod(lineSpace,fontH),(int)iratiod(charSpaceKern,fontW));
  out->SetRect((int)dout.x,(int)dout.y,(int)dout.w,(int)dout.h);
  return;
 }
 int line=0;
 int ox=0;
 int oy=0;
 int longest=0;
 const char *p=s;
 GLImage *space=tf->glyph(' ');
 while ( *p != '\0' ) {
  if ( *p=='\n' || *p == forcednewline ) {
   line++;
   if ( ox > longest ) longest=ox;
   ox=0;
   oy=lineSpace*line+0+line*fontH;
   p++;
   continue;
  } else if ( *p==' ' && !space ) {
   ox+=(fontW/2)+charSpaceKern;
   p++;
   continue;
  }
  GLImage *glyph=tf->glyph(*p);   
  if ( !glyph ) {
   p++;
   continue;
  }
  ox+=fontW+charSpaceKern;
  p++;
 }
 if ( ox > longest ) longest=ox;
 Strings lines;
 lines.SplitPush(s,'\n');
 String *lastLine=(String *) lines.last;
 while ( lastLine ) {
  Zstring ss(lastLine->s);
  ss.Trim();
  if ( ss.length != 0 ) break;
  oy-=lineSpace+fontH;
 }
 oy+=lineSpace+fontH;
 out->SetRect(longest/2,oy/2,longest,oy);
}


void TexFontColorCodesBoxBounds( Cartesian *out, TexFont *tf, int lineSpace, int charSpaceKern, int fontW, int fontH, const char *s, char forcednewline ) {
 int line=0;
 int ox=0;
 int oy=0;
 int longest=0;
 const char *p=s;
 GLImage *space=tf->glyph(' ');
 while ( *p != '\0' ) {
  if ( *p == '^' ) {
   p++;
   if ( *p == '\0' ) break;
   bool continuation=false;
   bool stop=false;
   switch ( UPPER(*p) ) {
    default:break;
    case '^':
    {
     GLImage *glyph=tf->glyph('^');   
     if ( !glyph ) {
      p++;
      continue;
     }
     ox+=fontW+charSpaceKern;
     p++;
     continuation=true;
    }
     break;
    case 'X': case '!': case '@': case '#': case '$': case '%': continuation=true; break;
    case '&': {
     p++;
     if ( *p == '\0' ) stop=true;
     else {
      continuation=true;
     }
    } break;
    case '*': 
    case '(': 
    case ')': 
    case '_': 
    case '-': 
    case '+': 
    case '=': continuation=true; break;
    case '\\': {
     line++;
     if ( ox > longest ) longest=ox;
     ox=0;
     oy=lineSpace*line+0+line*fontH;
     p++;
     continue;
    } break;
    case '|':     case '0':     case '1':     case '2':     case '3': 
    case '4':     case '5':     case '6':     case '7':     case '8': 
    case '9':     case 'A':     case 'B':     case 'C':     case 'D': 
    case 'E':     case 'F':     case 'G':     case 'H':     case 'I': 
    case 'J':     case 'K':     case 'L':     case 'M':     case 'N': 
    case 'O':     case 'P':     case 'Q':     case 'R':     case 'S': 
    case 'T':     case 'U':     case 'V':     case 'W':     case 'Y': 
    case 'Z': continuation=true; break;
   }
   if ( stop ) break;
   if ( continuation ) { p++; continue; }
  } else if ( *p=='\n' || *p == forcednewline ) {
   line++;
   if ( ox > longest ) longest=ox;
   ox=0;
   oy=lineSpace*line+0+line*fontH;
   p++;
   continue;
  } else if ( *p==' ' && !space ) {
   ox+=(fontW/2)+charSpaceKern;
   p++;
   continue;
  }
  GLImage *glyph=tf->glyph(*p);   
  if ( !glyph ) {
   p++;
   continue;
  }
  ox+=fontW+charSpaceKern;
  p++;
 }
 if ( ox > longest ) longest=ox;
 Strings lines;
 lines.SplitPush(s,'\n');
 String *lastLine=(String *) lines.last;
 while ( lastLine ) {
  Zstring ss(lastLine->s);
  ss.Trim();
  if ( ss.length != 0 ) break;
  oy-=lineSpace+fontH;
 }
 oy+=lineSpace+fontH;
 out->SetRect(longest/2,oy/2,longest,oy);
}


void Text( TexFont *tf, Crayon tint, Blends blend, int x, int y, int fw, int fh, int ls, int cs, const char *t ) {
 if ( tf->afont ) {
  tf->afont->Write(t,tint,x, y,blend,fw,fh,ls,cs);
  return;
 }
 int line=0;
 int sx=x;
 int sy=y;
 int ox=sx;
 int oy=sy;
 if ( !t ) return;
 const char *p=t;
 Blending(blend);
 GLImage *space=tf->glyph(' ');
 if ( tf->relative ) while ( *p != '\0' ) {
  if ( *p=='\r' ) {
   p++;
   continue;
  } else if ( *p==' ' && !space ) {
   p++;
   ox+=(fw/2)+cs;
   continue;
  } else if ( *p=='\n' ) {
   line++;
   ox=sx;
   oy=ls*line+sy+line*fh;
   p++;
   continue;
  }
  GLImage *glyph=tf->glyph(*p);
  if ( !glyph ) glyph=tf->glyph('?');
  if ( !glyph ) {
   p++;
   continue; 
  }
  int gw=(int) ( (float) fw * (float)(glyph->width)/tf->widest );
  glFlush();
  art.Stretch(glyph,tint,ox,oy,gw,fh);
  ox+=gw+cs;
  p++;
 }
 else while ( *p != '\0' ) {
  if ( *p=='\n' ) {
   line++;
   ox=sx;
   oy=ls*line+sy+line*fh;
   p++;
   continue;
  }
  GLImage *glyph=tf->glyph(*p);   
  if ( !glyph ) glyph=tf->glyph('?');
  if ( !glyph ) {
   p++;
   continue;
  }
  glFlush();
  art.Stretch(glyph,tint,ox,oy,fw,fh);
  ox+=fw+cs;
  p++;
 }
}

void Text( TexFont *tf, Crayon tint, Blends blend, int x, int y, int w, int fw, int fh, int ls, int cs, const char *t ) {
 /*if ( tf->afont ) {
  tf->afont->Write(t,tint,x, y,blend,fw,fh,ls,cs);
  return;
 }*/
 int line=0;
 int sx=x;
 int sy=y;
 int ox=sx;
 int oy=sy;
 if ( !t ) return;
 const char *p=t;
 Blending(blend);
 GLImage *space=tf->glyph(' ');
 if ( tf->relative ) while ( *p != '\0' ) {
  if ( *p=='\n' ) {
   line++;
   ox=sx;
   oy=ls*line+sy+line*fh;
   p++;
   continue;
  } else if ( *p=='\r' ) {
   p++;
   continue;
  } else if ( !space && *p==' ' ) {
   if ( ox+fw-x > w ) while ( *p != '\0' && *p != '\n' ) p++;
   ox+=fw/2+cs;
   p++;
   continue;
  } 
  GLImage *glyph=tf->glyph(*p);
  if ( !glyph ) glyph=tf->glyph('?');
  if ( !glyph ) {
   p++;
   continue; 
  }
  int gw=(int) ( (float) fw * (float)(glyph->width)/tf->widest );
  glFlush();
  art.Stretch(glyph,tint,ox,oy,gw,fh);
  ox+=gw+cs;
  if ( ox+fw-x > w ) while ( *p != '\0' && *p != '\n' ) p++;
  else p++;
 }
 else while ( *p != '\0' ) {
  if ( *p=='\n' ) {
   line++;
   ox=sx;
   oy=ls*line+sy+line*fh;
   p++;
   continue;
  }
  GLImage *glyph=tf->glyph(*p);   
  if ( !glyph ) glyph=tf->glyph('?');
  if ( !glyph ) {
   p++;
   continue;
  }
  glFlush();
  art.Stretch(glyph,tint,ox,oy,fw,fh);
  ox+=fw+cs;
  p++;
 }
}

void Text( TexFont *tf, Crayon tint, Blends blend, int x, int y, int w, int lines, int fw, int fh, int ls, int cs, const char *t ) {
 /*if ( tf->afont ) {
  tf->afont->Write(t,tint,x,y,blend,fw,fh,ls,cs);
  return;
 }*/
 int line=0;
 int sx=x;
 int sy=y;
 int ox=sx;
 int oy=sy;
 if ( !t ) return;
 const char *p=t;
 GLImage *space=tf->glyph(' ');
 if ( tf->relative ) while ( *p != '\0' ) {
  if ( *p=='\n' ) {
   line++;
   if ( line == lines ) return;
   ox=sx;
   oy=ls*line+sy+line*fh;
   p++;
   continue;
  } else if ( *p=='\r' ) {
   p++;
   continue;
  } else if ( *p==' ' ) {
   if ( ox+fw-x > w ) while ( *p != '\0' && *p != '\n' ) p++;
   else if ( !space ) {
    ox+=(space?space->width:fw/2)+cs;
    p++;
   }
   continue;
  } 
  GLImage *glyph=tf->glyph(*p);
  if ( !glyph ) glyph=tf->glyph('?');
  if ( !glyph ) {
   p++;
   continue; 
  }
  int gw=(int) ( (float) fw * (float)(glyph->width)/tf->widest );
  glFlush();
  art.Stretch(glyph,tint,ox,oy,gw,fh);
  ox+=gw+cs;
  if ( ox+fw-x > w ) while ( *p != '\0' && *p != '\n' ) p++;
  else p++;
 }
 else while ( *p != '\0' ) {
  if ( *p=='\n' ) {
   line++;
   ox=sx;
   oy=ls*line+sy+line*fh;
   p++;
   continue;
  }
  GLImage *glyph=tf->glyph(*p);   
  if ( !glyph ) glyph=tf->glyph('?');
  if ( !glyph ) {
   p++;
   continue;
  }
  glFlush();
  art.Stretch(glyph,tint,ox,oy,fw,fh);
  ox+=fw+cs;
  p++;
 }
}

Zp<TexFont> defaultTexFont;

Zp<TexFont> goldFont,silverFont,calibri,bytes,digicaps,
        microFont,neonNumbers,redNeonNumbers,
        goldNeonNumbers,alarmClockNumbers,nexaBold,termFont,glowTermFont;

TexFontLibrary texfonts;

void LoadTexFonts() {
#if !defined(NOLOAD_FONTS)
 texfonts.LoadFonts();
#endif
}

TexFont *texFontByName( const char *s ) {
 if ( !str_cmp(s,"micro") ) return microFont;
 else if ( !str_cmp(s,"alarm") ) return alarmClockNumbers;
 else if ( !str_cmp(s,"neon") ) return neonNumbers;
 else if ( !str_cmp(s,"gold") ) return goldNeonNumbers;
 else if ( !str_cmp(s,"red") ) return redNeonNumbers;
 else if ( !str_cmp(s,"digicaps") ) return digicaps;
 else if ( !str_cmp(s,"silver") ) return silverFont;
 else if ( !str_cmp(s,"calibri") ) return calibri;
 else if ( !str_cmp(s,"bytes") ) return bytes;
 else if ( !str_cmp(s,"nexa") ) return nexaBold;
 else if ( !str_cmp(s,"term") ) return termFont;
 else if ( !str_cmp(s,"glowterm") ) return glowTermFont;
 return defaultTexFont;
}