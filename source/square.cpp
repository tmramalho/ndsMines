#include "square.h"

square::square() {
	_type = 0;
	_estado = 0;
	_open = 0;
}
square::~square() {
	//do nothing
}
void square::setBomb() {
	_type = 9;	
}
void square::incrementType() {
	if(_type < 8)
          _type++;	
}
u16 square::getType() {
	return _type;
}
u16 square::getEstado() {
	return _estado;
}
u16 square::isOpen() {
	return _open;
}
s16 square::onClick(u8 action) {
	switch(action) {
		case 0:
			_open = 1;//fica aberto
			break;
		case 1:
			if(_estado == 0) {
				_estado = 1;//bandeira
				return 1;
			}
			else if(_estado == 1) {
				_estado = 0;
				return -1;
			}
			break;
		case 2:
			if(_estado == 0)
				_estado = 2;//?
			else if(_estado == 2)
					_estado = 0;
			break;
	}
	
	return 0;	
}
