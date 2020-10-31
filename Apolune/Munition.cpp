#include "Munition.h"

Munitions munitionTypes;

void Munitions::AssignUnassigned( Campaign *c ) {
 FOREACH(Munition,m) {
  if ( !m->campaign ) m->campaign=c;
  m->fragment=munitionTypes.firstNamed(m->fragmentReference.c_str());
 }
}

void Munitions::Query( Campaign *c, MunitionHandles *out ) {
 FOREACH(Munition,q) if ( q->campaign == c ) out->Add(q);
}