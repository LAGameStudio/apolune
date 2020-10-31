/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2005-2009 Shun-Yun Hu (syhu@yahoo.com)
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
 *  MessageQueue.h -- a queue to hook message handlers for processing incoming messages
 *                    also allows sending messages to a given target
 *
 *                    An incoming message will be handled locally if the message targets
 *                    include nodeID mapped to the local hostID. Otherwise the message 
 *                    may be forwarded to another host if such nodeID -> hostID mapping exist
 *              
 *                    It is thus important for the application to notify MessageQueue 
 *                    (via MessageHandler), the mapping between nodeID & hostID
 */

#ifndef _VAST_MessageQueue_H
#define _VAST_MessageQueue_H

#include "VASTnet.h"
#include "MessageHandler.h"
#include <map>


using namespace std;

namespace Vast 
{

    class EXPORT MessageQueue
    {
    public:
        MessageQueue (VASTnet *net)
            :_net (net), 
             _default_host (NET_ID_UNASSIGNED)
        {       
            // automatically start off the network
            _net->start ();
        }

        ~MessageQueue ()
        {            
            // TODO: check to remove all currently active handlers?

            // shutdown all network connections
            _net->stop ();
        }

        // store message handler for a particular group of messages (specified by group_id)
        // return false if handler already exists
        bool registerHandler (MessageHandler *handler);

        // unregister an existing handler
        bool unregisterHandler (MessageHandler *handler);

        // notify a remote target node's network layer of nodeID -> Address mapping
        // target can be self id or 0 to indicate storing locally
        bool notifyMapping (id_t node_id, Addr *addr);
        
        // send a message to some target nodes  
        // an optional list of failed targets can be returned for those targets sent unsuccessfully
        // return the # of targets with valid connections (so message can probably be delivered)
        int sendMessage (Message &msg, vector<id_t> *failed_targets = NULL);

        // replace current host_id with new one
        //void registerHostID (id_t my_id, id_t default_host = 0);

        // obtain a unique ID generated on this host, based on an optional user-specified group ID
        //id_t getUniqueID (int group_id = 0, bool is_gateway = false);

        // get a specific address by nodeID
        Addr &getAddress (id_t id);

        // perform external and internal message processing, and post handling tasks for each handler
        // input is time interval in ms available for each tick
        void tick ();

        // store default route for unaddressable targets
        void setDefaultHost (id_t default_host);

    private:

        // process all currently received messages (invoking previously registered handlers)
        // return the number of messsages processed
        int processMessages ();

        VASTnet *                   _net;           // network interface to send/recv messages
        map<id_t, MessageHandler *> _handlers;      // map from msggroup to handler        
           
        // map from nodeID to physical host's hostID (representing a network end-point)
        // TODO: make this function-queryable
        map<id_t, id_t>         _id2host;          

        // counter for local ID assignments
        map<int, id_t>          _IDcounters;

        // default route if target cannot be resolved
        id_t                    _default_host;
    };

} // end namespace Vast

#endif
