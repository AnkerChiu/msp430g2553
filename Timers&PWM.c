#include <msp430g2553.h>

#define CPU_frequency 16e6 
#define SW_frequency 20e3 
#define Period CPU_frequency/SW_frequency 

unsigned int duty =80; 

void main(void)
{
 //stop watchdog
 WDTCTL = WDTPW | WDTHOLD; 
 
 //set SMCLK=16MHz
 DCOCTL = 0;
 BCSCTL1 = CALBC1_16MHZ;
 DCOCTL = CALDCO_16MHZ;

 //set I/O
 P2DIR |= BIT1;
 P2SEL = BIT1; 
 P2SEL2 = ~BIT1; 

 //set up mode
 TA1CCR0= Period; //週期
 //set duty cycle
 TA1CCR1=Period/100*duty;
 //set output mode-->toggle/set
 TA1CCTL1 = OUTMOD_6;
 //SMCLK,-->up mode,
 //MC_1-->up to the value in TA0CCR0, and start timer.
 TA1CTL = TASSEL_2 + MC_1;
 //Switch to low power mode 0.

 __bis_SR_register(LPM0_bits);
}
