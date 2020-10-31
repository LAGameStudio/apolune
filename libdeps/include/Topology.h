/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2005-2009 Shun-Yun Hu  (syhu@yahoo.com)
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
 *  Topology.h  
 *
 *  locate my physical coordinate on a network topology through relay nodes
 *
 *  history 2009/04/17      starts 1st implementation
 *
 */



#ifndef _VAST_Topology_H
#define _VAST_Topology_H

#include "Config.h"
#include "VASTTypes.h"
#include "MessageHandler.h"
#include "Vivaldi.h"

// number of seconds before a new round of queries is sent for neighbors' coordinates
const int TIMEOUT_COORD_QUERY = 30;
const int TIMEOUT_RELAY_QUERY = 10;
const int TIMEOUT_RELAY_JOIN  = 10;

#define MAX_CONCURRENT_PING          (5)

#define TOPOLOGY_CONSTANT_ERROR      (0.1f)     
#define TOPOLOGY_CONSTANT_FRACTION   (0.1f)      
#define TOPOLOGY_TOLERANCE           (0.5f)     // how much local error is considered okay
#define TOPOLOGY_DEFAULT_ERROR       (1.0f)     // default value for local error


using namespace std;

namespace Vast
{

    typedef enum 
    {
        REQUEST = 1,            // request for relays
        RELAY,                  // notifying an existing relay        
        PING,                   // query to measure latency
        PONG,                   // reponse to PING
        PONG_2,                 // reponse to PONG
        RELAY_QUERY,            // find closest relay
        RELAY_QUERY_R,          // response to closest relay query
        RELAY_JOIN,             // attach to the physically closest relay
        RELAY_JOIN_R,           // response to JOIN request

    } TOPOLOGY_Message;

    class EXPORT Topology : public MessageHandler
    {

    public:

        // constructor for physical topology class, 
        // may supply an artifical physical coordinate
        Topology (size_t client_limit = 0, size_t relay_limit = 0, Position *phys_coord = NULL);

        ~Topology ();
        
        // obtain my physical coordinate, returns NULL if not yet obtained
        Position* getPhysicalCoordinate ();

        // whether the current node is joined (find & connect to closest relay)
        bool isJoined ();

        // obtain the ID of my relay node
        id_t getRelayID ();

    private:

        // perform initialization tasks for this handler (optional)
        // NOTE that all internal variables (such as handler_no) have been set at this point
        void initHandler ();

        // returns whether the message was successfully handled
        bool handleMessage (Message &in_msg);

        // performs tasks after all messages are handled
        void postHandling ();

        //
        // connect to physically closest relay
        //

        // connect to the closest, currently known relay
        bool joinRelay (Node *relay = NULL);      

        // find the relay closest to a position, which can be myself
        // returns the relay's Node info
        Node *closestRelay (Position &pos);
               
        // add a newly learned node as a Relay
        void addRelay (Node &relay);

        // remove a relay no longer valid
        void removeRelay (id_t id);

        // remove non-useful relays
        void cleanupRelays ();

        // send to target a list of known relays, return # of relays sent
        int sendRelays (id_t target, int limit = 0);

        //
        //  physical coordinate discovery
        //

        // send a message to a remote host in order to obtain round-trip time
        bool ping ();

        // response to a PING message
        bool pong (id_t target, timestamp_t querytime, bool first = false);


        //
        //  process functions
        //

        // re-calculate my coordinate, 
        // given a neighbor j's position xj & error estimate ej
        void vivaldi (float rtt, Position &xi, Position &xj, float &ei, float &ej);

        // randomly select some items from a set
        bool randomSet (int size, int total, vector<bool> &random_set);

        //
        //  private variables
        //
        Node        _self;              // info about myself
        Node       *_curr_relay;        // pointer to closest relay
        Position    _temp_coord;        // still in-progress coordinate
        float       _error;             // error estimate for the local node

        NodeState   _state;             // current state

        // counter for timeout	
        int         _timeout_ping;      // countdown counter to send query
        int         _timeout_query;      // timeout for querying the initial relay
        int         _timeout_join;      // timeout for joining a relay

        int         _request_times;     // # of times we've sent out PING requests

        map<id_t, Node> _relays;        // list of contact neighbors
        multimap<double, Node *> _dist2relay;    // pointers to known relays (neighbors or otherwise), listed / queried by distance        
        
        map<id_t, Node> _accepted;  // list of accepted clients to this relay

        size_t          _relay_limit;   // # of relays this node keeps
        size_t          _client_limit;  // number of clients I can accomodate, could vary depending on load

        map<id_t, bool> _pending;       // list of pending PING requests sent        
	};

} // namespace Vast

#endif
