A simple PQ may be overwhelmed by a lot of parallel enqueues and dequeues. 

A better PQ, called FancyPQ, can maintain multiple PQs to distributed the enqueue load. A dequeue would have to peak at all the queues (a bitset based implementation is used for performance)

INITIAL RESULT: FancyPQ has more overhead and does not perform better than a simple PQ.  
