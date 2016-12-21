//
// Created by cheng on 2016/12/14.
//


#pragma  once

#include <condition_variable>

template <class Predict>
void synchronized(std::mutex &i_mutex,Predict && i_predict)
{
    std::unique_lock<std::mutex> lock(i_mutex);

    i_predict();
}

struct Signal
{
public:

    void wait()
    {
        std::unique_lock<std::mutex> lock(stateLock);
        stateEvent.wait(lock);
    }

    template <class Predict>
    void wait(Predict && i_predict)
    {
        std::unique_lock<std::mutex> lock(stateLock);
        stateEvent.wait(lock,std::forward<Predict>(i_predict)); //????? forward
    }

    void notifyOne()
    {
        stateEvent.notify_one();
    }

    void notifyAll()
    {
        stateEvent.notify_all();
    }
private:
    std::condition_variable stateEvent;
    std::mutex stateLock;
};