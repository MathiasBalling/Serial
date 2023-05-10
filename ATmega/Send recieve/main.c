#define __AVR_ATmega32A__
#include <avr/interrupt.h>
#include <avr/io.h>

#define F_CPU 1000000UL // Define the clock frequency of the AVR
#define BAUD_RATE 4800  // Define the baud rate for UART communication
#define UBRR_VAL ((F_CPU / 16 / BAUD_RATE) - 1) // Calculate the UBRR value

volatile uint8_t conv_count = 0;  // Declare and initialize a volatile variable
                                  // to keep track of the number of conversions
volatile uint16_t conv_delay = 0; // Declare and initialize a volatile variable
                                  // to keep track of the conversion delay

int main(void) {

  // Setup PORTB
  DDRB = 0xFF; // Set all pins of PORTB as output
  // Setup UART
  UBRRH = (UBRR_VAL >>
           8); // Set the UBRRH register with the high byte of the UBRR value
  UBRRL =
      UBRR_VAL; // Set the UBRRL register with the low byte of the UBRR value
  UCSRB = (1 << TXEN) | (1 << RXEN) |
          (1 << RXCIE); // Enable RX and TX and RX interrupt in UCSRB register

  // Setup ADC
  ADMUX = (1 << ADLAR); // Left-justify the ADC result in ADMUX register
  ADCSRA =
      (1 << ADEN) | (1 << ADIE) | (0 << ADPS2) | (1 << ADPS1) |
      (1 << ADPS0); // Enable ADC, enable ADC interrupt, set ADC prescaler to 8

  // Enable Compare Match interrupt, but don't start it yet
  TIMSK = (1 << OCIE0); // Enable Compare Match interrupt

  sei(); // Enable interrupts globally

  while (1) {
    // Keep code Part empty :)
  }
}

////////////////// ISR Part //////////////////////

// UART receive interrupt service routine
ISR(USART_RXC_vect) {
  static uint8_t command_count = 0; // Declare a static variable to keep track
                                    // of the number of bytes received
  static uint8_t
      command_data[2]; // Declare a static array to store the received bytes

  if (command_count < 2) {
    command_data[command_count] =
        UDR; // Read the received byte from UDR and store it in the array
    command_count++; // Increment the byte counter
  }

  if (command_count >= 2) {
    conv_count = command_data[0]; // Store the first byte as the number of
                                  // conversions to perform
    conv_delay = command_data[1]; // Store the second byte as the delay between
                                  // conversions
    command_count = 0;            // Reset the byte counter
    if (conv_count > 0) {
      ADCSRA |= (1 << ADSC); // Start the ADC conversion
    }
  }
}

// ADC interrupt service routine
ISR(ADC_vect) {
  UDR = ADCH;    // Send the high byte of the ADC result to the UART
  PORTB = ~ADCH; // Send the high byte of the ADC result to the PORTB
  conv_count--;  // Decrement the conversion counter
  if (conv_count > 0) {
    OCR0 = conv_delay; // Set the timer compare value to the conversion delay
    TCNT0 = 0;         // Reset the timer counter
    TCCR0 =
        (1 << CS02) | (1 << CS00); // Start the timer with a prescaler of 1024
  } else {
    TCNT0 = 0; // Reset the timer counter
    TCCR0 = 0; // Stop the timer
  }
}

// Timer compare match interrupt service routine
ISR(TIMER0_COMP_vect) {
  ADCSRA |= (1 << ADSC); // start conversion
}