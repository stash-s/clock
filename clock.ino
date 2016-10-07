
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Time.h>
#include <TimerOne.h>
#include <DS1307RTC.h>

#include <stdio.h>

#include "Job.h"
#include "PriorityScheduler.h"
#include "CyclicScheduler.h"

#define LCD_BACKLIGHT_PIN 3

#define DS1307_CTRL_ID 0x68
#define MAX_BUF 32

#define SQW_FREQ 4096
#define REFRESH_FREQ 16

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


using namespace clock;


PriorityScheduler loop_scheduler;
CyclicScheduler   interrupt_scheduler;
tmElements_t      tm;

char *time_formats [] = {
    "%02d:%02d:%02d",
    "%02d %02d %02d"
};

char *time_format = time_formats[0];

int freeRam () 
{
    extern int __heap_start, *__brkval; 
    int v; 
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void clock_isr () 
{
    interrupt_scheduler.execute ();
}


uint32_t counter;

tmElements_t * parseTime (tmElements_t *tm, char *str) ;


void setup ()
{
    Serial.begin(9600);

    lcd.begin(16, 2);
    lcd.clear();

    parseTime (&tm, __TIME__);
    
    interrupt_scheduler.schedule (0,  13,
                                  make_job([](){ ++counter; }));

    auto display_time_job = make_job([](){
            char buf[MAX_BUF];
            snprintf (buf, MAX_BUF-1, time_format, tm.Hour, tm.Minute, tm.Second);
            
            lcd.setCursor(4, 0);
            lcd.print (buf);
        });
        
    interrupt_scheduler.schedule (1, 1000,
                                  make_job([display_time_job]() {

                                          ++ tm.Second;
                                          
                                          if (tm.Second >= 60) {
                                              
                                              tm.Second = 0;
                                              ++ tm.Minute;

                                              if (tm.Minute >= 60) {

                                                  tm.Minute = 0;
                                                  ++ tm.Hour;

                                                  if (tm.Hour >= 24) {

                                                      tm.Hour = 0;
                                                  }
                                              }
                                          }
                                          
                                          
                                          loop_scheduler.schedule (display_time_job);
                                      }));

    interrupt_scheduler.schedule (2, 500,
                                  make_job([display_time_job]() {

                                          if (time_format == time_formats[0]) {
                                              time_format = time_formats[1];
                                          } else {
                                              time_format = time_formats[0];
                                          }
                                          
                                          loop_scheduler.schedule (display_time_job);
                                      }));
    

//    interrupt_scheduler.schedule (2, 500, &serial_trigger_job);

    auto print_free_mem_job = make_job([]()
                                       {
                                           char buf[MAX_BUF];
                                           snprintf (buf, MAX_BUF-1, "free:%6d", freeRam());
                                 
                                           lcd.setCursor(0, 1);
                                           lcd.print (buf);
                                       });
    
    interrupt_scheduler.schedule (3, 2000, make_job ([print_free_mem_job](){ loop_scheduler.schedule (print_free_mem_job); }));
    
                                           
    
    Timer1.initialize(1000);
    Timer1.attachInterrupt (clock_isr);
    
    
    //current_state = new MainScreen ();
    
    //setup_interrupts ();

    //lcd backlight control
    //digitalWrite( LCD_BACKLIGHT_PIN, LOW );  //backlight control pin D3 is high (on)
    //pinMode( LCD_BACKLIGHT_PIN, OUTPUT );     //D3 is an output
}        


void loop () 
{
    loop_scheduler.execute ();
    
    //delayMicroseconds(10);
    
//    current_state->display ();   
}

tmElements_t * parseTime (tmElements_t *tm, char *str) 
{
    int hour, minute, sec;
    
    if (sscanf (str, "%d:%d:%d", &hour, &minute, &sec)) {
        tm->Hour   = hour;
        tm->Minute = minute;
        tm->Second = sec;

        return tm;
        
    }
    return NULL;
}

tmElements_t * parseDate(tmElements_t *tm, char *str)
{
    

}



/*
class MainScreen;

class Pendulum :public Job 
{
  public:
    
    tmElements_t *time;
    int interrupt_count;
    
  private:
    
    Pendulum (tmElements_t * t)
        :time (t),
        interrupt_count(0)
    {}

    virtual void execute () override;
};

void
Pendulum::execute () 
{
    ++interrupt_count;    
    
    if (interrupt_count >= 10) {
        interrupt_count = 0;    

        if (time_str == time_formats[0]) {
            time_str = time_formats[1];
        }
        else {
            time_str = time_formats[0];
        }
    }
    
    refresh_screen = true;
}


class MainScreen : public Job 
{
  public:
    
    static char *time_formats[];

  private:
    
    char *time_format;// = time_formats[0];

  public :
    
    
    volatile bool refresh_screen;

  public:

    MainScreen ();

    void set_time_format (char *format) 
    {
        time_format = format;
    }    
    
    virtual void execute () override;    
};

MainScreen::MainScreen ()
:time_str (time_formats[0]),
    interrupt_count (0),
    refresh_screen (false)
{}


char *MainScreen::time_formats [] = {
    "%02d:%02d:%02d",
    "%02d %02d %02d"
};


void
MainScreen::execute () 
{
    char buf[MAX_BUF];

    tmElements_t tm;
    
    if (refresh_screen) {
        refresh_screen = false;
    

        bool result = RTC.read(tm);

        //noInterrupts();
        
        if (result) {
            snprintf (buf, MAX_BUF-1, time_str, tm.Hour, tm.Minute, tm.Second);
            lcd.setCursor(0, 1);
            lcd.print (buf);        
        
        } else {        
            lcd.setCursor(0, 0);
            lcd.print ("boo");        
        }

        //interrupts();

    }    

}



State * current_state = NULL; 

void clock_isr () 
{
    if (current_state) {
        current_state->tick();
    }
    
}


bool error (char *str) 
{
    lcd.write (str);
    return false;    
}


bool setup_interrupts () 
{
    //Wire.begin();

    // read SQW/OUT state
    //Wire.beginTransmission(DS1307_CTRL_ID);
    //Wire.write((uint8_t)0x07);
    //Wire.write(0x00);

    //if (Wire.endTransmission() != 0) return error ("error writing");
    

    Timer1.initialize(50000);
    Timer1.attachInterrupt (clock_isr);
    
    
    //pinMode (2, INPUT_PULLUP);    
    //attachInterrupt(digitalPinToInterrupt (2), clock_isr, CHANGE);
    //interrupts();
    
}
    
*/
