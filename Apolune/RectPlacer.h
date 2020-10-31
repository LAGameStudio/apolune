#pragma once

#include "Cartesian.h"
#include "Numbers.h"
#include <vector>

class BinPacker {
public:
 BinPacker() { maxBins=10; }
    // The input and output are in terms of vectors of ints to avoid
    // dependencies (although I suppose a public member struct could have been
    // used). The parameters are:

    // rects : An array containing the width and height of each input rect in
    // sequence, i.e. [w0][h0][w1][h1][w2][h2]... The IDs for the rects are
    // derived from the order in which they appear in the array.

    // packs : After packing, the outer array contains the packs (therefore
    // the number of packs is packs.size()). Each inner array contains a
    // sequence of sets of 4 ints. Each set represents a rectangle in the
    // pack. The elements in the set are 1) the rect ID, 2) the x position
    // of the rect with respect to the pack, 3) the y position of the rect
    // with respect to the pack, and 4) whether the rect was rotated (1) or
    // not (0). The widths and heights of the rects are not included, as it's
    // assumed they are stored on the caller's side (they were after all the
    // input to the function).

    // allowRotation : when true (the default value), the packer is allowed
    // the option of rotating the rects in the process of trying to fit them
    // into the current working area.
    Zint maxBins;
    bool Pack(
        const std::vector<int>&          rects,
        std::vector< std::vector<int> >& packs,
        int                              packSizeW,
        int                              packSizeH,
        bool                             allowRotation = true
    );
private:
    struct Rect {
        Rect(int size) : x(0), y(0), w(size), h(size), ID(-1), rotated(false), packed(false) {
            children[0] = -1;
            children[1] = -1;
        }
        Rect(int sizew,int sizeh) : x(0), y(0), w(sizew), h(sizeh), ID(-1), rotated(false), packed(false) {
            children[0] = -1;
            children[1] = -1;
        }
        Rect(int x, int y, int w, int h, int ID = 1)
         : x(x), y(y), w(w), h(h), ID(ID), rotated(false), packed(false) {
            children[0] = -1;
            children[1] = -1;
        }
        int GetArea() const { return w * h; }
        void Rotate() {
            std::swap(w, h);
            rotated = !rotated;
        }
        bool operator<(const Rect& rect) const { return GetArea() < rect.GetArea(); }
        int  x;
        int  y;
        int  w;
        int  h;
        int  ID;
        int  children[2];
        bool rotated;
        bool packed;
    };
    void Clear();
    void Fill(int pack, bool allowRotation);
    void Split(int pack, int rect);
    bool Fits(Rect& rect1, const Rect& rect2, bool allowRotation);
    void AddPackToArray(int pack, std::vector<int>& array) const;
    bool RectIsValid(int i) const;
    bool PackIsValid(int i) const;
    int               m_packSizeW,m_packSizeH;
    int               m_numPacked;
    std::vector<Rect> m_rects;
    std::vector<Rect> m_packs;
    std::vector<int>  m_roots;
    Integers unpacked;
};

_ONE(PackedRect,{})
 PackedRect( int id, int x, int y, int w, int h, bool rotated ) : ListItem() { 
  r.SetRect(x,y,w,h);
  this->id=id;
  this->rotated=rotated;
 }
 Cartesian r;
 Zint id;
 Zbool rotated;
_MANY(PackedRect,PackedRects,{})
DONE(PackedRect);

ONE(PackedRectList,{})
 PackedRects rects;
MANY(PackedRectList,PackedRectListHandle,PackedRectListHandles,"PackedRectList",PackedRectLists,{})
DONE(PackedRectList);

class Proce55ors;
class RectPack {
public:
 BinPacker binPacker;
 Cartesians inputs;
 Zbool allowRotation;
 PackedRectLists outputs;
 Cartesian packSize; // Square/rect region to be packed.
 Cartesian growth,growLimit;
 Zint growthStepLimit;
 Zbool lastResult;
 RectPack() { packSize.Set(512,512); growLimit.Set(32768,32768); growthStepLimit=10000; }
 void Pack() {
  outputs.Clear();
  std::vector<int> rects;
  EACH(inputs.first,Cartesian,c) {
   rects.push_back(c->w);
   rects.push_back(c->h);
  }
  std::vector< std::vector<int> > packs;
  lastResult=binPacker.Pack(rects,packs,packSize.w,packSize.h,false);
  vector< vector<int> >::iterator row;
  for (row = packs.begin(); row != packs.end(); row++) {
   PackedRectList *outputting=new PackedRectList;
   outputs.Append(outputting);
   for (unsigned int col = 0; col < row->size(); col+=4 ) {
    int rect_id=row->at(col+0);    
    Cartesian *original=(Cartesian *)inputs.Element(rect_id);
    bool rotated=row->at(col+3) == 1;
    outputting->rects.Append(new PackedRect(rect_id,row->at(col+1),row->at(col+2),original->w,original->h,rotated));
   }
  }
 }
 Cartesian grownSize;
 bool Grow() {
  int retries=growthStepLimit;
  if ( growth.x <= 0 && growth.y <= 0 ) {
   OUTPUT("RectPack: Cannot grow with a growth rate of 0\n");
   return false;
  }
  Cartesian originalPackSize;
  originalPackSize.Set(&packSize);
  Pack();
  while ( (!lastResult || outputs.count > 1) && retries > 0 ) {
   packSize.w+=growth.x;
   packSize.h+=growth.y;
   Pack();
   retries--;
  }
  grownSize.Set(packSize.w,packSize.h);
  packSize.Set(&originalPackSize);
  if ( retries == 0 ) {
   OUTPUT("RectPack: hit limit configured of growth steps, most likely means one or more rectangles did not fit, try changing growth rate to grow both axis or expand the pack size to include dimensions large enough to handle the biggest rectangle.\n");
   return false;
  } else return true;
 }
 void Import( Proce55ors *list );
 void Export( Proce55ors *list );
 void Export( int offset_x, int offset_y, Proce55ors *list );
};

extern RectPack rectPack;