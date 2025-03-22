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
#include "radar.h"
#include "magnetometer.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/uart2.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
/*
                         Main application
 */

#define magnet_thresh 80 //threshold for when we think a train passed by

// Radar struct for detection modules
struct RadarData {
    uint8_t num_targets;
    uint8_t distance_low[8];
    uint8_t distance_high[8];
    int8_t speed_low[8];
    int8_t speed_high[8];
    int8_t angle_low[8];
    int8_t angle_high[8];
    uint8_t magnitude_low[8];
    uint8_t magnitude_high[8]; 
    uint8_t identification[8];
};

//LORA FUNCTIONS BELOW HERE
uint16_t uart2_rxcount = 0;
char received_msg[150];
uint16_t message_available = 0;

//reads one character and puts it in a global character array for other functions to parse
void uart2_ISR(void)//uart takes a really long time so we will need to disable all other processes while uart is happening I think
{
    if(message_available == 0)
    {
        //disable timer2 so that nothing else can get in between uart2 interrupts
        T2CONbits.TON = 0;
        TMR2 = 0;//sets timer2 to 0 count
        IFS0bits.T2IF = 0;//clear the flag so that it doesn't go to the ISR and also sets it to be ready when it get re-enabled
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
    else//onces a finished message is sent ignore anything afterwards/throw into the void: also re-enable timer2 interrupt
    {//this should never evaluate, I think
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
        cmd[10] = 'p';//this does nothing/is to keep here while waiting for the response
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
        cmd[10] = 'p';//this does nothing/is to keep here while waiting for the response
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
        cmd[10] = 'p';//this does nothing/is to keep here while waiting for the response
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
        cmd[10] = 'p';//this does nothing/is to keep here while waiting for the response
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
char msg2[149];//contains ABCD\n

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
                msg1[ii] = received_msg[ii];
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

uint8_t LoRa_transmit_msg(char *msg)//this should transmit a message over LoRa
{
    char cmd[64];
    sprintf(cmd, "AT+TEST=TXLRSTR,\"%s\"\r\n", msg);
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
        cmd[10] = 'p';//this does nothing/is to keep here while waiting for the response
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
//LORA FUNCTIONS ABOVE HERE


//unsigned integer values of x, y, z with corresponding direction
static uint16_t mag_x_val = 0;
static uint16_t mag_x_dir = 0;

static uint16_t mag_y_val = 0;
static uint16_t mag_y_dir = 0;

static uint16_t mag_z_val = 0;
static uint16_t mag_z_dir = 0;

//integer values of x, y, z
static int16_t mag_x = 0;
static int16_t mag_y = 0;
static int16_t mag_z = 0;

static uint16_t new_mag_data = 0;

static int32_t mag_x_cal = 0;
static int32_t mag_y_cal = 0;
static int32_t mag_z_cal = 0;

uint16_t magISR(void)//maybe takes to long :/
{
    //read x direction
    read_xaxis(&mag_x_val, &mag_x_dir, &mag_x);//returns magnitude and direction
    //read y direction
    read_yaxis(&mag_y_val, &mag_y_dir, &mag_y);
    //read z direction
    read_zaxis(&mag_z_val, &mag_z_dir, &mag_z);
    //new data flag
    new_mag_data = 1;
    return 1; 
    //new data was collected so return 1
}

#define FILTER_ORDER 101  // Number of filter coefficients (order + 1)
#define SAMPLING_RATE 5000  // Sampling rate in Hz
#define CUTOFF_FREQUENCY 10  // Cutoff frequency in Hz

// Define the FIR filter coefficients (hardcoded for the low-pass filter)
float fir_coefficients[FILTER_ORDER] = {
    0.00142746, 0.00144611, 0.0014972, 0.0015807, 0.00169644, 0.00184411,
    0.00202327, 0.00223336, 0.00247365, 0.00274333, 0.00304144, 0.00336689,
    0.00371851, 0.00409497, 0.00449486, 0.00491668, 0.0053588, 0.00581954,
    0.0062971, 0.00678963, 0.00729521, 0.00781187, 0.00833755, 0.0088702,
    0.0094077, 0.00994792, 0.0104887, 0.0110279, 0.01156335, 0.01209291,
    0.01261446, 0.0131259, 0.01362516, 0.01411025, 0.0145792, 0.01503012,
    0.01546118, 0.01587063, 0.01625683, 0.0166182, 0.01695328, 0.0172607,
    0.01753921, 0.0177877, 0.01800513, 0.01819063, 0.01834345, 0.01846296,
    0.01854867, 0.01860024, 0.01861745, 0.01860024, 0.01854867, 0.01846296,
    0.01834345, 0.01819063, 0.01800513, 0.0177877, 0.01753921, 0.0172607,
    0.01695328, 0.0166182, 0.01625683, 0.01587063, 0.01546118, 0.01503012,
    0.0145792, 0.01411025, 0.01362516, 0.0131259, 0.01261446, 0.01209291,
    0.01156335, 0.0110279, 0.0104887, 0.00994792, 0.0094077, 0.0088702,
    0.00833755, 0.00781187, 0.00729521, 0.00678963, 0.0062971, 0.00581954,
    0.0053588, 0.00491668, 0.00449486, 0.00409497, 0.00371851, 0.00336689,
    0.00304144, 0.00274333, 0.00247365, 0.00223336, 0.00202327, 0.00184411,
    0.00169644, 0.0015807, 0.0014972, 0.00144611, 0.00142746
};

// Static buffer to store the latest `FILTER_ORDER` input samples
static float buffer[FILTER_ORDER] = {0.f};  
static int buffer_index = 0;  // Index to keep track of the current position in the buffer

// FIR filter function using float calculations and fixed-point output
int16_t fir_filter(int16_t input_sample) {
    float accumulator = 0.0f;
    int16_t filtered_value;

    // Insert the new sample into the static buffer
    buffer[buffer_index] = (float)input_sample;

    // Apply FIR filter (convolution)
    for (int i = 0; i < FILTER_ORDER; i++) {
        // Circular access to the buffer: (index - i + FILTER_ORDER) % FILTER_ORDER ensures circular behavior
        int circular_index = (buffer_index - i + FILTER_ORDER) % FILTER_ORDER;
        accumulator += buffer[circular_index] * fir_coefficients[i];
    }

    // Move to the next index in the static buffer
    buffer_index = (buffer_index + 1) % FILTER_ORDER;

    // Convert the floating-point result back to int16 and clip to 16-bit range
    filtered_value = (int16_t)(accumulator);  // Direct cast to int16
    if (filtered_value > 32767) {
        filtered_value = 32767;
    } else if (filtered_value < -32768) {
        filtered_value = -32768;
    }

    return filtered_value;
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

void calibrate_mag_data(void)//makes calibration values
{
    uint16_t num_samples = 0;
    while(num_samples < 40)
    {
        if (new_mag_data == 1)
        {
            mag_x_cal += mag_x;
            mag_y_cal += mag_y;
            mag_z_cal += mag_z;
            num_samples++;
            new_mag_data = 0;
        }
        
    }
    mag_x_cal = mag_x_cal / (int32_t)num_samples;
    mag_y_cal = mag_y_cal / (int32_t)num_samples;
    mag_z_cal = mag_z_cal / (int32_t)num_samples;
}

uint16_t light_status = 0;
uint16_t tmr4_first = 1;

void tmr3_ISR(void)
{
    T4CONbits.TON = 0;//turns off t4
    TMR4 = 0;//clears tmr4 counter so that it starts from 0 when re-enabled
    IFS1bits.T4IF = 0;//clears tmr4 flag
    IO_RE0_SetLow();//turns off light1
    IO_RE1_SetLow();//turns off light2
    light_status = 2;//set light status to recently turned off status
    T3CONbits.TON = 0;//turn off timer 3 - turns itself off for funsies
    TMR3 = 0;//clears timer3 register so that it starts from zero the next time we run it.
    tmr4_first = 1;
}



void tmr4_ISR(void)
{
    //this is the timer4 isr which controls the alternation of the lights
    if(tmr4_first)
    {
        IO_RE0_SetHigh();//set one of the lights on initially
        tmr4_first = 0;
        light_status = 1;//changed light status to running
    }
    else
    {
        IO_RE0_Toggle();
        IO_RE1_Toggle();
    }
}

uint16_t detect_train(void)//reads mag data and outputs 1 or 0
{
    uint16_t magnitude_bcd = 0;
    float filtered_magnitude;
    float mag_magnitude = 0;
    float tmp1 = 0;
    float tmp2 = 0;
    float tmp3 = 0;
    
    float x_copy = 0;
    float y_copy = 0;
    float z_copy = 0;
    uint16_t out_mag = 0;
    //calibrate_mag_data();//set calibration values for upcoming mag data
    
    mag_magnitude = 0;
    //read mag and radar,
    //if train transmit
    //else transmit heartbeat if time to do so
    if(new_mag_data == 1)
    {
        INTERRUPT_GlobalDisable();//disable all interrupts so that all calcs can be finished before something else changes them....

        x_copy = mag_x;
        y_copy = mag_y;
        z_copy = mag_z;

        x_copy = x_copy - mag_x_cal;
        y_copy = y_copy - mag_y_cal;
        z_copy = z_copy - mag_z_cal;

        tmp1 = (float)(x_copy);
        tmp2 = (float)(y_copy);
        tmp3 = (float)(z_copy);

        tmp1 = powf(tmp1, 2.0f);
        tmp2 = powf(tmp2, 2.0f);
        tmp3 = powf(tmp3, 2.0f);

        mag_magnitude = sqrtf(tmp1 + tmp2 + tmp3);
        filtered_magnitude = fir_filter(mag_magnitude);
        out_mag = ((uint16_t)mag_magnitude);//put to unsigned integer values, unsigned is fine since it will come out of the filter unsigned anyway
        //binary_to_bcd(&out_mag, &magnitude_bcd);
        //transmit_magnitude_to_pc(&magnitude_bcd, &new_mag_data);
        new_mag_data = 0;
        
        if(out_mag >= magnet_thresh)
        {
            INTERRUPT_GlobalEnable();
            return 1;
        }
        else
        {
            INTERRUPT_GlobalEnable();
            return 0;
        }
       
        INTERRUPT_GlobalEnable();
    }
    return 0;
}

//error flags

int16_t det1_mod_error = 0;
int16_t det2_mod_error = 0;

//MODES BELOW
int main_mode(void)
{
    init_LoRa_rx();//initialize LoRa as reciever
    
    //turn on t2 to get calibration values for magenetometer
    //once calibration is finished disable the timer again so that the reciever works properly
    TMR2 = 0;//clears tmr2 counter so that it starts from 0 when re-enabled
    T2CONbits.TON = 1;//turns on t2
    uint16_t t2_warmup = 0;
    while(t2_warmup < 0x00FF)
    {
        t2_warmup++;//let mag chill for a sec before taking calibration
    }
    calibrate_mag_data();
    T2CONbits.TON = 0;//turns off t2
    TMR2 = 0;//resets timer2
    t2_warmup = 0;
    
    uint16_t new_msg = 0;
    uint16_t train_detected = 0;
    uint16_t wait_for_train = 0;
    while(1)
    {
        here = 0;
        //this is the master mode,
        /*
         * the order of ops for this mode will be
         * -setup magnetometer calibration data
         * in this mode we only care about reading mag data when we receive something from either detection module
         */
        //if we recieve a message it will be two messages each is stored in a buffer called msg1 and msg2
        if(message_available)
        {
            read_message();
            new_msg++;
        }
        if(new_msg >= 2)//do stuff in here based on the message
        {
            //read msg2 to see check detection module status and if it saw something
                //message will be a string of chars where like ABCD, A is the status char, B and C are the ID chars, D is the train detected char
            //if module saw something yipee!! also set status flag or reset heartbeat timer
            if(msg2[0] == '0')//check status of module: 0 is okay
            {
                if(msg2[3] == '1')//check last char of msg, if it's a 1 it means it saw a train
                {
                    //disable uart2 because if it goes off it turns off t2 :P
                    U2MODEbits.UARTEN = 0;
                    T4CONbits.TON = 1;//turns on timer4 which alternates the lights
                    wait_for_train = 1;//wait for mag to detect train
                    T2CONbits.TON = 1;
                    while(t2_warmup < 0x00FF)
                    {
                        t2_warmup++;//let mag chill for a sec before taking calibration
                        train_detected = detect_train();
                    }
                    t2_warmup = 0;
                    train_detected = 0;
                    while(wait_for_train)
                    {
                        if(train_detected)
                        {
                            T2CONbits.TON = 0;//turn off timer2 because we are done looking for the train, we found it :)
                            T3CONbits.TON = 1;//turns on 2 min light disabling timer
                            wait_for_train = 0;//leave mag data reading loop
                        }
                        else
                        {
                            //lmao make sure that tmr2 is still enabled :)
                            T2CONbits.TON = 1;
                            train_detected = detect_train();//see if there is a train
                        }
                    }
                    while(light_status < 2)
                    {
                        //wait here while the timers count down to disable the light
                    }
                    //once lights are disabled reset the light status flag and train seen status
                    light_status = 0;
                    train_detected = 0;
                    U2MODEbits.UARTEN = 1;//re-enable uart
                }
                else
                {
                    //train not detected
                }
            }
            else if(msg2[0] == '1')//if not okay raise error flags
            {
                if(msg2[2] == '0')//if ID = 0 then module 1 has error
                {
                    det1_mod_error = 1;
                }
                else if(msg2[2] == '1')//if ID = 1 then module 2 has error
                {
                    det2_mod_error = 1;
                }
            }
            new_msg = 0;
        }
        else
        {
            //do nothing :)
            new_msg = new_msg;
        }
    }
}


//ID of mod1 = "00"
//ID of mod2 = "01";

uint16_t init_det_radar(void)//change this to set for detecting trains :)
{
    //U2MODEbits.UARTEN = 0;//disable uart2, i.e. LoRa uart (for testing purposes)
    uint16_t error = 0;
    
    if (RADAR_facreset() != 0){//resets radar
        error++;
    }
    if (RADAR_init(0) != 0){//set baud rate I think
        error++;
    }
    if (RADAR_speedset(1) !=0){//set speed range
        error++;
    }
    if (RADAR_rangeset(0) !=0){//set detection range
        error++;
    }
    if (RADAR_mindetzone(0) !=0){//set closest detection zone
        error++;
    }
    if (RADAR_maxdetzone(2) !=0){//set furthest detection zone
        error++;
    }
    if (RADAR_minangle(-30) !=0){//set minimum detection angle, from straight on which = 0
        error++;
    }
    if (RADAR_maxangle(30) !=0){//set maximum detection angle, from straight on which = 0
        error++;
    }
    if (RADAR_minspeed(4) !=0){//set the slowest speed it can see
        error++;
    }
    if (RADAR_maxspeed(8) !=0){//set the fastest speed it can see
        error++;
    }
    return error;
    //U2MODEbits.UARTEN = 1;//re-enable lora uart
}

int detect_mod1(void)
{
    //this will be the detection mode1,
    /*
     * the order of ops for this mode will be
     * any initial setup.. i.e. 
     * -setup radar
     * main loop
     * 
     */
    static struct RadarData det_result;
    uint16_t all_good = 0;
    
    all_good = init_det_radar();//initialize all the radar settings
    
    uint16_t wait = 0;
    while(wait < 0xFFFF){
        wait++;
    }
    
    init_LoRa_tx();//initalize LoRa to transimit messages
    
    int16_t speed_out = 0;
    uint16_t transmitting = 0;
    
    char *positive_det_msg = "0001";// module status, module id1, module id 0, detection status
    //char positive_det_msg[] = {};
    
    while(1)
    {
        if (RADAR_nexttdat(&det_result) !=0){//reads radar data and puts into results
        }//kindof error checking :)
        if(det_result.num_targets != 0){//if the number of targets is not zero then check to see if the direction on the speed is coming towards us.
            //RADAR_printdata(&det_result);
            speed_out = UINT8toINT16(det_result.speed_high[0], det_result.speed_low[0]);
            if(speed_out < 0){
                while(transmitting == 0){
                    LoRa_transmit_msg(positive_det_msg);
                    transmitting = 1;
                }
                
            }
            RADAR_printdata(&det_result);
            transmitting = 0;
            speed_out = 0;//reset speed out
        }
        
        
    }
}

int detect_mod2(void)//basically the same as detect_mod1 but with a different lora address
{
    while(1)
    {
        here = 2;
        //this will be the detection mode2, this mode is the same as detect_mod1 but with a different address
        /*
         * the order of ops for this mode will be
         * any initial setup.. i.e. 
         * -setup for lora transmitter
         * -setup magnetometer calibration data
         * -setup radar
         * the main loop will be be where the action happens :D
         * 
         */
    }
}

int radar_test(void)
{
    struct RadarData result;
    
    U2MODEbits.UARTEN = 0;//disable uart2, i.e. LoRa uart (for testing purposes)
    
    if (RADAR_facreset() != 0){
        here = 3;
    }
    if (RADAR_init(0) != 0){
        here = 3;
    }
    if (RADAR_speedset(1) !=0){
        here = 3;
    }
    if (RADAR_rangeset(0) !=0){
        here = 3;
    }
    if (RADAR_mindetzone(0) !=0){
        here = 3;
    }
    if (RADAR_maxdetzone(2) !=0){
        here = 3;
    }
    if (RADAR_minangle(-30) !=0){
        here = 3;
    }
    if (RADAR_maxangle(30) !=0){
        here = 3;
    }
    if (RADAR_minspeed(4) !=0){
        here = 3;
    }
    if (RADAR_maxspeed(8) !=0){
        here = 3;
    }
    RADAR_printhead();
    
    while(1)
    {
        if (RADAR_nexttdat(&result) !=0){
            here = 3;
        }
        
        if(result.num_targets != 0){
            RADAR_printdata(&result);
            here = 3;
        }
        
    }
}

int mag_test(void) //I wasted so much time working on this, very sadge
{
    U2MODEbits.UARTEN = 0;
    uint16_t magnitude_bcd = 0;
    float filtered_magnitude;
    float mag_magnitude = 0;
    float tmp1 = 0;
    float tmp2 = 0;
    float tmp3 = 0;
    
    float x_copy = 0;
    float y_copy = 0;
    float z_copy = 0;
    uint16_t out_mag = 0;
    
    T2CONbits.TON = 1;
    uint16_t t2_warmup = 0;
    while(t2_warmup < 0x00FF)
    {
        t2_warmup++;
    }
    
    calibrate_mag_data();//set calibration values for upcoming mag data
    
    while(1)
    {
        mag_magnitude = 0;
        //read mag and radar,
        //if train transmit
        //else transmit heartbeat if time to do so
        if(new_mag_data == 1)
        {
            INTERRUPT_GlobalDisable();//disable all interrupts so that all calcs can be finished before something else changes them....
            
            x_copy = mag_x;
            y_copy = mag_y;
            z_copy = mag_z;
            
            x_copy = x_copy - mag_x_cal;
            y_copy = y_copy - mag_y_cal;
            z_copy = z_copy - mag_z_cal;
            
            tmp1 = (float)(x_copy);
            tmp2 = (float)(y_copy);
            tmp3 = (float)(z_copy);
            
            tmp1 = powf(tmp1, 2.0f);
            tmp2 = powf(tmp2, 2.0f);
            tmp3 = powf(tmp3, 2.0f);
            
            mag_magnitude = sqrtf(tmp1 + tmp2 + tmp3);
            filtered_magnitude = fir_filter(mag_magnitude);
            out_mag = ((uint16_t)mag_magnitude);
            binary_to_bcd(&out_mag, &magnitude_bcd);
            transmit_magnitude_to_pc(&magnitude_bcd, &new_mag_data);
            new_mag_data = 0;
            INTERRUPT_GlobalEnable();
        }
        
    }
}

int lora_receive_test(void)
{
    //init lora as receiver
    //wait and then read the message
    init_LoRa_rx();
    int16_t new_msg = 0;
    while(1)
    {
        here = 5;
        if(message_available)
        {
            read_message();
            new_msg++;
        }
        if(new_msg >= 2)//do stuff in here based on the message
        {
            //read msg2 to see check detection module status and if it saw something
                //message will be a string of chars where like ABCD, A is the status char, B and C are the ID chars, D is the train detected char
            //if module saw something yipee!! also set status flag or reset heartbeat timer
            new_msg = 0;
        }
    }
}

int lora_transmit_test(void)
{
    //init lora as transmitter
    //wait and send a test message
    init_LoRa_tx();
    char *message = "TestMessage";
    while(1)
    {
        int wait = 0;
        while(wait < 0xffff)
        {
            wait++;
            LoRa_transmit_msg(message);
        }
        here = 6;
    }
}

uint16_t test_light_cnt = 0;
int light_test(void)//works yipee!!!
{   
    T3CONbits.TON = 1;
    T4CONbits.TON = 1;//turns on t4 which alternates the lights
    //below is for testing purposes only
    T2CONbits.TON = 0;
    IFS0bits.T2IF = 0;//clear the flag so that it doesn't go to the ISR and also sets it to be ready when it get re-enabled
    while(1)
    {
        here = 7;
//        if(light_status = 0)
//        {
//            //run lights for 2 min 15 sec
//            T4CONbits.TON = 1;//turns on t4 which alternates the lights
//            light_status = 1;
//        }
//        else if(light_status = 2)//lights were recently disabled so wait some time before rerunning them/ this is for test purposes only
//        {
//            uint16_t wait = 0;
//            while(wait <= 0xFFFF)
//            {
//                wait++;
//            }
//        }
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
    
    TMR4_SetInterruptHandler(tmr4_ISR);
    TMR3_SetInterruptHandler(tmr3_ISR);
    TMR2_SetInterruptHandler(magISR); //set timer 2 interrupt to magnetometer ISR
    UART2_SetRxInterruptHandler(&uart2_ISR);//set uart2 receiever ISR
    
    //turn off tmr3 and tmr4, clear the timer registers
    T3CONbits.TON = 0;//turns off t3
    TMR3 = 0;//clears tmr3 counter so that it starts from 0 when re-enabled
    T4CONbits.TON = 0;//turns off t4
    TMR4 = 0;//clears tmr4 counter so that it starts from 0 when re-enabled
    
    T2CONbits.TON = 0;//turns off t2
    TMR2 = 0;//clears tmr2 counter so that it starts from 0 when re-enabled
    
    
    init_magnetometer(); //initialize magnetometer
    //init radar
    //lora mode set will be determined based on which mode we are in...
    
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

