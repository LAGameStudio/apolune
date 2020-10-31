Lost Astronaut's Apolune README
Thanks for supporting

.s5SSSs.                                                              
      SS. .s5SSSs.  .s5SSSs.  .s        .s    s.  .s    s.  .s5SSSs.  
sS    S%S       SS.       SS.                 SS.       SS.       SS. 
SS    S%S sS    S%S sS    S%S sS        sS    S%S sSs.  S%S sS    `:; 
SSSs. S%S SS .sS::' SS    S%S SS        SS    S%S SS `S.S%S SSSs.     
SS    S%S SS        SS    S%S SS        SS    S%S SS  `sS%S SS        
SS    `:; SS        SS    `:; SS        SS    `:; SS    `:; SS        
SS    ;,. SS        SS    ;,. SS    ;,. SS    ;,. SS    ;,. SS    ;,. 
:;    ;:' `:        `:;;;;;:' `:;;;;;:' `:;;;;;:' :;    ;:' `:;;;;;:' 
                                                                      

Keyboard Commands
-----------------

The Game Menu (PLAY, EXIT) can be invoked by:

To Start a new game: ENTER KEY or either CTRL (Control Key)
To Exit the game:    ESC, (`) "Back Tick" / "Tilde" or BKSP (Backspace)

The Game:

Sharing a keyboard or standard single player controls
-----------------------------------------------------

Navigator side of keyboard:      Gunner side of keyboard:

WASD    MOVE                     Arrow Keys	TURN / FLY
LCTRL   SHOOT                    RCTRL		SHOOT


Alternative keyboard* keys to map to multiple controllers:
,. [] <>		TURN LEFT/RIGHT
BKSP (BACKSPACE)	EXIT

Numpad 4,6 (like left and right arrows)
Numpad 8,2 (like up and down arrows, or W S)

Second player option
--------------------

To unlock second player, during first generation (first 30
kills) hit the Y or U keys.  Two player games get their
own leaderboard, separate from the single player leaderboards.
Second player is tethered to first player and both
die simultaneously.

Player 2 controls for controller mapping:
IJKL (equivalent to arrow keys)
FTGH (equivalent to WASD)
YU   (LCTRL and RCTRL)

* Attaching a second keyboard might work


Using a Controller
------------------

There is a recommended controller setting for Generic USB Controller.
It requires you to have dual triggers like the iBuffalo Classic USB Gamepad.
It also works with the Steam Controller.  The first 200 kills are easiest
using the keyboard, however, unless your keyboard sucks.

Advanced Launch Options
-----------------------

Did you know you can run Apolune at a variety of resolutions in Fullscreen or
Windowed mode?  

If you right click the application in the STEAM Client, you can use the
Advanced Launch Options to enter one or more options from the list below:

Please note that all of these options provided as command line parameters to
Apolune.exe should precede with a single minus sign (-).

For example, "-windowed" works but "window" won't work without the minus sign.

For example, to load in 720p mode in a window, enter the following into the
Steam client "ADVANCED LAUNCH OPTIONS" textbox:

	-windowed -720p

Order does not matter, so this will also work:

	-720p -windowed

Window Options

	-window					Disables use of full screen when creating the OpenGL application window.
	-windowed				Same as above.

	-borderless				Create a borderless full screen window instead of GPU mode full screen

	-bordered				Create a bordered full screen window instead of GPU mode full screen

Engine Options

	-nolimit
			Do not limit the frame rate (will speed up time on some systems,
			otherwise the frame sleep will fill frames that don't take up 17ms at 60FPS

Specific Resolutions

 -320x200	-720x480
 -1152x768	-1280x1024
 -1600x1200
	-320x240
 -854x480	-1280x720
 -1366x768	-1768x992

 -320x400	-768x576
 -1280x768	-1400x1050
 -1920x1080
	-320x486
 -800x600	-1280x800
 -1440x900	-480x320
 -1024x600	-1280x854
 -1440x960	-640x480
 -1024x768	-1280x960
 -1680x1050 			

HD Resolutions by Trade Name

	-hd720   
	-hd720p  
	-720p    
	
	Sets 1280x768 (HD720p) as the resolution.

	-hd1080 
	-1080i  
	-1080p  
	-1080 
	
	Sets 1920x1080 (HD 1080i) as the resolution

Specialty Resolutions

	-cga      		Sets 320x200 (CGA) as the resolution                 
	-tv       		Sets 320x400 (Classic scanline TV) as the resolution 
	-qvga     		Sets 320x240 (QVGA) as the resolution                
	-320x243  		Sets 320x243 (pseudo NTSC) as the resolution         
	-ntsc     		Sets 320x486 (NTSC non-interlaced) as the resolution 
	-pal      		Sets 768x576 (PAL) as the resolution                 
	-vga      		Sets 640x480 (VGA) as the resolution                 
	-800x480  		Sets 800x480 (WVGA) as the resolution                
	-wvga     		Sets 854x480 (WVGA) as the resolution                
	-svga     		Sets 800x600 (SVGA) as the resolution                
	-wsvga    		Sets 1024x600 (WSVGA) as the resolution              
	-xga      		Sets 1024x768 (XGA) as the resolution                
	-wxga     		Sets 1280x768 (WXGA) as the resolution               
	-sxga     		Sets 1280x1024 (SXGA) as the resolution              
	-sxga+    		Sets 1400x1050 (SXGA+) as the resolution             
	-wsxga+   		Sets 1680x1050 (WSXGA+) as the resolution            
	-uxga     		Sets 1600x1200 (UXGA) as the resolution              
	-qxga     		Sets 2048x1536 (QXGA) as the resolution              
	-cinema   		Sets 2560x1440 (Apple Cinema T) as the resolution    
	-wqxga    		Sets 2560x1600 (WQXGA) as the resolution             
	-qsxga    		Sets 2560x2048 (WSXGA) as the resolution             

Support for Matrox Dual- and TripleHead2Go

	-32godual1		1600x600 DualHead2Go Dual Mode 1      
	-32godual2 		2048x768 DualHead2Go Dual Mode 2      
	-32godual3 		2560x720 DualHead2Go Dual Mode 3      
	-32godual4 		2560x768 DualHead2Go Dual Mode 4      
	-32godual5 		2560x800 DualHead2Go Dual Mode 5      
	-32godual6 		2560x800 DualHead2Go Dual Mode 6      
	-32godual7 		2560x960 DualHead2Go Dual Mode 7      
	-32godual8 		2560x1024 DualHead2Go Dual Mode 8     
	-32godual9 		2720x768 DualHead2Go Dual Mode 9      
	-32godual10		2800x1050 DualHead2Go Dual Mode 10    
	-32godual11		3200x900 DualHead2Go Dual Mode 11     
	-32godual12		3200x1200 DualHead2Go Dual Mode 12    
	-32godual13		3360x1050 DualHead2Go Dual Mode 13    
	-32godual14		3840x1080 DualHead2Go Dual Mode 14    
	-32godual15		3840x1200 DualHead2Go Dual Mode 15    
	-32godual16		4096x1152 DualHead2Go Dual Mode 16    
	-32godual17		4096x1152 DualHead2Go Dual Mode 17    
	-32gotri1  		1920x480 TripleHead2Go Triple Mode 1  
	-32gotri2  		2400x480 TripleHead2Go Triple Mode 2  
	-32gotri3  		2400x600 TripleHead2Go Triple Mode 3  
	-32gotri4  		3072x768 TripleHead2Go Triple Mode 4  
	-32gotri5  		3840x720 TripleHead2Go Triple Mode 5  
	-32gotri6  		3840x768 TripleHead2Go Triple Mode 6  
	-32gotri7  		3840x800 TripleHead2Go Triple Mode 7  
	-32gotri8  		3840x960 TripleHead2Go Triple Mode 8  
	-32gotri9  		3840x1024 TripleHead2Go Triple Mode 9 
	-32gotri10 		4080x768 TripleHead2Go Triple Mode 10 
	-32gotri11 		4098x768 TripleHead2Go Triple Mode 11 
	-32gotri12 		4200x1050 TripleHead2Go Triple Mode 12
	-32gotri13 		4320x900 TripleHead2Go Triple Mode 13 
	-32gotri14 		4800x900 TripleHead2Go Triple Mode 14 
	-32gotri15 		5040x1050 TripleHead2Go Triple Mode 15
	-32gotri16 		5760x1080 TripleHead2Go Triple Mode 16

Activate cheat codes and commands
(will NOT set the Leaderboard and most achievements)

-cheating



