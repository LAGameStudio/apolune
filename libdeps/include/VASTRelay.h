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
 *  VASTRelay.h  
 *
 *  locate my physical coordinate on a network topology through relay nodes
 *  also join the relay mesh by connecting with the closest physical relay
 *
 *  history 2009/04/17      starts 1st implementation
 *          2010/02/09      change name from Topology to VASTRelay
 *
 */



#ifndef _VAST_VASTRelay_H
#define _VAST_VASTRelay_H

#include "Config.h"
#include "VASTTypes.h"
#include "MessageHandler.h"
#include "VAST.h"               // for VASTMessage
//#include "Vivaldi.h"

// number of seconds before a new round of queries is sent for neighbors' coordinates
const int TIMEOUT_COORD_QUERY = 30;

// number of seconds before a keepalive message is sent to my current relay (by a client)
// IMPORTANT NOTE: this has to be shorter than network inactivity connection removal
const int KEEPALIVE_RELAY = 5;

// timeout for re-sending relay query
const int TIMEOUT_RELAY_QUERY = 5;

// timeout for re-sending join request to a relay
const int TIMEOUT_RELAY_JOIN  = 5;

// number of pings sent concurrently to neighbor relays
//#define MAX_CONCURRENT_PING          (5)
const int MAX_CONCURRENT_PING = 5;

// maximum number of queries sent to obtain new physical coordinate
const int MAX_RELAY_QUERIES     = 5;

// NOTE the below parameters need to be fine-tuned & tested
//      currently local tests reveal that it'll take about 3-7 ping-pong to converge a physical coordinate upon join (~ 2 seconds)
#define RELAY_CONSTANT_ERROR      (0.7f)     // affects how heavy the local error will count in updating the local error (moving average) 
#define RELAY_CONSTANT_FRACTION   (1.0f)     // % of weight the position will move (given the difference between estimated / actual distance, larger = faster movement)
#define RELAY_TOLERANCE           (0.5f)     // how much local error is considered okay
#define RELAY_DEFAULT_ERROR       (1.0f)     // default value for local error


using namespace std;

namespace Vast
{

    /*
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

    } RELAY_Message;
    */

    // this is an export class so physical coordinates can be obtained externally
    class EXPORT VASTRelay : public MessageHandler
    {

    public:

        // constructor for physical topology class, 
        // may supply an artifical physical coordinate
        VASTRelay (bool as_relay, size_t client_limit = 0, size_t relay_limit = 0, Position *phys_coord = NULL);

        ~VASTRelay ();
        
        // obtain my physical coordinate, returns NULL if not yet obtained
        Position* getPhysicalCoordinate ();

        // whether the current node is joined (find & connect to closest relay)
        bool isJoined ();

        // whether the current node is a relay
        bool isRelay ();

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
        // relay management 
        //

        // connect to the closest, currently known relay
        bool joinRelay (Node *relay = NULL);      

        // find the relay closest to a position, which can be myself
        // returns the relay's Node info
        Node *closestRelay (Position &pos);

        // find next available relay
        Node *nextRelay ();
               
        // add a newly learned node as a Relay
        void addRelay (Node &relay);

        // remove a relay no longer valid
        void removeRelay (id_t id);

        // send message to a relay, remove the relay if send isn't successful
        // return # of targets sent successfully
        int sendRelay (Message &msg);

        // remove non-useful relays
        void cleanupRelays ();

        // send to target a list of known relays, return # of relays sent
        int sendRelayList (id_t target, int limit = 0);

        // notify neighboring relay of my updated physical coordinate
        bool notifyPhysicalCoordinate ();


        //
        // client management
        //

        // specify we've joined
        bool setJoined (id_t relay_id = NET_ID_UNASSIGNED);

        // accept a client
        void addClient (id_t from_host, Node &client);

        // remove a client no longer connected
        void removeClient (id_t id);

        // forward a message meant for client 'sub_id', to an actual client host
        // if host_id is not provided then the message is simply removed
        // returns the number of processed messages (forward or delete)
        int forwardMessage (id_t sub_id, id_t host_id = NET_ID_UNASSIGNED);

        //
        //  physical coordinate discovery
        //

        // send a message to a remote host in order to obtain round-trip time
        bool ping (bool curr_relay_only = false);

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
        Node       *_curr_relay;        // pointer to closest relay (could be myself)
        Node       *_contact_relay;     // pointer to the relay we try to join
        Position    _temp_coord;        // still in-progress coordinate
        float       _error;             // error estimate for the local node

        bool        _as_relay;          // whether I will act as a relay
        NodeState   _state;             // current state

        // counter for timeout	
        timestamp_t _timeout_ping;      // countdown counter to send query
        timestamp_t _timeout_query;     // timeout for querying the initial relay
        timestamp_t _timeout_join;      // timeout for joining a relay
        int         _ping_all_count;    // whether to ping all relays

        int         _request_times;     // # of times we've sent out PING requests

        map<id_t, Node> _relays;        // list of contact neighbors
        multimap<double, Node *> _dist2relay;    // pointers to known relays (neighbors or otherwise), listed / queried by distance        
        
        map<id_t, Node> _clients;       // list of accepted clients to this relay
        map<id_t, id_t> _sub2client;    // mapping from subscription ID to client's hostID

        size_t          _relay_limit;   // # of relays this node keeps
        size_t          _client_limit;  // number of clients I can accomodate, could vary depending on load

        map<id_t, timestamp_t> _pending; // list of pending PING requests sent & sent time
        
        multimap<id_t, Message *> _queue;    // forward messages that can yet be sent (mapping not yet received)
        map<id_t, timestamp_t> _queue_time;  // time a particular message is queued
	};

} // namespace Vast

#endif

