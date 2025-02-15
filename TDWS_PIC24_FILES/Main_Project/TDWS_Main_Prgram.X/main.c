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
#include "LoRa_transmitter.h"
#include "radar.h"
#include "magnetometer.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/interrupt_manager.h"

/*
                         Main application
 */

uint16_t mag_x_val = 0;
uint16_t mag_x_dir = 0;

uint16_t mag_y_val = 0;
uint16_t mag_y_dir = 0;

uint16_t mag_z_val = 0;
uint16_t mag_z_dir = 0;

uint16_t new_mag_data = 0;

uint16_t magISR(void)
{
    //read x direction
    read_xaxis(&mag_x_val, &mag_x_dir);
    //read y direction
    read_yaxis(&mag_y_val, &mag_y_dir);
    //read z direction
    read_zaxis(&mag_z_val, &mag_z_dir);
    //new data flag
    new_mag_data = 1;
    return 1; //new data was collected so return 1
}

static uint16_t mode = 0x00;
static uint16_t mode_set = 0;

void read_switches(void)//read the switch values and indicate if they changed
{
//    uint16_t prev_mode = mode;
    
    uint16_t sw0;
    uint16_t sw1;
    uint16_t sw2;
    uint16_t sw3;
    
    sw0 = IO_RE2_GetValue();
    sw1 = IO_RE3_GetValue();
    sw2 = IO_RE4_GetValue();
    sw3 = IO_RE5_GetValue();
    
    sw0 = (~sw0 & 0x0001);
    sw1 = (~sw1 & 0x0001) << 1;
    sw2 = (~sw2 & 0x0001) << 2;
    sw3 = (~sw3 & 0x0001) << 3;
    
    mode = (sw0 | sw1 | sw2 | sw3);
}

static uint16_t here;

int main_mode(void)
{
    while(1)
    {
        here = 0;
    }
}

int detect_mod1(void)
{
    while(1)
    {
        here = 1;
    }
}

int detect_mod2(void)
{
    while(1)
    {
        here = 2;
    }
}

int radar_test(void)
{
    while(1)
    {
        here = 3;
    }
}

int mag_test(void)
{
    //in mag test, want to send data over uart 4 to pc
    uint16_t x_bcd;
    uint16_t y_bcd;
    uint16_t z_bcd;
    
    while(1)
    {
        if (new_mag_data == 1)
        {
            binary_to_bcd(&mag_x_val, &x_bcd);
            binary_to_bcd(&mag_y_val, &y_bcd);
            binary_to_bcd(&mag_z_val, &z_bcd);

            transmit_mag_to_pc(&x_bcd, &mag_x_dir,
                           &y_bcd, &mag_y_dir,
                            &z_bcd, &mag_z_dir,
                            &new_mag_data);

            
        }
        here = 4;
    }
}

int lora_receive_test(void)
{
    while(1)
    {
        here = 5;
    }
}

int lora_transmit_test(void)
{
    INTERRUPT_GlobalDisable();
    while(1)
    {
        here = 6;
    }
}

int light_test(void)
{
    while(1)
    {
        here = 7;
    }
}

int data_collection(void)//maybe not needed?
{
    while(1)
    {
        here = 8;
    }
}


int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    uint16_t wait = 0;
    while(wait < 0x00ff)
    {
        wait ++;
    }
    
    init_LoRa();
    
    TMR2_SetInterruptHandler(magISR); //set timer 2 interrupt to magnetometer ISR
    
    init_magnetometer(); //initialize magnetometer
    
    INTERRUPT_GlobalEnable();

    wait = 0;
    
    while(wait < 10)//read switches
    {
        wait++;
        read_switches();
    }
    
    if(mode == 0x0001)
    {
        detect_mod1();
    }
    else if(mode == 0x0003)
    {
        detect_mod2();
    }
    else if(mode == 0x0007)
    {
        lora_receive_test();
    }
    else if(mode == 0x0008)
    {
        radar_test();
    }
    else if(mode == 0x0009)
    {
        mag_test();
    }
    else if(mode == 0x000A)
    {
        lora_transmit_test();
    }
    else if(mode == 0x000B)
    {
        light_test();
    }
    else if((mode == 0x000C) | (mode == 0x000D) | (mode == 0x000E) | (mode == 0x000F))
    {
        data_collection();
    }
    else
    {
        main_mode();
    }
    
    while (1)
    {
        uint16_t error = 1;//if we ever get into this while statement we gots problems :p
        while(wait < 0xffff)
        {
            wait++;
        }
        
    }

    return 1;
}
/**
 End of File
*/

