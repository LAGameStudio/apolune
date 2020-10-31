#pragma once

#include "Crayon.h"
#include "ListItem.h"

class ColorHandle : public ListItem {
public:
 Crayon color;
};

class Palette : public LinkedList {
public:
 Palette(void) {}
 void Add( Crayon c ) {
  ColorHandle *ch=new ColorHandle;
  ch->color=c;
  Append(ch);
 }
 CLEARLIST(ColorHandle);
 ~Palette(void) {
  Clear();
 }
};

