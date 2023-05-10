#define __AVR_ATmega32A__
#include <avr/interrupt.h>
#include <avr/io.h>

#define F_CPU 1000000UL // Define the clock frequency of the AVR
#define BAUD_RATE 4800  // Define the baud rate for UART communication
#define UBRR_VAL ((F_CPU / 16 / BAUD_RATE) - 1) // Calculate the UBRR value

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

  sei(); // Enable interrupts globally

  while (1) {
  }
}

////////////////// ISR Part //////////////////////

// UART receive interrupt service routine
ISR(USART_RXC_vect) {
  static uint8_t
      command_data; // Declare a static array to store the received bytes
  command_data = UDR;
  PORTB = ~command_data; // Write the received byte to PORTB
  UDR = command_data;    // Send the received byte back to the PC
}