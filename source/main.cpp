#include <ulib/ulib.h>

#include <PA9.h>

#include <math.h>

#include "matriz.h"
#include "square.h"
#include "campo.h"
#include "menu.h"
#include "GameInfo.h"

int main(int argc, char ** argv) {

	PA_Init();
	PA_InitVBL();

	//Sound
	PA_VBLFunctionInit(AS_SoundVBL);
	AS_Init(AS_MODE_MP3 | AS_MODE_SURROUND | AS_MODE_16CH);
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND);

	ulInit(UL_INIT_ALL);
	ulInitGfx();
	ulInitText();

	GameInfo info;

	Menu menu;

	ulSetMainLcd(0);
	ulInitDualScreenMode();

	while(1) {

		menu.enterMenu(info);

		campo *game = new campo(info._width , info._height, info._nBombs, info._size);

		game->play();

		delete game;

	}

	return 0;
}
