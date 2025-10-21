//this driver will implement CAN commuication on mcp2515 but should work independent of hardware

#include "can.h"
#include "mcp2515.h"
#include <stdio.h>

void canSetLoopbackMode() {
    mcp2515BitModify(MCP_CANCTRL, 0b11100000, 0b01000000);
}


void canTransmit(uint8_t id, uint64_t data)  {

    /* Setup
- Message ID (TXBnSIDH &
TXBnSIDL)
- Data length (TXBnDLC)
- Data (TXBnDm)
▶ Request-to-send command.
(TXBnCTRL.TXREQ)
▶ Datasheet chapter 3*/

//TXBnSIDH.SID3 |= 0;
//TXBnSIDL.SID3 |= 1;
uint8_t CanCTRL = mcp2515Read(TXB0CTRL);
//printf("CANCTRL: %d\t", CanCTRL);
if ((CanCTRL >> 3) & 1) {
    //printf("TRANSMIT FAIL. BUSY.\n");
    return;
}
//printf("TRANSMITTING....\n");

uint8_t databytes = 0;
while ((1 << BYTE_SIZE*databytes) < data) {
    databytes++; printf("bytes %d\t", databytes); printf("data %d\n", data);}

id = id << 5;
printf("ID %d\n", id);

mcp2515Write(TXB0SIDL, id);
mcp2515Write(TXB0SIDH, 0);
mcp2515Write(TXB0DLC, databytes);

for (uint8_t byte = 0; byte < databytes; byte++) {
    mcp2515Write(TXB0D0+byte, (data >> BYTE_SIZE*byte) & 0xFF);
}


}

uint64_t canReceive() {
    /*Wait for a received message
- Interrupt pin (enable using
CANINTE.RXnIE)
- Read status registers (check
CANINTF.RXnIF)

▶ Read message
- ID (RXBnSIDH & RXBnSIDL)
- Data length (RXBnDLC)
- Data (RXBnDM)
▶ Filter and Masks
- RXBxCTRL.FILHIT<2:0> with
RXFnSIDH, RXMnSIDH. . . .
▶ Datasheet chapter 4 */

mcp2515BitModify(MCP_CANINTE, 0b00000001, 0b00000001); //interrupt enable for receive buffer 0

if ((mcp2515Read(MCP_CANINTF) & 0b00000001) == 1) {//if recieve buffer is ready

    //id
    if (mcp2515Read (MCP_RXB0SIDH) >= 1) {
        printf("too high ID %d, not implemented\n", mcp2515Read(MCP_RXB0SIDH));
    }

    int id = mcp2515Read(RXB0SIDL) & 0b11100000 >> 5; //we only have two nodes, we don't care

    //data length and data
    int dataLength = mcp2515Read(RXB0DLC);
    dataLength = 0x00001111 & dataLength; //only bits 0-3 are the data length size
    uint64_t bytes = 0;

    for (uint8_t byte = 0; byte < dataLength; byte++) {
        bytes += mcp2515Read(RXB0D0+byte) << BYTE_SIZE*byte;
    }

    mcp2515BitModify(MCP_CANINTF,0b00000011, 0);
    return bytes;
}
/*
bytes += mcp2515Read(RXB0D0);
bytes += mcp2515Read(RXB0D1) << BYTE_SIZE;
bytes += mcp2515Read(RXB0D2) << BYTE_SIZE*2;
bytes += mcp2515Read(RXB0D3) << BYTE_SIZE*3;
bytes += mcp2515Read(RXB0D4) << BYTE_SIZE*4;
bytes += mcp2515Read(RXB0D5) << BYTE_SIZE*5;
bytes += mcp2515Read(RXB0D6) << BYTE_SIZE*6;
bytes += mcp2515Read(RXB0D7) << BYTE_SIZE*7;
*/
//mcp2515BitModify(MCP_RXF0SIDH, 0b10000000, 0b100000000);




}

