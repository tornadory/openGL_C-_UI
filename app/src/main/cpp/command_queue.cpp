//
// Created by cheng on 2016/12/14.
//
#include "common.h"
#include "command_queue.h"

void CommandQueue::flush() noexcept
{

    std::list<std::unique_ptr<Command>> list;
    uint64_t i;

    synchronized(_lock,[&, this]
    {
        i=_sequence;

        list=std::move(_queue);
    });

    for(auto &r:list)
    {
        r->execute();
    }

    _fence=i;
    _signal.notifyOne();
}

bool CommandQueue::empty() const noexcept
{
    return _queue.empty();
}

void CommandQueue::waitFence(std::uint64_t i_fence) noexcept
{
    _signal.wait([=]{return i_fence<=_fence;});
}
