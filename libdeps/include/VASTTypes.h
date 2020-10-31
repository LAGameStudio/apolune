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
    VASTTypes.h     - Basic Data Structures for VAST

    NOTE:   most serializable strcutures are all 8-byte aligned (VC's default)
            except the "Message" class which is variable

    TODO:   a smarter way with less code to perform serialize / deserialize

*/

#ifndef _VAST_Types_H
#define _VAST_Types_H

#include "Config.h"
#include <string.h>     // memcpy
#include <math.h>       // abs ()
#include <stdlib.h>     // atoi

// include standard integer depending on compiler used
#ifdef _WIN32
#include "./standard/stdint.h"
#else
#include <stdint.h>
#endif

#include <string>
#include <sstream>
#include <vector>
#include <map>

namespace Vast
{

// # of most significant bits reserved for private ID assignments by each host
// must be even # of bits

/*
#define VAST_ID_PRIVATE_BITS 32

// the most significant bits are private, least significant bits are hostID
#define EXTRACT_HOST_ID(x)  (x & (0xFFFFFFFFFFFFFFFF >> VAST_ID_PRIVATE_BITS))
#define EXTRACT_LOCAL_ID(x) (x >> (64 - VAST_ID_PRIVATE_BITS))

// turn any number into unique ID by reserving the least significant bits as local ID
// so format is local+host
#define UNIQUE_ID(x,y) ((y << (32 - VAST_ID_PRIVATE_BITS)) | x)
*/


#define VAST_EXTRACT_ASSIGNED_ID(host_id) (host_id & (0xFFFF >> 2))

typedef uint64_t    id_t;           // hostID (globally unique) or nodeID (unique within each world)
typedef uint32_t    timestamp_t;    // timestamp type, stores the timestamp obtained from system
                                    // NOTE: that current net_ace time is millisecond accuracy since program start (so up to 50 days since execution)
typedef uint8_t     byte_t;
typedef uint16_t    word_t;
typedef uint32_t    vsize_t;        // size_t used in VAST

typedef float       coord_t;        // type of coordinates
typedef uint16_t    length_t;       // type for a length, for radius or rectangle length

typedef int16_t     msgtype_t;      // the types of messages (0 - 65535)
typedef int16_t     layer_t;        // the number of layers in the overlay (0 - 255)

typedef uint8_t     listsize_t;     // size of a list sent over network

#define VAST_MSGTYPE_RESERVED 8                         // 8 bits for reserved message type
#define VAST_MSGTYPE(x) (0x0FF & x)                     // mask off app-specific message types
#define APP_MSGTYPE(x)  (x >> VAST_MSGTYPE_RESERVED)    // mask off app-specific message types

// this constant is to determine how far will two different points be considered the same
// potential BUG: is it small enough?
const double EQUAL_DISTANCE = 0.000001;

// optimized data structure for positions (used by VON_MOVE)
typedef struct 
{
    coord_t x, y;

} VONPosition;

// status on known nodes in the neighbor list, can be either just inserted / deleted / updated
typedef enum  
{
    INSERTED = 1,
    DELETED,
    UNCHANGED,
    UPDATED
} NeighborUpdateStatus;

// node / message handler can all have these states
typedef enum 
{
    ABSENT  = 0,
    QUERYING,           // finding / determing certain thing
    JOINING,            // different stages of join
    JOINING_2,
    JOINING_3,
    JOINED,
} NodeState;

// failure method for simulations
typedef enum 
{
    RANDOM,
    RELAY_ONLY,
    MATCHER_ONLY,
    CLIENT_ONLY

} FailMethod;

// bandwidth type define
typedef enum 
{
    BW_UNKNOWN = 0,
    BW_UPLOAD, 
    BW_DOWNLOAD

} bandwidth_t;

// a particular stat record for certain data
class EXPORT StatType
{
public:
    StatType ()
    {
        reset ();
    }

    void calculateAverage ()
    {
        if (num_records > 0)
            average = ((double)total / (double)num_records);

		if (minimum == (size_t)(-1))
			minimum = 0;
    }

    void addRecord (size_t value)
    {
        // do not record empty values
        if (value == 0)
            return;

        total += value;
        if (value < minimum)
            minimum = value;
        if (value > maximum)
            maximum = value;
        num_records++;
    }

    void reset ()
    {
        maximum = total = num_records = 0;
        average = 0.0;
        minimum = (size_t)(-1);
    }

    size_t  minimum;        // minimum single record
    size_t  maximum;        // maximum single record    

    size_t  total;          // total collected value
    size_t  num_records;    // total # of records

    double  average;        // average = total / num_records
};

// virtual interface that a class should implement to become
// serializable into a bytestring sendable over network
class EXPORT Serializable 
{
public:
    Serializable () {}
    virtual ~Serializable () {}

    // size of this class, must be implemented
    virtual size_t sizeOf () const = 0;

    // store into a buffer (assuming the buffer has enough space)
    // buffer can be NULL (simply to query the total size)
    // returns the total size of the packed class
    virtual size_t serialize (char *buffer) const = 0;

    // restores a buffer into the object
    // returns number of bytes restored (should be > 0 if correct)
    virtual size_t deserialize (const char *buffer, size_t size) = 0;
};

//
// a spot location in the virtual world
//
class EXPORT Position : public Serializable 
{

public:
    Position () 
        : x (0), y (0), z (0)
    {
    }

    Position (int x_coord, int y_coord, int z_coord = 0)
        : x ((coord_t)x_coord), y ((coord_t)y_coord), z ((coord_t)z_coord)
    {
    }

    Position (coord_t x_coord, coord_t y_coord, coord_t z_coord = 0)
        : x (x_coord), y (y_coord), z (z_coord)
    {
    }

    Position (const char *p) 
    {
        memcpy (this, p, sizeof (Position));
    }

    Position (const Position &p)
    {
        x = p.x;
        y = p.y;
        z = p.z;
    }

    Position &operator=(const Position& c)
    {
        x = c.x;
        y = c.y;
        z = c.z;

        return *this;
    }

    void clear ()
    {
        x = y = z = 0;
    }

    bool operator==(const Position& c)
    {
        return (this->x == c.x && this->y == c.y && this->z == c.z);
    }

    Position &operator+=(const Position& c)
    {
        x += c.x;
        y += c.y;
        z += c.z;

        return *this;
    }

    Position &operator-=(const Position& c)
    {
        x -= c.x;
        y -= c.y;
        z -= c.z;

        return *this;
    }

    Position &operator*=(double value)
    {
        x = (coord_t)((double)x * value);
        y = (coord_t)((double)y * value);
        z = (coord_t)((double)z * value);

        return *this;
    }

    Position &operator/=(double c)
    {
        x = (coord_t)((double)x / c);
        y = (coord_t)((double)y / c);
        z = (coord_t)((double)z / c);

        return *this;
    }

    Position operator+(const Position& c)
    {
        return Position (x + c.x, y + c.y, z + c.z);
    }

    Position operator-(const Position& c)
    {
        return Position (x - c.x, y - c.y, z - c.z);
    }

    Position operator*(double value)
    {
        return Position ((coord_t)(x * value), (coord_t)(y * value), (coord_t)(z * value));
    }

    void set (coord_t x, coord_t y, coord_t z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
            
    coord_t distance (Position const &p) const
    {        
        coord_t dx = ((coord_t)p.x - (coord_t)x);
        coord_t dy = ((coord_t)p.y - (coord_t)y);
        coord_t dz = ((coord_t)p.z - (coord_t)z);

        return sqrt (pow (dx, 2) + pow (dy, 2) + pow (dz, 2));
    }

    coord_t distanceSquare (Position const &p) const
    {        
        coord_t dx = (p.x - x);
        coord_t dy = (p.y - y);
        coord_t dz = (p.z - z);

        return (dx*dx + dy*dy + dz*dz);
    }

    bool isEmpty ()
    {
        return (x == 0 && y == 0 && z == 0);
    }

    // make current vector unit vector
    bool makeUnit ()
    {
        coord_t dist = sqrt (pow (this->x, 2) + pow (this->y, 2) + pow (this->z, 2));
        
        if (dist == 0)
            return false;

        this->x /= dist;
        this->y /= dist;
        this->z /= dist;

        return true;
    }

    bool operator!= (Position const &p)
    {
        return (x != p.x || y != p.y || z != p.z);
    }

    bool operator< (Position const &p)
    {
        if (x < p.x)
            return true;

        if (x == p.x && y < p.y)
            return true;

        if (x == p.x && y == p.y && z < p.z)
            return true;

        return false;        
    }

    // size of this class
    inline size_t sizeOf () const
    {
        return sizeof (coord_t)*3;
    }

    size_t serialize (char *p) const
    {
        if (p != NULL)
        {
            memcpy (p, &x, sizeof (coord_t));   p += sizeof (coord_t);
            memcpy (p, &y, sizeof (coord_t));   p += sizeof (coord_t);
            memcpy (p, &z, sizeof (coord_t)); 
        }
        return sizeOf ();
    }

    size_t deserialize (const char *p, size_t size)
    {
        // perform size check        
        if (p != NULL && size >= sizeOf ())
        {
            memcpy (&x, p, sizeof (coord_t));   p += sizeof (coord_t);
            memcpy (&y, p, sizeof (coord_t));   p += sizeof (coord_t);
            memcpy (&z, p, sizeof (coord_t));

            return sizeOf ();
        }
        return 0;
    }

    coord_t x;
    coord_t y;
    coord_t z;
};

class EXPORT Area : public Serializable
{
public:
    Area ()
        :center (0, 0, 0), radius (0), height (0) //width (0),
    {
    }

    Area (const Position &c, length_t r)
        :center (c), radius (r), height (0)
    {
    }

    ~Area ()
    {
    };

    void clear ()
    {
        radius = 0;
        height = 0;
        center.clear ();
    }

    bool operator==(const Area& a)
    {
        return (this->center == a.center && this->radius == a.radius && this->height == a.height);
    }

    bool operator!=(const Area& a)
    {
        return !(*this == a);
    }

    Area &operator=(const Area & a)
    {
        this->center = a.center;
        this->radius = a.radius;
        this->height = a.height;

        return *this;
    }

    // size of this class
    size_t sizeOf () const
    {
        return center.sizeOf () + sizeof (length_t) * 2;
    }

    // whether this area covers a given position
    bool overlaps (const Position &pos, length_t buffer = 0)
    {
        // circular area
        if (height == 0)
            return (center.distance (pos) <= (radius + buffer));
        // rectangular area
        else
            return (fabs (center.x - pos.x) <= (radius/2 + buffer)) && (fabs (center.y - pos.y) <= (height/2 + buffer));
    }

    // whether this area covers a given position
    bool overlaps (const Area &area)
    {
        // circular area
        if (height == 0)
            return (center.distance (area.center) <= (radius + area.radius));
        // rectangular area
        else
            return (fabs (center.x - area.center.x) <= ((radius + area.radius)/2)) && (fabs (center.y - area.center.y) <= ((height + area.height)/2));
    }

    size_t serialize (char *p) const
    {
        if (p != NULL)
        {            
            p += center.serialize (p);
            memcpy (p, &radius, sizeof (length_t));     p += sizeof (length_t);            
            memcpy (p, &height, sizeof (length_t));
        }
        return sizeOf ();
    }

    size_t deserialize (const char *p, size_t size)
    {
        if (p != NULL && size >= sizeOf ())
        {
            p += center.deserialize (p, size);
            memcpy (&radius, p, sizeof (length_t));     p += sizeof (length_t);            
            memcpy (&height, p, sizeof (length_t));
            
            return sizeOf ();
        }
        return 0;
    }

    Position    center;
    length_t    radius;         
    length_t    height;     // if height is available then radius acts as width in a rectangle
    //length_t    width;
};


class EXPORT IPaddr : public Serializable
{
public:
    IPaddr ()
        : host(0), port(0)
    {
    }
    
    IPaddr (uint32_t i, uint16_t p)
    {
        host = i;
        port = p;
    }
    
    IPaddr (const char *ip_string, uint16_t p)
    {
        uint32_t i = 0;

        if (ip_string != NULL)
        {
            int j=0;
            char c;
            
            uint32_t t = 0;
            while ((c = ip_string[j++]) != '\0')
            {
                if (c >= '0' && c <= '9')
                    t = t*10 + (c - '0');
                else if (c == '.')
                {
                    i = (i<<8) + t;
                    t = 0;
                }
            }
            i = (i<<8) + t;
        }
        else
            i = 0;

        host = i;
        port = p;
    }
    
    ~IPaddr ()
    {
    }   
        
    IPaddr &operator= (IPaddr const &a)
    {
        host = a.host;
        port = a.port;
        pad  = a.pad;

        return *this;
    }

    void getString (char *p)
    {
        sprintf (p, "%d.%d.%d.%d:%u", (int)((host>>24) & 0xff), (int)((host>>16) & 0xff), (int)((host>>8) & 0xff), (int)(host & 0xff), port);
    }

    void parseIP (const std::string & instr)
    {
        IPaddr::parseIP (*this, instr);
    }

    static int parseIP (IPaddr & addr, const std::string & instr)
    {
        size_t port_part;

        if ((port_part = instr.find (":")) == ((size_t)(-1)))
            return -1;

        int port = atoi (instr.substr (port_part + 1).c_str ());
        if (port <= 0 || port >= 65535)
            return -1;

        addr = IPaddr (instr.substr (0, port_part).c_str (), (uint16_t)port);
        return 0;
    }

    inline bool operator== (const IPaddr & adr) const
    {
        return (adr.host == host && adr.port == port);
    }

    // size of this class
    size_t sizeOf () const
    {
        return sizeof (uint32_t) + sizeof (uint16_t) * 2;
    }

    size_t serialize (char *p) const
    {
        if (p != NULL)
        {
            memcpy (p, &host, sizeof (uint32_t));  p += sizeof (uint32_t);            
            memcpy (p, &port, sizeof (uint16_t)); p += sizeof (uint16_t);
            memcpy (p, &pad, sizeof (uint16_t)); 
        }
        return sizeOf ();
    }

    size_t deserialize (const char *p, size_t size)
    {        
        if (p != NULL && size >= sizeOf ())
        {
            memcpy (&host, p, sizeof (uint32_t));  p += sizeof (uint32_t);            
            memcpy (&port, p, sizeof (uint16_t)); p += sizeof (uint16_t);
            memcpy (&pad, p, sizeof (uint16_t));
            return sizeOf ();
        }
        return 0;        
    }

    // NOTE: that pad right now is used to store layer info in VASTATE
    // see VASTNode::addPeer (). do not change it without also modifying VASTATE
    // TODO: cleaner way?

    uint32_t      host;
    uint16_t      port;   
    uint16_t      pad;        
    
};

/*
 *  Addr class
 *  raw string format ( [] means ignorable )
 *    PublicIP:port[|Private:port]
 *  for ex bbs site "PTT" should be 
 *    140.112.172.11:23
 *  someone behind NAT should be
 *    140.112.172.11:23|192.168.5.8:22018
 */
class EXPORT Addr : public Serializable
{
    
public:
    
    Addr ()
    {
        this->clear ();
    }

    Addr (id_t i, const IPaddr *pub) //, IPaddr *priv = NULL)
    {
        host_id  = i;
        publicIP = *pub;
        
        //if (priv != NULL)
        //    privateIP = *priv;

        lastAccessed = 0;
    }

    ~Addr ()
    {
    }

    void clear ()
    {
        host_id = 0;
        lastAccessed = 0;
        publicIP.host = 0;
        publicIP.port = 0;
        
        //privateIP.host = 0;
        //privateIP.port = 0;
    }

    void setPublic (uint32_t i, uint16_t p)
    {
        publicIP.host = i;
        publicIP.port = p;
    }
    
/*
    void setPrivate (uint32_t i, uint16_t p)
    {
        privateIP.host = i;
        privateIP.port = p;
    }
*/
    
    const bool toString (std::string &str)
    {
        //static std::string str;

        // temp variables to help output
        char tmpstr[4*4+7];   // max 3 digit + 1 dot and 4 fields + ":" and port totally
        
        publicIP.getString (tmpstr);

/*
        if (privateIP.port != 0)
        {
            str.append ("|");
            privateIP.getString (tmpstr);
            oss << tmpstr << ":" << privateIP.port;
        }
*/

        str.empty ();
        str = std::string (tmpstr);

        return true;
        //return str;
    }

    //    
    // format is "publicIP:publicPort|privateIP:privatePort"  (privateIP part is optional)
    //
    // return 0 on success
    int fromString (const std::string & str)
    {
        clear ();

        size_t pos = str.find ("|");

        // found no "|" mark, indicates public IP only
        if (pos == (size_t)(-1))
        {
            if (IPaddr::parseIP (publicIP, str))
                return -1;
        }
        else
        {
            std::string pub = str.substr (0, pos);
            //std::string priv = str.substr (pos + 1);

            if (IPaddr::parseIP (publicIP, pub)) // || IPaddr::parseIP (privateIP, priv))
                return -1;
        }

        return 0;
    }
    
    inline bool operator== (const Addr & adr) const
    {
        return (host_id == adr.host_id && 
                publicIP == adr.publicIP); // && privateIP == adr.privateIP);
    }

    Addr &operator=(const Addr& a)
    {
        this->host_id = a.host_id;
        this->lastAccessed = a.lastAccessed;
        this->publicIP = a.publicIP;
        //this->privateIP = a.privateIP;

        return *this;
    }

    inline bool operator!= (const Addr & adr) const
    {
        return !(*this == adr);
    }

    size_t sizeOf () const
    {
        return sizeof (id_t) + sizeof (timestamp_t) + publicIP.sizeOf (); // + privateIP.sizeOf ();
    }

    size_t serialize (char *p) const
    {
        if (p != NULL)
        {
            memcpy (p, &host_id, sizeof (id_t));                p += sizeof (id_t);            
            memcpy (p, &lastAccessed, sizeof (timestamp_t));    p += sizeof (timestamp_t);
            publicIP.serialize (p);                             //p += publicIP.sizeOf ();
            //privateIP.serialize (p);
        }
        return sizeOf ();
    }

    size_t deserialize (const char *p, size_t size)
    {        
        if (p != NULL && size >= sizeOf ())
        {
            memcpy (&host_id, p, sizeof (id_t));                p += sizeof (id_t);            
            memcpy (&lastAccessed, p, sizeof (timestamp_t));    p += sizeof (timestamp_t);            
            publicIP.deserialize (p, publicIP.sizeOf ());      //p += publicIP.sizeOf ();
            //privateIP.deserialize (p);
            
            return sizeOf ();
        }
        return 0;        
    }

    id_t            host_id;        // hostID uniquely associated with this address
    timestamp_t     lastAccessed;   // last time something gets send to this address
    IPaddr          publicIP;       // public IP
    //IPaddr          privateIP;      // private IP
};

class EXPORT Node : public Serializable
{
public:
    Node ()
        : id (0), time (0), aoi (), addr () 
    {
    }

    Node (id_t i, timestamp_t t, Area &a, Addr &ar) 
        : id (i), time (t), aoi (a), addr (ar) 
    {
    }

    Node (const Node & n)
        : id (n.id), time (n.time), aoi (n.aoi), addr (n.addr) 
    {
    }

    ~Node ()
    {
    }

    void update (Node const &rhs)
    {
        id      = rhs.id;        
        time    = rhs.time;
        aoi     = rhs.aoi;        
        addr    = rhs.addr;
    }

    Node& operator= (const Node& n)
    {        
        id      = n.id;        
        time    = n.time;
        aoi     = n.aoi;        
        addr    = n.addr;
        return *this;
    }

    // size of this class
    size_t sizeOf () const
    {
        return sizeof (id_t) + sizeof (timestamp_t) + aoi.sizeOf () + addr.sizeOf ();
    }

    size_t serialize (char *p) const
    {
        if (p != NULL)
        {
            memcpy (p, &id, sizeof (id_t));             p += sizeof (id_t);
            memcpy (p, &time, sizeof (timestamp_t));    p += sizeof (timestamp_t);
            p += aoi.serialize (p);
            addr.serialize (p);
        }
        return sizeOf ();
    }

    size_t deserialize (const char *p, size_t size)
    {        
        if (p != NULL && size >= sizeOf ())
        {
            memcpy (&id, p, sizeof (id_t));             p += sizeof (id_t);
            memcpy (&time, p, sizeof (timestamp_t));    p += sizeof (timestamp_t);
            p += aoi.deserialize (p, aoi.sizeOf ());
            addr.deserialize (p, addr.sizeOf ());

            return sizeOf ();
        }
        return 0;        
    }

    id_t            id;         // unique ID for the node   
    timestamp_t     time;       // last update time    
    Area            aoi;        // the area of interest    
    Addr            addr;       // IP address for this node    
};

class EXPORT Subscription : public Serializable
{
public:
    Subscription ()
    {
        clear ();
    }

    ~Subscription ()
    {
    }

    void clear ()
    {
        host_id = 0;
        id = 0;   
        layer = 0;        
        active = false;    
        dirty = false;
        in_region = false; 
        time = 0;
        aoi.clear ();
    }

    bool addNeighbor (Subscription *neighbor)
    {
        std::map<id_t, Area>::iterator it;

        // if neighbor already exists, simply update AOI
        if ((it = _neighbors.find (neighbor->id)) != _neighbors.end ())
        {
            // check if there's update in position
            if (it->second != neighbor->aoi)
            {
                _neighbor_states[neighbor->id] = UPDATED;
                it->second = neighbor->aoi;                
            }
            // also record if not changed (to check for out-of-AOI neighbors later)
            else
                _neighbor_states[neighbor->id] = UNCHANGED;
        }
        else
        {
            // new neighbor, insert it
            _neighbor_states[neighbor->id] = INSERTED;
            _neighbors[neighbor->id] = neighbor->aoi;            
        }

        return true;
    }

    // remove neighbors no longer in view
    // returns # of neighbors still in view
    // TODO: more efficient method (than to look up in map every time?)
    int clearInvisibleNeighbors ()
    {
        std::map<id_t, Area>::iterator it = _neighbors.begin ();
        std::vector<id_t> remove_list;

        for (; it != _neighbors.end (); it++)
            // the neighbor is no longer visible, remove it
            if (_neighbor_states.find (it->first) == _neighbor_states.end ())
                remove_list.push_back (it->first);

        for (size_t i=0; i < remove_list.size (); i++)
        {
            _neighbors.erase (remove_list[i]);
            _neighbor_states[remove_list[i]] = DELETED;
        }

        return 0;
    }

    // reset all neighbor states so to perform a new round of visibility test
    void clearStates (id_t neighbor_id = 0)
    {
        // we need to reset 
        if (neighbor_id != 0)
            _neighbors.erase (neighbor_id);
        else
        {
            _neighbor_states.clear ();
            in_region = false;
        }
    }

    // remove all records of my visible neighbors
    void clearNeighbors ()
    {
        _neighbors.clear ();
    }

    // obtain an update of the states of neighbors
    std::map<id_t, NeighborUpdateStatus>& getUpdateStatus ()
    {
        return _neighbor_states;
    }

    /*
    // get the info for a particular neighbor
    Subscription *getNeighbor (id_t id)
    {
        std::map<id_t, Area>::iterator it;
        if ((it = _neighbors.find (id)) == _neighbors.end ())
            return NULL;

        return it->second;
    }
    */

    // size of this class
    size_t sizeOf () const
    {
        return sizeof (id_t) * 2 + sizeof (layer_t) + aoi.sizeOf () + relay.sizeOf ();
    }

    // NOTE that 'active' 'time' flag is not serialized and will be restored as 'false' by default
    size_t serialize (char *p) const
    {
        if (p != NULL)
        {
            memcpy (p, &host_id, sizeof (id_t));        p += sizeof (id_t);
            memcpy (p, &id, sizeof (id_t));             p += sizeof (id_t);            
            memcpy (p, &layer, sizeof (layer_t));       p += sizeof (layer_t);
            //memcpy (p, &active, sizeof (bool));         p += sizeof (bool);
            p += aoi.serialize (p);
            relay.serialize (p);
        }
        return sizeOf ();
    }

    // NOTE that 'active' 'time' flag is not serialized and will be restored as 'false' by default
    size_t deserialize (const char *p, size_t size)
    {        
        if (p != NULL && size >= sizeOf ())
        {
            memcpy (&host_id, p, sizeof (id_t));        p += sizeof (id_t);
            memcpy (&id, p, sizeof (id_t));             p += sizeof (id_t);
            memcpy (&layer, p, sizeof (layer_t));       p += sizeof (layer_t);
            //memcpy (&active, p, sizeof (bool));         p += sizeof (bool);
            this->active = false;
            this->time = 0;
            p += aoi.deserialize (p, aoi.sizeOf ());
            relay.deserialize (p, relay.sizeOf ());

            return sizeOf ();
        }
        return 0;        
    }

    id_t        host_id;        // HostID of the subscriber
    id_t        id;             // subscriptionID (different subscriptions may have same hostID)
    layer_t     layer;          // layer number for the subscription    
    bool        active;         // whether the subscription is successful
    bool        dirty;          // whether the subscription has been updated
    bool        in_region;      // whether the subscriber lies within the current managed region
    timestamp_t time;           // last update time for this subscriber
    Area        aoi;            // aoi of the subscription (including a center position)
    Addr        relay;          // the address of the relay of the subscriber (to receive messages)

private:
    std::map<id_t, NeighborUpdateStatus> _neighbor_states;
    std::map<id_t, Area> _neighbors;
};

class EXPORT Message : public Serializable
{
public:

    Message (msgtype_t type, id_t sender = 0) 
    {
        // allocate the default space for message
        from      = sender;
        msggroup  = 0;
        msgtype   = type;
        reliable  = true;       // default is reliable message
        priority  = 3;          // default is 3, smaller value indicates higher importance
        _alloc    = true;
        _curr     = data = new char[VAST_MSGSIZ];                
        size      = 0;
        _free     = VAST_MSGSIZ;        
    }

    // create a message to store a bytestring
    // NOTE: optional "no allocation" is possible (i.e., the Message created will simply 
    //       access some pre-existing memory bloack) if it is expected that the string being
    //       extracted will not need to be manipulated or changed
    Message (msgtype_t type, const char *msg, size_t msize, bool alloc = true, id_t sender = 0)
    {
        from     = sender;
        msgtype  = type;
        reliable = true;
        priority = 3;          // default is 3, smaller value indicates more important
        _alloc   = alloc;

        // check if we need to allocate new space, or could simply use the pointer given
        if (alloc == true)
        {
            _curr = data = new char[msize];
            memcpy (data, msg, msize);
        }
        else
            _curr = data = (char *)msg;

        size = msize;
        _free = 0;
    }

    // copy constructor
    Message (Message const& msg)
    {
        // copy metadata
        from      = msg.from;
        size      = msg.size;
        msgtype   = msg.msgtype;        
        msggroup  = msg.msggroup;
        reliable  = msg.reliable;
        priority  = msg.priority;
        _alloc    = msg._alloc;
        _free     = msg._free;

        // copy actual data & set pointers
        data    = new char[size + _free];
        _curr   = data + (msg._curr - msg.data);
        memcpy (data, msg.data, size);

        targets = msg.targets;
    }

    ~Message ()
    {
        // we only release memory if we had previously allocated
        if (_alloc == true)
            delete[] data;
    }

    Message & operator= (const Message &msg)
    {
        // copy metadata
        from      = msg.from;
        size      = msg.size;
        msgtype   = msg.msgtype;        
        msggroup  = msg.msggroup;
        reliable  = msg.reliable;
        priority  = msg.priority;
        _alloc    = msg._alloc;
        _free     = msg._free;

        // de-allocate previous space & copy actual data & set pointers
        delete[] data;
        data    = new char[size + _free];
        _curr   = data;      
        memcpy (data, msg.data, size);

        targets = msg.targets;
        return *this;
    }

    // ensure the Message object can store at least data of size 'len' 
    void expand (size_t len)
    {
        // don't expand if we have enough
        if (len <= _free)
            return;

        // estimate new size
        size_t newsize = (((size + len) / VAST_MSGSIZ) + 1) * VAST_MSGSIZ;
        
        char *temp = new char[newsize];

        if (temp == NULL)
        {
            printf ("Message::expand () cannot allocate enough memory\n");
            return;
        }

        // copy old data to new buffer & adjust pointer 
        memcpy (temp, data, size);        
        _curr = temp + (_curr - data);
        _free = newsize - size;

        // remove old buffer
        delete[] data;
        data = temp;
    }

    // reset the current position so the message content may be extracted again
    void reset ()
    {
        _curr = data;
    }

    // store an item to this message, record_size indicates whether this is
    // a fixed-size item or variable-size item (needs to records its size)
    bool store (const char *p, vsize_t item_size, bool record_size = false)
    {
        // if no internal allocation, then cannot store new data
        if (_alloc == false)
            return false;

        // check available space
        expand ((record_size ? (item_size + sizeof (vsize_t)) : item_size));

        // always store at end
        _curr = data + size;

        if (record_size)
        {
            memcpy (_curr, &item_size, sizeof (vsize_t));
            addSize (sizeof (vsize_t));
        }

        // copy data and adjust pointer & size
        memcpy (_curr, p, item_size);        
        addSize (item_size);

        return true;
    }

    bool store (const Serializable &obj)
    {
        // first make sure we've got enough space
        vsize_t item_size = obj.serialize (NULL);
        this->expand (item_size);

        _curr = data + size;

        // store away (at end)
        if (obj.serialize (_curr) != item_size)
            return false;

        // update size count
        addSize (item_size);
        
        return true;
    }

    bool store (id_t item)
    {
        return store ((char *)&item, sizeof (id_t));
    }

    bool store (timestamp_t item)
    {
        return store ((char *)&item, sizeof (timestamp_t));
    }

    bool store (listsize_t item)
    {
        return store ((char *)&item, sizeof (listsize_t));
    }

    bool store (layer_t item)
    {
        return store ((char *)&item, sizeof (layer_t));
    }

    bool store (float item)
    {
        return store ((char *)&item, sizeof (float));
    }

    // extract an item from this message
    // item_size == 0 indicates a variable size item, item_size > 0 means fixed-size item
    // NOTE: the destination pointer is assumed to have enough space
    size_t extract (char *p, vsize_t item_size, bool from_end = false)
    {
        // extract "item size" first, if not specified
        if (item_size == 0)
        {
            // not possible to extract item_size from end
            if (from_end == true)
                return 0;

            if ((size_t)((_curr + sizeof (vsize_t)) - data) <= size)
            {
                memcpy (&item_size, _curr, sizeof (vsize_t));
                _curr += sizeof (vsize_t);
            }
        }

        // see if we still have something to extract (_curr should not extend pass valid size)
        if ((size_t)((_curr + item_size) - data) > size)
            return 0;

        // copy the item out & update current pointer
        if (from_end)
        {
            // create temporary pointer close to end
            char *curr = data + size - item_size;
            memcpy (p, curr, item_size);

            // update 'size' as data should be smaller now
            size -= item_size;
        }
        else
        {
            memcpy (p, _curr, item_size);
            _curr += item_size;
        }

        return item_size;
    }

    // extract to a serializable object
    size_t extract (Serializable &obj, bool from_end = false)
    {
        vsize_t item_size = obj.serialize (NULL);

        // see if we still have something to extract
        if ((size_t)((_curr + item_size) - data) > size)
            return 0;
        
        // store current pointer if extracting from end
        char *temp = NULL;
        if (from_end)
        {
            temp = _curr;
            _curr = data + size - item_size;
        }

        if (obj.deserialize (_curr, item_size) == 0)
            return 0;

        _curr += item_size;

        // restore _curr pointer and reduce size if extracting from end
        if (from_end)
        {
            _curr = temp;
            size -= item_size;
        }

        return item_size;
    }

    size_t extract (id_t &id, bool from_end = false)
    {
        extract ((char *)&id, sizeof (id_t), from_end);
        return sizeof (id_t);
    }

    size_t extract (timestamp_t &item, bool from_end = false)
    {
        extract ((char *)&item, sizeof (timestamp_t), from_end);
        return sizeof (timestamp_t);
    }

    size_t extract (layer_t &item, bool from_end = false)
    {
        extract ((char *)&item, sizeof (layer_t), from_end);
        return sizeof (layer_t);
    }

    size_t extract (listsize_t &size, bool from_end = false)
    {
        extract ((char *)&size, sizeof (listsize_t), from_end);
        return sizeof (listsize_t);
    }

    size_t extract (float &item, bool from_end = false)
    {
        extract ((char *)&item, sizeof (float), from_end);
        return sizeof (float);
    }

    // clear existing content of the message, reset all counters
    void clear (msgtype_t type, id_t sender = 0, byte_t priority_value = 3)
    {
        from    = sender;
        targets.clear ();

        msgtype     = type;
        reliable    = true;
        priority    = priority_value;
        _alloc      = true;
        _curr       = data;
        _free       = _free + size;
        size        = 0;
        msggroup    = 0;        
    }

    // serialize this object to a buffer, returns total size in bytes
    // NOTE: assumes enough space is pre-allocated to buffer, 
    //       use NULL for buffer to get total size
    //      currently a fixed overhead of 15 bytes for 1 target
    size_t serialize (char *buffer) const
    {
        char *p = buffer;

        if (buffer != NULL)
        {            
            // copy header part:
            //  sender, size, type, group, priority, reliablity
            memcpy (p, &from, sizeof (id_t));
            p += sizeof (id_t);

            memcpy (p, &size, sizeof (vsize_t));
            p += sizeof (vsize_t);
            
            memcpy (p, &msgtype, sizeof (msgtype_t));
            p += sizeof (msgtype_t);

            memcpy (p, &msggroup, sizeof (byte_t));
            p += sizeof (byte_t);

            memcpy (p, &priority, sizeof (byte_t));
            p += sizeof (byte_t);

            memcpy (p, &reliable, sizeof (bool));
            p += sizeof (bool);

            // copy target IDs
            listsize_t num_targets = (listsize_t)targets.size ();
            memcpy (p, &num_targets, sizeof (listsize_t));
            p += sizeof (listsize_t);

            for (size_t i = 0; i < targets.size (); i++)                
            {
                memcpy (p, &targets[i], sizeof (id_t));
                p += sizeof (id_t);
            }

            // copy data
            memcpy (p, data, size);
            p += size;

            return (size_t)(p - buffer);
        }

        return sizeOf ();
    }

    // restores a buffer into the Message object
    // returns number of bytes of the message restored (should be >= 0 if correct)
    // NOTE: message size could be 0 and is still correct
    // TODO: more efficient error checking
    size_t deserialize (const char *buffer, size_t size)
    {
        size_t size_restored = 0;
        char *p = (char *)buffer;
        clear (0, 0);
        
        // NOTE: we cannot yet check whether 'size' is correct because it'll depend on
        //       what we find while decoding content in buffer
        if (buffer != NULL)
        {            
            // restore header part first
            // sender, size, type, group, priority, reliablity
            size_t header_size = sizeof (id_t) + 
                                 sizeof (vsize_t) +
                                 sizeof (msgtype_t) + 
                                 sizeof (byte_t) +
                                 sizeof (byte_t) +
                                 sizeof (bool) + 
                                 sizeof (listsize_t);

            //printf ("deserialize header: %lu, received size: %lu vsize_t: %lu\n", header_size, size, sizeof (vsize_t));

            // size check
            if (size < header_size)
                return 0;
            size -= header_size;
                                                                  
            // restore from ID
            memcpy (&from, p, sizeof (id_t));
            p += sizeof (id_t);

            // restore message size
            memcpy (&size_restored, p, sizeof (vsize_t));
            p += sizeof (vsize_t);

            // restore message type, priority, message size 
            memcpy (&msgtype, p, sizeof (msgtype_t));            
            p += sizeof (msgtype_t);            

            memcpy (&msggroup, p, sizeof (byte_t));
            p += sizeof (byte_t);

            memcpy (&priority, p, sizeof (byte_t));
            p += sizeof (byte_t);

            memcpy (&reliable, p, sizeof (bool));
            p += sizeof (bool);

            // restore receiver NodeIDs
            listsize_t num_targets;
            memcpy (&num_targets, p, sizeof (listsize_t));
            p    += sizeof (listsize_t);
            
            // check target size & content size
            if (size < (num_targets * sizeof (id_t) + size_restored))
                return 0;
           
            id_t target;
            for (listsize_t i=0; i < num_targets; i++)
            {
                memcpy (&target, p, sizeof (id_t));
                p += sizeof (id_t);             
                targets.push_back (target);
            }

            // restore message content
            if (store (p, size_restored) == false)
                return 0;
           
            // restore internal _curr pointer (very important!)
            // otherwise subsequent calls to extract () would be incorrect
            _curr = data;

            // total size restored includes header
            // NOTE: this also makes sure that a message with content size = 0 is
            //       acceptable to send / receive
            size_restored += header_size;
        }

        return size_restored;
    }

    void addTarget (id_t target)
    {
        // avoid redundency
        size_t i;
        for (i=0; i < targets.size (); i++)
            if (targets[i] == target)
                break;
        if (i == targets.size ())
            targets.push_back (target);
    }

    // add a specified size to the current valid message size
    // returns the adjusted _curr pointer
    inline char *addSize (vsize_t item_size)
    {
        _curr += item_size;
        _free -= item_size;
        size  += item_size;

        return _curr;
    }

    // size of this class
    //      currently a fixed overhead of 15 bytes for one target
    size_t sizeOf () const
    {        
        return (sizeof (id_t) +                     // sender (from)    
                sizeof (vsize_t) +                   // msg size
                sizeof (msgtype_t) +                // msgtype
                sizeof (byte_t) +                   // msggroup                 
                sizeof (byte_t) +                   // priority
                sizeof (bool) +                     // reliable               
                sizeof (listsize_t) +               // target length
                sizeof (id_t) * targets.size () +   // targets                                
                size);                              // msg content                
    }

    //
    // message variables (NOTE: may not be how it's serialized)
    //
    id_t        from;           // nodeID of the sender    
    size_t      size;           // size of the message
    msgtype_t   msgtype;        // type of message
    byte_t      msggroup;       // msggroup to indicate which handler should process the message
    byte_t      priority;       // priority of message
    bool        reliable;       // whether the message should be sent reliably        
    byte_t      reserved1;      // reserved
    byte_t      reserved2;      // reserved
    byte_t      reserved3;      // reserved

    char       *data;           // pointer to the message buffer
    std::vector<id_t> targets;  // NodeIDs this message targets    
           
private:
    // book-keeping variables
    size_t      _free;          // # of bytes left in buffer
    char       *_curr;          // current position in buffer
    bool        _alloc;         // whether internal allocation was used

};



// a simple class to keep count & calculate statistics
class EXPORT Ratio
{
public:
    Ratio ()
        :normal (0), total (0)
    {
    }

    // calculates between ratio of two numbers
    double ratio ()
    {
        return (double)normal / (double)total;
    }

    size_t  normal;
    size_t  total;    
};


//
//  Following are used by Voronoi class
//

class EXPORT point2d
{
public:

	double  x;
	double  y;

	point2d (double a = 0 , double b = 0)
	    :x(a), y(b)
	{
	}

    point2d (const point2d& a)
        : x(a.x), y(a.y)
    {
    }

	void operator= (const point2d& a)
	{
            x = a.x;
            y = a.y;
	}

	bool operator< (const point2d& a)
	{
        return (y < a.y ? true : false);
	}

    double distance (const point2d& p) 
    {        
        double dx = p.x - x;
        double dy = p.y - y;
        return sqrt (pow (dx, 2) + pow (dy, 2));
    }

    double dist_sqr (const point2d& p) 
    {        
        double dx = p.x - x;
        double dy = p.y - y;
        return pow (dx, 2) + pow (dy, 2);
    }
};


class EXPORT segment
{
public:
	point2d p1;
	point2d p2;
	
    segment (double x1, double y1, double x2, double y2)
    {
        p1.x = x1;
        p1.y = y1;
        p2.x = x2;
        p2.y = y2;
    }

    segment (point2d& a, point2d& b)
    {
        p1.x = a.x;	p1.y = a.y;
        p2.x = b.x;	p2.y = b.y;
    }
    
    void operator= (const segment& s)
    {
        p1 = s.p1; p2 = s.p2;
    }
    
	bool is_inside (point2d& p)
	{
		double xmax, xmin, ymax, ymin;

		if (p1.x > p2.x) 
	    {	
            xmax = p1.x; 	xmin =  p2.x;	
        }
		else
		{	
            xmax = p2.x;	xmin =  p1.x;	
        }
		if (p1.y > p2.y)     
		{	
            ymax = p1.y; 	ymin =  p2.y;	
        }
		else
		{	
            ymax = p2.y; 	ymin =  p1.y;	
        }
		if (xmin <= p.x && p.x <= xmax && ymin <= p.y && p.y <= ymax) 
			return true;
		else
			return false;	
	}

    bool intersects (const point2d& p3, int radius)
    {
        double u;

        //ACE_DEBUG( (LM_DEBUG, "check if (%d, %d) intersects with p1: (%d, %d) p2: (%d, %d)\n", (int)p3.x, (int)p3.y, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y));

        // we should re-order p1 and p2's position such that p2 > p1
        double x1, x2, y1, y2;
        if (p2.x > p1.x)
        {
            x1 = p1.x;  y1 = p1.y;
            x2 = p2.x;  y2 = p2.y;
        }
        else
        {
            x1 = p2.x;  y1 = p2.y;
            x2 = p1.x;  y2 = p1.y;         
        }

        // formula from http://astronomy.swin.edu.au/~pbourke/geometry/sphereline/
        u = ((p3.x - x1) * (x2 - x1) + (p3.y - y1) * (y2 - y1)) /
            ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        
        if (u >= 0 && u <= 1)
        {
            double x = x1 + (x2 - x1) * u;
            double y = y1 + (y2 - y1) * u;
            
            bool result = ((point2d (x, y).distance (p3) <= (double)radius) ? true : false);
            
            /*
            if (result == true)
                ACE_DEBUG ((LM_DEBUG, "(%d, %d) intersects with p1: (%d, %d) p2: (%d, %d) at (%d, %d)\n", (int)p3.x, (int)p3.y, (int)x1, (int)y1, (int)x2, (int)y2, (int)x, (int)y));
            */
            return result; 
        }
        else
            return false;
    }
};

class EXPORT line2d
{
public:

    double  a,b,c;
    int     bisecting[2];
    int     vertexIndex[2];
    segment seg;
    
	line2d (double x1, double y1, double x2 , double y2)
        : seg(x1, y1, x2, y2)
	{
		if (y1 == y2) 
		{	
			a = 0; b = 1; c = y1;
		}
		else if (x1 == x2) 
		{
			a = 1; b = 0; c = x1;
		}
		else
		{
			double dx = x1 - x2; 
			double dy = y1 - y2;
			double m = dx / dy;
			a = -1 * m;
			b = 1;
			c = a*x1 + b*y1;
		}
	}

	line2d (double A=0, double B=0, double C=0)
		:a (A), b (B), c (C), seg(0, 0, 0, 0)
	{
		vertexIndex[0] = -1;
		vertexIndex[1] = -1;
	}

	bool intersection (line2d& l , point2d& p)
	{
		//The polynomial judgement
		double D = (a * l.b) - (b * l.a);
		if (D == 0) 
        {
			p.x = 0;
			p.y = 0;
			return false;
		}
		else
		{
			p.x=(c*l.b-b*l.c)/D/1.0;
			p.y=(a*l.c-c*l.a)/D/1.0;
            return true;
		}	
	}

    double distance (const point2d& p) 
    {        
        /*
        double u;

        // u = ((x3-x1)(x2-x1) + (y3-y1)(y2-y1)) / (p2.dist(p1)^2)
        u = ((p.x - seg.p1.x) * (seg.p2.x - seg.p1.x) + 
             (p.y - seg.p1.y) * (seg.p2.y - seg.p1.y)) /
             (seg.p1.distance (seg.p2))

        double x = seg.p1.x + u * (seg.p2.x - seg.p1.x);
        double y = seg.p1.y + u * (seg.p2.y - seg.p1.y);

        return p.distance (point2d (x, y));
        */

        return fabs (a * p.x + b * p.y + c) / sqrt (pow (a, 2) + pow (b, 2));
    }
};

/*
class rect
{
public:
	point2d vertex[4];
	line2d  lines[4];

	rect (point2d& c, uint32_t w, uint32_t h)
		:center (c), width (w), height(h)
	{
		calculateVertex();
	}

	bool is_inside (point2d& p)
	{
	    return vertex[1].x >= p.x && vertex[3].x <= p.x &&
               vertex[1].y >= p.y && vertex[3].y <= p.y;
	}

	void setCenter (point2d& np)
	{
		center.x = np.x;
		center.y = np.y;
		calculateVertex();
	}
	
	point2d getCenter ()
	{
		return center;
	}

	uint32_t getWidth()
	{
		return width;
	}
	
	uint32_t getHeight()
	{
		return height;
	}

	void setWidth (uint32_t nw)
	{
		width = nw;
		calculateVertex();
	}

	void setHeight (uint32_t nh)
	{
		height = nh;
		calculateVertex();
	}

protected:

    point2d center;
    int     width, height;

	void calculateVertex()
	{
		vertex[0].x = center.x - width/2;
		vertex[0].y = center.y + height/2;
		vertex[1].x = center.x + width/2;
		vertex[1].y = center.y + height/2;
		vertex[2].x = center.x + width/2;
		vertex[2].y = center.y - height/2;
		vertex[3].x = center.x - width/2;
		vertex[3].y = center.y - height/2;
		
		lines[0].a = 0 ; lines[0].b = 1 ; lines[0].c = vertex[1].y;
		lines[1].a = 1 ; lines[1].b = 0 ; lines[1].c = vertex[1].x;	
		lines[2].a = 0 ; lines[2].b = 1 ; lines[2].c = vertex[3].y;
		lines[3].a = 1 ; lines[3].b = 0 ; lines[3].c = vertex[3].x;
	}

private:

};
*/


} // end namespace Vast

#endif // TYPEDEF_H

