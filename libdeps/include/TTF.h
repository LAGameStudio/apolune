// *************************************************************************************
//	TTF.h
//
//		True type (*.ttf) and OpenType (*.otf) font loading/triangulation.
//
//		Basic usage: 
//			Font font("some_font.ttf");											// create a Font object
//			Triangualtor2dI triangulator();										// create a triangulator
//			font.TriangulateGlyph(CodePoint('A'), triangulator);		// triangulate glyph
//
//		Glyphs are specified by a CodePoint:
//			- code's interpretation is dependent on platform, encoding, and language
//			- platform defaults to 3 (Microsoft)
//			- encoding defaults to 1 (Unicode Basic Multilingual Plane)
//			- language defaults to 0 (language neutral)
//
//		Fixed size multi-byte encodings are supported (8, 16, and 32 bits), but not variable sized (see caveats).
//
//		All mesh data is stored in the Triangulator.  Each triangulator has different outputs/formats.
//
//		The quadratic curves found in the gylphs are rendered using a technique described in GPU Gems 3,
//		'Rendering Vector Art on the GPU', found here: http://developer.nvidia.com/node/182.  Very simply,
//		each curve is rendered using a single triangle with special texture coordinates.  The curve is then calculated
//		using a pixel shader to determine whether a pixel is 'in' or 'out'.  This keeps things both compact in memory,
//		quicker to render (fewer tris as the curves don't need to approximated by lines), and truly resolution
//		independent.
//
//		The following equation can be used to convert the (x,y,coef) texture coords passed from a vertex shader
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
//																																		February 2015
//
// *************************************************************************************

# pragma once

# include "TTFExceptions.h"
# include "TTFTypes.h"
# include "TTFMath.h"

# include "TTFTriangulator2D.h"
# include "TTFTriangulator2DLinear.h"
# include "TTFTriangulator3D.h"
# include "TTFTriangulator3DBezel.h"
# include "TTFFont.h"


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
using TTFCore::vec2t;
using TTFCore::vec3t;
using TTFCore::vec4t;


// ----- types -----
using TTFCore::TriangulatorFlags;

using TTFCore::Font;
using TTFCore::CodePoint;
using TTFCore::GlyphMetrics;
using TTFCore::FontMetrics;
using TTFCore::VGlyphMetrics;
using TTFCore::VFontMetrics;
using TTFCore::MapFromData;

using TTFCore::TriSmall;
using TTFCore::TriLarge;
typedef TTFCore::Triangulator2D<vec2t, TTFCore::TriSmall> Triangulator2DI;
typedef TTFCore::Triangulator2D<vec2t, TTFCore::TriLarge> Triangulator2DII;
typedef TTFCore::Triangulator2DLinear<vec2t, TTFCore::TriSmall> Triangulator2DLinearI;
typedef TTFCore::Triangulator2DLinear<vec2t, TTFCore::TriLarge> Triangulator2DLinearII;
typedef TTFCore::Triangulator3D<vec3t, TTFCore::TriSmall> Triangulator3DI;
typedef TTFCore::Triangulator3D<vec3t, TTFCore::TriLarge> Triangulator3DII;
typedef TTFCore::Triangulator3DBezel<vec3t, TTFCore::TriSmall> Triangulator3DBezelI;
typedef TTFCore::Triangulator3DBezel<vec3t, TTFCore::TriLarge> Triangulator3DBezelII;


// ----- constants -----
const TTFCore::MapFromData map_from_data;


// ----- end of TTF namespace -----
}


// ---------------------------------------------------------------------------------------------------------------------------
//	End
// ---------------------------------------------------------------------------------------------------------------------------

