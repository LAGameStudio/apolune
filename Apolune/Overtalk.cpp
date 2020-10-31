#include "Overtalk.h"

#include "Campaign.h"

Overtalks overtalks;

void Overtalks::AssignUnassigned( Campaign *c ) {
 FOREACH(Overtalk,m) if ( !m->campaign ) m->campaign=c;
}

void Overtalks::Query( Campaign *c, OvertalkHandles *out ) {
 FOREACH(Overtalk,q) if ( q->campaign == c ) out->Add(q);
}