#ifndef BOARD_H
#define BOARD_H

#include <iboard.h>
#include <ipiece.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>


class Board : public IBoard{

    std::vector<IPiece*> board;

	int height, length;
    
    public:
        Board();
        Board(int height, int length);
		void createBoard(int height, int length) override;
        int getLength() override;
        int getHeight() override;
        IPiece* getPiece(int y, int x) override;
		
		void setPiece(int y, int x, IPiece* piece) override;
		IBoard* copy() override;
		void display() override;
        
};


#endif