/* 
 * File:   radar.h
 * Author: Owner
 *
 * Created on January 1, 2025, 2:32 PM
 */

#ifndef RADAR_H
#define	RADAR_H

#ifdef	__cplusplus
extern "C" {
#endif
    
struct RadarData;
struct RadarParam;

uint8_t RADAR_facreset(void);

uint8_t RADAR_disconnect(void);

uint8_t RADAR_init(uint8_t baud_setting);

uint8_t RADAR_mindetzone(uint8_t minimum_range);

uint8_t RADAR_maxdetzone(uint8_t maximum_range);

uint8_t RADAR_threshoffset(uint8_t thresh_offset);

uint8_t RADAR_nexttdat(struct RadarData *result);

uint8_t RADAR_readparam(struct RadarParam *result);

uint8_t RADAR_speedset(uint8_t speed_setting);

uint8_t RADAR_rangeset(uint8_t range_setting);

uint8_t RADAR_filttype(uint8_t filter_type);

uint8_t RADAR_minangle(int8_t min_angle);

uint8_t RADAR_maxangle(int8_t max_angle);

uint8_t RADAR_minspeed(uint8_t min_speed);

uint8_t RADAR_maxspeed(uint8_t max_speed);

uint8_t RADAR_directset(uint8_t direction_setting);

uint16_t UINT8to16(uint8_t high_byte, uint8_t low_byte);

int16_t UINT8toINT16(uint8_t high_byte, uint8_t low_byte);

void RADAR_printhead(void);

void RADAR_printdecimal(uint16_t number);

void RADAR_printdata(struct RadarData *results);

#ifdef	__cplusplus
}
#endif

#endif	/* RADAR_H */

