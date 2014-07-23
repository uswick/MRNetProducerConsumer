/****************************************************************************
 * Copyright � 2003-2012 Dorian C. Arnold, Philip C. Roth, Barton P. Miller *
 *                  Detailed MRNet usage rights in "LICENSE" file.          *
 ****************************************************************************/

#include "MRNetProdConIterator.h"
#include "MRNetProdCon.h"

using namespace mrnstreaming;
using namespace MRN;


std::vector<int>  MRNetQueueIterator::next() {
    std::vector<int> temp;
    int num = 0;
    //wait for next 'TOTAL_PACKET_SIZE' number of integers from inputqueue
    //remove from shared queue  and return
    while (num < TOTAL_PACKET_SIZE && !end()) {
        //wait for signal from producer
#ifdef DEBUG_ON
        fprintf(stdout, "[MRNetQueueIterator #next().. PID : %d num : %d condition :  %p mutex : %p ]\n"
                , getpid(),num , inQueueSignal, inQueueMutex)  ;
#endif
        //todo handle this properly
        synchronizer->set_mutex_lock(inQueueMutex);
        synchronizer->set_cond_wait(inQueueSignal, inQueueMutex);

        //get iterator for incoming queue
        std::vector<int>::iterator it = inputQueue->begin();
        std::vector<int>::iterator del;

        for (; it != inputQueue->end(), num < TOTAL_PACKET_SIZE ;) {
            temp.push_back(*it);
            //remove integer from queue
            del = it;
            it = inputQueue->erase(del);
            num++;
        }
#ifdef DEBUG_ON
        printf("[MRNetQueueIterator read from incoming queue.. PID : %d [TEMP values ]  --> ", getpid());
        for(std::vector<int>::iterator itr = temp.begin() ; itr != temp.end() ; itr++){
            printf(" : [[ %d ]] " ,*itr);
        }
        printf("[   [END TEMP values ] \n ");
#endif
        synchronizer->set_mutex_unlock(inQueueMutex);
    }

    //update total integers recieved
    total_ints += num;
    return temp;
}

bool MRNetQueueIterator::end() {
    return total_ints >= TOTAL_STREAM_SIZE;
}

