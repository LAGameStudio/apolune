#pragma once

#include "ColorNetwork.h"
#include "Universe.h"
#include "LightningSystem2d.h"
#include "Lithosphere.h"
#include "Stratosphere.h"
#include "ClassicPlanetSurfacer.h"

class LandfallEnvironment {
public:
 Zint seed;
 Magnetosphere magnetosphere;
 // Weather and seismic activity settings
 Zfloat rainFactor,rainMax; // increases seaLevel temporarily
 Zfloat stormTime,stormDuration; // where are during the current weather system
 Zfloat stormIntensity,stormFrequency;  // modifies cloud amount, determines how often rain occurs
 Zbool hasPrecipitation;
 Zdouble minStormIntensity,maxStormIntensity,minStormLength,maxStormLength,stormVolatility;
 StormPhases current,going;
 Zdouble magnetics;
 Zbool stormOnly; // when stormOnly is false, lightning strikes whenever
 Zbool lightningBolts,areaLightning;
 Zdouble staticElectricity,staticVolatility,staticDelta,staticSpeed,maxStaticVolatility;
 ColorGear cloudColors;
 Crayon snowColor;
 Zdouble accumulation,maxAccumulation,accumulationFadeIn;
 // determines how often lightning strikes, how volatile lightning is, and where its heading on the static index
 Zdouble tornados; // determines how often tornados form during storms
 Zdouble hurricanes;  // determines how often hurricanes form during storms
 Zdouble seismicActivity,seismicDuration,seismicIntensity; // earthquake parameters
 Zfloat tectonics; // movement of mountains (erosionTime)
 Zbool radioactive;  // uses bloom to blur/emph the planet more
 Zbool onlySnows,neverSnows;
 Blends rainBlend;
 // water
 Zdouble mmhg,mmhgDelta; // atmospheric change
 Zfloat waterTime; // for water shimmer
 Zfloat waterShimmer;
 Crayon waterColor,waterReflection;
 Zfloat shimmerThickness,shimmerFactor,shimmerSpeed;
 Zfloat seaLevel,tideExtremes,tideSpeed,tide,tidalMovement;
 // land
 Zfloat erosionTime; // mountain combobulator
 Zfloat mountainHeight,mountainFactor;
 // air
 Zfloat windTime; // for moving clouds
 Zfloat windSpeed;
 Zfloat windDirection; // only supports + or -
 Zfloat cloudThickness,cloudFactor;
 Crayon cloudColor,cloudShadowThickness;
 Zint cloudHeight;

 ForeverWrappingTiledFilms precipitation;
 Zp<ForeverWrappingTiledFilm> splatterFilm,rainFilm,snowFilm;
 Zp<Tween> stormShape;

 // Deltas (change each tick)
 Zdouble cloudFloor,cloudCover,cloudAmpTarget,cloudAmpDelta,windSpeedTarget,windSpeedDelta;
 Zdouble raining,rainingAmountTarget,rainingAmountDelta;
 Zdouble snowing,snowingAmountTarget,snowingAmountDelta;
 Zdouble accumulationTarget,accumulationDelta;
 Zdouble droughtIntensity,drought,droughtTarget,droughtDelta; // both drought and wetness
 Crayon sea;
 Zdouble droughtPlusSeaLevel,droughtCondition;
 Zfloat waterTable;

 LandfallEnvironment() {
  rainBlend=transparency;
  splatterFilm=precipitation.Add(vault.find("Rain","Splatter"),rainBlend,crayons.Pick(alabaster));
  rainFilm=precipitation.Add(vault.find("Rain","Tile"),rainBlend,crayons.Pick(alabaster));
  snowFilm=precipitation.Add(vault.find("Rain","Snow"),rainBlend,crayons.Pick(alabaster));
  precipitation.MoveBy(0.0,0.0);
  splatterFilm->tiles.Set(5.0,5.0);
  rainFilm->tiles.Set(6.0,6.0);
  snowFilm->tiles.Set(10.0,10.0);
 }

 double AccumMod( double acc ) {
  if ( acc >= (1.0-accumulationFadeIn) ) {
   double t= 1.0-sin(((acc-(1.0-accumulationFadeIn))/accumulationFadeIn)*HALF_PI);
   return t;
  }
  return 1.0;
 }

 Zp<ClassicPlanetSurfaceProfile> profile;
 void Setup( ClassicPlanetSurfaceProfile *p ) {
  profile=p;
  precipitation.screen.Set(0,0,display.w,display.h);
  rainBlend=seed%2==1?additive:transparency;
  radioactive=p->radioactive;
  mountainHeight=2.0f*p->mountainHeight;
  mountainFactor=7.2f*p->mountainFactor;
  shimmerFactor=cloudFactor=3.0f*p->shimmerFactor;
  shimmerThickness=3.5f*p->shimmerThickness;
  cloudThickness=4.0f*p->cloudThickness;
  cloudFloor=0.1f*p->cloudFloor;
  windSpeed=0.001f+0.01f*p->windSpeed;
  shimmerSpeed=0.005f*p->shimmerSpeed;
  windDirection=1.0f;
  magnetics=p->magnetics;
  stormOnly=p->stormOnly;
  seaLevel=p->seaLevel;
  tideExtremes=iratiof(5,255)*p->tideExtremes;
  tideSpeed=PIf/100.0f*p->tideSpeed;
  cloudHeight=p->cloudHeight;
  cloudColor.Any(p->cloudColorSeed);
  waterColor.Any(p->waterColorSeed);
  waterReflection.Any(p->waterColorSeed+2);
  erosionTime=p->erosionTime;
  cloudColors.Clear();
  cloudColors.Add(crayons.Any(p->cloudColorSeed+4));
  cloudColors.Add(crayons.Any(p->cloudColorSeed+5));
  cloudColors.Add(crayons.Any(p->cloudColorSeed+6));
  cloudColor.Pick(alabaster);
  snowColor.fromCrayon(p->snowColor);
  droughtIntensity=p->droughtIntensity;
  stormVolatility=p->stormVolatility;
  double resizeFactor=5.0+(1.0-4.0*p->resizeFactor);
  splatterFilm->tiles.Set(resizeFactor,resizeFactor);
  rainFilm->tiles.Set(resizeFactor,resizeFactor);
  snowFilm->tiles.Set(resizeFactor,resizeFactor);
  cloudShadowThickness.Double(0.0,0.0,0.0,p->cloudShadowThickness);
  lightningBolts=p->lightningBolts;
  areaLightning=p->areaLightning;
  tectonics=FRAMETIME*p->tectonics;
  shimmerSpeed+=FRAMETIME*p->shimmerSpeed;
  shimmerFactor*=p->shimmerFactor;
  stormShape=p->stormShape;
 }

 /*
 void Seed( int seed ) {
  precipitation.screen.Set(0,0,display.w,display.h);
  magnetosphere.Seed(seed+321);
  Random *setA=randoms.GetSet(seed);
  Random *setB=randoms.GetSet(seed+3);
  Random *setC=randoms.GetSet(seed+54);
  Random *setD=randoms.GetSet(seed+2);
  rainBlend=seed%2==1?additive:transparency;
  radioactive=(seed%2==1);
  mountainHeight=2.0f*(float)setA->aunique(seed);
  mountainFactor=7.2f*(float)setB->aunique(seed);
  shimmerFactor=cloudFactor=3.0f*(float)setC->aunique(seed);
  shimmerThickness=3.5f*(float)setD->aunique(seed);
  cloudThickness=4.0f*(float)(setA->aunique(seed+2)+setA->aunique(seed+4));
  cloudFloor=0.1f*(float)(setB->aunique(seed+2)+setB->aunique(seed+4));
  windSpeed=0.001f+0.01f*(float)setA->aunique(seed+99);
  shimmerSpeed=0.005f*(float)setB->aunique(seed+88);
  windDirection=1.0f;
  if ( setC->Query(seed,seed/3) ) magnetics=iratiod(10,100)*(float)setC->aunique(seed+12345);
  else magnetics=0.0;
  stormOnly=setC->Query(seed,seed+1);
  seaLevel=iratiof(100,255)*(float)setC->aunique(seed+77);
  tideExtremes=iratiof(5,255)*(float)setD->aunique(seed+66);
  tideSpeed=PIf/100.0f*(float)setC->aunique(seed+44);
  cloudHeight=128;
  cloudColor.Any(seed+1);
  waterColor.Any(seed+2);
  waterReflection.Any(seed+3);
  erosionTime=setC->unique(seed+12);
  cloudColors.Clear();
  cloudColors.Add(crayons.Any(seed+4));
  cloudColors.Add(crayons.Any(seed+5));
  cloudColors.Add(crayons.Any(seed+6));
  snowColor.Brights(seed);
  droughtIntensity=((setA->aunique(11+seed)+setC->aunique(66+seed))/2.0);
  stormVolatility=0.5+(setD->unique(123+seed)*0.5);
  if ( stormVolatility > 1.0 ) stormVolatility=1.0;
  if ( stormVolatility < 0.0 ) stormVolatility=0.0;
  double resizeFactor=5.0+(1.0-4.0*setD->aunique(seed));
  splatterFilm->tiles.Set(resizeFactor,resizeFactor);
  rainFilm->tiles.Set(resizeFactor,resizeFactor);
  snowFilm->tiles.Set(resizeFactor,resizeFactor);
 }
 */

 void Init() {
  current=storm_sunny;
  going=storm_partly_cloudy;
  droughtIntensity=0.2;
  rainFactor=0.0f;
  rainMax=0.5f; // increases seaLevel temporarily
  stormTime=0.0f;
  stormDuration=30.0f; // where are during the current weather system
  stormIntensity=0.05f;
  stormFrequency=60.0f;  // modifies cloud amount, determines how often rain occurs
  hasPrecipitation=true;
  minStormIntensity=0.05f;
  maxStormIntensity=0.1f;
  minStormLength=10.0f;
  maxStormLength=90.0f;
  stormVolatility=0.5f; // how often things get worse
  snowColor.Pick(alabaster);
  accumulation=1.0;
  maxAccumulation=0.9;
  accumulationFadeIn=0.1;
  magnetics=0.01;
  stormOnly=true;
  lightningBolts=true;
  areaLightning=true;
  staticElectricity=0.1;
  staticVolatility=0.0;
  staticDelta=0.001;
  staticSpeed=0.05;
  maxStaticVolatility=0.5;
  cloudColors.Add(crayons.Pick(alabaster));
  cloudColors.Add(crayons.Pick(cyan));
  mountainHeight=2.0f;
  mountainFactor=7.2f;
  shimmerFactor=cloudFactor=3.0f;
  shimmerThickness=3.5f;
  cloudThickness=4.0f;
  windSpeed=0.01f;
  shimmerSpeed=0.005f;
  windDirection=1.0f;
  seaLevel=iratiof(100,255);
  tideExtremes=iratiof(5,255);
  tideSpeed=PIf/100.0f;
  cloudHeight=128;
  cloudColor.Float(1.0f,1.0f,1.0f,1.0f);
  cloudShadowThickness.Float(0.0f,0.0f,0.0f,0.25f);
  waterColor.Pick(blue);
  waterReflection.Pick(cyan);
  mmhg=0.0;
  mmhgDelta=0.001;//FRAMETIME;///60.0; // /(60.0*60.0*0.1);
 }

 void UpdateLightning() {
  if ( magnetics > 0.0 ) {
   if ( !stormOnly || current == storm_thunder ) {
    if ( uniform() < this->magnetics ) {
     magnetosphere.Impact( seed%12,
      -display.wd*0.5+2.0*uniform()*display.wd, 
      -display.hd*0.5+2.0*uniform()*display.hd
     );
    }
   }
  }
 }

 void InitStorm() {
  switch ( current ) {
  case storm_sunny:
   droughtTarget=-1.0;
   droughtDelta=0.001;
   cloudAmpTarget=0.0;
   cloudAmpDelta=0.005;
   rainingAmountTarget=0.0;
   rainingAmountDelta=0.1;
   break;
  case storm_partly_cloudy:
   droughtTarget=0.0;
   droughtDelta=0.001;
   cloudAmpTarget=0.5;
   cloudAmpDelta=0.001;
   accumulationTarget=1.0f;
   accumulationDelta=0.01;
   rainingAmountTarget=0.0;
   rainingAmountDelta=0.01;
   break;
  case storm_light_rain:
   droughtTarget=0.125;
   droughtDelta=0.0001;
   rainingAmountTarget=1.0;
   rainingAmountDelta=0.001;
   accumulationTarget=1.0f;
   accumulationDelta=0.001;
   cloudAmpTarget=0.5;
   cloudAmpDelta=0.005;
   break;
  case storm_thunder:
   droughtTarget=0.25;
   droughtDelta=0.0001;
   snowingAmountTarget=0.0;
   snowingAmountDelta=0.001;
   cloudAmpTarget=1.0;
   cloudAmpDelta=0.01;
   accumulationTarget=1.0f;
   accumulationDelta=0.001;
   break;
  case storm_rain_and_snow:
   droughtTarget=0.5;
   droughtDelta=0.0001;
   rainingAmountTarget=0.0;
   rainingAmountDelta=0.001;
   snowingAmountTarget=1.0;
   snowingAmountDelta=0.001;
   accumulationTarget=0.9f;
   accumulationDelta=0.0001;
   break;
  case storm_snow:
   droughtTarget=0.6;
   droughtDelta=0.0001;
   snowingAmountTarget=1.0;
   snowingAmountDelta=0.0001;
   accumulationTarget=0.5f;
   accumulationDelta=0.00001;
   break;
  case storm_blizzard:
   droughtTarget=0.7;
   droughtDelta=0.0001;
   snowingAmountTarget=1.0;
   snowingAmountDelta=0.0001;
   accumulationTarget=0.25f;
   accumulationDelta=0.0001;
   break;
  case storm_deep_freeze:
   droughtTarget=1.0;
   droughtDelta=0.0001;
   snowingAmountTarget=0.0;
   snowingAmountDelta=0.001;
   accumulationTarget=0.01f;
   accumulationDelta=0.00001;
   break;
  }
 }
 void UpdateStorm() {
  TOWARDS(snowing,snowingAmountTarget,snowingAmountDelta);
  TOWARDS(raining,rainingAmountTarget,rainingAmountDelta);
  TOWARDS(accumulation,accumulationTarget,accumulationDelta);
  TOWARDS(drought,droughtTarget,droughtDelta);
  TOWARDS(cloudCover,cloudAmpTarget,cloudAmpDelta);
  UpdateLightning();
  /*
  switch ( current ) {
  case storm_sunny:
   break;
  case storm_partly_cloudy:
   break;
  case storm_light_rain:
   break;
  case storm_thunder:
   break;
  case storm_rain_and_snow:
   break;
  case storm_snow:
   break;
  case storm_blizzard:
   break;
  case storm_deep_freeze:
   break;
  }*/
 }
 void UpdateWeather() {
  mmhg+=mmhgDelta;
  if ( mmhg >= 1.0 ) { // change the weather
   mmhg=0.0+mmhgDelta+uniform()*0.25;
   current=going;
   InitStorm();
   if ( (int) going == MAX_STORM || uniform() > stormVolatility ) { // got better
    if ( going != storm_sunny ) {
     going=(StormPhases) ( (int) going - 1 );
     if ( going == storm_rain_and_snow && onlySnows ) going = storm_partly_cloudy;
    }
   } else { // got worse
    going=(StormPhases) ( (int) going + 1 );
    if ( going == storm_light_rain && onlySnows ) going = storm_snow;
    else if ( going == storm_rain_and_snow && neverSnows ) going = storm_thunder;
   }
  }
  snowFilm->blend=rainBlend;
  snowFilm->tint.Float(snowColor.rf,snowColor.gf,snowColor.bf,(float)snowing);
  splatterFilm->blend=rainBlend;
  splatterFilm->tint.Float(snowColor.rf,snowColor.gf,snowColor.bf,(float)raining);
  rainFilm->blend=rainBlend;
  rainFilm->tint.Float(snowColor.rf,snowColor.gf,snowColor.bf,(float)raining);
  precipitation.Between();
  UpdateStorm();
 }
 void Randomize() {
  mountainHeight=2.0f*(float)uniform();
  mountainFactor=7.2f*(float)uniform();
  shimmerFactor=cloudFactor=3.0f*(float)uniform();
  shimmerThickness=3.5f*(float)uniform();
  cloudThickness=4.0f*(float)(uniform()+uniform());
  cloudFloor=0.1f*(float)(uniform()+uniform());
  windSpeed=0.01f*(float)uniform();
  shimmerSpeed=0.005f*(float)uniform();
  windDirection=1.0f;
  seaLevel=iratiof(100,255)*(float)uniform();
  tideExtremes=iratiof(5,255)*(float)uniform();
  tideSpeed=PIf/100.0f*(float)uniform();
  cloudHeight=128;
  cloudColor.Any();
  waterColor.Any();
  waterReflection.Any();
  cloudColors.Clear();
  cloudColors.Add(crayons.Any());
  cloudColors.Add(crayons.Any());
  cloudColors.Add(crayons.Any());
 }
 void Between() {
  UpdateWeather();
  tidalMovement+=tideSpeed;
  tide=sin(tidalMovement)*tideExtremes;
  waterTime+=FRAMETIME*shimmerSpeed*windDirection;
  windTime+=FRAMETIME*windSpeed*windDirection;
  erosionTime+=tectonics;
  magnetosphere.Between();
  droughtCondition=seaLevel+droughtIntensity*drought;
  droughtPlusSeaLevel=CLAMPTO(droughtCondition,0.0,0.99);
  waterTable=(float)droughtPlusSeaLevel+tide;
  sea.Float(waterTable,waterTable,waterTable,1.0f);
 }
 void MoveBy( double x, double y ) {
  precipitation.MoveBy(x/10.0,y/10.0);
  magnetosphere.MoveBy(-x,-y);
 }
 void DebugInfo();
};