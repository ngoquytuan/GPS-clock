#include <time.h>
#include "socketdefines.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include "socket.h"

extern uint8_t  synchronized_ntp;
extern uint16_t synchronizePeriod;
uint8_t NTP_alreadySent =0;
uint8_t NTP_alreadySentTimeOut = 10;
extern volatile uint8_t waitForSetTime;
uint8_t NTP_busy =0;

#define fractionOfSecond TIM1->CNT
time_t t0,t0Frac,t3Frac,t3;
time_t round_trip_delay;
time_t newfractionOfSecond;
/* Shifts usecs in unixToNtpTime */
//#ifndef USECSHIFT
//#define USECSHIFT (1LL << 32) * 1.0e-6
//#endif
#define usShift 4294.967296;
//Number of seconds from 1st January 1900 to start of Unix epoch
//According to the Time protocol in RFC 868 it is 2208988800L.
#define STARTOFTIME 2208988800UL



#define ntpClientDebug
#define	MAX_SNTP_BUF_SIZE	sizeof(ntpformat)		///< maximum size of DNS buffer. */


#define sntp_port 1234 //my sntp port, cho de gui ban ti ve
#define ntp_port		123                     //ntp server port number
#define SECS_PERDAY		86400UL             	// seconds in a day = 60*60*24
#define UTC_ADJ_HRS		0              	        // Vietnam : GMT+7
#define EPOCH			1900                    // NTP start year

void SNTP_init(void);
int8_t SNTP_run(void);

extern time_t timenow;
extern struct tm* timeinfo;
extern uint8_t ntpTimeServer_ip[4];

uint8_t TimeIsSet = 0;
uint16_t RetrySend = 0; //60 giay
uint16_t sycnPeriod = 0;// 1 gio
//uint8_t Domain_ntpTimeServer[] = "0.asia.pool.ntp.org";    // for Example domain name
//uint8_t Domain_ntpTimeServer[] = "time.windows.com";
//uint8_t Domain_IP[4]  = {0, };               // Translated IP address by DNS
//#define DNS_BUF_SIZE   200
uint8_t ntpTimeServer_buf[60];

//uint8_t ntpTimeServer_ip[4] ={103, 123, 108, 222};// NTP time server

//uint8_t ntpTimeServer_ip[4] ={192, 168, 1, 7};// NTP time server
//uint8_t ntpTimeServer_buf[56];
uint8_t ntpmessage[48]={0};
//uint8_t ntpServerRespond[56];
// Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
const uint32_t seventyYears = 2208988800;

time_t micros_transmit;
double sec_frac;
uint32_t sec;

void processNTPmeassage(void)
{
	uint16_t size;
	uint32_t destip = 0;
	uint16_t destport;
	
	if ((size = getSn_RX_RSR(SOCK_SNTP)) > 0)
		{
			printf("NTP received size:%d \r\n",size);
			//if (size > 56) size = 56;	// if Rx data size is lager than TX_RX_MAX_BUF_SIZE
			recvfrom(SOCK_SNTP, ntpTimeServer_buf, 56, (uint8_t *)&destip, &destport);
			
			close(SOCK_SNTP);
			//printf("Closed SOCK NTP\r\n");
			synchronizePeriod = 6;
			synchronized_ntp = 1;
			NTP_alreadySent  = 0;
		}
		else printf("NTP error???\r\n");
	//synchronized_ntp = 1;
}

void wzn_event_handle(void)
{
	uint16_t ir = 0;
	uint8_t sir = 0;
	
	
	if (ctlwizchip(CW_GET_INTERRUPT, &ir) == -1) {
		printf("Cannot get ir...");
		return;
	}
	//set ngat voi tung socket!!!
	if (ir & IK_SOCK_2) 
		{
				sir = getSn_IR(SOCK_SNTP);
				
				if ((sir & Sn_IR_RECV) > 0) {
					/* Clear Sn_IR_RECV flag. */
					setSn_IR(SOCK_SNTP, Sn_IR_RECV);
					
					return;
				}
				else if ((sir & Sn_IR_SENDOK) > 0) {
					/* Clear Sn_IR_SENDOK flag. */
					setSn_IR(SOCK_SNTP, Sn_IR_SENDOK);					
				}
		}
}

void SNTP_init(void)
{
	  // Initialize values needed to form NTP request
	  // (see URL above for details on the packets)
	  ntpmessage[0] = 0xE3;   // LI, Version, Mode
	  ntpmessage[1] = 0;     // Stratum, or type of clock
	  ntpmessage[2] = 6;     // Polling Interval
	  ntpmessage[3] = 0xEC;  // Peer Clock Precision
	  // 8 bytes of zero for Root Delay & Root Dispersion
	  ntpmessage[12]  = 49;
	  ntpmessage[13]  = 0x4E;
	  ntpmessage[14]  = 49;
	  ntpmessage[15]  = 52;
		
}




int8_t procesingNTPmessage(void)
{
				
			t3Frac = fractionOfSecond;
			t3 = timenow;
			round_trip_delay = t3 -t0;
			round_trip_delay = round_trip_delay*10000 +t3Frac -t0Frac;
			//printf("Round-trip delay %d ms\r\n",round_trip_delay/10);
			
			//Mot ban tin tu NTP Time Server
			//24 3 6 e8 0 0 2c 3c 0 0 e 7d 8e 93 5c 5 e1 6 3a 76 77 3a 48 cf 0 0 0 0 0 0 0 0 e1 6 3e 9d 25 4f 82 99 e1 6 3e 9d 25 52 19 13
//			for(i=0;i<48;i++)
//						{
//						   printf("%x ",*(ntpTimeServer_buf+i));
//						}
			sec = (ntpTimeServer_buf[40]<<24) + (ntpTimeServer_buf[41]<<16) + (ntpTimeServer_buf[42]<<8) + ntpTimeServer_buf[43] ;
			
			
			
			//micros_transmit = 100*1234;
			micros_transmit = (ntpTimeServer_buf[44]<<24) + (ntpTimeServer_buf[45]<<16) + (ntpTimeServer_buf[46]<<8) + ntpTimeServer_buf[47] ;

			sec_frac = micros_transmit / usShift;
			sec_frac = sec_frac - 1;
			sec_frac = sec_frac / 100;

			//printf("micros_transmit: %u ; sec_frac %f ms\r\n",micros_transmit,sec_frac/10);
			newfractionOfSecond = sec_frac + 1 + (round_trip_delay/2);
			//Neu dung diem chuyen giao cua giay thi bo qua => qua mat giay moi roi
			if(newfractionOfSecond > 9999) return 0;
			
			fractionOfSecond = newfractionOfSecond;
			//printf("New sec_frac %d \r\n",newfractionOfSecond);
			
			timenow = sec - seventyYears;
			
			//printf("\r\nSynced with ntp server, seconds: %u\r\n",timenow);
			timeinfo = localtime( &timenow );
			
			
			asctime(timeinfo);
			days = timeinfo->tm_mday;
			months = 1+timeinfo->tm_mon;
			years = timeinfo->tm_year-100;
			hours = timeinfo->tm_hour;
			minutes = timeinfo->tm_min;
			seconds = timeinfo->tm_sec;
			//Bo qua tranh loi
			if(seconds > 58) return 0;
			
			if(newfractionOfSecond > 9963)//Sang giay moi roi....
			{
				//Thoi gian con it qua, neu luu co the bi tre
				//seconds++;
				//RTC_Update();
				//printf("Time set NTP");
				//Bo qua...
				return 0;
			}
			else {waitForSetTime = 1;}
			
			#ifdef			ntpClientDebug
			//printf("Current local time and date: %s\r\n", asctime(timeinfo));
		  //printf("HH-MM-SS :%d-%d-%d\r\n",hours,minutes,seconds);
		  //printf("DD-MM-YY :%d-%d-%d\r\n",days,months,years);
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
			
			TimeIsSet = 1;
			return 1;
}
void sendNTPpacket(void)
{
	sendto(SOCK_SNTP,ntpmessage,48,ntpTimeServer_ip,123);
	NTP_busy = 1;				
					t0 = timenow;
					t0Frac = fractionOfSecond;
					
#ifdef			ntpClientDebug		
					printf("NTP ask...\r\n");					
//					printf("Gui ban tin di :");//35 0 6 236 0 0 0 0 0 0 0 0 49 78 49 52 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
//					for(i=0;i<48;i++)
//					{
//						printf("%d ",*(ntpmessage+i));
//					
//					}
//					printf("\r\n");
#endif			
}

int8_t SNTP_run2(void)
{
	uint32_t ret;
	uint16_t size;
	uint32_t destip = 0;
	uint16_t destport;
	
	switch(getSn_SR(SOCK_SNTP))
	{
	case SOCK_UDP:
		if ((size = getSn_RX_RSR(SOCK_SNTP)) > 0)
		{
			printf("NTP received size:%d \r\n",size);
			//if (size > 56) size = 56;	// if Rx data size is lager than TX_RX_MAX_BUF_SIZE
			recvfrom(SOCK_SNTP, ntpTimeServer_buf, 56, (uint8_t *)&destip, &destport);
			//processNTPmeassage();
			close(SOCK_SNTP);
			//printf("Closed SOCK NTP\r\n");
			synchronizePeriod = 6;
			synchronized_ntp = 1;
			NTP_alreadySent  = 0;
			return 1;
		}
		
		if(	NTP_alreadySent == 0)
			{
				sendto(SOCK_SNTP,ntpmessage,48,ntpTimeServer_ip,123);
				printf("NTP sent\r\n");	
				NTP_alreadySent = 1;
				NTP_alreadySentTimeOut = 6;
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
/**
Tinh toan phan le cua giay( fraction Of Second) : https://en.wikipedia.org/wiki/Network_Time_Protocol
- Coi t1 = t2 : thoi gian xu ly cua server rat nhanh va sap xi 0s
- Coi thoi gian truyen Client to Server va Server ve Client la bang nhau
=> tre duong truyen round-trip delay / 2
#ifdef			ntpClientDebug
			#endif
*/
int8_t SNTP_run(void)//datetime sntp;
{
	uint32_t ret;
	uint16_t size;
	uint32_t destip = 0;
	uint16_t destport;
	//uint16_t startindex = 40; //last 8-byte of data_buf[size is 48 byte] is xmt, so the startindex should be 40
	int8_t i;
	
	if (sycnPeriod >= 61) // dong bo lai thoi gian
	{
		//printf("TimeIsSet = 0\r\n");
		TimeIsSet = 0;
		sycnPeriod = 0;
		RetrySend = 19;
	}
	if(TimeIsSet == 1) return 1;
	
	switch(getSn_SR(SOCK_SNTP))
	{
	case SOCK_UDP:
		if ((size = getSn_RX_RSR(SOCK_SNTP)) > 0)
		{
			//printf("\r\nsize:%d, ret:%d, NTP: ",size,ret);
			//if (size > 56) size = 56;	// if Rx data size is lager than TX_RX_MAX_BUF_SIZE
			recvfrom(SOCK_SNTP, ntpTimeServer_buf, 56, (uint8_t *)&destip, &destport);
			if(procesingNTPmessage() == 0) 
			{
				close(SOCK_SNTP);
				//printf("Closed SOCK NTP1");
				NTP_busy = 0;
				return 0;
			}
			
			NTP_busy = 0;
			close(SOCK_SNTP);
			//printf("Closed SOCK NTP\r\n");
			return 1;
		}
		
		if(NTP_busy ==1)
		{
			if(timenow -t0 > 10)
			{
				printf("ko nhan dc ban tin NTP\r\n");
				NTP_busy = 0;
				return 0;
			}
		}
				if(TimeIsSet == 0) //chua chinh gio
			{
				if(RetrySend > 19) //Try Again gui ban tin hoi gio
				{
					RetrySend = 0;
					sendNTPpacket();		
				}
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




