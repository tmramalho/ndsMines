#ifndef _MATRIZ_H
#define _MATRIZ_H

#include "square.h"

class linha {
	public:
		~linha();
		void alocarLinha(u8 j);
		square &operator [](u8 j); //retorna referencia podermos modificar o valor
	
	private:
		square *vec;
};

class matriz {
	public:
		matriz(u8 n_lin, u8 n_col);
		~matriz();
		linha &operator [](u8 i); //retorna referencia para para aceder a linha
	
	private:
		u8 lin,col;
		linha *mat;
};
#endif
