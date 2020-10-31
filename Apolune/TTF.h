// *************************************************************************************
//	TTF.h
//
//		True type (*.ttf) and OpenType (*.otf) font loading/triangulation.
//
//			Font is the main class that handles true type and open type fonts.  Simply create a Font object, and use
//		GetGlyphSmall() or GetGlyphLarge() to get a glyph's mesh. GetGlyphSmall() works for almost all glyphs,
//		though it is possible some esoteric glyph's might exceed the capabilities of MeshSmall.  GetGlyphLarge()
//		will work for any glyph.
//
//		Glyphs are specified by a CodePoint:
//			- code's interpretation is dependent on platform, encoding, and language
//			- platform defaults to 3 (Microsoft)
//			- encoding defaults to 1 (Unicode Basic Multilingual Plane)
//			- language defaults to 0 (language neutral)
//
//		Fixed size multi-byte encodings are supported (8, 16, and 32 bits), but not variable sized (see caveats).
//
//			Glyph mesh's (MeshSmall and MeshLarge) are composed of a vector of vertices and a vector of triangles
//		that index the vertices.  GetMeshTriangle() will return a given triangle with everything filled in.
//		GetMeshIndexTriangle() will return a triangle with indices into the vector of vertices.  The raw data is also
//		accessible to make integrating with other API's easy.
//
//			The quadratic curves found in the gylphs are rendered using a technique described in GPU Gems 3,
//		'Rendering Vector Art on the GPU', found here: http://developer.nvidia.com/node/182.  Very simply,
//		each curve is rendered using a single triangle with special texture coordinates.  The curve is then calculated
//		using a pixel shader to determine whether a pixel is 'in' or 'out'.  This keeps things both compact in memory,
//		quicker to render (fewer tris as the curves don't need to approximated by lines), and truly resolution
//		independent.
//
//			The following equation can be used to convert the (x,y,coef) texture coords passed from a vertex shader
//		to its alpha value in a pixel:
//			float2 coords;		// texture coordinats
//			float coef;				// triangle coefficient
//			alpha = 1.0f - round((coords.x * coords.x - coords.y) * coef + 0.5f);
//
//		A more complex pixel shader that does proper AA of the curves can be found in the GPU Gems article above.
//
//		caveats:
//			- doesn't support .ttc file formats
//			- variable size multi byte encodings are not supported
//			- not all glyphs will triangulate properly, there's the possibilty of some esoteric contours that will not
//				produce valid triangulations (self intersecting contours, contours intersecting off boundary points, ect...),
//				but this should be quite rare
//			- glyph composition with point/anchor offsets isn't supported (and will throw an UnsupportedCap()
//				exception if encountered), the documention says almost nothing on this, all code I could find online
//				simply fails/errors on encountering this, and as far as I can tell it's rarely used if ever?
//
//																																		Ryan Boghean
//																																		April 2012
//																																		January 2014
//																																		June 2014
//
// *************************************************************************************

#pragma once

#include "TTFExceptions.h"
#include "TTFMath.h"
#include "TTFTriangulator.h"
#include "TTFTriangulatorI.h"
#include "TTFTriangulatorII.h"
#include "TTFTriangulatorIII.h"
#include "TTFFont.h"



// ---------------------------------------------------------------------------------------------------------------------------
//	TTF namespace
//		- bring important TTFCore types into TTF namespace
//		- just makes things easier/cleaner
//		- a user only has to worry about the types below
// ---------------------------------------------------------------------------------------------------------------------------

// ----- TTF namespace -----
namespace TTF {


// ----- exceptions -----
using TTFCore::FontException;
using TTFCore::FileFailure;
using TTFCore::FileLengthError;
using TTFCore::TableDoesNotExist;
using TTFCore::ChecksumException;
using TTFCore::VersionException;
using TTFCore::InvalidFontException;
using TTFCore::UnsupportedCap;


// ----- math types -----
using TTFCore::vec2f;
using TTFCore::vec3f;
using TTFCore::vec4f;
using TTFCore::vec2s;
using TTFCore::vec3s;
using TTFCore::vec4s;


// ----- types -----
using TTFCore::Font;
using TTFCore::CodePoint;
using TTFCore::GlyphMetrics;
using TTFCore::FontMetrics;
using TTFCore::VGlyphMetrics;
using TTFCore::VFontMetrics;
using TTFCore::MeshTriangle;
using TTFCore::MeshIndexTriangle;
using TTFCore::MeshSmall;
using TTFCore::MeshLarge;
using TTFCore::MapFromData;
using TTFCore::Triangulator;
using TTFCore::TriangulatorI;
using TTFCore::TriangulatorII;
using TTFCore::TriangulatorIIFixed;
using TTFCore::TriangulatorIII;
using TTFCore::TriangulatorIIIFixed;


// ----- constants -----
const TTFCore::MapFromData map_from_data;


// ----- end of TTF namespace -----
}



// ---------------------------------------------------------------------------------------------------------------------------
//	End
// ---------------------------------------------------------------------------------------------------------------------------

