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
 * VASTnet.h -- generic network layer (virtual class)
 *
 *
 */

#ifndef _VAST_Network_H
#define _VAST_Network_H

// whether to build with the ACE library
// (which supports real network transmission)
//#define ACE_DISABLED    

#include "VASTTypes.h"
#include "VASTBuffer.h"
#include <map>
#include <vector>

#define GATEWAY_DEFAULT_PORT    (1037)          // default port for gateway

#define NET_ID_UNASSIGNED       (0)             // default ID for yet assigned ID
#define NET_ID_RELAY            (1)             // default ID for gateway node

#define NET_ID_NEWASSIGNED      ((id_t)(-1))    // ID to signify new ID assignment

// grouping for locally-generated ID
#define ID_GROUP_VON_VAST       1
//#define ID_GROUP_VON_VASTATE    2

// # of elapsed seconds before removing a connection
#define TIMEOUT_REMOVE_CONNECTION   (10)
//#define TIMEOUT_REMOVE_CONNECTION   (3000)

// # of seconds before a connection cleanup is called (don't want to do it too often)
#define TIMEOUT_CONNECTION_CLEANUP  (1)

// # of seconds before an ID request is sent
#define TIMEOUT_ID_REQUEST          (5)

namespace Vast {

    // common message types
    typedef enum
    {
        DISCONNECT = 0,         // disconnection without action: leaving overlay or no longer overlapped
        //IPMAPPING,              // mapping between msggroup and IP addresses (stored at MessageQueue layer)
        //ID_REQUEST              // check validity of self-determined ID or assignment of ID
    } VASTnetMessage;

    // simple structure for a queued message
    typedef struct 
    {
        id_t        fromhost;
        timestamp_t recvtime;
        Message    *msg;
    } QMSG;

    // header used by all VAST messages
    typedef struct 
    {
        uint32_t type     : 2;     // type of message (0: ID request; 1: ID assignment; 3: handshake; 4: regular) 
        uint32_t msg_size : 30;    // size of message
    } VASTHeader;

    // common message types
    typedef enum
    {
        ID_REQUEST = 0,     // requesting a new ID & public IP detection
        ID_ASSIGN,          // assigning a new ID
        HANDSHAKE,          // handshake message (notify my hostID)
        REGULAR             // regular message 

    } VASTHeaderType;

    class EXPORT VASTnet
    {
    public:

        // initialize the network layer with a number of network entry points
        // if no entries are given then this host is assumed to be a well-known entry
        VASTnet ();

        virtual ~VASTnet ();

        // starting and stopping the network service
        virtual void start ();
        virtual void stop ();
       
        // get current physical timestamp
        virtual timestamp_t getTimestamp () = 0;

        // get IP address from host name
        virtual const char *getIPFromHost (const char *hostname) = 0;

        // check the validity of an IP address, modify it if necessary
        // (for example, translate "127.0.0.1" to actual IP)
        virtual bool validateIPAddress (IPaddr &addr) = 0;

        // set bandwidth limitation to this network interface (limit is in Kilo bytes / second)
        virtual void setBandwidthLimit (bandwidth_t type, size_t limit) {}

        // record the HostID obtained from a remote Relay
        void registerHostID (id_t my_id, bool is_public);

        // whether we have joined the overlay successfully and obtained a network ID
        bool isJoined ();

        // get the IP address of current host machine
        Addr &getHostAddress ();

        // send messages to some nodes, note that everything in msg will be preserved (from, data, targets)
        // returns number of bytes sent
        size_t sendMessage (id_t target, Message &msg, bool reliable = true, VASTHeaderType type = REGULAR);

        // obtain next message in queue
        // return pointer to Message, or NULL for no more message
        Message* receiveMessage (id_t &fromhost);

        // process an incoming raw message (decode its header)
        // returns the remote host's ID, or NET_ID_UNASSIGNED for failures
        id_t processRawMessage (VASTHeader &header, const char *msg, id_t remote_id, IPaddr *actual_IP = NULL, void *handler = NULL);

        // store an incoming message for processing by message handlers
        // return the number of bytes stored
        size_t storeRawMessage (id_t fromhost, Message *msg); 

        // send out all pending messages to each host
        // return number of bytes sent
        virtual size_t flush (bool compress = false);

        // store hostID -> Address mapping        
        void storeMapping (const Addr &address);

        // check if connected with the node
        bool isConnected (id_t id);
       
        // return whether this host has public IP or not
        bool isPublic ();

        // get how many ticks exist in a second (for stat reporting)
        //int getTickPerSecond ();

        // set how many ticks exist in a second (for stat reporting)
        void setTimestampPerSecond (int ticks);

        // get how many timestamps (as returned by getTimestamp) is in a second 
        timestamp_t getTimestampPerSecond ();

        // check if a target is connected, and attempt to connect if not
        bool validateConnection (id_t id);

        // get a list of currently active connections' remote id and IP addresses
        std::map<id_t, Addr> &getConnections ();

        // add entry points to respository
        void addEntries (std::vector<IPaddr> &entries);

        // get a list of entry points on the overlay network
        std::vector<IPaddr> &getEntries ();

        // get a specific address by id
        Addr &getAddress (id_t id);

        // obtain a HostID based on public IP + port for entry point hosts
        //                    or on public IP + port + number for non-entry hosts;
        static id_t resolveHostID (const IPaddr *addr);

        // obtain the assigned number from a HostID;
        static id_t resolveAssignedID (id_t host_id);

        // obtain the port portion of the ID
        static id_t resolvePort (id_t host_id);

        // obtain a unique NodeID, given one of the ID groups
        id_t getUniqueID (int id_group = 0);

        // get hostID for myself
        id_t getHostID ();

        // if I'm an entry point on the overlay
        bool isEntryPoint (id_t id);

        // extract the ID group from an ID
        static int extractIDGroup (id_t);

        // extract the host IP from an ID
        static int extractHost (id_t);

        // accept or assign ID for newly joined remote host (depend on whether public IP exists or not)
        // also send back the remote host's ID if it's newly assigned
        id_t processIDRequest (Message &msg, IPaddr *actualIP);

        // store an incoming assignment of my ID
        // returns the newly assigned ID
        id_t processIDAssignment (Message &msg);

        // send a handshake message to a newly established connection
        void sendHandshake (id_t target);

        // decode the remote host's ID or assign one if not available
        id_t processHandshake (Message &msg);

        //
        // stat collection functions
        //

        // obtain the tranmission size by message type, default is to return all types
        size_t getSendSize (msgtype_t msgtype = 0);
        size_t getReceiveSize (msgtype_t msgtype = 0);
        
        // zero out send / recv size records
        void   resetTransmissionSize ();

        // record which other IDs belong to the same host
        // TODO: added due to the two networks / host design in VASTATE
        //       a cleaner way?
        void recordLocalTarget (id_t target);

    protected:

        // connect or disconnect a remote node (should check for redundency)
        // returns (-1) for error, (0) for success
        virtual int     connect (id_t target) = 0;
        virtual int     disconnect (id_t target) = 0;

        // send an outgoing message to a remote host
        // return the number of bytes sent
        virtual size_t send (id_t target, char const *msg, size_t size, bool reliable = true) = 0;

        // receive an incoming message
        // return pointer to next QMSG structure or NULL for no more message
        virtual QMSG *receive () = 0;

        // store a message into priority queue
        // returns success or not
        virtual bool store (QMSG *qmsg) = 0;

        // methods to keep track of active connections (associate ID with connection stream)
        // returns NET_ID_UNASSIGNED if failed
        virtual id_t register_conn (id_t id, void *stream) = 0;
        virtual id_t unregister_conn (id_t id) = 0;
                
        // clear up send queue's content
        virtual size_t clearQueue () {return 0;}

        // periodic cleanup of inactive connections
        void cleanConnections ();

        // remove a single connection cleanly
        bool removeConnection (id_t target);

        // update send/recv size statistics
        // type: 1 = send, type: 2 = receive
        void updateTransmissionStat (id_t target, msgtype_t msgtype, size_t total_size, int type);

        // unique id for the VAST node using this network interface
        id_t                            _id;
        Addr                            _addr;

        // whether the current interface is working
        bool                            _active;

        // whether network is properly binded to listen for incoming messages
        // TODO: replaced by _active alone?
        bool                            _binded;

        // whether this host has public IP or not
        bool                            _is_public;             

        // entry points for joining the overlay network
        std::vector<IPaddr>             _entries;

        // map of active connections
        std::map<id_t, void *>          _id2conn;

        // map from nodeIDs to IP addresses
        std::map<id_t, Addr>            _id2addr;

        // map from nodeIDs to last time the connection was accessed
        // used to determine connection cleanup / removals
        // TODO: combine the connection/address/time mapping?
        std::map<id_t, timestamp_t>     _id2time;
        
        // timeouts
        timestamp_t                     _timeout_IDrequest;     // timeout for sending ID request
        timestamp_t                     _timeout_cleanup;   // # of ticks before cleanupConnections is called

        // buffer for incoming/outgoing messages
        // TODO: combine the TCP & UDP buffers?
        std::map<id_t, VASTBuffer *>    _sendbuf_TCP;
        std::map<id_t, VASTBuffer *>    _sendbuf_UDP;        
        std::multimap<byte_t, QMSG *>   _msgqueue;          // priority queue for incoming messages
        Message                        *_recvmsg;           // next available message for processing
        
        // counter for assigning unique ID
        std::map<int, id_t>             _IDcounter;         // counter for assigning IDs by this host

    
        //
        // logical / physical time management
        //
        //int                             _tick_persec;       // how many ticks per second
        timestamp_t                       _sec2timestamp;     // ratio converting seconds to timestamp as returned by getTimestamp () 


        // 
        // stat collection
        //

        // accumulated send/receive size
        size_t _sendsize, _recvsize;

        // accumulated send/receive size stored by type
        std::map<msgtype_t, size_t>     _type2sendsize,
                                        _type2recvsize;

        std::map<id_t, bool>            _local_targets;     // send/receive targets on the same host
    };

} // end namespace Vast

#endif // VAST_NETWORK_H
