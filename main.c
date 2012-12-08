//======================================================================================
// Auto Irrigation
// Water plants automatically based on the dryness of the soil
// or at regular intervals using the MSP430 microprocessor.
//
// Copyright 2012 Xavier Bick
//
// See README.md for usage and configuration instructions
//
// Licensed under the MIT license
//======================================================================================

//#include <msp430g2553.h>
//volatile unsigned int i;
//void main(void)
//{
//WDTCTL = WDT_ADLY_1000; // WDT 1s interval timer
//IE1 |= WDTIE; // Enable WDT interrupt
//P1DIR = 0xFF; // All P1.x outputs
//P1OUT = 0; // All P1.x reset
//P2DIR = 0xFF; // All P2.x outputs
//P2OUT = 0; // All P2.x reset
//BCSCTL3 = XCAP_3; //12.5pF cap- setting for 32768Hz crystal
//// An immediate Osc Fault will occur next
//IE1 |= OFIE; // Enable Osc Fault
//while(1)
//{
//P1OUT ^= 0x01; // Toggle P1.0 using exclusive-OR
//_BIS_SR(LPM3_bits + GIE); // Enter LPM3 w/interrupt
//}
//}
//#pragma vector=WDT_VECTOR
//__interrupt void watchdog_timer (void)
//{
//_BIC_SR_IRQ(LPM3_bits); // Clear LPM3 bits from 0(SR)
//}
//#pragma vector=NMI_VECTOR
//__interrupt void nmi_ (void)
//{
//do
//{
//IFG1 &= ~OFIFG; // Clear OSCFault flag
//for (i = 0xFFF; i > 0; i--); // Time for flag to set
//P1OUT ^= 0x01; // Toggle P1.0 using exclusive-OR
//}
//while (IFG1 & OFIFG); // OSCFault flag still set?
//IE1 |= OFIE; // Enable Osc Fault
//}

#include "msp430G2553.h"

// Pin Configuration (Defaults in parentheses):
#define WATER BIT6								// Output for water flow control (P1.6)
#define PROBE BIT5								// Input for dryness detection probe (P1.1 = A1)
#define MIN_DRY 0x0100							// ADC Input level at which to turn on water flow (0x220)
#define W_TIME 30								// How long to water the plany in seconds
#define DELAY 0.5								// Minimum time to wait between waterings in minutes
#define BTN BIT3								// Button for bypassing automatic watering

int flow = 0;
int bypass;
int i = 0;
int delay = (DELAY*60);

void main(void)
{
//	ADC10CTL1 = CONSEQ_2;
	WDTCTL = WDT_ADLY_1000; // WDT 1s interval timer
	IE1 |= WDTIE; // Enable WDT interrupt
	P1DIR = 0xFF; // All P1.x outputs
	P1OUT = 0; // All P1.x reset
	P2DIR = 0xFF; // All P2.x outputs
	P2OUT = 0; // All P2.x reset
	BCSCTL3 = XCAP_3; //12.5pF cap- setting for 32768Hz crystal
	ADC10CTL0 = ADC10SHT_2 + ADC10ON;			// ADC On
	// An immedate Osc Fault will occur next
	IE1 |= OFIE; // Enable Osc Fault
	switch ( PROBE ) {							// Sets ADC channel to match PROBE
	case BIT0:
		ADC10CTL1 |= INCH_0;
		break;
	case BIT1:
		ADC10CTL1 |= INCH_1;
		break;
	case BIT2:
		ADC10CTL1 |= INCH_2;
		break;
	case BIT3:
		ADC10CTL1 |= INCH_3;
		break;
	case BIT4:
		ADC10CTL1 |= INCH_4;
		break;
	case BIT5:
		ADC10CTL1 |= INCH_5;
		break;
	case BIT6:
		ADC10CTL1 |= INCH_6;
		break;
	case BIT7:
		ADC10CTL1 |= INCH_7;
		break;
	case BIT8:
		ADC10CTL1 |= INCH_8;
		break;
	case BIT9:
		ADC10CTL1 |= INCH_9;
		break;
	case BITA:
		ADC10CTL1 |= INCH_10;
		break;
	case BITB:
		ADC10CTL1 |= INCH_11;
		break;
	case BITC:
		ADC10CTL1 |= INCH_12;
		break;
	case BITD:
		ADC10CTL1 |= INCH_13;
		break;
	case BITE:
		ADC10CTL1 |= INCH_14;
		break;
	case BITF:
		ADC10CTL1 |= INCH_15;
		break;
/*	Adjust the following for additional pins on devices with more than 16 channels
 * 	case BIT0:
 *		ADC10CTL1 |= INCH_0;
		break;
 */
	}
	ADC10AE0 |= PROBE;							// Enables analog input on PROBE
	ADC10CTL0 |= ENC;

	P1DIR |= WATER;
	P1DIR &= ~BTN;
	P1IN |= BTN;
	P1IE |= BTN;
	P1OUT %= ~WATER;							// Water flow initally off

	__enable_interrupt();

	for (;;) {
		ADC10CTL0 |= ADC10SC;					// Collects data
		if (ADC10MEM > MIN_DRY && delay >= (DELAY*60)) {				// When the dryness rises above MIN_DRY and Bypass is disabled,
			P1OUT |= WATER;						// Turns on water flow
			flow = 1;
			delay = 0;
			_BIS_SR(LPM3_bits + GIE); // Enter LPM3 w/interrupt

		}
//		else if (ADC10MEM < MIN_DRY - HYDR) {	// When the dryness falls below MIN_DRY minus HYDR
//			P1OUT &= ~WATER;					// Turns off water flow
//		}
	}

}

#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void)
{
	if (flow == 1) {
		if (i < W_TIME) {
			i++;
		}
		else {
			i = 0;
			flow = 0;
			P1OUT &= ~WATER;
			delay = 0;
			_BIC_SR_IRQ(LPM3_bits); // Clear LPM3 bits from 0(SR)
		}
	}
	else if (flow == 0) {
		delay++;
	}
}
#pragma vector=NMI_VECTOR
__interrupt void nmi_ (void)
{
do
{
IFG1 &= ~OFIFG; // Clear OSCFault flag
for (i = 0xFFF; i > 0; i--); // Time for flag to set
}
while (IFG1 & OFIFG); // OSCFault flag still set?
IE1 |= OFIE; // Enable Osc Fault
}
