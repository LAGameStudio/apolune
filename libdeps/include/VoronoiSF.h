/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2005 Shun-Yun Hu (syhu@yahoo.com)
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


#ifndef VAST_VORONOI_SF_H
#define VAST_VORONOI_SF_H

#include "VASTTypes.h"
#include "Voronoi.h"
#include "VoronoiSFAlgorithm.h"
#include <vector>

using namespace std;

namespace Vast {

typedef pair<id_t, Position>        site_t;  // HPPair;    
typedef vector<site_t>::iterator    site_it; // HPPVItor;

class VoronoiSF : public Voronoi 
{

public:
    VoronoiSF ();
    ~VoronoiSF ();

    // insert a new site, the first inserted is myself
    void insert (id_t id, const Position &coord);

    // remove a site
    void remove (id_t id);
    
    // modify the coordinates of a site
    void update (id_t id, const Position &coord);

    // get the point of a site
    Position get (id_t id);
    
    // check if a point lies inside a particular region
    bool contains (id_t id, const Position &coord);
    
    // check if the node is a boundary neighbor
    bool is_boundary (id_t id, const Position &center, length_t radius);
        
    // check if the node is an enclosing neighbor
    bool is_enclosing (id_t id, id_t center_node_id = ((id_t)-1));    

    // get a list of enclosing neighbors
    //int get_en (id_t id, vector<id_t> &list);
    vector<id_t> &get_en (id_t id, int level = 1);
    
    // check if a circle overlaps with a particular region
    bool overlaps (id_t id, const Position &center, length_t radius, bool accurate_mode = false);

    // remove all sites in the diagram
    void clear ();

    //
    // non Voronoi-specific methods
    //

    // returns the closest node to a point
    id_t closest_to (const Position &pt);

    std::vector<line2d> &getedges();

    // obtain the bounding box for this Voronoi object
    // returns true if the box exists, false if one of the dimensions is empty
    bool get_bounding_box (point2d& min, point2d& max);

    // expose for displaying partitioning scheme
    //sfVoronoi &get_Voronoi ();

    vector<site_t> &get_sites ()
    {
        return _sites;
    }

    // get the number of sites currently maintained
    int size ()
    {
        return _sites.size ();
    }

    // get edges of sites with ID = id
    std::set<int> & get_site_edges (int id);


        
private:    

    //
    // Peter variables
    //    
    
    bool invalidated;    
    sfVoronoi       _Voronoi;       // mVoronoi;
    vector<site_t>  _sites;         // mHPPVector;    
   
    //int handleToSiteID(int h);
    int get_idx (id_t h);    
    void recompute();

    vector<id_t> _en_list;
};

    
} // end namespace Vast

#endif // VAST_VORONOI_SF_H

