#ifndef CYCLIC_SCHEDULER_H
#define CYCLIC_SCHEDULER_H

#include <stdint.h>

#include "Job.h"

namespace clock 
{
    
    
    class CyclicScheduler 
    {

        struct JobItem
        {
            uint32_t  cycle;
            Job      *job;

            JobItem ()
                    :cycle(0),
                     job (0)
                {}
        
        };

        static const uint8_t max_priority = 32;

        uint32_t cycle;
        uint32_t max_cycle;
        JobItem items[max_priority];
        
    public:
        CyclicScheduler (uint32_t max=50000);
        void schedule (uint8_t priority, uint32_t cycle, Job * job);
        void execute  ();
        
    };
    
        
}


#endif // CYCLIC_SCHEDULER_H
