#include <threadPoolSpecial2.h>


ThreadPoolSpecial2::ThreadPoolSpecial2(int nbThread,IBoard* myBoard){
    finishPool = false;
    for (int i=0; i < nbThread; i++){
        this->threadWork.push_back(false);
        this->pool.push_back(std::thread(&ThreadPoolSpecial2::run,this,i,myBoard->copy()));
        
    }

   
}



void ThreadPoolSpecial2::execute(int value1, int value2, std::function<void(int,int,IBoard*)> function){
    mtxEntry.lock();
    ToTreat tmp = {value1,value2, function};
    entry.push_back(tmp);
    
    cv1.notify_one();
    mtxEntry.unlock();
    
}


bool ThreadPoolSpecial2::haveTask(){
    for (unsigned int i =0 ; i < this->threadWork.size() ; i++){
        if(this->threadWork[i]) return true;
    }
    if((!this->entry.empty())) return true;
    
    return false;
}



void ThreadPoolSpecial2::run(int ind, IBoard* board){
   
    
    while(!finishPool){
        threadWork[ind] = false;
        while(entry.empty() && !finishPool){ 
            std::unique_lock<std::mutex> lock(mtx);
            this->cv1.wait(lock);
        }
        threadWork[ind] = true;
        //cv1.notify_all();
        if(finishPool){
			threadWork[ind] = false;
			return;
		}

        

        mtxEntry.lock();
        
        if( !entry.empty()){
           
            ToTreat myTreat = entry[0];
            entry.erase(entry.begin());
            mtxEntry.unlock();

            myTreat.function(myTreat.value1,myTreat.value2,board);

        }else{
           
            mtxEntry.unlock();
        }
       

        
        //cv1.notify_all();
    }
  


}

void ThreadPoolSpecial2::end(){
    finishPool =true;
    cv1.notify_all();
    for (unsigned int i = 0; i < pool.size() ; i++){
        if (pool[i].joinable() ) pool[i].join();
    }
}