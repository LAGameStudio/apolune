/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2007 Shun-Yun Hu (syhu@yahoo.com)
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
 *  Arbitrator.h -- VASTATE Arbitrator class (everything related to an arbitrator)
 *
 *  ver 0.1 (2006/07/18)
 *   
 */

#ifndef VASTATE_ARBITRATOR_H
#define VASTATE_ARBITRATOR_H

#include "VASTATETypes.h"
#include "ArbitratorLogic.h"

namespace Vast 
{  

    class EXPORT Arbitrator : public MessageHandler
    {
    public:
        
        // initialize an arbitrator
        Arbitrator ()
            :MessageHandler (MSG_GROUP_VASTATE_ARBITRATOR)
        {
        }

        virtual ~Arbitrator ()
        {
        }

        //
        // Arbitrator interface
        //        

        // join a virtual location to assume arbitrator role
        virtual bool    join (const Position &pos) = 0;

        // leave the arbitrator overlay
        virtual bool    leave () = 0;

        // sends the results of a join request to a joining Agent
        virtual void    admit (id_t agent, char *status, size_t size) = 0;

        // send a message to a given agent
        virtual bool    send (Message &msg) = 0;

        //
        // for use mostly by ArbitratorLogic
        //

        // create or delete a new Object (can only delete if I'm the owner)
        // agent_id indicates if it's an avatar object

        virtual Object *createObject (byte_t type, const Position &pos, obj_id_t *obj_id = NULL, id_t agent_id = 0) = 0;
        virtual bool    destroyObject (const obj_id_t &obj_id) = 0;

        // updating an existing object
        virtual bool    updateObject (const obj_id_t &obj_id, int index, int type, void *value) = 0;
        virtual bool    moveObject (const obj_id_t &obj_id, const Position &newpos) = 0;
                
        // NOTE: overload & underload should be called continously as long as the 
        //       condition still exist as viewed by the application

        // Arbitrator loading monitor 
        // 0 = normal, 1 = overload, -1 = underload
        virtual void    notifyLoading (int status) = 0;

        //
        //  MessageHandler methods
        //

        // returns whether the message has been handled successfully
        virtual bool    handleMessage (Message &in_msg) = 0;

        virtual Node *      getSelf () = 0;
        virtual bool        isJoined() = 0;
        virtual const char *toString () = 0;
        
        // TODO: doesn't look clean to have such low-level access
        virtual ArbitratorLogic *getLogic () = 0;       // get a copy of the arbitrator logic
        virtual VAST *      getVAST ()    = 0;          // obtain a reference of the VAST object
       
        // get the arbitrator's position in the virtual world
        virtual Position*   getPosition () = 0;

        // get the arbitrator's boundaries (edges with neighbor arbitrators)
        virtual vector<Vast::line2d> *getEdges () = 0;

        // get the boundary box for the arbitrator's Voronoi diagram
        virtual bool getBoundingBox (point2d& min, point2d& max) = 0;

        //
        // for statistical purposes
        //

        // obtain a copy of VAST node
        //virtual vast *  get_vastnode () = 0;

        // get a list of objects I own
        //virtual map<obj_id_t, bool> &get_owned_objs () = 0;

        // get a list of neighboring arbitrators
        //virtual map<id_t, Node> &get_arbitrators () = 0;
        

        
    };

} // end namespace Vast

#endif // #define VASTATE_ARBITRATOR_H
