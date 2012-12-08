#include "campo.h"

campo::campo(u16 lins, u16 cols, u16 noBombas,u16 squareSize) {

	_scrollHor = 0;
	_scrollVer = 0;
	_linhas = lins;
	_colunas = cols;
	_gerado = false;
	_noBombas = noBombas;
	_openSquares = 0;
	_discoveredBombs = 0;
	_squareSize = squareSize;
	_action=0;
	_startTime = time(NULL);
	_endTime = 0;
	_victory = 0;
	_prevVic = false;
	_vicTimer = 0;

	memset(_debug, 0, sizeof(u16)*13);

	toPiece = ulLoadImageFilePNG((const char*)top, (int)top_size, UL_IN_VRAM, UL_PF_PAL8);
	piece = ulLoadImageFilePNG((const char*)pieces, (int)pieces_size, UL_IN_VRAM, UL_PF_PAL8);
	status = ulLoadImageFilePNG((const char*)stat, (int)stat_size, UL_IN_VRAM, UL_PF_PAL8);

	//alocar uma matriz
	_mat = new matriz(_linhas, _colunas);

}
campo::~campo() {
	bool a = false, b = false;

	while(!a || !b) {

		ulStartDrawing2D();

		if(ulGetMainLcd()) {
			ulDrawGradientRect(0, 0, MSW, MSH, RGB15(0, 0, 24), RGB15(0, 0, 24), RGB15(0, 16, 31), RGB15(0, 16, 31));
			a = true;
		} else {
			ulDrawGradientRect(0, 0, MSW, MSH, RGB15(0, 0, 24), RGB15(0, 0, 24), RGB15(0, 16, 31), RGB15(0, 16, 31));
			b = true;
		}

		ulEndDrawing();

		ulSyncFrame();

	}

	if(_victory) ulDeleteImage(winTitle);
	ulDeleteImage(toPiece);
	ulDeleteImage(piece);
	ulDeleteImage(status);
	delete _mat;//endereco da matriz
}

void campo::play() {

	u16 tempAction = 1;

	_soundChan = AS_SoundQuickPlay(intro_snd);

	while (1) {
		ulReadKeys(0);

		if(ul_keys.touch.click && !_victory) {
			onClick(ul_keys.touch.x, ul_keys.touch.y);
		}
		if(ul_keys.held.right || ul_keys.held.A) {
			scroll(1,0);
		}
		else if(ul_keys.held.left || ul_keys.held.Y) {
			scroll(-1,0);
		}
		if(ul_keys.held.up || ul_keys.held.X) {
			scroll(0,-1);
		}
		else if(ul_keys.held.down || ul_keys.held.B) {
			scroll(0,1);
		}

		if(ul_keys.held.R || ul_keys.held.L) {
			setAction(tempAction);
		} else setAction(0);

		if(ul_keys.pressed.select) {
			if(tempAction == 1) tempAction = 2;
			else tempAction = 1;
		}
		//alocar so uma das imagens
		if(_prevVic == false && _victory != 0) {
			if(_victory == 2) winTitle = ulLoadImageFilePNG((const char*)win, (int)win_size, UL_IN_VRAM, UL_PF_5551);
			else winTitle = ulLoadImageFilePNG((const char*)boom, (int)boom_size, UL_IN_VRAM, UL_PF_5551);
			_prevVic = true;
			if(_victory == 1) {
				AS_SoundQuickPlay(boom_snd);
				showBombs();
			}
		}

		if(_victory && _vicTimer < 3) _vicTimer++;

		if(ul_keys.pressed.start || (ul_keys.touch.click && _vicTimer > 2 && _victory)) {

			break;
		}

		draw();

		ulSyncFrame();
	}
}

void campo::geraCampo(u16 hp,u16 vp) {

	u16 maxSize = _linhas * _colunas;
	u16 list[maxSize];

	memset(list, 0, sizeof(u16)*maxSize); //memoria a zeros

	/*
	explicar o algoritmo
	dar a volta somando um primo a posicao
	o primo escolhido e 787,
	maior que o numero maximo de posicoes, 768
	*/

	//eliminar o q s carregou e os q tao a volta

	for(s8 i = -1 ; i < 2 ; i++) {
		for(s8 j = -1 ; j < 2 ; j++) {
			//se for menor q zero da a vlta, nao e preciso testar
			if(hp + i >= _linhas || vp + j >= _colunas) continue;
			if(hp + i < 0 || vp + j < 0) continue;
			list[getPos(hp + i, vp + j)] = 1;
		}
	}

	srand ( time(NULL) + hp + vp );

	for(u16 i = 0 ; i < _noBombas ; i++) {

		u16 pos = (u16)rand() % maxSize;

		while(1) {

			if(list[pos]) pos = (pos + PRIME) % maxSize;
			else break;

		}

		list[pos] = 1;

		u16 vrp = getvPosInv(pos);
		u16 hrp = gethPosInv(pos);

		(*_mat)[hrp][vrp].setBomb();

		for(s8 i = -1 ; i < 2 ; i++) {
			for(s8 j = -1 ; j < 2 ; j++) {
				//se for menor q zero da a vlta, nao e preciso testar
				if(hrp + i >= _linhas || vrp + j >= _colunas) continue;
				if(hrp + i < 0 || vrp + j < 0) continue;
				(*_mat)[hrp+i][vrp+j].incrementType();
			}
		}

	}

}

void campo::onClick(u16 x, u16 y) {

	u16 desvx = 0;

	u16 desvy = 0;

	if(_linhas < 16 ) desvx = (16 - _linhas) *_squareSize / 2;

	if(_colunas < 12 ) desvy = (12 - _colunas) *_squareSize / 2;

	u16 hp = ((x - desvx) / _squareSize) + _scrollHor;
	u16 vp = ((y - desvy) / _squareSize) + _scrollVer;

	/*_debug[10] = hp;
	_debug[11] = vp;
	_debug[12] = getPos(hp,vp);*/

	if(!_gerado && _action == 0 ) {

		//AS_SoundStop(_soundChan);
		//_soundChan = AS_SoundQuickPlay(start_snd);

		geraCampo(hp,vp);
		_gerado = true;
	}

	_victory = openSquare(hp, vp);

}

void campo::scroll(s8 hor,s8 ver) {

	u16 linhasHor = MSW / _squareSize;
	u16 linhasVer = MSH / _squareSize;

	if(_scrollHor+linhasHor+hor <= _linhas && _scrollHor+hor >= 0)
		_scrollHor += hor;

	if(_scrollVer+linhasVer+ver <= _colunas && _scrollVer+ver >= 0)
		_scrollVer += ver;

}

void campo::draw() {

	u16 xPosicao;
	u16 yPosicao;
	ulStartDrawing2D();

	if(ulGetMainLcd()) {
		//desenhar o background
		ulDrawGradientRect(0, 0, MSW, MSH, RGB15(0, 0, 24), RGB15(0, 0, 24), RGB15(0, 16, 31), RGB15(0, 16, 31));

		u16 desvx = 0;

		u16 desvy = 0;

		if(_linhas < 16 ) desvx = (16 - _linhas) *_squareSize / 2;

		if(_colunas < 12 ) desvy = (12 - _colunas) *_squareSize / 2;


		for(u16 i=0;i<_linhas;i++) {
			for(u16 j=0;j<_colunas;j++) {

				xPosicao = (i - _scrollHor)*_squareSize + desvx;
				yPosicao = (j - _scrollVer)*_squareSize + desvy;

				if((*_mat)[i][j].isOpen() == 1) {
					piece->x = xPosicao;
					piece->y = yPosicao;
					//desenhar so se for visivel
					if(piece->x >= 0 && piece->y >= 0 && piece->x < MSW && piece->y < MSH) {
						ulSetImageTileSize(piece,
							0,
							_squareSize*(*_mat)[i][j].getType(),
							_squareSize,
							_squareSize);
						ulDrawImage(piece);
					}
				}
				else {
					toPiece->x = xPosicao;
					toPiece->y = yPosicao;
						//desenhar so se for visivel
					if(toPiece->x >= 0 && toPiece->y >= 0 && toPiece->x < MSW && toPiece->y < MSH) {
						ulSetImageTileSize(toPiece,
							0,
							_squareSize*(*_mat)[i][j].getEstado(),
							_squareSize,
							_squareSize);
						ulDrawImage(toPiece);
					}
				}
			}
		}
	} else {

		u16 xplus = (32 - _linhas) / 2 * 8;
		u16 xc = (32 - _linhas) % 2 * 4;

		u16 yplus = (24 - _colunas) / 2 * 8;
		u16 yc = (24 - _colunas) % 2 * 4;

		u16 offx = _scrollHor * _squareSize + (xplus + xc) * 2;
		u16 offy = _scrollVer * _squareSize + (yplus + yc) * 2;

		//para quando o campo e menor que o ecran de baixo, nao quero desenhar o quadrado branco inteiro
		u16 screenWidth, screenHeight;

		if(_linhas < 16) screenWidth = _linhas * _squareSize;
		else screenWidth = MSW;
		if(_colunas < 12) screenHeight = _colunas * _squareSize;
		else screenHeight = MSH;

		//desenhar o background
		ulSetAlpha(UL_FX_DEFAULT, 0, 0);
		ulDrawGradientRect(0, 0, MSW, MSH, RGB15(0, 0, 24), RGB15(0, 0, 24), RGB15(0, 16, 31), RGB15(0, 16, 31));

		//muito transparente
		ulSetAlpha(UL_FX_ALPHA, 12, 1);
		ulDrawGradientRect((offx)/2,(offy)/2,(offx + screenWidth)/2,(offy + screenHeight)/2,RGB15(31, 31, 31),RGB15(31, 31, 31),RGB15(31, 31, 31),RGB15(31, 31, 31));

		//transparente
		ulSetAlpha(UL_FX_ALPHA, 16, 2);

		for(u16 i = 0;i<_linhas;i++) {
			for(u16 j = 0;j<_colunas;j++) {
				xPosicao = i*_squareSize/2;
				yPosicao = j*_squareSize/2;

				if((*_mat)[i][j].isOpen() == 1) {
					piece->x = xPosicao + xplus + xc;
					piece->y = yPosicao + yplus + yc;

					ulSetImageTileSize(piece,
						0,
						_squareSize*(*_mat)[i][j].getType(),
						_squareSize,
						_squareSize);

					piece->stretchX = 8;
					piece->stretchY = 8;

					ulDrawImage(piece);

				}
				else {
					toPiece->x = xPosicao + xplus + xc;
					toPiece->y = yPosicao + yplus + yc;

					ulSetImageTileSize(toPiece,
						0,
						_squareSize*(*_mat)[i][j].getEstado(),
						_squareSize,
						_squareSize);

					toPiece->stretchX = 8;
					toPiece->stretchY = 8;

					ulDrawImage(toPiece);
				}
			}
		}

		//pouco transparente
		ulSetAlpha(UL_FX_ALPHA, 22, 3);


		//action menu
		u16 menupos = MENUPOS;
		if( _linhas > 20 && _scrollVer >= 8 && _scrollHor < 10 ) menupos = 0;

		status->x = 10;
		status->y = menupos + 10;
		if(_action == 0) ulSetImageTileSize(status, 0, 0 + 48, _squareSize, _squareSize);
		else ulSetImageTileSize(status, 0, 0, _squareSize, _squareSize);
		ulDrawImage(status);

		status->x = 36;
		status->y = menupos + 10;
		if(_action == 1) ulSetImageTileSize(status, 0, 16 + 48, _squareSize, _squareSize);
		else ulSetImageTileSize(status, 0, 16, _squareSize, _squareSize);
		ulDrawImage(status);

		status->x = 62;
          	status->y = menupos + 10;
		if(_action == 2) ulSetImageTileSize(status, 0, 32 + 48, _squareSize, _squareSize);
		else ulSetImageTileSize(status, 0, 32, _squareSize, _squareSize);
		ulDrawImage(status);

		//opaco
		ulSetAlpha(UL_FX_DEFAULT, 0, 0);
		ulDrawRect((offx)/2,(offy)/2,(offx + screenWidth)/2,(offy + screenHeight)/2,RGB15(0, 0, 24));

		//debug info
		/*_debug[0] = ulGetTexVramAvailMemory();
		_debug[1] = ulGetTexVramUsedMemory();
		_debug[2] = ulGetTexPalAvailMemory();
		_debug[3] = ulGetTexPalUsedMemory();
		_debug[4] = ulGetVertexAvailMemory();
		_debug[5] = ulGetVertexUsedMemory();
		ulPrintf_xy(0, 180, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", _debug[0],_debug[1],_debug[2],_debug[3],_debug[4],_debug[5],_debug[6],_debug[7],_debug[8], _debug[9]);
		ulPrintf_xy(0,170,"%d",(int)time(NULL));*/

		ulPrintf_xy(3, 3, "%ld:%ld" , (time(NULL) - _startTime) / 60, (time(NULL) - _startTime) % 60);

		ulPrintf_xy(234, 3, "%d" , _noBombas - _discoveredBombs);

	}
	if(_victory && _vicTimer > 2) {
		if(ulGetMainLcd()) {
			ulSetAlpha(UL_FX_ALPHA, 22, 3);
			ulDrawGradientRect(64,48,192,144,RGB15(0, 0, 24), RGB15(0, 0, 24), RGB15(0, 16, 31), RGB15(0, 16, 31));
			ulDrawRect(64,48,192,144,RGB15(0, 0, 24));
			ulSetAlpha(UL_FX_DEFAULT, 0, 0);
			if(_victory == 1) {
				ulPrintf_xy(68, 52, "you lost the game");
			}
			else {
				ulPrintf_xy(68, 52, "you won the game");
				ulPrintf_xy(68, 62, "in %ld minutes", _endTime / 60);
				ulPrintf_xy(68, 72, "   %ld seconds", _endTime % 60);
			}

			ulPrintf_xy(68, 92, "touch to");
			ulPrintf_xy(68, 102, "return to menu");

		} else {
			//ulSetAlpha(UL_FX_ALPHA, 31, 4);
				//winTitle->stretchX = stretchRatio/100 * 256;
				//winTitle->stretchY = stretchRatio/100 * 192;
			ulSetAlpha(UL_FX_DEFAULT, 0, 0);
			winTitle->x = 0;
			winTitle->y = 0;
			ulDrawImage(winTitle);
				/*if(stat == 2) {
			ulPrintf_xy(0, 180, "vitoria");
		}
			else {
			ulPrintf_xy(0, 180, "es parvo");
		}*/
		}
	} else _endTime = (unsigned long) time(NULL) - (unsigned long) _startTime;

	ulEndDrawing();
}

void campo::setAction(u16 action) {

	_action = action;

}

u16 campo::getPos(u16 hp, u16 vp) {

	return vp * _linhas + hp;

}

u16 campo::gethPosInv(u16 pos) {

	return pos % _linhas;

}

u16 campo::getvPosInv(u16 pos) {

	return pos / _linhas;

}
void campo::showBombs() {
	for(u16 i=0; i<_linhas ; i++) {
		for(u16 j=0; j<_colunas; j++) {
			square &target = (*_mat)[i][j];
			if(target.getType() == 9) target.onClick(0);
		}
	}
}

u16 campo::openSquare(u16 hp, u16 vp) {

	if(hp >= _linhas || vp >= _colunas) return 0;

	square &target = (*_mat)[hp][vp];

	if(target.isOpen() == 1) return 0; //esta aberto nao acontece nada

	if((_action == 0) && target.getEstado() == 1) return 0; //se estiver em bandeira nao acontece nada
	if((_action == 0) && target.getEstado() == 2) {
		target.onClick(2); //?
		return 0;
	}

	_discoveredBombs += target.onClick(_action);


	if(_action == 0) {

		_openSquares++;

		u16 type = target.getType();

		if(type == 9) {

			return 1;

		}
		else if(type == 0){

			cena(hp,vp);

		}

		if(_openSquares + _noBombas == _linhas * _colunas) return 2;
	}

	return 0;
}


u16 campo::cena(u16 hp, u16 vp) {

	u16 pos = 0;

	u16 target[1024];

	square *aux;

	square *tmp;

	u16 hpos;

	u16 vpos;


	target[pos++] = hp;

	target[pos++] = vp;


	while(1) {

		//if(pos > _debug[9]) _debug[9] = pos;

		if(pos != 0) {

			vpos = target[--pos];

			hpos = target[--pos];

			aux = &(*_mat)[hpos][vpos];

		}
		else break;

		for(s8 i = -1 ; i < 2 ; i++){

			for(s8 j = -1 ; j < 2 ; j++){

				if(hpos + i >= _linhas || vpos + j >= _colunas) continue;
				if(hpos + i < 0 || vpos + j < 0) continue;

				tmp = &(*_mat)[hpos + i][ vpos +j ];

				if(tmp->isOpen()) continue;
				else {

					tmp->onClick(0);

					_openSquares++;

					if(tmp->getType() ==0) {

						target[pos++] = hpos + i;

						target[pos++] = vpos + j;

					}
				}
			}
		}
	}

	return 0;
}
