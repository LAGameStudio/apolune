#pragma once

#include "GLWindow.h"
#include "Atlas.h"
class AtlasFontViewer : public GLWindow {
public:
 Zp<AtlasFont> font;
 void OnLoad() {
  Fullscreen();
  background.Pick(gray);
  EACH(atlasFonts.first,AtlasFont,af) af->GenGlyphSheet();
  font = (AtlasFont *) atlasFonts.first;
 }
 void Between() {
  if ( input->KeyUp(DX_LEFT) ) {
   font=(AtlasFont *) (font->prev);
   if ( !font ) font = (AtlasFont *) atlasFonts.last;
  }
  if ( input->KeyUp(DX_RIGHT) ) {
   font=(AtlasFont *) (font->next);
   if ( !font ) font = (AtlasFont *) atlasFonts.first;
  }
  if ( input->right ) background.Any();
 }
 void Render() {
  if ( font ) {
   if ( font->glyphSheet > 0 ) {
    art.PushViewport(w,h);
//    art.PushViewport(&fboScreen);
//    fboScreen.Bind();
    art.Element(crayons.Pick(gray),none,art.stash.filledRect,1024.0+96.0,64.0,(double)font->atlas.texture->width,(double)font->atlas.texture->width);
    art.Stretch(font->atlas.texture,crayons.Pick(alabaster),transparency,1024.0+96.0,64.0,(double)font->atlas.texture->width,(double)font->atlas.texture->width);
    art.Element(font->atlas.texture,crayons.Pick(ANSIgreen),transparency,font->glyphSheet,64.0,64.0,1024.0,1024.0);
    art.ElementLines(crayons.Pick(hotPink),transparency,font->glyphSheet,64.0,64.0,1024.0,1024.0);
    art.Element(font->atlas.texture,crayons.Pick(ANSIboldgreen),additive,font->fontNameText,1024.0,16.0,256.0,128.0);
    art.Element(font->atlas.texture,crayons.Pick(ANSIboldgreen),transparency,font->fontNameText,1024.0,16.0+font->base,256.0,128.0);
//    fboScreen.Unbind();
    art.PopViewport();
//    fboScreen.RenderUpsideDown((int)x,(int)y,(int)(x+w),(int)(y+h));
   }
  }
 }
};

