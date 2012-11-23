#include <msp430g2553.h>
 
__attribute__((interrupt(TIMER0_A0_VECTOR)))
void CCR0_ISR(void) {
    P1OUT ^= BIT6 | BIT0;
}
 
__attribute__((interrupt(PORT1_VECTOR)))
void BtnClick(void) {
    if( P1IFG & BIT3 ) {
        P1IFG &= ~BIT3;
 
        P1DIR ^= BIT6|BIT0;
    }
}
 
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
 
    TACCR0 = 62500 - 1;
    TACCTL0 = CCIE;
    TACTL = TASSEL_2 + ID_3 + MC_1 + TACLR;
    P1REN = BIT3;
    P1DIR = BIT6;
    P1OUT = BIT6|BIT3;
    P1IES |= BIT3;
    P1IFG &= ~BIT3;
    P1IE |= BIT3;
 
    __enable_interrupt();
 
    for(;;);
}

