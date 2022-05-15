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
}

TASK(Task1) {
	printfSerial("Task1 begins. ");

	mdelay(3500);

	printfSerial("Task1 ends. ");
	TerminateTask();
};

TASK(Task2) {
	printfSerial("Task2 begins. ");

	mdelay(2000);

	printfSerial("Task2 ends. ");
	TerminateTask();
};
