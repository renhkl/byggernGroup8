#include "avr/io.h"
#include </usr/lib/avr/include/avr/iom162.h>
#include </usr/lib/avr/include/avr/interrupt.h>
#include <stdio.h>
//#include <avr/interrupt.h>
//#include <util/delay.h>

#define F_CPU 4915200UL // Define CPU frequency as 16 MHz
#define BAUD 9600 // The desired baud rate for serial communication
#define UBRR_VAL ((F_CPU/16/BAUD)-1)

// --- Circular Buffer (Ring Buffer) for Reception ---
// This buffer is used to store received characters from the interrupt service routine
// to prevent data from being lost while the main application is busy.
#define RX_BUFFER_SIZE 32
volatile char rx_buffer[RX_BUFFER_SIZE];
volatile unsigned int rx_head = 0;
volatile unsigned int rx_tail = 0;
volatile unsigned int rx_count = 0;

// --- Function Prototypes ---
void uart_init(unsigned int ubrr);
int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);

// --- Main Program ---
int main(void) {
// Initialize the USART (UART) interface
uart_init(UBRR_VAL);

// Link the `printf` and `scanf` functions to our driver.
// The first argument is the transmit function, the second is the receive function.
// This addresses the user's requirement to link with the stdio library.
fdevopen(uart_putchar, uart_getchar);

// Enable global interrupts
sei();

printf("\n\rUART Driver Initialized. Ready for commands.\n\r");
char input_char;

while (1) {
printf("Type a character and press Enter: ");
input_char = getchar();
printf("\n\rYou typed: %c\n\r", input_char);
}

return 0;
}

// --- UART Driver Functions ---

// Initializes the USART interface with a given baud rate
void uart_init(unsigned int ubrr) {
// Set baud rate
UBRR0H = (unsigned char)(ubrr >> 8);
UBRR0L = (unsigned char)ubrr;
// Enable receiver, transmitter, and the receive complete interrupt
UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
// Set frame format: 8 data, 1 stop bit
UCSR0C = (3 << UCSZ00);
}

// Transmits a single character to the UART.
// This function handles the "UART is busy" situation (a) by waiting for the
// transmit buffer to be empty before sending a new character.
int uart_putchar(char c, FILE *stream) {
// Check for newline characters and add a carriage return for terminal compatibility
if (c == '\n') {
uart_putchar('\r', stream);
}
// Wait for the transmit buffer to be empty
while (!(UCSR0A & (1 << UDRE0)));
// Put data into the buffer and send it 
UDR0 = c;
return 0;
}

// Receives a single character from the UART.
// It waits for a character to be available in the ring buffer before returning.
int uart_getchar(FILE *stream) {
// Wait until there is at least one character in the buffer
while (rx_count == 0);
// Retrieve the character from the buffer
char c = rx_buffer[rx_tail];
rx_tail = (rx_tail + 1) % RX_BUFFER_SIZE;
rx_count--;
return c;
}

// --- Interrupt Service Routine (ISR) for UART Reception ---
// This ISR is triggered when a new character is received by the UART.
// It addresses the "be notified" requirement (b) by immediately
// handling the incoming data and storing it in the buffer.
ISR(USART0_RX_vect) {
// Read the incoming character
char received_data = UDR0;

// Store the data in the circular buffer if it's not full
if (rx_count < RX_BUFFER_SIZE) {
rx_buffer[rx_head] = received_data;
rx_head = (rx_head + 1) % RX_BUFFER_SIZE;
rx_count++;
}
}


