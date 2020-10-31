/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once

#include "macros.h"
#include "Randoms.h"

struct coord2i{
    int x;
    int y;
};

#define WALL '#'
#define FLOOR '.'
#define DOOR 'D'
#define GROUND ' '
#define UNDEFINED '?'

class BDungeon {
private:
 int seed;
 char * dungeon;
 int sizeX;
 int sizeY;
 int dungeonSize;
 int maxRooms;
 int currentRooms;
 int maxDoors,roomChance,maxRoomSize,maxIterations,hallLength,maxPossibleRooms;
 void initializeDungeon();
public:
 BDungeon(int seed, int w, int h, int maxDoors, int roomChance, int maxRoomSize, int maxIterations, int hallLength, int maxPossibleRooms );
 BDungeon(int seed, int maxDoors, int roomChance, int maxRoomSize, int maxIterations, int hallLength, int maxPossibleRooms );
 ~BDungeon();
 int getSizeX();
 int getSizeY();
 char getTile(int, int);
 void setTile(int, int, char);
 void printInfo();
 void addRoom(int, int, int, int);
 void generate();
};

#include "Cartesian.h"

/**
 * This class uses the cellular automata algorithm
 * to generate very sexy caves.
 * (Coded by Eddie Lee, October 16, 2010)
 * Given away without a license, but it is basically the same algorithim as Jim Babock's algorithm, based on a public domain algorithm
 */
class FlxCaveGenerator {
public:
 int seed;
 int numTilesCols;
 int numTilesRows;
 int numSmoothingIterations;
 double initWallRatio;
 matrix2d<char> map;
 FlxCaveGenerator( int seed ) {
  this->seed=seed;
  numTilesCols=10;
  numTilesRows=10;
  numSmoothingIterations=6;
  initWallRatio=0.5;
  map.resize(numTilesCols,numTilesRows);
  Generate();
 }
 FlxCaveGenerator( int seed, int w, int h, int smoothing, double wallRatio ) {
  numTilesCols=w;
  numTilesRows=h;
  numSmoothingIterations=smoothing;
  initWallRatio=wallRatio;
  map.resize(w,h);
  Generate();
 }
 void Generate() {
  int marcher=0;
  for ( int x=0; x<numTilesCols; x++ ) for ( int y=0; y<numTilesRows; y++ )
   map(x,y)=( (double)randoms.Unique(abs(seed+x+(marcher++)),abs(seed+y+(marcher++))) < initWallRatio ? WALL:FLOOR );
  for ( int i=0; i < numSmoothingIterations; ++i ) Smooth();
 }
	
 /**
  * @param	xPos	Column we are examining
  * @param	yPos	Row we are exampining
  * @param	dist	Radius of how far to check for neighbors
  * @return	Number of walls around the target, including itself
  */
 int countNumWallsNeighbors( int xPos, int yPos, int dist=1 ) {
  int count=0;
  for ( int y=-dist; y<=dist; y++ ) {
   for ( int x=-dist; x<=dist; x++ ) {
	if ( xPos + x < 0 || xPos + x > numTilesCols - 1
      || yPos + y < 0 || yPos + y > numTilesRows - 1 ) continue;
	if ( map(xPos + x,yPos + y) == WALL ) ++count;
   }
  }
  return count;
 }
	
 // Use the 4-5 rule to smooth cells
 void Smooth() {
  matrix2d<char> copy;
  copy.resize(numTilesCols,numTilesRows);
  for ( int x=0; x<numTilesCols; x++ ) for ( int y=0; y<numTilesRows; y++ ) {
   int numWalls=countNumWallsNeighbors( x, y, 1 );
   if ( numWalls >= 5 ) copy(x,y)=WALL;
   else copy(x,y)=FLOOR;
  }
  for ( int x=0; x<numTilesCols; x++ ) for ( int y=0; y<numTilesRows; y++ ) map(x,y)=copy(x,y);
 }
};


#define MIN_SIZE 5
class BSPDungeonRectangle : public ListItem {
public:
 int depth;
 int seed,marcher;
 int top, left, width, height;
 BSPDungeonRectangle *leftChild;
 BSPDungeonRectangle *rightChild;
 BSPDungeonRectangle *dungeon;
 BSPDungeonRectangle(int top, int left, int height, int width, int seed, int depth ) : ListItem() {
  this->top = top;
  this->left = left;
  this->width = width;
  this->height = height;
  this->seed=seed;
  this->marcher=seed+54321+depth;
  this->depth=depth;
  leftChild=rightChild=dungeon=null;
 }
 ~BSPDungeonRectangle() {
  if ( leftChild ) delete leftChild;
  if ( rightChild ) delete rightChild;
  if ( dungeon ) delete dungeon;
 }

 bool split() {
  if ( depth > 4 ) {
   return false;
  }
  if( leftChild ) //if already split, bail out
    return false;
  marcher++;
  bool horizontal = randoms.Query(seed,marcher);
  int max = (horizontal ? height : width ) - MIN_SIZE; //maximum height/width we can split off
  if( max <= MIN_SIZE ) // area too small to split, bail out
            return false;
  int split = randoms.Upto(max,seed,marcher); // generate split point 
  if( split < MIN_SIZE )  // adjust split point so there's at least MIN_SIZE in both partitions
      split = MIN_SIZE;
  if( horizontal ) { //populate child areas
   leftChild = new BSPDungeonRectangle( top, left, split, width, marcher+seed, depth+1 ); 
   rightChild = new BSPDungeonRectangle( top+split, left, height-split, width, seed+marcher+2, depth+1 );
  } else {
   leftChild = new BSPDungeonRectangle( top, left, height, split, seed+marcher+3, depth+1 );
   rightChild = new BSPDungeonRectangle( top, left+split, height, width-split, seed+marcher+4, depth+1 );
  }
  return true; //split successful
 }

 void Generate() {
  marcher+=randoms.Upto(100,seed,marcher+seed);
  if( leftChild != null ) { //if current are has child areas, propagate the call
    leftChild->Generate();
    rightChild->Generate();
  } else { // if leaf node, create a dungeon within the minimum size constraints
   int dungeonTop = (height - MIN_SIZE <= 0) ? 0 : randoms.Upto( height - MIN_SIZE, seed+marcher+10, marcher );
   int dungeonLeft =  (width - MIN_SIZE <= 0) ? 0 : randoms.Upto( width - MIN_SIZE, seed+marcher+11, marcher+seed );
   int dungeonHeight = UMAX(randoms.Upto( height - dungeonTop, seed+marcher+143, marcher ), MIN_SIZE );
   int dungeonWidth = UMAX(randoms.Upto( width - dungeonLeft, seed+marcher+123, seed ), MIN_SIZE );
   dungeon = new BSPDungeonRectangle( top + dungeonTop, left+dungeonLeft, dungeonHeight, dungeonWidth, seed+marcher, depth+1 );
  }
 }
};

class BSPDungeonRectangles : public LinkedList {
public:
 void Generate( int seed, int x, int y, int w, int h ) {
  int marcher=seed+123;
  int maxSplits=100;
  BSPDungeonRectangle *root = new BSPDungeonRectangle( x, y, w, h, seed, 0 ); //
  Append( root  ); //populate rectangle store with root area
  while( count < 19 && maxSplits > 0 ) { // this will give us 10 leaf areas
   maxSplits--;
   marcher++;
   int splitIdx = randoms.Upto( 100, count+seed, marcher )%count;
   BSPDungeonRectangle *toSplit=(BSPDungeonRectangle *) Element(splitIdx);
   if ( toSplit->split() ) { //attempt to split
    Append( toSplit->leftChild );
    Append( toSplit->rightChild );
   } 
  }
  root->Generate();
 }
 CLEARLISTRESET(BSPDungeonRectangle);
};
