#include <time.h>
#include "socketdefines.h"
#include "mydefines.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include "socket.h"

//#define NTP_CLIENT_DEBUG

#ifdef OverTheAir
extern char udp_message[];
extern uint8_t send_debug_message;
extern uint8_t mysize;
#endif
uint8_t t_days, t_months, t_years, t_hours, t_minutes, t_seconds;
uint8_t WAIT_FOR_SET_TIME = 0;

extern uint32_t countOfNTPrequest;
extern uint32_t countOfNTP_Rec;

extern time_t built_time;
extern struct tm currtime;
uint8_t setTimeNow = 0;
extern uint8_t  synchronized_ntp;
extern uint16_t synchronizePeriod;
uint8_t NTP_Server_Responded = 0;
uint8_t NTP_alreadySent = 1;
uint8_t NTP_alreadySentTimeOut = 13;
extern volatile uint8_t waitForSetTime;



time_t t0,t0Frac,t3Frac,t3;
time_t loop_delay;
time_t round_trip_delay;
time_t newfractionOfSecond;
time_t Server_time;

int32_t offset;
int32_t avg_offset;
int32_t avg_offset_stable;//Gia tri sai lech on dinh
int32_t lastOffset = 0;
int32_t deltaOffset = 0;
#define HeSoOnDinh 2
uint8_t TimeServerStable = 0;

/* Shifts usecs in unixToNtpTime */
//#ifndef USECSHIFT
//#define USECSHIFT (1LL << 32) * 1.0e-6
//#endif
#define usShift 4294.967296;
//Number of seconds from 1st January 1900 to start of Unix epoch
//According to the Time protocol in RFC 868 it is 2208988800L.
#define STARTOFTIME 2208988800UL



//#define ntpClientDebug
#define	MAX_SNTP_BUF_SIZE	sizeof(ntpformat)		///< maximum size of DNS buffer. */


#define sntp_port 1234 //my sntp port, cho de gui ban tin ve
#define ntp_port		123                     //ntp server port number
#define SECS_PERDAY		86400UL             	// seconds in a day = 60*60*24
#define UTC_ADJ_HRS		0              	        // Vietnam : GMT+7
#define EPOCH			1900                    // NTP start year

void SNTP_init(void);
int8_t SNTP_run(void);

extern time_t timenow;
extern uint8_t time2SaveRTC;

extern uint8_t ntpTimeServer_ip[4];

uint8_t TimeIsSet = 0;
uint16_t RetrySend = 0; //60 giay
uint16_t sycnPeriod = 0;// 1 gio
//uint8_t Domain_ntpTimeServer[] = "0.asia.pool.ntp.org";    // for Example domain name
//uint8_t Domain_ntpTimeServer[] = "time.windows.com";

uint8_t ntpTimeServer_buf[60];
uint8_t ntpmessage[48]={0};


// Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
const uint32_t seventyYears = 2208988800;

time_t micros_transmit;
double sec_frac;





void SNTP_init(void)
{
	//db 00 0a e9 00 00 00 8a 00 08 c2 9d 00 00 00 00 e1 0f 48 0f 0a 93 29 3d 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 e1 0f 48 12 ea 93 54 db
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




void sycn_RTC_Now(time_t x)
{
			timeinfo = localtime( &x );
			asctime(timeinfo);
	
			seconds = timeinfo->tm_sec;
			if(seconds > 56) return;
			days = timeinfo->tm_mday;
			months = 1+timeinfo->tm_mon;
			years = timeinfo->tm_year-100;
			hours = timeinfo->tm_hour;
			minutes = timeinfo->tm_min;
			
			RTC_Update();
			//printf("Save RTC! %d\r\n",timenow);
}
//Neu RTC cham qua
uint8_t sycn_RTC_slow(time_t x)
{
	uint32_t temp;
	//Du lieu sai
	if(avg_offset_stable < 0	) 
	{
		TimeServerStable = 0;
		avg_offset = 0;
		return 0;
	}
	
	
	server_second++;

	if(server_second > 56) 
	{
		TimeServerStable = 0;
		avg_offset = 0;
		return 0;
	}

	temp = fractionOfSecond + avg_offset_stable;
	//printf("slower clock! temp %d\r\n",temp);
	if(temp >19000) 
		{
			server_second ++;
			TIM4->CNT = temp - 9999;
		}
		else TIM4->CNT = temp;
	WAIT_FOR_SET_TIME = 1;
	
			
	#ifdef NTP_CLIENT_DEBUG
	printf("slower clock! Loop delay %d, avg offset :%d,TIM4 %d\r\n",loop_delay,avg_offset_stable,TIM4->CNT);	
	#endif
	    
			return 1;
}


/*
avg_offset_stable
*/
uint8_t sycn_RTC_fast(time_t x)
{
	uint32_t temp;
	//Du lieu sai
	if(avg_offset_stable >= 0	) 
	{
		TimeServerStable = 0;
		avg_offset = 0;
		return 0;
	}

	timeinfo = localtime( &x );
	asctime(timeinfo);
	
	t_seconds = timeinfo->tm_sec;

	if(t_seconds > 56) 
	{
		TimeServerStable = 0;
		avg_offset = 0;
		return 0;
	}
	
	temp = 19999 + avg_offset_stable +fractionOfSecond;
	//printf("faster clock! temp %d\r\n",temp);	
	if(temp >19000) 
		{
			server_second ++;
			TIM4->CNT = temp - 10000;
		}
		else TIM4->CNT  =temp;
	WAIT_FOR_SET_TIME = 1;
	#ifdef NTP_CLIENT_DEBUG
	printf("faster clock! Loop delay %d, avg offset :%d,TIM4 %d\r\n",loop_delay,avg_offset_stable,TIM4->CNT);	
	#endif
	return 1;
}
int8_t SNTP_run(void)
{
	uint32_t ret;
	uint32_t sec;
	uint16_t size;
	uint32_t destip = 0;
	uint16_t destport;
	int32_t SecondOffset; 
	
	
	
	switch(getSn_SR(SOCK_SNTP))
	{
	case SOCK_UDP:
		if ((size = getSn_RX_RSR(SOCK_SNTP)) > 0)
		{
			
			loop_delay = counterOf10Second;
			NTP_Server_Responded = 1;
			slave_clock.ntp_client_status = CONNECTED;
			countOfNTP_Rec++;
			//if (size > 56) size = 56;	// if Rx data size is lager than TX_RX_MAX_BUF_SIZE
			recvfrom(SOCK_SNTP, ntpTimeServer_buf, 56, (uint8_t *)&destip, &destport);
			
			if(slave_clock.rs458_connection == NO_CONNECTION)
			{
				//Tin hieu GPS ko on dinh!!!
				if(ntpTimeServer_buf[1] != 1) slave_clock.sync_status = BOTH;
				else slave_clock.sync_status = GREEN;
			}
			
			sec = (ntpTimeServer_buf[40]<<24) + (ntpTimeServer_buf[41]<<16) + (ntpTimeServer_buf[42]<<8) + ntpTimeServer_buf[43] ;
			Server_time = sec - seventyYears;
			//Neu thoi gian Server ko tin cay!
			if(Server_time < built_time) return 0;
			SecondOffset = 	Server_time - timenow;		
			//Sai khac qua 1s
			if((SecondOffset > 1 ) || (SecondOffset < -1))
			{	
				//printf("Sai lech nhieu %d s\r\n",SecondOffset);	
				#ifdef OverTheAir
						send_debug_message = 1;
						mysize = sprintf(udp_message,"Sai lech nhieu %d s, now %d\r\n",SecondOffset,timenow);
				#endif		
				
				if(slave_clock.rs458_connection == NO_CONNECTION)
				{
						sycn_RTC_Now(Server_time);
																		
					return 1;
				}							
			}
			
			//Tinh toan phan le cua giay nhan dc
			micros_transmit = (ntpTimeServer_buf[44]<<24) + (ntpTimeServer_buf[45]<<16) + (ntpTimeServer_buf[46]<<8) + ntpTimeServer_buf[47] ;
			sec_frac = micros_transmit / usShift;
			sec_frac = sec_frac - 1;
			sec_frac = sec_frac / 100;
			
			//Neu sai lech <= 1s
			timeinfo = localtime( &Server_time );
			asctime(timeinfo);
			server_day    = timeinfo->tm_mday;
			server_month  = timeinfo->tm_mon +1;
			server_year   = timeinfo->tm_year-100;
			server_hour   = timeinfo->tm_hour;
			server_minute = timeinfo->tm_min;
			server_second = timeinfo->tm_sec;
			
			offset = 10000*(server_second - seconds) + ((int)sec_frac - fractionOfSecond) + (loop_delay)/2;
			
			#ifdef NTP_CLIENT_DEBUG
			printf("Tre thoi gian giua server - device: %d ms\r\n",offset/10);
			#endif
			
			//Neu offset <99.9ms thi thoi luon cho nhanh!
			if(((offset < 999) && (offset >= 0)) || ((offset > -999) && (offset < 0)))
			{
				avg_offset_stable = offset;
				TimeServerStable = 0;
				avg_offset = 0;
				
				#ifdef NTP_CLIENT_DEBUG
				printf("Gio chuan, offset :%d ms\r\n",offset/10);
				#endif
				
				#ifdef OverTheAir
				send_debug_message = 1;
				mysize = sprintf(udp_message,"Gio chuan, offset :%d ms\r\n",offset/10);
				#endif
				
				NTP_alreadySentTimeOut = 19;
				return 1;
			}
			deltaOffset = offset - lastOffset;
			//Do sai lech offset/last offset < 100ms => tin hieu co ve on dinh => dong bo
			//if( ((offset >= lastOffset) && (deltaOffset <= 999))  || ((offset < lastOffset) && ((lastOffset - offset) <= 999)) )
			if((deltaOffset<999) || (deltaOffset>-999) )
			{
				//printf("offset > lastOffset %d\r\n",offset- lastOffset);
				TimeServerStable++;
				avg_offset = avg_offset + offset;
			}
			else 
			{
				#ifdef NTP_CLIENT_DEBUG
				printf("Unstable offset > lastOffset %d\r\n",offset- lastOffset);
				#endif
				
				#ifdef OverTheAir
				send_debug_message = 1;
				mysize = sprintf(udp_message,"Unstable offset: %d ms\r\n",deltaOffset/10);
				#endif
				
				TimeServerStable = 0;
				avg_offset = 0;
			}
					
			lastOffset = offset;
			
			if(TimeServerStable > HeSoOnDinh)
			{
				avg_offset_stable = avg_offset/TimeServerStable;
				
				#ifdef NTP_CLIENT_DEBUG			
				//printf("Time server on dinh! AVG: %d ms\r\n",avg_offset_stable/10);//Chinh gio thoi
				#endif
				#ifdef OverTheAir
				send_debug_message = 1;
				mysize = sprintf(udp_message,"Time server stable %d!sum offset %d ms Big offset AVG: %d ms \r\n",TimeServerStable,avg_offset/10,avg_offset_stable/10);
				#endif
			  TimeServerStable = 0;
				avg_offset = 0;
			}
			else //dang kiem tra do on dinh cua Time server
			{
				
				return 0;// chua lam gi ca
			}
			
			//Bat dau chinh gio neu ko co RS485
			if(slave_clock.rs458_connection == NO_CONNECTION)
			{
				if(avg_offset_stable>=0)// Dong ho bi cham
					{
								
								//printf("Slower clock! Loop delay %d, avg offset :%d\r\n",loop_delay,avg_offset_stable);	
						 sycn_RTC_slow(Server_time);
					}
					else if(avg_offset_stable<0)//Dong ho nhanh
					{
						
							sycn_RTC_fast(Server_time);
							//printf("faster clock! Loop delay %d, avg offset :%d\r\n",loop_delay,avg_offset_stable);	

					}
			}
			
			
			//printf("Loop delay %d, offset :%d\r\n",loop_delay,offset);	
							
			//synchronizePeriod = 16;
			//synchronized_ntp = 1;
			//NTP_alreadySent  = 0;
			return 1;
		}
		
		if(	NTP_alreadySent == 0)
			{
				ret = sendto(SOCK_SNTP,ntpmessage,48,ntpTimeServer_ip,123);
				
				counterOf10Second = 0;
				countOfNTPrequest++;
				
				NTP_alreadySent = 1;
				NTP_alreadySentTimeOut = 5;
				
				#ifdef NTP_CLIENT_DEBUG
				//Neu socket timeout thi se bi treo 2s!!!!
				if(ret == 48) 							printf("Sent ok\r\n");
				if(ret == SOCKERR_TIMEOUT)	printf("Socket timeout\r\n");
				printf("NTP_alreadySent, %d\r\n",ret);
				#endif	
				return 0;
			}
			
			
			
		
		break;
	case SOCK_CLOSED:
		if((ret=socket(SOCK_SNTP,Sn_MR_UDP,sntp_port,0x00)) != SOCK_SNTP) return ret;
		#ifdef NTP_CLIENT_DEBUG	
		printf(" Socket[%d] UDP Socket for SNTP client started at port [%d]\r\n", SOCK_SNTP, sntp_port);
		#endif	
	break;
	}
	// Return value
	// 0 - failed / 1 - success
	return 0;
}





