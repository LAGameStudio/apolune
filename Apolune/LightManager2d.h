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
#include "ListItem.h"
#include "Crayon.h"
#include "Vertex.h"
#include "Light2d.h"
#include "GLSL2Shader.h"
#include "FBO.h"
#include "TextureLibrary.h"

// Could be called 'LightMapper2d'
class LightManager2d : public ListItem {
public:
 Obstruction2ds walls;
 ObjectiveObstructions objects;
 ShadedLightBoard2ds lightboards;
 ShadedNoRadiusLightBoard2ds lightpatches;
 ShadedNoColorLightBoard2ds lightpoints;
 ShadedPulsingLightBoard2ds lightpulses;
 Light2ds controllers; // controllers for the lightboards
 Light2ds statics;//,culled?
 Light2ds shadowless;//,culled?
 Light2ds casters;//,culled?
 Light2ds shadedLights;//,culled?
 Cartesian screen;
 FBOColorDepth obstructions;
 FBOColor *light,*accumulator; // two pass system
 Blends blend;
 Crayon ambient,add;
 LightManager2d() {
  blend=multiply;
  light=&fboScreen1;
  accumulator=&fboScreen2;
  screen.SetRect(0,0,display.w,display.h);
  walls.ambient=&ambient;
  objects.ambient=&ambient;
//  blur.Init();
//  blur.texture_id=&(light->color->texture);
  ambient.Float(0.01f,0.01f,0.01f,1.0f);
  casters.ambient=shadedLights.ambient=statics.ambient=shadowless.ambient=&ambient;
  add.Pick(alabaster); add.ScaleByAlphaf(0.1f);
  obstructions.background.Pick(clear);
  obstructions.Create(display.w,display.h);
 }
 ~LightManager2d() {}
 virtual void Add( Light2d *light ) {
  switch ( light->atomic_type ) {
  case l2d_caster:
    casters.Append(light);
   break;
  case l2d_shadowless:
    shadowless.Append(light);
   break;
  case l2d_shaded:
    shadowless.Append(light);
   break;
  case l2d_static:
    statics.Append(light);
   break;
  }
 }
 void Clear() {
  walls.Clear();
  objects.Clear();
  lightboards.Reset();
  lightpatches.Reset();
  lightpoints.Reset();
  lightpulses.Reset();
  controllers.Clear();
  statics.Clear();
  shadowless.Clear();
  casters.Clear();
  shadedLights.Clear();
 }
 Light2d *Add(Vertexd position, double intensity, double radius, double attenuation, Crayon color) {
  Light2d *light=new Light2d( position, intensity, radius, attenuation, color );
  light->ambient=&ambient;
  light->Customize();
  casters.Append(light);
  return light;
 }
 ShadowlessLight2d *AddShadowless(Vertexd position, double intensity, double radius, double attenuation, Crayon color) {
  ShadowlessLight2d *light=new ShadowlessLight2d( position, intensity, radius, attenuation, color );
  light->ambient=&ambient;
  light->Customize();
  shadowless.Append(light);
  return light;
 }
 Spot2d *Spot(Vertexd position, double intensity, double radius, double attenuation, Crayon color, double angle, double spread ) {
  Spot2d *spot=new Spot2d( position, intensity, radius, attenuation, color, angle, spread );
  spot->ambient=&ambient;
  spot->flare=library.Load("data/images/elements/swab.png");
  spot->source=null;
  casters.Append(spot);
  return spot;
 }
 Spot2d *Spot(Vertexd position, double intensity, double radius, double attenuation, Crayon color, double angle, double spread, double emission ) {
  Spot2d *spot=new Spot2d( position, intensity, radius, attenuation, color, angle, spread );
  spot->ambient=&ambient;
  spot->emission=emission;
  spot->flare=library.Load("data/images/elements/swab.png");
  casters.Append(spot);
  return spot;
 }
 ShadedLight2d *Shaded(Vertexd position, double intensity, double radius, double attenuation, Crayon color ) {
  ShadedLight2d *L=new ShadedLight2d( position, intensity, radius, attenuation, color );
  shadedLights.Append(L);
  return L;
 }
 ShadedShadowlessLight2d *ShadedShadowless(Vertexd position, double intensity, double radius, double attenuation, Crayon color ) {
  ShadedShadowlessLight2d *L=new ShadedShadowlessLight2d( position, intensity, radius, attenuation, color );
  shadedLights.Append(L);
  return L;
 }
 GPUMultiLight2dController *GPULight(Vertexd position, float radius, Crayon color) {
  GPUMultiLight2dController *light=new GPUMultiLight2dController( &lightboards,
   lightboards.Add( radius, color, (float) position.x, (float) position.y )
  );
  controllers.Append(light);
  return light;
 }
 GPUNoRadiusMultiLight2dController *GPUPatch(Vertexd position, Crayon color) {
  GPUNoRadiusMultiLight2dController *light=new GPUNoRadiusMultiLight2dController( &lightpatches,
   lightpatches.Add( color, (float) position.x, (float) position.y )
  );
  controllers.Append(light);
  return light;
 }
 GPUNoColorMultiLight2dController *GPUPoint(Vertexd position) {
  GPUNoColorMultiLight2dController *light=new GPUNoColorMultiLight2dController( &lightpoints,
   lightpoints.Add( (float) position.x, (float) position.y )
  );
  controllers.Append(light);
  return light;
 }
 GPUPulsingMultiLight2dController *GPUPulsing(Vertexd position, Crayon color, float radius, float cycle) {
  GPUPulsingMultiLight2dController *light=new GPUPulsingMultiLight2dController( &lightpulses,
   lightpulses.Add( cycle, radius, color, (float) position.x, (float) position.y )
  );
  controllers.Append(light);
  return light;
 }
 void RenderObstructions() {
  obstructions.Bind();
  EACH(walls.first,Obstruction2d,o) o->RenderToFBO();
  EACH(objects.first,ObjectiveObstruction,o) EACH(o->obstructions.first,Obstruction2d,p) p->RenderToFBO();
  obstructions.Unbind();
 }
 void MoveLightsBy( double x, double y ) {
  casters.MoveBy(x,y);
  shadowless.MoveBy(x,y);
  shadedLights.MoveBy(x,y);
  statics.MoveBy(x,y);
 }
 void MoveLightboardsBy( float x, float y ) {
  lightboards.MoveBy (x,y);
  lightpatches.MoveBy(x,y);
  lightpoints.MoveBy (x,y);
  lightpulses.MoveBy (x,y);
 }
 void MoveBy( double x, double y ) {
  EACH(walls.first,Obstruction2d,o) o->MoveBy(x,y);
  EACH(objects.first,ObjectiveObstruction,o) EACH(o->obstructions.first,Obstruction2d,p) p->MoveBy(x,y);
//  RenderObstructions();
  screen.MoveByd(x,y);
 }
 void SetAmbient( Crayon color ) {
  ambient.fromCrayon(color);
 }
 virtual void Remove( Light2d *light ) {
  switch ( light->atomic_type ) {
  case l2d_caster:
    casters.Remove(light);
   break;
  case l2d_shadowless:
    shadowless.Remove(light);
   break;
  case l2d_shaded:
    shadowless.Remove(light);
   break;
  case l2d_static:
    statics.Remove(light);
   break;
  }
 }
 virtual void Between() {
  statics.Between();
  shadowless.Between();
  shadedLights.Between();
  casters.Between();
  casters.CastShadows(&walls);
  casters.CastShadows(&objects); // also creates reflections
 }
 /*
  * Use these to modify lighting.
  */
 virtual void PreProcessing() {}
 virtual void PostProcessing() {}
 virtual void PreProcessingPerLight() {}
 virtual void PostProcessingPerLight() {}
 // Refreshes the FBO, blurs the FBO to another FBO, then draws the new FBO to the screen.
 // Before this, in your engine or Proce55or or whatnot, you render a background followed
 // by a full screen quad that is your "ambient" color multiplying the scene down to some
 // value of "darkness".  Then, you call this which renders the two FBOs and then outputs it
 // to the screen multiplying screen content.
 void RenderCPUs( Light2ds *lights ) {  
  EACH(lights->first,Light2d,L) if ( L->lit /*&& L->OnScreen()*/ ) {
   light->Bind();
   PreProcessingPerLight();
   L->Render();
   PostProcessingPerLight();
   light->Unbind();
   accumulator->BindWithoutClearing();
   light->Render(additive,0,0,accumulator->w,accumulator->h);
   accumulator->Unbind();
  }
 }
 void RenderGPUCPU() {
  light->blend=additive;
  accumulator->background.fromCrayon(ambient);
  accumulator->blend=multiply;
  accumulator->Bind();
  PreProcessing();
  Blending(additive);
  EACH(shadedLights.first,ShadedLight2d,S) if ( S->lit ) { //&& S->OnScreen()
//   light->Bind();
   S->Render();
//   light->Unbind();
//   light->Render(0,0,accumulator->w,accumulator->h);
  }
  lightpatches.Render();
  lightboards.Render();
  lightpoints.Render();
  lightpulses.Render();
  accumulator->Unbind();
  RenderCPUs(&statics);
  RenderCPUs(&shadowless);
  RenderCPUs(&casters);
 }
 void Render() {
  shadows2dShader.obstructions=&obstructions.texture;
  RenderGPUCPU();
  PostProcessing();
  RenderAccumulatorFullScreen();
 }
 void RenderWithoutDisplaying() {
  shadows2dShader.obstructions=&obstructions.texture;
  RenderGPUCPU();
  PostProcessing();
 }
 void RenderAccumulatorFullScreen() {
  accumulator->RenderUpsideDown(multiply,screen.x,screen.y,screen.x2,screen.y2);
 }
};

class LightMasks : public Cartesians {
public:
 // Default: black mask, or one of two filter options.  Hacky, I know, but I'm tired.  Fix later, or not.
 Crayon filter1,filter2;
 void SetAllToFilter( int filter1_2) { FOREACH(Cartesian,c) c->a=(float) filter1_2; }
 // Left,Up,Right,Down
 void AddBox( int x, int y, int x2, int y2 ) { // Wound CC
  Cartesian *c;
  int w=abs(x2-x);
  int h=abs(y2-y);
  c=new Cartesian; c->x=x2; c->y=y;   c->x2=x;   c->y2=y;   c->w=w; c->h=0; Append(c); // Down Right, Down Left
  c=new Cartesian; c->x=x;  c->y=y;   c->x2=x;   c->y2=y2;  c->w=0; c->h=h; Append(c); // Down Left, Up Left
  c=new Cartesian; c->x=x;  c->y=y2;  c->x2=x2;  c->y2=y2;  c->w=w; c->h=0; Append(c); // Up Left, Up Right
  c=new Cartesian; c->x=x2; c->y=y2;  c->x2=x2;  c->y2=y;   c->w=0; c->h=h; Append(c); // Up Right, Down Right
 }
};

class LightDescription2d : public ListItem {
public:
 Light2dTypes type;
 // Unified Light Settings
 bool lit;
 double angle,turnRate,spread,emission;
 double radius;      // radius in pixels (w/2)
 GLImage *flare,*source; // Desired light flare billboard
 Blends flareBlend,sourceBlend;
 Vertexd position;   // Position à l'écran
 double intensity;   // (alpha value)
 double attenuation; // attenuation (prefers to be smaller than radius)
 Blends attenuationBlend,shadowBlend;
 Crayon color;       // Couleur de la lumière
 bool reflects;
 double duration,delta;
 bool randomstart;
 //
 LightDescription2d() {
  type=gpulight_2d;
  lit=true;
  reflects=true;
  randomstart=true;
  angle=0.0;
  turnRate=0.1;
  spread=50.0;
  duration=0.0;
  radius=1.0;
  intensity=1.0;
  attenuation=64.0;
  flare=library.Load("data/images/flares/neoflare1.png");
  source=library.Load("data/images/flares/neoflare2.png");
  flareBlend=additive;
  sourceBlend=additive;
  attenuationBlend=multiply;
  shadowBlend=none;
  delta=FRAMETIME;
  emission=0.0;
 }
 SKEYSTART
  SKEY("type",        { SPARAM(type,Light2dTypes,atoi); })
  SKEY("lit",         { SPARAMB(lit); })
  SKEY("reflects",    { SPARAMB(reflects); })
  SKEY("randomstart", { SPARAMB(randomstart); })
  SKEY("angle",       { SPARAM(angle,double,atof); })
  SKEY("turn",        { SPARAM(turnRate,double,atof); })
  SKEY("spread",      { SPARAM(spread,double,atof); })
  SKEY("emission",    { SPARAM(emission,double,atof); })
  SKEY("radius",      { SPARAM(radius,double,atof); })
  SKEY("intensity",   { SPARAM(intensity,double,atof); })
  SKEY("attenuation", { SPARAM(attenuation,double,atof); })
  SKEY("duration",    { SPARAM(duration,double,atof); })
  SKEY("delta",       { SPARAM(delta,double,atof); })
  SKEY("flare",       { SKEYPAIR; flare=library.find(key.c_str()); })
  SKEY("source",      { SKEYPAIR; source=library.find(key.c_str()); })
  SKEY("flareblend",  { SKEYPAIR; flareBlend=(key); })
  SKEY("sourceblend", { SKEYPAIR; sourceBlend=(key); })
  SKEY("attenblend",  { SKEYPAIR; attenuationBlend=(key); })
  SKEY("shadowblend", { SKEYPAIR; shadowBlend=(key); })
  SKEY("color",       { SPARAMColor(color); })
 SKEYEND("LightDescription2d");
 string toString() {
  string out=string("");
  incdent();
  FORMAT(buf,1024,"type %d%s%s%s\n", (int) type, (lit?" lit":""), (reflects?" reflects":""), (randomstart?" randomstart":""));
  out+=indention+string(buf);
  FORMAT(buf,1024,"angle %1.5f turn %1.5f spread %1.5f emission %1.5f radius %1.5f intensity %1.5f attenuation %1.5f duration %1.5f delta %1.9f\n",
   (float) angle, (float) turnRate, (float) spread, (float) emission, (float) radius, (float) intensity, (float) attenuation, (float) duration, (float) delta );
  out+=indention+string(buf);
  if ( flare ) {
   FORMAT(buf,1024,"flare {%s}\n", flare->filename.c_str() );
   out+=indention+string(buf);
  }
  if ( source ) {
   FORMAT(buf,1024,"source {%s}\n", flare->filename.c_str() );
   out+=indention+string(buf);
  }
  if ( flareBlend != additive ) out+=indention+string("flareblend ")+flareBlend.toString()+string("\n");
  if ( sourceBlend != additive ) out+=indention+string("sourceblend ")+sourceBlend.toString()+string("\n");
  if ( attenuationBlend != multiply ) out+=indention+string("attenuationblend ")+attenuationBlend.toString()+string("\n");
  if ( shadowBlend != none ) out+=indention+string("shadowblend ")+shadowBlend.toString()+string("\n");
  out+=indention+string("color ")+color.toString()+string("\n");
  decdent();
  return out;
 }
 LightDescription2d *Clone() {
  LightDescription2d *light=new LightDescription2d;
  light->type=type;
  light->lit=lit;
  light->angle=angle;
  light->spread=spread;
  light->turnRate=turnRate;
  light->radius=radius;
  light->flare=flare;
  light->source=source;
  light->flareBlend=flareBlend;
  light->sourceBlend=sourceBlend;
  light->position.Set(&position);
  light->intensity=intensity;
  light->attenuation=attenuation;
  light->attenuationBlend=attenuationBlend;
  light->shadowBlend=shadowBlend;
  light->color.fromCrayon(color);
  light->reflects=reflects;
  light->duration=duration;
  light->delta=delta;
  light->randomstart=randomstart;
  return light;
 }
};

class LightDescriptions2d : public LinkedList {
public:
 SKEYSTART
  SKEY("light", { SKEYPAIR; LightDescription2d *light=new LightDescription2d; light->fromString(key.c_str()); Append(light); })
 SKEYEND("LightDescriptions2d");
 string toString() {
  string out=string("");
  incdent();
  FOREACH(LightDescription2d,d) out+=indention+string("light {")+d->toString()+string("}\n");
  decdent();
  return out;
 }
 CLEARLISTRESET(LightDescription2d);
 ~LightDescriptions2d() { Clear(); }
 LightDescriptions2d *Clone() {
  LightDescriptions2d *lights=new LightDescriptions2d;
  FOREACH(LightDescription2d,light) lights->Append(light->Clone());
  return lights;
 }
 void CloneTo( LightDescriptions2d *lights ) {
  FOREACH(LightDescription2d,light) lights->Append(light->Clone());
 }
 Light2dHandle *Write_To_Light( Vertexd offset, LightManager2d *lighting, Light2dHandles *handles ) {
  Light2dHandle *added=null;
  Vertexd position;
  FOREACH(LightDescription2d,light) {
   switch ( light->type ) {
               case triangle_spot_2d:
                    {TriangleSpot2d *spot=new TriangleSpot2d;
                    spot->lit=light->lit;
                    spot->angle=            light->angle;
                    spot->turnRate=         light->turnRate;
                    spot->spread=           light->spread;
                    spot->emission=         light->emission;
                    spot->radius=           light->radius;
                    spot->flare=            light->flare;
                    spot->source=           light->source;
                    spot->flareBlend=       light->flareBlend;
                    spot->sourceBlend=      light->sourceBlend;
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    spot->position.Set(&position);
                    spot->intensity=        light->intensity;
                    spot->attenuation=      light->attenuation;
                    spot->attenuationBlend= light->attenuationBlend;
                    spot->shadowBlend=      light->shadowBlend;
                    spot->color.fromCrayon(light->color);
                    spot->actual.fromCrayon(light->color);
                    spot->reflects=         light->reflects;
                    if ( light->randomstart ) spot->expired=double_range(0.0,light->duration);
                    spot->duration=         light->duration;
                    spot->delta=            light->delta;
                    lighting->casters.Append(spot);
                    added=handles->Add((Light2d *) spot);
                    }break;
                  case spot_light_2d:
                    {Spot2d *spot=new Spot2d;
                    spot->lit=light->lit;
                    spot->angle=            light->angle;
                    spot->turnRate=         light->turnRate;
                    spot->spread=           light->spread;
                    spot->emission=         light->emission;
                    spot->radius=           light->radius;
                    spot->flare=            light->flare;
                    spot->source=           light->source;
                    spot->flareBlend=       light->flareBlend;
                    spot->sourceBlend=      light->sourceBlend;
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    spot->MoveTo(position.x,position.y);
                    spot->intensity=        light->intensity;
                    spot->attenuation=      light->attenuation;
                    spot->attenuationBlend= light->attenuationBlend;
                    spot->shadowBlend=      light->shadowBlend;
                    spot->color.fromCrayon(light->color);
                    spot->actual.fromCrayon(light->color);
                    spot->reflects=         light->reflects;
                    if ( light->randomstart ) spot->expired=double_range(0.0,light->duration);
                    spot->duration=         light->duration;
                    spot->delta=            light->delta;
                    lighting->casters.Append(spot);
                    added=handles->Add((Light2d *) spot);
                    }break;
                 case point_light_2d:
                    {Light2d *point=new Light2d;
                    point->lit=light->lit;
                    point->angle=            light->angle;
                    point->turnRate=         light->turnRate;
                    point->radius=           light->radius;
                    point->flare=            light->flare;
                    point->source=           light->source;
                    point->flareBlend=       light->flareBlend;
                    point->sourceBlend=      light->sourceBlend;
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    point->position.Set(&position);
                    point->intensity=        light->intensity;
                    point->attenuation=      light->attenuation;
                    point->attenuationBlend= light->attenuationBlend;
                    point->shadowBlend=      light->shadowBlend;
                    point->color.fromCrayon(light->color);
                    point->actual.fromCrayon(light->color);
                    point->reflects=         light->reflects;
                    if ( light->randomstart ) point->expired=double_range(0.0,light->duration);
                    point->duration=         light->duration;
                    point->delta=            light->delta;
                    lighting->casters.Append(point);
                    added=handles->Add((Light2d *) point);
                    }break;
                case shaded_light_2d:
                    {ShadedLight2d *shaded=new ShadedLight2d;
                    shaded->lit=light->lit;
                    shaded->angle=            light->angle;
                    shaded->turnRate=         light->turnRate;
                    shaded->radius=           light->radius;
                    shaded->flare=            light->flare;
                    shaded->source=           light->source;
                    shaded->flareBlend=       light->flareBlend;
                    shaded->sourceBlend=      light->sourceBlend;
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    shaded->MoveTo(position.x,position.y);
                    shaded->intensity=        light->intensity;
                    shaded->attenuation=      light->attenuation;
                    shaded->attenuationBlend= light->attenuationBlend;
                    shaded->shadowBlend=      light->shadowBlend;
                    shaded->color.fromCrayon(light->color);
                    shaded->actual.fromCrayon(light->color);
                    shaded->reflects=         light->reflects;
                    if ( light->randomstart ) shaded->expired=double_range(0.0,light->duration);
                    shaded->duration=         light->duration;
                    shaded->delta=            light->delta;
                    lighting->shadedLights.Append(shaded);
                    added=handles->Add((Light2d *) shaded);
                    }break;
            case shadowless_light_2d:
                    {ShadowlessLight2d *point=new ShadowlessLight2d;
                    point->lit=light->lit;
                    point->angle=            light->angle;
                    point->turnRate=         light->turnRate;
                    point->radius=           light->radius;
                    point->flare=            light->flare;
                    point->source=           light->source;
                    point->flareBlend=       light->flareBlend;
                    point->sourceBlend=      light->sourceBlend;
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    point->position.Set(&position);
                    point->intensity=        light->intensity;
                    point->attenuation=      light->attenuation;
                    point->attenuationBlend= light->attenuationBlend;
                    point->shadowBlend=      light->shadowBlend;
                    point->color.fromCrayon(light->color);
                    point->actual.fromCrayon(light->color);
                    point->reflects=         light->reflects;
                    if ( light->randomstart ) point->expired=double_range(0.0,light->duration);
                    point->duration=         light->duration;
                    point->delta=            light->delta;
                    lighting->shadowless.Append(point);
                    added=handles->Add((Light2d *) point);
                    }break;
     case shadowless_shaded_light_2d:
                    {ShadedShadowlessLight2d *shaded=new ShadedShadowlessLight2d;
                    shaded->lit=light->lit;
                    shaded->angle=            light->angle;
                    shaded->turnRate=         light->turnRate;
                    shaded->radius=           light->radius;
                    shaded->flare=            light->flare;
                    shaded->source=           light->source;
                    shaded->flareBlend=       light->flareBlend;
                    shaded->sourceBlend=      light->sourceBlend;
                    position.Set(display.w-offset.x+light->position.x,display.h-offset.y+light->position.y);
                    shaded->MoveTo(position.x,position.y);
                    shaded->intensity=        light->intensity;
                    shaded->attenuation=      light->attenuation;
                    shaded->attenuationBlend= light->attenuationBlend;
                    shaded->shadowBlend=      light->shadowBlend;
                    shaded->color.fromCrayon(light->color);
                    shaded->actual.fromCrayon(light->color);
                    shaded->reflects=         light->reflects;
                    if ( light->randomstart ) shaded->expired=double_range(0.0,light->duration);
                    shaded->duration=         light->duration;
                    shaded->delta=            light->delta;
                    lighting->shadedLights.Append(shaded);
                    added=handles->Add((Light2d *) shaded);
                    }break;
     case light_beam_2d:
                    {
                    }break;
     case static_spot_2d:
                    {Spot2d *spot=new Spot2d;
                    spot->lit=light->lit;
                    spot->angle=            light->angle;
                    spot->turnRate=         light->turnRate;
                    spot->spread=           light->spread;
                    spot->emission=         light->emission;
                    spot->radius=           light->radius;
                    spot->flare=            light->flare;
                    spot->source=           light->source;
                    spot->flareBlend=       light->flareBlend;
                    spot->sourceBlend=      light->sourceBlend;
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    spot->MoveTo(position.x,position.y);
                    spot->intensity=        light->intensity;
                    spot->attenuation=      light->attenuation;
                    spot->attenuationBlend= light->attenuationBlend;
                    spot->shadowBlend=      light->shadowBlend;
                    spot->color.fromCrayon(light->color);
                    spot->actual.fromCrayon(light->color);
                    spot->reflects=         light->reflects;
                    if ( light->randomstart ) spot->expired=double_range(0.0,light->duration);
                    spot->duration=         light->duration;
                    spot->delta=            light->delta;
                    lighting->casters.Append(spot);
                    added=handles->Add((Light2d *) spot);
                    }break;
     case static_point_2d:
                    {StaticLight2d *point=new StaticLight2d;
                    point->lit=light->lit;
                    point->angle=            light->angle;
                    point->turnRate=         light->turnRate;
                    point->radius=           light->radius;
                    point->flare=            light->flare;
                    point->source=           light->source;
                    point->flareBlend=       light->flareBlend;
                    point->sourceBlend=      light->sourceBlend;
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    point->position.Set(&position);
                    point->intensity=        light->intensity;
                    point->attenuation=      light->attenuation;
                    point->attenuationBlend= light->attenuationBlend;
                    point->shadowBlend=      light->shadowBlend;
                    point->color.fromCrayon(light->color);
                    point->actual.fromCrayon(light->color);
                    point->reflects=         light->reflects;
                    if ( light->randomstart ) point->expired=double_range(0.0,light->duration);
                    point->duration=         light->duration;
                    point->delta=            light->delta;
                    lighting->statics.Append(point);
                    added=handles->Add((Light2d *) point);
                    }break;
     case pulsing_point_2d:
                    {PulsingLight2d *point=new PulsingLight2d;
                    point->lit=light->lit;
                    point->angle=            light->angle;
                    point->turnRate=         light->turnRate;
                    point->radius=           light->radius;
                    point->flare=            light->flare;
                    point->source=           light->source;
                    point->flareBlend=       light->flareBlend;
                    point->sourceBlend=      light->sourceBlend;
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    point->position.Set(&position);
                    point->intensity=        light->intensity;
                    point->attenuation=      light->attenuation;
                    point->attenuationBlend= light->attenuationBlend;
                    point->shadowBlend=      light->shadowBlend;
                    point->color.fromCrayon(light->color);
                    point->actual.fromCrayon(light->color);
                    point->reflects=         light->reflects;
                    if ( light->randomstart ) point->expired=double_range(0.0,light->duration);
                    point->duration=         light->duration;
                    point->delta=            light->delta;
                    lighting->casters.Append(point);
                    added=handles->Add((Light2d *) point);
                    }break;
     case shadowless_static_point_2d:
                    {ShadowlessStaticLight2d *point=new ShadowlessStaticLight2d;
                    point->lit=light->lit;
                    point->angle=            light->angle;
                    point->turnRate=         light->turnRate;
                    point->radius=           light->radius;
                    point->flare=            light->flare;
                    point->source=           light->source;
                    point->flareBlend=       light->flareBlend;
                    point->sourceBlend=      light->sourceBlend;
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    point->position.Set(&position);
                    point->intensity=        light->intensity;
                    point->attenuation=      light->attenuation;
                    point->attenuationBlend= light->attenuationBlend;
                    point->shadowBlend=      light->shadowBlend;
                    point->color.fromCrayon(light->color);
                    point->actual.fromCrayon(light->color);
                    point->reflects=         light->reflects;
                    if ( light->randomstart ) point->expired=double_range(0.0,light->duration);
                    point->duration=         light->duration;
                    point->delta=            light->delta;
                    lighting->shadowless.Append(point);
                    added=handles->Add((Light2d *) point);
                    }break;
     case shadowless_pulsing_point_2d:
                    {ShadowlessPulsingLight2d *point=new ShadowlessPulsingLight2d;
                    point->lit=light->lit;
                    point->angle=            light->angle;
                    point->turnRate=         light->turnRate;
                    point->radius=           light->radius;
                    point->flare=            light->flare;
                    point->source=           light->source;
                    point->flareBlend=       light->flareBlend;
                    point->sourceBlend=      light->sourceBlend;
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    point->position.Set(&position);
                    point->intensity=        light->intensity;
                    point->attenuation=      light->attenuation;
                    point->attenuationBlend= light->attenuationBlend;
                    point->shadowBlend=      light->shadowBlend;
                    point->color.fromCrayon(light->color);
                    point->actual.fromCrayon(light->color);
                    point->reflects=         light->reflects;
                    if ( light->randomstart ) point->expired=double_range(0.0,light->duration);
                    point->duration=         light->duration;
                    point->delta=            light->delta;
                    lighting->shadowless.Append(point);
                    added=handles->Add((Light2d *) point);
                    }break;
     case pulsing_spot_2d:
                    {PulsingSpot2d *spot=new PulsingSpot2d;
                    spot->lit=light->lit;
                    spot->angle=            light->angle;
                    spot->turnRate=         light->turnRate;
                    spot->spread=           light->spread;
                    spot->emission=         light->emission;
                    spot->radius=           light->radius;
                    spot->flare=            light->flare;
                    spot->source=           light->source;
                    spot->flareBlend=       light->flareBlend;
                    spot->sourceBlend=      light->sourceBlend;
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    spot->MoveTo(position.x,position.y);
                    spot->intensity=        light->intensity;
                    spot->attenuation=      light->attenuation;
                    spot->attenuationBlend= light->attenuationBlend;
                    spot->shadowBlend=      light->shadowBlend;
                    spot->color.fromCrayon(light->color);
                    spot->actual.fromCrayon(light->color);
                    spot->reflects=         light->reflects;
                    if ( light->randomstart ) spot->expired=double_range(0.0,light->duration);
                    spot->duration=         light->duration;
                    spot->delta=            light->delta;
                    lighting->casters.Append(spot);
                    added=handles->Add((Light2d *) spot);
                    }break;
     case shadowless_pulsing_spot_2d:
                    {ShadowlessPulsingSpot2d *spot=new ShadowlessPulsingSpot2d;
                    spot->lit=light->lit;
                    spot->angle=            light->angle;
                    spot->turnRate=         light->turnRate;
                    spot->spread=           light->spread;
                    spot->emission=         light->emission;
                    spot->radius=           light->radius;
                    spot->flare=            light->flare;
                    spot->source=           light->source;
                    spot->flareBlend=       light->flareBlend;
                    spot->sourceBlend=      light->sourceBlend;
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    spot->MoveTo(position.x,position.y);
                    spot->intensity=        light->intensity;
                    spot->attenuation=      light->attenuation;
                    spot->attenuationBlend= light->attenuationBlend;
                    spot->shadowBlend=      light->shadowBlend;
                    spot->color.fromCrayon(light->color);
                    spot->actual.fromCrayon(light->color);
                    spot->reflects=         light->reflects;
                    if ( light->randomstart ) spot->expired=double_range(0.0,light->duration);
                    spot->duration=         light->duration;
                    spot->delta=            light->delta;
                    lighting->shadowless.Append(spot);
                    added=handles->Add((Light2d *) spot);
                    }break;
     case shadowless_spot_2d:
                    {ShadowlessSpot2d *spot=new ShadowlessSpot2d;
                    spot->lit=light->lit;
                    spot->angle=            light->angle;
                    spot->turnRate=         light->turnRate;
                    spot->spread=           light->spread;
                    spot->emission=         light->emission;
                    spot->radius=           light->radius;
                    spot->flare=            light->flare;
                    spot->source=           light->source;
                    spot->flareBlend=       light->flareBlend;
                    spot->sourceBlend=      light->sourceBlend;
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    spot->MoveTo(position.x,position.y);
                    spot->intensity=        light->intensity;
                    spot->attenuation=      light->attenuation;
                    spot->attenuationBlend= light->attenuationBlend;
                    spot->shadowBlend=      light->shadowBlend;
                    spot->color.fromCrayon(light->color);
                    spot->actual.fromCrayon(light->color);
                    spot->reflects=         light->reflects;
                    if ( light->randomstart ) spot->expired=double_range(0.0,light->duration);
                    spot->duration=         light->duration;
                    spot->delta=            light->delta;
                    lighting->shadowless.Append(spot);
                    added=handles->Add((Light2d *) spot);
                    }break;
     case shadowless_static_spot_2d:
                    {ShadowlessStaticSpot2d *spot=new ShadowlessStaticSpot2d;
                    spot->lit=light->lit;
                    spot->angle=            light->angle;
                    spot->turnRate=         light->turnRate;
                    spot->spread=           light->spread;
                    spot->emission=         light->emission;
                    spot->radius=           light->radius;
                    spot->flare=            light->flare;
                    spot->source=           light->source;
                    spot->flareBlend=       light->flareBlend;
                    spot->sourceBlend=      light->sourceBlend;
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    spot->MoveTo(position.x,position.y);
                    spot->intensity=        light->intensity;
                    spot->attenuation=      light->attenuation;
                    spot->attenuationBlend= light->attenuationBlend;
                    spot->shadowBlend=      light->shadowBlend;
                    spot->color.fromCrayon(light->color);
                    spot->actual.fromCrayon(light->color);
                    spot->reflects=         light->reflects;
                    if ( light->randomstart ) spot->expired=double_range(0.0,light->duration);
                    spot->duration=         light->duration;
                    spot->delta=            light->delta;
                    lighting->statics.Append(spot);
                    added=handles->Add((Light2d *) spot);
                    }break;
     case lightboard_static_2d:
     case gpulight_2d:
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    {GPUMultiLight2dController *point=lighting->GPULight( position, (float) light->radius, light->color );
                     added=handles->Add((Light2d *) point);
                    }break;
     case patch_gpulight_2d:
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    {GPUNoRadiusMultiLight2dController *point=lighting->GPUPatch( position, light->color );
                     added=handles->Add((Light2d *) point);
                    }break;
     case point_gpulight_2d:
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    {GPUNoColorMultiLight2dController *point=lighting->GPUPoint( position );
                     added=handles->Add((Light2d *) point);
                    }break;
     case pulsing_gpulight_2d:
                    position.Set(offset.x+light->position.x,offset.y+light->position.y);
                    {GPUPulsingMultiLight2dController *point=lighting->GPUPulsing( position, light->color, (float) light->radius,
                      light->randomstart ? (float) (light->duration/2.0+uniform()*light->duration) : (float) light->duration );
                     added=handles->Add((Light2d *) point);
                    }break;

   }
  }
  return added;
 }
};