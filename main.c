#include <msp430.h>

unsigned int ADC_Value;
unsigned int v;
unsigned int x;
void adcInit();
void gpioInit();
void uartInit();
void main(void)
{
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;
    adcInit();
    gpioInit();
    uartInit();

    while(1)
        {
        while(x<20000) // collect data for 20 seconds
               {
                   if( ADC_Value >= v){ //Compare ADC value to V, if it is higher or equal
                   P6OUT |= BIT0; // turn buzzer on
                   }
                   if(!(P2IN & BIT3)){ //if button is pressed
                       P6OUT &= ~BIT0; // turn buzzer off
                   }
               x++; // add 1 to x
               __delay_cycles(10000) ;
               }
               ADCCTL0 |= ADCENC | ADCSC; //Enable and start conversion
               while((ADCIFG & ADCIFG0) == 0); // wait to complete
               ADC_Value = ADCMEM0; // read adc results and assign them
               UCA1TXBUF = ADC_Value; //Send the ADC value through Tx
        }

}



void gpioInit(void) {
    // Configure PIR pin as input
       P1DIR &= ~BIT3;

       // Configure buzzer pin as output
      // P6DIR |= BIT0;
       P6OUT &= ~BIT0;

       // Configure button pin as input with pull-up resistor
       P2DIR &= ~BIT3;
       P2OUT |= BIT3;
       P2REN |= BIT3;

       P6DIR |= BIT6;              // Configure P6.6 to an Output
}
void adcInit(void) {
    ADCCTL0 &= ~ADCENC;             // Disable ADC

    ADCCTL1 |= ADCSHP;              // Enable sample timer
    ADCCTL1 |= ADCSSEL_2;           // Set ADC clock source to SMCLK (16 MHz)
    ADCCTL2 |= ADCRES;              // Set ADC resolution to 10 bits
    ADCMCTL0 |= ADCINCH_11;         // Set ADC input channel to A11

    ADCCTL0 |= ADCMSC;              // Enable multiple sample and conversion mode
    ADCCTL0 |= ADCSHT_4;            // Set sample-and-hold time to 16 clock cycles

    ADCIE |= ADCIE0;                // Enable ADC interrupt

    ADCCTL0 |= ADCENC;              // Enable ADC
}

void uartInit(void){
    UCA1CTLW0 |= UCSWRST; // PUT A1 INTO SW RESET
    UCA1CTLW0 |= UCSSEL__SMCLK;
    UCA1BRW = 8;
    UCA1MCTLW = 0XD600;

    P4SEL1 &= ~BIT2;
    P4SEL0 |= BIT2;

    PM5CTL0 &= ~LOCKLPM5;
    UCA1CTLW0 &= ~UCSWRST;
}
