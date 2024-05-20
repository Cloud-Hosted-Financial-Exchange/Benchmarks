#include <iostream>
#include <thread>
#include <queue>
#include <vector>
#include <algorithm>
#include <random> 

#include "pqueue.h"
#include "simplepq.h"
#include "fancypq.h"

uint64_t get_time() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

template<typename PriorityQueue>
void enqueue(PriorityQueue* pq, std::vector<PQ_ITEM>& items) {
    for (int i = 0; i < TOTAL_ITEMS; i++) {
        pq->push(items[i]);
        for (int j = 0; j < 10; j++) {}
    }
}

template<typename PriorityQueue>
void dequeue(PriorityQueue* pq) {
    while (pq->get_total_dequeues() < TOTAL_ITEMS) {
        pq->try_pop();
    }
}

std::vector<PQ_ITEM> create_items() {
    std::vector<PQ_ITEM> res;
    for (int i = 0; i < TOTAL_ITEMS; i++) {
        res.push_back(get_time());
    }

    std::random_device rd; // obtain a random number from hardware
    std::default_random_engine eng(rd()); // seed the generator

    int shuffle_range = 500;

    int i = 0;
    while (i < res.size()) {
        std::shuffle(res.begin()+i, res.begin()+std::min(i+shuffle_range, (int)res.size()-i), eng);
        i += shuffle_range;
    }
    
    return res;
}

int main() {
    std::vector<double> res;

    for (int epoch = 0; epoch < EXPERIMENT_ITERATIONS; epoch++) {
        // auto pq = new SimplePQ(SEQUENCER_DELAY);
        auto pq = new FancyPQ(SEQUENCER_DELAY, TOTAL_QS_FOR_FANCYPQ);

        std::vector<PQ_ITEM> items = create_items();

        auto start = std::chrono::high_resolution_clock::now();

        std::thread t1(enqueue<FancyPQ>, pq, std::ref(items));
        std::thread t2(dequeue<FancyPQ>, pq);
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