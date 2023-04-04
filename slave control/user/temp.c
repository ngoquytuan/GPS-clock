code linh tinh
#ifdef _U1_DEBUG_ENABLE_
		printf("new timestamp:%d, %d\r\n",timenow, timeOutLostSignal);
		timeinfo = localtime( &timenow );
		printf("Current local time and date: %s\r\n", asctime(timeinfo));
		#endif
		
		

//void BCD_Decoder()
//{
//	//printf("i2c_rv[3]: %d",i2c_rv[3]);
//	for(char x=0;x<7;x++) ds3231_reg[x]=(i2c_rv[x] & 0x0f) + (i2c_rv[x]>>4)*10;
//	//ds3231_reg[3] --; 
//}

		//RTC second
		//RTC hoat dong binh thuong
		//rtc_timeout = 0;
		
  	
		//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
		//printf("SQW\r\n");
		//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);

//	

//		HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
//		printf("Factory reset\r\n");
//		HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);

		//Receied NTP message, processing and respond
		//wzn_event_handle();
		//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
		//printf("NTP ngat\r\n");
		//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
//void currentUptime(void *ptr, uint8_t *len)
//	{}
//void setMyValue(int value)	//snmpset -v 1 -c public 192.168.1.246 .1.3.6.1.4.1.6.1.2 i 123456
//{
//	printf("Got my value :%d\r\n",value);
//}
/*
void getMyValue()						//snmpget -v 1 -c public 192.168.1.246 .1.3.6.1.4.1.6.1.2
{
	printf("getMyValue\r\n");
	//return 301188;
}*/

/*
    // Example Codes for SNMP Trap
    {
		dataEntryType enterprise_oid = {0x0a, {0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0x9b, 0x19, 0x01, 0x00},
    	    									SNMPDTYPE_OBJ_ID, 0x0a, {"\x2b\x06\x01\x04\x01\x81\x9b\x19\x10\x00"},	NULL, NULL};

		dataEntryType trap_oid1 = {8, {0x2b, 6, 1, 4, 1, 0, 11, 0}, SNMPDTYPE_OCTET_STRING, 30, {""}, NULL, NULL};
		dataEntryType trap_oid2 = {8, {0x2b, 6, 1, 4, 1, 0, 12, 0}, SNMPDTYPE_INTEGER, 4, {""}, NULL, NULL};

		strcpy((char *)trap_oid1.u.octetstring, "Alert!!!"); 	// String added
		trap_oid2.u.intval = 123456;	// Integer value added

		// Generic Trap: warmStart
		snmp_sendTrap((void *)"192.168.0.214", (void *)"192.168.0.112", (void *)"public", enterprise_oid, SNMPTRAP_WARMSTART, 0, 0);

		// Enterprise-Specific Trap
		snmp_sendTrap((void *)"192.168.0.214", (void *)"192.168.0.112", (void *)"public", enterprise_oid, 6, 0, 2, &trap_oid1, &trap_oid2);
	}*/
	
	#ifdef _SNMP_DEBUG_
void dumpCode(uint8_t* header, uint8_t* tail, uint8_t *buff, int32_t len)
{
	int i;

	printf((char const*)header);

	for (i=0; i<len; i++)
	{
		if ( i%16==0 )	printf("0x%04x : ", i);
		printf("%02x ",buff[i]);

		if ( i%16-15==0 )
		{
			int j;
			printf("  ");
			for (j=i-15; j<=i; j++)
			{
				if ( isprint(buff[j]) )	printf("%c", buff[j]);
				else					printf(".");
			}
			printf("\r\n");
		}
	}

	if ( i%16!=0 )
	{
		int j;
		int spaces=(len-i+16-i%16)*3+2;
		for (j=0; j<spaces; j++) 	printf(" ");
		for (j=i-i%16; j<len; j++)
		{
			if ( isprint(buff[j]) )	printf("%c", buff[j]);
			else					printf(".");
		}
	}
	printf((char const*)tail);
}
#endif

/**
int32_t snmpd_run(void)
{
  int32_t ret;
	int32_t len = 0;
  //uint8_t a[4] = {192,168,22,164};  
	uint8_t svr_addr[6];
	uint16_t  svr_port;

	if(SOCK_SNMP_AGENT > _WIZCHIP_SOCK_NUM_) return -99;
    
	switch(getSn_SR(SOCK_SNMP_AGENT))
	{
		case SOCK_UDP :
			if ( (len = getSn_RX_RSR(SOCK_SNMP_AGENT)) > 0)
			{
				request_msg.len= recvfrom(SOCK_SNMP_AGENT, request_msg.buffer, len, svr_addr, &svr_port);
			}
			else
			{
				request_msg.len = 0;
			}

			if (request_msg.len > 0)
			{
#ifdef _SNMP_DEBUG_
				dumpCode((void *)"\r\n[Request]\r\n", (void *)"\r\n", request_msg.buffer, request_msg.len);
#endif
				// Initialize
				request_msg.index = 0;
				response_msg.index = 0;
				errorStatus = errorIndex = 0;
				memset(response_msg.buffer, 0x00, MAX_SNMPMSG_LEN);

				// Received message parsing and send response process
				if (parseSNMPMessage() != -1)
				{
					ret = sendto(SOCK_SNMP_AGENT, response_msg.buffer, response_msg.index, svr_addr, svr_port);
					
					//printf("sent \r\n");
					if(ret < 0)
               {
                  //printf("S %d: sendto error. %d\r\n",SOCK_SNMP_AGENT,ret);
                  return ret;
               }
				}

#ifdef _SNMP_DEBUG_
				
				printf("S %d: sendto %d.%d.%d.%d port %d\r\n",SOCK_SNMP_AGENT,svr_addr[0],svr_addr[1],svr_addr[2],svr_addr[3],svr_port);
				dumpCode((void *)"\r\n[Response]\r\n", (void *)"\r\n", response_msg.buffer, response_msg.index);
				sendto(SOCK_SNMP_AGENT, response_msg.buffer, response_msg.index, svr_addr, svr_port);
				sendto(4,response_msg.buffer, response_msg.index, svr_addr, svr_port);
				//sendto(SOCK_UDPS,serverPacket,NTP_PACKET_SIZE,destip,destport);
				printf("sent \r\n");
#endif
			}
			break;

		case SOCK_CLOSED :
			if((ret = socket(SOCK_SNMP_AGENT, Sn_MR_UDP, PORT_SNMP_AGENT, 0x00)) != SOCK_SNMP_AGENT)
				return ret;
#ifdef _SNMP_DEBUG_
			printf(" Socket[%d] UDP Socket for SNMP Agent, port [%d]\r\n", SOCK_SNMP_AGENT, PORT_SNMP_AGENT);
#endif
			break;

		default :
			break;
	}


	return 1;
}
*/

//		HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
//		printf("RTC_Update_NOW, Time set!\r\n");	
//		HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
	
	
	//  if(setTimeNow == 1)
//	{
//		setTimeNow = 0;
//		RTC_Update();
//	}
typedef struct{

  uint8_t work_mode;
	uint8_t rs458_connection;
	uint8_t ntp_client_status;
	uint8_t ethernet_connection;
	uint8_t rtc_status; 
	uint8_t sync_status; 
} slave_status;
extern slave_status slave_clock;

/*
Thi thoang LED treo ko ro ly do, reset neu nghi ngo
*/	
void resetInitLED(void)
{
	#ifdef SLAVE_MATRIX
	up7_matrix_init();
	line2_matrix_init();
#endif
#ifndef SLAVE_MATRIX	
	display_init_check();
#endif	
}

void ntp_client_run(void)
{
	/* dong bo NTP
		Chu ky dong bo : synchronizePeriod
		synchronized_ntp = 0 => tien hanh dong bo
*/
		
//		if(synchronized_ntp != 0)
//		{
//			return;
//		}
		
		SNTP_run();
		
		
}


				//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
				//HAL_UART_Transmit(&huart2, aRxBuffer, 20, 100);
				//main_message_handle();
				
				//saved = 1;
				//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
				//printf("aRxBuffer %s; \r\n",aRxBuffer);
//Xu ly ban tin tu mach main
void main_message_handle(void)
{//=> Ban tin GPS: $GPS034007060819AA10	;$$GPS091259280422AA10
	if((aRxBuffer[0] =='$')&((aRxBuffer[1] =='G')|(aRxBuffer[1] =='g'))&((aRxBuffer[2] =='P')|(aRxBuffer[2] =='p'))&((aRxBuffer[3] =='S')|(aRxBuffer[3] =='s')))
	{
		
		#ifndef SLAVE_MATRIX	
		MAX7219_SendAddrDat(0x07,0x00);//Tat het den => dan dc ban tin
		#endif	
		#ifdef SLAVE_MATRIX
		 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2|PC3_Pin, GPIO_PIN_SET);//Tat het den => dan dc ban tin
		#endif
		
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
//		currtime.tm_year = 100+ years;//100+10*convert_atoi(aRxBuffer[14])+convert_atoi(aRxBuffer[15]);//In fact: 2000+xxx-1900
//		currtime.tm_mon  = months-1;//10*convert_atoi(aRxBuffer[12])+convert_atoi(aRxBuffer[13])-1;
//		currtime.tm_mday = days;//10*convert_atoi(aRxBuffer[10])+convert_atoi(aRxBuffer[11]);
//		
//		currtime.tm_sec  = seconds;//10*convert_atoi(aRxBuffer[8])+convert_atoi(aRxBuffer[9]);
//		currtime.tm_min  = minutes;//10*convert_atoi(aRxBuffer[6])+convert_atoi(aRxBuffer[7]);
//		currtime.tm_hour = hours;//10*convert_atoi(aRxBuffer[4])+convert_atoi(aRxBuffer[5]);
//		
//		
		
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
		
		
		
	}
	else control();
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
			//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
			//printf("Linh tinh\r\n");
			//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
		}
	}
	
	
}

if((aRxBuffer[16]=='V') && (aRxBuffer[17]=='V') )	
	

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

/*
	t_days = timeinfo->tm_mday;
	t_months = 1+timeinfo->tm_mon;
	t_years = timeinfo->tm_year-100;
	t_hours = timeinfo->tm_hour;
	t_minutes = timeinfo->tm_min;
			*/
//	#ifdef NTP_CLIENT_DEBUG
//	HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
//	printf("faster clock! Loop delay %d, avg offset :%d,TIM4 %d\r\n",loop_delay,avg_offset_stable,TIM4->CNT);	
//	HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
//	#endif

int8_t SNTP_run_backUp(void)
{
	uint32_t ret;
	uint16_t size;
	uint32_t destip = 0;
	uint16_t destport;
	int32_t offset;
	switch(getSn_SR(SOCK_SNTP))
	{
	case SOCK_UDP:
		if ((size = getSn_RX_RSR(SOCK_SNTP)) > 0)
		{
			//loop_delay = fractionOfSecond;
			loop_delay = counterOf10Second;
			slave_clock.sync_status = GREEN;
			//if (size > 56) size = 56;	// if Rx data size is lager than TX_RX_MAX_BUF_SIZE
			recvfrom(SOCK_SNTP, ntpTimeServer_buf, 56, (uint8_t *)&destip, &destport);
			
			sec = (ntpTimeServer_buf[40]<<24) + (ntpTimeServer_buf[41]<<16) + (ntpTimeServer_buf[42]<<8) + ntpTimeServer_buf[43] ;
			//timenow = sec - seventyYears;
			Server_time = sec - seventyYears;
			//Phan le cua giay nhan dc
			micros_transmit = (ntpTimeServer_buf[44]<<24) + (ntpTimeServer_buf[45]<<16) + (ntpTimeServer_buf[46]<<8) + ntpTimeServer_buf[47] ;
			sec_frac = micros_transmit / usShift;
			sec_frac = sec_frac - 1;
			sec_frac = sec_frac / 100;

			
//			if( (Server_time - timenow >1) || (timenow >Server_time))
//			{
//				 printf("Khac %d s\r\n",Server_time - timenow);
//				//timenow = Server_time;
//				//sycn_RTC_Now();
//			}
//			else if(Server_time != timenow) 

				//printf("Khac %d s\r\n",Server_time - timenow);
			
				//printf("Server frac %d, my frac: %d\r\n",(int)sec_frac,fractionOfSecond);
				//tre truyen + nhan ban tin
				//printf("NTP received size:%d Delta %d\r\n",size,loop_delay);
			//printf("Loop delay %d\r\n",loop_delay);
			currtime.tm_year = 100+ years;
			currtime.tm_mon  = months-1;
			currtime.tm_mday = days;
			currtime.tm_sec  = seconds;
			currtime.tm_min  = minutes;
			currtime.tm_hour = hours;
			timenow = mktime(&currtime);
			//printf("timenow: %d\r\n",timenow);
			
			offset = 10000*(Server_time - timenow) + (int)sec_frac + (loop_delay)/2 - fractionOfSecond;
			//printf("Tre thoi gian giua server - device %d\r\n",offset);

			if(((offset>0)&&(offset < 1000)) || ((offset<0)&&((0-offset)<1000)))
			{
				printf("Loop delay %d, offset :%d\r\n",loop_delay,offset);
				slave_clock.sync_status = GREEN;
				return 0;
			}
			else if(timenow > Server_time) 
			{
				sycn_RTC_Now(Server_time);
				slave_clock.sync_status = GREEN;
				return 0;
			}
			else
			{
				timeinfo = localtime( &timenow );
				asctime(timeinfo);
				days = timeinfo->tm_mday;
				months = 1+timeinfo->tm_mon;
				years = timeinfo->tm_year-100;
				hours = timeinfo->tm_hour;
				minutes = timeinfo->tm_min;
				seconds = timeinfo->tm_sec + offset/10000 + 1;
				if(seconds > 59) return 0;
				setTimeNow = 1;
				fractionOfSecond = fractionOfSecond + offset%10000;
			}
			slave_clock.sync_status = GREEN;
			printf("2. Loop delay %d, offset :%d\r\n",loop_delay,offset);	
				
				
			
			//close(SOCK_SNTP);
			//printf("Closed SOCK NTP\r\n");
			//synchronizePeriod = 16;
			//synchronized_ntp = 1;
			//NTP_alreadySent  = 0;
			return 1;
		}
		
		if(	NTP_alreadySent == 0)
			{
				sendto(SOCK_SNTP,ntpmessage,48,ntpTimeServer_ip,123);
				//tSend = fractionOfSecond;
				slave_clock.sync_status = RED;
				counterOf10Second = 0;
				//printf("NTP sent\r\n");	
				NTP_alreadySent = 1;
				NTP_alreadySentTimeOut = 9;
				tSend = fractionOfSecond;
				return 0;
			}
			
			
			
		
		break;
	case SOCK_CLOSED:
		if((ret=socket(SOCK_SNTP,Sn_MR_UDP,sntp_port,0x00)) != SOCK_SNTP) return ret;
		printf(" Socket[%d] UDP Socket for SNTP client started at port [%d]\r\n", SOCK_SNTP, sntp_port);
		break;
	}
	// Return value
	// 0 - failed / 1 - success
	return 0;
}

//void wzn_event_handle(void)
//{
//	uint16_t ir = 0;
//	uint8_t sir = 0;
//	
//	
//	if (ctlwizchip(CW_GET_INTERRUPT, &ir) == -1) {
//		printf("Cannot get ir...");
//		return;
//	}
//	//set ngat voi tung socket!!!
//	if (ir & IK_SOCK_2) 
//		{
//				sir = getSn_IR(SOCK_SNTP);
//				
//				if ((sir & Sn_IR_RECV) > 0) {
//					/* Clear Sn_IR_RECV flag. */
//					setSn_IR(SOCK_SNTP, Sn_IR_RECV);
//					
//					return;
//				}
//				else if ((sir & Sn_IR_SENDOK) > 0) {
//					/* Clear Sn_IR_SENDOK flag. */
//					setSn_IR(SOCK_SNTP, Sn_IR_SENDOK);					
//				}
//		}
//}
#define fullconfigpageold			"<!DOCTYPE html>"\
"<html>"\
"<body><CENTER>"\
"<h3>IP config</h3>"\
"<FORM METHOD='POST' action='config.cgi'><br>"\
"<input type=\"text\" name=\"ip\" value=\"192.168.1.39\"> DeviveIP<br>"\
"<input type=\"text\" name=\"gw\" value=\"0.0.0.0\"> Getway<br>"\
"<input type=\"text\" name=\"sn\" value=\"255.255.255.0\"> Subnet <br>"\
"<input type=\"submit\" value=\"SAVE\">"\
"</form>"\
"</CENTER></body>"\
"</html>"\

#define configpage			"<!DOCTYPE html>"\
"<html>"\
	"<head>"\
	"<title>GPS Master clock configuration</title>"\
	"<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>"\
		"<style>"\
			"body{font-family: Arial, Tahoma;font-size: 14px;}"\
			"#main{width: 500px;border: 1px solid #6495ED;padding: 0;margin-left: auto;margin-right: auto;}"\
			"#head{border: 1px solid #CDCDCD;height: 70px;background-color:#008800;color:white;overflow: hidden;font-weight: bold;font-size:30px;text-align:center;margin-bottom:5px;}"\
			"#foot{background-color: #0099FF;border:1px solid #ccc;color:black;font-size:15px;height: 20px;overflow: hidden;padding-top: 5px;padding-bottom: 5px;text-align:center;}"\
 			"h1{color:black;font-size:18px;text-align:center;}"\
			"#net_info_general{min-height: 150px;border: 1px solid #CDCDCD;float:left;margin-bottom:5px;background-color: #F5F5DC;}"\
			"#list_head{background-color:#FFCC99;height: 40px;width: 100%;display: inline-block;font-size: 18px;}"\
			".tab{font-family:Arial; width: 498px; align : center; border: 0px; cellpadding:5px; cellspacing: 0px; height:50px;}"\
		"</style>"\
	"</head>"\
	"<body>"\
		"<div id='main'>"\
			"<div id='head'>GPS MASTER CLOCK CONFIGURATION</div>"\
			"<div id='net_info_general'>"\
				"<div id ='list_head'>"\
					"<h1>Network information</h1>"\
				"</div>"\
				"<div>"\
					"<FORM METHOD='POST' action='config.cgi'>"\
						"<table type ='table' class='tab'>"\
							"<tbody>"\
								"<tr>"\
									"<td>IP adress:&nbsp</td>"\
									"<td colspan='2'><input id='txtip' name='ip' type='text' value='192.168.1.39'/></td>"\
								"</tr>"\
								"<tr>"\
									"<td>Subnet mask:&nbsp</td>"\
									"<td colspan='2'><input id='txtsn' name='sn' type='text' value='255.255.255.0'/></td>"\
								"</tr>"\
								"<tr>"\
									"<td></td>"\
									"<td colspan='2'>"\
										"<input type='submit' class ='btn' name='bt_setup' value='Save' id='bt_setup'>"\
									"</td>"\
								"</tr>"\
							"</tbody>"\
						"</table>"\
					"</FORM>"\
				"</div>"\
			"</div>"\
			"<div id = 'foot'>"\
				"Powered by <a style='color: #000000; font-weight: bold; text-decoration: none'>Air Traffic Technical Company - <span style='color:red'>A</span><span style='color: blue;'>TTECH</span></a>"\
			"</div>"\
		"</div>"\
	"</body>"\
"</html>"

#ifdef unknown

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void make_json_serial_data(uint8_t * buf, uint16_t * len)
{
	uint8_t * buf_ptr;
	//buf_ptr = getUSART1buf();
	*len = sprintf((char *)buf,"getRs232DataCallback({\"data\":\"%s\"});", buf_ptr); // serial buffer

	// Serial data buffer clear
	//USART1_flush();
}


/************************************************************************************************/
uint8_t predefined_get_cgi_processor(uint8_t * uri_name, uint8_t * buf, uint16_t * len)
{
		uint8_t ret = 1;	// ret = 1 means 'uri_name' matched
	uint8_t cgibuf[14] = {0, };
	int8_t cgi_dio = -1;
	int8_t cgi_ain = -1;

	uint8_t i;

	if(strcmp((const char *)uri_name, "todo.cgi") == 0)
	{
		// to do
		;//make_json_todo(buf, len);
	}
	else if(strcmp((const char *)uri_name, "get_netinfo.cgi") == 0)
	{
		make_json_netinfo2(buf, len);
	}
	else
	{
		// get_dio0.cgi ~ get_dio15.cgi
		for(i = 0; i < DIOn; i++)
		{
			memset(cgibuf, 0x00, 14);
			sprintf((char *)cgibuf, "get_dio%d.cgi", i);
			if(strcmp((const char *)uri_name, (const char *)cgibuf) == 0)
			{
				make_json_dio(buf, len, i);
				cgi_dio = i;
				break;
			}
		}

		if(cgi_dio < 0)
		{
			// get_ain0.cgi ~ get_ain5.cgi (A0 - A5), get_ain6.cgi for on-board potentiometer / Temp.Sensor
			for(i = 0; i < AINn; i++)
			{
				memset(cgibuf, 0x00, 14);
				sprintf((char *)cgibuf, "get_ain%d.cgi", i);
				if(strcmp((const char *)uri_name, (const char *)cgibuf) == 0)
				{
					make_json_ain(buf, len, i);
					cgi_ain = i;
					break;
				}
			}
		}

		if((cgi_dio < 0) && (cgi_ain < 0)) ret = 0;
	}

	return ret;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pre-defined Get CGI functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void make_json_dio(uint8_t * buf, uint16_t * len, uint8_t pin)
{
	uint8_t pin_state 	= 1;//Chip_GPIO_GetPinState(LPC_GPIO, dio_ports[pin], dio_pins[pin]);
	uint8_t pin_dir 	= 2;//Chip_GPIO_GetPinDIR(LPC_GPIO, dio_ports[pin], dio_pins[pin]);

	*len = sprintf((char *)buf, "DioCallback({\"dio_p\":\"%d\",\
											\"dio_s\":\"%d\",\
											\"dio_d\":\"%d\"\
											});",
											pin,					// Digital io pin number
											pin_state,				// Digital io status
											pin_dir					// Digital io directions
											);
}

void make_json_ain(uint8_t * buf, uint16_t * len, uint8_t pin)
{
	*len = sprintf((char *)buf, "AinCallback({\"ain_p\":\"%d\",\
											\"ain_v\":\"%d\"\
											});",
											pin,					// ADC input pin number
											6		// get_ADC_val(pin), ADC input value
											);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pre-defined Set CGI functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int8_t set_diodir(uint8_t * uri)
{
	uint8_t * param;
	uint8_t pin = 0, val = 0;

	//printf("uri :%s\r\n",uri);
	
	if((param = get_http_param_value((char *)uri, "pin"))) // GPIO; D0 ~ D15
	{
		pin = (uint8_t)ATOI(param, 10);
		if(pin > 15) return -1;

		if((param = get_http_param_value((char *)uri, "val")))  // Direction; NotUsed/Input/Output
		{
			val = (uint8_t)ATOI(param, 10);
			if(val > Output) val = Output;
		}
	}

	if(val == Input) 		;//Chip_GPIO_SetPinDIRInput(LPC_GPIO, dio_ports[pin], dio_pins[pin]);	// Input
	else 					;//Chip_GPIO_SetPinDIROutput(LPC_GPIO, dio_ports[pin], dio_pins[pin]); // Output

	return pin;
}



int8_t set_diostate(uint8_t * uri)
{
	uint8_t * param;
	uint8_t pin = 0, val = 0;

	//printf("uri :%s\r\n",uri);
	//Toi day trong uri van chua nguyen ban tin
	if((param = get_http_param_value((char *)uri, "pin"))) // GPIO; D0 ~ D15
	{
		pin = (uint8_t)ATOI(param, 10);
		if(pin > 15) return -1;

		if((param = get_http_param_value((char *)uri, "val")))  // State; high(on)/low(off)
		{
			val = (uint8_t)ATOI(param, 10);
			if(val > On) val = On;
		}

		if(val == On) 		;//Chip_GPIO_SetPinState(LPC_GPIO, dio_ports[pin], dio_pins[pin], true); 	// High
		else				;//Chip_GPIO_SetPinState(LPC_GPIO, dio_ports[pin], dio_pins[pin], false);	// Low
	}

	return pin;
}
#endif
//	}
//	else
//	{
//		*len = sprintf((char *)buf, "{\"mac\":\"%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\",\"txtip\":\"%d.%d.%d.%d\",\"gw\":\"%d.%d.%d.%d\",\"txtsn\":\"%d.%d.%d.%d\",\"dns\":\"%d.%d.%d.%d\",\"dhcp\":\"%d\",\"txtdays\":\"%d\",\"txtmonths\":\"%d\",\"txtyears\":\"%d\",\"txthours\":\"%d\",\"txtminutes\":\"%d\",\"txtseconds\":\"%d\",\"txtgps01\":\"%s\",\"txtgps02\":\"%s\",\"txtpower01\":\"%s\",\"txtpower02\":\"%s\"}",
//											netinfo.mac[0], netinfo.mac[1], netinfo.mac[2], netinfo.mac[3], netinfo.mac[4], netinfo.mac[5],
//											netinfo.ip[0], netinfo.ip[1], netinfo.ip[2], netinfo.ip[3],
//											netinfo.gw[0], netinfo.gw[1], netinfo.gw[2], netinfo.gw[3],
//											netinfo.sn[0], netinfo.sn[1], netinfo.sn[2], netinfo.sn[3],
//											netinfo.dns[0], netinfo.dns[1], netinfo.dns[2], netinfo.dns[3],
//											netinfo.dhcp,
//											days,
//											months,
//											years,
//											hours,
//											minutes,
//											seconds,
//											gps1_stt?"ON":"OFF",
//											gps2_stt?"ON":"OFF",
//											power1_stt?"ON":"OFF",
//											power2_stt?"ON":"OFF"
//											);
//	}		

	// DHCP: 1 - Static, 2 - DHCP
//	if(lostSignal == LOST_GPS_MASTER)
//	{

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t predefined_set_cgi_processor(uint8_t * uri_name, uint8_t * uri, uint8_t * buf, uint16_t * len)
{
	uint8_t ret = 1;	// ret = '1' means 'uri_name' matched
//	uint8_t val = 0;

//	if(strcmp((const char *)uri_name, "set_diodir.cgi") == 0)
//	{
//		//val = set_diodir(uri);
//		*len = sprintf((char *)buf, "%d", val);
//	}
////	else if(strcmp((const char *)uri_name, "set_diostate.cgi") == 0)
////	{
////		//val = set_diostate(uri);
////		*len = sprintf((char *)buf, "%d", val);
////	}
//	else
//	{
//		ret = 0;
//		//printf("predefined_set_cgi_processor not found\r\n");
//	}

	return ret;
}

//Sai khac qua 1s
			if(Server_time > timenow)
			{
				if((Server_time - timenow) > 1)
				{
					

					printf("Offset BIG %d s\r\n",Server_time - timenow);

					sycn_RTC_Now(Server_time);
					return 1;
				}
			}
			else if(Server_time < timenow)
			{
				if((timenow - Server_time) > 1)
				{

					printf("Offset BIG %d s\r\n",timenow- Server_time);

					sycn_RTC_Now(Server_time);
					return 1;
				}
			}
			
			
//Neu RTC cham qua
uint8_t sycn_RTC_slow_old(time_t x)
{
			timeinfo = localtime( &x );
			asctime(timeinfo);
	
	    seconds = timeinfo->tm_sec + 1;
			if(seconds > 56) return 0;
	
			days = timeinfo->tm_mday;
			months = 1+timeinfo->tm_mon;
			years = timeinfo->tm_year-100;
			hours = timeinfo->tm_hour;
			minutes = timeinfo->tm_min;
			
			RTC_Update();
	    //printf("Save low RTC!\r\n");
	    
			return 1;
}


//Neu RTC nhanh qua
uint8_t sycn_RTC_fast_new(time_t x)
{
			timeinfo = localtime( &x );
			asctime(timeinfo);
	
	    seconds = timeinfo->tm_sec;
			if(seconds > 56) return 0;
	
			days = timeinfo->tm_mday;
			months = 1+timeinfo->tm_mon;
			years = timeinfo->tm_year-100;
			hours = timeinfo->tm_hour;
			minutes = timeinfo->tm_min;
			
			RTC_Update();
	    
	    
			return 1;
}


if((offset >= lastOffset) && ((offset - lastOffset) <= 999))
			{
				//printf("offset > lastOffset %d\r\n",offset- lastOffset);
				TimeServerStable++;
				avg_offset = avg_offset + offset;
			}
			else if((offset < lastOffset) && ((lastOffset - offset) <= 999))
			{
				//printf("offset < lastOffset %d\r\n",lastOffset - offset);
				TimeServerStable++;
				avg_offset = avg_offset + offset;
			}
			
			
			
//Do sai lech offset/last offset < 100ms => tin hieu co ve on dinh => dong bo
			if( ((offset >= lastOffset) && ((offset - lastOffset) <= 999))  || ((offset < lastOffset) && ((lastOffset - offset) <= 999)) )
			{
				//printf("offset > lastOffset %d\r\n",offset- lastOffset);
				TimeServerStable++;
				avg_offset = avg_offset + offset;
			}
			
			
			//Do sai lech Time Server > 100ms => tin hieu co ve vo van, tu tu roi tinh!!!
			if((offset >= lastOffset) && ((offset - lastOffset) > 999))
			{
				#ifdef NTP_CLIENT_DEBUG
				
				printf("Unstable offset > lastOffset %d\r\n",offset- lastOffset);
				
				#endif
				TimeServerStable = 0;
				avg_offset = 0;
			}
			else if((offset < lastOffset) && ((lastOffset - offset) > 999))
			{
				#ifdef NTP_CLIENT_DEBUG
				
				printf("Unstable offset < lastOffset %d\r\n",lastOffset - offset);
				
				#endif
				TimeServerStable = 0;
				avg_offset = 0;
			}
			
			
			
			
			//Bat dau chinh gio
			if(avg_offset_stable>=0)// Dong ho bi cham
			{
				if(avg_offset_stable < 999) //Sai lech duoi 99.9ms
					{
						#ifdef NTP_CLIENT_DEBUG
						printf("Time server on dinh! AVG: %d\r\n",avg_offset_stable);
				#endif
						return 1;// gio chuan ko phai chinh
					}
					
				else 
					{
						sycn_RTC_slow(Server_time);
						printf("Slower clock! Loop delay %d, avg offset :%d\r\n",loop_delay,avg_offset_stable);	

					}
			}
			else if(avg_offset_stable<0)//Dong ho nhanh
			{
				if(avg_offset_stable > -999) //Sai lech duoi 99.9ms
					{
						printf("Time server on dinh! AVG: %d\r\n",avg_offset_stable);
						return 1;// gio chuan ko phai chinh	
					}
					
				else 
				{
					sycn_RTC_fast(Server_time);
					

					printf("faster clock! Loop delay %d, avg offset :%d\r\n",loop_delay,avg_offset_stable);	

				}
			}
			
			
			
			ntpmessage[0] = 0xE3;   // LI, Version, Mode
	  ntpmessage[1] = 0;     // Stratum, or type of clock
	  ntpmessage[2] = 6;     // Polling Interval
	  ntpmessage[3] = 0;  // Peer Clock Precision
	  // 8 bytes of zero for Root Delay & Root Dispersion
	  ntpmessage[12]  = 49;
	  ntpmessage[13]  = 0x4E;
	  ntpmessage[14]  = 49;
	  ntpmessage[15]  = 52;
			
			
void RTC_Update_NOW(void)
	{
		ghids(14,0);//1HZ out SQW
		ghids(DS_SECOND_REG,	t_seconds);
		ghids(DS_MIN_REG,			t_minutes);
		ghids(DS_HOUR_REG,		t_hours);
		//ghids(DS_DAY_REG,2);
		ghids(DS_DATE_REG,		t_days);
		ghids(DS_MONTH_REG,		t_months);
		ghids(DS_YEAR_REG,		t_years);
		
	
		laythoigian();
		
//		currtime.tm_year = 100+ years;
//		currtime.tm_mon  = months-1;
//		currtime.tm_mday = days;
//		currtime.tm_sec  = seconds;
//		currtime.tm_min  = minutes;
//		currtime.tm_hour = hours;
//		timenow = mktime(&currtime);
		
		//printf("Time set!!!!\r\n");
	}	


//		if(testLinhTinh == 5)
//		{
//			
//			if(fractionOfSecond > 300	)
//			{
//				testLinhTinh = 0; // Moi giay kiem tra 1 lan 
//				if(slave_clock.rs458_connection == CONNECTED)
//				{
//					//Neu thoi gian khac nhau !
//					if(server_second != seconds) 
//					{
//						//time2SaveRTC = 6;
//						#ifdef OverTheAir
//						send_debug_message = 1;
//						mysize = sprintf(udp_message,"Sai gio %d/local %d, frac %d, tx %d\r\n",server_second,seconds,fractionOfSecond,tx);
//						#endif
//						
//					}
//				}
//					
//				
//			}
//		}	