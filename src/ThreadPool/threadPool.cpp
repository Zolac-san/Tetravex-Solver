#include <threadPool.h>


ThreadPool::ThreadPool(int nbThread){
    finishPool = false;
    for (int i=0; i < nbThread; i++){
        this->threadWork.push_back(false);
        this->pool.push_back(std::thread(&ThreadPool::run,this,i));
        
    }

   
}



void ThreadPool::execute(int value1, int value2, IBoard* value3, std::function<void(int,int,IBoard*)> function){
    mtxEntry.lock();
    ToTreat tmp = {value1,value2,value3, function};
    entry.push_back(tmp);
    mtxEntry.unlock();
    cv.notify_one();
}


bool ThreadPool::haveTask(){
    for (int i =0 ; i < this->threadWork.size() ; i++){
        if(this->threadWork[i]) return true;
    }
    if((!this->entry.empty())) return true;
    
    return false;
}



void ThreadPool::run(int ind){
    
    while(!finishPool){
        threadWork[ind] = false;
        while(entry.empty() && !finishPool){  
            std::unique_lock<std::mutex> lock(mtx);
            this->cv.wait(lock);
        }
        threadWork[ind] = true;
        
        if(finishPool){
			threadWork[ind] = false;
			return;
		}

        

        mtxEntry.lock();
        if( !entry.empty()){
            ToTreat myTreat = entry[0];
            entry.erase(entry.begin());
            mtxEntry.unlock();


            myTreat.function(myTreat.value1,myTreat.value2,myTreat.value3);

        }else{
            mtxEntry.unlock();
        }
        

        
        //cv.notify_all();
    }
    


}

void ThreadPool::end(){
    finishPool =true;
    cv.notify_all();
    for (int i = 0; i < pool.size() ; i++){
        if (pool[i].joinable() ) pool[i].join();
    }
}