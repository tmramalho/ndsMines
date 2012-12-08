=(

u16 campo::openSquare(u16 hp, u16 vp) {
	//se for menor q zero da a vlta, nao e preciso testar
	//if(hp >= _linhas || vp >= _colunas) return 0;
	
	square &target = (*_mat)[hp][vp];
	
	_debug[1] = target.getEstado();
	
	if(target.isOpen() == 1) return 0; //esta aberto nao acontece nada
	
	if((_action == 0) && target.getEstado()) return 0; //se estiver em bandeira ou ? nao acontece nada
	
	_discoveredBombs += target.onClick(_action);
	
	if(_action == 0) {
		u16 type = target.getType();
		
		if(type == 9) {
			
			return 1;
			
		}
		else {
			
			if(type == 0) {
				//expansão das casas em branco
				for(s16 i = -1 ; i < 2 ; i++) {
					for(s16 j = -1 ; j < 2 ; j++) {
					
						if(hp + i >= _linhas || vp + j >= _colunas) continue;
						if(hp + i < 0 || vp + j < 0) continue;
					
						openSquare(hp+i,vp+j);
					}
				}
			}
			
			_openSquares++;
		}
		
		if(_openSquares + _noBombas == _linhas * _colunas) return 2;
		
	}
	
	return 0;
	
}