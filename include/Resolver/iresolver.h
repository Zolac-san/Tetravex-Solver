#ifndef IRESOLVER_H
#define IRESOLVER_H

#include <string>
#include <iboard.h>
#include <vector>
class IResolver{

	public:
		virtual void load(std::string nameFile){}
		virtual std::vector<IBoard*> resolveSequencielRec(bool onlyFirst){return std::vector<IBoard*>();}
		virtual std::vector<IBoard*> resolveSequencielIt(bool onlyFirst){return std::vector<IBoard*>();}
		virtual std::vector<IBoard*> resolveParrallele1(bool onlyFirst){return std::vector<IBoard*>();}
		virtual std::vector<IBoard*> resolveParrallele2(bool onlyFirst){return std::vector<IBoard*>();}
		virtual std::vector<IBoard*> resolveParrallele3(bool onlyFirst){return std::vector<IBoard*>();}
		virtual std::vector<IBoard*> resolveParrallele4Rec(bool onlyFirst){return std::vector<IBoard*>();}
		virtual std::vector<IBoard*> resolveParrallele4It(bool onlyFirst){return std::vector<IBoard*>();}



};


#endif