#include "ee.h"
#include "Arduino.h"
#include "serial.h"

#define TIMER1_US 100000U /* 100 ms */

extern "C"
{

	void mdelay(unsigned long delay_ms)
	{
		unsigned long prev_ms = millis(), current_ms = millis();
		unsigned long period_ms = 20, cnt = 0;
		while (cnt < (delay_ms / period_ms))
		{
			current_ms = millis();
			if (current_ms - prev_ms >= period_ms)
			{
				cnt++;
				prev_ms = millis();
			}
		}
	}

} /* extern "C" */

void loop(void)
{
	;
}

void setup(void)
{
	Serial.begin(9600);
	OsEE_atmega_startTimer1(TIMER1_US);
}

int main(void)
{

	init();

#if defined(USBCON)
	USBDevice.attach();
#endif

	setup();

	StartOS(OSDEFAULTAPPMODE); /* never returns */
	return 0;
}
