#ifndef RESOLVER_H
#define RESOLVER_H

#include <iostream>
#include <iresolver.h>
#include <board.h>
#include <piece.h>
#include <fstream>
#include <algorithm>
#include <threadPool.h>
#include <threadPoolSpecial.h>
#include <threadPoolSpecial2.h>
#include <functional>
#include <mutex>
#include <chrono>



class Resolver : public IResolver{
	//Declaration ici pour gain memoire

	std::mutex mtxAppend;

	IBoard* boardBase;
	std::vector<IPiece*> listPiece;
	std::vector<IBoard*> allSoluceBoard; // uniquement pour les fonction recursive
	bool onlyFirst;
	ThreadPool* pool;

	ThreadPoolSpecial* poolSpecial;
	ThreadPoolSpecial2* poolSpecial2;


	bool findSol;
	void backtrackingSequentielIt();
	void backtrackingSequentielInitRec();
	void backtrackingSequentielRec(int y, int x);

	void backtrackingParralleleInit1();
	void backtrackingParrallele1(int y,int x, IBoard* board);

	void backtrackingParralleleInit2();
	void backtrackingParrallele2(int y,int x, IBoard* board);

	void backtrackingParralleleInit3();
	void backtrackingParrallele3(std::vector<int> listOfmove, IBoard* board);

	void backtrackingParralleleInit4Rec();
	void backtrackingParralleleInit4It();
	void backtrackingParrallele4StartRec(int y,int x, IBoard* board);
	void backtrackingParrallele4StartIt(int y,int x, IBoard* board);
	void backtrackingParrallele4Rec(int y,int x, IBoard* board);
	void backtrackingParrallele4It(IBoard* board);



	public:
		Resolver();
		void load(std::string nameFile) override;
		std::vector<IBoard*> resolveSequencielRec(bool onlyFirst) override;
		std::vector<IBoard*> resolveSequencielIt(bool onlyFirst) override;
		std::vector<IBoard*> resolveParrallele1(bool onlyFirst) override;
		std::vector<IBoard*> resolveParrallele2(bool onlyFirst) override;
		std::vector<IBoard*> resolveParrallele3(bool onlyFirst) override;
		std::vector<IBoard*> resolveParrallele4Rec(bool onlyFirst) override;
		std::vector<IBoard*> resolveParrallele4It(bool onlyFirst) override;

};


#endif
