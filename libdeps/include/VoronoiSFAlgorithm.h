/*
 * The author of this software is Steven Fortune. 
 * Copyright (c) 1994 by AT&T Bell Laboratories.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHORS NOR AT&T MAKE ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 */

/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2004 Guan-Ming Liao (gm.liao@msa.hinet.net) 
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


#ifndef VAST_VORONOI_SF_ALGORITHM_H
#define VAST_VORONOI_SF_ALGORITHM_H

// disable warning about "identifier exceeds'255' characters"
#ifdef _WINDOWS
#pragma warning(disable: 4786)
#endif

#define NODESIZE_INCREMENT  50      // how much space should be allocated if not enough at a time
#define MEMSIZE_INCREMENT   20000   // how much memory to allocate when not enough

#include <iostream>
#include <vector>
#include <set>
#include <math.h>
#include "VASTTypes.h"

namespace Vast {

//////////////////////////////////////////////////////////////////////////

class Freenode	
{
public:	
    Freenode *nextfree;
};

class Freelist	
{
public:
	Freenode	*head;
	int			nodesize;
};

class Point	
{
public:
	double x,y;
};

/* structure used both for sites and for vertices */
class Site	
{	
public:
	Point	coord;
	int		sitenbr;
	int		refcnt;
};

class Edge	
{
public:
	double	a,b,c;
	Site 	*ep[2];
	Site	*reg[2];
	int		edgenbr;
};

class Halfedge 
{
public:
	Halfedge    *ELleft, *ELright;
	Edge	    *ELedge;
	int		    ELrefcnt;
	char		ELpm;
	Site	    *vertex;
	double		ystar;
	Halfedge    *PQnext;
};

class SiteEx : public point2d
{
public:
	SiteEx (point2d& p)
	{
		this->x = p.x;
		this->y = p.y;
	}
	std::set<int> mEdgeIndexSet;	
};

class sfVoronoi
{
public:
	sfVoronoi()
		: sites (NULL), 
          sfvBuffer (NULL), nodesize_alloc (0), 
          mem_alloc (0), mem_free (0), mem_pos (0), le (0), re (1)
	{        
        DELETED = (Edge *)(-2);
	}

	~sfVoronoi()
	{
        // free sfvBuffer
        for (size_t i=0; i < buf_list.size (); i++)
            delete[] buf_list[i];     
        buf_list.clear ();
        
        
        if (sites != NULL)
        {
            delete[] sites;
            sites = NULL;
        }
        
        //sites.clear ();

        mSites.clear ();
        mLines.clear ();
        mVertex.clear ();
        mDebug.clear ();
	}
	
	//main.c
    std::vector<SiteEx>     mSites;
    std::vector<line2d>     mLines;
    std::vector<point2d>    mVertex;
    std::vector<point2d>    mDebug;
        
    void calsfv();
    void clearAll();
    //void recalculate();

    void getNeighborSet (int index , std::set<int>& nset);
    bool isPointInsideSiteRegion (int index, const point2d& p);
    //bool twoSitesAreNeighbor (int idx1 , int idx2 , int& bisectingLineIndex);
    bool isBoundarySite (int index);
    bool collides (int index, const point2d &center, int radius);
    
    // whether a polygon is fully enclosed within a circle
    bool enclosed (int index, point2d& center, int radius);

    // obtain the bounding box for this Voronoi object
    // returns true if the box exists, false if one of the dimensions is empty
    bool getBoundingBox (point2d& min, point2d& max)
    {
        min.x = xmin;
        min.y = ymin;
        max.x = xmax;
        max.y = ymax;

        return !(xmin == xmax || ymin == ymax);
    }

protected:
	
	Site* nextone()
	{		
		if(siteidx < nsites)
		    return &sites[siteidx++];		
		else	
		    return NULL;
	}

	void readsites();

	//defs.h
	//////////////////////////////////////////////////////////////////////////	
	int triangulate, sorted, plot, debug;
	double xmin, xmax, ymin, ymax, deltax, deltay;
	
    Site	*sites;
    //std::vector<Site> sites;
	int		nsites;
	int		siteidx;
	int		sqrt_nsites;
	int		nvertices;	
    Freelist sfl;
	Site	*bottomsite;
	
    int nedges;	
    Freelist efl;
	Halfedge *PQhash;
	int PQhashsize;
	int PQcount;
	int PQmin;
	
    Freelist hfl;
	Halfedge *ELleftend, *ELrightend;
	int 	ELhashsize;
	Halfedge **ELhash;
	//int DELETED ;
    Edge*    DELETED;

	//////////////////////////////////////////////////////////////////////////
	//Geometry.h

	void geominit ();
	Edge *bisect (Site *s1,Site *s2);
	Site *intersect (Halfedge *el1, Halfedge *el2);
	int right_of (Halfedge *el , Point *p );
	void endpoint (Edge *e , int	lr , Site *s);
	double dist (Site *s,Site *t);
	void makevertex ( Site *v );// return int change to void
	void deref (Site *v);
	void ref (Site *v);
	//////////////////////////////////////////////////////////////////////////
	//memory.c


	void freeinit (Freelist *fl , int size);
	char* getfree (Freelist *fl);
	void makefree (Freenode *curr, Freelist *fl);
	char* myalloc(unsigned n);

	//////////////////////////////////////////////////////////////////////////
	//output.c
	double pxmin, pxmax, pymin, pymax, cradius;
	void out_bisector ( Edge *e ) ;
	void out_ep ( Edge *e ) ;
	void out_vertex ( Site *v );
	
    //void out_site ( Site *s );
	//void out_triple(Site *s1,Site *s2, Site *s3);

	void plotinit();
	void clip_line (Edge * e);

	//////////////////////////////////////////////////////////////////////////
	//heap.c
	void PQinsert(Halfedge *he , Site *v , double offset);
	void PQdelete(Halfedge *he);
	int PQbucket(Halfedge *he);
	int PQempty();
	Point PQ_min();
	Halfedge *PQextractmin();
	void PQinitialize();
	
	//////////////////////////////////////////////////////////////////////////
	//edgelist.c
	//int ntry, totalsearch;
	void ELinitialize ();
	Halfedge* HEcreate (Edge *e, int pm);
	void ELinsert (Halfedge *lb, Halfedge* newH); //change arg2 to newH
	Halfedge* ELgethash (int b);
	Halfedge* ELleftbnd (Point* p);
	void ELdelete (Halfedge *he) ;
	Halfedge* ELright (Halfedge *he);
	Halfedge* ELleft (Halfedge *he);
	Site* leftreg (Halfedge* he);
	Site* rightreg (Halfedge* he);

	//////////////////////////////////////////////////////////////////////////
	//Voronoi.c
	void Voronoi (int triangulate);

	// dynamically allocated buffers
	char* sfvBuffer;
    std::vector<char *> buf_list;
    
    int   nodesize_alloc;
    int   mem_alloc;
    int   mem_free;
    int   mem_pos;

private:
	int le;
	int re;
};

} // end namespace Vast

#endif // VAST_VORONOI_SF_ALGORITHM_H

