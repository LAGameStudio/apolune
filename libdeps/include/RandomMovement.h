
#ifndef _VAST_UTIL_RANDOMMOVEMENT_H
#define _VAST_UTIL_RANDOMMOVEMENT_H

#include "Movement.h"

namespace Vast
{  

//
// available parameters:
//    double _x_min, _x_max, _y_min, _y_max;      // boundary for the movement space
//    int    _num_nodes;                          // total number of nodes
//    double _speed;                              // speed of node movmenet

class RandomMovement : public MovementModel
{
public:
    RandomMovement (Position &p1, Position &p2, int num_nodes, double speed) 
        :MovementModel (p1, p2, num_nodes, speed)
    {
        _dest = new Position[num_nodes];
    }

    ~RandomMovement () 
    {
        delete[] _dest;
    }

    bool init ()
    {
        // create new positions & destination waypoint targets
        for (int i=0; i<_num_nodes; ++i)
        {
            rand_pos (_pos[i], _topleft, _bottomright);
            rand_pos (_dest[i], _topleft, _bottomright);
        }
        return true;
    }

    bool move ()
    {
        double dist, ratio;
        for (int i=0; i<_num_nodes; ++i)
        {            
            dist = _dest[i].distance (_pos[i]);

            // move towards the destination one step
            Position delta = _dest[i] - _pos[i];
            
            // adjust deltas for constant velocity            
            if ((ratio = dist / _speed) > 1.0)
                delta /= ratio;

            _pos[i] += delta;

            // new position needed if destination already reached
            if (dist < 0.1)
                rand_pos (_dest[i], _topleft, _bottomright);
        }
        
        return true;
    }

private:
    Position *_dest;                          // destination coordinates of all nodes
};

} // namespace Vast

#endif /* _VAST_UTIL_RANDOMMOVEMENT_H */
