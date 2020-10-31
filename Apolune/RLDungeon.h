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
//////////////////////////////////////////////////////////////////////////
// Roguelikelib 0.4
//////////////////////////////////////////////////////////////////////////
#pragma once
#pragma warning (disable:4267) 

#include <limits>
#include <vector>
#include <iostream>
#include <list>

#include "Randoms.h"

enum RLEDirection {
 RLEDirectionMin=0,
 RLNorth=RLEDirectionMin,
 RLNorthEast,
 RLEast,
 RLSouthEast,
 RLSouth,
 RLSouthWest,
 RLWest,
 RLNorthWest,
 RLEDirectionMax=RLNorthWest
};

class RLDirection {
private:
 RLEDirection direction;
public:
 RLDirection():direction(RLNorth) {};
 RLDirection(RLEDirection start):direction(start) {};
 inline RLEDirection Get() { return direction; };
 inline void Set(RLEDirection to_set) { direction=to_set; };
 RLEDirection Incrase() { 
  direction = (( direction < RLEDirectionMax ) ? (RLEDirection)( (int)direction + 1 ) : RLEDirectionMin) ;
  return direction; 
 }
 RLEDirection Decrase() { 
  direction = (( direction > RLEDirectionMin ) ? (RLEDirection)( (int)direction - 1 ) : RLEDirectionMax ) ;
  return direction; 
 }
};

RLEDirection RandomDirection( int seed );

struct RLPosition {
 int x;
 int y;
 RLPosition() { x=-1; y=-1; };
 RLPosition(int a_x, int a_y):x(a_x),y(a_y) {};
 RLPosition(size_t a_x, size_t a_y):x(static_cast<int>(a_x)),y(static_cast<int>(a_y)) {};
 const RLPosition& operator+=(const RLPosition& r) {
  x+=r.x;
  y+=r.y;
  return *this;
 }
 const RLPosition& operator-=(const RLPosition& r) {
  x-=r.x;
  y-=r.y;
  return *this;
 }	
 bool operator==(const RLPosition& r) {	return (x==r.x && y==r.y); }
 bool operator!=(const RLPosition& r) { return !operator==(r); }
};

typedef RLPosition RLSize; // alias name

enum RLELevelElement {
 RLLevelElementWall='#',
 RLLevelElementCorridor='.',
 RLLevelElementGrass='"',
 RLLevelElementPlant='&',
 RLLevelElementRoom=',',
 RLLevelElementDoorClose='+',
 RLLevelElementDoorOpen='/',
 RLLevelElementWater='~',
 RLLevelElementCorridor_value=INT_MAX-2, // Some algorithms (like pathfinding) needs values instead of tiles
 RLLevelElementRoom_value=INT_MAX-1,
 RLLevelElementWall_value=INT_MAX
};

class RLCMap {
private:
	std::vector <int> m_map;
	RLSize size;
public:
	void Resize(int width,int height)
	{
		if (width<=0 || height<=0)
			return;
		size.x  = width;
		size.y = height;

		m_map.resize(size.x*size.y);			
	};
	void Resize(RLSize size)
	{
		return Resize(size.x,size.y);
	};

	unsigned int GetWidth()  { return size.x; };
	unsigned int GetHeight() { return size.y; };

	void Clear(int filler)
	{
		if (m_map.size()==0)
			return;

		for (int x=0;x<size.x;++x)
			for (int y=0;y<size.y;++y)
				m_map[x*size.y+y]=filler;
	}
	void Clear(RLELevelElement filler=RLLevelElementWall)
	{
		if (m_map.size()==0)
			return;

		for (int x=0;x<size.x;++x)
			for (int y=0;y<size.y;++y)
				m_map[x*size.y+y]=filler;
	}

	bool OnMap(const int& x, const int& y)
	{ 
		return (x>=0 && x<(int) size.x && y>=0 && y<(int) size.y); 
	};
	bool OnMap(const RLPosition &pos)
	{ 
		return OnMap(pos.x,pos.y);
	};

	void SetCell(const int& x, const int& y, RLELevelElement element)
	{ 
		if (OnMap(x,y))
			m_map[x*size.y+y]=element;
	};
	void SetCell(const int& x, const int& y, int element)
	{ 
		if (OnMap(x,y))
			m_map[x*size.y+y]=element;
	};
	void SetCell(const RLPosition &pos, int element)
	{ 
		return SetCell(pos.x,pos.y,element);
	};

	int GetCell(const int& x, const int& y)
	{ 
		if (OnMap(x,y))
			return m_map[x*size.y+y];
		else
			return -1;
	};

	int GetCell(const RLPosition &pos)
	{ 
		return GetCell(pos.x,pos.y);
	};

	/*void PrintMap()
	{
		for (int y=0;y<size.y;++y)
		{
			for (int x=0;x<size.x;++x)
				std::cout << static_cast <char> ((int) GetCell(x,y));
			std::cout << std::endl;
		}
	}*/
};

struct RLSRoom {		
 RLPosition corner1, corner2;
 Zint type;
 bool IsInRoom(const RLPosition &pos) { return (pos.x>=corner1.x && pos.x<=corner2.x && pos.y>=corner1.y && pos.y<=corner2.y); };
 bool IsInRoom(const int x, const int y) { return (x>=corner1.x && x<=corner2.x && y>=corner1.y && y<=corner2.y); };
};

class RLDungeon {
public:
    void CreateStandardDungeon(RLCMap &level,int seed,int max_number_of_rooms=10,bool with_doors=true) {
		if (level.GetWidth()==0 || level.GetHeight()==0) return;
		level.Clear();
		RLPosition p;
		RLPosition room;
		RLSize room_size;
		// place rooms
		for (int room_number=0;room_number<max_number_of_rooms;++room_number) {
			// size of room
			room_size.x = randoms.Upto(5,seed+1,seed)+8;
			room_size.y = randoms.Upto(5,seed,seed+1)+5;		
			if (FindOnMapRandomRectangleOfType(level, seed+room_number*3, RLLevelElementWall, p, room_size))
			{
				for (int x=1;x<room_size.x-1;x++)
					for (int y=1;y<room_size.y-1;y++)
						level.SetCell(p.x+x,p.y+y, RLLevelElementRoom);
			}
		}

		ConnectClosestRooms(level,true,true); // changes tiles to values
		ConvertValuesToTiles(level);
		if (with_doors)
			AddDoors(level,seed,1,0.5);
	}

    
	void CreateSpaceShuttle(RLCMap &level, int seed,
     const int& max_number_of_rooms=15, 
     bool mirror_vertical=false,
     bool rooms_the_same=true)
	{
		const int room_min_size=3;
		const int room_max_size=15;

		int number_of_rooms;
		int free_cells;

		std::list < RLSRoom > list_of_rooms;
		std::list < RLSRoom >::iterator m,_m;
		int x,y;

		int x1,y1,x2,y2, rx, ry;

        int marcher=seed+122;
        int tries=0;

		while(tries < 30000) // create until shuttle looks good
		{
            marcher++;
			list_of_rooms.clear();

			// fill with walls
			for (x=0;x<(int) level.GetWidth();++x)
				for (y=0;y<(int) level.GetHeight();++y)
					level.SetCell(x,y,RLLevelElementWall_value);


			for (number_of_rooms=0;number_of_rooms<max_number_of_rooms;)
			{
                marcher++;
				if (number_of_rooms==0)
				{
					x1 = (int) level.GetWidth()/2 - randoms.Upto(room_max_size,marcher+seed+1,seed+1);
					y1 = (int) level.GetHeight()/2 - randoms.Upto(room_max_size,marcher+seed+1,seed+2) - room_min_size;
					rx = randoms.Upto(room_max_size,marcher+seed+2,seed+3) + room_min_size;
					ry = randoms.Upto(room_max_size - room_min_size,seed+4,marcher+seed+5) + room_min_size;
					x2 = x1+rx;
					y2 = (int) level.GetHeight()/2;
					if (x2 >= (int) level.GetWidth()) continue;
				}
				else
				{
					x1 = randoms.Upto((int) level.GetWidth() - room_min_size,marcher+seed+number_of_rooms,seed+1) + 1;
					y1 = randoms.Upto((int) level.GetHeight() - room_min_size,marcher+seed+number_of_rooms,seed+2)/2 + 1;
					rx = randoms.Upto(room_max_size - room_min_size,marcher+seed+number_of_rooms,seed+3) + room_min_size;
					ry = randoms.Upto(room_max_size - room_min_size,seed+number_of_rooms,marcher+seed+4) + room_min_size;
					x2 = x1+rx;
					y2 = y1+ry;

					if (x2 >= (int) level.GetWidth()-1 || y2 >= (int) level.GetHeight()/2 + 3)
						continue;
				}

				// is in existing room?
				bool rand_again= false;
				for (m = list_of_rooms.begin(), _m = list_of_rooms.end();m!=_m;m++)
				{
                    marcher++;
					RLSRoom &room = *m;
					rand_again = true;
					if (room.IsInRoom(x1,y1))
					{
						if (!room.IsInRoom(x2,y2))
						{
							rand_again = false;
							room.type++;
						}
						break;
					}
					if (room.IsInRoom(x2,y2))
					{
						if (!room.IsInRoom(x1,y1))
						{
							rand_again = false;
							room.type++;
						}
						break;
					}
				}
                tries++;
				if (rand_again && tries < 100)
					continue;

				// Create room
				RLSRoom new_room;
				new_room.corner1.x = x1;
				new_room.corner1.y = y1;
				new_room.corner2.x = x2;
				new_room.corner2.y = y2;
				if (number_of_rooms == 0)
					new_room.type = 0;

				list_of_rooms.push_back(new_room);
				number_of_rooms++;

			} // end of for
            marcher++;

			// create mirror
			m = list_of_rooms.begin();
			int index;
			for (index = 0;index<number_of_rooms;index++,m++)
			{
                marcher++;
				RLSRoom room = *m;
				if (mirror_vertical)
				{
					room.corner1.x = (int) level.GetWidth() - room.corner1.x -1;
					room.corner2.x = (int) level.GetWidth() - room.corner2.x -1;		
					x1 = room.corner1.x;
					room.corner1.x = room.corner2.x;
					room.corner2.x = x1;
				}
				else
				{
					room.corner1.y = (int) level.GetHeight() - room.corner1.y -1;
					room.corner2.y = (int) level.GetHeight() - room.corner2.y -1;		
					y1 = room.corner1.y;
					room.corner1.y = room.corner2.y;
					room.corner2.y = y1;
				}
				list_of_rooms.insert(m, room);
			};
			for (m = list_of_rooms.begin(), _m = list_of_rooms.end();m!=_m;m++)
			{
                marcher++;
				RLSRoom &room = *m;
				for (x=room.corner1.x;x<=room.corner2.x;x++)
 					for (y=room.corner1.y;y<=room.corner2.y;y++)
					{
                        marcher++;
						if (level.GetCell(x,y) == RLLevelElementWall_value)
							level.SetCell(x,y,room.type);
					}			
			}

			// Create walls on connections
			free_cells = 0;
			for(x=0; x<(int) level.GetWidth()-1; x++)
			{
                marcher++;
				for(y=0; y<(int) level.GetHeight()/2; y++)
				{
                    marcher++;
					if (level.GetCell(x,y)!= level.GetCell(x+1,y) && level.GetCell(x+1,y) != RLLevelElementWall_value)
						level.SetCell(x,y,RLLevelElementWall_value);
					else if (level.GetCell(x,y) != level.GetCell(x,y+1) && level.GetCell(x,y+1) != RLLevelElementWall_value)
						level.SetCell(x,y,RLLevelElementWall_value);
					else if (level.GetCell(x,y) != level.GetCell(x+1,y+1) && level.GetCell(x+1,y+1) != RLLevelElementWall_value)
						level.SetCell(x,y,RLLevelElementWall_value);

					if (level.GetCell(x,y)!= RLLevelElementWall_value)
						free_cells+=2; // +2 for mirror

					level.SetCell(x,level.GetHeight()-y-1, level.GetCell(x,y)); // i odbicie lustrzane
				}
			}

			//	Size of ship

			if (free_cells<(int) level.GetHeight()*(int) level.GetWidth()/4)
				continue;

			ConvertValuesToTiles(level);		
			ConnectClosestRooms(level,marcher,true);
			break;
		} 
		ConvertValuesToTiles(level);
		AddDoors(level, seed+marcher, 1,0);
	}

    
	void CreateSimpleCity(RLCMap &level,int seed,const int& a_number_of_buildings)
	{
        int marcher=seed*2;
		const int min_building_width=5;
		const int max_building_width=10;
		const int min_building_height=5;
		const int max_building_height=10;

		if (level.GetWidth()==0 || level.GetHeight()==0)
			return;

		for (;;) // until created with proper # of buildings
		{
            marcher++;
			level.Clear(RLLevelElementGrass);

			RLSRoom main;
			main.corner1.x=0;
			main.corner1.y=0;
			main.corner2.x=(int) level.GetWidth();
			main.corner2.y=(int) level.GetHeight();

			AddRecursiveRooms(level, seed+marcher, RLLevelElementCorridor,max_building_width,max_building_height, main, false);

			int build_count=0;

			int tries=0;
            int marcher=0;
			while(build_count!=a_number_of_buildings && tries<100)
			{
				int size_x=max_building_width*2;
				int size_y=max_building_height*2;
                marcher++;
				while(1)
				{
                    marcher++;
					RLPosition pos;
					if (FindOnMapRandomRectangleOfType(level,seed+marcher,RLLevelElementGrass,pos,RLSize(size_x+2,size_y+2)))
					{
						RLSRoom building, smaller;
						pos.x++;
						pos.y++;
						building.corner1=pos;
						building.corner2=pos;
						building.corner2.x+=size_x;
						building.corner2.y+=size_y;
						smaller = building;
						smaller.corner1.x++;
						smaller.corner1.y++;
						smaller.corner2.x--;
						smaller.corner2.y--;
						DrawRectangleOnMap(level, building.corner1,building.corner2, RLLevelElementWall);
						DrawRectangleOnMap(level, smaller.corner1,smaller.corner2, RLLevelElementRoom);
						AddRecursiveRooms(level,seed+marcher+1, RLLevelElementWall,3,3,smaller);

						// add a doors leading out (improve to lead to nearest road)
						if (randoms.Query(seed,seed+marcher))
						{
							if (randoms.Query(seed,seed+marcher+1))
								level.SetCell(building.corner1.x+randoms.Upto(size_x-2,seed,marcher+22)+1,building.corner1.y,RLLevelElementDoorClose);
							else
								level.SetCell(building.corner1.x+randoms.Upto(size_x-2,seed,marcher+23)+1,building.corner2.y-1,RLLevelElementDoorClose);
						}
						else
						{
							if (randoms.Query(seed,seed+marcher+2))
								level.SetCell(building.corner1.x,building.corner1.y+randoms.Upto(size_y-2,seed,marcher+12)+1,RLLevelElementDoorClose);
							else
								level.SetCell(building.corner2.x-1,building.corner1.y+randoms.Upto(size_y-2,seed,marcher+13)+1,RLLevelElementDoorClose);
						}


						build_count++;
						if (build_count==a_number_of_buildings)
							break;
					}
					else
					{
						if (randoms.Query(seed,seed+marcher+3))
							size_x--;
						else
							size_y--;
						if (size_x<=min_building_width || size_y<=min_building_height)
						{
							tries++;
							break;
						}
					}
				}			
			}
            marcher++;
			if (tries<100)
			{
				// plant some trees
				for (int index=0;index<level.GetWidth()*level.GetHeight()*0.3;index++)
				{
					int x=randoms.Upto(static_cast <int> (level.GetWidth()),seed,index+seed+marcher);
					int y=randoms.Upto(static_cast <int> (level.GetHeight()),seed,index+seed+marcher+1);
					if (level.GetCell(x,y)==RLLevelElementGrass && CountNeighboursOfType(level,RLLevelElementWall,RLPosition(x,y),true)==0)
						level.SetCell(x,y,RLLevelElementPlant);
				}
				return;
			}
		}
	}

   	bool RandomLowerThatLimit(int limit, int value)
	{
		if (value==0)
			return false;
		if (randoms.Upto(value,limit,value)<limit)
			return true;
		return false;
	}

	void CreateMines(RLCMap &level,int seed,int max_number_of_rooms=10)
	{
		if (level.GetWidth()==0 || level.GetHeight()==0)
			return;

		level.Clear();

		int x,y,sx,sy;

		typedef std::list < RLSRoom > list_of_rooms;

		list_of_rooms :: iterator m;
		list_of_rooms rooms;
		RLSRoom room;

		int random_number;
		int diff_x, diff_y;

		RLPosition p,p1,p2;

		// Place rooms
        int marcher=seed+seed;
		for (int room_number=0;room_number<max_number_of_rooms;++room_number)
		{
            marcher+=randoms.Upto(100,seed,marcher);
			// size of room
			sx = randoms.Upto(5,seed,marcher+1)+6;
			sy = randoms.Upto(5,seed,marcher+1)+6;
			if (FindOnMapRandomRectangleOfType(level,seed+marcher,RLLevelElementWall, p, RLSize(sx+4, sy+4)))
			{
				p.x+=2;
				p.y+=2;
				// Connect the room to existing one

				if (rooms.size()>0)
				{

					random_number = randoms.Upto(static_cast<int>(rooms.size()),seed,marcher+29);
					for (m = rooms.begin();--random_number>0;++m);

					// center of this room
					p1.x= p.x+sx/2;
					p1.y= p.y+sy/2;
					// center of second room
					p2.x= (*m).corner1.x + ((*m).corner2.x - (*m).corner1.x)/2;
					p2.y= (*m).corner1.y + ((*m).corner2.y - (*m).corner1.y)/2;
					// found the way to connect rooms

					diff_x = p2.x-p1.x;
					diff_y = p2.y-p1.y;

					if (diff_x<0)
						diff_x=-diff_x;
					if (diff_y<0)
						diff_y=-diff_y;

					x=p1.x; y=p1.y;

					while (!(diff_x==0 && diff_y==0))
					{
						if (RandomLowerThatLimit(diff_x,diff_x+diff_y)) // move horizontally
						{
							diff_x--;
							if (x>p2.x)
								x--;
							else
								x++;
						}
						else
						{
							diff_y--;
							if (y>p2.y)
								y--;
							else
								y++;
						}
						// Check what is on that position
						if (level.GetCell(x,y)==RLLevelElementRoom)
							break;
						else if (level.GetCell(x,y)==RLLevelElementCorridor)
							if (randoms.Query(seed,marcher+seed))
								break;

						level.SetCell(x,y,RLLevelElementCorridor);
					}
				}
				// add to list of rooms

				room.corner1.x=p.x;
				room.corner1.y=p.y;
				room.corner2.x=p.x+sx;
				room.corner2.y=p.y+sy;
				room.type = room_number;
				rooms.push_back(room);

				// draw_room

				int room_type = randoms.Upto(4,seed,marcher+666);
				if (sx==sy)
					room_type=3;

				if (room_type!=2)
				{
					for (y=0;y<sy;y++)
						for (x=0;x<sx;x++)
						{
							switch (room_type)
							{
							case 0: // rectangle room
							case 1: 
								level.SetCell(p.x+x,p.y+y,RLLevelElementRoom);
								break;
							case 3: // round room
								if ((int)idistance(sx/2,sx/2,x,y)<sx/2)
									level.SetCell(p.x+x,p.y+y,RLLevelElementRoom);
								break;
							}
						}
				} // end if
				else // typ==2 - Diamond
				{
					for (y=0;y<=sy/2;y++)
						for (x=0;x<=sx/2;x++)
						{
							if (y>=x)
							{
								level.SetCell(p.x+x+sx/2,p.y+y,RLLevelElementRoom);
								level.SetCell(p.x+x+sx/2,p.y+sy-y,RLLevelElementRoom);
								level.SetCell(p.x+sx/2-x,p.y+y,RLLevelElementRoom);
								level.SetCell(p.x+sx/2-x,p.y+sy-y,RLLevelElementRoom);
							} 
						} 
				} 
			} // end of room addition
		}
	}

    
	void CreateMaze(RLCMap &level,int seed,bool allow_loops=false)
	{
        int marcher=seed*2;
		if (level.GetWidth()==0 || level.GetHeight()==0)
			return;

		level.Clear();

		std::list < RLPosition > drillers;
		drillers.push_back(RLPosition((size_t)level.GetWidth()/2, (size_t)level.GetHeight()/2));
		while(drillers.size()>0)
		{
            marcher++;
			std::list < RLPosition >::iterator m,_m,temp;
			m=drillers.begin();
			_m=drillers.end();
			while (m!=_m)
			{
                marcher++;
				bool remove_driller=false;
				switch( randoms.Upto(4,seed,seed+marcher) )
				{
				case 0:
					(*m).y-=2;
					if ((*m).y<0 || (level.GetCell((*m).x,(*m).y)==RLLevelElementCorridor))
					{
						if (!allow_loops || (allow_loops && randoms.Upto(5,seed,marcher)))
						{
							remove_driller=true;
							break;
						}
					}
					level.SetCell((*m).x,(*m).y+1,RLLevelElementCorridor);
					break;
				case 1:
					(*m).y+=2;
					if ((*m).y>=static_cast <int> (level.GetHeight()) || level.GetCell((*m).x,(*m).y)==RLLevelElementCorridor)
					{
						remove_driller=true;
						break;
					}
					level.SetCell((*m).x,(*m).y-1,RLLevelElementCorridor);
					break;
				case 2:
					(*m).x-=2;
					if ((*m).x<0 || level.GetCell((*m).x,(*m).y)==RLLevelElementCorridor)
					{
						remove_driller=true;
						break;
					}
					level.SetCell((*m).x+1,(*m).y,RLLevelElementCorridor);
					break;
				case 3:
					(*m).x+=2;
					if ((*m).x>=static_cast <int> (level.GetWidth()) || level.GetCell((*m).x,(*m).y)==RLLevelElementCorridor)
					{
						remove_driller=true;
						break;
					}
					level.SetCell((*m).x-1,(*m).y,RLLevelElementCorridor);
					break;
				}
				if (remove_driller)
					m = drillers.erase(m);
				else
				{
					drillers.push_back(RLPosition((*m).x,(*m).y));
					drillers.push_back(RLPosition((*m).x,(*m).y));

					level.SetCell((*m).x,(*m).y,RLLevelElementCorridor);
					++m;
				}
			}
		}
	}

    
	void CreateCaves(RLCMap &level, int seed, int iterations=1, float density=0.65) {
        int marcher=seed*3;
		if (level.GetWidth()==0 || level.GetHeight()==0)
			return;

		level.Clear(RLLevelElementRoom);

		// create a game of life cave

		int x,y;

		for(int fill=0; fill<(level.GetWidth()*level.GetHeight()*density); fill++) {
         marcher++;
         int ddx=randoms.Upto((int) level.GetWidth(),seed,fill+seed),
             ddy=randoms.Upto((int) level.GetHeight(),seed+1,fill+seed);
			level.SetCell( ddx,ddy, RLLevelElementWall );
        }

		for(int iteration=0; iteration<iterations; iteration++)
		{
            marcher++;
			for(x=0; x<(int) level.GetWidth(); x++)
			{
                marcher++;
				for(y=0; y<(int) level.GetHeight(); y++)
				{
                    marcher++;
					int neighbours=CountNeighboursOfType(level,RLLevelElementWall,RLPosition(x,y));

					if (level.GetCell(x,y)==RLLevelElementWall)
					{
						if (neighbours<4)
							level.SetCell(x,y,RLLevelElementRoom);
					}
					else
					{
						if (neighbours>4)
							level.SetCell(x,y,RLLevelElementWall);
					}

					if (x==0 || x==(int) level.GetWidth()-1 || y==0 || y==(int) level.GetHeight()-1)
						level.SetCell(x,y,RLLevelElementWall);
				}
			}
		}

		ConnectClosestRooms(level,seed+marcher,true);
		ConvertValuesToTiles(level);
    }

    
	void CreateAntNest(RLCMap &level, int seed, bool with_rooms=false)
	{
		if (level.GetWidth()==0 || level.GetHeight()==0)
			return;

		level.Clear();

		int x,y;

		level.SetCell(level.GetWidth()/2,level.GetHeight()/2,RLLevelElementCorridor);

		double x1,y1;
		double k;
		double dx, dy;
		int px, py;
        unsigned int marcher=seed*2;
		for (int object=0;object<(int) level.GetWidth()*(int) level.GetHeight()/3;++object)
		{
            marcher++;
			// degree
			k = randoms.Upto(360,seed,marcher+3)*3.1419532/180;
			// position on ellipse by degree
			x1 = (double) level.GetWidth()/2+((double)level.GetWidth()/2)*sin(k);	
			y1 = (double) level.GetHeight()/2+((double)level.GetHeight()/2)*cos(k);

			// object will move not too horizontal and not too vertival
			do {
				dx=randoms.Upto(100,seed,marcher);
				dy=randoms.Upto(100,seed,marcher+1);
			} while ((abs((int) dx)<10 && abs((int) dy)<10));
			dx-=50;
			dy-=50;
			dx/=100;
			dy/=100;

			int counter=0;
			while (1)
			{
				// didn't catch anything after 1000 steps (just to avoid infinite loops)
				if (counter++>1000)
				{
					object--;
					break;
				}
				// move object by small step
				x1+=dx;
				y1+=dy;

				// change float to int

				px=(int) x1;
				py=(int) y1;

				// go through the border to the other side

				if (px<0)
				{
					px=(int) level.GetWidth()-1;
					x1=px;
				}
				if (px>(int) level.GetWidth()-1)
				{
					px=0;
					x1=px;
				}
				if (py<0)
				{
					py=(int) level.GetHeight()-1;
					y1=py;
				}
				if (py>(int) level.GetHeight()-1)
				{
					py=0;
					y1=py;
				}

				// if object has something to catch, then catch it

				if ((px>0 && level.GetCell(px-1,py)==RLLevelElementCorridor) ||
					(py>0 && level.GetCell(px,py-1)==RLLevelElementCorridor) ||
					(px<(int) level.GetWidth()-1 && level.GetCell(px+1,py)==RLLevelElementCorridor) ||
					(py<(int) level.GetHeight()-1 && level.GetCell(px,py+1)==RLLevelElementCorridor))
				{
					level.SetCell(px,py,RLLevelElementCorridor);
					break;
				}
			}

		}

		if (with_rooms)
		{
			// add halls at the end of corridors
			for (y=1;y<(int) level.GetHeight()-1;y++)
			{
				for (x=1;x<(int) level.GetWidth()-1;x++)
				{
					if ((x>(int) level.GetWidth()/2-10 && x<(int) level.GetWidth()/2+10 && y>(int) level.GetHeight()/2-5 && y<(int) level.GetHeight()/2+5) || level.GetCell(x,y)==RLLevelElementWall)
						continue;

					int neighbours=CountNeighboursOfType(level,RLLevelElementCorridor,RLPosition(x,y));

					if (neighbours==1)
					{
						for (px=-1;px<=1;px++)
							for (py=-1;py<=1;py++)
							{
								level.SetCell(x+px,y+py,RLLevelElementRoom);
							}
					}
				}		
			}
		} // end of if (with_rooms)
	}

    
	void FindOnMapAllRectanglesOfType(RLCMap &level, const RLELevelElement& type, const RLSize &size, std::vector < RLPosition >& positions )
	{
		RLCMap good_points;
		good_points = level;

		for (unsigned int y=0;y<level.GetHeight();++y)
			for (unsigned int x=0;x<level.GetWidth();++x)
				good_points.SetCell(x,y,0);

		for (unsigned int y=0;y<level.GetHeight();++y)
		{
			int horizontal_count=0;
			for (unsigned int x=0;x<level.GetWidth();++x)
			{
				if (level.GetCell(x,y)==type)
					horizontal_count++;
				else
					horizontal_count=0;

				if (horizontal_count==size.x)
				{
					good_points.SetCell(x-size.x+1,y,1);
					horizontal_count--;
				}
			}
		}

		// count verticals

		for (unsigned int  x=0;x<level.GetWidth();++x)
		{
			int vertical_count=0;
			for (unsigned int  y=0;y<level.GetHeight();++y)
			{
				if (good_points.GetCell(x,y)==1)
					vertical_count++;
				else
					vertical_count=0;

				if (vertical_count==size.y)
				{
					positions.push_back(RLPosition((size_t) x,(size_t) y-size.y+1));
					vertical_count--;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	bool FloodFill(RLCMap &level, RLPosition position, int value, bool diagonal=true, int gradient=0, RLPosition end=RLPosition(-1,-1))
	{
		// flood fill room
		int area_value = level.GetCell(position.x,position.y);
		level.SetCell(position.x,position.y,value);

		std::list < RLPosition > positions;
		positions.push_back(position);

		std::list < RLPosition > ::iterator m,_m;
		m=positions.begin();
		while (m!=positions.end())
		{

			// Fill only to the end?
			if (end.x!=-1 && end==(*m))
				break;

			int pos_x=(*m).x;
			int pos_y=(*m).y;

			int this_value = level.GetCell(pos_x,pos_y);

			if (pos_x>0)
				if (level.GetCell(pos_x-1,pos_y)==area_value)
				{
					level.SetCell(pos_x-1,pos_y,this_value+gradient);
					positions.push_back(RLPosition(pos_x-1,pos_y));
				}

			if (pos_x<(int) level.GetWidth()-1)
				if (level.GetCell(pos_x+1,pos_y)==area_value)
				{
					level.SetCell(pos_x+1,pos_y,this_value+gradient);
					positions.push_back(RLPosition(pos_x+1,pos_y));
				}

			if (pos_y>0)
				if (level.GetCell(pos_x,pos_y-1)==area_value)
				{
					level.SetCell(pos_x,pos_y-1,this_value+gradient);
					positions.push_back(RLPosition(pos_x,pos_y-1));
				}

			if (pos_y<(int) level.GetHeight()-1)
				if (level.GetCell(pos_x,pos_y+1)==area_value)
				{
					level.SetCell(pos_x,pos_y+1,this_value+gradient);
					positions.push_back(RLPosition(pos_x,pos_y+1));
				}

			if (diagonal)
			{
				if (pos_x>0 && pos_y>0)
					if (level.GetCell(pos_x-1,pos_y-1)==area_value)
					{
						level.SetCell(pos_x-1,pos_y-1,this_value+gradient);
						positions.push_back(RLPosition(pos_x-1,pos_y-1));
					}

				if (pos_x<(int) level.GetWidth()-1 && pos_y<(int) level.GetHeight()-1)
					if (level.GetCell(pos_x+1,pos_y+1)==area_value)
					{
						level.SetCell(pos_x+1,pos_y+1,this_value+gradient);
						positions.push_back(RLPosition(pos_x+1,pos_y+1));
					}

				if (pos_x<(int) level.GetWidth()-1 && pos_y>0)
					if (level.GetCell(pos_x+1,pos_y-1)==area_value)
					{
						level.SetCell(pos_x+1,pos_y-1,this_value+gradient);
						positions.push_back(RLPosition(pos_x+1,pos_y-1));
					}

				if (pos_x>0 && pos_y<(int) level.GetHeight()-1)
					if (level.GetCell(pos_x-1,pos_y+1)==area_value)
					{
						level.SetCell(pos_x-1,pos_y+1,this_value+gradient);
						positions.push_back(RLPosition(pos_x-1,pos_y+1));
					}
			}		
			m=positions.erase(m);
		}
		// filling to the end cell without success
		if (m==positions.end())
			return false;
		if (end.x!=-1 && end!=(*m))
			return false;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	
	bool FindOnMapRandomRectangleOfType(RLCMap &level, int seed, const RLELevelElement& type, RLPosition& pos, const RLSize &size)
	{
		std::vector < RLPosition > positions;
		FindOnMapAllRectanglesOfType(level,type,size,positions);
		if (positions.size()==0)
			return false;

		// get position of Random rectangle 

		pos = positions[ randoms.Upto((int) positions.size(), seed, seed) ];	
		return true;
	}

	//////////////////////////////////////////////////////////////////////////

	int CountNeighboursOfType(RLCMap &level, RLELevelElement type, const RLPosition& pos, bool diagonal=true)
	{
		int neighbours=0;
		if (pos.y>0)
			if (level.GetCell(pos.x,pos.y-1)==type) // N
				neighbours++;     

		if (pos.x<(int) level.GetWidth()-1)
			if (level.GetCell(pos.x+1,pos.y)==type) // E
				neighbours++;     

		if (pos.x>0 && pos.y<(int) level.GetHeight()-1)
			if (level.GetCell(pos.x,pos.y+1)==type) // S
				neighbours++;     

		if (pos.x>0 && pos.y>0)
			if (level.GetCell(pos.x-1,pos.y)==type) // W
				neighbours++;     

		if (diagonal)
		{
			if (pos.x>0 && pos.y>0)
				if (level.GetCell(pos.x-1,pos.y-1)==type)  // NW
					neighbours++;  

			if (pos.x<(int) level.GetWidth()-1 && pos.y>0)
				if (level.GetCell(pos.x+1,pos.y-1)==type) // NE
					neighbours++;   

			if (pos.x<(int) level.GetWidth()-1 && pos.y<(int) level.GetHeight()-1) // SE
				if (level.GetCell(pos.x+1,pos.y+1)==type) 
					neighbours++;   


			if (pos.x>0 && pos.y<(int) level.GetHeight()-1)
				if (level.GetCell(pos.x-1,pos.y+1)==type) // SW
					neighbours++;   
		}

		return neighbours;
	}

	//////////////////////////////////////////////////////////////////////////

	void AddDoors(RLCMap &level, int seed, float door_probability, float open_probability)
	{
		for (size_t x=0;x<level.GetWidth();++x)
			for (size_t y=0;y<level.GetHeight();++y)
			{
				RLPosition pos(x,y);
				int room_cells = CountNeighboursOfType(level,RLLevelElementRoom,pos);
				int corridor_cells = CountNeighboursOfType(level,RLLevelElementCorridor,pos);
				int open_door_cells = CountNeighboursOfType(level,RLLevelElementDoorOpen,pos);
				int close_door_cells = CountNeighboursOfType(level,RLLevelElementDoorClose,pos);
				int door_cells = open_door_cells + close_door_cells;

				if (level.GetCell((int)x,(int)y)==RLLevelElementCorridor)
				{
					if ((corridor_cells==1 && door_cells==0 && room_cells>0 && room_cells<4) ||
						(corridor_cells==0 && door_cells==0))
					{
						float exist = ((float) randoms.Upto(1000,seed,(int)(x+y)))/1000;
						if (exist<door_probability)
						{
							float is_open = ((float) randoms.Upto(1000,seed+x+y,x+y))/1000;
							if (is_open<open_probability)
								level.SetCell(x,y,RLLevelElementDoorOpen);
							else
								level.SetCell(x,y,RLLevelElementDoorClose);
						}
					}
				} 
			}
	}

	//////////////////////////////////////////////////////////////////////////

	bool AddCorridor(RLCMap &level, int seed, const int& start_x1, const int& start_y1, const int& start_x2, const int& start_y2, bool straight=false)
	{	
		if (!level.OnMap(start_x1,start_y1) || !level.OnMap(start_x2,start_y2))
			return false;
		// we start from both sides 
		int x1,y1,x2,y2;
		x1=start_x1;
		y1=start_y1;
		x2=start_x2;
		y2=start_y2;
		int dir_x;
		int dir_y;
		if (start_x2>start_x1) dir_x=1;
	    else dir_x=-1;
		if (start_y2>start_y1) dir_y=1;
		else dir_y=-1;

		// move into direction of the other end
		bool first_horizontal=randoms.Query(seed,seed+1);
		bool second_horizontal=randoms.Query(seed,seed+99);

        int marcher=seed*2;
		while(1)
		{
            marcher+=randoms.Upto(123,seed,marcher);
			if (!straight)
			{
				first_horizontal=randoms.Query(seed,marcher+2);
				second_horizontal=randoms.Query(seed,marcher+31);
			}

			if (x1!=x2 && y1!=y2)
			{
				if (first_horizontal)
					x1+=dir_x;
				else
					y1+=dir_y;
			}
			// connect rooms
			if (x1!=x2 && y1!=y2)
			{
				if (second_horizontal)
					x2-=dir_x;
				else
					y2-=dir_y;
			}

			if (level.GetCell(x1,y1)==RLLevelElementWall_value)
				level.SetCell(x1,y1,RLLevelElementCorridor_value);
			if (level.GetCell(x2,y2)==RLLevelElementWall_value)
				level.SetCell(x2,y2,RLLevelElementCorridor_value);

			// connect corridors if on the same level
			if (x1==x2)
			{
				while(y1!=y2)
				{
					y1+=dir_y;
					if (level.GetCell(x1,y1)==RLLevelElementWall_value)
						level.SetCell(x1,y1,RLLevelElementCorridor_value);
				}
				if (level.GetCell(x1,y1)==RLLevelElementWall_value)
					level.SetCell(x1,y1,RLLevelElementCorridor_value);
				return true;
			}
			if (y1==y2)
			{
				while(x1!=x2)
				{
					x1+=dir_x;
					if (level.GetCell(x1,y1)==RLLevelElementWall_value)
						level.SetCell(x1,y1,RLLevelElementCorridor_value);
				}
				if (level.GetCell(x1,y1)==RLLevelElementWall_value)
					level.SetCell(x1,y1,RLLevelElementCorridor_value);
				return true;
			}
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////

	int FillDisconnectedRoomsWithDifferentValues(RLCMap &level)
	{
		for (unsigned int  y=0;y<level.GetHeight();++y)
			for (unsigned int  x=0;x<level.GetWidth();++x)
			{
				if (level.GetCell(x,y)==RLLevelElementRoom)
					level.SetCell(x,y,RLLevelElementRoom_value);
				else if (level.GetCell(x,y)==RLLevelElementWall)
					level.SetCell(x,y,RLLevelElementWall_value);
			}

			int room_number=0;

			for (size_t y=0;y<level.GetHeight();++y)
			{
				for (size_t x=0;x<level.GetWidth();++x)
				{
					if (level.GetCell(x,y)==RLLevelElementRoom_value)
					{
						FloodFill(level,RLPosition(x,y),room_number++);
					}
				}
			}
			return room_number;
	}


	//////////////////////////////////////////////////////////////////////////

	void ConnectClosestRooms(RLCMap &level, int seed, bool with_doors, bool straight_connections=false)
	{
        int marcher=seed*2;
		FillDisconnectedRoomsWithDifferentValues(level);

		std::vector < std::list < RLPosition > > rooms;

		for (size_t y=0;y<level.GetHeight();++y)
		{
			for (size_t x=0;x<level.GetWidth();++x)
			{
				if (level.GetCell(x,y)!=RLLevelElementWall_value)
				{
					if (level.GetCell(x,y)>=(int) rooms.size())
						rooms.resize(level.GetCell(x,y)+1);

					if (CountNeighboursOfType(level,RLLevelElementWall_value,RLPosition(x,y),false)>0) // only border cells without diagonals
						rooms[ level.GetCell(x,y) ].push_back(RLPosition(x,y));
				}
			}
		}

		random_shuffle(rooms.begin(),rooms.end());

		if (rooms.size()<2)
			return;

		// for warshall algorithm
		// set the connection matrix

		std::vector < std::vector < bool > > room_connections;
		std::vector < std::vector < bool > > transitive_closure;
		std::vector < std::vector < int > > distance_matrix;
		std::vector < std::vector < std::pair < RLPosition, RLPosition >  > > closest_cells_matrix;

		room_connections.resize(rooms.size());
		transitive_closure.resize(rooms.size());
		distance_matrix.resize(rooms.size());
		closest_cells_matrix.resize(rooms.size());

		for (size_t a=0;a<rooms.size();++a)
		{	
			room_connections[a].resize(rooms.size());
			transitive_closure[a].resize(rooms.size());
			distance_matrix[a].resize(rooms.size());
			closest_cells_matrix[a].resize(rooms.size());
			for (size_t b=0;b<rooms.size();++b)
			{
				room_connections[a][b]=false;
				distance_matrix[a][b]=INT_MAX;
			}
		}

		// find the closest cells for each room - Random closest cell
		std::list < RLPosition >::iterator m,_m,k,_k;

		for (int room_a=0;room_a<(int) rooms.size();++room_a)
		{
			for (int room_b=0;room_b<(int) rooms.size();++room_b)
			{
				if (room_a==room_b)
					continue;
				std::pair < RLPosition, RLPosition > closest_cells;
				for (m=rooms[room_a].begin(),_m=rooms[room_a].end();m!=_m;++m)
				{
					// for each boder cell in room_a try each border cell of room_b
					int x1 = (*m).x;
					int y1 = (*m).y;

					for (k=rooms[room_b].begin(),_k=rooms[room_b].end();k!=_k;++k)
					{
						int x2 = (*k).x;
						int y2 = (*k).y;

						int dist_ab = (int) idistance(x1,y1,x2,y2);
						
						if (dist_ab<distance_matrix[room_a][room_b] || (dist_ab==distance_matrix[room_a][room_b] && randoms.Query(seed,seed+23)))
						{
							closest_cells = std::make_pair( RLPosition(x1,y1), RLPosition(x2,y2) );
							distance_matrix[room_a][room_b] = dist_ab;
						}
					}
				}
				closest_cells_matrix[room_a][room_b] = closest_cells;
			}
		}

		// Now connect the rooms to the closest ones

		for (int room_a=0;room_a<(int) rooms.size();++room_a)
		{
			int min_distance=INT_MAX;
			int closest_room;
			for (int room_b=0;room_b<(int) rooms.size();++room_b)
			{
				if (room_a==room_b)
					continue;
				int distance = distance_matrix[room_a][room_b];
				if (distance<min_distance)
				{
					min_distance = distance;
					closest_room=room_b;
				}
			}

			// connect room_a to closest one
			std::pair < RLPosition, RLPosition > closest_cells;
			closest_cells = closest_cells_matrix[room_a][closest_room];

			int x1=closest_cells.first.x;
			int y1=closest_cells.first.y;
			int x2=closest_cells.second.x;
			int y2=closest_cells.second.y;

			if (room_connections[room_a][closest_room]==false && AddCorridor(level,x1,y1,x2,y2,straight_connections))
			{
				room_connections[room_a][closest_room]=true;
				room_connections[closest_room][room_a]=true;
			}
		}

		// The closest rooms connected. Connect the rest until all areas are connected


		for(int to_connect_a=0;to_connect_a!=-1;)
		{
			size_t a,b,c;
			int to_connect_b;


			for (a=0;a<rooms.size();a++)
				for (b=0;b<rooms.size();b++)
					transitive_closure[a][b] = room_connections[a][b];

			for (a=0;a<rooms.size();a++)
			{
				for (b=0;b<rooms.size();b++)
				{
					if (transitive_closure[a][b]==true && a!=b)
					{
						for (c=0;c<rooms.size();c++)
						{
							if (transitive_closure[b][c]==true)
							{
								transitive_closure[a][c]=true;
								transitive_closure[c][a]=true;
							}
						}
					}
				}
			}

			// Check if all rooms are connected
			to_connect_a=-1;
			for (a=0;a<rooms.size() && to_connect_a==-1;++a)
			{
				for (b=0;b<rooms.size();b++)
				{
					if (a!=b && transitive_closure[a][b]==false)
					{
						to_connect_a=(int) a;
						break;
					}
				}
			}

			if (to_connect_a!=-1)
			{
				// connect rooms a & b
				do {
                 marcher++;
					to_connect_b = randoms.Upto((int) rooms.size(),marcher,seed);
				} while(to_connect_b==to_connect_a);
				std::pair < RLPosition, RLPosition > closest_cells;
				closest_cells = closest_cells_matrix[to_connect_a][to_connect_b];

				int x1=closest_cells.first.x;
				int y1=closest_cells.first.y;
				int x2=closest_cells.second.x;
				int y2=closest_cells.second.y;

				AddCorridor(level,x1,y1,x2,y2,straight_connections);

				room_connections[to_connect_a][to_connect_b]=true;
				room_connections[to_connect_b][to_connect_a]=true;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	void AddRecursiveRooms(RLCMap &level, int seed, const RLELevelElement& type,int min_size_x,int min_size_y, RLSRoom room, bool with_doors=true)
	{
        int marcher=seed*2;
		int size_x = room.corner2.x - room.corner1.x;
		if (size_x%2!=0)
			size_x-=randoms.Query(seed,seed);

		int size_y = room.corner2.y - room.corner1.y;
		if (size_y%2!=0)
			size_y-=randoms.Query(seed,seed+2);

		bool split_horizontal;

		if (size_y*4>size_x)
			split_horizontal=true;
		else if (size_x*4>size_y)
			split_horizontal=false;
		else
		{
			split_horizontal = randoms.Query(seed,seed+4);
		}

		if (split_horizontal) // split horizontal
		{
			if (size_y/2<min_size_y)
				return;
			int split = size_y/2+randoms.Upto(size_y/2-min_size_y,seed,marcher);
			for (int x=room.corner1.x;x<room.corner2.x;x++)
				level.SetCell(x,room.corner1.y+split,type);

			if (with_doors)
			{
				level.SetCell(room.corner1.x+randoms.Upto(size_x-1,seed,marcher+2)+1,room.corner1.y+split,RLLevelElementDoorClose);
			}

			RLSRoom new_room = room;
			new_room.corner2.y = room.corner1.y+split;
			AddRecursiveRooms(level,seed+marcher+3,type,min_size_x,min_size_y,new_room,with_doors);

			new_room = room;
			new_room.corner1.y = room.corner1.y+split;
			AddRecursiveRooms(level,seed+marcher+4,type,min_size_x,min_size_y,new_room,with_doors);
		}
		else
		{
			if (size_x/2<min_size_x)
				return;
			int split = size_x/2+randoms.Upto(size_x/2-min_size_x,seed,marcher+76);		
			for (int y=room.corner1.y;y<room.corner2.y;y++)
				level.SetCell(room.corner1.x+split,y,type);

			if (with_doors)
				level.SetCell(room.corner1.x+split,room.corner1.y+randoms.Upto(size_y-1,seed,marcher+123)+1,RLLevelElementDoorClose);

			RLSRoom new_room = room;
			new_room.corner2.x = room.corner1.x+split;
			AddRecursiveRooms(level,marcher+58,type,min_size_x,min_size_y,new_room,with_doors);

			new_room = room;
			new_room.corner1.x = room.corner1.x+split;
			AddRecursiveRooms(level,seed+marcher+213,type,min_size_x,min_size_y,new_room,with_doors);
		}
	}

	//////////////////////////////////////////////////////////////////////////

	void ConvertValuesToTiles(RLCMap &level)
	{
		for (unsigned int  y=0;y<level.GetHeight();++y)
		{
			for (unsigned int  x=0;x<level.GetWidth();++x)
			{
				if (level.GetCell(x,y)==RLLevelElementCorridor_value)
					level.SetCell(x,y,RLLevelElementCorridor);
				else if (level.GetCell(x,y)==RLLevelElementWall_value)
					level.SetCell(x,y,RLLevelElementWall);
				else 
					level.SetCell(x,y,RLLevelElementRoom);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void DrawRectangleOnMap(RLCMap &level, const RLPosition& p1, const RLPosition& p2, int value) {
		for (int y=p1.y;y<p2.y;++y)
			for (int x=p1.x;x<p2.x;++x)
				level.SetCell(x,y,value);
	}

};