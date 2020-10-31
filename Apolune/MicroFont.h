#pragma once

#include "TexFont.h"

extern Zp<TexFont> microFont;

void Small( const char *s, double x, double y );
void Small( const char *s, double x, double y, Crayon tint );
void Small( const char *s, double x, double y, Crayon tint, Blends blend );
void SmallShadow( const char *s, double x, double y, Crayon tint, Blends blend );

void Small( const char *s, int x, int y );
void Small( const char *s, int x, int y, Crayon tint );
void Small( const char *s, int x, int y, Crayon tint, Blends blend );
void SmallShadow( const char *s, int x, int y, Crayon tint, Blends blend );
