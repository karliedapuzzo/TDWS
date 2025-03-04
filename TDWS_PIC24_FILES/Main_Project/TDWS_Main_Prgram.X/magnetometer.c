#include "magnetometer.h"
#include "mcc_generated_files/spi1.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/uart4.h"
#include "mcc_generated_files/interrupt_manager.h"

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

void read_xaxis(uint16_t *x_data, uint16_t *x_dir, int16_t *x_int)
{
    int16_t tmp = 0;
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
    tmp = read;
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
    tmp = tmp*0.122;
    read = read >> 3; //0.122 mG/b = 1/2^n N = 3
    
    //convert absolute binary value into 4 digit bcd
    //the value we get will be represented in mG
    //uint16_t bcd_value;
    
    //binary_to_bcd(&read, &bcd_value);
    *x_data = read;
    *x_dir = neg;
    *x_int = tmp;
}
void read_yaxis(uint16_t *y_data, uint16_t *y_dir, int16_t *y_int)
{
    int16_t tmp = 0;
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
    tmp = read;
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
    tmp = tmp*0.122;
    read = read >> 3; //0.122 mG/b = 1/2^n N = 3
    
    //convert absolute binary value into 4 digit bcd
    //the value we get will be represented in mG
    //uint16_t bcd_value;
    
    //binary_to_bcd(&read, &bcd_value);
    *y_data = read;
    *y_dir = neg;
    *y_int = tmp;
}
void read_zaxis(uint16_t *z_data, uint16_t *z_dir, int16_t *z_int)
{
    int16_t tmp = 0;
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
    tmp = read;
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
    tmp = tmp*0.122;
    read = read >> 3; //0.122 mG/b = 1/2^n N = 3
    
    //convert absolute binary value into 4 digit bcd
    //the value we get will be represented in mG
    //uint16_t bcd_value;
    
    //binary_to_bcd(&read, &bcd_value);
    *z_data = read;
    *z_dir = neg;
    *z_int = tmp;
}

void transmit_mag_to_pc(uint16_t *mag_x_bcd, uint16_t *mag_x_dir,
                        uint16_t *mag_y_bcd, uint16_t *mag_y_dir,
                        uint16_t *mag_z_bcd, uint16_t *mag_z_dir,
                        uint16_t *new_mag_data)
{
    //disable interrupts so that uart is not interrupted...
    INTERRUPT_GlobalDisable();
    
    *new_mag_data = 0;
    //data format will be [xdirection][xdigit4][xdigit3][xdigit2][xdigit1][space]...
    //                    [ydirection][ydigit4][ydigit3][ydigit2][ydigit1][space]
    //                    [zdirection][zdigit4][zdigit3][zdigit2][zdigit1][newline character]
    //the data will look like -xxxx -yyyy -zzzz\n
    uint16_t formatted_data[18] = {0, 0, 0, 0, 0, 0x0020, //x direction
                                  0, 0, 0, 0, 0, 0x0020, //y direction
                                  0, 0, 0, 0, 0, 0x000A};//z direction
                                    //20H is ascii space, 0AH is newline char
    
    if(*mag_x_dir == 1)
    {
        //positive mag_x_dir is negative
        formatted_data[0] = 0x002D; //minus symbol in ascii
    }
    else
    {
        formatted_data[0] = 0x002B;//plus symbol in ascii
    }
    if(*mag_y_dir == 1)
    {
        //positive mag_x_dir is negative
        formatted_data[6] = 0x002D; //minus symbol in ascii
    }
    else
    {
        formatted_data[6] = 0x002B;//plus symbol in ascii
    }
    if(*mag_z_dir == 1)
    {
        //positive mag_x_dir is negative
        formatted_data[12] = 0x002D; //minus symbol in ascii
    }
    else
    {
        formatted_data[12] = 0x002B;//plus symbol in ascii
    }
    
    
    formatted_data[1] = ((*mag_x_bcd >> 12) & 0x000F) + 0x0030;//adds 30H to make the digit a valid ascii digit
    formatted_data[2] = ((*mag_x_bcd >> 8) & 0x000F) + 0x0030;
    formatted_data[3] = ((*mag_x_bcd >> 4) & 0x000F) + 0x0030;
    formatted_data[4] = ((*mag_x_bcd) & 0x000F) + 0x0030;
    formatted_data[7] = ((*mag_y_bcd >> 12) & 0x000F) + 0x0030;//adds 30H to make the digit a valid ascii digit
    formatted_data[8] = ((*mag_y_bcd >> 8) & 0x000F) + 0x0030;
    formatted_data[9] = ((*mag_y_bcd >> 4) & 0x000F) + 0x0030;
    formatted_data[10] = ((*mag_y_bcd) & 0x000F) + 0x0030;
    formatted_data[13] = ((*mag_z_bcd >> 12) & 0x000F) + 0x0030;//adds 30H to make the digit a valid ascii digit
    formatted_data[14] = ((*mag_z_bcd >> 8) & 0x000F) + 0x0030;
    formatted_data[15] = ((*mag_z_bcd >> 4) & 0x000F) + 0x0030;
    formatted_data[16] = ((*mag_z_bcd) & 0x000F) + 0x0030;
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

void transmit_magnitude_to_pc(uint16_t *magnitude_bcd,
                        uint16_t *new_mag_data)
{
    //disable interrupts so that uart is not interrupted...
    INTERRUPT_GlobalDisable();
    
    *new_mag_data = 0;
    //data format will be [magnitude_dir][magdigit4][magdigit3][magdigit2][magdigit1][newline character]
    //the data will look like +xxxx\n
    uint16_t formatted_data[5] = {0, 0, 0, 0, 0x000A};//0AH is newline char
    
    
    formatted_data[0] = ((*magnitude_bcd >> 12) & 0x000F) + 0x0030;//adds 30H to make the digit a valid ascii digit
    formatted_data[1] = ((*magnitude_bcd >> 8) & 0x000F) + 0x0030;
    formatted_data[2] = ((*magnitude_bcd >> 4) & 0x000F) + 0x0030;
    formatted_data[3] = ((*magnitude_bcd) & 0x000F) + 0x0030;
    
    uint16_t ii = 0;
    while(ii < 5) // 4 is the number of digits that need to be transmitted
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

void init_magnetometer(void)
{
    uint16_t conf_cmd = ctrl_3_reg; //sends configuration command to set to continuous conversion mode
    SS1_SetLow();
    SPI1_Exchange16bit(conf_cmd);
    SS1_SetHigh();
}
