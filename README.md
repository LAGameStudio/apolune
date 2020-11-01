Apolune and the Lost Astronaut Game Creation Framework
======================================================

This is the Lost Astronaut Game Creation Framework ...

![alt text](https://github.com/LAGameStudio/apolune/blob/trunk/Apolune/data/images/splash.jpg?raw=true)

... along with the commercial Apolune Game (artwork reduced from commercially available version) as an implementation example, organized as a Visual Studio (2017) solution.

![alt text](https://github.com/LAGameStudio/apolune/blob/trunk/Apolune/data/images/title.png?raw=true)

Written in Visual C++ for Win32 using WinAPI as a WinProc Application.  Does not use SDL.  External dependencies kept to a minimum.

This game creation framework has taken well over 10 years of my life to develop.  At various times it has been licensed to companies that I managed, for projects I developed, in the commercial computer graphics space.  In particular one company, PieceMaker Technologies Inc., of which I held an equity stake, allowed me to reuse non-application-level code I developed, so in this way the framework has had some indirect funding applied to it.

Lost Astronaut Studios (lostastronaut.com) will adapt portions of this code for use in a new proprietary game engine project, but I am releasing it because:

1) It's in a space that always needs more input, so it adds value to the spaces it interacts with (software, image analysis, automation, graphics and gaming)
2) It would be useful for third parties who wish to produce transparent research and small independent developers who wish to learn and make games
3) It is solely a Windows targetted project, and is stuck in the Win32 legacy, and when the project started Windows Desktop Users represented over 76% of all gamers, but today that number is more like 24%.   While useful for producing Windows Desktop games, it wouldn't easily be ported to any other platform including the Xbox (though it might work on the Xbox with some effort).
4) It is built on the aging OpenGL library, a tried-but-true-but-deprecated library from Khronos, and thus is stuck in the throws of that beast
5) It contains over a decade of distilled knowledge and shows an evolution in home PC entertainment
6) It is useful for makers and DIY enthusiasts
7) It is powerful and will empower people who choose to learn it

Come find us on our Discord.  DM "@Retrofriends" to start a conversation with the author.  https://discord.gg/tUvdnxJS
## Donations and Contribution

### If you download this project, please Star it.   

### If you use this project, please Watch it.

### If you care about this project, please Sponsor it, or Contribute (see CONTRIBUTING.md for more information)

As the primary author, initiator and only vested official team member, I do appreciate donations and any donations will be used to fund making of video tutorials, documentation and code cleanup.  

Another great way to support the project is to buy our games on STEAM, which you may find by visiting LostAstronaut.com or by searching steam for "Apolune" and looking for "Lost Astronaut Studios" on the Store pages.

### Feel free to make requests by reporting issues, as they will be considered and appropriately prioritized.

### Fork us

Feel free to create Pull requests with fixes and suggested improvements only after you have tested them.  Explain why you are making the request in the comment section of the request.  We have a lax but optional Pull Request Template.

## License

Any original artwork not licensed elsewhere and unique to this software is licensed under CC BY NC ND 4.0 and should be replaced if you wish to make a commercial product.  This artwork is found in the ``/data`` folder (https://github.com/LAGameStudio/apolune/tree/trunk/Apolune/data)

As it states in the LICENSE file, this work is licensed, unless otherwise indicated, under the AGPL V3 or greater, providing a framework for creating additional open source and is rather permissive.  If you wish to purchase an exception to this license, feel free to contact the author directly.

The licensed does not cover third party code, snippets and libraries, only the work unique to this framework.  Most of the other work is either in the public domain or, more commonly, in an MIT or MIT-compatible license (some are BSD or 3 Clause).  The library inventory was vetted by attorneys in 2015 when it was licensed to PieceMaker and used in a retail setting at Toys-R-Us for commercial purposes, so it has already been combed for any copyright violations or patent infringements.

I politely request that if you use our framework to make a product, game or otherwise, please include the Lost Astronaut Game Creation Framework splash screen among your logos and other marketing material.  You can find the highest quality copy of this splash screen image in ``Apolune/data/images/splash.png``  You are also welcome to isolate the center portion as a "sticker style label" if you don't want to use the whole screen.

Crediting the author, H. Elwood Gilliland III in your documentation and/or credits would also be nice.  Also, linking to this repository would be nice.

Note that if you remove a section you should not remove any copyright notices.  If a file or section of a file that is desired to be used in another project does not have a copyright notice, then, to assure compliance, please use the following snippet and place it at the beginning of the section or file:

```
--- SNIPPET STARTS
/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 * This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
 * See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
 *********************************************************************************************/
--- SNIPPET ENDS
```


## Features
A massive 1800+ class 1400 file extravaganza of OpenGL, WinAPI, OpenAL and many other features, written in Visual Studio for over a decade!  

### Technicals:
* Supports building all the way back to Win 7 (Win32 only)
* Written entirely in Visual Studio (Visual C++)
* OpenAL (Audio Layer)
* OpenGL and GLSL (Graphics Layer)
* Unified device interface: Supports Multitouch (WinTouch), XInput, Direct Input, dinput8 (live keyboard detections), Mouse, Logitech Kit (Legacy option)
* WinAPI (WinProc) and Messaging, Windows Dialogs, and COM (for OS dialog support)
* Reimplementations in Integer, Float and Double precision
* Built on ultra-fast LinkedLists and ZIndexed and ultra-lightweight ZeroTypes Library (https://zero.handmade.network) (no need for BOOST)
* Textural datafiles in JSON and/or a proprietary "HDATA" format reminiscent of Poser files

### OpenGL Implementations:
* GLSetup Threaded-Texture-Loading OpenGL 3.1 compatibility context (for widest support)
* Texture uploading
* Full shaded GLSL integration
* VBOs, IBOs, EBOs
* FBOs (multisample)
* DisplayLists (deprecated) and pBuffer (deprecated!)
* GLWindow "OpenGL Extensible Windowing System" that ties to interface events

### Image support:
* GDI+ (PNG and JPG read/write)
* Procedural image processing and generation support
* CPU image manipulation
* GPU texture uploads
* GLImage texture library (TextureLibrary, management as global singleton)
* Fit, stretch, and other types of real-time manipulation
* Grid-based texture manipulation
* Atlas textures

### Animation Features:
* Curves and paths
* AStar pathfinding
* Tweens and easing (Waveform animation)
* Enhanced 2D and 2.5D collision, and 3D collisions
* Several particle effect libraries
* Box2D implementation for use with anything
* Platformer tiles
* 2D Top-Down Navigation AI
* Many useful programmable and configurable implementions of visual effects like cracking, lightning, destruction, breaking, fading
* Presentation / Slide classes implementing a way to do programmatic interactive and cut-scene movies

### Geometry:
* STL, OBJ, PLY
* Generative primitives in 3D
* Cube-mapped Sphere Generation
* Extensive Vertex, mPolygon and ImmediateModel support classes for handling 3D geometry
* Cartesian 2D helper and its family of related classes
* Semi-automatic element "caching"
* Rounded rectangle generation
* Bin packing
* Z-sorting for 2.5D

### Fonts:

* Texture atlas fonts (ATFont)
* Bitmap texture fonts (TexFont)
* 2D vector font format (Art2D)
* 3D extruded font format (TTFEx font style)

### UI:
* Proce55ing-style object-oriented (Proce55or, Proce55ors, Pro55essViewer including buttons, sliders, text boxes, NPCs etc)
* Panning 2D environments utilizing UI elements as framework
* Wrappable worlds
* Zone-activated ultra-large 2D worlds
* Customizable OpenGL implemented UI framework options:  GLWindow (first-tier game panel framework), "FastGUI" function-based UI programming, "EditGUI" (window-based) elemental semi-auto UI framework

### Audio:
* OpenAL Community Edition
* Source loading, mixing
* AI Disc Jockeying with crossfades
* Long-form audio streams
* FLAC, WAV, etc via LibSndFile
* EFX/EAX (Legacy Creative Sound)

### Networking and threading:
* RESTful support
* VAST peer-to-peer support
* Sockets
* WinThreads and fallback to pThreads (legacy)

### Shaders:
* Entire Orange Book and Red Book implementations (legacy shaders)
* Bloom
* Photoshop-like layering shaders
* 3D Normal Mapping, Phong Light Model
* Shadow Shaders for 2D
* Multilight Shaders
* Over 100 individual effects!!

### Collision
* FBO Depth Stencil with Readback for "pixel perfect 2D collision"
* Trigger.h which includes lots of useful ways of detecting collision including "shooting through a circle", "box-line" "line-line" "circle-line" tests.

### Utilities:
* 1D/2D Vector (Indexed / ZIndexed)
* Auto-zeroed POD-non-POD classes (ZeroTypes)
* String manipulation and handling of Strings (linkedlist) and KV pairs
* Command line options wrangling
* Macro-based class building supporting ONE-MANY-DONE mantra and the ability to easily add "to/from" string support for files and network interfaces
* Math math and more math!
* Extensive random numbers support including random.org atmospheric randomizations and pseudorandoms
* Roguelike dungeon generation

Getting Started
===============
Select "Release" mode for 32 bit.  64 bit does not build and will not work.

Clean.  After you are done with "Build All", copy the contents of DLLs to the target folder (probably SolutionDir/Release)

The project filters are carefully organized.

"Application" is your application, and most likely the work you will do will involve adding new files to the "Game" filter/folder under this.  When you first download this repo, the "Game" filter contains the Apolune game.  You can modify this to get started, or you can stick all of those subfilters into Apolune, then create new ones.  The entire application is built off the singleton "windows" -- to that you add derivatives of the GLWindow class, and those can contain many of the other classes, including other derivatives of GLWindow, which you can "hook in" to your GLWindow. You can also switch between GLWindows and you can also build distinct pieces of UI as overlay using multiple GLWindows.

"Framework" contains all of the utilities, helper classes, OpenGL, OpenAL and other middleware classes.  Worth exploring.  It is organized by purpose.  Skimming through the files here is pretty important.  Take inventory of your massive graphics encyclopedia and decide what "legos" you want to plug in to make your next user experience.

### The project configurations are as follows:

* Debug  -- not useful unless you want a lot of Output
* Deug QDbyZ GL_DEBUG  -- for granular debugging of OpenGL without a third-party debugger, used rarely
* Debug Quiet Divide by Zero Warning  -- use most often to debug, this is your "debug mode", used commonly
* Release -- a finely optimized release copy
* Release 2 -- when all issues are fixed in release, this interim Release stage should be tested before Production, in most cases can be skipped
* Production -- when you are ready to release

There are variants that offer features like Logging, Live Edit, Unoptimized, etc.  Some are more useful than others.

No real use of namespaces.  It's all :: most of the time (global namespace)

## Getting your feet wet
Data files are stored in SolutionDir/Apolune/data

Do not manipulate main.cpp, and most likely not Application or AppFramework classes

Play around with GLWindow derivatives:
* Locate the file "Load.cpp"
* Locate the part of Load.cpp at the bottom (function is called LoadWindows)
* Comment out windows.Add(new GameMenu) (turns off Apolune game)
* Uncomment one of the other windows.Add like BestRectTest or AtlasFontViewer or SmokeTest
* Once you rebuild and play around, you'll realize that those are the individual windows/panels.  Inspect them.  See how they are implemented.  Look in the Tests filter of the project solution under "Examples and Tests" in "Framework" -- you can play around with these individually by loading them into the windows list.

### Use of Global Singletons
I looked at each new component as one that was global and reusable.  The main singletons are listed here:

* audio - the audio API
* input - the unified interface class
* display - the display information (window size)
* windows - implements the GLWindowManager class and is the primary way to create new views derived from GLWindow
* library - the texture library, often invoked as  ``GLImage *myTexture = library.Load("somefilename.png");``
* art - Implements the TwoDeeElement artistic VBO wrangler, and has lots of useful functions to draw these elements
* many more!

### Use of Functional Libraries
Some files implement drawing and other routines as functional calls or macros, rather than organizing them into a class.  These are listed here

* art.h
* string_functions.h
* moremath.h
* macros.h

### Data Wrangling Classes
* Strings, Numbers, Integers, Doubles, Floats -- for maintaining lists
* ZIndexed and Indexed - two styles of the same lightweight vector, also StringHash (for string-indexed hashtable with fast lookup)
* Tweens and Tween, for waveform library storage and retrieval for use with animations
* Curve2d, Line, Cartesian, Box, Vertex / Vertexd
* Vertex and mPolygon work with ImmediateModel - for handling 3D geometry, also see Primitive3d

### Proce55ing and Particles
* Proce55or, and its various flavors, for creating UIs and 2D Game Elements (also known as "Entities")
* Proce55ors, Proce55Viewer and flavors like Zoned, Wrapping, ForeverWrapping -- mainly for scene management
* FireAndForget* one and done managed particles
* Particle, Particles, ParticleSystem, ParticleSystems

### Cameras
There are multiple cameras, one in particular is GLMCamera which is built on GLM.   There are others that were useful for specific implementations, now older and less used.

### Shaders
* GLSLShader (main shader derivable class), wrangles uniforms, binding, activation, deactivation
* ``*Shader``  -- any shader implementation, such as BloomShader
* Too numerous to list here...

## Dependencies (DLLs)

There are many.  Here they are in a nutshell:

* pThreads (deprecated, now uses WinThreads)
* OpenAL (ALURE32)
* OpenGL (GLEW, GLFW, GLM, GLU)
* Xinput
* CURL (optional), including libeay32, SSL, libsasl, zlib
* AV Codec (for video)
* Freetype, used in one area but not so much anymore (TTFEx)
* libsndfile, libflac, libogg, no longer directly used: libpng, libjpg, jpeg62
* libspeex (not currently used)
* LgKit1.1 (for Logitech devices, legacy)
* SDL and SDL_Image (no longer directly used)
* steam_api and steamclient
* VAST and VASTd (for VAST peer-to-peer networking, not actively used anymore)
* libtiff

## Project History
Let's go through a brief history of this project, which, until 10/30/2020 was completely done in secret as closed source. 

### 2005
I do some work in Processing and make some small games and audiovisual helpers.

### 2008
When I first developed this engine, it was written in pure C using a toolkit called Agar GUI.  I had been using Agar GUI for a postdoctoral research project called Avenzoar Digital Pathology Tool (https://sourceforge.net/projects/avenzoar) and after that appointment concluded I wanted to develop a game called Interstellar Empire as a "quick 6 month project" that extended earlier work I had done using Processing IDE (Java).

### 2009
Frustrated with the extremely odd world of Agar and remembering now why I stopped using C to develop games, I discovered "Enhanced C" and the Ecere SDK.  The lure of this toolkit was that I could agnostically develop for Linux and Windows using a more modern language (and quite a nifty one at that) and with the flip of a switch support either OpenGL or DirectX. I quickly moved the engine into "eC" (Enhanced C) (https://ecere.org) -- but after about a year of learning OpenGL and eC, I realized I needed to take the plunge and learn C++ because Ecere had sparce support compared to what I felt were "well funded and organized" commercial platforms like Microsoft Visual Studio.

### 2010

After transliterating all of the work from eC to C++ and filling in most of the requirements by developing an OpenGL engine "from a single triangle demo" in Visual Studio, I decided I would develop a game called Empire in the Sky (https://lostastronaut.com/empire).    It was around this time that I released libALgore, a joke on the name of former vice president of the USA, for the OpenAL ecosystem, which implemented an object-oriented approach to libALURE in C++.

At the time I was using the first "free" version of Visual Studio, called MS Visual C++ 2010 Express Edition.  Unfortunately, this "freebie" by Microsoft had an inherent flaw: the team at Microsoft had inadvertently colluded C# rules with C++ rules.  And so, even after several years of effort, I kept running into undefined behavior.  After I got in touch with Microsoft, three VPs apologized and gave me "lifetime professional level support" for my fledgling game development company.  They also lead me through a series of complex patches to fix the problem, but ultimately I moved on to newer versions of Visual Studio.

### 2013
Three years had passed since I began work in Visual Studio.  My "six month game project" was nowhere near complete.  By this time, Empire in the Sky (EITS) and its home, Desura, had questionable futures.  EITS contains some of my best visual work with spheres and planetoids, and some of the more complicated work I've done in 3D editors, but I hit a wall that is probably familiar to players of No Man Sky.  I realized that a procedurally generated universe was either too broad of a scope for one lone game developer to muster, or that it would take 20 years to complete.  I learned a powerful lesson about art and procedurally generated worlds at this time.  I decided I would "pause" EITS in lieu of completing one "traditional space game" title, a prequel, called Fringes of the Empire.  I had learned an aweful lot about graphics and 3D.  And, after many trials and challenges, I had an engine that's code was a flawless diamond.

### 2014
Fringes of the Empire is in development.  I decided that -- perhaps I was wrong in creating my own engine.  Perhaps I had overextended myself?  Bloggers had advised against it.  Maybe I was too determined?  I needed support.  So, I bought GameMaker for a wopping $500, thinking I would skip all that engine development hell and in 1 year produce a game close to what I was hoping.  It was to be an RPG in space only (scoping it back, roping it in) and it was going to be all pre-designed levels for a single player experience.  

Well, GameMaker 1.2 was horribly buggy, too!  I spent countless months talking to GameMaker's developer YoyoGames.  I hoped they would hire me, since I could make engines and had farther-reaching ideas than they would even believe!  I did end up influencing GM:S 2, but those lousy bastards wouldn't even give me a free upgrade and discontinued GM:S 1.2 (the version I had bought) which in my opinion broke their promises for lifetime updates.  They said it was technically only for GMS version 1.2 !!!!  WHAT!!!   See it:  https://www.youtube.com/watch?v=v61uaki9GZw&ab_channel=HerbertGilliland

After 1 year of working in GameMaker, I couldn't do it anymore.  I scrapped it and decided to rewrite everything back in this game framework again.  I wanted to finish the game!

### 2015
This year I licensed the engine to PieceMaker Technologies Inc., to build creative 3D printing kiosks gamified for the Toys-R-Us retail space.  Entire branches of source code (still private) built out an award-winning 3D system for retail environments that won Best in Toy Fair 2016.

I give up on GameMaker, but continue to enhance the framework and begin creating a new "Fringes" in C++.

### 2017
I take a two year haitus while working on Poser 3D: Character Art and Animation Studio.  I implement more 4.x style OpenGL VBO functionality eliminating most of the older 

### 2020
I decide to release the project on Github.  Happy Devil's Night 10/30/2020
