//
// Created by kyxo on 7/10/17.
//

#include <iostream>
#include <map>
#include "ThreadPool.hpp"


void my_1(int nb, float c) {
    std::cout << "1 " << nb << " " <<  c << std::endl;
}

void my_2(int nb, float c) {
    std::cout << "2 " << nb << " " << c << std::endl;
}

int main() {
    int i = 0;
    std::map<std::string, std::function<void(int, int)>> t; /* map of std::function */
    t["1"] = my_1;
    t["2"] = my_2;
    ThreadPool<int, int> tp; /* template with the function's parameters */
    tp.Start(4, t); /* Starting the tp with the Number of Workers and the map of std::function */
    while (++i < 1000)
        if (i % 2 == 0)
            tp.addTask("1", i, i); /* Adding Tasks */
        else
            tp.addTask("2", i, i);
    while (!tp.empty());  /*  Wait for all the tasks  */
    return (0);
}