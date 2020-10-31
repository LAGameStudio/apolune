

/*
*  Vivaldi.cpp -- vivaldi algorithm implementation
*
*/

#include "Vivaldi.h"
#include <stdlib.h> // rand ()

#define PRINT_MSG_

namespace Vast {

Vivaldi::Vivaldi ()
: _index (0), _band_index (0), _error (0), _tolerance (0.2f), _con_frac (0.1f), _con_error (0.1f), _dtime (0.25f)
{	

	if (read_para () == false)
	{
		//_node_size = 1000;
		_phys_map_size = 256;
	}

	if (read_latency () == false)
	{
		printf ("Latency table reading error ... ");
		system ("pause");
	}

	if (read_bandwidth () == false)
	{
		// use defaults gnutella nodes bandwidth distribution on paper
		// "Analyzing and Improving a BitTorrent Network's Performance Mechanisms", INFOCOM in 2006
		// 1.Down(Bytes/S) 2.Up(Bytes/S) 3.RangeBase (Cumulative Distribution Function)

		SimBand bt_data[] =
		{
			{ 81920,	40960,	0  },
			{ 117760,	58880,	20 },
			{ 171520,	85760,	40 },
			{ 230400,	115200,	60 },
			{ 307200,	153600,	70 },
			{ 460800,	230400,	80 },
			{ 768000,	384000,	90 }
		};

		if (bt_data == NULL)
		{
			printf ("Null pointer error\n");
			system ("pause");
		}

		for (int s = sizeof (bt_data) / sizeof (bt_data[0]), i = 0; i < s; ++i)
			_band_table.push_back (bt_data[i]);
	}

	create_bandwidth ();

	init_physcoord ();

	//proc_vivaldi_simple ();
	proc_vivaldi_adaptive ();
	sort_coordinate ();
/*
	int x[100];
	for (int i=0 ; i < 100;i++)
	{
		x[i] = get_index (i);
		if ( (i % 3) == 0)
			free_index (i);
	}
*/
}

Vivaldi::~Vivaldi ()
{

}

void Vivaldi::sort_coordinate ()
{
	Position center (_phys_map_size / 2, _phys_map_size / 2, 0);
	float distance = 0;

	for (int i = 0; i < _node_size; i++)
	{
		DisIndex disIndex;
		distance = _nodes[i].PHYS_COORD.distance (center);
		disIndex.dis = distance;
		disIndex.index = i;
		_distance.push_back (disIndex);
	}

	for (int i = 0; i < _node_size; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (_distance[j].dis > _distance[i].dis)
			{
				DisIndex temp;
				temp.dis = _distance[i].dis;
				temp.index = _distance[i].index;
				_distance[i].dis = _distance[j].dis;
				_distance[i].index = _distance[j].index;
				_distance[j].dis = temp.dis;
				_distance[j].index = temp.index;
			}
		}
	}

	for (int i=0 ; i < _node_size; i++)
	{
		_sortindex[i] = _distance[i].index;
	}
}

void Vivaldi::init_physcoord ()
{
	// random a initial position for physical coordinate
	//srand ((unsigned)time(NULL));

	for (int i = 0; i < _node_size ; i++)
	{
		PhysCoord node;

		// 256 * 256 coordinate range size
		node.PHYS_COORD.set (rand24 () * _phys_map_size, rand24 () * _phys_map_size, 0);
		node.P_ERROR = 0;
		node.E_RATE = 1;

		_nodes.push_back (node);
	}
}

void Vivaldi::proc_vivaldi_simple ()
{
	Position force, vector;           // force on a node and vector between two coordinate
	float error;                      // error estimation

	do
	{
		force.set (0, 0, 0);
		_error = 0;

		for (int i = 0; i < _node_size; i++)
		{
			_nodes[i].P_ERROR = 0;

			for (int j = 0; j < _node_size; j++)
			{	
				if (i == j)
					continue;

				vector.set (0, 0, 0);

				// e = L(i,j) - || phy_coord(i) - phy_coord(j) ||
				error = _latency_table[i][j] - _nodes[i].PHYS_COORD.distance (_nodes[j].PHYS_COORD);
				_nodes[i].P_ERROR += error;

				// F = F + e * vector(phy_coord(i) - phy_coord(j))
				unit_vector (vector, _nodes[i].PHYS_COORD, _nodes[j].PHYS_COORD);
				vector *= error;
				force += vector;
			}

			// node's average error
			_nodes[i].P_ERROR /= (_node_size - 1);

			// sum up the average error
			_error += _nodes[i].P_ERROR;

			// phy_coord(i) = phy_coord(i) + t * F
			force *= _dtime;
			_nodes[i].PHYS_COORD += force;
#ifdef PRINT_MSG_
			printf ("Peer[%d]'s mean error is %f\n",i ,_nodes[i].P_ERROR);
#endif
		}
#ifdef PRINT_MSG_
		printf ("Coordinate's mean error is %f\n",_error /= _node_size);
#endif

	}
	while (fabs (_error /_node_size) > _tolerance );
}

void Vivaldi::proc_vivaldi_adaptive ()
{
	Position vector;                   // force on a node and vector between two coordinate
	float error, relative_error, weight;      // error, relative error between two node and weight for local and remote error

	do
	{
		_error = 0;

		for (int i = 0; i < _node_size; i++)
		{
			for (int j = 0; j < _node_size; j++)
			{	
				if (i == j)
					continue;

                // create a unit vector between node i & j's physical coordinate
				vector.set (0, 0, 0);
				unit_vector (vector, _nodes[i].PHYS_COORD, _nodes[j].PHYS_COORD);

				// weight = err_i / (err_i + err_j)
				weight = _nodes[i].E_RATE / (_nodes[i].E_RATE + _nodes[j].E_RATE);

                // calculate & update error, defined as difference between
                // actual & estimated latency (distance between physical coordinates)
				// e = L(i,j) - || phy_coord(i) - phy_coord(j) ||
                coord_t dist = _nodes[i].PHYS_COORD.distance (_nodes[j].PHYS_COORD);                               
				             
                error = _latency_table[i][j] - dist;
				_nodes[i].P_ERROR += error;

				// e_s = | || phy_coord(i) - phy_coord(j) || - rtt | / rtt
				relative_error = rel_err_sample (dist, _latency_table[i][j]);

				// update weighted moving average of local error
				_nodes[i].E_RATE = relative_error * _con_error * weight + _nodes[i].E_RATE * (1 - _con_error * weight);

				// update local coordinates
				_dtime = _con_frac * weight;
				vector *= (_dtime * (_latency_table[i][j] - dist));
				_nodes[i].PHYS_COORD += vector;
			}

			// node's average error
			_nodes[i].P_ERROR /= (_node_size - 1);

			// sum up the average error
			_error += _nodes[i].P_ERROR;

#ifdef PRINT_MSG_
			printf ("Peer[%d]'s mean error is %f\n",i ,_nodes[i].P_ERROR);
#endif
		}
#ifdef PRINT_MSG_
		printf ("Coordinate's mean error is %f\n",_error /= _node_size);
#endif

	}
	while (fabs (_error /_node_size) > _tolerance);
}

int Vivaldi::get_index (id_t node_id)
{
	// if node have no index

	if (_id2index.find (node_id) == _id2index.end ())
	{
		if (!_leak_index.empty ())
		{
			_id2index[node_id] = _leak_index[0];
			_leak_index.erase (_leak_index.begin ());
		}
		else
		{
			_id2index[node_id] = _sortindex[_index];
			_index++;
		}
	}

	return _id2index[node_id];
}

bool Vivaldi::free_index (id_t node_id)
{
	if (_id2index.find (node_id) == _id2index.end ())
		return false;
	
	_leak_index.push_back (_id2index[node_id]);
	_id2index.erase (node_id);
	
	return true;
}

bool Vivaldi::release_phys_coord (id_t node_id)
{
	return free_index (node_id);
}

Position Vivaldi::get_phys_coord (id_t node_id)
{
	
	size_t index = get_index (node_id);

	if (_index < _node_size)
		return  _nodes[index].PHYS_COORD;
	else
	{
		printf ("Coordinate Index %d larger than node_size %d...\n", _index, _node_size);
		printf ("No more physical coordinate for nodes...\n");
		return Position (-1, -1, -1);
	}
}


int Vivaldi::get_down_bandwidth (id_t node_id)
{
	if (_band_list.find (node_id) == _band_list.end ())
	{
		_band_list.insert (pair<id_t, int> (node_id, _band_index) );
		return _band[_band_index++].UPLOAD;
	}
	else
	{
		return _band[_band_list[node_id]].DOWNLOAD;
	}
}

int Vivaldi::get_up_bandwidth (id_t node_id)
{
	if (_band_list.find (node_id) == _band_list.end ())
	{
		_band_list.insert (pair<id_t, int> (node_id, _band_index) );
		return _band[_band_index++].UPLOAD;
	}
	else
	{
		return _band[_band_list[node_id]].UPLOAD;
	}
}

float Vivaldi::get_latency (id_t src, id_t dest)
{
	int i = get_index (src);
	int j = get_index (dest);
	
	return _latency_table[i][j];
}

void Vivaldi::unit_vector (Position& V, Position &coord_i, Position &coord_j)
{
	V = coord_i - coord_j;
	V /= coord_i.distance (coord_j);
}


bool Vivaldi::read_para ()
{
	FILE *fp;
	if ((fp = fopen ("Vivaldi.ini", "rt")) == NULL)
            return false;

	int *p[] = {
		&_node_size,
		&_phys_map_size,
		0
	};

	char buff[255];
	int n = 0;
	while (fgets (buff, 255, fp) != NULL)
	{
		// skip any comments or empty lines
		if (buff[0] == '#' || buff[0] == ' ' || buff[0] == '\n')
		    continue;

		// read the next valid parameter
		if (sscanf (buff, "%d", p[n]) != 1)
		    return false;

		n++;

		if (p[n] == 0)
			return true;
	}

	return false;
}

float Vivaldi::rel_err_sample (float distance, float rtt)
{
	return fabs (distance - rtt) / rtt;
}

coord_t Vivaldi::rand24 ()
{
	return ( (rand () << 9) + (rand () >> 6) ) / (float)0x01000000;
}

bool Vivaldi::read_latency ()
{
	// read a given latency table for peers
	_node_size = read_tg_latency ();
	//_node_size = 30;
	//lat_table_size = ReadPairPingLatency (pp_lat_table);

	// error message
	if (_node_size == -1)
	{
		printf ("Fail to reading latency table ...\n");
		return false;
	}
	else if (_node_size == -2)
	{
		printf ("File open error ...\n");
		return false;
	}

	return true;
}


int Vivaldi::read_tg_latency ()
{
	FILE *fp;
#ifdef _DEBUG
	if ((fp = fopen ("latency90Peers.dat", "rt")) == NULL)
#else
	if ((fp = fopen ("latency90peers.dat", "rt")) == NULL)
#endif
		return -2;

	int size;
	float f;
	char buff[20480];

	while (fgets (buff, 20480, fp) != NULL)
	{
		// skip any comments or empty lines
		if (buff[0] == '#' || buff[0] == ' ' || buff[0] == '\t' || buff[0] == '\n')
			continue;

		// read the next valid parameter
		char *p = buff;
		int length = 0;

		if (sscanf (p, "%d%n", &size, &length) == 1)
		{
			vector<float> inner;

			// for parsing character
			p += length;
			for ( ; sscanf (p, "%f%n", &f, &length) == 1 ; p+=length )
				inner.push_back (f);

			_latency_table.push_back (inner);
		}
		else
			return -1;
	}

	size++;
	return size;
}

/*
int Vivaldi::read_pp_latency ()
{
FILE *fp;
if ((fp = fopen ("pairping.dat", "rt")) == NULL)
return -2;

int length = 0;
int size;
int ip_mask1, ip_mask2, ip_mask3, ip_mask4;
char buff[20480];
char *p = buff;
PairPing node;

// parsing number of nodes
while (fgets (buff, 20480, fp) != NULL)
{
if (buff[0] == '#' || buff[0] == ' ' || buff[0] == '\n')
continue;

while (sscanf (p, "%d.%d.%d.%d%n", &ip_mask1, &ip_mask2, &ip_mask3, &ip_mask4, &length ) == 4)
{
NodeList inner;
sprintf (inner.SOUR_IP, "%d.%d.%d.%d\0", ip_mask1, ip_mask2, ip_mask3, ip_mask4);
pp_lat_table.push_back (inner);
p += length + 1;
}

break;
}

size = pp_lat_table.size ();

// parsing pairs-ping for nodes
for (int i = 0; fgets (buff, 20480, fp) != NULL && i < size; i++)
{
p = buff;

for (int j = 0; j < size; ++j)
{
// parsing pattern 1: min_ping / avg_ping / max_ping
if (sscanf (p, "%f/%f/%f%n", &node.MIN_PING, &node.AVG_PING, &node.MAX_PING, &length) == 3)
{
sprintf (node.DEST_IP, "%s", pp_lat_table[j].SOUR_IP);
pp_lat_table[i].DEST_NODE.push_back (node);
//printf ("node[%d][%d]: min_ping = %f, avg_ping = %f, max_ping = %f\n", i, j, node.MIN_PING, node.AVG_PING, node.MAX_PING);
}

// parsing pattern 2: ***no_traceroute***
else if (strncmp(p, "***no_traceroute***", 19) == 0)
{
node.MIN_PING = -1.;
node.AVG_PING = -1.;
node.MAX_PING = -1.;

//pp_lat_table[i].push_back (node);

//printf ("node[%d][%d]: ***no_traceroute***\n", i, j);
length = 19;
}

// parsing pattern 3:  ***last_ip_address***
else if (sscanf (p, "***%d%*[.]%d%*[.]%d%*[.]%d***%n", &ip_mask1, &ip_mask2, &ip_mask3, &ip_mask4, &length) == 4)
{
node.MIN_PING = 10000;
node.AVG_PING = 10000;
node.MAX_PING = 10000;
//pp_lat_table[i].push_back (node);
//printf ("node[%d][%d]: %d.%d.%d.%d\n", i, j, ip_mask1, ip_mask2, ip_mask3, ip_mask4);
}

// parsing pattern 4: *** no data received for node (IP) ***
else if (sscanf (p, "*** no data received for %*[0-9].%*[0-9].%*[0-9].%*[0-9] ***%n", &length) == 0)
{

do
{
node.MIN_PING = -1.;
node.AVG_PING = -1.;
node.MAX_PING = -1.;

pp_lat_table[i].push_back (node);

j++;
}
while (j < g_pp_lat_table.size ());
*/

//printf ("node[%d][%d]: *** no data received for IP... ***", i, j);
//pp_lat_table.erase (g_pp_lat_table.end () - 1);
/*
i--;
break;
}

// exception catch
else
{
printf ("node[%d][%d]: Something Error!!", i, j);
return -1;
}

p += length + 1;
}
}

return size;
}
*/

bool Vivaldi::read_bandwidth ()
{
	FILE *fp;
	if ((fp = fopen ("bandwidth.dat", "rt")) == NULL)
		return false;

	// check whether the sum of percentage read from file equals to 100% or not
	int upload,download = upload = 0;

	char buff[255];
	while (fgets (buff, 255, fp) != NULL)
	{
		// skip any comments or empty lines
		if (buff[0] == '#' || buff[0] == ' ' || buff[0] == '\n')
			continue;

		// read bandwidth from input file
		SimBand data;
		if (sscanf (buff, "%d%d%d", &download, &upload, &data.RANGEBASE ) != 3)
			return false;
		data.DOWNLINK = download;
		data.UPLINK = upload;
		_band_table.push_back (data);
	}

	return true;
}

void Vivaldi::create_bandwidth ()
{
	srand (0);
	// set different bandwidth limitation for each node
	for (int i = 0; i < _node_size; ++i)
	{
		int seed = 0;
		seed = (int)( 100 * ( rand() / ((float)RAND_MAX + 1))  );

		for (int j = _band_table.size () - 1; j >= 0 ; --j)
		{
			Bandwidth bandwidth;
			if (seed >= _band_table[j].RANGEBASE)
			{
				//bandwidth.DOWNLOAD = (_band_table[j].DOWNLINK << 7) / _para.STEPS_PERSEC;
				//bandwidth.UPLOAD = (_band_table[j].UPLINK << 7) / _para.STEPS_PERSEC;
				bandwidth.DOWNLOAD = (_band_table[j].DOWNLINK);
				bandwidth.UPLOAD = (_band_table[j].UPLINK);
				_band.push_back (bandwidth);
				break;
			}
		}
	}
}

void Vivaldi::get_bandwidth( Bandwidth& bandwidth )
{

	//int seed = (int)( 100 * ( rand() / (float)(RAND_MAX + 1) ) );
	int seed = rand() % 100;

	for (int j = _band_table.size () - 1; j >= 0 ; --j)
	{		
		if (seed >= _band_table[j].RANGEBASE)
		{
			//bandwidth.DOWNLOAD = (_band_table[j].DOWNLINK << 7) / _para.STEPS_PERSEC;
			//bandwidth.UPLOAD = (_band_table[j].UPLINK << 7) / _para.STEPS_PERSEC;
			bandwidth.DOWNLOAD = (_band_table[j].DOWNLINK);
			bandwidth.UPLOAD = (_band_table[j].UPLINK);
			//_band.push_back (bandwidth);
			break;
		}
	}
	//return bandwidth;
}

} // end namespace Vast

