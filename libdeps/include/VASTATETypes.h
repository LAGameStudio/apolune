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
 *  VASTATETypes.h -- VASTATE - shared basic components
 *
 *  ver 0.1 (2006/07/18)
 *   
 */

#ifndef VASTATE_TYPES_H
#define VASTATE_TYPES_H


#ifdef WIN32
#pragma warning(disable: 4786)  // disable warning about "identifier exceeds'255' characters"
#endif


#include <map>

// TODO: reliable/unreliable flag for each attribute
//       hidden Object for user position

#include "VASTVerse.h"
#include <vector>

#include <string>           // for UID
#include <cstring>          // for UID
#include <cstdio>

#define VASTATE_BUFSIZ  (10240)         // size for generic VASTATE buffer

// flags that define VASTATE behaviors
#define UID_FOR_OBJECT_ID_              // use UID as object ID
#define DISCOVERY_BY_NOTIFICATION       // allow arbitrators to pro-actively notify agents of objects
#define DISCOVERY_BY_REQUEST            // allow agents to request full states from arbitrators when learning updates for unknown objects

#define DECLEAR_OWNERSHIP_ON_TRANSFER_  // notify neighboring arbitrators of a newly owned object

#define OBJECT_KEEP_ALIVE_              // send position updates for each objects once a while
#define CLEAN_NONALIVE_OBJECT_          // periodically remove non-alive objects 

// load balancing settings
#define ARBITRATOR_MOVEMENT_FRACTION (0.1f) // fraction of remaining distance to move for arbitrators
#define REPORT_LOADING_                 // notify enclosing arbitrators of loading periodically

// fault tolerance options
#define FAULT_TOLERANT_0_               // no fault tolerance
#define FAULT_TOLERANT_1_               // fault tolerance method 1 (full object backup at neighbors + event publication to all)
#define FAULT_TOLERANT_2                // fault tolerance method 2 (one backup only at neighbors + targeted event)

#define EVENT_TO_ENCLOSING_ARBITRATORS  // send event also to enclosing arbitrators

#ifdef FAULT_TOLERANT_0
  #define SEND_OBJECT_ON_TRANSFER       // send full states of the objects to a neighbor arbitrator when transferring ownership
#endif

#ifdef FAULT_TOLERANT_1
  #define BACKUP_STATES               // keep extra copies of states at neighboring arbitrators  
  #define PUBLISH_EVENT                 // agents's events are sent to arbitrators via publications (instead of directly) 
#endif


//
// NOTE: using lean backup, objects' most current states are stored at the closest
//       neighbor arbitrator only, when the closest neighbor arbitrator changes,
//       the new closest may not know all the state changes that had occur
//       since its last knowledge of the object.
//       TODO: perhaps a full backup / store is needed, every time the 
//             closest neighbor changes? (but this may be too costly?)
//
#ifdef FAULT_TOLERANT_2  
  #define BACKUP_STATES
  #define BACKUP_STATES_LEAN            // backup states only to object's nearest arbitrator
#endif

//
// various system parameters (thresholds & countdown values)
//

#define COUNTDOWN_REMOVE_AVATAR 20      // # of steps to delete disconnected avatar object
#define COUNTDOWN_PROMOTE       20      // # of steps can insert a new Arbitrator in the same area after an Arbitrator inserted
#define COUNTDOWN_TRANSFER      5       // # of steps before transfering ownership to a neighbor
#define COUNTDOWN_TAKEOVER      5       // # of steps to wait before ownership takeover
#define COUNTDOWN_REMOVE_OBJ    5       // # of steps to wait before removing a non-AOI object at an agent

#define TIMEOUT_EXPIRING_OBJECT     (10)    // time-steps to delete an un-ownered object (assuming 25 steps / sec * 10 sec)
#define TIMEOUT_JOINING             (25*3)  // time-steps to re-send a join request
#define TIMEOUT_OVERLOAD_REQUEST    (25*1)  // time-steps to re-send a overload help request

#define STAT_REPORT_INTERVAL_IN_SEC 10      // # of seconds to report a stat collection

//
// attribute IDs for storing different datatypes in Attribute class
//
#define VASTATE_ATTRIBUTE_TYPE_BOOL   1
#define VASTATE_ATTRIBUTE_TYPE_INT    2
#define VASTATE_ATTRIBUTE_TYPE_FLOAT  3
#define VASTATE_ATTRIBUTE_TYPE_STRING 4
#define VASTATE_ATTRIBUTE_TYPE_VEC3   5

//
// other settings
//
#define VASTATE_BUFFER_RATIO (0.05f)    // additional percentage radius to compensate movement during object discovery

#define VAST_LAYER_EVENT                1   // layer # for sending user events in VAST
#define VAST_LAYER_UPDATE               2   // layer # for sending object updates in VAST

namespace Vast 
{   
    // system parameters used by VASTATE
    struct VASTATEPara
    {
        length_t    world_width;        // dimension of the whole world
        length_t    world_height;       // to determine initial subscription area, also to check arbitrator insertion position, 
        length_t    default_aoi;        // default AOI size (to check if arbitrator insertion position is legal)        
        unsigned    overload_limit;     // # of agents considered overloaded at an arbitrator
    };

    // NOTE: change in message ID should also reflect in VASTATETypes.cpp 
    typedef enum
    {
        // basic login
        LOGIN = VON_MAX_MSG,    // an agent attempts to login into the system
        LOGOUT,                 // an agent attempts to logout of the system
        JOIN,                   // an agent is appearing at a given location
        LEAVE,                  // an agent departs from a location (opposite to JOIN)
        
        // event & object management
        EVENT,                  // Notification of an Agent-generated Event        
        MOVEMENT,               // Notification of a movement event
        OBJECT_C,               // Notification of Object creation
        OBJECT_D,               // Notification of Object destruction
        OBJECT_R,               // Request for full object state transfer
        POSITION,               // Notification of Object position update (in response to OWNER)
        STATE,                  // Notification of attribute creation and update
        AGENT_MSG,              // Message from agent to server (gateway arbitrator)
        GATEWAY_MSG,            // Message from server to agent
        
        // ownership transfer
        OWNER,                  // Notification of objects that I own
        REJOIN,                 // notification from arbitrator to agent to re-join
        NEWOWNER,               // Auto ownership assumption if arbitrators fail.        
        TRANSFER,               // Ownership transfer from old to new owner (arbitrator)
        TRANSFER_ACK,           // Acknowledgement of ownership transfer
        ARBITRATOR,             // notify an agent that I'm its arbitrator
        
        // load balancing
        CAPCITY,                // Notification of an Agent's capcity to the gateway
        PROMOTE,                // Info for the overloaded and the newly promoted arbitrator.
        ARBITRATOR_C,           // notify gateway of a candidate arbitrator
        ARBITRATOR_J,           // notify gateway of a new arbitrator join (so to remove from candidate)        
        OVERLOAD_M,             // Overloaded arbitrator's request for moving closer
        OVERLOAD_I,             // Overloaded arbitrator's request for inserting new arbitrator
        UNDERLOAD,              // Underloaded arbitrator's request for helping others        
        LOADING,                // reporting current loading (supported agent #) to neighbor arbitrators

        // fault tolerance
        SWITCH,                 // switching backup arbitrator 

        // misc
        STAT,                   // statistics collections

    } VASTATE_Message;

    // class for Universally Unique ID (used as object identifier)
    class EXPORT UID : public Serializable
    {
    public:
        #define UID_BYTE_LEN 16
        #define UID_CHAR_LEN 36
        typedef unsigned int  B32;
        typedef unsigned char B08;

        static UID NULL_ID;
    
        // constructors and destructor
        UID ();
        UID (const B08* b);
        UID (const char* s);
        UID (B32 id);
        ~UID();
    
        // copy constructor
        UID (const UID& r);
    
        // comparisons
        bool   operator== (const UID& r) const;
        bool   operator!= (const UID& r) const;
        bool   operator<  (const UID& r) const;
        bool   operator>  (const UID& r) const;
    
        // assignments
        UID&  operator=  (const UID& r);
        bool   fromString (const char* src);    ///< set value from UID string
        void   fromBytes  (const B08*  bytes);  ///< set value from bytes
        void   setZero    ();                   ///< make empty UID
        const string& toString () const;        ///< convert to UID string;
        char*  toBytes    () const;             ///< get bytes data
        static UID rand4 ();                   ///< generate a random version4 UID


        //
        // serialization
        //
        // encode an to a buffer, return the number of bytes encoded
        // buffer can be NULL (simply to query the total size)
        // returns the total size of the packed class
        size_t serialize (char *buf) const;

        // decode an serialized Event back to this class
        // returns number of bytes restored (should be > 0 if correct)
        size_t deserialize (const char *buf);

        // size of this class, must be implemented
        size_t sizeOf () const;

    private:
        B08 _data[UID_BYTE_LEN];
    };

    #define VASTATE_OBJID_PRIVATE_BITS (16) // # of bits reserved for local object unique ID assignment 

#ifdef UID_FOR_OBJECT_ID
    typedef UID  obj_id_t;                 // unique id for each game object
    const obj_id_t NULL_OBJECT_ID ((Vast::UID::B32)0);    
#else
    typedef unsigned long  obj_id_t;        // unique id for each game object
    const obj_id_t NULL_OBJECT_ID = 0;
#endif

    typedef unsigned long  query_id_t;      // unique id for referencing a given query
    typedef unsigned long  version_t;       // sequential ver# for obj/attribute updates   

    // converter of an ID to a HEX string 
    class ID_STR 
    {        
    public:
                
        static void toString (unsigned long id, char *str)
        {            
            static char c[] = "01234567890ABCDEF";
            sprintf (str, "%c%c%c%c%c%c%c%c", c[(id >> 28) & 0x0F], 
                c[(id >> 24) & 0x0F], 
                c[(id >> 20) & 0x0F], 
                c[(id >> 16) & 0x0F], 
                c[(id >> 12) & 0x0F], 
                c[(id >> 8) & 0x0F], 
                c[(id >> 4) & 0x0F], 
                c[id & 0x0F]);
            str[8] = 0;
        }
    };    

    //
    // message package to travel on the network
    //
    

    class Msg_OBJECT
    {
    public:
        Msg_OBJECT ()
        {
        }

        Msg_OBJECT (char *p)
        {
            memcpy (this, p, sizeof (Msg_OBJECT));
        }

        obj_id_t    obj_id;
        Position    pos;
        version_t   pos_version;
    };
    
    class Msg_STATE
    {
    public:
        Msg_STATE ()
        {
        }
        
        Msg_STATE (char *p)
        {
            memcpy (this, p, sizeof (Msg_STATE));
        }
        
        obj_id_t    obj_id;
        version_t   version;
        size_t      size;
    };
    
    // used for packing/unpacking Attributes
    class Msg_ATTR_UPDATE
    {
    public:        
        byte_t index;
        byte_t type;
        word_t length;
    };
        
    // used for TRASNFER, signalling which Object versions a node already knows
    class Msg_NODE_UPDATEINFO
    {
    public:
        id_t        node_id;
        version_t   version;
    };


    class Msg_EVENT
    {
    public:
        byte_t      type;        
        version_t   version;

        id_t        sender;
        timestamp_t timestamp;
    };

    /*
    class Msg_TRANSFER
    {
    public:
        Msg_TRANSFER ()
        {
        }

        Msg_TRANSFER (char * buf)
        {
            memcpy (this, buf, sizeof (Msg_TRANSFER));
        }

        ~Msg_TRANSFER ()
        {
        }

        obj_id_t obj_id;
        id_t     new_owner;
        id_t     old_owner;
    };
    */


    // a list of Attributes used by an Object or an Event
    class EXPORT Attributes
    {
    public:

        Attributes ()
        {
            type = 0;
            dirty = false;
            version = 0;            
        }

        virtual ~Attributes ()
        {

        }

        // return the size of the attribute list
        virtual byte_t size () = 0;
        
        // store a new attribute into the list
        // returns the index within the list
        virtual int add (bool   value) = 0;
        virtual int add (int    value) = 0;
        virtual int add (float  value) = 0;
        virtual int add (const string &value) = 0;
        virtual int add (const Position &value) = 0;        
        
        // get the attribute value by index
        virtual bool get (int index, bool   &value) = 0;
        virtual bool get (int index, int    &value) = 0;
        virtual bool get (int index, float &value) = 0;     
        virtual bool get (int index, string &value) = 0;
        virtual bool get (int index, Position &value) = 0;
        
        // replace the existing value of an attribute by index (dirty flag will be set)
        virtual bool set (int index, bool   value) = 0;
        virtual bool set (int index, int    value) = 0;
        virtual bool set (int index, float value) = 0;        
        virtual bool set (int index, const string& value) = 0;
        virtual bool set (int index, const Position &value) = 0; 
        
        // encode attributes with optional 'dirty only' copy (which will unset all dirty flag)
        virtual size_t pack (char **, bool dirty_only = false) const = 0;
        
        // unpack an encoded bytestring
        virtual size_t unpack (char *str) = 0;
                
        byte_t      type;           // type is an app-specific identifer for the specific Object or Event
        bool        dirty;          // right now any updated attribute will be re-sent in whole 
        version_t   version;
        
    };
    
    // a list of Attributes used by an Object or an event
    class EXPORT AttributesImpl : public Attributes
    {
    public:
        
        AttributesImpl ();

        // copy constructor
        AttributesImpl (AttributesImpl const &a);

        ~AttributesImpl ();
        
        // return the size of the attribute list
        byte_t size ();
        
        // store a new attribute into the list
        // returns the index within the list
        int add (bool   value);
        int add (int    value);
        int add (float  value);
        int add (const string &value);
        int add (const Position &value);
        
        // get the attribute value by index
        bool get (int index, bool   &value);
        bool get (int index, int    &value);
        bool get (int index, float  &value);     
        bool get (int index, string &value);
        bool get (int index, Position &value);  
        bool get (int index, void **ptr, word_t &length);

        // replace the existing value of an attribute by index (dirty flag will be set)
        bool set (int index, bool   value);
        bool set (int index, int    value);
        bool set (int index, float  value);        
        bool set (int index, const string& value);
        bool set (int index, const Position &value); 
        

        // encode attributes with optional 'dirty only' copy (which will unset all dirty flag)
        size_t pack (char **, bool dirty_only = false) const;
        
        // unpack an encoded bytestring
        size_t unpack (char *str);      

        // if a particular field has been reset
        bool isDirty (int index);

        // clear all dirty flags
        void resetDirty ();       

    private:
        byte_t          _size;          // number of attributes stored         
        vector<byte_t>  _types;         // 16 bytes in memory
        string          _data;          // 16 bytes in memory
        vector<word_t>  _length;
        vector<bool>    _dirty;         // per attribute dirty flag

        inline int startIndex (int index);
        inline const char *startPointer (int index);
        void printData ();
    };
    
    class Event : public AttributesImpl, public Serializable
    {
    public:
        Event () 
        {
        }

        Event (id_t sender, msgtype_t event_type, timestamp_t occur_time)
            :_sender (sender), _timestamp (occur_time)
        {
            type = (byte_t)event_type;
        }

        id_t        getSender ();
        timestamp_t getTimestamp ();

        // encode an Event to a buffer, return the number of bytes encoded
        // buffer can be NULL (simply to query the total size)
        // returns the total size of the packed class
        size_t serialize (char *buf) const;

        // decode an serialized Event back to this class
        // returns number of bytes restored (should be > 0 if correct)
        size_t deserialize (const char *buf);

        // size of this class, must be implemented
        size_t sizeOf () const;

    private:
        id_t        _sender;
        timestamp_t _timestamp;
    };
        
    class EXPORT Object : public AttributesImpl
    {
    public:
        Object ();

        Object (obj_id_t &id);

        Object (Object const &obj);

        ~Object ();

        const obj_id_t &getID () const;
        
        void setPosition (const Position &pos);

        const Position &getPosition () const;
                
        void markDeleted ();

		bool isAlive ();

        bool isAOIObject (Node &n, bool add_buffer = false);

        const char *toString () const;
            
        // encode an Object header, returns the # of bytes encoded
        size_t encodePosition (Message &msg, bool dirty_only);

        bool decodePosition (Message &msg);
        // encode an Object states
        // returns the # of bytes encoded
        // packing order: 
        //      1.    Msg_STATE
        //      2.    encoded bytestring (variable length)        
        size_t encodeStates (Message &msg, bool dirty_only);
        
        bool decodeStates (Message &msg);
                    
        id_t        agent;
        bool        pos_dirty;    // new position has been given		
        bool	    visible;
        version_t   pos_version;

    private:
        obj_id_t    _id;        // object ID, transferable over network
        Position    _pos;       // object position, transferable over network
        
        bool        _alive;     // whether this object is alive, local use only

    };

    // an object inside object store
    class StoredObject
    {
    public:

        StoredObject ()
        {
            obj         = NULL;
            is_owner    = false;
            in_transit  = 0;
            last_update = 0;
            closest_arb = 0;
        };
    
        Object *    obj;            // pointer to the object
        bool        is_owner;       // whether it is owned by the local host
        timestamp_t in_transit;     // countdown of object in ownership transfer to others 
        timestamp_t last_update;    // time of object's last update (used for Object expiring)
        id_t        closest_arb;    // ID for closest arbitrator
    };

    //
    // message for transport over network
    //

    class Msg_NODE
    {
    public:
        Msg_NODE () 
        {
        }
        
        Msg_NODE (char const *p)
        {
            memcpy (this, p, sizeof(Msg_NODE));
        }
        
        Msg_NODE (Node &n, int c)
            :node(n), capacity(c)
        {
        }
        
        void set (Node const &node, int c)
        {
            this->node     = node;
            this->capacity = c;
        }
        
        Node        node;  
        int         capacity;
    };

} // end namespace Vast

#endif // VASTATE_TYPES_H
