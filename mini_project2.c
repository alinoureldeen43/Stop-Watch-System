#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

unsigned char sec1=0,sec2=0,min1=0,min2=0,hr1=0,hr2=0;

void enable_stopwatch(void)
{
	TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS12) | (1<<CS10);
	TCCR1A = (1<<FOC1A);
	TCNT1=0;
	ICR1=960;
	TIMSK |= (1<<OCIE1A);
	SREG |= (1<<7);
}
ISR(TIMER1_COMPA_vect)
{
	SREG |=(1<<7);
	sec1++;
}

void enable_reset(void)
{
	SREG |=(1<<7);
	MCUCR |= (1<<ISC01);
	MCUCR &= ~(1<<ISC00);
	GICR |= (1<<INT0);

}
ISR(INT0_vect)
{

	sec1=0;
	sec2=0;
	min1=0;
	min2=0;
	hr1=0;
	hr2=0;
}

void enable_pause(void)
{
	SREG |=(1<<7);
	MCUCR |= (1<<ISC11) | (ISC10);
	GICR |= (1<<INT1);
}
ISR(INT1_vect)
{
	TCCR1B=0;
}

void enable_resume(void)
{
	SREG |=(1<<7);
	GICR |=(1<<INT2);
	MCUCSR &= ~(1<<ISC2);

}
ISR(INT2_vect)
{
	enable_stopwatch();
}
int main()
{
	enable_stopwatch();

	DDRC |= 0x0F;
	PORTC |= 0x0F;

	DDRA = 0x3F;
	PORTA = 0x3F;

	enable_reset();
	DDRD &= ~(1<<2);

	enable_pause();
	DDRD &=  ~(1<<3);

	enable_resume();
	DDRB &= ~(1<<2);

	while(1)
	{

		if(sec1==10)
		{
			sec2++;
			sec1=0;

		}
		if((sec2==6) & (sec1==10))
		{
			sec2=0;
			min1++;
		}
		if(min1==10)
		{
			min2++;
			min1=0;

		}
		if((min2==6) & (min1==10))
		{
			hr1++;
			min2=0;

		}
		if(hr1==10)
		{
			hr2++;
		}

		PORTA = (PORTA & 0x00) | (1<<0);
		PORTC = (PORTC & 0xF0) | sec1 ;

		_delay_ms(3);

		PORTA = (PORTA & 0x00) | (1<<1);
		PORTC = (PORTC & 0xF0) | sec2;

		_delay_ms(3);

		PORTA = (PORTA & 0x00) | (1<<2);
		PORTC = (PORTC & 0xF0) | min1;

		_delay_ms(3);

		PORTA = (PORTA & 0x00) | (1<<3);
		PORTC = (PORTC & 0xF0) | min2;

		_delay_ms(3);

		PORTA = (PORTA & 0x00) | (1<<4);
		PORTC = (PORTC & 0xF0) | hr1;

		_delay_ms(3);

		PORTA = (PORTA & 0x00) | (1<<5);
		PORTC = (PORTC & 0xF0) | hr2;

		_delay_ms(3);

	}
}
