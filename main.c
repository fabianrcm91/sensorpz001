#include "mod_library/mod_pins_io.h"
#include "mod_libs.h"
#include "mod_user_defs.h"
#include "msp430g2553.h"
#include <stdint.h>


int main(void) {
    WDTCTL = WDTPW + WDTHOLD + WDTNMI + WDTNMIES;
    IE1 |= NMIIE;
    P1DIR = 0;P2DIR = 0;P3DIR = 0;
    __delay_cycles(1000000);

    loadBasicSetUp();

    pinModeOutput(LedS1);
    pinModeOutput(LedS2);
    pinDigLow(LedS1);
    pinDigLow(LedS2);
  





    uartSetup(baudrate_9600,DATA_8BITS+PARITY_DIS+STOP_1BIT);
    uartPuts("\n\r Testing Uart2...");
    delay_ms(10);
    uint8_t i=0;
/*
    bitLow(P1DIR,0b11110000);
    bitHigh(P1REN,0b11110000);
    for (i=5; i>0; i--){
        __delay_cycles(500000*DCO_frec_MHz);
        bitToggle(P1OUT,0b11110000);
        bitToggle(P3OUT,BIT4+BIT5);
        uartPuts(" .");
    }
*/
    uartPuts(" Testing Finished...\n\r");
    sr04Init();

    radioInit(7094272, 1);//7094272/0.016384=433000000
    delay_ms(10);

    //uartSoftSetup();
    /*bitLow(P1DIR,0xF0);bitHigh(P3DIR,BIT7);
    bitHigh(P1REN,0xF0);//bitHigh(P3REN,BIT7);
    bitLow(P1OUT,0xF0);bitLow(P3OUT,BIT7);*/

    uint16_t bufferCm[2]={12000,12000};uint16_t delta1=0,delta2=0;
    uint8_t buffLora[64], thKntSD=0;
    uint8_t cntOcupado=0;
    while(-1){

        //uartPuts(" ...Hi... ");

        //uartSoftPuts(" ...Pin Ok... ");


        if ((bufferCm[1]<60)||(bufferCm[0]<60)){
            cntOcupado++;
        }else{
            cntOcupado=0;
        }

        if (radioGet(buffLora,LOCAL_ADD)) {
            if (cntOcupado>15) {
                cntOcupado = 15;
                //uartPuts("\n\r Espacio Ocupado \n\r");
                radioPut("Ocupado", 7, LOCAL_ADD, GW_ADDRESS);
            }else {
                //uartPuts("\n\r Espacio Desocupado \n\r");
                radioPut("Desocupado", 10, LOCAL_ADD, GW_ADDRESS);
            }
        }else {
            sr04GetCm(bufferCm);
            thKntSD++;
        }
        pinDigLow(LedS2);
        //bitToggle(P1OUT,0XF0);

        if (thKntSD>10) {
            thKntSD=0;
            sr04Print(bufferCm);
            pinDigToggle(LedS1);
            //radioPut("LoraTx", 6, LOCAL_ADD, GW_ADDRESS);
        }



    }
}


#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR_HOOK(void){
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR_HOOK(void){
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR_HOOK(void){
}

#pragma vector=NMI_VECTOR
__interrupt void NMI_ISR_HOOK(void){
    IFG1 &= ~NMIIFG;                          // Reclear NMI flag in case bounce
    IE1 |= NMIIE;                             // Enable NMI
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR_HOOK(void){
}
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR_HOOK(void){

}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMERA0_ISR_HOOK(void){

}


#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMERA1_ISR_HOOK(void){

}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR_HOOK(void){
}
