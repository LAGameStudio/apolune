/***********************************************************************************************
 * ____ _  _ ___  _ ____ ____    _ _  _    ___ _  _ ____    ____ _  _ _   _ (c)2009-2011       *
 * |___ |\/| |__] | |__/ |___    | |\ |     |  |__| |___    [__  |_/   \_/  Lost Astronaut     *
 * |___ |  | |    | |  \ |___    | | \|     |  |  | |___    ___] | \_   |   Studios + Ganos.biz*
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include "Chemistry.h"

#include "WorldShader.h"

Substances substances;

Substance *Substances::CreateWorldShader( const char *name, const char *fn, int num_lights, float value ) {
 WorldShader *ws=new WorldShader;
 ws->Load(fn,num_lights);
 materials.Append(ws);  
 Substance *s=new Substance;
 s->Assign(name, value, ws);
 substances.Append(s);
 return s;
}



void CreateChemistry() {

}