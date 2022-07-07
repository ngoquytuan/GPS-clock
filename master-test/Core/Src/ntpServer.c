#include <stdio.h>
#include <time.h>
#include <string.h>
#include "stm32g4xx_hal.h"
#include "w5500.h"
#include "socket.h"
#include "socketdefines.h"

//#include "ntpserver.h"
// Time Server Port
//#define SOCK_UDPS        			0
//#define NTP_PORT							123
//#define NTP_PACKET_SIZE 			48
//#define NTP_PACKET_RAWSIZE		56
#define fractionOfSecond TIM4->CNT
uint32_t countOfNTPrequest = 0;
extern time_t timenow;
//for NTP server
time_t unixTime_last_sync;// = 1650858760;// lan chuan gio gan nhat 1650858760
/* SNTP Packet array */
uint8_t serverPacket[NTP_PACKET_SIZE] = {0};
uint8_t clientPacket[NTP_PACKET_RAWSIZE] = {0};
time_t micros_recv = 0;
time_t micros_offset = 0;
time_t transmitTime = 0;
time_t micros_transmit = 0;
time_t recvTime = 0;





#define LEAP_MASK               0xC0
#define LEAP_NONE               0x0
#define LEAP_INSERT             0x1
#define LEAP_DELETE             0x2
#define LEAP_UNKNOWN            0x3
#define VN_MASK                 0x38
#define VN_4                    (4 << 3)
#define MODE_MASK               0x07
#define MODE_CLIENT             0x3
#define MODE_SERVER             0x4
/* Shifts usecs in unixToNtpTime */
//??? ko hieu nhung dung! 
#ifndef USECSHIFT
#define USECSHIFT (1LL << 32) * 1.0e-6
#endif
//Number of seconds from 1st January 1900 to start of Unix epoch
//According to the Time protocol in RFC 868 it is 2208988800L.
#define STARTOFTIME 2208988800UL

#ifndef UTIL_H
#define UTIL_H

#define htons(x) ( ((x)<< 8 & 0xFF00) | \
                   ((x)>> 8 & 0x00FF) )
#define ntohs(x) htons(x)

#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                   ((x)<< 8 & 0x00FF0000UL) | \
                   ((x)>> 8 & 0x0000FF00UL) | \
                   ((x)>>24 & 0x000000FFUL) )
#define ntohl(x) htonl(x)
#endif


void wzn_event_handle(void);
void ntpserverdefaultconfig(void);
int32_t ntpserverprocess(void);


void INT_NTP(void)
{
	micros_recv = 100*fractionOfSecond;
	recvTime = (timenow + STARTOFTIME);//gio luc nhan dc ban tin
}

void wzn_event_handle(void)
{
	uint16_t ir = 0;
	uint8_t sir = 0;
	
	
	if (ctlwizchip(CW_GET_INTERRUPT, &ir) == -1) {
		printf("Cannot get ir...");
		return;
	}
	
	if (ir & IK_SOCK_0) 
		{
				sir = getSn_IR(SOCK_UDPS);
				
				if ((sir & Sn_IR_RECV) > 0) {
					/* Clear Sn_IR_RECV flag. */
					setSn_IR(SOCK_UDPS, Sn_IR_RECV);
					return;
				}
				else if ((sir & Sn_IR_SENDOK) > 0) {
					/* Clear Sn_IR_SENDOK flag. */
					setSn_IR(SOCK_UDPS, Sn_IR_SENDOK);
					//printf("app_sent();\r\n");
					//app_sent();
				}
		}
}


void ntpserverdefaultconfig(void)
{
		
		//Phan co dinh cua ban tin NTP
		serverPacket[0] = 0x24;   // LI, Version, Mode // Set version number and mode
		serverPacket[1] = 1; // Stratum, or type of clock
		serverPacket[2] = 0;     // Polling Interval
		serverPacket[3] = -12;  // Peer Clock Precision
		serverPacket[12] = 'G';
		serverPacket[13] = 'P';
		serverPacket[14] = 'S';
		//[Reference Timestamp]: unsigned 32-bit seconds value : Lan lay chuan gan nhat la bao gio	
					
		unixTime_last_sync = (timenow + STARTOFTIME);//gio luc tryen ban tin
		unixTime_last_sync = htonl(unixTime_last_sync);// gio luc truyen
		memcpy(&serverPacket[16], &unixTime_last_sync, 4);
		
		unixTime_last_sync = timenow;
}

int32_t NTPUDP(void)
{
   int32_t  ret;
   uint16_t size, sentsize=0;
   uint8_t  destip[4];
   uint16_t destport;
	 
	// Ban tin NTP co size = 56 ( ca header : IP[4],port[2],length[2], tru di header chi con 48
   switch(getSn_SR(SOCK_UDPS))
   {
      case SOCK_UDP :
         if((size = getSn_RX_RSR(SOCK_UDPS)) == NTP_PACKET_RAWSIZE)//56 is size raw of NTP message
         {

					 ret = recvfrom(SOCK_UDPS,clientPacket,size,destip,(uint16_t*)&destport);
            
					  
            if(ret <= 0)
            {
               printf("%d: recvfrom error. %d\r\n",SOCK_UDPS,ret);
               return ret;
            }
					countOfNTPrequest++;
					//printf("NTP received!\r\n");
					//unixTime_last_sync = (timenow + STARTOFTIME);
					//unixTime_last_sync = htonl(unixTime_last_sync);
					//memcpy(&serverPacket[16], &unixTime_last_sync, 4);
					//memcpy(&serverPacket[16], &clientPacket[40], 8);

					
						//Transmit Timestamp, T3 from client, copy and return! 
						memcpy(&serverPacket[24], &clientPacket[40], 8);
						
						
						//Thoi gian nhan dc ban tin
						//printf("recvTime: %u, micros_recv:%u\r\n",recvTime,micros_recv);
						recvTime = htonl(recvTime);
						memcpy(&serverPacket[32], &recvTime, 4);
						//phan thap phan : fraction Of Second
						micros_recv = (micros_recv + 1) * USECSHIFT;
						micros_recv = htonl(micros_recv);
						memcpy(&serverPacket[36], &micros_recv, 4);
						
						memcpy(&serverPacket[16], &serverPacket[32], 8);//unixTime_last_sync == recvTime
						
						transmitTime = (timenow + STARTOFTIME);//gio luc tryen ban tin
						
					  //micros_transmit = 100*(fractionOfSecond);
						//printf("tranTime: %u, micros_tran:%u\r\n",transmitTime,micros_transmit);
						
						transmitTime = htonl(transmitTime);// gio luc truyen
						memcpy(&serverPacket[40], &transmitTime, 4);
						
						
						//Tinh toan phan thap phan cua thoi diem truyen tin
						
						micros_transmit = 100*fractionOfSecond;
						micros_transmit = (micros_transmit + 1) * USECSHIFT;
						micros_transmit = htonl(micros_transmit);//Ko hieu lam gi nhi, nhung dung!
						memcpy(&serverPacket[44], &micros_transmit, 4);
						
						//Gui tra ban tin NTP
						while(sentsize != NTP_PACKET_SIZE)
            {
               ret = sendto(SOCK_UDPS,serverPacket,NTP_PACKET_SIZE,destip,destport);
							 
               if(ret < 0)
               {
                  printf("%d: sendto error. %d\r\n",SOCK_UDPS,ret);
                  return ret;
               }
               sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
            }
						wzn_event_handle();
						//printf("\r\nNTPsent \r\n");
         }
         break;
      case SOCK_CLOSED:
				 
         if((ret=socket(SOCK_UDPS,Sn_MR_UDP,NTP_PORT,0x00)) != SOCK_UDPS)
            return ret;
         printf(" Socket[%d] UDP Socket for NTP Time Server started, port [%d]\r\n", SOCK_UDPS, NTP_PORT);
         break;
      default :
         break;
   }
   return 1;
}
