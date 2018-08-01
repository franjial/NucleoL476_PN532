/*
 * adafruit_pn532.h
 *
 *  Created on: 27 jul. 2018
 *      Author: FJJA
 */

#ifndef INC_ADAFRUIT_PN532_H_
#define INC_ADAFRUIT_PN532_H_

#include "mistipos.h"
#include "periph_i2c.h"

#define PN532_I2C_ADDRESS 0x48

#define PN532_WAITRESP (uint32_t)1000

#define PN532_HOSTTOPN532                   (0xD4)
#define PN532_PN532TOHOST                   (0xD5)

#define PN532_ERR_INVALID_RESP              (ERRORCODE)0x40000001
#define PN532_ERROR_TOUT                    (ERRORCODE)0x40000002

#define PN532_PTT_CHOICES uint8_t /* passive target type */
#define PN532_B106TYPEA   (PN532_PTT_CHOICES)0x00
#define PN532_B212        (PN532_PTT_CHOICES)0x01
#define PN532_B424        (PN532_PTT_CHOICES)0x02
#define PN532_B106TYPEB   (PN532_PTT_CHOICES)0x03
#define PN532_B106JEWEL   (PN532_PTT_CHOICES)0x04




typedef struct {
	PeriphI2C* i2c;

	uint8_t ic;
	uint8_t ver;
	uint8_t rev;
	uint8_t support;
}PN532;



/** \addtogroup <public functions> 
 *  @{
 */
ERRORSTATUS PN532_On(PN532* this, PeriphI2C* wire, ERRORCODE* error);
ERRORSTATUS PN532_Off(PN532* this, ERRORCODE* error);
ERRORSTATUS PN532_WakeUp(PN532* this, ERRORCODE* error);

/**
 * @brief detect as many targets (maximum maxTg) as possible in passive mode
 * @param this
 * @param maxTg[in]     maximum number of targets to be initilized. this field should not exceed 0x02
 * @param brTy[in]      baud rate and the modulation type to be used during the initialization.
 * 	- PN532_B106TYPEA  
 *	- PN532_B212
 *	- PN532_B424
 *	- PN532_B106TYPEB
 *	- PN532_B106JEWEL
 * @param initiatorData  is an array of data to be used during the initializtion of the target(s).
 *   Depending on the Baud Rate specified, the content of this field is different. Read page 115 of UM0701 document.
 * @param error
 */
ERRORSTATUS PN532_InListPassiveTarget(PN532* this, uint8_t maxTg, PN532_PTT_CHOICES brTy, char* initiatorData, ERRORCODE* error);



/**
 * @brief this command is used to support protocol data exchanges between the PN532 as initiator and a target
 * @param tg[out]  is a byte containig the logical number of the relevant target.
 * @param dataOut[out] array of raw data to be sent to the target by the PN532.
 */
ERRORSTATUS PN532_InDataExchange(PN532* this, int tg, const char* dataOut, ERRORCODE* error);

ERRORSTATUS PN532_GetFirmwareVersion(PN532* this, ERRORCODE* error);
ERRORSTATUS PN532_AbortCommand(PN532* this, ERRORCODE* error);


ERRORSTATUS PN532_WriteACK(PN532* this, ERRORCODE* error);
ERRORSTATUS PN532_ReadACK(PN532* this, ERRORCODE* error);

/** @}*/

/** \addtogroup <private functions> 
 *  @{
 */


//TODO funciones privadas deben ser static
ERRORSTATUS PN532_WriteNIFrame(PN532* this, const char *data, ERRORCODE* error);
ERRORSTATUS PN532_ReadNIFrame(PN532* this, char *data, int timeout, ERRORCODE* error);


/* util */
BOOL PN532_IsACK(const char* frame, ERRORCODE* error);
BOOL PN532_IsNACK(const char* frame, ERRORCODE* error);

/** @}*/

#endif /* INC_ADAFRUIT_PN532_H_ */
