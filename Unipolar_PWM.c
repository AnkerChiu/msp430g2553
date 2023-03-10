#include <msp430g2553.h>
unsigned int frequency=20e3; 
unsigned int Vc = 20;
unsigned int TA1CCR0_value;
unsigned int TA1CCR1_value;
unsigned int TA1CCR2_value;

void main(void)
{
 //stop watchdog
 WDTCTL = WDTPW | WDTHOLD;

 //set SMCLK=16MHz
 DCOCTL = 0;
 BCSCTL1 = CALBC1_16MHZ;
 DCOCTL = CALDCO_16MHZ;

 //set I/O
 P2DIR |= (BIT1+BIT4);
 //P2.1=compare Out1 output, P2.4=compare Out2 output
 P2SEL = (BIT1+BIT4); 
 P2SEL2 = ~(BIT1+BIT4); 

 //Set the period in the Timer (up/down counter)
 TA1CCR0_value = 16e6/frequency/2; //16M/20k/2=400
 TA1CCR0=TA1CCR0_value;

 //+Vc
 TA1CCR1_value=TA1CCR0_value/2+(TA1CCR0_value*Vc/200);
 TA1CCR1=TA1CCR1_value;

 //toggle/reset
 TA1CCTL1 = OUTMOD_2;

 //-Vc
 TA1CCR2_value =TA1CCR0_value/2-TA1CCR0_value*Vc/200;
 TA1CCR2=TA1CCR2_value;

 //toggle/reset
 TA1CCTL2 = OUTMOD_2;
 //TASSEL_2 selects SMCLK as the clock source,
 //MC_3-->up/down to the value in TA0CCR0, and start timer.

 TA1CTL = TASSEL_2 + MC_3;
 __bis_SR_register(LPM0_bits); //Switch to low power mode 0.
}
