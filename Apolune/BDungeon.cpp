#include "BDungeon.h"

#include "ZIndexed.h"


BDungeon::BDungeon(int seed, int x, int y, int maxDoors, int roomChance, int maxRoomSize, int maxIterations, int hallLength, int maxPossibleRooms ) {
 this->seed=seed;
 this->maxDoors=maxDoors;
 this->roomChance=roomChance;
 this->maxRoomSize=maxRoomSize;
 this->maxIterations=maxIterations;
 this->hallLength=hallLength;
 this->maxPossibleRooms=maxPossibleRooms;
 sizeX = x;
 sizeY = y;
 dungeonSize = sizeX * sizeY;
 dungeon = new char [dungeonSize];
 initializeDungeon();
 generate();
 printInfo();
}

BDungeon::BDungeon( int seed, int maxDoors, int roomChance, int maxRoomSize, int maxIterations, int hallLength, int maxPossibleRooms ) {
 this->maxDoors=maxDoors;
 this->roomChance=roomChance;
 this->maxRoomSize=maxRoomSize;
 this->maxIterations=maxIterations;
 this->hallLength=hallLength;
 this->maxPossibleRooms=maxPossibleRooms;
 sizeX = randoms.Upto(31,seed,seed) + 40;
 sizeY = randoms.Upto(31,seed,seed) + 40;
 dungeonSize = sizeX * sizeY;
 dungeon = new char [dungeonSize];
 initializeDungeon();
 generate();
 printInfo();
}

BDungeon::~BDungeon() {
 delete [] dungeon;
}

int BDungeon::getSizeX() {
 return sizeX;
}

int BDungeon::getSizeY() {
 return sizeY;
}

void BDungeon::initializeDungeon() {
 for (int size = 0; size < dungeonSize; size++) {
  dungeon[size] = GROUND;
 }
}

char BDungeon::getTile(int x, int y) {
 return dungeon[y * sizeX + x];
}

void BDungeon::setTile(int x, int y, char tile) {
 dungeon[y * sizeX + x] = tile;
}

void BDungeon::printInfo() {
 OUTPUT("Dungeon has %d rooms+hallways.\n",currentRooms);
}

void BDungeon::addRoom(int posX, int posY, int roomX, int roomY) {
    for (int y = 0; y < roomY; y++) {
        for (int x = 0; x < roomX; x++) {
            if (dungeon[(posY + y) * sizeX + (posX + x)] == GROUND) {
                if (x == 0 || x == roomX - 1 || y == 0 || y == roomY - 1) {
                    dungeon[(posY + y) * sizeX + (posX + x)] = WALL;
                } else {
                    dungeon[(posY + y) * sizeX + (posX + x)] = FLOOR;
                }
            }
        }
    }
}

void BDungeon::generate() {
    maxRooms = randoms.Upto(maxPossibleRooms,12+seed,seed) + 50;//allow 50 - 100 rooms
    currentRooms = 0;
        
    int numWalls = 0;
    ZIndexed<coord2i> walls;
    
    int roomPosX;
    int roomPosY;
    int roomSizeX;
    int roomSizeY;
    
    //create the first room
    roomPosX = randoms.Upto(getSizeX() - 10,seed,seed);//subtract to allow the largest room to fit anywhere
    roomPosY = randoms.Upto(getSizeY() - 10,seed,roomPosX);//subtract to allow the largest room to fit anywhere
    roomSizeX =randoms.Upto(6,seed,roomPosY+seed) + 5;//a room 5 - 10 blocks wide
    roomSizeY =randoms.Upto(6,seed+roomSizeX,roomPosY) + 5;//a room 5 - 10 blocks tall 
    addRoom(roomPosX, roomPosY, roomSizeX, roomSizeY);
    currentRooms++;//We added a room
    
    //1000 tries to create the desired number of rooms
    for (int counter = maxIterations; counter > 0; counter--) {
        if (currentRooms == maxRooms) {
            break;
        }
        
        //get number of eligible walls
        numWalls = 0;
        for (int x = 1; x < getSizeX() - 1; x++) {
            for (int y = 1; y < getSizeY() - 1; y++) {
                if (getTile(x, y) == WALL) {
                    if (getTile(x - 1, y) == FLOOR || getTile(x + 1, y) == FLOOR || getTile(x, y - 1) == FLOOR || getTile(x, y + 1) == FLOOR) {
                        numWalls++;
                    }
                }
            }
        }
        if (numWalls == 0) {
            break;
        }
        
        //get coordinates for each wall
		      walls.Size((size_t)numWalls);
        int currentWall = 0;
        for (int x = 1; x < getSizeX() - 1; x++) {
            for (int y = 1; y < getSizeY() - 1; y++) {
                if (getTile(x, y) == WALL) {
                    if (getTile(x - 1, y) == FLOOR || getTile(x + 1, y) == FLOOR || getTile(x, y - 1) == FLOOR || getTile(x, y + 1) == FLOOR) {
                        walls[currentWall].x = x;
                        walls[currentWall].y = y;
                        currentWall++;
                    }
                }
            }
        }
        
        //pick a random wall to build off of
        coord2i destWall = walls[randoms.Upto(numWalls,seed+numWalls+counter,counter+numWalls)];
        
        //get which direction to build the room
        int direction = 0;
        if (getTile(destWall.x, destWall.y - 1) == FLOOR) {
            direction = 2;//down
        }
        if (getTile(destWall.x, destWall.y + 1) == FLOOR) {
            direction = 1;//up
        }
        if (getTile(destWall.x + 1, destWall.y) == FLOOR) {
            direction = 3;//left
        }
        if (getTile(destWall.x - 1, destWall.y) == FLOOR) {
            direction = 4;//right
        }
        
        //decide whether it should be a room or a hall
        int roomType = randoms.Upto(100,seed+maxDoors+counter,seed+destWall.x) + 1;
        if (roomType > roomChance) { //try to build a hallway
            int length = 1+randoms.Upto(6,roomType,seed);//1-6 long
            switch (direction) {
                case 1://Up
                    roomPosX = destWall.x - 1;
                    roomPosY = destWall.y - length + 1;
                    roomSizeX = 3;
                    roomSizeY = length;
                    break;
                case 2://Down
                    roomPosX = destWall.x - 1;
                    roomPosY = destWall.y;
                    roomSizeX = 3;
                    roomSizeY = length;
                    break;
                case 3://Left
                    roomPosX = destWall.x - length + 1;
                    roomPosY = destWall.y - 1;
                    roomSizeX = length;
                    roomSizeY = 3;
                    break;
                case 4://Right
                    roomPosX = destWall.x;
                    roomPosY = destWall.y - 1;
                    roomSizeX = length;
                    roomSizeY = 3;
                    break;
                default:
                    break;
            }
        } else { //try to build a room
            roomSizeX = randoms.Upto(maxRoomSize,roomType+maxRoomSize+counter,seed+maxRoomSize+roomType+counter) + 5;//5 - 10 wide
            roomSizeY = randoms.Upto(maxRoomSize,roomType+seed+roomSizeX,seed+roomType) + 5;//5 - 10 tall
            switch (direction) {
                case 1://Up
                    roomPosX = destWall.x - (roomSizeX / 2);
                    roomPosY = destWall.y - (roomSizeY - 1);
                    break;
                case 2://Down
                    roomPosX = destWall.x - (roomSizeX / 2);;
                    roomPosY = destWall.y;
                    break;
                case 3://Left
                    roomPosX = destWall.x - (roomSizeX - 1);
                    roomPosY = destWall.y - (roomSizeY / 2);
                    break;
                case 4://Right
                    roomPosX = destWall.x;
                    roomPosY = destWall.y - (roomSizeY / 2);
                    break;
                default:
                    break;
            }
        }
        
        //check if the room fits
        bool roomFits = true;
        if (roomPosX < 0 || roomPosY < 0) {
            roomFits = false;
        }
        if (roomPosX >= (getSizeX() - roomSizeX) || roomPosY >= (getSizeY() - roomSizeY)) {
            roomFits = false;
        }
        if (roomFits) {
            for (int x = roomPosX; x < roomPosX + roomSizeX; x++) {
                for (int y = roomPosY; y < roomPosY + roomSizeY; y++) {
                    if (getTile(x, y) == FLOOR) {
                        roomFits = false;
                    }
                }
            }
        }
        
        //if it fits, go ahead and build it
        if (roomFits) {
            //add the room
            addRoom(roomPosX, roomPosY, roomSizeX, roomSizeY);
            currentRooms++;
            
            //open up a path to the hall
            if ( (--maxDoors > 0) && randoms.Upto(4,seed+maxDoors+counter,roomPosY) == 0) {//1 in 4 chance the opening is a door
                setTile(destWall.x, destWall.y, DOOR);
            } else {
                setTile(destWall.x, destWall.y, FLOOR);
            }
        }
                
        //check if we've reached the max number of rooms
        if (currentRooms == maxRooms) {
            break;
        }
    }
}