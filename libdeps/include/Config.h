/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2005-2007 Shun-Yun Hu (syhu@yahoo.com)
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

#ifndef VAST_CONFIG_H
#define VAST_CONFIG_H

#ifdef WIN32

// disable warning about "identifier exceeds'255' characters"
#pragma warning(disable: 4786)

// disable warning about "unreferenced formal parameter"
#pragma warning(disable: 4100)

// disable warning about C4996: 'sprintf' was declared deprecated
#pragma warning(disable: 4996)

// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#pragma warning (disable : 4251)

#endif

#ifdef _WINDOWS
#define EXPORT __declspec (dllexport)
#else
#define EXPORT /* nothing */
#endif

#include <stdio.h>      // for printing out debug messages

#define VAST_BUFSIZ     32000       // generic message buffer size, note that send/recv cannot exceed this size
#define VAST_MSGSIZ     1024        // generic message size, can expand if not enough
#define VON_MAX_MSG     20          // maximum number of VONpeer messages


// when only simulation is run
#define SIMULATION_ONLY_

// whether to record statistics during simulation runs (disable will run faster)
#define SIM_RECORD_STAT

// whether VAST should send timestamps to calculate & record transmission latencies
#define VAST_RECORD_LATENCY_

//
// for topology-aware simulations
//

// enable latency table be used (make sure it's in the working directory)
#define  ENABLE_TOPOLOGY_AWARE_

#ifdef SIMULATION_ONLY
#define ACE_DISABLED
#endif

#define DEBUG_MSG_ID(x) {printf ("[%3d] ", (int)_self.id); printf ((x));}
#define ERROR_MSG_ID(x) {printf ("[%3d] error: ", (int)_self.id); printf ((x));}

#define DEBUG_MSG(x) {printf ((x));}
#define ERROR_MSG(x) {printf ((x));}

#ifdef _DEBUG            
#define DEBUG_DETAIL_       // show detail debug messages
#endif

// force to enable debug message
#define DEBUG_DETAIL_

#define VAST_POSFILE_FORMAT "N%04dW%dx%dS%d.pos"                            

// Send all messages by bandwidth limitation
// /* don't define anything */
// Send only PAYLOAD by bandwidth limitation
//#define VAST_NETEMU_BL_TYPE_SENDDIRECT(msgtype) (msgtype!=PAYLOAD)
// Comment below line to disable a limitation of size of send queue in NET_EMU_BL
#define VAST_NETEMU_BL_SIZED_QUEUE

#endif // VAST_CONFIG_H

