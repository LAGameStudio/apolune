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

#include "Variadics.h"

enum MathOperator {
 mo_unknown=-1,
 mo_add=0,
 mo_sub=1,
 mo_mul=2,
 mo_div=3,
 mo_mod=4,
 mo_less=5,
 mo_more=6,
 mo_equal=7,
 mo_lessEq=8,
 mo_moreEq=9
};

MathOperator StringToOperator( const char *op );
MathOperator StringToOperator( char op );

class Consulate;
class BoundFunction : public ListItem {
public:
 Zstring name;
 PackedString name_packed;
 Zdisposable<Variadic> returned;
 BoundFunction( const char *name ) : ListItem() {
  Name(name);
 }
 void Name( const char *s ) {
  name=s;
  name_packed=s;
 }
 void EvalMath( MathOperator op, Consulate *es, const char *a, const char *b, const char *target );
 // Called before execute (function parameters)
 virtual void fromString( Consulate *es, const char *in ) {}
 // Called to invoke
 virtual void Execute() {}
};

class BoundFunctions : public LinkedList {
public:
 BoundFunction *find( PackedString *name ) {
  FOREACH(BoundFunction,b) if ( b->name_packed.exactly(name) ) return b;
  return null;
 }
 CLEARLISTRESET(BoundFunction);
};

class FunctionStore : public HashStore<BoundFunction,BoundFunctions> {
public:
 FunctionStore() : HashStore() {
  Init();
 }
 void Init();
 void Add( BoundFunction *b ) {
  this->Append(b,&b->name_packed);
 }
 void Bind( BoundFunction *f ) {
  this->Append(f,&f->name_packed);
 }
 BoundFunction *Find( const char *name ) {
  PackedString packed(name);
  BoundFunctions *functions=Bucket(&packed);
  return functions->find(&packed);
 }
};

extern FunctionStore boundFunctions;

/*** Define Bound Functions here ***/


// graphics functions

class BoundCircle : public BoundFunction {
 Zstring source1,source2,target;
 BoundCircle() : BoundFunction("circle") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundCircleFilled : public BoundFunction {
 Zstring source1,source2,target;
 BoundCircleFilled() : BoundFunction("circle.filled") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundArea : public BoundFunction {
public:
 Crayon color;
 Cartesian place;
 BoundArea() : BoundFunction( "area" ) {}
 void fromString( Consulate *es, const char *in );
 void Execute() {
  Area(color,&place);
 }
};

class BoundRectangle : public BoundFunction {
public:
 Crayon color;
 Cartesian place;
 BoundRectangle() : BoundFunction("rectangle") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {
  Rectangle(color,&place);
 }
};

class BoundStretch : public BoundFunction {
public:
 Zp<GLImage> image;
 Crayon tint;
 Cartesian place;
 Zfloat alpha;
 Blends blend;
 BoundStretch() : BoundFunction("stretch") {
 }
 void fromString( Consulate *es, const char *in );
 void Execute() {
  Stretchi(image,tint,alpha,blend,place.x,place.y,place.w,place.h);
 }
};

#include "Proce55ors.h"
class GLWindowScriptable : public GLWindow {
public:
 Proce55ors fx;
 Zp<Consulate> consulate;
 Zdisposable<Consulate> load,between,render;
 GLWindowScriptable( Consulate *es );
 void OnLoad();
 void Between();
 void Render();
 // Scripting options (used also for 'with' statement)
 void fromString( const char *in );
};

class BoundWindow : public BoundFunction {
public:
 BoundWindow() : BoundFunction("window") {}
 void fromString( GLWindowScriptable *win, const char *in );
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

#include "PresentationScriptable.h"
class BoundPresentation : public BoundFunction {
public:
 Zp<PresentationScriptable> presentation;
 BoundPresentation() : BoundFunction("presentation") {}
 void fromString( PresentationScriptable *win, const char *in );
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

enum DiscardTypes {
 discard_window,
 discard_presentation,
 discard_viewer,
 discard_unknown
};

class BoundDiscard : public BoundFunction {
public:
 Zstring name;
 GLWindowHandles windows;
 BoundDiscard() : BoundFunction("discard") {}
 void fromString( Consulate *es, const char *in );
 void Execute();
};

class BoundLinewidth : public BoundFunction {
public:
 Zfloat setting;
 BoundLinewidth() : BoundFunction("line.width") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {
  glLineWidth(setting.value);
 }
};

// text.functions

class BoundStringLoad : public BoundFunction {
public:
 Zstring target_variable;
 Zstring filename;
 BoundStringLoad() : BoundFunction("text.load") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundStringSave : public BoundFunction {
public:
 Zstring output;
 Zstring filename;
 BoundStringSave() : BoundFunction("text.save") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundStringChar : public BoundFunction {
public:
 Zint number;
 Zstring source,target;
 BoundStringChar() : BoundFunction("text.char") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundStringInteger : public BoundFunction {
public:
 Zstring source,target;
 BoundStringInteger() : BoundFunction("text.integer") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundStringDecimal : public BoundFunction {
public:
 Zstring source,target;
 BoundStringDecimal() : BoundFunction("text.decimal") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundStringWord : public BoundFunction {
public:
 Zint number;
 Zstring source,target;
 BoundStringWord() : BoundFunction("text.word") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundStringWords : public BoundFunction {
public:
 Zstring source,target;
 BoundStringWords() : BoundFunction("text.words") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundStringMatches : public BoundFunction {
public:
 Zstring source,query,target;
 BoundStringMatches() : BoundFunction("text.matches") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundStringLine : public BoundFunction {
public:
 Zint number;
 Zstring source,target;
 BoundStringLine() : BoundFunction("text.line") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundStringLines : public BoundFunction {
public:
 Zstring source,target;
 BoundStringLines() : BoundFunction("text.lines") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundStringConcat : public BoundFunction {
public:
 Zstring a,b,target;
 BoundStringConcat() : BoundFunction("text.concat") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundStringConcatLine : public BoundFunction {
public:
 Zstring a,b,target;
 BoundStringConcatLine() : BoundFunction("text.concat.line") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundStringReplace : public BoundFunction {
public:
 Zstring source1,search,replace,target;
 BoundStringReplace() : BoundFunction("text.replace") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundStringReplace1 : public BoundFunction {
public:
 Zstring source1,search,replace,target;
 BoundStringReplace1() : BoundFunction("text.replace1") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

// math.functions

class BoundMathAdd : public BoundFunction {
public:
 MathOperator operation;
 BoundMathAdd() : BoundFunction("math.add") { operation=mo_add; }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundMathSub : public BoundFunction {
public:
 MathOperator operation;
 BoundMathSub() : BoundFunction("math.sub") { operation=mo_sub; }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundMathDiv : public BoundFunction {
public:
 MathOperator operation;
 BoundMathDiv() : BoundFunction("math.div") { operation=mo_div; }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundMathMul : public BoundFunction {
public:
 MathOperator operation;
 BoundMathMul() : BoundFunction("math.mul") { operation=mo_mul; }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundMathMod : public BoundFunction {
public:
 MathOperator operation;
 BoundMathMod() : BoundFunction("math.mod") { operation=mo_mod; }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundMathRad : public BoundFunction {
public:
 Zstring target;
 BoundMathRad() : BoundFunction("math.rad") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundMathDeg : public BoundFunction {
public:
 Zstring target;
 BoundMathDeg() : BoundFunction("math.deg") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundMathLeast : public BoundFunction {
public:
 Zstring target;
 BoundMathLeast() : BoundFunction("math.least") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundMathGreatest : public BoundFunction {
public:
 Zstring target;
 BoundMathGreatest() : BoundFunction("math.greatest") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundMathDice : public BoundFunction {
public:
 Zstring target;
 BoundMathDice() : BoundFunction("math.dice") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundMathRandom : public BoundFunction {
public:
 Zstring target;
 BoundMathRandom() : BoundFunction("math.random") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundMathPseudo : public BoundFunction {
public:
 Zstring target;
 BoundMathPseudo() : BoundFunction("math.pseudo") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundMathLerp : public BoundFunction {
public:
 Zstring target;
 BoundMathLerp() : BoundFunction("math.lerp") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundMathRange : public BoundFunction {
public:
 Zstring target;
 BoundMathRange() : BoundFunction("math.range") {}
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

// bool.functions

class BoundBoolEqual : public BoundFunction {
public:
 MathOperator operation;
 BoundBoolEqual() : BoundFunction("bool.equals") { operation=mo_equal; }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundBoolLess : public BoundFunction {
public:
 MathOperator operation;
 BoundBoolLess() : BoundFunction("bool.less") { operation=mo_less; }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundBoolMore : public BoundFunction {
public:
 MathOperator operation;
 BoundBoolMore() : BoundFunction("bool.more") { operation=mo_more; }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundBoolLessEqual : public BoundFunction {
public:
 MathOperator operation;
 BoundBoolLessEqual() : BoundFunction("bool.less.equal") { operation=mo_lessEq; }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundBoolMoreEqual : public BoundFunction {
public:
 MathOperator operation;
 BoundBoolMoreEqual() : BoundFunction("bool.more.equal") { operation=mo_moreEq; }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundBoolOdd : public BoundFunction {
public:
 BoundBoolOdd() : BoundFunction("bool.odd") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundBoolEven : public BoundFunction {
public:
 BoundBoolEven() : BoundFunction("bool.even") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

// type.functions

class BoundTypeIs : public BoundFunction {
public:
 Zstring source,target;
 BoundTypeIs() : BoundFunction("type.is") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundTypeInteger : public BoundFunction {
public:
 Zstring source,target;
 BoundTypeInteger() : BoundFunction("type.integer") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundTypeDecimal : public BoundFunction {
public:
 Zstring source,target;
 BoundTypeDecimal() : BoundFunction("type.decimal") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundTypeString : public BoundFunction {
public:
 Zstring source,target;
 BoundTypeString() : BoundFunction("type.string") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundTypeBlend : public BoundFunction {
public:
 Zstring source,target;
 BoundTypeBlend() : BoundFunction("type.blend") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundTypeImage : public BoundFunction {
public:
 Zstring source,target;
 BoundTypeImage() : BoundFunction("type.image") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundTypeCrayon : public BoundFunction {
public:
 Zstring source,target;
 BoundTypeCrayon() : BoundFunction("type.crayon") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundTypeCartesian : public BoundFunction {
public:
 Zstring source,target;
 BoundTypeCartesian() : BoundFunction("type.cartesian") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundTypeTween : public BoundFunction {
public:
 Zstring source,target;
 BoundTypeTween() : BoundFunction("type.tween") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

// tween.functions

class BoundTweenInstant : public BoundFunction {
public:
 Zstring target;
 BoundTweenInstant() : BoundFunction("tween.instant") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

// cartesian.functions

class BoundCartesianX : public BoundFunction {
public:
 Zstring source,target;
 BoundCartesianX() : BoundFunction("cartesian.x") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundCartesianY : public BoundFunction {
public:
 Zstring source,target;
 BoundCartesianY() : BoundFunction("cartesian.y") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundCartesianW : public BoundFunction {
public:
 Zstring source,target;
 BoundCartesianW() : BoundFunction("cartesian.w") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundCartesianH : public BoundFunction {
public:
 Zstring source,target;
 BoundCartesianH() : BoundFunction("cartesian.h") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundCartesianX2 : public BoundFunction {
public:
 Zstring source,target;
 BoundCartesianX2() : BoundFunction("cartesian.x2") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundCartesianY2 : public BoundFunction {
public:
 Zstring source,target;
 BoundCartesianY2() : BoundFunction("cartesian.y2") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

// crayon.functions

class BoundCrayonR : public BoundFunction {
public:
 Zstring source,target;
 BoundCrayonR() : BoundFunction("crayon.r") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundCrayonG : public BoundFunction {
public:
 Zstring source,target;
 BoundCrayonG() : BoundFunction("crayon.g") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundCrayonB : public BoundFunction {
public:
 Zstring source,target;
 BoundCrayonB() : BoundFunction("crayon.b") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundCrayonA : public BoundFunction {
public:
 Zstring source,target;
 BoundCrayonA() : BoundFunction("crayon.a") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundCrayonR255 : public BoundFunction {
public:
 Zstring source,target;
 BoundCrayonR255() : BoundFunction("crayon.r255") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundCrayonG255 : public BoundFunction {
public:
 Zstring source,target;
 BoundCrayonG255() : BoundFunction("crayon.g255") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundCrayonB255 : public BoundFunction {
public:
 Zstring source,target;
 BoundCrayonB255() : BoundFunction("crayon.b255") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundCrayonA255 : public BoundFunction {
public:
 Zstring source,target;
 BoundCrayonA255() : BoundFunction("crayon.a255") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

// image.functions

class BoundImageName : public BoundFunction {
public:
 Zstring source,target;
 BoundImageName() : BoundFunction("image.name") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

// geo.toolkit

class BoundGeoDist1d : public BoundFunction {
public:
 Zstring source,target;
 BoundGeoDist1d() : BoundFunction("geo.dist1d") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundGeoDist2d : public BoundFunction {
public:
 Zstring source,target;
 BoundGeoDist2d() : BoundFunction("geo.dist2d") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundGeoDist3d : public BoundFunction {
public:
 Zstring source,target;
 BoundGeoDist3d() : BoundFunction("geo.dist3d") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundGeoBoxBox : public BoundFunction {
public:
 Zstring source1,source2,target;
 BoundGeoBoxBox() : BoundFunction("geo.box.box") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundGeoBoxBoxIntersection : public BoundFunction {
public:
 Zstring source1,source2,target;
 BoundGeoBoxBoxIntersection() : BoundFunction("geo.box.box.intersection") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundGeoBoxLine : public BoundFunction {
public:
 Zstring source1,source2,target;
 BoundGeoBoxLine() : BoundFunction("geo.box.line") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundGeoLineLine : public BoundFunction {
public:
 Zstring source1,source2,target;
 BoundGeoLineLine() : BoundFunction("geo.line.line") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundGeoLineLineIntersection : public BoundFunction {
public:
 Zstring source1,source2,target;
 BoundGeoLineLineIntersection() : BoundFunction("geo.line.line.intersection") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundGeoPointCircle : public BoundFunction {
public:
 Zstring source1,source2,target;
 BoundGeoPointCircle() : BoundFunction("geo.point.circle") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundGeoLineCircle : public BoundFunction {
public:
 Zstring source1,source2,target;
 BoundGeoLineCircle() : BoundFunction("geo.line.circle") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundGeoLineCircleIntersection : public BoundFunction {
public:
 Zstring source1,source2,target;
 BoundGeoLineCircleIntersection() : BoundFunction("geo.line.circle.intersection") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundGeoRotate : public BoundFunction {
public:
 Zstring target;
 BoundGeoRotate() : BoundFunction("geo.rotate") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

// fbo.functions

class BoundFBOActivate : public BoundFunction {
public:
 Zstring fbo;
 BoundFBOActivate() : BoundFunction("fbo.activate") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundFBODeactivate : public BoundFunction {
public:
 BoundFBODeactivate() : BoundFunction("fbo.deactivate") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundFBOStretch : public BoundFunction {
public:
 BoundFBOStretch() : BoundFunction("fbo.stretch") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundFBORead : public BoundFunction {
public:
 Zstring target;
 BoundFBORead() : BoundFunction("fbo.read") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundFBOReadStencil : public BoundFunction {
public:
 Zstring target;
 BoundFBOReadStencil() : BoundFunction("fbo.read.stencil") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundFBOReadDepth : public BoundFunction {
public:
 Zstring target;
 BoundFBOReadDepth() : BoundFunction("fbo.read.depth") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

// vbo.functions

class BoundVBOLoad : public BoundFunction {
public:
 Zstring fbo;
 BoundVBOLoad() : BoundFunction("vbo.load") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundVBOSave : public BoundFunction {
public:
 Zstring fbo;
 BoundVBOSave() : BoundFunction("vbo.save") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundVBOView : public BoundFunction {
public:
 Zstring fbo;
 BoundVBOView() : BoundFunction("vbo.view") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

// ui

class BoundUIVirtual : public BoundFunction {
public:
 BoundUIVirtual() : BoundFunction("virtual") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

// audio.functions

class BoundAudioPlay : public BoundFunction {
public:
 BoundAudioPlay() : BoundFunction("audio.play") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundAudioStop : public BoundFunction {
public:
 BoundAudioStop() : BoundFunction("audio.stop") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundAudioCue : public BoundFunction {
public:
 BoundAudioCue() : BoundFunction("audio.cue") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundAudioCueLoop : public BoundFunction {
public:
 BoundAudioCueLoop() : BoundFunction("audio.cue.loop") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundAudioCueSingleton : public BoundFunction {
public:
 BoundAudioCueSingleton() : BoundFunction("audio.cue.singleton") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundAudioCueLimited : public BoundFunction {
public:
 BoundAudioCueLimited() : BoundFunction("audio.cue.limited") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundAudioCueLimitedLoop : public BoundFunction {
public:
 BoundAudioCueLimitedLoop() : BoundFunction("audio.cue.limited.loop") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundAudioVolumeMaster : public BoundFunction {
public:
 BoundAudioVolumeMaster() : BoundFunction("audio.volume.master") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundAudioVolumeMusic : public BoundFunction {
public:
 BoundAudioVolumeMusic() : BoundFunction("audio.volume.music") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundAudioVolumeSounds : public BoundFunction {
public:
 BoundAudioVolumeSounds() : BoundFunction("audio.volume.sounds") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

// system.functions

class BoundSystemURL : public BoundFunction {
public:
 Zstring target;
 BoundSystemURL() : BoundFunction("url") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundSystemURLSecured : public BoundFunction {
public:
 Zstring target;
 BoundSystemURLSecured() : BoundFunction("url.secured") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundSystemTCPConnect : public BoundFunction {
public:
 BoundSystemTCPConnect() : BoundFunction("tcp.connect") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundSystemTCPSend : public BoundFunction {
public:
 BoundSystemTCPSend() : BoundFunction("tcp.send") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundSystemTCPListen : public BoundFunction {
public:
 BoundSystemTCPListen() : BoundFunction("tcp.listen") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundSystemTCPDisconnect : public BoundFunction {
public:
 BoundSystemTCPDisconnect() : BoundFunction("tcp.disconnect") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundSystemMouse : public BoundFunction {
public:
 Zstring target;
 BoundSystemMouse() : BoundFunction("mouse") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundSystemTouches : public BoundFunction {
public:
 Zstring target;
 BoundSystemTouches() : BoundFunction("touches") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};

class BoundSystemTime : public BoundFunction {
public:
 Zstring target;
 BoundSystemTime() : BoundFunction("time") { }
 void fromString( Consulate *es, const char *in );
 void Execute() {}
};