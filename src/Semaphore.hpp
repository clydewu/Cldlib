
#ifndef Semaphore_h
#define Semaphore_h

#include <mutex>
#include <iostream>
#include <condition_variable>

using namespace std;

namespace cld {

    class Semaphore
    {
    public:
        Semaphore(unsigned int max): amount_(max) { }
        ~Semaphore() { }

        void Enter()
        {
            unique_lock<mutex> lock(mutex_);
            cond_.wait(lock, [&]()->bool{ return amount_ > 0;} );
            --amount_;
        }

        void Leave()
        {
            unique_lock<mutex> lock(mutex_);
            ++amount_;
            cond_.notify_all();
        }

    private:
        unsigned int amount_;
        mutex mutex_;
        condition_variable cond_;
        
    };

}

#endif /* Semaphore_h */
