#ifndef PQUEUE_H
#define PQUEUE_H

#include <iostream>

const int SEQUENCER_DELAY = 0;  // microseconds
const int TOTAL_ITEMS = 1'000'000;
const int EXPERIMENT_ITERATIONS = 1;

const int MICRO = 1'000'000;

typedef uint64_t PQ_ITEM;

uint64_t get_time();

#endif  // PQUEUE_H