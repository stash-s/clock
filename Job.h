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


    template <typename T>
    class GenericJob : public Job 
    {
        T callable;

      public:
        GenericJob (T c)
            :callable(c)
        {}
        
        void execute () 
            {
                callable();
            }
        
    };

    template <typename T>
    static Job * make_job (T callable) 
    {
        return new GenericJob<T> (callable);
    }

}


#endif // JOB_H
