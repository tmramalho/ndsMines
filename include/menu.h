#ifndef _MENU_H
#define _MENU_H

#include "square.h"
#include "matriz.h"
#include "GameInfo.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

//imagens
#include "mines.h"
#include "slider.h"
#include "seta.h"
#include "espaco.h"
#include "start.h"
#include "meh.h"
#include "easy.h"
#include "hard.h"

#define MSW 256
#define MSH 192

#define SQUARE 16
#define RECT 24
#define START 88
#define TWOTHIRDS 0.66666666

class Menu {
	
	private:
		
		u16 _state;
		
		u16 touchTest(u16 x1, u16 y1, u16 x2, u16 y2);

		u16 checkBomb(u16 w, u16 h, u16 b);
		
	public:
		
		Menu() : _state(0) {}
			
		void enterMenu(GameInfo &info);
			
};

#endif
