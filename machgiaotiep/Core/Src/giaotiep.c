#include "main.h"
#include <stdint.h>
#include "stdio.h"
#include "wizchip_conf.h"
#include "storevalue.h"

extern wiz_NetInfo gWIZNETINFO;

static uint32_t GetPage(uint32_t Address);
static uint32_t GetBank(uint32_t Address);

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
		//printf("Error detected \r\n");
		return;
  }
	
	data32 = *(__IO uint32_t *)(FLASH_USER_START_ADDR+4);
	//printf("first data32 %x\r\n",data32);
	
	gWIZNETINFO.ip[3] = (uint8_t)data32;
	gWIZNETINFO.ip[2] = (uint8_t)(data32>>8);
	gWIZNETINFO.ip[1] = (uint8_t)(data32>>16);
	gWIZNETINFO.ip[0] = (uint8_t)(data32>>24);	
	//printf("Load IP: %d.%d.%d.%d\r\n",gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
		
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
	temp |= (gWIZNETINFO.ip[0] <<24) |(gWIZNETINFO.ip[1] <<16)|(gWIZNETINFO.ip[2] <<8)|gWIZNETINFO.ip[3];
  printf("temp : %x\r\n",(gWIZNETINFO.ip[0] <<24) +(gWIZNETINFO.ip[1] <<16)+(gWIZNETINFO.ip[2] <<8)+gWIZNETINFO.ip[3]);
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

