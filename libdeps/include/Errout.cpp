/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2007 Shao-Chen Chang (cscxcs at gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */


//#include "errout.h"
#include "VASTUtil.h"

namespace Vast
{  


errout * errout::_actout = NULL;
char errout::textbuf [10240];

errout::errout ()
{
}

errout::~errout ()
{
}

void errout::setout (errout * out)
{
	_actout = out;
}

void errout::output (const char * str)
{
	if (_actout != NULL)
		_actout->outputHappened (str);
	else
		outputHappened (str);
}

void errout::outputHappened (const char * str)
{
	puts (str);
}

} // namespace Vast

