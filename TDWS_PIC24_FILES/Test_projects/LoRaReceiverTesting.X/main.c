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
#include "mcc_generated_files/interrupt_manager.h"

#include <ctype.h>

/*
                         Main application
 */

uint16_t uart2_rxcount = 0;
char received_msg[150];
uint16_t message_available = 0;

//reads one character and puts it in a global character array for other functions to parse
void uart2_ISR(void)
{
    if(message_available == 0)
    {
        uint16_t one_char = UART2_Read();
        received_msg[uart2_rxcount] = one_char & 0x00FF;
        uart2_rxcount++;
        if(uart2_rxcount > 149) //if over message array limit reset uart2_rx_count
        {
            uart2_rxcount = 0;
        }
        else if(received_msg[uart2_rxcount - 1] == '\n')
        {
            uart2_rxcount = 0;
            message_available = 1;
        }
    }
    else
    {
        uint16_t trash = UART2_Read();//throw away anything in the uart buffer
    }
}

uint16_t check_lora(void)
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
    while(message_available == 0)
    {
        cmd[10] = 'p';//this does nothing/is just a place holder for debugging/ to keep here while waiting for the response
    }
    message_available = 0;
    uint8_t check_msg = (received_msg[5] == 'O') && (received_msg[6] == 'K');
    if(check_msg)
    {
        for(int jj = 0; jj <=149; jj++)
            {
                received_msg[jj] = 0x0000;
            }
        return 1;
    }
    else
    {
        for(int jj = 0; jj <=149; jj++)
            {
                received_msg[jj] = 0x0000;
            }
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
    while(message_available == 0)
    {
        cmd[10] = 'p';//this does nothing/is just a place holder for debugging/ to keep here while waiting for the response
    }
    message_available = 0;
    uint8_t check_msg = (received_msg[7] == 'T') && (received_msg[8] == 'E') && (received_msg[9] == 'S') && (received_msg[10] == 'T');
    if(check_msg)
    {
        for(int jj = 0; jj <=149; jj++)
        {
            received_msg[jj] = 0x0000;
        }
        return 1;
    }
    else
    {
        for(int jj = 0; jj <=149; jj++)
        {
            received_msg[jj] = 0x0000;
        }
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
    while(message_available == 0)
    {
        cmd[10] = 'p';//this does nothing/is just a place holder for debugging/ to keep here while waiting for the response
    }
    message_available = 0;
    int check_msg = (received_msg[7] == 'R') && (received_msg[8] == 'F') && (received_msg[9] == 'C') && (received_msg[10] == 'F');
    if(check_msg)
    {
        for(int jj = 0; jj <=149; jj++)
        {
            received_msg[jj] = 0x0000;
        }
        return 1;
    }
    else
    {
        for(int jj = 0; jj <=149; jj++)
        {
            received_msg[jj] = 0x0000;
        }
        return 0;
    }
}

uint8_t LoRa_set_rx(void)
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
    //should receive back +TEST: RXLRSTR
    while(message_available == 0)
    {
        cmd[10] = 'p';//this does nothing/is just a place holder for debugging/ to keep here while waiting for the response
    }
    message_available = 0;
    int check_msg = (received_msg[7] == 'R') && (received_msg[8] == 'X') && (received_msg[9] == 'L') && (received_msg[10] == 'R');
    if(check_msg)
    {
        for(int jj = 0; jj <=149; jj++)
        {
            received_msg[jj] = 0x0000;
        }
        return 1;
    }
    else
    {
        for(int jj = 0; jj <=149; jj++)
        {
            received_msg[jj] = 0x0000;
        }
        return 0;
    }
}

char msg1[149];
char msg2[149];

void read_message(void)//only run if LoRa is in reciever mode
{
    //looking for message that looks like +TEST: RX "xyxyxyxyxyxy..." where xy is one char, x and y are repped in hex
    uint8_t check_msg = (received_msg[7] == 'R' & received_msg[8] == 'X');
    if(check_msg)
    {
        int ii = 11;
        int kk = 0;
        while(ii < 150)
        {
            if(received_msg[ii] == '"')
            {
                ii = 150;
                msg2[kk] = '\n';//marks the end of the message that we got
                
            }
            else
            {
                uint16_t x_char = (uint16_t)(isdigit(received_msg[ii]) ? received_msg[ii] - '0' : toupper(received_msg[ii]) - 'A' + 10);
                uint16_t y_char = (uint16_t)(isdigit(received_msg[ii+1]) ? received_msg[ii+1] - '0' : toupper(received_msg[ii+1]) - 'A' + 10);
                char final_char = (char)((x_char << 4) | y_char);
                msg2[kk] = final_char;
                ii+=2;
                kk++;
            }
        }
    }
    else
    {
        int ii = 0;
        while(ii < 150)
        {
            if(received_msg[ii] == '\n')
            {
                ii = 150;
            }
            else
            {
                msg1[ii] = received_msg[ii];
                ii++;
            }
        }
    }
    message_available = 0;
}

uint8_t LoRa_transmit_msg(void)//this should transmit a message over LoRa
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
    //should receive back +TEST: TXLRSTR "..."\n and +TEST: DONE\n


    
    while(message_available == 0)
    {
        cmd[10] = 'p';//this does nothing/is just a place holder for debugging/ to keep here while waiting for the response
    }
    message_available = 0;//skips first message +TEST: TXLRSTR
    while(message_available == 0)
    {
        cmd[10] = 'p';//this does nothing/is just a place holder for debugging/ to keep here while waiting for the response
    }
    message_available = 0;//looks at second message and checks if it did sent the packet
    int check_msg = (received_msg[7] == 'D') && (received_msg[8] == 'O') && (received_msg[9] == 'N') && (received_msg[10] == 'E');
    if(check_msg)
    {
        for(int jj = 0; jj <=149; jj++)
        {
            received_msg[jj] = 0x0000;
        }
        return 1;
    }
    else
    {
        for(int jj = 0; jj <=149; jj++)
        {
            received_msg[jj] = 0x0000;
        }
        return 0;
    }
}

void init_LoRa_rx(void)//the difference between rx and tx is that rx is a specific setting, otherwise will not notify if it received something
{
    int is_active;
    int mode_set;
    int rfconfig_set;
    int set_rx;
    int all_good = 0;
    
    is_active = check_lora();
    if(is_active == 1)
    {
        mode_set = lora_set_mode();
        rfconfig_set = lora_rfconfig();
        set_rx = LoRa_set_rx();
    }
    
    all_good = (rfconfig_set && mode_set && set_rx);
    int trahs = 0x0000;
}

void init_LoRa_tx(void)//initializes lora but doesn't set it for receiving
{
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
    
    UART2_SetRxInterruptHandler(&uart2_ISR);
    
    
    
    int warmup_wait = 0;
    while(warmup_wait < 0xffff)
    {
        warmup_wait++;
    }
    
    init_LoRa_rx();
//    init_LoRa_tx();
    
//    INTERRUPT_GlobalDisable();
    
    while (1)
    {
        int new_msg = 0;
        int wait = 0;
        
        while(wait < 0xffff)
        {
            wait++;
            if(message_available)
            {
                read_message();
                new_msg++;
            }
            if(new_msg >= 2)
            {
                new_msg = 0;
            }
//            LoRa_transmit_msg();
        }
    }

    return 1;
}
/**
 End of File
*/

