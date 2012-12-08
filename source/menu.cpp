#include "menu.h"

void Menu::enterMenu(GameInfo &info) {
	
	UL_IMAGE *minesTitle;
	
	minesTitle = ulLoadImageFilePNG((const char*)mines, (int)mines_size, UL_IN_VRAM, UL_PF_5551 );
	
	if(!_state) {
		
		while(1) {
			
			ulReadKeys(0);
			
			if(ul_keys.touch.click || ul_keys.pressed.start) {
				
				_state = 1;
				break;
			}
			
			ulStartDrawing2D();
		
			if(ulGetMainLcd()) {
				
				ulDrawGradientRect(0, 0, MSW, MSH, RGB15(0, 0, 24), RGB15(0, 0, 24), RGB15(0, 16, 31), RGB15(0, 16, 31));
				
				ulPrintf_xy(92, 100, "Touch Screen");
				
			}
			else {
				
				ulSetAlpha(UL_FX_DEFAULT, 0, 0);
				ulDrawGradientRect(0, 0, MSW, MSH, RGB15(0, 0, 24), RGB15(0, 0, 24), RGB15(0, 16, 31), RGB15(0, 16, 31));
				
				minesTitle->x = 0;
				minesTitle->y = 0;
				
				ulSetAlpha(UL_FX_ALPHA, 31, 1);
				ulDrawImage(minesTitle);
				ulSetAlpha(UL_FX_DEFAULT, 0, 0);
			}
		
			ulEndDrawing();
		
			ulSyncFrame();
			
		}
	}
	
	if(_state == 1) {
		
		u16 w = info._width, h = info._height, b = info._nBombs, s = info._size;
	
		u16 who = 0, count = 0, wait = 30;
	
		u16 slid1 = START, slid2 = START, slid3 = START, curPos = START;
		
		UL_IMAGE *slid;
		UL_IMAGE *arrow;
		UL_IMAGE *space;
		UL_IMAGE *startbt;
		UL_IMAGE *diff1;
		UL_IMAGE *diff2;
		UL_IMAGE *diff3;
		
		arrow = ulLoadImageFilePNG((const char*)seta, (int)seta_size, UL_IN_VRAM, UL_PF_PAL8);
		space = ulLoadImageFilePNG((const char*)espaco, (int)espaco_size, UL_IN_VRAM, UL_PF_PAL8);
		startbt = ulLoadImageFilePNG((const char*)start, (int)start_size, UL_IN_VRAM, UL_PF_PAL8);
		diff1 = ulLoadImageFilePNG((const char*)easy, (int)easy_size, UL_IN_VRAM, UL_PF_PAL8);
		diff2 = ulLoadImageFilePNG((const char*)meh, (int)meh_size, UL_IN_VRAM, UL_PF_PAL8);
		diff3 = ulLoadImageFilePNG((const char*)hard, (int)hard_size, UL_IN_VRAM, UL_PF_PAL8);
		slid = ulLoadImageFilePNG((const char*)slider, (int)slider_size, UL_IN_VRAM, UL_PF_5551);
	
		ulDisableTransparentColor();
		
		while(1) {
			
			ulReadKeys(0);
			
			if(wait > 0) wait--;
			
			if(ul_keys.touch.released){
				who = 0;
				count = 0;
			}
			
			if(ul_keys.touch.click || ul_keys.touch.held) {
				
			
				if(touchTest(10 ,20 ,10 + SQUARE ,20 + SQUARE)) {
				
					if(who == 1){
						
						if(count > 10) w--;
						else count++;
					}
					else if(who == 0) {
						
						who = 1;
						count = 0;
						w--;
					}
					
					if(w < 5) w = 5;
				}
				
				if(touchTest(58 ,20 ,58 + SQUARE ,20 + SQUARE)) {
					
					if(who == 2){
						
						if(count > 10) w++;
						else count++;
					}
					else if(who == 0) {
						
						who = 2;
						count = 0;
						w++;
					}
					
					if(w > 32) w = 32;
				}
				
				if(touchTest(10 ,60 ,10 + SQUARE ,60 + SQUARE)) {
					
					if(who == 3){
						
						if(count > 10) h--;
						else count++;
					}
					else if(who == 0) {
						
						who = 3;
						count = 0;
						h--;
					}
					
					if(h < 5) h = 5;
				}
				
				if(touchTest(58 ,60 ,58 + SQUARE ,60 + SQUARE)) {
					
					if(who == 4){
						
						if(count > 10) h++;
						else count++;
					}
					else if(who == 0) {
						
						who = 4;
						count = 0;
						h++;
					}
					
					if(h > 24) h = 24;
				}
				
				if(touchTest(10 ,100 ,10 + SQUARE ,100 + SQUARE)) {
					
					if(who == 5){
						
						if(count > 10) b--;
						else count++;
					}
					else if(who == 0) {
						
						who = 5;
						count = 0;
						b--;
					}
				}
				
				if(touchTest(58 ,100 ,58 + SQUARE ,100 + SQUARE)) {
					
					if(who == 6){
						
						if(count > 10) b++;
						else count++;
					}
					else if(who == 0) {
						
						who = 6;
						count = 0;
						b++;
					}
				}
				
				if(touchTest(10, 125, 10 + 2*SQUARE, 125 + SQUARE)) {
					if( who == 0 || who == 7 ) {
						w=9;
						h=9;
						b=10;
						who = 7;
					}
				}
				if(touchTest(50, 125, 50 + 2*SQUARE, 125 + SQUARE)) {
					if( who == 0 || who == 8 ) {
						w=16;
						h=16;
						b=40;
						who = 8;
					}
				}
				if(touchTest(90, 125, 90 + 2*SQUARE, 125 + SQUARE)) {
					if( who == 0 || who == 9 ) {
						w=30;
						h=16;
						b=99;
						who = 9;
					}
				}
				
				if(touchTest(START + 4 ,20 ,150 + START + 4,20 + SQUARE)) {
					if( who == 0 || who == 10 ) {
						curPos = ul_keys.touch.x - 4;
						w = (u16) round(0.27 * (curPos - START)*TWOTHIRDS + 5);
						who = 10;
					}
				}
				
				if(touchTest(START + 4,60 ,150 + START + 4,60 + SQUARE)) {
					if( who == 0 || who == 11 ) {
						curPos = ul_keys.touch.x - 4;
						h = (u16) round(0.19 * (curPos - START)*TWOTHIRDS + 5);
						who = 11;
					}
				}
				
				if(touchTest(START + 4,100 ,150 + START + 4,100 + SQUARE)) {
					if( who == 0 || who == 12 ) {
						curPos = ul_keys.touch.x - 4;
						b = (u16) round(w*h*0.65*0.01 * (curPos - START)*TWOTHIRDS + w*h*0.05);
						who = 12;
					}
				}
				
				b = checkBomb(w, h, b);
				
				if(ul_keys.pressed.Y) {
					//size s
				}
				
				if(touchTest(10 ,150 ,10 + 64 ,150 + 32)) {
					if( who == 0 || who == 13 ) {
						break;
						//(w, h, b, s);
						//wait = 30;
						//who = 13;
					}
				}
			}
			
			if(ul_keys.pressed.start && !wait) {
				if( who == 0 || who == 13 ) {
					break;
					//mainGame(w, h, b, s);
					//wait = 30;
					//who = 13;
				}
			}
			
			slid1 = (u16) floor((w - 5) / (0.27 * (TWOTHIRDS)) + START);
			slid2 = (u16) floor((h - 5) / (0.19 * (TWOTHIRDS)) + START);
			slid3 = (u16) floor((b - w*h*0.05) / (w*h*0.65*0.01 * (TWOTHIRDS)) + START);
			
			ulStartDrawing2D();
			
			if(ulGetMainLcd()) {
				
				ulDrawGradientRect(0, 0, MSW, MSH, RGB15(0, 0, 24), RGB15(0, 0, 24), RGB15(0, 16, 31), RGB15(0, 16, 31));
				
				ulSetTextColor(RGB15(0,0,0));
				
				ulMirrorImageH(arrow,1);
				arrow->x = 10;
				arrow->y = 20;
				ulDrawImage(arrow);
				ulMirrorImageH(arrow,0);
				space->x = 30;
				space->y = 20;
				ulDrawImage(space);
				ulPrintf_xy(35, 25, "%d" , w);
				arrow->x = 58;
				arrow->y = 20;
				ulDrawImage(arrow);
				
				ulMirrorImageH(arrow,1);
				arrow->x = 10;
				arrow->y = 60;
				ulDrawImage(arrow);
				ulMirrorImageH(arrow,0);
				space->x = 30;
				space->y = 60;
				ulDrawImage(space);
				ulPrintf_xy(35, 65, "%d" , h);
				arrow->x = 58;
				arrow->y = 60;
				ulDrawImage(arrow);
				
				ulMirrorImageH(arrow,1);
				arrow->x = 10;
				arrow->y = 100;
				ulDrawImage(arrow);
				ulMirrorImageH(arrow,0);
				space->x = 30;
				space->y = 100;
				ulDrawImage(space);
				if(b > 99) ulPrintf_xy(33, 105, "%d" , b);
				else ulPrintf_xy(35, 105, "%d" , b);
				arrow->x = 58;
				arrow->y = 100;
				ulDrawImage(arrow);
				
				ulDrawLine(START, 28, 150 + START, 28, RGB15(0, 0, 24));
				ulDrawLine(START, 68, 150 + START, 68, RGB15(0, 0, 24));
				ulDrawLine(START, 108, 150 + START, 108, RGB15(0, 0, 24));
				slid->x = slid1;
				slid->y = 20;
				ulDrawImage(slid);
				slid->x = slid2;
				slid->y = 60;
				ulDrawImage(slid);
				slid->x = slid3;
				slid->y = 100;
				ulDrawImage(slid);
				
				//ulPrintf_xy(0, 0, "%d,%d,%d,%d" , slid1,slid2,slid3,curPos-START);

				diff1->x = 10;
				diff1->y = 125;
				ulDrawImage(diff1);
				diff2->x = 50;
				diff2->y = 125;
				ulDrawImage(diff2);
				diff3->x = 90;
				diff3->y = 125;
				ulDrawImage(diff3);
				
				startbt->x = 10;
				startbt->y = 150;
				ulDrawImage(startbt);
				
				ulSetTextColor(RGB15(31,31,31));
				ulPrintf_xy(84,158, "Press start at anytime");
				ulPrintf_xy(84,168, "to return to menu");
				ulPrintf_xy(10, 10, "Width:");
				ulPrintf_xy(10, 50, "Height:");
				ulPrintf_xy(10, 90, "Bombs:");
				
			} else {
				//Group 0, no blending
				ulSetAlpha(UL_FX_DEFAULT, 0, 0);
				ulDrawGradientRect(0, 0, MSW, MSH, RGB15(0, 0, 24), RGB15(0, 0, 24), RGB15(0, 16, 31), RGB15(0, 16, 31));
				minesTitle->x = 0;
				minesTitle->y = 0;
				//Group 1, alpha = 31 (fully opaque)
				ulSetAlpha(UL_FX_ALPHA, 31, 1);
				//This image can have an alpha channel (or if you set alpha to another value, it will be semi transparent). It will blend with polygons of any group other than 1.
				ulDrawImage(minesTitle);
				ulSetAlpha(UL_FX_DEFAULT, 0, 0);
			}
			
			ulEndDrawing();
			
			ulSyncFrame();
		}
		
		info._width = w;
		info._height = h;
		info._nBombs = b;
		info._size = s;
		info._type = 0;
		
		ulDeleteImage(slid);
		ulDeleteImage(arrow);
		ulDeleteImage(space);
		ulDeleteImage(startbt);
		ulDeleteImage(diff1);
		ulDeleteImage(diff2);
		ulDeleteImage(diff3);
		
	}
	
	ulDeleteImage(minesTitle);
}

u16 Menu::touchTest(u16 x1, u16 y1, u16 x2, u16 y2) {
     
	return (ul_keys.touch.x > x1 && ul_keys.touch.x < x2 && ul_keys.touch.y > y1 && ul_keys.touch.y < y2);
     
}

u16 Menu::checkBomb(u16 w, u16 h, u16 b) {
	u16 minBomb = (u16) floor(w*h*0.05);
	u16 maxBomb = (u16) floor(w*h*0.7);
	if(maxBomb > w*h-9) maxBomb = w*h-9;
	if(b < minBomb) b = minBomb;
	else if(b > maxBomb) b = maxBomb;
	return b;
}
