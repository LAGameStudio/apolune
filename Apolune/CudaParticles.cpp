#include "CudaParticles.h"

//#include <cuda_runtime.h>
//#include <cuda_gl_interop.h>
//#include <vector_types.h>

#if defined(NEVER)
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			CUDA ACCELERATED PARTICLE ENGINE

	Written By Craig Mouser, (http://www.craigmouser.com)
	in April of 2010, 
	using CUDA 3.0 and Visual Studio 2008

	This program is a simple particle engine, it only uses x and y
	coordinates, but renders as though it were all 3. This is shown by
	right clicking to rotate the view. A force is applied at 
	the mouse location whenever the left button is clicked.

	This was loosely based off the SimpleGL example from the CUDA
	SDK. I used it as a starting point which made it easy to adjust
	the code to my needs.

	Feel free to adapt, modify, duplicate, share this code
	all you want. This was meant as a learning experience for
	me, and I hope that it helps you too.

	Inspiration: http://www.youtube.com/watch?v=CyAZ2Y7nOTw
	I was hoping for 750,001 can successfully run over 4 million!
	Two years later, shows how much technology has improved.

	Files:
	common.h: holds structures and defintions all files need
	cudaParticles.cpp: does basically everything but the CUDA part
	particles_kernel.cu: holds the CUDA functions and one c++ function
						 that that calls the CUDA functions
	settings.txt: A place to load settings so the user can easily adjust
				  the number of particles, etc.

	Libraries used:
	CUDA 3.0: http://developer.nvidia.com/object/cuda_3_0_downloads.html
	OPENGL: http://www.opengl.org/sdk/
	GLUT: http://www.opengl.org/resources/libraries/glut/
	GLEW: http://glew.sourceforge.net/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//Windows specific statemetns
#ifdef _WIN32
#  define WINDOWS_LEAN_AND_MEAN
#  define NOMINMAX
#  include <windows.h>
#endif

// Include Statements
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "GLSetup.h"

//Apple gets a special directory
#if defined (__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


//Define Statements
#define MINCOLOR		0.1f							//The lowest any RGB value can go (RGB values range from 0.0 - 1.0)
#define MAXCOLOR		0.6f							//The highest any RGB value can go 
#define COLORSPEED		0.5f							//A multiplier for how fast the colors change.
#define FONT			GLUT_BITMAP_HELVETICA_12		//The font used for drawing options

//------------------------------------------------------------------//
// Constants
//------------------------------------------------------------------//
const float BOX_SIZE_AT_480		= 0.01203125f;			//Constant used to set the width of the box based on the height and width of the screen
const int	Options				= 4;					//Number of options the user can control with arrows.

//------------------------------------------------------------------//
// Variables
//------------------------------------------------------------------//
int		winWidth = 1280;								//Window Width
int		winHeight = 768;								//Window Height

bool	mousePressed	= false;						//Holds whether the left mouse button is pressed
float2	mouseLocation;									//Holds the mouse location in the world.
int2	mouseScreenLocation;							//Holds the mouse location on the screen.
int		mouseButtons;									//Holds which buttons are currently pressed;

float	boxWidth;										//Width of the Collision Box
float	boxHeight;										//Height of the Colliison Box

int	ParticleCount			= (4096*1600);				//Number of particles 
float4	particleColor = make_float4(MAXCOLOR, MINCOLOR, MINCOLOR, 0.1f);//holds color for particle
float	ParticleSize		= 1.0f;						//Size of particle to be rendered
float	Gravity				= 0.98f;					//Gravitational acceleration
float	mouseForce			= 0.25f;					//The force the mouse applies
int		currentOption = 0;								//Holds the option to be displayed, 0 is none
bool	cycleColor = true;								//Cycles through colors when true, otherwise stays on current color


// VBO variables
GLuint vboPos;	//Position	
GLuint vboVel;	//Velocity


// mouse controls
float2 cameraRot = make_float2(0.0f, 0.0f);
float cameraZPos = -10.0;


//FPS Variables
int gameTime = 0;
int lastTime = 0;
int frames = 0;
float dt;

//------------------------------------------------------------------//
// Prototypes
//------------------------------------------------------------------//

//Kernel wrapper prototype
extern "C" 
void launch_kernel(float2* pos, float2* vel, float dt, unsigned int particleCount, float height, float width, int Function,
				   float mouseForce, float Gravity, bool mousePressed, float2 mouseLocation);

//Prototypes
CUTBoolean runTest(int argc, char** argv);
void cleanup();
void getColor();
void printInstructions();
void DrawControls(int x, int y);
void UpdateControls(bool plus);
void runCuda(GLuint vboPos, GLuint vboVel, float dt, int Function);
void computeFPS();
bool readSettings();


//GL Functionality Prototypes
CUTBoolean initGL(int argc, char** argv);
void createVBO(GLuint* pos, GLuint* vel);
void deleteVBO(GLuint* pos, GLuint* vel);
void renderBitmapString(float x, float y, char *string);
void setOrthographicProjection();
void resetPerspectiveProjection();

//GL Callback Prototypes
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void keyboardSpecial(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);


//------------------------------------------------------------------//
//							 Main Function
//------------------------------------------------------------------//
int main(int argc, char** argv)
{
	printf("--------------------------------\n");
	printf("CUDA Accelerated Particle Engine\n");
	printf("--------------------------------\n");

	printf("\n");

	boxHeight = BOX_SIZE_AT_480 * 480;			//Calculate Height for collision box(Guessed and checked at 480, made up a formula that works)
	boxWidth  = boxHeight*winWidth/winHeight;	//Calculate Width for collision box
	
	// First initialize OpenGL context, so we can properly set the GL for CUDA.
	// This is necessary in order to achieve optimal performance with OpenGL/CUDA interop.
	if(readSettings() && initGL(argc, argv))
	{
		if(runTest(argc, argv))
		{
			cudaGLSetGLDevice(cutGetMaxGflopsDeviceId());//It's only set up to use 1 device, so get the fastest one

			// create VBO
			createVBO(&vboPos, &vboVel);

			//Initialize particles
			printf("Initializing %d particles...\n", ParticleCount);
			int size = ParticleCount*sizeof(particle);							//Only used to display how many KB it the particles use
			printf("Particles are currently using %d kilobytes.\n", size/1024);
			runCuda(vboPos, vboVel, dt, FUNC_INIT);

			printInstructions();												//Prints the instructions to the console window

			// start rendering mainloop
			glutMainLoop();

			cudaThreadExit();
		}
	}

    cutilExit(argc, argv);
}

//---------------------------------------------------------------------------------------------------//
/*---------------------------------------------------------------------------- Initializing ---------*/
//---------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------//
//							Init GL
// DESCRIPTION: Initializes OpenGL
// RETURNS:     True if a OpenGL is supported
//              False if OpenGL can't be used
//------------------------------------------------------------------//
CUTBoolean initGL(int argc, char **argv)
{
	//Gets the entire main monitors Width and Height
	int screenWidth = glutGet(GLUT_SCREEN_WIDTH);					
	int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);					

	//Calculates where the top Left corner of the window goes to center it on the monitor.
	int offsetW = (screenWidth - winWidth) / 2;						//Horizontally
	int offsetH = (screenHeight - winHeight) / 2;					//Vertically

	// Glut Code
    glutInit(&argc, argv);											//Init GLUT library
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);					//Set display mode to double buffered RGBA
	glutInitWindowPosition(offsetW, offsetH);						//Position the window using the above locations
    glutInitWindowSize(winWidth, winHeight);						//Set the window's height and width
    glutCreateWindow("CUDA Accelerated Particle Engine:");			//Create a window

	//The following set which functions handle events
	glutDisplayFunc(display);										//Draws things to screen
	glutReshapeFunc (reshape);										//If someone resizes the window
    glutKeyboardFunc(keyboard);										//If an ascii key is pressed
	glutSpecialFunc(keyboardSpecial);								//If something else is pressed (i.e. shift, alt, etc.)
	glutMouseFunc(mouse);											//Mouse Clicks
    glutMotionFunc(motion);											//Mouse dragging

	// initialize necessary OpenGL extensions
    glewInit();
    if (! glewIsSupported("GL_VERSION_2_0 ")) {						//checks if the required core or extension functionality is available
        fprintf(stderr, "ERROR: Support for necessary OpenGL extensions missing.");
        fflush(stderr);
        return CUTFalse;
    }

    // default initialization
    glClearColor(0.0, 0.0, 0.0, 1.0);								//Background color is black
    glDisable(GL_DEPTH_TEST);										//We don't want depth testing

    // viewport
    glViewport(0, 0, winWidth, winHeight);							//sets the viewport

    // projection
    glMatrixMode(GL_PROJECTION);									//Use projection Matrix
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)winWidth / (GLfloat) winHeight, 1.0, 100.0); //Create a perspective view

    CUT_CHECK_ERROR_GL();											//Check for errors, prints them if there are any, and exits program

    return CUTTrue;
}


//------------------------------------------------------------------//
//							Create VBO
// DESCRIPTION: Creates the Vertex Buffer Object
//------------------------------------------------------------------//
void createVBO(GLuint* pos, GLuint* vel)
{
	// create pos buffer object
	glGenBuffers(1, pos);
	glBindBuffer(GL_ARRAY_BUFFER, *pos);

	// initialize pos buffer object
	unsigned int size = ParticleCount * 2 * sizeof(float);		//Using float2's
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//repeat for velocity
	glGenBuffers(1, vel);
	glBindBuffer(GL_ARRAY_BUFFER, *vel);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// register buffer objects with CUDA
	cutilSafeCall(cudaGLRegisterBufferObject(*pos));
	cutilSafeCall(cudaGLRegisterBufferObject(*vel));

}

//------------------------------------------------------------------//
//							Read Settings
// DESCRIPTION: Reads in settings from the Settings.txt file
// RETURNS:     True if a Successful
//              False if it fails
//------------------------------------------------------------------//
bool readSettings()
{
	FILE *fp;
	fopen_s(&fp, "settings.txt", "r");
	if(fp == NULL)
	{
		printf("Problem opening Settings.txt.\n");
		return false;
	}
	fscanf_s(fp, "Particles: %d", &ParticleCount);
	ParticleCount*=4096;
	fclose(fp);
	return true;
}


//------------------------------------------------------------------//
//							RUN TEST
// DESCRIPTION: Checks for a Cuda Device and prints information
//              about the one being used.
// RETURNS:     True if a Usable CUDA device is found
//              False if no device is available
//------------------------------------------------------------------//
CUTBoolean runTest(int argc, char** argv)
{
	int deviceCount;
	cudaGetDeviceCount(&deviceCount);								//Get number of CUDA devices
	if(deviceCount == 0)
	{
		printf("No CUDA enabled device found.\n");
		return CUTFalse;
	}
	else
	{
		int device = cutGetMaxGflopsDeviceId();						//Get device we are going to use
		cudaDeviceProp deviceProperties;
        cudaGetDeviceProperties(&deviceProperties, device);			//Get it's properties
		printf("Running On:\n Device %d: %s\n\n", device, deviceProperties.name);
	}

	return CUTTrue;
}
//---------------------------------------------------------------------------------------------------//
/*----------------------------------------------------------------------------- CUDA Code -----------*/
//---------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------//
//							RUN CUDA
// DESCRIPTION: Calls the launch_kernel function to execute the
//              CUDA kernels.
//------------------------------------------------------------------//
void runCuda(GLuint vboPos, GLuint vboVel, float dT, int Function)
{
    //enable VBOs for writing from CUDA
    float2 *pPtr;
	float2 *vPtr;
    cutilSafeCall(cudaGLMapBufferObject((void**)&pPtr, vboPos));
	cutilSafeCall(cudaGLMapBufferObject((void**)&vPtr, vboVel));


	//launch CUDA code
    launch_kernel(pPtr, vPtr, dT, ParticleCount, boxHeight, boxWidth, Function, mouseForce, Gravity, mousePressed, mouseLocation);

    //Unmap VBOs back to OpenGL
    cutilSafeCall(cudaGLUnmapBufferObject(vboPos));
	cutilSafeCall(cudaGLUnmapBufferObject(vboVel));
}
//---------------------------------------------------------------------------------------------------//
/*---------------------------------------------------------------------------- GL Callbacks----------*/
//---------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------//
//							Display
// DESCRIPTION: Display callback for OpenGL
//------------------------------------------------------------------//
void display()
{ 
	dt = (glutGet(GLUT_ELAPSED_TIME) - lastTime) / 1000.0f;
	lastTime = glutGet(GLUT_ELAPSED_TIME);

    // run CUDA kernel to update particles
    runCuda(vboPos, vboVel, dt, FUNC_UPDATE);

	if(cycleColor)													//If we are cycling colors
		getColor();													//get a new color

    glClear(GL_COLOR_BUFFER_BIT);									//Clear to our ClearColor(black)

    // set view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, cameraZPos);
    glRotatef(cameraRot.x, 1.0, 0.0, 0.0);
    glRotatef(cameraRot.y, 0.0, 1.0, 0.0);

    // render from the vbo
	glPointSize(ParticleSize);
	glColor4f(particleColor.x, particleColor.y, particleColor.z, particleColor.w);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);								//Use additive blending
    glBindBuffer(GL_ARRAY_BUFFER, vboPos);
    glVertexPointer(2, GL_FLOAT, 0, 0);								//using 2 values, type float, offset by 16 bytes, start at beggining

    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_POINTS, 0, ParticleCount);
    glDisableClientState(GL_VERTEX_ARRAY);

	//Render text
	//All thanks to this guy: http://www.lighthouse3d.com/opengl/glut/index.php?bmpfont
	//Go there if you want explanations
	glColor4f(1.0f, 1.0f, 0.0f, 0.8f);
	setOrthographicProjection();
	glLoadIdentity();
	DrawControls(10, 20);
	glColor4f(1.0f, 0.0f, 0.0f, 0.8f);
	DrawControls(11, 21);
	resetPerspectiveProjection();
	glPopMatrix();


	glDisable(GL_BLEND);
	//Post new frame
    glutSwapBuffers();												
    glutPostRedisplay();

	//Calculate the FPS
	frames++;
	computeFPS();
}
//------------------------------------------------------------------//
//							Reshape
// DESCRIPTION: Reshape callback for OpenGL
//------------------------------------------------------------------//
void reshape (int w, int h) {
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);						//Update Viewport
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	glMatrixMode (GL_MODELVIEW);
	winHeight = h;													//Save window height 
	winWidth = w;													//and width
	boxWidth  = boxHeight*winWidth/winHeight;						//Calculate new box width (It's based of aspect ratio)

	//Call resize CUDA function so no particles get stuck outside the window forever
	runCuda(vboPos, vboVel,dt, FUNC_RESIZE);
}




//------------------------------------------------------------------//
//							Keyboard
// DESCRIPTION: Keyboard callback for OpenGL
//              Handles normal keyboard input
//------------------------------------------------------------------//
void keyboard(unsigned char key, int /*x*/, int /*y*/)
{
    switch(key) {
    case(27) :						//ESC -exit
        exit(0);
        break;
	case(' '):						//Spacebar - toggle cycling colors on/off
		cycleColor = !cycleColor;
		break;
	case('r'):						//R - reset view
		cameraRot.x = 0;
		cameraRot.y = 0;
    }
}

//------------------------------------------------------------------//
//						Keyboard Special
// DESCRIPTION: Special Keyboard callback for OpenGL
//              Handles special keyboard input (ie. shift, alt, etc.)
//------------------------------------------------------------------//
void keyboardSpecial(int key, int /*x*/, int /*y*/)
{
    switch(key) {
	case(GLUT_KEY_LEFT):				//Left arrow - decrement current option
		UpdateControls(false);
		break;
	case(GLUT_KEY_RIGHT):				//Right arrow - increment current option
		UpdateControls(true);
		break;
	case(GLUT_KEY_UP):					//Up Arrow - change current option
		currentOption++;
		if(currentOption > Options)
			currentOption = 0;
		break;
	case(GLUT_KEY_DOWN):				//Down Arrow - change current option
		currentOption--;
		if(currentOption < 0)
			currentOption = Options;
		break;
    }
}

//------------------------------------------------------------------//
//							Mouse
// DESCRIPTION: Mouse callback for OpenGL
//              Handles clicking/ releasing
//------------------------------------------------------------------//
void mouse(int button, int state, int x, int y)
{
	mouseButtons = button;
    if (state == GLUT_DOWN) {					//If button was pressed down
		if(mouseButtons == GLUT_LEFT_BUTTON)	//If left button
			mousePressed = true;				//Apply mouse force
    } else if (state == GLUT_UP) {				//If button was released
		if(mouseButtons == GLUT_LEFT_BUTTON)	//If left button
			mousePressed = false;				//Don't apply force
    }

	//Use some basic algebra to convert screen coordinates into world coordinates (always from 0,0,-10)
	float newx = 2.0f * boxWidth *(float)x / winWidth - boxWidth;
	float newy = -1*(2.0f * boxHeight *(float)y / winHeight - boxHeight);

	mouseLocation = make_float2(newx, newy);	//Save its world location
	mouseScreenLocation = make_int2(x, y);		//save its screen location
    glutPostRedisplay();
}

//------------------------------------------------------------------//
//							Motion
// DESCRIPTION: Mouse Motion callback for OpenGL
//              Handles mouse motion
//------------------------------------------------------------------//
void motion(int x, int y)
{
	if (mousePressed)
	{
		if(mouseButtons == GLUT_LEFT_BUTTON)						//If left button is held down
		{
			float newx = 2.0f * boxWidth *(float)x / winWidth - boxWidth;
			float newy = -1*(2.0f * boxHeight *(float)y / winHeight - boxHeight);
			mouseLocation = make_float2(newx, newy);				//Save new world location
		}
	}
	else if(mouseButtons & GLUT_RIGHT_BUTTON)						//If the right button is down
	{
		float dx, dy;
		dx = x - mouseScreenLocation.x;
		dy = y - mouseScreenLocation.y;
		cameraRot.x += dy * 0.2;									//rotate cameras by the difference
		cameraRot.y += dx * 0.2;
	}
	mouseScreenLocation = make_int2(x, y);							//Save new Screen Location
}


//---------------------------------------------------------------------------------------------------//
/*---------------------------------------------------------------------------------- Drawing---------*/
//---------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------//
//						Print Instructions
// DESCRIPTION: Prints controls to screen so the user knows
//              how to use the program
//------------------------------------------------------------------//
void printInstructions()
{
	printf("\nControl Layout:\n");
	printf(" Apply Force	: Left Mouse\n");
	printf(" Rotate View	: Right Mouse\n");
	printf(" Reset View	: R\n");
	printf(" Color Toggle	: Space Bar\n");
	printf(" Options Toggle	: Up/Down Arrow\n");
	printf(" Adjust Option	: Left/Right Arrow\n");
}

//------------------------------------------------------------------//
//						Draw Controls
// DESCRIPTION: Draws the current option to the screen if currentOption
//              does not equal 0.
//------------------------------------------------------------------//
void DrawControls(int x, int y)
{
																	//0 - Don't draw anything (default)
	if(currentOption == 1)											//1 - Mouse Force
	{
		char msg[128];
		sprintf(msg,"Mouse Force: %.3f", mouseForce*10);			//Multiply by 100 to give the user prettier units
		renderBitmapString(x,y,msg);
	}	
	else if(currentOption == 2)										//2 - Gravity
	{
		char msg[128];
		sprintf(msg,"Gravity: %.3f", Gravity*10);					//Multiply by 100 to give the user prettier units
		renderBitmapString(x,y,msg);
	}	
	else if(currentOption == 3)										//3 - Particle Color
	{
		char msg[128];
		sprintf(msg,"Particle Opacity: %.3f", particleColor.w);
		renderBitmapString(x,y,msg);
	}	
	else if(currentOption == 4)										// Particle Size
	{
		char msg[128];
		sprintf(msg,"Particle Size: %.1f", ParticleSize);
		renderBitmapString(x,y,msg);
	}
}

//------------------------------------------------------------------//
//							Compute FPS
// DESCRIPTION: Computes Frames per second 
//              and displays it in the title bar
//------------------------------------------------------------------//
void computeFPS()
{
	if(frames%30==0)
	{
		int currentTime = glutGet(GLUT_ELAPSED_TIME);				//Get total elapsed tIme
		float elapsedTime = (currentTime-gameTime)/30000.0f;		//Divide by 1000 and 30 to get Avg elapsed time for last 30 frames
		float FPS = 1.0f/elapsedTime;								//change time into FPS (1/(seconds per frame) -> (frames/second)
		char title[128];
		sprintf(title, "CUDA Accelerated Particle Engine: %3.1f FPS",  FPS);  //create title
		glutSetWindowTitle(title);									//Set FPS to title
		gameTime = currentTime;										//remember time
	}
}


//------------------------------------------------------------------//
//						Update Controls
// DESCRIPTION: Updates the controls by incrementing or decrementing 
//              the current option 
// PARAMETERS:  
//     BOOL PLUS - increments if true, decrements if false
//------------------------------------------------------------------//
void UpdateControls(bool plus)
{
	if(currentOption == 1)					//1 - Mouse Force
	{
		if(plus)
		{
			mouseForce += 0.0625f;
		}
		else
		{
			mouseForce -= 0.0625f;
		}
	}
	else if(currentOption == 2)				//2 - Gravity
	{
		if(plus)
		{
			Gravity += 0.02f;
		}
		else
		{
			if(Gravity > 0.01f)
				Gravity -= 0.02f;
		}
	}
	else if(currentOption == 3)				//3 - Particle Opacity
	{
		if(plus)
		{
			if(particleColor.w < 0.99f)
				particleColor.w += 0.025f;
		}
		else
		{
			if(particleColor.w > 0.05f)
				particleColor.w -= 0.025f;
		}
	}
	else if(currentOption == 4)				//4 - Particle Size
	{
		if(plus)
		{
				ParticleSize += 1.0f;
		}
		else
		{
			if(ParticleSize > 1.0f)
				ParticleSize -= 1.0f;
		}
	}
}

//------------------------------------------------------------------//
//					Set Orthographic Projection
// DESCRIPTION: Sets the screen into 2D coordinates 
//------------------------------------------------------------------//
void setOrthographicProjection() 
{
	//All thanks to this guy: http://www.lighthouse3d.com/opengl/glut/index.php?bmpfont
	//Go there if you want explanations

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, winWidth, 0, winHeight);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -winHeight, 0);
	glMatrixMode(GL_MODELVIEW);
}

//------------------------------------------------------------------//
//					Reset Perspective Projection
// DESCRIPTION: Resets the screen into 3D coordinates
//------------------------------------------------------------------//
void resetPerspectiveProjection() {
	//All thanks to this guy: http://www.lighthouse3d.com/opengl/glut/index.php?bmpfont
	//Go there if you want explanations
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

//------------------------------------------------------------------//
//						Render Bitmap String
// DESCRIPTION: Renders text to the screen
// PARAMETERS:  
//     FLOAT X - X coordinate of the first letter
//	   FLOAT Y - Y coordinate of the first letter
//     CHAR* STRING - The string to be drawn
//------------------------------------------------------------------//
void renderBitmapString(float x, float y, char *string) {  
	//All thanks to this guy: http://www.lighthouse3d.com/opengl/glut/index.php?bmpfont
	//Go there if you want explanations
	char *c;
	glRasterPos2f(x, y);
	for (c=string; *c != '\0'; c++) {
	glutBitmapCharacter(FONT, *c);
  }
}


//------------------------------------------------------------------//
//						GetColor
// DESCRIPTION: Cycles through the Rainbow of colors 
//              reference: http://answers.yahoo.com/question/index?qid=20091123051136AAEdZnT
//              but instead of going from 0-255, I go from MINCOLOR
//				to MAXCOLOR
//------------------------------------------------------------------//
void getColor()
{
	static unsigned int state		= 0;	//holds which state we are in
	switch(state) {							// (R, G, B)
	case(0):								//(High, Increasing, Low)
		particleColor.y  += COLORSPEED*1.0f/255.0f;
		if(particleColor.y >= MAXCOLOR)
		{
			state = 1;
			particleColor.y = MAXCOLOR;
		}
		break;
	case(1):								//(Decreasing, High, Low)
		particleColor.x  -= COLORSPEED*1.0f/255.0f;
		if(particleColor.x <= MINCOLOR)
		{
			state = 2;
			particleColor.x = MINCOLOR;
		}
		break;
	case(2):								//(Low, High, Increasing)
		particleColor.z  += COLORSPEED*1.0f/255.0f;
		if(particleColor.z >= MAXCOLOR)
		{
			state = 3;
			particleColor.z = MAXCOLOR;
		}
		break;
	case(3):								//(Low, Decreasing, High)
		particleColor.y  -= COLORSPEED*1.0f/255.0f;
		if(particleColor.y <= MINCOLOR)
		{
			state = 4;
			particleColor.y = MINCOLOR;
		}
		break;
	case(4):								//(Increasing, Low, High)
		particleColor.x  += COLORSPEED*1.0f/255.0f;
		if(particleColor.x >= MAXCOLOR)
		{
			state = 5;
			particleColor.x = MAXCOLOR;
		}
		break;
	case(5):								//(High, Low, Decreasing)
		particleColor.z  -= COLORSPEED*1.0f/255.0f;
		if(particleColor.z <= MINCOLOR)
		{
			state = 0;
			particleColor.z = MINCOLOR;
		}
		break;
    } 

}

//---------------------------------------------------------------------------------------------------//
/*---------------------------------------------------------------------------- Exiting Code ---------*/
//---------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------//
//							Delete VBO
// DESCRIPTION: Deletes the Vertex Buffer Object
//------------------------------------------------------------------//
void deleteVBO(GLuint* pos, GLuint* vel)
{
	glBindBuffer(1, *pos);
	glDeleteBuffers(1, pos);

	cutilSafeCall(cudaGLUnregisterBufferObject(*pos));

	*pos = 0;

	glBindBuffer(1, *vel);
	glDeleteBuffers(1, vel);

	cutilSafeCall(cudaGLUnregisterBufferObject(*vel));

	*vel = 0;
}


//------------------------------------------------------------------//
//							Clean up
// DESCRIPTION:Closing code, calls deleteVBO
//------------------------------------------------------------------//
void cleanup()
{
	//Hmm, Didn't use this as much as I expected
    deleteVBO(&vboPos, &vboVel);
}
#endif