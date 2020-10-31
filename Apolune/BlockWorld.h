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

#include "LinkedList.h"
#include "MultiPointSprite.h"
#include "Matter.h"

// A triple-layer 3D world organized as 2D world
//   ______
//  /_/_/_/| < "air"
// /_/_/_/|| < "ground level" sprites
// |_|_|_||| < "underground / underwater"
// |_|_|_||/
// |_|_|_|/
//

class Entity : public ListItem {
public:
 virtual void Render() {}
 virtual void Between() {}
 bool End() {
  return false;
 }
};

// Something 3D that is displayed within this block.
class Entity3d : public Entity {
public:
};

class MatterEntity3d : public Entity3d {
public:
 Matters *m;
};

class MPSEntity3d : public Entity3d {
public:
};

// A billboard quad controlled by some AI
class Entity2d : public Entity {
public:
};

class Block : public ListItem {
public:
};

class Blocks : public matrix2d<Block> {
public:
};

class BlockWorld : public Entity {
public:
};

class BlockWorldTest : public GLWindow {
public:

};