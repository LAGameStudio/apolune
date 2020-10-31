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
 *  ArbitratorLogic.h -- interface for handling the logics for an arbitrator
 *
 *  ver 0.1 (2006/07/18)
 *   
 */

#ifndef VASTATE_ARBITRATOR_LOGIC_H
#define VASTATE_ARBITRATOR_LOGIC_H

#include "VASTATETypes.h"

namespace Vast 
{  

    class EXPORT ArbitratorLogic
    {
    public:
        virtual ~ArbitratorLogic () {}
        
        // callback - for handling a login request with authentication message
        virtual bool onLogin (id_t from_id, const char *auth, size_t size) = 0;

        // callback - for handling a logout request with authentication message
        virtual bool onLogout (id_t from_id) = 0;

        // callback - an app-specific message from agent to gateway server
        virtual void onMessage (Message &in_msg) = 0;

        // callback -  for receiving a remote Event (Arbitrator only)
        virtual bool onEvent (id_t from_id, Event &e) = 0;
        virtual bool onMoveEvent (id_t from_id, const obj_id_t &obj_id, const Position &newpos) = 0;
        
        // callback - by remote Arbitrator to create or initialize objects        
        virtual void onCreate       (Object *obj, bool initializable) = 0;
        virtual void onDestroy      (const obj_id_t &obj_id) = 0;
        
        // callback - by remote Arbitrator to notify their Object states have changed
        virtual void onUpdate (const obj_id_t &obj_id, int index, const void *value, size_t length, version_t version) = 0;
        virtual void onMove (const obj_id_t &obj_id, const Position &newpos, version_t version) = 0;
        
        // callback - to learn about the creation of avatar objects
        virtual void onAgentEntering (Node &agent) = 0;
        virtual void onAgentLeaving (Node &agent) = 0;
        
        // callback - to process any non-triggered Arbitrator logic (perform actions not provoked by events)
        virtual void tick () = 0; 

        // store access to Arbitrator class
        // NOTE: the app-specific implementation of ArbitratorLogic must
        //       store a pointer to an 'arbitrator' Object as private variable
        void registerInterface (void *);
        void unregisterInterface ();

        // store access to Storage class
        // NOTE: the app-specific implementation of ArbitratorLogic must
        //       store a pointer to an 'Storage' Object as private variable
        //virtual void register_Storage (void *) = 0;

    protected:
        // reference to Arbitrator class to perform actions when responding
        void *_arbitrator;
    };

} // end namespace Vast

#endif // #define VASTATE_ARBITRATOR_LOGIC_H
