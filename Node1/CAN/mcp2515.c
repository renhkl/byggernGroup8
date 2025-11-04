//this driver is made for setting up and changing the mcp2515 CAN controller's control and status registers

#include "mcp2515.h"
#include "../MCU/spi.h"

#define CAN_CS PB0

void SPIMCP2515Transmit(char cData) {
    
    _delay_ms(0.408);

    SPDR = cData;

    while(!(SPSR & (1 << SPIF))){}
}

uint8_t SPIMCP2515Receive(){

    SPDR = 0;
    _delay_us(2);

    while(!(SPSR & (1 << SPIF))){}
    _delay_us(2);

    return SPDR;
}

uint8_t mcp2515Read (uint8_t address)
{
    PORTB &= ~(1 << CAN_CS); // Select CAN - controller
    SPIMCP2515Transmit(MCP_READ); // Send read instruction
    SPIMCP2515Transmit(address); // Send address
    uint8_t result = SPIMCP2515Receive(); //  Read result
    PORTB |= (1 << CAN_CS); // Deselect CAN - controller

    return result;
}


void mcp2515Reset (){
    PORTB &= ~(1 << CAN_CS);
    SPIMCP2515Transmit(MCP_RESET);
    _delay_ms(0.408);
    PORTB |= (1 << CAN_CS);
    return 0;
}


uint8_t mcp2515Init ()
{
    mcp2515Reset(); // Send reset - command
    // Self - test
    _delay_ms(10);
    uint8_t value = mcp2515Read(MCP_CANSTAT);
    //printf("VALUE IS: %d\n", value);
    if ((value & MODE_MASK) != MODE_CONFIG) {
        //printf("MCP2515 is NOT in configuration mode after reset!\n");
        return 1;
    }
    else {
        //printf("MCP2515 is in configuration mode after reset!\n");
    }
    // More initialization
    return 0;
}

void mcp2515Write(uint8_t address, uint8_t data) {

    PORTB &= ~(1 << CAN_CS);
    SPIMCP2515Transmit(0b00000010);
    SPIMCP2515Transmit(address);
    SPIMCP2515Transmit(data);
    PORTB |= (1 << CAN_CS);
}

void mcp2515BitModify(uint8_t address, uint8_t mask, uint8_t data) {

    PORTB &= ~(1 << CAN_CS);
    SPIMCP2515Transmit(0b00000101);
    SPIMCP2515Transmit(address);
    SPIMCP2515Transmit(mask);
    SPIMCP2515Transmit(data);
    PORTB |= (1 << CAN_CS);
}

uint8_t mcp2515ReadStatus () {
    PORTB &= ~(1 << PB0);
    SPIMCP2515Transmit(MCP_READ_STATUS);
    uint8_t result = SPIMCP2515Receive();
    PORTB |= (1 << PB0);
}

void mcp2515RTSAll() {
    PORTB &= ~(1 << PB0);
    SPIMCP2515Transmit(MCP_RTS_ALL);
    PORTB |= (1 << PB0);
}

void mcp2515RTSBuffer0() {
    PORTB &= ~(1 << PB0);
    SPIMCP2515Transmit(0b10000001);
    PORTB |= (1 << PB0);
}

void mcp2515RTS(uint8_t select) {
    PORTB &= ~(1 << PB0);
    if(select == 0)
        SPIMCP2515Transmit(MCP_RTS_TX0);
    else if(select == 1)
        SPIMCP2515Transmit(MCP_RTS_TX1);
    else if(select == 2)
        SPIMCP2515Transmit(MCP_RTS_TX2);
    else
        SPIMCP2515Transmit(MCP_RTS_ALL);
    PORTB |= (1 << PB0);
}