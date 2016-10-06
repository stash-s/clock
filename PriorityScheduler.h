#ifndef PRIORITY_SCHEDULER_H
#define PRIORITY_SCHEDULER_H

#include <QueueArray.h>

#include "Job.h"


template <class T> class QueueArray;

namespace clock 
{
    class PriorityScheduler : public Job 

    {

    private:
        QueueArray<Job *> items;
        
        //Job *items[Job::max_priority];
        
    public:

        PriorityScheduler();
        
        //void schedule (uint8_t priority, const Job * job);
        void schedule (const Job * job);
        
        virtual void execute  () override;
    };
        
}

#endif // PRIORITY_SCHEDULER_H
