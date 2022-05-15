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

TASK(TaskH) {
	printfSerial("TaskH begins. ");

	mdelay(2500);

	printfSerial("TaskH ends. ");
	TerminateTask();
};

TASK(TaskM) {
	printfSerial("TaskM begins. ");

	mdelay(2000);

	printfSerial("TaskM ends. ");
	TerminateTask();
};

TASK(TaskL) {
	printfSerial("TaskL begins. ");

	mdelay(1000);

	printfSerial("TaskL ends. ");
	TerminateTask();
};
