#pragma once
#include "Art.h"
#include "TextureLibrary.h"
#include "Sunlamps.h"

ONE(ClassicPlanetSurfaceProfile,{
 magnetoSeed=44;
 cloudShadowThickness=0.25;
 cloudColorSeed=22;
 waterColorSeed=33;
 mountainFactor=0.5;
 mountainHeight=1.0;
 cloudHeight=128.0;
 shimmerSpeed=1.0;
 shimmerFactor=10.0;
 bloomMod=0.001;
 tectonics=1.0;
 seaLevel=100.0;
 waterNoise=library.Load("data/images/noise1.png",GL_REPEAT,GL_REPEAT);
 stormShape=tweens.find("Sine");
})
 Zbool hasPrecipitation,stormOnly,radioactive,areaLightning,lightningBolts;
 Zint magnetoSeed,environment_seed,cloudColorSeed,waterColorSeed,regionsSeed;
 Zdouble cloudShadowThickness,erosionTime,windTime,cloudFactor,cloudFloor,
         cloudCover,cloudThickness,mountainFactor,mountainHeight,waterTime,
         accumulation,shimmerThickness,windSpeed,droughtIntensity,stormVolatility,
         resizeFactor,tideExtremes,tideSpeed,cloudHeight,shimmerSpeed,shimmerFactor,
         bloomMod,tectonics,magnetics,seaLevel;
 Sunlamps lamps;
 Crayon waterColor,cTint1,cTint2,cTint3,cTint4,hTint1,hTint2,hTint3,hTint4,seaColor,waterReflection,snowColor,rThreshold1,rThreshold2,rThreshold3,rThreshold4;
 Zp<GLImage> waterNoise,cImage1,cImage2,cImage3,cImage4,hImage1,hImage2,hImage3,hImage4,rImage1,rImage2,rImage3,rImage4;
 Zp<Tween> stormShape;
 void Set( ClassicPlanetSurfaceProfile *in ) {
  hasPrecipitation=in->hasPrecipitation;
  stormOnly=in->stormOnly;
  magnetoSeed=in->magnetoSeed;
  radioactive=in->radioactive;
  cloudShadowThickness=in->cloudShadowThickness;
  areaLightning=in->areaLightning;
  lightningBolts=in->lightningBolts;
  environment_seed=in->environment_seed;
  cloudColorSeed=in->cloudColorSeed;
  waterColorSeed=in->waterColorSeed;
  regionsSeed=in->regionsSeed;
  erosionTime=in->erosionTime;
  windTime=in->windTime;
  cloudFactor=in->cloudFactor;
  cloudFloor=in->cloudFloor;
  cloudCover=in->cloudCover;
  cloudThickness=in->cloudThickness;
  mountainFactor=in->mountainFactor;
  mountainHeight=in->mountainHeight;
  waterTime=in->waterTime;
  accumulation=in->accumulation;
  shimmerThickness=in->shimmerThickness;
  windSpeed=in->windSpeed;
  droughtIntensity=in->droughtIntensity;
  stormVolatility=in->stormVolatility;
  resizeFactor=in->resizeFactor;
  tideExtremes=in->tideExtremes;
  tideSpeed=in->tideSpeed;
  cloudHeight=in->cloudHeight;
  shimmerSpeed=in->shimmerSpeed;
  shimmerFactor=in->shimmerFactor;
  bloomMod=in->bloomMod;
  tectonics=in->tectonics;
  magnetics=in->magnetics;
  seaLevel=in->seaLevel;
  lamps.Set(&in->lamps);
  waterColor=in->waterColor;
  cTint1=in->cTint1;
  cTint2=in->cTint2;
  cTint3=in->cTint3;
  cTint4=in->cTint4;
  hTint1=in->hTint1;
  hTint2=in->hTint2;
  hTint3=in->hTint3;
  hTint4=in->hTint4;
  seaColor=in->seaColor;
  waterReflection=in->waterReflection;
  snowColor=in->snowColor;
  rThreshold1=in->rThreshold1;
  rThreshold2=in->rThreshold2;
  rThreshold3=in->rThreshold3;
  rThreshold4=in->rThreshold4;
  cImage1=in->cImage1;
  cImage2=in->cImage2;
  cImage3=in->cImage3;
  cImage4=in->cImage4;
  hImage1=in->hImage1;
  hImage2=in->hImage2;
  hImage3=in->hImage3;
  hImage4=in->hImage4;
  waterNoise=in->waterNoise;
  rImage1=in->rImage1;
  rImage2=in->rImage2;
  rImage3=in->rImage3;
  rImage4=in->rImage4;
  stormShape=in->stormShape;
 }
 KEYWORDS({  
  TXTWORD("name",name)
  else TAGWORD("hasPrecipitation",hasPrecipitation)
  else TAGWORD("stormOnly",stormOnly)
  else NUMWORD("magnetoSeed",magnetoSeed)
  else TAGWORD("radioactive",radioactive)
  else NUMWORD("cloudShadowThickness",cloudShadowThickness)
  else TAGWORD("areaLightning",areaLightning)
  else TAGWORD("lightningBolts",lightningBolts)
  else NUMWORD("environment_seed",environment_seed)
  else NUMWORD("cloudColorSeed",cloudColorSeed)
  else NUMWORD("waterColorSeed",waterColorSeed)
  else NUMWORD("regionsSeed",regionsSeed)
  else NUMWORD("erosionTime",erosionTime)
  else NUMWORD("windTime",windTime)
  else NUMWORD("cloudFactor",cloudFactor)
  else NUMWORD("cloudFloor",cloudFloor)
  else NUMWORD("cloudCover",cloudCover)
  else NUMWORD("cloudThickness",cloudThickness)
  else NUMWORD("mountainFactor",mountainFactor)
  else NUMWORD("mountainHeight",mountainHeight)
  else NUMWORD("waterTime",waterTime)
  else NUMWORD("accumulation",accumulation)
  else NUMWORD("shimmerThickness",shimmerThickness)
  else NUMWORD("windSpeed",windSpeed)
  else NUMWORD("droughtIntensity",droughtIntensity)
  else NUMWORD("stormVolatility",stormVolatility)
  else NUMWORD("resizeFactor",resizeFactor)
  else NUMWORD("tideExtremes",tideExtremes)
  else NUMWORD("tideSpeed",tideSpeed)
  else NUMWORD("cloudHeight",cloudHeight)
  else NUMWORD("shimmerSpeed",shimmerSpeed)
  else NUMWORD("shimmerFactor",shimmerFactor)
  else NUMWORD("bloomMod",bloomMod)
  else NUMWORD("tectonics",tectonics)
  else NUMWORD("magnetics",magnetics)
  else NUMWORD("seaLevel",seaLevel)
  else SUBWORD("lamps",lamps)
  else TINTWORD("waterColor",waterColor)
  else TINTWORD("cTint1",cTint1)
  else TINTWORD("cTint2",cTint2)
  else TINTWORD("cTint3",cTint3)
  else TINTWORD("cTint4",cTint4)
  else TINTWORD("hTint1",hTint1)
  else TINTWORD("hTint2",hTint2)
  else TINTWORD("hTint3",hTint3)
  else TINTWORD("hTint4",hTint4)
  else TINTWORD("seaColor",seaColor)
  else TINTWORD("waterReflection",waterReflection)
  else TINTWORD("snowColor",snowColor)
  else TINTWORD("rThreshold1",rThreshold1)
  else TINTWORD("rThreshold2",rThreshold2)
  else TINTWORD("rThreshold3",rThreshold3)
  else TINTWORD("rThreshold4",rThreshold4)
  else TILEWORD("cImage1",cImage1)
  else TILEWORD("cImage2",cImage2)
  else TILEWORD("cImage3",cImage3)
  else TILEWORD("cImage4",cImage4)
  else TILEWORD("hImage1",hImage1)
  else TILEWORD("hImage2",hImage2)
  else TILEWORD("hImage3",hImage3)
  else TILEWORD("hImage4",hImage4)
  else TILEWORD("waterNoise",waterNoise)
  else TILEWORD("rImage1",rImage1)
  else TILEWORD("rImage2",rImage2)
  else TILEWORD("rImage3",rImage3)
  else TILEWORD("rImage4",rImage4)
  else TWEENWORD("stormShape",stormShape)
 })
 WORDKEYS({  
  WORDKEY("name",name)
  TAGKEY("hasPrecipitation",hasPrecipitation)
  TAGKEY("stormOnly",stormOnly)
  NUMBKEY("magnetoSeed",magnetoSeed)
  TAGKEY("radioactive",radioactive)
  NUMBKEY("cloudShadowThickness",cloudShadowThickness)
  TAGKEY("areaLightning",areaLightning)
  TAGKEY("lightningBolts",lightningBolts)
  NUMBKEY("environment_seed",environment_seed)
  NUMBKEY("cloudColorSeed",cloudColorSeed)
  NUMBKEY("waterColorSeed",waterColorSeed)
  NUMBKEY("regionsSeed",regionsSeed)
  NUMBKEY("erosionTime",erosionTime)
  NUMBKEY("windTime",windTime)
  NUMBKEY("cloudFactor",cloudFactor)
  NUMBKEY("cloudFloor",cloudFloor)
  NUMBKEY("cloudCover",cloudCover)
  NUMBKEY("cloudThickness",cloudThickness)
  NUMBKEY("mountainFactor",mountainFactor)
  NUMBKEY("mountainHeight",mountainHeight)
  NUMBKEY("waterTime",waterTime)
  NUMBKEY("accumulation",accumulation)
  NUMBKEY("shimmerThickness",shimmerThickness)
  NUMBKEY("windSpeed",windSpeed)
  NUMBKEY("droughtIntensity",droughtIntensity)
  NUMBKEY("stormVolatility",stormVolatility)
  NUMBKEY("resizeFactor",resizeFactor)
  NUMBKEY("tideExtremes",tideExtremes)
  NUMBKEY("tideSpeed",tideSpeed)
  NUMBKEY("cloudHeight",cloudHeight)
  NUMBKEY("shimmerSpeed",shimmerSpeed)
  NUMBKEY("shimmerFactor",shimmerFactor)
  NUMBKEY("bloomMod",bloomMod)
  NUMBKEY("tectonics",tectonics)
  NUMBKEY("magnetics",magnetics)
  NUMBKEY("seaLevel",seaLevel)
  SSUBKEY("lamps",lamps)
  TINTKEY("waterColor",waterColor)
  TINTKEY("cTint1",cTint1)
  TINTKEY("cTint2",cTint2)
  TINTKEY("cTint3",cTint3)
  TINTKEY("cTint4",cTint4)
  TINTKEY("hTint1",hTint1)
  TINTKEY("hTint2",hTint2)
  TINTKEY("hTint3",hTint3)
  TINTKEY("hTint4",hTint4)
  TINTKEY("seaColor",seaColor)
  TINTKEY("waterReflection",waterReflection)
  TINTKEY("snowColor",snowColor)
  TINTKEY("rThreshold1",rThreshold1)
  TINTKEY("rThreshold2",rThreshold2)
  TINTKEY("rThreshold3",rThreshold3)
  TINTKEY("rThreshold4",rThreshold4)
  IMGKEY("cImage1",cImage1)
  IMGKEY("cImage2",cImage2)
  IMGKEY("cImage3",cImage3)
  IMGKEY("cImage4",cImage4)
  IMGKEY("hImage1",hImage1)
  IMGKEY("hImage2",hImage2)
  IMGKEY("hImage3",hImage3)
  IMGKEY("hImage4",hImage4)
  IMGKEY("waterNoise",waterNoise)
  IMGKEY("rImage1",rImage1)
  IMGKEY("rImage2",rImage2)
  IMGKEY("rImage3",rImage3)
  IMGKEY("rImage4",rImage4)
  TWEENKEY("stormShape",stormShape)
 })
 CLONE(ClassicPlanetSurfaceProfile,{
  DUPE(hasPrecipitation)
  DUPE(stormOnly)
  DUPE(magnetoSeed)
  DUPE(radioactive)
  DUPE(cloudShadowThickness)
  DUPE(areaLightning)
  DUPE(lightningBolts)
  DUPE(environment_seed)
  DUPE(cloudColorSeed)
  DUPE(waterColorSeed)
  DUPE(regionsSeed)
  DUPE(erosionTime)
  DUPE(windTime)
  DUPE(cloudFactor)
  DUPE(cloudFloor)
  DUPE(cloudCover)
  DUPE(cloudThickness)
  DUPE(mountainFactor)
  DUPE(mountainHeight)
  DUPE(waterTime)
  DUPE(accumulation)
  DUPE(shimmerThickness)
  DUPE(windSpeed)
  DUPE(droughtIntensity)
  DUPE(stormVolatility)
  DUPE(resizeFactor)
  DUPE(tideExtremes)
  DUPE(tideSpeed)
  DUPE(cloudHeight)
  DUPE(shimmerSpeed)
  DUPE(shimmerFactor)
  DUPE(bloomMod)
  DUPE(tectonics)
  DUPE(magnetics)
  DUPE(seaLevel)
  a->lamps.Set(&lamps);
  DUPE(waterColor)
  DUPE(cTint1)
  DUPE(cTint2)
  DUPE(cTint3)
  DUPE(cTint4)
  DUPE(hTint1)
  DUPE(hTint2)
  DUPE(hTint3)
  DUPE(hTint4)
  DUPE(seaColor)
  DUPE(waterReflection)
  DUPE(snowColor)
  DUPE(rThreshold1)
  DUPE(rThreshold2)
  DUPE(rThreshold3)
  DUPE(rThreshold4)
  DUPE(cImage1)
  DUPE(cImage2)
  DUPE(cImage3)
  DUPE(cImage4)
  DUPE(hImage1)
  DUPE(hImage2)
  DUPE(hImage3)
  DUPE(hImage4)
  DUPE(waterNoise)
  DUPE(rImage1)
  DUPE(rImage2)
  DUPE(rImage3)
  DUPE(rImage4)
 })
MANY(ClassicPlanetSurfaceProfile,ClassicPlanetSurfaceProfileHandle,ClassicPlanetSurfaceProfileHandles,"classicplanetsurfaceprofile",ClassicPlanetSurfaceProfiles,{})
 KEYWORDSGroup("classicplanetsurfaceprofile",ClassicPlanetSurfaceProfile)
 WORDKEYSGroup("classicplanetsurfaceprofile",ClassicPlanetSurfaceProfile)
 CLONES(ClassicPlanetSurfaceProfile,ClassicPlanetSurfaceProfiles)
DONE(ClassicPlanetSurfaceProfile);
