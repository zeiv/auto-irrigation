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

#include "msp430G2553.h"

// Pin Configuration (Defaults in parentheses):
#define WATER BIT6								// Output for water flow control (P1.6)
#define PROBE BIT5								// Input for dryness detection probe (P1.1 = A1)
#define MIN_DRY 0x220							// ADC Input level at which to turn on water flow (0x220)
#define HYDR 0x150								// How much the plant should be watered --CANNOT EXCEED MIN_DRY!-- (0x150)
#define BTN BIT3								// Button for bypassing automatic watering

int bypass;

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;					// Stops WD Timer
	ADC10CTL0 = ADC10SHT_2 + ADC10ON;			// ADC On
//	ADC10CTL1 = CONSEQ_2;
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

	P1DIR |= WATER + BIT0;
	P1DIR &= ~BTN;
	P1IN |= BTN;
	P1IE |= BTN;

	__enable_interrupt();

	for (;;) {
		ADC10CTL0 |= ADC10SC;					// Collects data
		if (bypass == 0x1) {
			P1OUT |= BIT0;
			P1OUT &= ~WATER;
		}
		else if (bypass == 0) {
			P1OUT &= ~BIT0;
		}

		if (ADC10MEM > MIN_DRY && bypass == 0) {				// When the dryness rises above MIN_DRY and Bypass is disabled,
			P1OUT |= WATER;						// Turns on water flow
		}
		else if (ADC10MEM < MIN_DRY - HYDR) {	// When the dryness falls below MIN_DRY minus HYDR
			P1OUT &= ~WATER;					// Turns off water flow
		}
	}

}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
	switch ( bypass ) {
	case 0:
		bypass = 1;
		break;
	case 1:
		bypass = 0;
		break;
	}
	P1IFG &= ~BTN;
}
