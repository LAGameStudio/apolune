// *************************************************************************************
//	TTFTriangulatorIII.h
//
//		Triangulator that emits 2d glyphs with no curvature data.  Curves are approximated by linear segments.
//
//																																		Ryan Boghean
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
#include "TTFTriangulator.h"



// ---------------------------------------------------------------------------------------------------------------------------
//	TTFCore namespace
// ---------------------------------------------------------------------------------------------------------------------------

namespace TTFCore {


	
// ---------------------------------------------------------------------------------------------------------------------------
//	TriangulatorII
//		- TriangulatorII must be inherited and GetCurveDivideCount() must be overloaded
//		- GetCurveDivideCount() is used to determine how many segments are used to approximate a curve
//
//		implemented in: TTFTriangulatorII.cpp
// ---------------------------------------------------------------------------------------------------------------------------

// ----- TriangulatorIII -----
class TriangulatorIII : public Triangulator {

	private:
		std::vector<ContourPoint> contours;
		std::vector<vec3s> verts3d;

		// not to be copied or moved
		TriangulatorIII (const TriangulatorIII&);// = delete;
		TriangulatorIII (TriangulatorIII&&);// = delete;
		TriangulatorIII& operator= (const TriangulatorIII&);// = delete;
		TriangulatorIII& operator= (TriangulatorIII&&);// = delete;

	protected:
		// internal types
		typedef std::vector<ContourPoint> ContourData;
		typedef std::vector<ContourPoint>::const_iterator CItr;

		// triangulation functions
		void AddCurvatureEdges (vec2s pp, vec2s ip, vec2s pc);
		void TraceContourHelper (const ContourPoint& cp, vec2s& pp, vec2s& ip, bool& ppc);
		void TraceContour (CItr begin, CItr end);
		void ExtrudeMesh ();
		void Triangulate ();

		// Triangulator interface
		virtual std::vector<ContourPoint>& GetContours ();
		virtual void AppendTriangulation (MeshSmall&);
		virtual void AppendTriangulation (MeshLarge&);
		virtual size_t GetVertexCount ();

		// TriangulatorIII interface
		virtual int16_t DepthOffset () = 0;																	// returns how far to offset the mesh
		virtual size_t GetCurveDivideCount (vec2s p0, vec2s p1, vec2s p2) = 0;		// returns the number of edge sub-divisions for a given curve (0 = straight line, 1 = 2 edges, 2 = 3, ect...)

	public:
		TriangulatorIII ();
		virtual ~TriangulatorIII ();
	};



// ---------------------------------------------------------------------------------------------------------------------------
//	TriangulatorIIIFixed
//		- an example of how to overload TriangulatorII
// ---------------------------------------------------------------------------------------------------------------------------

// ----- TriangulatorIIFixed -----
class TriangulatorIIIFixed : public TriangulatorIII {

	private:
		int16_t depth;
		size_t k;

	protected:
		virtual int16_t DepthOffset ();
		virtual size_t GetCurveDivideCount (vec2s p0, vec2s p1, vec2s p2);

	public:
		TriangulatorIIIFixed (int16_t depth, size_t k);
	};



// ---------------------------------------------------------------------------------------------------------------------------
//	End
// ---------------------------------------------------------------------------------------------------------------------------

}			// end of TTFCore namespace
