// *************************************************************************************
//	TTFTriangulatorI.h
//
//		Triangulator that emits 2d glyphs with curvature data.
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
//	TriangulatorI
//		- basic glyph triangulator
//		- emits 2d glyphs with curvature data (ie. uses a pixel shader to render curves)
//
//		implemented in: TTFTriangulator.cpp
// ---------------------------------------------------------------------------------------------------------------------------

// ----- TriangulatorI -----
class TriangulatorI : public Triangulator {

	private:
		std::vector<ContourPoint> contours;

		// not to be copied or moved
		TriangulatorI (const TriangulatorI&);// = delete;
		TriangulatorI (TriangulatorI&&);// = delete;
		TriangulatorI& operator= (const TriangulatorI&);// = delete;
		TriangulatorI& operator= (TriangulatorI&&);// = delete;

	protected:
		// internal types
		typedef std::vector<ContourPoint> ContourData;						// contour data
		typedef std::vector<ContourPoint>::const_iterator CItr;			// contour iterator

		// triangulation functions
		void TraceContourHelper (const ContourPoint& cp, vec2s& pp, vec2s& ip, bool& ppc);		// TraceContour helper
		void TraceContour (CItr begin, CItr end);																			// creates edge data from contours
		void Triangulate ();																											// triangulates the contours, filling verts and tris with the final mesh data

		// Triangulator interface
		virtual std::vector<ContourPoint>& GetContours ();
		virtual void AppendTriangulation (MeshSmall&);
		virtual void AppendTriangulation (MeshLarge&);
		virtual size_t GetVertexCount ();

	public:
		TriangulatorI ();
		virtual ~TriangulatorI ();
	};



// ---------------------------------------------------------------------------------------------------------------------------
//	End
// ---------------------------------------------------------------------------------------------------------------------------

}			// end of TTFCore namespace
