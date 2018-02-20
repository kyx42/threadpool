//
// Created by florian on 4/21/17.
//

#include "ThreadPool.hpp"

void		ThreadPool::threadLoop() {
    std::mutex &mutex = getMutex();
    std::shared_ptr<Func> t;
    _stop = false;

    while (42) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            while (!getStatus() && empty())
                wait(lock);
            if (getStatus())
                break;
            t = getTask();
        }
        (*t)();
    }
}

void ThreadPool::Start(const unsigned int nb) {
    _stop = false;
    for (unsigned int i = 0; i < nb; ++i)
        _workers.emplace_back(std::thread([&]() { ThreadPool::threadLoop(); }));
}

ThreadPool::~ThreadPool()
{
    _stop = true;
    _cdtVar.notify_all();
    for(auto &i : _workers)
        i.join();
}

bool ThreadPool::getStatus() const {
    return _stop;
}

void ThreadPool::addTask(Func const &task) {
    {
        std::unique_lock<std::mutex> lock(_mutexQ);
        _Queue.push_back(std::make_shared<Func>(task));
    }
    _cdtVar.notify_one();
}

Worker::Worker(ThreadPool const &tp) : _pool(tp)  {
}

std::shared_ptr<Func> ThreadPool::getTask() {
    std::shared_ptr<Func> a = this->_Queue.front();
    this->_Queue.pop_front();
    return (a);
}

bool ThreadPool::empty() const
{
    return (this->_Queue.empty());
}

void ThreadPool::wait(std::unique_lock<std::mutex> &lock)
{
    this->_cdtVar.wait(lock);
}

std::mutex &ThreadPool::getMutex()
{
    return (this->_mutexQ);
}

ThreadPool::ThreadPool() : _stop(true) {
}