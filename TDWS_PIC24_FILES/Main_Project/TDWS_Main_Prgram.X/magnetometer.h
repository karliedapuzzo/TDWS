/* 
 * File:   magnetometer.h
 * Author: Karlie D'Apuzzo
 *
 * Created on January 1, 2025, 2:32 PM
 */

#ifndef MAGNETOMETER_H
#define	MAGNETOMETER_H

//will need a magISR routine in main

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define mag_x_regl 0x2800 //address x_data low byte in magnetometer registers
#define mag_x_regh 0x2900 //address x_data high byte in magnetometer registers
#define mag_y_regl 0x2a00 //address y_data low byte in magnetometer registers
#define mag_y_regh 0x2b00 //address y_data high byte in magnetometer registers
#define mag_z_regl 0x2c00 //address z_data low byte in magnetometer registers
#define mag_z_regh 0x2d00 //address z_data high byte in magnetometer registers

#define status_reg 0x2700 //address of status register in magnetometer
#define ctrl_3_reg 0x2200 //address of control register 3 in magnetometer

#define dummy_reg 0x0fff; //address of dummy register in magnetometer
#define read_bit 0x8000; //sets instruction to read
#define write_bit 0x0000; //sets instruction to write



/*
     * Function: binary_to_bcd
     * ---------------------------
     * converts unsigned binary value to a bcd value
     *
     * binary_data: pointer to binary value
     * bcd_data: pointer to bcd value
     *
     * returns: none
*/
void binary_to_bcd(uint16_t *binary_data, uint16_t *bcd_data);


/*
     * Function: read_xaxis
     * ---------------------------
     * reads the x-axis of the magnetometer data
     *
     * x_data: pointer to x-axis data
     *
     *
     * returns:
*/
void read_xaxis(uint16_t *x_data, uint16_t *x_dir);

/*
     * Function: read_yaxis
     * ---------------------------
     * reads the y-axis of the magnetometer data
     *
     * y_data: pointer to y-axis data
     *
     *
     * returns:
*/
void read_yaxis(uint16_t *y_data, uint16_t *y_dir);

/*
     * Function: read_zaxis
     * ---------------------------
     * reads the z-axis of the magnetometer data
     *
     * z_data: pointer to z-axis data
     *
     *
     * returns:
*/
void read_zaxis(uint16_t *z_data, uint16_t *z_dir);

/*
     * Function: transmit_mag_to_pc
     * ---------------------------
     * transmit magnetometer data to pc via uart4 module
     *
     * mag_x_bcd: pointer to binary coded decimal x-axis data
     * mag_x_dir: pointer to x-axis direction
     * mag_y_bcd: pointer to binary coded decimal y-axis data
     * mag_y_dir: pointer to y-axis direction
     * mag_z_bcd: pointer to binary coded decimal z-axis data
     * mag_z_dir: pointer to z-axis direction
     *
     * returns:
*/
void transmit_mag_to_pc(uint16_t *mag_x_bcd, uint16_t *mag_x_dir,
                        uint16_t *mag_y_bcd, uint16_t *mag_y_dir,
                        uint16_t *mag_z_bcd, uint16_t *mag_z_dir,
                        uint16_t *new_mag_data);


void init_magnetometer(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MAGNETOMETER_H */

