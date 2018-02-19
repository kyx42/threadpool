//
// Created by florian on 4/21/17.
//

#pragma once

#include <mutex>
#include <condition_variable>
#include <functional>
#include <deque>
#include <thread>
#include <map>
#include <iostream>
#include <tuple>

template<typename ... T>
class Answer {
public:
    explicit Answer(std::string const &s, std::tuple<T ...> const &);

    Answer() = default;

    std::string const &getString();

    std::tuple<T ...> const &getParam();

private:
    std::string _s;
    std::tuple<T ...> _obj;
};

template<typename ... T>
class Worker;

template<typename ... T>
class ThreadPool {
public:

    explicit ThreadPool() = default;

    ~ThreadPool();

    void Start(const unsigned int, std::map<std::string, std::function<void(T ...)>> const &t);

    void addTask(std::string const &, T ...);

    bool getStatus() const;

    std::shared_ptr<Answer<T ...>> getTask(void);

    std::mutex &getMutex(void);

    void threadLoop();

    bool empty(void) const;

    void wait(std::unique_lock<std::mutex> &lock);

private:
    std::deque<std::shared_ptr<Answer<T ...>>> _Queue;
    std::map<std::string, std::function<void(T ...)>> _t;
    std::condition_variable _cdtVar;
    std::mutex _mutexQ;
    std::deque<std::thread> _workers;
    bool _stop;
};

template<typename ... T>
class Worker {
public:
    Worker(ThreadPool<T...> const &tp);

    ~Worker();

private:
    ThreadPool<T...> &_pool;
};

template class ThreadPool<int, int>;
template class ThreadPool<int, char>;