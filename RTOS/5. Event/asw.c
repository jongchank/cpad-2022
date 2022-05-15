#include "ee.h"
#include "Arduino.h"
#include "serial.h"

void mdelay(unsigned long delay_ms);

/* Timer1 ISR2 */
ISR2(TimerISR)
{
    static long c = -5;
    IncrementCounter(counter1);
    printfSerial("\n%4ld: ", ++c);
    if(c % 5 == 0){
        SetEvent(Task1, Event1);
    }
}

TASK(Task1) {
	printfSerial("Task1 begins. ");

	EventMaskType mask;

	for(;;)
	{
		WaitEvent(Event1);
	    GetEvent(Task1, &mask);
	    if (mask & Event1) {
	    	printfSerial("Event has occured!");
	    }
		ClearEvent(Event1);
	}

	printfSerial("Task1 ends. ");
	TerminateTask();
};

TASK(Task2) {
	printfSerial("Task2 begins. ");

	mdelay(1500);

	printfSerial("Task2 ends. ");
	TerminateTask();
};
