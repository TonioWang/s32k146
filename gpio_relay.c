/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * NXP Confidential. This software is owned or controlled by NXP and may only be
 * used strictly in accordance with the applicable license terms. By expressly
 * accepting such terms or by downloading, installing, activating and/or otherwise
 * using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software. The production use license in
 * Section 2.3 is expressly granted for this software.
 */

#include "FreeRTOS.h"
#include "sdk_project_config.h"

#include <stdint.h>
#include <stdbool.h>
#include "S32K146.h"  // Include the S32K146 specific header file

void rtos_start(void);

//In the PTA0 example, RELAY_PIN is defined as 0.
//In the PTA4 example, RELAY_PIN is defined as 4.

//https://cache.nxp.com/secured/assets/downloads/en/schematics/S32K146EVB-SPF-29844-RA.pdf?fileExt=.pdf
#define RELAY_PIN_1 1  // Example: Relay connected to PTA0 (Pin 0 of Port A)
#define RELAY_PIN_2 2  // Example: Relay connected to PTA1 (Pin 1 of Port A)
#define RELAY_PIN_3 3  // Example: Relay connected to PTA2 (Pin 2 of Port A)

void delay(uint32_t ms) {
    volatile uint32_t i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 7000; j++) {
            __asm("nop");
        }
    }
}


/** GPIO - Register Layout Typedef
typedef struct {
  __IO uint32_t PDOR;                              < Port Data Output Register, offset: 0x0
  __O  uint32_t PSOR;                              < Port Set Output Register, offset: 0x4
  __O  uint32_t PCOR;                              < Port Clear Output Register, offset: 0x8
  __O  uint32_t PTOR;                              < Port Toggle Output Register, offset: 0xC
  __I  uint32_t PDIR;                              < Port Data Input Register, offset: 0x10
  __IO uint32_t PDDR;                              < Port Data Direction Register, offset: 0x14
  __IO uint32_t PIDR;                              < Port Input Disable Register, offset: 0x18
} GPIO_Type, *GPIO_MemMapPtr;
 */

int main(void) {
    // Initialize GPIO pin for relay control
    PCC->PCCn[PCC_PORTA_INDEX] |= PCC_PCCn_CGC_MASK; // Enable clock for PORTA
    PORTA->PCR[RELAY_PIN_1] |= PORT_PCR_MUX(1);        // Set pin PTA0 as GPIO
    PORTA->PCR[RELAY_PIN_2] |= PORT_PCR_MUX(1);        // Set pin PTA0 as GPIO
    PORTA->PCR[RELAY_PIN_3] |= PORT_PCR_MUX(1);        // Set pin PTA0 as GPIO
    PTA->PDDR |= (1U << RELAY_PIN_1);                   // Set PTA0 as output
    PTA->PDDR |= (1U << RELAY_PIN_2);                   // Set PTA0 as output
    PTA->PDDR |= (1U << RELAY_PIN_3);                   // Set PTA0 as output

    while (1) {
        // Activate relay (assuming active high configuration)
        PTA->PSOR = (1U << RELAY_PIN_1);  // Set PTA0 (output high)
        PTA->PSOR = (1U << RELAY_PIN_2);  // Set PTA0 (output high)
        PTA->PSOR = (1U << RELAY_PIN_3);  // Set PTA0 (output high)
        delay(2000);                    // Delay for 1 second

        // Deactivate relay
        PTA->PCOR = (1U << RELAY_PIN_1);  // Clear PTA0 (output low)
        PTA->PCOR = (1U << RELAY_PIN_2);  // Clear PTA0 (output low)
        PTA->PCOR = (1U << RELAY_PIN_3);  // Clear PTA0 (output low)
        delay(2000);                    // Delay for 1 second
    }

    return 0;
}
