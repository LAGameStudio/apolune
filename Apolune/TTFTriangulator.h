// *************************************************************************************
//	TTFTriangulator.h
//
//		Triangulator base class.
//
//																																		Ryan Boghean
//																																		April 2012
//																																		January 2014
//																																		June 2014
//
// *************************************************************************************

#pragma once

#include <exception>
#include <vector>
#include <map>
#include <cstdint>

#include "TTFExceptions.h"
#include "TTFMath.h"



// ---------------------------------------------------------------------------------------------------------------------------
//	TTFCore namespace
// ---------------------------------------------------------------------------------------------------------------------------

namespace TTFCore {


	
// ---------------------------------------------------------------------------------------------------------------------------
//	Mesh types
//		- types used to define the resulting triangulated glyphs
//		- MeshSmall can be used to store glyphs with up to 1024 vertices
//		- MeshLarge can store glyphs with up to 64k vertices
//		- raw triangle data is stored as uint32_t and uint64_t simply to make it easy to pass the data to other APIs
//		- the raw data doesn't store the texture coordinates as its implied given the order of the indices
//
//		implemented in: TTFTriangulator.cpp
// ---------------------------------------------------------------------------------------------------------------------------

// ----- internal mesh types -----
union MeshTriSmall {
	uint32_t value;
	struct {
		uint32_t i0 : 10;				// implied tex coords (0.0f, 0.0f)
		uint32_t i1 : 10;				// implied tex coords (0.5f, 0.0f)
		uint32_t i2 : 10;				// implied tex coords (1.0f, 1.0f)
		uint32_t coef : 2;				// 0 = -1.0f, 1 = 0.0f, 2 = 1.0f
		};
	};

union MeshTriLarge {
	uint64_t value;
	struct {
		uint16_t i0;				// implied tex coords (0.0f, 0.0f)
		uint16_t i1;				// implied tex coords (0.5f, 0.0f)
		uint16_t i2;				// implied tex coords (1.0f, 1.0f)
		int16_t coef;				// -1 = -1.0f, 0 = 0.0f, +1 = 1.0f
		};
	};


// ----- external mesh types -----
struct MeshTriangle {
	vec3f v0, v1, v2;
	vec2f t0, t1, t2;
	float coef;
	};

struct MeshIndexTriangle {
	uint32_t i0, i1, i2;
	vec2f t0, t1, t2;
	float coef;
	};


// ----- MeshSmall -----
struct MeshSmall {

	std::vector<vec3s> verts;
	std::vector<uint32_t> tris;

	MeshSmall ();
	MeshSmall (const MeshSmall&);
	MeshSmall (MeshSmall&&);
	MeshSmall& operator= (const MeshSmall&);
	MeshSmall& operator= (MeshSmall&&);

	size_t VertexCount () const;
	size_t TriangleCount () const;
	vec3f GetVertex (size_t) const;													// returns a vertex
	MeshTriangle GetMeshTriangle (size_t) const;							// returns a MeshTriangle
	MeshIndexTriangle GetMeshIndexTriangle (size_t) const;			// returns a MeshIndexTriangle

	void Clear ();
	void Trim ();
	void Append (const MeshSmall&);			// appends the mesh to this one
	bool IsValid () const;								// returns true if a valid mesh (false indicates the glyph was too large)
	};


// ----- MeshLarge -----
struct MeshLarge {

	std::vector<vec3s> verts;
	std::vector<uint64_t> tris;

	MeshLarge ();
	MeshLarge (const MeshLarge&);
	MeshLarge (MeshLarge&&);
	MeshLarge& operator= (const MeshLarge&);
	MeshLarge& operator= (MeshLarge&&);

	size_t VertexCount () const;
	size_t TriangleCount () const;
	vec3f GetVertex (size_t) const;													// returns a vertex
	MeshTriangle GetMeshTriangle (size_t) const;							// returns a MeshTriangle
	MeshIndexTriangle GetMeshIndexTriangle (size_t) const;			// returns a MeshIndexTriangle

	void Clear ();
	void Trim ();
	void Append (const MeshLarge&);			// appends the mesh to this one
	bool IsValid () const;								// returns true if a valid mesh (false indicates the glyph was too large)
	};



// ---------------------------------------------------------------------------------------------------------------------------
//	Triangulator internal types
//
//		implemented in: TTFTriangulator.cpp
// ---------------------------------------------------------------------------------------------------------------------------

// ----- Triangulator internal types -----
struct ContourPoint {
	vec2s pos;
	bool end_point;
	uint8_t flags;

	// helper functions
	ContourPoint ();

	bool OnCurve () const;					// returns true if this point is on curve, false if off curve (control point)
	bool XShortVector () const;			// x coordinate is 1 byte
	bool YShortVector () const;			// y coordinate is 1 byte

	bool XIsSame () const;					// used when XShortVector() returns false
	bool XIsDifferent () const;				// used when XShortVector() returns false
	bool XIsPositive () const;				// used when XShortVector() returns true
	bool XIsNegative () const;				// used when XShortVector() returns true

	bool YIsSame () const;					// used when YShortVector() returns false
	bool YIsDifferent () const;				// used when YShortVector() returns false
	bool YIsPositive() const;				// used when YShortVector() returns true
	bool YIsNegative () const;				// used when YShortVector() returns true
	};

struct Triangle {
	size_t v0, v1, v2;						// vertices
	char coef, debug;						// shader coeficient (-1 = CW edge, 0 = inner tri, 1 = CCW edge)
	Triangle (size_t v0, size_t v1, size_t v2, char coef);
	Triangle (size_t v0, size_t v1, size_t v2, char coef, char debug);
	};

struct Edge {
	size_t v0, v1;
	size_t pe, ne;							// previous edge offset, next edge offset (forms a linked list)
	Edge (size_t v0, size_t v1);		// create from (v0, v1)
	};

struct LineSegment {
	size_t v0, v1;
	int32_t length;
	LineSegment (size_t v0, size_t v1, int32_t length);
	};

struct Bound {
	size_t v0, v1;
	Bound (size_t v0, size_t v1);
	};

struct TriEdge {
	size_t v0, v1;							// vertex indices
	bool in_use;								// for triangle construction (start as true and become false when used in creating a tri)
	TriEdge (size_t v0, size_t v1);
	};



// ---------------------------------------------------------------------------------------------------------------------------
//	Triangulator
//		- base class, also holds code/data that is used in all triangulator classes
//
//		implemented in: TTFTriangulator.cpp
// ---------------------------------------------------------------------------------------------------------------------------

// ----- Triangulator -----
class Triangulator {
	friend class Font;

	private:
		std::vector<LineSegment> segs;			// inner segments
		std::vector<Bound> bounds;				// bounds used for checking edge intersections
		std::vector<TriEdge> tri_edges;			// final valid triangle edges

		// not to be copied or moved
		Triangulator (const Triangulator&);// = delete;
		Triangulator (Triangulator&&);// = delete;
		Triangulator& operator= (const Triangulator&);// = delete;
		Triangulator& operator= (Triangulator&&);// = delete;

	protected:
		std::vector<vec2s> verts;					// vertex cache
		std::vector<Edge> edges;					// edge cache
		std::vector<Triangle> tris;					// triangle cache

		// math helpers
		static int32_t GetTriSign (vec2s p0, vec2s p1, vec2s p2);							// returns the the winding of the triangle (+result = CCW rotation = outer, -result = CW rotation = inner)
		static bool IsContained (vec2s p0, vec2s p1, vec2s p2, vec2s p3);				// returns true if the vector p0->p3 is between the vectors p0->p1 and p0->p2
		static bool Intersect1D (vec2s p0, vec2s p1, vec2s p2, vec2s p3);				// 1d intersection with p0 = shared vertex, p0->p1 = line 1, p0->p2 = line 2
		static bool Intersect2D (vec2s p0, vec2s p1, vec2s p2, vec2s p3);				// 2d line intersection test

		// triangulation helpers
		void ConstructTri (std::vector<TriEdge>::iterator);			// helper function for CreateTris
		void CreateTris ();																// creates tris from the edges/line segments
		void TriangulateEdges ();													// adds triangulation line segments to the edge data

		// debug functions
		void CreateTrisDebug ();					// converts tris to lines
		void CreateTrisDebug2 ();					// converts edges lines
		void CreateTrisDebug3 ();					// converts segments to lines
		void CreateTrisDebug4 ();					// converts tri_edges to lines

		// interface
		virtual std::vector<ContourPoint>& GetContours () = 0;			// returns a vector of ContourPoints, to be filled
		virtual void AppendTriangulation (MeshSmall&) = 0;					// triangulates and appends the stored contours to the mesh
		virtual void AppendTriangulation (MeshLarge&) = 0;					// triangulates and appends the stored contours to the mesh
		virtual size_t GetVertexCount () = 0;											// triangulates the contours, only returns the vertex count

	public:
		Triangulator ();
		virtual ~Triangulator ();
	};



// ---------------------------------------------------------------------------------------------------------------------------
//	End
// ---------------------------------------------------------------------------------------------------------------------------

}			// end of TTFCore namespace
