#include <stddef.h>


#include "PriorityScheduler.h"

using namespace clock;

PriorityScheduler::PriorityScheduler () 
{
    for (uint8_t i=0; i < Job::max_priority; ++i) {
        items[i]=0;
    }
}

void
PriorityScheduler::schedule (uint8_t priority, Job *job) 
{
    items[priority] = job;
}


void
PriorityScheduler::execute ()
{
    for (int i=0; i < Job::max_priority; ++i) {
        
        Job *job = items[i];
        
        if (job != NULL) {
            
            items[i] = NULL;
            job->execute ();
            
        }
    }
}

