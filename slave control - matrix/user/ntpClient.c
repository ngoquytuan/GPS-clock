#include <time.h>
#include "socketdefines.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include "socket.h"

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

//#ifndef UTIL_H
//#define UTIL_H

//#define htons(x) ( ((x)<< 8 & 0xFF00) | \
//                   ((x)>> 8 & 0x00FF) )
//#define ntohs(x) htons(x)

//#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
//                   ((x)<< 8 & 0x00FF0000UL) | \
//                   ((x)>> 8 & 0x0000FF00UL) | \
//                   ((x)>>24 & 0x000000FFUL) )
//#define ntohl(x) htonl(x)
//#endif


#define ntpClientDebug
#define	MAX_SNTP_BUF_SIZE	sizeof(ntpformat)		///< maximum size of DNS buffer. */
uint32_t countOfNTPrequest = 0;	

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



int8_t SNTP_run(void)//datetime sntp;
{
	uint32_t ret;
	uint16_t size;
	uint32_t destip = 0;
	uint16_t destport;
	//uint16_t startindex = 40; //last 8-byte of data_buf[size is 48 byte] is xmt, so the startindex should be 40
	int8_t i;
	uint32_t sec;
	if (sycnPeriod >= 320) // dong bo lai thoi gian
	{
		TimeIsSet = 0;
		sycnPeriod = 0;
		RetrySend = 0;
	}
	if(TimeIsSet == 1) return 1;
	
	switch(getSn_SR(SOCK_SNTP))
	{
	case SOCK_UDP:
		if ((size = getSn_RX_RSR(SOCK_SNTP)) > 0)
		{
#ifdef			ntpClientDebug
			printf("\r\nsize:%d, ret:%d, NTP: ",size,ret);
#endif
			if (size > 56) size = 56;	// if Rx data size is lager than TX_RX_MAX_BUF_SIZE
			recvfrom(SOCK_SNTP, ntpTimeServer_buf, size, (uint8_t *)&destip, &destport);
			//Mot ban tin tu NTP Time Server
			//24 3 6 e8 0 0 2c 3c 0 0 e 7d 8e 93 5c 5 e1 6 3a 76 77 3a 48 cf 0 0 0 0 0 0 0 0 e1 6 3e 9d 25 4f 82 99 e1 6 3e 9d 25 52 19 13
//			for(i=0;i<48;i++)
//						{
//						   printf("%x ",*(ntpTimeServer_buf+i));
//						}
			sec = (ntpTimeServer_buf[40]<<24) + (ntpTimeServer_buf[41]<<16) + (ntpTimeServer_buf[42]<<8) + ntpTimeServer_buf[43] ;
			//printf("Seconds: %u\r\n",sec-seventyYears);
			timenow = sec-seventyYears;
			
			printf("\r\nSynced with ntp server, seconds: %u\r\n",timenow);
			timeinfo = localtime( &timenow );
			asctime(timeinfo);
			#ifdef			ntpClientDebug
			printf("Current local time and date: %s\r\n", asctime(timeinfo));
		  printf("HH-MM-SS :%d-%d-%d\r\n",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
		  printf("DD-MM-YY :%d-%d-%d\r\n",timeinfo->tm_mday,1+timeinfo->tm_mon,timeinfo->tm_year-100);
			#endif
			days = timeinfo->tm_mday;
			months = 1+timeinfo->tm_mon;
			years = timeinfo->tm_year-100;
			hours = timeinfo->tm_hour;
			minutes = timeinfo->tm_min;
			seconds = timeinfo->tm_sec;
			
			RTC_Update();
			load_line1(days,months,years);
			scan_7up();
			load_line2(hours,minutes,seconds,1);
			scan_5down();
			
			TimeIsSet = 1;
			close(SOCK_SNTP);

			return 1;
		}
				if(TimeIsSet == 0) //chua chinh gio
			{
				if(RetrySend > 60) //Try Again gui ban tin hoi gio
				{
					RetrySend = 0;
					sendto(SOCK_SNTP,ntpmessage,48,ntpTimeServer_ip,123);
					countOfNTPrequest++;
					//sendto(SOCK_SNTP,ntpmessage,48,ntpTimeServer_ip,123);
#ifdef			ntpClientDebug		
					printf("NTP ask for time sent");					
//					printf("Gui ban tin di :");//35 0 6 236 0 0 0 0 0 0 0 0 49 78 49 52 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
//					for(i=0;i<48;i++)
//					{
//						printf("%d ",*(ntpmessage+i));
//					
//					}
//					printf("\r\n");
#endif					
				}
				return 0;
			}
			
			
		
		break;
	case SOCK_CLOSED:
	    //printf("%d:SNTP client try to start at port %d\r\n",SOCK_SNTP,sntp_port);
		//socket(NTP_SOCKET,Sn_MR_UDP,sntp_port,0);
		if((ret=socket(SOCK_SNTP,Sn_MR_UDP,sntp_port,0x00)) != SOCK_SNTP)
            return ret;
		//printf("%d:Opened, port [%d]\r\n",SOCK_SNTP, sntp_port);
		printf(" Socket[%d] UDP Socket for SNTP client started at port [%d]\r\n", SOCK_SNTP, sntp_port);
		break;
	}
	// Return value
	// 0 - failed / 1 - success
	return 0;
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
			
			
			if(newfractionOfSecond > 9980)//Sang giay moi roi....
			{
				seconds++;
				RTC_Update();
				printf("Time set NTP");
			}
			else {waitForSetTime = 1;}
			
			#ifdef			ntpClientDebug
			//printf("Current local time and date: %s\r\n", asctime(timeinfo));
		  //printf("HH-MM-SS :%d-%d-%d\r\n",hours,minutes,seconds);
		  //printf("DD-MM-YY :%d-%d-%d\r\n",days,months,years);
			#endif
			
			load_line1(days,months,years);
			scan_7up();
			load_line2(hours,minutes,seconds,1);
			scan_5down();
			
			TimeIsSet = 1;
			return 1;
}
void sendNTPpacket(void)
{
	sendto(SOCK_SNTP,ntpmessage,48,ntpTimeServer_ip,123);
	NTP_busy = 1;				
					t0 = timenow;
					t0Frac = fractionOfSecond;
					countOfNTPrequest++;
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
/**
Tinh toan phan le cua giay( fraction Of Second) : https://en.wikipedia.org/wiki/Network_Time_Protocol
- Coi t1 = t2 : thoi gian xu ly cua server rat nhanh va sap xi 0s
- Coi thoi gian truyen Client to Server va Server ve Client la bang nhau
=> tre duong truyen round-trip delay / 2
#ifdef			ntpClientDebug
			#endif
*/
int8_t SNTP_run2(void)//datetime sntp;
{
	uint32_t ret;
	uint16_t size;
	uint32_t destip = 0;
	uint16_t destport;
	//uint16_t startindex = 40; //last 8-byte of data_buf[size is 48 byte] is xmt, so the startindex should be 40
	int8_t i;
	
	if (sycnPeriod >= 61) // dong bo lai thoi gian
	{
		printf("TimeIsSet = 0\r\n");
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
				printf("Closed SOCK NTP1");
				NTP_busy = 0;
				return 0;
			}
			
			NTP_busy = 0;
			close(SOCK_SNTP);
			printf("Closed SOCK NTP\r\n");
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




/*
00)UTC-12:00 Baker Island, Howland Island (both uninhabited)
01) UTC-11:00 American Samoa, Samoa
02) UTC-10:00 (Summer)French Polynesia (most), United States (Aleutian Islands, Hawaii)
03) UTC-09:30 Marquesas Islands
04) UTC-09:00 Gambier Islands;(Summer)United States (most of Alaska)
05) UTC-08:00 (Summer)Canada (most of British Columbia), Mexico (Baja California)
06) UTC-08:00 United States (California, most of Nevada, most of Oregon, Washington (state))
07) UTC-07:00 Mexico (Sonora), United States (Arizona); (Summer)Canada (Alberta)
08) UTC-07:00 Mexico (Chihuahua), United States (Colorado)
09) UTC-06:00 Costa Rica, El Salvador, Ecuador (Galapagos Islands), Guatemala, Honduras
10) UTC-06:00 Mexico (most), Nicaragua;(Summer)Canada (Manitoba, Saskatchewan), United States (Illinois, most of Texas)
11) UTC-05:00 Colombia, Cuba, Ecuador (continental), Haiti, Jamaica, Panama, Peru
12) UTC-05:00 (Summer)Canada (most of Ontario, most of Quebec)
13) UTC-05:00 United States (most of Florida, Georgia, Massachusetts, most of Michigan, New York, North Carolina, Ohio, Washington D.C.)
14) UTC-04:30 Venezuela
15) UTC-04:00 Bolivia, Brazil (Amazonas), Chile (continental), Dominican Republic, Canada (Nova Scotia), Paraguay,
16) UTC-04:00 Puerto Rico, Trinidad and Tobago
17) UTC-03:30 Canada (Newfoundland)
18) UTC-03:00 Argentina; (Summer) Brazil (Brasilia, Rio de Janeiro, Sao Paulo), most of Greenland, Uruguay
19) UTC-02:00 Brazil (Fernando de Noronha), South Georgia and the South Sandwich Islands
20) UTC-01:00 Portugal (Azores), Cape Verde
21) UTC&#177;00:00 Cote d'Ivoire, Faroe Islands, Ghana, Iceland, Senegal; (Summer) Ireland, Portugal (continental and Madeira)
22) UTC&#177;00:00 Spain (Canary Islands), Morocco, United Kingdom
23) UTC+01:00 Angola, Cameroon, Nigeria, Tunisia; (Summer)Albania, Algeria, Austria, Belgium, Bosnia and Herzegovina,
24) UTC+01:00 Spain (continental), Croatia, Czech Republic, Denmark, Germany, Hungary, Italy, Kinshasa, Kosovo,
25) UTC+01:00 Macedonia, France (metropolitan), the Netherlands, Norway, Poland, Serbia, Slovakia, Slovenia, Sweden, Switzerland
26) UTC+02:00 Libya, Egypt, Malawi, Mozambique, South Africa, Zambia, Zimbabwe, (Summer)Bulgaria, Cyprus, Estonia,
27) UTC+02:00 Finland, Greece, Israel, Jordan, Latvia, Lebanon, Lithuania, Moldova, Palestine, Romania, Syria, Turkey, Ukraine
28) UTC+03:00 Belarus, Djibouti, Eritrea, Ethiopia, Iraq, Kenya, Madagascar, Russia (Kaliningrad Oblast), Saudi Arabia,
29) UTC+03:00 South Sudan, Sudan, Somalia, South Sudan, Tanzania, Uganda, Yemen
30) UTC+03:30 (Summer)Iran
31) UTC+04:00 Armenia, Azerbaijan, Georgia, Mauritius, Oman, Russia (European), Seychelles, United Arab Emirates
32) UTC+04:30 Afghanistan
33) UTC+05:00 Kazakhstan (West), Maldives, Pakistan, Uzbekistan
34) UTC+05:30 India, Sri Lanka
35) UTC+05:45 Nepal
36) UTC+06:00 Kazakhstan (most), Bangladesh, Russia (Ural: Sverdlovsk Oblast, Chelyabinsk Oblast)
37) UTC+06:30 Cocos Islands, Myanmar
38) UTC+07:00 Jakarta, Russia (Novosibirsk Oblast), Thailand, Vietnam
39) UTC+08:00 China, Hong Kong, Russia (Krasnoyarsk Krai), Malaysia, Philippines, Singapore, Taiwan, most of Mongolia, Western Australia
40) UTC+09:00 Korea, East Timor, Russia (Irkutsk Oblast), Japan
41) UTC+09:30 Australia (Northern Territory);(Summer)Australia (South Australia))
42) UTC+10:00 Russia (Zabaykalsky Krai); (Summer)Australia (New South Wales, Queensland, Tasmania, Victoria)
43) UTC+10:30 Lord Howe Island
44) UTC+11:00 New Caledonia, Russia (Primorsky Krai), Solomon Islands
45) UTC+11:30 Norfolk Island
46) UTC+12:00 Fiji, Russia (Kamchatka Krai);(Summer)New Zealand
47) UTC+12:45 (Summer)New Zealand
48) UTC+13:00 Tonga
49) UTC+14:00 Kiribati (Line Islands)
*/


