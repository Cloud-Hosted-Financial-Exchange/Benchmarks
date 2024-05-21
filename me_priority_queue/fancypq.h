#include <cstdlib>
#include <ctime>
#include <queue>
#include "pqueue.h"

class FancyPQ {
    uint32_t d_s;  // sequencer delay
    uint8_t total_qs;  // total qs

    uint64_t e;  // total enqueues
    uint64_t d;  // total dequeues

    std::vector<std::mutex> mtxs;
    std::vector<std::priority_queue<PQ_ITEM, std::vector<PQ_ITEM>, std::greater<PQ_ITEM>>> pqs;

    std::bitset<TOTAL_QS_FOR_FANCYPQ> bits;
public:
    FancyPQ(uint32_t d_s=SEQUENCER_DELAY, uint8_t total_qs=TOTAL_QS_FOR_FANCYPQ): d_s(d_s), total_qs(total_qs), e(0), d(0) {
        mtxs = std::vector<std::mutex>(total_qs);
        pqs = std::vector<std::priority_queue<PQ_ITEM, std::vector<PQ_ITEM>, std::greater<PQ_ITEM>>>(total_qs);

        srand(time(nullptr));
    }

    void push(PQ_ITEM x) {
        int ind = x % total_qs;
        std::lock_guard<std::mutex> lock(mtxs[ind]);
        e++;
        pqs[ind].push(x);
    }

    bool try_pop() {
        PQ_ITEM top = UINT64_MAX;
        int ind = -1;
        bits.reset();
        
        while(bits.count() < total_qs) {
            for (int i = 0; i < bits.size(); i++) {
                if (bits.test(i)) continue;

                if (mtxs[i].try_lock()) {
                    if (pqs[i].empty() == false && pqs[i].top() < top) {
                        top = pqs[i].top();
                        ind = i;
                    }

                    bits.set(i);
                    mtxs[i].unlock();
                }
            }
        }

        if (ind == -1) return false;
        if (get_time() - top < d_s) return false;

        d++;
        std::lock_guard<std::mutex> lock(mtxs[ind]);
        pqs[ind].pop();
        return true;
    }

    // Random number in [0, n)
    int get_random(int n) {
        return (rand() % n);
    }

    int get_total_dequeues() {
        return d;
    }
};
