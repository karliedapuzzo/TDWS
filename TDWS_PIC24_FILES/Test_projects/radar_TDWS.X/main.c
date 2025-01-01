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

uint8_t RADAR_Facreset(void)
{
    uint8_t RFSE_test[8] = {'R', 'F', 'S', 'E', 0x00, 0x00, 0x00, 0x00};
    uint8_t RFSE_RESP_test[9];
    uint8_t error_code = 0x08;
    uint8_t i = 0;
    
    //sends command to reset radar to factory settings 
    for (i = 0; i <= 7; i++){               //shifts through all our command bytes
        UART1_Write(RFSE_test[i]);          //writes command byte to TX
        while (UART1_IsTxReady() == 0)      //holds here till we can transmit our next byte
        {
        }
    }
            
    //reads response from the initialization command
    for (i = 0; i <= 8; i++){               //shifts through all our respond bytes
        RFSE_RESP_test[i] = UART1_Read();   //reads response byte from RX
        if (i == 8){
            error_code = RFSE_RESP_test[i]; //save error_code from response from radar
        }
    }
    
    return(error_code);
}

uint8_t RADAR_Init(uint8_t baud_setting)
{
    uint8_t INIT_test[8] = {'I', 'N', 'I', 'T', 0x01, 0x00, 0x00, 0x00};
    uint8_t INIT_RESP_test[9] = {1};
    uint8_t VERS_test[27] = {1};
    uint8_t i = 0;
    uint8_t error_code = 0x08;
    
    //sends command to initialize radar to a baud rate 
    for (i = 0; i <= 8; i++){               //shifts through all our command bytes
        if (i < 8){
            UART1_Write(INIT_test[i]);      //writes command byte to TX
        } else if (i == 8){
            UART1_Write(baud_setting);      //writes baud rate setting from input
        }
        while (UART1_IsTxReady() == 0)      //holds here till we can transmit our next byte
        {
        }
    }
            
    //reads response from the initialization command
    for (i = 0; i <= 8; i++){          //shifts through all our respond bytes
        INIT_RESP_test[i] = UART1_Read();    //reads response byte from RX
        if (i == 8){
            error_code = INIT_RESP_test[i]; //save error_code from response from radar
        }
    }
            
    //reads firmware_string from the initialization command
    for (i = 0; i <= 26; i++){          //shifts through all our version bytes
        VERS_test[i] = UART1_Read();    //reads response byte from RX
    }
    
    return(error_code);
}

uint8_t RADAR_Rangesetting(uint8_t range_setting)
{
    uint8_t RRAI_test[8] = {'R', 'R', 'A', 'I', 0x01, 0x00, 0x00, 0x00};
    uint8_t RRAI_RESP_test[9] = {1};
    uint8_t i = 0;
    uint8_t error_code = 0x08;
    
    //sends command to set radar range setting 
    for (i = 0; i <= 8; i++){               //shifts through all our command bytes
        if (i < 8){
            UART1_Write(RRAI_test[i]);      //writes command byte to TX
        } else if (i == 8){
            UART1_Write(range_setting);     //writes range setting from input
        }
        while (UART1_IsTxReady() == 0)      //holds here till we can transmit our next byte
        {
        }
    }
    
    //reads response from the initialization command
    for (i = 0; i <= 8; i++){               //shifts through all our respond bytes
        RRAI_RESP_test[i] = UART1_Read();   //reads response byte from RX
        if (i == 8){
            error_code = RRAI_RESP_test[i]; //save error_code from response from radar
        }
    }
    
    return(error_code);
}

uint8_t RADAR_Mindetzone(uint8_t minimum_range)
{
    uint8_t MIRA_test[8] = {'M', 'I', 'R', 'A', 0x01, 0x00, 0x00, 0x00};
    uint8_t MIRA_RESP_test[9] = {1};
    uint8_t i = 0;
    uint8_t error_code = 0x08;
    
    //sends command to set minimum range of detection zone 
    for (i = 0; i <= 8; i++){               //shifts through all our command bytes
        if (i < 8){
            UART1_Write(MIRA_test[i]);      //writes command byte to TX
        } else if (i == 8){
            UART1_Write(minimum_range);     //writes minimum range setting from input
        }
        while (UART1_IsTxReady() == 0)      //holds here till we can transmit our next byte
        {
        }
    }
    
    //reads response from the initialization command
    for (i = 0; i <= 8; i++){               //shifts through all our respond bytes
        MIRA_RESP_test[i] = UART1_Read();   //reads response byte from RX
        if (i == 8){
            error_code = MIRA_RESP_test[i]; //save error_code from response from radar
        }
    }
    
    return(error_code);
}

uint8_t RADAR_Maxdetzone(uint8_t maximum_range)
{
    uint8_t MARA_test[8] = {'M', 'A', 'R', 'A', 0x01, 0x00, 0x00, 0x00};
    uint8_t MARA_RESP_test[9] = {1};
    uint8_t i = 0;
    uint8_t error_code = 0x08;
    
    //sends command to set maximum range of detection zone 
    for (i = 0; i <= 8; i++){               //shifts through all our command bytes
        if (i < 8){
            UART1_Write(MARA_test[i]);      //writes command byte to TX
        } else if (i == 8){
            UART1_Write(maximum_range);     //writes maximum range setting from input
        }
        while (UART1_IsTxReady() == 0)      //holds here till we can transmit our next byte
        {
        }
    }
    
    //reads response from the initialization command
    for (i = 0; i <= 8; i++){               //shifts through all our respond bytes
        MARA_RESP_test[i] = UART1_Read();   //reads response byte from RX
        if (i == 8){
            error_code = MARA_RESP_test[i]; //save error_code from response from radar
        }
    }
    
    return(error_code);
}

uint8_t RADAR_Threshoffset(uint8_t thresh_offset)
{
    uint8_t THOF_test[8] = {'T', 'H', 'O', 'F', 0x01, 0x00, 0x00, 0x00};
    uint8_t THOF_RESP_test[9] = {1};
    uint8_t i = 0;
    uint8_t error_code = 0x08;
    
    //sends command to set threshold offset 
    for (i = 0; i <= 8; i++){               //shifts through all our command bytes
        if (i < 8){
            UART1_Write(THOF_test[i]);      //writes command byte to TX
        } else if (i == 8){
            UART1_Write(thresh_offset);     //writes threshold from input
        }
        while (UART1_IsTxReady() == 0)      //holds here till we can transmit our next byte
        {
        }
    }
    
    //reads response from the initialization command
    for (i = 0; i <= 8; i++){               //shifts through all our respond bytes
        THOF_RESP_test[i] = UART1_Read();   //reads response byte from RX
        if (i == 8){
            error_code = THOF_RESP_test[i]; //save error_code from response from radar
        }
    }
    
    return(error_code);
}

uint16_t UINT8to16(uint8_t high_byte, uint8_t low_byte)
{
    uint16_t result;
    result = (high_byte << 8) | low_byte;
    return(result);
}

int16_t UINT8toINT16(uint8_t high_byte, uint8_t low_byte)
{
    int16_t result;
    result = (int16_t)((high_byte << 8) | low_byte);
    return(result);
}

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    UART1_Initialize();
    TMR1_Initialize();
        
    uint8_t GRPS_test[8] = {'G', 'R', 'P', 'S', 0x00, 0x00, 0x00, 0x00};
    uint8_t RSPT_RESP_test[48];
    
    uint8_t GNFD_test[9] = {'G', 'N', 'F', 'D', 0x01, 0x00, 0x00, 0x00, 0x28};
    uint8_t GNFD_RESP_test[93] = {1};
    
    uint8_t i;
    uint8_t approaching = 0;
    uint8_t neg_angle = 0;
    uint8_t reading_done = 0;
    uint8_t done_count = 0;
    uint8_t num_targets = 0x09;
    uint8_t k = 0;
    uint8_t init = 1;
    uint8_t test = 1;
    
    uint16_t distance;
    uint8_t distance_string[5];
    int16_t speed;
    uint8_t speed_string[5];
    int16_t angle;
    uint8_t angle_string[4];
    uint16_t magnitude;
    uint8_t magnitude_string[5];
    uint8_t identification;
    
    while (1)
    {
        if (init == 1){
            
            while (RADAR_Facreset() != 0x00){           //factory reset
            }
                        
            while (RADAR_Init(0x00) != 0x00){           //initialize uart communication to setting 0 (115200 baud)
            }
            
            while (RADAR_Rangesetting(0x00) != 0x00){   //set range setting to setting 0 (100m)
            }
            
            while (RADAR_Mindetzone(0x00) != 0x00){     //set minimum range setting to 0% of range setting
            }
            
            while (RADAR_Maxdetzone(0x07) != 0x00){     //set maximum range setting to 7% of range setting
            }
            
            while (RADAR_Threshoffset(0x0C) != 0x00){   //set threshold offset to 0dB
            }
            
            init = 0;
            TMR1_Start();
//            
        }
        
        if (TMR1_GetElapsedThenClear()){
            if (test == 1 || test == 0){
                test = 0;

                for (i = 0; i <= 7; i++){          //shifts through all our command bytes
                    UART1_Write(GRPS_test[i]);      //writes command byte to TX
                    while (UART1_IsTxReady() == 0)   //holds here till we can transmit our next byte
                    {
                    }
                }

                for (i = 0; i <= 47; i++){                //shifts through all our respond bytes
                    RSPT_RESP_test[i] = UART1_Read();    //reads response byte from RX
                }
                
                test = 0;        
                //sends command to get next data frame, specifically tracked data 
                for (i = 0; i <= 8; i++){          //shifts through all our command bytes
                    UART1_Write(GNFD_test[i]);      //writes command byte to TX
                    while (UART1_IsTxReady() == 0)   //holds here till we can transmit our next byte
                    {
                    }
                }

                //reads response from the next data frame command
                for (i = 0; i <= 92; i++){                //shifts through all our respond bytes
                    GNFD_RESP_test[i] = UART1_Read();    //reads response byte from RX
                    if (reading_done == 1){
                        if (done_count > 1){
                            done_count = done_count - 1;
                        } else {
                            done_count = 0;
                            reading_done = 0;
//                            UART4_Write(num_targets);      //writes command byte to TX
//                            while (UART4_IsTxReady() == 0)   //holds here till we can transmit our next byte
//                            {
//                            }
                            break;
                        }
                    }else if (i >= 3){
                        if(GNFD_RESP_test[i-4] == 'T'){
                            if(GNFD_RESP_test[i-3] == 'D'){
                                if(GNFD_RESP_test[i-2] == 'A'){
                                    if(GNFD_RESP_test[i-1] == 'T'){
                                        num_targets = (GNFD_RESP_test[i] / 9);
                                    }
                                }
                            } 
                        }
                        if(GNFD_RESP_test[i-3] == 'D'){
                            if(GNFD_RESP_test[i-2] == 'O'){
                                if(GNFD_RESP_test[i-1] == 'N'){
                                    if(GNFD_RESP_test[i] == 'E'){
                                        reading_done = 1;
                                        done_count = 8;
                                    }
                                }
                            } 
                        }
                    }
                }
                
                if (num_targets > 0x00){
                    for (i = 0; i < num_targets; i++){
                        
                        distance = UINT8to16(GNFD_RESP_test[18 + (i*9)], GNFD_RESP_test[17 + (i*9)]);
                        speed = UINT8toINT16(GNFD_RESP_test[20 + (i*9)], GNFD_RESP_test[19 + (i*9)]);
                        angle = UINT8toINT16(GNFD_RESP_test[22 + (i*9)], GNFD_RESP_test[21 + (i*9)]);
                        magnitude = UINT8to16(GNFD_RESP_test[24 + (i*9)], GNFD_RESP_test[23 + (i*9)]);
                        identification = GNFD_RESP_test[25 + (i*9)];
                        
                        for (k = 0; k < 5; k++){                    //splice digits of distance
                            if (distance != 0){
                                distance_string[k] = 0x30 + distance % 10;
                                distance /= 10;
                            }
                        }
                        
                        if (speed < 0){
                            approaching = 1;
                            speed *= -1;
                        }
                        
                        for (k = 0; k < 5; k++){                    //splice digits of speed
                            if (speed != 0){
                                speed_string[k] = 0x30 + speed % 10;
                                speed /= 10;
                            }
                        }
                        
                        if (angle < 0){
                            neg_angle = 1;
                            angle *= -1;
                        }
                        
                        for (k = 0; k < 3; k++){                    //splice digits of angle
                            if (angle != 0){
                                angle_string[k] = 0x30 + angle % 10;
                                angle /= 10;
                            }
                        }
                        
                        for (k = 0; k < 4; k++){                    //splice digits of magnitude
                            if (magnitude != 0){
                                magnitude_string[k] = 0x30 + magnitude % 10;
                                magnitude /= 10;
                            }
                        }
                        
                        UART4_Write(0x30 + identification);      //writes command byte to TX
                        while (UART4_IsTxReady() == 0)   //holds here till we can transmit our next byte
                        {
                        }
                        
                        test = 0;
                        
                        UART4_Write(' ');      //writes command byte to TX
                        while (UART4_IsTxReady() == 0)   //holds here till we can transmit our next byte
                        {
                        }
                        
                        test = 0;
                        
                        for (k = 0; k <= 4; k++){
                            if (distance_string[4 - k] != 0x00){
                                UART4_Write(distance_string[4 - k]);      //writes command byte to TX
                                while (UART4_IsTxReady() == 0)   //holds here till we can transmit our next byte
                                {
                                }
                            }
                            test = 0;
                        }
                        
                        test = 0;
                        
                        UART4_Write(0x0A);      //writes command byte to TX
                        while (UART4_IsTxReady() == 0)   //holds here till we can transmit our next byte
                        {
                        }
                        
                        test = 0;
                        
                        UART4_Write(0x0D);      //writes command byte to TX
                        while (UART4_IsTxReady() == 0)   //holds here till we can transmit our next byte
                        {
                        }
                        
                        test = 0;
                    }
                    approaching = 0;
                    for (k = 0; k <= 4; k++){
                        distance_string[4 - k] = 0x00;
                    }
                    num_targets = 0;
                }
                test = 0;
            }
        }
    }
    
    return 1;
}
/**
 End of File
*/

