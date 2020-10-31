/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2006 Shun-Yun Hu (syhu@yahoo.com)
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
 * Voronoi.h -- Voronoi-related functions (virtual class)
 *   
 *  
 */

#ifndef VAST_Voronoi_H
#define VAST_Voronoi_H

#include "VASTTypes.h"
#include <vector>
#include <set>


namespace Vast {

    class EXPORT Voronoi
    {
    public:
        Voronoi () {}
        virtual ~Voronoi () {}

        // insert a new site, the first inserted is myself
        virtual void insert (id_t id, const Position &coord) = 0;
        
        // remove a site
        virtual void remove (id_t id) = 0;
        
        // modify the coordinates of a site
        virtual void update (id_t id, const Position &coord) = 0;
        
        // get the point of a site
        virtual Position get (id_t id) = 0;
        
        // check if a point lies inside a particular region
        virtual bool contains (id_t id, const Position &coord) = 0;
        
        // check if the node is a boundary neighbor
        virtual bool is_boundary (id_t id, const Position &center, length_t radius) = 0;
        
        // check if the node is an enclosing neighbor
        virtual bool is_enclosing (id_t id, id_t center_node_id = ((id_t)-1)) = 0;
        
        // get a list of enclosing neighbors
        //virtual int get_en (id_t id, std::vector<id_t> &list) = 0;
        virtual std::vector<id_t> & get_en (id_t id, int level = 1) = 0;
        
        // check if a circle overlaps with a particular region
        virtual bool overlaps (id_t id, const Position &center, length_t radius, bool accurate_mode = false) = 0;
        
        // remove all sites in the diagram
        virtual void clear () = 0;

        //
        // non Voronoi-specific methods
        //
        
        // returns the closest node to a point
        virtual id_t closest_to (const Position &pt) = 0;
        
        virtual std::vector<line2d> &getedges() = 0;

        // obtain the bounding box for this Voronoi object
        // returns true if the box exists, false if one of the dimensions is empty
        virtual bool get_bounding_box (point2d& min, point2d& max) = 0;        

        // expose for displaying partitioning scheme
        // sfVoronoi &get_Voronoi ();
        
        /*
        vector<site_t> &get_sites ()
        {
            return _sites;
        }
        */

        // get the number of sites currently maintained
        virtual int size () = 0;

        // get edges of sites with ID = id
        virtual std::set<int> & get_site_edges (int id) = 0;
    };   

} // end namespace Vast

#endif // VAST_Voronoi_H
