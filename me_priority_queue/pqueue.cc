#include <iostream>
#include <thread>
#include <queue>
#include <vector>
#include <algorithm>

#include "pqueue.h"
#include "simplepq.h"

uint64_t get_time() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

template<typename PriorityQueue>
void enqueue(PriorityQueue* pq) {
    for (int i = 0; i < TOTAL_ITEMS; i++) {
        pq->push(get_time());
    }
}

template<typename PriorityQueue>
void dequeue(PriorityQueue* pq) {
    while (pq->get_total_dequeues() < TOTAL_ITEMS) {
        pq->try_pop();
    }
}

int main() {
    std::vector<double> res;

    for (int epoch = 0; epoch < EXPERIMENT_ITERATIONS; epoch++) {
        auto simpelpq = new SimplePQ(SEQUENCER_DELAY);

        auto start = std::chrono::high_resolution_clock::now();

        std::thread t1(enqueue<SimplePQ>, simpelpq);
        std::thread t2(dequeue<SimplePQ>, simpelpq);
        t1.join();
        t2.join();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        res.push_back(duration/(1.0*MICRO));
        std::cout << res.back() << " seconds." << std::endl;
    }

    sort(res.begin(), res.end());
    int mid = res.size()/2;
    std::cout << "~Median: " << res[mid] << std::endl;

    return 0;
}