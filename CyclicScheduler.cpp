#include "CyclicScheduler.h"

using namespace clock;

CyclicScheduler::CyclicScheduler (uint32_t max)
        :cycle(0),
         max_cycle (max)
{}


void
CyclicScheduler::schedule  (uint8_t priority, uint32_t cycle, const Job * job) 
{
    items[priority].cycle = cycle;
    items[priority].job   = const_cast<Job *>(job);
}

void
CyclicScheduler::execute () 
{
    if (cycle >= max_cycle) {
        cycle = 0;
    }
    ++ cycle;    

    
    for (JobItem * item = items; item < items + Job::max_priority; ++item ) {
        
        if (item->job != 0) {
            if (0 == (cycle % item->cycle)) {
                item->job->execute();
            }
        }
    }
    
}
