/// Copyright (c) 2009 Ruben Penalva. All rights reserved.
/// Redistribution and use in source and binary forms, with or without modification, 
/// are permitted provided that the following conditions are met:
///    1. Redistributions of source code must retain the above copyright notice, this 
///    list of conditions and the following disclaimer.
///    2. Redistributions in binary form must reproduce the above copyright notice,
///    this list of conditions and the following disclaimer in the documentation and/or
///    other materials provided with the distribution.
///    3. The name of the author may not be used to endorse or promote products derived
///    from this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
/// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
/// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, 
/// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
/// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
/// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF 
/// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/// @file Point.h
/// This source file is part of 2dRender LetterCDT Test.
/// The file holds the Point class interface.
/// @author Ruben Penalva (ruben.penalva@rpenalva.com)

#ifndef _POINT_H_
#define _POINT_H_

namespace LetterCDT
{

/// @brief It has two coordinates: x and y
class CDTPoint
{
public:
	/// X coordinate
	float m_x;

	/// Y coordinate
	float m_y;

	/// Constructor
	CDTPoint(void);

	/// Overloaded constructor
	/// @param x x coordinate
	/// @param y y coordinate
	CDTPoint(float x, float y);

	/// Destructor
	~CDTPoint(void);

	bool operator==(CDTPoint& point) const;

	CDTPoint& operator+=(const CDTPoint& point);

	CDTPoint& operator/=(float i);

	/// @brief Gives orientation of three points
	/// @param b 
	/// @param c
	bool CounterClockWise(const CDTPoint& b, const CDTPoint& c) const;
};

}

#endif