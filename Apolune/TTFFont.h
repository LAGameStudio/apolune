// *************************************************************************************
//	TTFFont.h
//
//		Defines Font, the main class for TTF font handling.
//
//																																		Ryan Boghean
//																																		April 2012
//																																		January 2014
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
//	Font internal types
//
//		implemented in: TTFFont.cpp
// ---------------------------------------------------------------------------------------------------------------------------
	
// ----- Font internal types -----
struct TTFHeader {
	int32_t version;							// sfnt version number (MS) and/or scaling tag (Apple)
	uint16_t num_tables;					// number of table entries
	uint16_t search_range;					// used for binary searches of the table entries (not necessary)
	uint16_t entry_selector;				// used for binary searches of the table entries (not necessary)
	uint16_t range_shift;						// used for binary searches of the table entries (not necessary)
	};

struct TableEntry {
	uint32_t tag;							// 'short' tag
	uint32_t check_sum;
	char tagstr[5];						// readable tag
	const char* begin;
	const char* end;

	TableEntry ();
	bool IsValid () const;
	};

struct HeadTable {
	int32_t table_version;							// currently just 1.0
	int32_t font_revision;							// information purposes only
	uint32_t check_sum_adjustment;
	uint32_t magic_number;						// set to 0x5F0F3CF5
	uint16_t flags;
	uint16_t units_per_em;
	int64_t created_date;					// seconds since 12:00 midnight, January 1, 1904
	int64_t modified_date;					// seconds since 12:00 midnight, January 1, 1904
	uint16_t xmin;								// glyph bounding box min x
	uint16_t ymin;								// glyph bounding box min y
	uint16_t xmax;								// glyph bounding box max x
	uint16_t ymax;								// glyph bounding box max y
	uint16_t mac_style;						// flags (bold, underline, ect...)
	uint16_t lowest_rec_PPEM;			// smallest size this font is legible in pixels per EM (not sure exactly what EM is)
	int16_t font_direction_hint;
	int16_t index_to_loc_format;		// 0 for short offsets, 1 for long
	int16_t glyph_data_format;			// 0 for current format
	};

struct FontMetrics {
	int16_t ascent, descent;					// distance in 'font units' of the glyphs will go from y = 0 (baseline)
	int16_t line_gap;								// distance in 'font units' between lines
	vec2s caret_slope;							// caret slope, vector in 'font units'
	int16_t caret_offset;							// caret offset in font units
	int16_t min_left_side_bearing;			// minimum (smallest) left side bearing
	int16_t min_right_side_bearing;			// minimum right side bearing (why not the max? not sure, maybe this is for right to left text?)
	int16_t advance_width_max;			// maximum advanceWidth
	};

struct VFontMetrics {
	bool has_vertical_font_metrics;										// set if this font has vertical font metrics (otherwise the rest of this struct is undefined)
	int16_t vert_typo_descender, vert_typo_ascender;		// left and right 'margins' or glyphs edges for vertical text (similar to ascent/descent)
	int16_t vert_typo_line_gap;											// distance between vertical 'lines' (similar to lineGap)
	vec2s caret_slope;														// caret slope, vector in 'font units'
	int16_t caret_offset;														// caret offset
	int16_t min_top_side_bearing;										// similar to left side bearing for horizontal text
	int16_t min_bottom_side_bearing;								// similar to right side bearing
	int16_t advance_height_max;										// maximum advanceHeight
	};

struct GlyphMetrics {
	int16_t left_side_bearing;			// farthest left point in font units, left 'anchor', starting point of the glyph
	uint16_t advance_width;			// width of the glyph (distance to advance to the next glyph's lsb) in 'font units' (yes I know this is uint16_t where-as everything else is int16_t... this format is stupid)
	};

struct VGlyphMetrics {
	bool has_vertical_font_metrics;
	int16_t top_side_bearing;					// top 'anchor', starting point of the glyph
	uint16_t advance_height;					// height of the glyph (distance to advance to the next glyph's tsb) in 'font units'
	};

struct GlyphProfile {
	uint32_t version;									// 0x8000 = only numGlyphs valid, 0x10000 = everything else valid
	uint16_t num_glyphs;							// The number of glyphs in the font.
	uint16_t max_points;							// Maximum points in a non-composite glyph.
	uint16_t max_contours;						// Maximum contours in a non-composite glyph.
	uint16_t max_composite_points;			// Maximum points in a composite glyph.
	uint16_t max_composite_contours;		// Maximum contours in a composite glyph.
	uint16_t max_zones;							// 1 if instructions do not use the twilight zone (Z0), or 2 if instructions do use Z0; should be set to 2 in most cases.
	uint16_t max_twilight_points;				// Maximum points used in Z0.
	uint16_t max_storage;							// Number of Storage Area locations.
	uint16_t max_function_defs;					// Number of FDEFs.
	uint16_t max_instruction_defs;				// Number of IDEFs.
	uint16_t max_stack_elements;				// Maximum stack depth2.
	uint16_t max_size_of_instructions;		// Maximum byte count for glyph instructions.
	uint16_t max_component_elements;	// Maximum number of components referenced at “top level” for any composite glyph.
	uint16_t max_component_depth;			// Maximum levels of recursion; 1 for simple components.
	};

struct CodePoint {
	uint32_t code;
	uint16_t platform;
	uint16_t encoding;
	uint16_t language;

	CodePoint (uint32_t code);
	CodePoint (uint32_t code, uint16_t platform, uint16_t encoding, uint16_t language);
	};

struct MapFromData {};



// ---------------------------------------------------------------------------------------------------------------------------
//	Font
//		- starting point for font rendering
//		- font info will return generic font info
//		- glyph info returns information on a specific glyph
//		- kerning info returns information on glyph spacing
//		- glyph triangulation creates the actual mesh for a given glyph
//
//		implemented in: TTFFont.cpp
// ---------------------------------------------------------------------------------------------------------------------------

// ----- Font -----
class Font {
	
	private:
		// internal types
		typedef std::map<uint32_t,TableEntry> TableMap;					// table mapping type
		typedef const char* FItr;															// Font data 'iterator' (can't be void*, as we need to be able to increment and decrement with it)
		typedef std::pair<FItr,FItr> FRange;											// Font data range
		typedef std::vector<ContourPoint> ContourData;						// contour data
		typedef std::vector<ContourPoint>::const_iterator CItr;			// contour iterator

		// font data
		TableMap table_map;								// maps tag -> raw table data
		std::vector<char> buffer_cache;				// stores the raw font data in the case of locally stored data
		const char* buffer;									// ptr to raw font data

		// read helpers
		// automatically advances the iterator
		uint8_t ReadBYTE (FItr&) const;
		int8_t ReadCHAR (FItr&) const;
		uint16_t ReadUSHORT (FItr&) const;
		int16_t ReadSHORT (FItr&) const;
		uint32_t ReadUINT24 (FItr&) const;
		uint32_t ReadULONG (FItr&) const;
		int32_t ReadLONG (FItr&) const;
		int32_t ReadFIXED32 (FItr&) const;						// reads a 16.16 signed value
		int64_t ReadLONGDATETIME (FItr&) const;
		int32_t ReadFIXED16 (FItr&) const;						// reads a 2.14 signed value and converts it to a 16.16

		// read helpers
		// doesn't advance the iterator (used like ReadBYTE(itr + 6))
		uint8_t ReadBYTE (FItr&&) const;
		int8_t ReadCHAR (FItr&&) const;
		uint16_t ReadUSHORT (FItr&&) const;
		int16_t ReadSHORT (FItr&&) const;
		uint32_t ReadUINT24 (FItr&&) const;
		uint32_t ReadULONG (FItr&&) const;
		int32_t ReadLONG (FItr&&) const;
		int32_t ReadFIXED32 (FItr&&) const;						// reads a 16.16 signed value
		int64_t ReadLONGDATETIME (FItr&&) const;
		int32_t ReadFIXED16 (FItr&&) const;						// reads a 2.14 signed value converts it to a 16.16

		// more read helpers
		TTFHeader ReadTTFHeader (FItr&) const;
		TableEntry ReadTableEntry (FItr&) const;
		HeadTable ReadHeadTable () const;
		GlyphProfile ReadMAXPTable () const;
		int16_t GetIndexToLocFormat () const;
		uint16_t GetNumGlyphs () const;

		// table helpers
		uint32_t CreateTag (char, char, char, char) const;
		uint32_t CreateTag (const char*) const;
		void DecomposeTag (uint32_t, char*) const;										// decomposes the tag into a readable string (must be at least 5 characters long)
		std::string DecomposeTag (uint32_t) const;										// decomposes the tag into a readable string
		TableEntry GetTableEntry (uint32_t) const;										// returns the table entry for a tag
		bool VerifyTableCheckSum (const TableEntry&) const;						// returns true if the table checksum is valid, false otherwise
		bool VerifyNormalCheckSum (const TableEntry&) const;					// verifys checksum for all tables but the 'head' table
		bool VerifyHeadCheckSum (const TableEntry&) const;						// verifys checksum for the 'head' table

		// initial loading functions
		void CreateTableMap ();								// creates table map entries from font data
		void VerifyTableCheckSums () const;				// verifys all the table checksums, throws an exception on error
		void VerifyRequiredTables () const;				// throws if the required tables are not available
		void VerifyTrueTypeTables () const;				// throws if the required tables for true type outlines are not available

		// CodePoint to glyph index mappings
		uint16_t GetGlyphIndexF0 (FItr, uint16_t language, uint32_t code) const;
		uint16_t GetGlyphIndexF2 (FItr, uint16_t language, uint32_t code) const;
		uint16_t GetGlyphIndexF4 (FItr, uint16_t language, uint32_t code) const;
		uint16_t GetGlyphIndexF6 (FItr, uint16_t language, uint32_t code) const;
		uint16_t GetGlyphIndexF8 (FItr, uint16_t language, uint32_t code) const;
		uint16_t GetGlyphIndexF10 (FItr, uint16_t language, uint32_t code) const;
		uint16_t GetGlyphIndexF12 (FItr, uint16_t language, uint32_t code) const;
		uint16_t GetGlyphIndexF13 (FItr, uint16_t language, uint32_t code) const;
		uint16_t GetGlyphIndexF14 (FItr, uint16_t language, uint32_t code) const;

		// data range mappings
		FRange GetGlyphRange (uint16_t glyph_index) const;			// returns the glyph data for the given code point (called from AddGlyphStrokeData())

		// metrics helpers
		vec2s GetKerning (uint16_t g0, uint16_t g1, bool horizontal) const;																				// returns the kerning value for the two glyphs (either horizontal or vertical kerning)
		int16_t ParseKernTableF0 (FItr, uint16_t g0, uint16_t g1) const;																						// parses a format 0 kern table
		int16_t ParseKernTableF2 (FItr, uint16_t g0, uint16_t g1) const;																						// parses a format 2 kern table
		uint16_t ParseMSKernTable (FItr, uint16_t glyph_index0, uint16_t glyph_index1, bool horizontal, vec2s&) const;				// parses a microsoft version'd kern table, returns the length of the sub table in bytes
		uint32_t ParseAppleKernTable (FItr, uint16_t glyph_index0, uint16_t glyph_index1, bool horizontal, vec2s&) const;			// parses an apple version'd kern table, returns the length of the sub table in bytes

		// triangulation functions
		void FillContours (FItr, Triangulator&) const;																	// used for simple glyphs, fills the contours cache
		bool ReadGlyphFlags (FItr&, matrix3x2s& matrix, uint16_t& glyph_index) const;				// used for complex glyphs, reads the transformation matrix and glyph index, returns true if there are more transforms
		void AppendSimpleMesh (FItr, Triangulator&, MeshSmall&) const;									// triangulates the data in the contours cache, appends to the mesh
		void AppendSimpleMesh (FItr, Triangulator&, MeshLarge&) const;									// triangulates the data in the contours cache, appends to the mesh
		void AppendComplexMesh (FItr, Triangulator&, MeshSmall&) const;								// appends the complex glyph to the mesh
		void AppendComplexMesh (FItr, Triangulator&, MeshLarge&) const;								// appends the complex glyph to the mesh
		void AppendGlyphMesh(uint16_t glyph_index, Triangulator&, MeshSmall&) const;			// entry point for MeshSmall triangulation
		void AppendGlyphMesh(uint16_t glyph_index, Triangulator&, MeshLarge&) const;			// entry point for MeshLarge triangulation

	public:
		explicit Font (std::string file_name);						// construct from file
		Font (const void* raw_data, MapFromData);			// map from raw data (no copy made, data must exist for the duration of the Font object)
		Font (const void* raw_data, size_t length);			// copy from raw data
		Font (const Font&);
		Font (Font&&);
		Font& operator= (const Font&);
		Font& operator= (Font&&);
		~Font ();

		// font info
		vec4s GetMasterRect () const;														// returns a vec4f in font units that encloses every glyph, format is (xMin,yMin,xMax,yMax)
		FontMetrics GetFontMetrics () const;												// returns the horizontal font metrics
		VFontMetrics GetVFontMetrics () const;											// returns the vertical font metrics (if they exists)
		uint16_t GlyphCount () const;														// returns the total number of glyphs in this font

		// glyph info
		uint16_t GetGlyphIndex (CodePoint) const;									// translates a code point to its glyph index
		vec4s GetGlyphRect (uint16_t glyph_index) const;							// returns a vec4s in font units that encloses the glyph, format is (xMin,yMin,xMax,yMax)
		vec4s GetGlyphRect (CodePoint) const;											// returns a vec4s in font units that encloses the glyph, format is (xMin,yMin,xMax,yMax)
		GlyphMetrics GetGlyphMetrics (uint16_t glyph_index) const;			// returns the specific glyph horizontal metrics
		GlyphMetrics GetGlyphMetrics (CodePoint) const;							// returns the specific glyph horizontal metrics
		VGlyphMetrics GetVGlyphMetrics (uint16_t glyph_index) const;		// returns vertical glyph metrics (specified by glyphIndex)
		VGlyphMetrics GetVGlyphMetrics (CodePoint) const;						// returns the specific glyph vertical metrics (if they exists)

		// kerning info
		vec2s GetKerning (uint16_t g0, uint16_t g1) const;						// returns horizontal kerning value for the pair of glyphs (offset in both x and y directions moving from g1 to g2)
		vec2s GetKerning (CodePoint, CodePoint) const;							// returns horizontal kerning value for the pair of glyphs (offset in both x and y directions moving from g1 to g2)
		vec2s GetVKerning (uint16_t g0, uint16_t g1) const;						// returns vertical kerning value for the pair of glyphs (offset in both x and y directions moving from g1 to g2)
		vec2s GetVKerning (CodePoint, CodePoint) const;							// returns vertical kerning value for the pair of glyphs (offset in both x and y directions moving from g1 to g2)

		// glyph triangulation
		void GetGlyph (uint16_t glyph_index, Triangulator&, MeshSmall&) const;
		void GetGlyph (CodePoint, Triangulator&, MeshSmall&) const;
		void GetGlyph (uint16_t glyph_index, Triangulator&, MeshLarge&) const;
		void GetGlyph (CodePoint, Triangulator&, MeshLarge&) const;
	};



// ---------------------------------------------------------------------------------------------------------------------------
//	End
// ---------------------------------------------------------------------------------------------------------------------------

}			// end of TTFCore namespace
