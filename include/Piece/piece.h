#ifndef PIECE_H
#define PIECE_H



#include <ipiece.h>

class Piece : public IPiece{

    int left,top,right,bottom;
	int id;

    public:
		Piece();
		Piece(int id);
        Piece(int id,int left, int top, int right, int bottom);
        void setLeft(int left) override;
		int getLeft() override;

		void setTop(int top) override;
		int getTop() override;

		void setBottom(int bottom) override;
		int getBottom() override;

		void setRight(int right) override;
		int getRight() override;

		void setId(int id) override;
		int getId() override;
		
};


#endif