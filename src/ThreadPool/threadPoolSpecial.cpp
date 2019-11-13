#include <threadPoolSpecial.h>


ThreadPoolSpecial::ThreadPoolSpecial(int nbThread,IBoard* myBoard){
    finishPool = false;
    for (int i=0; i < nbThread; i++){
        this->threadWork.push_back(false);
        this->pool.push_back(std::thread(&ThreadPoolSpecial::run,this,i,myBoard->copy()));
        
    }

   
}



void ThreadPoolSpecial::execute(std::vector<int> value, std::function<void(std::vector<int>,IBoard*)> function){
    mtxEntry.lock();
    ToTreat tmp = {value, function};
    entry.push_back(tmp);
    mtxEntry.unlock();
    cv.notify_one();
}


bool ThreadPoolSpecial::haveTask(){
    for (int i =0 ; i < this->threadWork.size() ; i++){
        if(this->threadWork[i]) return true;
    }
    if((!this->entry.empty())) return true;
    
    return false;
}



void ThreadPoolSpecial::run(int ind, IBoard* board){
    
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

            myTreat.function(myTreat.value,board);

        }else{
            mtxEntry.unlock();
        }
        

        
        //cv.notify_all();
    }
    


}

void ThreadPoolSpecial::end(){
    finishPool =true;
    cv.notify_all();
    for (int i = 0; i < pool.size() ; i++){
        if (pool[i].joinable() ) pool[i].join();
    }
}