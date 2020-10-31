/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2007 Shao-Chen Chang   (cscxcs at gmail.com) 
 *               2006 Jiun-Shiang Chiou
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/*
 *  MovementGenerator class
 *
 */

#include <stdlib.h>
#include <time.h>

#include "RandomMovement.h"
#include "ClusterMovement.h"
#include "GroupMovement.h"

using namespace std;

namespace Vast
{  

#define MOVEMENT_SECTION 2

MovementModel::MovementModel (Position &world_p1, Position &world_p2, int num_nodes, double speed)
    :_topleft (world_p1), _bottomright (world_p2), _num_nodes (num_nodes), _speed (speed)

{
    _dim.x = _bottomright.x - _topleft.x;
    _dim.y = _bottomright.y - _topleft.y;
    _pos = new Position[num_nodes];
}

MovementModel::~MovementModel () 
{
    delete[] _pos;
}

// create a random position
void MovementModel::rand_pos (Position &pos, Position &topleft, Position &bottomright)
{
    pos.x = topleft.x + rand () % (int)(bottomright.x - topleft.x);
    pos.y = topleft.y + rand () % (int)(bottomright.y - topleft.y);
}

MovementGenerator::MovementGenerator ()  
        :_num_nodes (0), _num_steps (0)
{
    __pos_list = new map<int, vector<MoveCoord> >;
}

MovementGenerator::~MovementGenerator () 
{           
   
    map<int, vector<MoveCoord> >::iterator it = __pos_list->begin ();
    for (; it != __pos_list->end (); it++)
        it->second.clear ();    
    
    __pos_list->clear ();
    
    delete __pos_list;    
}

MovementGenerator *g_MovementGeneratorInstance;

// create a global instance of the MovementGenerator object
MovementGenerator *
MovementGenerator::createInstance ()
{
    if (g_MovementGeneratorInstance == NULL)
        g_MovementGeneratorInstance = new MovementGenerator;

    return g_MovementGeneratorInstance;
}

void 
MovementGenerator::destroyInstance ()
{
    if (g_MovementGeneratorInstance != NULL)
    {
        delete g_MovementGeneratorInstance;
        g_MovementGeneratorInstance = NULL;
    }
}


bool
MovementGenerator::initModel (int model, SectionedFile *record, bool replay,
                              Position top_left, Position bottom_right,
                              int num_nodes, int num_steps, double speed)
{
    errout eo;
    srand ((uint32_t) time (NULL));
    
    map<int, vector<MoveCoord> > &_pos_list = *__pos_list;

    MoveCoord pos;
    if (replay == true)
    {
        for (int p = 0; p < num_nodes; p++)
        {
            // fill in placeholder
            _pos_list[p].insert (_pos_list[p].end (), num_steps + 1, pos);

            // then read all positions from file
            if (record->read (MOVEMENT_SECTION, &_pos_list[p][0], sizeof (MoveCoord) * (num_steps + 1), 1) != 1)
            {
                eo.output ("MovementGenerator: reading position list failed.\n");
            }
        }
    }
    else
    {
        // create the proper movement model
        MovementModel *move_model;
        switch (model)
        {
        case VAST_MOVEMENT_RANDOM:
            move_model = new RandomMovement (top_left, bottom_right, num_nodes, speed);
            break;

        case VAST_MOVEMENT_CLUSTER:
            move_model = new ClusterMovement (top_left, bottom_right, num_nodes, speed);
            break;

        case VAST_MOVEMENT_GROUP:
            move_model = new GroupMovement (top_left, bottom_right, num_nodes, speed);
            break;

        default:
            return false;
        }

        move_model->init ();

        // create all positions at once
        int s,p;
        for (s = 0; s <= num_steps; s++)
        {
            for (p = 0; p < num_nodes; p++)
            {
                MoveCoord coord;
                Position &pos = move_model->getpos (p);
                coord.x = (movecoord_t)pos.x;
                coord.y = (movecoord_t)pos.y;
                
                _pos_list[p].push_back (coord);
            }
            move_model->move ();
        }

        delete move_model; 

        // record the positions to file
        if (record != NULL)
        {
            // note that the file is written such that all steps (steps+1) for node 1 is written first,
            // then all steps for node 2, etc...
            for (int p = 0; p < num_nodes; p++)
            {
                if (record->write (MOVEMENT_SECTION, &_pos_list[p][0], sizeof (MoveCoord) * (num_steps + 1), 1) != 1)
                {
                    eo.output ("MovementGenerator: writing position list to file failed.\n");
                }
            }
        }
    }

    // successful initialization
    _num_nodes = num_nodes;
    _num_steps = num_steps;

    return true;
}

Position *MovementGenerator::getPos (int node, int step)
{
    static Position pos;
 
    // return failure if the model wasn't initialized
    if (_num_nodes == 0 || node >= _num_nodes || step > _num_steps)
        return NULL;
    else        
    {
        pos.x = (coord_t)((*__pos_list)[node][step]).x;
        pos.y = (coord_t)((*__pos_list)[node][step]).y;
        pos.z = 0;
        return &pos;
        //return &((*__pos_list)[node][step]);
    }
}

} // namespace Vast

