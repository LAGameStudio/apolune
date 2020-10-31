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

#include "VoronoiSFAlgorithm.h"
#include <math.h>
#include <time.h>
#include <iostream>
using namespace std;


namespace Vast {

//////////////////////////////////////////////////////////////////////////
//Geometry.h
void 
sfVoronoi::
geominit ()
{
    //Edge e;
    freeinit (&efl, sizeof (Edge));
    nvertices = 0;
    nedges = 0;
    double sn = (double)(nsites+4);
    sqrt_nsites = (int)sqrt(sn);
    deltay = ymax - ymin;
    deltax = xmax - xmin;
}

Edge *
sfVoronoi::
bisect (Site *s1,Site *s2)
{
    //printf ("bisecting (%d, %d) (%d, %d)\n", (int)s1->coord.x, (int)s1->coord.y, (int)s2->coord.x, (int)s2->coord.y);
    double dx,dy,adx,ady;
    Edge *newedge;

    newedge = (Edge *) getfree(&efl);

    newedge->reg[0] = s1;
    newedge->reg[1] = s2;
    ref(s1); 
    ref(s2);
    newedge->ep[0] = (Site *) NULL;
    newedge->ep[1] = (Site *) NULL;

    dx = s2->coord.x - s1->coord.x;
    dy = s2->coord.y - s1->coord.y;
    adx = dx>0 ? dx : -dx;
    ady = dy>0 ? dy : -dy;
    newedge->c = (double)(s1->coord.x * dx + s1->coord.y * dy + (dx*dx + dy*dy)*0.5);
    if (adx>ady)
    {   newedge->a = 1.0; newedge->b = dy/dx; newedge->c /= dx;}
    else
    {   newedge->b = 1.0; newedge->a = dx/dy; newedge->c /= dy;};

    newedge->edgenbr = nedges;
    out_bisector(newedge);
    nedges += 1;
    return(newedge);
}

Site* 
sfVoronoi::
intersect (Halfedge *el1, Halfedge *el2)
{
    Edge *e1,*e2, *e;
    Halfedge *el;
    double d, xint, yint;
    int right_of_site;
    Site *v;

    e1 = el1->ELedge;
    e2 = el2->ELedge;

    if (e1 == NULL || e2 == NULL)
        return NULL;

    if (e1->reg[1] == e2->reg[1]) 
        return NULL;

    d = e1->a * e2->b - e1->b * e2->a;
    if (-1.0e-10 < d && d < 1.0e-10) 
        return NULL;

    xint = (e1->c*e2->b - e2->c*e1->b)/d;
    yint = (e2->c*e1->a - e1->c*e2->a)/d;

    if ((e1->reg[1]->coord.y < e2->reg[1]->coord.y) ||
        (e1->reg[1]->coord.y == e2->reg[1]->coord.y &&
        e1->reg[1]->coord.x < e2->reg[1]->coord.x))
    {   
        el = el1; 
        e = e1;
    }
    else
    {   
        el = el2; 
        e = e2;
    }

    right_of_site = xint >= e->reg[1]->coord.x;

    if ((right_of_site && el->ELpm == le) ||
       (!right_of_site && el->ELpm == re)) 
       return NULL;

    v = (Site *)getfree (&sfl);
    v->refcnt = 0;
    v->coord.x = xint;
    v->coord.y = yint;

    return v;
}

int 
sfVoronoi::
right_of (Halfedge *el, Point *p)
{
    Edge *e;
    Site *topsite;
    int right_of_site, above, fast;
    double dxp, dyp, dxs, t1, t2, t3, yl;

    e = el->ELedge;
    topsite = e->reg[1];
    right_of_site = p->x > topsite->coord.x;
    if(right_of_site && el->ELpm == le) return(1);
    if(!right_of_site && el->ELpm == re) return (0);

    if (e->a == 1.0)
    {   
        dyp = p->y - topsite->coord.y;
        dxp = p->x - topsite->coord.x;
        fast = 0;

        if ((!right_of_site & (e->b<0.0)) | (right_of_site & (e->b>=0.0)))
            fast = above = (dyp >= e->b*dxp);
        else 
        {   
            above = p->x + p->y*e->b > e-> c;
            if(e->b<0.0) above = !above;
            if (!above) fast = 1;
        }
        if (!fast)
        {   
            dxs = topsite->coord.x - (e->reg[0])->coord.x;
            
            // joker: update, skip divide by zero 05.05.27
            // TODO: need to further check what cases could cause divide by 0
            if (dxs != 0)
                above = e->b * (dxp*dxp - dyp*dyp) < dxs*dyp*(1.0+2.0*dxp/dxs + e->b*e->b);
            else
                above = 0;

            if (e->b<0.0) 
                above = !above;
        };
    }
    else  /*e->b==1.0 */
    {   
        yl = e->c - e->a*p->x;
        t1 = p->y - yl;
        t2 = p->x - topsite->coord.x;
        t3 = yl - topsite->coord.y;
        above = t1*t1 > (t2*t2 + t3*t3);
    };
    return (el->ELpm==le ? above : !above);
}

void 
sfVoronoi::
endpoint (Edge *e , int lr , Site *s)
{
    e->ep[lr] = s;
    ref(s);
    if(e->ep[re-lr]== NULL) return;
    out_ep(e);
    deref(e->reg[le]);
    deref(e->reg[re]);
    //makefree(e, &efl);
}

double 
sfVoronoi::
dist (Site *s, Site *t)
{
    double dx,dy;
    
    dx = s->coord.x - t->coord.x;
    dy = s->coord.y - t->coord.y;
    return (sqrt(dx*dx + dy*dy));
}

void 
sfVoronoi::
makevertex( Site *v )
{
    v->sitenbr = nvertices++;
    out_vertex(v);
}

void 
sfVoronoi::
deref(Site *v)
{
    v->refcnt--;
    //if (v->refcnt == 0 ) makefree(v, &sfl);
}

void 
sfVoronoi::
ref(Site *v)
{
    v->refcnt++;  
}
//////////////////////////////////////////////////////////////////////////
//Memroy.h

void sfVoronoi::freeinit (Freelist *fl, int size)
{
    fl->head = (Freenode *)NULL;
    fl->nodesize = size;
}

char* sfVoronoi::getfree (Freelist *fl)
{
    int i; 
    Freenode *t;

    // one-time allocation of memory linklist
    if (fl->head == (Freenode *) NULL)
    {   
        t = (Freenode *)myalloc (sqrt_nsites * fl->nodesize);

        for (i=0; i<sqrt_nsites; i++)   
            makefree ((Freenode *)((char *)t + i * fl->nodesize), fl);
    };

    // we return the first available piece of memory, and shift the linklist
    t = fl->head;
    fl->head = (fl->head)->nextfree;
    return (char *)t;
}

// link up a free memory block to the head of a linklist
void sfVoronoi::makefree (Freenode *curr, Freelist *fl)
{
    curr->nextfree = fl->head;
    fl->head = curr;
}

//static char* sfvBuffer = NULL;

// BUG: memory allocation is one-time only, could run out of memory
// BUG: right now no release is made, but curr_alloc is reset in calsfv()
//      means that every time Voronoi is recomputed memory alloc is resumed from
//      beginning (thus no need to worry about releasing memory?)
char* sfVoronoi::myalloc (unsigned n)
{   
    // if memory not enough
    if (mem_free < (int)n)
    {
        int size = (n/MEMSIZE_INCREMENT + 1) * MEMSIZE_INCREMENT;        
                    
        //sfvBuffer = new char[size];
        
        buf_list.push_back (new char[size]);
        sfvBuffer = buf_list[buf_list.size ()-1];

        //if ((sfvBuffer = (char *)malloc (size)) == NULL)
        //if ((sfvBuffer = new char[size]) == NULL)
        if (sfvBuffer == NULL)
        {
            printf ("sfVoronoi::myalloc (): memory allocation error\n");
            return NULL;
        }        
        
        mem_alloc += size;
        mem_free = size;
        mem_pos = 0;

        // put to list for later dellocation
        //buf_list.push_back (sfvBuffer);
    }

    char *t = &(sfvBuffer[mem_pos]);
    mem_free -= n;
    mem_pos  += n;    
    return t;
}

//////////////////////////////////////////////////////////////////////////
//output.c
void sfVoronoi::out_bisector (Edge *e) 
{
    //printf ("out_bisector [%f, %f, %f]\n", e->a, e->b, e->c);

  if (triangulate & plot & !debug)
  {
    //line(e->reg[0]->coord.x, e->reg[0]->coord.y,e->reg[1]->coord.x, e->reg[1]->coord.y);

  }
  if (!triangulate & !plot &!debug)
  {
    //printf("l %f %f %f\n", e->a, e->b, e->c);
  }
  if (debug)
  {
    //printf("line(%d) %gx+%gy=%g, bisecting %d %d\n", e->edgenbr,e->a, e->b, e->c, e->reg[le]->sitenbr, e->reg[re]->sitenbr);
  }

  //////////////////////////////////////////////////////////////////////////  
  line2d temp (e->a,e->b,e->c);
  temp.bisecting[0] = e->reg[le]->sitenbr ;
  temp.bisecting[1] = e->reg[re]->sitenbr ;
  mSites[ temp.bisecting[0] ].mEdgeIndexSet.insert (e->edgenbr);
  mSites[ temp.bisecting[1] ].mEdgeIndexSet.insert (e->edgenbr);

  //temp.vertexIndex[0] = (e->ep[le] != ( Site *)NULL )?(e->ep[le]->sitenbr):(-1) ;
  //temp.vertexIndex[1] = (e->ep[re] != ( Site *)NULL )?(e->ep[re]->sitenbr):(-1) ;

  //cout<<temp.vertexIndex[0]<<":"<<temp.vertexIndex[1]<<endl;
  mLines.push_back( temp );
}

void sfVoronoi::out_ep ( Edge *e ) 
{
  mLines[e->edgenbr].vertexIndex[0] = (e->ep[le] != ( Site *)NULL )?(e->ep[le]->sitenbr):(-1) ;
  mLines[e->edgenbr].vertexIndex[1] = (e->ep[re] != ( Site *)NULL )?(e->ep[re]->sitenbr):(-1) ;

  /*
  cout<<e->edgenbr<<":"<<e->reg[le]->sitenbr<<","
      <<e->reg[re]->sitenbr<<endl;
  */
  if (!triangulate & plot)
  { 
    clip_line (e);
  }

  if (!triangulate & !plot)
  {
    //printf("e %d", e->edgenbr);
    //printf(" %d ", e->ep[le] != ( Site *)NULL ? e->ep[le]->sitenbr : -1);
    //printf("%d\n", e->ep[re] != ( Site *)NULL ? e->ep[re]->sitenbr : -1);
  }
  //////////////////////////////////////////////////////////////////////////  

}

void sfVoronoi::out_vertex (Site *v)
{
    this->mVertex.push_back (point2d( v->coord.x,v->coord.y ));
}

/*
void sfVoronoi::out_triple (Site *s1,Site *s2, Site *s3)
{
  if (triangulate & !plot &!debug)
  {
    //printf("%d %d %d\n", s1->sitenbr, s2->sitenbr, s3->sitenbr);
  }

  if (debug)
  {
    //printf("circle through left=%d right=%d bottom=%d\n", s1->sitenbr, s2->sitenbr, s3->sitenbr);
  }
}
*/

void sfVoronoi::plotinit()
{
  double dx;
  double dy;
  double d;

  dy = ymax - ymin;
  dx = xmax - xmin;
  d = ( dx > dy ? dx : dy) * 1.1;

  pxmin = xmin - (d-dx)/2.0;
  pxmax = xmax + (d-dx)/2.0;
  pymin = ymin - (d-dy)/2.0;
  pymax = ymax + (d-dy)/2.0;

  cradius = (pxmax - pxmin)/350.0;
  //openpl();
  //range(pxmin, pymin, pxmax, pymax);
}

void sfVoronoi::clip_line (Edge * e)
{

 Site *s1, *s2;
 double x1,x2,y1,y2;

    if(e->a == 1.0 && e ->b >= 0.0)
    {   s1 = e->ep[1];
        s2 = e->ep[0];
    }
    else 
    {   s1 = e->ep[0];
        s2 = e->ep[1];
    };

    if(e->a == 1.0)
    {
        y1 = pymin;
        if (s1!=( Site *)NULL && s1->coord.y > pymin)
             y1 = s1->coord.y;
        if(y1>pymax) return;
        x1 = e->c - e->b * y1;
        y2 = pymax;
        if (s2!=( Site *)NULL && s2->coord.y < pymax) 
            y2 = s2->coord.y;
        if(y2<pymin) return;
        x2 = e->c - e->b * y2;
        if ( ( (x1> pxmax) & (x2>pxmax) ) | ( (x1 < pxmin) & (x2<pxmin) ) ) 
            return;
        if(x1> pxmax)
        {   x1 = pxmax; y1 = (e->c - x1)/e->b;};
        if(x1<pxmin)
        {   x1 = pxmin; y1 = (e->c - x1)/e->b;};
        if(x2>pxmax)
        {   x2 = pxmax; y2 = (e->c - x2)/e->b;};
        if(x2<pxmin)
        {   x2 = pxmin; y2 = (e->c - x2)/e->b;};
    }
    else
    {
        x1 = pxmin;
        if (s1!=( Site *)NULL && s1->coord.x > pxmin) 
            x1 = s1->coord.x;
        if(x1>pxmax) return;
        y1 = e->c - e->a * x1;
        x2 = pxmax;
        if (s2!=( Site *)NULL && s2->coord.x < pxmax) 
            x2 = s2->coord.x;
        if(x2<pxmin) return;
        y2 = e->c - e->a * x2;
        if (( (y1> pymax) & (y2>pymax)) | ( (y1<pymin) & (y2<pymin))) return;
        if(y1> pymax)
        {   y1 = pymax; x1 = (e->c - y1)/e->a;};
        if(y1<pymin)
        {   y1 = pymin; x1 = (e->c - y1)/e->a;};
        if(y2>pymax)
        {   y2 = pymax; x2 = (e->c - y2)/e->a;};
        if(y2<pymin)
        {   y2 = pymin; x2 = (e->c - y2)/e->a;};
    };
    
    //line(x1,y1,x2,y2);
    mLines[ e->edgenbr ].seg.p1 = point2d(x1,y1) ;
    mLines[ e->edgenbr ].seg.p2 = point2d(x2,y2) ;

    //printf ("[%f, %f, %f]\n", e->a, e->b, e->c);
    //printf ("(%f, %f) (%f, %f)\n", x1, y1, x2, y2);
    //mSegments.push_back( segment(point2d(x1,y1),point2d(x2,y2)  ) );

    return;
}

//////////////////////////////////////////////////////////////////////////
//heap.c
void sfVoronoi::PQinsert (Halfedge *he, Site *v, double offset)
{
    Halfedge *last, *next;

    he->vertex = v;
    ref(v);
    he->ystar = v->coord.y + offset;
    last = &PQhash[PQbucket(he)];
    while ((next = last->PQnext) != (Halfedge *) NULL &&
          (he->ystar  > next->ystar  ||
        (he->ystar == next->ystar && v->coord.x > next->vertex->coord.x)))
        {   last = next;};
    he->PQnext = last->PQnext; 
    last->PQnext = he;
    PQcount++;
}

void sfVoronoi::PQdelete (Halfedge *he)
{
    Halfedge *last;

    if (he->vertex != (Site *)NULL)
    {   
        last = &PQhash[PQbucket(he)];
        while (last->PQnext != he)
            last = last->PQnext;
        
        last->PQnext = he->PQnext;
        PQcount--;
        deref(he->vertex);
        he->vertex = (Site *) NULL;
    }
}

int sfVoronoi::PQbucket (Halfedge *he)
{
    int bucket;

    bucket = (int)((he->ystar - ymin)/deltay * PQhashsize);
    if (bucket<0) bucket = 0;
    if (bucket>=PQhashsize) bucket = PQhashsize-1;
    if (bucket < PQmin) PQmin = bucket;
    return (bucket);
}

int sfVoronoi::PQempty ()
{
    return (PQcount==0);
}

Point sfVoronoi::PQ_min ()
{
    Point answer;

    while (PQhash[PQmin].PQnext == NULL)     
        PQmin++;

    answer.x = PQhash[PQmin].PQnext->vertex->coord.x;
    answer.y = PQhash[PQmin].PQnext->ystar;

    return (answer);
}

Halfedge* sfVoronoi::PQextractmin()
{
    Halfedge *curr;

    curr = PQhash[PQmin].PQnext;
    PQhash[PQmin].PQnext = curr->PQnext;
    PQcount -= 1;
    return(curr);
}

void sfVoronoi::PQinitialize()
{
    int i; //Point *s;

    PQcount = 0;
    PQmin = 0;
    PQhashsize = 4 * sqrt_nsites;
    PQhash = (Halfedge *) myalloc(PQhashsize * sizeof (Halfedge));

    for(i=0; i<PQhashsize; i++) 
        PQhash[i].PQnext = (Halfedge *)NULL;
}

//////////////////////////////////////////////////////////////////////////
//edgelist.c

void sfVoronoi::ELinitialize()
{
    int i;
    freeinit (&hfl, sizeof (Halfedge));
    ELhashsize = 2 * sqrt_nsites;
    ELhash = (Halfedge **)myalloc (ELhashsize * sizeof (Halfedge *));

    for(i=0; i<ELhashsize; i++) 
        ELhash[i] = (Halfedge *)NULL;
    
    ELleftend = HEcreate ((Edge *)NULL, 0);
    ELrightend = HEcreate ((Edge *)NULL, 0);
    ELleftend->ELleft = (Halfedge *)NULL;
    ELleftend->ELright = ELrightend;
    ELrightend->ELleft = ELleftend;
    ELrightend->ELright = ( Halfedge *)NULL;
    ELhash[0] = ELleftend;
    ELhash[ELhashsize-1] = ELrightend;
}

Halfedge* sfVoronoi::HEcreate (Edge *e, int pm)
{
    Halfedge *he;
    
    he = (Halfedge *) getfree (&hfl);
    he->ELedge = e;
    he->ELpm = (char)pm;
    he->PQnext = (Halfedge *)NULL;
    he->vertex = (Site *)NULL;
    he->ELrefcnt = 0;
    he->ystar = 0;
    
    return he;
}

void sfVoronoi::ELinsert( Halfedge *lb ,Halfedge* newH )
{
    newH->ELleft = lb;
    newH->ELright = lb->ELright;
    (lb->ELright)->ELleft = newH;
    lb->ELright = newH;
}

Halfedge* sfVoronoi::ELgethash( int b )
{
    Halfedge *he;

    if(b<0 || b>=ELhashsize) return(( Halfedge *) NULL);
    he = ELhash[b]; 
    if (he == ( Halfedge *) NULL || 
        he->ELedge != DELETED ) return (he);

    /* Hash table points to deleted half edge.  Patch as necessary. */
    ELhash[b] = ( Halfedge *) NULL;

    //if ((he->ELrefcnt -= 1) == 0)   makefree(he, &hfl);
    return (( Halfedge *) NULL);
}

Halfedge* sfVoronoi::ELleftbnd (Point* p)
{
    //printf ("ELleftbnd process (%d, %d)\n", (int)p->x, (int)p->y);
    int bucket;
    Halfedge *he;

    /* Use hash table to get close to desired halfedge */
    bucket = (int)((p->x - xmin)/deltax * ELhashsize);
    if (bucket<0)
        bucket = 0;
    if (bucket >= ELhashsize)
        bucket = ELhashsize - 1;
    he = ELgethash (bucket);

    if (he == (Halfedge *) NULL)
    {   
        //printf ("ELleftbnd: first he is null\n");
        int i=1;
        /*
        while (true)
        {
            if ((he=ELgethash (bucket-i)) != (Halfedge *) NULL) 
                break;
            if ((he=ELgethash (bucket+i)) != (Halfedge *) NULL) 
                break;
            i++;
        };
        */

        while ((he = ELgethash (bucket-i)) == NULL && (he=ELgethash (bucket+i)) == NULL)
            i++;

        //totalsearch += i;
    };
    //printf ("ELleftbnd: bucket %d hsize: %d - elpm: %d ref_count: %d ystar: %f\n", bucket, ELhashsize, he->ELpm, he->ELrefcnt, he->ystar);

    //ntry++;
    
    /* Now search linear list of halfedges for the corect one */
    if (he==ELleftend || (he != ELrightend && right_of(he,p)))
    {
        //printf ("ELleftbnd: loop 1\n");
        do {he = he->ELright;} while (he!=ELrightend && right_of(he,p));
        he = he->ELleft;
    }
    else 
        do {he = he->ELleft;} while (he!=ELleftend && !right_of(he,p));

    /* Update hash table and reference counts */
    if(bucket > 0 && bucket <ELhashsize-1)
    {   
        if(ELhash[bucket] != (Halfedge *) NULL) 
            ELhash[bucket]->ELrefcnt -= 1;
        ELhash[bucket] = he;
        ELhash[bucket]->ELrefcnt += 1;
    };
    //printf ("ELleftbnd: elpm: %d ref_count: %d ystar: %f\n", he->ELpm, he->ELrefcnt, he->ystar);

    return (he);
}

void sfVoronoi::ELdelete (Halfedge *he) 
{
    (he->ELleft)->ELright = he->ELright;
    (he->ELright)->ELleft = he->ELleft;
    //he->ELedge = ( Edge *)DELETED;
    he->ELedge = DELETED;
}

Halfedge* sfVoronoi::ELright( Halfedge *he )
{
    return (he->ELright);
}

Halfedge* sfVoronoi::ELleft( Halfedge *he )
{
    return (he->ELleft);
}

Site* sfVoronoi::leftreg (Halfedge* he) 
{
    if(he->ELedge == (Edge *)NULL) return(bottomsite);
    return( he->ELpm == le ? 
        he->ELedge->reg[le] : he->ELedge->reg[re]);
}

Site* sfVoronoi::rightreg (Halfedge* he) 
{    
    if (he->ELedge == (Edge *)NULL) {
        //printf ("rightreg..returning bottomsite\n");
        return (bottomsite);
    }
    //printf ("rightreg..returning other...\n");
    return (he->ELpm == le ? he->ELedge->reg[re] : he->ELedge->reg[le]);
}

////////////////////////////////////////


//////////////////////////////////
//Voronoi.c
void sfVoronoi::Voronoi (int triangulate)
{
    // unused   
    triangulate = triangulate;

    Site *newsite, *bot, *top, *temp, *p;
    Site *v;
    Point newintstar;
    newintstar.x = 0;
    newintstar.y = 0;

    int pm;
    Halfedge *lbnd, *rbnd, *llbnd, *rrbnd, *bisector;
    Edge *e;

    PQinitialize ();
    bottomsite = nextone();
    //out_site(bottomsite);
    ELinitialize ();

    newsite = nextone ();
    bool done = false;

    while (!done)
    {
        if (!PQempty ()) 
            newintstar = PQ_min ();

        if (newsite != NULL && 
            (PQempty () || 
             newsite->coord.y < newintstar.y || 
             (newsite->coord.y == newintstar.y && newsite->coord.x < newintstar.x)))
        {/* new site is smallest */
            //out_site(newsite);
            //printf ("new site is smallest\n");
            lbnd = ELleftbnd (&(newsite->coord));
            rbnd = ELright (lbnd);
            bot = rightreg (lbnd);
            e = bisect (bot, newsite);
            bisector = HEcreate (e, le);
            ELinsert (lbnd, bisector);
            if ((p = intersect (lbnd, bisector)) != (Site *) NULL) 
            {   
                PQdelete(lbnd);
                PQinsert(lbnd, p, dist(p,newsite));
            };
            lbnd = bisector;
            bisector = HEcreate (e, re);
            ELinsert (lbnd, bisector);
            if ((p = intersect (bisector, rbnd)) != (Site *) NULL)
            {   
                PQinsert(bisector, p, dist(p,newsite)); 
            };
            newsite = nextone();    
        }
        else if (!PQempty ()) 
        /* intersection is smallest */
        {   
            lbnd = PQextractmin ();
            llbnd = ELleft (lbnd);
            rbnd = ELright (lbnd);
            rrbnd = ELright (rbnd);
            bot = leftreg (lbnd);
            top = rightreg (rbnd);
            //out_triple (bot, top, rightreg (lbnd));

            v = lbnd->vertex;
            makevertex (v);
            endpoint (lbnd->ELedge,lbnd->ELpm,v);
            endpoint (rbnd->ELedge,rbnd->ELpm,v);
            ELdelete (lbnd); 
            PQdelete (rbnd);
            ELdelete (rbnd); 
            pm = le;

            if (bot->coord.y > top->coord.y)
            {   
                temp = bot; 
                bot = top; 
                top = temp; 
                pm = re;
            }

            e = bisect(bot, top);
            bisector = HEcreate (e, pm);
            ELinsert(llbnd, bisector);
            endpoint(e, re-pm, v);
            deref(v);

            if((p = intersect (llbnd, bisector)) != NULL)
            {   
                PQdelete(llbnd);
                PQinsert(llbnd, p, dist(p,bot));
            };
            if ((p = intersect (bisector, rrbnd)) != NULL)
            {   
                PQinsert(bisector, p, dist(p,bot));
            };
        }
        else 
            done = true;
    };

    for (lbnd = ELright(ELleftend); lbnd != ELrightend; lbnd = ELright(lbnd))
    {   
        e = lbnd->ELedge;
        out_ep (e);
    };
}
//////////////////////////////////////////////////////////////////////////
//main.c
void sfVoronoi::clearAll()
{
    this->mLines.clear();
    this->mSites.clear();
    this->mVertex.clear();
    //this->mSegments.clear();
    
    // TODO: should clear up linklist allocation as well

}

/* not used
void sfVoronoi::recalculate()
{
    this->mLines.clear();
    this->mVertex.clear();
    //this->mSegments.clear();
    
    for( int i = 0 ; i< (int)mSites.size() ; i++ )
        this->mSites[i].mEdgeIndexSet.clear();

    calsfv();
}
*/

void sfVoronoi::calsfv()
{
    // combine all currently allocated memory into one continous piece
    if (buf_list.size () > 1)
    {        
        // free all memory
        for (size_t i=0; i < buf_list.size (); i++)
            //free (buf_list[i]);
            delete[] buf_list[i];

        buf_list.clear ();
        
        //if ((sfvBuffer = (char *)malloc (mem_alloc)) == NULL)
        if ((sfvBuffer = new char[mem_alloc]) == NULL)
        {
            printf ("sfVoronoi::calsfv (): memory allocation error\n");
            return;
        }
        buf_list.push_back (sfvBuffer);
    }

    mem_free = mem_alloc;
    mem_pos = 0 ;

    sorted = 0; triangulate = 0; plot = 1; debug = 1;
    freeinit (&sfl, sizeof (Site));
    readsites();
    siteidx = 0;
    geominit();
    if (plot) 
        plotinit();

    Voronoi (triangulate); 
}

int scomp(const void* ss1,const void *ss2)
{
    Point* s1 = (Point*)ss1;
    Point* s2 = (Point*)ss2;
    if(s1->y < s2->y) return(-1);
    if(s1->y > s2->y) return(1);
    if(s1->x < s2->x) return(-1);
    if(s1->x > s2->x) return(1);
    return(0);
}

// obtain site info (converting from 'mSites' into 'sites')
void sfVoronoi::readsites ()
{   
    int n = (int)mSites.size();
      
    // check to see if we need to increase space
    if (nodesize_alloc < n)
    {
        int n_blocks = (n/NODESIZE_INCREMENT + 1)*NODESIZE_INCREMENT;
                
        
        if ((sites = (Site *)realloc (sites, n_blocks * sizeof (Site))) == NULL)
        {
            printf ("sfVoronoi::readsites (): memory allocation error\n");
            return;
        }
        

        /*
        if (sites != NULL)
            delete[] sites;

        if ((sites = (Site *)new char[n_blocks * sizeof (Site)]) == NULL)
        {
            printf ("sfVoronoi::readsites (): memory allocation error\n");
            return;
        }
        */
        
        nodesize_alloc = n_blocks;
    }
   
    
    /*
    // push placeholder Site into sites
    int j = n - (int)sites.size ();
    if (j > 0)
    {
        Site s;
        for (; j > 0; j--)
            sites.push_back (s);
    }
    */
    
    // store info in mSites to sites
    xmin = xmax = mSites[0].x; 
    
    int i;
    for(i=0; i < n; i++)
    {
        sites[i].sitenbr = i;
        sites[i].refcnt = 0;
        
        sites[i].coord.x = mSites[i].x ;
        sites[i].coord.y = mSites[i].y ;

        if (sites[i].coord.x < xmin) 
            xmin = sites[i].coord.x;
        if (sites[i].coord.x > xmax) 
            xmax = sites[i].coord.x;        
    }
    nsites = i;
    
    // order all sites according to coordinate comparisons
    qsort (sites, nsites, sizeof (Site), &scomp);
    
    // set upper & lower bound for y-values (sites is already sorted by y)
    ymin = sites[0].coord.y;
    ymax = sites[nsites-1].coord.y;
}


bool sfVoronoi::collides (int index, const point2d &center, int radius)
{
    //printf("\nchecking if idx: %d collides with (%.2f, %.2f), radius=%d\n", index, center.x, center.y, radius);
    
    // case 1: check if center lies inside the polygon
    if (isPointInsideSiteRegion(index, center) == true) 
    {
        //printf ("center (%.2f, %.2f) inside polygon [%d]\n", center.x, center.y, index);
        return true;
    }
    
    int j = 0;
    int idx;
    for (set<int>::iterator it = mSites[index].mEdgeIndexSet.begin (); 
         it != mSites[index].mEdgeIndexSet.end (); it++)
    {
        j = *it;
        line2d &line = mLines[j];

        // case 2: check if each vertex lies inside the circle
        for (int k=0; k<2; k++)
        {
            if ((idx = mLines[j].vertexIndex[k]) != -1)
            {
                if (mVertex[idx].distance (center) <= (double)radius) 
                {
                    //printf ("[%d] vertex (%.2f, %.2f) inside circle (%.2f, %.2f)\n", index, mVertex[idx].x, mVertex[idx].y, center.x, center.y);
                    return true;
                }
            }
        }

        // case 3: check if the line seg intersects with the circle
        if (line.seg.intersects (center, radius) == true)
            return true;
    }
    
    //printf ("[%d] doesn't overlap with (%.2f, %.2f)\n", index, center.x, center.y);
    return false;


/* rectangular overlap check 

    rect rec(center, radius*2, radius*2);

    point2d isect;
    int j = 0;
    
    //calculate rect 4 vertex 
    for (j=0 ; j<4 ; j++)
    {
        if ( isPointInsideSiteRegion(index,rec.vertex[j] ) ) 
        {
            return true;
        }
    }
    
    //calculate Voronoi vertex
    for( set<int>::iterator itr = mSites[index].mEdgeIndexSet.begin() ; 
    itr != mSites[index].mEdgeIndexSet.end() ; itr++ )
    {
        j = *itr;
        int index;
        for( int k=0;k<2;k++)
        {
            index = mLines[j].vertexIndex[k];
            if (index  !=-1 ) 
                if ( rec.is_inside( mVertex[index]) ) 
                    return true;
        }
    }       
    //////////////////////////////////////////////////////////////////////////
    for( int i = 0 ; i< 4 ; i ++ )
    {   set<int>::iterator itr = NULL;
    for( itr =  mSites[index].mEdgeIndexSet.begin() ; 
             itr != mSites[index].mEdgeIndexSet.end() ; 
             itr++)
             {
                 mLines[*itr].intersection( rec.lines[i] , isect );
                 //mDebug.push_back( isect );   
                 if ( rec.is_inside( isect) && isPointInsideSiteRegion(index,isect) ) 
                     return true;
             }
    }
    return false;
*/
}

/*
bool sfVoronoi::twoSitesAreNeighbor( int index1 , int index2 , int& bisectingLineIndex )
{
    if( index1 == index2)
        return false;
    for( set<int>::iterator itr = mSites[index1].mEdgeIndexSet.begin() ;
         itr != mSites[index1].mEdgeIndexSet.end() ; itr++)
    {
        if( mLines[*itr].bisecting[0] == index2 || 
            mLines[*itr].bisecting[1] == index2  )
        {
            bisectingLineIndex = *itr;
            return true;
        }
    }
    return false;

}
*/

void 
sfVoronoi::
getNeighborSet (int index, set<int>& nset)
{
    if (index < 0 || index >= (int)mSites.size())
        return;

    nset.clear ();
    for (set<int>::iterator it = mSites[index].mEdgeIndexSet.begin();
         it != mSites[index].mEdgeIndexSet.end() ; it++)
    {       
        int idx = (mLines[*it].bisecting[0] == index ? mLines[*it].bisecting[1] : mLines[*it].bisecting[0]);
        nset.insert (idx);
    }
}

bool sfVoronoi::isPointInsideSiteRegion (int index , const point2d& p)
{

    int j;
    bool b1, b2 ;
        set<int>::iterator itr = mSites[index].mEdgeIndexSet.begin ();
        
    for ( ;itr != mSites[index].mEdgeIndexSet.end (); itr++)
    {
        j = *itr;
        b1 = (mLines[j].a * mSites[index].x + mLines[j].b * mSites[index].y > mLines[j].c );
        b2 = (mLines[j].a * p.x + mLines[j].b * p.y > mLines[j].c );
        if (fabs(mLines[j].a * p.x + mLines[j].b * p.y - mLines[j].c ) < 0.001) 
            b2 = b1;
        if (b1 != b2)         
            return false;        
    }

    return (itr == mSites[index].mEdgeIndexSet.end () ? true : false);
}

bool sfVoronoi::isBoundarySite (int index)
{
    for (set<int>::iterator it = mSites[index].mEdgeIndexSet.begin();
         it != mSites[index].mEdgeIndexSet.end(); it++)
    {
        if ( mLines[*it].vertexIndex[0] == -1 || 
             mLines[*it].vertexIndex[1] == -1 ) 
        {
            return true;
        }
    }
    return false;
}

bool sfVoronoi::enclosed (int index, point2d& center, int radius)
{
    int idx1, idx2;
    
    for (set<int>::iterator it = mSites[index].mEdgeIndexSet.begin();
         it != mSites[index].mEdgeIndexSet.end(); it++)
    {
        idx1 = mLines[*it].vertexIndex[0];
        idx2 = mLines[*it].vertexIndex[1];

        // TODO: we are checking redundent points, can be avoided
        if ( idx1 == -1 || 
             idx2 == -1 ||
             center.distance (mVertex[idx1]) >= (double)radius ||
             center.distance (mVertex[idx2]) >= (double)radius ) 
        {
            return false;
        }
    }
    return true;
}


} // end for namespace Vast

//////////////////////////////////////////////////////////////////////////
