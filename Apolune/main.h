/*! \mainpage It's An Engine Documentation
 * \copyright 2006-2015 H. Elwood Gilliland III
 *
 * \section intro_sec Introduction
 *
 * Welcome to the official documentation for \b It's \b An \b Engine - a substrate
 * material for developing C++ applications available under a non-profit
 * open-source license that has a commercial option.
 *
 * \section history_sec Project History
 *
 * Began in 2006 as a Linux C project, \b It's \b An \b Engine is a massive
 * collection of C++ graphics classes that work together under a common
 * class foundation that is ultra-portable and is geared for multithreaded
 * real-time Desktop systems.
 *
 * Originally distributed as the Open Art Library and Ecere's Open Art Module,
 * it has grown to over 1800 classes and 300,000 lines of \c code that covers
 * a wide range of image analysis, graphics processing, 2D, 2.5D and 3D using
 * OpenGL and GLSL exclusively. 
 *
 * While primarily authored by H. Elwood Gilliland III (Carnegie Mellon, Pittsburgh, PA),
 * it includes the work of Jerome Jacovella-St-Louis (Ecere Corporation, Quebec), Emil
 * "Humus" Persson (ATI, Avalance Studios, Stockholm), Chris "kittycat" Robinson (OpenAL),
 * Christopher "Tesla" Tchou (Bungie), and Dr. Erin Catto (Blizzard, Box2D), as well as
 * a group of IRC regulars include the Geordi bot.  It is informed by and pays homage to
 * Processing.org though it is an unrelated project.
 *
 * Additionally it includes data from random.org
 *
 * \section overview_sec Overview
 *
 * Beyond just a library, the \b It's \b An \b Engine is a \a substrate of a potential
 * application in that it implements a \a wunderkammer or wonder-room of enlightened
 * functions that provide the user with a maximally optimized interface to powerful
 * functionality common to many applications.
 *
 * The downside to this large collection of substrate is that its infrastructure
 * adds a length of time to compile.  This time is constantly becoming shorter, but
 * this one caveat should be considered when moving forward.
 *
 * The upside, beyond the power of its utility, is the ability to develop amazing
 * applications fully utilizing the hardware in a way that is portable, friendly
 * to open source and commerce.
 *
 * \section more_sec More Than Just Graphics
 *
 * \b It's \b An \b Engine is more than just an OpenGL framework.  It provides essential
 * control over random numbers, simplified file operations, and is evolving into a fast
 * OpCode language with vast potential for greatly simplifying application development.
 *
 * - Foundation Classes Implement Auto-Initialized Variables
 * - Easy to use and versatile, robust objects that have high performance
 * - Multi-threading and thread-management
 * - Web and networking
 *
 * \section docinfo_sec How To Read This Documentation
 *
 * In the documentation, we will refer to "Engine" as the substrate material, "Game", "Product" or
 * "Application" as the growth on the substrate.  There is "reference", which is a broad term
 * generalizing a "relationship", and sometimes include references to evolutionary and geneaological 
 * terms such as "genus" and "specie" of classes, "family", "group", "cousin object", "daughter" or
 * "son", and "parent", "grand-parent" or simply "ancestor" and "lineage".
 *
 * We will also discuss "interface", meaning interface to OS-level functions at times,
 * user interface, at times hardware, and OOP interfaces.
 *
 * \section methods_sec Compiler Compatibility
 *
 * - Microsoft Visual C++ 2010
 * - CodeBlocks
 * - g++ compiler
 * 
 * \section methods_sec Necessary Methodologies
 *
 * In brief I will cover the development methods to be used:
 *
 * Method #1: Create Test and evolve Test during a second pass into a modular object, 
 * which can be then included in a GLWindow-derivative as part of product
 * functionality.
 *
 * Method #2: Create a Testing Framework as a Proce55Viewer*-derived object,
 * and then include Proce55or-derived, or other derivable object.
 *
 * Method #3: Develop new underlying interpretations of necessary features,
 * then include inside a pluralized objective framework for use as an
 * effect combined with other effects, inside a GLWindow-derivative, but
 * reusable inside other GLWindow-derivatives.
 *
 * Method #4: Don't override, write along side
 *
 * Method #5: Don't forget your default constructor.  Don't override constructors,
 * instead build additional ones and chain together when appropriate.
 *
 * Method #6: Variable names are chosen carefully.
 *
 * Method #7: Almost everything is \b public
 *
 * \section gstarted_sec Getting Started
 *
 * Take a look at Load(), where everything begins.  Explore.
 */
/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
/* do not include this file in any other file than main.cpp */