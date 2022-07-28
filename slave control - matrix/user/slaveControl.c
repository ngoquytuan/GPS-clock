#include "main.h"
#include <time.h>
#include <string.h>
#include "stdio.h"
#include "wizchip_conf.h"
#include "snmp.h"
#include "httpServer.h"

extern volatile uint8_t waitForSetTime;	
extern volatile uint32_t tim4ct;
extern uint8_t ntpTimeServer_ip[4];
//NTP client
extern uint16_t RetrySend ; //60 giay
extern uint16_t sycnPeriod ;// 1 gio
extern time_t timenow;
extern wiz_NetInfo myipWIZNETINFO;
extern volatile uint16_t phystatus_check_cnt;
extern SPI_HandleTypeDef hspi1;
extern I2C_HandleTypeDef hi2c3;
extern struct tm currtime;

slave_status slave_clock;
char ds3231_reg[7];
#define fractionOfSecond TIM1->CNT
uint8_t u2Timeout = 0;

uint8_t phylink = PHY_LINK_OFF;
#define MSEC_PHYSTATUS_CHECK 		1000		// msec
uint16_t t_check_link_ms = 0;

static uint8_t unlock_config =0;
#define MSEC_PHYSTATUS_CHECK 		1000		// msec
#define PHYStatus_check_enable 1


uint8_t rtc_timeout = 30;
int8_t haveSignalFromRS485 = NO_SIGNAL;
int8_t stableSignal = SIGNAL_FROM_MASTER_BAD;
int8_t count_Stable_signal = 0;
int8_t timeOutLostSignal = 0;
//Thoi gian luu gio tu RS485 vao RTC
int16_t timeSaveRS485_to_RTC = 60;
uint32_t countOfMasterMessages = 0;	
uint32_t countOfNTPrequest = 0;	

/* Khai bao cac chuong trinh con!!
*/
static void  wizchip_select(void);
static void  wizchip_deselect(void);
static uint8_t wizchip_read(void);
static void  wizchip_write(uint8_t wb);
static void wizchip_readburst(uint8_t* pBuf, uint16_t len);
static void  wizchip_writeburst(uint8_t* pBuf, uint16_t len);




/*
W5500	
*/

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
	printf("NTP time server IP: %d.%d.%d.%d\r\n", ntpTimeServer_ip[0], ntpTimeServer_ip[1], ntpTimeServer_ip[2], ntpTimeServer_ip[3]);
	
}

void Net_Conf(wiz_NetInfo temp_netinfo)
{

	ctlnetwork(CN_SET_NETINFO, (void*) &temp_netinfo);
	#ifdef DebugEnable
	Display_Net_Conf();
	#endif
}
/*
Khoi dong card mang!
*/
void w5500_lib_init(void){
		
		uint8_t tmp;
		intr_kind temp;
		uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};	
		

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

		
		//Cau hinh ngat tren Socket 0, vi can biet thoi diem ban tin NTP den!!
		temp = IK_SOCK_0;
		if(ctlwizchip(CW_SET_INTRMASK, &temp) == -1)
		{
			//printf("Cannot set imr...\r\n");
		}
		Net_Conf(myipWIZNETINFO);
	
}
/*
	Kiem tra W5500, SPI co on ko, day mang co cam ko?
*/
uint8_t checkDaymang(void)
{
	uint8_t tmp;
  //phylink = PHY_LINK_OFF;
	//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
	if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
	{
		#ifdef DebugEnable
		//printf("Unknown PHY Link stauts.\r\n");//Loi spi???
		#endif
		phylink = PHY_LINK_OFF;
		return 0;
	}
	
	if(tmp == PHY_LINK_OFF) 
		{
			phylink = PHY_LINK_OFF;
			return 0;//Ko cam day mang
		}
	
//	#ifdef DebugEnable
	//printf("Co cam day mang nhe\r\n");
//	#endif
	phylink = PHY_LINK_ON;
	//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
  return 1;  
}
/*
Control
*/
void control(void)
{
	uint8_t tmp[4];
	if(unlock_config == 1)
	{//set all config here!!
		if((aRxBuffer[0] =='I')&&(aRxBuffer[1] =='P')&&(aRxBuffer[7] =='.')&&(aRxBuffer[11] =='.')&&(aRxBuffer[15] =='.'))
		{
			//IP: 192.168.222.252\r\n;IP: 092.068.022.052\r\n
			if((aRxBuffer[4]>'2') || (aRxBuffer[4]<'0')) return;
			if((aRxBuffer[8]>'2') || (aRxBuffer[8]<'0')) return;	
			if((aRxBuffer[12]>'2') || (aRxBuffer[12]<'0')) return;
			if((aRxBuffer[16]>'2') || (aRxBuffer[16]<'0')) return;
			
			if((aRxBuffer[5]>'9') || (aRxBuffer[5]<'0')) return;
			if((aRxBuffer[9]>'9') || (aRxBuffer[9]<'0')) return;	
			if((aRxBuffer[13]>'9') || (aRxBuffer[13]<'0')) return;
			if((aRxBuffer[17]>'9') || (aRxBuffer[17]<'0')) return;
			
			if((aRxBuffer[6]>'9') || (aRxBuffer[6]<'0')) return;
			if((aRxBuffer[10]>'9') || (aRxBuffer[10]<'0')) return;	
			if((aRxBuffer[14]>'9') || (aRxBuffer[14]<'0')) return;
			if((aRxBuffer[18]>'9') || (aRxBuffer[18]<'0')) return;
			
			myipWIZNETINFO.ip[0] = 100*(aRxBuffer[4]-'0')  + 10*(aRxBuffer[5]-'0')  + (aRxBuffer[6]-'0');
			myipWIZNETINFO.ip[1] = 100*(aRxBuffer[8]-'0')  + 10*(aRxBuffer[9]-'0')  + (aRxBuffer[10]-'0');
			myipWIZNETINFO.ip[2] = 100*(aRxBuffer[12]-'0') + 10*(aRxBuffer[13]-'0') + (aRxBuffer[14]-'0');
			myipWIZNETINFO.ip[3] = 100*(aRxBuffer[16]-'0') + 10*(aRxBuffer[17]-'0') + (aRxBuffer[18]-'0');
			HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
			//storeValue(3);

			HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
			#ifdef DebugEnable
			printf("New IP: %d.%d.%d.%d\r\n",myipWIZNETINFO.ip[0],myipWIZNETINFO.ip[1],myipWIZNETINFO.ip[2],myipWIZNETINFO.ip[3]);
			#endif
			HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
			
			//NVIC_SystemReset();
		}
		if((aRxBuffer[0] =='R')&&(aRxBuffer[1] =='E')&&(aRxBuffer[2] =='S')&&(aRxBuffer[3] =='E')&&(aRxBuffer[4] =='T'))
		{
			//Restart MCU
			NVIC_SystemReset();
		}
	}
	else
	{
		if((aRxBuffer[0] =='A')&&(aRxBuffer[1] =='T'))
		{
			//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
			//printf("AT on\r\n");
			//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
			unlock_config = 1;
		}
		else 
		{
			HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
			printf("Linh tinh\r\n");
			HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
		}
	}
	
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
		
		//Ban tin dung, cap nhat du lieu
		//If there is not GPS master message, no time on webserver
		days 		= 10*convert_atoi(aRxBuffer[10])+convert_atoi(aRxBuffer[11]);
		months 	= 10*convert_atoi(aRxBuffer[12])+convert_atoi(aRxBuffer[13]);
		years 	= 10*convert_atoi(aRxBuffer[14])+convert_atoi(aRxBuffer[15]);
		hours 	= 10*convert_atoi(aRxBuffer[4])+convert_atoi(aRxBuffer[5])  ;//UTC
		minutes = 10*convert_atoi(aRxBuffer[6])+convert_atoi(aRxBuffer[7]);
		seconds = 10*convert_atoi(aRxBuffer[8])+convert_atoi(aRxBuffer[9]);
		/*
		load_line2(hours,minutes,seconds,1);
		scan_5down();
		*/
		
		/*Cap nhap thoi gian NTP*/
		currtime.tm_year = 100+ years;//100+10*convert_atoi(aRxBuffer[14])+convert_atoi(aRxBuffer[15]);//In fact: 2000+xxx-1900
		currtime.tm_mon  = months-1;//10*convert_atoi(aRxBuffer[12])+convert_atoi(aRxBuffer[13])-1;
		currtime.tm_mday = days;//10*convert_atoi(aRxBuffer[10])+convert_atoi(aRxBuffer[11]);
		
		currtime.tm_sec  = seconds;//10*convert_atoi(aRxBuffer[8])+convert_atoi(aRxBuffer[9]);
		currtime.tm_min  = minutes;//10*convert_atoi(aRxBuffer[6])+convert_atoi(aRxBuffer[7]);
		currtime.tm_hour = hours;//10*convert_atoi(aRxBuffer[4])+convert_atoi(aRxBuffer[5]);
		
		timenow = mktime(&currtime);
		//timenow = timenow - 25200;//Tru di 7 tieng
		
		
		haveSignalFromRS485 = HAVE_SIGNAL;
		timeOutLostSignal = 10;
//	  if(count_Stable_signal < STABE_NUMBER) count_Stable_signal++; 
//		if(count_Stable_signal >= STABE_NUMBER) 
//			{
//				stableSignal = SIGNAL_FROM_MASTER_OK;
//			}
			
		if((aRxBuffer[16]=='A') || (aRxBuffer[17]=='A') )
						{ 
							slave_clock.sync_status = GREEN;
						}
		if((aRxBuffer[16]=='V') && (aRxBuffer[17]=='V') )				
		  slave_clock.sync_status = BOTH;
							
		if(timeSaveRS485_to_RTC == 1)
		{
			//seconds++;
			timeSaveRS485_to_RTC = 60*5;
			//sync rs485 time to RTC 
			ghids(14,0);//1HZ out SQW
			ghids(DS_SECOND_REG,seconds);
			ghids(DS_MIN_REG,minutes);
			ghids(DS_HOUR_REG,hours);
			//ghids(DS_DAY_REG,6);
			ghids(DS_DATE_REG,days);
			ghids(DS_MONTH_REG,months);
			ghids(DS_YEAR_REG,years);
		}
		/*
		load_line1(days,months,years);
		scan_7up();
		*/
		#ifdef _U1_DEBUG_ENABLE_
		printf("new timestamp:%d, %d\r\n",timenow, timeOutLostSignal);
		timeinfo = localtime( &timenow );
		printf("Current local time and date: %s\r\n", asctime(timeinfo));
		#endif
		//Update last sync NTP time server field!
//		unixTime_last_sync = timenow + STARTOFTIME;
//		unixTime_last_sync = htonl(unixTime_last_sync);
//		memcpy(&serverPacket[16], &unixTime_last_sync, 4);
		
		//Update SNMP data table
//		if(aRxBuffer[16]=='A') gps1_stt = 1;
//		else gps1_stt = 0;
//		if(aRxBuffer[17]=='A') gps2_stt = 1;
//		else gps2_stt = 0;
//		if(aRxBuffer[18]=='1') power1_stt = 1;
//		else power1_stt = 0;
//		if(aRxBuffer[19]=='1') power2_stt = 1;
//		else power2_stt = 0;
	}
	else control();
}
void uart2_processing(void)
{
	if(u2Timeout == 1) 
			{
				u2Timeout = 0;
				//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
				//HAL_UART_Transmit(&huart2, aRxBuffer, 20, 100);
				main_message_handle();
				//saved = 1;
				//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
				//printf("aRxBuffer %s; \r\n",aRxBuffer);
				huart2.pRxBuffPtr = (uint8_t *)aRxBuffer;
				huart2.RxXferCount = RXBUFFERSIZE;
				memset(aRxBuffer,0,RXBUFFERSIZE);
				//LED off
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			}
}
/*
Tra ve trang thai xuat xuong
Factory reset
Hold FR button and turn Clock on => factory reset!!!
*/
void slaveClockFactoryLoad(void)
{
	uint32_t count=0;
	//Kiem tra xem nut reset factory seting co dc bam hay ko???
	if(HAL_GPIO_ReadPin(FR_GPIO_Port,FR_Pin) == GPIO_PIN_RESET)
	{
		while(HAL_GPIO_ReadPin(FR_GPIO_Port,FR_Pin) == GPIO_PIN_RESET)
		{
			count++;
			HAL_Delay(1);
			if(count > 2000)
			{
				HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
				HAL_UART_Transmit(&huart2, (uint8_t *)"Factory reset!!!\r\n", 18, 100);
				HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
				stm32g474flashEraseThenSave();
				HAL_Delay(1000);
				//Restart MCU
				NVIC_SystemReset();
			}
		}
	}
}

/*
Pre load programs
*/
void slaveClockFucnsInit(void)
{
	
	//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
	#ifdef DebugEnable
	printf("This code gen by STMcube STM32G474@128MHz\r\n");
	#endif
	slaveClockFactoryLoad();
	stm32g474_FactoryLoad();
	//stm32g474flashEraseThenSave();
	//LEDintensity = 1;
#ifdef SLAVE_MATRIX
	up7_matrix_init();
	line2_matrix_init();
	load_line1(88,88,88);
	scan_7up();
	load_line2(88,88,88,1);
	scan_5down();
#endif
#ifndef SLAVE_MATRIX	
	display_init_check();
#endif	
	HAL_Delay(500);
	
	w5500_lib_init();
	snmp_init();
	SNTP_init();
	loadwebpages();
	
	laythoigian();
	
	printf("Time :%dh%dm%ds;%d %d/%d/%d; LED intensity : %d\r\n",hours,minutes,seconds,ds3231_reg[3],days,months,years, LEDintensity);
	
	
#ifdef SLAVE_MATRIX
	load_line1(days,months,years);
	scan_7up();
	load_line2(hours,minutes,seconds,1);
	scan_5down();
#endif
#ifndef SLAVE_MATRIX
#ifdef SLAVE_WALL			
			day_display();
#endif
			console_display();
			console_blink();
#endif
//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
	/** Cac che do hoat dong cua dong ho slave
	*  Hoat dong theo RTC hoac NTP client hoac RS485
	*  Uu tien 1: NTP client, neu khong co day mang, khong ket noi dc server thi cho tin hieu rs485, neu cung ko co tin hieu rs485 tiep tuc chay local
	*  Chay local voi RTC cho toi khi co rs458 hoac ntp client thi chuyen du lieu, neu du lieu on dinh thi ghi du lieu sang rtc
	*  Neu RTC ko co hoac loi??
	*  IC RTC hong => NO RTC
  *  IC RTC het pin => NO BATTERY 	
	*/
}
/*
	Main programs
*/
void slaveClockRun(void)
{
	if(t_check_link_ms > MSEC_PHYSTATUS_CHECK)
		{
			t_check_link_ms = 0;
			checkDaymang();
		}
		
	if(timct > 990) {
			timct = 0;

			if(slave_clock.sync_status == GREEN)
			{
				if(count_Stable_signal > 0) count_Stable_signal--;
				if(count_Stable_signal == 0) slave_clock.sync_status = RED;
			}
			//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
			
			//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);

			
			if(timeOutLostSignal) timeOutLostSignal--;
			else haveSignalFromRS485 = NO_SIGNAL;
			
			if(haveSignalFromRS485 == NO_SIGNAL)
			{
			
			}
			//seconds++;
			RetrySend++;
			sycnPeriod++;
			
			
			
			//kiem tra tinh on dinh cua du lieu GPS
				 
			//slave_clock.sync_status = RED;
			
//			if(stableSignal == SIGNAL_FROM_MASTER_OK) count_Stable_signal--; 
//      if(count_Stable_signal == 0) 
//				{
//					stableSignal = SIGNAL_FROM_MASTER_BAD;
//					haveSignalFromRS485 = NO_SIGNAL;
//					//slave_clock.sync_status = RED;
//				}
			
			//if((timeSaveRS485_to_RTC > 1) && (stableSignal == SIGNAL_FROM_MASTER_OK)) timeSaveRS485_to_RTC --;
				//if((timeSaveRS485_to_RTC > 1)) timeSaveRS485_to_RTC --;
			
//			if(rtc_timeout > 1) rtc_timeout --;
//			if(rtc_timeout == 1) 
//			{
//				slave_clock.rtc_status = RTC_OUT_OF_BATTERY;
//			}
//			if(rtc_timeout == 0) 
//			{
//				slave_clock.rtc_status = RTC_FINE;
//			}
			
		}
	
	if((waitForSetTime == 1) && (TIM1->CNT > 9936)) 
		{
			waitForSetTime = 0;
			countOfNTPrequest++;	
			seconds++;
			RTC_Update();
			slave_clock.sync_status = GREEN;
			count_Stable_signal = 16;
			HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
			printf("Time set IT\r\n");
			HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);

#ifdef SLAVE_MATRIX
			load_line1(days,months,years);
			scan_7up();
			load_line2(hours,minutes,seconds,1);
			scan_5down();
#endif
#ifndef SLAVE_MATRIX
#ifdef SLAVE_WALL			
			day_display();
#endif
			console_display();
			console_blink();
#endif
			}
		
			uart2_processing();
			
			if(phylink != PHY_LINK_ON) return;// ko cam day mang thi ko lam gi het!!!
			
			HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
			SNTP_run();
			HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
	
			//SNMP
			snmpd_run();
			// web server 	
			httpServer_run(0);
			httpServer_run(1);
			httpServer_run(2);
}

//khi nao buffer full thi no se goi ham nay
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
			if(UartHandle == &huart2) 
			{
				HAL_UART_Abort(&huart2);
				if (HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
				{
					/* Transfer error in reception process */
					Error_Handler();
				}
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
	
	if (GPIO_Pin == INTn_Pin)
  {
		//Nhan duoc ban tin NTP, xu ly thoi gian va phan hoi
		//Receied NTP message, processing and respond
  }
	if (GPIO_Pin == FR_Pin)
  {
		//factory reset
//		HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
//		printf("Factory reset\r\n");
//		HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
		chinhdosang();
		//Luu bo sang moi vao bo nho!
		stm32g474flashEraseThenSave();
		
  }
	if (GPIO_Pin == SQW_Pin)
  {
		//Dong bo lai phan le cua giay
		if(waitForSetTime == 0) fractionOfSecond = 0;
		//RTC second
		//RTC hoat dong binh thuong
		//rtc_timeout = 0;
  	timenow++;
		update_display();
		
  }

}

void update_display(void)
{
	seconds++;
	if(seconds > 59) 
		{
			//Moi phut se dong bo thoi gian voi RTC mot lan
			laythoigian();
		}	
#ifdef SLAVE_MATRIX
			load_line1(days,months,years);
			scan_7up();
			load_line2(hours,minutes,seconds,1);
			scan_5down();
#endif
#ifndef SLAVE_MATRIX
#ifdef SLAVE_WALL			
			day_display();
#endif
			console_display();
			console_blink();
#endif
}
/*
Cac chuong trinh con!!!!
*/
//----------------------Phan giao tiep i2c-thoi gian thuc ---------------------//
//Phai pull up SDA SCL


//void BCD_Decoder()
//{
//	//printf("i2c_rv[3]: %d",i2c_rv[3]);
//	for(char x=0;x<7;x++) ds3231_reg[x]=(i2c_rv[x] & 0x0f) + (i2c_rv[x]>>4)*10;
//	//ds3231_reg[3] --; 
//}
unsigned char BCD_Encoder(unsigned char temp)
{
	return ((temp/10)<<4)|(temp%10);
}
void laythoigian(void)
{
	unsigned char i2c_rv[19];
	if( HAL_I2C_Mem_Read(&hi2c3,0x68<<1,0,I2C_MEMADD_SIZE_8BIT,i2c_rv,19,1000) == HAL_ERROR) 
		{
			slave_clock.rtc_status = NO_RTC;
			//printf("NO RTC");
			return;
		}
	
	//BCD_Decoder(); //chuyen doi
	for(char x=0;x<7;x++) ds3231_reg[x]=(i2c_rv[x] & 0x0f) + (i2c_rv[x]>>4)*10;
	
	hours   = ds3231_reg[2];
	minutes = ds3231_reg[1];
	seconds = ds3231_reg[0];
	days		= ds3231_reg[4];
	months  = ds3231_reg[5]; 
	years   = ds3231_reg[6];
}
void ghids(unsigned char add, unsigned char dat)
{
	unsigned char i2c_write[1];
	i2c_write[0] = BCD_Encoder(dat);
	HAL_I2C_Mem_Write(&hi2c3,0x68<<1,add,I2C_MEMADD_SIZE_8BIT,i2c_write,1,1000); 
}
//----------------------Phan giao tiep i2c-thoi gian thuc ---------------------//


//Trong truong hop RTC ko chay, goi ham nay de cau hinh lai RTC
void RTC_factory_RST(void)
	{
		ghids(14,0);//1HZ out SQW
		ghids(DS_SECOND_REG,0);
		ghids(DS_MIN_REG,26);
		ghids(DS_HOUR_REG,15);
		ghids(DS_DAY_REG,2);
		ghids(DS_DATE_REG,23);
		ghids(DS_MONTH_REG,5);
		ghids(DS_YEAR_REG,22);
	}
	
void RTC_Update(void)
	{
		ghids(14,0);//1HZ out SQW
		ghids(DS_SECOND_REG,seconds);
		ghids(DS_MIN_REG,minutes);
		ghids(DS_HOUR_REG,hours);
		//ghids(DS_DAY_REG,2);
		ghids(DS_DATE_REG,days);
		ghids(DS_MONTH_REG,months);
		ghids(DS_YEAR_REG,years);
		//printf("Time set!!!!\r\n");
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

