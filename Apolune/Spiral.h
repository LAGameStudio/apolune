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

#include "macros.h"
#include "moremath.h"
#include "Cartesian.h"
#include "Vertex.h"
#include "Vertexd.h"

enum Reflection {
 positive=1,
 negative=-1
};

enum SpiralTypes {
 spiral_archimedian,
 spiral_archimedian_reflection,
 spiral_equiangular,
 spiral_parabolic,
 spiral_logrithmic,
 spiral_root,
 spiral_inverse,
 spiral_lituus, // inverse square
 spiral_clothoide,
 spiral_curlicue,
 spiral_curlicue2, // uses Seed
// spiral_arcs,
// spiral_fibonacci,
// spiral_lines,
 spiral_helix3d,
 spiral_conical_helix3d,
 spiral_archimedian_conical_helix3d,
 spiral_equiangular_conical_helix3d,
 spiral_loxodrome // aka spherical_helix
};

extern char *SpiralTypeNames[18];

class Spirals {
public:
 SpiralTypes mode;
 Reflection reflection;
 int slices,seed;
 double Seed;
 Vertexd scale;
 double z,zScale,zIncrement;
 double t,tScale,tIncrement;
 double r,rScale,rIncrement;
 double a,aScale,aIncrement;
 Vertexd calculator;
 PROGRAMMABLE(Spirals,CalculatePtr,calculate,Calculate);
 Spirals() {
  seed=1;
  Seed=1.0;
  scale.Set(1.0,1.0,1.0);
  reflection=positive;
  slices=120;
  a=z=t=r=0.0;
  zScale=tScale=rScale=1.0;
  zIncrement=tIncrement=rIncrement=iratiod(1,slices);
  aScale=TWO_PI;
  aIncrement=zIncrement*TWO_PI;
  Mode(spiral_archimedian);
 }
 void Reset() {
  calculator.None();
  t=z=r=a=0.0;
 }
 void SetAngle( double end ) {
  aScale=end;
  aIncrement=iratiod(1,slices)*end;
 }
 void Set( SpiralTypes mode, int slices, double r, double t, double z ) {
  Mode(mode);
  this->slices=slices;
  this->rScale=r;
  this->tScale=t;
  this->zScale=z;
  double delta=iratiod(1,slices);
  this->tIncrement=tScale*delta;
  this->rIncrement=rScale*delta;
  this->zIncrement=zScale*delta;
  tScale=t;
  tIncrement=delta;
  zScale=z;
  this->r=this->t=this->z=this->a=0.0;
 }
 void SetZ( double z ) {
  zScale=z;
  zIncrement=z/(double)slices;
 }
 void SetR( double r ) {
  rScale=r;
  rIncrement=r/(double)slices;
 }
 void SetT( double t ) {
  tScale=t;
  tIncrement=t/(double)slices;
 }
 void Increment() {
  r+=rIncrement;
  t+=tIncrement;
  z+=zIncrement;
  a+=aIncrement;
 }
 void Archimedian(){
  calculator.Set(
   scale.x*(double)reflection*r*cos(t),
   scale.y*(double)reflection*r*sin(t),
   scale.z*z
  );
 }
 void Equiangular(){
  calculator.Set(
   scale.x*(double)reflection*exp(0.1*t)*cos(t),
   scale.y*(double)reflection*exp(0.1*t)*sin(t),
   scale.z*z
  );
 }
 void Parabolic(){
  calculator.Set(
   scale.x*(double)reflection*SQ(t)*cos(t),
   scale.y*(double)reflection*SQ(t)*sin(t),
   scale.z*z
  );
 }
 void Logrithmic(){
  calculator.Set(
   scale.x*(double)reflection*log(t)*cos(t),
   scale.y*(double)reflection*log(t)*sin(t),
   scale.z*z
  );
 }
 void SquareRoot(){
  calculator.Set(
   scale.x*(double)reflection*abs(sqrt(t))*cos(t),
   scale.y*(double)reflection*abs(sqrt(t))*sin(t),
   scale.z*z
  );
 }
 void Inverse(){
  calculator.Set(
   scale.x*(double)reflection*(t!=0.0?1/t:0.0)*cos(t),
   scale.y*(double)reflection*(t!=0.0?1/t:0.0)*sin(t),
   scale.z*z
  );
 }
 void InverseSquare(){
  calculator.Set(
   scale.x*(double)reflection*(t!=0.0?1/sqrt(t):0.0)*cos(t),
   scale.y*(double)reflection*(t!=0.0?1/sqrt(t):0.0)*sin(t),
   scale.z*z
  );
 }
 void Clothoide(){
//  calculator.Set(
//  );
 }
 void Curlicue() {
  calculator.Set(
   scale.x*(double)reflection*(calculator.x*cos(t*t)),
   scale.y*(double)reflection*(calculator.y*sin(t*t)),
   scale.z*z
  );
 }
 void Curlicue2() {
  double seeded=t+Seed*TWO_PI;
  calculator.Set(
   scale.x*(double)reflection*(calculator.x+cos(seeded)),
   scale.y*(double)reflection*(calculator.y+sin(seeded)),
   scale.z*z
  );
 }
// void Arcs(){}
// void Fibonacci(){}
// void Lines(){}
 void Helix3d(){
  calculator.Set(
   scale.x*(double)reflection*r*cos(t),
   scale.y*(double)reflection*r*sin(t),
   scale.z*z
  );
 }
 void ConicalArchimedianHelix3d(){
  calculator.Set(
   scale.x*(double)reflection*t*cos(6*t),
   scale.y*(double)reflection*t*sin(6*t),
   scale.z*z
  );
 }
 void ConicalEquiangularHelix3d(){
  calculator.Set(
   scale.x*(double)reflection*0.5*exp(0.15*t)*cos(2.0*t),
   scale.y*(double)reflection*0.5*exp(0.15*t)*sin(2.0*t),
   scale.z*0.5*exp(0.15*t)
  );
 }
 void Loxodrome(){
  calculator.Set(
   scale.x*(double)reflection*cos(t)*cos(1.0/tan(aScale*t)),
   scale.y*(double)reflection*sin(t)*cos(1.0/tan(aScale*t)),
   scale.z*-sin(1.0/tan(aScale*t))
  );
 }
 void NextMode() {
  if ( mode == spiral_loxodrome ) mode=spiral_archimedian;
  else mode=(SpiralTypes) ((int) mode+1);
  Mode(mode);
 }
 char *ModeName() {
  return SpiralTypeNames[(int)mode];
 }
 void Mode( SpiralTypes type ) {
  mode=type;
  switch ( type ) {
  case spiral_archimedian: calculate=&Spirals::Archimedian; break;
  case spiral_equiangular: calculate=&Spirals::Equiangular; break;
  case spiral_parabolic: calculate=&Spirals::Parabolic; break;
  case spiral_logrithmic: calculate=&Spirals::Logrithmic; break;
  case spiral_root: calculate=&Spirals::SquareRoot; break;
  case spiral_lituus: calculate=&Spirals::InverseSquare; break;
  case spiral_inverse: calculate=&Spirals::Inverse; break;
  case spiral_clothoide: calculate=&Spirals::Clothoide; break;
  case spiral_curlicue: calculate=&Spirals::Curlicue; break;
  case spiral_curlicue2: calculate=&Spirals::Curlicue2; break;
//  case spiral_arcs: calculate=&Spirals::Arcs; break;
//  case spiral_fibonacci: calculate=&Spirals::Fibonacci; break;
//  case spiral_lines: calculate=&Spirals::Lines; break;
  case spiral_helix3d: calculate=&Spirals::Helix3d; break;
  case spiral_archimedian_conical_helix3d: calculate=&Spirals::ConicalArchimedianHelix3d; break;
  case spiral_equiangular_conical_helix3d: calculate=&Spirals::ConicalEquiangularHelix3d; break;
  case spiral_loxodrome: calculate=&Spirals::Loxodrome; break;
  default: OUTPUT("Spirals::Mode() provided mode is not supported.\n"); break;
  }
 }
 Cartesian *point() {
  Calculate();
  Cartesian *c=new Cartesian;
  c->fx=(float) calculator.x;
  c->fy=(float) calculator.y;
  c->fz=(float) calculator.z;
  c->x=(int)calculator.x;
  c->y=(int)calculator.y;
  c->z=(int)calculator.z;
  return c;
 }
 Vertexd *pointd() {
   Calculate();
  return new Vertexd(&calculator);
 }
 Cartesians *points() {
  Cartesians *p=new Cartesians;
  Reset();
  for ( int i=0; i<slices; i++ ) {
   Calculate();
   Increment();
   p->Append(point());
  }
  return p;
 }
 Vertices *vertices() {
  Vertices *v=new Vertices;
  Reset();
  for ( int i=0; i<slices; i++ ) {
   Calculate();
   Increment();
   v->Append(new Vertex((float)calculator.x,(float)calculator.y,(float)calculator.z));
  }
  return v;
 }
};

extern Spirals spirals;

/*
                                                  :                                                
                                                  :                     ,,,,,,,,,,,,,:             
                                                  :                ,,,,:              ,,;;.        
                                                  :             :;;,                     :;;:      
                                                  :           ,;:                           :i.    
                                                  :         ,;,                  ...          t.   
                                                  :       .i.               :;,,,:,,,,;;,      ti  
                                                  :      :t                ;; ,;;tttt;;,,i     .t. 
                                                  :     t;               ;t :tjfKGLftGLft.t      t 
                                                  :    t:               .t. tjEEi    .fDft.i     j 
                                                  :   i:                t  i;GKt       tKt t.    t 
                                                  :  :i                 t  ;;GKf      ,LGj.i    ,, 
                                                  : ,j                  .i  tiGDGt;;ifGLj:t    it  
                                                  : t.                   ;t  ;;tijjjjtii;;    i:   
                                                  :,,                     .i: .,,,,,,,,;    :i:    
                                                  :j                        :;,,        .,;;:      
                                                  :t                          .,,,,,:,,,,:         
                                                  t,                                               
                                                  f                                                
                                                  f                                                
                                                  f                                                
                                                  j                                                
                                                  t                                                
                                                  f                                                
                                                  t                                                
                                                  t                                                
 .................................................j................................................
                                                  t                                                
                                                  t                                                
                                                  t                                                
                                                  t                                                
                                                 .t                                                
                                                 :i                                                
                                                 ,;                                                
                                                 i,                                                
                                                 t:                                                
                                                 j:                                                
            .:,::,:.                             t:                                                
        ,;,,:      :,;;:                        ;::                                                
      ;;.    ,,,,,,,:  .i:                     ,t :                                                
    :i,    ;;tiittiit;;  i:                    ti :                                                
   :i.   ,i;fGGftjjDDjit. ti                  ,;  :                                                
   t    ;j:fDj,    .LEL;t ,j                  j   :                                                
  .i    j;ttD        EDtj .t                 t.   :                                                
  :;    jiijE:      ;KGit ;t                ,;    :                                                
  .i    :t.tfDLt;tjGEfti :i.              .t,     :                                                
   j     .t:;jfLffjfj;; ,i                t       :                                                
   :;      ,;,,,,,,::,;;.               ,i        :                                                
    it       :,,,:,,,:                :i;         :                                                
      i,                            ,;:           :                                                
       :;;.                      ;;;.             :                                                
         .;;,                :,,;,                :                                                
             :,,,,,,,,,,,,,,,.                    :                                                
                    ...                           :                                                
                                                  ,                                                

*/


/*
     
                                                                                                                      .,:.                                      
                                                                                                                .::,::.                                     
                                        ,,:::,::                                                         .::::::.
                                       ;,ifj:, .:,                                                    .:,.
                                      ;;L###G.,   ;                                                  :, ::::
                        .             .ifKKEj ;    ;.                                                ;.;iLf,:,.
                   :,:::.:::::,:.       :::. ;.     ;        :::,::,.                                ;;f###D; ;
                  ;,,,;:,:      .:::::::::,::        ;.     ;,ffLt,:.,:                              ,,iDKKj, .,          ::::::.
                 ,,;EWKD,.;        .;;;;,:            ,.   ;iE####f;  ::                              .::::.  ;    :::::,j;itt::;,:
                 .iiW##Kt  ;      :tLDDf.i,.           ,,   ,jDDED;;   ;                                     ,:  .;;::::Lt####E: ;.,     
                  .;ijti.  ;      it###Wi i;             :,:. ::,:,.    ;                                  .,.   i,DKKL,;LW#KKD; ; :,   ..iiii;,.      
                          ,.       ;iLL;  ;;                .:::.       ;                                .,:     jiW##Kj .j,,,. ,i  ;,iLfDE##Ki;::
                         ,:              ; ;                            ;                               ,:        ,ijj;   ;     ;:,:;jK######Ki; ;
                       ,,             :,: ,:                            ;       .::.                  ,,                  ;   :,   .tiW####LDt:, ;
                     :,        ::::,,:   ::                             ;.    ,;,:.:::.             .,              ,,,,,,;,::.     ::iLKWDfij. ;.
                    :,.    ::::::::.     ;                              :,   ,;jLfi: :;             ;.           .,,;,,,iLit:::      ; ,ifjtt; .;
                  ,,   .,::            .;                                ,.  ;j###Ki:  ;           ,. :,:;::,:: i;    ,,i   ,,:;;;,,:i::;iiLjjji:
                 ,.  :,.               ;                                  ,: .iDDDfi:   ;          ; :,fEKEi  .f,:fDDt.i:,::::::,,iiijti;,jK##W;i: 
                i .:,i::,i,:          i                                    .:::,;,:.    ;          .,;;K###i;  j;L###DG  .::,,::      ,:  jD##K,;;     
                ;.,:fjifDKD;i:.      ;                                                  ;           :,;iffti   :iLDKGG,,,.             .ii. :,:,;:
                ;i E###EK#K;, :,.    ;   ijDLt;                                         .,            .:::       jj,jDWKGi             ,. .::.:,.      
                ,,,iKKEft;,,    :,   ::  D###Ei;                                         ;      :::::.           ,j.L####KDDjjt:::::::i,::,:::       
                 .:;;;i:          ,,  ,: ;GDDii,                                         ;    ,,,,:  .,:.         iiijGjK###EEj       ;  ijDji,.
                                    :,:.,,::,i:                                          ;.  ;:LWWEt,   ,:        ;  ,jj,LEEffi       ; ,f###Ei;
                                       .::::.                                             ,. ,,G###j;    .,       .;  ;;L###WWi,      .,.iGEDi,;       
                                                                                           :,.:,iti;:     :,       ,i:,,,K##EEti::.     ::,,:,,.
                                                                                             .:::,.        .,     ;.:tii;,;;ii;   .:,:::::::::
                                                                                                            :,    ;,E##Kji::
                                                                                                             .,   ;;E##Kt; 
                                                                                                               ,,. ,,ii;,:
*/
