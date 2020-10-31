#include "Thing.h"
#include "Campaign.h"

Things things;

void Things::AssignUnassigned( Campaign *c ) {
 FOREACH(Thing,m) if ( !m->campaign ) m->campaign=c;
}

void Things::Query( Campaign *c, ThingHandles *out ) {
 FOREACH(Thing,q) if ( q->campaign == c ) out->Add(q);
}