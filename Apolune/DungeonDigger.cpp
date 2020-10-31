#include "DungeonDigger.h"

//Passes back a "BSP Dungeon" (room rectangle collection)

Cartesians *ErraticGrid( int gx, int gy, int w, int h ) {
 Cartesians *squares=new Cartesians;
 int dx=0;
 int dy=0;

 return squares;
}

CardinalDirection *north=null;
CardinalDirection *south=null;
CardinalDirection *east=null;
CardinalDirection *west=null;

CardinalDirections cardinals;

void ALifeHandles::Clear() {
 ALifeHandle *n;
 for ( ALifeHandle *p=(ALifeHandle *) first; p; p=n ) {
  n=(ALifeHandle *)(p->next);
  delete p;
 }
 first=last=null;
 count=0;
}

ALife::ALife() : ListItem() {
 alives=null;
 this->level=null;
 Constructor();
 set=randoms.GetSet(seed);
 Birth();
}

ALife::ALife( int seed, DungeonLevel *level ) : ListItem() {
 alives=null;
 this->level=level;
 Constructor();
 this->seed=seed;
 set=randoms.GetSet(seed);
 Birth();
}

bool ALife::TurnLeftOrRight( bool reversed ) {
 CardinalDirection *L=reversed?cardinals.Right(heading):cardinals.Left(heading);
 CardinalDirection *R=reversed?cardinals.Left(heading):cardinals.Right(heading);
 DungeonSquare *candidate=MovingTo(L,1);
 if ( candidate && (!candidate->gauranteed && !candidate->traversible && !candidate->impregnable ) ) {
  heading=L;
  return true;
 }
 else {
  candidate=MovingTo(R,1);
  if ( candidate && (!candidate->gauranteed && !candidate->traversible && !candidate->impregnable ) ) {
   heading=R;
   return true;
  }
 }
 return false;
}

int ALife::NonTraversibleNeighbors( int dist ) {
 int n=0;
 DungeonSquare *test=MovingTo(north,dist);
 if ( test && !test->traversible ) n++;
 test=MovingTo(south,dist);
 if ( test && !test->traversible ) n++;
 test=MovingTo(east,dist);
 if ( test && !test->traversible ) n++;
 test=MovingTo(west,dist);
 if ( test && !test->traversible ) n++;
 return n;
}

DungeonSquare *ALife::MovingTo( CardinalDirection *heading, int steps ) {
 if ( !heading ) return null;
 int dx=position.x+heading->vector.x*steps;
 int dy=position.y+heading->vector.y*steps;
 if ( dx < 0 || dy < 0 || dx >= level->squares.M() || dy >= level->squares.N() ) return null;
 return &level->squares(dx,dy);
}

DungeonSquare *ALife::MovingTo( int steps=1 ) {
 if ( !heading ) return null;
 int dx=position.x+heading->vector.x*steps;
 int dy=position.y+heading->vector.y*steps;
 if ( dx < 0 || dy < 0 || dx >= level->squares.M() || dy >= level->squares.N() ) return null;
 return &level->squares(dx,dy);
}

bool ALife::Move( CardinalDirection *s )  {
 int dx=position.x+s->vector.x;
 int dy=position.y+s->vector.y;
 if ( dx < 0 || dy < 0 || dx >= level->squares.M() || dy >= level->squares.N() ) return false;
 position.Set(dx,dy);
 level->Nearby( position.x, position.y, this );
 return true;
}

bool ALife::Move( CardinalDirection *s, int dist )  {
 int dx=position.x+s->vector.x*dist;
 int dy=position.y+s->vector.y*dist;
 if ( dx < 0 || dy < 0 || dx >= level->squares.M() || dy >= level->squares.N() ) return false;
 position.Set(dx,dy);
 level->Nearby( position.x, position.y, this );
 return true;
}

DungeonSquareNearby::DungeonSquareNearby() {
 mapEdge=false;
 here=topLeft=left=bottomLeft=topRight=right=bottomRight=up=down=null;
}

// Corresponds to CardinalDirections
void DungeonSquareNearby::Query( int x, int y, DungeonMap *map ) {
 here=&map->index(x,y,true);
 topLeft=&map->index(x-1,y-1,true);
 left=&map->index(x-1,y,true);
 bottomLeft=&map->index(x-1,y+1,true);
 topRight=&map->index(x+1,y-1,true);
 right=&map->index(x+1,y,true);
 bottomRight=&map->index(x+1,y+1,true);
 up=&map->index(x,y-1,true);
 down=&map->index(x,y+1,true);
 Completeness(map);
}

void DungeonSquareNearby::Completeness( DungeonMap *map ) {
 if ( here == &map->outOfBounds ) here=null;
 if ( topLeft == &map->outOfBounds ) topLeft=null;
 if ( left == &map->outOfBounds ) left=null;
 if ( bottomLeft == &map->outOfBounds ) bottomLeft=null;
 if ( topRight == &map->outOfBounds ) topRight=null;
 if ( right == &map->outOfBounds ) right=null;
 if ( bottomRight == &map->outOfBounds ) bottomRight=null;
 if ( up == &map->outOfBounds ) up=null;
 if ( down == &map->outOfBounds ) down=null;
 mapEdge= ( !up || !down || !left || !right );
}

bool DungeonSquareNearby::Pillar() {
 return ( !here->traversible
  && (left->traversible && right->traversible && up->traversible && down->traversible )
  && (topLeft->traversible && bottomRight->traversible && bottomLeft->traversible && topRight->traversible)
 );
}

bool DungeonSquareNearby::MoveLeftAndRightOrUpAndDown() {
 return  ( !up->traversible && !down->traversible && left->traversible && right->traversible )
      || ( up->traversible && down->traversible && !left->traversible && !right->traversible );
}

bool DungeonSquareNearby::SurroundedSolidBySolid() {
 return ( !here->traversible
  && (!left->traversible && !right->traversible && !up->traversible && !down->traversible )
  && (!topLeft->traversible && !bottomRight->traversible && !bottomLeft->traversible && !topRight->traversible)
 );
}

bool DungeonSquareNearby::Enclosed() {
 return ( 
     (!left->traversible && !right->traversible && !up->traversible && !down->traversible )
  && (!topLeft->traversible && !bottomRight->traversible && !bottomLeft->traversible && !topRight->traversible)
 );
}

bool DungeonSquareNearby::Open() {
 return (
     (left->traversible && right->traversible && up->traversible && down->traversible )
  && (topLeft->traversible && bottomRight->traversible && bottomLeft->traversible && topRight->traversible)
 );
}

bool DungeonSquareNearby::Plus() {
 return ( !here->traversible
  && (!left->traversible && !right->traversible && !up->traversible && !down->traversible )
  && (topLeft->traversible && bottomRight->traversible && bottomLeft->traversible && topRight->traversible)
 );
}

bool DungeonSquareNearby::HorizontalStraightWay( int NTNCN ) {
 if ( !here->traversible && NTNCN == 2 ) {
  if ( !left->traversible && !right->traversible ) return true;
 }
 return false;
}

bool DungeonSquareNearby::VerticalStraightWay( int NTNCN ) {
 if ( !here->traversible && NTNCN == 2 ) {
  if ( !up->traversible && !down->traversible ) return true;
 }
 return false;
}

bool DungeonSquareNearby::HorizontalHall( int NTNCN ) {
 if ( here->traversible && NTNCN == 6 ) {
  if ( !up->traversible && !down->traversible && left->traversible && right->traversible ) return true;
 }
 return false;
}

bool DungeonSquareNearby::VerticalHall( int NTNCN ) {
 if ( here->traversible && NTNCN == 6 ) {
  if ( !left->traversible && !right->traversible && up->traversible && down->traversible ) return true;
 }
 return false;
}

bool DungeonSquareNearby::CShaped( int NTNCN ) {
 return ( here->traversible && NTNCN == 7
  && ( left->traversible || right->traversible || up->traversible || down->traversible )
 );
}

bool DungeonSquareNearby::PartialC( int NTNCN, bool isHallway ) {
 return (!isHallway && NTNCN == 6);
}

int DungeonSquareNearby::TraversibleNeighbors() { return 4-NonTraversibleNeighbors(); }

int DungeonSquareNearby::NonTraversibleNeighbors() {
 int n=0;
 if ( up && !up->traversible ) n++;
 if ( down && !down->traversible ) n++;
 if ( left && !left->traversible ) n++;
 if ( right && !right->traversible ) n++;
 return n;
}

int DungeonSquareNearby::NonTraversibleNonCardinalNeighbors() {
 int n=0;
 if ( up && !up->traversible ) n++;
 if ( down && !down->traversible ) n++;
 if ( left && !left->traversible ) n++;
 if ( right && !right->traversible ) n++;
 if ( topLeft && !topLeft->traversible ) n++;
 if ( topRight && !topRight->traversible ) n++;
 if ( bottomLeft && !bottomLeft->traversible ) n++;
 if ( bottomRight && !bottomRight->traversible ) n++;
 return n;
}

int DungeonSquareNearby::NonTraversibleNonCardinalNeighbors( int NTN ) {
 int n=NTN;
 if ( topLeft && !topLeft->traversible ) n++;
 if ( topRight && !topRight->traversible ) n++;
 if ( bottomLeft && !bottomLeft->traversible ) n++;
 if ( bottomRight && !bottomRight->traversible ) n++;
 return n;
}

int DungeonSquareNearby::Doors() {
 int doors=0;
 if ( topLeft->door.door ) doors++;
 if ( topRight->door.door ) doors++;
 if ( bottomLeft->door.door ) doors++;
 if ( bottomRight->door.door ) doors++;
 if ( left->door.door ) doors++;
 if ( right->door.door ) doors++;
 if ( up->door.door ) doors++;
 if ( down->door.door ) doors++;
 return doors;
}

bool DungeonSquareNearby::LeadsNowhere() { // Leads to the same continguous sector (a door with no purpose), called when door.door
 MapSectionInfo *section=null;
 DungeonSquare *from_b,*to_b;
 here->Leading(&from_b,&to_b);
 if ( !from_b || !to_b || !from_b->section || !to_b->section || to_b->section==from_b->section ) return true;
 /*
      if ( up->section != null ) section=up->section;
 else if ( down->section != null ) section=down->section;
 else if ( left->section != null ) section=left->section;
 else if ( right->section != null ) section=right->section;
 else if ( topLeft->section != null ) section=topLeft->section;
 else if ( bottomRight->section != null ) section=bottomRight->section;
 else if ( bottomLeft->section != null ) section=bottomLeft->section;
 else if ( topRight->section != null ) section=topRight->section;
 if (  
     ( up->section == null          || up->section == section )
  && ( down->section == null        || down->section == section )
  && ( left->section == null        || left->section == section )
  && ( right->section == null       || right->section == section )
  && ( topLeft->section == null     || topLeft->section == section )
  && ( topRight->section == null    || topRight->section == section )
  && ( bottomLeft->section == null  || bottomLeft->section == section )
  && ( bottomRight->section == null || bottomRight->section == section )
    ) return true;
  */
 /*
 DungeonSquare *from_a,*to_a;
 if ( up->door.door ) {
  up->Leading(&from_a,&to_a);
  if ( from_a && from_b && from_a->section && from_b->section && to_a->section && to_b->section ) {
   MapSectionInfoHandles sections;
   sections.AddIfUnique(from_a->section);
   sections.AddIfUnique(to_a->section);
   sections.AddIfUnique(from_b->section);
   sections.AddIfUnique(to_b->section);
   if ( sections.count > 2 ) return true;
  }
 } else if ( left->door.door ) {
  left->Leading(&from_a,&to_a);
  if ( from_a && from_b && from_a->section && from_b->section && to_a->section && to_b->section ) {
   MapSectionInfoHandles sections;
   sections.AddIfUnique(from_a->section);
   sections.AddIfUnique(to_a->section);
   sections.AddIfUnique(from_b->section);
   sections.AddIfUnique(to_b->section);
   if ( sections.count > 2 ) return true;
  }
 } else if ( right->door.door ) {
  right->Leading(&from_a,&to_a);
  if ( from_a && from_b && from_a->section && from_b->section && to_a->section && to_b->section ) {
   MapSectionInfoHandles sections;
   sections.AddIfUnique(from_a->section);
   sections.AddIfUnique(to_a->section);
   sections.AddIfUnique(from_b->section);
   sections.AddIfUnique(to_b->section);
   if ( sections.count > 2 ) return true;
  }
 } else if ( down->door.door ) {
  down->Leading(&from_a,&to_a);
  if ( from_a && from_b && from_a->section && from_b->section && to_a->section && to_b->section ) {
   MapSectionInfoHandles sections;
   sections.AddIfUnique(from_a->section);
   sections.AddIfUnique(to_a->section);
   sections.AddIfUnique(from_b->section);
   sections.AddIfUnique(to_b->section);
   if ( sections.count > 2 ) return true;
  }
 }
 */
 return false;
}

bool DungeonSquareNearby::UpWall( int NTNCN ) {
 return ( !here->traversible && NTNCN == 5
   && ( down->traversible && bottomLeft->traversible && bottomRight->traversible )
 );
}

bool DungeonSquareNearby::DownWall( int NTNCN ) {
 return ( !here->traversible && NTNCN == 5
   && ( up->traversible && topLeft->traversible && topRight->traversible )
 );
}

bool DungeonSquareNearby::LeftWall( int NTNCN ) {
 return ( !here->traversible && NTNCN == 5
   && ( right->traversible && topRight->traversible && bottomRight->traversible )
 );
}

bool DungeonSquareNearby::RightWall( int NTNCN ) {
 return ( !here->traversible && NTNCN == 5
   && ( left->traversible && bottomLeft->traversible && topLeft->traversible )
 );
}

bool DungeonSquareNearby::NearUpWall() {
 return ( here->traversible && left->traversible && right->traversible
   && ( !up->traversible && !topLeft->traversible && !topRight->traversible )
 );
}

bool DungeonSquareNearby::NearDownWall() {
 return ( here->traversible && left->traversible && right->traversible
   && ( !down->traversible && !bottomLeft->traversible && !bottomRight->traversible )
 );
}

bool DungeonSquareNearby::NearLeftWall() {
 return ( here->traversible && up->traversible && down->traversible
   && ( !left->traversible && !topLeft->traversible && !bottomLeft->traversible )
 );
}

bool DungeonSquareNearby::NearRightWall() {
 return ( here->traversible && up->traversible && down->traversible
   && ( !right->traversible && !bottomRight->traversible && !topRight->traversible )
 );
}

bool DungeonSquareNearby::WallBreakUpRight() {
 MapSectionInfo *a;
 MapSectionInfo *b;
 if ( here->door.door || here->traversible 
   || topRight->door.door || topRight->traversible ) return false;
 if ( up->traversible ) a=up->section;
 else return false;
 if ( right->traversible ) b=right->section;
 else return false;
 if ( a == b ) return false;
 return true;
}

bool DungeonSquareNearby::WallBreakDownLeft() {
 MapSectionInfo *a;
 MapSectionInfo *b;
 if ( here->door.door || here->traversible 
   || bottomLeft->door.door || bottomLeft->traversible ) return false;
 if ( down->traversible ) a=down->section;
 else return false;
 if ( left->traversible ) b=left->section;
 else return false;
 if ( a == b ) return false;
 return true;
}

bool DungeonSquareNearby::WallBreakDownRight() {
 MapSectionInfo *a;
 MapSectionInfo *b;
 if ( here->door.door || here->traversible 
   || bottomRight->door.door || bottomRight->traversible ) return false;
 if ( down->traversible ) a=down->section;
 else return false;
 if ( right->traversible ) b=right->section;
 else return false;
 if ( a == b ) return false;
 return true;
}

bool DungeonSquareNearby::WallBreakUpLeft() {
 MapSectionInfo *a;
 MapSectionInfo *b;
 if ( here->door.door || here->traversible 
   || topLeft->door.door || topLeft->traversible ) return false;
 if ( up->traversible ) a=up->section;
 else return false;
 if ( left->traversible ) b=left->section;
 else return false;
 if ( a == b ) return false;
 return true;
}

void DungeonMap::Stamp( BitBrush *stamp, int x, int y, int copyScale, int seed, DungeonLevel *level ) {
  for ( int i=0; i<stamp->width; i++ ) for ( int j=0; j<stamp->height; j++ ) {
   for ( int k=0; k<copyScale; k++ ) for ( int l=0; l<copyScale; l++ ) {
    int
     sqx=x+i*copyScale+k,
     sqy=y+j*copyScale+l;
    DungeonSquare *here=&index(sqx,sqy,true);
    BitBrushMode brush=stamp->brush(i,j);
    if ( here != &outOfBounds && !here->gauranteed ) {
     if ( brush != bb_blank ) {
      here->traversible=true;
      switch ( brush ) {
       case bb_solid:
         here->traversible=false;
         here->impregnable=true;
        break;
       case bb_opaque:         
         here->door.door=true;
         here->traversible=false;
         // Note: in a later pass you could find this door and use A* to tunnel, so you could mark it needsAStar etc
        break;
       case bb_transparent:
        if ( level ) { // User requested a "room deployer" to be deployed here.
         CardinalDirection *direction;
         if ( i>0 && stamp->brush(i-1,j) == bb_blank ) direction=west;
         else if ( j>0 && stamp->brush(i,j-1) == bb_blank ) direction=north;
         else if ( i<stamp->width-1 && stamp->brush(i+1,j) == bb_blank ) direction=east;
         else if ( j<stamp->height-1 && stamp->brush(i,j+1) == bb_blank ) direction=south;
         else direction=null;
         if ( direction ) {
          DungeonSquare *there=&index(sqx+direction->vector.x,sqy+direction->vector.y,true);
          if ( there != &outOfBounds && !here->traversible && !here->gauranteed ) {
           here->door.door=true;
           here->traversible=false;
           DungeonRoomDeployer *t=new DungeonRoomDeployer(direction,here,stamp->height/(3+randoms.Upto(3,seed,i+j+k)),seed+j+k+i+l,level);
           level->alives.Append(t);
          }
         }
        }
        break;
       default: break;
      }
      here->corner=(brush == bb_fortyfive);
      here->outside=(brush == bb_outside);
      if ( stamp->brush(i,j) == bb_opaque ) {
      }
      here->stamp=stamp;
     }
    }
   }
  }
 }

void MapSectionInfo::Calculate( DungeonMap *map ) {
 int lowx=100000, lowy=100000, hix=0, hiy=0;
 EACH(spaces.first,Cartesian,c) {
  if ( lowx > c->x ) lowx=c->x;
  if ( lowy > c->y ) lowy=c->y;
  if ( hix < c->x ) hix=c->x;
  if ( hiy < c->y ) hiy=c->y;
 }
 extent.Corners(lowx,lowy,hix,hiy);
 // Attempt to mark all edges where a door can be placed
 DungeonSquare *here;
 EACH(walls.first,Cartesian,c) {
  here=&map->index(c->x,c->y);
  int NTN=here->nearby.NonTraversibleNeighbors();
  int NTNCN=here->nearby.NonTraversibleNonCardinalNeighbors(NTN);
  if ( here != &map->outOfBounds && !here->nearby.mapEdge ) {
   if ( here->nearby.HorizontalStraightWay(NTNCN) ) {
    if ( here->nearby.left->section != here->nearby.down->section ) {
     here->edge=true;
    }
   } else if ( here->nearby.VerticalStraightWay(NTNCN) ) {
    if ( here->nearby.up->section != here->nearby.down->section ) {
     here->edge=true;
    }
   } else {
    if ( here->nearby.LeftWall(NTNCN) ) {
     // Walk to the left until we hit a space, or the map edge.  If we hit a map edge, stop attempting to dig.
     static Cartesian direction; direction.Set(-1,0);
     DungeonSquare *adjacent=here->nearby.left;
     while ( adjacent != &map->outOfBounds && !adjacent->traversible ) {
      adjacent=&map->index(adjacent->coordinates.x+direction.x,adjacent->coordinates.y+direction.y,true);
     }
     if ( adjacent != &map->outOfBounds && adjacent->traversible && adjacent->section != here->section ) here->edge=true;
    } else if ( here->nearby.RightWall(NTNCN) ) {
     // Walk to the right until we hit a space, or the map edge.  If we hit a map edge, stop attempting to dig.
     static Cartesian direction; direction.Set(1,0);
     DungeonSquare *adjacent=here->nearby.right;
     while ( adjacent != &map->outOfBounds && !adjacent->traversible ) {
      adjacent=&map->index(adjacent->coordinates.x+direction.x,adjacent->coordinates.y+direction.y,true);
     }
     if ( adjacent != &map->outOfBounds && adjacent->traversible && adjacent->section != here->section ) here->edge=true;
    } else if ( here->nearby.UpWall(NTNCN) )  {
     // Walk up until we hit a space, or the map edge.  If we hit a map edge, stop attempting to dig.
     static Cartesian direction; direction.Set(0,-1);
     DungeonSquare *adjacent=here->nearby.up;
     while ( adjacent != &map->outOfBounds && !adjacent->traversible ) {
      adjacent=&map->index(adjacent->coordinates.x+direction.x,adjacent->coordinates.y+direction.y,true);
     }
     if ( adjacent != &map->outOfBounds && adjacent->traversible && adjacent->section != here->section ) here->edge=true;
    } else if ( here->nearby.DownWall(NTNCN) ) {
     // Walk down until we hit a space, or the map edge.  If we hit a map edge, stop attempting to dig.
     static Cartesian direction; direction.Set(0,1);
     DungeonSquare *adjacent=here->nearby.down;
     while ( adjacent != &map->outOfBounds && !adjacent->traversible ) {
      adjacent=&map->index(adjacent->coordinates.x+direction.x,adjacent->coordinates.y+direction.y,true);
     }
     if ( adjacent != &map->outOfBounds && adjacent->traversible && adjacent->section != here->section ) {
      here->edge=true;
     }
    }
   }
  }
 }
}

// Clean up strange doors.
// Makes sure stairwells can be connected
 void DungeonLevel::Post( DungeonLevels *levels ) {
  squares.SealMapEdges();
  squares.MarkSections( seed );
  squares.ConnectUnconnectedSections();
  squares.RemoveStrangeDoors( seed );
  squares.MarkSections( seed );
  squares.ConnectUnconnectedSections();
  squares.MarkSections( seed );
  squares.ConnectUnconnectedSections();
  squares.MarkSections( seed );
  squares.FillCaddyCornerHoles( seed );
  squares.RemoveStrangeDoors( seed );
  squares.MarkSections( seed );
  squares.RemoveSingleRoomSections();
  squares.UpdateAStarMap(false);
  squares.ConnectOrphanedSectionsWithSecrets();
  squares.MarkSections( seed );
 }