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
 *  AgentLogic.h -- interface for handling logics of a Agent
 *
 *  ver 0.1 (2006/07/18)
 *   
 */

#ifndef VASTATE_AGENT_LOGIC_H
#define VASTATE_AGENT_LOGIC_H

#include "VASTATETypes.h"

namespace Vast 
{  
    class EXPORT AgentLogic
    {
    public:
        AgentLogic ()
            : _self (NULL)
        {
        }

        virtual ~AgentLogic () {}

        // callback - learn about the addition/removal of objects
        virtual void onCreate (Object &obj, bool is_self = false) = 0;
        virtual void onDestroy (const obj_id_t &obj_id) = 0;
        
        // callback - learn about state changes of known AOI objects
        virtual void onUpdate (const obj_id_t &obj_id, int index, const void *value, size_t length, version_t version) = 0;
        virtual void onMove (const obj_id_t &obj_id, const Position &newpos, version_t version) = 0;  

        // callback - learn about the result of login
        virtual void onAdmit (const char *status, size_t size) = 0;
        
        // callback - an app-specific message from gateway arbitrator (server) to agent
        virtual void onMessage (Message &in_msg) = 0;

        // store access to Agent class for the callback to perform Agent-specific tasks
        void registerInterface (void *);
        void unregisterInterface ();

        // obtain a pointer to the agent itself
        Node *getSelf ();

    protected:
        // reference to Agent class to perform actions when responding
        void *_agent;

        // pointer about myself (easier reference into _agent's self object)
        Node *_self;
    };
    
} // end namespace Vast

#endif // #define VASTATE_AGENT_LOGIC_H

