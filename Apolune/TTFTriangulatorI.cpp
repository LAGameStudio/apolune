// *************************************************************************************
//	TTFTriangulatorI.cpp
//
//		Implementation of the TTF TriangulatorI.
//
//																															Ryan Bogean
//																															April 2012
//																															January 2014
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
#include <set>

#include "TTFExceptions.h"
#include "TTFMath.h"
#include "TTFTriangulator.h"
#include "TTFTriangulatorI.h"


using namespace TTFCore;



// ---------------------------------------------------------------------------------------------------------------------------
//	TriangulatorI
// ---------------------------------------------------------------------------------------------------------------------------

// ----- constructor/destructor -----
TriangulatorI::TriangulatorI () {}
TriangulatorI::~TriangulatorI () {}


// ----- triangulation functions -----
void TriangulatorI::TraceContourHelper (const ContourPoint& cp, vec2s& pp, vec2s& ip, bool& ppc) {

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
		verts.push_back(pc);		// add vertex
		ip = pc;
		ppc = false;
		}
	else if (!ppc && cpc) {
		verts.push_back(pc);		// add vertex
		size_t n = verts.size();
		
		int16_t sign = GetTriSign(pp,ip,pc);
		if (sign > 0) {
			edges.push_back(Edge(n-3,n-1));						// tri is an outer, need 1 edge (cap)
			tris.push_back(Triangle(n-3,n-2,n-1,-1));
			}
		else if (sign < 0) {
			edges.push_back(Edge(n-3,n-2));						// tri is an inner, need both edges (wedge)
			edges.push_back(Edge(n-2,n-1));
			tris.push_back(Triangle(n-3,n-2,n-1,+1));
			}
		else {
			edges.push_back(Edge(n-3,n-1));						// control point sits right on the curve, simply ignore control point
			}

		pp = pc;
		ppc = true;
		}
	else if (!ppc && !cpc) {
		vec2s np = RoundBy2(ip + pc);
		verts.push_back(np);					// add new vertex
		size_t n = verts.size();

		int16_t sign = GetTriSign(pp,ip,np);
		if (sign > 0) {
			edges.push_back(Edge(n-3,n-1));						// tri is an outer, need 1 edge (cap)
			tris.push_back(Triangle(n-3,n-2,n-1,-1));
			}
		else if (sign < 0) {
			edges.push_back(Edge(n-3,n-2));						// tri is an inner, need both edges (wedge)
			edges.push_back(Edge(n-2,n-1));
			tris.push_back(Triangle(n-3,n-2,n-1,+1));
			}
		else {
			edges.push_back(Edge(n-3,n-1));						// control point sits right on the curve, simply ignore control point
			}

		verts.push_back(pc);					// add vertex
		pp = np;
		ip = pc;
		}
	}

void TriangulatorI::TraceContour (CItr begin, CItr end) {

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
	//verts.pop_back();							// the last vertex may be needed (part of a curve)

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

void TriangulatorI::Triangulate () {

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

	// triangulate polygon edges
	TriangulateEdges();
	CreateTris();
	}


// ----- Triangulator interface -----
std::vector<ContourPoint>& TriangulatorI::GetContours () {
	return contours;
	}

void TriangulatorI::AppendTriangulation (MeshSmall& mesh) {
	using namespace std;

	// fill triangulation data
	Triangulate();

	// copy to mesh
	uint32_t offset = static_cast<uint32_t>(mesh.verts.size());
	mesh.verts.insert(mesh.verts.end(),verts.begin(),verts.end());
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

void TriangulatorI::AppendTriangulation (MeshLarge& mesh) {
	using namespace std;
	
	// fill triangulation data
	Triangulate();

	// copy to mesh
	uint32_t offset = static_cast<uint32_t>(mesh.verts.size());
	mesh.verts.insert(mesh.verts.end(),verts.begin(),verts.end());
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

size_t TriangulatorI::GetVertexCount () {
	
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
	return verts.size();
	}
