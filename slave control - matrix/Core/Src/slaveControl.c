#include "main.h"
#include "storevalue.h"
#include "stdio.h"
#include "wizchip_conf.h"

static uint32_t GetPage(uint32_t Address);
static uint32_t GetBank(uint32_t Address);
static uint8_t unlock_config =0;
#define MSEC_PHYSTATUS_CHECK 		1000		// msec
#define PHYStatus_check_enable 1


extern wiz_NetInfo myipWIZNETINFO;
extern volatile uint16_t phystatus_check_cnt;
extern SPI_HandleTypeDef hspi1;
extern I2C_HandleTypeDef hi2c3;

//----------------------Phan giao tiep i2c-thoi gian thuc ---------------------//
//Phai pull up SDA SCL
unsigned char i2c_write[1];
char ds3231_reg[7],giaycu,ngay_al,thang_al,nhietdo,nhietdole;
unsigned char i2c_rv[19];
void BCD_Decoder()
{
	//printf("i2c_rv[3]: %d",i2c_rv[3]);
	for(char x=0;x<7;x++) ds3231_reg[x]=(i2c_rv[x] & 0x0f) + (i2c_rv[x]>>4)*10;
	//ds3231_reg[3] --; 
}
unsigned char BCD_Encoder(unsigned char temp)
{
	return ((temp/10)<<4)|(temp%10);
}
void laythoigian(void)
{
	//HAL_I2C_Mem_Read(&hi2c3,0x68<<1,0,I2C_MEMADD_SIZE_8BIT,i2c_rv,19,1000); //read time
	if( HAL_I2C_Mem_Read(&hi2c3,0x68<<1,0,I2C_MEMADD_SIZE_8BIT,i2c_rv,19,1000) == HAL_ERROR) 
		{
			slave_clock.rtc_status = NO_RTC;
			//printf("NO RTC");
			return;
		}
	
	BCD_Decoder(); //chuyen doi
	
//	nhietdo = i2c_rv[14];
//	nhietdole = i2c_rv[18]>>6;
//	if(nhietdole == 1) nhietdole = 25;
//	else if(nhietdole == 2) nhietdole = 5;
//	else if(nhietdole == 3) nhietdole = 75;
//	else nhietdole = 0;
	
	hours   = ds3231_reg[2];
	minutes = ds3231_reg[1];
	seconds = ds3231_reg[0];
	days		= ds3231_reg[4];
	months  = ds3231_reg[5]; 
	years   = ds3231_reg[6];
}
void ghids(unsigned char add, unsigned char dat)
{
	i2c_write[0] = BCD_Encoder(dat);
	HAL_I2C_Mem_Write(&hi2c3,0x68<<1,add,I2C_MEMADD_SIZE_8BIT,i2c_write,1,1000); 
}
//----------------------Phan giao tiep i2c-thoi gian thuc ---------------------//

//Kiem tra xem day mang co cam hay k?
void PHYStatus_Check(void)
{
	uint8_t tmp;
	//static bool LED_status_backup;

	
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
	}	
void factoryRST(void)
	{
		
	}
void loadValue(void)
{
	__IO uint32_t MemoryProgramStatus = 0;
	__IO uint32_t data32 = 0;
	//uint32_t Address = 0;
	/* Check if the programmed data is OK
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/

  MemoryProgramStatus = 0x0;

  
    data32 = *(__IO uint32_t *)FLASH_USER_START_ADDR;
    
    if (data32 != DATA_32)
    {
      MemoryProgramStatus++;
    }
    
	
	/*Check if there is an issue to program data*/
  if (MemoryProgramStatus == 0)
  {
    /* No error detected. Switch on LED1*/
    //BSP_LED_On(LED1);
		//printf("Memory is ok\r\n");
  }
  else
  {
    /* Error detected. Switch on LED3*/
    //BSP_LED_On(LED3);
		printf("Error detected \r\n");
		return;
  }
	
	data32 = *(__IO uint32_t *)(FLASH_USER_START_ADDR+4);
	//printf("first data32 %x\r\n",data32);
	
	myipWIZNETINFO.ip[3] = (uint8_t)data32;
	myipWIZNETINFO.ip[2] = (uint8_t)(data32>>8);
	myipWIZNETINFO.ip[1] = (uint8_t)(data32>>16);
	myipWIZNETINFO.ip[0] = (uint8_t)(data32>>24);	
	//printf("Load IP: %d.%d.%d.%d\r\n",myipWIZNETINFO.ip[0],myipWIZNETINFO.ip[1],myipWIZNETINFO.ip[2],myipWIZNETINFO.ip[3]);
		
	data32 = *(__IO uint32_t *)(FLASH_USER_START_ADDR+8);
	//printf("last data32 %x\r\n",data32);
		data32 = *(__IO uint32_t *)(FLASH_USER_START_ADDR+12);
	//printf("first data32 %x\r\n",data32);
	data32 = *(__IO uint32_t *)(FLASH_USER_START_ADDR+16);
	//printf("last data32 %x\r\n",data32);
		data32 = *(__IO uint32_t *)(FLASH_USER_START_ADDR+20);
	//printf("first data32 %x\r\n",data32);
}
void storeValue(void)
{
	uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0,PageError = 0;
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint64_t temp;
	/* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();
	/* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
	/* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
  
  /* Get the 1st page to erase */
  FirstPage = GetPage(FLASH_USER_START_ADDR);

  /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage(FLASH_USER_END_ADDR) - FirstPage + 1;

  /* Get the bank */
  BankNumber = GetBank(FLASH_USER_START_ADDR);
  
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks       = BankNumber;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;
	
	
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
  {
    /*
      Error occurred while page erase.
      User can add here some code to deal with this error.
      PageError will contain the faulty page and then to know the code error on this page,
      user can call function 'HAL_FLASH_GetError()'
    */
		printf("Error occurred while page erase.\r\n");
    
  }
	
	/* Program the user Flash area word by word
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
	
	//temp = DATA_32;
	//temp = temp<<32;
	temp  = 0;
	temp |= (myipWIZNETINFO.ip[0] <<24) |(myipWIZNETINFO.ip[1] <<16)|(myipWIZNETINFO.ip[2] <<8)|myipWIZNETINFO.ip[3];
  printf("temp : %x\r\n",(myipWIZNETINFO.ip[0] <<24) +(myipWIZNETINFO.ip[1] <<16)+(myipWIZNETINFO.ip[2] <<8)+myipWIZNETINFO.ip[3]);
	temp = DATA_32 | (temp<<32);
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_USER_START_ADDR, temp) == HAL_OK)
    {
			printf("Saved!\r\n");
    }
   else
    {
      /* Error occurred while writing data in Flash memory.
         User can add here some code to deal with this error */
			printf("Error occurred while writing data in Flash memory\r\n");
      
    }
		
		temp  = 0xFFFFFF01C0A816FC;//sn|fw
		//temp |= (gWIZNETINFO.ip[0] <<24) |(gWIZNETINFO.ip[1] <<16)|(gWIZNETINFO.ip[2] <<8)|gWIZNETINFO.ip[3];
		//printf("temp : %x\r\n",(gWIZNETINFO.ip[0] <<24) +(gWIZNETINFO.ip[1] <<16)+(gWIZNETINFO.ip[2] <<8)+gWIZNETINFO.ip[3]);
		
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (FLASH_USER_START_ADDR+8), temp) == HAL_OK)
    {
			printf("Saved!\r\n");
    }
   else
    {
      /* Error occurred while writing data in Flash memory.
         User can add here some code to deal with this error */
			printf("Error occurred while writing data in Flash memory\r\n");
      
    }
		
		temp  = 0x0808080801020304;//sn|fw
		//temp |= (gWIZNETINFO.ip[0] <<24) |(gWIZNETINFO.ip[1] <<16)|(gWIZNETINFO.ip[2] <<8)|gWIZNETINFO.ip[3];
		//printf("temp : %x\r\n",(gWIZNETINFO.ip[0] <<24) +(gWIZNETINFO.ip[1] <<16)+(gWIZNETINFO.ip[2] <<8)+gWIZNETINFO.ip[3]);
		
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (FLASH_USER_START_ADDR+16), temp) == HAL_OK)
    {
			printf("Saved!\r\n");
    }
   else
    {
      /* Error occurred while writing data in Flash memory.
         User can add here some code to deal with this error */
			printf("Error occurred while writing data in Flash memory\r\n");
      
    }
  
	
	/* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();
}

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
  uint32_t page = 0;

  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static uint32_t GetBank(uint32_t Addr)
{
  return FLASH_BANK_1;
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
	
	//printf("\r\n=== DNS Client Example ===============\r\n");
  //printf("> DNS 1st : %d.%d.%d.%d\r\n", tempWIZNETINFO.dns[0], tempWIZNETINFO.dns[1], tempWIZNETINFO.dns[2], tempWIZNETINFO.dns[3]);
  //printf("> DNS 2nd : %d.%d.%d.%d\r\n", DNS_2nd[0], DNS_2nd[1], DNS_2nd[2], DNS_2nd[3]);
  //printf("======================================\r\n");
   //printf("> Example Domain Name : %s\r\n", Domain_name);
}

void Net_Conf(wiz_NetInfo temp_netinfo)
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
	ctlnetwork(CN_SET_NETINFO, (void*) &temp_netinfo);
	#ifdef DebugEnable
	Display_Net_Conf();
	#endif
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
			printf("Cannot set imr...\r\n");
		}
		//printf("...all ok\r\n");
		Net_Conf(myipWIZNETINFO);
	
}
void control(void)
{
	if(unlock_config == 1)
	{//set all config here!!
	}
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



