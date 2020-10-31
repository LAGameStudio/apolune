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

#include "VisualAid.h"
#include "fx_Buttons.h"
#include "fx_CircleMenu.h"
#include "fx_Film.h"
#include "fx_Glow.h"
#include "fx_IconSelect.h"
#include "fx_Lightning.h"
#include "fx_LineGraph.h"
#include "fx_List.h"
#include "fx_MixedBag.h"
#include "fx_Numbers.h"
#include "fx_RGBAPal.h"
#include "fx_RotaryMenu.h"
#include "fx_SectionalPercentages.h"
#include "fx_Shader2d.h"
#include "fx_Sliders.h"
//#include "fx_Switchers.h"
#include "fx_TexFont.h"
#include "fx_Toggles.h"
#include "fx_Tweakables.h"
#include "fx_Tween.h"
#include "fx_UglyText.h"
#include "fx_VBOView.h"

class Consulate;

class ScriptableVisual : public Visual {
public:
 Zp<Consulate> consulate;
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class ScriptableVisualAid : public VisualAid {
public:
 Zp<Consulate> consulate;
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class ScriptableVisualAid2 : public VisualAid2 {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class ScriptableAid : public Aid {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class ScriptableVisualAidAdditive : public VisualAidAdditive {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class ScriptableVisualRotated : public VisualRotated {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class ScriptableVisualSkewed : public VisualSkewed {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class ScriptableWobblingVisualAid2 : public WobblingVisualAid2 {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class ScriptableWanderingVisualAid2 : public WanderingVisualAid2 {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class ScriptableDriftingVisualAid2 : public DriftingVisualAid2 {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class ScriptableDriftingStretchingVisualAid2 : public DriftingStretchingVisualAid2 {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class ScriptableDriftingRotatingVisualAid2 : public DriftingRotatingVisualAid2 {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class ScriptableDriftingStretchingRotatingVisualAid2 : public DriftingStretchingRotatingVisualAid2 {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_Glow : public fx_Glow {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_Glow2 : public fx_Glow2 {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_Glow3 : public fx_Glow3 {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_Image : public fx_Image {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_Box : public fx_Box {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_UglyTextBox : public fx_UglyTextBox {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_Circle2d : public fx_Circle2d {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_TexFont : public fx_TexFont {
public:
 Zp<Consulate> consulate;
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_TexFontFader : public fx_TexFontFader {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_TexFontBox : public fx_TexFontBox {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_TexFontBoxString : public fx_TexFontBoxString {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_TexNumberToggle : public fx_TexNumberToggle {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_XFade : public fx_XFade {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_CoolButton : public fx_CoolButton {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_UglyCoolButton : public fx_UglyCoolButton {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_DirectButton : public fx_DirectButton {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_UglyButton : public fx_UglyButton {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_TexFontCoolButton : public fx_TexFontCoolButton {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_CoolButtonToggle : public fx_CoolButtonToggle {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_CoolToggle : public fx_CoolToggle {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_ToggleBox : public fx_ToggleBox {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_ToggleIcon : public fx_ToggleIcon {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_UglyToggleBox : public fx_UglyToggleBox {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_IconSelect : public fx_IconSelect {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_IntegerSelect : public fx_IntegerSelect {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_FloatSelect : public fx_FloatSelect {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_CoolRGBAPal4f : public fx_CoolRGBAPal4f {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_CoolRGBPal3f : public fx_CoolRGBPal3f {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_SimpleRGBPal3f : public fx_SimpleRGBPal3f {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_UglyRGBPal : public fx_UglyRGBPal {
public:
 Zp<Consulate> consulate; 
 Scriptablefx_UglyRGBPal() : fx_UglyRGBPal( 0, 0, null, string("") ) {
 }
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_SectionalPercentages : public fx_SectionalPercentages {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_RangeSelector : public fx_RangeSelector {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_CoordPlane : public fx_CoordPlane {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_XYPad : public fx_XYPad {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_TweenSelect : public fx_TweenSelect {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_Knob : public fx_Knob {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_Scroller : public fx_Scroller {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_VScroller : public fx_VScroller {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_TabView : public fx_TabView {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_Shader2d : public fx_Shader2d {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_LabeledSlider : public fx_LabeledSlider {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_UglySlider : public fx_UglySlider {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_VerticalSlider : public fx_VerticalSlider {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_ReadOnlySlider : public fx_ReadOnlySlider {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_UglyText : public fx_UglyText {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_Film : public fx_Film {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_FilmLoop : public fx_FilmLoop {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_FilmLoopFader : public fx_FilmLoopFader {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_FilmLineFader : public fx_FilmLineFader {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_FilmIsotrapf : public fx_FilmIsotrapf {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_Lightning2d : public fx_Lightning2d {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_LineGraph : public fx_LineGraph {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_FadeIn : public fx_FadeIn {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_FadeOut : public fx_FadeOut {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};

class Scriptablefx_MovingLine : public fx_MovingLine {
public:
 Zp<Consulate> consulate; 
 void fromString( const char *in, Consulate *e );
 void getValue( const char *target, Consulate *e ) {}
};