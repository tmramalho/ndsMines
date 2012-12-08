#ifndef _GAMEINFO_H
#define _GAMEINFO_H

class GameInfo {
	
	public:
		
		u16 _nBombs;
		
		u16 _height;
		
		u16 _width;
		
		u16 _size;
		
		u16 _type;
		
		GameInfo() : _nBombs(40), _height(16), _width(16), _size(16), _type(0) {}
		
};

#endif
