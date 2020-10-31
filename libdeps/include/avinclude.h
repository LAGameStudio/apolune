#pragma once

/* avinclude.h
 * ----------- 
 * Author: E. Pronk
 * Descr.: Includes everything needed for AV decoding
 */

#ifndef LMTZ_AVINCLUDE_H
#define LMTZ_AVINCLUDE_H

#define __STDC_CONSTANT_MACROS

#pragma comment(lib, "libavcodec/avcodec.lib")
#pragma comment(lib, "libavdevice/avdevice.lib")
#pragma comment(lib, "libavfilter/avfilter.lib")
#pragma comment(lib, "libavformat/avformat.lib")
#pragma comment(lib, "libavutil/avutil.lib")
#pragma comment(lib, "libswscale/swscale.lib")

extern "C" 
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
}

#endif