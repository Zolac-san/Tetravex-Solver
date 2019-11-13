#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <functional>
#include <iboard.h>
/* Test d'un threadpool non typé pour reutilisé plus tard*/

class ThreadPool{

    struct ToTreat{
        int value1;
        int value2;
        IBoard* value3;
        std::function<void(int,int,IBoard*)> function;
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
        ThreadPool(int nbThread);
        void execute(int value1, int value2,IBoard* value3, std::function<void(int,int,IBoard*)> function);
        bool haveTask();
        bool haveResult();
        //void run(int ind, bool* finishPool, std::mutex* mtx, std::condition_variable* cv, std::vector<ToTreat>* entry, std::vector<resultClass>* result,std::vector<bool>* threadWork,std::mutex* mtxEntry,std::mutex* mtxResult);
        void run(int ind);
        
        void end();  
};


#endif