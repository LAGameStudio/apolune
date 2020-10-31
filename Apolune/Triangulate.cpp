#include "Triangulate.h"

Triangulate triangulate;

// TVector
bool operator==(const TVector &a, const TVector &b) {
 return a.point[0]==b.point[0] && a.point[1]==b.point[1] && a.point[2]==b.point[2];
}

// TTriangle
bool operator== ( const TTriangle &a, const TTriangle &b ) {
 return a.p1 == b.p1 && a.p2 == b.p2 && a.p3 == b.p3;   
}