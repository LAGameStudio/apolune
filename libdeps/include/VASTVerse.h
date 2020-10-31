/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2005-2010 Shun-Yun Hu (syhu@yahoo.com)
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
 *  VASTVerse.h -- VAST factory (to create actual instance of VAST)
 *
 *  history:    2005/04/14      ver 0.1
 *              2009/04/28      refactored for SPS-based VAST
 *              2010/01/28      IDGenerator moved into VASTnet
 */


#ifndef VASTVERSE_H
#define VASTVERSE_H

#include "Config.h"
#include "VAST.h"           // provides spatial publish / subscribe (SPS)
#include "VASTRelay.h"      // provides physical coordinate, IP address, and public IP


namespace Vast
{
    typedef enum 
    {
        VAST_NET_EMULATED = 1,
        VAST_NET_EMULATED_BL,
        VAST_NET_ACE
    } VAST_NetModel;

    struct VASTPara_Net
    {
        VAST_NetModel   model;          // network model
        bool            is_entry;       // whether current node is an entry point to the overlay (assigns ID, determine physical coordinates)
	    bool            is_relay;       // whether this node should join as a relay (may not succeed depend on public IP is available)
        bool            is_matcher;     // whether this node should join as a candidate matcher (need to be a relay as well)       
        bool            is_static;      // whether static partitioning is used
        unsigned short  port;           // default port to use 
        Position        phys_coord;     // default physical coordinate (optional)
        Position        matcher_coord;  // default matcher join coordinate (optional)        
        int             client_limit;   // max number of clients connectable to this relay
        int             relay_limit;    // max number of relays each node maintains
        int             overload_limit; // max number of subscriptions at each matcher
        int             conn_limit;     // connection limit
        int             step_persec;    // step/ sec (network layer)
        size_t          send_quota;     // upload quota (bandwidth limit)
        size_t          recv_quota;     // download quota (bandwidth limit)        
    };

    struct VASTPara_Sim
    {
        int     loss_rate;      // packet loss rate
        int     fail_rate;      // node fail rate
        bool    with_latency;   // latency among nodes' connection
    };

    // the main factory to create VAST nodes and join the overlay
    //
    // NOTE that VASTVerse creates some internal message handlers, including:
    //
    //          1) ID generator                 local ID: 0
    //          2) VASTRelay finder              local ID: 1
    //          3) VAST node (client & relay)   local ID: 2
    //
    //      so other message handlers have to use getUniqueID () to obtain
    //      a globally unique handlerID first before creating the message handler
    //      and hook to an existing handler (such as VAST)
    //
    
    // TODO: need to cleanup current implementation (too ugly for hiding internal classes from user)

    class EXPORT VASTVerse
    {
    public:

        // specify a number of entry points (hostname / IP) to the overlay, 
        // also the network & simulation parameters
        VASTVerse (vector<IPaddr> &entries, VASTPara_Net *netpara, VASTPara_Sim *simpara);
        ~VASTVerse ();

        // whether we'ved connect with the closest relay
        // (ready to create client)
        bool isLogined ();
        
        // NOTE: to run a gateway-like entry point only, there's no need to call
        //       createVASTNode (), as long as isLogined () returns success
        
        // create & destroy a VASTNode
        // currently only supports one per VASTVerse
        bool createVASTNode (const IPaddr &gateway, Area &area, layer_t layer);    
        bool destroyVASTNode (VAST *node);

        // obtain a reference to the created VASTNode
        VAST *getVASTNode ();
       
        // TODO: support this function? so clients can enter a different world
        //       without having to destroyClient?
        // bool switchWorld (IPaddr &gateway);

        // advance one time-step for all nodes to process messages  
        // input time budget for this tick in millisecond, specify 0 for unlimited budget
        // returns time left in millisecond, 0 for no more time, (-1) for unlimited budget
        // NOTE: currently only (-1) would return
        int     tick (int time_budget = 0);

        // move logical clock forward (mainly for simulation purpose, but also records per-second tasks)
        void    tickLogicalClock ();

        // stop operations on this node
        void    pause ();

        // resume operations on this node
        void    resume ();

        // obtain access to Voronoi class of the matcher (usually for drawing purpose)
        // returns NULL if matcher does not exist on this node
        Voronoi *getMatcherVoronoi ();

        // obtain the matcher's adjustable AOI radius, returns 0 if no matcher exists
        Area *getMatcherAOI ();

        // obtain the number of active connections at this node
        int getConnectionSize ();

        // obtain the tranmission size by message type, default is to return all types
        StatType &getSendStat (bool interval_only = false);
        StatType &getReceiveStat (bool interval_only = false);

        // reset stat collection for a particular interval, however, accumulated stat will not be cleared
        void    clearStat ();

        // record nodeID on the same host
        void    recordLocalTarget (id_t target);

        // translate a string-based address into Addr object
        static Addr *translateAddress (const string &addr);

        // read default parameters from command line
        //bool parseCommandLine (const char *cmd, 

    private:

        // create & destroy a VAST client
        VAST *   createClient (const IPaddr &gateway);
        bool     destroyClient (VAST *node);

        // obtain & destory a Voronoi object
        //Voronoi *createVoronoi ();
        //bool     destroyVoronoi (Voronoi *v);

        NodeState           _state;
        VASTPara_Net        _netpara;
        VASTPara_Sim        _simpara;
        void               *_pointers;      // pointers to VASTPointer
        bool                _logined;

        vector<Subscription> _vastinfo;     // info about a VASTNode to be created
        vector<IPaddr>      _entries;       // entry points for the overlay

        // stat collection class
        timestamp_t         _next_periodic; // next timestamp when per-second task is executed
        StatType            _sendstat;      // stat on send size
        StatType            _recvstat;      // stat on recv size

        StatType            _sendstat_interval;      // stat on send size per interval
        StatType            _recvstat_interval;      // stat on recv size per interval

        StatType            _connstat;      // stat on connection size

        size_t              _lastsend;      // last accumulated send bytes
        size_t              _lastrecv;      // last accumulated recv bytes
    };

} // end namespace Vast

#endif // VASTVerse_h
