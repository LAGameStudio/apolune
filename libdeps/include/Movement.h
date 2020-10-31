/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2007 Shao-Chen Chang (cscxcs at gmail.com)
 *                    Shun-Yun Hu     (syhu at yahoo.com)
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
    Movement Model for VASTsim
*/

#ifndef _VAST_MOVEMENT_H
#define _VAST_MOVEMENT_H

#include "Config.h"
#include "VASTTypes.h"
#include "VASTUtil.h"

using namespace std;

namespace Vast
{

//
// MovementModel: abstract class to create various movement patterns
//

#define VAST_MOVEMENT_RANDOM      1
#define VAST_MOVEMENT_CLUSTER     2
#define VAST_MOVEMENT_GROUP       3

/*
class Coord
{
public:
    coord_t x,y;

    Coord ()
        :x (0), y (0)
    {}

    Coord (const Coord &c)
        :x(c.x), y(c.y) {}

    Coord (int x_coord, int y_coord)
        :x((coord_t)x_coord), y((coord_t)y_coord) {}

    Coord (coord_t x_coord, coord_t y_coord)
        :x(x_coord), y(y_coord) {}

    Coord &operator=(const Coord& c)
    {
        x = c.x;
        y = c.y;
        return *this;
    }

    bool operator==(const Coord& c)
    {
        return (this->x == c.x && this->y == c.y);
    }

    Coord &operator+=(const Coord& c)
    {
        x += c.x;
        y += c.y;
        return *this;
    }

    Coord &operator-=(const Coord& c)
    {
        x -= c.x;
        y -= c.y;
        return *this;
    }

    Coord &operator*=(double value)
    {
        x *= value;
        y *= value;
        return *this;
    }

    Coord &operator/=(double c)
    {
        x = (coord_t)((double)x / c);
        y = (coord_t)((double)y / c);
        return *this;
    }

    Coord operator+(const Coord& c)
    {
        return Coord (x + c.x, y + c.y);
    }

    Coord operator-(const Coord& c)
    {
        return Coord (x - c.x, y - c.y);
    }

    double dist (const Coord &c) const
    {
        double dx = (c.x - x);
        double dy = (c.y - y);
        return sqrt (pow (dx, 2) + pow (dy, 2));
    }

    inline double distanceSquare (const Coord &c) const
    {
        return (pow (c.x - x, 2) + pow (c.y - y, 2));
    }
};
*/

// specific movement model-only coordinates,
// content is adjustable based on needs
typedef uint16_t movecoord_t;

class MoveCoord
{
public:
    movecoord_t x, y;
};

class MovementModel
{
public:
    MovementModel (Position &world_p1, Position &world_p2, int num_nodes, double speed);
    virtual ~MovementModel ();

    virtual bool init () = 0;                                   // initialize positions
    virtual bool move () = 0;                                   // make one move for all nodes

    // get position for a node
    Position &getpos (int node)
    {
        return _pos[node];
    }

protected:
    // create a random position
    void rand_pos (Position &pos, Position &topleft, Position &bottomright);

    Position  _topleft, _bottomright;              // boundary for the movement space
    Position  _dim;                                // dimension of the world
    int       _num_nodes;                          // total number of nodes
    double    _speed;                              // speed of node movmenet
    Position *_pos;                                // coordinates of all nodes
};

//
//  MovementGenerator:  factory class to create actual movements,
//                      also able to store/retrieve movement records
//

class EXPORT MovementGenerator
{
public:
    MovementGenerator ();

    ~MovementGenerator ();

    // create & destroy a globally unique instance of the MovementGenerator object
    static MovementGenerator *createInstance ();
    static void destroyInstance ();

    bool initModel (int model, SectionedFile *record, bool replay, Position top_left, Position bottom_right, int num_nodes, int num_steps, double speed);

    // get the positin for a given node at a given step
    // NOTE that both node number and step begin from 0
    Position *getPos (int node, int step);

private:
    // NOTE: we have to use pointer to avoid warning from VC
    // about an exported object uses the 'map' data structure
    map<int, vector<MoveCoord> > *__pos_list;          // complete positions for all nodes
    int _num_nodes, _num_steps;
};



} // end namespace Vast

#endif /* _VAST_MOVEMENT_H */

