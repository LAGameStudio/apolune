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
 *  VASTATE.h -- VASTATE - State Management based on VAST
 *
 *  ver 0.1 (2006/07/15)
 *      0.2 (2009/05/26)
 */

#ifndef VASTATE_H
#define VASTATE_H

#include "VASTATETypes.h"
#include "Arbitrator.h"
#include "Agent.h"
#include "VASTVerse.h"

#include <map>

// TODO: reliable/unreliable flag for each attribute
//       hidden Object for user position
//       certain attributes are only reflected to clients when requested

namespace Vast 
{        

    class EXPORT VASTATE
    {
    public:
        
        // the VASTVerse should be an already logined & ready-to-use object (i.e., hasLogin () returns true)
        VASTATE (VASTATEPara &para, VASTPara_Net &netpara, VASTPara_Sim *simpara);
        ~VASTATE (); 

        // create a VASTATE node at a specified AOI (includes both an arbitrator and agent component)
        // if 'arb_pos' is given then the node will attempt to join as an arbitrator at the given position
        // returns the created node index
        // NOTE: the 1st arbitrator (gateway) must supply arb_pos so that it can be ready to accept other join requests
        bool createNode (Area &aoi, ArbitratorLogic *arb_logic, AgentLogic *agent_logic, std::string &password, Position *arb_pos = NULL);

        // destroy the current VASTATE node
        void destroyNode ();

        // whether the VASTATE node has been successfully created
        bool isLogined ();

        // move logical clock forward, 
        // input time budget for this tick in millisecond, specify 0 for unlimited budget
        // returns time left in millisecond, 0 for no more time, (-1) for unlimited budget
        int tick (int time_budget = 0);

        // stop operations on this factory
        void pause ();

        // resume operations on this factory
        void resume ();

        // obtain the point to the created agent
        // returns NULL if not yet created
        Agent *getAgent ();

        // obtain the reference to the created arbitrator
        // returns NULL if not yet created
        Arbitrator *getArbitrator ();

        // obtain the tranmission size by message type, default is to return all types
        size_t getSendSize (const msgtype_t msgtype = 0);
        size_t getReceiveSize (const msgtype_t msgtype = 0);

    private:

        // create an Agent entity
        Agent *createAgent (AgentLogic *logic);

        // create an arbitrator
        Arbitrator *createArbitrator (ArbitratorLogic *logic);

        // close down an Agent
        void destroyAgent (Agent *agent);

        // close down an arbitrator
        void destroyArbitrator (Arbitrator *arbitrator);
        
        // create VASTVerse for use by arbitrator / agent
        VASTVerse *createVASTVerse (void *logic);
        void destroyVASTVerse (VASTVerse *vastverse);

        // notify each created VASTverse of a node belonging to the same host
        void recordLocalTarget (id_t target);

        map<void *, VASTVerse *>    _vastverses;        // record for all the VASTverses created       
        vector<Arbitrator *>        _arbitrators;       // list of arbitrators created at this host
        vector<Agent *>             _agents;            // list of agents created at this host
        vector<ArbitratorLogic *>   _arb_logics;        // reference to arbitrator logics
        vector<AgentLogic *>        _agent_logics;      // reference to agent logics    

        VASTATEPara                 _para;              // system parameters
        VASTPara_Net                _netpara;           // network parameters
        VASTPara_Sim *              _simpara;           // simulation parameters                
        std::string                 _pass;              // password for login

        Position                    _arb_position;      // join position for the arbitrator
        Area                        _agent_aoi;         // joining spot for the agent
        NodeState                   _state;             // state for current node
    };

} // end namespace Vast

#endif // VASTATE_H
