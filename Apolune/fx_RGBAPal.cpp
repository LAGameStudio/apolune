#include "fx_RGBAPal.h"

#include "FastGUI.h"

void fx_UglyRGBPal::ShowHelpInfo() {
 fast.boxedtext(
  ( hoverR || hoverG || hoverB ) ? 
      "Click/Hold Left = Tune +\n"
      "Click Middle = Full\n"
      "Wheel Up/Down = Fine Tune +/-\n"
      "Click/Hold Right = Tune -\n"
    : "Click Left = Randomize\n"
      "Click Middle = White\n"
      "Wheel Up/Down = Bright/Dark\n"
      "Click Right = Black",
  input.mxi-256, input.myi,false,
  crayons.Pick(alabaster),crayons.Pick(gray),crayons.Pick(rgb0007),4,true
 );
}