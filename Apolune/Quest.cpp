#include "Quest.h"
#include "Campaign.h"

Quests quests;

void Quests::AssignUnassigned( Campaign *c ) {
 FOREACH(Quest,m) if ( !m->campaign ) m->campaign=c;
}

void Quests::Query( Campaign *c, QuestHandles *out ) {
 FOREACH(Quest,q) if ( q->campaign == c ) out->Add(q);
}