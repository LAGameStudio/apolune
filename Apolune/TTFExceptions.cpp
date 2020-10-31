// *************************************************************************************
//	TTFExceptions.cpp
//
//		Implementation of the various TTF exceptions.
//
//																															Ryan Bogean
//																															April 2012
//																															January 2014
//
// *************************************************************************************

#include <exception>
#include <sstream>

#include "TTFExceptions.h"


using namespace TTFCore;



// ---------------------------------------------------------------------------------------------------------------------------
//	exceptions
// ---------------------------------------------------------------------------------------------------------------------------

// ----- FontException -----
FontException::FontException () : msg("Unknown error message.") {} 
FontException::FontException (const char* msg_) : msg(msg_) {} 
FontException::FontException (const std::string& msg_) : msg(msg_) {} 
const char* FontException::what() const throw() { return msg.c_str(); }


// ----- FileFailure -----
FileFailure::FileFailure (const std::string& flnm) {
	std::stringstream ss;
	ss << "Unknown error reading file: '" << flnm << "'.";
	msg = ss.str();
	} 


// ----- FileLengthError -----
FileLengthError::FileLengthError () : FontException("Length error reading memory mapped file, file has size of 0.") {}

FileLengthError::FileLengthError (const std::string& flnm) {
	std::stringstream ss;
	ss << "Length error reading file: '" << flnm << "', file has size of 0.";
	msg = ss.str();
	} 


// ----- TableDoesNotExist -----
TableDoesNotExist::TableDoesNotExist (const std::string& table) {
	std::stringstream ss;
	ss << "Cannot load font, '" << table << "' table does not exist.";
	msg = ss.str();
	}


// ----- ChecksumException -----
ChecksumException::ChecksumException (const std::string& table) {
	std::stringstream ss;
	ss << "Check sum error in '" << table << "'.";
	msg = ss.str();
	}


// ----- VersionException -----
VersionException::VersionException (const std::string& msg) : FontException(msg) {}


// ----- InvalidFontException -----
InvalidFontException::InvalidFontException (const std::string& msg) : FontException(msg) {}


// ----- UnsupportedCap -----
UnsupportedCap::UnsupportedCap (const std::string& msg) : FontException(msg) {}

