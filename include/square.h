#ifndef _SQUARE_H
#define _SQUARE_H

#include <ulib/ulib.h>

class square {
	public:
		square();
		~square();
		void setBomb();
		void incrementType();
		s16 onClick(u8 action);//0=abrir,1=bandeira,2=? (o retorno incrementa/decrementa o numero de bandeiras)
		u16 getType();
		u16 getEstado();
		u16 isOpen();
	private:
		u16 _type;//9=bomba, (0-8)=numero de bombas
		u16 _estado;//0fechada,1bandeira,2?
		u16 _open;
};

#endif
