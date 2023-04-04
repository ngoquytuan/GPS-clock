//last edit 6July2022 by tuannq
#include "main.h"
#include <stdint.h>
#include "stdio.h"
#include "wizchip_conf.h"
#include "w5500.h"
#include <string.h>
#include "snmp.h"
#include <time.h>
#include "httpServer.h"
#include "irigb.h"
#include "connector.h"
#include "socketdefines.h"
///////////////////////////////////////////////////////////////////////
// PHYStatus check ////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#define MSEC_PHYSTATUS_CHECK 		1000		// msec
uint16_t t_check_link_ms = 0;

//#define irigb_debug
//bo dem irigB 0-99
int8_t codecounter = 0;
volatile unsigned int hourcode=0, minutecode=0, secondcode=0, daycode=0, monthcode=0, yearcode=0, nodaycode=0;
volatile char time_buff1[7], time_buff2[7];
volatile char date_buff1[7], date_buff2[7];
unsigned char daysofmonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};
unsigned char daysofmonth2[12]={31,29,31,30,31,30,31,31,30,31,30,31};

extern uint8_t IRIGB_GAIN;
extern wiz_NetInfo myipWIZNETINFO;
extern volatile uint16_t phystatus_check_cnt;
extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern uint8_t u1Timeout;
extern uint8_t u2Timeout;
extern uint32_t timct;

extern time_t timenow;
extern struct tm currtime;
extern time_t unixTime_last_sync;
extern uint8_t serverPacket[];
extern time_t micros_recv;
extern time_t recvTime;
extern int8_t sat1,sat2,satInuse;

uint8_t phylink = PHY_LINK_OFF;
uint8_t unlock_config =0;

//for smnp
// lost signal : if after timeOutLostSignal seconds without GPS master message => lost
int8_t lostSignal = LOST_GPS_MASTER;
int8_t timeOutLostSignal = 30;
uint8_t gps1_stt = 0;
uint8_t gps2_stt = 0;
uint8_t power1_stt = 0;
uint8_t power2_stt = 0;

int8_t count_to_send_main=0;
uint32_t count_reset_ntp_server = 0;
uint32_t real_delay;

void U1RS485Transmit(const uint8_t *data,uint16_t length)
		{
			HAL_GPIO_WritePin(TimeRD_GPIO_Port, TimeRD_Pin, GPIO_PIN_SET);
			HAL_UART_Transmit(&huart1, data, length, 100);
			HAL_GPIO_WritePin(TimeRD_GPIO_Port, TimeRD_Pin, GPIO_PIN_RESET);
		}
void uart1_processing(void)//RS485
{
	if(u1Timeout == 1) 
			{
				u1Timeout = 0;
				//printf("Rx1Buffer %s; \r\n",Rx1Buffer);
				//U1RS485Transmit(Rx1Buffer,10);
				//control(aRxBuffer);

				huart1.pRxBuffPtr = (uint8_t *)Rx1Buffer;
				huart1.RxXferCount = RX1BUFFERSIZE;
				memset(Rx1Buffer,0,RX1BUFFERSIZE); 
			}
}
/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
	* Neu co GPS, se co xung PPS tai thoi diem bat dau moi giay
  * If GPS avaiable, PPS pulse start at the start of a second
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  //Dong bo xung PPS voi mach main GPS, moi lan co xung la thoi diem bat dau cua giay
	//sync the start of a second by pps signal
	if (GPIO_Pin == PPS_Pin)
  {
		//HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_SET);
		//printf("PPS main");
		fractionOfSecond = 9;
		//codecounter = 0;
		//timenow++;
		//real_delay = TIM3->CNT ;
		//TIM3->CNT = 0;
		//U1RS485Transmit("PPS main",10);
		
  }
	if (GPIO_Pin == INTn_Pin)
  {
		//Nhan duoc ban tin NTP, xu ly thoi gian va phan hoi
		//Receied NTP message, processing and respond
		//printf("NTP\r\n");
		INT_NTP();
  }

}
//Kiem tra W5500, SPI co on ko, day mang co cam ko?
uint8_t checkDaymang(void)
{
	uint8_t tmp;
  phylink = PHY_LINK_OFF;
	
	if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
	{
		#ifdef DebugEnable
		printf("Unknown PHY Link stauts.\r\n");//Loi spi???
		#endif
		return 0;
	}
	
	if(tmp == PHY_LINK_OFF) return 0;//Ko cam day mang
	
//	#ifdef DebugEnable
//	printf("Co cam day mang nhe\r\n");
//	#endif
	phylink = PHY_LINK_ON;
  return 1;  
}

void resetirigbcode(void)
{
unsigned char i=0;
	for (i=0;i<100;i++)
	{
		irigb_code[i] = IRIGB_bit0;
	}
	irigb_code[0] = IRIGB_bitP;irigb_code[9] = IRIGB_bitP;irigb_code[19] = IRIGB_bitP;irigb_code[29]=IRIGB_bitP;
	irigb_code[39]=IRIGB_bitP;irigb_code[49]=IRIGB_bitP;irigb_code[59]=IRIGB_bitP;irigb_code[69]=IRIGB_bitP;
	irigb_code[79]=IRIGB_bitP;irigb_code[89]=IRIGB_bitP;irigb_code[99]=IRIGB_bitP;
	
	hourcode=0;
	minutecode=0; 
	secondcode=0; 
	daycode=0; 
	monthcode=0; 
	yearcode=0; 
	nodaycode=0;
}

void build_irigB_code(void)
{
	uint8_t n=0;						
					
									
						hourcode   = hours;
						minutecode = minutes;
						secondcode = seconds;

						daycode   = days;
						monthcode = months;
						yearcode  = years;
	
#ifdef irigb_debug
						printf("HH-MM-SS :%d-%d-%d",hours,minutes,seconds);
						printf("DD-MM-YY :%d-%d-%d",days,months,years);
#endif
						for (n=0; n<monthcode-1;n++)
						{
							if ((yearcode%4)==0) nodaycode+=daysofmonth2[n];
							else nodaycode+=daysofmonth[n];
						}
						nodaycode+=daycode;
								

						if (secondcode>=40) 
																{
																	irigb_code[8]=IRIGB_bit1;
																	secondcode-=40;
																} 
																else 
																	irigb_code[8] = IRIGB_bit0;
																
						if (secondcode>=20) 
																{
																	irigb_code[7]=IRIGB_bit1;
																	secondcode-=20;
																}
																else 
																	irigb_code[7] = IRIGB_bit0;
						if (secondcode>=10) 
																{
																	irigb_code[6]=IRIGB_bit1;
																	secondcode-=10;
																}
																else 
																	irigb_code[6] = IRIGB_bit0;
						if (secondcode>=8) {irigb_code[4]=IRIGB_bit1;secondcode-=8;}
						else 
																irigb_code[4] = IRIGB_bit0;
						if (secondcode>=4) {irigb_code[3]=IRIGB_bit1;secondcode-=4;}
						else 
																irigb_code[3] = IRIGB_bit0;
						if (secondcode>=2) {irigb_code[2]=IRIGB_bit1;secondcode-=2;}
						else 
																irigb_code[2] = IRIGB_bit0;
						if (secondcode>=1) {irigb_code[1]=IRIGB_bit1;}
						else 
																irigb_code[1] = IRIGB_bit0;

						if (minutecode>=40) {irigb_code[17]=IRIGB_bit1;minutecode-=40;}
						else 
																irigb_code[17] = IRIGB_bit0;
						if (minutecode>=20) {irigb_code[16]=IRIGB_bit1;minutecode-=20;}
						else 
																irigb_code[16] = IRIGB_bit0;
						if (minutecode>=10) {irigb_code[15]=IRIGB_bit1;minutecode-=10;}
						else 
																irigb_code[15] = IRIGB_bit0;
						if (minutecode>=8) {irigb_code[13]=IRIGB_bit1;minutecode-=8;}
						else 
																irigb_code[13] = IRIGB_bit0;
						if (minutecode>=4) {irigb_code[12]=IRIGB_bit1;minutecode-=4;}
						else 
																irigb_code[12] = IRIGB_bit0;
						if (minutecode>=2) {irigb_code[11]=IRIGB_bit1;minutecode-=2;}
						else 
																irigb_code[11] = IRIGB_bit0;
						if (minutecode>=1) {irigb_code[10]=IRIGB_bit1;}
						else 
																irigb_code[10] = IRIGB_bit0;

						if (hourcode>=20) {irigb_code[26]=IRIGB_bit1;hourcode-=20;}
						else 
															 irigb_code[26] = IRIGB_bit0;
						if (hourcode>=10) {irigb_code[25]=IRIGB_bit1;hourcode-=10;}
						else 
															 irigb_code[25] = IRIGB_bit0;
						if (hourcode>=8) {irigb_code[23]=IRIGB_bit1;hourcode-=8;}
						else 
														  irigb_code[23] = IRIGB_bit0;
						if (hourcode>=4) {irigb_code[22]=IRIGB_bit1;hourcode-=4;}
						else 
														  irigb_code[22] = IRIGB_bit0;
						if (hourcode>=2) {irigb_code[21]=IRIGB_bit1;hourcode-=2;}
						else 
														  irigb_code[21] = IRIGB_bit0;
						if (hourcode>=1) {irigb_code[20]=IRIGB_bit1;}
						else 
														  irigb_code[20] = IRIGB_bit0;

					
						if (yearcode>=80) {irigb_code[58]=IRIGB_bit1;yearcode-=80;}
						else 
														   irigb_code[58] = IRIGB_bit0;
						if (yearcode>=40) {irigb_code[57]=IRIGB_bit1;yearcode-=40;}
						else 
														   irigb_code[57] = IRIGB_bit0;
						if (yearcode>=20) {irigb_code[56]=IRIGB_bit1;yearcode-=20;}
						else 
														   irigb_code[56] = IRIGB_bit0;
						if (yearcode>=10) {irigb_code[55]=IRIGB_bit1;yearcode-=10;}
						else 
														   irigb_code[55] = IRIGB_bit0;
						if (yearcode>=8) {irigb_code[53]=IRIGB_bit1;yearcode-=8;}
						else 
														  irigb_code[53] = IRIGB_bit0;
						if (yearcode>=4) {irigb_code[52]=IRIGB_bit1;yearcode-=4;}
						else 
														  irigb_code[52] = IRIGB_bit0;
						if (yearcode>=2) {irigb_code[51]=IRIGB_bit1;yearcode-=2;}
						else 
														  irigb_code[51] = IRIGB_bit0;
						if (yearcode>=1) {irigb_code[50]=IRIGB_bit1;}
						else 
														  irigb_code[50] = IRIGB_bit0;
					

						if (nodaycode>=200) {irigb_code[41]=IRIGB_bit1;nodaycode-=200;}
						else 
														     irigb_code[41] = IRIGB_bit0;
						if (nodaycode>=100) {irigb_code[40]=IRIGB_bit1;nodaycode-=100;}
						else 
														     irigb_code[40] = IRIGB_bit0;
						if (nodaycode>=80) {irigb_code[38]=IRIGB_bit1;nodaycode-=80;}
						else 
														    irigb_code[38] = IRIGB_bit0;
						if (nodaycode>=40) {irigb_code[37]=IRIGB_bit1;nodaycode-=40;}
						else 
														    irigb_code[37] = IRIGB_bit0;
						if (nodaycode>=20) {irigb_code[36]=IRIGB_bit1;nodaycode-=20;}
						else 
														    irigb_code[36] = IRIGB_bit0;
						if (nodaycode>=10) {irigb_code[35]=IRIGB_bit1;nodaycode-=10;}
						else 
														    irigb_code[35] = IRIGB_bit0;
						if (nodaycode>=8) {irigb_code[33]=IRIGB_bit1;nodaycode-=8;}
						else 
														   irigb_code[33] = IRIGB_bit0;
						if (nodaycode>=4) {irigb_code[32]=IRIGB_bit1;nodaycode-=4;}
						else 
														   irigb_code[32] = IRIGB_bit0;
						if (nodaycode>=2) {irigb_code[31]=IRIGB_bit1;nodaycode-=2;}
						else 
														   irigb_code[31] = IRIGB_bit0;
						if (nodaycode>=1) {irigb_code[30]=IRIGB_bit1;}
						else 
														   irigb_code[30] = IRIGB_bit0;
#ifdef irigb_debug						
						printf("IRIGB-code:\r\n");
						for(n = 0; n<100; n++)
						{
							if(irigb_code[n] == 2) printf("P ");
							else printf("%d ",irigb_code[n]);
						}
						printf("\r\n end IRIGB-code:\r\n");
#endif
}

//Ham chuyen doi char sang int
uint8_t convert_atoi( uint8_t c)
{
	return (uint8_t)c-48;
}
//Xu ly ban tin tu mach main
void main_message_handle(void)
{//=> Ban tin GPS: $GPS034007060819AA10	;$$GPS091259280422AA10
	if((aRxBuffer[0] =='$')&((aRxBuffer[1] =='G')|(aRxBuffer[1] =='g'))&((aRxBuffer[2] =='P')|(aRxBuffer[2] =='p'))&((aRxBuffer[3] =='S')|(aRxBuffer[3] =='s')))
	{
		//real_delay = TIM3->CNT;
		//TIM3->CNT = 0;
		//Vao den day thi it nhat da qua 63ms ???
		//fractionOfSecond = 639;
		//Truyen ban tin cho cac dong ho slave
		HAL_GPIO_WritePin(TimeRD_GPIO_Port, TimeRD_Pin, GPIO_PIN_SET);
		HAL_UART_Transmit(&huart1, aRxBuffer, 20, 100);
		HAL_GPIO_WritePin(TimeRD_GPIO_Port, TimeRD_Pin, GPIO_PIN_RESET);
		/*Truyen gia tri gui len web server*/
		//If there is not GPS master message, no time on webserver
		days 		= 10*convert_atoi(aRxBuffer[10])+convert_atoi(aRxBuffer[11]);
		months 	= 10*convert_atoi(aRxBuffer[12])+convert_atoi(aRxBuffer[13]);
		years 	= 10*convert_atoi(aRxBuffer[14])+convert_atoi(aRxBuffer[15]);
		hours 	= 10*convert_atoi(aRxBuffer[4])+convert_atoi(aRxBuffer[5])  ;//UTC
		minutes = 10*convert_atoi(aRxBuffer[6])+convert_atoi(aRxBuffer[7]);
		seconds = 10*convert_atoi(aRxBuffer[8])+convert_atoi(aRxBuffer[9]);
		
		//Day coi nhu la diem bat dau cua second, bat dau irigb tu day
		codecounter = 0;
		build_irigB_code();	
		
		/*Cap nhap thoi gian NTP*/
		currtime.tm_year = 100+ years;//100+10*convert_atoi(aRxBuffer[14])+convert_atoi(aRxBuffer[15]);//In fact: 2000+xxx-1900
		currtime.tm_mon  = months-1;//10*convert_atoi(aRxBuffer[12])+convert_atoi(aRxBuffer[13])-1;
		currtime.tm_mday = days;//10*convert_atoi(aRxBuffer[10])+convert_atoi(aRxBuffer[11]);
		
		currtime.tm_sec  = seconds;//10*convert_atoi(aRxBuffer[8])+convert_atoi(aRxBuffer[9]);
		currtime.tm_min  = minutes;//10*convert_atoi(aRxBuffer[6])+convert_atoi(aRxBuffer[7]);
		currtime.tm_hour = hours;//10*convert_atoi(aRxBuffer[4])+convert_atoi(aRxBuffer[5]);
		
		timenow = mktime(&currtime);

		timeOutLostSignal = 30;//seconds 
		lostSignal = GPS_MASTER_OK;
		
		
		#ifdef _U1_DEBUG_ENABLE_
		printf("new timestamp:%d\r\n",timenow);
		timeinfo = localtime( &timenow );
		printf("Current local time and date: %s\r\n", asctime(timeinfo));
		#endif
		//Update last sync NTP time server field!
		unixTime_last_sync = timenow + STARTOFTIME;
		unixTime_last_sync = htonl(unixTime_last_sync);
		memcpy(&serverPacket[16], &unixTime_last_sync, 4);
		
		//Update SNMP data table
		if(aRxBuffer[16]=='A') gps1_stt = 1;
		else gps1_stt = 0;
		if(aRxBuffer[17]=='A') gps2_stt = 1;
		else gps2_stt = 0;
		if(aRxBuffer[18]=='1') power1_stt = 1;
		else power1_stt = 0;
		if(aRxBuffer[19]=='1') power2_stt = 1;
		else power2_stt = 0;
		
		if((gps1_stt == 0) && (gps2_stt == 0)) serverPacket[1] = 2; // Tin hieu GPS chua on dinh
		else serverPacket[1] = 1; // Stratum, or type of clock
	}
	//#S1:01S2:02U2
	if((aRxBuffer[20] =='#')&&(aRxBuffer[21] =='S')&&(aRxBuffer[22] =='1')&&(aRxBuffer[26] =='S')&&(aRxBuffer[31] =='U'))
	{
		//HAL_GPIO_WritePin(TimeRD_GPIO_Port, TimeRD_Pin, GPIO_PIN_SET);
		//HAL_UART_Transmit(&huart1, (uint8_t *)aRxBuffer, 40, 100);
		//HAL_UART_Transmit(&huart1, (uint8_t *)"OK", 3, 100);
		//HAL_GPIO_WritePin(TimeRD_GPIO_Port, TimeRD_Pin, GPIO_PIN_RESET);
		//int8_t sat1,sat2,satInuse;
		sat1 = 10*(aRxBuffer[24]-'0')+aRxBuffer[25]-'0';
		sat2 = 10*(aRxBuffer[29]-'0')+aRxBuffer[30]-'0';
		satInuse = aRxBuffer[32]-'0';
	}
}

void uart_check(void)
{
	//Xu ly ban tin tu mach main
	if(u2Timeout == 1) 
			{
				u2Timeout = 0;
				
				
				//HAL_GPIO_WritePin(TimeRD_GPIO_Port, TimeRD_Pin, GPIO_PIN_SET);
				//HAL_UART_Transmit(&huart1, (uint8_t *)aRxBuffer, 40, 100);
				//HAL_GPIO_WritePin(TimeRD_GPIO_Port, TimeRD_Pin, GPIO_PIN_RESET);
				main_message_handle();
				//printf("aRxBuffer %s; \r\n",aRxBuffer);
				huart2.pRxBuffPtr = (uint8_t *)aRxBuffer;
				huart2.RxXferCount = RXBUFFERSIZE;
				memset(aRxBuffer,0,RXBUFFERSIZE);
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			}
	uart1_processing();
}

void machGiaoTiep(void)
{
	
	
		if(t_check_link_ms > MSEC_PHYSTATUS_CHECK)
		{
			t_check_link_ms = 0;
			checkDaymang();
		}
		//Chuyen ban tin tu mach main sang slave
		uart_check();
		
		
		
		if(timct > 999) {
			timct = 0;
			count_to_send_main++;
			count_reset_ntp_server++;
			if(count_reset_ntp_server > 369)
			{
				reInitNTP(SOCK_UDPS);
				count_reset_ntp_server = 0;
			}
			if(timeOutLostSignal> 0) timeOutLostSignal--;
			if(timeOutLostSignal == 1) lostSignal = LOST_GPS_MASTER;
		}
		
		//sau bao lau thi gui thong tin cho mach main?
		if(count_to_send_main > 5)
		{
			count_to_send_main = 0;
			printf("IP :%d.%d.%d.%d",myipWIZNETINFO.ip[0],myipWIZNETINFO.ip[1],myipWIZNETINFO.ip[2],myipWIZNETINFO.ip[3]);
			
		}
		
		if(phylink != PHY_LINK_ON) return;// ko cam day mang thi ko lam gi het!!!
		
	  //NTP server 
		NTPUDP(SOCK_UDPS);
		//NTPUDP(SOCK_NTP2);
		//NTPUDP(SOCK_NTP3);
			{	//SNMPv1 run
				//Run SNMP Agent Fucntion
				/* SNMP Agent Handler */
				//SMI Network Management Private Enterprise Codes: : moi cong ty phai dang ky 1 so rieng, 
				//tham khao : https://www.iana.org/assignments/enterprise-numbers/enterprise-numbers
				// Vi du Arduino : 36582
				// SNMP Agent daemon process : User can add the OID and OID mapped functions to snmpData[] array in snmprun.c/.h
				// [net-snmp version 5.7 package for windows] is used for this demo.
				// * Command example
				// [Command] Get:			  snmpget -v 1 -c public 192.168.22.163 .1.3.6.1.2.1.1.1.0 			// (sysDescr)
				// [Command] Get: 			snmpget -v 1 -c public 192.168.1.165 .1.3.6.1.4.1.6.1.0 			// (Custom, get LED status)
				// [Command] Get-Next: 	snmpwalk -v 1 -c public 192.168.22.163 .1.3.6.1
				// [Command] Set: 			snmpset -v 1 -c public 192.168.1.165 .1.3.6.1.4.1.6.1.1 i 1			// (Custom, LED 'On')
				// [Command] Set: 			snmpset -v 1 -c public 192.168.1.165 .1.3.6.1.4.1.6.1.1 i 0			// (Custom, LED 'Off')
				snmpd_run2();
				
			}
			// web server 	
			httpServer_run(0);
			httpServer_run(1);
			httpServer_run(2);
}




static void  wizchip_select(void)
{
	HAL_GPIO_WritePin(SCSn_GPIO_Port, SCSn_Pin, GPIO_PIN_RESET);
}
static void  wizchip_deselect(void)
{
	HAL_GPIO_WritePin(SCSn_GPIO_Port, SCSn_Pin, GPIO_PIN_SET);
}
static uint8_t wizchip_read(void)
{
	uint8_t temp;
	 
	HAL_SPI_Receive(&hspi1,&temp,1,100);
	return temp;
}
static void  wizchip_write(uint8_t wb)
{
	HAL_SPI_Transmit(&hspi1,&wb,1,100);
}
static void wizchip_readburst(uint8_t* pBuf, uint16_t len)
{
	HAL_SPI_Receive(&hspi1,pBuf,len,100);
}

static void  wizchip_writeburst(uint8_t* pBuf, uint16_t len)
{
	HAL_SPI_Transmit(&hspi1,pBuf,len,100);
}

void print_info_rs485 (void)
{
	char msg[150];
	uint8_t tmpstr[6] = {0,};
	wiz_NetInfo tempWIZNETINFO;

	ctlnetwork(CN_GET_NETINFO, (void*) &tempWIZNETINFO);
	ctlwizchip(CW_GET_ID,(void*)tmpstr);
	// Display Network Information
	if(tempWIZNETINFO.dhcp == NETINFO_DHCP) sprintf(msg,"\r\n===== %s NET CONF : DHCP =====\r\n",(char*)tmpstr);
	else sprintf(msg,"\r\n===== %s NET CONF : Static =====\r\nMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\nIP: %d.%d.%d.%d\r\nGW: %d.%d.%d.%d\r\nSN: %d.%d.%d.%d\r\nDNS: %d.%d.%d.%d\r\n",(char*)tmpstr,
			tempWIZNETINFO.mac[0],tempWIZNETINFO.mac[1], tempWIZNETINFO.mac[2], tempWIZNETINFO.mac[3], tempWIZNETINFO.mac[4], tempWIZNETINFO.mac[5], 
			tempWIZNETINFO.ip[0], tempWIZNETINFO.ip[1],tempWIZNETINFO.ip[2], tempWIZNETINFO.ip[3], 
			tempWIZNETINFO.gw[0], tempWIZNETINFO.gw[1], tempWIZNETINFO.gw[2], tempWIZNETINFO.gw[3], 
			tempWIZNETINFO.sn[0], tempWIZNETINFO.sn[1], tempWIZNETINFO.sn[2], tempWIZNETINFO.sn[3],
			tempWIZNETINFO.dns[0], tempWIZNETINFO.dns[1], tempWIZNETINFO.dns[2], tempWIZNETINFO.dns[3]);
	
	HAL_GPIO_WritePin(TimeRD_GPIO_Port, TimeRD_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart1, (uint8_t *)msg, 150, 200);
	HAL_GPIO_WritePin(TimeRD_GPIO_Port, TimeRD_Pin, GPIO_PIN_RESET);
	//printf("\r\nMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", tempWIZNETINFO.mac[0], tempWIZNETINFO.mac[1], tempWIZNETINFO.mac[2], tempWIZNETINFO.mac[3], tempWIZNETINFO.mac[4], tempWIZNETINFO.mac[5]);
	//printf("IP: %d.%d.%d.%d\r\n", tempWIZNETINFO.ip[0], tempWIZNETINFO.ip[1], tempWIZNETINFO.ip[2], tempWIZNETINFO.ip[3]);
	//printf("GW: %d.%d.%d.%d\r\n", tempWIZNETINFO.gw[0], tempWIZNETINFO.gw[1], tempWIZNETINFO.gw[2], tempWIZNETINFO.gw[3]);
	//printf("SN: %d.%d.%d.%d\r\n", tempWIZNETINFO.sn[0], tempWIZNETINFO.sn[1], tempWIZNETINFO.sn[2], tempWIZNETINFO.sn[3]);
	//printf("DNS: %d.%d.%d.%d\r\n", tempWIZNETINFO.dns[0], tempWIZNETINFO.dns[1], tempWIZNETINFO.dns[2], tempWIZNETINFO.dns[3]);
	
	//sprintf(msg,"IP %d.%d.%d.%d\r\n",myipWIZNETINFO.ip[0],myipWIZNETINFO.ip[1],myipWIZNETINFO.ip[2],myipWIZNETINFO.ip[3]);
	
}
void Display_Net_Conf()
{
	uint8_t tmpstr[6] = {0,};
	wiz_NetInfo tempWIZNETINFO;

	ctlnetwork(CN_GET_NETINFO, (void*) &tempWIZNETINFO);
	ctlwizchip(CW_GET_ID,(void*)tmpstr);

	// Display Network Information
	if(tempWIZNETINFO.dhcp == NETINFO_DHCP) printf("\r\n===== %s NET CONF : DHCP =====\r\n",(char*)tmpstr);
		else printf("\r\n===== %s NET CONF : Static =====\r\n",(char*)tmpstr);

	printf("\r\nMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", tempWIZNETINFO.mac[0], tempWIZNETINFO.mac[1], tempWIZNETINFO.mac[2], tempWIZNETINFO.mac[3], tempWIZNETINFO.mac[4], tempWIZNETINFO.mac[5]);
	printf("IP: %d.%d.%d.%d\r\n", tempWIZNETINFO.ip[0], tempWIZNETINFO.ip[1], tempWIZNETINFO.ip[2], tempWIZNETINFO.ip[3]);
	printf("GW: %d.%d.%d.%d\r\n", tempWIZNETINFO.gw[0], tempWIZNETINFO.gw[1], tempWIZNETINFO.gw[2], tempWIZNETINFO.gw[3]);
	printf("SN: %d.%d.%d.%d\r\n", tempWIZNETINFO.sn[0], tempWIZNETINFO.sn[1], tempWIZNETINFO.sn[2], tempWIZNETINFO.sn[3]);
	printf("DNS: %d.%d.%d.%d\r\n", tempWIZNETINFO.dns[0], tempWIZNETINFO.dns[1], tempWIZNETINFO.dns[2], tempWIZNETINFO.dns[3]);
	
}

void Net_Conf(wiz_NetInfo temp_netinfo)
{
	ctlnetwork(CN_SET_NETINFO, (void*) &temp_netinfo);
	//Display_Net_Conf();
	//In du lieu ve mang ra dang sau dong ho!
	print_info_rs485();
}
void w5500_lib_init(void){
		
		uint8_t tmp;
		intr_kind temp;
		uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};	
		
		//printf("w5500_lib_init...1");
		//GPIO_ResetBits(W5500_RST_GPIO, W5500_RST);
		HAL_GPIO_WritePin(RSTn_GPIO_Port, RSTn_Pin, GPIO_PIN_RESET);
		HAL_Delay(100);
		
		//////////
   // TODO //
   ////////////////////////////////////////////////////////////////////////////////////////////////////
   // First of all, Should register SPI callback functions implemented by user for accessing WIZCHIP //
   ////////////////////////////////////////////////////////////////////////////////////////////////////
   
		/* Critical section callback - No use in this example */
		//reg_wizchip_cris_cbfunc(0, 0);
			
    /* Chip selection call back */
		reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
		/* SPI Read & Write callback function */
    reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);
		reg_wizchip_spiburst_cbfunc(wizchip_readburst, wizchip_writeburst);
		//GPIO_SetBits(W5500_RST_GPIO, W5500_RST);//RST High to run
		HAL_GPIO_WritePin(RSTn_GPIO_Port, RSTn_Pin, GPIO_PIN_SET);
		HAL_Delay(100);
		
    ////////////////////////////////////////////////////////////////////////
		/* WIZCHIP SOCKET Buffer initialize */
		//Initializes to WIZCHIP with SOCKET buffer size 2 or 1 dimension array typed uint8_t
    if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
    {
       //printf("WIZCHIP Initialized fail.\r\n");
    }
		
		//printf("...get PHY Link status");
		/* PHY link status check */
    
//		do
//    {
//       if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
//          ;//printf("Unknown PHY Link stauts.\r\n");
//    }while(tmp == PHY_LINK_OFF);

		
		//Cau hinh ngat tren Socket 0, vi can biet thoi diem ban tin NTP den!!
		temp = IK_SOCK_0;
		if(ctlwizchip(CW_SET_INTRMASK, &temp) == -1)
		{
			//printf("Cannot set imr...\r\n");
		}
		//printf("...all ok\r\n");
		Net_Conf(myipWIZNETINFO);
	
}




