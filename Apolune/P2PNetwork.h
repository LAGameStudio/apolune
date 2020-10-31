#pragma once
#include <VASTVerse.h>

#define VAST_EVENT_LAYER    1                   // layer ID for sending events
#define VAST_UPDATE_LAYER   2                   // layer ID for sending updates

#define INPUT_SIZE 1024
 
using namespace Vast;
using namespace std;

class P2PNetwork {
public:
 // VAST-specific variables
 VASTVerse *     world;
 VAST *          self;
 id_t            subscription;     // subscription # for my client (peer)
 Area            aoi;              // my AOI (with center as current position)
 Vast::NodeState state;
 Voronoi      *  voronoi;   // access to Voronoi class
 VASTPara_Net    parameters;  // network parameters
 char recv_buf[INPUT_SIZE];
 P2PNetwork(void) {
  state = ABSENT;
  world = NULL;
  self = NULL;
  voronoi = NULL;
  subscription=0;
 }
 void Init(string gateway_address, unsigned short p ) {
   parameters.port = p;
   parameters.gateway.publicIP = IPaddr(gateway_address.c_str(), parameters.port);
    
   // if physical coordinate is assigned, then no inference will be made, 
   // only do it for gateway (as localhost ping is not accurate can physical coord may not converge)
   if (parameters.is_gateway) parameters.phys_coord = aoi.center;
   world = new VASTVerse (&parameters, NULL);
 }

 // make sure we've joined the network
 void CheckJoin()
 {
  // create the VAST node
  if (state == ABSENT) {
   if ((self = world->createClient ()) != NULL) {
    // non-gateway join as Client only 
    if (self->getSelf()->id == NET_ID_GATEWAY)
    self->join (aoi.center, true);
    else
    self->join (aoi.center, false);
    // everyone joins as relay
    //g_self->join (g_aoi.center, true);
    state = JOINING;
   }
  }       
  else if (state == JOINING) {           
   if (self->isJoined()) {
    subscription = self->subscribe (self->getSelf ()->aoi, VAST_EVENT_LAYER);
    state = JOINED;
   }
  }
 }
 void Loop() {
  voronoi=self->getVoronoi(subscription);
  size_t size=0;
  do {
   Message *msg = NULL;
   if ((msg = self->receive()) != NULL) {
    size = msg->extract(recv_buf, 0);
    recv_buf[size]=0;
   }
   else size=0;
  }
  while (size > 0);
  CheckJoin();
  world->tick(0);
 }
 void MoveTo( int x, int y ) {
  aoi.center.x = (coord_t)x;
  aoi.center.y = (coord_t)y;
 }
 void Update() {
  self->move(subscription, aoi);
 }
 // Preferred over Send()
 void Publish( char *in ) {
  Message send_msg (12);
  send_msg.clear(123);
  send_msg.store(in, strlen(in), true);
  self->publish (aoi, VAST_EVENT_LAYER, send_msg);
 }
 void Send( char *in ) {
  Message send_msg (12);
  send_msg.clear(123);
  send_msg.store(in, strlen(in), true);
  vector<Node *>&nodes = self->list();
  for(size_t j=0; j < nodes.size (); j++)
  {
   Position pos = nodes[j]->aoi.center;
   id_t id      = nodes[j]->id;
   send_msg.addTarget (id);
  }
  self->send (send_msg);
 }
 ~P2PNetwork(void) {
  self->leave();
  world->destroyClient(self);
  delete world;
 }
};