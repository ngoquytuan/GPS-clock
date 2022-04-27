#ifndef _SNMP_CUSTOM_H_
#define _SNMP_CUSTOM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#include "snmp.h"

extern dataEntryType snmpData[];
extern const int32_t maxData;

// Define for using W5500-EVB: H/W Dependency (e.g., LEDs...)
//#define _USE_WIZNET_W5500_EVB_

#define COMMUNITY					"public\0"
#define COMMUNITY_SIZE				(strlen(COMMUNITY))

/* Predefined function: Response value control */
void initTable(void);


/* SNMP Trap: warmStart(1) */
void initial_Trap(uint8_t * SmanagerIP, uint8_t * SagentIP);

#ifdef __cplusplus
}
#endif

#endif
