// *************************************************************************************
//	TTF.h http://sourceforge.net/projects/ttftriangulator/
//
//		True type (*.ttf) and OpenType (*.otf) font loading/triangulation.
//		Font is the main class that handles true type and open type fonts.  Simply create a Font object, and use
//			GetTriangulation() to get a glyph's mesh.
//		Glyphs are specified by a CodePoint:
//			- codeId's interpretation is dependent on platformId, encodingId, and languageId
//			- platformId defaults to 3 (Microsoft)
//			- encodingId defaults to 1 (Unicode Basic Multilingual Plane)
//			- languageId defaults to 0 (language neutral)
//		Fixed size multi-byte encodings are supported (8, 16, and 32 bits), but not variable sized (see caveats).
//		Glyph mesh's are just a vector of vertices.  Each vertex contains a 'texCoord' and 'coef' value (see below
//			on how to use them).  Each triangle is specified by 3 vertices in order.  Triangle winding is CCW.
//		The quadratic curves found in the gylphs are rendered using a technique described in GPU Gems 3,
//			'Rendering Vector Art on the GPU', found here: http://developer.nvidia.com/node/182.
//		Very simply, each curve is rendered using a single triangle with special texture coordinates.  The curve
//			is then calculated using a pixel shader to determine whether a pixel is 'in' or 'out'.  This keeps things
//			both compact in memory, quicker to render (fewer tris as the curves don't need to approximated by
//			lines), and truly resolution independent.
//
//		Mesh vertices store their texture coords (which is used for calculating curves) in a compacted format.
//		The following equations can be used in a vertex shader to 'decode' the tex coord value:
//			// t = texCoord value from MeshVertex
//			// c = coef value from MeshVertex
//			out.x = t * 0.5f;
//			out.y = max(t - 1.0f,0.0f);
//			out.z = c;
//
//		The following equation can be used to convert the (x,y,z) texture coords from above into a usable alpha value:
//			// coords = tex coords from the above vertex shader
//			alpha = round((coords.x * coords.x - coords.y) * coords.z + 0.5f);
//		A more complex pixel shader that does proper AA of the curves can be found in the GPU Gems article above.
//
//		caveats:
//			- doesn't support .ttc file formats
//			- variable size multi byte encodings are not supported
//			- not all glyphs will triangulate properly, there's the possibilty of some esoteric contours that will not
//				produce valid triangulations (self intersecting contours, contours intersecting off boundary points, ect...),
//				but this should be quite rare
//			- glyph composition with point/anchor offsets isn't supported (and will throw an UnsupportedCap() exception
//				if encountered), the documention says almost nothing on this, all code I could find online simply
//				fails/errors on encountering this, and as far as I can tell it's rarely used if ever?
//
//																																		Ryan Boghean
//																																		April 28, 2012
//
// *************************************************************************************

#ifndef TTF_H
#define TTF_H

#include <exception>
#include <vector>
#include <map>



// ---------------------------------------------------------------------------------------------------------------------------
//	Utility namespace
// ---------------------------------------------------------------------------------------------------------------------------

namespace Utility {



// ---------------------------------------------------------------------------------------------------------------------------
//	Font exceptions:
//		FontException			- general font exception catch-all
//		FileFailure					- general file error
//										- thrown when attempting to read the file causes an error
//										- not thrown when parsing the file
//		FileLengthError			- thrown when a file or memory copy has a length of 0
//		TableDoesNotExist		- thrown when attempting to access a table that doesn't exist
//										- font is incomplete or format isn't supported
//		ChecksumException	- thrown when the file has a check sum error
//										- file is corrupt
//										- only thrown when loading the file (creating the Font object)
//		VersionException		- thrown when an unsupported table/feature version is encountered
//		InvalidFontException	- something didn't match up
//										- font is invalid/errornous/cannot be parsed
//		UnsupportedCap		- a feature in the font is not supported
//
//		implemented in: TTF.cpp
// ---------------------------------------------------------------------------------------------------------------------------

// ----- TTFCore namespace -----
namespace TTFCore {


// ----- TTF exceptions -----
struct FontException : public std::exception {
	std::string msg;

	FontException ();
	FontException (const char* msg);
	FontException (const std::string& msg);
	virtual const char* what() const throw();
	};

struct FileFailure : public FontException { 
	FileFailure (const std::string& flnm);
	};

struct FileLengthError : public FontException { 
	FileLengthError ();
	FileLengthError (const std::string& flnm);
	};

struct TableDoesNotExist : public FontException {
	TableDoesNotExist (const std::string& table);
	};

struct ChecksumException : public FontException { 
	ChecksumException (const std::string& table);
	};

struct VersionException : public FontException { 
	VersionException (const std::string& msg);
	};

struct InvalidFontException : public FontException { 
	InvalidFontException (const std::string& msg);
	};

struct UnsupportedCap : public FontException { 
	UnsupportedCap (const std::string& msg);
	};


// ----- end of TTFCore namespace -----
}



// ---------------------------------------------------------------------------------------------------------------------------
//	Simple math 'library'
//		- a small snippet from a larger math library
//		- here in case I want to share TTF.h/TTF.c without needing to include everything
//
//		implemented in: TTF.cpp
// ---------------------------------------------------------------------------------------------------------------------------

// ----- TTFCore namespace -----
namespace TTFCore {


// ----- simple typedefs -----
typedef unsigned short ushort;
typedef unsigned long ulong;


// ----- vec2f -----
struct vec2f {
	float x, y;

	vec2f ();
	vec2f (float x, float y);

	bool operator== (vec2f) const;
	bool operator!= (vec2f) const;

	vec2f& operator+= (vec2f);
	vec2f& operator-= (vec2f);
	vec2f& operator*= (vec2f);
	vec2f& operator/= (vec2f);

	vec2f& operator+= (float);
	vec2f& operator-= (float);
	vec2f& operator*= (float);
	vec2f& operator/= (float);
	};

vec2f operator+ (vec2f, vec2f);
vec2f operator- (vec2f, vec2f);
vec2f operator* (vec2f, vec2f);
vec2f operator/ (vec2f, vec2f);

vec2f operator+ (float, vec2f);
vec2f operator- (float, vec2f);
vec2f operator* (float, vec2f);
vec2f operator/ (float, vec2f);

vec2f operator+ (vec2f, float);
vec2f operator- (vec2f, float);
vec2f operator* (vec2f, float);
vec2f operator/ (vec2f, float);

float dot (vec2f, vec2f);				// 2d dot product
float cross (vec2f, vec2f);				// 2d cross product


// ----- matrix3x2f -----
struct matrix3x2f {
	float a, b, c, d, e, f;
	matrix3x2f ();		// intiailized to the identity
	};

vec2f mul (vec2f, const matrix3x2f&);
vec2f normal (vec2f);


// ----- vec4f -----
struct vec4f {
	float x, y, z, w;

	vec4f ();
	vec4f (float x, float y, float z, float w);
	};


// ----- end of TTFCore namespace -----
}



// ---------------------------------------------------------------------------------------------------------------------------
//	Triangulator
//		- used to triangulate the glyph contours
//		- just made more sense/cleaner code to make a separate class
//
//		implemented in: TTF.cpp
// ---------------------------------------------------------------------------------------------------------------------------

// ----- TTFCore namespace -----
namespace TTFCore {


// ----- Triangulator internal types -----
struct ContourPoint {
	vec2f pos;
	bool endPoint;
	unsigned char flags;

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
	size_t v0,v1, v2;						// vertices
	char coef;									// shader coeficient (-1 = CW edge, 0 = inner tri, 1 = CCW edge)
	Triangle (size_t v0, size_t v1, size_t v2, char coef);
	};

struct Edge {
	size_t v0, v1;
	size_t pe, ne;							// previous edge offset, next edge offset (forms a linked list)
	Edge (size_t v0, size_t v1);		// create from (v0, v1)
	};

struct LineSegment {
	size_t v0, v1;
	float length;
	LineSegment (size_t v0, size_t v1, float length);
	};

struct Bound {
	vec2f p, d;								// point, direction vector (pre-computed for faster intersection tests)
	Bound (vec2f p, vec2f d);
	};

struct TriEdge {
	size_t v0, v1;							// vertex indices
	bool inUse;								// for triangle construction (bounds start as true and become false when used in creating a tri)
	TriEdge (size_t v0, size_t v1);
	};

struct MeshVertex {
	vec2f pos;
	signed char texCoord;		// 0 = (0,0), 1 = (0.5,0), 2 = (1,1)
	signed char coef;				// -1 = CW edge, 0 = inner segment, +1 = CCW segment
	};

struct Mesh {
	std::vector<vec2f> contour_verts;					// vertex cache
	std::vector<MeshVertex> verts;		// vertices
	void Clear ();
	std::vector<MeshVertex>::iterator begin ();
	std::vector<MeshVertex>::const_iterator begin () const;
	std::vector<MeshVertex>::iterator end ();
	std::vector<MeshVertex>::const_iterator end () const;
};


// ----- Triangulator -----
class Triangulator {
 
	public:
		// internal types
		typedef std::vector<ContourPoint> ContourData;						// contour data
		typedef std::vector<ContourPoint>::const_iterator CItr;			// contour iterator

		// internal cache data
		std::vector<vec2f> verts;					// vertex cache
		std::vector<Edge> edges;					// edge cache
		std::vector<Triangle> tris;					// triangle cache
		std::vector<LineSegment> seg;			// inner segments
		std::vector<Bound> bounds;				// bounds used for checking edge intersections
		std::vector<TriEdge> triEdges;				// final valid triangle edges

		// internal helper functions
		void ClearCache ();								// clears all cache data, doesn't deallocate data

		// math helpers
		float GetTriSign (vec2f p0, vec2f p1, vec2f p2) const;							// returns the the winding of the triangle (+result = CCW rotation = outer, -result = CW rotation = inner)
		float GetTriSign (const std::vector<vec2f>&) const;								// returns the sign of the angle made by the triangle consisting of the last 3 vertices (+result = CCW rotation = outer, -result = CW rotation = inner)
		bool IsContained (vec2f p0, vec2f p1, vec2f p2, vec2f p3) const;			// returns true if the vector p0->p3 is between the vectors p0->p1 and p0->p2
		bool Intersect (vec2f p0, vec2f p1, vec2f p2, vec2f p3) const;				// returns true if the 2d lines (p1, p0) and (p3,p2) intersect
		bool Intersect2 (vec2f p, vec2f r, vec2f q, vec2f s) const;						// returns true if the 2d lines (p + t*r) and (q + u*s) intersect
		bool Intersect (float p0, float p1, float p2, float p3) const;						// returns true if the 1d lines (p1, p0) and (p3,p2) intersect/overlap

		// triangulation functions
		void CreateTris ();																					// creates tris from the edges/line segments
		void CreateTrisDebug ();																		// converts edges/line segments to tris
		void TriangulateEdges ();																		// adds triangulation line segments to the edge data
		void TraceContourHelper (CItr i, vec2f& pp, vec2f& ip, bool& ppc);			// TraceContour helper
		void TraceContour (CItr begin, CItr end);												// creates edge data from contours
		void TraceContourInter (CItr begin, CItr end);										// intermediary trace contour function (handles the rare case that begin doesn't start on curve)

		Mesh GetTriangulation (CItr begin, CItr end);
	};


// ----- end of namespace TTFCore -----
}



// ---------------------------------------------------------------------------------------------------------------------------
//	Font
//		- starting point for font rendering
//		- glyphs are triangulated 'on the fly' as required and cached (can pre load the cache if desired)
//		- triagulation took an average of 1.4 milliseconds / glyph for Times New Roman glyphs on my AMD PII 3.3ghz 
//
//		implemented in: TTF.cpp
// ---------------------------------------------------------------------------------------------------------------------------

// ----- TTFCore namespace -----
namespace TTFCore {

	
// ----- Font internal types -----
struct TTFHeader {
	float version;							// sfnt version number (MS) and/or scaling tag (Apple)
	ushort numTables;				// number of table entries
	ushort searchRange;				// used for binary searches of the table entries (not necessary)
	ushort entrySelector;			// used for binary searches of the table entries (not necessary)
	ushort rangeShift;					// used for binary searches of the table entries (not necessary)
	};

struct TableEntry {
	ulong tag;							// 'short' tag
	char tagstr[5];					// readable tag
	ulong checkSum;
	const char* begin;
	const char* end;

	TableEntry ();
	bool IsValid () const;
	};

struct HeadTable {
	float tableVersion;							// currently just 1.0
	float fontRevision;							// information purposes only
	ulong checkSumAdjustment;
	ulong magicNumber;						// set to 0x5F0F3CF5
	ushort flags;
	ushort unitsPerEm;
	long long createdDate;					// seconds since 12:00 midnight, January 1, 1904
	long long modifiedData;					// seconds since 12:00 midnight, January 1, 1904
	short xMin;									// glyph bounding box min x
	short yMin;									// glyph bounding box min y
	short xMax;									// glyph bounding box max x
	short yMax;									// glyph bounding box max y
	ushort macStyle;							// flags (bold, underline, ect...)
	ushort lowestRecPPEM;					// smallest size this font is legible
	short fontDirectionHintl;
	short indexToLocFormat;				// 0 for short offsets, 1 for long
	short glyphDataFormat;				// 0 for current format
	};

struct FontMetrics {
	float ascent, descent;			// distance in 'font units' of the glyphs will go from y = 0 (baseline)
	float lineGap;							// distance in 'font units' between lines
	vec2f caretSlope;					// caret slope, vector in 'font units'
	float caretOffset;					// caret offset in font units
	float minLeftSideBearing;		// minimum (smallest) left side bearing
	float minRightSideBearing;		// minimum right side bearing (why not the max? not sure, maybe this is for right to left text?)
	float advanceWidthMax;		// maximum advanceWidth
	};

struct VFontMetrics {
	bool hasVerticalFontMetrics;								// set if this font has vertical font metrics (otherwise the rest of this struct is undefined)
	float vertTypoDescender, vertTypoAscender;		// left and right 'margins' or glyphs edges for vertical text (similar to ascent/descent)
	float vertTypoLineGap;										// distance between vertical 'lines' (similar to lineGap)
	vec2f caretSlope;												// caret slope, vector in 'font units'
	float caretOffset;												// caret offset
	float minTopSideBearing;									// similar to left side bearing for horizontal text
	float minBottomSideBearing;								// similar to right side bearing
	float advanceHeightMax;									// maximum advanceHeight
	};

struct GlyphMetrics {
	float leftSideBearing;			// farthest left point in font units, left 'anchor', starting point of the glyph
	float advanceWidth;			// width of the glyph (distance to advance to the next glyph's lsb) in 'font units'
	};

struct VGlyphMetrics {
	bool hasVerticalFontMetrics;
	float topSideBearing;					// top 'anchor', starting point of the glyph
	float advanceHeight;					// height of the glyph (distance to advance to the next glyph's tsb) in 'font units'
	};

struct GlyphProfile {
	float version;									// 0.5 = only numGlyphs valid, 1.0 = everything else valid
	ushort numGlyphs;						// The number of glyphs in the font.
	ushort maxPoints;							// Maximum points in a non-composite glyph.
	ushort maxContours;					// Maximum contours in a non-composite glyph.
	ushort maxCompositePoints;			// Maximum points in a composite glyph.
	ushort maxCompositeContours;	// Maximum contours in a composite glyph.
	ushort maxZones;							// 1 if instructions do not use the twilight zone (Z0), or 2 if instructions do use Z0; should be set to 2 in most cases.
	ushort maxTwilightPoints;				// Maximum points used in Z0.
	ushort maxStorage;						// Number of Storage Area locations.
	ushort maxFunctionDefs;				// Number of FDEFs.
	ushort maxInstructionDefs;			// Number of IDEFs.
	ushort maxStackElements;			// Maximum stack depth2.
	ushort maxSizeOfInstructions;		// Maximum byte count for glyph instructions.
	ushort maxComponentElements;	// Maximum number of components referenced at “top level” for any composite glyph.
	ushort maxComponentDepth;		// Maximum levels of recursion; 1 for simple components.
	};

struct CodePoint {
	ulong codeId;
	ushort platformId;
	ushort encodingId;
	ushort languageId;

	CodePoint (ulong codeId);
	CodePoint (ulong codeId, ushort platformId, ushort encodingId, ushort languageId);
	};

struct MapFromData {};


// ----- Font -----
class Font {
	
	private:
		// internal types
		typedef std::map<ulong,TableEntry> TableMap;						// table mapping type
		typedef std::map<ushort,Mesh> MeshCache;							// glyph cache data
		typedef const char* FItr;															// Font data 'iterator' (can't be void*, as we need to be able to increment and decrement with it)
		typedef std::pair<FItr,FItr> FRange;											// Font data range
		typedef std::vector<ContourPoint> ContourData;						// contour data
		typedef std::vector<ContourPoint>::const_iterator CItr;			// contour iterator

		// font data
		std::vector<char> bufferCache;				// stores the raw font data in the case of locally stored data
		TableMap tableMap;									// maps tag -> raw table data
		MeshCache meshCache;							// glyph mesh cache
		Triangulator trianglulator;							// triangulation data structures/functions
		const char* buffer;									// ptr to raw font data

		// read helpers
		// automatically advances the iterator
		unsigned char ReadBYTE (FItr&) const;
		signed char ReadCHAR (FItr&) const;
		unsigned short ReadUSHORT (FItr&) const;
		signed short ReadSHORT (FItr&) const;
		unsigned long ReadUINT24 (FItr&) const;
		unsigned long ReadULONG (FItr&) const;
		signed long ReadLONG (FItr&) const;
		float ReadFIXED32 (FItr&) const;						// reads a 16.16 signed value and converts it to a float
		long long ReadLONGDATETIME (FItr&) const;
		float ReadFIXED16 (FItr&) const;						// reads a 2.14 signed value and converts it to a float

		// read helpers
		// doesn't advance the iterator (used like ReadBYTE(itr + 6))
		unsigned char ReadBYTE (FItr&&) const;
		signed char ReadCHAR (FItr&&) const;
		unsigned short ReadUSHORT (FItr&&) const;
		signed short ReadSHORT (FItr&&) const;
		unsigned long ReadUINT24 (FItr&&) const;
		unsigned long ReadULONG (FItr&&) const;
		signed long ReadLONG (FItr&&) const;
		float ReadFIXED32 (FItr&&) const;						// reads a 16.16 signed value and converts it to a float
		long long ReadLONGDATETIME (FItr&&) const;
		float ReadFIXED16 (FItr&&) const;						// reads a 2.14 signed value and converts it to a float

		// more read helpers
		TTFHeader ReadTTFHeader (FItr&) const;
		TableEntry ReadTableEntry (FItr&) const;
		HeadTable ReadHeadTable () const;
		GlyphProfile ReadMAXPTable () const;
		short GetIndexToLocFormat () const;
		ushort GetNumGlyphs () const;

		// table helpers
		ulong CreateTag (char, char, char, char) const;
		ulong CreateTag (char*) const;
		void DecomposeTag (ulong, char*) const;											// decomposes the tag into a readable string (must be at least 5 characters long)
		std::string DecomposeTag (ulong) const;											// decomposes the tag into a readable string
		TableEntry GetTableEntry (ulong) const;											// returns the table entry for a tag
		bool VerifyTableCheckSum (const TableEntry&) const;						// returns true if the table checksum is valid, false otherwise
		bool VerifyNormalCheckSum (const TableEntry&) const;					// verifys checksum for all tables but the 'head' table
		bool VerifyHeadCheckSum (const TableEntry&) const;						// verifys checksum for the 'head' table

		// initial loading functions
		void CreateTableMap ();								// creates table map entries from font data
		void VerifyTableCheckSums () const;				// verifys all the table checksums, throws an exception on error
		void VerifyRequiredTables () const;				// throws if the required tables are not available
		void VerifyTrueTypeTables () const;				// throws if the required tables for true type outlines are not available

		// CodePoint mapping
		FRange GetGlyphData (CodePoint) const;											// returns the glyph data for the given code point (called from AddGlyphStrokeData())
		ushort GetGlyphIndex (CodePoint) const;											// translates a code point to its glyph index
		FRange MapGlyphIndexToRange (ushort glyphIndex) const;				// maps the glyph index to its glyph data
		ushort GetGlyphIndexF0 (FItr, ushort langId, ulong codeId) const;
		ushort GetGlyphIndexF2 (FItr, ushort langId, ulong codeId) const;
		ushort GetGlyphIndexF4 (FItr, ushort langId, ulong codeId) const;
		ushort GetGlyphIndexF6 (FItr, ushort langId, ulong codeId) const;
		ushort GetGlyphIndexF8 (FItr, ushort langId, ulong codeId) const;
		ushort GetGlyphIndexF10 (FItr, ushort langId, ulong codeId) const;
		ushort GetGlyphIndexF12 (FItr, ushort langId, ulong codeId) const;
		ushort GetGlyphIndexF13 (FItr, ushort langId, ulong codeId) const;
		ushort GetGlyphIndexF14 (FItr, ushort langId, ulong codeId) const;

		// metrics helpers
		GlyphMetrics GetGlyphMetrics (ushort glyphIndex) const;																				// returns glyph metrics (specified by glyphIndex)
		VGlyphMetrics GetVGlyphMetrics (ushort glyphIndex) const;																			// returns vertical glyph metrics (specified by glyphIndex)
		vec2f GetKerning (CodePoint, CodePoint, bool horizontal) const;																	// returns the kerning value for the two glyphs (either horizontal or vertical kerning)
		float ParseKernTableF0 (FItr, ushort g0, ushort g1) const;																				// parses a format 0 kern table
		float ParseKernTableF2 (FItr, ushort g0, ushort g1) const;																				// parses a format 2 kern table
		ushort ParseMSKernTable (FItr, ushort glyphIndex0, ushort glyphIndex1, bool horizontal, vec2f&) const;			// parses a microsoft version'd kern table, returns the length of the sub table in bytes
		ulong ParseAppleKernTable (FItr, ushort glyphIndex0, ushort glyphIndex1, bool horizontal, vec2f&) const;		// parses a microsoft version'd kern table, returns the length of the sub table in bytes

		// triangulation functions
		Mesh GetSimpleMesh (FItr);									// gets the simple glyph's mesh
		Mesh GetComplexMesh (FItr);								// gets the complex glyph's mesh
		Mesh GetGlyphMesh (ushort glyphIndex);				// gets the glyph's mesh

	public:
		Font (std::string flnm);											// construct from file
		Font (const void* rawData, MapFromData);			// map from raw data (no copy made, data must exist for the duration of the Font object)
		Font (const void* rawData, size_t length);			// copy from raw data
		Font (const Font&);
		Font (Font&&);
		Font& operator= (const Font&);
		Font& operator= (Font&&);
		~Font ();

		// font info
		vec4f GetMasterRect () const;											// returns a vec4f in font units that encloses every glyph, format is (xMin,yMin,xMax,yMax)
		vec4f GetGlyphRect (CodePoint) const;								// returns a vec4f in font units that encloses the glyph, format is (xMin,yMin,xMax,yMax)
		FontMetrics GetFontMetrics () const;									// returns the horizontal font metrics
		GlyphMetrics GetGlyphMetrics (CodePoint) const;				// returns the specific glyph horizontal metrics
		VFontMetrics GetVFontMetrics () const;								// returns the vertical font metrics (if they exists)
		VGlyphMetrics GetVGlyphMetrics (CodePoint) const;			// returns the specific glyph vertical metrics (if they exists)
		vec2f GetKerning (CodePoint, CodePoint) const;				// returns horizontal kerning value for the pair of glyphs (offset in both x and y directions moving from g1 to g2)
		vec2f GetVKerning (CodePoint, CodePoint) const;				// returns vertical kerning value for the pair of glyphs (offset in both x and y directions moving from g1 to g2)

		// glyph triangulation
		const Mesh& GetTriangulation (CodePoint);																											// returns a triangulated mesh for the given glyphIndex
		size_t GetTriCount (CodePoint);																															// returns the number of tris in the triangulated mesh for the given code point
		void ClearGlyphCache ();																																		// clear the glyph triangulation cache (all mesh data is lost, any references to it are invalidated)
		void PreCache (ulong codeStart, ulong codeEnd);																									// ensure the glyphs corresponding to the codepoints (inclusive range) are triangulated and in the cache
		void PreCache (ulong codeStart, ulong codeEnd, ushort platformId, ushort encodingId, ushort languageId);				// ensure the glyphs corresponding to the codepoints (inclusive range) are triangulated and in the cache

		// helpers
		void PreCacheBasicLatin ();					// pre cache all the standard basic latin glyphs
	};


// ----- end of TTFCore namespace -----
}



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


// ----- types -----
using TTFCore::Font;
using TTFCore::CodePoint;
using TTFCore::GlyphMetrics;
using TTFCore::FontMetrics;
using TTFCore::VGlyphMetrics;
using TTFCore::VFontMetrics;
using TTFCore::MeshVertex;
using TTFCore::Mesh;
using TTFCore::MapFromData;
using TTFCore::vec2f;
using TTFCore::vec4f;

const TTFCore::MapFromData mapFromData;


// ----- end of TTF namespace -----
}



// ---------------------------------------------------------------------------------------------------------------------------
//	End
// ---------------------------------------------------------------------------------------------------------------------------

}	// Utility namespace

#endif
