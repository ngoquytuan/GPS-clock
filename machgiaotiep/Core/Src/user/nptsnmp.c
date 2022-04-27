#include "main.h"
//#include "stm32g4xx_hal.h"
#include "wizchip_conf.h"
#include "stdio.h"

///////////////////////////////////////////////////////////////////////
// PHYStatus check ////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#define MSEC_PHYSTATUS_CHECK 		1000		// msec
#define PHYStatus_check_enable 1

extern SPI_HandleTypeDef hspi1;

extern wiz_NetInfo gWIZNETINFO;
extern volatile uint16_t phystatus_check_cnt;
#define WZN_ERR -1

//Kiem tra xem day mang co cam hay k?
void PHYStatus_Check(void)
{
	uint8_t tmp;
	//static bool LED_status_backup;

	////LED_status_backup = RGBLED_enable;
		ctlwizchip(CW_GET_PHYLINK, (void*) &tmp);

		// Error indicator: LED Green ON when no PHY link detected
		if(tmp == PHY_LINK_OFF)
		{
			/* Turn on LED1 */
			//GPIO_PinWrite(GPIOC, 1, 1);
			//delay_ms(1000);
		}
		else{
		/* Turn off LED1 */
    //GPIO_PinWrite(GPIOC, 1, 0);
		}

	//RGBLED_enable = LED_status_backup;
}

void checklink(void)
{// PHY status check counter
			if(PHYStatus_check_enable == 1)
			{
				if (phystatus_check_cnt > MSEC_PHYSTATUS_CHECK)
				{
					PHYStatus_Check();
					phystatus_check_cnt = 0;
					//printf("PHYStatus_Check\r\n");
				}
			}
			else
			{
				phystatus_check_cnt = 0;
			}
}

static void  wizchip_select(void)
{
	//GPIO_ResetBits(W5500_CS_GPIO_PORT, W5500_CS_PIN);
	HAL_GPIO_WritePin(SCSn_GPIO_Port, SCSn_Pin, GPIO_PIN_RESET);
}
static void  wizchip_deselect(void)
{
	//GPIO_SetBits(W5500_CS_GPIO_PORT, W5500_CS_PIN);
	uint8_t temp=0xff;
	HAL_GPIO_WritePin(SCSn_GPIO_Port, SCSn_Pin, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi1,&temp,1,100);
	//stm32_spi_rw(0xFF);
}
static uint8_t wizchip_read(void)
{
	uint8_t temp;
	//while (  SPI_I2S_GetFlagStatus(SPI2 , SPI_I2S_FLAG_TXE) == RESET );
	//SPI_I2S_SendData(SPI2 , 0xff);     // Dummy write to generate clock
	//while (  SPI_I2S_GetFlagStatus(SPI2 , SPI_I2S_FLAG_RXNE) == RESET );
	//return (unsigned char)SPI_I2S_ReceiveData(SPI2);
	 
	HAL_SPI_Receive(&hspi1,&temp,1,100);
	return temp;
}
static void  wizchip_write(uint8_t wb)
{
	//while (  SPI_I2S_GetFlagStatus(SPI2 , SPI_I2S_FLAG_TXE) == RESET );  // sending data Wait
	//SPI_I2S_SendData(SPI2 , wb);
	//while (  SPI_I2S_GetFlagStatus(SPI2 , SPI_I2S_FLAG_RXNE) == RESET ); // Wait for data
	//wb = SPI_I2S_ReceiveData(SPI2);        // Dummy read to generate clock
	HAL_SPI_Transmit(&hspi1,&wb,1,100);
}
static void wizchip_readburst(uint8_t* pBuf, uint16_t len)
{
	//stm32_wizchip_dma_transfer(1, pBuf, len);  //FALSE(0) for buff->SPI, TRUE(1) for SPI->buff
	HAL_SPI_Receive(&hspi1,pBuf,len,100);
}

static void  wizchip_writeburst(uint8_t* pBuf, uint16_t len)
{
	//stm32_wizchip_dma_transfer(0, pBuf, len);  //FALSE(0) for buff->SPI, TRUE(1) for SPI->buff
	HAL_SPI_Transmit(&hspi1,pBuf,len,100);
}

void Display_Net_Conf()
{
	uint8_t tmpstr[6] = {0,};
	wiz_NetInfo gWIZNETINFO;

	ctlnetwork(CN_GET_NETINFO, (void*) &gWIZNETINFO);
	ctlwizchip(CW_GET_ID,(void*)tmpstr);

	// Display Network Information
	if(gWIZNETINFO.dhcp == NETINFO_DHCP) printf("\r\n===== %s NET CONF : DHCP =====\r\n",(char*)tmpstr);
		else printf("\r\n===== %s NET CONF : Static =====\r\n",(char*)tmpstr);

	printf("\r\nMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", gWIZNETINFO.mac[0], gWIZNETINFO.mac[1], gWIZNETINFO.mac[2], gWIZNETINFO.mac[3], gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
	printf("IP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
	printf("GW: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0], gWIZNETINFO.gw[1], gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
	printf("SN: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1], gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);
	printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0], gWIZNETINFO.dns[1], gWIZNETINFO.dns[2], gWIZNETINFO.dns[3]);
	
	//printf("\r\n=== DNS Client Example ===============\r\n");
  //printf("> DNS 1st : %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0], gWIZNETINFO.dns[1], gWIZNETINFO.dns[2], gWIZNETINFO.dns[3]);
  //printf("> DNS 2nd : %d.%d.%d.%d\r\n", DNS_2nd[0], DNS_2nd[1], DNS_2nd[2], DNS_2nd[3]);
  //printf("======================================\r\n");
   //printf("> Example Domain Name : %s\r\n", Domain_name);
}

void Net_Conf(wiz_NetInfo netinfo)
{
	/*
	wiz_NetInfo gWIZNETINFO = {
		{ 0x00, 0x08, 0xDC, 0x44, 0x55, 0x66 },				// Mac address
		{ 192, 168, 1, 91 },								// IP address
		{ 255, 255, 255, 0},								// Subnet mask
		{ 192, 168, 1, 1},									// Gateway
		{ 8, 8, 8, 8},										// DNS Server
	};
	*/
	ctlnetwork(CN_SET_NETINFO, (void*) &netinfo);

	Display_Net_Conf();
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
		//printf("...2");
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
    
		do
    {
       if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
          ;//printf("Unknown PHY Link stauts.\r\n");
    }while(tmp == PHY_LINK_OFF);

		
		//Cau hinh ngat tren Socket 0, vi can biet thoi diem ban tin NTP den!!
		temp = IK_SOCK_0;
		if(ctlwizchip(CW_SET_INTRMASK, &temp) == WZN_ERR)
		{
			printf("Cannot set imr...\r\n");
		}
		//printf("...all ok\r\n");
		Net_Conf(gWIZNETINFO);
		

}



