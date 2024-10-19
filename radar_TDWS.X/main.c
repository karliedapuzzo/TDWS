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
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    UART1_Initialize();
    TMR1_Initialize();
    
    uint8_t INIT_test[12] = {0x49, 0x4E, 0x49, 0x54, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t RESP_test[9] = {1};
    uint8_t DDAT_test[12] = {0x47, 0x4E, 0x46, 0x44, 0x04, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
    uint8_t TDAT_test[12] = {0x47, 0x4E, 0x46, 0x44, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00};
    uint8_t RSPI_test[12] = {0x52, 0x53, 0x50, 0x49, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t RESP_DDAT[32] = {1};
    uint8_t RESP_TDAT[25] = {1};
    uint8_t RESP_test_3[9] = {1};
    uint8_t i;
    uint8_t init = 1;
    uint8_t approaching = 2;
    uint8_t approaching_ASCII;
    char approaching_char;
    enum LCD_MODE {
    LCD_MODE_NORMAL,
    LCD_MODE_ENHANCED,
    };
    
    while (1)
    {
        if (init == 1){
            
            //sends command to initialize radar to 115200 baud 
            for (i = 0; i <= 11; i++){          //shifts through all our command bytes
                UART1_Write(INIT_test[i]);      //writes command byte to TX
                while (UART1_IsTxReady() == 0)   //holds here till we can transmit our next byte
                {
                }
            }
            
            //reads response from the initialization command
            for (i = 0; i <= 8; i++){          //shifts through all our respond bytes
                RESP_test[i] = UART1_Read();    //reads response byte from RX
            }
            
            //sends command to set threshold speed for radar detection
            for (i = 0; i <= 11; i++){          //shifts through all our command bytes
                UART1_Write(RSPI_test[i]);      //writes command byte to TX
                while (UART1_IsTxReady() == 0)   //holds here till we can transmit our next byte
                {
                }
            }
            
            //reads response from the speed threshold command
            for (i = 0; i <= 8; i++){          //shifts through all our respond bytes
                RESP_test_3[i] = UART1_Read();    //reads response byte from RX
            }
            
            //initializes the LCD to normal operation
            //LCD_ModeSet(LCD_MODE_NORMAL);
            //starts timer for a 100us refresh rate
            TMR1_Start();
            //no longer goes through this if statement after first loop
            init = 0;
        }
        //timer activation
        if (TMR1_GetElapsedThenClear()){
            
            //sends a command to radar to get detection data
            for (i = 0; i <= 13; i++){          //shifts through all our command bytes
                while (!UART1_IsTxReady())   //holds here till we can transmit our next byte
                {
                }
                UART1_Write(DDAT_test[i]);      //writes command byte to TX
            }
            
            //reads response from radar after detection data command including if 
            //something is approaching or receding from the radar
            for (i = 0; i <= 31; i++){          //shifts through all our respond bytes
                while (!UART1_IsRxReady()){
                }
                RESP_DDAT[i] = UART1_Read();    //reads response byte from RX
            }
            
            //parses data out of response to find if an object is approaching or
            //receding our radar
            for (i = 0; i <= 18; i++){
                if (RESP_DDAT[i] == 0x44 && RESP_DDAT[i+1] == 0x44 && RESP_DDAT[i+2] == 0x41 &&
                        RESP_DDAT[i+3] == 0x54 && RESP_DDAT[i+4] == 0x06){
                    approaching = RESP_DDAT[i + 11];
                }
            } 
            
            //sets approaching flag to a character to print to a LCD
            approaching_ASCII = approaching + '0';
            approaching_char = (char)approaching_ASCII;
//            LCD_APPROACHING_Print(approaching_char);
            if (UART4_IsTxReady() == 1){
                UART4_Write(approaching_char);
            }
        }
    }

    return 1;
}
/**
 End of File
*/

