#pragma once

// Contains only global app compilation pre-processor commands

/*
 * USE_GLFW 
 * Turn on this option to switch from "Fucking WINAPI" to GLFW
 *
 * USE_RESPONSIVE
 * Turn on this option to use the "responsive OpenGL window" mode (the only one that works)
 * If this option is turned off, all hell breaks loose.  (5 attempts to get this right, failed)
 */

//#define USE_GLFW 1
#ifndef USE_GLFW
#define USE_RESPONSIVE 1
#endif

/*
 * Options: using_opengl, dont_use_glew, use_dhpowareGL
 * Options used in GLSetup.h that effect the use of dhpowareGL (turned off a long time ago)
 */

/*
 * GL DEBUGGING FEATURE
 *
 * GL_DEBUG, REPORT_ALL_ERRORS
 * The above line locally enables debug strings and is here specifically
 * due to glPop() hacky frowned upon but necessary implementation.
 * Note: glPop() swallows internal errors.  These may be indicators of a
 * problem in OpenGL code being run before glPop() is called.  To see
 * this oddity, define REPORT_ALL_ERRORS.
 * Affects all files and GL_Report etc listed in GLWindow.h
 */

//#define GL_DEBUG 1
//#define REPORT_ALL_ERRORS 1

/*
 * SEVERELY_DEPRECATED_GL
 * Turns on a very old piece of GL that we don't use anymore.
 */


/*
 * USE_LOGITECH_KIT
 * Turns on and off the support for LOGITECH KIT v1.1
 */
