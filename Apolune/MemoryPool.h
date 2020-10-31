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
#include "moremath.h"
#include "ZeroTypes.h"

using namespace std;

class ListItem;
class LinkedList;

#define MP_GRANULARITY 100

struct MemoryPool {
public:
 Zdisposable<matrix2d<LinkedList>> recycling;
 int available( string type );
 void Recycle( ListItem *L );
 MemoryPool();
 ~MemoryPool();
};

class MemoryManager {
public:
 MemoryPool pool;
 Zdisposable<LinkedList> leashed;
 MemoryManager();
 ~MemoryManager();
 ListItem *New( string type );
 void Delete( ListItem *L );
 void Between();
};

extern MemoryManager mm;

extern Zp<MemoryManager> memory;