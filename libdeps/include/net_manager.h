/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2005-2011 Shun-Yun Hu (syhu@ieee.org)
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
 * net_manager.h -- interface for implemention-specific network functions 
 *                  (virtual class)
 *
 *      init:   2010-09-30
 */

#ifndef _VAST_NET_MANAGER_H
#define _VAST_NET_MANAGER_H

#include <map>
#include <vector>
#include "VASTTypes.h"
#include "VASTBuffer.h"

// TODO: move this to VASTnet? (but resolveHostID needs them)
#define NET_ID_UNASSIGNED       (0)             // default ID for yet assigned ID
#define NET_ID_RELAY            (1)             // default ID for gateway node

#define NET_ID_NEWASSIGNED      ((id_t)(-1))    // ID to signify new ID assignment

namespace Vast {

    // simple structure for a queued message
    class NetSocketMsg
    {
    public:
        NetSocketMsg ()
        {
            fromhost = 0;
            recvtime = 0;
            msg = NULL;
            size = 0;
        }

        // copy constructor
        NetSocketMsg (const NetSocketMsg &m)
        {
            fromhost = m.fromhost;
            recvtime = m.recvtime;
            size = m.size;

            msg = new char[m.size];
            memcpy (msg, m.msg, size);
        }

        ~NetSocketMsg ()
        {
            if (msg != NULL)
            {
                delete[] msg;
                msg = NULL;
            }
        }

        id_t        fromhost;
        timestamp_t recvtime;
        char        *msg;
        size_t      size;
    };

    // information about a connection
    class ConnectInfo
    {
    public:
        ConnectInfo ()
        {
            stream = NULL;
            lasttime = 0;
            is_secure = false;
        }

        ConnectInfo (void *s, timestamp_t time, bool is_secure)
        {
            stream      = s;
            lasttime    = time;
            is_secure   = is_secure;
        }

        ConnectInfo (const ConnectInfo &c)
        {
            this->stream   = c.stream;
            this->lasttime = c.lasttime;
            this->is_secure = c.is_secure;
        }

        ConnectInfo &operator=(const ConnectInfo& c)
        {
            this->stream   = c.stream;
            this->lasttime = c.lasttime;
            this->is_secure = c.is_secure;
    
            return *this;
        }

        bool         is_secure;        // whether the stream is TLS-based
        void        *stream;        // stream object 
        timestamp_t  lasttime;      // last accessed time for this connection
    };

    // generic socket class
    class net_manager
    {
    public:

        // can be overwritten by sub-classes
        net_manager () {};
        virtual ~net_manager () {};
     
        //
        // basic services 
        //
        
        // starting and stopping the network service
        virtual void start ();
        virtual void stop ();

        // see if the network interface is active
        bool isActive ();

        // check if a certain host is connected
        bool isConnected (id_t target);
     
        // set my ID, return the previous ID
        id_t setID (id_t my_id);

        // obtain my ID
        id_t getID ();

        // obtain my address
        Addr &getAddress ();

        // get timestamps per second
        timestamp_t getTimestampPerSecond ();

        // get last access time of a connection
        timestamp_t getLastTime (id_t id);

        //
        // pure virtual, customizable methods (implementation-specific)
        //   

        // get current physical timestamp (milliseconds since start)
        virtual timestamp_t getTimestamp () = 0;

        // get IP address from host name
        virtual const char *getIPFromHost (const char *hostname) = 0;

        // obtain the IP / port of a remotely connected host
        virtual bool getRemoteAddress (id_t host_id, IPaddr &addr) = 0;

        // connect or disconnect a remote node (should check for redundency)
        virtual bool connect (id_t target, unsigned int host, unsigned short port, bool is_secure = false) = 0;
        virtual bool disconnect (id_t target) = 0;

        // send an outgoing message to a remote host, if 'addr' is provided, the message is sent as UDP
        // return the number of bytes sent
        virtual size_t send (id_t target, char const *msg, size_t size, const Addr *addr = NULL) = 0;

        // receive an incoming message
        // return pointer to valid NetSocketMsg structure or NULL for no messages
        virtual NetSocketMsg *receive () = 0;

        // change the ID for a remote host
        virtual bool switchID (id_t prevID, id_t newID) = 0;

        // perform a tick of the logical clock 
        virtual void tickLogicalClock () = 0;

        //
        // callback services called by specific network binding (e.g. ACE or emulation)
        //
        
        // store a message into priority queue
        // returns success or not
        virtual bool msg_received (id_t fromhost, const char *message, size_t size, timestamp_t recvtime, bool in_front = false) = 0;

        virtual bool socket_connected (id_t id, void *stream, bool is_secure) = 0;
        virtual bool socket_disconnected (id_t id) = 0;

        // 
        // static methods (tools for external classes)
        //

        // obtain a HostID based on public IP + port for entry point hosts
        //                    or on public IP + port + number for non-entry hosts;
        static id_t resolveHostID (const IPaddr *addr);

        // obtain the assigned number from a HostID;
        static id_t resolveAssignedID (id_t host_id);

        // extract the ID group from an ID
        static int extractIDGroup (id_t);

        // extract the host IP from an ID
        static int extractHost (id_t);

    protected:

        //
        // maintain functions
        //

        // unique id for the VAST node using this network interface
        id_t                            _id;
        Addr                            _self_addr;

        // whether the current interface is working
        bool                            _active;

        // map of active connections
        std::map<id_t, ConnectInfo>     _id2conn;

        // buffers of received messages
        std::vector<NetSocketMsg *>     _recv_queue;        // FIFO queue for incoming messages
        NetSocketMsg *                  _recvmsg;           // last received message

        //
        // logical / physical time management
        //
        timestamp_t                     _sec2timestamp;     // ratio converting seconds to timestamp (same unit returned by getTimestamp ())

    };
        
} // end namespace Vast

#endif // VAST_NET_MANAGER_H
