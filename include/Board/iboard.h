#ifndef IBOARD_H
#define IBOARD_H


#include <ipiece.h>





class IBoard{
    public:
		virtual void createBoard(int height, int length){}
		virtual int getLength(){return 0;}
		virtual int getHeight(){return 0;}
		virtual IPiece* getPiece(int y, int x){return nullptr;}
		virtual void setPiece(int y, int x, IPiece* piece){}
		virtual IBoard* copy(){return nullptr;}
		virtual void display(){}
		

};


#endif