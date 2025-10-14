//this driver is made for setting up and changing the mcp2515 CAN controller's control and status registers

#include "mcp2515.h"

//#define CAN_CS 3

/*
uint8_t mcp2515Read (uint8_t address )
{
uint8_t result ;
PORTB &= ~(1 < < CAN_CS ) ; // Select CAN - controller
SPI_write ( MCP_READ ) ; // Send read instruction
SPI_write ( address ) ; // Send address
result = SPI_read () ; // Read result
PORTB |= (1 < < CAN_CS ) ; // Deselect CAN - controller
return result ;
}


uint8_t mcp2515Init ()
{
uint8_t value ;
SPI_init () ; // Initialize SPI
mcp2515_reset () ; // Send reset - command
// Self - test
mcp2515_read ( MCP_CANSTAT , & value ) ;
if (( value & MODE_MASK ) != MODE_CONFIG ) {
printf ( " MCP2515 is NOT in configuration mode after reset !\ n " ) ;
return 1;
}
// More initialization
return 0;
}
*/

void mcp2515Write(uint8_t address, uint8_t data) {

    SPIMasterTransmit(0b00000010, 3);
    SPIMasterTransmit(address, 3);
    SPIMasterTransmit(data, 3);
}

void mcp2515BitModify(uint8_t address, uint8_t mask, uint8_t data) {

    SPIMasterTransmit(0b00000101, 3);
    SPIMasterTransmit(address, 3);
    SPIMasterTransmit(mask, 3);
    SPIMasterTransmit(data, 3);
}

void mcp2515Transmit(uint8_t data) {

    //CANINTEXinE
    //txbnctrl txreq
    //if (TxBnCTRL & (1 << TXREQ)) {

    //}

    //CP_RXF0SIDH

// sendes med spi dXBnCTRL.TXREQ bi //means ready for transmission

//message ID
//TXBnSIDH
//TXBnSIDL

//data length
//TXBnDm


//CAninte.TXniE is set interrupt

}