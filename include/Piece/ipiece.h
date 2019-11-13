#ifndef IPIECE_H
#define IPIECE_H




class IPiece {
    public:
        virtual void setLeft(int left){};
		virtual int getLeft(){return 0;};

		virtual void setTop(int top){};
		virtual int getTop(){return 0;};

		virtual void setBottom(int bottom){};
		virtual int getBottom(){return 0;};

		virtual void setRight(int right){};
		virtual int getRight(){return 0;};

		virtual void setId(int id){};
		virtual int getId(){return 0;};
};


#endif