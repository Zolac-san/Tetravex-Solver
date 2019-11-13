#include <resolver.h>

Resolver::Resolver(){
	findSol = false;
	this->boardBase = nullptr;
	pool = new ThreadPool(5);
	allSoluceBoard.clear();
	this->onlyFirst = false;
}

void Resolver::load(std::string nameFile){
	std::ifstream file(nameFile, std::ios::in);
	if(file){
		std::string line;
		std::string oneNum;

		//Taille 
		std::getline(file,line);
		oneNum = "";
		int length = 0;
		int height = 0;
		for (unsigned int i = 0; i < line.length();i++){
			if(line[i] != ' ') oneNum += line[i];
			else{
				height = atoi(oneNum.c_str());
				oneNum = "";
			}
		}
		length = atoi(oneNum.c_str());

		boardBase = new Board(height,length);

		listPiece.clear();


		while(std::getline(file,line)){
			
			oneNum = "";
			std::vector<int> coord;
			for (unsigned int i = 0; i < line.length();i++){
				if(line[i] != ' ') oneNum += line[i];
				else{
					coord.push_back(atoi(oneNum.c_str()));
					oneNum = "";
				}
			}
			coord.push_back(atoi(oneNum.c_str()));
			
			IPiece* newPiece = new Piece(listPiece.size(),coord[0],coord[1],coord[2],coord[3]);
			listPiece.push_back(newPiece);
			
		}
		file.close();
	}else{
		std::cout<<"Impossible d'ouvrir le fichier"<<std::endl;
	}



		
	
}

std::vector<IBoard*> Resolver::resolveSequencielRec(bool onlyFirst){
	auto start = std::chrono::steady_clock::now();
	this->allSoluceBoard.clear();
	this->onlyFirst = onlyFirst;
	this->findSol = false;
	backtrackingSequentielInitRec();
	auto end = std::chrono::steady_clock::now();
	std::cout<<"Time Sequentiel Recursive  : "<<std::chrono::duration <double, std::milli> (end-start).count()<<" ms"<<std::endl;
	return this->allSoluceBoard;
}

std::vector<IBoard*> Resolver::resolveSequencielIt(bool onlyFirst){
	auto start = std::chrono::steady_clock::now();
	this->allSoluceBoard.clear();
	this->onlyFirst = onlyFirst;
	this->findSol = false;
	backtrackingSequentielIt();
	auto end = std::chrono::steady_clock::now();
	std::cout<<"Time Sequentiel Iteratif : "<<std::chrono::duration <double, std::milli> (end-start).count()<<" ms"<<std::endl;
	return this->allSoluceBoard;
}


std::vector<IBoard*> Resolver::resolveParrallele1(bool onlyFirst){
	auto start = std::chrono::steady_clock::now();
	this->allSoluceBoard.clear();
	this->onlyFirst = onlyFirst;
	this->findSol = false;
	backtrackingParralleleInit1();
	auto end = std::chrono::steady_clock::now();
	std::cout<<"Time Parrallele 1 : "<<std::chrono::duration <double, std::milli> (end-start).count()<<" ms"<<std::endl;
	return this->allSoluceBoard;
}





void Resolver::backtrackingSequentielInitRec(){
	
	findSol = false;

	long int ind = 0;
	for (int y = 0; y < boardBase->getHeight(); y++){
		for (int x = 0; x < boardBase->getLength(); x++){

			boardBase->setPiece(y,x,listPiece[ind]);
			ind++;
		}
	}
	
	//On change a cahque fois la premiere valeur
	long int max = this->boardBase->getLength() * this->boardBase->getHeight();
	for(long int i = 0; i < max; i++){
		//boardBase->display();
		int newX = i%this->boardBase->getLength();
		int newY = i/this->boardBase->getLength();

		IPiece* tmp = this->boardBase->getPiece(newY,newX);
		this->boardBase->setPiece(newY,newX,this->boardBase->getPiece(0,0));
		this->boardBase->setPiece(0,0,tmp);
		backtrackingSequentielRec(0,1);
		if(findSol) return;
	}
	
}

void Resolver::backtrackingSequentielRec(int y, int x){
	if(y == this->boardBase->getHeight()){
		//On a trouve une sol
		this->allSoluceBoard.push_back(this->boardBase->copy());
		if(this->onlyFirst) findSol = true;
		return;
	}

	//boardBase->display();
	long int max = this->boardBase->getLength() * this->boardBase->getHeight();
	long int start = y * this->boardBase->getLength() + x;

	int nextX = x+1;
	int nextY = y;
	if(nextX == this->boardBase->getLength()){
		nextY++;
		nextX = 0;
	}
	
	

	for (long int i = start; i < max; i++){
		// on essaye de voir si la celuel en haut et a droite sont bonne
		int newX = i%this->boardBase->getLength();
		int newY = i/this->boardBase->getLength();
		IPiece* thePiece = this->boardBase->getPiece(newY,newX);
		bool good = true;
		if( x > 0 &&  thePiece->getLeft() != this->boardBase->getPiece(y,x-1)->getRight()){
			good = false;
		}
		if( y > 0 &&  thePiece->getTop() != this->boardBase->getPiece(y-1,x)->getBottom()){
			good = false;
		}

		if (good){
			//On switch la piece et on passe a la suite

			

			IPiece* memPiece = boardBase->getPiece(y,x);
			boardBase->setPiece(y,x,thePiece);
			boardBase->setPiece(newY,newX,memPiece);

			backtrackingSequentielRec(nextY,nextX);

			//si bon faire un truc
			if(findSol) return;

			//reset
			boardBase->setPiece(y,x,memPiece);
			boardBase->setPiece(newY,newX,thePiece);

		}
	}


}

void Resolver::backtrackingSequentielIt(){
	findSol = false;
	
	long int ind = 0;
	for (int y = 0; y < boardBase->getHeight(); y++){
		for (int x = 0; x < boardBase->getLength(); x++){

			boardBase->setPiece(y,x,listPiece[ind]);
			ind++;
		}
	}
	

	

	//On change a cahque fois la premiere valeur
	long int max = this->boardBase->getLength() * this->boardBase->getHeight();
	

	std::vector<long int> stockLastEtape;
	stockLastEtape.push_back(0);

	long int x = 0;
	long int y = 0;

	while(!findSol){
		//this->boardBase->display();
		long int start = stockLastEtape.back();

		if(start == max){
			stockLastEtape.pop_back();
			long int mem = stockLastEtape.at((long int)stockLastEtape.size()-1);
			x--;
			if(x == -1){
				x = this->boardBase->getLength()-1;
				y--;
			}
			
			int memX = mem%this->boardBase->getLength();
			int memY = mem/this->boardBase->getLength();


			IPiece* tmpPiece = boardBase->getPiece(y,x);
			boardBase->setPiece(y,x,boardBase->getPiece(memY,memX));
			boardBase->setPiece(memY,memX,tmpPiece);

			stockLastEtape[(long int)stockLastEtape.size()-1]++;
		}else{
			//std::this_thread::sleep_for(std::chrono::seconds(1));
			for(long int i = start;i < max;i++){
				
				int newX = i%this->boardBase->getLength();
				int newY = i/this->boardBase->getLength();
				IPiece* thePiece = this->boardBase->getPiece(newY,newX);
				bool good = true;
				if( x > 0 &&  thePiece->getLeft() != this->boardBase->getPiece(y,x-1)->getRight()){
					good = false;
				}
				if( y > 0 &&  thePiece->getTop() != this->boardBase->getPiece(y-1,x)->getBottom()){
					good = false;
				}

				if (good){
					//On switch la piece et on passe a la suite
				
					stockLastEtape.at((long int)stockLastEtape.size()-1) = i;

					IPiece* memPiece = boardBase->getPiece(y,x);
					boardBase->setPiece(y,x,thePiece);
					boardBase->setPiece(newY,newX,memPiece);

					x++;
					if(x == this->boardBase->getLength()){
						x = 0;
						y++;
					}


					if(y == this->boardBase->getHeight()){
						//On a trouve une sol
						this->allSoluceBoard.push_back(boardBase->copy());
						if(this->onlyFirst) findSol = true;
						return;
					}

					stockLastEtape.push_back(y*this->boardBase->getLength()+x);
					break;
				

				}

				if(i == max-1){
					//reset
					stockLastEtape.pop_back();
					long int mem = stockLastEtape.at((long int)stockLastEtape.size()-1);
					x--;
					if(x == -1){
						x = this->boardBase->getLength()-1;
						y--;
					}
					
					int memX = mem%this->boardBase->getLength();
					int memY = mem/this->boardBase->getLength();


					IPiece* tmpPiece = boardBase->getPiece(y,x);
					boardBase->setPiece(y,x,boardBase->getPiece(memY,memX));
					boardBase->setPiece(memY,memX,tmpPiece);

					stockLastEtape[(long int)stockLastEtape.size()-1]++;


				}

			}
		}
	}



}


void Resolver::backtrackingParralleleInit1(){
	findSol = false;
	allSoluceBoard.clear();
	
	long int ind = 0;
	for (int y = 0; y < boardBase->getHeight(); y++){
		for (int x = 0; x < boardBase->getLength(); x++){

			boardBase->setPiece(y,x,listPiece[ind]);
			ind++;
		}
	}
	
	

	//On change a cahque fois la premiere valeur puis on lance le thread qui gere la suite
	long int max = this->boardBase->getLength() * this->boardBase->getHeight();
	for(long int i = 0; i < max; i++){
		//boardBase->display();
		int newX = i%this->boardBase->getLength();
		int newY = i/this->boardBase->getLength();

		IPiece* tmp = this->boardBase->getPiece(newY,newX);
		this->boardBase->setPiece(newY,newX,this->boardBase->getPiece(0,0));
		this->boardBase->setPiece(0,0,tmp);
		
		//on copy le tableua et on lance donc il ya aura max thread
		pool->execute(0,1, this->boardBase->copy(), std::bind(&Resolver::backtrackingParrallele1,this, std::placeholders::_1,std::placeholders::_2, std::placeholders::_3));

		
		if(findSol) return;
	}
	
	while(pool->haveTask()) std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
std::mutex mhh;

void Resolver::backtrackingParrallele1(int y, int x,IBoard* board){
	
	
	if(y == board->getHeight()){
		//On a trouve une sol
		
		mtxAppend.lock();
		this->allSoluceBoard.push_back(board->copy());
		if(this->onlyFirst) this->findSol = true;
		mtxAppend.unlock();
		return;
	}
	
	//board->display();
	long int max = board->getLength() * board->getHeight();
	long int start = y * board->getLength() + x;

	int nextX = x+1;
	int nextY = y;
	if(nextX == board->getLength()){
		nextY++;
		nextX = 0;
	}
	
	
	
	
	for (long int i = start; i < max; i++){
		if(findSol) return;
		// on essaye de voir si la celuel en haut et a droite sont bonne
		int newX = i%board->getLength();
		int newY = i/board->getLength();
		
		
		
		IPiece* thePiece = board->getPiece(newY,newX);
		
		bool good = true;
		if( x > 0 &&  thePiece->getLeft() != board->getPiece(y,x-1)->getRight()){
			good = false;
		}
		if( y > 0 &&  thePiece->getTop() != board->getPiece(y-1,x)->getBottom()){
			good = false;
		}

		if (good){
			//On switch la piece et on passe a la suite

			

			IPiece* memPiece = board->getPiece(y,x);


			board->setPiece(y,x,thePiece);
			board->setPiece(newY,newX,memPiece);
			
			backtrackingParrallele1(nextY,nextX,board);

			//si bon faire un truc
			if(findSol) return;

			//reset
			
			board->setPiece(y,x,memPiece);
			board->setPiece(newY,newX,thePiece);
			
		}
	}

	
}










std::vector<IBoard*> Resolver::resolveParrallele2(bool onlyFirst){
	auto start = std::chrono::steady_clock::now();
	this->allSoluceBoard.clear();
	this->onlyFirst = onlyFirst;
	this->findSol  =false;
	backtrackingParralleleInit2();
	auto end = std::chrono::steady_clock::now();
	std::cout<<"Time Parrallele 2 : "<<std::chrono::duration <double, std::milli> (end-start).count()<<" ms"<<std::endl;
	return this->allSoluceBoard;
}



void Resolver::backtrackingParralleleInit2(){
	
	allSoluceBoard.clear();

	long int ind = 0;
	for (int y = 0; y < boardBase->getHeight(); y++){
		for (int x = 0; x < boardBase->getLength(); x++){

			boardBase->setPiece(y,x,listPiece[ind]);
			ind++;
		}
	}
	
	

	//On change a cahque fois la premiere valeur puis on lance le thread qui gere la suite
	long int max = this->boardBase->getLength() * this->boardBase->getHeight();
	for(long int i = 0; i < max; i++){
		//boardBase->display();
		int newX = i%this->boardBase->getLength();
		int newY = i/this->boardBase->getLength();

		IPiece* tmp = this->boardBase->getPiece(newY,newX);
		this->boardBase->setPiece(newY,newX,this->boardBase->getPiece(0,0));
		this->boardBase->setPiece(0,0,tmp);
		
		//on copy le tableua et on lance donc il ya aura max thread
		pool->execute(0,1, this->boardBase->copy(), std::bind(&Resolver::backtrackingParrallele2,this, std::placeholders::_1,std::placeholders::_2, std::placeholders::_3));

		
		if(findSol) return;
	}

	while(pool->haveTask())  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}


void Resolver::backtrackingParrallele2(int y, int x,IBoard* board){
	if(findSol) return;
	if(y == board->getHeight()){
		//On a trouve une sol
		mtxAppend.lock();
		this->allSoluceBoard.push_back(board->copy());
		if(this->onlyFirst) this->findSol = true;
		mtxAppend.unlock();
		return;
	}

	//board->display();
	long int max = board->getLength() * board->getHeight();
	long int start = y * board->getLength() + x;

	int nextX = x+1;
	int nextY = y;
	if(nextX == board->getLength()){
		nextY++;
		nextX = 0;
	}
	
	

	for (long int i = start; i < max; i++){
		if(findSol) return;
		// on essaye de voir si la celuel en haut et a droite sont bonne
		int newX = i%board->getLength();
		int newY = i/board->getLength();
		IPiece* thePiece = board->getPiece(newY,newX);
		bool good = true;
		if( x > 0 &&  thePiece->getLeft() != board->getPiece(y,x-1)->getRight()){
			good = false;
		}
		if( y > 0 &&  thePiece->getTop() != board->getPiece(y-1,x)->getBottom()){
			good = false;
		}

		if (good){
			//On switch la piece et on passe a la suite

			

			IPiece* memPiece = board->getPiece(y,x);
			board->setPiece(y,x,thePiece);
			board->setPiece(newY,newX,memPiece);

			//backtrackingParrallele1(nextY,nextX,board);
			pool->execute(nextY,nextX, board->copy(), std::bind(&Resolver::backtrackingParrallele2,this, std::placeholders::_1,std::placeholders::_2, std::placeholders::_3));
			//si bon faire un truc
			

			//reset
			board->setPiece(y,x,memPiece);
			board->setPiece(newY,newX,thePiece);

		}
	}


}






std::vector<IBoard*> Resolver::resolveParrallele3(bool onlyFirst){
	auto start = std::chrono::steady_clock::now();
	this->allSoluceBoard.clear();
	this->onlyFirst = onlyFirst;
	this->findSol  =false;
	backtrackingParralleleInit3();
	auto end = std::chrono::steady_clock::now();
	std::cout<<"Time Parrallele 3 : "<<std::chrono::duration <double, std::milli> (end-start).count()<<" ms"<<std::endl;
	return this->allSoluceBoard;
}



void Resolver::backtrackingParralleleInit3(){
	allSoluceBoard.clear();

	long int ind = 0;
	for (int y = 0; y < boardBase->getHeight(); y++){
		for (int x = 0; x < boardBase->getLength(); x++){

			boardBase->setPiece(y,x,listPiece[ind]);
			ind++;
		}
	}
	
	this->poolSpecial = new ThreadPoolSpecial(4,boardBase);

	//On change a cahque fois la premiere valeur puis on lance le thread qui gere la suite
	long int max = this->boardBase->getLength() * this->boardBase->getHeight();
	for(long int i = 0; i < max; i++){
		//boardBase->display();
		std::vector<int> tmp;
		tmp.push_back(i);

			
		//on copy le tableua et on lance donc il ya aura max thread
		poolSpecial->execute(tmp, std::bind(&Resolver::backtrackingParrallele3,this, std::placeholders::_1,std::placeholders::_2));

		
		if(findSol) return;
	}

	while(poolSpecial->haveTask())  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}


void Resolver::backtrackingParrallele3(std::vector<int> listOfmove, IBoard* board){
	
	if(findSol) return;

	for (unsigned int i = 0 ;i < listOfmove.size();i++){
		int firstX = i%board->getLength();
		int firstY = i/board->getLength();

		int secondX = listOfmove[i]%board->getLength();
		int secondY = listOfmove[i]/board->getLength();
		
		IPiece* tmp = board->getPiece(firstY,firstX);
		board->setPiece(firstY,firstX,board->getPiece(secondY,secondX));
		board->setPiece(secondY,secondX,tmp);
	}


	int x = listOfmove.size()%board->getLength();
	int y = listOfmove.size()/board->getLength();

	if(y == board->getHeight()){
		//On a trouve une sol
		mtxAppend.lock();
		this->allSoluceBoard.push_back(board->copy());
		if(this->onlyFirst) this->findSol = true;
		mtxAppend.unlock();
		return;
	}

	//board->display();
	long int max = board->getLength() * board->getHeight();
	long int start = y * board->getLength() + x;

	int nextX = x+1;
	int nextY = y;
	if(nextX == board->getLength()){
		nextY++;
		nextX = 0;
	}
	


	for (long int i = start; i < max; i++){
		if(findSol) return;
		// on essaye de voir si la celuel en haut et a droite sont bonne
		int newX = i%board->getLength();
		int newY = i/board->getLength();

		IPiece* thePiece = board->getPiece(newY,newX);
		bool good = true;
		if( x > 0 &&  thePiece->getLeft() != board->getPiece(y,x-1)->getRight()){
			good = false;
		}
		if( y > 0 &&  thePiece->getTop() != board->getPiece(y-1,x)->getBottom()){
			good = false;
		}

		if (good){
			//On switch la piece et on passe a la suite

			

			IPiece* memPiece = board->getPiece(y,x);
			board->setPiece(y,x,thePiece);
			board->setPiece(newY,newX,memPiece);

			std::vector<int> nextVector;
			
			for(unsigned int k = 0; k < listOfmove.size();k++) nextVector.push_back(listOfmove[k]);
			
			nextVector.push_back(i);
			//backtrackingParrallele1(nextY,nextX,board);
			poolSpecial->execute(nextVector, std::bind(&Resolver::backtrackingParrallele3,this, std::placeholders::_1,std::placeholders::_2));
			//si bon faire un truc
			

			//reset
			board->setPiece(y,x,memPiece);
			board->setPiece(newY,newX,thePiece);

		}
	}
	
	for (int i = listOfmove.size()-1  ;i >= 0;i--){
		int firstX = i%board->getLength();
		int firstY = i/board->getLength();

		int secondX = listOfmove[i]%board->getLength();
		int secondY = listOfmove[i]/board->getLength();

	
		IPiece* tmp = board->getPiece(firstY,firstX);
		board->setPiece(firstY,firstX,board->getPiece(secondY,secondX));
		board->setPiece(secondY,secondX,tmp);
	}
	

}





std::vector<IBoard*> Resolver::resolveParrallele4Rec(bool onlyFirst){
	auto start = std::chrono::steady_clock::now();
	this->allSoluceBoard.clear();
	this->onlyFirst = onlyFirst;
	this->findSol  =false;
	backtrackingParralleleInit4Rec();
	auto end = std::chrono::steady_clock::now();
	std::cout<<"Time Parrallele 4 Recursive : "<<std::chrono::duration <double, std::milli> (end-start).count()<<" ms"<<std::endl;
	return this->allSoluceBoard;
}

std::vector<IBoard*> Resolver::resolveParrallele4It(bool onlyFirst){
	auto start = std::chrono::steady_clock::now();
	this->allSoluceBoard.clear();
	this->onlyFirst = onlyFirst;
	this->findSol  =false;
	backtrackingParralleleInit4It();
	auto end = std::chrono::steady_clock::now();
	std::cout<<"Time Parrallele 4 Iteratif : "<<std::chrono::duration <double, std::milli> (end-start).count()<<" ms"<<std::endl;
	return this->allSoluceBoard;
}



void Resolver::backtrackingParralleleInit4Rec(){
	
	allSoluceBoard.clear();

	long int ind = 0;
	for (int y = 0; y < boardBase->getHeight(); y++){
		for (int x = 0; x < boardBase->getLength(); x++){

			boardBase->setPiece(y,x,listPiece[ind]);
			ind++;
		}
	}
	
	this->poolSpecial2 = new ThreadPoolSpecial2(4,boardBase);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	//On change a cahque fois la premiere valeur puis on lance le thread qui gere la suite
	
	for (int y = 0; y < boardBase->getHeight(); y++){
		for (int x = 0; x < boardBase->getLength(); x++){


				
			//on copy le tableua et on lance donc il ya aura max thread
			poolSpecial2->execute(y,x, std::bind(&Resolver::backtrackingParrallele4StartRec,this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));

			
			if(findSol) return;
		}
	}

	while(poolSpecial2->haveTask())  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}


void Resolver::backtrackingParralleleInit4It(){
	
	allSoluceBoard.clear();

	long int ind = 0;
	for (int y = 0; y < boardBase->getHeight(); y++){
		for (int x = 0; x < boardBase->getLength(); x++){

			boardBase->setPiece(y,x,listPiece[ind]);
			ind++;
		}
	}
	
	this->poolSpecial2 = new ThreadPoolSpecial2(4,boardBase);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	//On change a cahque fois la premiere valeur puis on lance le thread qui gere la suite
	
	for (int y = 0; y < boardBase->getHeight(); y++){
		for (int x = 0; x < boardBase->getLength(); x++){


				
			//on copy le tableua et on lance donc il ya aura max thread
			poolSpecial2->execute(y,x, std::bind(&Resolver::backtrackingParrallele4StartIt,this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));

			
			if(findSol) return;
		}
	}

	while(poolSpecial2->haveTask())  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}




void Resolver::backtrackingParrallele4StartRec(int y, int x,IBoard* board){


	if(findSol) return;
	
	IPiece* tmp = board->getPiece(0,0);
	board->setPiece(0,0,board->getPiece(y,x));
	board->setPiece(y,x,tmp);
	backtrackingParrallele4Rec(0,1,board);
	board->setPiece(y,x,board->getPiece(0,0));
	board->setPiece(0,0,tmp);
}

void Resolver::backtrackingParrallele4StartIt(int y, int x,IBoard* board){
	if(findSol) return;
	IPiece* tmp = board->getPiece(0,0);
	board->setPiece(0,0,board->getPiece(y,x));
	board->setPiece(y,x,tmp);
	backtrackingParrallele4It(board);
	board->setPiece(y,x,board->getPiece(0,0));
	board->setPiece(0,0,tmp);
}



void Resolver::backtrackingParrallele4Rec(int y, int x,IBoard* board){
	

	if(findSol) return;
	


	
	if(y == board->getHeight()){
		//On a trouve une sol
		mtxAppend.lock();
		this->allSoluceBoard.push_back(board->copy());
		if(this->onlyFirst) this->findSol = true;
		mtxAppend.unlock();
		return;
	}

	//board->display();
	long int max = board->getLength() * board->getHeight();
	long int start = y * board->getLength() + x;

	int nextX = x+1;
	int nextY = y;
	if(nextX == board->getLength()){
		nextY++;
		nextX = 0;
	}
	
	

	for (long int i = start; i < max; i++){
		if(findSol) return;
		// on essaye de voir si la celuel en haut et a droite sont bonne
		
		int newX = i%board->getLength();
		int newY = i/board->getLength();

		IPiece* thePiece = board->getPiece(newY,newX);
		bool good = true;
		if( x > 0 &&  thePiece->getLeft() != board->getPiece(y,x-1)->getRight()){
			good = false;
		}
		if( y > 0 &&  thePiece->getTop() != board->getPiece(y-1,x)->getBottom()){
			good = false;
		}

		if (good){
			//On switch la piece et on passe a la suite

			

			IPiece* memPiece = board->getPiece(y,x);
			board->setPiece(y,x,thePiece);
			board->setPiece(newY,newX,memPiece);

			backtrackingParrallele4Rec(nextY,nextX,board);
			
			
			//si bon faire un truc
			

			//reset
			board->setPiece(y,x,memPiece);
			board->setPiece(newY,newX,thePiece);

		}
	}


}


void Resolver::backtrackingParrallele4It(IBoard* board){
	

	if(findSol) return;
	
	long int max = board->getLength() * board->getHeight();
	

	std::vector<long int> stockLastEtape;
	stockLastEtape.push_back(0);
	stockLastEtape.push_back(1);

	long int x = 1;
	long int y = 0;

	while(!findSol && stockLastEtape.size() > 1){
		long int start = stockLastEtape.back();

		if(start == max){
			stockLastEtape.pop_back();
			long int mem = stockLastEtape.at((long int)stockLastEtape.size()-1);
			x--;
			if(x == -1){
				x = this->boardBase->getLength()-1;
				y--;
			}
			
			int memX = mem%this->boardBase->getLength();
			int memY = mem/this->boardBase->getLength();


			IPiece* tmpPiece = boardBase->getPiece(y,x);
			boardBase->setPiece(y,x,boardBase->getPiece(memY,memX));
			boardBase->setPiece(memY,memX,tmpPiece);

			stockLastEtape[(long int)stockLastEtape.size()-1]++;
		}else{

			for(long int i = start;i < max;i++){
				int newX = i%board->getLength();
				int newY = i/board->getLength();

				IPiece* thePiece = board->getPiece(newY,newX);
				bool good = true;
				if( x > 0 &&  thePiece->getLeft() != board->getPiece(y,x-1)->getRight()){
					good = false;
				}
				if( y > 0 &&  thePiece->getTop() != board->getPiece(y-1,x)->getBottom()){
					good = false;
				}

				if (good){
					//On switch la piece et on passe a la suite

					stockLastEtape.at((long int)stockLastEtape.size()-1) = i;

					IPiece* memPiece = board->getPiece(y,x);
					board->setPiece(y,x,thePiece);
					board->setPiece(newY,newX,memPiece);

					x++;
					if(x == board->getLength()){
						x = 0;
						y++;
					}


					if(y == board->getHeight()){
						//On a trouve une sol
						this->allSoluceBoard.push_back(board->copy());
						if(this->onlyFirst) findSol = true;
						return;
					}

					stockLastEtape.push_back(y*board->getLength()+x);
					break;
					

				}

				if(i == max-1){
					//reset
					stockLastEtape.pop_back();
					long int mem = stockLastEtape.at((long int)stockLastEtape.size()-1);
					x--;
					if(x == -1){
						x = board->getLength()-1;
						y--;
					}

					int memX = mem%board->getLength();
					int memY = mem/board->getLength();


					IPiece* tmpPiece = board->getPiece(y,x);
					board->setPiece(y,x,board->getPiece(memY,memX));
					board->setPiece(memY,memX,tmpPiece);

					stockLastEtape[(long int)stockLastEtape.size()-1]++;


				}

			}
		}
	}


}