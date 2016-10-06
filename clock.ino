
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


void clock_isr () 
{
    interrupt_scheduler.execute ();
}


uint32_t counter;

void setup ()
{
    Serial.begin(9600);

    lcd.begin(16, 2);
    lcd.clear();

    interrupt_scheduler.schedule (0,  13,
                                  make_job([](){ ++counter; }));
    
    interrupt_scheduler.schedule (1, 500,
                                  make_job([]() { loop_scheduler.schedule (make_job([](){
                                                      char buf[MAX_BUF];
                                                      snprintf (buf, MAX_BUF-1, "%6lu", counter);
                                                      
                                                      lcd.setCursor(0, 1);
                                                      lcd.print (buf);
                                                  }));    
                                      })); 
    
    auto my_generic_job = make_generic_job ([]()
                             {
                                 lcd.setCursor(0, 0);
                                 lcd.print (counter);
                             });
    
    interrupt_scheduler.schedule (2, 2000, make_job ([my_generic_job](){ loop_scheduler.schedule (&my_generic_job); }));
//    interrupt_scheduler.schedule (2, 500, &serial_trigger_job);
    
    
    Timer1.initialize(500);
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
