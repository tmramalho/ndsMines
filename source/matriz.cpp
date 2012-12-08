#include "matriz.h"

/***********************************
classe matriz
***********************************/

matriz::matriz(u8 n_lin, u8 n_col) {
	mat=new linha[n_lin];
	//if(!(mat)) exit(1); //erro
	//memset(mat,0, n_lin*sizeof(linha));
	for(u8 i=0; i < n_lin ;++i) 
		mat[i].alocarLinha(n_col);
	lin=n_lin;
	col=n_col;
}
matriz::~matriz() {
	delete[] mat;
}
//vai buscar a linha, para depois chamar o operador []
//segunda vez, que retorna o valor
linha &matriz::operator [](u8 i) {
	return mat[i];
}

/**************************************
classe linha
**************************************/

void linha::alocarLinha(u8 n_col) {
	vec = new square[n_col];
	//if(!(vec)) exit(1); //erro
	//memset(vec,0, n_col*sizeof(square));
}
linha::~linha() {
	delete[] vec;
}
square &linha::operator [](u8 j) {
	return vec[j];
}
