#include <msp430g2553.h>

int Tx=9;
int Rx;

void main(void)
{
 WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
//**SMCLK=16MHz**
 DCOCTL = 0;
 BCSCTL1 = CALBC1_16MHZ;
 DCOCTL = CALDCO_16MHZ;
//**1.Set UCSWRST(Enabled. USCI logic held in reset state)**
 UCA0CTL1 |= UCSWRST;
//**2.Initialize all USCI registers with UCSWRST = 1 (including UCAxCTL1)
 //UCA0CTL0=0x00（No parity check, LSB first, 8bit, 1stop bit, UART, Asynchonous）
 UCA0CTL0 &= ~( UCPEN+UCPAR+UCMSB+UC7BIT+UCSPB+UCMODE1+UCMODE0+UCSYNC);
 //UCA0CTL1(SMCLK)
 UCA0CTL1|= UCSSEL1;
 UCA0CTL1&= ~( UCSSEL0+UCRXEIE+UCBRKIE+UCDORM+UCTXADDR+UCTXBRK);
 //UCOS16=1
 UCA0BR0 = 8;
 UCA0BR1 = 0;
 UCA0MCTL = UCBRS_0 + UCBRF_11 + UCOS16;

//關閉回傳功能
 UCA0STAT &= ~UCLISTEN; //disable feedback
//**3. Configure ports.
P1DIR = 0xFF&(~BIT1) // P1.1 = RXD 設為 input direction,
 // 其餘設為 output direction, P1.2=TXD (output direction)
 P2DIR=0xFF; // P2 設為 output direction
 P3DIR=0xFF; // P3設為output direction
 P1SEL = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
 P1SEL2 = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
//**4.Clear UCSWRST(Disabled. USCI reset released for operation)
 UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
//**5. Enable interrupts (optional) via UCAxRXIE and/or UCAxTXIE
 IE2 &= ~ UCA0TXIE; // Disable USCI_A0TX interrupt
 IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt
 //UCA0TXBUF = Tx; //for master to start UART
 __bis_SR_register(LPM0_bits + GIE); // Enter LPM0, interrupts enabled
}

// USCI A0/B0 Receive ISR
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
 Rx=UCA0RXBUF;
* __no_operation(); //設中斷點觀察Tx及Rx的變化
 UCA0TXBUF = Tx;
 Tx -= 1;
 if(Tx<=-1) Tx=9;
}
//*****************no more****************************