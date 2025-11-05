//this driver will implement CAN commuication on mcp2515 but should work independent of hardware

#include "can.h"
#include "mcp2515.h"
#include <stdio.h>
#define CAN_CS PB0

#define MCP_16MHz_250kBPS_CFG1 (0x41)
#define MCP_16MHz_250kBPS_CFG2 (0xF1)
#define MCP_16MHz_250kBPS_CFG3 (0x85)

void canInit(canTimeConfig tConfig) {
    uint8_t phase2 = tConfig.phaseSeg2 & 0b111;         // Phase 2 segment
    uint8_t propag = tConfig.propagTimeSeg & 0b111;     // Propagation time segment
    uint8_t phase1 = tConfig.phaseSeg1 & 0b111;         // Phase 1 segment
    uint8_t sjw = tConfig.syncJumpWidth & 0b11;         // Synchronization jump width
    uint8_t brp = tConfig.baudRatePrescaler & 0b111111; // Baud rate prescaler
    uint8_t smp = tConfig.samplingMode & 0b1;           // Sampling mode

    uint8_t cnf1 = brp + (sjw << 6);
    uint8_t cnf2 = propag + (phase1 << 3) + (1 << 7);
    uint8_t cnf3 = phase2;

    //printf("CNF1 %d\n",cnf1);
    //printf("CNF2 %d\n",cnf2);
    //printf("CNF3 %d\n",cnf3);

    uint8_t value = mcp2515Read(MCP_CANSTAT);
    if ((value & MODE_MASK) == MODE_CONFIG) {
        //printf("MCP2515 is in configuration mode during initialization\n");
    }

    mcp2515Write(MCP_CNF3, MCP_16MHz_250kBPS_CFG3);
    mcp2515Write(MCP_CNF2, MCP_16MHz_250kBPS_CFG2);
    mcp2515Write(MCP_CNF1, MCP_16MHz_250kBPS_CFG1);

    
    //printf("cnf1 %d\n",mcp2515Read(MCP_CNF1));
    //printf("cnf2 %d\n",mcp2515Read(MCP_CNF2));
    //printf("cnf3 %d\n",mcp2515Read(MCP_CNF3));
    

    canSetNormalMode();
}

void canSetLoopbackMode() {
    mcp2515BitModify(MCP_CANCTRL, 0b11100000, 0b01000000);
}

void canSetNormalMode() {
    mcp2515BitModify(MCP_CANCTRL, 0b11100000, 0b00000000);
}


void canTransmit(frame message) {


 //init message transmission
    /* Setup
- Message ID (TXBnSIDH &
TXBnSIDL)
- Data length (TXBnDLC)
- Data (TXBnDm)
▶ Request-to-send command.
(TXBnCTRL.TXREQ)
▶ Datasheet chapter 3*/

//TXBnSIDH.SID3 |= 0;mcp2515RTSAll();
//TXBnSIDL.SID3 |= 1;

//printf("TRANSMITTING....\n");
//mcp2515RTSAll();

uint8_t CanCTRL = mcp2515Read(TXB0CTRL);
printf("%d\n",CanCTRL);
//printf("CANCTRL: %d\t", CanCTRL);
if ((CanCTRL >> 3) & 1) {
    printf("TRANSMIT FAIL. BUSY.\n");
    return;
}

uint8_t databytes = message.length;

uint8_t dlc = (message.length > 8) ? 8 : message.length;

printf("ID %d\n", message.id);

PORTB &= ~(1 << CAN_CS);
SPIMCP2515Transmit(MCP_LOAD_TX0);
SPIMCP2515Transmit((message.id >> 3) & 0xFF); //high
SPIMCP2515Transmit((message.id << 5) & 0b11100000); //low
SPIMCP2515Transmit(0x00);
SPIMCP2515Transmit(0x00);
SPIMCP2515Transmit(message.length & 0x0F);

for (uint8_t byte = 0; byte < dlc; byte++) {
    //mcp2515Write(TXB0D0+byte, message.data[byte]);
    SPIMCP2515Transmit(message.data[byte]);
}

PORTB |= (1 << CAN_CS);
printf("data: %d\n", mcp2515Read(TXB0D0));

mcp2515RTS(0);
//mcp2515BitModify(TXB0CTRL, 0b00000100, 0);
//mcp2515BitModify(MCP_CANINTF, 0b00000011, 0);


    

}

frame canReceive() {
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

        frame m;

        if (mcp2515Read (MCP_RXB0SIDH) >= 1) {
            printf("too high ID %d, not implemented\n", mcp2515Read(MCP_RXB0SIDH));
        }

        m.id = (mcp2515Read(RXB0SIDL) & 0b11100000) >> 5; //we only have two nodes, we don't care

        //data length and data
        m.length = 0b00001111 & mcp2515Read(RXB0DLC); //only bits 0-3 are the data length size

        for (uint8_t byte = 0; byte < m.length; byte++) {
            m.data[byte] = mcp2515Read(RXB0D0+byte);
        }

        mcp2515BitModify(MCP_CANINTF, 0b00000011, 0);

        
        return m;
    }
/*
bytes += mcp2515Read(RXB0D0);
bytes += mcp2515Read(RXB0D1) << BYTE_SIZE;
bytes += mcp2515Read(RXB0D2) << BYTE_SIZE*2;
bytes += mcp2515Read(RXB0D3) << BYTE_SIZE*32515BitModify(MCP_CANINTF,0b00000011, 0);;
bytes += mcp2515Read(RXB0D4) << BYTE_SIZE*4;
bytes += mcp2515Read(RXB0D5) << BYTE_SIZE*5;
bytes += mcp2515Read(RXB0D6) << BYTE_SIZE*6;
bytes += mcp2515Read(RXB0D7) << BYTE_SIZE*7;
*/
//mcp2515BitModify(MCP_RXF0SIDH, 0b10000000, 0b100000000);
}

