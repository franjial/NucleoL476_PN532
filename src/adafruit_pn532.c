/*
 * adafruit_pn532.c
 *
 *  Created on: 27 jul. 2018
 *      Author: francisco
 */


#include <stm32l4xx.h>
#include "adafruit_pn532.h"
#include "periph_i2c.h"

ERRORSTATUS PN532_On(PN532* this, PeriphI2C* wire, ERRORCODE* error){

	this->i2c = wire;
	I2C_On(wire, error);
	return SUC;
}

ERRORSTATUS PN532_WakeUp(PN532* this, ERRORCODE* error){
	char frame[1];
	frame[0] = 0x00;
	if(I2C_Send(this->i2c, PN532_I2C_ADDRESS, frame, error) == SUC){
		I2C_EndTransmission(this->i2c, error);
	}
	return SUC;
}

/**
 * @brief Write Normal Information Frame (NIFrame) to the NF532
 */
ERRORSTATUS PN532_WriteNIFrame(PN532* this, const char *data, ERRORCODE* error){
	char ni_frame[256];
	int i;
	uint8_t sum;

	if(data[0]+8 > 255){
		// TODO error code
		return ERR;
	}

	ni_frame[0] = data[0]+8;
	ni_frame[1] = PN532_PREAMBLE;
	ni_frame[2] = PN532_STARTCODE1;
	ni_frame[3] = PN532_STARTCODE2;
	ni_frame[4] = data[0]+1;
	ni_frame[5] = ~ni_frame[4]+1; // len checksum
	ni_frame[6] = PN532_HOSTTOPN532;

	sum = PN532_HOSTTOPN532;
	for(i=7; i<=ni_frame[0]-2;i++){
		sum+=data[i-6];
		ni_frame[i] = data[i-6];
	}

	ni_frame[((uint8_t)ni_frame[0])-1] = ~sum + 1; // checksum
	ni_frame[(uint8_t)ni_frame[0]] = PN532_POSTAMBLE;

	if(I2C_Send(this->i2c, PN532_I2C_ADDRESS, ni_frame, error) == SUC){
		I2C_EndTransmission(this->i2c, error);
	}

	return SUC;
}

/* util */
BOOL PN532_IsACK(const char* frame, ERRORCODE* error){
	// TODO check checksum

	int i;
	char expected[6];

	if(frame[0]!=7){
		//TODO error invalid frame
		return FALSE;
	}

	expected[0] = 0x00;
	expected[1] = 0x00;
	expected[2] = 0xff;
	expected[3] = 0x00;
	expected[4] = 0xff;
	expected[5] = 0x00;

	for(i=0; i<6; i++){
		if(frame[i+2] != expected[i]){
			return FALSE;
		}
	}

	return TRUE;
}
