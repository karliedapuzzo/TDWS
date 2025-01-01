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

/*
                         Main application
 */

/*for LoRa start by checking if connected by sending AT
 * check for +AT: OK
 * set to test mode by AT+MODE=TEST
 * check for +MODE: TEST
 * set rf config in style of AT+TEST=RFCFG,[FREQUENCY],[SF],[BANDWIDTH],[TX PR],[RX PR],[TX POWER],[CRC],[IQ],[NET]
 * for our use cmd = AT+TEST=RFCFG,915,SF10,125,12,15,9,ON,OFF,OFF
 * to send AT+TEST=TXLRSTR, "stuff to send"
 */


//sends a message via uart to the lora module
//returns 1 if response is okay 0 if not okay
//int lora_send(char msg[32])
//{
//    int ii = 0;
//    while(ii < 32)
//    {
//        if(msg[ii] != 0)
//        {
//            if(UART2_IsTxReady() == 1)
//            {
//                UART2_Write(msg[ii]); 
//                ii += 1;
//            }
//        }
//        else
//        {
//            break;
//        }
//    }
//    
//    
//}

//check if LoRa module is active
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
    
    if((resp[5] == 0x4f) && (resp[6] == 0x4b))
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
    int garbage_var = (resp[7] == 0x54) && (resp[8] == 0x45) && (resp[9] == 0x53) && (resp[10] == 0x54);
    if((resp[7] == 0x54) && (resp[8] == 0x45) && (resp[9] == 0x53) && (resp[10] == 0x54))
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
    int garbage_var = (resp[7] == 0x52) && (resp[8] == 0x46) && (resp[9] == 0x43) && (resp[10] == 0x46);
    if((resp[7] == 0x52) && (resp[8] == 0x46) && (resp[9] == 0x43) && (resp[10] == 0x46))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t LoRa_transmit_msg(void)
{
    char cmd[64];
    sprintf(cmd, "AT+TEST=TXLRSTR,\"ThisIsATest\"\r\n");//this will eventually be replaced with status indicator message
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
    //should receive back +TEST: TXLRSTR "..." and +TEST: DONE

    char resp1[100];
    char resp2[100];
    ii = 0;
    while(ii < 91)
    {
        resp1[ii] = UART2_Read();
        ii +=1;
        if(resp1[ii-1]==0x0a)//order matters!!!
        {
            ii = 99;
        }
    }
    
    ii = 0;
    while(ii < 91)
    {
        resp2[ii] = UART2_Read();
        ii +=1;
        if(resp2[ii-1]==0x0a)//order matters!!!
        {
            ii = 99;
        }
    }
    
    int garbage_var = (resp2[10] == 0x44) && (resp2[11] == 0x4F) && (resp2[12] == 0x4E) && (resp2[13] == 0x45);
    if((resp2[10] == 0x44) && (resp2[11] == 0x4F) && (resp2[12] == 0x4E) && (resp2[13] == 0x45))
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
    
    is_active = check_lora();
    if(is_active == 1)
    {
        mode_set = lora_set_mode();
        rfconfig_set = lora_rfconfig();
    }
    
    int more_garbage = rfconfig_set && mode_set;
    int temp = 0;
}


int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    UART2_Initialize();
    init_LoRa();
    uint8_t transmit_done = 0;
//    init_LoRa();
    while (1)
    {
        //Transmit message continuosly :D
        transmit_done = LoRa_transmit_msg();
        uint16_t wait=0;
        while(wait < 0xFFFF)
        {
            wait +=1;
        }
    }

    return 1;
}
/**
 End of File
*/

