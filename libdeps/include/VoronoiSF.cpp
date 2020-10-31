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

#include "VoronoiSF.h"

namespace Vast
{

VoronoiSF::
VoronoiSF ()
    :invalidated (false)
{
}

VoronoiSF::
~VoronoiSF ()
{
}

// insert a new site, the first inserted is myself
void 
VoronoiSF::
insert (id_t id, const Position &pt)
{
	// avoid duplicate insert
	if (get_idx (id) == -1)
	{
		invalidated = true;
		_sites.push_back (site_t (id, pt));
	}
}

// remove a site
void 
VoronoiSF::
remove (id_t id)
{    
    int idx = get_idx (id);
    if (idx != -1)
    {
        invalidated = true;
        _sites.erase (_sites.begin () + idx);
    }
}

// modify the coordinates of a site
void 
VoronoiSF::
update (id_t id, const Position &pt)
{
    int idx = get_idx (id);
    if (idx != -1)
    {
        invalidated = true;
        _sites[idx].second = pt;
    }
}

// get the point of a site
Position
VoronoiSF::
get (id_t id)
{
    return _sites[get_idx (id)].second;
}

// check if a point lies inside a particular region
bool 
VoronoiSF::
contains (id_t id, const Position &pt)
{    
    int idx = get_idx (id);
    if (idx == -1) 
        return false;
    
    recompute();
    bool result = _Voronoi.isPointInsideSiteRegion (idx, point2d (pt.x, pt.y));
    
    return result;
}

// check if the node is a boundary neighbor
bool 
VoronoiSF::
is_boundary (id_t id, const Position &pt, length_t radius)
{   
    int idx = get_idx (id);
    if (idx == -1) 
        return false;
    
    recompute();
    point2d center (pt.x, pt.y); 
    bool result = _Voronoi.enclosed (idx, center, (long)radius);

    return !result;
}

// check if the node 'id' is an enclosing neighbor of 'center_node_id'
bool 
VoronoiSF::
is_enclosing (id_t id, id_t center_node_id)
{   
    recompute();    
    std::set<int> temp;

    int idx = (center_node_id == ((id_t)(-1)) ? 0 : get_idx (center_node_id));
    _Voronoi.getNeighborSet (idx, temp);
    
    for (set<int>::iterator it = temp.begin(); it != temp.end(); ++it)
    {
        //id_t en_id = _sites[*it].first;
        if (_sites[*it].first == id) 
            return true;
    }

    return false;
}


// get a list of enclosing neighbors
/*
vector<id_t> &
VoronoiSF::
get_en (id_t id, int level)
{
    recompute();
    int idx = get_idx (id);
    
    std::set<int> idxlist;
    _Voronoi.getNeighborSet (idx, idxlist);
    
    _en_list.clear ();
    
    for (set<int>::iterator it = idxlist.begin(); it != idxlist.end(); ++it)
    {
        _en_list.push_back (_sites[(*it)].first);
    }

    //return list.size();
    return _en_list;
}
*/

/*
vector<id_t> &
VoronoiSF::
get_en (id_t id, int level)
{
    _en_list.clear ();
    
    int idx;
    
    std::set<int> idxlist;
    std::vector<Position> pos_list;
    std::vector<id_t> remove_list;

    int remove_count = 0;
    int j;

    while (true)
    {
        recompute();
        remove_list.clear ();
        idx = get_idx (id);
        _Voronoi.getNeighborSet (idx, idxlist);        
        
        for (set<int>::iterator it = idxlist.begin(); it != idxlist.end(); ++it)
        {
            _en_list.push_back (_sites[(*it)].first);
            pos_list.push_back (_sites[(*it)].second);
            remove_list.push_back (_sites[(*it)].first);
        }            
        
        // check to see if we're done
        if (--level <= 0)
            break;

        // remove the enclosing neighbors
        for (j=0; j<(int)remove_list.size (); j++)
        {
            remove (remove_list[j]);
            remove_count++;
        }
    }

    // add back the removed neighbors
    if (remove_count > 0)
    {
        for (j=0; j<(int)_en_list.size (); j++)
            insert (_en_list[j], pos_list[j]);
    }
    
    //return list.size();
    return _en_list;
}
*/

// a more efficient version (avoids Voronoi rebuilt unless sites are removed by necessity)
vector<id_t> &
VoronoiSF::
get_en (id_t id, int level)
{
    _en_list.clear ();
        
    std::set<int> idxlist;
    std::vector<Position> pos_list;

    int remove_count = 0;
    int j;

    while (level > 0)
    {
        // remove enclosing neighbors already recorded, if any
        for (j = remove_count; j<(int)_en_list.size (); j++)
        {
            remove (_en_list[j]);
            remove_count++;
        }

        recompute();

        _Voronoi.getNeighborSet (get_idx (id), idxlist);        
        
        // get nearest neighbors given the current center
        for (set<int>::iterator it = idxlist.begin(); it != idxlist.end(); ++it)
        {
            _en_list.push_back (_sites[(*it)].first);
            pos_list.push_back (_sites[(*it)].second);
        }       

        level--;
    }

    // add back the removed neighbors (when query beyond 1st-level neighbors are made)
    for (j=0; j < remove_count; j++)
        insert (_en_list[j], pos_list[j]);
    
    return _en_list;
}


// check if a circle overlaps with a particular region
bool 
VoronoiSF::
overlaps (id_t id, const Position &pt, length_t radius, bool accurate_mode)
{    
    int idx = get_idx (id);
    if (idx == -1) 
        return false;

    if (accurate_mode)
    {
        // version 1: accurate but slower
        recompute();
        point2d center (pt.x, pt.y); 
        return _Voronoi.collides (idx, center, (int)(radius+5));
    }
    else
    {
        // version 2: simply check if the center of region is within AOI
        Position &pt2 = _sites[idx].second;    
        return (pt2.distance (pt) <= (double)(radius) ? true : false);
    }
}


// remove all sites in the diagram
void 
VoronoiSF::
clear ()
{
    int n = _sites.size ();
    for (int i = 0; i < n; i ++)
    {
        remove (_sites[_sites.size () - 1].first);
    }
}


// returns the closest node to a point
// Added 20080724 by Csc
//  an extra case added for ensuring a unique closest point is returned
//      by checking IDs of them 
//  (i.e. when two or more sites is closest to the point, smallest ID one is returned)
id_t
VoronoiSF::
closest_to (const Position &pt)
{
    int n = _sites.size ();

    // error checking
    if (n == 0)
        return 0;

    id_t closest = _sites[0].first;
    double min = pt.distance (_sites[0].second);
    double d;

    id_t id;   
    for (int i = 0; i<n; i++)
    {
        id          = _sites[i].first;
        Position &p = _sites[i].second;

        // NOTE: it's important if distance is equal or very close, then 
        //       there's a second way to determine ordering (i.e., by ID)
        //       otherwise the query may be thrown in circles

        //ACE_DEBUG ((LM_DEBUG, "(%5t) closest_to() checking idx:%d id:%d\n", i, id));
        d = pt.distance (p);

        if (d < min || ((d-min < EQUAL_DISTANCE) && id < closest))
        {
            min = d;
            closest = id;
        }
    }
    
    return closest;
}

/*
sfVoronoi &
VoronoiSF::
get_Voronoi ()
{
    recompute ();
    return _Voronoi;
}
*/

void 
VoronoiSF::
recompute()
{
    if (invalidated == false)
        return;

    _Voronoi.clearAll();
    int n = _sites.size ();
    for (int i = 0; i < n; i++)
    {
        point2d pt (_sites[i].second.x, _sites[i].second.y);
        _Voronoi.mSites.push_back (pt);
    }
    _Voronoi.calsfv();
    invalidated = false;
}

int 
VoronoiSF::
get_idx(id_t h)
{        
    int n = _sites.size ();
    for (int i = 0; i < n; i++)
    {
        if (_sites[i].first == h)
            return i;
    }    
    return -1;
}


vector<line2d> & 
VoronoiSF::getedges()
{
    recompute ();
    return _Voronoi.mLines;
}


// obtain the bounding box for this Voronoi object
// returns true if the box exists, false if one of the dimensions is empty
bool 
VoronoiSF::get_bounding_box (point2d& min, point2d& max)
{
    return _Voronoi.getBoundingBox (min, max);
}

// get edges of sites with ID = id
std::set<int> &
VoronoiSF::get_site_edges (int id)
{
    static std::set<int> empty_set;

    int idx = get_idx (id);
    if (idx == -1) 
        return empty_set;
    
    recompute();
    return _Voronoi.mSites[idx].mEdgeIndexSet;
}


} // namespace Vast


