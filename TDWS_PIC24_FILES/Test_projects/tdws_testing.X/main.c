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
#include "mcc_generated_files/mcc.h"

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

uint16_t new_mag_data = 0;
uint16_t mag_x_bcd = 0;
uint16_t mag_x_dir = 0;
uint16_t mag_y_bcd = 0;
uint16_t mag_y_dir = 0;
uint16_t mag_z_bcd = 0;
uint16_t mag_z_dir = 0;

void binary_to_bcd(uint16_t *binary_data, uint16_t *bcd_data)
{
    uint16_t ii = 0;
    *bcd_data = 0;
    while(ii < 16)
    {
        uint16_t temp = 0;
        temp = (*binary_data >> (15-ii)) & 1;
        *bcd_data = *bcd_data << 1;
        
        *bcd_data = *bcd_data | temp;
        if((*bcd_data & 0x000F) >= 0x0005) // check 1s place
        {
            *bcd_data += 0x0003;
        }
        if((*bcd_data & 0x00F0) >= 0x0050)
        {
            *bcd_data += 0x0030;
        }
        if((*bcd_data & 0x0F00) >= 0x0500)
        {
            *bcd_data += 0x0300;
        }
        if((*bcd_data & 0xF000) >= 0x5000)
        {
            *bcd_data += 0x3000;
        }
        
        ii += 1;
    }
    //idk y this fixes the problem 
    if((*bcd_data & 0x000F) > 0x0005) // check 1s place
    {
        *bcd_data -= 0x0003;
    }
    if((*bcd_data & 0x00F0) > 0x0050)
    {
        *bcd_data -= 0x0030;
    }
    if((*bcd_data & 0x0F00) > 0x0500)
    {
        *bcd_data -= 0x0300;
    }
    if((*bcd_data & 0xF000) > 0x5000)
    {
        *bcd_data -= 0x3000;
    }
}

void read_xaxis(void)
{
    uint16_t neg;
    uint16_t wait;
    uint16_t read_l;
    uint16_t read_h;
    uint16_t read;
    uint16_t send_addr = mag_x_regl | read_bit;
    SS1_SetLow();
    SPI1_Exchange16bitBuffer(&send_addr, 2, &read_l);
    SS1_SetHigh();
    wait = 0x000f;
    while(wait > 0)
    {
        wait -= 1;
    }
    send_addr = mag_x_regh | read_bit;
    SS1_SetLow();
    SPI1_Exchange16bitBuffer(&send_addr, 2, &read_h);
    SS1_SetHigh();
    
    //put high and low byte into same value
    read_h = read_h << 8;
    read = read_h | read_l;   
    
    //get absolute value of 2s complement number
    if((0x8000 & read) == 0x8000)
    {
        read -= 1;
        read = ~read;
        neg = 1;
    }
    else
    {
        neg = 0;
    }
    //convert number to value in mG
    read = read >> 3; //0.122 mG/b = 1/2^n N = 3
    
    //convert absolute binary value into 4 digit bcd
    //the value we get will be represented in mG
    uint16_t bcd_value;
    
    binary_to_bcd(&read, &bcd_value);
    mag_x_bcd = bcd_value;
    mag_x_dir = neg;
    
    //then send bcd_value over uart to pc
}

void read_yaxis(void)
{
    uint16_t neg;
    uint16_t wait;
    uint16_t read_l;
    uint16_t read_h;
    uint16_t read;
    uint16_t send_addr = mag_y_regl | read_bit;
    SS1_SetLow();
    SPI1_Exchange16bitBuffer(&send_addr, 2, &read_l);
    SS1_SetHigh();
    wait = 0x000f;
    while(wait > 0)
    {
        wait -= 1;
    }
    send_addr = mag_y_regh | read_bit;
    SS1_SetLow();
    SPI1_Exchange16bitBuffer(&send_addr, 2, &read_h);
    SS1_SetHigh();
    
    //put high and low byte into same value
    read_h = read_h << 8;
    read = read_h | read_l;   
    
    //get absolute value of 2s complement number
    if((0x8000 & read) == 0x8000)
    {
        read -= 1;
        read = ~read;
        neg = 1;
    }
    else
    {
        neg = 0;
    }
    //convert number to value in mG
    read = read >> 3; //0.122 mG/b = 1/2^n N = 3
    
    //convert absolute binary value into 4 digit bcd
    //the value we get will be represented in mG
    uint16_t bcd_value;
    
    binary_to_bcd(&read, &bcd_value);
    mag_y_bcd = bcd_value;
    mag_y_dir = neg;
    
    //then send bcd_value over uart to pc
}

void read_zaxis(void)
{
    uint16_t neg;
    uint16_t wait;
    uint16_t read_l;
    uint16_t read_h;
    uint16_t read;
    uint16_t send_addr = mag_z_regl | read_bit;
    SS1_SetLow();
    SPI1_Exchange16bitBuffer(&send_addr, 2, &read_l);
    SS1_SetHigh();
    wait = 0x000f;
    while(wait > 0)
    {
        wait -= 1;
    }
    send_addr = mag_z_regh | read_bit;
    SS1_SetLow();
    SPI1_Exchange16bitBuffer(&send_addr, 2, &read_h);
    SS1_SetHigh();
    
    //put high and low byte into same value
    read_h = read_h << 8;
    read = read_h | read_l;   
    
    //get absolute value of 2s complement number
    if((0x8000 & read) == 0x8000)
    {
        read -= 1;
        read = ~read;
        neg = 1;
    }
    else
    {
        neg = 0;
    }
    //convert number to value in mG
    read = read >> 3; //0.122 mG/b = 1/2^n N = 3
    
    //convert absolute binary value into 4 digit bcd
    //the value we get will be represented in mG
    uint16_t bcd_value;
    
    binary_to_bcd(&read, &bcd_value);
    mag_z_bcd = bcd_value;
    mag_z_dir = neg;
    
    //then send bcd_value over uart to pc
}

void magISR(void)
{
    //read x direction
    read_xaxis();
    //read y direction
    read_yaxis();
    //read z direction
    read_zaxis();
    //new data flag
    new_mag_data = 1;
   
}



void transmit_to_pc(void)
{
    //disable interrupts so that uart is not interrupted...
    INTERRUPT_GlobalDisable();
    
    new_mag_data = 0;
    //data format will be [xdirection][xdigit4][xdigit3][xdigit2][xdigit1][space]...
    //                    [ydirection][ydigit4][ydigit3][ydigit2][ydigit1][space]
    //                    [zdirection][zdigit4][zdigit3][zdigit2][zdigit1][newline character]
    //the data will look like -xxxx -yyyy -zzzz\n
    uint16_t formatted_data[18] = {0, 0, 0, 0, 0, 0x0020, //x direction
                                  0, 0, 0, 0, 0, 0x0020, //y direction
                                  0, 0, 0, 0, 0, 0x000A};//z direction
                                    //20H is ascii space, 0AH is newline char
    
    if(mag_x_dir == 1)
    {
        //positive mag_x_dir is negative
        formatted_data[0] = 0x002D; //minus symbol in ascii
    }
    else
    {
        formatted_data[0] = 0x002B;//plus symbol in ascii
    }
    if(mag_y_dir == 1)
    {
        //positive mag_x_dir is negative
        formatted_data[6] = 0x002D; //minus symbol in ascii
    }
    else
    {
        formatted_data[6] = 0x002B;//plus symbol in ascii
    }
    if(mag_z_dir == 1)
    {
        //positive mag_x_dir is negative
        formatted_data[12] = 0x002D; //minus symbol in ascii
    }
    else
    {
        formatted_data[12] = 0x002B;//plus symbol in ascii
    }
    
    
    formatted_data[1] = ((mag_x_bcd >> 12) & 0x000F) + 0x0030;//adds 30H to make the digit a valid ascii digit
    formatted_data[2] = ((mag_x_bcd >> 8) & 0x000F) + 0x0030;
    formatted_data[3] = ((mag_x_bcd >> 4) & 0x000F) + 0x0030;
    formatted_data[4] = ((mag_x_bcd) & 0x000F) + 0x0030;
    formatted_data[7] = ((mag_y_bcd >> 12) & 0x000F) + 0x0030;//adds 30H to make the digit a valid ascii digit
    formatted_data[8] = ((mag_y_bcd >> 8) & 0x000F) + 0x0030;
    formatted_data[9] = ((mag_y_bcd >> 4) & 0x000F) + 0x0030;
    formatted_data[10] = ((mag_y_bcd) & 0x000F) + 0x0030;
    formatted_data[13] = ((mag_z_bcd >> 12) & 0x000F) + 0x0030;//adds 30H to make the digit a valid ascii digit
    formatted_data[14] = ((mag_z_bcd >> 8) & 0x000F) + 0x0030;
    formatted_data[15] = ((mag_z_bcd >> 4) & 0x000F) + 0x0030;
    formatted_data[16] = ((mag_z_bcd) & 0x000F) + 0x0030;
    uint16_t ii = 0;
    while(ii < 18) // 4 is the number of digits that need to be transmitted
    {
        if(UART4_IsTxReady() == 1)
        {
            UART4_Write(formatted_data[ii]);
            ii += 1;
        }
    }
    //re-enable interrupts after uart is finished
    INTERRUPT_GlobalEnable();
}

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    TMR1_SetInterruptHandler (magISR);
    
    //config mag to run in continuous conversion mode
    uint16_t conf_cmd = ctrl_3_reg;
    SS1_SetLow();
    SPI1_Exchange16bit(conf_cmd);
    SS1_SetHigh();
    
    INTERRUPT_GlobalEnable();
    
    while (1)
    {
        if(new_mag_data = 1)
        {
            transmit_to_pc();
        }
    }

    return 1;
}
/**
 End of File
*/

