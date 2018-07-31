#ifndef MISTIPOS_H
#define MISTIPOS_H

#include <stdint.h>

#ifndef NULL
#define NULL 0x00000000
#endif

#define BOOL uint32_t
#define TRUE  (BOOL)0x00000001
#define FALSE (BOOL)0x00000000

#define ERRORSTATUS uint32_t
#define ERR   (ERRORSTATUS)0x00000000
#define SUC   (ERRORSTATUS)0x00000001

#define FUNCTIONALSTATE uint32_t
#define ON    (FUNCTIONALSTATE)0x00000001
#define OFF   (FUNCTIONALSTATE)0x00000000

#define ERRORCODE uint32_t
#define ERR_RANGE (ERRORCODE)0x80000001
#define ERR_BSTAT (ERRORCODE)0x80000002 /*< estado del perif no permite operacion */

/* errores tarjeta SD */
#define ERR_SD_R1     (ERRORCODE)0x40000000
#define ERR_SD_BRESP  (ERRORCODE)0x40000100 /*< respuesta de comando  incorrecta */
#define ERR_SD_UNK    (ERRORCODE)0x40000200 /*< tarjeta desconocida */
#define ERR_SD_NRESP  (ERRORCODE)0x40000400 /*< comando no recibe respuesta */
#define ERR_SD_NINI   (ERRORCODE)0x40000800 /*< no encuentra inicio de un bloque */
#define ERR_SD_NLFREE (ERRORCODE)0x40000840 /*< no libera la linea */
#define ERR_SD_CNREAD (ERRORCODE)0x40000820 /*< comando leer bloque no responde */
#define ERR_SD_RTOKEN (ERRORCODE)0x40000810 /*< recibe read token al intentar leer */

/* errores genericos */
#define ERR_G_TOUT    (ERRORCODE)0x10000001 /*< timeout error */
#define ERR_G_BPAR    (ERRORCODE)0X10000002 /*< bad parameters */

#endif
