
#include <Arduino.h>
#include <stddef.h>
#include <LiquidCrystal.h>

#include <QueueArray.h>

#include "PriorityScheduler.h"

extern LiquidCrystal lcd;

using namespace clock;

PriorityScheduler::PriorityScheduler ()
{
    items.setPrinter (lcd);
}

void
PriorityScheduler::schedule (const Job *job) 
{
    noInterrupts();
    items.push (const_cast<Job *> (job));
    interrupts();
}

void
PriorityScheduler::execute () 
{
    while ( true ) {
        noInterrupts();

        if ( items.isEmpty () ) {
            interrupts();
            return;
        }    
        
        Job * job = items.pop();
        interrupts();

        if (job) {
            job->execute ();
        } else {
            return;
        }
    }    
}

