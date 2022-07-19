//last edit 6July2022 by tuannq
#include "main.h"
#include <stdint.h>
#include "stdio.h"
#include "wizchip_conf.h"

extern wiz_NetInfo myipWIZNETINFO;
extern uint8_t ntpTimeServer_ip[4];													 
 														 


#define DATA_32                 ((uint32_t)0x12345678)
#define DATA_64                 ((uint64_t)0x1234567812345678)
uint32_t FirstPage = 0, NbOfPages = 0;
uint32_t Address = 0, PageError = 0;
__IO uint32_t MemoryProgramStatus = 0;
__IO uint32_t data32 = 0;


/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;

static uint32_t GetPage(uint32_t Addr)
{
  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;;
}

void stm32g474flashErase(void)
{
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

  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;
  EraseInitStruct.Banks       = FLASH_BANK_1;
  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
  {
    
    while (1)
    {
      #ifdef DebugEnable
      printf("error in Erase operation\r\n");
			#endif
    }
  }
	

  HAL_FLASH_Lock();
}

void stm32g474flashEraseThenSave(void)
{
	uint64_t temp;
	uint32_t t;

	/* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

 /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);


  /* Get the 1st page to erase */
  FirstPage = GetPage(FLASH_USER_START_ADDR);

  /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage(FLASH_USER_END_ADDR) - FirstPage + 1;

  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;
  EraseInitStruct.Banks       = FLASH_BANK_1;
  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
  {
    /*
      Error occurred while page erase.
      User can add here some code to deal with this error.
      PageError will contain the faulty page and then to know the code error on this page,
      user can call function 'HAL_FLASH_GetError()'
    */
    /* Infinite loop */
    
    {
      #ifdef DebugEnable
      printf("error in Erase operation\r\n");
			#endif
    }while (1);
  }
	

	
	//temp = DATA_64;
	temp=0;
	temp = (myipWIZNETINFO.sn[3] <<24) |(myipWIZNETINFO.sn[2] <<16)|(myipWIZNETINFO.sn[1] <<8)|myipWIZNETINFO.sn[0];
	temp = temp <<32;
	t = 0;
	t = (myipWIZNETINFO.ip[3] <<24) |(myipWIZNETINFO.ip[2] <<16)|(myipWIZNETINFO.ip[1] <<8)|myipWIZNETINFO.ip[0];
	temp = temp|t;
	
	
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_USER_START_ADDR, temp) == HAL_OK)
    {
			#ifdef DebugEnable
			printf("Saved IP & SN!\r\n");
			#endif
    }
		else
    {
			#ifdef DebugEnable
			printf("Error occurred while writing\r\n");
      #endif
    }
		
  temp=0;
	temp = (myipWIZNETINFO.dns[3] <<24) |(myipWIZNETINFO.dns[2] <<16)|(myipWIZNETINFO.dns[1] <<8)|myipWIZNETINFO.dns[0];
	temp = temp <<32;
	t = 0;
	t = (myipWIZNETINFO.gw[3] <<24) |(myipWIZNETINFO.gw[2] <<16)|(myipWIZNETINFO.gw[1] <<8)|myipWIZNETINFO.gw[0];
	temp = temp|t;
	
	
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_USER_START_ADDR+8, temp) == HAL_OK)
    {
			#ifdef DebugEnable
			printf("Saved GW & DNS!\r\n");
			#endif
    }
		else 
    {
			#ifdef DebugEnable
			printf("Cant save GW & DNS\r\n");
			#endif
    }
		
		temp = LEDintensity;
		temp = temp <<32;
		temp = temp + DATA_32;
		
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_USER_START_ADDR+16, temp) == HAL_OK)
    {
			#ifdef DebugEnable
			printf("Saved password\r\n");
			#endif
    }
		else 
    {
			#ifdef DebugEnable
			printf("Cant save password\r\n");
			#endif
    }
		
		temp = 0;
		temp = (ntpTimeServer_ip[3] <<24) |(ntpTimeServer_ip[2] <<16)|(ntpTimeServer_ip[1] <<8)|ntpTimeServer_ip[0];
		
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_USER_START_ADDR+24, temp) == HAL_OK)
    {
			#ifdef DebugEnable
			printf("Saved NTP server IP\r\n");
			#endif
    }
		else 
    {
			#ifdef DebugEnable
			printf("Cant save NTP server IP\r\n");
			#endif
    }
  
  HAL_FLASH_Lock();
}

void stm32g474_FactoryLoad(void)
{
	if(myMem->keyword == 0x12345678)
	{
		#ifdef DebugEnable
		printf("Mem ok, load and return\r\n");
		#endif
		//Muc sang cua LED
		LEDintensity 				 = myMem->LED_intensity;
		myipWIZNETINFO.ip[0] = myMem->ip[0];
		myipWIZNETINFO.ip[1] = myMem->ip[1];
		myipWIZNETINFO.ip[2] = myMem->ip[2];
		myipWIZNETINFO.ip[3] = myMem->ip[3];		
		
		myipWIZNETINFO.sn[0] = myMem->sn[0];
		myipWIZNETINFO.sn[1] = myMem->sn[1];
		myipWIZNETINFO.sn[2] = myMem->sn[2];
		myipWIZNETINFO.sn[3] = myMem->sn[3];
		
		myipWIZNETINFO.gw[0] = myMem->gw[0];
		myipWIZNETINFO.gw[1] = myMem->gw[1];
		myipWIZNETINFO.gw[2] = myMem->gw[2];
		myipWIZNETINFO.gw[3] = myMem->gw[3];
		
		myipWIZNETINFO.dns[0] = myMem->dns[0];
		myipWIZNETINFO.dns[1] = myMem->dns[1];
		myipWIZNETINFO.dns[2] = myMem->dns[2];
		myipWIZNETINFO.dns[3] = myMem->dns[3];
		
		ntpTimeServer_ip[0] = myMem->ntp_ip[0];
		ntpTimeServer_ip[1] = myMem->ntp_ip[1];
		ntpTimeServer_ip[2] = myMem->ntp_ip[2];
		ntpTimeServer_ip[3] = myMem->ntp_ip[3];
		//printf("NTP time server: %d.%d.%d.%d\r\n", ntpTimeServer_ip[0], ntpTimeServer_ip[1], ntpTimeServer_ip[2], ntpTimeServer_ip[3]);
		return;
	}
	#ifdef DebugEnable
		printf("Mem lost, load default\r\n");
	#endif
	stm32g474flashEraseThenSave();
}




