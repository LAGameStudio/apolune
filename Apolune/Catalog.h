#pragma once
#include "GLWindow.h"
#include "fx_Buttons.h"
#include "Tweens.h"
#include "Proce55ors.h"
#include "CarouselItem.h"
#include "TexFont.h"
#include "FastGUI.h"

//enum NavButFuntions { forward=0, back=1, up=2, down=3 };

extern TexFont
 goldFont,
 silverFont,
 calibri,
 bytes,
 digicaps,
 microFont,
 neonNumbers,
 redNeonNumbers,
 goldNeonNumbers,
 alarmClockNumbers,
 nexaBold;

struct CatalogGridSettings {
	Crayon titleFontDrawing, catFontDrawing, iconTitleFontDrawing, iconDescFontDrawing;
	Zpointer<TexFont> titleFont, catFont, iconTitleFont, iconDescFont;
	Zpointer<Tween> titleFontTw;
	Zdouble titleFontDuration;
	Zfloat titleFontColorChangeDelay, catFontColorChangeDelay, iconTitleFontColorChangeDelay, iconDescFontColorChangeDelay;
	Blends titleFontBlend, catFontBlend, iconTitleFontBlend, iconDescFontBlend;
	Zfloat wStdf, error, iconTitleFontDescRatio;
	Zint xPixels, yPixels,iconTitleLineSpacing, iconTitleFontHeight, iconTitleFontWidth, iconTitleFontSpace, iconDescLineSpacing,
		iconDescFontWidth, iconDescFontHeight, iconDescFontSpace, catFontHeight, catFontWidth, catFontSpace, titleFontHeight, titleFontWidth, titleFontSpace,
		titleFontStart, xSpacing, ySpacing, xStart, yStart, xNum, yNum, xEnd, yEnd, wStd, hStd, wSides, wCatOffset, hCatOffset;

	CatalogGridSettings() {
	}

	void Set() {
			// screen title
		titleFontTw = tweens.find(string("Humanized In Out Slow"));
		titleFontDuration = 3.0;
		titleFont=&nexaBold;
		titleFontColorChangeDelay=5.0f;
		titleFontBlend=additive;
		titleFontDrawing.Pick(alabaster);
		titleFontHeight = 75;
		titleFontWidth = 40;
		titleFontSpace = 5;
		titleFontStart = 50;
			// individual catalog titles
		catFont = &nexaBold;
		catFontColorChangeDelay=5.0f;
		catFontBlend=additive;
		catFontDrawing.Pick(alabaster);
		catFontHeight = 62;
		catFontWidth = 32;
		catFontSpace = 5;
			// individual icon titles
		iconTitleFont = &nexaBold;
		iconTitleFontColorChangeDelay=5.0f;
		iconTitleFontBlend=additive;
		iconTitleFontDrawing.Pick(alabaster);
		iconTitleLineSpacing = 12; 
		iconTitleFontHeight = 24; 
		iconTitleFontWidth = 16;  
		iconTitleFontSpace = 2;
			// individual icon descriptions
		iconDescFont = &nexaBold;
		iconDescFontColorChangeDelay=5.0f;
		iconDescFontBlend=additive;
		iconDescFontDrawing.Pick(alabaster);
		iconDescLineSpacing = 12; 
		iconDescFontHeight = 17; 
		iconDescFontWidth = 12;  
		iconDescFontSpace = 1.5;  
			// layout parameters
		xPixels = display.w;
		yPixels = display.h;
		wCatOffset = 50; // space between x pos of 1st catalog icon and x pos of catalog title
		hCatOffset = 50; // space between y pos of catalog icons and y pos of catalog title
		xSpacing = 20; // space between catalog icons
		ySpacing = 3*iconTitleLineSpacing+iconTitleFontHeight+hCatOffset; // space between catalog rows
		wSides = 145; // horizontal padding for each row
		xStart = wSides; 
		xEnd = xPixels-wSides;
		yStart = 200;
		xNum = 4; // number of visible icons in each row
		wStdf = ((float)(xEnd-xStart)/(float) xNum)-(float)xSpacing; // width of each icon based on xNum and screen width
		wStd = (int) wStdf; // icon width casted to float
		error = wStdf - (float) wStd; // error between actual width and calculated width -- was important when calculating if a row should be rendered...probably not important anymore 
		hStd = wStd;
		xEnd = xStart + wStd*xNum + xSpacing*(xNum-1);
		yEnd = yPixels-hStd-2*(iconTitleLineSpacing+iconTitleFontHeight); 
		yNum = 2;
		//yNum = (yPixels-yStart)/(hStd+ySpacing-2*iconTitleLineSpacing); // number of visible rows on the screen
		yEnd = yEnd-(error*(yNum-1)); //adjust for float rounding error with hStd

		OUTPUT("Screen -- xPixels: %d, yPixels: %d\n", (int) xPixels, (int) yPixels);
		OUTPUT("Size -- wStd: %d, hStd: %d\n", (int) wStd, (int) hStd);
		OUTPUT("Start -- xStart: %d, yStart: %d\n", (int) xStart, (int) yStart);
		OUTPUT("End -- xEnd: %d, yEnd: %d\n", (int) xEnd, (int) yEnd);
		OUTPUT("Num -- yNum: %d\n", (int) yNum);
	}
};

extern CatalogGridSettings catalogGridSettings;

class NavButton : public Proce55or {
public:
	Zpointer<Tween> t,t2;
	Zpointer<GLImage> icon;
	Zint function;
	Cartesian rect;
	Zbool active, clicking, clicked, wasClicked, wasSelected;
	Zdouble expired, duration, instant, countdown, alpha, alpha2;
	Crayon color, inner, outer, innerDefault, outerDefault;

	NavButton(int f) : Proce55or() {
		function = f;
		switch ((int) function) {
				//forward button -- needs a different icon
			case 0: icon = library.Load("data/PM/icons/arrow_right_2.png"); break;
				//back button -- needs a different icon
			case 1: icon = library.Load("data/PM/icons/arrow_left_2.png"); break;
				//up button -- needs a different icon
			case 2: icon = library.Load("data/PM/icons/arrow_up.png"); break;
				//down button -- needs a different icon
			case 3: icon = library.Load("data/PM/icons/arrow_down.png"); break;
		}
	}

	void Setup() {
		t = tweens.find(string("Humanized In Out Slow"));
		t2 = tweens.find(string("Humanized Wobble Rise"));
		duration = 3.0;
		color.Pick(alabaster);
	}

	void OnSetExtents() {
		rect.SetRect(x, y, w, h);
	}

	void Between() {
		active = within(input.mxi,input.myi);
		clicking = active && (input.left > 0);
		clicked = active && input.leftReleased();
		if (!wasClicked && clicked) { 
			wasClicked = true;
			countdown = .25;
		} else if (wasClicked) {
			countdown -= FRAMETIME;
			if (countdown <= 0) {
				wasSelected = true;
				wasClicked = false;
			}
		}
		if (!active && input.leftReleased()) {
			wasSelected = false;
			wasClicked = false;
		}
		expired += FRAMETIME;

	}

	void Render() {
		Stretchi(icon.pointer,color,1.0f,transparency,rect.x,rect.y,rect.w,rect.h);
	}
};

class NavArrows {
public:
	NavButton *forwardButtonTop, *forwardButtonBot, *backButtonTop, *backButtonBot, *upButton, *downButton;
	Zfloat groupScale;
	Proce55ors fx;
	Zpointer<GLWindow> parent;

	NavArrows(GLWindow *p, float scale) {
		parent = p;
		groupScale = scale;
		AddArrows();
	}

	void AddArrows() {
		forwardButtonTop = new NavButton(0);	
		backButtonTop = new NavButton(1);
		forwardButtonBot = new NavButton(0);				
		backButtonBot = new NavButton(1);	
		upButton = new NavButton(2);
		downButton = new NavButton(3);
		
		fx.Add(forwardButtonTop,parent);
		fx.Add(backButtonTop,parent);
		fx.Add(forwardButtonBot,parent);
		fx.Add(backButtonBot,parent);
		fx.Add(upButton, parent);
		fx.Add(downButton, parent);

		forwardButtonTop->Setup();
		forwardButtonTop->SetExtents((int)(catalogGridSettings.xPixels-(catalogGridSettings.wSides/2))-(int)(60.0f*groupScale/2.0f),
			catalogGridSettings.yStart+catalogGridSettings.hCatOffset+catalogGridSettings.hStd/2-(int)(120.0f*groupScale/2.0f), 
			(int) (60.0f*groupScale),
			(int)(120.0f*groupScale));

		backButtonTop->Setup();
		backButtonTop->SetExtents((int)(catalogGridSettings.wSides/2)-(int)(60.0f*groupScale/2.0f),
			catalogGridSettings.yStart+catalogGridSettings.hCatOffset+catalogGridSettings.hStd/2-(int)(120.0f*groupScale/2.0f), 
			(int) (60.0f*groupScale),
			(int)(120.0f*groupScale));
		
		forwardButtonBot->Setup();
		forwardButtonBot->SetExtents((int)(catalogGridSettings.xPixels-(catalogGridSettings.wSides/2))-(int)(60.0f*groupScale/2.0f),
			catalogGridSettings.yStart+catalogGridSettings.hCatOffset+catalogGridSettings.hStd/2+catalogGridSettings.ySpacing+catalogGridSettings.hStd-(int)(120.0f*groupScale/2.0f), 
			(int) (60.0f*groupScale),
			(int)(120.0f*groupScale));
		
		backButtonBot->Setup();
		backButtonBot->SetExtents((int)(catalogGridSettings.wSides/2)-(int)(60.0f*groupScale/2.0f),
			catalogGridSettings.yStart+catalogGridSettings.hCatOffset+catalogGridSettings.hStd/2+catalogGridSettings.ySpacing+catalogGridSettings.hStd-(int)(120.0f*groupScale/2.0f), 
			(int) (60.0f*groupScale),
			(int)(120.0f*groupScale));
		
		upButton->Setup();
		upButton->SetExtents((int) catalogGridSettings.xPixels/2-(int)(120.0f*groupScale/2.0f), (int) catalogGridSettings.yStart- catalogGridSettings.hCatOffset -(int)(60.0f*groupScale/2.0f), (int)(120.0f*groupScale),(int)(60.0f*groupScale));
		
		downButton->Setup();
		downButton->SetExtents((int) catalogGridSettings.xPixels/2-(int)(120.0f*groupScale/2.0f), (int) (catalogGridSettings.yPixels-catalogGridSettings.yStart/3)-(int)(60.0f*groupScale/2.0f), (int)(120.0f*groupScale),(int)(60.0f*groupScale));
	}
};

class PersonalizeButton : public Proce55or {
public:
	Zstring target;
	Cartesian rect;
	Zpointer<GLImage> icon;
	Zbool active, clicking, clicked, wasClicked, wasSelected, wasRegistered, isVisible;
	Zdouble expired, duration, instant, alpha, countdown;
	Crayon color;

	PersonalizeButton() : Proce55or() {}

	void Setup(Zstring t, Zint _x, Zint _y) {
		wasClicked = false;
		wasSelected = false;
		target = t;
		icon = library.Load("data/PM/icons/personalizebutton_gray_bg.png"); 
		color.Pick(alabaster);
		duration = 3.0;
		countdown = 1.0;
		SetExtents(_x, _y, 3*catalogGridSettings.wStd/4, 40); 
	}

	void Between() {
		active = within(input.mxi,input.myi);
		clicking = active && (input.left > 0);
		clicked = active && input.leftReleased();
		if (!wasClicked && clicked) { 
			wasClicked = true;
			countdown = .25;
		} else if (wasClicked) {
			countdown -= FRAMETIME;
			if (countdown <= 0) {
				wasSelected = true;
				wasClicked = false;			
			}
		}

		if (wasSelected) WhenClicked();

		if (!active && input.leftReleased()) wasSelected = false;
		expired += FRAMETIME;
	}

	void OnSetExtents() {
		rect.SetRect(x,y,w,h);
	}

	void Render() {
		Stretchi(icon.pointer,color,1.0f,transparency,rect.x,rect.y,rect.w,rect.h);
	}
		//populate with actions to begin customization vignettes
	void WhenClicked() {
		wasSelected = false;
		OUTPUT("clicked: personalize %s", (char *) target);
	}
};

class CatalogIcon : public Proce55or {
public:
	Zpointer<Tween> t;
	Zpointer<GLImage> icon, highlight; 
	Zpointer<PersonalizeButton> personalizeButton;
	Zint yIndex;
	Cartesian rect;
	Zbool active, clicking, clicked, wasClicked, wasSelected, wasRegistered;
	Zdouble expired, duration, instant, alpha, countdown;
	Crayon color;
	CarouselItem carouselItem;
	
	CatalogIcon( CarouselItem *i, Zint *c ) : Proce55or() {
		carouselItem = *i;
		yIndex = *c;
		highlight = library.Load("data/PM/icons/SelectedIconBox.png");
		icon = carouselItem.icon;
		personalizeButton = new PersonalizeButton();
  hidden=true;
		//catalogGridSettings.titleFontColorChangeDelay=10.0f;
		//catalogGridSettings.titleFontBlend=additive;
		//catalogGridSettings.titleFontDrawing.Pick(alabaster);
	}

	void Setup() {
		wasClicked = false;
		wasSelected = false;
		wasRegistered = false; //tracks whether selection has been acknowledged and other icons "slid" out of way accordingly
		color.Pick(alabaster);
		personalizeButton->Setup(carouselItem.filename, x, y);
		t = tweens.find(string("Humanized In Out Slow"));
		duration = 3.0;
		countdown = 1.0;
			// proce55ors->count-1 used to determine position of this icon in the row
		SetExtents(catalogGridSettings.xStart+((proce55ors->count-1)*(catalogGridSettings.wStd+catalogGridSettings.xSpacing)),
			catalogGridSettings.yStart+catalogGridSettings.hCatOffset+(yIndex*(catalogGridSettings.hStd+catalogGridSettings.ySpacing)),
			catalogGridSettings.wStd,
			catalogGridSettings.hStd);
	}

	void OnSetExtents() {
			// set location of personalize button -- offset to the left or right, depending on which half of screen the icon is on
		if (x < catalogGridSettings.xPixels/2) personalizeButton->SetExtents(x + (1.125f*(float)catalogGridSettings.wStd) + catalogGridSettings.xSpacing,
			y + catalogGridSettings.hStd - 45, personalizeButton->w, personalizeButton->h);
		else personalizeButton->SetExtents(x - catalogGridSettings.xSpacing - (0.875f*(float)catalogGridSettings.wStd),
			y + catalogGridSettings.hStd - 45, personalizeButton->w, personalizeButton->h);

			// set location of the icon image
		rect.SetRect(x,y,w,h);
	}

	void Between() {
  if ( !hidden ) {
	 	personalizeButton->Between();
	 	
	 	active = within(input.mxi,input.myi);
	 	clicking = active && (input.left > 0);
	 	clicked = active && input.leftReleased();
	 	if (!wasClicked && clicked) { 
	 		wasClicked = true;
	 		countdown = .25;
	 	} else if (wasClicked) {
	 		countdown -= FRAMETIME;
	 		if (countdown <= 0) {
	 			wasSelected = true;
	 			wasClicked = false;
	 		}
	 	}
	 	expired += FRAMETIME;
	 		// only display 4 icons in each row
	 	hidden = ! ( (rect.x >= (int) catalogGridSettings.xStart) 
	 		&& (rect.x <= (int) (catalogGridSettings.xEnd-catalogGridSettings.wStd) ) );
  }
	}

	void Render() {
			//only draw if icon is on the screen
		if (!hidden) {			
			if (wasSelected) {	
					// icon is on left half of screen --> draw info right of icon
				if (x < catalogGridSettings.xPixels/2) { 
						// outline the selected icon with a box -- improvement: hard code the box instead of using .png
					Stretchi(highlight.pointer,color,1.0f,transparency,
					rect.x-9,rect.y-9,(int)(2*rect.w+40),(int)(rect.h+17));
					
						//write price and description in the gray box
					/*
					WriteTexFont(catalogGridSettings.iconDescFont,catalogGridSettings.iconDescFontDrawing,catalogGridSettings.iconDescFontBlend,
						rect.x+catalogGridSettings.wStd+catalogGridSettings.xSpacing/2,
						rect.y,catalogGridSettings.iconDescFontWidth,catalogGridSettings.iconDescFontHeight,catalogGridSettings.iconDescFontSpace, carouselItem.description);
					*/
				} else { // icon is on right half of screen --> draw info left of icon
						// outline the selected icon with a box -- improvement: hard code the box instead of using .png
					Stretchi(highlight.pointer,color,1.0f,transparency,
					rect.x-(rect.w+29),rect.y-9,(int)(2*rect.w+40),(int)(rect.h+17));

						//write price and description in the gray box
					/*
					WriteTexFont(catalogGridSettings.iconDescFont,catalogGridSettings.iconDescFontDrawing,catalogGridSettings.iconDescFontBlend,
						rect.x-catalogGridSettings.wStd+catalogGridSettings.xSpacing/2,
						rect.y,catalogGridSettings.iconDescFontWidth,catalogGridSettings.iconDescFontHeight,catalogGridSettings.iconDescFontSpace, carouselItem.description);
					*/
				}
				personalizeButton->Render();
			}
			
			Stretchi(icon.pointer,color,1.0f,transparency,rect.x,rect.y,rect.w,rect.h);
				
				// draw icon title
			WriteTexFont(catalogGridSettings.iconTitleFont,catalogGridSettings.iconTitleFontDrawing,catalogGridSettings.iconTitleFontBlend,rect.x,rect.y+catalogGridSettings.hStd+catalogGridSettings.iconTitleLineSpacing,
				catalogGridSettings.iconTitleFontWidth,catalogGridSettings.iconTitleFontHeight,catalogGridSettings.iconTitleFontSpace,carouselItem.title);
		}
	}
};

class CatalogCategory : public ListItem {
public:
	Zstring name;
	Zint count;
	Proce55ors fx;
	Proce55or *firstIcon, *lastIcon;
	CarouselCatalog carouselCatalog;
	ScreenPosition position; // not used
	Zpointer<GLWindow> parent;
	Zp<CatalogIcon> currentSelection; //catIcon
	Zbool containsSelection;
 Zbool hidden;

	CatalogCategory() : ListItem() {}

	CatalogCategory( String *s, GLWindow *p, Zint *c ) : ListItem() {
		Set(s,p,c);
		//catIcon = (CatalogIcon *) fx.first; //probably not needed -- remnant
	}

	void Set( String *s, GLWindow *p, Zint *c ) { 
		name=s->s;
		parent=p;
		count=*c;
  hidden=true;
		containsSelection = false;
	}
	
	bool atStart() {
		return ( firstIcon->x == catalogGridSettings.xStart );
	}

	bool atEnd() {
		return ( lastIcon->x <= catalogGridSettings.xEnd );
	}

	void Add( CarouselItem *i ) { 
		CatalogIcon *catIcon;
		catIcon = new CatalogIcon(i, &count); // count used to keep track of yPostion for the icon -- it is the index of the category
		fx.Add(catIcon,parent); 
		catIcon->Setup();
	}

	void AdjustXPosition(int deltaX) {
		if ((!atStart() && (deltaX > 0)) || (!atEnd() && (deltaX < 0))) {
			//if (containsSelection) closeSelection(); //improvement: disappear only if on edge
			EACH(fx.first, CatalogIcon, ci) {
				ci->wasSelected = false; //workaround (shouldn't be needed) -- for some reason catalog icons were being selected erroneously
				ci->SetExtents(ci->x+deltaX,ci->y,ci->w,ci->h);
			}
		} 
	}

	void checkForSelection(){
		EACH(fx.first, CatalogIcon, ci) {
			if (ci->wasSelected && !ci->wasRegistered) {
					//if selection already exists, close it
				if (currentSelection) closeSelection();
				openSelection(ci);
			}
		}
	}

	void openSelection(CatalogIcon *ci) {
			//shift icon positions out 
		OUTPUT("opening %s from %s category\n", (char *) ci->carouselItem.title, (char *) name);
		OUTPUT("ci_x: %d -- xPixels/2: %d\n", (int) ci->x, (int) catalogGridSettings.xPixels/2);
		if ((ci->x < catalogGridSettings.xPixels/2) && ci->next) {
			OUTPUT("Left half\n");
			EACH(ci->next, CatalogIcon, ci_) ci_->SetExtents(ci_->x+catalogGridSettings.wStd+catalogGridSettings.xSpacing, ci_->y, ci_->w, ci_->h);
		} else if (ci->prev) {
			OUTPUT("Right half\n");
			EACHREV(ci->prev, CatalogIcon, ci_) ci_->SetExtents(ci_->x-catalogGridSettings.wStd-catalogGridSettings.xSpacing, ci_->y, ci_->w, ci_->h);
		}
		ci->wasRegistered = true;
		containsSelection = true;
		currentSelection = ci;
	}

	void closeSelection() {
			//shift icon positions back
		OUTPUT("closing %s piece from %s category\n", (char *) currentSelection->carouselItem.title, (char *) name);
		OUTPUT("current_x: %d -- xPixels/2: %d\n", (int) currentSelection->x, (int) catalogGridSettings.xPixels/2);
		if ((currentSelection->x < catalogGridSettings.xPixels/2) && currentSelection->next) {
			OUTPUT("LEft half\n");
			EACH(currentSelection->next, CatalogIcon, ci) ci->SetExtents(ci->x-catalogGridSettings.wStd-catalogGridSettings.xSpacing, ci->y, ci->w, ci->h);
		} else if (currentSelection->prev) {
			OUTPUT("Right half\n");
			EACHREV(currentSelection->prev, CatalogIcon, ci) ci->SetExtents(ci->x+catalogGridSettings.wStd+catalogGridSettings.xSpacing, ci->y, ci->w, ci->h);
		}
		currentSelection->wasRegistered = false;
		currentSelection->wasSelected = false;
		currentSelection = NULL;
		containsSelection = false;
	}
};

class CatalogCategories : public LinkedList {
public:
	CLEARLISTRESET(CatalogCategory);
 Strings categories;
	Zpointer<GLWindow> parent;
	Zp<CatalogCategory> activeTop, activeBot, firstCat, lastCat, temp;

	CatalogCategories(GLWindow *p) : LinkedList() {
		parent = p;
		AddFromCatalog();
		activeTop = (CatalogCategory *)first;
		activeBot = (CatalogCategory *)first->next;
		firstCat = (CatalogCategory *)first;
		lastCat = (CatalogCategory *)last;
	}

	void AddFromCatalog() {
			// Find all unique strings to categories
		EACH(carouselCatalog.first,CarouselItem,c) {
			if (!categories.inList(c->category.value)) categories.Add(c->category.c_str());
		}
			// Create a category adding it to this list
		EACH(categories.first,String,s) {
			Append(new CatalogCategory(s,parent, &count));
		}
			// Add each item to the correct category
		FOREACH(CatalogCategory,cc) {
			EACH(carouselCatalog.first,CarouselItem,ci) {
				if ( !str_cmp((char *) cc->name, (char *) ci->category) ) {
					cc->Add(ci);
				}
				cc->firstIcon = (Proce55or *) cc->fx.first;
				cc->lastIcon = (Proce55or *) cc->fx.last;
			}
		}
	}
		// check if the y position of the last categories' icons are within the visible region
	bool atBottom() {
		Zp<CatalogIcon> temp = (CatalogIcon *) lastCat->fx.first;
		return ( temp->y <= catalogGridSettings.yEnd ); 
	}

		// check if the y position of the first categories' icons are within the visible region
	bool atTop() {
		Zp<CatalogIcon> temp = (CatalogIcon *) firstCat->fx.first;
		return ( temp->y == catalogGridSettings.yStart+catalogGridSettings.hCatOffset);
	}

	void AdjustYPosition(int deltaY) {
		if ((!atBottom() && (deltaY < 0)) || (!atTop() && (deltaY > 0))) {
			FOREACH(CatalogCategory, cc) {
				EACH(cc->fx.first, CatalogIcon, ci) ci->SetExtents(ci->x,ci->y+deltaY,ci->w,ci->h);
			}
			if (deltaY < 0) {
				activeTop = (CatalogCategory *) activeTop->next;
				activeBot = (CatalogCategory *) activeTop->next;
			} else {
				activeTop = (CatalogCategory *) activeTop->prev;
				activeBot = (CatalogCategory *) activeTop->next;
			}
		} 
	}

	void Setup() {
		;//FOREACH(CatalogCategory,cc) cc->fx.Setup();
	}

	void Between() {
		FOREACH(CatalogCategory,cc) {
				// check if selection is open in any of the other categories			
			if (cc->containsSelection && activeTop->containsSelection){
				if (cc != activeTop) cc->closeSelection();
			}
			if (cc->containsSelection && activeBot->containsSelection){
				if (cc != activeBot) cc->closeSelection();
			}
				
				// check if selection is open in the current category
			cc->checkForSelection();
			
			cc->fx.Between(null);
			
				// display only up to yNum categories
			cc->hidden = true;
			temp = (CatalogCategory *) activeTop;
			for (int i=0; i<catalogGridSettings.yNum; i++){ //checks if the current category is the active top or the active bottom
				if (cc == temp) cc->hidden=false;
				temp = (CatalogCategory *) temp->next;
			}
		}
	}

	void Render() {
		FOREACH(CatalogCategory,cc) {
			if (!cc->hidden) cc->fx.Render();
		}

	}
};

	// not used right now
HANDLES(CatalogCategory,CatalogCategoryHandle,CatalogCategoryHandles,"CatalogCategory"); 

class CatalogCategoryTitles: public LinkedList {
public:
	CLEARLISTRESET(CatalogCategoryTitles);
    Strings activeCategories;
	Zp<GLWindow> parent;

	CatalogCategoryTitles(GLWindow *p) : LinkedList() {
		parent = p;
	}
};

class CatalogView : public GLWindow {
public:
	CatalogCategories *kioskCategories;
	CatalogCategoryHandles activated, deactivated;
	NavArrows *navArrows;
	Zdouble expired, duration, instant, alpha;

	CatalogView() : GLWindow() {}

	void OnLoad() { 
		background.Pick(indigo);
		Fullscreen();
		catalogGridSettings.Set();
		kioskCategories = new CatalogCategories(this);
		navArrows = new NavArrows(this,1.0);
		
		kioskCategories->Setup();
			// not used right now
		for (int i=0; i<catalogGridSettings.yNum; i++) {
			activated.Add((CatalogCategory *) kioskCategories->Element(i));
		}
	}

	void ShiftGridX(int deltaX) {
		EACH(kioskCategories->first,CatalogCategory, cc) {
			cc->AdjustXPosition(deltaX);
		}
	}

	void Between() {
		kioskCategories->Between();
		navArrows->fx.Between(null);

		/*if ( catalogGridSettings.titleFontColorChangeDelay > 0.0f ) catalogGridSettings.titleFontColorChangeDelay-=FRAMETIME;
		else {
			catalogGridSettings.titleFontDrawing.Any();
			catalogGridSettings.titleFontColorChangeDelay=5.0f;
		}*/

		expired += FRAMETIME;
		instant = expired/catalogGridSettings.titleFontDuration;
		alpha = catalogGridSettings.titleFontTw->tweend(instant);
		//catalogGridSettings.titleFontDrawing.ScaleByAlphaf((float) alpha);

			//Will two ever register clicked simultaneously?
		if (navArrows->forwardButtonTop->wasSelected) {		
			if (kioskCategories->activeTop->containsSelection) kioskCategories->activeTop->closeSelection();
			kioskCategories->activeTop->AdjustXPosition(-catalogGridSettings.wStd-catalogGridSettings.xSpacing);
			navArrows->forwardButtonTop->wasSelected = false;
		} else if (navArrows->backButtonTop->wasSelected) {
			if (kioskCategories->activeTop->containsSelection) kioskCategories->activeTop->closeSelection();
			kioskCategories->activeTop->AdjustXPosition(catalogGridSettings.wStd+catalogGridSettings.xSpacing);
			navArrows->backButtonTop->wasSelected = false;
		} else if (navArrows->forwardButtonBot->wasSelected) {
			if (kioskCategories->activeBot->containsSelection) kioskCategories->activeBot->closeSelection();
			kioskCategories->activeBot->AdjustXPosition(-catalogGridSettings.wStd-catalogGridSettings.xSpacing);
			navArrows->forwardButtonBot->wasSelected = false;
		} else if (navArrows->backButtonBot->wasSelected) {
			if (kioskCategories->activeBot->containsSelection) kioskCategories->activeBot->closeSelection();
			kioskCategories->activeBot->AdjustXPosition(catalogGridSettings.wStd+catalogGridSettings.xSpacing);
			navArrows->backButtonBot->wasSelected = false;
		} else if (navArrows->downButton->wasSelected) { // doesn't close an open selection when moving up and down (selection will get closed when a new selection is made) -- uncomment below to change this
			if (kioskCategories->activeBot->containsSelection) kioskCategories->activeBot->closeSelection();
			if (kioskCategories->activeTop->containsSelection) kioskCategories->activeTop->closeSelection();
			kioskCategories->AdjustYPosition(-catalogGridSettings.wStd-catalogGridSettings.ySpacing);
			navArrows->downButton->wasSelected = false;
		} else if (navArrows->upButton->wasSelected) { // doesn't close an open selection when moving up and down (selection will get closed when a new selection is made) -- uncomment below to change this
			if (kioskCategories->activeBot->containsSelection) kioskCategories->activeBot->closeSelection();
			if (kioskCategories->activeTop->containsSelection) kioskCategories->activeTop->closeSelection();
			kioskCategories->AdjustYPosition(catalogGridSettings.wStd+catalogGridSettings.ySpacing);
			navArrows->upButton->wasSelected = false;
		}

	}

	void Render() {
		Zp<CatalogCategory> temp;
		kioskCategories->Render();
		navArrows->fx.Render();
		
		temp = (CatalogCategory *) kioskCategories->activeTop;
		
			// draw screen title
		WriteTexFont(catalogGridSettings.titleFont,catalogGridSettings.titleFontDrawing,catalogGridSettings.titleFontBlend,
			50,catalogGridSettings.titleFontStart, catalogGridSettings.titleFontWidth,catalogGridSettings.titleFontHeight,catalogGridSettings.titleFontSpace,"CHOOSE A PIECE TO PERSONALIZE");
		
			// draw active category titles
		for(int i=0; i<catalogGridSettings.yNum; i++) {
			WriteTexFont(catalogGridSettings.catFont, catalogGridSettings.catFontDrawing, catalogGridSettings.catFontBlend,
				catalogGridSettings.xStart-catalogGridSettings.wCatOffset, catalogGridSettings.yStart+i*(catalogGridSettings.ySpacing+catalogGridSettings.hStd), 
				catalogGridSettings.catFontWidth, catalogGridSettings.catFontHeight, catalogGridSettings.catFontSpace, (char *) temp->name);
			temp = (CatalogCategory *) temp->next;
		}
	}
};

