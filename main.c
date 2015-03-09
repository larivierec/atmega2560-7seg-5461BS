#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUD 57600
#define LED PB7
#define USART_LED PL2
#define PIN22 PA0 //SEGMENT B
#define PIN24 PA2 //SEGMENT C
#define PIN26 PA4 //SEGMENT D
#define PIN28 PA6 //SEGMENT G
#define PIN30 PC7 //SEGMENT E
#define PIN32 PC5 //SEGMENT A
#define PIN34 PC3 //SEGMENT DP
#define PIN36 PC1 //DIGIT 3 FROM LEFT
#define PIN38 PD7 //DIGIT 2 FROM LEFT
#define PIN40 PG1 //DIGIT 1 FROM LEFT
#define PIN42 PL7 //DIGIT 4 FROM LEFT
#define PIN43 PL6 //SEGMENT F

//INPUT OR OUTPUT
#define SET_DIRECTION(DDR, PIN_NUMBER)	        (DDR |= (1 << PIN_NUMBER))
#define CLEAR_DIRECTION(DDR,PIN_NUMBER)         (DDR &= ~(1 << PIN_NUMBER))
#define SET_PIN(PORT,PIN_NUMBER)		(PORT |= (1 << PIN_NUMBER))
#define TOGGLE_PIN(PORT,PIN_NUMBER)             (PORT ^= (1 << PIN_NUMBER))
#define CLEAR_PIN(PORT,PIN_NUMBER)		(PORT &= ~(1 << PIN_NUMBER))

//FLAG CONTROL

//Flag setters
#define LED_FLAG_SET    1 << 0
#define USART_RECEIVED  1 << 1
#define USART_TRANSMIT  1 << 2

//Flag clears
#define LED_FLAG_CLEAR (booleanFlags &= ~0x01)
#define CLEAR_USART_RECV (booleanFlags &= ~0x02);
#define CLEAR_USART_TRANS (booleanFlags &= ~0x04);

//Flag compares

#define IS_LED_FLAG_SET (booleanFlags == (booleanFlags | LED_FLAG_SET))
#define IS_USART_RECEIVED (booleanFlags == (booleanFlags | USART_RECEIVED))
#define IS_USART_TRANSMIT (booleanFlags == (booleanFlags | USART_TRANSMIT))

volatile int booleanFlags = 0;
volatile unsigned char receivedUSART;
int numberToWrite = 0;

ISR(TIMER1_OVF_vect)
{
  booleanFlags |= LED_FLAG_SET;
}

ISR(USART0_RX_vect)
{
  booleanFlags |= USART_RECEIVED;
}

void initGPIO(void){

  //set all pins from 22-29 as digital outs
  //BOARD LED
  SET_DIRECTION(DDRB, LED);
  SET_DIRECTION(DDRL, USART_LED);

  SET_DIRECTION(DDRA,PIN22);
  SET_DIRECTION(DDRA,PIN24);
  SET_DIRECTION(DDRA,PIN26);
  SET_DIRECTION(DDRA,PIN28);
  SET_DIRECTION(DDRC,PIN30);
  SET_DIRECTION(DDRC,PIN32);
  SET_DIRECTION(DDRC,PIN34);
  SET_DIRECTION(DDRC,PIN36);
  SET_DIRECTION(DDRD,PIN38);
  SET_DIRECTION(DDRG,PIN40);
  SET_DIRECTION(DDRL,PIN42);
  SET_DIRECTION(DDRL,PIN42);
}

void initTimer(void)
{
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS11) | (1 << CS10);
  TIMSK1 |= (1 << TOIE1);
}

void init(void)
{
  initTimer();
  initUSART();
  initGPIO();
}

void initUSART(void)
{
 UBRR0H = (BAUD >> 8);
 UBRR0L = (BAUD);
 UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
 UCSR0C = (1 << UCSZ10) | (1 << UCSZ00);
}


int main(void)
{
  cli();
  init();
  sei();

  while (1)
  {

   if(IS_LED_FLAG_SET)
   {
    TOGGLE_PIN(PORTB,LED);
    LED_FLAG_CLEAR;
   }

   if(IS_USART_RECEIVED)
   {
	   receivedUSART = UDR0;
	   CLEAR_USART_RECV;
	   UCSR0A |= (1 << UDRE0);
	   UCSR0B |= (1 << TXCIE0);
   }


   if(IS_USART_TRANSMIT)
   {
    while(1 != (UDRE0 << UCSR0A))
    UDR0 = receivedUSART;
    TOGGLE_PIN(PORTL,USART_LED);
    CLEAR_USART_TRANS;
   }
   switch(numberToWrite)
   {
     SET_PIN(PORTG,PIN40);

     case 0:
       SET_PIN(PORTC,PIN32);
       SET_PIN(PORTA,PIN22);
       SET_PIN(PORTA,PIN24);
       SET_PIN(PORTA,PIN26);
       SET_PIN(PORTC,PIN30);
       SET_PIN(PORTL,PIN43);
       CLEAR_PIN(PORTA,PIN28);
       CLEAR_PIN(PORTC,PIN34);
       break;
     case 1:
       SET_PIN(PORTC,PIN32);
       SET_PIN(PORTA,PIN22);
       CLEAR_PIN(PORTA,PIN24);
       CLEAR_PIN(PORTA,PIN26);
       CLEAR_PIN(PORTC,PIN30);
       CLEAR_PIN(PORTL,PIN43);
       CLEAR_PIN(PORTA,PIN28);
       CLEAR_PIN(PORTC,PIN34);
       break;
     case 2:
       SET_PIN(PORTC,PIN32);
       SET_PIN(PORTA,PIN22);
       CLEAR_PIN(PORTA,PIN24);
       SET_PIN(PORTA,PIN26);
       SET_PIN(PORTC,PIN30);
       CLEAR_PIN(PORTL,PIN43);
       SET_PIN(PORTA,PIN28);
       CLEAR_PIN(PORTC,PIN34);
       break;
     case 3:
       SET_PIN(PORTC,PIN32);
       SET_PIN(PORTA,PIN22);
       SET_PIN(PORTA,PIN24);
       SET_PIN(PORTA,PIN26);
       CLEAR_PIN(PORTC,PIN30);
       CLEAR_PIN(PORTL,PIN43);
       SET_PIN(PORTA,PIN28);
       CLEAR_PIN(PORTC,PIN34);
       break;
     case 4:
       CLEAR_PIN(PORTC,PIN32);
       SET_PIN(PORTA,PIN22);
       SET_PIN(PORTA,PIN24);
       SET_PIN(PORTA,PIN26);
       CLEAR_PIN(PORTC,PIN30);
       SET_PIN(PORTL,PIN43);
       SET_PIN(PORTA,PIN28);
       CLEAR_PIN(PORTC,PIN34);
       break;
     case 5:
       SET_PIN(PORTC,PIN32);
       SET_PIN(PORTA,PIN22);
       SET_PIN(PORTA,PIN24);
       SET_PIN(PORTA,PIN26);
       CLEAR_PIN(PORTC,PIN30);
       SET_PIN(PORTL,PIN43);
       SET_PIN(PORTA,PIN28);
       CLEAR_PIN(PORTC,PIN34);
       break;
     case 6:
       SET_PIN(PORTC,PIN32);
       CLEAR_PIN(PORTA,PIN22);
       SET_PIN(PORTA,PIN24);
       SET_PIN(PORTA,PIN26);
       SET_PIN(PORTC,PIN30);
       SET_PIN(PORTL,PIN43);
       SET_PIN(PORTA,PIN28);
       CLEAR_PIN(PORTC,PIN34);
       break;
     case 7:
       SET_PIN(PORTC,PIN32);
       SET_PIN(PORTA,PIN22);
       SET_PIN(PORTA,PIN24);
       CLEAR_PIN(PORTA,PIN26);
       CLEAR_PIN(PORTC,PIN30);
       CLEAR_PIN(PORTL,PIN43);
       CLEAR_PIN(PORTA,PIN28);
       CLEAR_PIN(PORTC,PIN34);
       break;
     case 8:
       SET_PIN(PORTC,PIN32);
       SET_PIN(PORTA,PIN22);
       SET_PIN(PORTA,PIN24);
       SET_PIN(PORTA,PIN26);
       SET_PIN(PORTC,PIN30);
       SET_PIN(PORTL,PIN43);
       SET_PIN(PORTA,PIN28);
       CLEAR_PIN(PORTC,PIN34);
       break;
     case 9:
       SET_PIN(PORTC,PIN32);
       SET_PIN(PORTA,PIN22);
       SET_PIN(PORTA,PIN24);
       SET_PIN(PORTA,PIN26);
       CLEAR_PIN(PORTC,PIN30);
       SET_PIN(PORTL,PIN43);
       SET_PIN(PORTA,PIN28);
       CLEAR_PIN(PORTC,PIN34);
       break;

     default:

       break;
    }

   /**********************
  *      A
  *   F      B
  *      G
  *   E      C
  *      D
  *   PIN22 PA0 //SEGMENT B
  *   PIN24 PA2 //SEGMENT C
  *   PIN26 PA4 //SEGMENT D
  *   PIN28 PA6 //SEGMENT G
  *   PIN30 PC7 //SEGMENT E
  *   PIN32 PC5 //SEGMENT A
  *   PIN43 PL6 //SEGMENT F
  *   PIN34 PC3 //SEGMENT DP
  **********************/

    numberToWrite++;
    if(numberToWrite > 9)
      numberToWrite = 0;
  }
  return 0; // never reached
}
