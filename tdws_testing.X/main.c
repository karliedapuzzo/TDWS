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
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.4
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

/*
                         Main application
 */

//bit 7 is read bit, bit 5-0 is address of register
#define mag_x_regl 0x2800
#define mag_x_regh 0x2900
#define mag_y_regl 0x2a00
#define mag_y_regh 0x2b00
#define mag_z_regl 0x2c00
#define mag_z_regh 0x2d00

#define status_reg 0x2700
#define ctrl_3_reg 0x2200

#define dummy_reg 0x0fff;
#define read_bit 0x8000;
#define write_bit 0x0000;

uint16_t read_h;
uint16_t read_l;
uint16_t read_x;
uint16_t neg = 0;

void magISR(void)
{
    
    uint16_t wait;
    uint16_t send_addr = mag_x_regl | read_bit;
    SS1_SetLow();
    SPI1_Exchange16bitBuffer(&send_addr, 2, &read_l);
    SS1_SetHigh();

    wait = 0x00ff;
    send_addr = mag_x_regh | read_bit;
    while(wait > 0)
    {
        wait -= 1;
    }
    SS1_SetLow();
    SPI1_Exchange16bitBuffer(&send_addr, 2, &read_h);
    SS1_SetHigh();
    
    read_h = read_h << 8;
    
    read_x = read_h | read_l;
    
    if((0x8000 & read_x) == 0x8000)
    {
        read_x -= 1;
        read_x = ~read_x;
        neg = 1;
    }
    else
    {
        neg = 0;
    }
    
}

//write command

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    TMR1_SetInterruptHandler (magISR);

    uint16_t wait;
    uint16_t garbage;
    
    //config mag to run in continuous conversion mode
    uint16_t conf_cmd = ctrl_3_reg;
    SS1_SetLow();
    SPI1_Exchange16bit(conf_cmd);
    SS1_SetHigh();
    
    INTERRUPT_GlobalEnable();
    
    while (1)
    {
        wait = 0x00ff;
        while(wait > 0)
        {
            wait -= 1;
        }
    }

    return 1;
}
/**
 End of File
*/

