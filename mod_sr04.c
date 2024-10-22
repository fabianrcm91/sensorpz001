#include "mod_sr04.h"

#define pulseTrigger 20*DCO_frec_MHz

void sr04Init(void){
    pinModeOutput(SR1_Tr);pinDigLow(SR1_Tr);
    pinModeOutput(SR2_Tr);pinDigLow(SR2_Tr);
    pinModeInput(SR1_Ec);//pinEnaPullDown(SR1_Ec);
    pinModeInput(SR2_Ec);//pinEnaPullDown(SR2_Ec);

    timerA0InitCounter(TimerACLKSource_SMCLK+TimerACLKDivBy_8);
}
void sr04GetCm(uint16_t* pBuffer){
    sr04GetUs(pBuffer);
    pBuffer[0] = (uint16_t)(0.0343*(float)pBuffer[0]);//(343m/s)/(1000000us/s)*(100cm/m)=0.0343
    pBuffer[1] = (uint16_t)(0.0343*(float)pBuffer[1]);//(343m/s)/(1000000us/s)*(100cm/m)=0.0343
}
void sr04Print(uint16_t* pBuffer){
    char varBuffer[10]=" ";
    printInt16(varBuffer,pBuffer[0]);
    uartPuts(" \n\rSensor1: ");uartPuts(varBuffer);
    printInt16(varBuffer,pBuffer[1]);
    uartPuts(" \n\rSensor2: ");uartPuts(varBuffer);
}
void sr04GetUs(uint16_t* pBuffer){
    pinDigHigh(SR1_Tr);__delay_cycles(pulseTrigger);pinDigLow(SR1_Tr);
    TA0R = 0;
    while((TA0R<65000) && (pinDigRead(SR1_Ec)==0)){}
    //while(pinDigRead(SR1_Ec)==0){}
    TA0R=0;
    while((TA0R<65000) && (pinDigReadWith(SR1_Ec,1))){}
    pBuffer[0] = TA0R;// 1/4 : 1/2 porque cada flanco son 0.5us y 1/2 porque el tiempo medido es ida y vuelta
    pBuffer[0] = (pBuffer[0])>>2;

    delay_ms(30);
    pinDigHigh(SR2_Tr);__delay_cycles(pulseTrigger);pinDigLow(SR2_Tr);
    TA0R = 0;
    while((TA0R<65000) &&  (pinDigRead(SR2_Ec)==0)){}
    //pBuffer[1] = TA0R;
    TA0R=0;
    while((TA0R<65000) && (pinDigReadWith(SR2_Ec,1))){}
    pBuffer[1] = TA0R;
    pBuffer[1] = (pBuffer[1])>>2;

}
