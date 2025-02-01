//this lora file is for transmitting messages only

#include "LoRa_transmitter.h"
#include "mcc_generated_files/uart2.h"

uint8_t check_lora_status(void)
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

uint8_t set_lora_mode(void)
{
    char cmd[32];
    sprintf(cmd, "AT+MODE=TEST\r\n");
    //write test mode command to uart
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

uint8_t set_lora_rfconfig(void)
{
    char cmd[64];
    sprintf(cmd, "AT+TEST=RFCFG,915,SF10,125,12,15,9,ON,OFF,OFF\r\n");
    //write rfconfig command to uart
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

uint8_t LoRa_transmit_msg(char *msg[32])
{
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+TEST=TXLRSTR,\"%s\"\r\n", *msg);//this will eventually be replaced with status indicator message
    //write send message command to uart
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

void init_LoRa(void)
{
    int is_active;
    int mode_set;
    int rfconfig_set;
    
    is_active = check_lora_status();
    if(is_active == 1)
    {
        mode_set = set_lora_mode();
        rfconfig_set = set_lora_rfconfig();
    }
}