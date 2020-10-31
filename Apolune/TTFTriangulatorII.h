// *************************************************************************************
//	TTFTriangulatorII.h
//
//		Triangulator that emits 2d glyphs with no curvature data.  Curves are approximated by linear segments.
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

// ----- TriangulatorII -----
class TriangulatorII : public Triangulator {

	private:
		std::vector<ContourPoint> contours;

		// not to be copied or moved
		TriangulatorII (const TriangulatorII&);// = delete;
		TriangulatorII (TriangulatorII&&);// = delete;
		TriangulatorII& operator= (const TriangulatorII&);// = delete;
		TriangulatorII& operator= (TriangulatorII&&);// = delete;

	protected:
		// internal types
		typedef std::vector<ContourPoint> ContourData;
		typedef std::vector<ContourPoint>::const_iterator CItr;

		// triangulation functions
		void AddCurvatureEdges (vec2s pp, vec2s ip, vec2s pc);
		void TraceContourHelper (const ContourPoint& cp, vec2s& pp, vec2s& ip, bool& ppc);
		void TraceContour (CItr begin, CItr end);
		void Triangulate ();

		// Triangulator interface
		virtual std::vector<ContourPoint>& GetContours ();
		virtual void AppendTriangulation (MeshSmall&);
		virtual void AppendTriangulation (MeshLarge&);
		virtual size_t GetVertexCount ();

		// TriangulatorII interface
		virtual size_t GetCurveDivideCount (vec2s p0, vec2s p1, vec2s p2) = 0;		// returns the number of edge sub-divisions for a given curve (0 = straight line, 1 = 2 edges, 2 = 3, ect...)

	public:
		TriangulatorII ();
		virtual ~TriangulatorII ();
	};



// ---------------------------------------------------------------------------------------------------------------------------
//	TriangulatorIIFixed
//		- an example of how to overload TriangulatorII
// ---------------------------------------------------------------------------------------------------------------------------

// ----- TriangulatorIIFixed -----
class TriangulatorIIFixed : public TriangulatorII {

	private:
		size_t k;

	protected:
		virtual size_t GetCurveDivideCount (vec2s p0, vec2s p1, vec2s p2);

	public:
		TriangulatorIIFixed (size_t k);
	};



// ---------------------------------------------------------------------------------------------------------------------------
//	End
// ---------------------------------------------------------------------------------------------------------------------------

}			// end of TTFCore namespace
