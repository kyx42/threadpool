//
// Created by florian on 4/21/17.
//

#include "ThreadPool.hpp"

template <typename ... T>
Answer<T ...>::Answer(std::string const &s, std::tuple<T ...> const & obj) {
    _s = s;
    _obj = obj;
}

template <typename ... T>
std::string const &Answer<T ...>::getString()
{
    return _s;
}

template <typename ... T>
std::tuple<T...> const &Answer<T ...>::getParam()
{
    return _obj;
}

template <typename ... T>
void		ThreadPool<T ...>::threadLoop() {
    std::mutex &mutex = getMutex();
    std::shared_ptr<Answer<T ...>> t;
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
        std::apply(_t[t->getString()], t->getParam());
    }
}

template <typename ... T>
void ThreadPool<T ...>::Start(const unsigned int nb, std::map<std::string, std::function<void(T ...)>> const &t) {
    _stop = false;
    for (unsigned int i = 0; i < nb; ++i)
        _workers.emplace_back(std::thread([&]() { ThreadPool<T ...>::threadLoop(); }));
    _t = t;
}

template <typename ... T>
ThreadPool<T ...>::~ThreadPool()
{
    _stop = true;
    _cdtVar.notify_all();
    for(auto &i : _workers)
        i.join();
}

template <typename ... T>
bool ThreadPool<T ...>::getStatus() const {
    return _stop;
}

template<typename ... T>
void ThreadPool<T ...>::addTask(std::string const &s, T... p) {
    {
        std::tuple<T ...> args = std::tuple<T...>(p...);
        std::unique_lock<std::mutex> lock(_mutexQ);
        _Queue.push_back(std::make_shared<Answer<T...>>(s, args));
    }
    _cdtVar.notify_one();
}

template<typename ... T>
Worker<T ...>::Worker(ThreadPool<T ...> const &tp) : _pool(tp)  {
}

template <typename ... T>
Worker<T ...>::~Worker() {}

template<typename ... T>
std::shared_ptr<Answer<T ...>>	 	ThreadPool<T...>::getTask(void) {
    std::shared_ptr<Answer<T ...>> a = this->_Queue.front();
    this->_Queue.pop_front();
    return (a);
}

template <typename ... T>
bool ThreadPool<T ...>::empty(void) const

{
    return (this->_Queue.empty());
}

template <typename ... T>
void ThreadPool<T ...>::wait(std::unique_lock<std::mutex> &lock)
{
    this->_cdtVar.wait(lock);
}

template <typename ... T>
std::mutex &ThreadPool<T ...>::getMutex(void)
{
    return (this->_mutexQ);
}