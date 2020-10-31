/*
 * VAST, a scalable Agent-to-Agent network for virtual environments
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
 *  StorageLogic.h -- interface for handling logics of a Storage class
 *
 *  ver 0.1 (2006/10/03)
 *   
 */

#ifndef VASTATE_STORAGE_LOGIC_H
#define VASTATE_STORAGE_LOGIC_H

#include "VASTATETypes.h"

namespace Vast 
{  
    class StorageLogic
    {
    public:        
        virtual ~StorageLogic () {}

        // callback - process a particular query for certain data
        virtual bool query_received (int query_id, char *query, size_t size) = 0;

        // callback - process the response to a particular query
        virtual bool reply_received (int query_id, char *reply, size_t size) = 0;

        // store access to Storage class
        virtual void register_Storage (void *) = 0;
    };
    
} // end namespace Vast

#endif // #define VASTATE_STORAGE_LOGIC_H

