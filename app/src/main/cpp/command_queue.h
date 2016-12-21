//
// Created by cheng on 2016/12/14.
//
#pragma once

#include "signal.h"

struct CommandQueue {

public:

struct Command
{
    virtual void execute()=0;
    virtual ~Command()
    {

    }
};
    template <class Task>
    std::uint64_t addTask(Task task)noexcept;

    void waitFence(std::uint64_t i_fence)noexcept;
    void flush()noexcept ;

    bool empty() const noexcept ;

private:

    template <class Task>
    struct TaskWrapper:Command
    {
    public :
        TaskWrapper(Task i_task)noexcept ;
        virtual void execute() override final  ;

    private:
        Task _task;

    };
    std::list<std::unique_ptr<Command>> _queue;
    std::mutex _lock;

    uint64_t  _fence=0;
    uint64_t _sequence=0;

    Signal _signal;

 };

template <class Task>
std::uint64_t CommandQueue::addTask(Task i_task) noexcept
{
    uint64_t  r;

    std::unique_ptr<Command> c=std::make_unique<TaskWrapper<Task>>(i_task);
    synchronized(_lock,[&]
    {
        r=++_sequence;
        _queue.push_back(std::move(c));
    });

    return r;
}

template <class Task>
CommandQueue::TaskWrapper<Task>::TaskWrapper(Task i_task)noexcept:_task(i_task)
{

}

template <class Task>
void CommandQueue::TaskWrapper<Task>::execute()
{
    _task();
}

