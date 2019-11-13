#ifndef THREADPOOLSPECIAL_H
#define THREADPOOLSPECIAL_H

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <functional>
#include <iboard.h>
/* Test d'un threadpool non typé pour reutilisé plus tard*/

class ThreadPoolSpecial{

    struct ToTreat{
        
        std::vector<int> value;
        std::function<void(std::vector<int>,IBoard*)> function;
    };

    std::vector<std::thread> pool;
    std::vector<bool> threadWork;

    std::vector<ToTreat> entry;

    std::mutex mtx;
    std::condition_variable cv;

    std::mutex mtxEntry;
    std::mutex mtxResult;

    bool finishPool;


    public:
        ThreadPoolSpecial(int nbThread, IBoard* myBoard);
        void execute( std::vector<int> value, std::function<void(std::vector<int>,IBoard*)> function);
        bool haveTask();
        bool haveResult();
        //void run(int ind, bool* finishPool, std::mutex* mtx, std::condition_variable* cv, std::vector<ToTreat>* entry, std::vector<resultClass>* result,std::vector<bool>* threadWork,std::mutex* mtxEntry,std::mutex* mtxResult);
        void run(int ind,IBoard* board);
        
        void end();  
};


#endif