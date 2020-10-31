/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2005-2011 Shun-Yun Hu  (syhu@ieee.org)
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
 *  VASTCallback -- an abstract interface used by VAST to notify application of messages
 *
 *  history 2010/09/01 init
 *
 */



#ifndef _VAST_CALLBACK_H
#define _VAST_CALLBACK_H

#include "Config.h"
#include "VASTTypes.h"

using namespace std;

namespace Vast
{
    class EXPORT VASTCallback
    {

    public:
        virtual ~VASTCallback () {}

        // process a message from the network by current node, 
        // returns whether we can process more
        virtual bool processMessage (Message &msg) = 0;

        // process a plain socket message from the network by current node, 
        // returns whether we can process more
        virtual bool processSocketMessage (id_t socket, const char *msg, size_t size) = 0;

        // perform some per-second tasks
        virtual void performPerSecondTasks (timestamp_t now) = 0;

        // perform some per-tick tasks
        virtual void performPerTickTasks () = 0;

        // notify of successful connection with gateway
        virtual void gatewayConnected (id_t host_id) = 0;

        // notify of successful disconnection from gateway
        virtual void gatewayDisconnected () = 0;

        // notify the successful join of the VAST node
        virtual void nodeJoined (VAST *vastnode) = 0;

        // notify the successful leave of the VAST node
        virtual void nodeLeft () = 0;

    };

} // namespace Vast

#endif

