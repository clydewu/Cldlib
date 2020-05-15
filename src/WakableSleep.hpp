#ifndef WakableSleep_h
#define WakableSleep_h

#include <thread>
#include <mutex>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;
namespace cld
{
    
    class WakableSleep
    {
    public:
        WakableSleep()
        : thr_(nullptr)
        , wakeup_flag_(false)
        , seq_(0)
        {
        }
        
        
        ~WakableSleep()
        {
        };
        
        void sleep(boost::posix_time::time_duration& duration)
        {
            sleep(duration.total_microseconds());
        }
        
        
        void sleep(long long sleep_micro_sec)
        {
            if (sleep_micro_sec <= 0)
            {
                return;
            }
            
            {
                unique_lock<mutex> lock(cv_mux_);
                wakeup_flag_ = false;
            }
            
            {
                unique_lock<mutex> lock(seq_mux_);
                seq_++;
            }
            
            thr_ = new thread([&]() {
                int seq = 0;
                {
                    unique_lock<mutex> lock(seq_mux_);
                    seq = seq_;
                }
                
                this_thread::sleep_for(chrono::microseconds(sleep_micro_sec));
                
                {
                    unique_lock<mutex> lock(seq_mux_);
                    if (seq == seq_)
                    {
                        unique_lock<mutex> lock(cv_mux_);
                        wakeup_flag_ = true;
                        cv_.notify_one();
                    }
                }
            });
            thr_->detach();
            
            unique_lock<mutex> lock(cv_mux_);
            cv_.wait(lock, [&]()->bool{ return wakeup_flag_ == true;} );
            delete thr_;
        }
        
        
        void wake_up()
        {
            unique_lock<mutex> lock(cv_mux_);
            wakeup_flag_ = true;
            cv_.notify_one();
        }
        
        
    private:
        thread* thr_;
        mutex cv_mux_;
        condition_variable cv_;
        bool wakeup_flag_;
        
        mutex seq_mux_;
        unsigned int seq_;
    };
    
}
#endif /* WakableSleep_h */
