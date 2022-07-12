#include "main.h"
#include <stdint.h>
#include "stdio.h"
#include "wizchip_conf.h"
#include "w5500.h"
#include "snmp.h"
#include "httpServer.h"
#include <string.h>
///////////////////////////////////////////////////////////////////////
//////////////////////// PHYStatus check //////////////////////////////
///////////////////////////////////////////////////////////////////////
#define MSEC_PHYSTATUS_CHECK 		1000		// msec
uint16_t t_check_link_ms = 0;

extern volatile uint32_t httpServer_tick_1s;
extern wiz_NetInfo myipWIZNETINFO;
extern volatile uint16_t phystatus_check_cnt;
extern uint8_t u1Timeout;
extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart1;
uint8_t phylink = PHY_LINK_OFF;
uint8_t unlock_config =0;
//static uint32_t GetPage(uint32_t Address);
//static uint32_t GetBank(uint32_t Address);
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
	Display_Net_Conf();
}

void w5500_lib_init(void){
		
		uint8_t tmp;
		intr_kind temp;
		uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};	
		
		//printf("w5500_lib_init...1");
		HAL_GPIO_WritePin(RSTn_GPIO_Port, RSTn_Pin, GPIO_PIN_RESET);
		HAL_Delay(100);
		
		//////////
   // TODO //
   ////////////////////////////////////////////////////////////////////////////////////////////////////
   // First of all, Should register SPI callback functions implemented by user for accessing WIZCHIP //
   ////////////////////////////////////////////////////////////////////////////////////////////////////
   	
    /* Chip selection call back */
		reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
		/* SPI Read & Write callback function */
    reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);
		reg_wizchip_spiburst_cbfunc(wizchip_readburst, wizchip_writeburst);
		//RST High to run
		HAL_GPIO_WritePin(RSTn_GPIO_Port, RSTn_Pin, GPIO_PIN_SET);
		HAL_Delay(100);
		
    ////////////////////////////////////////////////////////////////////////
		/* WIZCHIP SOCKET Buffer initialize */
		//Initializes to WIZCHIP with SOCKET buffer size 2 or 1 dimension array typed uint8_t
    if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
    {
       printf("WIZCHIP Initialized fail.\r\n");
       //while(1);
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
			#ifdef DebugEnable
			printf("Cannot set imr...\r\n");
			#endif
		}
		//printf("...all ok\r\n");
		Net_Conf(myipWIZNETINFO);
	
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

void control(uint8_t* uartcmd)
{
	//uint8_t tmp[4];
	if(unlock_config == 1)
	{//set all config here!!
		if((uartcmd[0] =='I')&&(uartcmd[1] =='P')&&(uartcmd[7] =='.')&&(uartcmd[11] =='.')&&(uartcmd[15] =='.'))
		{
			//IP: 192.168.222.252\r\n;IP: 092.068.022.052\r\n
			if((uartcmd[4]>'2') || (uartcmd[4]<'0')) return;
			if((uartcmd[8]>'2') || (uartcmd[8]<'0')) return;	
			if((uartcmd[12]>'2') || (uartcmd[12]<'0')) return;
			if((uartcmd[16]>'2') || (uartcmd[16]<'0')) return;
			
			if((uartcmd[5]>'9') || (uartcmd[5]<'0')) return;
			if((uartcmd[9]>'9') || (uartcmd[9]<'0')) return;	
			if((uartcmd[13]>'9') || (uartcmd[13]<'0')) return;
			if((uartcmd[17]>'9') || (uartcmd[17]<'0')) return;
			
			if((uartcmd[6]>'9') || (uartcmd[6]<'0')) return;
			if((uartcmd[10]>'9') || (uartcmd[10]<'0')) return;	
			if((uartcmd[14]>'9') || (uartcmd[14]<'0')) return;
			if((uartcmd[18]>'9') || (uartcmd[18]<'0')) return;
			
			myipWIZNETINFO.ip[0] = 100*(uartcmd[4]-'0')  + 10*(uartcmd[5]-'0')  + (uartcmd[6]-'0');
			myipWIZNETINFO.ip[1] = 100*(uartcmd[8]-'0')  + 10*(uartcmd[9]-'0')  + (uartcmd[10]-'0');
			myipWIZNETINFO.ip[2] = 100*(uartcmd[12]-'0') + 10*(uartcmd[13]-'0') + (uartcmd[14]-'0');
			myipWIZNETINFO.ip[3] = 100*(uartcmd[16]-'0') + 10*(uartcmd[17]-'0') + (uartcmd[18]-'0');
			
			printf("New IP: %d.%d.%d.%d\r\n",myipWIZNETINFO.ip[0],myipWIZNETINFO.ip[1],myipWIZNETINFO.ip[2],myipWIZNETINFO.ip[3]);
			stm32g474flashEraseThenSave();
			
			//NVIC_SystemReset();
		}
		if((uartcmd[0] =='G')&&(uartcmd[1] =='W')&&(uartcmd[7] =='.')&&(uartcmd[11] =='.')&&(uartcmd[15] =='.'))
		{
			//GW: 192.168.222.252\r\n;GW: 092.068.022.052\r\n
			if((uartcmd[4]>'2') || (uartcmd[4]<'0')) return;
			if((uartcmd[8]>'2') || (uartcmd[8]<'0')) return;	
			if((uartcmd[12]>'2') || (uartcmd[12]<'0')) return;
			if((uartcmd[16]>'2') || (uartcmd[16]<'0')) return;
			
			if((uartcmd[5]>'9') || (uartcmd[5]<'0')) return;
			if((uartcmd[9]>'9') || (uartcmd[9]<'0')) return;	
			if((uartcmd[13]>'9') || (uartcmd[13]<'0')) return;
			if((uartcmd[17]>'9') || (uartcmd[17]<'0')) return;
			
			if((uartcmd[6]>'9') || (uartcmd[6]<'0')) return;
			if((uartcmd[10]>'9') || (uartcmd[10]<'0')) return;	
			if((uartcmd[14]>'9') || (uartcmd[14]<'0')) return;
			if((uartcmd[18]>'9') || (uartcmd[18]<'0')) return;
			
			myipWIZNETINFO.gw[0] = 100*(uartcmd[4]-'0')  + 10*(uartcmd[5]-'0')  + (uartcmd[6]-'0');
			myipWIZNETINFO.gw[1] = 100*(uartcmd[8]-'0')  + 10*(uartcmd[9]-'0')  + (uartcmd[10]-'0');
			myipWIZNETINFO.gw[2] = 100*(uartcmd[12]-'0') + 10*(uartcmd[13]-'0') + (uartcmd[14]-'0');
			myipWIZNETINFO.gw[3] = 100*(uartcmd[16]-'0') + 10*(uartcmd[17]-'0') + (uartcmd[18]-'0');
			
			printf("New GW: %d.%d.%d.%d\r\n",myipWIZNETINFO.ip[0],myipWIZNETINFO.ip[1],myipWIZNETINFO.ip[2],myipWIZNETINFO.ip[3]);
			stm32g474flashEraseThenSave();
			
			//NVIC_SystemReset();
		}
		if((uartcmd[0] =='R')&&(uartcmd[1] =='E')&&(uartcmd[2] =='S')&&(uartcmd[3] =='E')&&(uartcmd[4] =='T'))
		{
			//Restart MCU
			NVIC_SystemReset();
		}
		//HELP
		printf("\r\nHELP\r\n");
		printf("IP: 192.168.022.162\r\n");
		printf("GW: 000.000.000.000\r\n");
	}
	else
	{
		if((uartcmd[0] =='A')&&(uartcmd[1] =='T'))
		{
			printf("AT on\r\n");
			
			unlock_config = 1;
		}
		else 
		{
			printf("Linh tinh\r\n");
		}
	}
	
}

void uart1_processing(void)
{
	if(u1Timeout == 1) 
			{
				u1Timeout = 0;
				//printf("aRxBuffer %s; \r\n",aRxBuffer);
				control(aRxBuffer);
				huart1.pRxBuffPtr = (uint8_t *)aRxBuffer;
				huart1.RxXferCount = RXBUFFERSIZE;
				memset(aRxBuffer,0,RXBUFFERSIZE); 
			}
}

void machGiaoTiep(void)
{
		if(t_check_link_ms > MSEC_PHYSTATUS_CHECK)
		{
			t_check_link_ms = 0;
			checkDaymang();
			httpServer_tick_1s++;
		}
		uart1_processing();
		if(phylink != PHY_LINK_ON) return;// ko cam day mang thi ko lam gi het!!!
		
	  //NTP server 
		NTPUDP();
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

