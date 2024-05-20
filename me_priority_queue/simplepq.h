#include <queue>
#include "pqueue.h"

class SimplePQ {
    uint16_t d_s;  // sequencer delay

    uint64_t e;  // total enqueues
    uint64_t d;  // total dequeues

    std::mutex mtx;
    std::priority_queue<PQ_ITEM> pq;
public:
    SimplePQ(int d_s): d_s(d_s), e(0), d(0) {}

    void push(PQ_ITEM x) {
        std::lock_guard<std::mutex> lock(mtx);
        e++;
        pq.push(x);
    }

    bool try_pop() {
        std::lock_guard<std::mutex> lock(mtx);
        if (pq.empty()) return false;

        PQ_ITEM x = unsafe_top();
        if (get_time() - x < d_s) return false;

        d++;
        pq.pop();

        return true;
    }

    PQ_ITEM unsafe_top() {
        return pq.top();
    }

    int get_total_dequeues() {
        return d;
    }
};
