

#ifndef VASTSIM_VIVALDI_H
#define VASTSIM_VIVALDI_H

#include <time.h>

#include "VASTUtil.h"
/*
#include "Movement.h"
#include "VASTVerse.h"
#include "VASTsim.h"
*/

//#include "ReadFunc.h"

using namespace std;

namespace Vast {

typedef struct 
{
	int DOWNLOAD;
	int UPLOAD;
} Bandwidth;

typedef struct
{
	int     DOWNLINK;
	int     UPLINK;
	int     RANGEBASE;          // random range for peer's bandwidth
} SimBand;

typedef struct
{
	Position   PHYS_COORD;      // physical coordinate
	float      P_ERROR;         // a node's self-predicted error 
	float      E_RATE;          // error estimate of node for adaptive timestep
} PhysCoord;

typedef struct
{
	char DEST_IP[16];
	float MIN_PING;
	float AVG_PING;
	float MAX_PING;
} PairPing;                     // pair ping per node

typedef struct
{
	char SOUR_IP[16];
	vector<PairPing> DEST_NODE;
} NodeList;                     // node's ping list

typedef struct 
{
	float dis;
	int   index;
} DisIndex;


/************************************************************************/
/*                        Vivaldi Algorithm                             */
/************************************************************************/

class EXPORT Vivaldi
{
public:

	//Vivaldi(){};
	Vivaldi (/*SimPara &para*/);

	~Vivaldi ();

	void proc_vivaldi_simple ();                                   // simple algorithm

	void proc_vivaldi_adaptive ();                                 // adaptive algorithm for convergence

	bool release_phys_coord (id_t node_id);                        // release used coordinate

	Position get_phys_coord (id_t node_id);                        // get a physical coordinate

	int get_down_bandwidth (id_t node_id);

	int get_up_bandwidth (id_t node_id);

	float get_latency (id_t src_node, id_t dest_node);

	bool replace_id (id_t old_id, id_t new_id);

	void get_bandwidth (Bandwidth& bandwidth);

private:

	void  init_physcoord ();

	void  unit_vector (Position& V, Position &coord_i, Position &coord_j);

	bool  read_para ();

	float rand24 ();                                               // RAND24_MAX = 16777215 

	float rel_err_sample (float distance, float rtt);

	bool  read_latency ();

	bool  read_bandwidth ();

	int   read_tg_latency ();

	void  create_bandwidth ();

	int   get_index (id_t node_id);

	bool  free_index (id_t node_id);

	void  sort_coordinate ();
	


	//bool                 ReadLatency (int &lat_size, vector<NodeList>& pp_lat_table);
	//int   ReadPairPingLatency ();

	// basic parameter
	//SimPara                       _para;
	vector<SimBand>               _band_table;                     // bandwidth cumulative distribution fraction
	vector<PhysCoord>             _nodes;                          // physical nodes position
	vector<Bandwidth>             _band;                           // nodes bandwidth
	vector<vector <coord_t> >     _latency_table;                  // TopGen latency table
	map<id_t, int>                _band_list;                      // nodes bandwidth list

	map<id_t, int>                _id2index;                       // id to index
	map<int, int>                 _sortindex;
	vector<DisIndex>              _distance;
	vector<int>                   _leak_index;
	int                           _index;                          // union index for latency and coordinate


	// general measurement use
	//vector<NodeList>              _pp_latency_table;

	// for more accurate measurement use
	//vector<NodeList>              _latency_table_pp;                     // pair ping latency table

	unsigned int                  _band_index;                     // the bandwidth index has been used
	int                           _node_size;                      // for latency or bandwidth table size
	int                           _phys_map_size;                  // physical coordinate system map size
	float                         _error;                          // total error in physical coordinate
	float                         _tolerance;                      // the error value we can allow of this coordinate
	float                         _con_frac;                       // Cc, constant fraction of a node's estimated error
	float                         _con_error;                      // Ce, constant for tuning parameter
	float                         _dtime;                          // determine how far a node moves at each time interval
};

} // end namespace Vast

#endif
