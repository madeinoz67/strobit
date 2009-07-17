/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief
 *      This file implements a beep function to emit a beep sound from Raven's
 *      speaker.
 *
 * \par Application note:
 *      User's Guide to be published.
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 *
 * $Name: RUM_REL_1_0_0_A $
 * $Revision: 1.3 $
 * $RCSfile: beep.c,v $
 * $Date: 2009/05/28 22:45:07 $  \n
 * $Id: beep.c,v 1.3 2009/05/28 22:45:07 bleverett Exp $
 ******************************************************************************/
/* Copyright (c) 2008  ATMEL Corporation
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/


#include "beep.h"
#include "key.h"
#include "lcd.h"

/**
   @addtogroup lcd
   @{
*/

/**
   @brief Emits a beep from the Raven's buzzer.  This routine simply toggles a port pin
   at an audio frequency, which causes a tone to be emitted from the Raven's speaker.
   The beep consists of two tones at two different frequencies.  At the end, the beeper
   port pin is kept low, which causes the speaker amplifier to shut down.
*/
void beep(void)
{
    uint8_t i;
    volatile uint8_t j;

    // LED on
    led_on();

    // Turn on the power to the speaker.
    ENTER_DDR |= (1 << PE7);
    ENTER_PUR |= (1 << PE7);

    // Setup port pin
    BEEP_DDR |= (1 << BEEP_BIT);

    for (i=0;i<100;i++)
    {
        // Toggle port pin
        BEEP_PIN |= (1 << BEEP_BIT);
        // Delay loop
        for (j=0;j<0xff;j++)
            ;
    }

    for (i=0;i<100;i++)
    {
        // Toggle port pin
        BEEP_PIN |= (1 << BEEP_BIT);
        // Delay loop
        for (j=0;j<0xa0;j++)
            ;
    }

    // Turn off speaker
    BEEP_PORT &= ~(1 << BEEP_BIT);
    ENTER_PUR &= ~(1 << PE7);


    // LED off
    led_off();

}

/// @}
