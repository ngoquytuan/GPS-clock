#include "main.h"
#include <time.h>
#include <string.h>
#include "stdio.h"
#include "wizchip_conf.h"
#include "snmp.h"
#include "httpServer.h"
#include "mydefines.h"
#include "socketdefines.h"

extern uint8_t phylink;

extern uint8_t tim4ITflag;
//extern uint32_t ms_couters2;
extern time_t built_time;
extern time_t timenow;
extern uint8_t useDNS;
extern uint8_t timeZone;

extern uint8_t NTP_alreadySent;
extern uint8_t NTP_alreadySentTimeOut;
extern uint8_t flag_to_switchNtpServer;
extern volatile uint8_t waitForSetTime;	
extern volatile uint32_t tim4ct;
extern uint8_t ntpTimeServer_ip[4];
//NTP client
extern uint16_t RetrySend ; //60 giay
extern uint16_t sycnPeriod ;// 1 gio

uint8_t  uartHalt = 0;
uint8_t  synchronized_ntp = 0;
uint16_t synchronizePeriod = 6;// seconds
uint16_t RetrySynchronize ; //60 giay
//uint8_t setTimeRS485now = 0;
extern int8_t lostSignal;

extern volatile uint16_t phystatus_check_cnt;


extern struct tm currtime;

slave_status slave_clock;
char ds3231_reg[7];
uint8_t rtc_pps = 0;
uint8_t u2Timeout = 0;


uint8_t LANServicesInitFlag = 0;
uint16_t t_check_link_ms = 0;

//static uint8_t unlock_config =0;

//uint8_t checkLANcable(void);
void sycn_RTC_Now(time_t x);
uint8_t rtc_timeout = 30;
int8_t haveSignalFromRS485 = NO_SIGNAL;
int8_t stableSignal = SIGNAL_FROM_MASTER_BAD;
int8_t count_Stable_signal = 0;
int8_t timeOutLostSignal = 0;
//Thoi gian luu gio tu RS485 vao RTC
int16_t timeSaveRS485_to_RTC = 60;
uint32_t countOfMasterMessages = 0;	
uint32_t countOfNTPrequest = 0;	
uint32_t countSetTime = 0;
extern uint8_t gps1_stt;
extern uint8_t gps2_stt;
extern uint8_t power1_stt;
extern uint8_t power2_stt;
int16_t time2RefreshNTP = ClockReFill;//seconds
uint32_t delta_RS485=9999;

extern uint8_t *ntp_ip;
extern const char* ntpServers[];
extern uint8_t ntpServersFound;

/* Khai bao cac chuong trinh con!!
*/
#ifdef SLAVE_MATRIX
void up7_matrix_init (void);
void load_line1(uint8_t dis_date,uint8_t dis_month,uint8_t dis_year);
void scan_7up(void);
void scan_5down(void);
void load_line2(uint8_t dis_hour,uint8_t dis_min,uint8_t dis_sec,uint8_t dot);
void line2_matrix_init (void);
#endif

#ifndef SLAVE_MATRIX
void display_init_check(void);
void console_blink(void);
void console_display(void);
void RTC_factory_RST(void);
void chinhdosang(void);
#ifdef SLAVE_WALL
void MAX7219_Init2 (void);
void MAX7219_SendAddrDat2 (unsigned char addr,unsigned char dat);
void day_display(void);
#endif
#endif


#ifndef SLAVE_MATRIX	
		void MAX7219_SendAddrDat (unsigned char addr,unsigned char dat);
#endif


uint8_t time2SaveRTC = PeriodSaveRTC;
uint8_t syncRS485_timeout = SYNC_OUTofTIME;
uint8_t just_set_time_flag = 0;





//Ham chuyen doi char sang int
uint8_t convert_atoi( uint8_t c)
{
	return (uint8_t)c-48;
}


/*
- LED RED: no sync
- LED GREEN + RED : sync + no GPS
- LED GREEN: sync + GPS

SYNC NOW: dong bo thoi gian theo gio luon, dinh ky dong bo lai

*/
void rs485SyncNow(void)
{
	//uint8_t temp_sec;server_second
	if((aRxBuffer[0] =='$')&((aRxBuffer[1] =='G')|(aRxBuffer[1] =='g'))&((aRxBuffer[2] =='P')|(aRxBuffer[2] =='p'))&((aRxBuffer[3] =='S')|(aRxBuffer[3] =='s')))
	{
	  delta_RS485 = fractionOfSecond;
		server_second = 10*convert_atoi(aRxBuffer[8])+convert_atoi(aRxBuffer[9]);
		
		//Thoi gian chinh xac tuyet doi => Luu thoi gian luon, dinh ky luu lai.
		if(time2SaveRTC == Time_to_SaveRTC)
		{
			time2SaveRTC = PeriodSaveRTC;
			
			if(server_second <56) //gan thoi diem chuyen giao qua, ko luu, bo qua
			{
				//setTimeRS485now = 1;
				ghids(DS_SECOND_REG,server_second);
			
				server_day 		= 10*convert_atoi(aRxBuffer[10])+convert_atoi(aRxBuffer[11]);
				ghids(DS_DATE_REG,server_day);
				
				server_month 	= 10*convert_atoi(aRxBuffer[12])+convert_atoi(aRxBuffer[13]);
				ghids(DS_MONTH_REG,server_month);
				
				server_year 	= 10*convert_atoi(aRxBuffer[14])+convert_atoi(aRxBuffer[15]);
				ghids(DS_YEAR_REG,server_year);
				
				server_hour 	= 10*convert_atoi(aRxBuffer[4])+convert_atoi(aRxBuffer[5])  ;
				ghids(DS_HOUR_REG,server_hour);
				
				server_minute = 10*convert_atoi(aRxBuffer[6])+convert_atoi(aRxBuffer[7]);
				ghids(DS_MIN_REG,server_minute);
				
				countSetTime++;
				just_set_time_flag = 1;
				
				#ifdef OverTheAir
				send_debug_message = 1;
				mysize = sprintf(udp_message,"RS485 SET %d:%d:%d,save times %d\r\n",server_hour,server_minute,server_second,countSetTime);
				#endif
				
			}
			
				
		}
		
		
		
		if((aRxBuffer[16]=='A') || (aRxBuffer[17]=='A') )
						{ 
							slave_clock.sync_status = GREEN;
						}
		else 			
							slave_clock.sync_status = BOTH;
		
		syncRS485_timeout 						= SYNC_TIMEOUT;
		slave_clock.rs458_connection	= CONNECTED;
		lostSignal 										= RS485_GPS_MASTER_OK;
		
		if(aRxBuffer[16]=='A') gps1_stt = 1;
		else  gps1_stt = 0;
		if(aRxBuffer[17]=='A') gps2_stt = 1;
		else  gps2_stt = 0;
		if(aRxBuffer[18]=='1') power1_stt = 1;
		else  power1_stt = 0;
		if(aRxBuffer[19]=='1') power2_stt = 1;
		else  power2_stt = 0;
		
		countOfMasterMessages++;
	}
}
/*
Xu ly ban tin UART
*/
void uart2_processing(void)
{
	if(u2Timeout == 1) 
			{
				u2Timeout = 0;
				//UART ko treo
				uartHalt = 0;
				rs485SyncNow();
				
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

void clock_info(void)
 {
 #ifdef DebugEnable
		#ifdef SLAVE_CONSOLE
			printf("SLAVE_CONSOLE runs on STM32G474@128MHz\r\n");
		#endif
		#ifdef SLAVE_WALL
			printf("SLAVE_WALL runs on STM32G474@128MHz\r\n");
		#endif
		#ifdef SLAVE_MATRIX
			printf("SLAVE_MATRIX runs on STM32G474@128MHz\r\n");
		#endif
			struct tm *tm_info;
			char buffer[26];
			tm_info = localtime(&built_time);
			strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
			printf("Built time: %s\n", buffer);
	#endif
 }
void displayInit(void)
{
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

void initLanServices(void)
{
	if(useDNS == 1)
	{
			dns_check();
			ntpServersFound = 0;
	}
	else if(useDNS == 0)
	{
		ntpServers[0] ="GPS master";
	}

	
	 
	snmp_init();
	SNTP_init();
	loadwebpages();
	LANServicesInitFlag = 1;
	#ifdef DebugEnable
			printf("LAN Services Init Done!\r\n");
	#endif 
}
/*
Pre load programs
*/
void slaveClockFucnsInit(void)
{

	HAL_Delay(999);
	RTCInit();
	clock_info();
	displayInit();
	//Hold FR button and turn Clock on => factory reset!!!
	slaveClockFactoryLoad();
	//LOAD memory settings
	stm32g474_FactoryLoad();
	ntp_ip = ntpTimeServer_ip;
	HAL_Delay(100);
	
	//Cau hinh ngat tren Socket 2, vi can biet thoi diem ban tin NTP den!!
	w5500_lib_init();
	if(phylink == PHY_LINK_ON) initLanServices();
	
	
	
	
		
	HAL_UART_Abort(&huart2);
	if (HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }
	
	slave_clock.sync_status = RED;
	slave_clock.ntp_client_status   = NO_CONNECTION;
	slave_clock.ethernet_connection = NO_CONNECTION;
	slave_clock.rs458_connection    = NO_CONNECTION;
	
	HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
}


/*
	One second fuctions
  Moi giay ham nay goi 1 lan
*/
void oneSecondfucns(void)
{
	if((time2RefreshNTP>0) && (useDNS == 1))
	{
		time2RefreshNTP--;
		if(time2RefreshNTP == 0) 
		{
			time2RefreshNTP = ClockReFill;
			
				ntpServersFound = 0;
				dns_check();
			
		}
	}
	
	
	if(time2SaveRTC > Time_to_SaveRTC) time2SaveRTC--;//Khi bien nay == 0 thi se tien hanh luu gio
	
	if(syncRS485_timeout > 1) syncRS485_timeout--;
	if(syncRS485_timeout == 1) 
			{
				slave_clock.sync_status = RED;//Den do bao mat dong bo master - slave
				syncRS485_timeout = SYNC_OUTofTIME;
				slave_clock.rs458_connection = NO_CONNECTION;
				slave_clock.work_mode = SYNC_WiTH_NTP_Server;//Cho phep dong bo thoi gian voi NTP server khi ko co tin hieu RS485
				lostSignal = LOST_RS485_GPS_MASTER;
			}

	
	if(NTP_alreadySent == 1) NTP_alreadySentTimeOut--;
	
	if(NTP_alreadySentTimeOut < 1) {
		if((phylink == PHY_LINK_ON)&&(ntpServersFound>0)) // LAN ok && at least 1 NTP server
		{
			//// Switch to the next NTP server on timeout
			//printf("Switch to the next NTP server on timeout?\r\n");
			flag_to_switchNtpServer++;
			if(flag_to_switchNtpServer > 2) 
			{
				flag_to_switchNtpServer = 0;	
				switch_to_next_ntp_server();
				
			}
		}
		
		NTP_alreadySent = 0;
				
				if(slave_clock.rs458_connection == NO_CONNECTION)
				{
					slave_clock.sync_status = RED;
				}
			}
	
	uartHalt++;
	if(uartHalt > UART_IS_HALT)//maybe uart treo
	{
		resetUART();
		uartHalt = 1;
	}
	//Tinh toan timenow
	currtime.tm_year = 100+ years;
	currtime.tm_mon  = months-1;
	currtime.tm_mday = days;
	currtime.tm_sec  = seconds;
	currtime.tm_min  = minutes;
	currtime.tm_hour = hours;
	timenow = mktime(&currtime);
}
/*
	Main programs - loop
*/
void slaveClockRun(void)
{
	
	
	if(t_check_link_ms > MSEC_PHYSTATUS_CHECK)
		{
			t_check_link_ms = 0;
			checkLANcable();
			if((LANServicesInitFlag == 0)&&(phylink == PHY_LINK_ON))
				initLanServices();
		}
		
	if(ms_couters > 999) 
		{
			ms_couters = 0;
			oneSecondfucns();	
		}
		

		
		
	uart2_processing();
		
	if(rtc_pps == JustHigh)	
		{
			rtc_pps = Done;
			update_display();	
			
		}
			
	if(phylink != PHY_LINK_ON) // NO LAN CABLE!!!
	{
		//Uu tien tin hieu RS485, neu ko co rs485 chay cai NTP
		if(syncRS485_timeout == SYNC_OUTofTIME) slave_clock.sync_status = RED;
		return;// ko cam day mang thi ko lam gi lien quan den LAN nua
	}
		
	//Uu tien tin hieu RS485, neu ko co rs485 chay cai NTP
	//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
	SNTP_run();
	//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
	//SNMP
	snmpd_run();
	
	// web server 	
	httpServer_run(0);
	httpServer_run(1);
	httpServer_run(2);
	
	#ifdef OverTheAir
	//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
	loopback_tcpc(SOCK_UDPS,(uint8_t*)udp_message,destip_PC,23);
	//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
	#endif
}



/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == SQW_Pin)
  {
		//Dong bo lai phan le cua giay
		fractionOfSecond = 3;
		rtc_pps = JustHigh;
		seconds++;
		if((just_set_time_flag == 1) || (seconds>59))
		{
			laythoigian();
			just_set_time_flag = 0;
		}
		else 
		{
			timenow++;
		}
		//update_display();	
		
		
  }
	
	if (GPIO_Pin == FR_Pin)
  {
		
		chinhdosang();
		
  }
	
	if (GPIO_Pin == INTn_Pin)
  {
		//Nhan duoc ban tin NTP, xu ly thoi gian va phan hoi

  }
	
}
/*
Hien thi thoi gian
*/
void MAX7219_Init (void);
void update_display(void)
{
	
		
	#ifdef SLAVE_MATRIX
				load_line1(days,months,years);
				scan_7up();
				load_line2(hours,minutes,seconds,1);
				scan_5down();
			
				
				if     (slave_clock.sync_status == RED) 		{HAL_GPIO_WritePin(GPIOC, PC3_Pin, GPIO_PIN_RESET);HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);}//RED
				else if(slave_clock.sync_status == GREEN)   {HAL_GPIO_WritePin(GPIOC, PC3_Pin, GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);}//GREEN
				else HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2|PC3_Pin, GPIO_PIN_RESET);//GREEN+RED
		
	#endif
		
	#ifndef SLAVE_MATRIX
			#ifdef SLAVE_WALL		
						MAX7219_Init();
						MAX7219_Init2();	
						day_display();
			#endif
	//Dong ho slave console bi nhieu khi hoat dong ko ro ly do @@
						MAX7219_Init();//Chong nhieu @@

						console_display();
						console_blink();
	#endif
}
/*
Cac chuong trinh con!!!!
*/


/*
Thi thoang uart treo ko ro ly do, reset neu nghi ngo
*/	
void resetUART(void)
{
	HAL_UART_Abort(&huart2);
	if (HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }
}






	
/*******************************************************************************/


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






