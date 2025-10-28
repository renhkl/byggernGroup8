//this driver will implement CAN commuication on mcp2515 but should work independent of hardware

#include "can.h"
#include "mcp2515.h"
#include <stdio.h>

void canInit(canTimeConfig tConfig) {
/*
    uint8_t phase2 = tConfig.phaseSeg2 & 0b111;         // Phase 2 segment
    uint8_t propag = tConfig.propagTimeSeg & 0b111;     // Propagation time segment
    uint8_t phase1 = tConfig.phaseSeg1 & 0b111;         // Phase 1 segment
    uint8_t sjw = tConfig.syncJumpWidth & 0b11;         // Synchronization jump width
    uint8_t brp = tConfig.baudRatePrescaler & 0b111111; // Baud rate prescaler
    uint8_t smp = tConfig.samplingMode & 0b1;           // Sampling mode

    uint8_t cnf1 = brp + sjw << 6;
    uint8_t cnf2 = propag + phase1 << 3 + 1 << 7;
    uint8_t cnf3 = phase2;
    mcp2515Write(MCP_CNF1, cnf1);
    mcp2515Write(MCP_CNF2, cnf2);
    mcp2515Write(MCP_CNF3, cnf3);
    */

    canSetNormalMode();
}

void canSetLoopbackMode() {
    mcp2515BitModify(MCP_CANCTRL, 0b11100000, 0b01000000);
}
void canSetNormalMode() {
    mcp2515BitModify(MCP_CANCTRL, 0b11100000, 0b00000000);
}

void canTransmit(uint8_t id, uint8_t data)  {
    //printf("TRANSMITting.\n");

//mcp2515RTSAll(); //init message transmission
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

//printf("TRANSMITTING.... %d\n",data);
uint8_t databytes = 1;
//while ((1 << BYTE_SIZE*databytes) < data && databytes < 8) {databytes++;}

printf("ID %d\t", id);
id = id << 5;


mcp2515Write(TXB0SIDL, id);
mcp2515Write(TXB0SIDH, 0);
mcp2515Write(TXB0DLC, databytes);
printf("-------------------- %x -------------------------\n", mcp2515Read(TXB0SIDL));

/*
for (uint8_t byte = 0; byte < databytes; byte++) {
    mcp2515Write(TXB0D0+byte, (data >> BYTE_SIZE*byte) & 0xFF);
}
    */
mcp2515Write(TXB0D0, data & 0xFF);

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


if ((mcp2515Read(MCP_CANINTF) & 0b00000001) == 0) {//if recieve buffer is ready

    //idsss
    if (mcp2515Read (MCP_RXB0SIDH) >= 1) {
        //printf("too high ID %d, not implemented\n", mcp2515Read(MCP_RXB0SIDH));
    }

    int id = mcp2515Read(RXB0SIDL) & 0b11100000 >> 5; //we only have two nodes, we don't care
    //printf("ID %d\t",id);

    //data length and data
    int dataLength = mcp2515Read(RXB0DLC);
    printf("dataLength %u\t",dataLength);
    dataLength = 0b00001111 & dataLength; //only bits 0-3 are the data length size
    

    uint64_t bytes = 0;
    uint64_t rsh;

    for (uint8_t byte = 0; byte < dataLength; byte++) {
        rsh = BYTE_SIZE*byte;
        bytes += mcp2515Read(RXB0D0+byte) << rsh;
    }

    mcp2515BitModify(MCP_CANINTF, 0b00000011, 0);

    
    return bytes;
}

else {
    return 12345;
}

//mcp2515BitModify(MCP_RXF0SIDH, 0b10000000, 0b100000000);




}

