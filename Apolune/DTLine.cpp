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

/// @file Line.cpp
/// This source file is part of 2dRender LetterCDT Test.
/// The file holds the Line class implementation.
/// @author Ruben Penalva (ruben.penalva@rpenalva.com)

#include "DTLine.h"

using namespace LetterCDT;

CDTLine::CDTLine(const CDTPoint& start, const CDTPoint& end)	:	m_start(start), m_end(end)
{
}

CDTLine::~CDTLine(void)
{
}
