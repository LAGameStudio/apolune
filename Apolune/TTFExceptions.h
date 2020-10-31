// *************************************************************************************
//	TTFExceptions.h
//
//		Exceptions used in TTF.
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



// ---------------------------------------------------------------------------------------------------------------------------
//	TTFCore namespace
// ---------------------------------------------------------------------------------------------------------------------------

namespace TTFCore {



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
//		implemented in: TTFExceptions.cpp
// ---------------------------------------------------------------------------------------------------------------------------

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



// ---------------------------------------------------------------------------------------------------------------------------
//	End
// ---------------------------------------------------------------------------------------------------------------------------

}			// end of TTFCore namespace

