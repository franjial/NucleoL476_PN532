/*
 * adafruit_pn532.c
 *
 *  Created on: 27 jul. 2018
 *      Author: FJJA
 */


#include <stm32l4xx.h>
#include "adafruit_pn532.h"
#include "periph_i2c.h"

extern __weak uint32_t HAL_GetTick(void);

ERRORSTATUS PN532_On(PN532* this, PeriphI2C* wire, ERRORCODE* error){
	this->i2c = wire;
	I2C_On(wire, error);
	return SUC;
}

ERRORSTATUS PN532_WakeUp(PN532* this, ERRORCODE* error){
	char frame[1];
	ERRORSTATUS ret;

	frame[0] = 0x00;
	ret = I2C_Send(this->i2c, PN532_I2C_ADDRESS, frame, error);
	I2C_EndTransmission(this->i2c, error);
	return ret;
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
	ni_frame[1] = 0x00;
	ni_frame[2] = 0x00;
	ni_frame[3] = 0xff;
	ni_frame[4] = data[0]+1;
	ni_frame[5] = ~ni_frame[4]+1; // len checksum
	ni_frame[6] = PN532_HOSTTOPN532;

	sum = PN532_HOSTTOPN532;
	for(i=7; i<=ni_frame[0]-2;i++){
		sum+=data[i-6];
		ni_frame[i] = data[i-6];
	}

	ni_frame[((uint8_t)ni_frame[0])-1] = ~sum + 1; // checksum
	ni_frame[(uint8_t)ni_frame[0]] = 0x00; // postamble

	if(I2C_Send(this->i2c, PN532_I2C_ADDRESS, ni_frame, error) == SUC){
		I2C_EndTransmission(this->i2c, error);
	}

	return SUC;
}

ERRORSTATUS PN532_AbortCommand(PN532* this, ERRORCODE* error){
	return PN532_WriteACK(this, error);
}

ERRORSTATUS PN532_WriteACK(PN532* this, ERRORCODE* error){
	char ack_seq[7];
	ack_seq[0] = 6;
	ack_seq[1] = 0x00;
	ack_seq[2] = 0x00;
	ack_seq[3] = 0xff;
	ack_seq[4] = 0x00;
	ack_seq[5] = 0xff;
	ack_seq[6] = 0x00;

	if(I2C_Send(this->i2c, PN532_I2C_ADDRESS, ack_seq, error)==ERR){
		I2C_EndTransmission(this->i2c, error);
		return ERR;
	}

	I2C_EndTransmission(this->i2c, error);
	return SUC;
}

ERRORSTATUS PN532_ReadACK(PN532* this, ERRORCODE* error){
	BOOL finish;
	int i;
	char data[8];

	if(error!=NULL)
		*error = 0; // no error

	/* wait status */
	finish=FALSE;
	//TODO timeout
	while(finish==FALSE){
		i = 0;
		I2C_Recv(this->i2c, 7, data, error); /* read frame header */

		if(i==0 && data[1]==0x00){
			/* Each time a status byte is read with NOT READY information, before retrying the host
		     * controller must close the communication by sending an I2C STOP condition. */
			I2C_EndTransmission(this->i2c, error);
		}
		else if(data[1]==0x01){
			finish = TRUE;
		}
	}
	I2C_EndTransmission(this->i2c, error);            // stop condition

	if(PN532_IsACK(data, error)==TRUE){
		return SUC;
	}
	else if(PN532_IsNACK(data, error)==TRUE){
		/* returning ERR and errorcode=0 -> nack founded */
		return ERR;
	}
	else{
		if(error!=NULL)
			*error = PN532_ERR_INVALID_RESP;
		return ERR;
	}
	return SUC;
}

ERRORSTATUS PN532_ReadNIFrame(PN532* this, char *data, int timeout, ERRORCODE* error){

	BOOL finish;
	int i;
	uint8_t len = data[0];
	int t_ini;
	ERRORSTATUS st_recv;

	if(error!=NULL)
		*error = 0;

	/* wait status */
	finish=FALSE;
	t_ini = HAL_GetTick();
	while(finish==FALSE && HAL_GetTick()-t_ini<timeout){
		i = 0;

		st_recv = I2C_Recv(this->i2c, len+1, data, error); /* read frame header */

		if(st_recv==ERR){
			/* error reading */
			I2C_EndTransmission(this->i2c, error);
		}
		else if(i==0 && data[1]==0x00){
			/* Each time a status byte is read with NOT READY information, before retrying the host
		     * controller must close the communication by sending an I2C STOP condition. */
			I2C_EndTransmission(this->i2c, error);
		}
		else if(data[1]==0x01){
			finish = TRUE;
			I2C_EndTransmission(this->i2c, error);            // stop condition
		}
	}

	if((finish==FALSE) && (error!=NULL)){
		I2C_EndTransmission(this->i2c, error);
		*error = PN532_ERROR_TOUT;
	}
	return finish;

}

ERRORSTATUS PN532_GetFirmwareVersion(PN532* this, ERRORCODE* error){
	char data[21];
	data[0] = 1;
	data[1] = 0x02; /* command */
	PN532_WriteNIFrame(this, data, error);
	if(PN532_ReadACK(this, error) == SUC){
		data[0] = 20;
		if(PN532_ReadNIFrame(this, data, 1000, error) == SUC){
			this->ic = data[9];
			this->ver = data[10];
			this->rev = data[11];
			this->support = data[12];
			return SUC;
		}
	}

	return ERR;
}


ERRORSTATUS PN532_InListPassiveTarget(PN532* this, uint8_t maxTg, PN532_PTT_CHOICES brTy, char* initiatorData, ERRORCODE* error){
	char data[256];
	uint8_t len;

	if(initiatorData!=NULL){
		len = initiatorData[0]+3;
		//TODO copy initiatorData[1..] in data[4..]
	}
	else{
		len = 3;
	}

	data[0] = len;
	data[1] = 0x4A; /* command */
	data[2] = maxTg;
	data[3] = brTy;
	if(initiatorData!=NULL){
		//TODO str_cpy
	}
	PN532_WriteNIFrame(this, data, error);
	return SUC;
}

ERRORSTATUS PN532_InDataExchange(PN532* this, int tg, const char* dataOut, ERRORCODE* error){

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

BOOL PN532_IsNACK(const char* frame, ERRORCODE* error){
	//TODO
	return ERR;
}
