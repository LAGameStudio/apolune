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

/*
 *  Sectioned File (Section-based File I/O class)
 *
 *
 */

#include "VASTUtil.h"

namespace Vast
{  


SectionedFile * FileClassFactory::CreateFileClass (int type)
{
    switch (type)
    {
    case 0:
        return new StdIO_SectionedFile ();

    default:
        return NULL;
    };
}

bool FileClassFactory::DestroyFileClass (SectionedFile * filec)
{
    if (filec != NULL)
    {
        delete ((StdIO_SectionedFile *)filec);
        return true;
    }
    else
        return false;
}

} // namespace Vast

