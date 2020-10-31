#include "ParallaxTest.h"

#if defined(NEVER)
void ParallaxTestInit( GLWindowManager *windows ) {
 // Parallax Presentation test:
 
 ParallaxTest *parallax=new ParallaxTest;
 ParallaxTestLayer *pt;
 parallax->parallax.SetBackground(new ParallaxTestBackgroundLayer);
 windows->Add(parallax->parallax.background);
 parallax->parallax.Add(pt=new ParallaxTestLayer,1.0f);
 windows->Add(pt);
 parallax->parallax.Add(pt=new ParallaxTestLayer,2.0f);
 windows->Add(pt);
 parallax->parallax.Add(pt=new ParallaxTestLayer,4.0f);
 windows->Add(pt);
 parallax->parallax.Add(pt=new ParallaxTestLayer,8.0f);
 windows->Add(pt);
 parallax->parallax.Add(pt=new ParallaxTestLayer,16.0f);
 windows->Add(pt);

 windows->Add(windows->send=parallax);
}
#endif