#include "LandfallEnvironment.h"

#include "UglyFont.h"

void LandfallEnvironment::DebugInfo() {
 crayons.Pick(blue).gl();
 string out=  FORMAT(buf,1024,
   "seed: %d\n"
   "sea level: %f tide: %f\n"
   "accumulation: %f -> %f\n"
   "weather type: %d -> %d\n"
   "mmhg: %f (delta: %f, volatility: %f)\n"
   "snowing: %f -> %f\n"
   "raining: %f -> %f\n"
   "clouds: %f -> %f\n"
   "drought: %f -> %f <- %f = %f (%f)\n"
   "water table: %f\n"
   "radioactive: %s\n"
   "magnetics: %f\n"
   "stormOnly: %s\n"
   "precipitation type: %s\n"
   ,
   (int) seed,
   (float) seaLevel,
   (float) tide,
   (float) accumulation,
   (float) accumulationTarget,
   (int)   current,
   (int)   going,
   (float) mmhg,
   (float) mmhgDelta,
   (float) stormVolatility,
   (float) snowing,
   (float) snowingAmountTarget,
   (float) raining,
   (float) rainingAmountTarget,
   (float) cloudCover,
   (float) cloudAmpTarget,
   (float) drought,
   (float) droughtTarget,
   (float) droughtIntensity,
   (double)droughtPlusSeaLevel,
   (double)droughtCondition,
   (float) waterTable,
   (float) magnetics,
   stormOnly ? "yes" : "no",
   current == storm_sunny ? "sunny" :
   current == storm_partly_cloudy ? "partly cloudy" :
   current == storm_light_rain ? "light rain" :
   current == storm_thunder ? "thunder" :
   current == storm_rain_and_snow ? "rain and snow" :
   current == storm_snow ? "snow" :
   current == storm_blizzard ? "blizzard" :
   current == storm_deep_freeze ? "deep freeze"
   : "unknown"
 );
 MultilineText(out,512,200,8,7,2,false);
 if ( input.KeyDown(DX_NUMPADPLUS) ) {
  seaLevel+=iratiof(1,255);
 } else if ( input.KeyDown(DX_NUMPADMINUS) ) {
  seaLevel-=iratiof(1,255);
 } else if ( input.KeyDown(DX_PGUP) ) {
  accumulation+=iratiof(1,255);
 } else if ( input.KeyDown(DX_PGDN) ) {
  accumulation-=iratiof(1,255);
 }
}