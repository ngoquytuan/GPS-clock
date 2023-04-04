#include "main.h"
#include <time.h>
#include <string.h>
#include "stdio.h"
#include "wizchip_conf.h"
#include "snmp.h"
#include "httpServer.h"
#include "mydefines.h"

//#define DebugEnable 1
extern uint8_t tim4ITflag;
//extern uint32_t ms_couters2;
extern time_t built_time;
extern time_t timenow;

extern uint8_t NTP_alreadySent;
extern uint8_t NTP_alreadySentTimeOut;

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
extern wiz_NetInfo myipWIZNETINFO;
extern volatile uint16_t phystatus_check_cnt;
extern SPI_HandleTypeDef hspi1;
extern I2C_HandleTypeDef hi2c3;
extern struct tm currtime;

slave_status slave_clock;
char ds3231_reg[7];
uint8_t rtc_pps = 0;
uint8_t u2Timeout = 0;

uint8_t phylink = PHY_LINK_OFF;

uint16_t t_check_link_ms = 0;

//static uint8_t unlock_config =0;


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

uint32_t delta_RS485=9999;

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
static void  wizchip_select(void);
static void  wizchip_deselect(void);
static uint8_t wizchip_read(void);
static void  wizchip_write(uint8_t wb);
static void wizchip_readburst(uint8_t* pBuf, uint16_t len);
static void  wizchip_writeburst(uint8_t* pBuf, uint16_t len);


#ifndef SLAVE_MATRIX	
		void MAX7219_SendAddrDat (unsigned char addr,unsigned char dat);
#endif


uint8_t time2SaveRTC = PeriodSaveRTC;


uint8_t syncRS485_timeout = SYNC_OUTofTIME;

//uint8_t testLinhTinh = 0;

uint8_t just_set_time_flag = 0;

#ifdef OverTheAir
#include "socket.h"
#include "wizchip_conf.h"
#include "socketdefines.h"
#ifndef DATA_BUF_SIZE
	#define DATA_BUF_SIZE	2048
#endif
#define	_LOOPBACK_DEBUG_
char udp_message[DATA_BUF_SIZE];
uint8_t  destip_PC[4]={192, 168, 1, 164};
uint8_t send_debug_message = 0;
uint8_t mysize = 9;
int32_t loopback_tcpc(uint8_t sn, uint8_t* buf, uint8_t* destip, uint16_t destport)
{
  int32_t ret; // return value for SOCK_ERRORs
  uint16_t size = 0, sentsize=0;

  // Destination (TCP Server) IP info (will be connected)
  // >> loopback_tcpc() function parameter
  // >> Ex)
  //	uint8_t destip[4] = 	{192, 168, 0, 214};
  //	uint16_t destport = 	5000;

  // Port number for TCP client (will be increased)
  uint16_t any_port = 	139;

  // Socket Status Transitions
  // Check the W5500 Socket n status register (Sn_SR, The 'Sn_SR' controlled by Sn_CR command or Packet send/recv status)
  switch (getSn_SR(sn))
  {
    case SOCK_ESTABLISHED:
      if (getSn_IR(sn) & Sn_IR_CON)	// Socket n interrupt register mask; TCP CON interrupt = connection with peer is successful
      {
#ifdef _LOOPBACK_DEBUG_
	printf("%d:Connected to - %d.%d.%d.%d : %d\r\n", sn, destip[0], destip[1], destip[2], destip[3], destport);
#endif
	setSn_IR(sn, Sn_IR_CON);  // this interrupt should be write the bit cleared to '1'
      }
			
			
			if(send_debug_message)
				{
					send_debug_message = 0;
					#ifdef SLAVE_CONSOLE
					ret = send(sn, (uint8_t*)"Console\r\n", 9);
					#endif
					#ifdef SLAVE_MATRIX
					ret = send(sn, (uint8_t*)"Matrix\r\n", 8);
					#endif
					ret = send(sn, buf, mysize); // Data send process (User's buffer -> Destination through H/W Tx socket buffer)
								if (ret < 0) // Send Error occurred (sent data length < 0)
								{
									close(sn); // socket close
									return ret;
								}
				}
      //////////////////////////////////////////////////////////////////////////////////////////////
      // Data Transaction Parts; Handle the [data receive and send] process
      //////////////////////////////////////////////////////////////////////////////////////////////
      if ((size = getSn_RX_RSR(sn)) > 0) // Sn_RX_RSR: Socket n Received Size Register, Receiving data length
      {
	if (size > DATA_BUF_SIZE)
          size = DATA_BUF_SIZE; // DATA_BUF_SIZE means user defined buffer size (array)
	ret = recv(sn, buf, size); // Data Receive process (H/W Rx socket buffer -> User's buffer)

	if (ret <= 0)    // If the received data length <= 0, receive failed and process end
          return ret;

	sentsize = 0;
  
	
	// Data sentsize control
	while (size != sentsize)
	{
          ret = send(sn, buf + sentsize, size - sentsize); // Data send process (User's buffer -> Destination through H/W Tx socket buffer)
          if (ret < 0) // Send Error occurred (sent data length < 0)
          {
            close(sn); // socket close
            return ret;
          }
          sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
	}
      }
      //////////////////////////////////////////////////////////////////////////////////////////////
      break;

    case SOCK_CLOSE_WAIT:
#ifdef _LOOPBACK_DEBUG_
      printf("%d:CloseWait\r\n",sn);
#endif
      if ((ret=disconnect(sn)) != SOCK_OK)
        return ret;
#ifdef _LOOPBACK_DEBUG_
      printf("%d:Socket Closed\r\n", sn);
#endif
      break;

    case SOCK_INIT:
#ifdef _LOOPBACK_DEBUG_
      printf("%d:Try to connect to the %d.%d.%d.%d : %d\r\n", sn, destip[0], destip[1], destip[2], destip[3], destport);
#endif
      if ((ret = connect(sn, destip, destport)) != SOCK_OK)   // Try to TCP connect to the TCP server (destination)
        return ret;
      break;

    case SOCK_CLOSED:
      close(sn);
      if ((ret = socket(sn, Sn_MR_TCP, any_port++, 0x00)) != sn)  // TCP socket open with 'any_port' port number
        return ret;
#ifdef _LOOPBACK_DEBUG_
      printf("%d:TCP client loopback start\r\n",sn);
      printf("%d:Socket opened\r\n",sn);
#endif
      break;
    default:
      break;
  }
  
  return 1;
}

#endif
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

		
		//Cau hinh ngat tren Socket 2, vi can biet thoi diem ban tin NTP den!!
//		temp = IK_SOCK_2;
//		if(ctlwizchip(CW_SET_INTRMASK, &temp) == -1)
//		{
//			//printf("Cannot set imr...\r\n");
//		}
		Net_Conf(myipWIZNETINFO);
	
}
/*
	Kiem tra W5500, SPI co on ko, day mang co cam ko?
*/
uint8_t checkDaymang(void)
{
	uint8_t tmp;
  //phylink = PHY_LINK_OFF;
	if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
	{
		#ifdef DebugEnable
		//printf("Unknown PHY Link stauts.\r\n");//Loi spi???
		#endif
		slave_clock.ethernet_connection = NO_CONNECTION;
		phylink = PHY_LINK_OFF;
		return 0;
	}
	
	if(tmp == PHY_LINK_OFF) 
		{
			phylink = PHY_LINK_OFF;
			slave_clock.ethernet_connection = NO_CONNECTION;
			return 0;//Ko cam day mang
		}
	

	//printf("Co cam day mang nhe\r\n");

	phylink = PHY_LINK_ON;
	slave_clock.ethernet_connection = CONNECTED;
  return 1;  
}

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

/*
Pre load programs
*/
void slaveClockFucnsInit(void)
{
//	time_t time_temp;
	//ghids(14,0);//1HZ out SQW
	
	HAL_Delay(999);
	//Neu ko co xung RTC => RTC het pin hoac hong!
	if(rtc_pps == 0) ghids(14,0);//1HZ out SQW
	//printf("RTC pps: %d\r\n",rtc_pps);
	#ifdef DebugEnable
	printf("This code gen by STMcube STM32G474@128MHz\r\n");
	#endif
	//Hold FR button and turn Clock on => factory reset!!!
	slaveClockFactoryLoad();
	//LOAD memory settings
	stm32g474_FactoryLoad();
	

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
	HAL_Delay(100);
	
	//Cau hinh ngat tren Socket 2, vi can biet thoi diem ban tin NTP den!!
	w5500_lib_init();
	
	snmp_init();
	SNTP_init();
	loadwebpages();
	slave_clock.sync_status = RED;
	laythoigian();
		
	currtime.tm_year = 100+ years;
	currtime.tm_mon  = months-1;
	currtime.tm_mday = days;
	currtime.tm_sec  = seconds;
	currtime.tm_min  = minutes;
	currtime.tm_hour = hours;
	timenow = mktime(&currtime);
	//RTC neu bi loi	
	if(timenow < built_time) 
		{
			timeinfo = localtime( &built_time );
			asctime(timeinfo);
			days    = timeinfo->tm_mday;
			months  = timeinfo->tm_mon +1;
			years   = timeinfo->tm_year-100;
			hours   = timeinfo->tm_hour;
			minutes = timeinfo->tm_min;
			seconds = timeinfo->tm_sec+1;
			//printf("RTC loi!!!\r\n");
			RTC_Update();
			timenow = built_time ;
		}
		
	//printf("Time :%dh%dm%ds;%d %d/%d/%d; LED intensity : %d\r\n",hours,minutes,seconds,ds3231_reg[3],days,months,years, LEDintensity);
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
		
	slave_clock.ntp_client_status   = NO_CONNECTION;
	slave_clock.ethernet_connection = NO_CONNECTION;
	slave_clock.rs458_connection    = NO_CONNECTION;
	
	HAL_UART_Abort(&huart2);
	if (HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }
	
}


/*
	One second fuctions
  Moi giay ham nay goi 1 lan
*/
void oneSecondfucns(void)
{
	//send_debug_message = 1;
	//mysize = sprintf(udp_message,"tuan %d\r\n",1);
	
	
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
	Main programs
*/
void slaveClockRun(void)
{
	
	
	if(t_check_link_ms > MSEC_PHYSTATUS_CHECK)
		{
			t_check_link_ms = 0;
			checkDaymang();
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
			//update_display();	
			
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
		update_display();	
		rtc_pps = JustHigh;
		
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

/*
Cap nhat thoi gian vao RTC
*/
void RTC_Update(void)
	{
		//ghids(14,0);//1HZ out SQW
		ghids(DS_SECOND_REG,seconds);
		ghids(DS_MIN_REG,minutes);
		ghids(DS_HOUR_REG,hours);
		
		ghids(DS_DATE_REG,days);
		ghids(DS_MONTH_REG,months);
		ghids(DS_YEAR_REG,years);		
		
		just_set_time_flag = 1;
		laythoigian();
		update_display();
	}
//----------------------Phan giao tiep i2c-thoi gian thuc ---------------------//
//Phai pull up SDA SCL

/*
Doc thoi gian tu RTC
*/
void laythoigian(void)
{
	unsigned char i2c_rv[19];
	if( HAL_I2C_Mem_Read(&hi2c3,0x68<<1,0,I2C_MEMADD_SIZE_8BIT,i2c_rv,19,1000) == HAL_ERROR) 
		{
			slave_clock.rtc_status = NO_RTC;
			//printf("NO RTC");
			return;
		}
	
	for(char x=0;x<7;x++) ds3231_reg[x]=(i2c_rv[x] & 0x0f) + (i2c_rv[x]>>4)*10;
	
	seconds = ds3231_reg[0];
	hours   = ds3231_reg[2];
	minutes = ds3231_reg[1];
	
	days		= ds3231_reg[4];
	months  = ds3231_reg[5]; 
	years   = ds3231_reg[6];
		
		//Tinh toan timenow
	currtime.tm_year = 100+ years;
	currtime.tm_mon  = months-1;
	currtime.tm_mday = days;
	currtime.tm_sec  = seconds;
	currtime.tm_min  = minutes;
	currtime.tm_hour = hours;
	timenow = mktime(&currtime);
}

unsigned char BCD_Encoder(unsigned char temp)
{
	return ((temp/10)<<4)|(temp%10);
}
void ghids(unsigned char add, unsigned char dat)
{
	unsigned char i2c_write[1];
	i2c_write[0] = BCD_Encoder(dat);
	HAL_I2C_Mem_Write(&hi2c3,0x68<<1,add,I2C_MEMADD_SIZE_8BIT,i2c_write,1,1000); 
}
//----------------------Phan giao tiep i2c-thoi gian thuc ---------------------//


	
/*******************************************************************************/
/*Cac ham phu tro ko can phai doc	*/
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
