#ifndef SemaphoreContainer_hpp
#define SemaphoreContainer_hpp

#include <deque>
#include <mutex>
#include <condition_variable>

#include "Semaphore.hpp"

using namespace std;

namespace cld
{
    template <typename T>
    class SemaphoreContainer
    {
    public:
        typedef typename T::value_type value_type;
        typedef typename T::size_type size_type;
        typedef typename T::iterator iterator;

        
        SemaphoreContainer(unsigned int max_size)
        : SemaphoreContainer(max_size, 0)
        {
        }
        
        
        SemaphoreContainer(unsigned int max_size, unsigned int min_size)
        : in_semaphore_(new Semaphore(max_size))
        , out_semaphore_(new Semaphore(-min_size))
        , queue_max_size_(max_size)
        , queue_min_size_(min_size)
        {
        }

        
        ~SemaphoreContainer()
        {
            delete in_semaphore_;
            delete out_semaphore_;
        };
        
        
        void push(value_type elm)
        {
            in_semaphore_->Enter();
            {
                unique_lock<recursive_mutex> locker(mux_);
                queue_.push_back(elm);
            }
            out_semaphore_->Leave();
        };
        
        
        value_type pop()
        {
            value_type* elm = nullptr;
            out_semaphore_->Enter();
            {
                unique_lock<recursive_mutex> locker(mux_);
                elm = &queue_.front();
                queue_.pop_front();
            }
            in_semaphore_->Leave();
            return *elm;
        };
        

        void push_front(value_type elm)
        {
            in_semaphore_->Enter();
            {
                unique_lock<recursive_mutex> locker(mux_);
                queue_.push_front(elm);
            }
            out_semaphore_->Leave();
        }
        
        size_type size()
        {
            unique_lock<recursive_mutex> locker(mux_);
            return queue_.size();
        }

        
    private:
        T queue_;
        recursive_mutex mux_;

        unsigned int queue_max_size_;
        unsigned int queue_min_size_;
        
        Semaphore* in_semaphore_;
        Semaphore* out_semaphore_;
    };
    
}

#endif /* SemaphoreContainer_hpp */
