// *************************************************************************************
//	TTFFont.cpp
//
//		True type (*.ttf) and OpenType (*.otf) font loading/rendering implementation.
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
#include <iterator>


#include "TTFExceptions.h"
#include "TTFMath.h"
#include "TTFTriangulator.h"
#include "TTFFont.h"


using namespace TTFCore;



// ---------------------------------------------------------------------------------------------------------------------------
//	Font internal types
// ---------------------------------------------------------------------------------------------------------------------------

// ----- TableEntry -----
TableEntry::TableEntry () {
	tag = 0;
	tagstr[0] = 0;
	check_sum = 0;
	begin = 0;
	end =0;
	}
	
bool TableEntry::IsValid () const {
	return tag != 0;
	}


// ----- CodePoint -----
CodePoint::CodePoint (uint32_t code_) : code(code_), platform(3), encoding(1), language(0) {}

CodePoint::CodePoint (uint32_t code_, uint16_t platform_, uint16_t encoding_, uint16_t language_) : 
	code(code_), platform(platform_), encoding(encoding_), language(language_) {}



// ---------------------------------------------------------------------------------------------------------------------------
//	Font
// ---------------------------------------------------------------------------------------------------------------------------

// ----- constructor/destructor -----
Font::Font (std::string file_name) {

	// load font file
	std::fstream file(file_name, std::fstream::in | std::fstream::binary);
	file.seekg(0,std::ios::end);
	size_t length = static_cast<size_t>(file.tellg());
	if (file.fail()) throw FileFailure(file_name);
	if (length == 0) throw FileLengthError(file_name);
	file.seekg(0,std::ios::beg);
	buffer_cache.resize(length,0);
	file.read(&*buffer_cache.begin(),length);
	if (file.fail()) throw FileFailure(file_name);

	// intialize variables
	buffer = &buffer_cache.front();

	// parse font
	CreateTableMap();
	VerifyTableCheckSums();
	VerifyRequiredTables();
	VerifyTrueTypeTables();
	}

Font::Font (const void* raw_data, MapFromData) {

	// intialize variables
	buffer = reinterpret_cast<const char*>(raw_data);

	// parse font
	CreateTableMap();
	VerifyTableCheckSums();
	VerifyRequiredTables();
	VerifyTrueTypeTables();
	}

Font::Font (const void* raw_data, size_t length) {

	// sanity check
	if (length == 0) throw FileLengthError();

	// 'load' font data
	buffer_cache.assign(reinterpret_cast<const char*>(raw_data),reinterpret_cast<const char*>(raw_data) + length);
	buffer = &buffer_cache.front();

	// parse font
	CreateTableMap();
	VerifyTableCheckSums();
	VerifyRequiredTables();
	VerifyTrueTypeTables();
	}

Font::Font (const Font& f) {
	buffer_cache = f.buffer_cache;
	table_map = f.table_map;
	buffer = buffer_cache.empty() ? f.buffer : buffer_cache.data();
	}

Font::Font (Font&& f) {
	buffer_cache = std::move(f.buffer_cache);
	table_map = std::move(f.table_map);
	buffer = buffer_cache.empty() ? f.buffer : buffer_cache.data();
	}

Font& Font::operator= (const Font& f) {
	if (this != &f) {
		buffer_cache = f.buffer_cache;
		table_map = f.table_map;
		buffer = buffer_cache.empty() ? f.buffer : buffer_cache.data();
		}
	return *this;
	}

Font& Font::operator= (Font&& f) {
	if (this != &f) {
		buffer_cache = std::move(f.buffer_cache);
		table_map = std::move(f.table_map);
		buffer = buffer_cache.empty() ? f.buffer : buffer_cache.data();
		}
	return *this;
	}

Font::~Font () {}


// ----- read helpers -----
uint8_t Font::ReadBYTE (FItr& itr) const { 
	uint8_t r = *reinterpret_cast<const uint8_t*>(itr);
	itr += 1;
	return r;
	}

int8_t Font::ReadCHAR (FItr& itr) const {
	int8_t r = *reinterpret_cast<const int8_t*>(itr);
	itr += 1;
	return r;
	}

uint16_t Font::ReadUSHORT (FItr& itr) const {
	uint16_t r = ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	return r;
	}

int16_t Font::ReadSHORT (FItr& itr) const {
	int16_t r = 0;
	uint8_t i = ReadBYTE(itr);
	if (i & 128) r = -1;
	r <<= 8;
	r += i;
	r <<= 8;
	r += ReadBYTE(itr);
	return r;
	}

uint32_t Font::ReadUINT24 (FItr& itr) const { 
	uint32_t r = ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	return r;
	}

uint32_t Font::ReadULONG (FItr& itr) const {
	uint32_t r = ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	r <<= 8;
	r += ReadBYTE(itr);
	return r;
	}

int32_t Font::ReadLONG (FItr& itr) const {
	int32_t r = 0;
	uint8_t i = ReadBYTE(itr);
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

int32_t Font::ReadFIXED32 (FItr& itr) const {
	return ReadLONG(itr);
	}

int64_t Font::ReadLONGDATETIME (FItr& itr) const {
	int64_t r = 0;
	uint8_t i = ReadBYTE(itr);
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

int32_t Font::ReadFIXED16 (FItr& itr) const {
	return static_cast<int32_t>(ReadSHORT(itr) << 2);			// convert from 2.14 to 16.16
	}

uint8_t Font::ReadBYTE (FItr&& itr) const { 
	return *reinterpret_cast<const uint8_t*>(itr);
	}

int8_t Font::ReadCHAR (FItr&& itr) const {
	return *reinterpret_cast<const int8_t*>(itr);
	}

uint16_t Font::ReadUSHORT (FItr&& itr_) const {
	FItr itr = itr_;
	return ReadUSHORT(itr);
	}

int16_t Font::ReadSHORT (FItr&& itr_) const {
	FItr itr = itr_;
	return ReadSHORT(itr);
	}

uint32_t Font::ReadUINT24 (FItr&& itr_) const { 
	FItr itr = itr_;
	return ReadUINT24(itr);
	}

uint32_t Font::ReadULONG (FItr&& itr_) const {
	FItr itr = itr_;
	return ReadULONG(itr);
	}

int32_t Font::ReadLONG (FItr&& itr_) const {
	FItr itr = itr_;
	return ReadLONG(itr);
	}

int32_t Font::ReadFIXED32 (FItr&& itr_) const {
	FItr itr = itr_;
	return ReadLONG(itr);
	}

int64_t Font::ReadLONGDATETIME (FItr&& itr_) const {
	FItr itr = itr_;
	return ReadLONGDATETIME(itr);
	}

int32_t Font::ReadFIXED16 (FItr&& itr_) const {
	FItr itr = itr_;
	return ReadSHORT(itr);
	}


// ----- more read helpers -----
TTFHeader Font::ReadTTFHeader (FItr& itr) const {

	TTFHeader header;
	header.version = ReadFIXED32(itr);
	header.num_tables = ReadUSHORT(itr);
	header.search_range = ReadUSHORT(itr);
	header.entry_selector = ReadUSHORT(itr);
	header.range_shift = ReadUSHORT(itr);

	return header;
	}

TableEntry Font::ReadTableEntry (FItr& itr) const {

	TableEntry te;
	te.tag = ReadULONG(itr);
	DecomposeTag(te.tag,te.tagstr);
	te.check_sum = ReadULONG(itr);
	te.begin = buffer + ReadULONG(itr);
	te.end = te.begin + ReadULONG(itr);

	return te;
	}

HeadTable Font::ReadHeadTable () const {

	FItr itr = GetTableEntry(CreateTag('h','e','a','d')).begin;

	HeadTable ht;
	ht.table_version = ReadFIXED32(itr);
	ht.font_revision = ReadFIXED32(itr);
	ht.check_sum_adjustment = ReadULONG(itr);
	ht.magic_number = ReadULONG(itr);
	ht.flags = ReadUSHORT(itr);
	ht.units_per_em = ReadUSHORT(itr);
	ht.created_date = ReadLONGDATETIME(itr);
	ht.modified_date = ReadLONGDATETIME(itr);
	ht.xmin = ReadSHORT(itr);
	ht.ymin = ReadSHORT(itr);
	ht.xmax = ReadSHORT(itr);
	ht.ymax = ReadSHORT(itr);
	ht.mac_style = ReadUSHORT(itr);
	ht.lowest_rec_PPEM = ReadUSHORT(itr);
	ht.font_direction_hint = ReadSHORT(itr);
	ht.index_to_loc_format = ReadSHORT(itr);
	ht.glyph_data_format = ReadSHORT(itr);

	return ht;
	}

GlyphProfile Font::ReadMAXPTable () const {

	FItr itr = GetTableEntry(CreateTag('m','a','x','p')).begin;

	GlyphProfile gp;
	gp.version = ReadULONG(itr);
	gp.num_glyphs = ReadUSHORT(itr);

	if (gp.version == 1) {
		gp.max_points = ReadUSHORT(itr);	
		gp.max_contours = ReadUSHORT(itr);
		gp.max_composite_points = ReadUSHORT(itr);
		gp.max_composite_contours = ReadUSHORT(itr);
		gp.max_zones = ReadUSHORT(itr);
		gp.max_twilight_points = ReadUSHORT(itr);
		gp.max_storage = ReadUSHORT(itr);
		gp.max_function_defs = ReadUSHORT(itr);
		gp.max_instruction_defs = ReadUSHORT(itr);
		gp.max_stack_elements = ReadUSHORT(itr);
		gp.max_size_of_instructions = ReadUSHORT(itr);
		gp.max_component_elements = ReadUSHORT(itr);
		gp.max_component_depth = ReadUSHORT(itr);
		}

	return gp;
	}

int16_t Font::GetIndexToLocFormat () const {
	FItr itr = GetTableEntry(CreateTag('h','e','a','d')).begin;
	return ReadSHORT(itr + 50);
	}

uint16_t Font::GetNumGlyphs () const {
	FItr itr = GetTableEntry(CreateTag('m','a','x','p')).begin;
	return ReadUSHORT(itr + 4);
	}


// ----- table helpers -----
uint32_t Font::CreateTag (char c0, char c1, char c2, char c3) const {

	uint32_t r;
	r = static_cast<uint32_t>(c0);
	r <<= 8;
	r |= static_cast<uint32_t>(c1);
	r <<= 8;
	r |= static_cast<uint32_t>(c2);
	r <<= 8;
	r |= static_cast<uint32_t>(c3);

	return r;
	}

uint32_t Font::CreateTag (const char* s) const {

	uint32_t r;
	r = static_cast<uint32_t>(s[0]);
	r <<= 8;
	r |= static_cast<uint32_t>(s[1]);
	r <<= 8;
	r |= static_cast<uint32_t>(s[2]);
	r <<= 8;
	r |= static_cast<uint32_t>(s[3]);

	return r;
	}

void Font::DecomposeTag (uint32_t tag, char* s) const {
	s[0] = static_cast<char>((tag >> 24) & 0xff);
	s[1] = static_cast<char>((tag >> 16) & 0xff);
	s[2] = static_cast<char>((tag >> 8) & 0xff);
	s[3] = static_cast<char>(tag & 0xff);
	s[4] = 0;
	}

std::string Font::DecomposeTag (uint32_t tag) const {
	char str[5];
	DecomposeTag(tag,str);
	return str;
	}

TableEntry Font::GetTableEntry (uint32_t tag) const {
	auto i = table_map.find(tag);
	if (i == table_map.end()) throw TableDoesNotExist(DecomposeTag(tag));
	return i->second;
	}

bool Font::VerifyTableCheckSum (const TableEntry& te) const {
	if (te.tag == CreateTag('h','e','a','d'))		return VerifyHeadCheckSum(te);
	else														return VerifyNormalCheckSum(te);
	}

bool Font::VerifyNormalCheckSum (const TableEntry& te) const {
	uint32_t check_sum = 0;
	FItr i = te.begin;
	while (i < te.end) check_sum += ReadULONG(i);
	return check_sum == te.check_sum;
	}

bool Font::VerifyHeadCheckSum (const TableEntry& te) const {
	uint32_t check_sum = 0;
	FItr i = te.begin;
	check_sum += ReadULONG(i);
	check_sum += ReadULONG(i);
	i += 4;																	// check_sum_adjustment member of head table, must be skipped or set to 0 for correct checksum
	while (i < te.end) check_sum += ReadULONG(i);
	return check_sum == te.check_sum;
	}


// ----- intial loading functions -----
void Font::CreateTableMap () {

	// intialize
	FItr itr = buffer;
	table_map.clear();

	// read in table entries
	TTFHeader header = ReadTTFHeader(itr);
	for (uint16_t i = 0; i < header.num_tables; ++i) {
		TableEntry te = ReadTableEntry(itr);
		table_map[te.tag] = te;
		}
	}

void Font::VerifyTableCheckSums () const {
	for (auto i = table_map.begin(); i != table_map.end(); ++i) {
		if (VerifyTableCheckSum(i->second) == false) throw ChecksumException(i->second.tagstr);
		}
	}

void Font::VerifyRequiredTables () const {

	uint32_t required_tables[] = {
		CreateTag('c','m','a','p'),
		CreateTag('h','e','a','d'),
		CreateTag('h','h','e','a'),
		CreateTag('h','m','t','x'),
		CreateTag('m','a','x','p'),
		CreateTag('n','a','m','e'),
		CreateTag('O','S','/','2'),
		CreateTag('p','o','s','t')
		};
	size_t table_length = sizeof(required_tables) / sizeof(uint32_t);

	for (size_t i = 0; i < table_length; ++i) {
		if (table_map.find(required_tables[i]) == table_map.end()) {
			throw TableDoesNotExist(DecomposeTag(required_tables[i]));
			}
		}
	}

void Font::VerifyTrueTypeTables () const {

	uint32_t required_tables[] = {
		CreateTag('c','v','t',' '),
		CreateTag('f','p','g','m'),
		CreateTag('g','l','y','f'),
		CreateTag('l','o','c','a'),
		CreateTag('p','r','e','p'),
		};
	size_t table_length = sizeof(required_tables) / sizeof(uint32_t);

	for (size_t i = 0; i < table_length; ++i) {
		if (table_map.find(required_tables[i]) == table_map.end()) {
			throw TableDoesNotExist(DecomposeTag(required_tables[i]));			
			}
		}
	}


// ----- CodePoint to glyph index mappings -----
uint16_t Font::GetGlyphIndexF0 (FItr itr, uint16_t langid, uint32_t code) const {

	// load header
	uint16_t format = ReadUSHORT(itr);
	uint16_t length = ReadUSHORT(itr);
	uint16_t language = ReadUSHORT(itr);
	if (format != 0) throw FontException("Internal error, calling GetGlyphIndexF0() on a 'cmap' table that isn't format 0.");
	if (language != langid) return 0;

	// map code point
	if (code < 256) return ReadBYTE(itr + code);
	return 0;
	}

uint16_t Font::GetGlyphIndexF2 (FItr itr, uint16_t langid, uint32_t code) const {
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

uint16_t Font::GetGlyphIndexF4 (FItr itr, uint16_t langid, uint32_t code) const {
	// I have code for both linear and binary search
	// tbh, I don't think either choice really matters given how fast it executes

	// load header
	uint16_t format = ReadUSHORT(itr);
	uint16_t length = ReadUSHORT(itr);
	uint16_t language = ReadUSHORT(itr);
	uint16_t seg_count_x2 = ReadUSHORT(itr);
	uint16_t search_range = ReadUSHORT(itr);
	uint16_t entry_selector = ReadUSHORT(itr);
	uint16_t range_shift = ReadUSHORT(itr);
	uint16_t seg_count = seg_count_x2 / 2;		// why not just store seg_count?, this file format is retarded

	if (format != 4) throw FontException("Internal error, calling GetGlyphIndexF4() on a 'cmap' table that isn't format 4.");
	if (language != langid) return 0;

	// map arrays
	FItr end_codes = itr;
	FItr start_codes = itr + 2 + seg_count*1*2;
	FItr id_deltas = itr + 2 + seg_count*2*2;
	FItr id_range_offsets = itr + 2 + seg_count*3*2;

	if (ReadUSHORT(end_codes + (seg_count - 1) * 2) != 0xffff) throw InvalidFontException("Last end code of format 4 'cmap' table is not 0xffff.");
	if (ReadUSHORT(end_codes + (seg_count) * 2) != 0) throw InvalidFontException("Variable 'reservePad' following end code data of format 4 'cmap' table is not 0.");

	// serach variables
	uint16_t i;									// if successful, i = matching index
	uint32_t start_code, end_code;

	// binary search
	uint16_t imin = 0, imax = seg_count - 1;			// inclusive range
	while (imin < imax) {
		uint16_t imid = (imin + imax) >> 1;
		if (code > ReadUSHORT(end_codes + imid*2))		imin = imid + 1;		// just need to check for greater than, so only need to read end code
		else																		imax = imid;
		}
	start_code = ReadUSHORT(start_codes + imin*2);
	end_code = ReadUSHORT(end_codes + imin*2);
	if (code >= start_code && code <= end_code) i = imin;
	else return 0;

	/*
	// linear search
	for (i = 0; i < seg_count; ++i) {
		start_code = ReadUSHORT(start_codes + i*2);
		end_code = ReadUSHORT(end_codes + i*2);
		if (code >= start_code && code <= end_code) break;
		}
	if (i == seg_count) return 0;
	*/

	// calculate glyph index
	uint16_t id_delta = ReadUSHORT(id_deltas + i*2);									// apple docs say unsigned, MS say signed
	uint16_t id_range_offset = ReadUSHORT(id_range_offsets + i*2);
	FItr id_range_offset_addr = id_range_offsets + i*2;

	// map code to its glyph index
	uint16_t glyph_index = 0;
	if (id_range_offset == 0) {
		glyph_index = (id_delta + static_cast<uint16_t>(code & 0xffff)) & 0xffff;			// obviously this only works for smaller codes
		}
	else {
		FItr glyph_index_array_addr = id_range_offset_addr + id_range_offset + 2*(code - start_code);
		glyph_index = ReadUSHORT(glyph_index_array_addr);
		}

	return glyph_index;
	}

uint16_t Font::GetGlyphIndexF6 (FItr itr, uint16_t langid, uint32_t code) const {

	// load header
	uint16_t format = ReadUSHORT(itr);
	uint16_t length = ReadUSHORT(itr);
	uint16_t language = ReadUSHORT(itr);
	uint32_t first_code = ReadUSHORT(itr);
	uint32_t entry_count = ReadUSHORT(itr);
	FItr glyph_id_array = itr;

	if (format != 6) throw FontException("Internal error, calling GetGlyphIndexF6() on a 'cmap' table that isn't format 6.");
	if (language != langid) return 0;

	// map code point
	if (code >= first_code && code < first_code + entry_count) {
		uint32_t i = code - first_code;
		return ReadUSHORT(glyph_id_array + i*2);
		}
	return 0;
	}

uint16_t Font::GetGlyphIndexF8 (FItr itr, uint16_t langid, uint32_t code) const {
	// variable sized format not supported
	return 0;
	}

uint16_t Font::GetGlyphIndexF10 (FItr itr, uint16_t langid, uint32_t code) const {
	// 32 bit dense format, same as format 6 with just a few changes to the types

	int32_t format = ReadFIXED32(itr);					// 10.0 (unlike the other formats, this is in FIXED32 format)
	uint32_t length = ReadULONG(itr);					// byte length of this subtable (including the header)
	uint32_t language = ReadULONG(itr);
	uint32_t first_code = ReadULONG(itr);				// First character code covered
	uint32_t entry_count = ReadULONG(itr);			// Number of character codes covered
	FItr glyph_id_array = itr;									// Array of glyph indices for the character codes covered (ushorts)

	const int32_t required_format = 10 << 16;
	if (format != required_format) throw FontException("Internal error, calling GetGlyphIndexF10() on a 'cmap' table that isn't format 10.0.");
	if (language != langid) return 0;

	// map code point
	if (code >= first_code && code < first_code + entry_count) {
		uint32_t i = code - first_code;
		return ReadUSHORT(glyph_id_array + i*2);
		}
	return 0;
	}

uint16_t Font::GetGlyphIndexF12 (FItr itr, uint16_t langid, uint32_t code) const {
	// 32 bit sparse format, similar to format 4 (actually much simpler and makes more sense)

	uint16_t format = ReadUSHORT(itr);
	uint16_t reserved = ReadUSHORT(itr);			// should be 0
	uint32_t length = ReadULONG(itr);				// byte length of this subtable (including the header)
	uint32_t language = ReadULONG(itr);
	uint32_t ngroups = ReadULONG(itr);				// number of groupings which follow

	if (format != 12) throw FontException("Internal error, calling GetGlyphIndexF12() on a 'cmap' table that isn't format 12.");
	if (language != langid) return 0;

	// binary search
	uint32_t imin = 0, imax = ngroups - 1;			// inclusive range
	while (imin < imax) {
		uint32_t imid = (imin + imax) >> 1;
		if (code > ReadULONG(itr + imid*12 + 8))		imin = imid + 1;		// just need to check for greater than, so only need to read end code
		else																imax = imid;
		}
	itr += imin*12;

	// map codeId to glyphId
	uint32_t start_code = ReadULONG(itr);
	uint32_t end_code = ReadULONG(itr);
	if (code >= start_code && code <= end_code)  {
		uint32_t glyph_id = ReadULONG(itr);
		return static_cast<uint16_t>(code - start_code + glyph_id);
		}
	
	return 0;
	}

uint16_t Font::GetGlyphIndexF13 (FItr itr, uint16_t langid, uint32_t code) const {
	// 32 bit 'many to one' format, similar to format 12

	uint16_t format = ReadUSHORT(itr);
	uint16_t reserved = ReadUSHORT(itr);			// should be 0
	uint32_t length = ReadULONG(itr);				// byte length of this subtable (including the header)
	uint32_t language = ReadULONG(itr);
	uint32_t nGroups = ReadULONG(itr);			// number of groupings which follow

	if (format != 13) throw FontException("Internal error, calling GetGlyphIndexF13() on a 'cmap' table that isn't format 13.");
	if (language != langid) return 0;

	// binary search
	uint32_t imin = 0, imax = nGroups - 1;			// inclusive range
	while (imin < imax) {
		uint32_t imid = (imin + imax) >> 1;
		if (code > ReadULONG(itr + imid*12 + 8))		imin = imid + 1;		// just need to check for greater than, so only need to read end code
		else																imax = imid;
		}
	itr += imin*12;

	// map codeId to glyphId
	uint32_t start_code = ReadULONG(itr);
	uint32_t end_code = ReadULONG(itr);
	if (code >= start_code && code <= end_code)  {
		uint32_t glyph_id = ReadULONG(itr);
		return static_cast<uint16_t>(glyph_id);
		}

	return 0;
	}

uint16_t Font::GetGlyphIndexF14 (FItr itr, uint16_t langid, uint32_t code) const {
	// unicode variation sequences
	// not supported
	return 0;
	}


// ----- data range mappings -----
Font::FRange Font::GetGlyphRange (uint16_t glyph_index) const {

	// intialize variables
	int16_t ilf = GetIndexToLocFormat();
	uint16_t num_glyps = GetNumGlyphs();
	TableEntry loca = GetTableEntry(CreateTag('l','o','c','a'));
	TableEntry glyf = GetTableEntry(CreateTag('g','l','y','f'));

	// sanity check
	if (glyph_index >= num_glyps) throw InvalidFontException("Invalid glyph mapping, attempting to map a glyph index that exceeds the maximum number of glyphs in the font.");

	// find glyph data range
	if (ilf == 0) {
		FRange range;
		FItr itr = loca.begin + static_cast<size_t>(glyph_index) * 2;
		range.first = glyf.begin + static_cast<size_t>(ReadUSHORT(itr)) * 2;
		range.second = glyf.begin + static_cast<size_t>(ReadUSHORT(itr)) * 2;
		return range;
		}
	else if (ilf == 1) {
		FRange range;
		FItr itr = loca.begin + static_cast<size_t>(glyph_index) * 4;
		range.first = glyf.begin + static_cast<size_t>(ReadULONG(itr));
		range.second = glyf.begin + static_cast<size_t>(ReadULONG(itr));
		return range;
		}
	
	// done
	throw InvalidFontException("Invalid 'head' indexToLocFormat value.");
	}


// ----- metrics helpers -----
vec2s Font::GetKerning (uint16_t g0, uint16_t g1, bool hk) const {

	// intialize variables
	vec2s kv;
 kv.x = static_cast<int16_t>(GetGlyphMetrics(g0).advance_width);
 kv.y= 0;		// get default advance width

	// get kerning table (not a required table, so we have to manually check)
	auto ti = table_map.find(CreateTag('k','e','r','n'));
	if (ti == table_map.end()) return kv;
	FItr itr = ti->second.begin;

	// parse kerning sub-tables
	uint16_t version = ReadUSHORT(itr);
	if (version == 0) {
		uint16_t ntables =  ReadUSHORT(itr);
		for (uint16_t i = 0; i < ntables; ++i) itr += ParseMSKernTable(itr,g0,g1,hk,kv);
		}
	else if (version == 1) {
		uint16_t version_lo = ReadUSHORT(itr);			// apple uses a different header, with the version being 4 bytes, so this is the last 2 bytes of version
		uint32_t ntables = ReadULONG(itr);
		for (uint32_t i = 0; i < ntables; ++i) itr += ParseAppleKernTable(itr,g0,g1,hk,kv);
		}
	
	// done
	return kv;
	}

int16_t Font::ParseKernTableF0 (FItr itr, uint16_t g0, uint16_t g1) const {

	// read header
	uint16_t npairs = ReadUSHORT(itr);
	uint16_t search_range = ReadUSHORT(itr);		// The largest power of two less than or equal to the value of nPairs, multiplied by the size in bytes of an entry in the table.
	uint16_t entry_selector = ReadUSHORT(itr);		// number of iterations required
	uint16_t range_shift = ReadUSHORT(itr);

	// sanity check
	if (npairs == 0) return 0;

	// perform binary search
	// "Deferred detection of equality" algorithm from: http://en.wikipedia.org/wiki/Binary_search_algorithm
	uint32_t key = static_cast<uint32_t>(g0) << 16 | static_cast<uint32_t>(g1);
	uint32_t imin = 0, imax = npairs - 1;			// inclusive range
	while (imin < imax) {
		uint16_t imid = (imin + imax) >> 1;
		if (key > ReadULONG(itr + imid*6))	imin = imid + 1;
		else													imax = imid;
		}

	if (ReadULONG(itr + imin*6) == key) return ReadSHORT(itr + imin*6 + 4);
	else return 0;
	}

int16_t Font::ParseKernTableF2 (FItr itr, uint16_t g0, uint16_t g1) const {

	uint16_t left_class_offset = 0;
	uint16_t right_class_offset = 0;

	// read header
	FItr begin_itr = itr;
	uint16_t row_width = ReadUSHORT(itr);
	uint16_t left_offset_table = ReadUSHORT(itr);
	uint16_t right_offset_table = ReadUSHORT(itr);
	uint16_t kern_array = ReadUSHORT(itr);

	// get left glyph class
	itr = begin_itr + left_offset_table;
	uint16_t left_first_glyph = ReadUSHORT(itr);
	uint16_t left_nglyphs = ReadUSHORT(itr);
	if (g0 >= left_first_glyph && g0 < left_first_glyph + left_nglyphs) {
		left_class_offset = ReadUSHORT(itr + (g0 - left_first_glyph) * 2);
		}

	// get right glyph class
	itr = begin_itr + right_offset_table;
	uint16_t right_first_glyph = ReadUSHORT(itr);
	uint16_t right_nglyphs = ReadUSHORT(itr);
	if (g0 >= right_first_glyph && g0 < right_first_glyph + right_nglyphs) {
		right_class_offset = ReadUSHORT(itr + (g0 - right_first_glyph) * 2);
		}

	// get kerning value
	itr = begin_itr + kern_array;
	return ReadSHORT(itr + left_class_offset + right_class_offset);
	}

uint16_t Font::ParseMSKernTable (FItr itr, uint16_t g0, uint16_t g1, bool hk, vec2s& kv) const {

	// read table header
	uint16_t version = ReadUSHORT(itr);
	uint16_t length = ReadUSHORT(itr);
	uint16_t coverage = ReadUSHORT(itr);
	bool horizontal = (coverage & 1) != 0;				// != 0 to prevent silly warnings
	bool minimum = (coverage & 2) != 0;
	bool xstream = (coverage & 4) != 0;
	bool replace = (coverage & 8) != 0;
	uint16_t format = coverage >> 8;

	// direction check
	if (horizontal != hk) return length;

	// get kern modifier
	int16_t km = 0;
	switch (format) {
		case 0: km = ParseKernTableF0(itr,g0,g1); break;
		case 2: km = ParseKernTableF2(itr,g0,g1); break;
		}

	// adjust kv
	if (horizontal && !xstream || !horizontal && xstream) {
		if (minimum) kv.x = (km >= 0) ? std::min(kv.x,km) : std::max(kv.x,km);
		else if (replace) kv.x = km;
		else kv.x += km;
		}
	else {
		if (minimum) kv.y = (km >= 0) ? std::min(kv.y,km) : std::max(kv.y,km);
		else if (replace) kv.y = km;
		else kv.y += km;
		}

	// done
	return length;
	}

uint32_t Font::ParseAppleKernTable (FItr itr, uint16_t g0, uint16_t g1, bool hk, vec2s& kv) const {

	// read table header
	uint32_t length = ReadULONG(itr);
	uint16_t coverage = ReadUSHORT(itr);
	uint16_t tupleIndex = ReadUSHORT(itr);
	bool horizontal = (coverage & 0x8000) == 0;
	bool xstream = (coverage & 0x4000) != 0;
	bool variation = (coverage & 0x2000) != 0;
	uint16_t format = coverage & 0xff;

	// direction check
	if (horizontal != hk) return length;

	// no support for variation values
	if (variation) return length;

	// get kern modifier
	int16_t km = 0;
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
void Font::FillContours (FItr itr, Triangulator& triangulator) const {
	using namespace std;

	// read header
	uint16_t contour_count = ReadUSHORT(itr);			// safe because its already been checked
	itr += 8;

	// create contour points
	std::vector<ContourPoint>& contours = triangulator.GetContours();
	contours.clear();
	for (uint16_t i = 0; i < contour_count; ++i) {
		uint16_t endpt = ReadUSHORT(itr);
		if (endpt >= contours.size()) contours.resize(endpt + 1);		// assumes zero based index
		contours[endpt].end_point = true;
		}
	size_t point_count = contours.size();

	// skip instructions (not used)
	uint16_t instruction_count = ReadUSHORT(itr);
	itr += instruction_count;

	// read flags
	for (size_t i = 0; i < point_count;) {
		uint8_t f = ReadBYTE(itr);							// flags
		uint16_t r = 1;											// repeat count
		if ( (f & 8) != 0 ) r += ReadBYTE(itr);		// 1 + ReadBYTE(itr)
		for (uint16_t j = 0; j < r; ++j) {
			contours[i].flags = f;
			++i;
			}
		}

	// read x coords
	for (size_t i = 0; i < point_count; ++i) {
		ContourPoint& cp = contours[i];

		// read offset
		int16_t offset;
		if (cp.XShortVector()) {
			offset = ReadBYTE(itr);
			if (cp.XIsNegative()) offset = -offset;
			}
		else {
			if (cp.XIsSame()) offset = 0;
			else offset = ReadSHORT(itr);
			}

		// store offset for later
		cp.pos.x = offset;
		}

	// read y coords
	for (size_t i = 0; i < point_count; ++i) {
		ContourPoint& cp = contours[i];

		// read offset
		int16_t offset;
		if (cp.YShortVector()) {
			offset = ReadBYTE(itr);
			if (cp.YIsNegative()) offset = -offset;
			}
		else {
			if (cp.YIsSame()) offset = 0;
			else offset = ReadSHORT(itr);
			}

		// store offset for later
		cp.pos.y = offset;
		}

	// adjust x/y offsets
	vec2s pre_point;
pre_point.x=0;
pre_point.y=0; //{ 0, 0 };
	for (auto i = begin(contours); i != end(contours); ++i) {
		i->pos += pre_point;
		pre_point = i->pos;
		}
	}

bool Font::ReadGlyphFlags (FItr& itr, matrix3x2s& matrix, uint16_t& glyph_index) const {
	using std::max;
	using std::abs;

	// define composite flags
	const uint16_t ARG_1_AND_2_ARE_WORDS = 0x0001;			// If this is set, the arguments are words; otherwise, they are bytes.
	const uint16_t ARGS_ARE_XY_VALUES = 0x0002;					// If this is set, the arguments are xy values; otherwise, they are points.
	const uint16_t ROUND_XY_TO_GRID = 0x0004;						// For the xy values if the preceding is true.
	const uint16_t WE_HAVE_A_SCALE = 0x0008;							// This indicates that there is a simple scale for the component. Otherwise, scale = 1.0.
	const uint16_t RESERVED	 = 0x0010;										// This bit is reserved. Set it to 0.
	const uint16_t MORE_COMPONENTS = 0x0020;						// Indicates at least one more glyph after this one.
	const uint16_t WE_HAVE_AN_X_AND_Y_SCALE = 0x0040;		// The x direction will use a different scale from the y direction.
	const uint16_t WE_HAVE_A_TWO_BY_TWO = 0x0080;				// There is a 2 by 2 transformation that will be used to scale the component.
	const uint16_t WE_HAVE_INSTRUCTIONS = 0x0100;				// Following the last component are instructions for the composite character.
	const uint16_t USE_MY_METRICS	 = 0x0200;							// If set, this forces the aw and lsb (and rsb) for the composite to be equal to those from this original glyph. This works for hinted and unhinted characters.
	const uint16_t OVERLAP_COMPOUND = 0x0400;						// Used by Apple in GX fonts.
	const uint16_t SCALED_COMPONENT_OFFSET = 0x0800;			// Composite designed to have the component offset scaled (designed for Apple rasterizer).
	const uint16_t UNSCALED_COMPONENT_OFFSET = 0x1000;		// Composite designed not to have the component offset scaled (designed for the Microsoft TrueType rasterizer).

	// intialize variables
	int32_t& a = matrix.a;
	int32_t& b = matrix.b;
	int32_t& c = matrix.c;
	int32_t& d = matrix.d;
	int32_t& e = matrix.e;
	int32_t& f = matrix.f;

	// read flags/glyph index
	uint16_t flags = ReadUSHORT(itr);
	glyph_index = ReadUSHORT(itr);

	// read 'e' and 'f' values
	if (flags & ARGS_ARE_XY_VALUES) {
		if (flags & ARG_1_AND_2_ARE_WORDS) {
			e = static_cast<int32_t>(ReadSHORT(itr)) << 16;
			f = static_cast<int32_t>(ReadSHORT(itr)) << 16;
			}
		else {
			e = static_cast<int32_t>(ReadCHAR(itr)) << 16;
			f = static_cast<int32_t>(ReadCHAR(itr)) << 16;
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
	const int32_t k = 33;						// 33.0f / 65536.0f;
	int32_t m = max(abs(a),abs(b));
	int32_t n = max(abs(c),abs(d));
	if (abs(abs(a) - abs(c)) <= k) m <<= 1;			// * 2.0f
	if (abs(abs(c) - abs(d)) <= k) n <<= 1;			// * 2.0f
	e = RoundBy16Large(e * m);
	f = RoundBy16Large(f * n);

	// done
	return (flags & MORE_COMPONENTS) != 0;
	}

void Font::AppendSimpleMesh (FItr itr, Triangulator& triangulator, MeshSmall& mesh) const {
	FillContours(itr,triangulator);
	triangulator.AppendTriangulation(mesh);
	}

void Font::AppendSimpleMesh (FItr itr, Triangulator& triangulator, MeshLarge& mesh) const {
	FillContours(itr,triangulator);
	triangulator.AppendTriangulation(mesh);
	}

void Font::AppendComplexMesh (FItr itr, Triangulator& triangulator, MeshSmall& mesh) const {

	// skip header
	itr += 10;

	// loop while glyphs to compose
	bool continue_loop;
	do {
		// read flags
		matrix3x2s matrix;
		uint16_t glyph_index;
		continue_loop = ReadGlyphFlags(itr,matrix,glyph_index);

		// add to glyph to mesh
		std::vector<vec3s>::iterator verts_begin = mesh.verts.end();
		AppendGlyphMesh(glyph_index,triangulator,mesh);
		for (auto i = verts_begin; i != mesh.verts.end(); ++i) {
			*i = mul(*i,matrix);
			}
		} while (continue_loop);
	}

void Font::AppendComplexMesh (FItr itr, Triangulator& triangulator, MeshLarge& mesh) const {

	// skip header
	itr += 10;

	// loop while glyphs to compose
	bool continue_loop;
	do {
		// read flags
		matrix3x2s matrix;
		uint16_t glyph_index;
		continue_loop = ReadGlyphFlags(itr,matrix,glyph_index);

		// add to glyph to mesh
		std::vector<vec3s>::iterator verts_begin = mesh.verts.end();
		AppendGlyphMesh(glyph_index,triangulator,mesh);
		for (auto i = verts_begin; i != mesh.verts.end(); ++i) {
			*i = mul(*i,matrix);
			}
		} while (continue_loop);
	}

void Font::AppendGlyphMesh(uint16_t glyph_index, Triangulator& triangulator, MeshSmall& mesh) const {

	// get glyph data
	auto range = GetGlyphRange(glyph_index);
	if (range.first == range.second) return;			// empty / no glyph data
	FItr itr = range.first;

	// get glyph contours
	int16_t contour_count = ReadSHORT(itr + 0);				// prevent incrementing the iterator
	if (contour_count > 0) return AppendSimpleMesh(itr,triangulator,mesh);
	else if (contour_count == -1) return AppendComplexMesh(itr,triangulator,mesh);
	else throw InvalidFontException("Unknown contour_count in glyph data.");	
	}

void Font::AppendGlyphMesh(uint16_t glyph_index, Triangulator& triangulator, MeshLarge& mesh) const {

	// get glyph data
	auto range = GetGlyphRange(glyph_index);
	if (range.first == range.second) return;
	FItr itr = range.first;

	// get glyph contours
	int16_t contour_count = ReadSHORT(itr + 0);
	if (contour_count > 0) return AppendSimpleMesh(itr,triangulator,mesh);
	else if (contour_count == -1) return AppendComplexMesh(itr,triangulator,mesh);
	else throw InvalidFontException("Unknown contour_count in glyph data.");	
	}


// ----- font info -----
vec4s Font::GetMasterRect () const {
	HeadTable ht = ReadHeadTable();
	return vec4s(ht.xmin,ht.ymin,ht.xmax,ht.ymax);
	}

FontMetrics Font::GetFontMetrics () const {

	// read hhea data
	FItr itr = GetTableEntry(CreateTag('h','h','e','a')).begin;
	uint16_t version = ReadULONG(itr);
	int16_t ascent = ReadSHORT(itr);
	int16_t descent = ReadSHORT(itr);
	int16_t line_gap = ReadSHORT(itr);
	uint16_t advance_width_max = ReadUSHORT(itr);
	int16_t min_left_side_bearing = ReadSHORT(itr);
	int16_t min_right_side_bearing = ReadSHORT(itr);
	int16_t x_max_extent = ReadSHORT(itr);
	int16_t caret_slope_rise = ReadSHORT(itr);
	int16_t caret_slope_run = ReadSHORT(itr);
	int16_t caret_offset = ReadSHORT(itr);
	int16_t reserved0 = ReadSHORT(itr);
	int16_t reserved1 = ReadSHORT(itr);
	int16_t reserved2 = ReadSHORT(itr);
	int16_t reserved3 = ReadSHORT(itr);
	int16_t metric_data_format = ReadSHORT(itr);
	uint16_t num_of_long_hor_metrics = ReadUSHORT(itr);

	// sanity check
	if (version != 0x00010000) throw VersionException("Invalid 'hhea' table version.");
	if (metric_data_format != 0) throw VersionException("Invalid 'hhea' metricDataFormat.");

	// translate
	FontMetrics fm;
	fm.ascent = ascent;
	fm.descent = descent;
	fm.line_gap = line_gap;
	fm.caret_slope.x=caret_slope_run;
 fm.caret_slope.y=caret_slope_rise;
	fm.caret_offset = caret_offset;
	fm.min_left_side_bearing = min_left_side_bearing;
	fm.min_right_side_bearing = min_right_side_bearing;
	fm.advance_width_max = advance_width_max;

	return fm;
	}

VFontMetrics Font::GetVFontMetrics () const {

	// get vhea table data
	VFontMetrics vfm;
	auto i = table_map.find(CreateTag('v','h','e','a'));
	if (i == table_map.end()) {
		vfm.has_vertical_font_metrics = false;
		return vfm;
		}
	vfm.has_vertical_font_metrics = true;
	FItr itr = i->second.begin;

	// read table
	uint16_t version = ReadULONG(itr);
	int16_t vert_typo_ascender = ReadSHORT(itr);
	int16_t vert_typo_descender = ReadSHORT(itr);
	int16_t vert_typo_line_gap = ReadSHORT(itr);
	int16_t advance_height_max = ReadSHORT(itr);
	int16_t min_top_side_bearing = ReadSHORT(itr);
	int16_t min_bottom_side_bearing = ReadSHORT(itr);
	int16_t y_max_extent = ReadSHORT(itr);
	int16_t caret_slope_rise = ReadSHORT(itr);
	int16_t caret_slope_run = ReadSHORT(itr);
	int16_t caret_offset = ReadSHORT(itr);
	int16_t reserved0 = ReadSHORT(itr);
	int16_t reserved1 = ReadSHORT(itr);
	int16_t reserved2 = ReadSHORT(itr);
	int16_t reserved3 = ReadSHORT(itr);
	int16_t metric_data_format = ReadSHORT(itr);
	int16_t num_of_long_ver_metrics = ReadSHORT(itr);

	// sanity check
	if (version != 0x00010000) throw VersionException("Invalid 'vhea' table version.");
	if (metric_data_format != 0) throw VersionException("Invalid 'vhea' metricDataFormat.");

	// translate
	vfm.vert_typo_descender = vert_typo_ascender;
	vfm.vert_typo_ascender = vert_typo_descender;
	vfm.vert_typo_line_gap = vert_typo_line_gap;
	vfm.caret_slope.x=caret_slope_run;
 vfm.caret_slope.y=caret_slope_rise;
	vfm.caret_offset = caret_offset;
	vfm.min_top_side_bearing = min_top_side_bearing;
	vfm.min_bottom_side_bearing = min_bottom_side_bearing;
	vfm.advance_height_max = advance_height_max;

	return vfm;
	}

uint16_t Font::GlyphCount () const {
	return GetNumGlyphs();
	}


// ----- glyph info -----
uint16_t Font::GetGlyphIndex (CodePoint code_point) const {

	// convenience variables
	uint32_t code = code_point.code;
	uint16_t rpid = code_point.platform;
	uint16_t reid = code_point.encoding;
	uint16_t langid = code_point.language;

	// intialize variables
	FItr begin = GetTableEntry(CreateTag('c','m','a','p')).begin;
	FItr itr = begin;
	uint32_t glyph_index = 0;

	// read cmap 'header'
	uint16_t version = ReadUSHORT(itr);
	if (version != 0) throw VersionException("Table 'cmap' version is unsupported.");
	uint16_t numTables = ReadUSHORT(itr);

	// iterate over tables
	for (uint16_t i = 0; i < numTables; ++i) {

		// load table info
		uint16_t platform = ReadUSHORT(itr);
		uint16_t encoding = ReadUSHORT(itr);
		FItr table_itr = begin + ReadULONG(itr);

		// check platform/encoding id
		if (rpid == platform && reid == encoding) {

			// load format and language info
			uint16_t format = ReadUSHORT(table_itr + 0);		// don't advance iterator

			// branch on format
			uint16_t glyph_index = 0;
			switch (format) {

				case 0:
					glyph_index = GetGlyphIndexF0(table_itr,langid,code);
					if (glyph_index != 0) return glyph_index;
					break;

				case 2:
					glyph_index = GetGlyphIndexF2(table_itr,langid,code);
					if (glyph_index != 0) return glyph_index;
					break;

				case 4:
					glyph_index = GetGlyphIndexF4(table_itr,langid,code);
					if (glyph_index != 0) return glyph_index;
					break;

				case 6:
					glyph_index = GetGlyphIndexF6(table_itr,langid,code);
					if (glyph_index != 0) return glyph_index;
					break;

				case 8:
					glyph_index = GetGlyphIndexF8(table_itr,langid,code);
					if (glyph_index != 0) return glyph_index;
					break;

				case 10:
					glyph_index = GetGlyphIndexF10(table_itr,langid,code);
					if (glyph_index != 0) return glyph_index;
					break;

				case 12:
					glyph_index = GetGlyphIndexF12(table_itr,langid,code);
					if (glyph_index != 0) return glyph_index;
					break;

				case 13:
					glyph_index = GetGlyphIndexF13(table_itr,langid,code);
					if (glyph_index != 0) return glyph_index;
					break;

				case 14:
					glyph_index = GetGlyphIndexF14(table_itr,langid,code);
					if (glyph_index != 0) return glyph_index;
					break;
				}
			}
		}

	return 0;			// the 0 glyph is always the 'cannot find' glyph
	}

vec4s Font::GetGlyphRect (uint16_t glyph_index) const {

	// get glyph data
	FRange glyph_data = GetGlyphRange(glyph_index);
	FItr itr = glyph_data.first;

	// read glyph header
	int16_t num_contours = ReadSHORT(itr);
	vec4s r;
 r.w=ReadSHORT(itr);
 r.x=ReadSHORT(itr);
 r.y=ReadSHORT(itr);
 r.z=ReadSHORT(itr);

	// done
	return r;
	}

vec4s Font::GetGlyphRect (CodePoint code_point) const {
	return GetGlyphRect(GetGlyphIndex(code_point));
	}

GlyphMetrics Font::GetGlyphMetrics (uint16_t glyph_index) const {

	// read hhea data
	FItr itr = GetTableEntry(CreateTag('h','h','e','a')).begin;
	uint32_t version = ReadULONG(itr);
	itr += 28;																	// skip rest of hhea table
	int16_t metric_data_format = ReadSHORT(itr);
	uint32_t num_of_long_hor_metrics = ReadUSHORT(itr);

	// sanity check
	if (version != 0x00010000) throw VersionException("Invalid 'hhea' table version.");
	if (metric_data_format != 0) throw VersionException("Invalid 'hhea' metricDataFormat.");

	// read hmtx
	uint16_t advance_width;
	int16_t left_side_bearing;

	itr = GetTableEntry(CreateTag('h','m','t','x')).begin;
	if (glyph_index < num_of_long_hor_metrics) {
		itr += glyph_index * 4;
		advance_width = ReadUSHORT(itr);
		left_side_bearing = ReadSHORT(itr);
		}
	else {
		// get advance_width from last entry
		itr += (num_of_long_hor_metrics - 1) * 4;
		advance_width = ReadUSHORT(itr);
		itr += 2;

		// get lsb
		itr += (glyph_index - num_of_long_hor_metrics) * 2;
		left_side_bearing = ReadSHORT(itr);
		}

	// convert to GlyphMetrics
	GlyphMetrics gm;
	gm.left_side_bearing = left_side_bearing;
	gm.advance_width = advance_width;
	return gm;
	}

GlyphMetrics Font::GetGlyphMetrics (CodePoint code_point) const {
	return GetGlyphMetrics(GetGlyphIndex(code_point));
	}

VGlyphMetrics Font::GetVGlyphMetrics (uint16_t glyph_index) const {

	VGlyphMetrics vgm;
	vgm.has_vertical_font_metrics = false;

	// get vhea table
	auto i = table_map.find(CreateTag('v','h','e','a'));
	if (i == table_map.end()) return vgm;
	FItr itr = i->second.begin;

	// read vhea data
	uint32_t version = ReadULONG(itr);
	itr += 28;																	// skip rest of vhea table
	int16_t metric_data_format = ReadSHORT(itr);
	uint32_t num_of_long_ver_metrics = ReadUSHORT(itr);

	// sanity check
	if (version != 0x00010000) throw VersionException("Invalid 'vhea' table version.");
	if (metric_data_format != 0) throw VersionException("Invalid 'vhea' metricDataFormat.");

	// get vmtx table
	i = table_map.find(CreateTag('v','m','t','x'));
	if (i == table_map.end()) return vgm;
	vgm.has_vertical_font_metrics = true;
	itr = i->second.begin;

	// read vmtx table
	uint16_t advance_height;
	int16_t top_side_bearing;

	if (glyph_index < num_of_long_ver_metrics) {
		itr += glyph_index * 4;
		advance_height = ReadUSHORT(itr);
		top_side_bearing = ReadSHORT(itr);
		}
	else {
		// get advanceWidth from last entry
		itr += (num_of_long_ver_metrics - 1) * 4;
		advance_height = ReadUSHORT(itr);
		itr += 2;

		// get lsb
		itr += (glyph_index - num_of_long_ver_metrics) * 2;
		top_side_bearing = ReadSHORT(itr);
		}

	// convert to VGlyphMetrics
	vgm.top_side_bearing = top_side_bearing;
	vgm.advance_height = advance_height;
	return vgm;
	}

VGlyphMetrics Font::GetVGlyphMetrics (CodePoint code_point) const {
	return GetVGlyphMetrics(GetGlyphIndex(code_point));
	}


// ----- kerning info -----
vec2s Font::GetKerning (uint16_t g0, uint16_t g1) const {
	return GetKerning(g0,g1,true);
	}

vec2s Font::GetKerning (CodePoint cp0, CodePoint cp1) const {
	return GetKerning(GetGlyphIndex(cp0),GetGlyphIndex(cp1),true);
	}

vec2s Font::GetVKerning (uint16_t g0, uint16_t g1) const {
	return GetKerning(g0,g1,false);
	}

vec2s Font::GetVKerning (CodePoint cp0, CodePoint cp1) const {
	return GetKerning(GetGlyphIndex(cp0),GetGlyphIndex(cp1),false);
	}


// ----- glyph triangulation -----
void Font::GetGlyph (uint16_t glyph_index, Triangulator& triangulator, MeshSmall& mesh) const {
	mesh.Clear();
	AppendGlyphMesh(glyph_index,triangulator,mesh);
	}

void Font::GetGlyph (CodePoint cp, Triangulator& triangulator, MeshSmall& mesh) const {
	mesh.Clear();
	AppendGlyphMesh(GetGlyphIndex(cp),triangulator,mesh);	
	}

void Font::GetGlyph (uint16_t glyph_index, Triangulator& triangulator, MeshLarge& mesh) const {
	mesh.Clear();
	AppendGlyphMesh(glyph_index,triangulator,mesh);	
	}

void Font::GetGlyph (CodePoint cp, Triangulator& triangulator, MeshLarge& mesh) const {
	mesh.Clear();
	AppendGlyphMesh(GetGlyphIndex(cp),triangulator,mesh);		
	}
