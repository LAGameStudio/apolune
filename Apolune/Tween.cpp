
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/
#include <Windows.h>

#include <iostream>
#include <string>
using namespace std;

#include "macros.h"
#include "Crayon.h"
#include "Art.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "TextureLibrary.h"
#include "Tween.h"


Tween::Tween(void) : ListItem()
{
 name=string("");
}

Tween::Tween( const char *name, int length ): ListItem() {
 this->name=string(name);
 tween.Size(length);
}

Tween::Tween( string name, int length ): ListItem() {
 this->name=name;
 tween.Size(length);
}

Tween::Tween( const char *name, float *tween, size_t length ): ListItem() {
 this->name=string(name);
 this->tween.Size(length);
 for ( unsigned int i=0; i<length; i++ ) this->tween[i]=tween[i];
 Constant=true;
}

Tween::Tween( const char *name, float *tween, size_t length, bool reverse ): ListItem() {
 this->name=string(name);
 this->tween.Size(length);
 for ( unsigned int i=0; i<length; i++ ) this->tween[i]=tween[length-i-1];
 Constant=true;
}

void Tween::Generate() {
 if ( !Constant ) this->generate((unsigned int)tween.length);
}

// Programmable virtual function
float Tween::uncertainty( unsigned int p, float f ) { return f; }

void Tween::generate( unsigned int L ) {
 if ( !Generative ) return;
 unsigned int i;
 tween.Size(L);
 for ( i=0; i<L; i++ ) tween[i]= uncertainty(i, iratiof(i,L) ); 
}

// Optimize with pre-computed points list!
void Tween::Render( int x, int y, int w, int h, Crayon color ) {
 GLImage *whiteInk=library.find("data/images/elements/white.png");

#if defined(GL_DEBUG)
 GL_Assert( "[entering Tween::Render()]" );
 glReportError( glGetError() );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(additive);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, whiteInk->texture);
 glColor4fv(color.floats);

 glPushMatrix();
 glTranslatef((GLfloat) x,(GLfloat) y,0);
 glScalef((GLfloat) w, (GLfloat) h,0);

 render.Call();
 
 glPopMatrix();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting Tween::Render()]" );
 glReportError( glGetError() );
#endif
}