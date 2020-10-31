// *************************************************************************************
//	TTF.cpp
//
//		True type (*.ttf) and OpenType (*.otf) font loading/rendering implementation.
//
//																															Ryan Bogean
//																															April 28, 2012
//
// *************************************************************************************

#include "TTF.h"

#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <limits>
#include <utility>
#include <cmath>
#include <sstream>


using namespace Utility;
using namespace TTFCore;



// ---------------------------------------------------------------------------------------------------------------------------
//	exceptions
// ---------------------------------------------------------------------------------------------------------------------------

// ----- FontException -----
FontException::FontException () : msg("Unknown error message.") {} 
FontException::FontException (const char* msg_) : msg(msg_) {} 
FontException::FontException (const std::string& msg_) : msg(msg_) {} 
const char* FontException::what() const throw() { return msg.c_str(); }


// ----- FileFailure -----
FileFailure::FileFailure (const std::string& flnm) {
	std::stringstream ss;
	ss << "Unknown error reading file: '" << flnm << "'.";
	msg = ss.str();
	} 


// ----- FileLengthError -----
FileLengthError::FileLengthError () : FontException("Length error reading memory mapped file, file has size of 0.") {}

FileLengthError::FileLengthError (const std::string& flnm) {
	std::stringstream ss;
	ss << "Length error reading file: '" << flnm << "', file has size of 0.";
	msg = ss.str();
	} 


// ----- TableDoesNotExist -----
TableDoesNotExist::TableDoesNotExist (const std::string& table) {
	std::stringstream ss;
	ss << "Cannot load font, '" << table << "' table does not exist.";
	msg = ss.str();
	}


// ----- ChecksumException -----
ChecksumException::ChecksumException (const std::string& table) {
	std::stringstream ss;
	ss << "Check sum error in '" << table << "'.";
	msg = ss.str();
	}


// ----- VersionException -----
VersionException::VersionException (const std::string& msg) : FontException(msg) {}


// ----- InvalidFontException -----
InvalidFontException::InvalidFontException (const std::string& msg) : FontException(msg) {}


// ----- UnsupportedCap -----
UnsupportedCap::UnsupportedCap (const std::string& msg) : FontException(msg) {}



// ---------------------------------------------------------------------------------------------------------------------------
//	Simple math 'library'
// ---------------------------------------------------------------------------------------------------------------------------

// ----- vec2f -----
vec2f::vec2f () {}
vec2f::vec2f (float x_, float y_) : x(x_), y(y_) {}

bool vec2f::operator== (vec2f v) const { return x == v.x && y == v.y; }
bool vec2f::operator!= (vec2f v) const { return x != v.x || y != v.y; }

vec2f& vec2f::operator+= (vec2f v) {
	x += v.x;
	y += v.y;
	return *this;
	}

vec2f& vec2f::operator-= (vec2f v) {
	x -= v.x;
	y -= v.y;
	return *this;
	}

vec2f& vec2f::operator*= (vec2f v) {
	x *= v.x;
	y *= v.y;
	return *this;
	}

vec2f& vec2f::operator/= (vec2f v) {
	x /= v.x;
	y /= v.y;
	return *this;
	}

vec2f& vec2f::operator+= (float f) {
	x += f;
	y += f;
	return *this;
	}

vec2f& vec2f::operator-= (float f) {
	x -= f;
	y -= f;
	return *this;
	}

vec2f& vec2f::operator*= (float f) {
	x *= f;
	y *= f;
	return *this;
	}

vec2f& vec2f::operator/= (float f) {
	x /= f;
	y /= f;
	return *this;
	}

vec2f Utility::TTFCore::operator+ (vec2f v0, vec2f v1) {
	return vec2f(v0.x + v1.x, v0.y + v1.y);
	}

vec2f Utility::TTFCore::operator- (vec2f v0, vec2f v1) {
	return vec2f(v0.x - v1.x, v0.y - v1.y);
	}

vec2f Utility::TTFCore::operator* (vec2f v0, vec2f v1) {
	return vec2f(v0.x * v1.x, v0.y * v1.y);
	}

vec2f Utility::TTFCore::operator/ (vec2f v0, vec2f v1) {
	return vec2f(v0.x / v1.x, v0.y / v1.y);
	}

vec2f Utility::TTFCore::operator+ (float f, vec2f v1) {
	return vec2f(f + v1.x, f + v1.y);
	}

vec2f Utility::TTFCore::operator- (float f, vec2f v1) {
	return vec2f(f - v1.x, f - v1.y);
	}

vec2f Utility::TTFCore::operator* (float f, vec2f v1) {
	return vec2f(f * v1.x, f * v1.y);
	}

vec2f Utility::TTFCore::operator/ (float f, vec2f v1) {
	return vec2f(f / v1.x, f / v1.y);
	}

vec2f Utility::TTFCore::operator+ (vec2f v0, float f) {
	return vec2f(v0.x + f, v0.y + f);
	}

vec2f Utility::TTFCore::operator- (vec2f v0, float f) {
	return vec2f(v0.x - f, v0.y - f);
	}

vec2f Utility::TTFCore::operator* (vec2f v0, float f) {
	return vec2f(v0.x * f, v0.y * f);
	}

vec2f Utility::TTFCore::operator/ (vec2f v0, float f) {
	return vec2f(v0.x / f, v0.y / f);
	}

float Utility::TTFCore::dot (vec2f v0, vec2f v1) {
	return (v0.x*v1.x) + (v0.y*v1.y);
	}

float Utility::TTFCore::cross (vec2f v0, vec2f v1) {
	return (v0.x*v1.y) - (v0.y*v1.x);
	}


// ----- matrix3x2f -----
matrix3x2f::matrix3x2f () : a(1.0f), b(0.0f), c(0.0f), d(1.0f), e(0.0f), f(0.0f) {}

vec2f Utility::TTFCore::mul (vec2f v, const matrix3x2f& m) {
	// obviously you can't multiply a 2d vector with a 3x2 matrix
	// in this case we 'extend' the 2d vector to a 3d vector by appending 1.0f
	// course this need'nt be done explicitly

	return vec2f( v.x*m.a + v.y*m.c + m.e , v.x*m.b + v.y*m.d + m.f );
	}

vec2f Utility::TTFCore::normal (vec2f v) {
	return v / std::sqrt(dot(v,v));
	}


// ----- vec4f -----
vec4f::vec4f () {}
vec4f::vec4f (float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}



// ---------------------------------------------------------------------------------------------------------------------------
//	Triangulator
// ---------------------------------------------------------------------------------------------------------------------------

// ----- Triangulator internal types -----
ContourPoint::ContourPoint () {
	endPoint = false;
	flags = 0;
	pos = vec2f(0,0);
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

Triangle::Triangle (size_t v0_, size_t v1_, size_t v2_, char coef_) {
	v0 = v0_;
	v1 = v1_;
	v2 = v2_;
	coef = coef_;
	}

Edge::Edge (size_t v0_, size_t v1_) {
	v0 = v0_;
	v1 = v1_;
	}

LineSegment::LineSegment (size_t v0_, size_t v1_, float length_) {
	v0 = v0_;
	v1 = v1_;
	length = length_;
	}

Bound::Bound (vec2f p_, vec2f d_) : p(p_), d(d_) {}

TriEdge::TriEdge (size_t v0_, size_t v1_) : v0(v0_), v1(v1_), inUse(true) {}

void Mesh::Clear () { verts.clear(); }
std::vector<MeshVertex>::iterator Mesh::begin () { return verts.begin(); }
std::vector<MeshVertex>::const_iterator Mesh::begin () const { return verts.begin(); }
std::vector<MeshVertex>::iterator Mesh::end () { return verts.end(); }
std::vector<MeshVertex>::const_iterator Mesh::end () const { return verts.end(); }


// ----- internal helper functions -----
void Triangulator::ClearCache () {
	verts.clear();
	edges.clear();
	tris.clear();
	seg.clear();
	bounds.clear();
	triEdges.clear();
	}


// ----- math helpers -----
float Triangulator::GetTriSign (vec2f p0, vec2f p1, vec2f p2) const {

	// create edges
	vec2f e0 = p2 - p0;
	vec2f e1 = p1 - p0;

	// use the '2d' cross product to determine sign
	return cross(e0,e1);
	}

float Triangulator::GetTriSign (const std::vector<vec2f>& verts) const {

	// get vertices
	vec2f p0 = *(verts.end() - 3);
	vec2f p1 = *(verts.end() - 2);
	vec2f p2 = *(verts.end() - 1);

	// create edges
	vec2f e0 = p2 - p0;
	vec2f e1 = p1 - p0;

	// use the '2d' cross product to determine sign
	return cross(e0,e1);
	}

bool Triangulator::IsContained (vec2f p0, vec2f p1, vec2f p2, vec2f p3) const {
	// p0 should be at the middle, with p1, p2, and p3 pointing away
	// if any of the vectors (p0,p1), (p0,p2), or (p0,p3) are null (0,0) then IsContained will return false

	// create vectors
	vec2f v1 = p1 - p0;
	vec2f v2 = p2 - p0;
	vec2f v3 = p3 - p0;

	// get cross products
	float c1x2 = cross(v1,v2);
	float c1x3 = cross(v1,v3);
	float c2x3 = cross(v2,v3);

	// determine containment
	if (c1x2 < 0.0f) {
		return (c1x3 < 0.0f && c2x3 > 0.0f);										// RRL
		}
	else if (c1x2 > 0.0f) {
		return (c1x3 < 0.0f || c2x3 > 0.0f);											// LRX or LXL
		}
	else {
		return (c1x3 < 0.0f && c2x3 > 0.0f && dot(v1,v2) < 0.0f);		// 0RL (if v1 and v2 are directed in the same direction, dot product is positive, the result is degenerate and no vector could be inside)
		}
	}

bool Triangulator::Intersect (vec2f p0, vec2f p1, vec2f p2, vec2f p3) const {

	// initialize variables
	vec2f p = p0;
	vec2f q = p2;
	vec2f r = p1 - p0;
	vec2f s = p3 - p2;
	float rxs = cross(r,s);
	vec2f dqp = q - p;
	float tt = cross(dqp,s);
	float ut = cross(dqp,r);

	// check for parallel lines
	if (rxs == 0.0f) {
		return	tt == 0.0f && ut == 0.0f &&																		// check for colinear lines (need to check both tt and ut, otherwise if either is a point, it can issue a false positive)
					Intersect(p0.x,p1.x,p2.x,p3.x) && Intersect(p0.y,p1.y,p2.y,p3.y);				// check for projection intersection (seems faster to check both x and y axis than test which is larger)
		}

	// solve for t and u
	float t = tt/rxs;
	float u = ut/rxs;
	return (t > 0.0f && t < 1.0f && u > 0.0f && u < 1.0f);		// 0.0f and 1.0f are not considered intersecting
	}

bool Triangulator::Intersect2 (vec2f p, vec2f r, vec2f q, vec2f s) const {

	// initialize variables
	float rxs = cross(r,s);
	vec2f qp = q - p;
	float tt = cross(qp,s);
	float ut = cross(qp,r);

	// check for parallel lines
	if (rxs > 0.0f)		return tt > 0.0f && tt < rxs && ut > 0.0f && ut < rxs;
	else if (rxs < 0.0f)	return tt < 0.0f && tt > rxs && ut < 0.0f && ut > rxs;
	else						return tt == 0.0f && ut == 0.0f && Intersect(p.x,p.x + r.x,q.x,q.x + s.x) &&  Intersect(p.y,p.y + r.y,q.y,q.y + s.y);
	}

bool Triangulator::Intersect (float p0, float p1, float p2, float p3) const {

	if (p1 < p0) std::swap(p0,p1);
	if (p3 < p2) std::swap(p2,p3);

	// don't need to check all 5 cases, these 3 are enough
	return	p0 == p2 && p1 == p3 || 
				p2 > p0 && p2 < p1 || p3 > p0 && p3 < p1;
				//p0 > p2 && p0 < p3 || p1 > p2 && p1 < p3;
	}


// ----- triangulation functions -----
void Triangulator::CreateTris () {

	// sanity check
	if (triEdges.size() < 3) return;

	// create new tris
	auto begin = triEdges.begin();
	auto end = triEdges.end();

	for (auto i = begin; i != end; ++i) {

		if (i->inUse) {

			for (auto j = begin; j != end; ++j) {

				if (j->inUse && j != i && j->v0 == i->v1) {

					for (auto k = begin; k != end; ++k) {

						if (k->inUse && k != i && k != j && k->v0 == j->v1 && k->v1 == i->v0) {

							// get triangle
							vec2f p0 = verts[i->v0];
							vec2f p1 = verts[j->v0];
							vec2f p2 = verts[k->v0];

							// ensure CCW winding order
							if (GetTriSign(p0,p1,p2) > 0.0f) {
								tris.push_back(Triangle(i->v0,j->v0,k->v0,0));
								i->inUse = false;
								j->inUse = false;
								k->inUse = false;
								}
							}
						}
					}
				}
			}
		}
	}

void Triangulator::CreateTrisDebug () {

	// create edge tris
	for (auto i = triEdges.begin(); i != triEdges.end(); ++i) {

		float w = 4.0f;

		// create offsets
		vec2f p0 = verts[i->v0];
		vec2f p1 = verts[i->v1];
		vec2f d = p1 - p0;
		vec2f off0 = normal(vec2f(-d.y,d.x)) * w;
		vec2f off1 = normal(vec2f(d.y,-d.x)) * w;

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

void Triangulator::TriangulateEdges () {

	// sanity check
	if (edges.size() < 2) return;

	// create inner segments
	for (auto i = edges.begin(); i != edges.end(); ++i) {

		// intialize vectors
		size_t i0 = i->v0;
		size_t i1 = i->v1;
		size_t i2 = edges[i->pe].v0;
		vec2f p0 = verts[i0];
		vec2f p1 = verts[i1];
		vec2f p2 = verts[i2];

		// iterate over edge vertices
		for (auto j = edges.begin(); j != edges.end(); ++j) {

			size_t i3 = j->v0;
			vec2f p3 = verts[i3];

			// IsContained will return false if (p3 == p0, p1, or p2), so no need to check explicitly
			if (IsContained(p0,p1,p2,p3)) {
				vec2f v = p3 - p0;
				float l = dot(v,v);												// not the true length, but we don't need the square root
				seg.push_back(LineSegment(i0,i3,l));	
				}
			}
		}
	
	// sort
	std::sort(seg.begin(),seg.end(), 
		[](const LineSegment& l0, const LineSegment& l1) {
			return l0.length < l1.length;
			}
		);

	// add edges to bounds/tri edges
	for (auto i = edges.begin(); i != edges.end(); ++i) {

		size_t i0 = i->v0;
		size_t i1 = i->v1;
		vec2f p0 = verts[i0];
		vec2f p1 = verts[i1];

		bounds.push_back(Bound(p0,p1 - p0));
		triEdges.push_back(TriEdge(i0,i1));
		}
	
	// trim inner segments
	for (auto i = seg.begin(); i != seg.end(); ++i) {

		// its a bit faster to 'hoist' the these calculations outide the inner loop
		vec2f p0 = verts[i->v0];
		vec2f p1 = verts[i->v1];
		vec2f r = p1 - p0;

		// check for intersection
		bool intersection = false;
		for (auto j = bounds.begin(); j != bounds.end() && !intersection; ++j) {
			intersection = intersection || Intersect2(p0,r,j->p,j->d);
			}

		// add to bounds/edges
		if (!intersection) {
			bounds.push_back(Bound(p0,r));
			triEdges.push_back(TriEdge(i->v0,i->v1));
			triEdges.push_back(TriEdge(i->v1,i->v0));
			}
		}

	// create tris
	CreateTris();
	}

void Triangulator::TraceContourHelper (CItr i, vec2f& pp, vec2f& ip, bool& ppc) {

	// intialize point variables
	vec2f pc = i->pos;					// point current/under consideration
	bool cpc = i->OnCurve();			// current point on curve

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
		
		float sign = GetTriSign(verts);
		if (sign > 0.0f) {
			edges.push_back(Edge(n-3,n-1));						// tri is an outer, need 1 edge (cap)
			tris.push_back(Triangle(n-3,n-2,n-1,-1));
			}
		else if (sign < 0.0f) {
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
		vec2f np = (ip + pc) * 0.5f;
		verts.push_back(np);					// add new vertex
		size_t n = verts.size();

		float sign = GetTriSign(verts);
		if (sign > 0.0f) {
			edges.push_back(Edge(n-3,n-1));						// tri is an outer, need 1 edge (cap)
			tris.push_back(Triangle(n-3,n-2,n-1,-1));
			}
		else if (sign < 0.0f) {
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

void Triangulator::TraceContour (CItr begin, CItr end) {

	// begin offsets
	size_t vbo = verts.size();				// store offset of 1st vertex
	size_t ebo = edges.size();				// store offset of 1st edge

	// intialize variables
	vec2f pp = begin->pos;					// previous point on curve
	vec2f ip;										// intermediate (off curve) point/control point
	bool ppc = true;							// previous point was on the curve
	verts.push_back(pp);					// add 1st vertex to vertices of trace
	
	// draw contour
	for (auto i = begin + 1; i != end; ++i) TraceContourHelper(i,pp,ip,ppc);
	TraceContourHelper(end,pp,ip,ppc);
	TraceContourHelper(begin,pp,ip,ppc);

	// store offset of last edge
	size_t eeo = edges.size() - 1;

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

void Triangulator::TraceContourInter (CItr begin, CItr end) {
	// this is mainly here because very rarely there are contours that start with an off-curve point
	// this will mess with contour tracing
	// there are more efficient ways to handle this apart from creating a new ContourData vector
	// but it happens so rarely I can't be bothered to worry about it

	// sanity checks
	if ( (end - begin) == 0) return;
	if ( (end - begin) < 2) throw InvalidFontException("Cannot trace contour, contour length too small.");

	// begin is on curve
	if (begin->OnCurve()) {
		TraceContour(begin,end);
		return;
		}

	// begin + 1 is OnCurve
	if ( (begin + 1)->OnCurve() ) {
		ContourData contours(begin + 1,end);
		contours.push_back(*end);													// end is an actual point, unlike most stl ranges
		contours.push_back(*begin);
		TraceContour(contours.begin(),contours.end() - 1);
		return;
		}
	
	// neither begin nor begin + 1 is OnCurve
	// we have to create a new ContourPoint to start the contour
	ContourPoint cp;
	cp.endPoint = false;
	cp.flags = 1;					// on curve
	cp.pos = (begin->pos + (begin + 1)->pos) * 0.5f;

	ContourData contours;
	contours.push_back(cp);
	contours.insert(contours.end(),begin + 1,end);
	contours.push_back(*end);
	contours.push_back(*begin);
	TraceContour(contours.begin(),contours.end() - 1);
	return;
	}

Mesh Triangulator::GetTriangulation (CItr begin, CItr end) {

	// intialize
	ClearCache();
	Mesh mesh;

	// iterate over contours, create edges/vertices
	if (end - begin == 0) return mesh;
	CItr contourBegin = begin;
	for (auto i = begin + 1; i != end; ++i) {
		if (i->endPoint) {
			TraceContourInter(contourBegin,i);
			contourBegin = i + 1;
			}
		}

 for (auto i = verts.begin(); i != verts.end(); ++i) {
  vec2f pp(i->x,i->y);
  mesh.contour_verts.push_back(pp);
 }

	// triangulate polygon edges
	TriangulateEdges();

	// create mesh data
	for (auto i = tris.begin(); i != tris.end(); ++i) {

		MeshVertex mv0, mv1, mv2;
		mv0.pos = verts[i->v0];
		mv1.pos = verts[i->v1];
		mv2.pos = verts[i->v2];

		if (i->coef != 0) {
			mv0.texCoord = 0;
			mv1.texCoord = 1;
			mv2.texCoord = 2;
			}
		else {
			mv0.texCoord = 0;
			mv1.texCoord = 0;
			mv2.texCoord = 0;
			}

		mv0.coef = i->coef;
		mv1.coef = i->coef;
		mv2.coef = i->coef;

		mesh.verts.push_back(mv0);
		mesh.verts.push_back(mv1);
		mesh.verts.push_back(mv2);
		}

	// done
	return mesh;
	}



// ---------------------------------------------------------------------------------------------------------------------------
//	Font
// ---------------------------------------------------------------------------------------------------------------------------

// ----- Font internal types -----
TableEntry::TableEntry () {
	tag = 0;
	tagstr[0] = 0;
	checkSum = 0;
	begin = 0;
	end =0;
	}
	
bool TableEntry::IsValid () const {
	return tag != 0;
	}

CodePoint::CodePoint (ulong codeId_) : codeId(codeId_), platformId(3), encodingId(1), languageId(0) {}

CodePoint::CodePoint (ulong codeId_, ushort platformId_, ushort encodingId_, ushort languageId_) : codeId(codeId_), platformId(platformId_), encodingId(encodingId_), languageId(languageId_) {}


// ----- constructor/destructor -----
Font::Font (std::string flnm) {

	// load font file
	std::fstream file(flnm, std::fstream::in | std::fstream::binary);
	file.seekg(0,std::ios::end);
	size_t length = static_cast<size_t>(file.tellg());
	if (file.fail()) throw FileFailure(flnm);
	if (length == 0) throw FileLengthError(flnm);
	file.seekg(0,std::ios::beg);
	bufferCache.resize(length,0);
	file.read(&*bufferCache.begin(),length);
	if (file.fail()) throw FileFailure(flnm);

	// intialize variables
	buffer = &bufferCache.front();

	// parse font
	CreateTableMap();
	VerifyTableCheckSums();
	VerifyRequiredTables();
	VerifyTrueTypeTables();
	}

Font::Font (const void* rawData, MapFromData) {

	// intialize variables
	buffer = reinterpret_cast<const char*>(rawData);

	// parse font
	CreateTableMap();
	VerifyTableCheckSums();
	VerifyRequiredTables();
	VerifyTrueTypeTables();
	}

Font::Font (const void* rawData, size_t length) {

	// sanity check
	if (length == 0) throw FileLengthError();

	// 'load' font data
	bufferCache.assign(reinterpret_cast<const char*>(rawData),reinterpret_cast<const char*>(rawData) + length);
	buffer = &bufferCache.front();

	// parse font
	CreateTableMap();
	VerifyTableCheckSums();
	VerifyRequiredTables();
	VerifyTrueTypeTables();
	}

Font::Font (const Font& f) {
	bufferCache = f.bufferCache;
	tableMap = f.tableMap;
	meshCache = f.meshCache;
	buffer = bufferCache.empty() ? f.buffer : bufferCache.data();
	}

Font::Font (Font&& f) {
	std::swap(bufferCache,f.bufferCache);
	std::swap(tableMap,f.tableMap);
	std::swap(meshCache,f.meshCache);
	buffer = bufferCache.empty() ? f.buffer : bufferCache.data();
	}

Font& Font::operator= (const Font& f) {
	if (this != &f) {
		bufferCache = f.bufferCache;
		tableMap = f.tableMap;
		meshCache = f.meshCache;
		buffer = bufferCache.empty() ? f.buffer : bufferCache.data();
		}
	return *this;
	}

Font& Font::operator= (Font&& f) {
	if (this != &f) {
		std::swap(bufferCache,f.bufferCache);
		std::swap(tableMap,f.tableMap);
		std::swap(meshCache,f.meshCache);
		buffer = bufferCache.empty() ? f.buffer : bufferCache.data();
		}
	return *this;
	}

Font::~Font () {}


// ----- read helpers -----
unsigned char Font::ReadBYTE (FItr& itr) const { 
	unsigned char r = *reinterpret_cast<const unsigned char*>(itr);
	itr += 1;
	return r;
	}

signed char Font::ReadCHAR (FItr& itr) const {
	signed char r = *reinterpret_cast<const signed char*>(itr);
	itr += 1;
	return r;
	}

unsigned short Font::ReadUSHORT (FItr& itr) const {
	unsigned short r = ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	return r;
	}

signed short Font::ReadSHORT (FItr& itr) const {
	signed short r = 0;
	unsigned char i = ReadBYTE(itr);
	if (i & 128) r = -1;
	r <<= 8;
	r += i;
	r <<= 8;
	r += ReadBYTE(itr);
	return r;
	}

unsigned long Font::ReadUINT24 (FItr& itr) const { 
	unsigned long r = ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	return r;
	}

unsigned long Font::ReadULONG (FItr& itr) const {
	unsigned long r = ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	return r;
	}

signed long Font::ReadLONG (FItr& itr) const {
	signed long r = 0;
	unsigned char i = ReadBYTE(itr);
	if (i & 128) r = -1;
	r <<= 8;
	r += i;
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	return r;
	}

float Font::ReadFIXED32 (FItr& itr) const {
	return static_cast<float>(ReadLONG(itr)) / 65536.0f;
	}

long long Font::ReadLONGDATETIME (FItr& itr) const {
	signed long r = 0;
	unsigned char i = ReadBYTE(itr);
	if (i & 128) r = -1;
	r <<= 8;
	r += i;
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	return r;
	}

float Font::ReadFIXED16 (FItr& itr) const {
	return static_cast<float>(ReadSHORT(itr)) / 16384.0f;
	}

unsigned char Font::ReadBYTE (FItr&& itr) const { 
	return *reinterpret_cast<const unsigned char*>(itr);
	}

signed char Font::ReadCHAR (FItr&& itr) const {
	return *reinterpret_cast<const signed char*>(itr);
	}

unsigned short Font::ReadUSHORT (FItr&& itr_) const {
	FItr itr = itr_;
	unsigned short r = ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	return r;
	}

signed short Font::ReadSHORT (FItr&& itr_) const {
	FItr itr = itr_;
	signed short r = 0;
	unsigned char i = ReadBYTE(itr);
	if (i & 128) r = -1;
	r <<= 8;
	r += i;
	r <<= 8;
	r += ReadBYTE(itr);
	return r;
	}

unsigned long Font::ReadUINT24 (FItr&& itr_) const { 
	FItr itr = itr_;
	unsigned long r = ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	return r;
	}

unsigned long Font::ReadULONG (FItr&& itr_) const {
	FItr itr = itr_;
	unsigned long r = ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	return r;
	}

signed long Font::ReadLONG (FItr&& itr_) const {
	FItr itr = itr_;
	signed long r = 0;
	unsigned char i = ReadBYTE(itr);
	if (i & 128) r = -1;
	r <<= 8;
	r += i;
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	return r;
	}

float Font::ReadFIXED32 (FItr&& itr_) const {
	FItr itr = itr_;
	return static_cast<float>(ReadLONG(itr)) / 65536.0f;
	}

long long Font::ReadLONGDATETIME (FItr&& itr_) const {
	FItr itr = itr_;
	signed long r = 0;
	unsigned char i = ReadBYTE(itr);
	if (i & 128) r = -1;
	r <<= 8;
	r += i;
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	return r;
	}

float Font::ReadFIXED16 (FItr&& itr_) const {
	FItr itr = itr_;
	return static_cast<float>(ReadSHORT(itr)) / 16384.0f;
	}


// ----- more read helpers -----
TTFHeader Font::ReadTTFHeader (FItr& itr) const {

	TTFHeader header;
	header.version = ReadFIXED32(itr);
	header.numTables = ReadUSHORT(itr);
	header.searchRange = ReadUSHORT(itr);
	header.entrySelector = ReadUSHORT(itr);
	header.rangeShift = ReadUSHORT(itr);

	return header;
	}

TableEntry Font::ReadTableEntry (FItr& itr) const {

	TableEntry te;
	te.tag = ReadULONG(itr);
	DecomposeTag(te.tag,te.tagstr);
	te.checkSum = ReadULONG(itr);
	te.begin = buffer + ReadULONG(itr);
	te.end = te.begin + ReadULONG(itr);

	return te;
	}

HeadTable Font::ReadHeadTable () const {

	FItr itr = GetTableEntry(CreateTag('h','e','a','d')).begin;

	HeadTable ht;
	ht.tableVersion = ReadFIXED32(itr);
	ht.fontRevision = ReadFIXED32(itr);
	ht.checkSumAdjustment = ReadULONG(itr);
	ht.magicNumber = ReadULONG(itr);
	ht.flags = ReadUSHORT(itr);
	ht.unitsPerEm = ReadUSHORT(itr);
	ht.createdDate = ReadLONGDATETIME(itr);
	ht.modifiedData = ReadLONGDATETIME(itr);
	ht.xMin = ReadSHORT(itr);
	ht.yMin = ReadSHORT(itr);
	ht.xMax = ReadSHORT(itr);
	ht.yMax = ReadSHORT(itr);
	ht.macStyle = ReadUSHORT(itr);
	ht.lowestRecPPEM = ReadUSHORT(itr);
	ht.fontDirectionHintl = ReadSHORT(itr);
	ht.indexToLocFormat = ReadSHORT(itr);
	ht.glyphDataFormat = ReadSHORT(itr);

	return ht;
	}

GlyphProfile Font::ReadMAXPTable () const {

	FItr itr = GetTableEntry(CreateTag('m','a','x','p')).begin;

	GlyphProfile gp;
	gp.version = ReadFIXED32(itr);
	gp.numGlyphs = ReadUSHORT(itr);

	if (gp.version == 1) {
		gp.maxPoints = ReadUSHORT(itr);	
		gp.maxContours = ReadUSHORT(itr);
		gp.maxCompositePoints = ReadUSHORT(itr);
		gp.maxCompositeContours = ReadUSHORT(itr);
		gp.maxZones = ReadUSHORT(itr);
		gp.maxTwilightPoints = ReadUSHORT(itr);
		gp.maxStorage = ReadUSHORT(itr);
		gp.maxFunctionDefs = ReadUSHORT(itr);
		gp.maxInstructionDefs = ReadUSHORT(itr);
		gp.maxStackElements = ReadUSHORT(itr);
		gp.maxSizeOfInstructions = ReadUSHORT(itr);
		gp.maxComponentElements = ReadUSHORT(itr);
		gp.maxComponentDepth = ReadUSHORT(itr);
		}

	return gp;
	}

short Font::GetIndexToLocFormat () const {
	FItr itr = GetTableEntry(CreateTag('h','e','a','d')).begin;
	return ReadSHORT(itr + 50);
	}

ushort Font::GetNumGlyphs () const {
	FItr itr = GetTableEntry(CreateTag('m','a','x','p')).begin;
	return ReadUSHORT(itr + 4);
	}


// ----- table helpers -----
ulong Font::CreateTag (char c0, char c1, char c2, char c3) const {

	ulong r;
	r = static_cast<ulong>(c0);
	r <<= 8;
	r |= static_cast<ulong>(c1);
	r <<= 8;
	r |= static_cast<ulong>(c2);
	r <<= 8;
	r |= static_cast<ulong>(c3);

	return r;
	}

ulong Font::CreateTag (char* s) const {

	ulong r;
	r = static_cast<ulong>(s[0]);
	r <<= 8;
	r |= static_cast<ulong>(s[1]);
	r <<= 8;
	r |= static_cast<ulong>(s[2]);
	r <<= 8;
	r |= static_cast<ulong>(s[3]);

	return r;
	}

void Font::DecomposeTag (ulong tag, char* s) const {
	s[0] = static_cast<char>((tag >> 24) & 0xff);
	s[1] = static_cast<char>((tag >> 16) & 0xff);
	s[2] = static_cast<char>((tag >> 8) & 0xff);
	s[3] = static_cast<char>(tag & 0xff);
	s[4] = 0;
	}

std::string Font::DecomposeTag (ulong tag) const {
	char str[5];
	DecomposeTag(tag,str);
	return str;
	}

TableEntry Font::GetTableEntry (ulong tag) const {

	// find table
	auto i = tableMap.find(tag);
	if (i == tableMap.end()) throw TableDoesNotExist(DecomposeTag(tag));
	return i->second;
	}

bool Font::VerifyTableCheckSum (const TableEntry& te) const {
	if (te.tag == CreateTag('h','e','a','d'))		return VerifyHeadCheckSum(te);
	else														return VerifyNormalCheckSum(te);
	}

bool Font::VerifyNormalCheckSum (const TableEntry& te) const {

	ulong checkSum = 0;
	FItr i = te.begin;

	while (i < te.end) checkSum += ReadULONG(i);

	checkSum &= 0xffffffff;					// in case long is larger than 32 bits
	return checkSum == te.checkSum;
	}

bool Font::VerifyHeadCheckSum (const TableEntry& te) const {
	
	ulong checkSum = 0;
	FItr i = te.begin;

	checkSum += ReadULONG(i);
	checkSum += ReadULONG(i);
	i += 4;																	// checkSumAdjustment member of head table, must be skipped or set to 0 for correct checksum
	while (i < te.end) checkSum += ReadULONG(i);

	checkSum &= 0xffffffff;
	return checkSum == te.checkSum;
	}


// ----- intial loading functions -----
void Font::CreateTableMap () {

	// intialize
	FItr itr = buffer;
	tableMap.clear();

	// read in table entries
	TTFHeader header = ReadTTFHeader(itr);
	for (ushort i = 0; i < header.numTables; ++i) {
		TableEntry te = ReadTableEntry(itr);
		tableMap[te.tag] = te;
		}
	}

void Font::VerifyTableCheckSums () const {
	for (auto i = tableMap.begin(); i != tableMap.end(); ++i) {
		if (VerifyTableCheckSum(i->second) == false) throw ChecksumException(i->second.tagstr);
		}
	}

void Font::VerifyRequiredTables () const {

	ulong requiredTables[] = {
		CreateTag('c','m','a','p'),
		CreateTag('h','e','a','d'),
		CreateTag('h','h','e','a'),
		CreateTag('h','m','t','x'),
		CreateTag('m','a','x','p'),
		CreateTag('n','a','m','e'),
		CreateTag('O','S','/','2'),
		CreateTag('p','o','s','t')
		};
	size_t tableLength = sizeof(requiredTables) / sizeof(ulong);

	for (size_t i = 0; i < tableLength; ++i) {
		if (tableMap.find(requiredTables[i]) == tableMap.end()) {
			throw TableDoesNotExist(DecomposeTag(requiredTables[i]));
			}
		}
	}

void Font::VerifyTrueTypeTables () const {

	ulong requiredTables[] = {
		CreateTag('c','v','t',' '),
		CreateTag('f','p','g','m'),
		CreateTag('g','l','y','f'),
		CreateTag('l','o','c','a'),
		CreateTag('p','r','e','p'),
		};
	size_t tableLength = sizeof(requiredTables) / sizeof(ulong);

	for (size_t i = 0; i < tableLength; ++i) {
		if (tableMap.find(requiredTables[i]) == tableMap.end()) {
			throw TableDoesNotExist(DecomposeTag(requiredTables[i]));			
			}
		}
	}


// ----- CodePoint mapping -----
Font::FRange Font::GetGlyphData (CodePoint codePoint) const {
	return MapGlyphIndexToRange(GetGlyphIndex(codePoint));
	}

ushort Font::GetGlyphIndex (CodePoint codePoint) const {

	// convenience variables
	ulong codeId = codePoint.codeId;
	ushort rpid = codePoint.platformId;
	ushort reid = codePoint.encodingId;
	ushort langId = codePoint.languageId;

	// intialize variables
	FItr begin = GetTableEntry(CreateTag('c','m','a','p')).begin;
	FItr itr = begin;
	ulong glyphIndex = 0;

	// read cmap 'header'
	ushort version = ReadUSHORT(itr);
	if (version != 0) throw VersionException("Table 'cmap' version is unsupported.");
	ushort numTables = ReadUSHORT(itr);

	// iterate over tables
	for (ushort i = 0; i < numTables; ++i) {

		// load table info
		ushort platformId = ReadUSHORT(itr);
		ushort encodingId = ReadUSHORT(itr);
		FItr tableItr = begin + ReadULONG(itr);

		// check platform/encoding id
		if (rpid == platformId && reid == encodingId) {

			// load format and language info
			ushort format = ReadUSHORT(tableItr + 0);		// don't advance iterator

			// branch on format
			ushort glyphIndex = 0;
			switch (format) {
				case 0:
					glyphIndex = GetGlyphIndexF0(tableItr,langId,codeId);
					if (glyphIndex != 0) return glyphIndex;
					break;

				case 2:
					glyphIndex = GetGlyphIndexF2(tableItr,langId,codeId);
					if (glyphIndex != 0) return glyphIndex;
					break;

				case 4:
					glyphIndex = GetGlyphIndexF4(tableItr,langId,codeId);
					if (glyphIndex != 0) return glyphIndex;
					break;

				case 6:
					glyphIndex = GetGlyphIndexF6(tableItr,langId,codeId);
					if (glyphIndex != 0) return glyphIndex;
					break;

				case 8:
					glyphIndex = GetGlyphIndexF8(tableItr,langId,codeId);
					if (glyphIndex != 0) return glyphIndex;
					break;

				case 10:
					glyphIndex = GetGlyphIndexF10(tableItr,langId,codeId);
					if (glyphIndex != 0) return glyphIndex;
					break;

				case 12:
					glyphIndex = GetGlyphIndexF12(tableItr,langId,codeId);
					if (glyphIndex != 0) return glyphIndex;
					break;

				case 13:
					glyphIndex = GetGlyphIndexF13(tableItr,langId,codeId);
					if (glyphIndex != 0) return glyphIndex;
					break;

				case 14:
					glyphIndex = GetGlyphIndexF14(tableItr,langId,codeId);
					if (glyphIndex != 0) return glyphIndex;
					break;
				}
			}
		}

	return 0;
	}

Font::FRange Font::MapGlyphIndexToRange (ushort glyphIndex) const {

	// intialize variables
	short ilf = GetIndexToLocFormat();
	ushort numGlyps = GetNumGlyphs();
	TableEntry loca = GetTableEntry(CreateTag('l','o','c','a'));
	TableEntry glyf = GetTableEntry(CreateTag('g','l','y','f'));

	// sanity check
	if (glyphIndex > numGlyps) throw InvalidFontException("Invalid glyph mapping, attempting to map a glyph index that exceeds the maximum number of glyphs in the font.");

	// find glyph data range
	if (ilf == 0) {
		FRange range;
		FItr itr = loca.begin + static_cast<size_t>(glyphIndex) * 2;
		range.first = glyf.begin + static_cast<size_t>(ReadUSHORT(itr)) * 2;
		range.second = glyf.begin + static_cast<size_t>(ReadUSHORT(itr)) * 2;
		return range;
		}
	else if (ilf == 1) {
		FRange range;
		FItr itr = loca.begin + static_cast<size_t>(glyphIndex) * 4;
		range.first = glyf.begin + static_cast<size_t>(ReadULONG(itr));
		range.second = glyf.begin + static_cast<size_t>(ReadULONG(itr));
		return range;
		}
	
	// done
	throw InvalidFontException("Invalid 'head' indexToLocFormat value.");
	}

ushort Font::GetGlyphIndexF0 (FItr itr, ushort langId, ulong codeId) const {

	// load header
	ushort format = ReadUSHORT(itr);
	ushort length = ReadUSHORT(itr);
	ushort language = ReadUSHORT(itr);
	if (format != 0) throw FontException("Internal error, calling GetGlyphIndexF0() on a 'cmap' table that isn't format 0.");
	if (language != langId) return 0;

	// map code point
	if (codeId < 256) return ReadBYTE(itr + codeId);
	return 0;
	}

ushort Font::GetGlyphIndexF2 (FItr itr, ushort langId, ulong codeId) const {
	// variable sized 1 or 2 byte character encoding
	// the docs are are very unclear at how to decode this
	// on top of that I have no easy way to indicate how many bytes were consumed
	// I have no real intention to support variable sized codeId's so this is pretty much here as a 'just in case'

	/*
	// load header
	ushort format = ReadUSHORT(itr);
	ushort length = ReadUSHORT(itr);
	ushort language = ReadUSHORT(itr);
	if (language != langId) return 0;

	// get high and low bytes
	ulong lo = codeId & 0xff;
	ulong hi = (codeId >> 8) & 0xff;

	// load sub header index (also called sub header key in docs)
	size_t k = static_cast<size_t>(ReadUSHORT(itr + hi*2));		// don't modify itr
	itr += 256 + k*8;																	// advance itr to sub header

	// load sub header
	ushort firstCode = ReadUSHORT(itr);
	ushort entryCount = ReadUSHORT(itr);
	short idDelta = ReadSHORT(itr);
	ushort idRangeOffset = ReadUSHORT(itr);

	// map to glyph index
	if (k == 0) {}			// special case, 1-byte character
	*/

	return 0;
	}

ushort Font::GetGlyphIndexF4 (FItr itr, ushort langId, ulong codeId) const {
	// I have code for both linear and binary search
	// linear is faster (due to early exit) for most english
	// binary would be faster if using alot of weird glyphs or other languages found later in the BMP
	// tbh, I don't think either choice really matters given how fast it executes

	// load header
	ushort format = ReadUSHORT(itr);
	ushort length = ReadUSHORT(itr);
	ushort language = ReadUSHORT(itr);
	ushort segCountX2 = ReadUSHORT(itr);
	ushort searchRange = ReadUSHORT(itr);
	ushort entrySelector = ReadUSHORT(itr);
	ushort rangeShift = ReadUSHORT(itr);
	ushort segCount = segCountX2 / 2;		// why not just store segCount?, this file format is retarded

	if (format != 4) throw FontException("Internal error, calling GetGlyphIndexF4() on a 'cmap' table that isn't format 4.");
	if (language != langId) return 0;

	// map arrays
	FItr endCodes = itr;
	FItr startCodes = itr + 2 + segCount*1*2;
	FItr idDeltas = itr + 2 + segCount*2*2;
	FItr idRangeOffsets = itr + 2 + segCount*3*2;

	if (ReadUSHORT(endCodes + (segCount - 1) * 2) != 0xffff) throw InvalidFontException("Last end code of format 4 'cmap' table is not 0xffff.");
	if (ReadUSHORT(endCodes + (segCount) * 2) != 0) throw InvalidFontException("Variable 'reservePad' following end code data of format 4 'cmap' table is not 0.");

	// check language
	if (language != langId) return 0;

	// serach variables
	ushort i;									// if successful, i = matching index
	ulong startCode, endCode;

	// binary search
	ushort imin = 0, imax = segCount - 1;			// inclusive range
	while (imin < imax) {
		ushort imid = (imin + imax) >> 1;
		if (codeId > ReadUSHORT(endCodes + imid*2))		imin = imid + 1;		// just need to check for greater than, so only need to read end code
		else																		imax = imid;
		}

	startCode = ReadUSHORT(startCodes + imin*2);
	endCode = ReadUSHORT(endCodes + imin*2);
	if (codeId >= startCode && codeId <= endCode) i = imin;
	else return 0;

	// linear search
	/*
	for (i = 0; i < segCount; ++i) {
		startCode = ReadUSHORT(startCodes + i*2);
		endCode = ReadUSHORT(endCodes + i*2);
		if (codeId >= startCode && codeId <= endCode) break;
		}
	if (i == segCount) return 0;*/

	// calculate glyph index
	ushort idDelta = ReadUSHORT(idDeltas + i*2);									// apple docs say unsigned, MS say signed
	ushort idRangeOffset = ReadUSHORT(idRangeOffsets + i*2);
	FItr idRangeOffsetAddr = idRangeOffsets + i*2;

	// map code to its glyph index
	ushort glyphIndex = 0;
	if (idRangeOffset == 0) {
		glyphIndex = (idDelta + codeId) & 0xffff;			// 0xffff is required if short/ushort is larger than 16 bits
		}
	else {
		FItr glyphIndexArrayAddr = idRangeOffsetAddr + idRangeOffset + 2*(codeId - startCode);
		glyphIndex = ReadUSHORT(glyphIndexArrayAddr);
		}

	return glyphIndex;
	}

ushort Font::GetGlyphIndexF6 (FItr itr, ushort langId, ulong codeId) const {

	// load header
	ushort format = ReadUSHORT(itr);
	ushort length = ReadUSHORT(itr);
	ushort language = ReadUSHORT(itr);
	ulong firstCode = ReadUSHORT(itr);
	ulong entryCount = ReadUSHORT(itr);
	FItr glyphIdArray = itr;

	if (format != 6) throw FontException("Internal error, calling GetGlyphIndexF6() on a 'cmap' table that isn't format 6.");
	if (language != langId) return 0;

	// map code point
	if (codeId >= firstCode && codeId <= firstCode + entryCount) {
		ulong i = codeId - firstCode;
		return ReadUSHORT(glyphIdArray + i*2);
		}
	return 0;
	}

ushort Font::GetGlyphIndexF8 (FItr itr, ushort langId, ulong codeId) const {
	// variable sized format not supported
	return 0;
	}

ushort Font::GetGlyphIndexF10 (FItr itr, ushort langId, ulong codeId) const {
	// 32 bit dense format, same as format 6 with just a few changes to the types

	float format = ReadFIXED32(itr);				// 10.0 (unlike the other formats, this is in FIXED32 format)
	ulong length = ReadULONG(itr);					// byte length of this subtable (including the header)
	ulong language = ReadULONG(itr);
	ulong firstCode = ReadULONG(itr);			// First character code covered
	ulong entryCount = ReadULONG(itr);			// Number of character codes covered
	FItr glyphIdArray = itr;								// Array of glyph indices for the character codes covered (ushorts)

	if (format != 10.0f) throw FontException("Internal error, calling GetGlyphIndexF10() on a 'cmap' table that isn't format 10.0.");
	if (language != langId) return 0;

	// map code point
	if (codeId >= firstCode && codeId <= firstCode + entryCount) {
		ulong i = codeId - firstCode;
		return ReadUSHORT(glyphIdArray + i*2);
		}
	return 0;
	}

ushort Font::GetGlyphIndexF12 (FItr itr, ushort langId, ulong codeId) const {
	// 32 bit sparse format, similar to format 4 (actually much simpler and makes more sense)

	ushort format = ReadUSHORT(itr);
	ushort reserved = ReadUSHORT(itr);			// should be 0
	ulong length = ReadULONG(itr);					// byte length of this subtable (including the header)
	ulong language = ReadULONG(itr);
	ulong nGroups = ReadULONG(itr);				// number of groupings which follow

	if (format != 12) throw FontException("Internal error, calling GetGlyphIndexF12() on a 'cmap' table that isn't format 12.");
	if (language != langId) return 0;

	// binary search
	ulong imin = 0, imax = nGroups - 1;			// inclusive range
	while (imin < imax) {
		ulong imid = (imin + imax) >> 1;
		if (codeId > ReadULONG(itr + imid*12 + 8))	imin = imid + 1;		// just need to check for greater than, so only need to read end code
		else																imax = imid;
		}
	itr += imin*12;

	// map codeId to glyphId
	ulong startCode = ReadULONG(itr);
	ulong endCode = ReadULONG(itr);
	if (codeId >= startCode && codeId <= endCode)  {
		ulong glyphID = ReadULONG(itr);
		return static_cast<ushort>(codeId - startCode + glyphID);
		}
	
	return 0;
	}

ushort Font::GetGlyphIndexF13 (FItr itr, ushort langId, ulong codeId) const {
	// 32 bit 'many to one' format, similar to format 12

	ushort format = ReadUSHORT(itr);
	ushort reserved = ReadUSHORT(itr);			// should be 0
	ulong length = ReadULONG(itr);					// byte length of this subtable (including the header)
	ulong language = ReadULONG(itr);
	ulong nGroups = ReadULONG(itr);				// number of groupings which follow

	if (format != 13) throw FontException("Internal error, calling GetGlyphIndexF13() on a 'cmap' table that isn't format 13.");
	if (language != langId) return 0;

	// binary search
	ulong imin = 0, imax = nGroups - 1;			// inclusive range
	while (imin < imax) {
		ulong imid = (imin + imax) >> 1;
		if (codeId > ReadULONG(itr + imid*12 + 8))	imin = imid + 1;		// just need to check for greater than, so only need to read end code
		else																imax = imid;
		}
	itr += imin*12;

	// map codeId to glyphId
	ulong startCode = ReadULONG(itr);
	ulong endCode = ReadULONG(itr);
	if (codeId >= startCode && codeId <= endCode)  {
		ulong glyphID = ReadULONG(itr);
		return static_cast<ushort>(glyphID);
		}

	return 0;
	}

ushort Font::GetGlyphIndexF14 (FItr itr, ushort langId, ulong codeId) const {
	// unicode variation sequences
	// not supported
	return 0;
	}


// ----- metrics helpers -----
GlyphMetrics Font::GetGlyphMetrics (ushort glyphIndex) const {

	// read hhea data
	FItr itr = GetTableEntry(CreateTag('h','h','e','a')).begin;
	ulong version = ReadULONG(itr);
	itr += 28;																	// skip rest of hhea table
	short metricDataFormat = ReadSHORT(itr);
	ulong numOfLongHorMetrics = ReadUSHORT(itr);

	// sanity check
	if (version != 0x00010000) throw VersionException("Invalid 'hhea' table version.");
	if (metricDataFormat != 0) throw VersionException("Invalid 'hhea' metricDataFormat.");

	// read hmtx
	ushort advanceWidth;
	short leftSideBearing;

	itr = GetTableEntry(CreateTag('h','m','t','x')).begin;
	if (glyphIndex < numOfLongHorMetrics) {
		itr += glyphIndex * 4;
		advanceWidth = ReadUSHORT(itr);
		leftSideBearing = ReadSHORT(itr);
		}
	else {
		// get advanceWidth from last entry
		itr += (numOfLongHorMetrics - 1) * 4;
		advanceWidth = ReadUSHORT(itr);
		itr += 2;

		// get lsb
		itr += (glyphIndex - numOfLongHorMetrics) * 2;
		leftSideBearing = ReadSHORT(itr);
		}

	// convert to GlyphMetrics
	GlyphMetrics gm;
	gm.leftSideBearing = static_cast<float>(leftSideBearing);
	gm.advanceWidth = static_cast<float>(advanceWidth);
	return gm;
	}

VGlyphMetrics Font::GetVGlyphMetrics (ushort glyphIndex) const {

	VGlyphMetrics vgm;
	vgm.hasVerticalFontMetrics = false;

	// get vhea table
	auto i = tableMap.find(CreateTag('v','h','e','a'));
	if (i == tableMap.end()) return vgm;
	FItr itr = i->second.begin;

	// read vhea data
	ulong version = ReadULONG(itr);
	itr += 28;																	// skip rest of vhea table
	short metricDataFormat = ReadSHORT(itr);
	ulong numOfLongVerMetrics = ReadUSHORT(itr);

	// sanity check
	if (version != 0x00010000) throw VersionException("Invalid 'vhea' table version.");
	if (metricDataFormat != 0) throw VersionException("Invalid 'vhea' metricDataFormat.");

	// get vmtx table
	i = tableMap.find(CreateTag('v','m','t','x'));
	if (i == tableMap.end()) return vgm;
	vgm.hasVerticalFontMetrics = true;
	itr = i->second.begin;

	// read vmtx table
	ushort advanceHeight;
	short topSideBearing;

	if (glyphIndex < numOfLongVerMetrics) {
		itr += glyphIndex * 4;
		advanceHeight = ReadUSHORT(itr);
		topSideBearing = ReadSHORT(itr);
		}
	else {
		// get advanceWidth from last entry
		itr += (numOfLongVerMetrics - 1) * 4;
		advanceHeight = ReadUSHORT(itr);
		itr += 2;

		// get lsb
		itr += (glyphIndex - numOfLongVerMetrics) * 2;
		topSideBearing = ReadSHORT(itr);
		}

	// convert to VGlyphMetrics
	vgm.topSideBearing = static_cast<float>(topSideBearing);
	vgm.advanceHeight = static_cast<float>(advanceHeight);
	return vgm;
	}

vec2f Font::GetKerning (CodePoint cp0, CodePoint cp1, bool hk) const {

	// intialize variables
	ushort g0 = GetGlyphIndex(cp0);
	ushort g1 = GetGlyphIndex(cp1);
	vec2f kv = vec2f(GetGlyphMetrics(g0).advanceWidth,0.0f);		// get default advance width

	// get kerning table (not a required table, so we have to manually check)
	auto ti = tableMap.find(CreateTag('k','e','r','n'));
	if (ti == tableMap.end()) return kv;
	FItr itr = ti->second.begin;

	// parse kerning sub-tables
	ushort version = ReadUSHORT(itr);
	if (version == 0) {
		ushort nTables =  ReadUSHORT(itr);
		for (ushort i = 0; i < nTables; ++i) itr += ParseMSKernTable(itr,g0,g1,hk,kv);
		}
	else if (version == 1) {
		ushort versionLo = ReadUSHORT(itr);			// apple uses a different header, with the version being 4 bytes, so this is the last 2 bytes of version
		ulong nTables = ReadULONG(itr);
		for (ulong i = 0; i < nTables; ++i) itr += ParseAppleKernTable(itr,g0,g1,hk,kv);
		}
	
	// done
	return kv;
	}

float Font::ParseKernTableF0 (FItr itr, ushort g0, ushort g1) const {

	// read header
	ushort nPairs = ReadUSHORT(itr);
	ushort searchRange = ReadUSHORT(itr);		// The largest power of two less than or equal to the value of nPairs, multiplied by the size in bytes of an entry in the table.
	ushort entrySelector = ReadUSHORT(itr);		// number of iterations required
	ushort rangeShift = ReadUSHORT(itr);

	// sanity check
	if (nPairs == 0) return 0.0f;

	// perform binary search
	// "Deferred detection of equality" algorithm from: http://en.wikipedia.org/wiki/Binary_search_algorithm
	ulong key = static_cast<ulong>(g0) << 16 | static_cast<ulong>(g1);
	ushort imin = 0, imax = nPairs - 1;			// inclusive range
	while (imin < imax) {
		ushort imid = (imin + imax) >> 1;
		if (key > ReadULONG(itr + imid*6))	imin = imid + 1;
		else													imax = imid;
		}

	if (ReadULONG(itr + imin*6) == key) return static_cast<float>(ReadSHORT(itr + imin*6 + 4));
	else return 0.0f;
	}

float Font::ParseKernTableF2 (FItr itr, ushort g0, ushort g1) const {

	ushort leftClassOffset = 0;
	ushort rightClassOffset = 0;

	// read header
	FItr beginItr = itr;
	ushort rowWidth = ReadUSHORT(itr);
	ushort leftOffsetTable = ReadUSHORT(itr);
	ushort rightOffsetTable = ReadUSHORT(itr);
	ushort kernArray = ReadUSHORT(itr);

	// get left glyph class
	itr = beginItr + leftOffsetTable;
	ushort leftFirstGlyph = ReadUSHORT(itr);
	ushort leftNGlyphs = ReadUSHORT(itr);
	if (g0 >= leftFirstGlyph && g0 < leftFirstGlyph + leftNGlyphs) leftClassOffset = ReadUSHORT(itr + (g0 - leftFirstGlyph) * 2);

	// get right glyph class
	itr = beginItr + rightOffsetTable;
	ushort rightFirstGlyph = ReadUSHORT(itr);
	ushort rightNGlyphs = ReadUSHORT(itr);
	if (g0 >= rightFirstGlyph && g0 < rightFirstGlyph + rightNGlyphs) rightClassOffset = ReadUSHORT(itr + (g0 - rightFirstGlyph) * 2);

	// get kerning value
	itr = beginItr + kernArray;
	return static_cast<float>(ReadSHORT(itr + leftClassOffset + rightClassOffset));
	}

ushort Font::ParseMSKernTable (FItr itr, ushort g0, ushort g1, bool hk, vec2f& kv) const {

	// read table header
	ushort version = ReadUSHORT(itr);
	ushort length = ReadUSHORT(itr);
	ushort coverage = ReadUSHORT(itr);
	bool horizontal = (coverage & 1) != 0;				// != 0 to prevent silly warnings
	bool minimum = (coverage & 2) != 0;
	bool xstream = (coverage & 4) != 0;
	bool replace = (coverage & 8) != 0;					// also called over-ride in the MS docs
	ushort format = coverage >> 8;

	// direction check
	if (horizontal != hk) return length;

	// get kern modifier
	float km = 0;
	switch (format) {
		case 0: km = ParseKernTableF0(itr,g0,g1); break;
		case 2: km = ParseKernTableF2(itr,g0,g1); break;
		}

	// adjust kv
	if (horizontal && !xstream || !horizontal && xstream) {
		if (minimum) kv.x = (km >= 0.0f) ? std::min(kv.x,km) : std::max(kv.x,km);
		else if (replace) kv.x = km;
		else kv.x += km;
		}
	else {
		if (minimum) kv.y = (km >= 0.0f) ? std::min(kv.y,km) : std::max(kv.y,km);
		else if (replace) kv.y = km;
		else kv.y += km;
		}

	// done
	return length;
	}

ulong Font::ParseAppleKernTable (FItr itr, ushort g0, ushort g1, bool hk, vec2f& kv) const {

	// read table header
	ulong length = ReadULONG(itr);
	ushort coverage = ReadUSHORT(itr);
	ushort tupleIndex = ReadUSHORT(itr);
	bool horizontal = (coverage & 0x8000) == 0;
	bool xstream = (coverage & 0x4000) != 0;
	bool variation = (coverage & 0x2000) != 0;
	ushort format = coverage & 0xff;

	// direction check
	if (horizontal != hk) return length;

	// no support for variation values
	if (variation) return length;

	// get kern modifier
	float km = 0;
	switch (format) {
		case 0: km = ParseKernTableF0(itr,g0,g1); break;
		case 2: km = ParseKernTableF2(itr,g0,g1); break;
		}

	// adjust kv
	if (horizontal && !xstream || !horizontal && xstream)		kv.x += km;
	else																				kv.y += km;

	// done
	return length;
	}


// ----- triangulation functions -----
Mesh Font::GetGlyphMesh (ushort glyphIndex) {

	// get glyph data
	auto range = MapGlyphIndexToRange(glyphIndex);
	if (range.first == range.second) return Mesh();			// empty / no glyph data
	FItr itr = range.first;

	// get glyph contours
	short contourCount = ReadSHORT(itr + 0);
	if (contourCount > 0) return GetSimpleMesh(itr);
	else if (contourCount == -1) return GetComplexMesh(itr);
	else throw InvalidFontException("Unknown contourCount in glyph data.");
	}

Mesh Font::GetSimpleMesh (FItr itr) {

	// read header
	ushort contourCount = ReadUSHORT(itr);			// safe because its already been checked
	itr += 8;

	// create contour points
	ContourData contours;
	for (ushort i = 0; i < contourCount; ++i) {
		ushort endPt = ReadUSHORT(itr);
		if (endPt >= contours.size()) contours.resize(endPt + 1);		// assumes zero based index
		contours[endPt].endPoint = true;
		}
	size_t pointCount = contours.size();

	// skip instructions (not used)
	ushort instructionCount = ReadUSHORT(itr);
	itr += instructionCount;

	// read flags
	for (size_t i = 0; i < pointCount;) {
		unsigned char f = ReadBYTE(itr);			// flags
		ushort r = 1;											// repeat count
		if ( (f & 8) != 0 ) r += ReadBYTE(itr);	// 1 + ReadBYTE(itr)
		for (ushort j = 0; j < r; ++j) {
			contours[i].flags = f;
			++i;
			}
		}

	// read x coords
	for (size_t i = 0; i < pointCount; ++i) {
		ContourPoint& cp = contours[i];

		// read offset
		short offset;
		if (cp.XShortVector()) {
			offset = ReadBYTE(itr);
			if (cp.XIsNegative()) offset = -offset;
			}
		else {
			if (cp.XIsSame()) offset = 0;
			else offset = ReadSHORT(itr);
			}

		// store offset for later
		cp.pos.x = static_cast<float>(offset);
		}

	// read y coords
	for (size_t i = 0; i < pointCount; ++i) {
		ContourPoint& cp = contours[i];

		// read offset
		short offset;
		if (cp.YShortVector()) {
			offset = ReadBYTE(itr);
			if (cp.YIsNegative()) offset = -offset;
			}
		else {
			if (cp.YIsSame()) offset = 0;
			else offset = ReadSHORT(itr);
			}

		// store offset for later
		cp.pos.y = static_cast<float>(offset);
		}

	// adjust x/y offsets
	vec2f prePoint = vec2f(0,0);
	for (auto i = contours.begin(); i != contours.end(); ++i) {
		i->pos += prePoint;
		prePoint = i->pos;
		}

	// convert contours to mesh
	return trianglulator.GetTriangulation(contours.begin(),contours.end());
	}

Mesh Font::GetComplexMesh (FItr itr) {
	using std::max;
	using std::abs;

	// define composite flags
	const ushort ARG_1_AND_2_ARE_WORDS = 0x0001;			// If this is set, the arguments are words; otherwise, they are bytes.
	const ushort ARGS_ARE_XY_VALUES = 0x0002;					// If this is set, the arguments are xy values; otherwise, they are points.
	const ushort ROUND_XY_TO_GRID = 0x0004;						// For the xy values if the preceding is true.
	const ushort WE_HAVE_A_SCALE = 0x0008;							// This indicates that there is a simple scale for the component. Otherwise, scale = 1.0.
	const ushort RESERVED	 = 0x0010;										// This bit is reserved. Set it to 0.
	const ushort MORE_COMPONENTS = 0x0020;						// Indicates at least one more glyph after this one.
	const ushort WE_HAVE_AN_X_AND_Y_SCALE = 0x0040;		// The x direction will use a different scale from the y direction.
	const ushort WE_HAVE_A_TWO_BY_TWO = 0x0080;			// There is a 2 by 2 transformation that will be used to scale the component.
	const ushort WE_HAVE_INSTRUCTIONS = 0x0100;				// Following the last component are instructions for the composite character.
	const ushort USE_MY_METRICS	 = 0x0200;							// If set, this forces the aw and lsb (and rsb) for the composite to be equal to those from this original glyph. This works for hinted and unhinted characters.
	const ushort OVERLAP_COMPOUND = 0x0400;						// Used by Apple in GX fonts.
	const ushort SCALED_COMPONENT_OFFSET = 0x0800;		// Composite designed to have the component offset scaled (designed for Apple rasterizer).
	const ushort UNSCALED_COMPONENT_OFFSET = 0x1000;	// Composite designed not to have the component offset scaled (designed for the Microsoft TrueType rasterizer).

	// intialize variables
	Mesh mesh;			// final mesh
	itr += 10;				// skip header

	// loop while glyphs to compose
	ushort flags;
	do {

		// intialize variables
		matrix3x2f mat;
		float& a = mat.a;
		float& b = mat.b;
		float& c = mat.c;
		float& d = mat.d;
		float& e = mat.e;
		float& f = mat.f;

		// read flags/glyph index
		flags = ReadUSHORT(itr);
		ushort glyphIndex = ReadUSHORT(itr);

		// read 'e' and 'f' values
		if (flags & ARGS_ARE_XY_VALUES) {
			if (flags & ARG_1_AND_2_ARE_WORDS) {
				e = static_cast<float>(ReadSHORT(itr));
				f = static_cast<float>(ReadSHORT(itr));
				}
			else {
				e = static_cast<float>(ReadCHAR(itr));
				f = static_cast<float>(ReadCHAR(itr));
				}
			}
		else throw UnsupportedCap("Compound glyph contains point/anchor offsets, which is currently unsupported.");

		// read transform data
		if (flags & WE_HAVE_A_SCALE) {
			a = d = ReadFIXED16(itr);
			}
		else if (flags & WE_HAVE_AN_X_AND_Y_SCALE) {
			a = ReadFIXED16(itr);
			d = ReadFIXED16(itr);
			}
		else if (flags & WE_HAVE_A_TWO_BY_TWO) {
			a = ReadFIXED16(itr);
			b = ReadFIXED16(itr);
			c = ReadFIXED16(itr);
			d = ReadFIXED16(itr);
			}

		// adjust 'e' and 'f' by the screwy TTF algorithm that makes absolutely no sense
		const float k = 33.0f / 65536.0f;
		float m = max(abs(a),abs(b));
		float n = max(abs(c),abs(d));
		if (abs(abs(a) - abs(c)) <= k) m*= 2.0f;
		if (abs(abs(c) - abs(d)) <= k) n*= 2.0f;
		e *= m;
		f *= n;

		// add component Mesh's to glyph contours
		Mesh tMesh = GetGlyphMesh(glyphIndex);
		for (auto i = tMesh.begin(); i != tMesh.end(); ++i) i->pos = mul(i->pos,mat);		// transform vertices
		mesh.verts.insert(mesh.end(),tMesh.begin(),tMesh.end());

		} while (flags & MORE_COMPONENTS);

	// done
	return mesh;
	}


// ----- font info -----
vec4f Font::GetMasterRect () const {

	HeadTable ht = ReadHeadTable();
	float x = static_cast<float>(ht.xMin);
	float y = static_cast<float>(ht.yMin);
	float z = static_cast<float>(ht.xMax);
	float w = static_cast<float>(ht.yMax);

	return vec4f(x,y,z,w);
	}

vec4f Font::GetGlyphRect (CodePoint codePoint) const {

	// get glyph data
	FRange glyphData = GetGlyphData(codePoint);
	FItr itr = glyphData.first;

	// read glyph header
	short numContours = ReadSHORT(itr);
	float xMin = static_cast<float>(ReadSHORT(itr));
	float yMin = static_cast<float>(ReadSHORT(itr));
	float xMax = static_cast<float>(ReadSHORT(itr));
	float yMax = static_cast<float>(ReadSHORT(itr));
	vec4f r(xMin,yMin,xMax,yMax);

	// done
	return r;
	}

FontMetrics Font::GetFontMetrics () const {

	// read hhea data
	FItr itr = GetTableEntry(CreateTag('h','h','e','a')).begin;
	ulong version = ReadULONG(itr);
	short ascent = ReadSHORT(itr);
	short descent = ReadSHORT(itr);
	short lineGap = ReadSHORT(itr);
	ushort advanceWidthMax = ReadUSHORT(itr);
	short minLeftSideBearing = ReadSHORT(itr);
	short minRightSideBearing = ReadSHORT(itr);
	short xMaxExtent = ReadSHORT(itr);
	short caretSlopeRise = ReadSHORT(itr);
	short caretSlopeRun = ReadSHORT(itr);
	short caretOffset = ReadSHORT(itr);
	short reserved0 = ReadSHORT(itr);
	short reserved1 = ReadSHORT(itr);
	short reserved2 = ReadSHORT(itr);
	short reserved3 = ReadSHORT(itr);
	short metricDataFormat = ReadSHORT(itr);
	ushort numOfLongHorMetrics = ReadUSHORT(itr);

	// sanity check
	if (version != 0x00010000) throw VersionException("Invalid 'hhea' table version.");
	if (metricDataFormat != 0) throw VersionException("Invalid 'hhea' metricDataFormat.");

	// translate
	FontMetrics fm;
	fm.ascent = static_cast<float>(ascent);
	fm.descent = static_cast<float>(descent);
	fm.lineGap = static_cast<float>(lineGap);
	fm.caretSlope = vec2f(static_cast<float>(caretSlopeRun),static_cast<float>(caretSlopeRise));
	fm.caretOffset = static_cast<float>(caretOffset);
	fm.minLeftSideBearing = static_cast<float>(minLeftSideBearing);
	fm.minRightSideBearing = static_cast<float>(minRightSideBearing);
	fm.advanceWidthMax = static_cast<float>(advanceWidthMax);

	return fm;
	}

GlyphMetrics Font::GetGlyphMetrics (CodePoint codePoint) const {
	return GetGlyphMetrics(GetGlyphIndex(codePoint));
	}

VFontMetrics Font::GetVFontMetrics () const {

	// get vhea table data
	VFontMetrics vfm;
	auto i = tableMap.find(CreateTag('v','h','e','a'));
	if (i == tableMap.end()) {
		vfm.hasVerticalFontMetrics = false;
		return vfm;
		}
	vfm.hasVerticalFontMetrics = true;
	FItr itr = i->second.begin;

	// read table
	ulong version = ReadULONG(itr);
	short vertTypoAscender = ReadSHORT(itr);
	short vertTypoDescender = ReadSHORT(itr);
	short vertTypoLineGap = ReadSHORT(itr);
	short advanceHeightMax = ReadSHORT(itr);
	short minTopSideBearing = ReadSHORT(itr);
	short minBottomSideBearing = ReadSHORT(itr);
	short yMaxExtent = ReadSHORT(itr);
	short caretSlopeRise = ReadSHORT(itr);
	short caretSlopeRun = ReadSHORT(itr);
	short caretOffset = ReadSHORT(itr);
	short reserved0 = ReadSHORT(itr);
	short reserved1 = ReadSHORT(itr);
	short reserved2 = ReadSHORT(itr);
	short reserved3 = ReadSHORT(itr);
	short metricDataFormat = ReadSHORT(itr);
	short numOfLongVerMetrics = ReadSHORT(itr);

	// sanity check
	if (version != 0x00010000) throw VersionException("Invalid 'vhea' table version.");
	if (metricDataFormat != 0) throw VersionException("Invalid 'vhea' metricDataFormat.");

	// translate
	vfm.vertTypoDescender = static_cast<float>(vertTypoAscender);
	vfm.vertTypoAscender = static_cast<float>(vertTypoDescender);
	vfm.vertTypoLineGap = static_cast<float>(vertTypoLineGap);
	vfm.caretSlope = vec2f(static_cast<float>(caretSlopeRun),static_cast<float>(caretSlopeRise));
	vfm.caretOffset = static_cast<float>(caretOffset);
	vfm.minTopSideBearing = static_cast<float>(minTopSideBearing);
	vfm.minBottomSideBearing = static_cast<float>(minBottomSideBearing);
	vfm.advanceHeightMax = static_cast<float>(advanceHeightMax);

	return vfm;
	}

VGlyphMetrics Font::GetVGlyphMetrics (CodePoint codePoint) const {
	return GetVGlyphMetrics(GetGlyphIndex(codePoint));
	}

vec2f Font::GetKerning (CodePoint cp0, CodePoint cp1) const {
	return GetKerning(cp0,cp1,true);
	}

vec2f Font::GetVKerning (CodePoint cp0, CodePoint cp1) const {
	return GetKerning(cp0,cp1,false);
	}


// ----- glyph triangulation -----
const Mesh& Font::GetTriangulation (CodePoint codePoint) {
	ushort glyphIndex = GetGlyphIndex(codePoint);
	auto i = meshCache.find(glyphIndex);
	if (i != meshCache.end()) return i->second;
	return meshCache.insert(std::make_pair(glyphIndex,GetGlyphMesh(glyphIndex))).first->second;
	}

size_t Font::GetTriCount (CodePoint codePoint) {
	return GetTriangulation(codePoint).verts.size();			// was more complex at one time...
	}
	
void Font::ClearGlyphCache () {
	meshCache.clear();
	}

void Font::PreCache (ulong codeStart, ulong codeEnd) {
	PreCache(codeStart,codeEnd,3,1,0);
	}

void Font::PreCache (ulong codeStart, ulong codeEnd, ushort platformId, ushort encodingId, ushort languageId) {
	for (ulong i = codeStart; i <= codeEnd; ++i) {
		ushort glyphIndex = GetGlyphIndex(CodePoint(i,platformId,encodingId,languageId));
		auto j = meshCache.find(glyphIndex);
		if (j == meshCache.end()) meshCache.insert(std::make_pair(glyphIndex,GetGlyphMesh(glyphIndex)));
		}
	}


// ----- helpers -----
void Font::PreCacheBasicLatin () {
	PreCache(0x20,0x7f,3,1,0);
	}
