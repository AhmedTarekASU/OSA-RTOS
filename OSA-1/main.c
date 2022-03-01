#include <avr/io.h>
#include <avr/interrupt.h>
#include "osa.h"
#include "OSAcfg.h"

uint8_t var = 0x00;

void InitT0(void)
{
	TCCR0 = (1<<CS00)|(1<<CS01);
	TCNT0 = 0x00;
	OCR0 = 0x00;
	TIMSK = (1<<TOV0);
}

ISR(TIMER0_OVF_vect)
{
	OS_Timer();
}

void Task1(void)
{
	DDRA = 0xFF;
	PORTA = 0x55;
	while(1)
	{
		PORTA = ~PORTA;
		OS_Delay(20);
	}
}

void Task2(void)
{
	DDRB = 0x00;
	PORTB = 0xFF;
	while(1)
	{
		var = PINB;
		OS_Yield();
	}
}

void Task3(void)
{
	DDRC = 0xFF;
	PORTC = 0x00;
	while(1)
	{
		PORTC = var;
		OS_Yield();
	}
}

void Task_Init(void)
{
	OS_Task_Create(0,Task1);
	//OS_Delay(250);
	OS_Task_Create(1,Task2);
	//OS_Delay(250);
	OS_Task_Create(2,Task3);
	OS_Task_Delete(this_task);
}

int main (void)
{
	OS_Init();
	InitT0();
	sei();
	OS_Task_Create(0,Task_Init);
	OS_Run();
	return 0;
}

