#include "PresentationScriptable.h"

#include "Scripting.h"
#include "ScriptableFX.h"
#include "FastGUIProcessors.h"

PresentationScriptable::PresentationScriptable( Consulate *es )
 : Presentation( display, input, audio ) {
 consulate=es;
}

void PresentationScriptable::fromString( const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {

  /*if ( !str_cmp(w, "drop" ) ) {
   w=words.Next();
   fx.Drop(string(w));
  } else*/ if ( !str_cmp(w, "discard" ) ) {
   this->destroyMe=true;
  } else if ( !str_cmp(w,"name") ) {
   w=words.Next();
   Name(w);
  } else if ( !str_cmp(w,"id") || !str_cmp(w,"code") ) {
   w=words.Next();
   code=atoi(w);
  } /*else if ( !str_cmp(w,"load") ) {
   w=words.Next();
   load.Recycle();
   load->variables=consulate->variables;
   load->scripts.New(w);
  } else if ( !str_cmp(w,"between") ) {
   w=words.Next();
   between.Recycle();
   between->variables=consulate->variables;
   between->scripts.New(w);
  } else if ( !str_cmp(w,"render") ) {
   w=words.Next();
   render.Recycle();
   render->variables=consulate->variables;
   render->scripts.New(w);
  } */else if ( !str_cmp(w,"fullscreen") ) {
   Fullscreen();
  } else if ( !str_cmp(w,"position") ) {
   w=words.Next();
   Zstring subwords(w);
   const char *p=subwords.Next();
   int xx=consulate->variables->EvaluateInteger(p);
   p=subwords.Next();
   int yy=consulate->variables->EvaluateInteger(p);
   this->MoveTo(xx,yy);
  } else if ( !str_cmp(w,"size") ) {
   w=words.Next();
   Zstring subwords(w);
   const char *p=subwords.Next();
   x2=x+consulate->variables->EvaluateInteger(p);
   p=subwords.Next();
   y2=y2+consulate->variables->EvaluateInteger(p);
  } else if ( !str_cmp(w,"centered") ) {
   w=words.Next();
   Zstring subwords(w);
   const char *p=subwords.Next();
   int ww=consulate->variables->EvaluateInteger(p);
   p=subwords.Next();
   int hh=consulate->variables->EvaluateInteger(p);
   SizeAndCenter(ww,hh);
  } else if ( !str_cmp(w,"bg") || !str_cmp(w,"background") ) {
   w=words.Next();
   consulate->variables->EvaluateCrayon(w,&background);
  } else if ( !str_cmp(w,"hide") ) {
   visible=false;
  } else if ( !str_cmp(w,"show") ) {
   visible=true;
  } else if ( !str_cmp(w,"line") ) {
  } else if ( !str_cmp(w,"box") ) {
  } else if ( !str_cmp(w,"circle") ) {
  } else if ( !str_cmp(w,"image") ) {
  } /*else if ( !str_cmp(w,"button") ) {
   fx_FastGUIButtonScriptable *f=new fx_FastGUIButtonScriptable(consulate);
   fx.Add(f,this);
   w=words.Next();
   f->ScriptModify(w);
  } else if ( !str_cmp(w,"icon") ) {
   fx_FastGUIIconScriptable *f=new fx_FastGUIIconScriptable(consulate);
   fx.Add(f,this);
   w=words.Next();
   f->ScriptModify(w);
  } else if ( !str_cmp(w,"text") ) {
   fx_FastGUITextScriptable *f=new fx_FastGUITextScriptable(consulate);
   fx.Add(f,this);
   w=words.Next();
   f->ScriptModify(w);
  } */else if ( !str_cmp( w, "slide" ) ) {
   slidePointer=new ScriptableSlide();
   this->Add(slidePointer.pointer);
   slidePointer->consulate=this->consulate;
   slidePointer->owner=this;
   w=words.Next();
   slidePointer->fromString(w);
  } else if ( !str_cmp( w, "rewind" ) ) {
   this->Rewind(false);
  } else if ( !str_cmp( w, "pause" ) ) {
   this->Pause();
  } else if ( !str_cmp( w, "letterbox" ) ) {
   this->letterbox=!this->letterbox;
  } else if ( !str_cmp(w,"loop") || !str_cmp(w,"loops") || !str_cmp( w, "looping" ) ) {
   this->loop=true;
  }
  w=words.Next();
 }
 Init(this->paused);
}

ScriptableSlide::ScriptableSlide() : Slide() {
 scripts.Recycle();
}

void ScriptableSlide::OnComplete() {
 this->scripts->Execute();
}

void ScriptableSlide::fromString( const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp( w, "visual" ) ) {
   w=words.Next();
   ScriptableVisual *e= new ScriptableVisual;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.aid1" ) ) {
   w=words.Next();
   ScriptableVisualAid *e= new ScriptableVisualAid;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.aid2" ) ) {
   w=words.Next();
   ScriptableVisualAid2 *e= new ScriptableVisualAid2;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.aid" ) ) {
   w=words.Next();
   ScriptableAid *e= new ScriptableAid;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.additive" ) ) {
   w=words.Next();
   ScriptableVisualAidAdditive *e= new ScriptableVisualAidAdditive;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.rotate" ) ) {
   w=words.Next();
   ScriptableVisualRotated *e= new ScriptableVisualRotated;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.skew" ) ) {
   w=words.Next();
   ScriptableVisualSkewed *e= new ScriptableVisualSkewed;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.wobble" ) ) {
   w=words.Next();
   ScriptableWobblingVisualAid2 *e= new ScriptableWobblingVisualAid2;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.wander" ) ) {
   w=words.Next();
   ScriptableWanderingVisualAid2 *e= new ScriptableWanderingVisualAid2;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.drift" ) ) {
   w=words.Next();
   ScriptableDriftingVisualAid2 *e= new ScriptableDriftingVisualAid2;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.drift.stretch" ) ) {
   w=words.Next();
   ScriptableDriftingStretchingVisualAid2 *e= new ScriptableDriftingStretchingVisualAid2;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.drift.rotate" ) ) {
   w=words.Next();
   ScriptableDriftingRotatingVisualAid2 *e= new ScriptableDriftingRotatingVisualAid2;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.drift.stretch.rotate" ) ) {
   w=words.Next();
   ScriptableDriftingStretchingRotatingVisualAid2 *e= new ScriptableDriftingStretchingRotatingVisualAid2;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.glow" ) ) {
   w=words.Next();
   Scriptablefx_Glow *e= new Scriptablefx_Glow;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.glow2" ) ) {
   w=words.Next();
   Scriptablefx_Glow2 *e= new Scriptablefx_Glow2;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.glow3" ) ) {
   w=words.Next();
   Scriptablefx_Glow3 *e= new Scriptablefx_Glow3;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.image" ) ) {
   w=words.Next();
   Scriptablefx_Image *e= new Scriptablefx_Image;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.box" ) ) {
   w=words.Next();
   Scriptablefx_Box *e= new Scriptablefx_Box;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.box.wirefont" ) ) {
   w=words.Next();
   Scriptablefx_UglyTextBox *e= new Scriptablefx_UglyTextBox;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.circle" ) ) {
   w=words.Next();
   Scriptablefx_Circle2d *e= new Scriptablefx_Circle2d;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.texfont" ) ) {
   w=words.Next();
   Scriptablefx_TexFont *e= new Scriptablefx_TexFont;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.texfont.fader" ) ) {
   w=words.Next();
   Scriptablefx_TexFontFader *e= new Scriptablefx_TexFontFader;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.texfont.box" ) ) {
   w=words.Next();
   Scriptablefx_TexFontBox *e= new Scriptablefx_TexFontBox;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.texfont.box.text" ) ) {
   w=words.Next();
   Scriptablefx_TexFontBoxString *e= new Scriptablefx_TexFontBoxString;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.texfont.integer" ) ) {
   w=words.Next();
   Scriptablefx_TexNumberToggle *e= new Scriptablefx_TexNumberToggle;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.crossfade" ) ) {
   w=words.Next();
   Scriptablefx_XFade *e= new Scriptablefx_XFade;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "button.cool" ) ) {
   w=words.Next();
   Scriptablefx_CoolButton *e= new Scriptablefx_CoolButton;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "button.cool.wirefont" ) ) {
   w=words.Next();
   Scriptablefx_UglyCoolButton *e= new Scriptablefx_UglyCoolButton;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "button.direct" ) ) {
   w=words.Next();
   Scriptablefx_DirectButton *e= new Scriptablefx_DirectButton;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "button.wirefont" ) ) {
   w=words.Next();
   Scriptablefx_UglyButton *e= new Scriptablefx_UglyButton;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "button.texfont" ) ) {
   w=words.Next();
   Scriptablefx_TexFontCoolButton *e= new Scriptablefx_TexFontCoolButton;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "toggle.cool" ) ) {
   w=words.Next();
   Scriptablefx_CoolButtonToggle *e= new Scriptablefx_CoolButtonToggle;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "toggle.cool2" ) ) {
   w=words.Next();
   Scriptablefx_CoolToggle *e= new Scriptablefx_CoolToggle;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "toggle.box" ) ) {
   w=words.Next();
   Scriptablefx_ToggleBox *e= new Scriptablefx_ToggleBox;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "toggle.icon" ) ) {
   w=words.Next();
   Scriptablefx_ToggleIcon *e= new Scriptablefx_ToggleIcon;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "toggle.box.wirefont" ) ) {
   w=words.Next();
   Scriptablefx_UglyToggleBox *e= new Scriptablefx_UglyToggleBox;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "select.icon" ) ) {
   w=words.Next();
   Scriptablefx_IconSelect *e= new Scriptablefx_IconSelect;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "select.integer" ) ) {
   w=words.Next();
   Scriptablefx_IntegerSelect *e= new Scriptablefx_IntegerSelect;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "select.decimal" ) ) {
   w=words.Next();
   Scriptablefx_FloatSelect *e= new Scriptablefx_FloatSelect;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "select.rgba.cool" ) ) {
   w=words.Next();
   Scriptablefx_CoolRGBAPal4f *e= new Scriptablefx_CoolRGBAPal4f;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "select.rgb.cool" ) ) {
   w=words.Next();
   Scriptablefx_CoolRGBPal3f *e= new Scriptablefx_CoolRGBPal3f;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "select.rgb.simple" ) ) {
   w=words.Next();
   Scriptablefx_SimpleRGBPal3f *e= new Scriptablefx_SimpleRGBPal3f;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "select.rgb.wirefont" ) ) {
   w=words.Next();
   Scriptablefx_UglyRGBPal *e= new Scriptablefx_UglyRGBPal;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "select.percents.sectional" ) ) {
   w=words.Next();
   Scriptablefx_SectionalPercentages *e= new Scriptablefx_SectionalPercentages;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "select.range" ) ) {
   w=words.Next();
   Scriptablefx_RangeSelector *e= new Scriptablefx_RangeSelector;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "select.pad" ) ) {
   w=words.Next();
   Scriptablefx_CoordPlane *e= new Scriptablefx_CoordPlane;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "select.pad.coord" ) ) {
   w=words.Next();
   Scriptablefx_XYPad *e= new Scriptablefx_XYPad;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "select.tween" ) ) {
   w=words.Next();
   Scriptablefx_TweenSelect *e= new Scriptablefx_TweenSelect;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "knob" ) ) {
   w=words.Next();
   Scriptablefx_Knob *e= new Scriptablefx_Knob;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "scrolling" ) ) {
   w=words.Next();
   Scriptablefx_Scroller *e= new Scriptablefx_Scroller;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "scrolling.vertical" ) ) {
   w=words.Next();
   Scriptablefx_VScroller *e= new Scriptablefx_VScroller;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "tabs" ) ) {
   w=words.Next();
   Scriptablefx_TabView *e= new Scriptablefx_TabView;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "shader.2d" ) ) {
   w=words.Next();
   Scriptablefx_Shader2d *e= new Scriptablefx_Shader2d;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "slider.texfont" ) ) {
   w=words.Next();
   Scriptablefx_LabeledSlider *e= new Scriptablefx_LabeledSlider;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "slider.wirefont" ) ) {
   w=words.Next();
   Scriptablefx_UglySlider *e= new Scriptablefx_UglySlider;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "slider.vertical" ) ) {
   w=words.Next();
   Scriptablefx_VerticalSlider *e= new Scriptablefx_VerticalSlider;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "slider.readonly" ) ) {
   w=words.Next();
   Scriptablefx_ReadOnlySlider *e= new Scriptablefx_ReadOnlySlider;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.wirefont" ) ) {
   w=words.Next();
   Scriptablefx_UglyText *e= new Scriptablefx_UglyText;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.film" ) ) {
   w=words.Next();
   Scriptablefx_Film *e= new Scriptablefx_Film;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.film.looping" ) ) {
   w=words.Next();
   Scriptablefx_FilmLoop *e= new Scriptablefx_FilmLoop;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.film.looping.fader" ) ) {
   w=words.Next();
   Scriptablefx_FilmLoopFader *e= new Scriptablefx_FilmLoopFader;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.film.line.fader" ) ) {
   w=words.Next();
   Scriptablefx_FilmLineFader *e= new Scriptablefx_FilmLineFader;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "film.isotrap" ) ) {
   w=words.Next();
   Scriptablefx_FilmIsotrapf *e= new Scriptablefx_FilmIsotrapf;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "lightning.2d" ) ) {
   w=words.Next();
   Scriptablefx_Lightning2d *e= new Scriptablefx_Lightning2d;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "linegraph" ) ) {
   w=words.Next();
   Scriptablefx_LineGraph *e= new Scriptablefx_LineGraph;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "fade.in" ) ) {
   w=words.Next();
   Scriptablefx_FadeIn *e= new Scriptablefx_FadeIn;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "fade.out" ) ) {
   w=words.Next();
   Scriptablefx_FadeOut *e= new Scriptablefx_FadeOut;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "visual.line.moving" ) ) {
   w=words.Next();
   Scriptablefx_MovingLine *e= new Scriptablefx_MovingLine;
   e->fromString(w,this->consulate);
   fx->Add(e,owner);
  } else if ( !str_cmp( w, "duration" ) ) {
   w=words.Next();
   this->duration=(float) consulate->variables->EvaluateDecimal(w);
  } else if ( !str_cmp( w, "name" ) ) { 
   w=words.Next();
   this->name=w;
  } else if ( !str_cmp( w, "rewind" ) ) { 
   w=words.Next();
   this->Reset(false);
  }
  w=words.Next();
 }
}