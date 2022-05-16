#include "ee.h"
#include "Arduino.h"
#include "serial.h"

void mdelay(unsigned long delay_ms);

void StartupHook(void)
{
	/* write below your code */


               /*end*/

    printfSerial("\nOS Begins... ", 0ul);
}

/* Timer1 ISR2 */
ISR2(TimerISR)
{
    static long c = -5;
    IncrementCounter(counter1);
    printfSerial("\n%4ld: ", ++c);
}


/* write below your code */



        /*end*/

