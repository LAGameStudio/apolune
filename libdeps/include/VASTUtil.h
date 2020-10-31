/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2007 Shao-Chen Chang (cscxcs at gmail.com)
 *                    Shun-Yun Hu     (syhu at yahoo.com)
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
    Utility classes for VAST: 
    
    NOTE: these classes should be self-sufficent, and should not rely on 
          any other VAST-related classes or type definitions except C++ standard includes
*/

#ifndef _VAST_UTIL_H
#define _VAST_UTIL_H

#include "Config.h"
#include "VASTTypes.h"

#include <string>
#include <map>
#include <vector>
#include <math.h>


using namespace std;

// forward declaration

//class EXPORT errout;

// 
// errout: Error output utility
//

namespace Vast {

class EXPORT errout
{
public:
	errout ();
	virtual ~errout ();
	void setout (errout * out);
	void output (const char * str);
	virtual void outputHappened (const char * str);

    static char textbuf [10240];
private:
	static errout * _actout;
};

//
//  SectionedFile: store/load data onto file in sections
//

enum SFOpenMode
{
    SFMode_NULL,
    SFMode_Read,
    SFMode_Write
};

class EXPORT SectionedFile
{
public:
    SectionedFile ()
    {
    }

    virtual ~SectionedFile ()
    {
    }

    virtual bool open    (const std::string & file_name, SFOpenMode mode) = 0;
    virtual int  read    (uint32_t section_id, void * buffer, int record_size, int record_count) = 0;
    virtual int  write   (uint32_t section_id, void * buffer, int record_size, int record_count) = 0;
    virtual bool close   () = 0;
    virtual bool refresh () = 0;
};

class EXPORT FileClassFactory
{
public:
    FileClassFactory ()
    {
    }

    ~FileClassFactory ()
    {
    }

    SectionedFile * CreateFileClass (int type);
    bool            DestroyFileClass (SectionedFile * filec);
};


//
//  StdIO_SectionedFile: store/load data onto StdIO
//

class StdIO_SectionedFile : public SectionedFile
{
public:
    StdIO_SectionedFile ()
        : _fp (NULL), _mode (SFMode_NULL)
    {
    }

    ~StdIO_SectionedFile ()
    {
        close ();
        section.clear ();
    }

    bool open  (const string & filename, SFOpenMode mode);
    int  read  (uint32_t section_id, void * buffer, int record_size, int record_count);
    int  write (uint32_t section_id, void * buffer, int record_size, int record_count);
    bool close ();
    bool refresh ();
    void error (const char * msg);

private:
    FILE * _fp;
    map<uint32_t,vector<uint8_t> > section;
    SFOpenMode _mode;
};

class EXPORT Compressor
{
public:
    size_t compress (uint8_t *source, uint8_t *dest, size_t size);
};

class EXPORT LogFileManager
{
public:
	static FILE *open (char *prefix)
    {
        // prefix too long
        if (strlen (prefix) >= 200)
            return NULL;

        FILE *fp = NULL;     
        int count = 1;

        char filename[255];

        do 
        {
            // generate filename
            sprintf (filename, "%s-%03d.log", prefix, count);

            if ((fp = fopen (filename, "rt")) == NULL)
            {
                if ((fp = fopen (filename, "w+t")) == NULL)                    
                    printf ("cannot open log file '%s', may not have write access to current directory\n", filename);
            }
            else
            {
                fclose (fp);
                fp = NULL;
            }
                
            count++;
        }  
        while (fp == NULL);

        return fp;
    }

    static bool close (FILE *fp)
    {
        if (fp != NULL)
        {
            fclose (fp);
            return true;
        }
        else 
            return false;
    }
};

//
// A helper class that allows the registeration and query of time budget left
// based on actual time detected by ACE
// 
// it uses the Singleton Design Pattern as described in:
// http://en.wikipedia.org/wiki/Singleton_pattern#C.2B.2B
//
class EXPORT TimeMonitor 
{
public:
    TimeMonitor ();
    ~TimeMonitor () {}

    // set how much time is left (in millisecond)
    void setBudget (int time_budget);

    // still time available? return time left (in millisecond)
    // return 0 for no more time, 
    //     (-1) for unlimited time
    int available ();

    // return a global instance of TimeMonitor
    static TimeMonitor &getInstance ();

private:
    static TimeMonitor _instance;   // static instance of the class

    long long          _budget;     // time left in current budget
    long long          _start;      // start time of setting the budget
    bool               _first;      // budget has just been set
};



} // end namespace Vast

#endif /* _VAST_UTIL_H */

