#include <piece.h>


Piece::Piece() : Piece(0) {
}


Piece::Piece(int id) : Piece(id,0,0,0,0){

}


Piece::Piece(int id,int left, int top, int right, int bottom){
	this->setId(id);
	this->setLeft(left);
	this->setTop(top);
	this->setRight(right);
	this->setBottom(bottom);

}


void Piece::setLeft(int left){
	this->left = left;
}


int Piece::getLeft(){
	return this->left;
}

void Piece::setTop(int top){
	this->top = top;
}


int Piece::getTop(){
	return this->top;
}

void Piece::setBottom(int bottom){
	this->bottom = bottom;
}

int Piece::getBottom(){
	return this->bottom;
}

void Piece::setRight(int right){
	this->right = right;
}


int Piece::getRight(){
	return this->right;
}

void Piece::setId(int id){
	this->id = id;
}

int Piece::getId(){
	return this->id;
}