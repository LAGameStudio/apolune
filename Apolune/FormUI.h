#pragma once
#include "RectPlacer.h"
#include "FastGUI.h"
#include "EditGUI.h"
#include "FileDialog.h"
#include "Film.h"

class FormEditorWidgetFontProfile {
public:
 Zp<TexFont> font;
 Dim2i tfont;
 Zint lineSpace;
 Zint charSpace;
 Dim2d fontSize;
 Blends blend;
 FormEditorWidgetFontProfile() {
  blend=transparency;
  tfont.w=fast.tfont.w;
  tfont.h=fast.tfont.h;
  lineSpace=fast.lineSpace;
  charSpace=fast.charSpace;
  fontSize.Set(fast.fontSize.x,fast.fontSize.y);
  font=termFont;
 }
};

class FormEditorWidget : public Proce55or {
public:
 FastExtents e;
 Zp<FormEditorWidgetFontProfile> font;
 Zstring label;
 FormEditorWidget() : Proce55or() {} // To allow few_Reserved to build from C_ONE
 FormEditorWidget( FormEditorWidgetFontProfile *font ) : Proce55or() { this->font=font; }
 virtual int GetWidth() { return 0; }
 virtual int GetHeight() { return 0; }
};

class few_Blend : public FormEditorWidget {
public:
 Zp<Blends> blend;
 few_Blend( FormEditorWidgetFontProfile *f, Blends *b ) : FormEditorWidget(f) {
  blend=b;
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 int GetWidth() {
  int longest=strlen("transparency");
  return 32+longest*(int)fast.fontSize.x;
 }
 int GetHeight() {
  return UMAX(16,(int)fast.fontSize.y);
 }
 void OnSetExtents() {
  e.x=x;
  e.y=y;
  e.w=32;
  e.h=16;
 }
 void Render() {
  (*blend)=fast.blend(label.c_str(),(*blend),e.x,e.y,e.w,e.h,10);
 }
};

class few_Enumerated : public FormEditorWidget {
public:
 Zp<Strings> enumerations;
 Zp<int> value;
 Zint characters;
 few_Enumerated( FormEditorWidgetFontProfile *f, int *value, Strings *enumerations ) : FormEditorWidget(f) {
  this->value=value;
  this->enumerations=enumerations;
  characters=5;
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 int GetWidth() {
  int longest=enumerations?enumerations->longest():0;
  return 32+longest*(int)fast.fontSize.x;
 }
 int GetHeight() {
  return UMAX(16,(int)fast.fontSize.y);
 }
 void OnSetExtents() {
  e.x=x;
  e.y=y;
  e.w=32;
  e.h=16;
 }
 void Render() {
  fast.enumerated(enumerations,label.c_str(),(*value),e,characters);
 }
};

class few_EnumeratedList : public FormEditorWidget {
public:
 Zp<Strings> enumerations;
 Zp<int> value;
 Zbool showRectangle;
 Zint rows,cols;
 Zint scroll;
 Zint clicked;
 few_EnumeratedList( FormEditorWidgetFontProfile *f, int *value, Strings *enumerations, int rows=4, int cols=20 ) : FormEditorWidget(f) {
  this->value=value;
  this->enumerations=enumerations;
  this->rows=rows;
  this->cols=cols;
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 int GetWidth() {
  return cols*fast.tfont.w+32;
 }
 int GetHeight() {
  return (rows+1)*fast.tfont.h+15;
 }
 void OnSetExtents() {
  e.x=x;
  e.y=y;
  e.w=w-32;
  e.h=h;
 }
 void Render() {
  clicked=fast.list(font->font,label.c_str(),enumerations.pointer,e.x,e.y,e.w,rows,value,&scroll.value,font->lineSpace,font->charSpace);
//  if ( clicked == (*value) ) (*value)=-1;
//  if ( clicked >= 0 ) (*value)=clicked;
 }
};

class few_ToggleList : public FormEditorWidget {
public:
 Zp<Strings> toggles;
 Zint rows,cols;
 Zint clicked;
 Zint scroll;
 few_ToggleList( FormEditorWidgetFontProfile *f, Strings *toggleable, int rows=4, int cols=20 ) : FormEditorWidget(f) {
  toggles=toggleable;
  this->rows=rows;
  this->cols=cols;
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 int GetWidth() {
  return cols*fast.tfont.w+32; //32 width of scroll
 }
 int GetHeight() {
  return (rows+1)*fast.tfont.h+15;
 }
 void OnSetExtents() {
  e.x=x;
  e.y=y;
  e.w=w-32;
  e.h=h;
 }
 void Render() {
  clicked=fast.togglelist(font->font,label.c_str(),toggles,e.x,e.y,e.w,rows,&scroll.value);
 }
};

class few_IntegerTuner : public FormEditorWidget {
public:
 Zp<int> value;
 Zint low,high,delta,deltaAlt,deltaCtrl;
 Zint characters;
 Cartesian offset;
 few_IntegerTuner( FormEditorWidgetFontProfile *f, int *v, int low=0, int high=1000000, int delta=1, int deltaAlt=10, int deltaCtrl=100 ) : FormEditorWidget(f) {
  value=v;
  characters=7;
  this->low=low;
  this->high=high;
  this->delta=delta;
  this->deltaAlt=deltaAlt;
  this->deltaCtrl=deltaCtrl;
  SetExtents(0,0,GetWidth(),GetHeight());
  offset.y=32;
 }
 int GetWidth() {
  return UMAX((int)label.length,characters)*(int)fast.fontSize.y+32;
 }
 int GetHeight() {
  return 32+UMAX(16,(int)fast.fontSize.y);
 }
 void OnSetExtents() {
  e.x=x+offset.x;
  e.y=y+offset.y;
  e.w=32;
  e.h=16;
 }
 void Render() {
  int factor=(input.KeyDown(DX_LALT) || input.KeyDown(DX_RALT)) ? deltaAlt : (input.KeyDown(DX_LCTRL) || input.KeyDown(DX_RCTRL)) ? deltaCtrl : delta;
  (*value)=fast.integer(label.c_str(),(*value),low,high,factor,e,characters);
 }
};


class few_DecimalTuner : public FormEditorWidget {
public:
 Zp<double> value;
 Zdouble low,high;
 Zdouble delta,deltaAlt,deltaCtrl,deltaShift;
 Zint characters;
 Cartesian offset;
 few_DecimalTuner( FormEditorWidgetFontProfile *f, double *v ) : FormEditorWidget(f) {
  value=v;
  this->low=0.0;
  this->high=1000000.0;
  characters=7;
  this->delta=0.1;
  this->deltaAlt=1.0;
  this->deltaCtrl=0.1;
  this->deltaShift=10.0;
  SetExtents(0,0,GetWidth(),GetHeight());
  offset.y=32;
 }
 few_DecimalTuner( FormEditorWidgetFontProfile *f, double *v, double low, double high, double delta=1.0, double deltaAlt=2.0, double deltaCtrl=10.0 ) : FormEditorWidget(f) {
  value=v;
  this->low=low;
  this->high=high;
  characters=7;
  this->delta=delta;
  this->deltaAlt=1.0;
  this->deltaCtrl=0.1;
  this->deltaShift=10.0;
  SetExtents(0,0,GetWidth(),GetHeight());
  offset.y=32;
 }
 int GetWidth() {
  return UMAX((int)label.length,characters)*(int)fast.fontSize.y+32;
 }
 int GetHeight() {
  return 32+UMAX(16,(int)fast.fontSize.y);
 }
 void OnSetExtents() {
  e.x=x+offset.x;
  e.y=y+offset.y;
  e.w=32;
  e.h=16;
 }
 void Render() {
  double factor=delta;
  if ( input.KeyDown(DX_LALT) || input.KeyDown(DX_RALT) ) {
   factor=deltaAlt;
   if ( input.KeyDown(DX_LSHIFT) || input.KeyDown(DX_RSHIFT) ) {
    factor*=deltaShift;
   } else if ( input.KeyDown(DX_LCTRL) || input.KeyDown(DX_RCTRL) ) {
    factor*=deltaCtrl;
   }
  } else if ( input.KeyDown(DX_LSHIFT) || input.KeyDown(DX_RSHIFT) ) {
   factor=deltaShift;
  } else if ( input.KeyDown(DX_LCTRL) || input.KeyDown(DX_RCTRL) ) {
   factor=deltaCtrl;
  }
  (*value)=fast.decimal(label.c_str(),(*value),low,high,factor,e,characters);
 }
};

class few_HSlider : public FormEditorWidget {
public:
 Zbool simple;
 Zdouble low,high;
 Zp<double> value;
 Zint thickness;
 Cartesian offset;
 few_HSlider( FormEditorWidgetFontProfile *f, double *v, double low, double high, bool simple=false ) : FormEditorWidget(f) {
  this->value=v;
  this->low=low;
  this->high=high;
  thickness=16;
  this->simple=simple;
  SetExtents(0,0,GetWidth(),GetHeight());
  offset.y=16+8+(int)fast.fontSize.y;
 }
 int GetWidth() {
  if ( simple ) {
   return 256;
  } else {
   return 256;
  }
 }
 int GetHeight() {
  if ( simple ) {
   return 32+8+(int)fast.fontSize.y;
  } else {
   return 32+8+(int)fast.fontSize.y;
  }
 }
 void OnSetExtents() {
  e.x=simple?x+8+(int)fast.fontSize.y:x+32 + offset.x;
  e.y=y+8+(int)fast.fontSize.y + offset.y;
  e.w=256-(simple?32:64);
  e.h=32;
 }
 void Render() {
  (*value)=fast.hslider(label.c_str(),(*value),low,high,e.x,e.y,e.w,thickness,simple);
 }
};

class few_VSlider : public FormEditorWidget {
public:
 Zbool simple;
 Zdouble low,high;
 Zp<double> value;
 Zint thickness;
 Zint height;
 few_VSlider( FormEditorWidgetFontProfile *f, double *v, double low, double high, bool simple=false ) : FormEditorWidget(f) {
  this->value=v;
  this->low=low;
  this->high=high;
  thickness=16;
  this->simple=simple;
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 int GetWidth() {
  return 32;
 }
 int GetHeight() {
  if ( height > 0 ) {
   if ( simple ) {
    return height;
   } else return height+4+(int)fast.fontSize.y;
  } else return 256;
 }
 void OnSetExtents() {
  e.x=x;
  e.y=y;
  e.w=32;
  e.h=simple?h:h-4+(int)fast.fontSize.y;
 }
 void Render() {
  (*value)=fast.vslider(label.c_str(),(*value),low,high,e.x,e.y,thickness,e.h,simple,simple);
 }
};

class few_HRange : public FormEditorWidget {
public:
 Zbool simple;
 Zp<double> A,B;
 Zdouble low,high;
 Zint thickness;
 few_HRange( FormEditorWidgetFontProfile *f, double *a, double *b, double low, double high ) : FormEditorWidget(f) {
  this->A=a;
  this->B=b;
  this->low=low;
  this->high=high;
  thickness=16;
  simple=true;
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 int GetWidth() {
  if ( simple ) {
   return 256;
  } else {
   return 256+64;
  }
 }
 int GetHeight() {
  if ( simple ) {
   return 8+(int)fast.fontSize.y;
  } else {
   return 8+(int)fast.fontSize.y;
  }
 }
 void OnSetExtents() {
  e.x=simple?x:x+32;
  e.y=y+8+(int)fast.fontSize.y;
  e.w=256+(simple?32:64);
  e.h=32;
 }
 void Render() {
  fast.hrange(label.c_str(),A,B,low,high,e.x,e.y,e.w,thickness,simple);
 }
};

class few_ImagePicker : public FormEditorWidget {
public:
 FileDialog fd;
 Zstring filename;
 few_ImagePicker( FormEditorWidgetFontProfile *f, GLImage **i, const char *path, const char *description ) : FormEditorWidget(f) {
  fd.bound=&filename.value;
  fd.path=path;
  fd.description=description;
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 int GetWidth() {
  return 128;
 }
 int GetHeight() {
  return 128+32;
 }
 void OnSetExtents() {
  e.x=x;
  e.y=y;
  e.w=w;
  e.h=h-32;
 }
 void Render() {
 }
};

class few_Textbox : public FormEditorWidget {
public:
 Zp<std::string> bound;
 Zint rows,cols,border;
 Blends fontBlend;
 Zbool limited,classic,alwaysKeys;
 Zp<TexFont> innerFont;
 Cartesian offset;
 few_Textbox( FormEditorWidgetFontProfile *f, std::string *t, int cols=20, int rows=1 ) : FormEditorWidget(f) {
  bound=t;
  this->cols=cols;
  this->rows=rows;
  limited=true;
  offset.Set(16,border+fast.tfont.h+24+32);
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 int GetWidth() {
  return cols*fast.tfont.w+32+border*2;
 }
 int GetHeight() {
  return (rows+2)*(font->lineSpace+fast.tfont.h)+32+border*2;
 }
 void OnSetExtents() {
  e.x=x+offset.x;
  e.y=y+offset.y;
  e.w=w-offset.x;
  e.h=h-offset.y;
 }
 void Render() {
  if ( bound ) {
   if ( classic ) {
    (*bound)=fast.textbox(label.c_str(),(*bound),e,cols,limited,rows,font->lineSpace,border);
   } else {
    (*bound)=fast.textbox(
     (innerFont?innerFont:font->font),
     (innerFont?fontBlend:font->blend),
     font->font,font->blend,label.c_str(),
     (*bound),e.x,e.y,
     cols,limited,rows,
     font->lineSpace,border,alwaysKeys,font->charSpace
    );
   }
  }
 }
};

class few_Movement : public FormEditorWidget {
public:
 Zp<Cartesian> c;
 Zp<Cartesiand> d;
 Zdouble delta,deltaAlt,deltaCtrl;
 Zbool disabled;
 few_Movement( FormEditorWidgetFontProfile *f, Cartesian *c, double delta=1.0, double deltaAlt=2.0, double deltaCtrl=10.0 ) : FormEditorWidget(f) {
  this->c=c;
  this->delta=delta;
  this->deltaAlt=deltaAlt;
  this->deltaCtrl=deltaCtrl;
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 few_Movement( FormEditorWidgetFontProfile *f, Cartesiand *d, double delta=1.0, double deltaAlt=2.0, double deltaCtrl=10.0 ) : FormEditorWidget(f) {
  this->d=d;
  this->delta=delta;
  this->deltaAlt=deltaAlt;
  this->deltaCtrl=deltaCtrl;
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 int GetWidth() {
  return 128;
 }
 int GetHeight() {
  return 128;
 }
 void OnSetExtents() {
  e.x=x+64;
  e.y=y+64;
 }
 void Render() {
  double factor=(input.KeyDown(DX_LALT) || input.KeyDown(DX_RALT)) ? deltaAlt : (input.KeyDown(DX_LCTRL) || input.KeyDown(DX_RCTRL)) ? deltaCtrl : delta;
  if ( c ) {
   int arrows=fast.arrows(label.c_str(),e.x,e.y,16,disabled);
   if ( arrows >= 0 ) {
    switch ( arrows ) {
      case DX_LEFT: c->MoveBy((int)-factor,0); break;
     case DX_RIGHT: c->MoveBy((int)factor,0); break;
        case DX_UP: c->MoveBy(0,(int)-factor); break;
      case DX_DOWN: c->MoveBy(0,(int)factor); break;
    }
   }
  } else if ( d ) {
   int arrows=fast.arrows(label.c_str(),e.x,e.y,16,disabled);
   if ( arrows >= 0 ) {
    switch ( arrows ) {
      case DX_LEFT: d->MoveBy(-factor,0.0); break;
     case DX_RIGHT: d->MoveBy(factor,0.0); break;
        case DX_UP: d->MoveBy(0.0,-factor); break;
      case DX_DOWN: d->MoveBy(0.0,factor); break;
    }
   }
  }
 }
};

class few_XYPad : public FormEditorWidget {
public:
 Zp<Cartesian> c;
 Zp<Cartesiand> d;
 few_XYPad( FormEditorWidgetFontProfile *f, Cartesian *c ) : FormEditorWidget(f) {
  this->c=c;
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 few_XYPad( FormEditorWidgetFontProfile *f, Cartesiand *d ) : FormEditorWidget(f) {
  this->d=d;
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 int GetWidth() {
  return 128;
 }
 int GetHeight() {
  return 128;
 }
 void OnSetExtents() {
  e.x=x;
  e.y=y;
  e.w=w;
  e.h=h;
 }
 void Render() {
  double xx=0.0,yy=0.0;
  if ( c ) {
   xx=(double)c->x;
   yy=(double)c->y;
  } else if ( d ) {
   xx=d->x;
   yy=d->y;
  }
  Rectangle(crayons.Pick(green255),(int)x,(int)y,(int)x+(int)w,(int)y+(int)h);
  fast.xyAngle(&xx,&yy,e.x,e.y,e.w/2,e.h/2);
  if ( c ) {
   c->Set((float)xx,(float)yy);
  } else if ( d ) {
   d->Set(xx,yy);
  }
 }
};

class few_Angle : public FormEditorWidget {
public:
 Zp<double> d;
 Zbool radians;
 Zint precision;
 few_Angle( FormEditorWidgetFontProfile *f, double *d, bool radians=false ) : FormEditorWidget(f) {
  this->d=d;
  this->radians=radians;
  precision=16;
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 int GetWidth() {
  return 128;
 }
 int GetHeight() {
  return 128;
 }
 void OnSetExtents() {
  e.x=x+64;
  e.y=y+64;
 }
 void Render() {
  if ( d ) {
   double ang=(*d);
   if ( radians ) ang=rad2deg(ang);
   ang=fast.angleDegrees(ang,e.x,e.y,precision);
   if ( radians ) (*d)=deg2rad(ang);
   else (*d)=ang;
  }
 }
};

class few_ColorPick : public FormEditorWidget {
public:
 Zp<Crayon> bound;
 Zbool simple,alphaSlider;
 Cartesian inset;
 Cartesian offset;
 few_ColorPick( FormEditorWidgetFontProfile *f, Crayon *c, bool fansi_or_rgb_if=false, bool alpha=false ) : FormEditorWidget(f) {
  simple=!fansi_or_rgb_if;
  alphaSlider=alpha;
  bound=c;
  inset.Set((int)fast.fontSize.x*6,0);
  offset.Set( simple ? 8 : 8, simple ? 42 : 32 );
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 int GetWidth() {
  if ( simple ) {
   return 96;
  } else {
   return 128;
  }
 }
 int GetHeight() {
  if ( simple ) {
   return 48 + (alphaSlider?16:0);
  } else {
   return 128 + (alphaSlider?16:0);
  }
 }
 void OnSetExtents() {
  e.x=x + offset.x;
  e.y=y + offset.y;
  e.w=w;
  e.h=h;
 }
 void Render() {
  if ( bound ) {
   double alphaWas=(*bound).af;
   if ( simple ) {
    (*bound)=fast.colorPickerTiny((*bound),e.x,e.y,e.w,e.h-(alphaSlider?16:0));
   } else {
    int lightColor=crayons.fansi((*bound));
    lightColor=fast.colorPicker(lightColor,e.x,e.y,7);
    (*bound)=crayons.fansi(lightColor);
   }
   if ( alphaSlider ) {
    (*bound).Alpha(alphaWas);
    double alph=(double)((*bound).af);
    alph=fast.hslider("",alph,0.0,1.0,e.x+2,e.y+(e.h-20),e.w-4-16,16,true);
    (*bound).Alpha(alph);
   }
  }
 }
};

class few_Toggle : public FormEditorWidget {
public:
 Zp<bool> toggle;
 Zstring label_on,label_off;
 Zbool switching;
 Cartesian offset;
 few_Toggle( FormEditorWidgetFontProfile *f, bool *toggle, const char *on="", const char *off="" ) : FormEditorWidget(f) {
  this->toggle=toggle;
  label_on=on;
  label_off=off;
  switching=( label_off.length > 0 );
  if ( !switching ) label=on;
  if ( switching ) offset.Set(GetWidth()/2-8,(int)fast.fontSize.y*2);
  SetExtents(0,0,GetWidth(),GetHeight());
  offset.y+=32;
 }
 int GetWidth() {
  if ( !switching ) {
   return 32+16+(label.length*(int)fast.fontSize.x);
  } else {
   return 32+16+(label.length*(int)fast.fontSize.y);
  }
 }
 int GetHeight() {
  return switching ? 32+((int)(fast.fontSize.y+4)*4)
                   : UMAX(16,(int)fast.fontSize.y);
 }
 void OnSetExtents() {
  e.x=x+offset.x;
  e.y=y+offset.y;
  e.w=16;
  e.h=switching?32:16;
 }
 void Render() {
  if ( toggle ) {
   if ( switching ) {
    (*toggle)=fast.toggle(label_on.c_str(),label_off.c_str(),label.c_str(),(*toggle),e.x,e.y,e.w,e.h);
   } else {
    (*toggle)=fast.toggle(label.c_str(),(*toggle),e);
   }
  }
 }
};

class few_Image : public FormEditorWidget {
public:
 Zp<GLImage> i;
 Zp<Crayon> tint;
 Zp<Blends> blend;
 Crayon defaultTint;
 Blends defaultBlend;
 Zint imageWidth,imageHeight;
 few_Image( FormEditorWidgetFontProfile *f, GLImage *i, int iw=128, int ih=128 ) : FormEditorWidget(f) {
  this->i=i;
  tint=&defaultTint;
  defaultTint.x_White();
  blend=&defaultBlend;
  defaultBlend=transparency;
  imageWidth=iw;
  imageHeight=ih;
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 int GetWidth() {
  return imageWidth;
 }
 int GetHeight() {
  return imageHeight;
 }
 void OnSetExtents() {
  e.x=x;
  e.y=y;
  e.w=w;
  e.h=h;
 }
 void Render() {
  fast.image(i,e.x,e.y,e.w,e.h,(*tint),(*blend));
 }
};

class few_Text : public FormEditorWidget {
public:
 Zstring txt;
 Zbool ljust;
 Dim2i padding;
 Crayon tint;
 few_Text( FormEditorWidgetFontProfile *f, const char *text, bool left_justified=false ) : FormEditorWidget(f) {
  txt=text;
  ljust=left_justified;
  tint.x_White();
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 int GetWidth() {
  Strings lines;
  lines.SplitPush(txt.c_str(),'\n');
  return lines.longest() * (font->tfont.w+font->charSpace);
 }
 int GetHeight() {
  return linefeeds(txt.c_str())*(font->tfont.h+font->lineSpace);
 }
 void OnSetExtents() {
  e.x=x;
  e.y=y;
  e.w=w;
  e.h=h+64;
 }
 void Render() {
  fast.text(font->font,label.c_str(),e,font->charSpace,font->lineSpace,tint,font->blend);
 }
};

class few_FilmPicker : public FormEditorWidget {
public:
 Zp<Film *> bound;
 Zstring va,fi;
 Zdouble instant;
 Zint scrolled;
 Zbool showRectangle;
 Cartesian offset;
 few_FilmPicker( FormEditorWidgetFontProfile *f, Film **filmPtr ) : FormEditorWidget(f) {
  bound=filmPtr;
  if (bound && (*bound)) {
   FilmLibrary *lib=(*bound)->GetLibrary();
   if ( lib ) va=lib->title;
   fi=(*bound)->title;
  }
  offset.Set(0,(fast.tfont.h+15));
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 int GetWidth() {
  return 384+offset.x;
 }
 int GetHeight() {
  return 256+offset.y+64;
 }
 void OnSetExtents() {
  e.x=x+offset.x;
  e.y=y+offset.y;
  e.w=w;
  e.h=h-64-offset.y;
 }
 void Render() {
  if ( !bound ) return;
  Film *f=fast.FilmPicker( font->font, va, fi, &instant.value, &scrolled.value, e.x,e.y,e.w,e.h, font->lineSpace, font->charSpace, showRectangle );
  if (f) {
   (*bound)=f;
   FilmLibrary *lib=(*bound)->GetLibrary();
   if ( lib ) va=lib->title;
   fi=(*bound)->title;
  } 
 }
};

class few_Button : public FormEditorWidget {
public:
};

C_ONE(few_Reserved,FormEditorWidget,{})
 Zint id;
 Cartesian area;
 few_Reserved( FormEditorWidgetFontProfile *f, int id, int w, int h ) : FormEditorWidget(f) {
  area.SetRect(0,0,w,h);
  SetExtents(0,0,GetWidth(),GetHeight());
 }
 int GetWidth() {
  return area.w;
 }
 int GetHeight() {
  return area.h;
 }
 void OnSetExtents() {
  e.x=x;
  e.y=y;
  e.w=w;
  e.h=h;
 }
 void Render() {
 }
MANY(few_Reserved,few_ReservedHandle,few_ReservedHandles,"few_Reserved",few_Reserveds,{})
DONE(few_Reserved);

class FormEditorWindow : public MoveableEditorWindow {
public:
 FormEditorWindow() : MoveableEditorWindow() {
  initialSize.Set(128,128);
 }
 FormEditorWindow( MoveableWindowManager *mwm ) : MoveableEditorWindow(mwm) {
  initialSize.Set(128,128);
 }
 Proce55ors widgets;
 Cartesian pen;
 Cartesian initialSize;
 Zbool growHorizOnly,growVertOnly;
 FormEditorWidgetFontProfile font;
 Cartesian lastArranged;
 few_ReservedHandles reserved;
 virtual void WhenSetup() {}
 void OnSetup() {
  clip=false;
  v.minimumSize.Set(initialSize.x,initialSize.y);
  v.maximumSize.Set(initialSize.x,initialSize.y);
  v.noMove=false;
  v.noMinimize=false;
  v.noMaximize=true;
  v.noResize=true;
  v.noTitle=false;
  v.noClose=true;
  v.noClipTitle=true;
  v.background.x_MidnightBlue();
  WhenSetup();
  Arrange();
  v.moving=true; // hack to refresh the arrangement after 1 frame.
 }
 void Arrange( int limitW=32768, int limitH=32768 ) {
  int largestX=0;
  int largestY=0;
  EACH(widgets.first,FormEditorWidget,widg) {
   int wW=widg->GetWidth();  if ( largestX < wW ) largestX=wW;
   int wH=widg->GetHeight(); if ( largestY < wH ) largestY=wH;
   widg->SetExtents(0,0,wW,wH);
  }
  rectPack.inputs.Clear();
  rectPack.Import(&widgets);
  rectPack.packSize.SetRect(0,0,largestX+1,largestY+1);
  rectPack.packSize.y-=((int)v.titlebar.h+4)*4;
  rectPack.packSize.x-=4;
  rectPack.growLimit.Set(limitW,limitH);
  if ( !growVertOnly && !growHorizOnly ) rectPack.growth.Set(8,8);
  else rectPack.growth.Set(growVertOnly?0:8,growHorizOnly?0:8);
  rectPack.Grow();
  rectPack.growLimit.Set(32768,32768);
  if ( v.noResize ) {
   v.minimumSize.Set(rectPack.grownSize.x,rectPack.grownSize.y);
   v.maximumSize.Set(rectPack.grownSize.x,rectPack.grownSize.y);
  }
  v.Resize(rectPack.grownSize.x,rectPack.grownSize.y+64);
  lastArranged.SetRect(x,y,w,h);
  rectPack.Export(x+2,y,&widgets);
  EACH(widgets.first,FormEditorWidget,widg) {
   widg->MoveBy(0,32);
  }
 }
 virtual void RenderBeforeWindow(){}
 virtual void RenderAfterWindow(){}
 void RenderWindow() {
  fast.member=&v;
  if ( (v.wasMoving && !v.moving)
    || (v.wasResizing  && !v.resizing)
    || (v.holder->wasSliding && !v.holder->sliding)
    || (v.holder->wasArranged && !v.holder->arranging) ) Arrange();
  RenderBeforeWindow();
  if ( !v.moving && !v.wasMoving
    && !v.resizing && !v.wasResizing
    && !v.holder->sliding && !v.holder->arranging ) widgets.Render();
  RenderAfterWindow();
#if defined(DEBUG)
  if ( input.KeyDown(DX_GRAVE) ) {
   Blending(additive);
   EACH(widgets.first,Proce55or,widget)
    Rectangle(crayons.Pick(salmonPink),widget->x,widget->y,widget->x2,widget->y2);
   Blending(transparency);
  }
#endif
 }
 few_Blend *Bind( Blends *b ) {
  few_Blend *p=new few_Blend(&font,b);
  widgets.Add(p);
  return p;
 }
 few_EnumeratedList *Bind( int *value, Strings *enumerations, int rows=4, int cols=20 ) {
  few_EnumeratedList *p=new few_EnumeratedList(&font,value,enumerations,rows,cols);
  widgets.Add(p);
  return p;
 }
 few_ToggleList *Bind( Strings *toggles, int lines=4, int cols=20 ) {
  few_ToggleList *p=new few_ToggleList(&font,toggles,lines);
  widgets.Add(p);
  return p;
 }

 few_IntegerTuner *Bind( int *v, int low=0, int high=1000000, int delta=1, int deltaAlt=10, int deltaCtrl=100 ) {
  few_IntegerTuner *p=new few_IntegerTuner(&font,v,low,high,delta,deltaAlt,deltaCtrl);
  widgets.Add(p);
  return p;
 }

 few_DecimalTuner *Bind( double *v ) {
  few_DecimalTuner *p=new few_DecimalTuner(&font,v);
  widgets.Add(p);
  return p;
 }
 few_DecimalTuner *Bind( double *v, double low, double high ) {
  few_DecimalTuner *p=new few_DecimalTuner(&font,v,low,high);
  widgets.Add(p);
  return p;
 }
 few_HSlider *Bind( having slider, double *v, double low, double high ) {
  few_HSlider *p=new few_HSlider(&font,v,low,high);
  widgets.Add(p);
  return p;
 }
 few_HSlider *Bind( having slider, double *v, double low, double high, having simplicity ) {
  few_HSlider *p=new few_HSlider(&font,v,low,high,true);
  widgets.Add(p);
  return p;
 }
 few_VSlider *Bind( having slider, double *v, double low, double high, having simplicity, having vertical ) {
  few_VSlider *p=new few_VSlider(&font,v,low,high,true);
  widgets.Add(p);
  return p;
 }
 few_VSlider *Bind( double *v, double low, double high, having vertical ) {
  few_VSlider *p=new few_VSlider(&font,v,low,high);
  widgets.Add(p);
  return p;
 }
 few_HRange *Bind( double *v, double *f, double low, double high ) {
  few_HRange *p=new few_HRange(&font,v,f,low,high);
  widgets.Add(p);
  return p;
 }
 few_ImagePicker *Bind( GLImage **i, const char *path, const char *description ) {
  few_ImagePicker *p=new few_ImagePicker(&font,i,path,description);
  widgets.Add(p);
  return p;
 }
 few_Textbox *Bind( std::string *t, int cols=20, int rows=1 ) {
  few_Textbox *p=new few_Textbox(&font,t,cols,rows);
  widgets.Add(p);
  return p;
 }
 few_ColorPick *Bind( Crayon *c ) {
  few_ColorPick *p=new few_ColorPick(&font,c);
  widgets.Add(p);
  return p;
 }
 few_ColorPick *Bind( Crayon *c, bool fansi,bool alpha=false ) {
  few_ColorPick *p=new few_ColorPick(&font,c,fansi,alpha);
  widgets.Add(p);
  return p;
 }
 few_Movement *Bind( Cartesian *c ) {
  few_Movement *p=new few_Movement(&font,c);
  widgets.Add(p);
  return p;
 }
 few_XYPad *Bind( Cartesian *c, having xyPad ) {
  few_XYPad *p=new few_XYPad(&font,c);
  widgets.Add(p);
  return p;
 }
 few_XYPad *Bind( Cartesiand *c ) {
  few_XYPad *p=new few_XYPad(&font,c);
  widgets.Add(p);
  return p;
 }
 few_Angle *Bind( having degrees, double *degAngle ) {
  few_Angle *p=new few_Angle(&font,degAngle);
  widgets.Add(p);
  return p;
 }
 few_Angle *Bind( double *radAngle, having radians ) {
  few_Angle *p=new few_Angle(&font,radAngle,true);
  widgets.Add(p);
  return p;
 }
 few_Toggle *Bind( bool *toggle ) {
  few_Toggle *p=new few_Toggle(&font,toggle);
  widgets.Add(p);
  return p;
 }
 few_Toggle *Bind( bool *toggle, const char *on, const char *off ) {
  few_Toggle *p=new few_Toggle(&font,toggle,on,off);
  widgets.Add(p);
  return p;
 }
 few_FilmPicker *Bind( Film **f ) {
  few_FilmPicker *p=new few_FilmPicker(&font,f);
  widgets.Add(p);
  return p;
 }
 few_Image *Show( GLImage *image ) {
  few_Image *p=new few_Image(&font,image);
  widgets.Add(p);
  return p;
 }
 few_Text *Show( const char *text ) {
  few_Text *p=new few_Text(&font,text);
  widgets.Add(p);
  return p;
 }
 few_Text *Show( const char *text, having left_justification ) {
  few_Text *p=new few_Text(&font,text,false);
  widgets.Add(p);
  return p;
 }
 few_Reserved *Reserve( int id, int w, int h ) {
  // Reserves a section with an empty widget so you can recall the region and draw on it later
  few_Reserved *p=new few_Reserved(&font,id,w,h);
  widgets.Add(p);
  return p;
 }
 void GetReserved( Cartesian *out, int id ) {
  few_Reserved *r=null;
  EACH(reserved.first,few_ReservedHandle,frh) if ( frh->p->id == id ) { out->SetRect(frh->p->x,frh->p->y,frh->p->w,frh->p->h); return; }
 }
 void Add( Proce55or *p ) {
  widgets.Append(p);
 }
};