#include "PlanetProfile.h"

#include "Campaign.h"

PlanetProfiles planetProfiles; 

void PlanetProfiles::AssignUnassigned( Campaign *c ) {
 FOREACH(PlanetProfile,m) if ( !m->campaign ) m->campaign=c;
}

void PlanetProfiles::Query( Campaign *c, PlanetProfileHandles *out ) {
 FOREACH(PlanetProfile,q) if ( q->campaign == c ) out->Add(q);
}