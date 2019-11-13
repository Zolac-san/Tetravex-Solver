#include <board.h>


Board::Board() : Board(3,3){
}

Board::Board(int height, int length){
    createBoard(height,length);
}

void Board::createBoard(int height, int length){
    this->height = height;
    this->length = length;

    for (int y = 0; y < height;y++){
		
        for (int x = 0 ; x < length; x++){
            this->board.push_back(nullptr);
        }
		
    }
}


int Board::getLength(){
	return this->length;
}


int Board::getHeight(){
	return this->height;
}

IPiece* Board::getPiece(int y, int x){
	long int ind = ((long int)y)*((long int)length)+ ((long int)x);
	return this->board[ind];
}

void Board::setPiece(int y, int x, IPiece* piece){
	long int ind = ((long int)y)*((long int)length)+ ((long int)x);
	this->board[ind] = piece;
}

IBoard* Board::copy(){
	IBoard* newBoard = new Board(this->getHeight(), this->getLength());
	for(int y = 0; y < this->getHeight() ; y++){
		for(int x = 0; x < this->getLength() ; x++){
			//Pas de copie de spiece car inutile , la position ets detreminer par le board , donc la piece ets juste la pour la data
			newBoard->setPiece(y,x,this->getPiece(y,x));
		}
	}
	return newBoard;
}



void Board::display(){
	//std::system("clear");
	for(int y = 0; y < this->getHeight() ; y++){
		std::string line1 = "";
		std::string line2 = "";
		std::string line3 = "";
		std::string line4 = "";
		for(int x = 0; x < this->getLength() ; x++){
			IPiece* tmp = getPiece(y,x);
			line1 += " "+std::to_string(tmp->getTop())+" |";
			line2 += std::to_string(tmp->getLeft())+" "+std::to_string(tmp->getRight())+"|";
			line3 += " "+std::to_string(tmp->getBottom())+" |";
			line4 += "----";
		}
		std::cout<<line1<<std::endl;
		std::cout<<line2<<std::endl;
		std::cout<<line3<<std::endl;
		std::cout<<line4<<std::endl;
	}
	std::cout<<std::endl;
	
	std::cout<<"fin display"<<std::endl;
}




