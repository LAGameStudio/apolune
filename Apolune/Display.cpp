
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include "Display.h"
#include "GLSetup.h"
#include "DisplayList.h"
#include "CommandLine.h"
#include "Graphics.h"

extern Display screenWindow;
extern Display *screenWindowPointer;

double windowDisplayMouseRatioX=1.0, windowDisplayMouseRatioY=1.0;

void SetDisplayWindowMouseRatio(Display *d,Display *sw) {
 windowDisplayMouseRatioX = sw?(sw->wd / d->wd):1.0;
 windowDisplayMouseRatioY = sw?(sw->hd / d->hd):1.0;
}


void Display::Set2DViewport( double DPI ) {
/* if ( gl->noFullscreen ) {
  glViewport(viewport.x,viewport.y,viewport.w-windowOffset.x,viewport.h-windowOffset.y); 
 } else */ glViewport(viewport.x,viewport.y,viewport.w,viewport.h);
 glOrtho(0,wd,hd,0,0.0,1.0);
}

void Display::Resized( int ww, int hh, bool updateWindowManager ) {
 viewport.SetRect(0,0,ww,hh);
 w=ww; wd=(double) ww; wf=(float) ww;
 h=hh; hd=(double) hh; hf=(float) hh;
 midx=w/2;
 w2=w/2;
 midy=w/2;
 h2=h/2;
 w2f=wf/2.0f;
 h2f=hf/2.0f;
 w2d=wd/2.0;
 h2d=hd/2.0;
 aspect=iratiof(w,h);
 inverseaspect=iratiof(h,w);
 screen.SetRectf(0.0f,0.0f,wf,hf);
 screen.SetRect(0,0,w,h);
 screend.SetRect(0.0,0.0,wd,hd);
 if ( updateWindowManager ) windows.Resized();
 Virtual(1920,1080); // 1080p
}

void Display::Resized( int xx, int yy, int ww, int hh, bool updateWindowManager ) {
 viewport.SetRect(xx,yy,ww,hh);
 w=ww; wd=(double) ww; wf=(float) ww;
 h=hh; hd=(double) hh; hf=(float) hh;
 midx=w/2;
 w2=w/2;
 midy=w/2;
 h2=h/2;
 w2f=wf/2.0f;
 h2f=hf/2.0f;
 w2d=wd/2.0;
 h2d=hd/2.0;
 aspect=iratiof(w,h);
 inverseaspect=iratiof(h,w);
 screen.SetRectf(0.0f,0.0f,wf,hf);
 screen.SetRect(0,0,w,h);
 screend.SetRect(0.0,0.0,wd,hd);
 if ( updateWindowManager ) windows.Resized();
 Virtual(1920,1080); // 1080p
}

void Display::VirtualScale2d() {
 glPushMatrix();
 glScaled(inverseVirtualRatio.x,inverseVirtualRatio.y,1.0);
}

void Display::UnVirtualScale2d() {
 glPopMatrix();
}

void Display::SaveScreenshot() {
	FILE * file;
	char filename[128];
	for(int i=0; i<1000; ++i)
	{
		FORMAT(filename,128, "screen%03d.tga", (int) i);
		fopen_s(&file,filename, "rb");
		if(!file)break;
		fclose(file);
		if(i==999)
		{
			OUTPUT("Display:SaveScreenshot reports no file namespace to save screenshot - 0 to 999 exist\n");
			return;
		}
	} 
	OUTPUT("Display::SaveScreenshot() is saving %s\n", filename);

	//Uncompressed true color tga header
	GLubyte TGAheader[12]={0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	GLubyte infoHeader[6];

	//Create space for the data
	unsigned char * data=new unsigned char[4*w*h];
	if(!data)	{
		OUTPUT("Display:SaveScreenshot is unable to allocate space for screen data\n");
		return;
	}

	//read in the data
	//Use the front buffer so we can capture an antialiased screen
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glReadBuffer(GL_BACK);

	//Data needs to be in BGR format
	//Swap B and R
	for(int i=0; i<(int)w*h*4; i+=4)
	{
		//repeated xor to swap bytes
		data[i] ^= data[i+2] ^= data[i] ^= data[i+2];
	}

	//Open the file
	errno_t result=fopen_s(&file,filename, "wb");
	if (!file) {
 	 OUTPUT("Error saving screenshot to file: %s", filename);
	 return;
	}

	//save header
	fwrite(TGAheader, 1, sizeof(TGAheader), file);

	//Fill in info header
	infoHeader[0]=(w & 0x00FF);
	infoHeader[1]=(w & 0xFF00) >> 8;
	infoHeader[2]=(h & 0x00FF);
	infoHeader[3]=(h & 0xFF00) >> 8;
	infoHeader[4]=32;
	infoHeader[5]=0;

	fwrite(infoHeader, 1, sizeof(infoHeader), file);
	fwrite(data, 1, w*h*4, file);
	fclose(file);

	if(data)	delete [] data;
	data=NULL;

	OUTPUT("Saved Screenshot: %s", filename);
	return;
}

void Display::CommandLine() {
 
 // Check command line and create the display profile
 if (CmdLine.Option("-320x200")  || CmdLine.Option("-cga")  ) {
  w=320;
  h=200;
  OUTPUT("Setting 320x200 (CGA) as the resolution.\n");   
 } else
 if (CmdLine.Option("-320x240")  || CmdLine.Option("-qvga")  ) {
  w=320;
  h=240;
  OUTPUT("Setting 320x240 (QVGA) as the resolution.\n");   
 } else
 if (CmdLine.Option("-320x400")  || CmdLine.Option("-tv")  ) {
  w=320;
  h=240;
  OUTPUT("Setting 320x400 (Classic TV) as the resolution.\n");   
 } else
 if (CmdLine.Option("-320x243")  ) {
  w=320;
  h=243;
  OUTPUT("Setting 320x243 (pseudo NTSC) as the resolution.\n");   
 } else
 if (CmdLine.Option("-320x486")  || CmdLine.Option("-ntsc")  ) {
  w=320;
  h=486;
  OUTPUT("Setting 320x486 (NTSC non-interlaced) as the resolution.\n");   
 } else
 if (CmdLine.Option("-480x320")  ) {
  w=480;
  h=320;
  OUTPUT("Setting 480x320 as the resolution.\n");   
 } else
 if (CmdLine.Option("-640x480")  || CmdLine.Option("-vga")  ) {
  w=640;
  h=480;
  OUTPUT("Setting 640x480 (VGA) as the resolution.\n");
 } else
 if (CmdLine.Option("-720x480") ) {
  w=720;
  h=480;
  OUTPUT("Setting 720x480 as the resolution.\n");
 } else
 if (CmdLine.Option("-800x480")  ) {
  w=800;
  h=480;
  OUTPUT("Setting 800x480 (WVGA) as the resolution.\n");
 } else
 if (CmdLine.Option("-854x480")  || CmdLine.Option("-wvga")  ) {
  w=854;
  h=480;
  OUTPUT("Setting 854x480 (WVGA) as the resolution.\n");
 } else
 if (CmdLine.Option("-768x576")  || CmdLine.Option("-pal")  ) {
  w=768;
  h=576;
  OUTPUT("Setting 768x576 (PAL) as the resolution.\n");
 } else
 if (CmdLine.Option("-800x600")  || CmdLine.Option("-svga")  ) {
  w=800;
  h=600;
  OUTPUT("Setting 800x600 (SVGA) as the resolution.\n");
 } else
 if (CmdLine.Option("-1024x600")  || CmdLine.Option("-wsvga")  ) {
  w=1024;
  h=600;
  OUTPUT("Setting 1024x600 (WSVGA) as the resolution.\n");
 } else
 if (CmdLine.Option("-1024x768")  || CmdLine.Option("-xga")  ) {
  w=1024;
  h=768;
  OUTPUT("Setting 1024x768 (XGA) as the resolution.\n");   
 } else
 if (CmdLine.Option("-1152x768")  ) {
  w=1152;
  h=768;
  OUTPUT("Setting 1152x768 as the resolution.\n");
 } else
 if (CmdLine.Option("-1280x720")  || CmdLine.Option("-hd720")  || CmdLine.Option("-hd720p")  || CmdLine.Option("-720p")  ) {
  w=1280;
  h=720;
  OUTPUT("Setting 1280x768 (HD720p) as the resolution.\n");
 } else
 if (CmdLine.Option("-1280x768")  ) {
  w=1280;
  h=768;
  OUTPUT("Setting 1280x768 (WXGA) as the resolution.\n");
 } else
 if (CmdLine.Option("-1280x800")  || CmdLine.Option("-wxga")   ) {
  w=1280;
  h=800;
  OUTPUT("Setting 1280x768 (WXGA) as the resolution.\n");
 } else
 if (CmdLine.Option("-1280x854")  ) {
  w=1280;
  h=854;
  OUTPUT("Setting 1280x854 as the resolution.\n");
 } else
 if (CmdLine.Option("-1280x960")  ) {
  w=1280;
  h=960;
  OUTPUT("Setting 1280x960 as the resolution.\n");
 } else
 if (CmdLine.Option("-1280x1024")  || CmdLine.Option("-sxga")  ) {
  w=1280;
  h=1024;
  OUTPUT("Setting 1280x1024 (SXGA) as the resolution.\n");
 } else
 if (CmdLine.Option("-1366x768")  ) {
  w=1366;
  h=768;
  OUTPUT("Setting 1366x768 as the resolution.\n");
 } else
 if (CmdLine.Option("-1400x1050")   || CmdLine.Option("-sxga+")  ) {
  w=1400;
  h=1050;
  OUTPUT("Setting 1400x1050 (SXGA+) as the resolution.\n");
 } else
 if (CmdLine.Option("-1440x900")  ) {
  w=1440;
  h=900;
  OUTPUT("Setting 1440x900 as the resolution.\n");
 } else
 if (CmdLine.Option("-1440x960")  ) {
  w=1440;
  h=960;
  OUTPUT("Setting 1440x960 as the resolution.\n");
 } else
 if (CmdLine.Option("-1680x1050")   || CmdLine.Option("-wsxga+")  ) {
  w=1680;
  h=1050;
  OUTPUT("Setting 1680x1050 (WSXGA+) as the resolution.\n");
 } else
 if (CmdLine.Option("-1600x1200")   || CmdLine.Option("-uxga")  ) {
  w=1600;
  h=1200;
  OUTPUT("Setting 1600x1200 (UXGA) as the resolution.\n");
 } else
 if (CmdLine.Option("-1768x992")  ) {
  w=1768;
  h=992;
  OUTPUT("Setting 1768x992 as the resolution.\n");
 } else
 if (CmdLine.Option("-1920x1080")   || CmdLine.Option("-hd1080")  || CmdLine.Option("-1080")  || CmdLine.Option("-1080i")  || CmdLine.Option("-1080p")  ) {
  w=1920;
  h=1080;
  OUTPUT("Setting 1920x1080 (HD 1080i) as the resolution.\n");
 } else
 if (CmdLine.Option("-1900x1080")  ) {
  w=1900;
  h=1080;
  OUTPUT("Setting 1900x1080 (HD 1080i) as the resolution.\n");
 } else
 if (CmdLine.Option("-1920x540")  ) {
  w=1920;
  h=540;
  OUTPUT("Setting 1920x540 (non-interlaced psuedo HD1080) as the resolution.\n");
 } else
 if (CmdLine.Option("-1920x1200")   || CmdLine.Option("-wuxga")  ) {
  w=1920;
  h=1200;
  OUTPUT("Setting 1920x1200 (WUXGA) as the resolution.\n");
 } else
 if (CmdLine.Option("-2048x1080")   || CmdLine.Option("-2k")  ) {
  w=2048;
  h=1080;
  OUTPUT("Setting 2048x1080 (2K) as the resolution.\n");
 } else
 if (CmdLine.Option("-2048x1536")   || CmdLine.Option("-qxga")  ) {
  w=2048;
  h=1536;
  OUTPUT("Setting 2048x1536 (QXGA) as the resolution.\n");
 } else
 if (CmdLine.Option("-2560x1440")   || CmdLine.Option("-cinema")  ) {
  w=2560;
  h=1440;
  OUTPUT("Setting 2560x1440 (Apple Cinema T) as the resolution.\n");
 } else
 if (CmdLine.Option("-2560x1600")   || CmdLine.Option("-wqxga")  ) {
  w=2560;
  h=1600;
  OUTPUT("Setting 2560x1600 (WQXGA) as the resolution.\n");
 } else
 if (CmdLine.Option("-2560x2048")   || CmdLine.Option("-qsxga")  ) {
  w=2560;
  h=2048;
  OUTPUT("Setting 2560x2048 (WSXGA) as the resolution.\n");
 } else
  // Matrox TripleHead2Go Support
 if (CmdLine.Option("-1600x600")   || CmdLine.Option("-32godual1")  ) {
  w=1600;
  h=600;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 1600x600 (Matrox TripleHead2Go DualMode 1) as the resolution.\n");
 } else
 if (CmdLine.Option("-2048x768")   || CmdLine.Option("-32godual2")  ) {
  w=2048;
  h=768;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2048x768 (Matrox TripleHead2Go DualMode 2) as the resolution.\n");
 } else
 if (CmdLine.Option("-2560x720")   || CmdLine.Option("-32godual3")  ) {
  w=2560;
  h=720;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2560x720 (Matrox TripleHead2Go DualMode 3) as the resolution.\n");
 } else
 if (CmdLine.Option("-2560x768")   || CmdLine.Option("-32godual4")  ) {
  w=2560;
  h=768;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2560x768 (Matrox TripleHead2Go DualMode 4) as the resolution.\n");
 } else 
 if (CmdLine.Option("-2560x800")   || CmdLine.Option("-32godual5")  ) {
  w=2560;
  h=800;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2560x800 (Matrox TripleHead2Go DualMode 5) as the resolution.\n");
 } else
 if (CmdLine.Option("-2560x800")   || CmdLine.Option("-32godual6")  ) {
  w=2560;
  h=800;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2560x800 (Matrox TripleHead2Go DualMode 6) as the resolution.\n");
 } else
 if (CmdLine.Option("-2560x960")   || CmdLine.Option("-32godual7")  ) {
  w=2560;
  h=960;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2560x960 (Matrox TripleHead2Go DualMode 7) as the resolution.\n");
 } else
 if (CmdLine.Option("-2560x1024")   || CmdLine.Option("-32godual8")  ) {
  w=2560;
  h=1024;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2560x1024 (Matrox TripleHead2Go DualMode 8) as the resolution.\n");
 } else 
 if (CmdLine.Option("-2720x768")   || CmdLine.Option("-32godual9")  ) {
  w=2720;
  h=768;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2720x768 (Matrox TripleHead2Go DualMode 9) as the resolution.\n");
 } else
 if (CmdLine.Option("-2800x1050")   || CmdLine.Option("-32godual10")  ) {
  w=2800;
  h=1050;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2800x1050 (Matrox TripleHead2Go DualMode 10) as the resolution.\n");
 } else
 if (CmdLine.Option("-2880x900")   || CmdLine.Option("-32godual11")  ) {
  w=2880;
  h=900;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2880x900 (Matrox TripleHead2Go DualMode 11) as the resolution.\n");
 } else
 if (CmdLine.Option("-3200x900")   || CmdLine.Option("-32godual12")  ) {
  w=3200;
  h=900;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 3200x900 (Matrox TripleHead2Go DualMode 12) as the resolution.\n");
 } else
  if (CmdLine.Option("-3200x1200")   || CmdLine.Option("-32godual13")  ) {
  w=3200;
  h=1200;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 3200x1200 (Matrox TripleHead2Go DualMode 13) as the resolution.\n");
 } else
 if (CmdLine.Option("-3360x1050")   || CmdLine.Option("-32godual14")  ) {
  w=3200;
  h=1050;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 3360x1050 (Matrox TripleHead2Go DualMode 14) as the resolution.\n");
 } else
 if (CmdLine.Option("-3840x1080")   || CmdLine.Option("-32godual15")  ) {
  w=3840;
  h=1080;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 3840x1080 (Matrox TripleHead2Go DualMode 15) as the resolution.\n");
 } else
 if (CmdLine.Option("-3840x1200")   || CmdLine.Option("-32godual16")  ) {
  w=3840;
  h=1200;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 3840x1200 (Matrox TripleHead2Go DualMode 16) as the resolution.\n");
 } else
 if (CmdLine.Option("-4096x1152")   || CmdLine.Option("-32godual17")  ) {
  w=4096;
  h=1152;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 4096x1152 (Matrox TripleHead2Go DualMode 17) as the resolution.\n");
 } else
 if (CmdLine.Option("-1920x480")   || CmdLine.Option("-32gotri1")  ) {
  w=1920;
  h=480;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 1920x480 (Matrox TripleHead2Go TripleHead Mode 1) as the resolution.\n");
 } else
 if (CmdLine.Option("-2400x480")   || CmdLine.Option("-32gotri2")  ) {
  w=2400;
  h=480;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 2400x480 (Matrox TripleHead2Go TripleHead Mode 2) as the resolution.\n");
 } else
 if (CmdLine.Option("-2400x600")   || CmdLine.Option("-32gotri3")  ) {
  w=2400;
  h=600;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 2400x600 (Matrox TripleHead2Go TripleHead Mode 3) as the resolution.\n");
 } else
 if (CmdLine.Option("-3072x768")   || CmdLine.Option("-32gotri4")  ) {
  w=3072;
  h=768;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 3072x768 (Matrox TripleHead2Go TripleHead Mode 4) as the resolution.\n");
 } else
 if (CmdLine.Option("-3840x720")   || CmdLine.Option("-32gotri5")  ) {
  w=3840;
  h=720;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 3840x720 (Matrox TripleHead2Go TripleHead Mode 5) as the resolution.\n");
 } else
 if (CmdLine.Option("-3840x768")   || CmdLine.Option("-32gotri6")  ) {
  w=3840;
  h=768;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 3840x768 (Matrox TripleHead2Go TripleHead Mode 6) as the resolution.\n");
 } else
 if (CmdLine.Option("-3840x800")   || CmdLine.Option("-32gotri7")  ) {
  w=3840;
  h=800;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 3840x800 (Matrox TripleHead2Go TripleHead Mode 7) as the resolution.\n");
 } else
 if (CmdLine.Option("-3840x960")   || CmdLine.Option("-32gotri8")  ) {
  w=3840;
  h=960;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 3840x960 (Matrox TripleHead2Go TripleHead Mode 8) as the resolution.\n");
 } else
 if (CmdLine.Option("-3840x1024")   || CmdLine.Option("-32gotri9")  ) {
  w=3840;
  h=1024;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 3840x1024 (Matrox TripleHead2Go TripleHead Mode 9) as the resolution.\n");
 } else
 if (CmdLine.Option("-3840x2160") || CmdLine.Option("-4k")) {
		 w = 3840;
		 h = 2160;
		 probablyMatroxTripleHead2Go = true;
		 probablyMatroxTripleHead = true;
		 OUTPUT("Setting 3840x2160 (4K) as the resolution.\n");
 }
 else
 if (CmdLine.Option("-4080x768")   || CmdLine.Option("-32gotri10")  ) {
  w=4080;
  h=768;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 4080x768 (Matrox TripleHead2Go TripleHead Mode 10) as the resolution.\n");
 } else
 if (CmdLine.Option("-4098x768")   || CmdLine.Option("-32gotri11")  ) {
  w=4098;
  h=768;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 4098x768 (Matrox TripleHead2Go TripleHead Mode 11) as the resolution.\n");
 } else
 if (CmdLine.Option("-4200x1050")   || CmdLine.Option("-32gotri12")  ) {
  w=4200;
  h=1050;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 4200x1050 (Matrox TripleHead2Go TripleHead Mode 12) as the resolution.\n");
 } else
 if (CmdLine.Option("-4320x900")   || CmdLine.Option("-32gotri13")  ) {
  w=4320;
  h=900;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 4320x900 (Matrox TripleHead2Go TripleHead Mode 13) as the resolution.\n");
 } else
 if (CmdLine.Option("-4800x900")   || CmdLine.Option("-32gotri14")  ) {
  w=4800;
  h=900;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 4800x900 (Matrox TripleHead2Go TripleHead Mode 14) as the resolution.\n");
 } else
 if (CmdLine.Option("-5040x1050")   || CmdLine.Option("-32gotri15")  ) {
  w=5040;
  h=1050;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 5040x1050 (Matrox TripleHead2Go TripleHead Mode 15) as the resolution.\n");
 } else
 if (CmdLine.Option("-5760x1080")   || CmdLine.Option("-32gotri16")  ) {
  w=5760;
  h=1080;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 5760x1080 (Matrox TripleHead2Go TripleHead Mode 16) as the resolution.\n");
 }
 else {
  string _res=CmdLine.OptionPair("-resolution");
  if ( _res.length() > 0 ) {
   Strings rs;
   rs.SplitPush(_res.c_str(),'x');
   String *A=(String *)rs.first;
   String *B=(String *)rs.last;
   int a=atoi(A?A->s.c_str():"100");
   int b=atoi(B?B->s.c_str():"100");
   OUTPUT("Forcing resolution to %d by %d (%f)\n",a,b,iratiod(a,b));
   w=a;
   h=b;
  } else { // Detect the display size and create the display profile
   w=GetSystemMetrics(SM_CXSCREEN);
   h=GetSystemMetrics(SM_CYSCREEN);
   OUTPUT("Detected resolution: %dx%d\n", w, h);
  }
 }
 
 if (CmdLine.Option("-8bit") ) {
  bits=8;
  OUTPUT("Using 8-bit mode by command line.\n");
 } else
 if (CmdLine.Option("-16bit") ) {
  bits=16;
  OUTPUT("Using 16-bit mode by command line.\n");
 } else
 if (CmdLine.Option("-24bit") ) {
  bits=24;
  OUTPUT("Using 24-bit mode by command line.\n");
 } else {
  bits=32;
  OUTPUT("Defaulting to 32-bit.\n");
 }
}


void Display::CommandLineAlt() {
 
 // Check command line and create the display profile
 if (CmdLine.Option("+320x200")  || CmdLine.Option("+cga")  ) {
  w=320;
  h=200;
  OUTPUT("Setting 320x200 (CGA) as the resolution.\n");   
 } else
 if (CmdLine.Option("+320x240")  || CmdLine.Option("+qvga")  ) {
  w=320;
  h=240;
  OUTPUT("Setting 320x240 (QVGA) as the resolution.\n");   
 } else
 if (CmdLine.Option("+320x400")  || CmdLine.Option("+tv")  ) {
  w=320;
  h=240;
  OUTPUT("Setting 320x400 (Classic TV) as the resolution.\n");   
 } else
 if (CmdLine.Option("+320x243")  ) {
  w=320;
  h=243;
  OUTPUT("Setting 320x243 (pseudo NTSC) as the resolution.\n");   
 } else
 if (CmdLine.Option("+320x486")  || CmdLine.Option("+ntsc")  ) {
  w=320;
  h=486;
  OUTPUT("Setting 320x486 (NTSC non-interlaced) as the resolution.\n");   
 } else
 if (CmdLine.Option("+480x320")  ) {
  w=480;
  h=320;
  OUTPUT("Setting 480x320 as the resolution.\n");   
 } else
 if (CmdLine.Option("+640x480")  || CmdLine.Option("+vga")  ) {
  w=640;
  h=480;
  OUTPUT("Setting 640x480 (VGA) as the resolution.\n");
 } else
 if (CmdLine.Option("+720x480") ) {
  w=720;
  h=480;
  OUTPUT("Setting 720x480 as the resolution.\n");
 } else
 if (CmdLine.Option("+800x480")  ) {
  w=800;
  h=480;
  OUTPUT("Setting 800x480 (WVGA) as the resolution.\n");
 } else
 if (CmdLine.Option("+854x480")  || CmdLine.Option("+wvga")  ) {
  w=854;
  h=480;
  OUTPUT("Setting 854x480 (WVGA) as the resolution.\n");
 } else
 if (CmdLine.Option("+768x576")  || CmdLine.Option("+pal")  ) {
  w=768;
  h=576;
  OUTPUT("Setting 768x576 (PAL) as the resolution.\n");
 } else
 if (CmdLine.Option("+800x600")  || CmdLine.Option("+svga")  ) {
  w=800;
  h=600;
  OUTPUT("Setting 800x600 (SVGA) as the resolution.\n");
 } else
 if (CmdLine.Option("+1024x600")  || CmdLine.Option("+wsvga")  ) {
  w=1024;
  h=600;
  OUTPUT("Setting 1024x600 (WSVGA) as the resolution.\n");
 } else
 if (CmdLine.Option("+1024x768")  || CmdLine.Option("+xga")  ) {
  w=1024;
  h=768;
  OUTPUT("Setting 1024x768 (XGA) as the resolution.\n");   
 } else
 if (CmdLine.Option("+1152x768")  ) {
  w=1152;
  h=768;
  OUTPUT("Setting 1152x768 as the resolution.\n");
 } else
 if (CmdLine.Option("+1280x720")  || CmdLine.Option("+hd720")  || CmdLine.Option("+hd720p")  || CmdLine.Option("+720p")  ) {
  w=1280;
  h=720;
  OUTPUT("Setting 1280x768 (HD720p) as the resolution.\n");
 } else
 if (CmdLine.Option("+1280x768")  ) {
  w=1280;
  h=768;
  OUTPUT("Setting 1280x768 (WXGA) as the resolution.\n");
 } else
 if (CmdLine.Option("+1280x800")  || CmdLine.Option("+wxga")   ) {
  w=1280;
  h=800;
  OUTPUT("Setting 1280x768 (WXGA) as the resolution.\n");
 } else
 if (CmdLine.Option("+1280x854")  ) {
  w=1280;
  h=854;
  OUTPUT("Setting 1280x854 as the resolution.\n");
 } else
 if (CmdLine.Option("+1280x960")  ) {
  w=1280;
  h=960;
  OUTPUT("Setting 1280x960 as the resolution.\n");
 } else
 if (CmdLine.Option("+1280x1024")  || CmdLine.Option("+sxga")  ) {
  w=1280;
  h=1024;
  OUTPUT("Setting 1280x1024 (SXGA) as the resolution.\n");
 } else
 if (CmdLine.Option("+1366x768")  ) {
  w=1366;
  h=768;
  OUTPUT("Setting 1366x768 as the resolution.\n");
 } else
 if (CmdLine.Option("+1400x1050")   || CmdLine.Option("+sxga+")  ) {
  w=1400;
  h=1050;
  OUTPUT("Setting 1400x1050 (SXGA+) as the resolution.\n");
 } else
 if (CmdLine.Option("+1440x900")  ) {
  w=1440;
  h=900;
  OUTPUT("Setting 1440x900 as the resolution.\n");
 } else
 if (CmdLine.Option("+1440x960")  ) {
  w=1440;
  h=960;
  OUTPUT("Setting 1440x960 as the resolution.\n");
 } else
 if (CmdLine.Option("+1680x1050")   || CmdLine.Option("+wsxga+")  ) {
  w=1680;
  h=1050;
  OUTPUT("Setting 1680x1050 (WSXGA+) as the resolution.\n");
 } else
 if (CmdLine.Option("+1600x1200")   || CmdLine.Option("+uxga")  ) {
  w=1600;
  h=1200;
  OUTPUT("Setting 1600x1200 (UXGA) as the resolution.\n");
 } else
 if (CmdLine.Option("+1768x992")  ) {
  w=1768;
  h=992;
  OUTPUT("Setting 1768x992 as the resolution.\n");
 } else
 if (CmdLine.Option("+1920x1080")   || CmdLine.Option("+hd1080")  || CmdLine.Option("+1080")  || CmdLine.Option("+1080i")  || CmdLine.Option("+1080p")  ) {
  w=1920;
  h=1080;
  OUTPUT("Setting 1920x1080 (HD 1080i) as the resolution.\n");
 } else
 if (CmdLine.Option("+1900x1080")  ) {
  w=1900;
  h=1080;
  OUTPUT("Setting 1900x1080 (HD 1080i) as the resolution.\n");
 } else
 if (CmdLine.Option("+1920x540")  ) {
  w=1920;
  h=540;
  OUTPUT("Setting 1920x540 (non-interlaced psuedo HD1080) as the resolution.\n");
 } else
 if (CmdLine.Option("+1920x1200")   || CmdLine.Option("+wuxga")  ) {
  w=1920;
  h=1200;
  OUTPUT("Setting 1920x1200 (WUXGA) as the resolution.\n");
 } else
 if (CmdLine.Option("+2048x1080")   || CmdLine.Option("+2k")  ) {
  w=2048;
  h=1080;
  OUTPUT("Setting 2048x1080 (2K) as the resolution.\n");
 } else
 if (CmdLine.Option("+2048x1536")   || CmdLine.Option("+qxga")  ) {
  w=2048;
  h=1536;
  OUTPUT("Setting 2048x1536 (QXGA) as the resolution.\n");
 } else
 if (CmdLine.Option("+2560x1440")   || CmdLine.Option("+cinema")  ) {
  w=2560;
  h=1440;
  OUTPUT("Setting 2560x1440 (Apple Cinema T) as the resolution.\n");
 } else
 if (CmdLine.Option("+2560x1600")   || CmdLine.Option("+wqxga")  ) {
  w=2560;
  h=1600;
  OUTPUT("Setting 2560x1600 (WQXGA) as the resolution.\n");
 } else
 if (CmdLine.Option("+2560x2048")   || CmdLine.Option("+qsxga")  ) {
  w=2560;
  h=2048;
  OUTPUT("Setting 2560x2048 (WSXGA) as the resolution.\n");
 } else
  // Matrox TripleHead2Go Support
 if (CmdLine.Option("+1600x600")   || CmdLine.Option("+32godual1")  ) {
  w=1600;
  h=600;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 1600x600 (Matrox TripleHead2Go DualMode 1) as the resolution.\n");
 } else
 if (CmdLine.Option("+2048x768")   || CmdLine.Option("+32godual2")  ) {
  w=2048;
  h=768;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2048x768 (Matrox TripleHead2Go DualMode 2) as the resolution.\n");
 } else
 if (CmdLine.Option("+2560x720")   || CmdLine.Option("+32godual3")  ) {
  w=2560;
  h=720;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2560x720 (Matrox TripleHead2Go DualMode 3) as the resolution.\n");
 } else
 if (CmdLine.Option("+2560x768")   || CmdLine.Option("+32godual4")  ) {
  w=2560;
  h=768;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2560x768 (Matrox TripleHead2Go DualMode 4) as the resolution.\n");
 } else 
 if (CmdLine.Option("+2560x800")   || CmdLine.Option("+32godual5")  ) {
  w=2560;
  h=800;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2560x800 (Matrox TripleHead2Go DualMode 5) as the resolution.\n");
 } else
 if (CmdLine.Option("+2560x800")   || CmdLine.Option("+32godual6")  ) {
  w=2560;
  h=800;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2560x800 (Matrox TripleHead2Go DualMode 6) as the resolution.\n");
 } else
 if (CmdLine.Option("+2560x960")   || CmdLine.Option("+32godual7")  ) {
  w=2560;
  h=960;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2560x960 (Matrox TripleHead2Go DualMode 7) as the resolution.\n");
 } else
 if (CmdLine.Option("+2560x1024")   || CmdLine.Option("+32godual8")  ) {
  w=2560;
  h=1024;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2560x1024 (Matrox TripleHead2Go DualMode 8) as the resolution.\n");
 } else 
 if (CmdLine.Option("+2720x768")   || CmdLine.Option("+32godual9")  ) {
  w=2720;
  h=768;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2720x768 (Matrox TripleHead2Go DualMode 9) as the resolution.\n");
 } else
 if (CmdLine.Option("+2800x1050")   || CmdLine.Option("+32godual10")  ) {
  w=2800;
  h=1050;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2800x1050 (Matrox TripleHead2Go DualMode 10) as the resolution.\n");
 } else
 if (CmdLine.Option("+2880x900")   || CmdLine.Option("+32godual11")  ) {
  w=2880;
  h=900;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 2880x900 (Matrox TripleHead2Go DualMode 11) as the resolution.\n");
 } else
 if (CmdLine.Option("+3200x900")   || CmdLine.Option("+32godual12")  ) {
  w=3200;
  h=900;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 3200x900 (Matrox TripleHead2Go DualMode 12) as the resolution.\n");
 } else
  if (CmdLine.Option("+3200x1200")   || CmdLine.Option("+32godual13")  ) {
  w=3200;
  h=1200;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 3200x1200 (Matrox TripleHead2Go DualMode 13) as the resolution.\n");
 } else
 if (CmdLine.Option("+3360x1050")   || CmdLine.Option("+32godual14")  ) {
  w=3200;
  h=1050;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 3360x1050 (Matrox TripleHead2Go DualMode 14) as the resolution.\n");
 } else
 if (CmdLine.Option("+3840x1080")   || CmdLine.Option("+32godual15")  ) {
  w=3840;
  h=1080;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 3840x1080 (Matrox TripleHead2Go DualMode 15) as the resolution.\n");
 } else
 if (CmdLine.Option("+3840x1200")   || CmdLine.Option("+32godual16")  ) {
  w=3840;
  h=1200;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 3840x1200 (Matrox TripleHead2Go DualMode 16) as the resolution.\n");
 } else
 if (CmdLine.Option("+4096x1152")   || CmdLine.Option("+32godual17")  ) {
  w=4096;
  h=1152;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxDualMode=true;
  OUTPUT("Setting 4096x1152 (Matrox TripleHead2Go DualMode 17) as the resolution.\n");
 } else
 if (CmdLine.Option("+1920x480")   || CmdLine.Option("+32gotri1")  ) {
  w=1920;
  h=480;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 1920x480 (Matrox TripleHead2Go TripleHead Mode 1) as the resolution.\n");
 } else
 if (CmdLine.Option("+2400x480")   || CmdLine.Option("+32gotri2")  ) {
  w=2400;
  h=480;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 2400x480 (Matrox TripleHead2Go TripleHead Mode 2) as the resolution.\n");
 } else
 if (CmdLine.Option("+2400x600")   || CmdLine.Option("+32gotri3")  ) {
  w=2400;
  h=600;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 2400x600 (Matrox TripleHead2Go TripleHead Mode 3) as the resolution.\n");
 } else
 if (CmdLine.Option("+3072x768")   || CmdLine.Option("+32gotri4")  ) {
  w=3072;
  h=768;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 3072x768 (Matrox TripleHead2Go TripleHead Mode 4) as the resolution.\n");
 } else
 if (CmdLine.Option("+3840x720")   || CmdLine.Option("+32gotri5")  ) {
  w=3840;
  h=720;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 3840x720 (Matrox TripleHead2Go TripleHead Mode 5) as the resolution.\n");
 } else
 if (CmdLine.Option("+3840x768")   || CmdLine.Option("+32gotri6")  ) {
  w=3840;
  h=768;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 3840x768 (Matrox TripleHead2Go TripleHead Mode 6) as the resolution.\n");
 } else
 if (CmdLine.Option("+3840x800")   || CmdLine.Option("+32gotri7")  ) {
  w=3840;
  h=800;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 3840x800 (Matrox TripleHead2Go TripleHead Mode 7) as the resolution.\n");
 } else
 if (CmdLine.Option("+3840x960")   || CmdLine.Option("+32gotri8")  ) {
  w=3840;
  h=960;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 3840x960 (Matrox TripleHead2Go TripleHead Mode 8) as the resolution.\n");
 } else
 if (CmdLine.Option("+3840x1024")   || CmdLine.Option("+32gotri9")  ) {
  w=3840;
  h=1024;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 3840x1024 (Matrox TripleHead2Go TripleHead Mode 9) as the resolution.\n");
 } else
 if (CmdLine.Option("+3840x2160") || CmdLine.Option("+4k")) {
		 w = 3840;
		 h = 2160;
		 probablyMatroxTripleHead2Go = true;
		 probablyMatroxTripleHead = true;
		 OUTPUT("Setting 3840x2160 (4K) as the resolution.\n");
 }
 else
 if (CmdLine.Option("+4080x768")   || CmdLine.Option("+32gotri10")  ) {
  w=4080;
  h=768;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 4080x768 (Matrox TripleHead2Go TripleHead Mode 10) as the resolution.\n");
 } else
 if (CmdLine.Option("+4098x768")   || CmdLine.Option("+32gotri11")  ) {
  w=4098;
  h=768;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 4098x768 (Matrox TripleHead2Go TripleHead Mode 11) as the resolution.\n");
 } else
 if (CmdLine.Option("+4200x1050")   || CmdLine.Option("+32gotri12")  ) {
  w=4200;
  h=1050;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 4200x1050 (Matrox TripleHead2Go TripleHead Mode 12) as the resolution.\n");
 } else
 if (CmdLine.Option("+4320x900")   || CmdLine.Option("+32gotri13")  ) {
  w=4320;
  h=900;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 4320x900 (Matrox TripleHead2Go TripleHead Mode 13) as the resolution.\n");
 } else
 if (CmdLine.Option("+4800x900")   || CmdLine.Option("+32gotri14")  ) {
  w=4800;
  h=900;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 4800x900 (Matrox TripleHead2Go TripleHead Mode 14) as the resolution.\n");
 } else
 if (CmdLine.Option("+5040x1050")   || CmdLine.Option("+32gotri15")  ) {
  w=5040;
  h=1050;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 5040x1050 (Matrox TripleHead2Go TripleHead Mode 15) as the resolution.\n");
 } else
 if (CmdLine.Option("+5760x1080")   || CmdLine.Option("+32gotri16")  ) {
  w=5760;
  h=1080;
  probablyMatroxTripleHead2Go=true;
  probablyMatroxTripleHead=true;
  OUTPUT("Setting 5760x1080 (Matrox TripleHead2Go TripleHead Mode 16) as the resolution.\n");
 }
 else {
  string _res=CmdLine.OptionPair("+resolution");
  if ( _res.length() > 0 ) {
   Strings rs;
   rs.SplitPush(_res.c_str(),'x');
   String *A=(String *)rs.first;
   String *B=(String *)rs.last;
   int a=atoi(A?A->s.c_str():"100");
   int b=atoi(B?B->s.c_str():"100");
   OUTPUT("Forcing resolution to %d by %d (%f)\n",a,b,iratiod(a,b));
   w=a;
   h=b;
  } else { // Detect the display size and create the display profile
   w=GetSystemMetrics(SM_CXSCREEN);
   h=GetSystemMetrics(SM_CYSCREEN);
   OUTPUT("Detected resolution: %dx%d\n", w, h);
  }
 }
 
 if (CmdLine.Option("+8bit") ) {
  bits=8;
  OUTPUT("Using 8-bit mode by command line.\n");
 } else
 if (CmdLine.Option("+16bit") ) {
  bits=16;
  OUTPUT("Using 16-bit mode by command line.\n");
 } else
 if (CmdLine.Option("+24bit") ) {
  bits=24;
  OUTPUT("Using 24-bit mode by command line.\n");
 } else {
  bits=32;
  OUTPUT("Defaulting to 32-bit.\n");
 }
}