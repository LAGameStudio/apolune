/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2007 Shun-Yun Hu (syhu@yahoo.com)
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

/*
 *  VASTBuffer.h -- generic buffer class with expandable size
 *
 *  history     2007/03/22
 *   
 */


#ifndef VASTBUF_H
#define VASTBUF_H

#include "Config.h"
#include "VASTTypes.h"

namespace Vast
{
    class VASTBuffer
    {
    public:

        VASTBuffer ()
            :size (0), _bufsize (VAST_BUFSIZ)
        {
            data = new char[VAST_BUFSIZ];
        }

        ~VASTBuffer ()
        {
            delete[] data;
        }

        // increase the current buffer capacity by 'len' (but at multiples of VAST_BUFSIZ)
        bool expand (size_t len)
        {
            // estimate size for new buffer
            size_t newsize = (((size + len) / VAST_BUFSIZ) + 1) * VAST_BUFSIZ;

            // our needed expansion is smaller than current size
            if (newsize <= _bufsize)
                return false;
           
            char *temp = new char[newsize];

            if (temp == NULL)
            {
                printf ("VASTBuffer: cannot allocate enough memory\n");
                return false;
            }

            // copy old data to new buffer
            memcpy (temp, data, size);

            // remove old buffer
            delete[] data;
            data = temp;
            _bufsize = newsize;

            return true;
        }

        void reserve (size_t len)
        {
            size = 0;
            if (len > _bufsize)            
                expand (len);
        }

        void clear ()
        {
            size = 0;
        }

        bool add (void *stuff, size_t len)
        {
            // check if the buffer has enough space to add new stuff
            if (size + len > _bufsize && expand (len) == false)                
                return false;
            
            memcpy (data+size, stuff, len);
            size += len;

            return true;
        }

        // add a serializable object into the buffer
        bool add (Serializable *obj)
        {
            // query obj size
            size_t len = obj->serialize (NULL);
            if (len <= 0)
                return false;

            // make sure the buffer has enough space to add new stuff
            if (size + len > _bufsize && expand (len) == false)
                return false;

            // store object into buffer
            if (obj->serialize (data+size) != len)
                return false;
            
            size += len;
            return true;
        }

        size_t size;
        char  *data;

    private:
        size_t _bufsize;
    };

} // end namespace Vast

#endif // VASTBUF_H
