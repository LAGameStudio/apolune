#pragma once

#include "Cartesian.h"

namespace rbp {

/** @file GuillotineBinPack.h
	@author Jukka Jylänki

	@brief Implements different bin packer algorithms that use the GUILLOTINE data structure.

	This work is released to Public Domain, do whatever you want with it.
*/

#include <vector>
/// Performs a lexicographic compare on (rect short side, rect long side).
/// @return -1 if the smaller side of a is shorter than the smaller side of b, 1 if the other way around.
///   If they are equal, the larger side length is used as a tie-breaker.
///   If the rectangles are of same size, returns 0.
//int CompareRectShortSide(const Cartesian &a, const Cartesian &b);

/// Performs a lexicographic compare on (x, y, width, height).
//int NodeSortCmp(const Cartesian &a, const Cartesian &b);

/// Returns true if a is contained in b.
bool IsContainedIn(const Cartesian &a, const Cartesian &b);

class DisjointRectCollection
{
public:
	std::vector<Cartesian> rects;

	bool Add(const Cartesian &r)	{
		// Degenerate rectangles are ignored.
		if (r.w == 0 || r.h == 0)	return true;
		if (!Disjoint(r))	return false;
		rects.push_back(r);
		return true;
	}

	void Clear()	{
		rects.clear();
	}

	bool Disjoint(const Cartesian &r) const	{
		// Degenerate rectangles are ignored.
		if (r.w == 0 || r.h == 0)	return true;
		for(size_t i = 0; i < rects.size(); ++i)	if (!Disjoint(rects[i], r))	return false;
		return true;
	}

	static bool Disjoint(const Cartesian &a, const Cartesian &b)
	{
		if (a.x + a.w <= b.x ||	b.x + b.w <= a.x ||	a.y + a.h <= b.y ||	b.y + b.h <= a.y)	return true;
		return false;
	}
};

/** GuillotineBinPack implements different variants of bin packer algorithms that use the GUILLOTINE data structure
	to keep track of the free space of the bin where rectangles may be placed. */
class GuillotineBinPack
{
public:
	/// The initial bin size will be (0,0). Call Init to set the bin size.
	GuillotineBinPack();

	/// Initializes a new bin of the given size.
	GuillotineBinPack(int width, int height);

	/// (Re)initializes the packer to an empty bin of width x height units. Call whenever
	/// you need to restart with a new bin.
	void Init(int width, int height);

	/// Specifies the different choice heuristics that can be used when deciding which of the free subrectangles
	/// to place the to-be-packed rectangle into.
	enum FreeRectChoiceHeuristic
	{
		RectBestAreaFit, ///< -BAF
		RectBestShortSideFit, ///< -BSSF
		RectBestLongSideFit, ///< -BLSF
		RectWorstAreaFit, ///< -WAF
		RectWorstShortSideFit, ///< -WSSF
		RectWorstLongSideFit ///< -WLSF
	};

	/// Specifies the different choice heuristics that can be used when the packer needs to decide whether to
	/// subdivide the remaining free space in horizontal or vertical direction.
	enum GuillotineSplitHeuristic
	{
		SplitShorterLeftoverAxis, ///< -SLAS
		SplitLongerLeftoverAxis, ///< -LLAS
		SplitMinimizeArea, ///< -MINAS, Try to make a single big rectangle at the expense of making the other small.
		SplitMaximizeArea, ///< -MAXAS, Try to make both remaining rectangles as even-sized as possible.
		SplitShorterAxis, ///< -SAS
		SplitLongerAxis ///< -LAS
	};

	/// Inserts a single rectangle into the bin. The packer might rotate the rectangle, in which case the returned
	/// struct will have the width and height values swapped.
	/// @param merge If true, performs free Rectangle Merge procedure after packing the new rectangle. This procedure
	///		tries to defragment the list of disjoint free rectangles to improve packing performance, but also takes up 
	///		some extra time.
	/// @param rectChoice The free rectangle choice heuristic rule to use.
	/// @param splitMethod The free rectangle split heuristic rule to use.
	Cartesian Insert(int width, int height, bool merge, FreeRectChoiceHeuristic rectChoice, GuillotineSplitHeuristic splitMethod);

	/// Inserts a list of rectangles into the bin.
	/// @param rects The list of rectangles to add. This list will be destroyed in the packing process.
	/// @param merge If true, performs Rectangle Merge operations during the packing process.
	/// @param rectChoice The free rectangle choice heuristic rule to use.
	/// @param splitMethod The free rectangle split heuristic rule to use.
	void Insert(std::vector<Cartesian> &rects, bool merge, 
		FreeRectChoiceHeuristic rectChoice, GuillotineSplitHeuristic splitMethod);

// Implements GUILLOTINE-MAXFITTING, an experimental heuristic that's really cool but didn't quite work in practice.
//	void InsertMaxFitting(std::vector<RectSize> &rects, std::vector<Rect> &dst, bool merge, 
//		FreeRectChoiceHeuristic rectChoice, GuillotineSplitHeuristic splitMethod);

	/// Computes the ratio of used/total surface area. 0.00 means no space is yet used, 1.00 means the whole bin is used.
	float Occupancy() const;

	/// Returns the internal list of disjoint rectangles that track the free area of the bin. You may alter this vector
	/// any way desired, as long as the end result still is a list of disjoint rectangles.
	std::vector<Cartesian> &GetFreeRectangles() { return freeRectangles; }

	/// Returns the list of packed rectangles. You may alter this vector at will, for example, you can move a Rect from
	/// this list to the Free Rectangles list to free up space on-the-fly, but notice that this causes fragmentation.
	std::vector<Cartesian> &GetUsedRectangles() { return usedRectangles; }

	/// Performs a Rectangle Merge operation. This procedure looks for adjacent free rectangles and merges them if they
	/// can be represented with a single rectangle. Takes up Theta(|freeRectangles|^2) time.
	void MergeFreeList();

private:
	int binWidth;
	int binHeight;

	/// Stores a list of all the rectangles that we have packed so far. This is used only to compute the Occupancy ratio,
	/// so if you want to have the packer consume less memory, this can be removed.
	std::vector<Cartesian> usedRectangles;

	/// Stores a list of rectangles that represents the free area of the bin. This rectangles in this list are disjoint.
	std::vector<Cartesian> freeRectangles;

#ifdef _DEBUG
	/// Used to track that the packer produces proper packings.
	DisjointRectCollection disjointRects;
#endif

	/// Goes through the list of free rectangles and finds the best one to place a rectangle of given size into.
	/// Running time is Theta(|freeRectangles|).
	/// @param nodeIndex [out] The index of the free rectangle in the freeRectangles array into which the new
	///		rect was placed.
	/// @return A Rect structure that represents the placement of the new rect into the best free rectangle.
	Cartesian FindPositionForNewNode(int width, int height, FreeRectChoiceHeuristic rectChoice, int *nodeIndex);

	static int ScoreByHeuristic(int width, int height, const Cartesian &freeRect, FreeRectChoiceHeuristic rectChoice);
	// The following functions compute (penalty) score values if a rect of the given size was placed into the 
	// given free rectangle. In these score values, smaller is better.

	static int ScoreBestAreaFit(int width, int height, const Cartesian &freeRect);
	static int ScoreBestShortSideFit(int width, int height, const Cartesian &freeRect);
	static int ScoreBestLongSideFit(int width, int height, const Cartesian &freeRect);

	static int ScoreWorstAreaFit(int width, int height, const Cartesian &freeRect);
	static int ScoreWorstShortSideFit(int width, int height, const Cartesian &freeRect);
	static int ScoreWorstLongSideFit(int width, int height, const Cartesian &freeRect);

	/// Splits the given L-shaped free rectangle into two new free rectangles after placedRect has been placed into it.
	/// Determines the split axis by using the given heuristic.
	void SplitFreeRectByHeuristic(const Cartesian &freeRect, const Cartesian &placedRect, GuillotineSplitHeuristic method);

	/// Splits the given L-shaped free rectangle into two new free rectangles along the given fixed split axis.
	void SplitFreeRectAlongAxis(const Cartesian &freeRect, const Cartesian &placedRect, bool splitHorizontal);
};

}



/*
Copyright (c) 2013, christopher stones < chris.stones@zoho.com >
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



/**
 * BinPack2D is a 2 dimensional, multi-bin, bin-packer. ( Texture Atlas Array! )
 * It supports an arbitrary number of bins, at arbitrary sizes.
 * rectangles can be added one at a time, chunks at a time, or all at once.
 * rectangles that dont fit are reported back.
 * Data can be associated to rectangles before processing via a template, and recalled after processing.
 * 
 * There is no documentation, See ExampleProgram() below for a taste.
 * 
 * Instead of tracking 'free rectangles' like other solutions I've found online,
 * this algorithm tracks free 'top lefts', keeps them sorted by closest to origin, and puts new rectangles into
 * the first free top left that doesnt collide. Consuming a top left creates 2 new top lefts (x+w,y) and (x,y+h).
 * If a rectangle doesnt fit into a bin, before condisering the next bin, the current bin is re-tried with the rectangle rotated.
 * This SOMTIMES helps... but not always.. i might disable this in future !?
 * 
 * This Header was origonally part of my rh_texture_packer program.
 * A program I wrote to take advantage of my nexus-7's GL_EXT_texture_array extension.
 * I wanted to be able to render out whole scenes with a single glDraw* 
 * blah blah blah...
 */


/** ***** EXAMPLE CODE **************************************
 
 // Your data - whatever you want to associate with 'rectangle'
 class MyContent {
  public:
  std::string str;
  MyContent() : str("default string") {}
  MyContent(const std::string &str) : str(str) {}
  };
 
 int ExampleProgram() {
   
  srandom(0x69);
    
  // Create some 'content' to work on.
  BinPack2D::ContentAccumulator<MyContent> inputContent;
  
  for(int i=0;i<20;i++) {
   
    // random size for this content
    int width  = ((random() % 32)+1) * ((random() % 10)+1);
    int height = ((random() % 32)+1) * ((random() % 10)+1);
    
    // whatever data you want to associate with this content
    std::stringstream ss;
    ss << "box " << i;
    MyContent mycontent( ss.str().c_str() );
    
    // Add it
    inputContent += BinPack2D::Content<MyContent>(mycontent, BinPack2D::Coord(), BinPack2D::Size(width, height), false );
  }
  
  // Sort the input content by size... usually packs better.
  inputContent.Sort();
  
  // Create some bins! ( 2 bins, 128x128 in this example )
  BinPack2D::CanvasArray<MyContent> canvasArray = 
    BinPack2D::UniformCanvasArrayBuilder<MyContent>(128,128,2).Build();
    
  // A place to store content that didnt fit into the canvas array.
  BinPack2D::ContentAccumulator<MyContent> remainder;
  
  // try to pack content into the bins.
  bool success = canvasArray.Place( inputContent, remainder );
  
  // A place to store packed content.
  BinPack2D::ContentAccumulator<MyContent> outputContent;
  
  // Read all placed content.
  canvasArray.CollectContent( outputContent );
  
  // parse output.
  typedef BinPack2D::Content<MyContent>::Vector::iterator binpack2d_iterator;
  printf("PLACED:\n");
  for( binpack2d_iterator itor = outputContent.Get().begin(); itor != outputContent.Get().end(); itor++ ) {
   
    const BinPack2D::Content<MyContent> &content = *itor;
    
    // retreive your data.
    const MyContent &myContent = content.content;
  
    printf("\t%9s of size %3dx%3d at position %3d,%3d,%2d rotated=%s\n",
	   myContent.str.c_str(), 
	   content.size.w, 
	   content.size.h, 
	   content.coord.x, 
	   content.coord.y, 
	   content.coord.z, 
	   (content.rotated ? "yes":" no"));
  }
  
  printf("NOT PLACED:\n");
  for( binpack2d_iterator itor = remainder.Get().begin(); itor != remainder.Get().end(); itor++ ) {
   
    const BinPack2D::Content<MyContent> &content = *itor;
    
    const MyContent &myContent = content.content;
  
    printf("\t%9s of size %3dx%3d\n",
	   myContent.str.c_str(), 
	   content.size.w, 
	   content.size.h);
  }
  
  exit(0);
}
*/

#include<vector>
#include<map>
#include<list>
#include<algorithm>
#include<math.h>
#include<sstream>

namespace BinPack2D {

class Size {  
public:  
  /*const*/ int w;
  /*const*/ int h;  
  Size(int w, int h) : w(w), h(h) {}
  bool operator < ( const Size &that ) const {
   if(this->w != that.w) return this->w < that.w;
   if(this->h != that.h) return this->h < that.h;
  }
};

class Coord { 
public:  
 typedef std::vector<Coord> Vector;
 typedef std::list<Coord>   List;
 /*const*/ int x;
 /*const*/ int y;
 /*const*/ int z;
 Coord() : x(0), y(0), z(0) {}
 Coord(int x, int y) : x(x), y(y), z(0) {}
 Coord(int x, int y, int z) : x(x), y(y), z(z) {}
 bool operator < ( const Coord &that ) const {    
  if(this->x != that.x) return this->x < that.x;
  if(this->y != that.y) return this->y < that.y;
  if(this->z != that.z) return this->z < that.z;
 }
};

template<typename _T> class Content {  
public: 
 typedef std::vector<Content<_T> > Vector; 
 /*const*/ bool rotated;
 /*const*/ Coord coord;
 /*const*/ Size  size;
 /*const*/ _T content;
 Content( const Content<_T> &src ) : rotated(src.rotated), coord(src.coord), size(src.size), content(src.content) {} 
 Content( const _T &content, const Coord &coord, const Size &size, bool rotated ) : content(content), coord(coord), size(size), rotated(rotated) {}
 void Rotate() {  
  rotated = !rotated;
  size = Size( size.h, size.w );
 }
 bool intersects(const Content<_T> &that) const {
  if(this->coord.x >= (that.coord.x + that.size.w)) return false;
  if(this->coord.y >= (that.coord.y + that.size.h)) return false;
  if(that.coord.x >= (this->coord.x + this->size.w)) return false;
  if(that.coord.y >= (this->coord.y + this->size.h)) return false;
  return true;
 }
};

template<typename _T> class Canvas {  
 Coord::List topLefts;
 typename Content<_T>::Vector contentVector;  
 bool needToSort;  
public:  
 typedef Canvas<_T> CanvasT;
 typedef typename std::vector<CanvasT> Vector;  
 static bool Place( Vector &canvasVector, const typename Content<_T>::Vector &contentVector, typename Content<_T>::Vector &remainder ) {    
  typename Content<_T>::Vector todo = contentVector;        
  for( typename Vector::iterator itor = canvasVector.begin(); itor != canvasVector.end(); itor++ ) {    
   Canvas <_T> &canvas = *itor;
   remainder.clear();
   canvas.Place(todo, remainder);
   todo = remainder;
  }
  if(remainder.size()==0) return true;
  return false;
 }
 static bool Place( Vector &canvasVector, const typename Content<_T>::Vector &contentVector ) {
  typename Content<_T>::Vector remainder;
  return Place( canvasVector, contentVector, remainder );
 }
 static bool Place( Vector &canvasVector, const Content<_T> &content ) {
  typename Content<_T>::Vector contentVector(1, content);
  return Place( canvasVector, contentVector );
 }
 const int w;
 const int h;
 Canvas(int w, int h) : needToSort(false), w(w), h(h) { topLefts.push_back( Coord(0,0) ); }
 bool HasContent() const { return ( contentVector.size() > 0); }
 const typename Content<_T>::Vector &GetContents( ) const { return contentVector; }
 bool operator < ( const Canvas &that ) const {
  if(this->w != that.w) return this->w < that.w;
  if(this->h != that.h) return this->h < that.h;
 }
 bool Place(const typename Content<_T>::Vector &contentVector, typename Content<_T>::Vector &remainder, bool experiements_ok=false ) {
  bool placedAll = true;
  for( typename Content<_T>::Vector::const_iterator itor = contentVector.begin(); itor != contentVector.end(); itor++ ) {
   const Content<_T> & content = *itor;
   if( Place( content, experiments_ok ) == false ) {
   	placedAll = false;
   	remainder.push_back( content );
   }
  }
  return placedAll;
 }
  
 bool Place(Content<_T> content, bool experiments_ok=false) {
  Sort();
  for( Coord::List::iterator itor = topLefts.begin(); itor != topLefts.end(); itor++ ) {
   content.coord = *itor;
   if( Fits( content ) ) {
   	Use( content );
   	topLefts.erase( itor );
   	return true;
   }
  }
  if ( experiments_ok ) {
   // EXPERIMENTAL - TRY ROTATED?
   content.Rotate();
   for( Coord::List::iterator itor = topLefts.begin(); itor != topLefts.end(); itor++ ) {      
    content.coord = *itor;
    if( Fits( content ) ) {	
    	Use( content );
    	topLefts.erase( itor );
    	return true;
    }
   }
   ////////////////////////////////
  }
  return false;
 }  
private:
 bool Fits( const Content<_T> &content ) const {
  if( (content.coord.x + content.size.w) > w ) return false;
  if( (content.coord.y + content.size.h) > h ) return false;
  for( typename Content<_T>::Vector::const_iterator itor = contentVector.begin(); itor != contentVector.end(); itor++ ) if( content.intersects( *itor ) )	return false;
  return true;
 }
 bool Use(const Content<_T> &content) {
  const Size  &size = content.size;
  const Coord &coord = content.coord;
  topLefts.push_front	( Coord( coord.x + size.w, coord.y          ) );
  topLefts.push_back	( Coord( coord.x         , coord.y + size.h ) );
  contentVector.push_back( content );
  needToSort = true;
  return true;
 }
private:
 struct TopToBottomLeftToRightSort {
  bool operator()(const Coord &a, const Coord &b) const { return ( a.x * a.x + a.y * a.y ) < ( b.x * b.x + b.y * b.y ); }
 }; 
public:
 void Sort() {
  if(!needToSort) return;
  topLefts.sort(TopToBottomLeftToRightSort());
  needToSort = false;
 }
};

template <typename _T> class ContentAccumulator {
 typename Content<_T>::Vector contentVector;
public:
 ContentAccumulator() {}
 const typename Content<_T>::Vector &Get() const {
  return contentVector;
 }
 typename Content<_T>::Vector &Get() {
  return contentVector;
 }
 ContentAccumulator<_T>& operator += ( const Content<_T> & content ) {
  contentVector.push_back( content );
  return *this;
 }
 ContentAccumulator<_T>& operator += ( const typename Content<_T>::Vector & content ) {
  contentVector.insert( contentVector.end(), content.begin(), content.end() );
  return *this;
 }
 ContentAccumulator<_T> operator + ( const Content<_T> & content ) {
  ContentAccumulator<_T> temp = *this;
  temp += content;
  return temp;
 }
 ContentAccumulator<_T> operator + ( const typename Content<_T>::Vector & content ) {
  ContentAccumulator<_T> temp = *this;
  temp += content;
  return temp;
 }
private:
 struct GreatestWidthThenGreatestHeightSort {
  bool operator()(const Content<_T> &a, const Content<_T> &b) const {
   const Size &sa = a.size; 
   const Size &sb = b.size;
//      return( sa.w * sa.h > sb.w * sb.h );
   if(sa.w != sb.w) return sa.w > sb.w;
   return sa.h > sb.h;
  }
 };
 struct MakeHorizontal {
  Content<_T> operator()( const Content<_T> &elem) {
   if(elem.size.h > elem.size.w) {
   	Content<_T> r = elem;
   	r.size.w = elem.size.h;
   	r.size.h = elem.size.w;
   	r.rotated = !elem.rotated;
   	return r;
   }
   return elem;
  }
 };
public:
 void Sort() {
//  if(allow_rotation) std::transform(contentVector.begin(), contentVector.end(), contentVector.begin(), MakeHorizontal());
  std::sort( contentVector.begin(), contentVector.end(), GreatestWidthThenGreatestHeightSort() );
 }
};

template <typename _T> class UniformCanvasArrayBuilder {  
 int w;
 int h;
 int d;
public:
 UniformCanvasArrayBuilder( int w, int h, int d ) : w(w), h(h), d(d) {}
 typename Canvas<_T>::Vector Build() {   
  return typename Canvas<_T>::Vector(d, Canvas<_T>(w, h) );
 }  
};

template<typename _T> class CanvasArray {  
  typename Canvas<_T>::Vector canvasArray;
public:  
 CanvasArray( const typename Canvas<_T>::Vector &canvasArray ) : canvasArray( canvasArray ) {}
 bool Place(const typename Content<_T>::Vector &contentVector, typename Content<_T>::Vector &remainder, bool experiments_ok=false) {
  return Canvas<_T>::Place( canvasArray, contentVector, remainder, experiments_ok );
 }
 bool Place(const ContentAccumulator<_T> &content, ContentAccumulator<_T> &remainder, bool experiments_ok=false) {
  return Place( content.Get(), remainder.Get(), experiments_ok );
 }
 bool Place(const typename Content<_T>::Vector &contentVector, bool experiments_ok=false) {
  return Canvas<_T>::Place( canvasArray, contentVector, experiments_ok );
 }
 bool Place(const ContentAccumulator<_T> &content) {
  return Place( content.Get() );
 }
 bool CollectContent( typename Content<_T>::Vector &contentVector ) const {
  int z = 0;
  for( typename Canvas<_T>::Vector::const_iterator itor = canvasArray.begin(); itor != canvasArray.end(); itor++ ) {
   const typename Content<_T>::Vector &contents = itor->GetContents();
   for( typename Content<_T>::Vector::const_iterator itor = contents.begin(); itor != contents.end(); itor++ ) {
	   Content<_T> content = *itor;
	   content.coord.z = z;
	   contentVector.push_back( content );
   }
   z++;
  }
  return true;
 }
 
 bool CollectContent( ContentAccumulator<_T> &content) const {
  return CollectContent( content.Get() );
 }
};

} /*** BinPack2D ***/