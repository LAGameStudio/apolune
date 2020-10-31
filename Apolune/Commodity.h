/*******************************************************************************************************************************\
| 88888888888          88                  LOST ASTRONAUT'S                 * Lost Astronaut Game Development Framework         |
| 88                   ""                                                   * Copyright (c) 2007-2017 H. Elwood Gilliland III   |
| 88aaaaa  8b,dPPYba,  88  8b,dPPYba,    ,adPPYb,d8   ,adPPYba,  ,adPPYba,  ****************************************************|
| 88UUUUU  88P'   "Y8  88  88P'   `"8a  a8"    `Y88  a8P_____88  I8[    ""  * This software is copyrighted software.            |
| 88       88          88  88       88  8b       88  8PP"""""""   `"Y8ba,   * Use of this code is given only with permission    |
| 88       88          88  88       88  "8a,   ,d88  "8b,   ,aa  aa    ]8I  * to parties who have been granted such permission  |
| 88       88          88  88       88   `"YbbdP"Y8   `"Ybbd8"'  `"YbbdP"'  * by its author, Herbert Elwood Gilliland III.      |
|    O  F      T H E     E M P I R E     aa,    ,88                         * All rights reserved.                              |
|_______________________________________  "Y8bbdP"   __________________________________________________________________________*/
#pragma once

#include "Strings.h"
#include "Numbers.h"
#include "Money.h"
#include "CSV.h"
#include "TextureLibrary.h"

class Campaign;
ONE(Commodity,{tint.x_White();})
 Zp<Campaign> campaign;
 Zp<GLImage> image;
 Crayon tint;
 Zstring catalog_name;
 Zdouble rarity,density;
 Zbool contraband;
 string GetReference() {
  return string("commodity/")+name;
 }
 BINARY_IN({
  B_IN_STRING(name)
  B_IN_IMAGE(image)
  B_IN_TINT(tint)
  B_IN_STRING(catalog_name)
  B_IN_NUM(rarity)
  B_IN_NUM(density)
  B_IN_BOOL(contraband)
 })
 BINARY_OUT({
  B_OUT_STRING(name)
  B_OUT_IMAGE(image)
  B_OUT_TINT(tint)
  B_OUT_STRING(catalog_name)
  B_OUT_NUM(rarity)
  B_OUT_NUM(density)
  B_OUT_BOOL(contraband)
 })
 KEYWORDS({
  TXTWORD("name",name)
  else TXTWORD("catalog_name",catalog_name)
  else KEYWORD("rarity",{rarity=w;})
  else KEYWORD("density",{density=w;})
  else KEYWORD("image",{image=library.Load(w);})
  else KEYWORD("tint",{tint.fromString(w);})
  else TAGWORD("contraband",contraband);
 })
 WORDKEYS({
  TXTKEY("name",name)
  TXTKEY("catalog_name",catalog_name)
  WORDKEY("rarity",rarity)
  WORDKEY("density",density)
  if ( image ) WORDKEY("image",image->filename.c_str())
  SSUBKEY("tint",tint)
  TAGKEY("contraband",contraband)
 })
 double Value() {
  return (rarity*1000.0);
 }
MANY(Commodity,CommodityHandle,CommodityHandles,"Commodity",Commodities,{})
 void AssignUnassigned( Campaign *c );
 void Query( Campaign *c, CommodityHandles *out );
 KEYWORDSGroup("Commodity",Commodity);
 WORDKEYSGroup("Commodity",Commodity);
 BINARY_INOUT(Commodity)
DONE(Commodities);

extern Commodities commodities;

ONE(CommodityRatio,{})
 Zp<Commodity> commodity; Zstring commodityString;
 Zdouble ratio;
 double Quantity( double mass ) {
  return ratio*mass;
 }
 KEYWORDS({
  KEYWORD("ratio",{ratio=w;})
  else KEYWORD("commodity",{commodityString=w;})
 })
 WORDKEYS({
  WORDKEY("ratio",ratio)
  WORDKEY("commodity",commodityString)
 })
MANY(CommodityRatio,CommodityRatioHandle,CommodityRatioHandles,"CommodityRatio",CommodityRatios,{})
 void Add( Commodity *c, double amount ) {
  CommodityRatio *cr=new CommodityRatio;
  cr->commodity=c;
  cr->commodityString=c->name;
  cr->ratio=amount;
  Append(cr);
 }
 void Adjust() {
  double total=TotalRatios();
  if ( total < 1.0 ) {
   double amount=(1.0-total) / (double)count;
   FOREACH(CommodityRatio,cr) if ( (cr->ratio+amount) < 1.0 ) cr->ratio += amount;
  } else if ( total > 1.0 ) {
   while ( TotalRatios() > 1.0 ) {
    FOREACH(CommodityRatio,cr) cr->ratio/=2.0;
   }
   total=TotalRatios();
   if ( total < 1.0 ) {
    double amount=(1.0-total) / (double)count;
    FOREACH(CommodityRatio,cr) if ( (cr->ratio+amount) < 1.0 ) cr->ratio += amount;
   }
  }
 }
 void Adjust( CommodityRatio *in ) {
  if (!in) return;
  if ( count == 1 ) {
   in->ratio=1.0;
   return;
  }
  CommodityRatio *most=Predominant();
  if ( in->ratio=0.0 ) {
   in->ratio=0.1;
   if ( most && most != in ) most->ratio-=0.1;
   FOREACH(CommodityRatio,cr) {
    if ( cr != in ) cr->ratio-=1.0/(double)count*0.1;
   }
  }
  double total=TotalRatios();
  if ( total < 1.0 ) {
   double amount=(1.0-total) / (double)count;
   FOREACH(CommodityRatio,cr) if ( cr != in && (cr->ratio+amount) < 1.0 ) cr->ratio += amount;
  } else if ( total > 1.0 ) {
   while ( TotalRatios() > 1.0 ) {
    FOREACH(CommodityRatio,cr) if ( cr != in ) cr->ratio/=2.0;
   }
   total=TotalRatios();
   if ( total < 1.0 ) {
    double amount=(1.0-total) / (double)count;
    FOREACH(CommodityRatio,cr) if ( cr != in && (cr->ratio+amount) < 1.0 ) cr->ratio += amount;
   }
  }
 }
 double TotalRatios() {
  double result=0.0;
  FOREACH(CommodityRatio,cr) result+=cr->ratio;
  return result;
 }
 CommodityRatio *Predominant() {
  if ( first == null ) return null;
  CommodityRatio *candidate=(CommodityRatio *) first;
  FOREACH(CommodityRatio,cr) if ( cr->ratio > candidate->ratio ) candidate=cr;
  return candidate;
 }
 CommodityRatio *Trace() {
  if ( first == null ) return null;
  CommodityRatio *candidate=(CommodityRatio *) first;
  FOREACH(CommodityRatio,cr) if ( cr->ratio < candidate->ratio ) candidate=cr;
  return candidate;
 }
 double BiggestRatio() {
  if ( !first ) return 1.0;
  double biggest=((CommodityRatio *) first)->commodity->density;
  FOREACH(CommodityRatio,cr) if ( cr->commodity->density > biggest ) biggest=cr->commodity->density;
  return biggest;
 }
 double TotalDensity( double factor=1.0 ) {
  double amount=0.0;
  FOREACH(CommodityRatio,cr) if ( cr->commodity ) amount+=cr->ratio * cr->commodity->density;
  return amount * factor;
 }
 double Value( double factor=1.0 ) {
  double totalDensity=TotalDensity(factor);
  double amount=0.0;
  FOREACH(CommodityRatio,cr) if ( cr->commodity ) amount+=( cr->ratio * cr->commodity->density / totalDensity ) * cr->commodity->Value();
  return factor * amount;
 }
 void Quantities( Doubles *out, double mass ) {
  FOREACH(CommodityRatio,cr) out->Add(cr->Quantity(mass));
 }
 void AssignUnassigned( Campaign *c );
 KEYWORDSGroup("CommodityRatio",CommodityRatio);
 WORDKEYSGroup("CommodityRatio",CommodityRatio);
DONE(CommodityRatio);