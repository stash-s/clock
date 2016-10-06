#ifndef PRIORITY_SCHEDULER_H
#define PRIORITY_SCHEDULER_H

#include "Job.h"

namespace clock 
{
    class PriorityScheduler : public Job 

    {

    private:
        Job *items[Job::max_priority];
        
    public:

        PriorityScheduler();
        
        void schedule (uint8_t priority, Job * job);
        void execute  ();
    };
        
}

#endif // PRIORITY_SCHEDULER_H
