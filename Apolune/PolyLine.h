#pragma once

/*

Implementation

Here is a sample "C++" implementation of this algorithm.

// Copyright 2002 softSurfer, 2012 Dan Sunday
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.
 

// Assume that classes are already given for the objects:
//    Point and Vector with
//        coordinates {float x, y, z;}     // as many as are needed
//        operators for:
//            == to test equality
//            != to test inequality
//            (Vector)0 = (0,0,0)         (null vector)
//            Point  = Point ± Vector
//            Vector = Point - Point
//            Vector = Vector ± Vector
//            Vector = Scalar * Vector    (scalar product)
//            Vector = Vector * Vector    (cross product)
//    Segment with defining endpoints {Point P0, P1;}
//===================================================================
 

// dot product (3D) which allows vector operations in arguments
#define dot(u,v)  ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
#define norm2(v)  dot(v,v)         // norm2 = squared length of vector
#define norm(v)   sqrt(norm2(v))   // norm = length of vector
#define d2(u,v)   norm2(u-v)       // distance squared = norm2 of difference
#define d(u,v)    norm(u-v)        // distance = norm of difference
 


// poly_decimate(): - remove vertices to get a smaller approximate polygon
//    Input:  tol = approximation tolerance
//            V[] = polyline array of vertex points
//            n   = the number of points in V[]
//    Output: sV[]= reduced polyline vertexes (max is n)
//    Return: m   = the number of points in sV[]
int
poly_decimate( float tol, Point* V, int n, Point* sV )
{
    int    i, k, m, pv;             // misc counters
    float  tol2 = tol * tol;        // tolerance squared
    Point* vt = new Point[n];       // vertex buffer
    int*   mk = new int[n] = {0};   // marker  buffer

    // STAGE 1.  Vertex Reduction within tolerance of  prior vertex cluster
    vt[0] = V[0];               // start at the beginning
    for (i=k=1, pv=0; i<n; i++) {
        if (d2(V[i], V[pv]) < tol2)
            continue;
        vt[k++] = V[i];
        pv = i;
    }
    if (pv < n-1)
        vt[k++] = V[n-1];       // finish at the end

    // STAGE 2.  Douglas-Peucker polyline reduction
    mk[0] = mk[k-1] = 1;       //  mark the first and last vertexes
    poly_decimateDP( tol, vt, 0, k-1, mk  );

    // copy marked vertices to the reduced polyline
    for (i=m=0; i<k; i++) {
        if (mk[i])
            sV[m++] =  vt[i];
    }
    delete vt;
    delete mk;
    return m;         //  m vertices in reduced polyline
}


// poly_decimateDP():
//  This is the Douglas-Peucker recursive reduction routine
//  It marks vertexes that are part of the reduced polyline
//  for approximating the polyline subchain v[j] to v[k].
//    Input:  tol  = approximation tolerance
//            v[]  = polyline array of vertex points
//            j,k  = indices for the subchain v[j] to v[k]
//    Output: mk[] = array of markers matching vertex array v[]
void
poly_decimateDP( float tol, Point* v, int j, int k, int* mk )
{
    if (k <= j+1) // there is nothing to decimate
        return;

    // check for adequate approximation by segment S from v[j] to v[k]
    int     maxi = j;           // index of vertex farthest from S
    float   maxd2 = 0;          // distance squared of farthest vertex
    float   tol2 = tol * tol;   // tolerance squared
    Segment S = {v[j], v[k]};   // segment from v[j] to v[k]
    Vector  u = S.P1 - S.P0;    // segment direction vector
    double  cu = dot(u,u);      // segment length squared

    // test each vertex v[i] for max distance from S
    // compute using the Algorithm dist_Point_to_Segment()
    // Note: this works in any dimension (2D, 3D, ...)
    Vector  w;
    Point   Pb;                 // base of perpendicular from v[i] to S
    double  b, cw, dv2;         // dv2 = distance v[i] to S squared

    for (int i=j+1; i<k; i++)
    {
        // compute distance squared
        w = v[i] - S.P0;
        cw = dot(w,u);
        if ( cw <= 0 )
            dv2 =d2(v[i], S.P0);
        else if ( cu <= cw )
            dv2 =d2(v[i], S.P1);
        else {
            b = cw / cu;
            Pb = S.P0 + b  * u;
            dv2 =d2(v[i], Pb);
        }
        // test with current max distance  squared
        if (dv2 <= maxd2)
            continue;
        // v[i] is a new max vertex
        maxi = i;
        maxd2 = dv2;
    }
    if (maxd2 > tol2)         // error is worse than the tolerance
    {
        // split the polyline at the farthest  vertex from S
        mk[maxi] = 1;       // mark v[maxi] for the reduced polyline
        // recursively decimate the two subpolylines at v[maxi]
        simplifyDP(  tol, v, j, maxi, mk );  // polyline v[j] to v[maxi]
        simplifyDP(  tol, v, maxi, k, mk );  // polyline v[maxi] to v[k]
    }
    // else the approximation is OK, so ignore intermediate vertexes
    return;
}
//===================================================================
	
I will try this tomorrow, if it works, i'll accept this :-), thanks –  Milo Jun 17 '10 at 5:18
	
@user146780: Happy to help :) I added some screenshots for what it's worth. –  genpfault Jun 17 '10 at 5:51
	
I'm getting results like this: img697.imageshack.us/img697/7544/edgyz.png why are the opposite edges not rounded? Thanks (see my edit for the code) –  Milo Jun 17 '10 at 17:10
	
Here's another issue img155.imageshack.us/img155/2619/eek2.png . –  Milo Jun 17 '10 at 17:17
1 	 
	
@user146780: Your dot product in slerp2d looks borked. Try (v0.x * v1.x + v0.y * v1.y) –  genpfault Jun 17 '10 at 18:05 


// v0 and v1 are normalized
// t can vary between 0 and 1
// http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/
Vector2f slerp2d( const Vector2f& v0, const Vector2f& v1, float t )
{
    float dot = v0.dot(v1);
    if( dot < -1.0f ) dot = -1.0f;
    if( dot > 1.0f ) dot = 1.0f;

    float theta_0 = acos( dot );
    float theta = theta_0 * t;

    Vector2f v2( -v0.y(), v0.x() );

    return ( v0*cos(theta) + v2*sin(theta) );
}


void glPolyline( const vector<Vector2f>& polyline, float width )
{
    if( polyline.size() < 2 ) return;
    float w = width / 2.0f;

    glBegin(GL_TRIANGLES);
    for( size_t i = 0; i < polyline.size()-1; ++i )
    {
        const Vector2f& cur = polyline[ i ];
        const Vector2f& nxt = polyline[i+1];

        Vector2f b = (nxt - cur).normalized();
        Vector2f b_perp( -b.y(), b.x() );

        Vector2f p0( cur + b_perp*w );
        Vector2f p1( cur - b_perp*w );
        Vector2f p2( nxt + b_perp*w );
        Vector2f p3( nxt - b_perp*w );

        // first triangle
        glVertex2fv( p0.data() );
        glVertex2fv( p1.data() );
        glVertex2fv( p2.data() );
        // second triangle
        glVertex2fv( p2.data() );
        glVertex2fv( p1.data() );
        glVertex2fv( p3.data() );

        // only do joins when we have a prv
        if( i == 0 ) continue;

        const Vector2f& prv = polyline[i-1];
        Vector2f a = (prv - cur).normalized();
        Vector2f a_perp( a.y(), -a.x() );

        float det = a.x()*b.y() - b.x()*a.y();
        if( det > 0 )
        {
            a_perp = -a_perp;
            b_perp = -b_perp;
        }

        // TODO: do inner miter calculation

        // flip around normals and calculate round join points
        a_perp = -a_perp;
        b_perp = -b_perp;

        size_t num_pts = 4;
        vector< Vector2f > round( 1 + num_pts + 1 );
        for( size_t j = 0; j <= num_pts+1; ++j )
        {
            float t = (float)j/(float)(num_pts+1);
            if( det > 0 )
                round[j] = cur + (slerp2d( b_perp, a_perp, 1.0f-t ) * w);
            else
                round[j] = cur + (slerp2d( a_perp, b_perp, t ) * w);
        }

        for( size_t j = 0; j < round.size()-1; ++j )
        {
            glVertex2fv( cur.data() );
            if( det > 0 )
            {
                glVertex2fv( round[j+1].data() );
                glVertex2fv( round[j+0].data() );
            }
            else
            {
                glVertex2fv( round[j+0].data() );
                glVertex2fv( round[j+1].data() );
            }
        }
    }
    glEnd();
}
*/

class PolyLine
{
public:
 PolyLine(void);
 ~PolyLine(void);
};

