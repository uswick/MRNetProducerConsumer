/****************************************************************************
 * Copyright � 2003-2012 Dorian C. Arnold, Philip C. Roth, Barton P. Miller *
 *                  Detailed MRNet usage rights in "LICENSE" file.          *
 ****************************************************************************/

#if !defined(integer_iterator_h )
#define integer_iterator_h 1

#include "mrnet/Types.h"
#include <unistd.h>
#include <mrnet/Stream.h>
#include <pthread.h>
#include "AtomicSyncPrimitives.h"

using namespace MRN;
using namespace std;
using namespace atomiccontrols;

namespace mrnstreaming {
    class GenericIterator {
        // Records whether this object opened the file on its own (in which case it needs to close it)
        // or was given a ready FILE* stream
        bool finished;
    public:
        GenericIterator():finished(false){};

        // Functions implemented by children of this class that specialize it to take input from various sources.
    public:
        virtual std::vector<int> next() = 0 ;

    };


    //producer and consumer write to this shared object
    class MRNetQueueIterator : public GenericIterator{
        std::vector<int>* inputQueue;
        atomic_cond_t* inQueueSignal ;

        AtomicSync* synchronizer ;
        //this is needed to synchronize reads from inputQueue
        atomic_mutex_t* inQueueMutex;
    public:
        int total_ints;
    public:
        MRNetQueueIterator(std::vector<int>& input, atomic_cond_t* cond, atomic_mutex_t* inQueueMutex, AtomicSync* s)
        :total_ints(0){
            inputQueue = &input;
            inQueueSignal = cond ;
            this->inQueueMutex = inQueueMutex;
            this->synchronizer = s;
        }
        std::vector<int> next();
        bool end();
    } ;


    class GenericMerger{
    protected:
        std::vector<int>* outputQueue;
        AtomicSync* synchronizer ;
        atomic_cond_t* outSignal;
        atomic_mutex_t* outQueueMutex;

        void handleOutput(int *sum_ar);
    public:
        virtual std::vector<int>* getOutputQueue() = 0;
        virtual void merge(std::vector<MRNetQueueIterator> parsers) = 0;
    } ;
    //class responsible for merging data and producing an output
    class MRNetMerger:public GenericMerger{

    public:
        MRNetMerger(std::vector<int>* output, atomic_cond_t* outSignal, atomic_mutex_t* outQueueMutex, AtomicSync* s){
            outputQueue = output;
            this->outSignal = outSignal;
            this->outQueueMutex = outQueueMutex;
            this->synchronizer = s ;
        }
        void merge(std::vector<MRNetQueueIterator> parsers);

        std::vector<int>* getOutputQueue(){
            return outputQueue;
        }

    }   ;

    //
    class MRNetProducer{
        //stores input data
        std::map<Rank, std::vector<int>*> bufferData;
        std::map<Rank, int> requestsByRank;
        AtomicSync* synchronizer ;

        //sync primitives
        std::map<Rank, atomic_cond_t*> inQueueSignals;
        atomic_mutex_t* inQueueMutex;

        //MRnet specific params
        Stream* stream;
        Network *net;
        std::set<Rank> peers;

    public:
        MRNetProducer(std::map<Rank, std::vector<int>*> bufferData , atomic_mutex_t* inQueueMutex, std::map<Rank, atomic_cond_t*> inQueueSignals,
                std::set<Rank> peers, Stream* stream, Network *net, AtomicSync* s){
            this->bufferData = bufferData;
            this->inQueueMutex = inQueueMutex;
            this->inQueueSignals = inQueueSignals;
            this->stream = stream;
            this->net = net;
            this->peers = peers ;

            this->synchronizer = s ;
//            no_of_ranks = peers.size();
            for(std::set<Rank>::iterator it = peers.begin() ; it != peers.end() ; it++){
                requestsByRank[(*it)] = 0 ;
            }
        }
        void loadBuffer(std::vector< PacketPtr > &packets_in, const TopologyLocalInfo& info);
        bool getWaitCondition();
        bool be_node;
    }   ;

    class MRNetThread {
        std::map<Rank, atomic_cond_t*> inputSignals ;
        std::map<Rank, std::vector<int>*> bufferData;
        pthread_t* thread1 ;
        GenericMerger* merger ;
        std::vector<MRNetQueueIterator> iterators;

        AtomicSync* synchronizer ;
        atomic_cond_t *out_signal;
        atomic_mutex_t* inQueueMutex;
        atomic_mutex_t* outQueueMutex;

        bool initialized;

    public:
        MRNetThread(AtomicSync* s):initialized(false){
            this->synchronizer = s ;
        }

        void init(std::set<Rank> ranks);
        void destroy();
        void consumerFunc();
        void prodFunc();
        static void* consumerFuncHelper(void *arg);
        static void* producerFuncHelper(void *arg);

        std::map<Rank, std::vector<int>*> getInputBuffer(){
            return bufferData;
        };

        std::map<Rank, atomic_cond_t*> getPerRankSignals(){
            return inputSignals;
        };

        atomic_mutex_t* getInputMutex(){
            return inQueueMutex;
        }

        atomic_mutex_t* getOutputMutex(){
            return outQueueMutex;
        }

        atomic_cond_t* getOutSignal(){
            return out_signal;
        };

        std::vector<int>* getOutputQueue(){
            return merger->getOutputQueue();
        }

    } ;

}
#endif /* integer_addition_h */
