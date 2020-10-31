#include "PackedString.h"

inline bool operator == (PackedString& a, PackedString& b) {
 return a.exactly(&b);
}

inline bool operator != (PackedString& a, PackedString& b) {
 return !a.exactly(&b);
}