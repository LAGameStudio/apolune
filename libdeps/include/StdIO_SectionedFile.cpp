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


#include "VASTUtil.h"

namespace Vast
{  

bool StdIO_SectionedFile::open  (const string & filename, SFOpenMode mode)
{
    if (_fp != NULL)
    {
        error ("StdIO_SectionedFile: open (): already opened.\n");
        return false;
    }

    string s_mode;
    switch (mode)
    {
    case SFMode_Read:
        s_mode = "rb";
        break;
    case SFMode_Write:
        s_mode = "wb";
        break;
    default:
        s_mode = "";
    }

    if ((_fp = fopen (filename.c_str (), s_mode.c_str ())) == NULL)
    {
        error ("StdIO_SectionedFile: open (): can't open file.");

        string s = "StdIO_SectionedFile: open (): filename: " + filename;
        error (s.c_str ());
        return false;
    }
    else
        _mode = mode;

    if (mode == SFMode_Read)
    {
        uint32_t section_count;
        vector<uint32_t> section_id;
        vector<uint32_t> section_size;
        if (fread (&section_count, sizeof (uint32_t), 1, _fp) != 1)
        {
            error ("StdIO_SectionedFile: open (): reading section count error.\n");
            return false;
        }

        for (uint32_t sci = 0; sci < section_count; sci ++)
        {
            uint32_t i[2];
            if (fread (&i, sizeof (uint32_t), 2, _fp) != 2)
            {
                error ("StdIO_SectionedFile: open (): reading section id&size error.\n");
                return false;
            }

            section_id.push_back (i[0]);
            section_size.push_back (i[1]);
        }

        for (uint32_t scii = 0; scii < section_count; scii ++)
        {
            section[section_id[scii]].insert (section[section_id[scii]].end (), section_size[scii], 0);
            if (fread (&(section[section_id[scii]][0]), section_size[scii], 1, _fp) != 1)
            {
                error ("StdIO_SectionedFile: open (): reading section content error.\n");
                section.clear ();
                return false;
            }
        }
    }

    return true;
}

int  StdIO_SectionedFile::read  (uint32_t section_id, void * buffer, int record_size, int record_count)
{
    if (_mode != SFMode_Read)
        return 0;

    uint32_t actually_readed = 0;
    int record_readed = 0;

    while ((section[section_id].size () >= static_cast<size_t>(record_size)) && (record_readed < record_count))
    {
        uint8_t * buffer_c = static_cast<uint8_t *>(buffer);
        memcpy (&buffer_c[actually_readed], &(section[section_id][actually_readed]), (size_t) record_size);
        record_readed ++;
        actually_readed += record_size;
    }

    section[section_id].erase (section[section_id].begin (), section[section_id].begin () + actually_readed);
    return record_readed;
}

int  StdIO_SectionedFile::write (uint32_t section_id, void * buffer, int record_size, int record_count)
{
    if (_mode != SFMode_Write)
        return 0;
    section[section_id].insert (section[section_id].end (), static_cast<uint8_t *>(buffer), static_cast<uint8_t *>(buffer) + (record_size * record_count));
    return record_count;
}

bool StdIO_SectionedFile::close ()
{
    if (_fp != NULL)
    {
        refresh ();
        fclose (_fp);
        _fp = NULL;        
    }
    return true;
}

bool StdIO_SectionedFile::refresh ()
{
    if ((_fp != NULL) && (_mode == SFMode_Write))
    {
        uint32_t record_count = static_cast<uint32_t>(section.size ());
        vector<uint8_t> buffer;
        buffer.reserve (record_count * sizeof (uint32_t) * 2);

        map<uint32_t,vector<uint8_t> >::iterator sit = section.begin ();
        for (; sit != section.end (); sit ++)
        {
            uint32_t id = sit->first;
            uint32_t size = (uint32_t) sit->second.size ();
            buffer.insert (buffer.end (), reinterpret_cast<uint8_t *>(&id), reinterpret_cast<uint8_t *>(&id + 1));
            buffer.insert (buffer.end (), reinterpret_cast<uint8_t *>(&size), reinterpret_cast<uint8_t *>(&size + 1));
        }

        if (fwrite (&record_count, sizeof(uint32_t), 1, _fp) != 1)
        {
            error ("StdIO_SectionedFile: refresh (): writing record count to file error.\n");
            return false;
        }

        if (fwrite (&buffer[0], buffer.size (), 1, _fp) != 1)
        {
            error ("StdIO_SectionedFile: refresh (): writing section record to file error.\n");
            return false;
        }

        map<uint32_t,vector<uint8_t > >::iterator sbit = section.begin ();
        for (; sbit != section.end (); sbit ++)
        {
            if (fwrite (&(sbit->second[0]), sbit->second.size (), 1, _fp) != 1)
            {
                error ("StdIO_SectionedFile: refresh (): writing section content to file error.\n");
                return false;
            }
        }
    }
    return true;
}

void StdIO_SectionedFile::error (const char * msg)
{
#ifdef _ERROROUTPUT_H
    errout eo;
    eo.output (msg);
#else
    puts (msg);
#endif
}

} // namespace Vast

