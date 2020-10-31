/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2007 Yu-Li Huang
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

#include "VASTUtil.h"

#include "zlib.h"
#include "assert.h"

namespace Vast
{  

#define CHUNK (1024 * 1024)	// 1024 * 1024 = 1MB


// deflate data from source buffer to destination buffer
size_t Compressor::compress (uint8_t *source, uint8_t *dest, size_t size)
{	
    int ret;
    size_t have;
    z_stream strm;

    // allocate deflate state
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit (&strm, -1);
    if (ret != Z_OK)
	    return ret;

    // compress until end of file 
    strm.avail_in = size;
    strm.next_in = source;
    strm.avail_out = CHUNK;
    strm.next_out = dest;

    ret = deflate (&strm, Z_FINISH);	// no bad return value
    assert (ret != Z_STREAM_ERROR);  // state not clobbered 

    have = CHUNK - strm.avail_out;
    (void)deflateEnd (&strm);
    return have;
}

} // namespace Vast

