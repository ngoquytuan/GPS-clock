#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "gps.h"
#include "stm32g4xx_hal.h"
#if (GPS_DEBUG == 1)
#include <usbd_cdc_if.h>
#endif

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

uint8_t rx1_data = 0;
uint8_t rx1_buffer[GPSBUFSIZE];
uint8_t rx1_index = 0;

uint8_t rx2_data = 0;
uint8_t rx2_buffer[GPSBUFSIZE];
uint8_t rx2_index = 0;

GPS_t GPS;

#if (GPS_DEBUG == 1)
void GPS_print(char *data){
	char buf[GPSBUFSIZE] = {0,};
	sprintf(buf, "%s\n", data);
	printf(buf);
	CDC_Transmit_FS((unsigned char *) buf, (uint16_t) strlen(buf));
}
#endif



void GPS_Init(void)
{
	//HAL_UART_Abort(GPS1_USART);
	//HAL_UART_Receive_IT(GPS1_USART, &rx1_data, 1);
	HAL_UART_Abort(GPS2_USART);
	HAL_UART_Receive_IT(GPS2_USART, &rx2_data, 1);
}

void GPS_UART_CallBack(void){
	if (rx1_data != '\n' && rx1_index < sizeof(rx1_buffer)) {
		rx1_buffer[rx1_index++] = rx1_data;
		//printf("GPS: %s\r\n",rx1_buffer);
	} else {

		#if (GPS_DEBUG == 1)
		//GPS_print((char*)rx1_buffer);
		printf("GPS: %s\r\n",rx1_buffer);
		#endif
    //printf("GPS: %s\r\n",rx1_buffer);
		if(GPS_validate((char*) rx1_buffer))
		{
			GPS_parse((char*) rx1_buffer,1);
			//printf("utc_time 1 %f\r\n",GPS.utc_time);
		}
		rx1_index = 0;
		memset(rx1_buffer, 0, sizeof(rx1_buffer));
	}
	HAL_UART_Receive_IT(GPS1_USART, &rx1_data, 1);
	//huart1.pRxBuffPtr = (uint8_t *)rx_data;
  //huart1.RxXferCount = 1;
}

void GPS2_UART_CallBack(void){
	if (rx2_data != '\n' && rx2_index < sizeof(rx2_buffer)) {
		rx2_buffer[rx2_index++] = rx2_data;
		//printf("GPS: %s\r\n",rx2_buffer);
	} else {

		#if (GPS_DEBUG == 1)
		//GPS_print((char*)rx2_buffer);
		printf("GPS: %s\r\n",rx2_buffer);
		#endif
    //printf("GPS: %s\r\n",rx2_buffer);
		if(GPS_validate((char*) rx2_buffer))
			{
				GPS_parse((char*) rx2_buffer,2);
				//printf("utc_time 2 %f\r\n",GPS.utc_time);
			}
		rx2_index = 0;
		memset(rx2_buffer, 0, sizeof(rx2_buffer));
	}
	HAL_UART_Receive_IT(GPS2_USART, &rx2_data, 1);
	//huart1.pRxBuffPtr = (uint8_t *)rx_data;
  //huart1.RxXferCount = 1;
}

int GPS_validate(char *nmeastr){
    char check[3];
    char checkcalcstr[3];
    int i;
    int calculated_check;
    
    i=0;
    calculated_check=0;

    // check to ensure that the string starts with a $
    if(nmeastr[i] == '$')
        i++;
    else
        return 0;

		//printf("GPS_validate %s\r\n",nmeastr);
    //No NULL reached, 75 char largest possible NMEA message, no '*' reached
    while((nmeastr[i] != 0) && (nmeastr[i] != '*') && (i < 75)){
        calculated_check ^= nmeastr[i];// calculate the checksum
        i++;
    }

    if(i >= 75){
        return 0;// the string was too long so return an error
    }

    if (nmeastr[i] == '*'){
        check[0] = nmeastr[i+1];    //put hex chars in check string
        check[1] = nmeastr[i+2];
        check[2] = 0;
    }
    else
        return 0;// no checksum separator found there for invalid

    sprintf(checkcalcstr,"%02X",calculated_check);
    return((checkcalcstr[0] == check[0])
        && (checkcalcstr[1] == check[1])) ? 1 : 0 ;
}

void GPS_parse(char *GPSstrParse, char gps_no){
    if(!strncmp(GPSstrParse, "$GPGGA", 6)){
    	if (sscanf(GPSstrParse, "$GPGGA,%f,%f,%c,%f,%c,%d,%d,%f,%f,%c", &GPS.utc_time, &GPS.nmea_latitude, &GPS.ns, &GPS.nmea_longitude, &GPS.ew, &GPS.lock, &GPS.satelites, &GPS.hdop, &GPS.msl_altitude, &GPS.msl_units) >= 1){
    		GPS.dec_latitude = GPS_nmea_to_dec(GPS.nmea_latitude, GPS.ns);
    		GPS.dec_longitude = GPS_nmea_to_dec(GPS.nmea_longitude, GPS.ew);
    		return;
    	}
    }
    else if (!strncmp(GPSstrParse, "$GNRMC", 6)){
    	if(sscanf(GPSstrParse, "$GNRMC,%f,%f,%c,%f,%c,%f,%f,%d", &GPS.utc_time, &GPS.nmea_latitude, &GPS.ns, &GPS.nmea_longitude, &GPS.ew, &GPS.speed_k, &GPS.course_d, &GPS.date) >= 1)
				
			if(gps_no == 1) printf("utc_time 1 %f\r\n",GPS.utc_time);
			if(gps_no == 2) printf("utc_time 2 %f\r\n",GPS.utc_time);
    		return;

    }
    else if (!strncmp(GPSstrParse, "$GPGLL", 6)){
        if(sscanf(GPSstrParse, "$GPGLL,%f,%c,%f,%c,%f,%c", &GPS.nmea_latitude, &GPS.ns, &GPS.nmea_longitude, &GPS.ew, &GPS.utc_time, &GPS.gll_status) >= 1)
            return;
    }
    else if (!strncmp(GPSstrParse, "$GPVTG", 6)){
        if(sscanf(GPSstrParse, "$GPVTG,%f,%c,%f,%c,%f,%c,%f,%c", &GPS.course_t, &GPS.course_t_unit, &GPS.course_m, &GPS.course_m_unit, &GPS.speed_k, &GPS.speed_k_unit, &GPS.speed_km, &GPS.speed_km_unit) >= 1)
            return;
    }
}


float GPS_nmea_to_dec(float deg_coord, char nsew) {
    int degree = (int)(deg_coord/100);
    float minutes = deg_coord - degree*100;
    float dec_deg = minutes / 60;
    float decimal = degree + dec_deg;
    if (nsew == 'S' || nsew == 'W') { // return negative
        decimal *= -1;
    }
    return decimal;
}
