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
 *  Agent.h -- VASTATE Agent class (handles everything related to an Agent)
 *
 *  ver 0.1 (2006/07/18)
 *   
 */

#ifndef VASTATE_AGENT_H
#define VASTATE_AGENT_H

#include "VASTATETypes.h"
#include "AgentLogic.h"

namespace Vast 
{  
    class EXPORT Agent : public MessageHandler
    {
    public:
        Agent ()
            :MessageHandler (MSG_GROUP_VASTATE_AGENT)
        {
        }
        
        virtual ~Agent () 
        {
        }

        //
        // Agent interface
        //
        
        // login/logout the system (to gateway arbitrator)
        virtual bool    login (char *URL, const char *auth, size_t auth_size) = 0;
        virtual bool    logout () = 0;

        // send a message to gateway arbitrator (i.e., server)
        virtual bool    send (Message &msg) = 0;

        // AOI related functions
        virtual void        setAOI (length_t radius) = 0;
        virtual length_t    getAOI () = 0;

        // enters a particular position in the system
        virtual bool    join (Position &pos) = 0;                
        
        // quit system
        virtual void    leave () = 0;

        // moves to a new position
        virtual void    move (Position &pos) = 0;

        virtual Event * createEvent (msgtype_t event_type) = 0;

        // send an Event to the current managing arbitrator
        virtual bool    act (Event *e) = 0;

        //
        //  MessageHandler methods
        //

        // returns whether the message has been handled successfully
        virtual bool    handleMessage (Message &in_msg) = 0;

        //
        // accessors
        //

        virtual Node *      getSelf ()    = 0;
        virtual bool        isJoined()    = 0;
        virtual const char *toString ()   = 0;
        
        // check if the login is successful
        virtual bool        isAdmitted () = 0;

        // TODO: doesn't look clean to have such low-level access
        virtual AgentLogic *getLogic ()   = 0;      // get a copy of the agent logic
        virtual VAST *      getVAST ()    = 0;      // obtain a reference of the VAST object

        virtual Node *      getCurrentArbitrator () = 0;   // learn of the current arbitrator managing this region
    };
    
} // end namespace Vast

#endif // #define VASTATE_AGENT_H

