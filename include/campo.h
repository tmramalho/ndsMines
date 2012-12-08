#ifndef _CAMPO_H
#define _CAMPO_H

#include "square.h"
#include "matriz.h"
#include <stdlib.h>
#include <time.h>
#include <PA9.h>

//imagens
#include "top.h"
#include "pieces.h"
#include "win.h"
#include "boom.h"
#include "stat.h"

//sons
#include "intro_snd.h"
#include "boom_snd.h"
#include "start_snd.h"

//height e width
#define MSW 256
#define MSH 192

#define PRIME 787

#define MENUPOS 156

class campo {
	public:
		campo(u16 lins, u16 cols, u16 noBombas, u16 squareSize);

		void play();

		~campo();

	private:

		void onClick(u16 x, u16 y);
		void scroll(s8 hor,s8 ver);
		void setAction(u16 action);
		void draw();
		void geraCampo(u16 x,u16 y);
		void showBombs();

		u16 getPos(u16 hp, u16 vp);
		u16 gethPosInv(u16 pos);
		u16 getvPosInv(u16 pos);
		u16 openSquare(u16 hp, u16 vp);
		u16 cena(u16 hp, u16 vp);

		u16 _victory;
		bool _prevVic;
		u16 _scrollHor;
		u16 _scrollVer;
		u16 _squareSize;
		u16 _linhas;
		u16 _colunas;
		bool _gerado;
		matriz *_mat;//pointer para matriz
		u16 _noBombas;
		u16 _openSquares;
		u16 _discoveredBombs;
		u16 _action;//0-abrir, 1-bandeira, 2-?
		u16 _debug[13];
		u16 _vicTimer;
		u8 _soundChan;
		time_t _startTime;
		unsigned long _endTime;

		UL_IMAGE *toPiece;
		UL_IMAGE *piece;
		UL_IMAGE *status;
		UL_IMAGE *winTitle;
};
#endif
