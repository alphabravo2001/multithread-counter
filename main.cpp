#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>

int max = 1000000;
int x;

void odd(std::atomic<int> &cur) {

    while (cur < max) {
        if (cur % 2 == 1) {
            cur.fetch_add(1);
        }
    }
}

void even(std::atomic<int> &cur) {

    while (cur < max) {
        if (cur % 2 == 0) {
            cur.fetch_add(1);
        }
    }
}

void inc(std::atomic<int> &cur) {

    while (cur < max) {
         x = cur.load();

         cur.compare_exchange_strong(x,x+1);
         //std::cout << cur << std::endl;
    }

}


int main() {

    int num_threads = 5;
    std::thread mythreads[num_threads];
    alignas(64) std::atomic<int> cur(0);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; ++i) {
        mythreads[i] = std::thread (inc,std::ref(cur));
    }

    auto end = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; ++i) {
        mythreads[i].join();
    }

    auto diff = end - start;

    std::cout<<diff.count()<<std::endl;


    // new test --------------------------------------------------------

    start = std::chrono::high_resolution_clock::now();

    std::thread e (even,std::ref(cur));
    std::thread o (odd,std::ref(cur));

    end = std::chrono::high_resolution_clock::now();

    diff = end - start;
    std::cout<<diff.count()<<std::endl;

    e.join();
    o.join();

}
