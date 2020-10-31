#include "GridSplitter.h" 

void GridSplitter::Generate( GridSplits preset, const char *prefix ) {
 string pre=strlen(prefix)<=0?GridSplitsName(preset):prefix;
 switch ( preset ) {
  default:
  case gs_Uno:
    sectors.Split(&outer,1,1,pre.c_str());
   break;
  case gs_Bisect:
    sectors.Split(&outer,1,2,pre.c_str());
   break;
  case gs_Trisect:
    sectors.Split(&outer,1,3,pre.c_str());
   break;
  case gs_Quadrisect:
    sectors.Split(&outer,2,2,pre.c_str());
   break;
  case gs_Quinquesect:
   {
    sectors.Split(&outer,2,3,pre.c_str());
    sectors.Select("-2x");
    GridRect *r=sectors.PopFirstSelected(); 
    double width=r->rectangle.dw/2.0;
    sectors.MoveSelectedBy(-width,0.0);
    remainders.Add((pre+("-left")).c_str(),r->rectangle.dx,r->rectangle.dy,width,r->rectangle.dh);
    remainders.Add((pre+("-right")).c_str(),outer.rectangle.dx2-width,r->rectangle.dy,width,r->rectangle.dh);
    delete r;
   }
   break;
  case gs_Sexisect:
    sectors.Split(&outer,2,3,pre.c_str());
   break;
  case gs_Septisect:
   {
    sectors.Split(&outer,2,4,pre.c_str());
    sectors.Select("-2x");
    GridRect *r=sectors.PopFirstSelected(); 
    double width=r->rectangle.dw/2.0;
    sectors.MoveSelectedBy(-width,0.0);
    remainders.Add((pre+("-left")).c_str(),r->rectangle.dx,r->rectangle.dy,width,r->rectangle.dh);
    remainders.Add((pre+("-right")).c_str(),outer.rectangle.dx2-width,r->rectangle.dy,width,r->rectangle.dh);
    delete r;
   }
   break;
  case gs_Octisect:
    sectors.Split(&outer,2,4,pre.c_str());
   break;
  case gs_Nonasect:
    sectors.Split(&outer,3,3,pre.c_str());
   break;
  case gs_Decisect:
    sectors.Split(&outer,2,5,pre.c_str());
   break;
  case gs_Duolayer:
    sectors.Split(&outer,2,1,pre.c_str());
   break;
  case gs_Trilayer:
    sectors.Split(&outer,3,1,pre.c_str());
   break;
  case gs_Quadrilayer:
    sectors.Split(&outer,4,1,pre.c_str());
   break;
  case gs_Quintilayer:
    sectors.Split(&outer,5,1,pre.c_str());
   break;
  case gs_Sexilayer:
    sectors.Split(&outer,6,1,pre.c_str());
   break;
  case gs_Septilayer:
    sectors.Split(&outer,7,1,pre.c_str());
   break;
  case gs_Octilayer:
    sectors.Split(&outer,8,1,pre.c_str());
   break;
  case gs_Nonalayer:
    sectors.Split(&outer,9,1,pre.c_str());
   break;
  case gs_Decilayer:
    sectors.Split(&outer,10,1,pre.c_str());
   break;
  case gs_Trisect2:
   {
    sectors.Split(&outer,2,2,pre.c_str());
    sectors.Select("-2x");
    GridRect *r=sectors.PopFirstSelected(); 
    double width=r->rectangle.dw/2.0;
    sectors.MoveSelectedBy(-width,0.0);
    remainders.Add((pre+("-left")).c_str(),r->rectangle.dx,r->rectangle.dy,width,r->rectangle.dh);
    remainders.Add((pre+("-right")).c_str(),outer.rectangle.dx2-width,r->rectangle.dy,width,r->rectangle.dh);
    delete r;
   }
   break;
  case gs_BisectQuadrisect:
   {
    sectors.Split(&outer,1,3);
    sectors.Select();
    GridRect *r=((GridRect*)sectors.first);
    GridRect *r2=(GridRect*)r->next;
    GridRect *r3=(GridRect*)r2->next;
    sectors.Deselect(r);
    sectors.Split(r,2,1);
    GridRect *m=sectors.MergeSelected();
    sectors.Split(m,2,2);
   }
   break;
  case gs_Col4Row3:
    sectors.Split(&outer,4,3,pre.c_str());
   break;
  case gs_BisectGrid6:
   {
    sectors.Split(&outer,1,3);
    sectors.Select();
    GridRect *r=((GridRect*)sectors.first);
    GridRect *r2=(GridRect*)r->next;
    GridRect *r3=(GridRect*)r2->next;
    sectors.Deselect(r);
    sectors.Split(r,2,1);
    GridRect *m=sectors.MergeSelected();
    sectors.Split(m,3,2);
   }
   break;
  case gs_QuadrisectL9:
   {
    double smidge=outer.rectangle.dw/7.0;
    GridRect *q=sectors.Add((pre+("-quad")).c_str(),
     outer.rectangle.dx+smidge,outer.rectangle.dy,outer.rectangle.dw-smidge,outer.rectangle.dh-smidge);
    GridRect *l=sectors.Add((pre+("-left")).c_str(),
     outer.rectangle.dx,outer.rectangle.dy,smidge,outer.rectangle.dh);
    GridRect *b=sectors.Add((pre+("-five")).c_str(),
     q->dx(),q->dy2(),q->dw(),smidge);
    sectors.Split(q,2,2);
    sectors.Split(l,4,1);
    sectors.Split(b,1,5);
   }
   break;
  case gs_NonaSeptisectTop:
   {
    double smidge=outer.rectangle.dh/4.0;
    GridRect *n=sectors.Add((pre+("-nona")).c_str(),
     outer.dx(),outer.dy()+smidge,outer.dw(),outer.dh()-smidge);
    GridRect *t=sectors.Add((pre+("-septi")).c_str(),
     outer.dx(),outer.dy(),outer.dw(),smidge);
    sectors.Split(n,3,3);
    sectors.Split(t,1,7);
   }
   break;
  case gs_3377:
   {
    sectors.Split(&outer,4,1);
    sectors.Select();
    GridRect *r1=(GridRect *)sectors.first;
    GridRect *r2=(GridRect *)r1->next;
    GridRect *r3=(GridRect *)r2->next;
    GridRect *r4=(GridRect *)r3->next;
    sectors.Split(r1,&sectors,1,7,"topstrip");
    sectors.Split(r2,&sectors,1,3,"top3");
    sectors.Split(r3,&sectors,1,3,"bottom3");
    sectors.Split(r4,&sectors,1,7,"bottomstrip");
   }
   break;
  case gs_RightBisect:
   {
    double smidge=outer.rectangle.dw/3.0;
    double smodge=outer.H()/2.0;
    sectors.Add(pre.c_str(),"left",outer.X(),outer.Y(),outer.W()-smidge,outer.H());
    sectors.Add(pre.c_str(),"right-top",outer.X()+(outer.W()-smidge),outer.Y(),smidge,smodge);
    sectors.Add(pre.c_str(),"right-bottom",outer.X()+(outer.W()-smidge),outer.Y()+smodge,smidge,smodge);
   }
   break;
  case gs_366:
   {
    sectors.Split(&outer,2,1);
    GridRect *top=(GridRect *)sectors.first;
    GridRect *bottom=(GridRect *)sectors.last;
    sectors.Split(top,&sectors,1,3,"top3");
    sectors.Split(bottom,&sectors,2,7,"icons");
   }
   break;
  case gs_U13Quadrisect:
   {
    double smidge=outer.W()/7.0;
    double smidge2=(outer.W()-smidge)/2.0;
    double smodge4=outer.H()/4.0;
    for ( int i=0; i<3; i++ )
     sectors.Add(pre.c_str(),Prefix(pre,FORMAT("left-%d",i)).c_str(),outer.X(),outer.Y()+(smodge4*(double)i),smidge,smodge4);
    for ( int i=0; i<3; i++ )
     sectors.Add(pre.c_str(),Prefix(pre,FORMAT("right-%d",i)).c_str(),outer.X()+outer.W()-smidge,outer.Y()+(smodge4*(double)i),smidge,smodge4);
    double smodge2=(outer.H()-smodge4)/2.0;
    sectors.Add(Prefix(pre,"top-left").c_str(),outer.X()+smidge,outer.Y(),smidge2,smodge2);
    sectors.Add(Prefix(pre,"top-right").c_str(),outer.X()+smidge+smodge2,outer.Y(),smidge2,smodge2);
    sectors.Add(Prefix(pre,"bottom-left").c_str(),outer.X()+smidge,outer.Y()+smodge2,smidge2,smodge2);
    sectors.Add(Prefix(pre,"bottom-right").c_str(),outer.X()+smidge+smodge2,outer.Y()+smodge2,smidge2,smodge2);
    for ( int j=0; j<7; j++ )
     sectors.Add(FORMAT("%s-%d",Prefix(pre,"bottom").c_str(),j).c_str(),outer.X()+smidge*(double)j,outer.Y()+smodge2*2,smidge,smodge4);
   }
   break;
  case gs_U8Uno:
   {
    double smidge=outer.W()/4.0;
    double smodge=outer.H()/3.0;
    sectors.Add(Prefix(pre,"center").c_str(),outer.X()+smidge,outer.Y(),smidge*2.0,smodge*2.0);
    sectors.Column(Prefix(pre,"left").c_str(),2,outer.X(),outer.Y(),smidge,smodge);
    sectors.Column(Prefix(pre,"right").c_str(),2,outer.X()+outer.H()-smidge,outer.Y(),smidge,smodge);
    sectors.Row(Prefix(pre,"bottom").c_str(),4,outer.X(),outer.Y()+smodge*2,smidge,smodge);
   }
   break;
  case gs_RightQuadrisect:
   {
    sectors.Split(&outer,1,2,"left");
    GridRect *R=(GridRect *)sectors.last;
    sectors.Split(R,2,2,Prefix(pre,"right").c_str());
   }
   break;
  case gs_Col8Row3QuadrisectTop:
   {
    Integers n;
    n.Add(4); n.Add(8); n.Add(8); n.Add(8);
    sectors.Rows(Prefix(pre,"4888").c_str(),&n,outer.X(),outer.Y(),outer.W(),outer.H());
   }
   break;
  case gs_U13LeftBisect:
   {
   }
   break;
  case gs_Col4QuadCol4:
   {
    double smidge=outer.W()/6.0;
    double smodge=outer.H()/4.0;
    sectors.Column(Prefix(pre,"left").c_str(),4,outer.X(),outer.Y(),smidge,smodge);
    sectors.Column(Prefix(pre,"right").c_str(),4,outer.X()+outer.W()-smidge,outer.Y(),smidge,smodge);
    GridRect r("quad",outer.X()+smidge,outer.Y(),outer.W()-smidge*2.0,outer.H());
    sectors.Split(&r,2,2,Prefix(pre,"quad").c_str());
   }
   break;
  case gs_Col4Quadrisect:
   {
    double smidge=outer.W()/3.0;
    double smodge=outer.H()/4.0;
    sectors.Column(Prefix(pre,"left").c_str(),4,outer.X(),outer.Y(),smidge,smodge);
    GridRect r("quad",outer.X()+smidge,outer.Y(),outer.W()-smidge,outer.H());
    sectors.Split(&r,2,2,Prefix(pre,"quad").c_str());
   }
   break;
  case gs_8448:
   {
    Integers n;
    n.Add(8); n.Add(4); n.Add(4); n.Add(8);
    sectors.Rows(Prefix(pre,"8448").c_str(),&n,outer.X(),outer.Y(),outer.W(),outer.H());
   }
   break;
  case gs_U13Bisect:
   {
   }
   break;
  case gs_LeftBisect:
   {
    double smidge=outer.rectangle.dw/3.0;
    double smodge=outer.H()/2.0;
    sectors.Add(Prefix(pre,"left-top").c_str(),outer.X(),outer.Y(),smidge,smodge);
    sectors.Add(Prefix(pre,"left-bottom").c_str(),outer.X(),outer.Y()+smodge,smidge,smodge);
    sectors.Add(Prefix(pre,"right").c_str(),outer.X()+smidge,outer.Y(),outer.W()-smidge,outer.H());
   }
   break;
  case gs_Col4UnoCol4:
   {
    double smidge=outer.W()/6.0;
    double smodge=outer.H()/4.0;
    sectors.Column(Prefix(pre,"left").c_str(),4,outer.X(),outer.Y(),smidge,smodge);
    sectors.Column(Prefix(pre,"right").c_str(),4,outer.X()+outer.W()-smidge,outer.Y(),smidge,smodge);
    sectors.Add(Prefix(pre,"center").c_str(),outer.X()+smidge,outer.Y(),outer.W()-smidge*2.0,outer.H());
   }
   break;
  case gs_LeftQuadrisect:
   {
    sectors.Split(&outer,1,2,"right");
    GridRect *L=(GridRect *)sectors.first;
    sectors.Split(L,2,2,Prefix(pre,"left").c_str());
   }
   break;
  case gs_848:
   {
    sectors.RowSplit(Prefix(pre,"top").c_str(),8,outer.X(),outer.Y(),outer.W()/4.0,outer.H()/4.0);
    sectors.RowSplit(Prefix(pre,"center").c_str(),4,outer.X(),outer.Y()+outer.W()/4.0,outer.W(),outer.H()-outer.H()/2.0);
    sectors.RowSplit(Prefix(pre,"bottom").c_str(),8,outer.X(),outer.Y()+outer.H()-outer.H()/4.0,outer.W()/4.0,outer.H()/4.0);
   }
   break;
  case gs_Col4Col4BisectCol4Col4:
   {
   }
   break;
  case gs_Col3Col3BisectCol3Col3Row7:
   {
   }
   break;
  case gs_727:
   {
   }
   break;
  case gs_7337:
   {
    Integers n;
    n.Add(7); n.Add(3); n.Add(3); n.Add(7);
    sectors.Rows(Prefix(pre,"7337").c_str(),&n,outer.X(),outer.Y(),outer.W(),outer.H());
   }
   break;
  case gs_488:
   {
    sectors.Split(&outer,2,1);
    GridRect *T=(GridRect *)sectors.first;
    GridRect *B=(GridRect *)sectors.last;
    sectors.Split(T,1,4,Prefix(pre,"top").c_str());
    sectors.Split(B,2,8,Prefix(pre,"bottom").c_str());
   }
   break;
  case gs_Col4Col4Quadrisect:
   {
   }
   break;
  case gs_616:
   {
   }
   break;
  case gs_636:
   {
   }
   break;
 }
}

string GridSplitsName( GridSplits in ) {
 switch ( in ) {
  case gs_Uno:                         return string("Uno");
  case gs_Bisect:                      return string("Bisect");
  case gs_Trisect:                     return string("Trisect");
  case gs_Quadrisect:                  return string("Quadrisect");
  case gs_Quinquesect:                 return string("Quinquesect");
  case gs_Sexisect:                    return string("Sexisect");
  case gs_Septisect:                   return string("Septisect");
  case gs_Octisect:                    return string("Octisect");
  case gs_Nonasect:                    return string("Nonasect");
  case gs_Decisect:                    return string("Decisect");
  case gs_Duolayer:                    return string("Duolayer");
  case gs_Trilayer:                    return string("Trilayer");
  case gs_Quadrilayer:                 return string("Quadrilayer");
  case gs_Quintilayer:                 return string("Quintilayer");
  case gs_Sexilayer:                   return string("Sexilayer");
  case gs_Septilayer:                  return string("Septilayer");
  case gs_Octilayer:                   return string("Octilayer");
  case gs_Nonalayer:                   return string("Nonalayer");
  case gs_Decilayer:                   return string("Decilayer");
  case gs_Trisect2:                    return string("Trisect2");
  case gs_BisectQuadrisect:            return string("BisectQuadrisect");
  case gs_Col4Row3:                    return string("Col4Row3");
  case gs_BisectGrid6:                 return string("BisectGrid8");
  case gs_QuadrisectL9:                return string("QuadrisectL9");
  case gs_NonaSeptisectTop:            return string("NonaSeptisectTop");
  case gs_3377:                        return string("3377");
  case gs_RightBisect:                 return string("RightBisect");
  case gs_366:                         return string("366");
  case gs_U13Quadrisect:               return string("U13Quadrisect");
  case gs_U8Uno:                       return string("U8Uno");
  case gs_RightQuadrisect:             return string("RightQuadrisect");
  case gs_Col8Row3QuadrisectTop:       return string("Col8Row3QuadrisectTop");
  case gs_U13LeftBisect:               return string("U13LeftBisect");
  case gs_Col4QuadCol4:                return string("Col4QuadCol4");
  case gs_Col4Quadrisect:              return string("Col4Quadrisect");
  case gs_8448:                        return string("8448");
  case gs_U13Bisect:                   return string("U13Bisect");
  case gs_LeftBisect:                  return string("LeftBisect");
  case gs_Col4UnoCol4:                 return string("Col4UnoCol4");
  case gs_LeftQuadrisect:              return string("LeftQuadrisect");
  case gs_848:                         return string("848");
  case gs_Col4Col4BisectCol4Col4:      return string("Col4Col4BisectCol4Col4");
  case gs_Col3Col3BisectCol3Col3Row7:  return string("Col3Col3BisectCol3Col3Row7");
  case gs_727:                         return string("727");
  case gs_7337:                        return string("7337");
  case gs_488:                         return string("488");
  case gs_Col4Col4Quadrisect:          return string("Col4Col4Quadrisect");
  case gs_616:                         return string("616");
  case gs_636:                         return string("636");
  case gs_Unknown:                     return string("Undefined");
  default: return string("Unknown");
 }
}

/*
 * In case one would ever need it:

  switch ( preset ) {
  case gs_Unknown:
  case gs_Uno:
  case gs_Bisect:
  case gs_Trisect:
  case gs_Quadrisect:
  case gs_Quinquesect:
  case gs_Sexisect:
  case gs_Septisect:
  case gs_Octisect:
  case gs_Nonasect:
  case gs_Decisect:
  case gs_Duolayer:
  case gs_Trilayer:
  case gs_Quadrilayer:
  case gs_Quintilayer:
  case gs_Sexilayer:
  case gs_Septilayer:
  case gs_Octilayer:
  case gs_Nonalayer:
  case gs_Decilayer:
  case gs_Trisect2:
  case gs_BisectQuadrisect:
  case gs_Col4Row3:
  case gs_BisectGrid6:
  case gs_QuadrisectL9:
  case gs_NonaSeptisectTop:
  case gs_3377:
  case gs_RightBisect:
  case gs_366:
  case gs_U13Quadrisect:
  case gs_U8Uno:
  case gs_RightQuadrisect:
  case gs_Col8Row3QuadrisectTop:
  case gs_U13LeftBisect:
  case gs_Col4QuadCol4:
  case gs_Col4Quadrisect:
  case gs_8448:
  case gs_U13Bisect:
  case gs_LeftBisect:
  case gs_Col4UnoCol4:
  case gs_LeftQuadrisect:
  case gs_848:
  case gs_Col4Col4BisectCol4Col4:
  case gs_Col3Col3BisectCol3Col3Row7:
  case gs_727:
  case gs_7337:
  case gs_488:
  case gs_Col4Col4Quadrisect:
  case gs_616:
  case gs_636:
 }

 */