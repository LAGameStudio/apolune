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
#include "Audio.h"
#include "GLWindow.h"
#include "UglyFont.h"
#include "BitBrush.h"
#include "AStar.h"
#include "BDungeon.h"
#include "RLDungeon.h"

extern Interface input;

Cartesians *ErraticGrid( int gx, int gy, int w, int h );

class SimpleMaze {
public:
 int seed;
 int w,h;
 matrix2d<int> maze;
 SimpleMaze( int w, int h, int seed ) {
  this->seed=seed;
  this->w=w;
  this->h=h;
  Generate();
 }
 SimpleMaze( int w, int h ) {
  seed=0;
  this->w=w;
  this->h=h;
  Generate();
 }
 SimpleMaze() {
  seed=1;
  w=49;
  h=49;
  Generate();
 }
 int g_intDepth,marcher;
 void Generate() {
  maze.resize(w,h);
  for ( int x=0; x<w; x++ ) for ( int y=0; y<h; y++ ) maze(x,y)=0;
  g_intDepth=0;
  marcher=0;
  DigMaze(1,1);
//  Maze[1*W+1] = 2;
//  Maze[47*W+48] = 1;
 }
private:
 void DigMaze(int x, int y) {
  Random *set=randoms.GetSet(x+y+g_intDepth+seed);
  g_intDepth = g_intDepth + 1;
  maze(x,y)=1;
  while ( ValidCount(x,y) ) {
   switch ( (int)(set->unique(x+y+g_intDepth+seed+marcher,marcher+seed,x+y+marcher)*100.0f)%4 ) {
    case 0: if (ValidMove(x,y-2)) { maze(x,y-1) = 1; DigMaze(x,y-2); } break;
    case 1: if (ValidMove(x+2,y)) { maze(x+1,y) = 1; DigMaze(x+2,y); } break;
    case 2: if (ValidMove(x,y+2)) { maze(x,y+1) = 1; DigMaze(x,y+2); } break;
    case 3: if (ValidMove(x-2,y)) { maze(x-1,y) = 1; DigMaze(x-2,y); } break;
   } // end switch
  } // end while
  g_intDepth = g_intDepth - 1;
 } // end DigMaze()

 bool ValidMove(int x, int y) {
  marcher++;
  if (x>=0 && x<w && y>=0 && y<h && maze(x,y) == 0) return true;
  return false;
 }

 bool ValidCount(int x, int y) {
  return (
     ValidMove(x,y-2)
  || ValidMove(x+2,y)
  || ValidMove(x,y+2)
  || ValidMove(x-2,y)
  );
 }
};

enum Cardinal {
 d_north,
 d_south,
 d_east,
 d_west
};

class CardinalDirections;
class CardinalDirection : public ListItem {
public:
 CardinalDirection( string n, int x, int y ) : ListItem() {
  name=n;
  vector.Set(x,y);
 }
 CardinalDirections *directions;
 CardinalDirection *left,*right,*reverse;
 string name;
 Cartesian vector;
 bool stationary() {
  return ( vector.x==0 && vector.y==0 );
 }
};

extern CardinalDirection *north;
extern CardinalDirection *south;
extern CardinalDirection *east;
extern CardinalDirection *west;

class CardinalDirections : public LinkedList {
public:
 void Append(CardinalDirection *d) {
  _Append(d);
  d->directions=this;
 }
 CardinalDirection *Left( CardinalDirection *d ) {
  if ( contains(d->name.c_str(),"n") ) return find("w");
  else if ( contains(d->name.c_str(),"e") ) return find("n");
  else if ( contains(d->name.c_str(),"s") ) return find("e");
  else return find("s");
 }
 CardinalDirection *Right( CardinalDirection *d ) {
  if ( contains(d->name.c_str(),"n") ) return find("e");
  else if ( contains(d->name.c_str(),"e") ) return find("s");
  else if ( contains(d->name.c_str(),"s") ) return find("w");
  else return find("n");
 }
 CardinalDirection *Reverse( CardinalDirection *d ) {
  if ( contains(d->name.c_str(),"n" ) ) return find("s");
  else if ( contains(d->name.c_str(),"s" ) ) return find("n");
  else if ( contains(d->name.c_str(),"w" ) ) return find("e");
  else return find("w");
 }
 CardinalDirections() : LinkedList() {
  Append(new CardinalDirection("here", 0, 0));
  Append(north=new CardinalDirection("n",    0,-1)); // up
  Append(south=new CardinalDirection("s",    0, 1)); // down
  Append(east=new CardinalDirection("e",    1, 0));  // right
  Append(west=new CardinalDirection("w",   -1, 0));  // left
//  Append(new CardinalDirection("ne",   1,-1));  Not included, factored or implemented (all dungeons are orthogonal).
//  Append(new CardinalDirection("sw",  -1, 1));
//  Append(new CardinalDirection("se",   1, 1));
//  Append(new CardinalDirection("nw",  -1,-1));
  north->reverse=south;
  south->reverse=north;
  east->reverse=west;
  west->reverse=east;
  north->left=west;
  south->left=east;
  east->left=north;
  west->left=south;
  north->right=east;
  south->right=west;
  east->right=south;
  west->right=north;
 }
 CardinalDirection *find( string x ) {
  FOREACH(CardinalDirection,d) if ( !str_cmp(d->name.c_str(), x.c_str()) ) return d;
  return null;
 }
 CardinalDirection *find( int x, int y ) {
  FOREACH(CardinalDirection,d) if ( d->vector.x==x && d->vector.y==y ) return d;
  return null;
 }
 CardinalDirection *findReverse( CardinalDirection *heading ) {
  if ( heading->vector.x == 0 && heading->vector.y == 0 ) return heading;
  FOREACH(CardinalDirection,d) if ( ( (d->vector.x == 0 && heading->vector.x == 0) || d->vector.x==-heading->vector.x )
                                 && ( (d->vector.y == 0 && heading->vector.y == 0) || d->vector.y==-heading->vector.y ) ) return d;
  return heading;
 }
 CardinalDirection *pick( int seed ) {
  return (CardinalDirection *) Element(1+seed%(count-1));
 }
 CLEARLISTRESET(CardinalDirection);
};

extern CardinalDirections cardinals;


class DungeonSquare;
class DungeonLevel;

class ALifeHandle;
class ALifeHandles : public LinkedList {
public:
 void Clear();
};

class ALives;
class ALife : public ListItem {
public:
 ALives *alives;
 Random *set;
 int seed;
 bool die;
 ALifeHandles nearby;
 DungeonLevel *level;
 CardinalDirection *heading,*oldHeading;
 Cartesian position;
 ALife();
 ALife( int seed, DungeonLevel *dungeon );
 void Constructor() {
  set=null;
  seed=0;
  lifespan=0;
  age=0;
  heading=oldHeading=null;
  die=false;
 }
 void ChangeDirection() {
  oldHeading=heading;
  int i=0;
  while ( heading == oldHeading ) heading=cardinals.pick(seed+age+(i++));
 }
 bool TurnLeftOrRight( bool reversed=false );
 DungeonSquare *MovingTo( CardinalDirection *heading, int steps=1 );
 DungeonSquare *MovingTo( int steps );
 int NonTraversibleNeighbors( int dist );
 bool Move( CardinalDirection *s );
 bool Move( CardinalDirection *s, int dist );
 int lifespan,age;          // The lifespan in generations and age incidence.
 virtual bool Dead() {
  return die || age >= lifespan;
 }
 virtual void Birth() {}       // "Formation"
 virtual void Generation() {}  // AI
 virtual void Legacy() {}      // Legacy (offspring creation)
 virtual void Death() {}
};

class ALifeHandle : public Handle<ALife> {
public:
 ALifeHandle( ALife *life ) : Handle() {
  p=life;
 }
};

class ALives : public LinkedList {
public:
 CLEARLISTRESET(ALife);
 void Append( ALife *a ) {
  _Append(a);
  a->Birth();
  a->alives=this;
 }
 void Generation() {
  EACHN(first,ALife,a,{
   a->age++;
   a->Generation();
   if ( a->Dead() ) {
    a->Death();
    Remove(a);
    delete a;
   }
  });
 }
};

enum DungeonDoorTypes {
 dd_doorway=0,        // no real open/close
 dd_locked=1,         // locked by a key
 dd_remote_control=2  // opened by button or lever or access code panel
};

enum DungeonSpecialTypes {
 dd_none=0,
 dd_column=1,
 dd_pit=2,
 dd_stair=3
};

struct DoorInfo {
public:
 int id;
 bool door,hidden;
 DungeonDoorTypes type;
 DoorInfo() {
  id=0;
  door=false;
  hidden=false;
  type=dd_doorway;
 }
};

enum DungeonStairTypes {
 dd_plane=-1,
 dd_down=0,
 dd_up=1,
 dd_chute=2
};

enum DungeonSquareTypes {
 dd_room=0,
 dd_hall=1,
 dd_anteroom=2
};

class DungeonMap;
class MapSectionInfoHandles;

ONE(MapSectionInfo,{})
 Zbool room;
 Crayon color;
 Cartesians walls, spaces, squares, doors, deadends, edges, corners;
 Cartesian extent;
 void Shuffle( int seed ) {
  walls.RandomizeOrder( seed );
  spaces.RandomizeOrder( seed );
  squares.RandomizeOrder( seed );
  doors.RandomizeOrder( seed );
  deadends.RandomizeOrder( seed );
  edges.RandomizeOrder( seed );
  corners.RandomizeOrder( seed );
 }
 void Calculate( DungeonMap *map );
MANY(MapSectionInfo,MapSectionInfoHandle,VanillaMapSectionInfoHandles,"",MapSectionInfos,{color=1;})
 Zint color;
 void InitAppend( MapSectionInfo *i ) {
  color++;
  i->color.Int(i->doors.count*64%255,(255-color*30%255),127+(64-color*30%64),255);
  Append(i);
 }
 SORTING(MapSectionInfo,SortSpaces,{},{ if ( A->spaces.count > B->spaces.count ) result=-1; },{});
 void Calculate( int seed, DungeonMap *map ) {
  SortSpaces();
  FOREACH(MapSectionInfo,m) {
   m->Calculate(map);
   m->Shuffle(seed);
  }
 }
DONE(MapSectionInfo);

class MapSectionInfoHandles : public LinkedList {
public:
 bool contains( MapSectionInfo *i ) { FOREACH(MapSectionInfoHandle,m) if ( m->p==i ) return true; return false; }
 void Add( MapSectionInfo *i ) {
  MapSectionInfoHandle *h=new MapSectionInfoHandle;
  h->p=i;
  Append(h);
 }
 void AddIfUnique( MapSectionInfo *i ) {
  if ( contains(i) ) return;
  MapSectionInfoHandle *h=new MapSectionInfoHandle;
  h->p=i;
  Append(h);
 }
 CLEARLISTRESET(MapSectionInfoHandle);
};

class DungeonMap;
class DungeonSquare;

/* Identified map 3x3 graph search patterns: (X=wall, 0=space, ?=not tested)
 *
 * Horizontal  Vertical  Horizontal  Vertical    Enclosed Pillar  LCorner  Hallway    LCorner   LCorner  Partial C  C-Shaped  Surrounded
 * Hall        Hall      StraightWay StraightWay                           LCorner    Filled    Unfilled                     
 * X X X       X 0 X     0 0 0       0 X 0       X X X    0 0 0   ? X ?    X X X      X X X     X 0 0    X 0 0       X 0 X    X X X
 * 0 0 0       X 0 X     X X X       0 X 0       X 0 X    0 X 0   0 X X    X 0 0      X X X     0 0 0    X 0 X       X 0 X    X X X
 * X X X       X 0 X     0 0 0       0 X 0       X X X    0 0 0   0 0 ?    X 0 X      X X 0     0 0 0    X X X       X X X    X X X
 *
 * UpWall      LeftWall  RightWall   DownWall    NearUpWall  NearDownWall  NearLeftWall  NearRightWall  T-Hall  Open
 * X X X       X X 0     0 X X       0 0 0       X X X       ? ? ?         X 0 ?         ? 0 X          X 0 X   0 0 0
 * X X X       X X 0     0 X X       X X X       0 0 0       0 0 0         X 0 ?         ? 0 X          0 0 0   0 ? 0
 * 0 0 0       X X 0     0 X X       X X X       ? ? ?       X X X         X 0 ?         ? 0 X          X X X   0 0 0
 *
 * Special 2x2 cases (where A is a section and B is a different section, X is a wall, 0=space):
 *
 * WallBreakUpRight WallBreakDownLeft WallBreakDownRight  WallBreakUpLeft
 * A X              A X               X B                 X A
 * X B              X B               A X                 B X
 * 
 */
struct DungeonSquareNearby {
public:
 bool mapEdge;
 DungeonSquare *here;
 DungeonSquare *left;
 DungeonSquare *right;
 DungeonSquare *up;
 DungeonSquare *down;
 DungeonSquare *topLeft;
 DungeonSquare *topRight;
 DungeonSquare *bottomLeft;
 DungeonSquare *bottomRight;
 DungeonSquareNearby();
 void Query( int x, int y, DungeonMap *map );
 // 3x3 queries that help decoration
 int NonTraversibleNeighbors();
 int TraversibleNonCardinalNeighbors();
 int NonTraversibleNonCardinalNeighbors();
 int NonTraversibleNonCardinalNeighbors( int NTN );
 int TraversibleNeighbors();
 int Doors();
 bool LeadsNowhere();
 void Completeness( DungeonMap *map ); // Determines if part of the 3x3 grid exceeds map boundaries
 // Sub-queries that look for specific features
 bool MoveLeftAndRightOrUpAndDown();
 bool Pillar();
 bool Enclosed();
 bool Open();
 bool SurroundedSolidBySolid();
 bool CShaped( int NTNCN );
 bool PartialC( int NTNCN, bool isHallway );
 bool VerticalStraightWay( int NTNCN );
 bool HorizontalStraightWay( int NTNCN );
 bool StraightWay( int NTNCN ) { return VerticalStraightWay( NTNCN ) || HorizontalStraightWay( NTNCN ); }
 bool HorizontalHall( int NTNCN );
 bool VerticalHall( int NTNCN );
 bool UpWall( int NTNCN );
 bool DownWall( int NTNCN );
 bool LeftWall( int NTNCN );
 bool RightWall( int NTNCN );
 bool NearUpWall();
 bool NearDownWall();
 bool NearLeftWall();
 bool NearRightWall();
 bool Hallway( int NTNCN ) { return HorizontalHall( NTNCN ) || VerticalHall( NTNCN ); }
 bool LCorner();
 bool Plus();
 bool WallBreakUpRight();
 bool WallBreakDownLeft();
 bool WallBreakDownRight();
 bool WallBreakUpLeft();
// bool FilledLCorner();
};

class DungeonSquare : public ListItem {
public:
 int code;                        // Some arbitrary mark
 DungeonSquareNearby nearby;
 MapSectionInfo *section;         // Marked section id
 MapSectionInfo *region;          // Marked region id (later pass)
 Cartesian coordinates;           // Location of this tile
 BitBrush *stamp;
 bool tabulaRasa;                 // Unwritten/unmolested initial tile.
 bool gauranteed;                 // This tile is further unwritable (flag set internally)
 bool secret;                     // hints that this is a secret area
 bool traversible;                // State (wall or not)
 bool outside;                    // for exterior marking to create entrances or building maps
 bool cave;                       // dug by a caver
 bool damaged;                    // marked as "damaged" due to war, decay, etc
 bool impregnable;                // Cannot be dug through by dungeon builder
 bool room;                       // painted as a room rectangle
 bool corner;                     // Remnants of BitBrush's fortyfive
 bool labyrinth;                  // painted by a maze routine
 bool teleports;                  // square causes teleportation
 bool flooded;                    // has water
 bool opening;                    // flag set by Maze() to indicate a spot where a door could be placed
 bool edge;                       // Edge flag indicates it borders on another section
 bool tunnel;                     // Was drawn with Tunnel()
 int dungeonDepth;                // calculated depth for determining loot/difficulty
 DungeonSquareTypes type;         // is it a room, a hall, or a sub-room space?
 DoorInfo door;                   // what kind of door is this?
 DungeonSpecialTypes special;
 DungeonStairTypes stair;
 DungeonSquare() {
  tabulaRasa=true;
  damaged=cave=outside=tunnel=room=corner=edge=teleports=flooded=opening=gauranteed=traversible=impregnable=false;
  type=dd_room;
  stair=dd_plane;
  teleports=false;
  secret=false;
  section=region=null;
  code=-1;
  stamp=null;
  dungeonDepth=-1;
 }
 void Dig( DungeonSquareTypes type, bool gauranteed ) {
  tabulaRasa=false;
  this->type=type;
  this->gauranteed=gauranteed;
 }
 void Leading( DungeonSquare **from, DungeonSquare **to ) { // do not call when mapEdge
  if ( nearby.up->traversible ) {
   *from=nearby.up;
   *to=nearby.down;
   return;
  }
  if ( nearby.left->traversible ) {
   *from=nearby.left;
   *to=nearby.right;
   return;
  }
  *from=*to=null;
 }
};

class DungeonSquares : public LinkedList {
public:
 CLEARLISTRESET(DungeonSquare);
};

class DungeonSquareHandle : public Handle<DungeonSquare> {
public:
 DungeonSquareHandle( DungeonSquare *ds ) : Handle() {
  this->p=ds;
 }
};

class DungeonSquareHandles : public LinkedList {
public:
 void Add( DungeonSquare *h ) { Append(new DungeonSquareHandle(h)); }
 CLEARLISTRESET(DungeonSquareHandle);
};

class DungeonRoom : public ListItem {
public:
 Cartesian extents;
 bool labyrinth;
 DungeonRoom() {
  labyrinth=false;
 }
};

class DungeonRooms : public LinkedList {
public:
 CLEARLISTRESET(DungeonRoom);
 ~DungeonRooms() { Clear(); }
};

#include "Art.h"

class DungeonLevel;
class DungeonMap : public matrix2d<DungeonSquare> {
public:
 AStarPathfinder pathfinder;
 Zdis<MapSectionInfos> infos;
 Zdis<MapSectionInfos> regions;
 DungeonMap() : matrix2d(0,0) { pathfinder.diagonal=false; }
 ~DungeonMap() { resize(0,0); }
 void UpdateAStarMap( bool avoidTunnels ) {
  int w=M();
  int h=N();
  pathfinder.map.resize(w,h);
  for ( int i=0; i<w; i++ ) for ( int j=0; j<h; j++ ) {
   pathfinder.map(i,j).cost=1.0;
  }
  // Make the map edges non-traversible.
  for ( int i=0; i<w; i++ ) for ( int j=0; j<h; j++ ) if ( pathfinder.map(i,j).cost != 0.0 ) {
   pathfinder.map(i,j).cost=0.0;
   if ( index(i,j).nearby.mapEdge ) {
    pathfinder.map(i,j).traversible=false;
   }
   if ( !(pathfinder.map(i,j).traversible=!index(i,j).traversible) ) {
    if ( avoidTunnels && !index(i,j).nearby.mapEdge && index(i,j).nearby.NonTraversibleNeighbors() == 2 ) {
     pathfinder.map(i-1,j).traversible=false;
     pathfinder.map(i-1,j).traversible=false;
     pathfinder.map(i+1,j).traversible=false;
     pathfinder.map(i,j+1).traversible=false;
     pathfinder.map(i,j-1).traversible=false;
     pathfinder.map(i+1,j+1).traversible=false;
     pathfinder.map(i+1,j-1).traversible=false;
     pathfinder.map(i-1,j+1).traversible=false;
     pathfinder.map(i-1,j-1).traversible=false;
    }
   }
  }
 }
 void ConnectOrphanedSectionsWithSecrets() {
  if ( infos ) {
   infos->SortSpaces();
   EACH(infos->first,MapSectionInfo,i) {
   }
  }
 }
 void Stamp( BitBrush *stamp, int x, int y, int copyScale, int seed, DungeonLevel *level=null );
 void Stroke( BitBrush *brush, int x, int y, int ex, int ey, int steps, int scale, int seed ) {
  double delta=iratiod(1,steps);
  for ( double ratio=0.0; ratio < 1.0; ratio+=delta ) {
   int dx=x+(int)((double)(ex-x)*ratio);
   int dy=y+(int)((double)(ey-y)*ratio);
   Stamp(brush,dx,dy,scale,seed);
  }
 }
 void AttachedRoomCluster( int id, int rooms, int x, int y, int w, int h, int tiny, int big, int seed ) {
  for ( int i=0; i<rooms; i++ ) {

  }
 }
 void Cavern( int id, int x, int y, int w, int h, int seed, bool gaurantee, bool outside ) {
  RoomRectangle(x,y,w,h,3,false,false);
  FlxCaveGenerator *cg=new FlxCaveGenerator(seed,w,h,1+randoms.Upto(6,seed+6,seed),0.4+(double)randoms.Unique(seed,seed)*0.5);
  for ( int i=0; i<w; i++ ) for ( int j=0; j<h; j++ ) {
   char c=cg->map(i,j);
   DungeonSquare *here=&index(x+i,y+j,true);
   if ( here == &outOfBounds || here->gauranteed ) continue;
   if ( c==GROUND && !outside ) { here->traversible=false; continue; }
   here->gauranteed=gaurantee;
   here->cave=true;
   switch ( c ) {
   case WALL: here->traversible = false; break;
   case FLOOR: here->traversible = true; break;
   default: break;
   }
  }
  delete cg;
 }
 void CaveGrid( int id, int x, int y, int w, int h, int partitionsX, int partitionsY, int seed, bool gaurantee, bool outside ) {
  int sw=w/partitionsX;
  int sh=h/partitionsY;
  int k=0;
  for ( int i=0; i<partitionsX; i++ ) for ( int j=0; j<partitionsY; j++ ) {
   int sx=i*sw;
   int sy=j*sh;
   Cavern(id,sx,sy,sw,sh,seed+k,gaurantee,outside);
   k++;
  }
 }
 void MultiRoomDungeon( int id, int x, int y, int w, int h, int seed, bool gaurantee, bool outside ) {
  RoomRectangle(x,y,w,h,3,false,false);
  BDungeon *bd=new BDungeon(seed,w,h,12+randoms.Upto(8,seed,seed),10+randoms.Upto(60,seed,w+id),10+randoms.Upto(65,seed,h+id),500,5+randoms.Upto(20,w+h+id,h),60);
  for ( int i=0; i<w; i++ ) for ( int j=0; j<h; j++ ) {
   char c=bd->getTile(i,j);
   DungeonSquare *here=&index(x+i,y+j,true);
   if ( here == &outOfBounds || here->gauranteed ) continue;
   if ( c==GROUND && !outside ) { here->traversible=false; continue; }
   here->gauranteed=gaurantee;
   switch ( c ) {
   case WALL: here->traversible = false; break;
   case FLOOR: here->traversible = true; break;
   case DOOR: here->traversible=true; here->door.door=true; break;
   case GROUND: here->outside=outside; here->traversible=false; break;
   default: break;
   }
  }
  delete bd;
 }
 void SuperComplex( int seed ) {
  int complexity=(4+seed%20);
  for ( int i=0; i<complexity; i++ ) MultiRoomDungeon( i+i, 0, 0, M(), N(), complexity+seed+i+i*complexity, false, false );
 }
 void RectilinearBSPDungeon( int id, int x, int y, int w, int h, int seed, bool gaurantee ) {
  BSPDungeonRectangles dungeon;
  dungeon.Generate(seed,x,y,w,h);
  EACH(dungeon.first,BSPDungeonRectangle,b) if ( b->dungeon ) {
   RoomRectangle(b->top,b->left,b->width,b->height,1+randoms.Upto(4,b->marcher+seed,seed),false,gaurantee);
  }
 }
 void Roguelike( int id, int type, int x, int y, int w, int h, int seed, bool gaurantee ) {
  RLCMap level;
  RLDungeon dungeon;
  level.Resize(RLSize(w,h));
  switch ( type ) {
   case 0: dungeon.CreateAntNest(level,seed,true); break;
   case 1: dungeon.CreateCaves(level,seed); break;
   case 2: dungeon.CreateMaze(level,seed,true); break;
   case 3: dungeon.CreateMines(level,seed); break;
   case 4: dungeon.CreateSimpleCity(level,seed,10); break;
   case 5: dungeon.CreateSpaceShuttle(level,seed); break;
   default: dungeon.CreateStandardDungeon(level,seed); break;
  }
  for ( int i=0; i<w; i++ ) for ( int j=0; j<h; j++ ) {
   DungeonSquare *here=&index(x+i,y+j,true);
   if ( here == &outOfBounds || here->gauranteed ) continue;
   switch ( level.GetCell(RLPosition(i,j)) ) {
   case -1: break;
   case RLLevelElementWall: here->traversible=false; break;
   case RLLevelElementCorridor: here->traversible=true; here->tunnel=true; break;
   case RLLevelElementGrass: here->traversible=true; here->outside=true; break;
   case RLLevelElementPlant: here->traversible=true; here->outside=true; break;
   case RLLevelElementRoom: here->room=true; here->traversible=true; break;
   case RLLevelElementDoorClose: here->door.door=true; break;
   case RLLevelElementDoorOpen: here->door.door=true; break;
   case RLLevelElementWater: here->outside=true; here->flooded=true; break;
   case RLLevelElementCorridor_value: here->tunnel=true; here->secret=true; here->traversible=true; break;
   case RLLevelElementRoom_value: here->room=true; here->traversible=true; break;
   case RLLevelElementWall_value: here->traversible=false; here->edge=true; break;
    break;
   }
   if ( gaurantee ) here->gauranteed=true;
  }
 }
 void Maze( int id, int x, int y, int mw, int mh, int copyScale, int seed ) {
  SimpleMaze *maze=new SimpleMaze(mw,mh,seed);
  Cartesian mapExtents;
  for ( int i=0; i<mw; i++ ) for ( int j=0; j<mh; j++ ) {
   mapExtents.Corners(x,y,x+mw*copyScale,y+mh*copyScale);
   for ( int k=0; k<copyScale; k++ ) for ( int l=0; l<copyScale; l++ ) {
    int mapx=x+i*copyScale+k;
    int mapy=y+j*copyScale+l;
    DungeonSquare *here=&index(mapx,mapy,true);
    here->code=id;
    if ( here != &outOfBounds && !here->gauranteed ) {
     here->labyrinth=true;
     if ( maze->maze(i,j) > 0 ) {
      here->traversible=true;
     }
     here->edge=(mapx==mapExtents.x || mapy==mapExtents.y || mapx==mapExtents.x2-1 || mapy==mapExtents.y2-1 ) && !here->traversible;
    }
   }
  }
  DungeonSquare *here;
  here=&index(mapExtents.x,mapExtents.y,true);
  if ( here != &outOfBounds ) here->edge=false;
  here=&index(mapExtents.x2-1,mapExtents.y2-1,true);
  if ( here != &outOfBounds ) here->edge=false;
  delete maze;
 }
 void MazeGrid( int id, int x, int y, int mw, int mh, int copyScale, int seed, int gw, int gh, int spacing ) {
  int w=mw*copyScale;
  int h=mh*copyScale;
  for ( int i=0; i<gw; i++ ) for ( int j=0; j<gh; j++ )
   Maze(id+i,x+i*mw+spacing*i,y+j*mh+spacing*j,mw,mh,copyScale,seed+j+i);
 }
 void MazeWithTreasureRooms( int id, int x, int y, int mw, int mh, int copyScale, int doorLimit, int seed, int bigRooms, int smallRooms, bool centerChamber ) {
  int bigRoomSize=(copyScale*mw)/(bigRooms*2);
  int smallRoomSize=(copyScale*mh)/(smallRooms*2);
  int actualBigRooms=1+randoms.Upto(bigRooms-1,seed,seed);
  int actualSmallRooms=1+randoms.Upto(smallRooms-1,seed,bigRooms);
  int w=copyScale*mw;
  int h=copyScale*mh;
  Maze(id,x,y,mw,mh,copyScale,seed);
  for ( int i=0; i<actualBigRooms; i++ ) {
   RoomRectangle(
    x+randoms.Upto(UMAX(mw*copyScale-bigRoomSize,0),seed+i,seed+i)+bigRoomSize,
    y+randoms.Upto(UMAX(mh*copyScale-bigRoomSize,0),seed+i,seed)+bigRoomSize,
    bigRoomSize+(randoms.Upto(bigRoomSize/8,seed+i,i+seed)-bigRoomSize/4),
    bigRoomSize+(randoms.Upto(bigRoomSize/8,seed+i,i)-bigRoomSize/4),
    doorLimit,
    true, true
    );
  }
  for ( int i=0; i<smallRooms; i++ ) {
   RoomRectangle(
    x+randoms.Upto(UMAX(mw*copyScale-smallRoomSize,0),seed+i,i+seed)+smallRoomSize,
    y+randoms.Upto(UMAX(mh*copyScale-smallRoomSize,0),seed+i,i)+smallRoomSize,
    smallRoomSize+(randoms.Upto(smallRoomSize/8,seed+i,i+seed)-smallRoomSize/4),
    smallRoomSize+(randoms.Upto(smallRoomSize/8,seed+i,i)-smallRoomSize/4),
    doorLimit,
    true, true
    );
  }
  if ( centerChamber ) RoomRectangle(x+(w/3),y+(h/3),w/3,h/3,doorLimit,true,true);
 }
 void RoomRectangle( int x, int y, int w, int h, int doorLimit, bool labyrinthine, bool gaurantee, bool border=true ) {
  if ( w < MIN_SIZE || h < MIN_SIZE ) return;
  Cartesians doors;
  for ( int i=0; i<w; i++ ) for ( int j=0; j<h; j++ ) {
   DungeonSquare *here=&index(x+i,y+j,true);
   if ( here != &outOfBounds && !here->gauranteed ) {
    bool corner=( i==0 && (j==0  || j==h-1)) || (j==0 && (i==0 || i==w-1));
    bool side=!corner && ( i==0 || j==0 || i==w-1 || j==h-1 );
    here->traversible=!( border && (corner||side) );
    here->room=true;
    here->gauranteed=gaurantee;
    here->corner=corner;
    here->labyrinth=here->labyrinth||labyrinthine;
    if ( !here->nearby.mapEdge && side && !corner && here->nearby.StraightWay(here->nearby.NonTraversibleNonCardinalNeighbors()) ) doors.Add(x+i,y+j);
   }
  }
  for ( int i=0; i<doorLimit && doors.count > 0; i++ ) {
   Cartesian *door=(Cartesian *) doors.Any(i);
   doors.Remove(door);
   DungeonSquare *there=&index(door->x,door->y);
   there->traversible=true;
   there->door.door=true;
   delete door;
  }
 }
 void MarkSection( int x, int y, MapSectionInfo *info ) {
  DungeonSquare *here=&index(x,y,true);
  if ( here == &outOfBounds ) return;
  if ( !here->traversible || here->section ) return;
  if ( here->door.door ) {
   info->doors.Add(x,y);
   return;
  }
  here->section=info;
//  info->squares.Add(x,y);
  info->spaces.AddIfUnique(x,y);
  MarkSection(x+1,y,info);
  MarkSection(x-1,y,info);
  MarkSection(x,y+1,info);
  MarkSection(x,y-1,info);
 }
 MapSectionInfos *MarkSections( int seed ) {
  if ( infos ) delete infos;
  this->infos= new MapSectionInfos;
  DungeonSquare *here;
  int w=M();
  int h=N();
  for ( int i=0; i<w; i++ ) for ( int j=0; j<h; j++ ) {
   here=&index(i,j);
   here->section=null;
  }
  for ( int i=0; i<w; i++ ) for ( int j=0; j<h; j++ ) {
   here=&index(i,j);
   if ( !here->section && here->traversible && !here->door.door ) {
    MapSectionInfo *info=new MapSectionInfo;
    infos->Append(info);
    info->room=here->room;
    MarkSection(here->coordinates.x,here->coordinates.y,info);
   }
  }
  // Calculate MapSectionInfo approximate box extents and other metrics
  infos->Calculate(seed,this);
  return infos;
 }
 void MarkRegion( int x, int y, MapSectionInfo *region ) {
  DungeonSquare *here=&index(x,y,true);
  if ( here == &outOfBounds || (!here->door.door && !here->traversible) || here->region ) return;
  if ( here->door.door ) region->doors.AddIfUnique(x,y);
  here->region=region;
//  info->squares.Add(x,y);
  region->spaces.AddIfUnique(x,y);
  MarkRegion(x+1,y,region);
  MarkRegion(x-1,y,region);
  MarkRegion(x,y+1,region);
  MarkRegion(x,y-1,region);
 }
 MapSectionInfos *MarkRegions( int seed ) {
  if ( regions ) delete regions;
  this->regions= new MapSectionInfos;
  DungeonSquare *here;
  int w=M();
  int h=N();
  for ( int i=0; i<w; i++ ) for ( int j=0; j<h; j++ ) {
   here=&index(i,j);
   here->region=null;
  }
  for ( int i=0; i<w; i++ ) for ( int j=0; j<h; j++ ) {
   here=&index(i,j);
   if ( !here->region && ( here->traversible || here->door.door) ) {
    MapSectionInfo *region=new MapSectionInfo;
    regions->Append(region);
    region->room=here->room;
    MarkRegion(here->coordinates.x,here->coordinates.y,region);
   }
  }
  // Calculate MapSectionInfo approximate box extents and other metrics
  regions->Calculate(seed,this);
  return regions;
 }
 DungeonSquare *FindFirstWall( int x, int y, bool NotGauranteed, bool NotImpregnable ) {
  DungeonSquare *candidate;
  candidate=&index(x+1,y,true);
  if ( candidate != &outOfBounds && !candidate->traversible && (!candidate->impregnable || NotImpregnable) && (!candidate->gauranteed || NotGauranteed) ) return candidate;
  candidate=&index(x,y+1);
  if ( candidate != &outOfBounds && !candidate->traversible && (!candidate->impregnable || NotImpregnable) && (!candidate->gauranteed || NotGauranteed) ) return candidate;
  candidate=&index(x-1,y);
  if ( candidate != &outOfBounds && !candidate->traversible && (!candidate->impregnable || NotImpregnable) && (!candidate->gauranteed || NotGauranteed) ) return candidate;
  candidate=&index(x,y-1);
  if ( candidate != &outOfBounds && !candidate->traversible && (!candidate->impregnable || NotImpregnable) && (!candidate->gauranteed || NotGauranteed) ) return candidate;
  return null;
 }
 MapSectionInfo *Probe(DungeonSquare *here,CardinalDirection *direction) {
  if ( /*here->gauranteed ||*/ here->corner ) return null;
  DungeonSquare *there;
  CardinalDirection *side=direction->left;
  DungeonSquare *left,*right;
  there=&index(here->coordinates.x+direction->vector.x,here->coordinates.y+direction->vector.y,true);
  left=&index(here->coordinates.x+side->vector.x,here->coordinates.y+side->vector.y,true);
  right=&index(here->coordinates.x+side->reverse->vector.x,here->coordinates.y+side->reverse->vector.y,true);
  while ( there != &outOfBounds && !there->traversible
       && left  != &outOfBounds && !left->traversible
       && right != &outOfBounds && !right->traversible ) {
   if ( /*there->gauranteed ||*/ there->corner ) return null;
   there=&index(there->coordinates.x+direction->vector.x,there->coordinates.y+direction->vector.y,true);
   left=&index(here->coordinates.x+side->vector.x,here->coordinates.y+side->vector.y,true);
   right=&index(here->coordinates.x+side->reverse->vector.x,here->coordinates.y+side->reverse->vector.y,true);
  }
  //if ( there->gauranteed ) return null;
  if ( there == &outOfBounds || left == &outOfBounds || right == &outOfBounds ) return null;
  if ( !there->traversible ) return null;
  return there->section;
 }
 MapSectionInfo *Probe(DungeonSquare *here,CardinalDirection *direction, bool searchRegions ) {
  if ( /*here->gauranteed ||*/ here->corner ) return null;
  DungeonSquare *there;
  CardinalDirection *side=direction->left;
  DungeonSquare *left,*right;
  there=&index(here->coordinates.x+direction->vector.x,here->coordinates.y+direction->vector.y,true);
  left=&index(there->coordinates.x+side->vector.x,there->coordinates.y+side->vector.y,true);
  right=&index(there->coordinates.x+side->reverse->vector.x,there->coordinates.y+side->reverse->vector.y,true);
  while ( there != &outOfBounds && !there->traversible
       && left  != &outOfBounds && !left->traversible
       && right != &outOfBounds && !right->traversible ) {
   if ( /*there->gauranteed ||*/ there->corner ) return null;
   there=&index(there->coordinates.x+direction->vector.x,there->coordinates.y+direction->vector.y,true);
   left=&index(there->coordinates.x+side->vector.x,there->coordinates.y+side->vector.y,true);
   right=&index(there->coordinates.x+side->reverse->vector.x,there->coordinates.y+side->reverse->vector.y,true);
  }
  //if ( there->gauranteed ) return null;
  if ( there == &outOfBounds || left == &outOfBounds || right == &outOfBounds ) return null;
  if ( !there->traversible ) return null;
  return there->region;
 }
 void Tunnel( bool door, DungeonSquare *first_square_to_demolish, CardinalDirection *direction, bool secret=false ) {
  DungeonSquare *there;
  DungeonSquare *left,*right;
  there=first_square_to_demolish;
  if ( secret ) there->secret=true;
  there->traversible=true;
  there->tunnel=true;
  there->door.door=door;
  there=&index(there->coordinates.x+direction->vector.x,there->coordinates.y+direction->vector.y,true);
  while ( there != &outOfBounds && !there->traversible ) {
   there->traversible=true;
   there->tunnel=true;
   if ( direction == north || direction == south ) {
    left=there->nearby.left;
    right=there->nearby.right;
   } else {
    left=there->nearby.up;
    right=there->nearby.down;
   }
   there->outside=false;
   left->outside=false;
   right->outside=false;
   if ( secret ) there->secret=true;
   there=&index(there->coordinates.x+direction->vector.x,there->coordinates.y+direction->vector.y,true);
  }
 }
 void RemoveStrangeDoors( int seed ) {
  StrayDoorAndWallRemoval();
  MarkSections( seed );
  StrayDoorAndWallRemoval();
  MarkSections( seed );
  StrayDoorAndWallRemoval();
  MarkSections( seed );
  StrayDoorAndWallRemoval();
  MarkSections( seed );
  StrayDoorAndWallRemoval();
  MarkSections( seed );
 }
 void FillCaddyCornerHoles( int seed ) {  
  DungeonSquare *here,*there;
  MapSectionInfo *a=null;
  MapSectionInfo *b=null;
  int w=M()-1;
  int h=N()-1;
  for ( int i=1; i<w; i++ ) for ( int j=1; j<h; j++ ) { // no mapEdge tiles
   here=&index(i,j);
   if ( !here->traversible ) {
    there=null;
    if ( here->nearby.WallBreakDownLeft() ) {
     if ( randoms.Query(seed+i,j) ) here->nearby.down->traversible=false;
     else here->nearby.left->traversible=false;
    } else
    if ( here->nearby.WallBreakUpRight() ) {
     if ( randoms.Query(seed+i,j) ) here->nearby.up->traversible=false;
     else here->nearby.right->traversible=false;
    } else
    if ( here->nearby.WallBreakDownRight() ) {
     if ( randoms.Query(seed+i,j) ) here->nearby.down->traversible=false;
     else here->nearby.right->traversible=false;
    } else
    if ( here->nearby.WallBreakUpLeft() ) {
     if ( randoms.Query(seed+i,j) ) here->nearby.up->traversible=false;
     else here->nearby.left->traversible=false;
    }
   }
  }
 }
 void RemoveSingleRoomSections() {
  if ( infos ) {
   EACHN(infos->first,MapSectionInfo,m,{
    if ( m->spaces.count == 1 && m->doors.count == 0 ) {
     Cartesian *sxy=(Cartesian *) m->spaces.first;
     DungeonSquare *square=&index(sxy->x,sxy->y);
     square->traversible=false;
     square->door.door=false;
     infos->Remove(m);
     delete m;
    }
   });
  }
 }
 void ConnectUnconnectedSections() {
  EACH(infos->first,MapSectionInfo,i) {
   if ( i->doors.count == 0 ) {
    EACH(i->spaces.first,Cartesian,square) {
     DungeonSquare *here=&index(square->x,square->y);
     if ( !here->traversible || here->nearby.mapEdge ) continue;
     int NTN=here->nearby.NonTraversibleNeighbors();
     int NTNCN=here->nearby.NonTraversibleNonCardinalNeighbors(NTN);
     if ( here->nearby.HorizontalHall(NTNCN) ) {
      MapSectionInfo *probed=Probe(here,north);
      if ( probed && probed != here->section ) {
       Tunnel(true,here->nearby.up,north);
       break;
      } else {
       probed=Probe(here,south);
       if ( probed && probed != here->section ) {
        Tunnel(true,here->nearby.down,south);
        ///
       } else {
        continue;
       }
      }
     } else if ( here->nearby.VerticalHall(NTNCN) ) {
      MapSectionInfo *probed=Probe(here,west);
      if ( probed && probed != here->section ) {
       Tunnel(true,here->nearby.left,west);
       break;
      } else {
       probed=Probe(here,east);
       if ( probed && probed != here->section ) {
        Tunnel(true,here->nearby.right,east);
        ///
       } else {
        continue;
       }
      }
     } else if ( here->nearby.NearUpWall() ) {
      MapSectionInfo *probed=Probe(here,north);
      if ( probed ) {
       Tunnel(true,here->nearby.up,north);
       break;
      } else continue;
     } else if ( here->nearby.NearDownWall() ) {
      MapSectionInfo *probed=Probe(here,south);
      if ( probed && probed != here->section ) {
       Tunnel(true,here->nearby.down,south);
       break;
      } else continue;
     } else if ( here->nearby.NearLeftWall() ) {
      MapSectionInfo *probed=Probe(here,west);
      if ( probed && probed != here->section ) {
       Tunnel(true,here->nearby.left,west);
       break;
      } else continue;
     } else if ( here->nearby.NearDownWall() ) {
      MapSectionInfo *probed=Probe(here,east);
      if ( probed && probed != here->section ) {
       Tunnel(true,here->nearby.right,east);
       break;
      } else continue;
     }
    }
    if ( i->edges.count > 0 ) { // At a random edge, we can dig a connecting tunnel
      /*
     EACH(i->edges.first,Cartesian,edge) {
      DungeonSquare *here=&index(edge->x,edge->y);
      int NTN=here->nearby.NonTraversibleNeighbors();
      int NTNCN=here->nearby.NonTraversibleNonCardinalNeighbors(NTN);
      Cartesian direction;
      if ( here->nearby.HorizontalStraightWay(NTN) ) {
      } else if ( here->nearby.VerticalStraightWay(NTN) ) {
      } else if ( here->nearby.LeftWall(NTN) ) {
      } else if ( here->nearby.RightWall(NTN) ) {
      } else if ( here->nearby.UpWall(NTN) ) {
      } else if ( here->nearby.DownWall(NTN) ) {
      }
      target->door.door=true;
      target->traversible=true;
      DungeonSquare *there;
      there=&index(target->coordinates.x+direction.x,target->coordinates.y+direction.y);
      while ( there != &outOfBounds && !there->traversible ) {
       there->traversible=true;
       there=&index(there->coordinates.x+direction.x,there->coordinates.y+direction.y);       
      }
     } */
    } else { // We can either remove the walls, place a room on its extents, or fill this in.
    }
   }
  }
 }
 void ConnectUnconnectedRegions() {
  if ( !regions || regions->count <= 1 ) return;
  // Check for easy spots to break a hole in the wall creating connections
  DungeonSquare *from,*to;
  int w=M()-1;
  int h=N()-1;
  for ( int i=1; i<w; i++ ) for ( int j=1; j<h; j++ ) {
   DungeonSquare *here=&index(i,j);
   if ( here->traversible ) continue;
   here->Leading(&from,&to);
   if ( from && to && from->region && to->region && from->region != to->region ) {
    here->door.door=true;
    here->secret=true;
    return;
   }
  }
  // Search for not-so-easy connections
  EACH(regions->first->next,MapSectionInfo,i) {
   EACH(i->spaces.first,Cartesian,square) {
    DungeonSquare *here=&index(square->x,square->y);
    if ( here->nearby.mapEdge || here->nearby.Doors() > 0 || !here->traversible ) continue;
    int NTN=here->nearby.NonTraversibleNeighbors();
    if ( NTN == 0 ) continue;
    int NTNCN=here->nearby.NonTraversibleNonCardinalNeighbors(NTN);
    if ( NTNCN == 0 ) continue;
    if ( here->nearby.HorizontalHall(NTNCN) ) {
     MapSectionInfo *probed=Probe(here,north,true);
     if ( probed && probed != here->region ) {
      Tunnel(true,here->nearby.up,north,true);
      return;
      break;
     } else {
      probed=Probe(here,south,true);
      if ( probed && probed != here->region ) {
       Tunnel(true,here->nearby.down,south,true);
       return;
      } else {
       continue;
      }
     }
    } else if ( here->nearby.VerticalHall(NTNCN) ) {
     MapSectionInfo *probed=Probe(here,west,true);
     if ( probed && probed != here->region ) {
      Tunnel(true,here->nearby.left,west,true);
      return;
      break;
     } else {
      probed=Probe(here,east,true);
      if ( probed && probed != here->region ) {
       Tunnel(true,here->nearby.right,east,true);
       return;
      } else {
       continue;
      }
     }
    } else if ( here->nearby.NearUpWall() ) {
     MapSectionInfo *probed=Probe(here,north,true);
     if ( probed ) {
      Tunnel(true,here->nearby.up,north,true);
      return;
      break;
     } else continue;
    } else if ( here->nearby.NearDownWall() ) {
     MapSectionInfo *probed=Probe(here,south,true);
     if ( probed && probed != here->region ) {
      Tunnel(true,here->nearby.down,south,true);
      return;
      break;
     } else continue;
    } else if ( here->nearby.NearLeftWall() ) {
     MapSectionInfo *probed=Probe(here,west,true);
     if ( probed && probed != here->region ) {
      Tunnel(true,here->nearby.left,west,true);
      return;
      break;
     } else continue;
    } else if ( here->nearby.NearDownWall() ) {
     MapSectionInfo *probed=Probe(here,east,true);
     if ( probed && probed != here->region ) {
      Tunnel(true,here->nearby.right,east,true);
      return;
      break;
     } else continue;
    }
   }
  }
  return;
 }

 void StrayDoorAndWallRemoval() {
  int w=M();
  int h=N();
  for ( int i=0; i<w; i++ ) for ( int j=0; j<h; j++ ) {
   DungeonSquare *here=&index(i,j);
   if (here->nearby.mapEdge) continue;
   if ( here->nearby.Open() ) {
    here->traversible=true;
    here->door.door=false;
   } else if ( here->nearby.LeadsNowhere() ) here->door.door=false;
   else   if ( !here->nearby.MoveLeftAndRightOrUpAndDown() ) here->door.door=false;
   else   if ( here->nearby.Enclosed() ) {
    here->door.door=false;
    here->traversible=false;
   }
   else if ( here->nearby.NonTraversibleNonCardinalNeighbors() == 0 ) {
    here->door.door=false;
   }
  }
 }
 void SealMapEdges( bool overwrite=false ) {
  DungeonSquare *here;
  int w=M();
  int h=N();
  for ( int i=0; i<w; i++ ) {
   here=&index(i,0);
   if ( (!here->gauranteed || overwrite) && here ) {
    here->traversible=false;
    here->section=null;
   }
   here=&index(i,h-1);
   if ( (!here->gauranteed || overwrite) && here ) {
    here->traversible=false;
    here->section=null;
   }
  }
  for ( int j=0; j<h; j++ ) {
   here=&index(0,j);
   if ( (!here->gauranteed || overwrite) && here ) {
    here->traversible=false;
    here->section=null;
   }
   here=&index(w-1,j);
   if ( (!here->gauranteed || overwrite) && here ) {
    here->traversible=false;
    here->section=null;
   }
  }
 }
 void OnResize() {
  int w=M();
  int h=N();
  for ( int i=0; i<w; i++ ) for ( int j=0; j<h; j++ ) {
   DungeonSquare *square=&index(i,j);
   square->coordinates.Set(i,j);
   square->nearby.Query(i,j,this);
  }
 }
 int TraversibleArea() {
  int tally=0;
  unsigned int w=M();
  unsigned int h=N();
  for ( unsigned int i=0; i<w; i++ ) for ( unsigned int j=0; j<h; j++ ) {
   DungeonSquare *square=&index(i,j);
   if ( square->traversible ) tally++;
  }
  return tally;
 }
#define DGSIZE 7
 void Draw() {
  
  DungeonSquare *here;

  // Draw map
  int w=M();
  int h=N();
  for ( int i=0; i<w; i++ ) for ( int j=0; j<h; j++ ) {
   here= &index(i,j);
   Blending(none);
   if ( here->region && input.KeyDown(DX_TAB) ) {
    Area(here->region->color,i*DGSIZE,j*DGSIZE,i*DGSIZE+DGSIZE,j*DGSIZE+DGSIZE);
   } else
   if ( here->outside ) {
    Area(crayons.Pick(purple),i*DGSIZE,j*DGSIZE,i*DGSIZE+DGSIZE,j*DGSIZE+DGSIZE);
   } else
   if ( here->door.door ) {
    Area(crayons.Pick(wfeYellow),i*DGSIZE,j*DGSIZE,i*DGSIZE+DGSIZE,j*DGSIZE+DGSIZE);
   } else if ( here->traversible ) {
    Area(here->section?here->section->color:crayons.Pick(vermillion),i*DGSIZE,j*DGSIZE,i*DGSIZE+DGSIZE,j*DGSIZE+DGSIZE);
   } else if ( here->impregnable ) {
    Blending(additive);
    Area(crayons.Pick(dupontGray),i*DGSIZE,j*DGSIZE,i*DGSIZE+DGSIZE,j*DGSIZE+DGSIZE);
   }   
   if ( input.KeyDown(DX_SPACE) && here->edge ) {
    Blending(additive);
    Area(crayons.Pick(hotPink),i*DGSIZE,j*DGSIZE,i*DGSIZE+DGSIZE,j*DGSIZE+DGSIZE);
   }
   if ( here->secret ) {
    Blending(additive);
    Rectangle(crayons.Pick(hotPink),i*DGSIZE,j*DGSIZE,i*DGSIZE+DGSIZE,j*DGSIZE+DGSIZE);
   }
  }  
  // Tile probe
  int mx=input.mxi/DGSIZE;
  int my=input.myi/DGSIZE;
  here=&index(mx,my,true);
  double tx=display.wd-display.wd/4.0;
  double ty=display.hd-display.hd/4.0;
  glColor3d(1.0,1.0,1.0);
  if ( here == &outOfBounds ) {
   Text("Out of Bounds",tx,ty,6.0,7.0,false);
  } else {
   char buf[128];
   if ( here->section ) here->section->Calculate(this);
   FORMAT(buf,128,"%d, %d\n%s\n%s\n%s\nsection %d [%dx%d]\n(%d doors %d walls %d spaces %d squares)\nregion %d\nid=%d\n%s\n%s\n%s",
    (int) mx, (int) my,
    here->traversible?"traversible":"wall",
    here->edge?"edge flagged":"",
    here->tunnel?"tunnel":"",
    (int) (here->section ? infos->IndexOf(here->section)  : -1),
    (int) (here->section ? here->section->extent.w        :  0),
    (int) (here->section ? here->section->extent.h        :  0),
    (int) (here->section ? here->section->doors.count     : -1),
    (int) (here->section ? here->section->walls.count     : -1),
    (int) (here->section ? here->section->spaces.count    : -1),
    (int) (here->section ? here->section->squares.count   : -1),
    (int) (here->region  ? regions->IndexOf(here->region) : -1),
    (int) here->code,
    here->outside?"outside":"",
    here->cave?"cave":"",
    here->room?"room":""
   );
   Text(buf,tx,ty,6.0,7.0,3.0,1.0);
   FORMAT(buf,128,"%3d%3d%3d\n%3d%3d%3d\n%3d%3d%3d",
    (int) (here->nearby.topLeft     ? (here->nearby.topLeft->section    ? infos->IndexOf(here->nearby.topLeft->section)     :-1) :-1),
    (int) (here->nearby.up          ? (here->nearby.up->section         ? infos->IndexOf(here->nearby.up->section)          :-1) :-1),
    (int) (here->nearby.topRight    ? (here->nearby.topRight->section   ? infos->IndexOf(here->nearby.topRight->section)    :-1) :-1),
    (int) (here->nearby.left        ? (here->nearby.left->section       ? infos->IndexOf(here->nearby.left->section)        :-1) :-1),
    (int) (here->nearby.here        ? (here->nearby.here->section       ? infos->IndexOf(here->nearby.here->section)        :-1) :-1),
    (int) (here->nearby.right       ? (here->nearby.right->section      ? infos->IndexOf(here->nearby.right->section)       :-1) :-1),
    (int) (here->nearby.bottomLeft  ? (here->nearby.bottomLeft->section ? infos->IndexOf(here->nearby.bottomLeft->section)  :-1) :-1),
    (int) (here->nearby.down        ? (here->nearby.down->section       ? infos->IndexOf(here->nearby.down->section)        :-1) :-1),
    (int) (here->nearby.bottomRight ? (here->nearby.bottomRight->section? infos->IndexOf(here->nearby.bottomRight->section) :-1) :-1)
   );
   if ( here->stamp ) here->stamp->Debug( 10.0, display.hd/2.0 );
   Text(buf,tx+display.wd/8.0,ty+display.hd/8.0);
   Rectangle(crayons.Any(),input.mxi-DGSIZE-DGSIZE/2,input.myi-DGSIZE-DGSIZE/2,input.mxi+DGSIZE+DGSIZE/2,input.myi+DGSIZE+DGSIZE/2);
  }
 }
 bool Edge( int x, int y ) {
  return ( x <= 0 || y <= 0 || x >= M()-1 || y >= N()-1 );
 }
 bool Outside( int x, int y ) {
  return ( x < 0 || y < 0 || x > M()-1 || y >=N()-1 );
 }
 bool Empty( int x, int y, int w, int h ) {
  for ( int i=x; i<x+w; i++ ) for ( int j=y; j<y+h; j++ ) {
   if ( Outside(i,j) || Edge(i,j) ) return false;
   DungeonSquare *here=&index(i,j);
   if ( !here || here->impregnable || here->traversible || !here->tabulaRasa || here->gauranteed ) return false;
  }
  return true;
 }
 int Clutter( int x, int y, int w, int h ) {
  int clutter=0;
  for ( int i=x; i<x+w; i++ ) for ( int j=y; j<y+h; j++ ) {
   if ( Outside(i,j) || Edge(i,j) ) {
    clutter++;
    continue;
   }
   DungeonSquare *here=&index(i,j);
   if ( !here || here->impregnable || here->traversible || !here->tabulaRasa || here->gauranteed ) clutter++;
  }
  return clutter;
 }
 DungeonSquare *Relative( int x, int y, CardinalDirection *direction, int mag ) {
  int dx=x+direction->vector.x*mag;
  int dy=y+direction->vector.y*mag;
  if ( Edge(dx,dy) ) return null;
  return &index(dx,dy);
 }
 DungeonSquare *Relative( int x, int y, int dirx, int diry, int mag ) {
  int dx=x+dirx*mag;
  int dy=y+diry*mag;
  if ( Edge(dx,dy) ) return null;
  return &index(dx,dy);
 }
 int TraversibleNeighbors( int x, int y, int dist ) { return 4-NonTraversibleNeighbors(x,y,dist); }
 int NonTraversibleNeighbors( int x, int y, int dist ) {
  int n=0;
  DungeonSquare *test;
  test=Relative(x,y,north,dist);
  if ( test && !test->traversible && !test->impregnable ) n++;
  test=Relative(x,y,south,dist);
  if ( test && !test->traversible && !test->impregnable ) n++;
  test=Relative(x,y,east,dist);
  if ( test && !test->traversible && !test->impregnable ) n++;
  test=Relative(x,y,west,dist);
  if ( test && !test->traversible && !test->impregnable ) n++;
  return n;
 }
 int NonTraversibleNonCardinalNeighbors( int x, int y, int dist ) {
  int n=0;
  DungeonSquare *test;
  test=Relative(x,y,0,1,dist);
  if ( test && !test->traversible && !test->impregnable ) n++;
  test=Relative(x,y,1,0,dist);
  if ( test && !test->traversible && !test->impregnable ) n++;
  test=Relative(x,y,0,-1,dist);
  if ( test && !test->traversible && !test->impregnable ) n++;
  test=Relative(x,y,-1,0,dist);
  if ( test && !test->traversible && !test->impregnable ) n++;
  test=Relative(x,y,1,1,dist);
  if ( test && !test->traversible && !test->impregnable ) n++;
  test=Relative(x,y,-1,-1,dist);
  if ( test && !test->traversible && !test->impregnable ) n++;
  test=Relative(x,y,-1,1,dist);
  if ( test && !test->traversible && !test->impregnable ) n++;
  test=Relative(x,y,1,-1,dist);
  if ( test && !test->traversible && !test->impregnable ) n++;
  return n;
 }
 int WallThickness( CardinalDirection *direction, int startx, int starty ) {
  DungeonSquare *square=&index(startx,starty);
  if ( square->traversible ) return 0;
  int d=0;
  while ( square != &outOfBounds && !square->traversible && !square->gauranteed ) { square=&index(startx+=direction->vector.x,starty+=direction->vector.y,true); d++; } 
  return d;
 }
 int NonwallDistance( CardinalDirection *direction, int x, int y ) {
  if ( direction->stationary() ) return 0;
  int d=0;
  if ( direction->vector.y != 0 ) {
   if ( direction->vector.y > 0 ) {
    int w=M();
    y++;
    DungeonSquare *square=&index(x,y);
    while ( y < w ) {
     if ( square->traversible || !square->impregnable || square->gauranteed ) return d;
     y++; d++;
     square=&index(x,y);
    }
   } else {
    int w=M();
    y--;
    DungeonSquare *square=&index(x,y);
    while ( y > 0 ) {
     if ( square->traversible || !square->impregnable || square->gauranteed ) return d;
     y--; d++;
     square=&index(x,y);
    }
   }
  } else {
   if ( direction->vector.x > 0 ) {
    int w=N();
    x++;
    DungeonSquare *square=&index(x,y);
    while ( x < w ) {
     if ( square->traversible || !square->impregnable || square->gauranteed ) return d;
     x++; d++;
     square=&index(x,y);
    }
   } else {
    int w=N();
    x--;
    DungeonSquare *square=&index(x,y);
    while ( x > 0 ) {
     if ( square->traversible || !square->impregnable || square->gauranteed ) return d;
     x--; d++;
     square=&index(x,y);
    }
   }
  }
  return d;
 }
 void Dig( DungeonSquareHandles *write_to, Cartesian *from_to, DungeonSquareTypes type, bool gauranteed ) {
  Cartesian i;
  int w=M();
  int h=N();
  i.x=URANGE(0,from_to->x,w-1);
  i.y=URANGE(0,from_to->y,h-1);
  i.x2=URANGE(0,from_to->x2,w-1);
  i.y2=URANGE(0,from_to->y2,h-1);
  for ( ; i.x < from_to->x2; i.x++ ) for ( ; i.y < from_to->y2; i.y++ ) {
   index(i.x,i.y).Dig(type,gauranteed);
   write_to->Append(new DungeonSquareHandle(&index(i.x,i.y)));
  }
 }
 void Dig( DungeonSquare *here, int radius, bool dig, bool square=true ) {
  here->traversible=true;
  if ( here->nearby.mapEdge ) return;
  if ( square ) {
   for ( int i=-radius; i<radius; i++ ) for ( int j=-radius; j<radius; j++ ) {
    DungeonSquare *there=&index(i+here->coordinates.x,j+here->coordinates.y,true);
    if ( there == &outOfBounds || there->gauranteed ) continue;
    there->traversible=dig;
   }
  } else {
   for ( int i=-radius; i<radius; i++ ) for ( int j=-radius; j<radius; j++ ) if ( idistance(i,j,0,0) < (float) radius ) {
    DungeonSquare *there=&index(i+here->coordinates.x,j+here->coordinates.y);
    if ( there->gauranteed ) continue;
    there->traversible=dig;
   }
  }
 }
};

class DungeonLevels;

// Level base class
class DungeonLevel : public ListItem {
public:
 ALives alives;
 int seed;
 Random *set;
 int openings;  // number of exits to leave on edges
 bool labyrinths;
 bool stairwells;
 bool ladders;
 bool anterooms;
 int w,h;
 DungeonMap squares;
 DungeonLevel() {
  Constructor();
 }
 DungeonLevel( int seed, int w, int h ) {
  Constructor();
  SetSize(w,h);
  SetSeed(seed);
 }
 void Constructor() {
  seed=0;
  set=randoms.GetSet(seed+420);
  openings=0;
  labyrinths=true;
  stairwells=false;
  ladders=false;
  anterooms=true;
 }
 void SetSeed( int seed ) {
  this->seed=seed;
  set=randoms.GetSet(seed+420);
 }
 void SetSize( int w, int h ) {
  squares.resize(w,h);
  this->w=w;
  this->h=h;
 }
 void Nearby( int x, int y, ALife *seeking ) {
  seeking->nearby.Clear();
  EACH(alives.first,ALife,a) if ( a!=seeking && a->position.x == x && a->position.y == y ) seeking->nearby.Append(new ALifeHandle(a));
 }
 void Generate() {
  while ( alives.count > 0 ) alives.Generation();
 }
 void Post( DungeonLevels *levels );
 void DrawMap() {
  squares.Draw();
 }
};

class DungeonLevels : public LinkedList {
public:
 void Post(int seed) {
  FOREACH(DungeonLevel,d) {
   d->squares.MarkSections(seed);
   d->Post(this);
   d->squares.MarkRegions(seed);
   int attempts=d->squares.regions->count*2;
   for ( int i=0; i<attempts; i++ ) {
    d->squares.ConnectUnconnectedRegions();
    d->squares.MarkRegions(seed);
    if ( d->squares.regions->count <= 1 ) break;
   }
  }
 }
 CLEARLISTRESET(DungeonLevel);
};

class DungeonRoomBuilder : public ALife {
public:
};

class DungeonDFS : public ALife {
public:
 DungeonDFS( DungeonLevel *level ) : ALife(0,level) {}
 void Birth() {
  age=0;
  lifespan=1;
 }
 void Generation() {
  DungeonSquare *here=&level->squares(position.x,position.y);
  if ( here && level->squares.NonTraversibleNonCardinalNeighbors(here->coordinates.x,here->coordinates.y,1) > 6 ) {
   here->traversible=true;
   Legacy();
  }
  else die=true;
 }
 void Deployment( CardinalDirection *direction ) {  
  DungeonDFS *dfs;
  DungeonSquare *test;
  test=MovingTo(direction,1);
  if ( test && !level->squares.Edge(test->coordinates.x,test->coordinates.y) && !test->traversible ) {
   if ( level->squares.NonTraversibleNonCardinalNeighbors(test->coordinates.x,test->coordinates.y,1) > 6 ) {
    level->alives.Append(dfs=new DungeonDFS(level));
    dfs->position.Set(&position);
    dfs->Move(direction);
   }
  }
 }
 void Legacy() {
  Deployment(north);
  Deployment(south);
  Deployment(east);
  Deployment(west);
 }
};

class DungeonRoomDeployer : public ALife {
public:
 int roomSize;
 DungeonRoomDeployer( CardinalDirection *direction, DungeonSquare *doorPosition, int roomSize, int seed, DungeonLevel *level ) : ALife( seed, level ) {
  heading=direction;
  position.Set(&doorPosition->coordinates);
  this->seed=seed;
  this->roomSize=roomSize;
 }
 bool Dead() {
  DungeonSquare *here=&level->squares(position.x,position.y,true);
  if ( here == &level->squares.outOfBounds || here->nearby.mapEdge ) return true;
  while ( here->traversible ) {
   position.x+=heading->vector.x;
   position.y+=heading->vector.y;
   here=&level->squares(position.x,position.y);
   if ( here == &level->squares.outOfBounds || here->nearby.mapEdge ) return true;
  }
  int proposedMagnitude=roomSize+(5-randoms.Upto(10,seed+roomSize,roomSize));
  if ( proposedMagnitude < MIN_SIZE ) return true;
  int availableMagnitude=level->squares.WallThickness(heading,position.x,position.y);
  if ( availableMagnitude < MIN_SIZE ) return true;
  int proposedBreadth=roomSize+(5-randoms.Upto(10,seed,roomSize));
  if ( proposedBreadth < MIN_SIZE ) return true;
  int proposedBreadth2=proposedBreadth/2;
  if ( availableMagnitude > proposedMagnitude ) {
   Cartesian point;
   for ( int i=0; i<proposedMagnitude; i++ ) {
    point.Set(position.x+i*heading->vector.x,position.y+i*heading->vector.y);
    int availableBreadthLeft=level->squares.WallThickness(heading->left,point.x,point.y);
    if ( availableBreadthLeft > MIN_SIZE/2 && availableBreadthLeft < proposedBreadth2 ) {
     proposedMagnitude=(int)((float)proposedMagnitude*iratiof(availableBreadthLeft,proposedBreadth2));
     proposedBreadth2=availableBreadthLeft;
     proposedBreadth=availableBreadthLeft*2;
    } else if ( availableBreadthLeft < proposedBreadth2 ) return true;
    int availableBreadthRight=level->squares.WallThickness(heading->right,point.x,point.y);
    if ( availableBreadthRight > MIN_SIZE/2 && availableBreadthRight < proposedBreadth2 ) {
     proposedMagnitude=(int)((float)proposedMagnitude*iratiof(availableBreadthRight,proposedBreadth2));
     proposedBreadth2=availableBreadthRight;
     proposedBreadth=availableBreadthRight*2;
    } else if ( availableBreadthRight < proposedBreadth2 ) return true;
   }
  } else return true;
  DungeonSquare *door;
  if ( heading == north ) {
   level->squares.RoomRectangle(position.x-proposedBreadth2,position.y-proposedMagnitude,proposedBreadth,proposedMagnitude,0,false,false);
   int prob=randoms.Upto(100,proposedBreadth2,seed);
   if ( prob < 33 ) door=&level->squares(position.x-1,position.y);
   else if ( prob < 66 ) door=&level->squares(position.x,position.y);
   else door=&level->squares(position.x+1,position.y);
   door->door.door=true;
  } else if ( heading == west ) {
   level->squares.RoomRectangle(position.x-proposedMagnitude,position.y-proposedBreadth2,proposedMagnitude,proposedBreadth,0,false,false);
   int prob=randoms.Upto(100,proposedMagnitude,seed);
   if ( prob < 33 ) door=&level->squares(position.x,position.y-1);
   else if ( prob < 66 ) door=&level->squares(position.x,position.y);
   else door=&level->squares(position.x,position.y+1);
   door->door.door=true;
  } else if ( heading == south ) {
   level->squares.RoomRectangle(position.x-proposedBreadth2,position.y,proposedBreadth,proposedMagnitude,0,false,false);
   int prob=randoms.Upto(100,proposedBreadth2,seed);
   if ( prob < 33 ) door=&level->squares(position.x-1,position.y);
   else if ( prob < 66 ) door=&level->squares(position.x,position.y);
   else door=&level->squares(position.x+1,position.y);
   door->door.door=true;
  } else if ( heading == east ) {
   level->squares.RoomRectangle(position.x,position.y-proposedBreadth2,proposedMagnitude,proposedBreadth,0,false,false);
   int prob=randoms.Upto(100,proposedMagnitude,seed);
   if ( prob < 33 ) door=&level->squares(position.x,position.y-1);
   else if ( prob < 66 ) door=&level->squares(position.x,position.y);
   else door=&level->squares(position.x+1,position.y+1);
   door->door.door=true;
  }
  return true;
 }
};

class DungeonHallwayMaker : public ALife {
public:
 double spawn;
 int steps;
 int radius;
 int roomSize;
 DungeonHallwayMaker( CardinalDirection *direction, int maxSteps, int roomSize, int seed, DungeonLevel *level ) : ALife( seed, level ) {
  steps=20+randoms.Upto(maxSteps,123,seed);
  heading=direction;
  this->roomSize=roomSize;
  spawn=0.1+(double) randoms.Unique(seed+steps,seed+12345)*0.5;
  radius=2;
 }
 bool Dead() {
  return --steps <=0;
 }
 void Birth() {
 }
 void Generation() {
  DungeonSquare *here=&level->squares(position.x,position.y,true);
  if ( here == &level->squares.outOfBounds ) {
   steps=0; return;
  }
  level->squares.Dig(here,radius,true,true);
  if ( (double) randoms.Unique(steps+seed,seed+position.x) < spawn ) {
   if ( (double) randoms.Unique(steps+position.x,seed+position.y) < 0.5 ) {
    DungeonSquare *there=&level->squares(position.x+heading->left->vector.x*radius,position.y+heading->left->vector.y*radius,true);
    if ( there != &level->squares.outOfBounds && !there->nearby.mapEdge ) {
     DungeonRoomDeployer *baby=new DungeonRoomDeployer(heading->left,there,roomSize,seed,level);
     level->alives.Append(baby);
    }
   } else {
    DungeonSquare *there=&level->squares(position.x-heading->right->vector.x*radius,position.y-heading->right->vector.y*radius,true);
    if ( there != &level->squares.outOfBounds && !there->nearby.mapEdge ) {
     DungeonRoomDeployer *baby=new DungeonRoomDeployer(heading->right,there,roomSize,seed,level);
     level->alives.Append(baby);
    }
   }
  }
  Move(heading,radius);
 }
};

class DungeonCaveSquiggler : public ALife {
public:
 Cartesians memory;
 bool suicidal,joined,penetrator;
 int width,length;
 double spin,ante,branch,joinPreference;
 int branches;
 int stepped;
 int minimumSteps;
 DungeonCaveSquiggler( int seed, DungeonLevel *level ) : ALife( seed, level ) {
  stepped=0;
  suicidal=!(seed%3==1);
  penetrator=!(seed%2==1);
  width=seed%3==1?1:(seed%2==1?1+set->cuban(seed,seed):1+seed%2);
  length=seed%3==1?1:(seed%2==1?1+set->cuban(seed,seed)%10:1+seed%2);
  spin=(double)set->unique(seed,seed,seed)*0.5;
  branch=(double)set->unique(seed,2,seed)*0.5;
  branches=seed%5;
  ante=(double)set->unique(seed,seed,3)*0.5;
  lifespan=seed%2==1?(level->w/4):(1+set->cuban(seed,seed)%(level->w*level->h/2));
  joined=false;
  joinPreference=(double)set->unique(3,3,seed)*0.5;
  die=false;
  minimumSteps=50;
 }
 bool Dead() {
 return minimumSteps <= 0 && ( die || NonTraversibleNeighbors(1)<2 );
 }
 void Birth() {
 }
 void Generation() {
  minimumSteps--;
  stepped++;
  DungeonSquare *here=&level->squares(position.x,position.y,true);
  if ( here == &level->squares.outOfBounds ) {
   minimumSteps=-1;
   die=true;
  } else
  if ( !heading || set->unique(seed,age,lifespan) < spin || here->nearby.mapEdge ) TurnLeftOrRight();
  else {
   if ( !here->traversible && !here->room ) {
    here->cave=true;
    here->traversible=true;
    if ( !here->nearby.mapEdge ) {
     here->nearby.up->traversible=true;
     here->nearby.up->cave=true;
     here->nearby.down->traversible=true;
     here->nearby.down->cave=true;
     here->nearby.left->traversible=true;
     here->nearby.left->cave=true;
     here->nearby.right->traversible=true;
     here->nearby.right->cave=true;
    }
   }
  }
  int tries=4;
  while ( !Move(heading) ) {
   tries--;
   TurnLeftOrRight();
   if ( tries <= 0 ) { die=true; return; }
  }
  if ( branches > 0 && set->unique(age,stepped,lifespan) < branch ) {
   Legacy();
   branches--;
  }
 }
 void Legacy() {
  DungeonCaveSquiggler *baby=new DungeonCaveSquiggler(seed+1,level);
  baby->minimumSteps=20;
  baby->position.Set(position.x,position.y);
  baby->ChangeDirection();
  baby->Move(heading);
  baby->branches=baby->branches>1?1:0;
  level->alives.Append(baby);
 }
};

// A stack of levels.
class Dungeon : public ListItem {
public:
 DungeonLevels levels;
 Dungeon( int w, int h, int lvls, int seed ) {
  for ( int i=0; i<lvls; i++ ) {
   DungeonLevel *level;
   levels.Append(level=new DungeonLevel(seed+i,w,h));
   switch ( seed % 24 ) {
    case 0: FourCorners(level); break;
    case 1: CrossFoyer(level);  break;
    case 2: RoomComplex(level); break;
    case 3: MultiMaze(level); break;
    case 4: AboveGroundComplex(level); break;
    case 5: CaveComplex(level); break;
    case 6: RandomPlant(level); break;
    case 7: UndergroundComplex(level); break;
    case 8: RandomCave(level); break;
    case 9: Cave(level); break;
    case 10: AlleyWays(level); break;
    case 11: Cavern(level); break;
    case 12: SuperComplex(level); break;
    case 13: CityRuins(level); break;
    case 14: CornedBeefHash(level); break;
    case 15: Smorgasbord(level); break;
    case 16: Caves(level); break;
    case 17: BSPWithinBSP(level); break;
    case 18: StampWorld(level); break;
    case 19: MacroMaze(level); break;
    case 20: MicroMaze(level); break;
    case 21: AnythingGoes(level); break;
    case 22: AlleyWays2(level); break;
    case 23: RandomRoguelike(level); break;
   }
   level->Generate();
   switch ( seed % 24 ) {
   default: /* no post */ break;
  //  case 0: FourCornersPost(level); break;
  //  case 1: CrossFoyerPost(level);  break;
  //  case 2: RoomComplexPost(level); break;
  //  case 3: MultiMazePost(level); break;
  //  case 4: AboveGroundComplexPost(level); break;
  //  case 5: CaveComplexPost(level); break;
  //  case 6: RandomPlantPost(level); break;
  //  case 7: UndergroundComplexPost(level); break;
  //  case 8: RandomCavePost(level); break;
    case 9: CavePost(level); break;
   }
  }
  levels.Post(seed);
 }
 void StampWorld( DungeonLevel *level ) {
  OUTPUT("StampWorld\n");
  int x=0,y=0,marcher=level->seed;
  int spacing=2+randoms.Upto(30,level->seed,level->seed+123);
  while ( x < level->w ) {
   int dx=0;
   while ( y < level->h ) {
    BitBrush *brush=(BitBrush *) bitbrushes.Any(marcher++);
    level->squares.Stamp(brush,x,y,1,level->seed);
    y+=brush->height.value;
    if ( dx < brush->width ) dx=brush->width;
   }
   y=0;
   x+=spacing+dx;
  }
  level->squares.SealMapEdges();
  level->squares.MarkSections( level->seed );
  level->squares.ConnectUnconnectedSections();
 }
 void SuperComplex( DungeonLevel *level ) {
  OUTPUT("SuperComplex\n");
  level->squares.SuperComplex(level->seed);
  level->squares.RectilinearBSPDungeon(level->seed,0,0,level->w,level->h,level->seed,false);
  level->squares.RectilinearBSPDungeon(level->seed,0,0,level->w,level->h,level->seed,false);
  level->squares.SealMapEdges();
  level->squares.MarkSections( level->seed );
  level->squares.ConnectUnconnectedSections();
 }
 void UndergroundComplex(DungeonLevel *level) {
  OUTPUT("UndergroundComplex\n");
  level->squares.RectilinearBSPDungeon(level->seed,0,0,level->w,level->h,level->seed,false);
  level->squares.MultiRoomDungeon(1,0,0,level->w,level->h,level->seed,true,false);
  level->squares.SealMapEdges();
  level->squares.MarkSections( level->seed );
  level->squares.ConnectUnconnectedSections();
  Cave(level);
 }
 void AboveGroundComplex(DungeonLevel *level) {
  OUTPUT("AboveGroundComplex\n");
  level->squares.MultiRoomDungeon(1,0,0,level->w,level->h,level->seed,true,true);
  level->squares.SealMapEdges();
  level->squares.MarkSections( level->seed );
  level->squares.ConnectUnconnectedSections();
 }
 void DualAboveGroundComplex(DungeonLevel *level) {
  OUTPUT("AboveGroundComplex\n");
  level->squares.MultiRoomDungeon(1,0,0,level->w,level->h,level->seed,true,true);
  level->squares.SealMapEdges();
  level->squares.MarkSections( level->seed );
  level->squares.ConnectUnconnectedSections();
 }
 void FourCorners( DungeonLevel *level ) {
  OUTPUT("FourCorners\n");
  int complexity=1+level->seed%10;
  for ( int i=0; i<10+complexity; i++ ) level->squares.RectilinearBSPDungeon(level->seed+i,0,0,level->w,level->h,level->seed,false);
  for ( int i=0; i<2; i++ )
   level->squares.RectilinearBSPDungeon(level->seed+i,
    randoms.Upto(level->w/2,level->seed,level->seed),
    randoms.Upto(level->w/2,level->seed,level->seed),
    level->w/2,level->h/2,level->seed,false
   );
  level->squares.SealMapEdges();
  level->squares.MarkSections( level->seed );
  level->squares.ConnectUnconnectedSections();
 }
 void BSPWithinBSP( DungeonLevel *level ) {
  OUTPUT("BSPWithinBSP\n");
  int complexity=level->seed%10;
  for ( int i=0; i<20+complexity; i++ ) level->squares.RectilinearBSPDungeon(level->seed+i,0,0,level->w,level->h,level->seed,false);
  for ( int i=0; i<5; i++ )
   level->squares.RectilinearBSPDungeon(level->seed+i,
    randoms.Upto(level->w/2,level->seed,level->seed),
    randoms.Upto(level->w/2,level->seed,level->seed),
    level->w/2,level->h/2,level->seed,false
   );
  level->squares.SealMapEdges();
  level->squares.MarkSections( level->seed );
  level->squares.ConnectUnconnectedSections();
 }
 void Caves( DungeonLevel *level ) {
  level->squares.CaveGrid( level->seed, 0, 0, level->w, level->h, 1+level->seed%6+level->seed%3, 1+level->seed%6, level->seed, false, false );
 }
 void Cave( DungeonLevel *level ) {
  OUTPUT("Cave\n");
  DungeonCaveSquiggler *t;
  for ( int i=0; i<(10+level->seed%20); i++ ) {
   level->alives.Append(t=new DungeonCaveSquiggler(level->seed+1111,level));
   t->minimumSteps=5*randoms.Upto(level->w+level->h,level->seed,123);
   t->position.Set(level->w/2,level->h/2);
   t->heading=level->seed%2==1?cardinals.find("w"):cardinals.find("n");
  }
  level->alives.Append(t=new DungeonCaveSquiggler(level->seed+1111,level));
  t->position.Set(level->w/2+1,level->h/2);
  t->heading=level->seed%2==1?cardinals.find("e"):cardinals.find("n");
  level->alives.Append(t=new DungeonCaveSquiggler(level->seed+1111,level));
  t->position.Set(level->w/2,level->h/2+1);
  t->heading=level->seed%2==1?cardinals.find("w"):cardinals.find("s");
  level->alives.Append(t=new DungeonCaveSquiggler(level->seed+1111,level));
  t->minimumSteps=level->w+level->h;
  t->position.Set(level->w/2+1,level->h/2+1);
  t->heading=level->seed%2==1?cardinals.find("e"):cardinals.find("s");
 }
 void CavePost( DungeonLevel *level ) {
  level->squares.SealMapEdges();
  level->squares.MarkSections( level->seed );
  level->squares.ConnectUnconnectedSections();
  level->squares.MarkSections( level->seed );
 }
 void CrossFoyer( DungeonLevel *level ) {
  OUTPUT("CrossFoyer\n");
  DungeonHallwayMaker *t;
  level->alives.Append(t=new DungeonHallwayMaker(north,50,20,level->seed+1111,level));
  t->position.Set(level->w/2+1,level->h/2);
  level->alives.Append(t=new DungeonHallwayMaker(south,50,20,level->seed+1111,level));
  t->position.Set(level->w/2+1,level->h/2);
  level->alives.Append(t=new DungeonHallwayMaker(east,50,20,level->seed+1111,level));
  t->position.Set(level->w/2+1,level->h/2);
  level->alives.Append(t=new DungeonHallwayMaker(west,50,20,level->seed+1111,level));
  t->position.Set(level->w/2+1,level->h/2);
 }
 void RandomCave( DungeonLevel *level ) {
  OUTPUT("RandomCave\n");
  DungeonCaveSquiggler *t;
  int complexity=(10+level->seed%20);
  for ( int i=0; i<complexity; i++ ) {
   level->alives.Append(t=new DungeonCaveSquiggler(level->seed+1111,level));
   t->minimumSteps=5*randoms.Upto(level->w+level->h,level->seed+i,123)%100;
   t->position.Set(randoms.Upto(level->w,level->w+i,i),randoms.Upto(level->h,level->w,level->seed));
   t->heading=level->seed%2==1?cardinals.find("w"):cardinals.find("n");
  }
  level->squares.SealMapEdges();
  level->squares.MarkSections( level->seed );
  level->squares.ConnectUnconnectedSections();
 }
 void CaveComplex( DungeonLevel *level ) {
  OUTPUT("CaveComplex\n");
  DungeonCaveSquiggler *t;
  for ( int i=0; i<(10+level->seed%20); i++ ) {
   level->alives.Append(t=new DungeonCaveSquiggler(level->seed+1111,level));
   t->minimumSteps=5*randoms.Upto(level->w+level->h,level->seed,123);
   t->position.Set(level->w/2,level->h/2);
   t->heading=level->seed%2==1?cardinals.find("w"):cardinals.find("n");
  }
  level->alives.Append(t=new DungeonCaveSquiggler(level->seed+1111,level));
  t->position.Set(level->w/2+1,level->h/2);
  t->heading=level->seed%2==1?cardinals.find("e"):cardinals.find("n");
  level->alives.Append(t=new DungeonCaveSquiggler(level->seed+1111,level));
  t->position.Set(level->w/2,level->h/2+1);
  t->heading=level->seed%2==1?cardinals.find("w"):cardinals.find("s");
  level->alives.Append(t=new DungeonCaveSquiggler(level->seed+1111,level));
  t->minimumSteps=level->w+level->h;
  t->position.Set(level->w/2+1,level->h/2+1);
  t->heading=level->seed%2==1?cardinals.find("e"):cardinals.find("s");
  level->squares.MazeGrid(1, 0, 0, level->w/5/2, level->h/5/2, 2, level->seed+1234, 1,1, 10 );
  for ( int i=0; i<30; i++ ) {
   level->squares.RoomRectangle(
    randoms.Upto(300,level->seed+i,level->seed),
    randoms.Upto(100,level->seed+i,i),
    5+randoms.Upto(50,level->seed+i,i),
    5+randoms.Upto(50,level->seed+i,level->seed),
    1+randoms.Upto(4,level->seed+i,i+level->seed),false,true);
  }
 }
 void RoomComplex( DungeonLevel *level ) {
  OUTPUT("RoomComplex\n");
  level->squares.Maze(1,0,0,80,80,1+(level->seed%10),level->seed);
  for ( int i=0; i<30; i++ ) {
   level->squares.RoomRectangle(
    randoms.Upto(300,level->seed+i,level->seed),
    randoms.Upto(100,level->seed+i,i),
    5+randoms.Upto(50,level->seed+i,i),
    5+randoms.Upto(50,level->seed+i,level->seed),
    1+randoms.Upto(4,level->seed+i,i+level->seed),false,true);
  }
  level->squares.RectilinearBSPDungeon( level->seed+412,0, level->h/2, level->w/2, level->h/2, level->seed, false );
  level->squares.SealMapEdges();
  level->squares.MarkSections( level->seed );
  level->squares.ConnectUnconnectedSections();
 }
 void MultiMaze( DungeonLevel *level ) {
  OUTPUT("MultiMaze\n");
  level->squares.MazeGrid(1, 0, 0, level->w/5/2, level->h/5/2, 2, level->seed+1234, 5,5, 10 );
  for ( int i=0; i<10; i++ ) {
   level->squares.RoomRectangle(
    randoms.Upto(level->w,level->seed+i,level->seed),
    randoms.Upto(level->h,level->seed+i,i),
    5+randoms.Upto(50,level->seed+i,i),
    5+randoms.Upto(50,level->seed+i,level->seed),
    1+randoms.Upto(4,level->seed+i,i+level->seed),false,true
   );
  }
  level->squares.RectilinearBSPDungeon( level->seed+412, 0, 0, level->w, level->h/5, level->seed, false );
  level->squares.RectilinearBSPDungeon( level->seed+4123, level->w-level->w/5, 0, level->w/5, level->h, level->seed, false );
  level->squares.RectilinearBSPDungeon( level->seed+412, level->w/5, level->h/5, level->w/3, level->h/3, level->seed, false );
  level->squares.RectilinearBSPDungeon( level->seed+2412, 0, level->h/2, level->w/2, level->h/2, level->seed, false );
  level->squares.SealMapEdges();
  level->squares.MarkSections( level->seed );
  level->squares.ConnectUnconnectedSections();
 }
 void AlleyWays( DungeonLevel *level ) {
  OUTPUT("AlleyWays\n");
  DungeonHallwayMaker *t;
  level->alives.Append(t=new DungeonHallwayMaker(north,10,40,level->seed+1211,level));
  t->position.Set(level->w/2+1,level->h/2);
  t->radius=1;
  level->alives.Append(t=new DungeonHallwayMaker(south,10,40,level->seed+1121,level));
  t->position.Set(level->w/2+1,level->h/2);
  t->radius=1+randoms.Upto(4,level->seed,level->seed);
  level->alives.Append(t=new DungeonHallwayMaker(east,10,40,level->seed+1112,level));
  t->position.Set(level->w/2+1,level->h/2);
  t->radius=1;
  level->alives.Append(t=new DungeonHallwayMaker(west,10,40,level->seed+1311,level));
  t->position.Set(level->w/2+1,level->h/2);
  t->radius=1+randoms.Upto(4,level->seed,level->seed);
  level->alives.Append(t=new DungeonHallwayMaker(north,1,20,level->seed+1131,level));
  t->position.Set(level->w/4+level->w/2,level->h/4);
  t->radius=1;
  level->alives.Append(t=new DungeonHallwayMaker(south,1,20,level->seed+1113,level));
  t->position.Set(level->w/4+level->w/2+1,level->h/2+level->h/4);
  t->radius=1;
  level->alives.Append(t=new DungeonHallwayMaker(east,1,20,level->seed+4111,level));
  t->position.Set(level->w/4,level->h/2+level->h/4);
  t->radius=1;
  level->alives.Append(t=new DungeonHallwayMaker(west,1,50,level->seed+1411,level));
  t->position.Set(level->w/4,level->h/2);
 }
 void AlleyWays2( DungeonLevel *level ) {
  OUTPUT("AlleyWays II\n");
  DungeonHallwayMaker *t;
  level->alives.Append(t=new DungeonHallwayMaker(north,randoms.Upto(30,level->seed,level->seed+2),randoms.Upto(100,level->seed+1,level->seed),level->seed+1211,level));
  t->position.Set(randoms.Upto(level->w,level->seed,level->seed+1),randoms.Upto(level->h,level->seed,level->seed+2));
  t->radius=randoms.Upto(4,level->seed,level->seed+3);
  level->alives.Append(t=new DungeonHallwayMaker(south,randoms.Upto(30,level->seed,level->seed+4),randoms.Upto(100,level->seed+2,level->seed),level->seed+1121,level));
  t->position.Set(randoms.Upto(level->w,level->seed,level->seed+4),randoms.Upto(level->h,level->seed,level->seed+5));
  t->radius=randoms.Upto(4,level->seed,level->seed+6);
  level->alives.Append(t=new DungeonHallwayMaker(east,randoms.Upto(30,level->seed,level->seed+6),randoms.Upto(100,level->seed+3,level->seed),level->seed+1112,level));
  t->position.Set(randoms.Upto(level->w,level->seed,level->seed+7),randoms.Upto(level->h,level->seed,level->seed+8));
  t->radius=randoms.Upto(4,level->seed,level->seed+9);
  level->alives.Append(t=new DungeonHallwayMaker(west,randoms.Upto(30,level->seed,level->seed+8),randoms.Upto(100,level->seed+4,level->seed),level->seed+1311,level));
  t->position.Set(randoms.Upto(level->w,level->seed,level->seed+10),randoms.Upto(level->h,level->seed,level->seed+11));
  t->radius=randoms.Upto(4,level->seed,level->seed+12);
  level->alives.Append(t=new DungeonHallwayMaker(north,randoms.Upto(30,level->seed,level->seed+12),randoms.Upto(30,level->seed,level->seed+2),level->seed+1131,level));
  t->position.Set(randoms.Upto(level->w,level->seed,level->seed+13),randoms.Upto(level->h,level->seed,level->seed+14));
  t->radius=randoms.Upto(4,level->seed,level->seed+15);
  level->alives.Append(t=new DungeonHallwayMaker(south,randoms.Upto(30,level->seed,level->seed+21),randoms.Upto(30,level->seed,level->seed+2),level->seed+1113,level));
  t->position.Set(randoms.Upto(level->w,level->seed,level->seed+16),randoms.Upto(level->h,level->seed,level->seed+17));
  t->radius=randoms.Upto(4,level->seed,level->seed+18);
  level->alives.Append(t=new DungeonHallwayMaker(east,randoms.Upto(30,level->seed,level->seed+22),randoms.Upto(30,level->seed,level->seed+2),level->seed+4111,level));
  t->position.Set(randoms.Upto(level->w,level->seed,level->seed+19),randoms.Upto(level->h,level->seed,level->seed+20));
  t->radius=randoms.Upto(4,level->seed,level->seed+21);
  level->alives.Append(t=new DungeonHallwayMaker(west,randoms.Upto(30,level->seed,level->seed+32),randoms.Upto(50,level->seed,level->seed+2),level->seed+1411,level));
  t->position.Set(randoms.Upto(level->w,level->seed,level->seed+22),randoms.Upto(level->h,level->seed,level->seed+23));
  t->radius=randoms.Upto(4,level->seed,level->seed+24);
 }
 void RandomPlant( DungeonLevel *level ) {
  OUTPUT("RandomPlant\n");
  level->squares.Maze(1,0,0,60,60,5,level->seed);
  level->squares.MazeWithTreasureRooms(2,0,400,50,10,10,4,level->seed+9,1,2,true);
  level->squares.Maze(3,75,175,20,20,1,1);
  level->squares.Maze(4,155,225,10,10,2,level->seed+2);
  level->squares.Maze(5,115,75,10,10,2,level->seed+3);
  level->squares.Maze(6,220,10,80,80,1,1);
  level->squares.Maze(7,20,10,10,4,2,1);
  level->squares.MazeGrid(50, 200, 200, 10, 10, 2, level->seed+1234, 5,5, 10 );
  level->squares.MazeWithTreasureRooms(8, 100,100,75,75,1,4,level->seed,3,5,true);
  level->squares.MazeWithTreasureRooms(9, 150,100,30,30,3,4,level->seed,3,5,true);
  for ( int i=0; i<30; i++ ) {
   level->squares.RoomRectangle(
    randoms.Upto(300,level->seed+i,level->seed),
    randoms.Upto(100,level->seed+i,i),
    5+randoms.Upto(50,level->seed+i,i),
    5+randoms.Upto(50,level->seed+i,level->seed),
    1+randoms.Upto(4,level->seed+i,i+level->seed),false,true);
  }
  level->squares.SealMapEdges();
  level->squares.MarkSections( level->seed );
  level->squares.ConnectUnconnectedSections();
 }
 void MacroMaze( DungeonLevel *level ) {
  OUTPUT("MacroMaze\n");
  level->squares.MazeGrid(1, 0, 0, level->w/2, level->h/2, 10, level->seed+1234, 2,2, 10 );
  level->squares.RectilinearBSPDungeon( level->seed+412, 0, 0, level->w, level->h/5, level->seed, false );
  level->squares.RectilinearBSPDungeon( level->seed+4123, level->w-level->w/5, 0, level->w/5, level->h, level->seed, false );
  level->squares.RectilinearBSPDungeon( level->seed+412, level->w/5, level->h/5, level->w/3, level->h/3, level->seed, false );
  level->squares.RectilinearBSPDungeon( level->seed+2412, 0, level->h/2, level->w/2, level->h/2, level->seed, false );
  level->squares.SealMapEdges();
  level->squares.MarkSections( level->seed );
  level->squares.ConnectUnconnectedSections();
 }
 void MicroMaze( DungeonLevel *level ) {
  OUTPUT("MicroMaze\n");
  level->squares.MazeGrid(1, 0, 0, level->w/5/2, level->h/5/2, 1, level->seed+1234, 25,25, 10 );
  level->squares.RectilinearBSPDungeon( level->seed+412, 0, 0, level->w, level->h/5, level->seed, false );
  level->squares.RectilinearBSPDungeon( level->seed+412, 0, 0, level->w, level->h/5, level->seed, false );
  level->squares.RectilinearBSPDungeon( level->seed+4123, level->w-level->w/5, 0, level->w/5, level->h, level->seed, false );
  level->squares.RectilinearBSPDungeon( level->seed+412, level->w/5, level->h/5, level->w/3, level->h/3, level->seed, false );
  level->squares.RectilinearBSPDungeon( level->seed+2412, 0, level->h/2, level->w/2, level->h/2, level->seed, false );
  level->squares.SealMapEdges();
  level->squares.MarkSections( level->seed );
  level->squares.ConnectUnconnectedSections();
 }
 void AnythingGoes( DungeonLevel *level ) {
  OUTPUT("AnythingGoes\n");
  int x=0,y=0,marcher=level->seed;
  int spacing=2+randoms.Upto(30,level->seed,level->seed+123);
  while ( x < level->w ) {
   int dx=0;
   while ( y < level->h ) {
    BitBrush *brush=(BitBrush *) bitbrushes.Any(marcher++);
    if ( randoms.Query(level->seed+x,level->seed+y) ) level->squares.Stamp(brush,x,y,1,level->seed+marcher);
    y+=brush->height.value;
    if ( dx < brush->width ) dx=brush->width;
   }
   y=0;
   x+=spacing+dx;
  }
  if ( randoms.Query(level->seed+x,level->seed+y) )
   level->squares.MazeGrid(1, 0, 0, level->w/5/2, level->h/5/2, 1, level->seed+1234, 25,25, 10 );
  if ( randoms.Query(level->seed,x) ) 
   level->squares.RectilinearBSPDungeon( level->seed+412, 0, 0, level->w, level->h/5, level->seed, false );
  if ( randoms.Query(y,level->seed) )
   level->squares.MazeGrid(1, 0, 0, level->w/2, level->h/2, 10, level->seed+1234, 2,2, 10 );
  if ( randoms.Query(level->seed,spacing) ) 
  {
   DungeonCaveSquiggler *t;
   for ( int i=0; i<spacing; i++ ) {
    level->alives.Append(t=new DungeonCaveSquiggler(level->seed+1111,level));
    t->minimumSteps=5*randoms.Upto(level->w+level->h,level->seed+i,123)%100;
    t->position.Set(randoms.Upto(level->w,level->w+i,i),randoms.Upto(level->h,level->w,level->seed));
    t->heading=level->seed%2==1?cardinals.find("w"):cardinals.find("n");
   }
  }
  if ( randoms.Query(level->seed,spacing+x) ) {
   DungeonHallwayMaker *t;
   if ( randoms.Query(level->seed,spacing+x+1) ) {
    level->alives.Append(t=new DungeonHallwayMaker(north,10,40,level->seed+1211,level));
    t->position.Set(level->w/2+1,level->h/2);
    t->radius=1;
   }
   if ( randoms.Query(level->seed,spacing+x+2) ) {
    level->alives.Append(t=new DungeonHallwayMaker(south,10,40,level->seed+1121,level));
    t->position.Set(level->w/2+1,level->h/2);
    t->radius=1+randoms.Upto(4,level->seed,level->seed);
   }
   if ( randoms.Query(level->seed,spacing+x+3) ) {
    level->alives.Append(t=new DungeonHallwayMaker(east,10,40,level->seed+1112,level));
    t->position.Set(level->w/2+1,level->h/2);
    t->radius=1;
   }
   if ( randoms.Query(level->seed,spacing+x+4) ) {
    level->alives.Append(t=new DungeonHallwayMaker(west,10,40,level->seed+1311,level));
    t->position.Set(level->w/2+1,level->h/2);
    t->radius=1+randoms.Upto(4,level->seed,level->seed);
   }
   if ( randoms.Query(level->seed,spacing+x+5) ) {
    level->alives.Append(t=new DungeonHallwayMaker(north,1,20,level->seed+1131,level));
    t->position.Set(level->w/4+level->w/2,level->h/4);
    t->radius=1;
   }
   if ( randoms.Query(level->seed,spacing+x+6) ) {
    level->alives.Append(t=new DungeonHallwayMaker(south,1,20,level->seed+1113,level));
    t->position.Set(level->w/4+level->w/2+1,level->h/2+level->h/4);
    t->radius=1;
   }
   if ( randoms.Query(level->seed,spacing+x+7) ) {
    level->alives.Append(t=new DungeonHallwayMaker(east,1,20,level->seed+4111,level));
    t->position.Set(level->w/4,level->h/2+level->h/4);
    t->radius=1;
   }
   if ( randoms.Query(level->seed,spacing+x+8) ) {
    level->alives.Append(t=new DungeonHallwayMaker(west,1,50,level->seed+1411,level));
    t->position.Set(level->w/4,level->h/2);
   }
  }
  if ( randoms.Query(level->seed+x+y,spacing+y) )
  level->squares.RectilinearBSPDungeon( level->seed+215, level->w/3, randoms.Upto(20,level->seed,level->seed)+5, level->w/5, level->h/2+level->h/3, level->seed, true );
  if ( randoms.Query(level->seed,level->seed) ) {
   int partitions=1+randoms.Upto(5,level->seed,level->seed);
   int sw=level->w/partitions;
   int sh=level->h/partitions;
   int k=0;
   for ( int i=0; i<partitions; i++ ) for ( int j=0; j<partitions; j++ ) if ( randoms.Query(i,j+level->seed) ) {
    int sx=i*sw;
    int sy=j*sh;
    switch ( (level->seed+k) % 7 ) {
     case 0: level->squares.RectilinearBSPDungeon( level->seed+i, sx,sy,sw,sh, level->seed+i+j+k, false ); break;
     case 1: level->squares.MazeWithTreasureRooms( level->seed+i+j, sx,sy,sw,sh, 2, 10, level->seed+i+j+k, 3, 4, i%2==1 ); break;
     case 2: level->squares.Maze( level->seed+k, sx, sy, sw, sh, 1+((level->seed+i+j)%3), level->seed+i+j+k ); break;
     case 3: level->squares.MultiRoomDungeon( level->seed+i, sx, sy, sw, sh, level->seed+i+j+k, false, false ); break;
     case 4: level->squares.MazeGrid( level->seed+j, sx, sy, sw, sh, 1+((level->seed+i)%3), level->seed+i+j+k, 1+((level->seed+k)%3), 1+((level->seed+j)%4), 1+((i+j)%10) );
     case 5: level->squares.Cavern( level->seed+j, sx, sy, sw, sh, level->seed+i+j+k, false, false ); break;
     case 6: level->squares.CaveGrid( level->seed+k, sx, sy, sw, sh, 1+(i+k)%3,1+(j+k)%3, level->seed+k+i+j, false, false ); break;
    }
    k++;
   }
  }
  level->squares.SealMapEdges();
  level->squares.MarkSections( level->seed );
  level->squares.ConnectUnconnectedSections();
 }
 void Cavern( DungeonLevel *level ) {
  OUTPUT("Cavern\n");
  int area=0;
  while ( area < (level->w*level->h)/4 ) {
   level->squares.Cavern(level->seed+area,0,0,level->w,level->h,level->seed,false,false);
   area = level->squares.TraversibleArea();
  }
 }
 void CityRuins( DungeonLevel *level ) {
  OUTPUT("CityRuins\n");
/*
  DungeonHallwayMaker *t;
  level->alives.Append(t=new DungeonHallwayMaker(north,10+randoms.Upto(30,level->seed,level->w+level->seed),10,level->seed+2111,level));
  t->position.Set(level->w/4+1,level->h-5);
  t->radius=2;
  level->alives.Append(t=new DungeonHallwayMaker(north,10+0+randoms.Upto(30,level->seed,level->seed),10+randoms.Upto(10,level->seed,level->seed),level->seed+2111,level));
  t->position.Set(level->w/4+1+level->w/2,level->h-5);
  t->radius=2;
  */
  level->squares.RectilinearBSPDungeon( level->seed+215, level->w/3, randoms.Upto(20,level->seed,level->seed)+5, level->w/5, level->h/2+level->h/3, level->seed, true );
  level->squares.RectilinearBSPDungeon( level->seed+25, level->w/3, randoms.Upto(20,level->seed,level->seed)+5, level->w/5, level->h/2+level->h/3, level->seed+123, true );
 }
 void CornedBeefHash( DungeonLevel *level ) {
  OUTPUT("CornedBeefHash\n");
  FourCorners(level);
  CrossFoyer(level);
  RoomComplex(level);
  MultiMaze(level);
  AboveGroundComplex(level);
  CaveComplex(level);
  RandomPlant(level);
  UndergroundComplex(level);
  RandomCave(level);
  Cave(level);
  AlleyWays(level);
  Cavern(level);
  SuperComplex(level);
  CityRuins(level);
 }
 void Smorgasbord( DungeonLevel *level ) {
  OUTPUT("Smorgasbord\n");
  int partitions=1+randoms.Upto(5,level->seed,level->seed);
  int sw=level->w/partitions;
  int sh=level->h/partitions;
  int k=0;
  for ( int i=0; i<partitions; i++ ) for ( int j=0; j<partitions; j++ ) {
   int sx=i*sw;
   int sy=j*sh;
   switch ( (level->seed+k) % 7 ) {
    case 0: level->squares.RectilinearBSPDungeon( level->seed+i, sx,sy,sw,sh, level->seed+i+j+k, false ); break;
    case 1: level->squares.MazeWithTreasureRooms( level->seed+i+j, sx,sy,sw,sh, 2, 10, level->seed+i+j+k, 3, 4, i%2==1 ); break;
    case 2: level->squares.Maze( level->seed+k, sx, sy, sw, sh, 1+((level->seed+i+j)%3), level->seed+i+j+k ); break;
    case 3: level->squares.MultiRoomDungeon( level->seed+i, sx, sy, sw, sh, level->seed+i+j+k, false, false ); break;
    case 4: level->squares.MazeGrid( level->seed+j, sx, sy, sw, sh, 1+((level->seed+i)%3), level->seed+i+j+k, 1+((level->seed+k)%3), 1+((level->seed+j)%4), 1+((i+j)%10) );
    case 5: level->squares.Cavern( level->seed+j, sx, sy, sw, sh, level->seed+i+j+k, false, false ); break;
    case 6: level->squares.CaveGrid( level->seed+k, sx, sy, sw, sh, 1+(i+k)%3,1+(j+k)%3, level->seed+k+i+j, false, false ); break;
   }
   k++;
  }
 }
 void RandomRoguelike( DungeonLevel *level ) {
  OUTPUT("RandomRoguelike\n");
  level->squares.Roguelike(1,level->seed%6,0,0,level->w,level->h,level->seed,false);
 }
 ~Dungeon() { levels.Clear(); }
};

class DungeonTest : public GLWindow {
 Dungeon *dungeon;
 int size;
 int seed;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  size=128;
  dungeon=new Dungeon(size+2,size+2,1,seed=22);
 }
 void Between() {
  if ( size != 32 && input->KeyDown(DX_1) ) {
   size=32;
   OUTPUT("Generate %dx%d dungeon for seed %d.\n",size,size,seed);
   delete dungeon;
   dungeon=new Dungeon(size+2,size+2,1,seed);
  } else
  if ( size != 64 && input->KeyDown(DX_2) ) {
   size=64;
   OUTPUT("Generate %dx%d dungeon for seed %d.\n",size,size,seed);
   delete dungeon;
   dungeon=new Dungeon(size+2,size+2,1,seed);
  } else
  if ( size != 128 && input->KeyDown(DX_3) ) {
   size=128;
   OUTPUT("Generate %dx%d dungeon for seed %d.\n",size,size,seed);
   delete dungeon;
   dungeon=new Dungeon(size+2,size+2,1,seed);
  } else
  if ( size != 256 && input->KeyDown(DX_4) ) {
   size=256;
   OUTPUT("Generate %dx%d dungeon for seed %d.\n",size,size,seed);
   delete dungeon;
   dungeon=new Dungeon(size+2,size+2,1,seed);
  }
  if ( input->KeyHit(DX_P) ) {
   seed+=20;
  }
 }
 void Render() {
  DungeonLevel *level=(DungeonLevel *) dungeon->levels.first;
  level->DrawMap();
  char buf[32];
  FORMAT(buf,32,"Size: %d Seed: %d", (int) size, (int) seed);
  Text(buf,10,display->hd-30.0,10.0,10.0,false);
 }
 void WheelUp() {
  seed++;
  OUTPUT("Generate %dx%d dungeon for seed %d.\n",size,size,seed);
  delete dungeon;
  dungeon=new Dungeon(size+2,size+2,1,seed);
 }
 void WheelDown() {
  seed--;
  seed=abs(seed);
  OUTPUT("Generate %dx%d dungeon for seed %d.\n",size,size,seed);
  delete dungeon;
  dungeon=new Dungeon(size+2,size+2,1,seed);
 }
};