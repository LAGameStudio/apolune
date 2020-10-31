#include "QMap.h"

QMaps qmaps;

void QMaps::AssignUnassigned( Campaign *c ) {
 FOREACH(QMap,m) if ( !m->campaign ) m->campaign=c;
}

void QMaps::Query( Campaign *c, QMapHandles *out ) {
 FOREACH(QMap,q) if ( q->campaign == c ) out->Add(q);
}