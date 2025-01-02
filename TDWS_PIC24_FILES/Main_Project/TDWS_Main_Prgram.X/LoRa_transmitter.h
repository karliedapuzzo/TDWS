/* 
 * File:   LoRa.h
 * Author: Owner
 *
 * Created on January 1, 2025, 2:33 PM
 */

#ifndef LORA_H
#define	LORA_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*
     * Function: check_lora_status
     * ---------------------------
     * send command "AT" and checks to make sure an "OK" was recieved
     *
     * returns:
*/
uint8_t check_lora_status(void);

/*
     * Function: set_lora_mode
     * ---------------------------
     * sets the lora module to test mode for PTP communication
     *
     *
     * returns: bool
*/
uint8_t set_lora_mode(void);

/*
     * Function: set_lora_rfconfig
     * ---------------------------
     * sets the radio frequency configuration of the lora module
     *
     *
     * returns: bool
*/
uint8_t set_lora_rfconfig(void);

/*
     * Function: LoRa_transmit_msg
     * ---------------------------
     * sends a message to be transmitted by the lora module
     *
     * msg: 32 long character array of message to be sent
     *
     * returns: bool
*/
uint8_t LoRa_transmit_msg(char *msg[32]);

/*
     * Function: init_LoRa
     * ---------------------------
     * calls all nessecary functions to initialize the lora module with a little bit of error checking
     *
     *
     * returns:
*/
void init_LoRa(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* LORA_H */

