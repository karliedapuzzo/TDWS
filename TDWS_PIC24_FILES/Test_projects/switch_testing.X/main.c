/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.5
        Device            :  PIC24FJ128GL306
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB 	          :  MPLAB X v6.05
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"

/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    IO_RE2_SetDigitalInput();
    uint16_t wait = 0;
    while(wait < 0x00ff)
    {
        wait ++;
    }

    while (1)
    {
        uint16_t wait = 0;
        
        uint16_t switch0_val;
        uint16_t switch1_val;
        uint16_t switch2_val;
        uint16_t switch3_val;
        uint16_t switch_mode;
        
        while(wait < 0xffff)
        {
            wait++;
        }
        
        //weak pull up (WPU in pin module) resistors need to be enabled for these to function properly
        switch0_val = IO_RE2_GetValue();
        switch1_val = IO_RE3_GetValue();
        switch2_val = IO_RE4_GetValue();
        switch3_val = IO_RE5_GetValue();
        
        switch0_val = (~switch0_val & 0x0001);
        switch1_val = (~switch1_val & 0x0001) << 1;
        switch2_val = (~switch2_val & 0x0001) << 2;
        switch3_val = (~switch3_val & 0x0001) << 3;
        
        switch_mode = (switch0_val | switch1_val | switch2_val | switch3_val);
        
        
        if(switch_mode > 0)
        {
            continue;
        }
        else
        {
            wait=0;
        }
        
    }

    return 1;
}
/**
 End of File
*/

