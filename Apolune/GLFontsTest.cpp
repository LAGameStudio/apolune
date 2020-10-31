#include "GLFontsTest.h"

#include <Windows.h>

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "GLWindow.h"
#include "GLImage.h"

#include "fx_Buttons.h"
#include "Presentation.h"
#include "TextureLibrary.h"

#include "TexFont.h"
#include "fx_TexFont.h"
#include "VisualAid.h"

void GLFontsTest::OnLoad()
{
 Zint ox,oy;
 background.Int(0,0,0,10);
 widest=tallest=0;
 Slide *s=new Slide(10.0f);
 s->rewind=true; s->unpause=true;
 Add(s);

 for ( int i=0; i<255; i++ ) if ( calibri->glyphs[i].glyph ) {
  VisualAid *v=new VisualAid;
  v->image=calibri->glyphs[i].glyph;
  v->w=v->image->width/4;
  v->h=v->image->height/4;
  ox+=v->w;
  if ( i % 15==0 ) {
   if ( widest < ox ) widest=ox+v->w;
   ox=0;
   oy+=v->h;
   tallest=oy+v->h;
  }
  v->sx=ox; v->w2=v->w;
  v->sy=oy; v->h2=v->h;
  v->Tint(crayons.Pick(alabaster));
  v->duration=10.1f;
//  v->Tint(crayons.Pick(alabaster));
  s->fx->Add(v,this);
 }
 fx_TexFont *t=new fx_TexFont(calibri,this,"Testing..!",10.1f,0,14,14,14,crayons.Pick(alabaster));
 s->fx->Add(t,this);
 duration=10.1f;
 tallest+=50;
 Init(false);
}
