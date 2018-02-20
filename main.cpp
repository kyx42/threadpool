//
// Created by kyxo on 7/10/17.
//

#include "ThreadPool.hpp"

void my_1(int nb) {
    std::cout << "1 " << nb << std::endl;
}

void my_2(std::string const &c) {
    std::cout << "2 " << c << std::endl;
}

int main() {
    int i = 0;
    ThreadPool tp;
    tp.Start(4); /* Start the threadpool with the Number of Workers */
    while (++i < 1000)
        if (i % 2 == 0)
            tp.addTask([i] { my_1(i); }); /* Adding Tasks */
        else
            tp.addTask([i] { my_2(std::to_string(i)); }); /* Adding Tasks */
    while (!tp.empty());  /*  Wait for all the tasks  */
    return (0);
}