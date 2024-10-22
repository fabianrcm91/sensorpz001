#include "mod_radio.h"

uint8_t radioInit(uint32_t varFreq,uint16_t varChannel){
    if (sx1278Init(varFreq)) {//7094272
        uartPuts("\n\r Radio Lora - Ok...\n\r");
        sx1278SetSyncWord(0xF1);
        sx1278SendExplicitPacket((uint8_t*)"Hola", 4);
        pinDigToggle(LedS2);
        delay_ms(500);
        pinDigToggle(LedS2);
        delay_ms(500);
        pinDigToggle(LedS2);
        delay_ms(500);
        pinDigToggle(LedS2);
        delay_ms(500);
        return 1;
    }

    uint8_t version=0;
    version = sx1278RegRead(REG_VERSION);
    char varBuffer[10]=" ";
    printInt16(varBuffer,version);
    uartPuts(" \n\r Version: ");uartPuts(varBuffer);
    uartPuts("\n\r Radio Lora - Failed...\n\r");
    return 0;
}

uint8_t radioGet(uint8_t* bufIn,uint8_t rfLocalAddress){
    uint8_t datasIn = sx1278IsDataAvailable(sx1278Header_Explicit);
    if (datasIn) {
        uint8_t bfIn[64];
        sx1278GetPacket(bfIn, datasIn);
        if (bfIn[0] == rfLocalAddress) {
            pinDigHigh(LedS2);
            bfIn[datasIn] = 0;
            uartPuts("\n\r Hi: ");
            uartPutc(bfIn[0]);
            uartPuts(", from: ");
            uartPutc(bfIn[1]);
            uartPuts(" , with Id: ");
            uartPutc(bfIn[3]);
            uartPuts(" . \n\r");
            uint8_t var;
            for (var = 4; var < (datasIn); var++) {
                uartPutc(bfIn[var]);
            }
        }else{
            datasIn = 0;
        }
        //    uartPuts("Lora message is not for me: ");
        //}
        //uartPuts((char*)bfIn);
    }
    return datasIn;
}
uint8_t radioPut(uint8_t* bufIn, uint8_t varSize,uint8_t rfLocalAddress,uint8_t rfDestAddress){
    uint8_t bufOut[64];
    bufOut[0] = rfDestAddress;
    bufOut[1] = rfLocalAddress;
    bufOut[2] = 0;
    bufOut[3] = varSize;
    uint8_t var;
    for (var = 0; var < varSize; var++) {
        bufOut[var+4] = bufIn[var];
    }

    sx1278BeginPacket(sx1278Header_Explicit);

    uint8_t varOut = sx1278PutBuffer(bufOut, varSize+4);
    sx1278EndPacket();
    return varOut;
}
