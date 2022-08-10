#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gps.h"
#include "stm32g4xx_hal.h"


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern volatile uint32_t tgps1,tgps2;

uint8_t rx1_data = 0;
uint8_t rx1_buffer[GPSBUFSIZE];
uint8_t rx1_index = 0;

uint8_t rx2_data = 0;
uint8_t rx2_buffer[GPSBUFSIZE];
uint8_t rx2_index = 0;

//GPS_t GPS1, GPS2;
char gps1_valid[2];
char gps2_valid[2];

char gps1_time[7];
char gps2_time[7];
char gps1_date[7];
char gps2_date[7];
char sat1_num[3];
char sat2_num[3];
//Number of satellites used
uint8_t numSat1=0;
uint8_t numSat2=0;
//khi nao buffer full thi no se goi ham nay
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{  
			
			
			if(UartHandle == &huart1) 
			{
				GPS_UART_CallBack();
			}
			
			if(UartHandle == &huart2) 
			{
				GPS2_UART_CallBack();
			}
				        
}

void GPS_Init(void)
{
	HAL_UART_Abort(GPS1_USART);
	HAL_UART_Receive_IT(GPS1_USART, &rx1_data, 1);
	HAL_UART_Abort(GPS2_USART);
	HAL_UART_Receive_IT(GPS2_USART, &rx2_data, 1);
	

	gps1_valid[0] = 'X';
	gps1_valid[1] = 0;
	
	gps2_valid[0] = 'X';
	gps2_valid[1] = 0;
	
	gps1_time[6] = 0;
	gps2_time[6] = 0;
	gps1_date[6] = 0;
	gps2_date[6] = 0;	
}

void GPS_UART_CallBack(void){
	
	uint8_t i,j;
	
	if (rx1_data != '\n' && rx1_index < sizeof(rx1_buffer)) 
		{
			rx1_buffer[rx1_index++] = rx1_data;
		} 
	else 
 {

	
    
	
		if(GPS_validate((char*) rx1_buffer))
		{			
			//
			//GPS1_parse((char*) rx1_buffer);
			if((rx1_buffer[0] == '$')&&(rx1_buffer[3] == 'R')&&(rx1_buffer[4] == 'M')&&(rx1_buffer[5] == 'C'))
			{
				//tgps1 = TIM3->CNT;
				//$GNRMC,031653.00,A,2102.50750,N,10552.87004,E,0.050,,190422,,,A*6B
				gps1_time[0] = rx1_buffer[7];
				gps1_time[1] = rx1_buffer[8];
				gps1_time[2] = rx1_buffer[9];
				gps1_time[3] = rx1_buffer[10];
				gps1_time[4] = rx1_buffer[11];
				gps1_time[5] = rx1_buffer[12]; 
				gps1_valid[0] = rx1_buffer[17];
				//printf("Time 1 %s\n",gps1_time);
				
				//Dem 9 so ',' de tim ngay thang nam
				i = 0;j=0;
				while((i<9) && (j<100))
				{
					if(rx1_buffer[j++] == ',') i++;
				}
				if((i == 9) && (rx1_buffer[j+6] ==','))//9 dau ','
				{
					gps1_date[0] = rx1_buffer[j];
					gps1_date[1] = rx1_buffer[j+1];
					gps1_date[2] = rx1_buffer[j+2];
					gps1_date[3] = rx1_buffer[j+3];
					gps1_date[4] = rx1_buffer[j+4];
					gps1_date[5] = rx1_buffer[j+5];
				}
				
			}
			if((rx1_buffer[0] == '$')&&(rx1_buffer[3] == 'G')&&(rx1_buffer[4] == 'G')&&(rx1_buffer[5] == 'A'))
			{
				//Dem 7 so ',' de tim Number of satellites used
				i = 0;j=0;
				while((i<7) && (j<100))
				{
					if(rx1_buffer[j++] == ',') i++;
				}
				if((i == 7)&& (rx1_buffer[j+2] == ',')) //8 dau ','
				{
					if((rx1_buffer[j] <= '9') && (rx1_buffer[j] >= '0')) sat1_num[0] = rx1_buffer[j];
					else sat1_num[0] = '0';
					if((rx1_buffer[j+1] <= '9') && (rx1_buffer[j+1] >= '0')) sat1_num[1] = rx1_buffer[j+1];
					else sat1_num[1] = '0';
					
					
					numSat1 = 10*(sat1_num[0] - '0') + (sat1_num[1] - '0');
					//printf("Sat1:%d\n\r",numSat1);
				}
			}
		}
		rx1_index = 0;
		memset(rx1_buffer, 0, sizeof(rx1_buffer));
	}
	HAL_UART_Receive_IT(GPS1_USART, &rx1_data, 1);
	
}

void GPS2_UART_CallBack(void)
	{
		uint8_t i,j;
		
	if (rx2_data != '\n' && rx2_index < sizeof(rx2_buffer))
	{
		rx2_buffer[rx2_index++] = rx2_data;
		//printf("GPS: %s\r\n",rx2_buffer);
	} 
	else 
	{

	
    
		
		if(GPS_validate((char*) rx2_buffer))
			{
				//printf("GPS 2 %s\n",rx2_buffer);
				//GPS2_parse((char*) rx2_buffer);
				if((rx2_buffer[0] == '$')&&(rx2_buffer[3] == 'R')&&(rx2_buffer[4] == 'M')&&(rx2_buffer[5] == 'C'))
				{
					//tgps2 = TIM3->CNT;
					gps2_time[0] = rx2_buffer[7];
					gps2_time[1] = rx2_buffer[8];
					gps2_time[2] = rx2_buffer[9];
					gps2_time[3] = rx2_buffer[10];
					gps2_time[4] = rx2_buffer[11];
					gps2_time[5] = rx2_buffer[12];
					gps2_valid[0] = rx2_buffer[17];
					//Dem 9 so ',' de tim ngay thang nam
					i = 0;j=0;
					while((i<9) && (j<100))
					{
						if(rx2_buffer[j++] == ',') i++;
					}
					if((i == 9) && (rx2_buffer[j+6] ==','))//9 dau ','
					{
						gps2_date[0] = rx2_buffer[j];
						gps2_date[1] = rx2_buffer[j+1];
						gps2_date[2] = rx2_buffer[j+2];
						gps2_date[3] = rx2_buffer[j+3];
						gps2_date[4] = rx2_buffer[j+4];
						gps2_date[5] = rx2_buffer[j+5];
					}
				}
			}
			if((rx2_buffer[0] == '$')&&(rx2_buffer[3] == 'G')&&(rx2_buffer[4] == 'G')&&(rx2_buffer[5] == 'A'))
			{
				//Dem 7 so ',' de tim 
				i = 0;j=0;
				while((i<7) && (j<100))
				{
					if(rx2_buffer[j++] == ',') i++;
				}
				if((i == 7)&& (rx2_buffer[j] != ',')) //7 dau ','
				{
					sat2_num[0] = rx2_buffer[j];
					sat2_num[1] = rx2_buffer[j+1];
					if(rx2_buffer[j+2] == ',') sat2_num[2] = 0;
					numSat2 = 10*(sat2_num[0] - '0') + (sat2_num[1] - '0');
					//printf("Sat2:%d\n\r",numSat2);
				}
			}
		rx2_index = 0;
		memset(rx2_buffer, 0, sizeof(rx2_buffer));
	}
	HAL_UART_Receive_IT(GPS2_USART, &rx2_data, 1);
	
}

int GPS_validate(char *nmeastr)
{
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

//void GPS_parse_old(char *GPSstrParse, char gps_no)
//	{
//    if(!strncmp(GPSstrParse, "$GPGGA", 6)){
//    	if (sscanf(GPSstrParse, "$GPGGA,%f,%f,%c,%f,%c,%d,%d,%f,%f,%c", &GPS1.utc_time, &GPS1.nmea_latitude, &GPS1.ns, &GPS1.nmea_longitude, &GPS1.ew, &GPS1.lock, &GPS1.satelites, &GPS1.hdop, &GPS1.msl_altitude, &GPS1.msl_units) >= 1){
//    		GPS1.dec_latitude = GPS_nmea_to_dec(GPS1.nmea_latitude, GPS1.ns);
//    		GPS1.dec_longitude = GPS_nmea_to_dec(GPS1.nmea_longitude, GPS1.ew);
//    		return;
//    	}
//    }
//		//$GNRMC,091505.00,A,2102.48977,N,10552.88132,E,0.348,326.83,310322,,,A*7D
//    else if (!strncmp(GPSstrParse, "$GNRMC", 6))
//			{
//			
//			
//			int rs = SplitGPS_Field(GPSstrParse,gps_no);
//			

//    }    
//		else if (!strncmp(GPSstrParse, "$GPGLL", 6)){
//        if(sscanf(GPSstrParse, "$GPGLL,%f,%c,%f,%c,%f,%c", &GPS1.nmea_latitude, &GPS1.ns, &GPS1.nmea_longitude, &GPS1.ew, &GPS1.utc_time, &GPS1.gll_status) >= 1)
//            return;
//    }
//    else if (!strncmp(GPSstrParse, "$GPVTG", 6)){
//        if(sscanf(GPSstrParse, "$GPVTG,%f,%c,%f,%c,%f,%c,%f,%c", &GPS1.course_t, &GPS1.course_t_unit, &GPS1.course_m, &GPS1.course_m_unit, &GPS1.speed_k, &GPS1.speed_k_unit, &GPS1.speed_km, &GPS1.speed_km_unit) >= 1)
//            return;		
//    }
//}

//void GPS1_parse(char *GPSstrParse)//, char gps_no)
//	{

//		//$GNRMC,091505.00,A,2102.48977,N,10552.88132,E,0.348,326.83,310322,,,A*7D
//    if (!strncmp(GPSstrParse, "$GNRMC", 6))// || !strncmp(GPSstrParse, "$GPRMC", 6))
//			{							
//				//printf("GPS1:%s\n", GPSstrParse );			
//				int rs = SplitGPS_Field(GPSstrParse,1);				
//			}			
//}
//	
//void GPS2_parse(char *GPSstrParse)//, char gps_no)
//	{

//		//$GNRMC,091505.00,A,2102.48977,N,10552.88132,E,0.348,326.83,310322,,,A*7D
//    if (!strncmp(GPSstrParse, "$GNRMC", 6))// || !strncmp(GPSstrParse, "$GPRMC", 6))
//			{							
//				//printf("GPS2:%s\n", GPSstrParse );			
//				int rs = SplitGPS_Field(GPSstrParse,2);				
//			}			
//}
	
//float GPS_nmea_to_dec(float deg_coord, char nsew) {
//    int degree = (int)(deg_coord/100);
//    float minutes = deg_coord - degree*100;
//    float dec_deg = minutes / 60;
//    float decimal = degree + dec_deg;
//    if (nsew == 'S' || nsew == 'W') { // return negative
//        decimal *= -1;
//    }
//    return decimal;
//}

//char *strtok_fr (char *s, char delim, char **save_ptr)
//{
//    char *tail;
//    char c;

//    if (s == NULL) {
//        s = *save_ptr;
//    }
//    tail = s;
//    if ((c = *tail) == '\0') {
//        s = NULL;
//    }
//    else {
//        do {
//            if (c == delim) {
//                *tail++ = '\0';
//                break;
//           }
//        }while ((c = *++tail) != '\0');
//    }
//    *save_ptr = tail;
//    return s;
//}

//char *strtok_f (char *s, char delim)
//{
//    static char *save_ptr;

//    return strtok_fr (s, delim, &save_ptr);
//}

//int SplitGPS_Field (char GPS_message[], int GPS_No)
//{
//    //char str[] = "$GPRMC,105954.000,A,3150.6731,N,11711.9399,E,0.00,96.10,250313,,,A*53";
//    //char str[] = "$GPRMC,105954.000,A,3150.6731,N,11711.9399,E,0.00,,250313,,,A*53";
//    char* Message_ID = strtok_f(GPS_message,',');
//    char* Time = strtok_f(NULL,',');
//    char* Data_Valid = strtok_f(NULL,',');
//    char* Raw_Latitude = strtok_f(NULL,',');
//    char* N_S = strtok_f(NULL,',');
//    char* Raw_Longitude = strtok_f(NULL,',');
//    char* E_W = strtok_f(NULL,',');
//    char* Speed = strtok_f(NULL,',');
//    char* COG = strtok_f(NULL,',');
//    char* Date = strtok_f(NULL,',');

//		
//		if ( Data_Valid[0]!='V' && Data_Valid[0]!= 'A')
//					Data_Valid[0] = 'X';
//		
//		if (GPS_No == 1)
//		{
//			GPS1.utc_time = atof(Time);
//			GPS1.date = atoi(Date);
//			GPS1.data_valid = Data_Valid[0];
//		}
//		
//		if (GPS_No == 2)
//		{
//			GPS2.utc_time = atof(Time);
//			GPS2.date = atoi(Date);
//			GPS2.data_valid = Data_Valid[0];
//		}
//    return 0;
//}

