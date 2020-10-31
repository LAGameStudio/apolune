#pragma once

/* exception.h
 * ----------- 
 * Author: E. Pronk
 * Descr.: Base exception class
 */

#ifndef LMTZ_EXCEPTION_H
#define LMTZ_EXCEPTION_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifndef FD_MAX_ERROR_MESSAGE
#define FD_MAX_ERROR_MESSAGE (1<<16)
#endif

#define GENERATE_EXCEPTION(type, base) \
		class type : public base \
		{ \
		public: \
			type() : base() {} \
			type(const char* fmt, ...) \
			{ \
				va_list args; \
				va_start(args, fmt); \
				vsprintf_s(_message, FD_MAX_ERROR_MESSAGE, fmt, args); \
				va_end(args); \
			} \
		}

class Exception
{
public:
	Exception() { memset(_message, 0, FD_MAX_ERROR_MESSAGE); }
	const char* Message() const { return _message; }
protected:
	char _message[FD_MAX_ERROR_MESSAGE];
};

#endif