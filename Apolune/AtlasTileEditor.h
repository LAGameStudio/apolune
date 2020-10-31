#pragma once

#include "AtlasTileset.h"
#include "FastGUI.h"
#include "Campaign.h"

extern Zint stashed_bigsquare,stashed_grid22,stashed_grid44,stashed_grid88,stashed_grid1616,stashed_gridAtlas,stashed_grid_square64,stashed_grid_square64b,stashed_grid_square64c,stashed_grid_square64d;

class AtlasTileEditor : public GLWindow {
public:
 Zp<Campaign> campaign;
 Zp<AtlasTileset> ed;
 Zp<AtlasTile> edTile;
 Zp<AtlasTile> selectingHeightTile;
 Zp<Atlas> selectingHeightTileAtlas;
 Zp<MapTile> edMapTile;
 Zp<Thing> edThing;
 Zp<GLWindow> back_to;
 Zp<Rotated2dQuad> r2dquad;
 Cartesiand area,pen,leftPressed;
 Zbool selectingAtlas,doubling;
 Zp<Atlas> selectedAtlas;
 Zbool level1,level2,level3,level4,drawSquare,dropSquare;
 Cartesiand lastRect;
 Zint r,c;
 Zint lastClickedCel, stashed_Cel;
 Cartesiand selectedGridPosition;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
  if ( stashed_bigsquare == 0 ) stashed_bigsquare=art.stash.GenGrid(1,1,0.1);
  if ( stashed_grid22   == 0 ) stashed_grid22  =art.stash.GenGrid( 2, 2,0.0025,crayons.jami(0,128,128,128,255));
  if ( stashed_grid44   == 0 ) stashed_grid44  =art.stash.GenGrid( 4, 4,0.0025,crayons.jami(0, 96, 96,  0,255));
  if ( stashed_grid88   == 0 ) stashed_grid88  =art.stash.GenGrid( 8, 8,0.0025,crayons.jami(0, 64,  0, 64,255));
  if ( stashed_grid1616 == 0 ) stashed_grid1616=art.stash.GenGrid(16,16,0.0025,crayons.jami(0,  0, 32, 32,255));
  if ( stashed_grid_square64 == 0 ) stashed_grid_square64=art.stash.GenGrid(1,1,0.005,crayons.jami(0,64,192,255,255));
  if ( stashed_grid_square64b == 0 ) stashed_grid_square64b=art.stash.GenGrid(1,1,0.01,crayons.jami(0,64,192,255,255));
  if ( stashed_grid_square64c == 0 ) stashed_grid_square64c=art.stash.GenGrid(1,1,0.02,crayons.jami(0,64,192,255,255));
  if ( stashed_grid_square64d == 0 ) stashed_grid_square64d=art.stash.GenGrid(1,1,0.04,crayons.jami(0,64,192,255,255));
 }
 void SetEditing( AtlasTile *at ) {
  edTile=at;
  SetAtlas(ed->atlas);
  level1=false; level2=false; level3=true; level4=false;
  c=0; r=0;
  ReplaceCelSample(c,r);
  lastClickedCel=-1;
  if ( edMapTile ) edMapTile->SetAtlasTile(ed,edTile);
  if ( edThing ) edThing->SetAtlasTile(ed,edTile);
 }
 void SetEditing( AtlasTileset *e ) {
  ed=e;
  SetAtlas(ed->atlas);
  level1=false; level2=false; level3=true; level4=false;
  r=-1; c=-1;
  lastClickedCel=-1;
  if ( edMapTile ) edMapTile->SetAtlasTile(ed,edTile);
  if ( edThing ) edThing->SetAtlasTile(ed,edTile);
  if ( selectingHeightTile ) selectingHeightTile->SetAtlasTile(ed,edTile);
 }
 void SetAtlas( Atlas *at ) {
  selectedAtlas=at;
  if ( !at ) { return; }
  if ( ed ) {
   ed->atlas=at;
   ed->atlasName=at->name;
   ed->tiles.Update(at);
  }
  if ( at->uniformGrid ) {
   if ( stashed_gridAtlas != 0 ) art.stash.Replace(stashed_gridAtlas);
   stashed_gridAtlas=art.stash.GenGrid(at->rows,at->columns,0.003,crayons.Pick(orange));
  } else {
  }
 }
 void Between() {}
 void RenderPreview() {
  area.SetRect(64,64,768,768);
  bool was=level1;
  if (input->KeyUp(DX_1)) { level1=true; level2=level3=level4=false; }
  if (input->KeyUp(DX_2)) { level2=true; level1=level3=level4=false; }
  if (input->KeyUp(DX_3)) { level3=true; level1=level2=level4=false; }
  if (input->KeyUp(DX_4)) { level4=true; level1=level2=level3=false; }
  if (input->KeyUp(DX_5)) { level1=level2=level3=level4=false; }
  level1=fast.toggle("1",level1,(int)area.x2+8,(int)area.y,20,20);
  if ( was != level1 ) { if ( level1 ) { level2=level3=level4=false; } }
  was=level2;
  level2=fast.toggle("2",level2,(int)area.x2+8,(int)area.y+24,20,20);
  if ( was != level2 ) { if ( level2 ) { level1=level3=level4=false; } }
  was=level3;
  level3=fast.toggle("3",level3,(int)area.x2+8,(int)area.y+48,20,20);
  if ( was != level3 ) { if ( level3 ) { level1=level2=level4=false; } }
  was=level4;
  level4=fast.toggle("4",level4,(int)area.x2+8,(int)area.y+72,20,20);  
  if ( was != level4 ) { if ( level4 ) { level1=level2=level3=false; } }
  was=(!level4&&!level3&&!level2&&!level1);
  bool level5=fast.toggle("Above",was,(int)area.x2+8,(int)area.y+96,20,20);  
  if ( level5 && !was ) level1=level2=level3=level4=false;
//  if ( !level1 && !level2 && !level3 && !level4 ) level4=true;
  drawSquare=fast.toggle("Draw Square",drawSquare,(int)area.x2+8,(int)area.y+120,20,20);  
  dropSquare=fast.toggle("Drop Square",dropSquare,(int)area.x2+8,(int)area.y+144,20,20);  
  art.Rectangle(crayons.Pick(black),area.x,area.y,area.w,area.h);
  if ( edTile->stashed_id > 0 && ed->atlas->texture ) {
   art.Element(ed->atlas->texture,crayons.Pick(alabaster),transparency,edTile->stashed_id,area.x,area.y,area.w,area.h);
   art.Element(ed->atlas->texture,crayons.Pick(alabaster),transparency,edTile->stashed_id,display->w-128-8,display->h-128-8,128,128);
   art.Element(ed->atlas->texture,crayons.Pick(alabaster),transparency,edTile->stashed_id,display->w-128-8-128,display->h-128-8,128,128);
   art.Element(ed->atlas->texture,crayons.Pick(alabaster),transparency,edTile->stashed_id,display->w-256-8,display->h-128-8-258-8,256,256);
   art.Element(ed->atlas->texture,crayons.Pick(alabaster),transparency,edTile->stashed_id,display->w-256-8,display->h-128-8-258-8-256,256,256);
  }
  if (!selectingHeightTile && fast.button("Pick Height", display->w - 256 - 8 - 256, display->h - 128 - 8 - 256 - 8 - 256, 256, 256)) {
   AtlasTileEditor *hms=new AtlasTileEditor;
   hms->selectingHeightTile=edTile;
   hms->selectingHeightTileAtlas=ed->atlas;
   hms->back_to=this;
   hms->campaign=campaign;
   this->visible=false;
   windows.Add(hms);
   return;
  }
  if ( fast.button("X",display->w - 16 - 8 - 256, display->h - 128 - 8 - 8 - 256, 16, 16) ) {
   edTile->at=null;
   edTile->ats=null;
   edTile->atlasTileName="";
   edTile->atlasTilesetName="";
  }
  if ( edTile->at && edTile->ats && edTile->ats->atlas ) {
   if (input->KeyDown(DX_H)) {
    Crayon t; t.Double(0.5,0.5,0.5,0.5);
    edTile->at->Render(t,transparency,edTile->ats->atlas,area.x,area.y,area.w,area.h);
   }
   edTile->at->Render(crayons.Pick(alabaster),none,edTile->ats->atlas,display->w-256-8-256,display->h-128-8-256-8-256,256,256);
  }
  if ( selectingHeightTile && selectingHeightTileAtlas ) {
   Crayon t; t.Double(0.5,0.5,0.5,0.25);
   selectingHeightTile->Render(t,transparency,selectingHeightTileAtlas,area.x,area.y,area.w,area.h);
  }
  art.Element(crayons.jami(0,128,128,128,255),transparency,stashed_grid1616,area.x,area.y,area.w,area.h);
  art.Element(crayons.jami(0, 96, 96,  0,255),transparency,stashed_grid88,area.x,area.y,area.w,area.h);
  art.Element(crayons.jami(0, 64,  0, 64,255),transparency,stashed_grid44,area.x,area.y,area.w,area.h);
  art.Element(crayons.jami(0,  0, 32, 32,255),transparency,stashed_grid22,area.x,area.y,area.w,area.h);
  bool hovers=area.within(input->mxi,input->myi);
  if ( drawSquare && hovers ) {
   if ( !dropSquare ) {
    if ( input->rightReleased() ) {
     r2dquad=null;
    }
    if ( input->leftPressed() ) {
     if ( !r2dquad ) {
      r2dquad=new Rotated2dQuad;
      if ( level1 ) {
       edTile->quads1.Append(r2dquad);
      } else if ( level2 ) {
       edTile->quads2.Append(r2dquad);
      } else if ( level3 ) {
       edTile->quads3.Append(r2dquad);
      } else if ( level4 ) {
       edTile->quads4.Append(r2dquad);
      } else {
       edTile->quads.Append(r2dquad);
      }
     }
     leftPressed.Set(input->mxi,input->myi);
    }
    if ( input->leftDepressed() ) { 
     if ( r2dquad ) {
      Line line(leftPressed.x,leftPressed.y,input->mxi,input->myi);
      r2dquad->x=((double)-area.x+(double)leftPressed.x)/(double)area.w;
      r2dquad->y=((double)-area.y+(double)leftPressed.y)/(double)area.h;
      r2dquad->a=rad2deg(line.LineAngle())-90.0;
      if ( !input->KeyDown(DX_LALT) && !input->KeyDown(DX_RALT) ) {
       r2dquad->w=((double)-area.x+line.x2-line.x)/(double)area.w;
       r2dquad->h=((double)-area.y+line.y2-line.y)/(double)area.h;
      }
      lastRect.SetRect(r2dquad->x,r2dquad->y,r2dquad->w,r2dquad->h);
      lastRect.d=r2dquad->a;
      Set_r2dquad_Texcoords();
     }
    }
    if ( input->leftReleased() ) {
     if ( r2dquad ) {
      Line line(leftPressed.x,leftPressed.y,input->mxi,input->myi);
      r2dquad->x=((double)-area.x+(double)leftPressed.x)/(double)area.w;
      r2dquad->y=((double)-area.y+(double)leftPressed.y)/(double)area.h;
      r2dquad->a=rad2deg(line.LineAngle())-90.0;
      if ( !input->KeyDown(DX_LALT) && !input->KeyDown(DX_RALT) ) {
       r2dquad->w=((double)-area.x+line.x2-line.x)/(double)area.w;
       r2dquad->h=((double)-area.y+line.y2-line.y)/(double)area.h;
      }
      lastRect.SetRect(r2dquad->x,r2dquad->y,r2dquad->w,r2dquad->h);
      lastRect.d=r2dquad->a;
      Set_r2dquad_Texcoords();
      if ( !input->KeyDown(DX_LCTRL) && !input->KeyDown(DX_RCTRL) ) {
       r2dquad=null;
      }
     }
    }
   } else {
    if ( input->leftPressed() ) {
     r2dquad=new Rotated2dQuad;
     if ( level1 ) {
      edTile->quads1.Append(r2dquad);
     } else if ( level2 ) {
      edTile->quads2.Append(r2dquad);
     } else if ( level3 ) {
      edTile->quads3.Append(r2dquad);
     } else if ( level4 ) {
      edTile->quads4.Append(r2dquad);
     } else {
      edTile->quads.Append(r2dquad);
     }
     leftPressed.Set(input->mxi,input->myi);
     r2dquad->x=((double)-area.x+(double)leftPressed.x)/(double)area.w;
     r2dquad->y=((double)-area.y+(double)leftPressed.y)/(double)area.h;
     r2dquad->w=lastRect.w;
     r2dquad->h=lastRect.h;
     r2dquad->a=lastRect.d;
     Set_r2dquad_Texcoords();
    }
    if ( input->leftDepressed() ) { 
     if ( r2dquad && !input->KeyDown(DX_LCTRL) && !input->KeyDown(DX_RCTRL) ) {
      Line line(leftPressed.x,leftPressed.y,input->mxi,input->myi);
      r2dquad->a=rad2deg(line.LineAngle())-90.0;
      Set_r2dquad_Texcoords();
     }
    }
    if ( input->leftReleased() ) {
     if ( r2dquad ) {
      if (  !input->KeyDown(DX_LCTRL) && !input->KeyDown(DX_RCTRL) ) {
       Line line(leftPressed.x,leftPressed.y,input->mxi,input->myi);
       r2dquad->a=rad2deg(line.LineAngle())-90.0;
       lastRect.SetRect(r2dquad->x,r2dquad->y,r2dquad->w,r2dquad->h);
       lastRect.d=r2dquad->a;
       Set_r2dquad_Texcoords();
      }
     }
    }
   }
  } else if ( hovers ) {
   Crayon tint; tint.Int(64,0,0,255);
   Cartesiand gridPosition;
   for ( int i=0; i<16; i++ ) for ( int j=0; j<16; j++ ) {
    double sx=area.x+(double)i*area.w/16.0;
    double sy=area.y+(double)j*area.h/16.0;
    double dx=(double)area.w/16.0;
    double dy=(double)area.h/16.0;
    bool hovering=WITHIN(input->mxi,input->myi,(int)sx,(int)sy,(int)(sx+dx),(int)(sy+dy));
    if ( hovering && level4 ) {
     gridPosition.SetRect(sx,sy,dx,dy);
     if ( input->leftReleased() ) { edTile->level4(i,j)=(r*ed->atlas->columns+c); edTile->Update(ed->atlas); }
     if ( input->rightReleased() ) { edTile->level4(i,j)=-1; edTile->Update(ed->atlas); }
    }
   }
   if ( level4 ) {
    art.Element(tint,additive,stashed_grid_square64d,gridPosition.x,gridPosition.y,gridPosition.w,gridPosition.h);
   }
   for ( int i=0; i<8; i++ ) for ( int j=0; j<8; j++ ) {
    double sx=area.x+(double)i*area.w/8.0;
    double sy=area.y+(double)j*area.h/8.0;
    double dx=(double)area.w/8.0;
    double dy=(double)area.h/8.0;
    bool hovering=WITHIN(input->mxi,input->myi,(int)sx,(int)sy,(int)(sx+dx),(int)(sy+dy));
    if ( hovering && level3 ) {
     gridPosition.SetRect(sx,sy,dx,dy);
     if ( input->leftReleased() ) { edTile->level3(i,j)=(r*ed->atlas->columns+c); edTile->Update(ed->atlas); }
     if ( input->rightReleased() ) { edTile->level3(i,j)=-1; edTile->Update(ed->atlas); }
    }
   }
   if ( level3 ) {
    art.Element(tint,additive,stashed_grid_square64c,gridPosition.x,gridPosition.y,gridPosition.w,gridPosition.h);
   }
   for ( int i=0; i<4; i++ ) for ( int j=0; j<4; j++ ) {
    double sx=area.x+(double)i*area.w/4.0;
    double sy=area.y+(double)j*area.h/4.0;
    double dx=(double)area.w/4.0;
    double dy=(double)area.h/4.0;
    bool hovering=WITHIN(input->mxi,input->myi,(int)sx,(int)sy,(int)(sx+dx),(int)(sy+dy));
    if ( hovering && level2 ) {
     gridPosition.SetRect(sx,sy,dx,dy);
     if ( input->leftReleased() ) { edTile->level2(i,j)=(r*ed->atlas->columns+c); edTile->Update(ed->atlas); }
     if ( input->rightReleased() ) { edTile->level2(i,j)=-1; edTile->Update(ed->atlas); }
    }
   }
   if ( level2 ) {
    art.Element(tint,additive,stashed_grid_square64b,gridPosition.x,gridPosition.y,gridPosition.w,gridPosition.h);
   }
   for ( int i=0; i<2; i++ ) for ( int j=0; j<2; j++ ) {
    double sx=area.x+(double)i*area.w/2.0;
    double sy=area.y+(double)j*area.h/2.0;
    double dx=(double)area.w/2.0;
    double dy=(double)area.h/2.0;
    bool hovering=WITHIN(input->mxi,input->myi,(int)sx,(int)sy,(int)(sx+dx),(int)(sy+dy));
    if ( hovering && level1 ) {
     gridPosition.SetRect(sx,sy,dx,dy);
     if ( input->leftReleased() ) { edTile->level1(i,j)=(r*ed->atlas->columns+c); edTile->Update(ed->atlas); }
     if ( input->rightReleased() ) { edTile->level1(i,j)=-1; edTile->Update(ed->atlas); }
    }
   }
   if ( level1 ) {
    art.Element(tint,additive,stashed_grid_square64,gridPosition.x,gridPosition.y,gridPosition.w,gridPosition.h);
   }
  }
 }
 void AtlasSelector() {
  double sx=0.0;
  double sy=0.0;
  double dx=128.0;
  double dy=128.0;
  int i=0;
  Cartesiand gridSelection;
  EACH(atlasLibrary.first,Atlas,at) {
   art.Rectangle(crayons.Pick(black),sx,sy,dx,dy);
   if ( i < selectorScrolled ) { i++; continue; }
   bool hovering=WITHIN(input->mxi,input->myi,(int)sx,(int)sy,(int)(sx+dx),(int)(sy+dy));
   if ( at->texture ) art.Stretch(at->texture,ed->tint,sx,sy,dx,dy,ed->flipX,ed->flipY);
   if ( hovering ) {
    gridSelection.SetRect(sx,sy,dx,dy);
    if ( input->leftReleased() ) {
     SetAtlas(at);
     selectingAtlas=false;
     return;
    }
   } else {
   }
   sx+=dx;
   if ( sx > (double)(display->w-128) ) {
    sy+=dy;
    sx=0.0;
    if ( sy > display->h-128 ) break;
   }
   i++;
  }
  if ( gridSelection.w != 0 )
   art.Element(crayons.Pick(alabaster),transparency,stashed_bigsquare,gridSelection.x,gridSelection.y,gridSelection.w,gridSelection.h);
 }
 void Set_r2dquad_Texcoords() {
  int lookup=c+r*ed->atlas->columns;
  Cartesiand atlasCoords;
  Atlas *in=ed->atlas;
  if ( in->uniformGrid ) {
   double ax=lookup%in->columns / (double)in->columns;
   double ay=lookup/in->columns / (double)in->rows;
   atlasCoords.SetRect( ax, ay, (1.0/(double)in->columns), (1.0/(double)in->rows) );
  } else {
   atlasCoords.Set(&in->coords[in->GetElement(lookup)]);
  }
  r2dquad->tc.SetRect(atlasCoords.x,atlasCoords.y,atlasCoords.w,atlasCoords.h);
	 edTile->Update(ed->atlas);
 }
 Zint r2dquadListScrolled;
 void TileInspector() {
  area.SetRect(64+768+16+64,16,768-64,384);
  //art.Rectangle(crayons.Pick(gray),area.x,area.y,area.w,area.h);
  Text("OVERLAY RECTANGLES",area.x,area.y,9.0,12.0);
  if ( fast.button("+",(int)area.x2-16,(int)area.y,16,16) ) {
   r2dquad=new Rotated2dQuad;
   edTile->quads.Append(r2dquad);
  }
  Rotated2dQuads *ourQuads=null;
  if ( level1 ) {
   ourQuads=&edTile->quads1;
  } else if ( level2 ) {
   ourQuads=&edTile->quads2;
  } else if ( level3 ) {
   ourQuads=&edTile->quads3;
  } else if ( level4 ) {
   ourQuads=&edTile->quads4;
  } else {
   ourQuads=&edTile->quads;
  }
  if ( ourQuads ) {
   int selected=r2dquad && ourQuads->ListIteminList(r2dquad) ? ourQuads->IndexOf(r2dquad) : -1;
   int selection=fast.list(ourQuads,(int)area.x2-64,(int)area.y+32,48,16,&selected,&r2dquadListScrolled.value);
   if ( selected == selection && selection >= 0 ) r2dquad=null;
   else if ( selection >= 0 && selection != selected ) r2dquad=(Rotated2dQuad *) ourQuads->Element(selection);
  }
  if ( r2dquad ) {
   if ( fast.button(fast.rotateLeft,(int)area.x,(int)area.y2-32,32,32) ) edTile->Update(ed->atlas);
   if ( fast.button(fast.closeIcon,(int)area.x+32+8,(int)area.y2-32,32,32) ) {
    if ( edTile->quads1.ListIteminList(r2dquad) ) {
     edTile->quads1.Remove(r2dquad);
    } else if ( edTile->quads2.ListIteminList(r2dquad) ) {
     edTile->quads2.Remove(r2dquad);
    } else if ( edTile->quads3.ListIteminList(r2dquad) ) {
     edTile->quads3.Remove(r2dquad);
    } else if ( edTile->quads4.ListIteminList(r2dquad) ) {
     edTile->quads4.Remove(r2dquad);
    } else if ( edTile->quads.ListIteminList(r2dquad) ) {
     edTile->quads.Remove(r2dquad);
    }
    delete r2dquad.pointer;
    r2dquad=null;
    return;
   }
   Cartesiand rect;
   rect.SetRect(r2dquad->x,r2dquad->y,r2dquad->w,r2dquad->h);
   double wasA=r2dquad->a;
   r2dquad->a=fast.angleDegrees(r2dquad->a,(int)area.x+32,(int)area.y+128,32);
   fast.arrows("Move",&rect,(int)area.x+32,(int)area.y+128+128,32,false,0.05);
   fast.xywh(true,&rect,(int)area.x+32+128,(int)area.y+32,-1.0,2.0);
   bool changed=wasA != r2dquad->a;
   if ( r2dquad->x != rect.x ) { r2dquad->x=rect.x; changed=true; }
   if ( r2dquad->y != rect.y ) { r2dquad->y=rect.y; changed=true; }
   if ( r2dquad->w != rect.w ) { r2dquad->w=rect.w; changed=true; }
   if ( r2dquad->h != rect.h ) { r2dquad->h=rect.h; changed=true; }
   if ( changed ) { edTile->Update(ed->atlas); }
   r2dquad->tintA=fast.colorPickerTiny(r2dquad->tintA,(int)area.x+128+32,(int)area.y+64+32,128,48);
   r2dquad->tintB=fast.colorPickerTiny(r2dquad->tintB,(int)area.x+128+8+128+32,(int)area.y+64+32,128,48);
   r2dquad->tintC=fast.colorPickerTiny(r2dquad->tintC,(int)area.x+128+8+128+32,(int)area.y+64+64+32,128,48);
   r2dquad->tintD=fast.colorPickerTiny(r2dquad->tintD,(int)area.x+128+32,(int)area.y+64+64+32,128,48);
   if ( r2dquad && ed->atlas && fast.button("Set Cel Texcoords",(int)area.x+32+128,(int)area.y+64+64+32+64+64,256,16) ) {
    Set_r2dquad_Texcoords();
   }
   rect.SetRect(r2dquad->tc.x,r2dquad->tc.y,r2dquad->tc.w,r2dquad->tc.h);
   fast.xywh(true,&rect,(int)area.x+32+128,(int)area.y+64+64+32+64,-1.0,2.0);
   r2dquad->tc.SetRect(rect.x,rect.y,rect.w,rect.h);
  }
 }
 void ReplaceCelSample( int i, int j ) {
  if ( stashed_Cel != 0 ) art.stash.Replace(stashed_Cel);
  double tcdx=1.0/(double)ed->atlas->columns;
  double tcdy=1.0/(double)ed->atlas->rows;
  stashed_Cel=art.stash.GenQuad(
   0.0,0.0,
   1.0,0.0,
   1.0,1.0,
   0.0,1.0,
   tcdx*(double)i,tcdy*(double)j,
   tcdx*(double)i+tcdx,tcdy*(double)j+tcdy,
   crayons.Pick(alabaster)
  );
 }
 void TilePalette() {
  area.SetRect(64+768+16,64+512+16,384,384);
  art.Rectangle(crayons.Pick(black),area.x,area.y,area.w,area.h);
  if ( ed->atlas ) {
   if ( ed->atlas->texture ) {
    art.Stretch(ed->atlas->texture,ed->tint,area.x,area.y,area.w,area.h,ed->flipX,ed->flipY);
   }
   if ( ed->atlas->uniformGrid ) {
    art.Element(crayons.Pick(red255),additive,stashed_gridAtlas,area.x,area.y,area.w,area.h);
    if ( area.within(input->mxi,input->myi) ) {
     Cartesiand gridPosition;
     for ( int i=0; i<ed->atlas->columns; i++ ) for ( int j=0; j<ed->atlas->rows; j++ ) {
      double dx=(double)area.w/(double)ed->atlas->columns;
      double dy=(double)area.h/(double)ed->atlas->rows;
      double sx=area.x+(double)i*dx;
      double sy=area.y+(double)j*dy;
      bool hovering=WITHIN(input->mxi,input->myi,(int)sx,(int)sy,(int)(sx+dx-1),(int)(sy+dy-1));
      if ( hovering ) {
       gridPosition.SetRect(sx,sy,dx,dy);
       if ( input->leftReleased() ) {
        r=j;
        c=i;
        selectedGridPosition.Set(&gridPosition);
        ReplaceCelSample(i,j);
       }
       if ( input->rightReleased() ) { r=-1; c=-1; }
      }
     }
     art.Element(crayons.Pick(black),transparency,stashed_bigsquare,gridPosition.x,gridPosition.y,gridPosition.w,gridPosition.h);
     art.Element(crayons.Pick(green255),transparency,stashed_bigsquare,gridPosition.x-1,gridPosition.y-1,gridPosition.w+2,gridPosition.h+2);
    }
    if ( c >= 0 && r >= 0 ) {
     Cartesian gridPosition;
     double dx=(double)area.w/(double)ed->atlas->columns;
     double dy=(double)area.h/(double)ed->atlas->rows;
     double sx=area.x+(double)c*dx;
     double sy=area.y+(double)r*dy;
     gridPosition.SetRect(sx,sy,dx,dy);
     art.Element(crayons.Pick(black),transparency,stashed_bigsquare,gridPosition.x,gridPosition.y,gridPosition.w,gridPosition.h);
     art.Element(crayons.Pick(alabaster),transparency,stashed_bigsquare,gridPosition.x-2,gridPosition.y-2,gridPosition.w+4,gridPosition.h+4);
    }
   }
  }
  if ( input->KeyUp(DX_LEFT) ) {
   if ( c >= 0 && r >= 0 ) {
    c-=1; if ( c < 0 ) c =0;
   } else { c=0; r=0; }   
   ReplaceCelSample(c,r);
  }
  if ( input->KeyUp(DX_RIGHT) ) {
   if ( c >= 0 && r >= 0 ) {
    c+=1; if ( c >= ed->atlas->columns ) c =ed->atlas->columns-1;
   } else { c=0; r=0; }
   ReplaceCelSample(c,r);
  }
  if ( input->KeyUp(DX_UP) ) {
   if ( c >= 0 && r >= 0 ) {
    r-=1; if ( r < 0 ) r =0;
   } else { c=0; r=0; }
   ReplaceCelSample(c,r);
  }
  if ( input->KeyUp(DX_DOWN) ) {
   if ( c >= 0 && r >= 0 ) {
    r+=1; if ( r >= ed->atlas->rows ) r =ed->atlas->rows-1;
   } else { c=0; r=0; }
   ReplaceCelSample(c,r);
  }
  if ( ed->atlas && stashed_Cel != 0 ) {
   art.Element(ed->atlas->texture,crayons.Pick(alabaster),transparency,stashed_Cel,area.x2+8,area.y2-64,64,64);
   art.Element(ed->atlas->texture,crayons.Pick(alabaster),transparency,stashed_Cel,area.x2+8+64+8,area.y2-128,128,128);
  }
 }
 void TileEdit() {
  TileInspector();
  TilePalette();
  RenderPreview();
  if ( input->KeyUp(DX_ESCAPE) || fast.button("<-",8,display->h-32,24,16) ) { edTile=null; r2dquad=null; return; }
  if ( !edTile ) return;
  edTile->name=fast.textbox("TILE Reference",edTile->name,64,display->h-64,30);
  if ( fast.button("SAVE",8+24+8,display->h-32,64,16) ) {
   campaign->Save();
  }
  if ( ( edThing || edMapTile || selectingHeightTile ) && edTile->name.length >0 && ed->name.length >0
    && fast.button(
     edThing             ? "SELECT THIS TILE AND RETURN TO THING EDITOR <+" :
     edMapTile           ? "SELECT THIS TILE AND RETURN TO MAP EDITOR <+" :
     selectingHeightTile ? "SELECT THIS TILE AS HEIGHT MAP AND RETURN <+" :
                           "SELECT THIS TILE AND RETURN TO PREVIOUS <+",
     8+24+8+64+8,8,512,24) ) 
  {
   this->destroyMe=true;
   this->back_to->visible=true;
   if ( selectingHeightTile ) {
    selectingHeightTile->at=edTile;
    selectingHeightTile->ats=ed;
    selectingHeightTile->atlasTileName=edTile->name;
    selectingHeightTile->atlasTilesetName=ed->name;
   }
   return;
  }
  if ( fast.button(fast.leftArrow, 384, display->h - 96, 32, 32))      { edTile->FlipHoriz(); edTile->Update(ed->atlas); }
  if ( fast.button(fast.upArrow, 384+32, display->h - 96, 32, 32))     { edTile->FlipVert();	 edTile->Update(ed->atlas); }
  if ( fast.button(fast.rotateLeft, 384+64, display->h - 96, 32, 32))  { edTile->RotateLeft();	 edTile->Update(ed->atlas); }
  if ( fast.button(fast.rotateRight, 384+96, display->h - 96, 32, 32)) { edTile->RotateRight();	 edTile->Update(ed->atlas); }
  if ( fast.tbutton("L=R",384, display->h-64,32,32,false) ) { edTile->MirrorRight();	 edTile->Update(ed->atlas); }
  if ( fast.tbutton("U=D",384+32, display->h-64,32,32,false) ) { edTile->MirrorUp();	 edTile->Update(ed->atlas); }
  if ( fast.tbutton("D=U",384+64, display->h-64,32,32,false) ) { edTile->MirrorDown();	 edTile->Update(ed->atlas); }
  if ( fast.tbutton("R=L",384+96, display->h-64,32,32,false) ) { edTile->MirrorLeft();	 edTile->Update(ed->atlas); }
 }
 void TileSelector() {
//  area.SetRect(16+512+16,16,512,512);
//  art.Rectangle(crayons.Pick(black),area.x,area.y,area.w,area.h);
  double sx=0.0;
  double sy=0.0;
  double dx=128.0;
  double dy=128.0;
  int i=0;
  Cartesiand gridSelection;
  EACH(ed->tiles.first,AtlasTile,at) {
   art.Rectangle(crayons.Pick(black),sx,sy,dx,dy);
   if ( i < selectorScrolled ) { i++; continue; }
   bool hovering=WITHIN(input->mxi,input->myi,(int)sx,(int)sy,(int)(sx+dx),(int)(sy+dy));
   at->Render(ed->tint,ed->atlas,sx,sy,dx,dy);
   art.Rectangle(crayons.Pick(ANSIboldgrey),additive,sx,sy+dy/2.0,dx,16.0);
   Text(at->name,crayons.Pick(black),none,sx+7.0,sy+dy/2.0+3.0,10.0,14.0);
  // Text(at->name,crayons.Pick(green255),none,sx+8.0,sy+dy/2.0+2.0);
   if ( hovering ) {
    gridSelection.SetRect(sx,sy,dx,dy);
    if ( input->leftReleased() ) { SetEditing(at); return; }
    if ( input->rightReleased() ) { 
     AtlasTile *at2=at->Duplicate();
     at2->name=I2S(ed->tiles.count);
     at2->Update(ed->atlas);
     ed->tiles.Append(at2);
     return;
    }
   }
   sx+=dx;
   if ( sx > (double)(display->w-128) ) {
    sy+=dy;
    sx=0.0;
    if ( sy > display->h-128 ) break;
   }
   i++;
  }
  if ( gridSelection.w != 0 )
   art.Element(crayons.Pick(alabaster),transparency,stashed_bigsquare,gridSelection.x,gridSelection.y,gridSelection.w,gridSelection.h);
  if ( input->KeyUp(DX_ESCAPE) || fast.button("<-",8,display->h-32,24,16) ) {
   ed=null; return;
  }
  ed->name=fast.textbox("TILESET Reference Name",ed->name,256,display->h-64,30);
  if ( fast.button("SAVE",8+24+8,display->h-32,64,16) ) {
   campaign->Save();
  }
  if ( fast.button("+ TILE",32,display->h-48-8,96,16) ) {
   AtlasTile *a=new AtlasTile;
   a->name=I2S(ed->tiles.count);
   ed->tiles.Append(a);
   edTile=a;
   SetEditing(a);
  }
 }
 void RenderEditor() {
  if ( selectingAtlas || !ed->atlas ) {
   AtlasSelector();
   if ( input->KeyUp(DX_ESCAPE) || fast.button("<-",8,display->h-32,24,16) ) {
    ed=null; return;
   }
   return; 
  }
  if ( edTile ) TileEdit();
  else TileSelector();
 }
 Zint selectorScrolled;
 void RenderSelector() {
  double sx=0.0;
  double sy=0.0;
  double dx=128.0;
  double dy=128.0;
  int i=0;
  Cartesiand gridSelection;
  EACH(atlasTilesets.first,AtlasTileset,ts) {
   art.Rectangle(crayons.Pick(black),sx,sy,dx,dy);
   if ( i < selectorScrolled ) { i++; continue; }
   bool hovering=WITHIN(input->mxi,input->myi,(int)sx,(int)sy,(int)(sx+dx),(int)(sy+dy));
   if ( ts->atlas && ts->atlas->texture ) art.Stretch(ts->atlas->texture,ts->tint,sx,sy,dx,dy,ts->flipX,ts->flipY);
   art.Rectangle(crayons.Pick(ANSIboldgrey),additive,sx,sy+dy/2.0,dx,16.0);
   Text(ts->name,crayons.Pick(black),none,sx+7.0,sy+dy/2.0+3.0,10.0,14.0);
  // Text(ts->name,crayons.Pick(green255),none,sx+8.0,sy+dy/2.0+2.0);
   if ( hovering ) {
    gridSelection.SetRect(sx,sy,dx,dy);
    if ( input->leftReleased() ) { SetEditing(ts); return; }
   } else {
   }
   sx+=dx;
   if ( sx > (double)(display->w-128) ) {
    sy+=dy;
    sx=0.0;
    if ( sy > display->h-128 ) break;
   }
   i++;
  }
  if ( gridSelection.w != 0 )
   art.Element(crayons.Pick(alabaster),transparency,stashed_bigsquare,gridSelection.x,gridSelection.y,gridSelection.w,gridSelection.h);
  if ( back_to && fast.button("<-",8,display->h-32,24,16) ) {
   back_to->visible=true;
   this->destroyMe=true;
   return;
  }
  if ( fast.button("SAVE",8+24+8,display->h-32,64,16) ) {
   campaign->Save();
  }
  if ( fast.button("+ TILESET",32,display->h-64,128,16) ) {
   AtlasTileset *at=new AtlasTileset;
   at->name=I2S(atlasTilesets.count);
   atlasTilesets.Append(at);
   campaign->campaignAtlasTilesets.Add(at);
  }
 }
 void Render() {
  fast.ResponsiveRatio(w,h);
  art.PushViewport(w,h);
  if ( ed ) RenderEditor();
  else RenderSelector();
  art.PopViewport();
 }
};

