//
// Created by florian on 4/21/17.
//

#pragma once

#include <mutex>
#include <functional>
#include <condition_variable>
#include <deque>
#include <thread>
#include <iostream>
#include <tuple>

typedef std::function<void(void)> Func;

class Worker;

class ThreadPool {
public:

    explicit ThreadPool();

    ~ThreadPool();

    void Start(unsigned int);

    void addTask(Func const &task);

    bool getStatus() const;

    std::shared_ptr<Func> getTask();

    std::mutex &getMutex();

    void threadLoop();

    bool empty() const;

    void wait(std::unique_lock<std::mutex> &lock);

private:
    std::deque<std::shared_ptr<Func>> _Queue;
    std::condition_variable _cdtVar;
    std::mutex _mutexQ;
    std::deque<std::thread> _workers;
    bool _stop;
};

class Worker {
public:
    explicit Worker(ThreadPool const &tp);

    ~Worker() = default;

private:
    const ThreadPool &_pool;
};