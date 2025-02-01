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
//#include "mcc_generated_files/interrupt_manager.h"//this shouldn't be needed but compiler hates it for some reason
//#include "mcc_generated_files/uart2.h"//this shouldn't be needed but compiler hates it for some reason
//#include <stdio.h>

/*
                         Main application
 */

uint8_t check_lora(void)
{
    char cmd[32];
    sprintf(cmd, "AT\r\n");
    //write AT to uart
    int ii = 0;
    while(ii < 4)
    {
        if(UART2_IsTxReady() == 1)
        {
            UART2_Write(cmd[ii]); 
            ii += 1;
        }
    }
    //should receive back +AT: OK\r\n 9 total chars
    char resp[32];
    ii = 0;
    while(ii < 9)
    {
        resp[ii] = UART2_Read();
        ii +=1;
        
        if(resp[ii-1]==0x0a)
        {
            ii=10;
        }
    }
    
    int check_resp = (resp[5] == 0x4f) && (resp[6] == 0x4b);
    if(check_resp == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t lora_set_mode(void)
{
    char cmd[32];
    sprintf(cmd, "AT+MODE=TEST\r\n");
    //write AT to uart
    int ii = 0;
    while(ii < 32)
    {
        UART2_Write(cmd[ii]); 
        if(cmd[ii] == 0x0a)
        {
            ii=32;
        }
        ii += 1;
    }
    //should receive back +MODE: TEST\r\n 13 total chars
    char resp[32];
    ii = 0;
    while(ii < 13)
    {
        resp[ii] = UART2_Read();
        ii +=1;
        if(resp[ii-1]==0x0a)
        {
            ii = 14;
        }
    }
    int check_resp = (resp[7] == 0x54) && (resp[8] == 0x45) && (resp[9] == 0x53) && (resp[10] == 0x54);
    if(check_resp == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t lora_rfconfig(void)
{
    char cmd[64];
    sprintf(cmd, "AT+TEST=RFCFG,915,SF10,125,12,15,9,ON,OFF,OFF\r\n");
    //write AT to uart
    int ii = 0;
    while(ii < 64)
    {
        if(UART2_IsTxReady() == 1)
        {
            UART2_Write(cmd[ii]); 
            if(cmd[ii]==0x0a)
            {
                ii = 64;
            }
            ii += 1;
        }
    }
    //should receive back +TEST: RFCFG F:915000000, SF10, BW125K, TXPR:12, RXPR:15, POW:9dBm, CRC:ON, IQ:OFF, NET:OFF

    char resp[100];
    ii = 0;
    while(ii < 91)
    {
        resp[ii] = UART2_Read();
        ii +=1;
        if(resp[ii-1]==0x0a)
        {
            ii = 99;
        }
    }
    int check_resp = (resp[7] == 0x52) && (resp[8] == 0x46) && (resp[9] == 0x43) && (resp[10] == 0x46);
    if(check_resp == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t LoRa_set_receive(void) //sets LoRa to receive data
{
    char cmd[64];
    sprintf(cmd, "AT+TEST=RXLRPKT\r\n");
    //write AT to uart
    int ii = 0;
    while(ii < 64)
    {
        if(UART2_IsTxReady() == 1)
        {
            UART2_Write(cmd[ii]); 
            if(cmd[ii]==0x0a)
            {
                ii = 65;
            }
            ii += 1;
        }
    }
    //should receive back +TEST: RXLRPKT

    char resp[100];
//    char resp2[100];
    ii = 0;
    while(ii < 91)
    {
        resp[ii] = UART2_Read();
        ii +=1;
        if(resp[ii-1]==0x0a)//order matters!!!
        {
            ii = 99;
        }
    }
    
//    ii = 0;
//    while(ii < 91)
//    {
//        resp2[ii] = UART2_Read();
//        ii +=1;
//        if(resp2[ii-1]==0x0a)//order matters!!!
//        {
//            ii = 99;
//        }
//    }
    
    
    int check_resp = (resp[7] == 0x52) && (resp[8] == 0x58) && (resp[9] == 0x4c) && (resp[10] == 0x52);
    if(check_resp == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void init_LoRa(void){
    int is_active;
    int mode_set;
    int rfconfig_set;
    int set_receive;
    
    is_active = check_lora();
    if(is_active == 1)
    {
        mode_set = lora_set_mode();
        rfconfig_set = lora_rfconfig();
        set_receive = LoRa_set_receive();
    }
    
    int more_garbage = (rfconfig_set && mode_set && set_receive);
    int temp = 0;
}

void LoRa_receive_ISR(void)
{
    //this work??
    char resp1[100];
//    char resp2[100];
    int remove_me = 0;
    int ii = 0;
    while(ii < 91)
    {
//        resp1[ii] = UART2_Read();
        ii +=1;
        if(resp1[ii-1]==0x0a)//order matters!!!
        {
            ii = 99;
        }
    }
    int garbage_var = 1;
    int temp_var = 1;
}


int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    INTERRUPT_GlobalDisable();// make for sure that all interrupts are disabled
    init_LoRa();

    //UART2_SetRxInterruptHandler(int_vec_addr);
//    UART2_SetTxInterruptHandler(do_nothing);
    //INTERRUPT_GlobalEnable(); //turn on UART2 Interrupt so that when we receive something from lora we go and read what it gave us
    while (1)
    {
        int temp_var = 1;//runs until interrupt
    }

    return 1;
}
/**
 End of File
*/

