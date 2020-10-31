#include "Commodity.h"

#include "Campaign.h"

Commodities commodities;

void Commodities::AssignUnassigned( Campaign *campaign ) {
 FOREACH(Commodity,c) if ( !c->campaign ) c->campaign=campaign;
}

void Commodities::Query( Campaign *campaign, CommodityHandles *out ) {
 FOREACH(Commodity,c) if ( c->campaign == campaign ) out->Add(c);
}

void CommodityRatios::AssignUnassigned( Campaign *c ) {
 FOREACH(CommodityRatio,cr) {
  cr->commodity=c->FindCommodity(cr->commodityString.c_str());
 }
}
