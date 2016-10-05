#ifndef JOB_H
#define JOB_H

#include <stdint.h>

namespace clock 
{
    class Job 
    {

    public:
        
        static const uint8_t max_priority = 32;
        
        virtual void execute ()=0;
        
    };
    
};


#endif // JOB_H
