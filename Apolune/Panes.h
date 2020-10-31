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
#pragma once

#include <Windows.h>
#include "GLWindow.h"

class Panes4 : public GLWindow {
public:
 GLWindow *leftTop;
 GLWindow *rightTop;
 GLWindow *leftBottom;
 GLWindow *rightBottom;
 float W,H;

 virtual void Setup() {}

 // Sets the divider to perw*w,perh*h for leftTop, etc.
 void Divide( float perw, float perh ) {
  int W1,H1,W2,H2,W3,H3,W4,H4;
  W=perw; H=perh; 
  W1=(int) (perw * (float) w);
  H1=(int) (perh * (float) h);
  W2=w-W1; H2=H1;
  W3=W1;   H3=h-H1;
  W4=W2;   H4=H3;
  leftTop->_Resized( W1, H1 );
  leftTop->_OnResize( W1, H1 );
  rightTop->MoveTo( x+W1, y );
  rightTop->_Resized( W2, H2 );
  rightTop->_OnResize( W2, H2 );
  leftBottom->MoveTo( x, y+H1 );
  leftBottom->_Resized( W3, H3 );
  leftBottom->_OnResize( W3, H3 );
  rightBottom->MoveTo( x+W1, y+H1 );
  rightBottom->_Resized( W4, H4 );
  rightBottom->_OnResize( W4, H4 );
 }

 virtual void OnResize( int nw, int nh ) {
  Divide(W,H);
 }

 virtual void BetweenFrames(void) {
  leftTop->_BetweenFrames();
  rightTop->_BetweenFrames();
  leftBottom->_BetweenFrames();
  rightBottom->_BetweenFrames();
 }

 virtual void Between(void) {
  leftTop->_Between();
  rightTop->_Between();
  leftBottom->_Between();
  rightBottom->_Between();
 }

 virtual void Render(void) {
  leftTop->_Render();
  rightTop->_Render();
  leftBottom->_Render();
  rightBottom->_Render();
 }

 void OnLoad(void) {
  this->W=0.5f;
  this->H=0.5f;
  Setup();
  leftTop->_OnLoad();
  rightTop->_OnLoad();
  leftBottom->_OnLoad();
  rightBottom->_OnLoad();
 }

 virtual void OnUnload(void) {
  leftTop->_OnUnload();
  rightTop->_OnUnload();
  leftBottom->_OnUnload();
  rightBottom->_OnUnload();
  delete leftTop;
  delete rightTop;
  delete leftBottom;
  delete rightBottom;
 }

 virtual void MouseMove() {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->MouseMove();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->MouseMove();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->MouseMove();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->MouseMove();
 }

 virtual void MouseOver() {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->MouseOver();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->MouseOver();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->MouseOver();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->MouseOver();
 }

 virtual void LeftDown() {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->LeftDown();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->LeftDown();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->LeftDown();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->LeftDown();
 }

 virtual void RightDown() {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->RightDown();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->RightDown();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->RightDown();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->RightDown();
 }

 virtual void LeftUp() {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->LeftUp();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->LeftUp();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->LeftUp();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->LeftUp();
 }

 virtual void RightUp() {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->RightUp();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->RightUp();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->RightUp();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->RightUp();
 }

 virtual void MouseEnter(void) {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->MouseEnter();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->MouseEnter();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->MouseEnter();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->MouseEnter();
 }

 virtual void MouseLeave(void) {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->MouseLeave();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->MouseLeave();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->MouseLeave();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->MouseLeave();
 }

 virtual void Unfocused(void) {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->Unfocused();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->Unfocused();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->Unfocused();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->Unfocused();
 }

 virtual void Focused(void) {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->Focused();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->Focused();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->Focused();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->Focused();
 }

 virtual void MiddleUp() {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->MiddleUp();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->MiddleUp();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->MiddleUp();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->MiddleUp();
 }

 virtual void MiddleDown() {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->MiddleDown();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->MiddleDown();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->MiddleDown();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->MiddleDown();
 }

 virtual void WheelUp() {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->WheelUp();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->WheelUp();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->WheelUp();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->WheelUp();
 }

 virtual void WheelDown() {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->WheelDown();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->WheelDown();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->WheelDown();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->WheelDown();
 }

 virtual void LeftDouble() {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->LeftDouble();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->LeftDouble();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->LeftDouble();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->LeftDouble();
 }

 virtual void RightDouble() {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->RightDouble();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->RightDouble();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->RightDouble();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->RightDouble();
 }

 virtual void MiddleDouble() {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->MiddleDouble();
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->MiddleDouble();
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->MiddleDouble();
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->MiddleDouble();
 }

 virtual void KeyUp( KeyList *keys ) {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->KeyUp(keys);
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->KeyUp(keys);
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->KeyUp(keys);
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->KeyUp(keys);
 }

 virtual void KeyDown( KeyList *keys ) {
  if ( WITHIN(input->mxi,input->myi,leftTop->x,leftTop->y,leftTop->x2,leftTop->y2) ) leftTop->KeyDown(keys);
  if ( WITHIN(input->mxi,input->myi,rightTop->x,rightTop->y,rightTop->x2,rightTop->y2) ) rightTop->KeyDown(keys);
  if ( WITHIN(input->mxi,input->myi,leftBottom->x,leftBottom->y,leftBottom->x2,leftBottom->y2) ) leftBottom->KeyDown(keys);
  if ( WITHIN(input->mxi,input->myi,rightBottom->x,rightBottom->y,rightBottom->x2,rightBottom->y2) ) rightBottom->KeyDown(keys);
 }

};


class PanesH : public GLWindow {
public:
 GLWindow *left;
 GLWindow *right;
 float W;

  // Sets the divider to perw*w,perh*h for leftTop, etc.
 void Divide( float perw ) {
  int W1,W2;
  W=perw; 
  W1=(int) (perw * (float) w);
  W2=w-W1;  
  left->_Resized( W1, h );
  left->_OnResize( W1, h );
  right->MoveTo( x+W1, y );
  right->_Resized( W2, h );
  right->_OnResize( W2, h );
 }

 virtual void OnResize( int nw, int nh ) {
  Divide(W);
 }

 virtual void BetweenFrames(void) {
  left->_BetweenFrames();
  right->_BetweenFrames();
 }

 virtual void Between(void) {
  left->_Between();
  right->_Between();
 }

 virtual void Render(void) {
  left->_Render();
  right->_Render();
 }

 void OnLoad(void) {
  left->_OnLoad();
  right->_OnLoad();
 }

 virtual void OnUnload(void) {
  left->_OnUnload();
  right->_OnUnload();
  delete left;
  delete right;
 }

 virtual void MouseMove() {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->MouseMove();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->MouseMove();
 }

 virtual void MouseOver() {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->MouseOver();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->MouseOver();
 }

 virtual void LeftDown() {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->LeftDown();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->LeftDown();
 }

 virtual void RightDown() {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->RightDown();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->RightDown();
 }

 virtual void LeftUp() {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->LeftUp();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->LeftUp();
 }

 virtual void RightUp() {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->RightUp();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->RightUp();
 }

 virtual void MouseEnter(void) {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->MouseEnter();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->MouseEnter();
 }

 virtual void MouseLeave(void) {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->MouseLeave();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->MouseLeave();
 }

 virtual void Unfocused(void) {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->Unfocused();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->Unfocused();
 }

 virtual void Focused(void) {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->Focused();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->Focused();
 }

 virtual void MiddleUp() {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->MiddleUp();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->MiddleUp();
 }

 virtual void MiddleDown() {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->MiddleDown();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->MiddleDown();
 }

 virtual void WheelUp() {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->WheelUp();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->WheelUp();
 }

 virtual void WheelDown() {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->WheelDown();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->WheelDown();
 }

 virtual void LeftDouble() {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->LeftDouble();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->LeftDouble();
 }

 virtual void RightDouble() {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->RightDouble();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->RightDouble();
 }

 virtual void MiddleDouble() {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->MiddleDouble();
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->MiddleDouble();
 }

 virtual void KeyUp( KeyList *keys ) {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->KeyUp(keys);
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->KeyUp(keys);
 }

 virtual void KeyDown( KeyList *keys ) {
  if ( WITHIN(input->mxi,input->myi,left->x,left->y,left->x2,left->y2) ) left->KeyDown(keys);
  if ( WITHIN(input->mxi,input->myi,right->x,right->y,right->x2,right->y2) ) right->KeyDown(keys);
 }

};

class PanesV : public GLWindow {
public:
 GLWindow *top;
 GLWindow *bottom;
 float H;

 // Sets the divider to perh*h
 void Divide( float perh ) {
  int H1,H2;
  H=perh; 
  H1=(int) (perh* (float) h);
  H2=h-H1;
  top->_Resized( w, H1 );
  top->_OnResize( w, H1 );
  bottom->MoveTo( x, y+H1 );
  bottom->_Resized( w, H2 );
  bottom->_OnResize( w, H2 );
 }

 virtual void OnResize( int nw, int nh ) {
  Divide(H);
 }

 virtual void BetweenFrames(void) {
  top->_BetweenFrames();
  bottom->_BetweenFrames();
 }

 virtual void Between(void) {
  top->_Between();
  bottom->_Between();
 }

 virtual void Render(void) {
  top->_Render();
  bottom->_Render();
 }

 void OnLoad(void) {
  top->_OnLoad();
  bottom->_OnLoad();
 }

 virtual void OnUnload(void) {
  top->_OnUnload();
  bottom->_OnUnload();
  delete top;
  delete bottom;
 }

 virtual void MouseMove() {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->MouseMove();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->MouseMove();
 }

 virtual void MouseOver() {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->MouseOver();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->MouseOver();
 }

 virtual void topDown() {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->LeftDown();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->LeftDown();
 }

 virtual void RightDown() {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->RightDown();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->RightDown();
 }

 virtual void topUp() {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->LeftUp();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->LeftUp();
 }

 virtual void RightUp() {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->RightUp();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->RightUp();
 }

 virtual void MouseEnter(void) {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->MouseEnter();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->MouseEnter();
 }

 virtual void MouseLeave(void) {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->MouseLeave();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->MouseLeave();
 }

 virtual void Unfocused(void) {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->Unfocused();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->Unfocused();
 }

 virtual void Focused(void) {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->Focused();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->Focused();
 }

 virtual void MiddleUp() {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->MiddleUp();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->MiddleUp();
 }

 virtual void MiddleDown() {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->MiddleDown();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->MiddleDown();
 }

 virtual void WheelUp() {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->WheelUp();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->WheelUp();
 }

 virtual void WheelDown() {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->WheelDown();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->WheelDown();
 }

 virtual void topDouble() {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->LeftDouble();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->LeftDouble();
 }

 virtual void RightDouble() {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->RightDouble();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->RightDouble();
 }

 virtual void MiddleDouble() {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->MiddleDouble();
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->MiddleDouble();
 }

 virtual void KeyUp( KeyList *keys ) {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->KeyUp(keys);
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->KeyUp(keys);
 }

 virtual void KeyDown( KeyList *keys ) {
  if ( WITHIN(input->mxi,input->myi,top->x,top->y,top->x2,top->y2) ) top->KeyDown(keys);
  if ( WITHIN(input->mxi,input->myi,bottom->x,bottom->y,bottom->x2,bottom->y2) ) bottom->KeyDown(keys);
 }

};