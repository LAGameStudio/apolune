// *************************************************************************************
//	TTFTriangulatorIII.cpp
//
//		Implementation of the TTF TriangulatorIII.
//
//																															Ryan Bogean
//																															June 2014
//
// *************************************************************************************

#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <limits>
#include <utility>
#include <cmath>
#include <sstream>

#include "TTFExceptions.h"
#include "TTFMath.h"
#include "TTFTriangulator.h"
#include "TTFTriangulatorIII.h"


using namespace TTFCore;



// ---------------------------------------------------------------------------------------------------------------------------
//	TriangulatorIII
// ---------------------------------------------------------------------------------------------------------------------------

// ----- constructor/destructor -----
TriangulatorIII::TriangulatorIII () {}
TriangulatorIII::~TriangulatorIII () {}


// ----- triangulation functions -----
void TriangulatorIII::AddCurvatureEdges (vec2s pp, vec2s ip, vec2s pc) {
	// assume pp has been pushed onto the vertex 'stack' and is the last vertex

	// get point count for curve
	size_t j = GetCurveDivideCount(pp,ip,pc);
	uint16_t k = static_cast<uint16_t>(0xffff) / static_cast<uint16_t>(j + 1);

	// create edges
	uint16_t f = k;
	for (size_t i = 0; i < j; ++i, f += k) {
		vec2s np = quad_lerp(pp,ip,pc,f);				// new interpolated point
		verts.push_back(np);
		size_t n = verts.size();
		edges.push_back(Edge(n - 2,n - 1));
		}
	verts.push_back(pc);
	size_t n = verts.size();
	edges.push_back(Edge(n - 2,n - 1));
	}

void TriangulatorIII::TraceContourHelper (const ContourPoint& cp, vec2s& pp, vec2s& ip, bool& ppc) {

	// intialize point variables
	vec2s pc = cp.pos;					// point current/under consideration
	bool cpc = cp.OnCurve();			// current point on curve

	// determine action
	if (ppc && cpc) {
		verts.push_back(pc);						// add vertex
		size_t n = verts.size();
		edges.push_back(Edge(n-2,n-1));		// add edge
		pp = pc;
		}
	else if (ppc && !cpc) {
		ip = pc;
		ppc = false;
		}
	else if (!ppc && cpc) {
		AddCurvatureEdges(pp,ip,pc);		// pp is already on the verts 'stack', ip is just a control point, pc will be added by thsis function	
		pp = pc;
		ppc = true;
		}
	else if (!ppc && !cpc) {
		vec2s np = RoundBy2(ip + pc);
		AddCurvatureEdges(pp,ip,np);		// pp is already on the verts 'stack', ip is just a control point, np will be added by this function	
		pp = np;
		ip = pc;
		//ppc = false;
		}
	}

void TriangulatorIII::TraceContour (CItr begin, CItr end) {

	// sanity checks
	if ( (end - begin) == 0) return;
	if ( (end - begin) < 2) throw InvalidFontException("Cannot trace contour, contour length too small.");

	// init variables
	size_t vbo = verts.size();				// store offset of 1st vertex
	size_t ebo = edges.size();				// store offset of 1st edge

	// trace contour
	if (begin->OnCurve()) {
		vec2s pp = begin->pos;					// previous point on curve
		vec2s ip;										// intermediate (off curve) point/control point
		bool ppc = true;							// previous point was on the curve
		verts.push_back(pp);					// add 1st vertex to vertices of trace
	
		for (auto i = begin + 1; i != end; ++i) TraceContourHelper(*i,pp,ip,ppc);
		TraceContourHelper(*end,pp,ip,ppc);
		TraceContourHelper(*begin,pp,ip,ppc);
		}
	else if ( (begin + 1)->OnCurve() ) {
		vec2s pp = (begin + 1)->pos;
		vec2s ip;
		bool ppc = true;
		verts.push_back(pp);
	
		for (auto i = begin + 2; i != end; ++i) TraceContourHelper(*i,pp,ip,ppc);
		TraceContourHelper(*end,pp,ip,ppc);
		TraceContourHelper(*begin,pp,ip,ppc);
		TraceContourHelper(*(begin + 1),pp,ip,ppc);
		}
	else {	
		// neither begin nor begin + 1 is OnCurve
		// we have to create a new ContourPoint to start the contour
		ContourPoint cp;
		cp.end_point = false;
		cp.flags = 1;					// on curve
		cp.pos = RoundBy2(begin->pos + (begin + 1)->pos);

		vec2s pp = cp.pos;
		vec2s ip;
		bool ppc = true;
		verts.push_back(pp);

		for (auto i = begin + 1; i != end; ++i) TraceContourHelper(*i,pp,ip,ppc);
		TraceContourHelper(*end,pp,ip,ppc);
		TraceContourHelper(*begin,pp,ip,ppc);
		TraceContourHelper(cp,pp,ip,ppc);
		}

	// store offset of last edge
	size_t eeo = edges.size() - 1;
	verts.pop_back();							// since there are no curves, the last vertex is never needed

	// stich last vertex to first vertex
	// only done for edges as tri's don't need their linked list
	// as well there is no guarantee for tris, while edges are guaranteed to have been added to the edge list
	edges.back().v1 = vbo;

	// create edge linked list
	edges[ebo].pe = eeo;
	edges[ebo].ne = ebo + 1;
	for (size_t i = ebo + 1; i < eeo; ++i) {
		edges[i].pe = i - 1;
		edges[i].ne = i + 1;
		}
	edges[eeo].pe = eeo - 1;
	edges[eeo].ne = ebo;
	}

void TriangulatorIII::ExtrudeMesh () {

	// init variables
	int16_t offset = DepthOffset();
	size_t n = verts.size();
	verts3d.clear();
	verts3d.reserve(n*n);

	// copy and duplicate vertices
	for (auto v=verts.begin(); v!=verts.end(); v++ ) verts3d.push_back(vec3s(v->x,v->y,0));
	for (auto v=verts.begin(); v!=verts.end(); v++ ) verts3d.push_back(vec3s(v->x,v->y,offset));

	// duplicate tris
	size_t m = tris.size();
	for (size_t i = 0; i < m; ++i) {
		Triangle t = tris[i];
		tris.push_back(Triangle(t.v0 + n,t.v2 + n,t.v1 + n,0));		// duplicate triangle and reverse winding order
		}

	// create edges
	for (auto i=edges.begin(); i!=edges.end(); i++) {
		size_t v0 = i->v0;
		size_t v1 = i->v1;
		size_t v2 = i->v0 + n;
		size_t v3 = i->v1 + n;
		tris.push_back(Triangle(v0,v2,v1,0));
		tris.push_back(Triangle(v1,v2,v3,0));
		}	
	}

void TriangulatorIII::Triangulate () {

	// intialize
	verts.clear();
	edges.clear();
	tris.clear();
	CItr begin = contours.begin();
	CItr end = contours.end();

	// iterate over contours, create edges/vertices
	if (end - begin == 0) return;
	CItr contour_begin = begin;
	for (auto i = begin + 1; i != end; ++i) {
		if (i->end_point) {
			TraceContour(contour_begin,i);
			contour_begin = i + 1;
			}
		}

	// finish
	TriangulateEdges();
	CreateTris();
	}


// ----- Triangulator interface -----
std::vector<ContourPoint>& TriangulatorIII::GetContours () {
	return contours;
	}

void TriangulatorIII::AppendTriangulation (MeshSmall& mesh) {
	using namespace std;

	// fill triangulation data
	Triangulate();
	ExtrudeMesh();

	// copy to mesh
	uint32_t offset = static_cast<uint32_t>(mesh.verts.size());
	mesh.verts.insert(mesh.verts.end(),verts3d.begin(),verts3d.end());
	mesh.tris.reserve( mesh.tris.size() + tris.size() );
	for (auto i = tris.begin(); i != tris.end(); ++i) {
		MeshTriSmall mts;
		mts.i0 = static_cast<uint32_t>(i->v0) + offset;
		mts.i1 = static_cast<uint32_t>(i->v1) + offset;
		mts.i2 = static_cast<uint32_t>(i->v2) + offset;
		mts.coef = static_cast<uint32_t>(i->coef + 1);
		mesh.tris.push_back(mts.value);
		}
	}

void TriangulatorIII::AppendTriangulation (MeshLarge& mesh) {
	using namespace std;
	
	// fill triangulation data
	Triangulate();
	ExtrudeMesh();

	// copy to mesh
	uint32_t offset = static_cast<uint32_t>(mesh.verts.size());
	mesh.verts.insert(mesh.verts.end(),verts3d.begin(),verts3d.end());
	mesh.tris.reserve( mesh.tris.size() + tris.size() );
	for (auto i = tris.begin(); i != tris.end(); ++i) {
		MeshTriLarge mtl;
		mtl.i0 = static_cast<uint16_t>(i->v0) + offset;
		mtl.i1 = static_cast<uint16_t>(i->v1) + offset;
		mtl.i2 = static_cast<uint16_t>(i->v2) + offset;
		mtl.coef = static_cast<int16_t>(i->coef);
		mesh.tris.push_back(mtl.value);
		}
	}

size_t TriangulatorIII::GetVertexCount () {
	
	// intialize
	verts.clear();
	edges.clear();
	tris.clear();
	CItr begin = contours.begin();
	CItr end = contours.end();

	// iterate over contours, create edges/vertices
	if (end - begin == 0) return 0;
	CItr contour_begin = begin;
	for (auto i = begin + 1; i != end; ++i) {
		if (i->end_point) {
			TraceContour(contour_begin,i);
			contour_begin = i + 1;
			}
		}

	// done
	return verts.size() * 2;
	}



// ---------------------------------------------------------------------------------------------------------------------------
//	TriangulatorIIIFixed
//		- an example of how to overload TriangulatorIII
// ---------------------------------------------------------------------------------------------------------------------------

TriangulatorIIIFixed::TriangulatorIIIFixed (int16_t depth_, size_t k_) : depth(depth_), k(k_) {}

int16_t TriangulatorIIIFixed::DepthOffset () {
	return depth;	
	}

size_t TriangulatorIIIFixed::GetCurveDivideCount (vec2s p0, vec2s p1, vec2s p2) {
	return k;
	}
