// *************************************************************************************
//	TTFTriangulator.cpp
//
//		Implementation of the TTF triangulator.
//
//																															Ryan Bogean
//																															April 2012
//																															January 2014
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


using namespace TTFCore;



// ---------------------------------------------------------------------------------------------------------------------------
//	MeshSmall
// ---------------------------------------------------------------------------------------------------------------------------

// ----- MeshSmall -----
MeshSmall::MeshSmall () {}

MeshSmall::MeshSmall (const MeshSmall& m) : verts(m.verts), tris(m.tris) {}

MeshSmall::MeshSmall (MeshSmall&& m) : verts(std::move(m.verts)), tris(std::move(m.tris)) {}

MeshSmall& MeshSmall::operator= (const MeshSmall& m){
	if (this != &m) {
		verts = m.verts;
		tris = m.tris;
		}
	return *this;
	}
	
MeshSmall& MeshSmall::operator= (MeshSmall&& m) {
	std::swap(verts,m.verts);
	std::swap(tris,m.tris);
	return *this;
	}

size_t MeshSmall::VertexCount () const {
	return verts.size();
	}

size_t MeshSmall::TriangleCount () const {
	return tris.size();
	}

vec3f MeshSmall::GetVertex (size_t n) const {
	return verts[n];
	}

MeshTriangle MeshSmall::GetMeshTriangle (size_t n) const {

	MeshTriSmall mts;
	mts.value = tris[n];

	MeshTriangle mt;
	mt.v0 = verts[mts.i0];
	mt.v1 = verts[mts.i1];
	mt.v2 = verts[mts.i2];

	mt.t0 = vec2f(0.0f,0.0f);
	mt.t1 = vec2f(0.5f,0.0f);
	mt.t2 = vec2f(1.0f,1.0f);

	mt.coef = static_cast<float>(mts.coef) - 1.0f;

	return mt;
	}

MeshIndexTriangle MeshSmall::GetMeshIndexTriangle (size_t n) const {
	
	MeshTriSmall mts;
	mts.value = tris[n];

	MeshIndexTriangle mt;
	mt.i0 = mts.i0;
	mt.i1 = mts.i1;
	mt.i2 = mts.i2;

	mt.t0 = vec2f(0.0f,0.0f);
	mt.t1 = vec2f(0.5f,0.0f);
	mt.t2 = vec2f(1.0f,1.0f);

	mt.coef = static_cast<float>(mts.coef) - 1.0f;

	return mt;
	}

void MeshSmall::Clear () {
	verts.clear();
	tris.clear();	
	}

void MeshSmall::Trim () {
	verts.shrink_to_fit();
	tris.shrink_to_fit();
	}

void MeshSmall::Append (const MeshSmall& mesh) {

	uint32_t offset = static_cast<uint32_t>(verts.size());
	verts.insert(verts.end(),mesh.verts.begin(),mesh.verts.end());
	tris.reserve(tris.size() + mesh.tris.size());
	for (auto i = mesh.tris.begin(); i != mesh.tris.end(); ++i) {
		MeshTriSmall mts;
		mts.value = *i;
		mts.i0 += offset;
		mts.i1 += offset;
		mts.i2 += offset;
		tris.push_back(mts.value);
		}
	}

bool MeshSmall::IsValid () const {
	return verts.size() <= 1024;
	}



// ---------------------------------------------------------------------------------------------------------------------------
//	MeshLarge
// ---------------------------------------------------------------------------------------------------------------------------

// ----- MeshLarge -----
MeshLarge::MeshLarge () {}

MeshLarge::MeshLarge (const MeshLarge& m) : verts(m.verts), tris(m.tris) {}

MeshLarge::MeshLarge (MeshLarge&& m) : verts(std::move(m.verts)), tris(std::move(m.tris)) {}

MeshLarge& MeshLarge::operator= (const MeshLarge& m){
	if (this != &m) {
		verts = m.verts;
		tris = m.tris;
		}
	return *this;
	}
	
MeshLarge& MeshLarge::operator= (MeshLarge&& m) {
	std::swap(verts,m.verts);
	std::swap(tris,m.tris);
	return *this;
	}

size_t MeshLarge::VertexCount () const {
	return verts.size();
	}

size_t MeshLarge::TriangleCount () const {
	return tris.size();
	}

vec3f MeshLarge::GetVertex (size_t n) const {
	return verts[n];
	}

MeshTriangle MeshLarge::GetMeshTriangle (size_t n) const {

	MeshTriLarge mtl;
	mtl.value = tris[n];

	MeshTriangle mt;
	mt.v0 = verts[mtl.i0];
	mt.v1 = verts[mtl.i1];
	mt.v2 = verts[mtl.i2];

	mt.t0 = vec2f(0.0f,0.0f);
	mt.t1 = vec2f(0.5f,0.0f);
	mt.t2 = vec2f(1.0f,1.0f);

	mt.coef = static_cast<float>(mtl.coef);

	return mt;
	}

MeshIndexTriangle MeshLarge::GetMeshIndexTriangle (size_t n) const {
	
	MeshTriLarge mtl;
	mtl.value = tris[n];

	MeshIndexTriangle mt;
	mt.i0 = mtl.i0;
	mt.i1 = mtl.i1;
	mt.i2 = mtl.i2;

	mt.t0 = vec2f(0.0f,0.0f);
	mt.t1 = vec2f(0.5f,0.0f);
	mt.t2 = vec2f(1.0f,1.0f);

	mt.coef = static_cast<float>(mtl.coef);

	return mt;
	}

void MeshLarge::Clear () {
	verts.clear();
	tris.clear();	
	}

void MeshLarge::Trim () {
	verts.shrink_to_fit();
	tris.shrink_to_fit();
	}

void MeshLarge::Append (const MeshLarge& mesh) {

	uint16_t offset = static_cast<uint16_t>(verts.size());
	verts.insert(verts.end(),mesh.verts.begin(),mesh.verts.end());
	tris.reserve(tris.size() + mesh.tris.size());
	for (auto i = mesh.tris.begin(); i != mesh.tris.end(); ++i) {
		MeshTriLarge mtl;
		mtl.value = *i;
		mtl.i0 += offset;
		mtl.i1 += offset;
		mtl.i2 += offset;
		tris.push_back(mtl.value);
		}
	}

bool MeshLarge::IsValid () const {
	return verts.size() <= 0x10000;
	}


// ---------------------------------------------------------------------------------------------------------------------------
//	Triangulator internal types
// ---------------------------------------------------------------------------------------------------------------------------

// ----- ContourPoint -----
ContourPoint::ContourPoint () {
	end_point = false;
	flags = 0;
	pos.x=0;
 pos.y=0;
	}

bool ContourPoint::OnCurve () const {
	return (flags & 1) != 0;
	}

bool ContourPoint::XShortVector () const {
	return (flags & 2) != 0;
	}

bool ContourPoint::YShortVector () const {
	return (flags & 4) != 0;
	}

bool ContourPoint::XIsSame () const {
	return (flags & 16) != 0;
	}

bool ContourPoint::XIsDifferent () const {
	return (flags & 16) == 0;
	}

bool ContourPoint::XIsPositive () const {
	return (flags & 16) != 0;
	}

bool ContourPoint::XIsNegative () const {
	return (flags & 16) == 0;
	}

bool ContourPoint::YIsSame () const {
	return (flags & 32) != 0;
	}

bool ContourPoint::YIsDifferent () const {
	return (flags & 32) == 0;
	}

bool ContourPoint::YIsPositive () const {
	return (flags & 32) != 0;
	}

bool ContourPoint::YIsNegative () const {
	return (flags & 32) == 0;
	}


// ----- Triangle -----
Triangle::Triangle (size_t v0_, size_t v1_, size_t v2_, char coef_) {
	v0 = v0_;
	v1 = v1_;
	v2 = v2_;
	coef = coef_;
	debug = 0;
	}

Triangle::Triangle (size_t v0_, size_t v1_, size_t v2_, char coef_, char debug_) {
	v0 = v0_;
	v1 = v1_;
	v2 = v2_;
	coef = coef_;
	debug = debug_;
	}


// ----- Edge -----
Edge::Edge (size_t v0_, size_t v1_) {
	v0 = v0_;
	v1 = v1_;
	}


// ----- LineSegment -----
LineSegment::LineSegment (size_t v0_, size_t v1_, int32_t length_) {
	v0 = v0_;
	v1 = v1_;
	length = length_;
	}


// ----- Bound -----
Bound::Bound (size_t v0_, size_t v1_) : v0(v0_), v1(v1_) {}


// ----- TriEdge -----
TriEdge::TriEdge (size_t v0_, size_t v1_) : v0(v0_), v1(v1_), in_use(true) {}



// ---------------------------------------------------------------------------------------------------------------------------
//	Triangulator
// ---------------------------------------------------------------------------------------------------------------------------

// ----- constructor / destructor -----
Triangulator::Triangulator () {}
Triangulator::~Triangulator () {}


// ----- math helpers -----
int32_t Triangulator::GetTriSign (vec2s p0, vec2s p1, vec2s p2) {

	// create edges
	vec2s e0 = p2 - p0;
	vec2s e1 = p1 - p0;

	// use the '2d' cross product to determine sign
	return cross(e0,e1);
	}

bool Triangulator::IsContained (vec2s p0, vec2s p1, vec2s p2, vec2s p3) {
	// p0 should be at the middle, with p1, p2, and p3 pointing away
	// if any of the vectors (p0,p1), (p0,p2), or (p0,p3) are null (0,0) then IsContained will return false

	// create vectors
	vec2s v1 = p1 - p0;
	vec2s v2 = p2 - p0;
	vec2s v3 = p3 - p0;

	// get cross products
	int32_t c1x2 = cross(v1,v2);
	int32_t c1x3 = cross(v1,v3);
	int32_t c2x3 = cross(v2,v3);

	// determine containment
	if (c1x2 < 0)			return c1x3 < 0 && c2x3 > 0;										// RRL
	else if (c1x2 > 0)	return c1x3 < 0 || c2x3 > 0;											// LRX or LXL
	else						return c1x3 < 0 && c2x3 > 0 && dot(v1,v2) < 0;			// 0RL (if v1 and v2 are directed in the same direction, dot product is positive, the result is degenerate and no vector could be inside)
	}

bool Triangulator::Intersect1D (vec2s p0, vec2s p1, vec2s p2, vec2s p3) {
	using namespace std;

	// find longest edge
	int16_t lx = abs(max(max(max(p0.x,p1.x),p2.x),p3.x) - min(min(min(p0.x,p1.x),p2.x),p3.x));
	int16_t ly = abs(max(max(max(p0.y,p1.y),p2.y),p3.y) - min(min(min(p0.y,p1.y),p2.y),p3.y));

	int16_t q0, q1, q2, q3;
	if (lx > ly) { q0 = p0.x; q1 = p1.x; q2 = p2.x; q3 = p3.x; }
	else { q0 = p0.y; q1 = p1.y; q2 = p2.y; q3 = p3.y; }

	// order points
	if (q1 < q0) std::swap(q0,q1);
	if (q3 < q2) std::swap(q2,q3);

	// only 4 out of the possible 5 cases need to be checked
	return	q0 == q2 && q1 == q3 || 
				q2 > q0 && q2 < q1 || 
				q3 > q0 && q3 < q1 || 
				q0 > q2 && q0 < q3; 
				// || q1 > q2 && q1 < q3;
	}

bool Triangulator::Intersect2D (vec2s p0, vec2s p1, vec2s p2, vec2s p3) {

	using namespace std;
	if (p0 == p1 || p2 == p3) return false;
	//if (p0 == p3) swap(p2,p3);
	//if (p1 == p2) swap(p0,p1);
	//if (p1 == p3) { swap(p0,p1); swap(p2,p3); }

	// initialize variables
	vec2s r = p1 - p0;
	vec2s s = p3 - p2;
	int32_t rxs = cross(r,s);
	vec2s qp = p2 - p0;
	int32_t tt = cross(qp,s);
	int32_t ut = cross(qp,r);

	// check for intersection
	if (rxs > 0)		return tt > 0 && tt < rxs && ut > 0 && ut < rxs;
	else if (rxs < 0)	return tt < 0 && tt > rxs && ut < 0 && ut > rxs;
	else					return tt == 0 && ut == 0 && Intersect1D(p0,p1,p2,p3);
	}


// ----- triangulation helpers -----
void Triangulator::ConstructTri (std::vector<TriEdge>::iterator i) {
 
	// sanity check
	if (tri_edges.size() < 3) return;

	// create new tris
	auto begin = tri_edges.begin();
	auto end = tri_edges.end();

	for (auto i = begin; i != end; ++i) {

		if (i->in_use) {

			for (auto j = begin; j != end; ++j) {

				if (j->in_use && j != i && j->v0 == i->v1) {

					for (auto k = begin; k != end; ++k) {

						if (k->in_use && k != i && k != j && k->v0 == j->v1 && k->v1 == i->v0) {

							// get triangle
							vec2s p0 = verts[i->v0];
							vec2s p1 = verts[j->v0];
							vec2s p2 = verts[k->v0];

							// ensure CCW winding order
							if (GetTriSign(p0,p1,p2) > 0.0f) {
								tris.push_back(Triangle(i->v0,j->v0,k->v0,0));
								i->in_use = false;
								j->in_use = false;
								k->in_use = false;
								}
							}
						}
					}
				}
			}
		}

 /*	
	auto begin = tri_edges.begin();
	auto end = tri_edges.end();

	auto range1 = std::equal_range(begin,end,TriEdge{ i->v1, 0 },
		[](const TriEdge& e0, const TriEdge& e1) { return e0.v0 < e1.v0; }		
		);

	for (auto j = range1.first; j != range1.second; ++j) {
		if (j->in_use == false) continue;

		auto range2 = std::equal_range(begin,end,TriEdge{ j->v1, i->v0 },
			[](const TriEdge& e0, const TriEdge& e1) { return e0.v0 < e1.v0 || e0.v0 == e1.v0 && e0.v1 < e1.v1; }
			);

		for (auto k = range2.first; k != range2.second; ++k) {
			if (k->in_use == false) continue;
			tris.push_back(Triangle(i->v0,j->v0,k->v0,0));
			i->in_use = false;
			j->in_use = false;
			k->in_use = false;
			return;
			}
		}
 */
	}

void Triangulator::CreateTris () {
	using namespace std;

	// sanity check
	if (tri_edges.size() < 3) return;

	// sort for fast searching
	sort(begin(tri_edges),end(tri_edges),
		[](const TriEdge& e0, const TriEdge& e1) { 
			return e0.v0 < e1.v0 || e0.v0 == e1.v0 && e0.v1 < e1.v1;		
			}
		);

	// triangulate
	for (auto i = begin(tri_edges); i != end(tri_edges); ++i) {
		if (i->in_use) ConstructTri(i);
		}
	}

void Triangulator::TriangulateEdges () {
	using namespace std;

	// init data
	segs.clear();
	bounds.clear();
	tri_edges.clear();

	// create inner segments
	for (auto i = begin(edges); i != end(edges); ++i) {

		// intialize vectors
		size_t i0 = i->v0;
		size_t i1 = i->v1;
		size_t i2 = edges[i->pe].v0;
		vec2s p0 = verts[i0];
		vec2s p1 = verts[i1];
		vec2s p2 = verts[i2];

		// iterate over edges (not vertices, as some vertices are only used for curves)
		for (auto j = begin(edges); j != end(edges); ++j) {

			size_t i3 = j->v0;
			if (i3 == i0 || i3 == i1 || i3 == i2) continue;
			vec2s p3 = verts[i3];

			// IsContained will return false if (p3 == p0, p1, or p2), but we check early none-the-less
			if (IsContained(p0,p1,p2,p3)) {
				vec2s v = p3 - p0;
				int32_t l = dot(v,v);												// not the true length, but we don't need the square root
				segs.push_back(LineSegment(i0,i3,l));
				}
			}
		}

	// sort
	sort(begin(segs),end(segs), 
		[](const LineSegment& l0, const LineSegment& l1) -> bool {
			return l0.length < l1.length;
			}
		);

	// add edges to bounds/tri edges
	for (auto i = begin(edges); i != end(edges); ++i) {
		size_t i0 = i->v0;
		size_t i1 = i->v1;
		bounds.push_back(Bound(i0,i1));
		tri_edges.push_back(TriEdge(i0,i1));
		}
	
	// trim inner segments
	for (auto i = begin(segs); i != end(segs); ++i) {

		// init variables
		size_t i0 = i->v0;
		size_t i1 = i->v1;
		vec2s p0 = verts[i0];
		vec2s p1 = verts[i1];

		// check for intersection
		bool intersection = false;
		for (auto j = begin(bounds); j != end(bounds) && !intersection; ++j) {
			intersection = intersection || Intersect2D(p0,p1,verts[j->v0],verts[j->v1]);
			}

		// add to bounds/edges
		if (!intersection) {
			bounds.push_back(Bound(i0,i1));
			tri_edges.push_back(TriEdge(i0,i1));
			tri_edges.push_back(TriEdge(i1,i0));
			}
		}
	}


// ----- debug functions -----
void Triangulator::CreateTrisDebug () {

	// create edge tris
	for (auto i = tri_edges.begin(); i != tri_edges.end(); ++i) {

		float w = 4.0f;

		// create offsets
		vec2s p0 = verts[i->v0];
		vec2s p1 = verts[i->v1];
		vec2s d = p1 - p0;
		vec2f o0 = normal(vec2f(static_cast<float>(-d.y),static_cast<float>(d.x))) * w;
		vec2f o1 = normal(vec2f(static_cast<float>(d.y),static_cast<float>(-d.x))) * w;
		vec2s off0;
   off0.x = static_cast<int16_t>(o0.x);
   off0.y = static_cast<int16_t>(o0.y);
		vec2s off1;
   off1.x = static_cast<int16_t>(o1.x);
   off1.y = static_cast<int16_t>(o1.y);

		// create new vertices
		verts.push_back(p0 + off0);
		verts.push_back(p0 + off1);
		verts.push_back(p1 + off0);
		verts.push_back(p1 + off1);

		// add tris
		size_t sz = verts.size();
		tris.push_back(Triangle(sz - 1,sz - 3,sz - 4,0));
		tris.push_back(Triangle(sz - 1,sz - 4,sz - 2,0));
		}
	}

void Triangulator::CreateTrisDebug2 () {

	// create edge tris
	for (auto i = edges.begin(); i != edges.end(); ++i) {

		float w = 6.0f;

		// create offsets
		vec2s p0 = verts[i->v0];
		vec2s p1 = verts[i->v1];
		vec2s d = p1 - p0;
		vec2f o0 = normal(vec2f(static_cast<float>(-d.y),static_cast<float>(d.x))) * w;
		vec2f o1 = normal(vec2f(static_cast<float>(d.y),static_cast<float>(-d.x))) * w;
		vec2s off0;
   off0.x = static_cast<int16_t>(o0.x);
   off0.y = static_cast<int16_t>(o0.y);
		vec2s off1;
   off1.x = static_cast<int16_t>(o1.x);
   off1.y = static_cast<int16_t>(o1.y);

		// create new vertices
		verts.push_back(p0 + off0);
		verts.push_back(p0 + off1);

		// add tris
		size_t sz = verts.size();
		tris.push_back(Triangle(sz - 1,sz - 2,i->v1,0));
		}
	}

void Triangulator::CreateTrisDebug3 () {

	// create edge tris
	for (auto i = segs.begin(); i != segs.end(); ++i) {

		float w = 4.0f;

		// create offsets
		vec2s p0 = verts[i->v0];
		vec2s p1 = verts[i->v1];
		vec2s d = p1 - p0;
		vec2f o0 = normal(vec2f(static_cast<float>(-d.y),static_cast<float>(d.x))) * w;
		vec2f o1 = normal(vec2f(static_cast<float>(d.y),static_cast<float>(-d.x))) * w;
		vec2s off0;
   off0.x = static_cast<int16_t>(o0.x);
   off0.y = static_cast<int16_t>(o0.y);
		vec2s off1;
   off1.x = static_cast<int16_t>(o1.x);
   off1.y = static_cast<int16_t>(o1.y);

		// create new vertices
		verts.push_back(p0 + off0);
		verts.push_back(p0 + off1);

		// add tris
		size_t sz = verts.size();
		tris.push_back(Triangle(sz - 1,sz - 2,i->v1,0));
		}
	}

void Triangulator::CreateTrisDebug4 () {
	using namespace std;

	const float w = 6.0f;

	// create edge tris
	for (auto i = begin(tri_edges); i != end(tri_edges); ++i) {

		if (!i->in_use) continue;

		// create offsets
		vec2s p0 = verts[i->v0];
		vec2s p1 = verts[i->v1];
		vec2s d = p1 - p0;
		vec2f o0 = normal(vec2f(static_cast<float>(-d.y),static_cast<float>(d.x))) * w;
		vec2f o1 = normal(vec2f(static_cast<float>(d.y),static_cast<float>(-d.x))) * w;
		vec2s off0;
   off0.x = static_cast<int16_t>(o0.x);
   off0.y = static_cast<int16_t>(o0.y);
		vec2s off1;
   off1.x = static_cast<int16_t>(o1.x);
   off1.y = static_cast<int16_t>(o1.y);

		// create new vertices
		verts.push_back(p0 + off0);
		verts.push_back(p0 + off1);

		// add tris
		size_t sz = verts.size();
		tris.push_back(Triangle(sz - 1,sz - 2,i->v1,0,1));
		}
	}
