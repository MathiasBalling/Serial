#define F_CPU 8000000UL
#define __AVR_ATmega32A__
#include "avr/interrupt.h"
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define BAUD 9600
#define UBRR_VAL ((F_CPU / (16UL * BAUD)) - 1)

volatile static uint8_t data = 0;

int main(void) {
  DDRB = 0xFF;         // PORTB as output
  DDRA = 0;            // PORTA as input
  ADMUX = 0b00100000;  // AREF internal as reference, ADC0 as input
  ADCSRA = 0b11001110; // ADC Enable, ADC Start Conversion, ADC Interrupt
                       // Enable, Prescaler = 8 so 1MHz/8 = 125kHz

  // UART
  UBRRH = 0;           // Configure baud rate
  UBRRL = UBRR_VAL;    // 9600 baud
  UCSRB = (1 << TXEN); // Enable transmitter
  sei();
  while (1) {
  }
}

ISR(ADC_vect) {
  uint8_t temp = ADCH;
  if (temp != data) {
    data = temp;
    while (!(UCSRA & (1 << UDRE))) {
    }
    UDR = data;
    PORTB = ~data;
  }
  _delay_ms(50);
  ADCSRA |= (1 << ADSC);
}