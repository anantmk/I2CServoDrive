#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usiTwiSlave.h"

// Note: The LSB is the I2C r/w flag and must not be used for addressing!
#define 	SLAVE_ADDR_ATTINY       0b00110100

#define uniq(LOW,HEIGHT)	((HEIGHT << 8)|LOW)			  // Create 16 bit number from two bytes
#define LOW_BYTE(x)        	(x & 0xff)					    // Get low byte from 16 bit number
#define HIGH_BYTE(x)       	((x >> 8) & 0xff)			  // Get high byte from 16 bit number

uint8_t dataOK = 0;

uint16_t CH1_out = 0;
uint16_t CH2_out = 0;
uint16_t CH3_out = 0;
uint16_t CH4_out = 0;
uint16_t CH5_out = 0;
uint16_t CH6_out = 0;
uint16_t CH7_out = 0;
uint16_t CH8_out = 0;
uint16_t CH9_out = 0;
uint16_t CH10_out = 0;
uint16_t CH11_out = 0;
uint16_t CH12_out = 0;

void InitTimer1(void);
void StartTimer1(void);
void initIO(void);

void initIO(void)
{
    //SET OUTPUTS
    
    //Servo channels as outputs
    
    DDRB |= (1 << PIN0) | (1 << PIN1) | (1 << PIN2) | (1 << PIN3) | (1 << PIN4);
    DDRD |= (1 << PIN0) | (1 << PIN1) | (1 << PIN2) | (1 << PIN3) | (1 << PIN4) | (1 << PIN5) | (1 << PIN6);
    
    //LED as ouput
    DDRB |= (1 << PIN6);
    
    //LED on
    PORTB |= (1 << PIN6);
    
    _delay_ms(500);
    
    PORTB &= ~(1 << PIN6);
    
    InitTimer1();
    
    StartTimer1();
    
    cli();  // Disable interrupts
    
    usiTwiSlaveInit(SLAVE_ADDR_ATTINY);	// TWI slave init
    
    sei();  // Re-enable interrupts
}

int main(void)
{
	initIO();

	while (1)
    {
        if (TCNT1<2000)
        {
            
        }
        
        if ((TCNT1>=2000) && (TCNT1<=5500) && (dataOK == 1))
        {
            //reset channels from high to low
            if (TCNT1>=CH1_out && bit_is_set(PORTB, PIN0)) PORTB &= ~(1<<PIN0);
            if (TCNT1>=CH2_out && bit_is_set(PORTB, PIN1)) PORTB &= ~(1<<PIN1);
            if (TCNT1>=CH3_out && bit_is_set(PORTB, PIN2)) PORTB &= ~(1<<PIN2);
            if (TCNT1>=CH4_out && bit_is_set(PORTB, PIN3)) PORTB &= ~(1<<PIN3);
            if (TCNT1>=CH5_out && bit_is_set(PORTB, PIN4)) PORTB &= ~(1<<PIN4);
            if (TCNT1>=CH6_out && bit_is_set(PORTD, PIN0)) PORTD &= ~(1<<PIN0);
            if (TCNT1>=CH7_out && bit_is_set(PORTD, PIN1)) PORTD &= ~(1<<PIN1);
            if (TCNT1>=CH8_out && bit_is_set(PORTD, PIN2)) PORTD &= ~(1<<PIN2);
            if (TCNT1>=CH9_out && bit_is_set(PORTD, PIN3)) PORTD &= ~(1<<PIN3);
            if (TCNT1>=CH10_out && bit_is_set(PORTD, PIN4)) PORTD &= ~(1<<PIN4);
            if (TCNT1>=CH11_out && bit_is_set(PORTD, PIN5)) PORTD &= ~(1<<PIN5);
            if (TCNT1>=CH12_out && bit_is_set(PORTD, PIN6)) PORTD &= ~(1<<PIN6);
        }
        
        if ((TCNT1>5500) && (TCNT1<35000))
        {
            CH1_out = uniq(rxbuffer[1], rxbuffer[2]);
            CH2_out = uniq(rxbuffer[3], rxbuffer[4]);
            CH3_out = uniq(rxbuffer[5], rxbuffer[6]);
            CH4_out = uniq(rxbuffer[7], rxbuffer[8]);
            CH5_out = uniq(rxbuffer[9], rxbuffer[10]);
            CH6_out = uniq(rxbuffer[11], rxbuffer[12]);
            CH7_out = uniq(rxbuffer[13], rxbuffer[14]);
            CH8_out = uniq(rxbuffer[15], rxbuffer[16]);
            CH9_out = uniq(rxbuffer[17], rxbuffer[18]);
            CH10_out = uniq(rxbuffer[19], rxbuffer[20]);
            CH11_out = uniq(rxbuffer[21], rxbuffer[22]);
            CH12_out = uniq(rxbuffer[23], rxbuffer[24]);
            
            //ready for output
            dataOK = 1;
        }
        
        
	}
	return 0; // never reached
}

//Initialize timer3 (PWM)
void InitTimer1(void)
{
    //Set Initial Timer value
    TCCR1A |= 1<<WGM11;
    TCCR1B |= 1<<WGM12 | 1<<WGM13 | 1<<CS11;
    TIMSK  |= 1<<OCIE1A;
}

void StartTimer1(void)
{
    //Start timer without prescalle
    ICR1 = 49999;
}

//PWM compare ISR
ISR(TIMER1_COMPA_vect)
{
    if (rxbuffer[0] == 111)
    {
        //LED off
        PORTB &= ~(1 << PIN6);
        
        //set all PWM channels to high
        if (CH1_out > 0) PORTB |= (1 << PIN0);
        if (CH2_out > 0) PORTB |= (1 << PIN1);
        if (CH3_out > 0) PORTB |= (1 << PIN2);
        if (CH4_out > 0) PORTB |= (1 << PIN3);
        if (CH5_out > 0) PORTB |= (1 << PIN4);
        if (CH6_out > 0) PORTD |= (1 << PIN0);
        if (CH7_out > 0) PORTD |= (1 << PIN1);
        if (CH8_out > 0) PORTD |= (1 << PIN2);
        if (CH9_out > 0) PORTD |= (1 << PIN3);
        if (CH10_out > 0) PORTD |= (1 << PIN4);
        if (CH11_out > 0) PORTD |= (1 << PIN5);
        if (CH12_out > 0) PORTD |= (1 << PIN6);
        
        
//        
//        PORTB |= (1 << PIN0) | (1 << PIN1) | (1 << PIN2) | (1 << PIN3) | (1 << PIN4);
//        PORTD |= (1 << PIN0) | (1 << PIN1) | (1 << PIN2) | (1 << PIN3) | (1 << PIN4) | (1 << PIN5) | (1 << PIN6);
    }
    else
    {
        //LED on
        PORTB |= (1 << PIN6);        
        
        //data not OK
        dataOK = 0;
        
        //set all PWM channels to low
        PORTB &= ~((1 << PIN0) | (1 << PIN1) | (1 << PIN2) | (1 << PIN3) | (1 << PIN4));
        PORTD &= ~((1 << PIN0) | (1 << PIN1) | (1 << PIN2) | (1 << PIN3) | (1 << PIN4) | (1 << PIN5) | (1 << PIN6));
    }
}
