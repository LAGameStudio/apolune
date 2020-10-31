/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2006-2007 Shun-Yun Hu (syhu@yahoo.com)
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
 *  MessageHandler.h -- interface to extend message processing functions
 *
 *  history:    2006/07/19
 *              2007/01/10  allow chaining of an already chained message handler
 *              2008/08/21  re-design to support MessageQueue class
 *   
 */

#ifndef _VAST_MessageHandler_H
#define _VAST_MessageHandler_H

#include "VASTnet.h"

using namespace std;

// TODO: a better place to define this?
// default IDs
#define MSG_GROUP_VAST_RELAY            1
#define MSG_GROUP_VAST_MATCHER          2
#define MSG_GROUP_VAST_CLIENT           3
#define MSG_GROUP_VASTATE_AGENT         4
#define MSG_GROUP_VASTATE_ARBITRATOR    5

namespace Vast 
{        
    class EXPORT MessageHandler
    {
    
    friend class MessageQueue;

    public:
        // init by specifying the unique group ID (defined by application)
        MessageHandler (id_t group_id)
            :_msggroup ((byte_t)group_id), _msgqueue (NULL), _net (NULL)
        {
        }

        virtual ~MessageHandler () {}

        // perform initialization tasks for this handler (optional)
        // NOTE that all internal variables (such as unique ID) have been set at this point
        virtual void initHandler () {}

        // returns whether the message has been handled successfully
        virtual bool handleMessage (Message &in_msg) = 0;

        // perform some tasks after all messages have been handled (default does nothing)        
        virtual void postHandling () {}

        // add additional handlers to the same message queue
        bool addHandler (MessageHandler *handler);

        // remove handlers added from current handler
        bool removeHandler (MessageHandler *handler);

    protected:

        // send a message to some target nodes in the current group (same msggroup)
        int sendMessage (Message &msg, vector<id_t> *failed_targets = NULL);

        // notify the network layer of nodeID -> Address mapping 
        bool notifyMapping (id_t node_id, Addr *addr);

        // get a specific address by nodeID
        Addr &getAddress (id_t id);

        // used by MessageQueue only to store pointer to the MessageQueue
        // returns the unique ID for this handler
        id_t setQueue (void *msgqueue, VASTnet *net);

        byte_t       _msggroup;         // unique ID to identify which message group the handler belongs
        void *       _msgqueue;         // pointer to message queue (in case to register new handlers from within)    
        VASTnet *    _net;              // pointer to network interface
    };

} // end namespace Vast

#endif // VAST_MSGHANDLER_H
