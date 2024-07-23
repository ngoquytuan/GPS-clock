#include <stdio.h>
#include "main.h"
#include "mydefines.h"
#include "socketdefines.h"
#include "wizchip_conf.h"

extern SPI_HandleTypeDef hspi1;
extern wiz_NetInfo myipWIZNETINFO;
extern uint8_t ntpTimeServer_ip[4];


uint8_t phylink = PHY_LINK_OFF;

#ifdef USE_DNS
#endif

static void  wizchip_select(void);
static void  wizchip_deselect(void);
static uint8_t wizchip_read(void);
static void  wizchip_write(uint8_t wb);
static void wizchip_readburst(uint8_t* pBuf, uint16_t len);
static void  wizchip_writeburst(uint8_t* pBuf, uint16_t len);


#include "dns.h"
// Define the DNS server IP (e.g., Google's public DNS server)
#define DNS_SERVER_IP {8, 8, 8, 8}

// Number of NTP servers
#define NUM_NTP_SERVERS 5
const char* ntpServers[] = {
  "0.asia.pool.ntp.org",
  "pool.ntp.org",
  "time.nist.gov",
  "time.google.com",
  "time.cloudflare.com"
};
// Define the DNS server IP (e.g., Google's public DNS server)
#define DNS_SERVER_IP {8, 8, 8, 8}
// Array to store IP addresses
uint8_t array_ip[NUM_NTP_SERVERS][4];
uint8_t current_ntp_index = 0; // Index of the current NTP server
uint8_t *ntp_ip;
uint8_t ntpServersFound = 0;
void print_ip(uint8_t *ip) {
    printf("%d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
}
void switch_to_next_ntp_server() {
    current_ntp_index = (current_ntp_index + 1) % NUM_NTP_SERVERS;
		ntp_ip = array_ip[current_ntp_index];
    printf("Switched to next NTP server: %s\n", ntpServers[current_ntp_index]);
}
int dns_check() {
    uint8_t dns_ip[] = DNS_SERVER_IP;
    uint8_t dns_buffer[MAX_DNS_BUF_SIZE];
    uint8_t socket_number = SOCK_DNS; // Define your socket number
    int8_t result;

    // Initialize the DNS process
    DNS_init(socket_number, dns_buffer);

    // Loop through each NTP server domain name
    for (int i = 0; i < NUM_NTP_SERVERS; i++) {
					#ifdef DebugEnable
        printf("Querying IP for %s...\n", ntpServers[i]);
					#endif
        result = DNS_run(dns_ip, (uint8_t *)ntpServers[i], array_ip[i]);

        if (result == 1) {
					ntpServersFound++;
					#ifdef DebugEnable
            printf("IP address of %s: ", ntpServers[i]);
            print_ip(array_ip[i]);
					#endif
        } else {
					#ifdef DebugEnable
            printf("Failed to get IP address of %s\n", ntpServers[i]);
					#endif
        }
    }
		
#ifdef DebugEnable
    // Optionally, print all collected IP addresses
    printf("\nCollected IP addresses:\n");
    for (int i = 0; i < NUM_NTP_SERVERS; i++) {
        printf("%s: ", ntpServers[i]);
        print_ip(array_ip[i]);
    }
#endif
    return 1;

    
}

/*
	Kiem tra W5500, SPI co on ko, day mang co cam ko?
*/
uint8_t checkLANcable(void)
{
	uint8_t tmp;

	if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
	{
		#ifdef DebugEnable
		//printf("Unknown PHY Link stauts.\r\n");//Loi spi???
		#endif
		slave_clock.ethernet_connection = NO_CONNECTION;
		phylink = PHY_LINK_OFF;
		return 0;
	}
	
	if(tmp == PHY_LINK_OFF) 
		{
			phylink = PHY_LINK_OFF;
			slave_clock.ethernet_connection = NO_CONNECTION;
			return 0;//Ko cam day mang
		}
	

	//printf("Co cam day mang nhe\r\n");

	phylink = PHY_LINK_ON;
	slave_clock.ethernet_connection = CONNECTED;
  return 1;  
}

/*
W5500	
*/

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
	printf("NTP time server IP: %d.%d.%d.%d\r\n", ntpTimeServer_ip[0], ntpTimeServer_ip[1], ntpTimeServer_ip[2], ntpTimeServer_ip[3]);
	
}

void Net_Conf(wiz_NetInfo temp_netinfo)
{

	ctlnetwork(CN_SET_NETINFO, (void*) &temp_netinfo);
	#ifdef DebugEnable
	Display_Net_Conf();
	#endif
}
/*
Khoi dong card mang!
*/
void w5500_lib_init(void){
		
		uint8_t tmp;
		intr_kind temp;
		uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};	
		

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
       //printf("WIZCHIP Initialized fail.\r\n");
    }

		
		//Cau hinh ngat tren Socket 2, vi can biet thoi diem ban tin NTP den!!
//		temp = IK_SOCK_2;
//		if(ctlwizchip(CW_SET_INTRMASK, &temp) == -1)
//		{
//			//printf("Cannot set imr...\r\n");
//		}
		Net_Conf(myipWIZNETINFO);
	  checkLANcable();
}


#ifdef OverTheAir
#include "socket.h"
#include "wizchip_conf.h"
#include "socketdefines.h"
#ifndef DATA_BUF_SIZE
	#define DATA_BUF_SIZE	2048
#endif
#define	_LOOPBACK_DEBUG_
char udp_message[DATA_BUF_SIZE];
uint8_t  destip_PC[4]={192, 168, 1, 164};
uint8_t send_debug_message = 0;
uint8_t mysize = 9;
int32_t loopback_tcpc(uint8_t sn, uint8_t* buf, uint8_t* destip, uint16_t destport)
{
  int32_t ret; // return value for SOCK_ERRORs
  uint16_t size = 0, sentsize=0;

  // Destination (TCP Server) IP info (will be connected)
  // >> loopback_tcpc() function parameter
  // >> Ex)
  //	uint8_t destip[4] = 	{192, 168, 0, 214};
  //	uint16_t destport = 	5000;

  // Port number for TCP client (will be increased)
  uint16_t any_port = 	139;

  // Socket Status Transitions
  // Check the W5500 Socket n status register (Sn_SR, The 'Sn_SR' controlled by Sn_CR command or Packet send/recv status)
  switch (getSn_SR(sn))
  {
    case SOCK_ESTABLISHED:
      if (getSn_IR(sn) & Sn_IR_CON)	// Socket n interrupt register mask; TCP CON interrupt = connection with peer is successful
      {
#ifdef _LOOPBACK_DEBUG_
	printf("%d:Connected to - %d.%d.%d.%d : %d\r\n", sn, destip[0], destip[1], destip[2], destip[3], destport);
#endif
	setSn_IR(sn, Sn_IR_CON);  // this interrupt should be write the bit cleared to '1'
      }
			
			
			if(send_debug_message)
				{
					send_debug_message = 0;
					#ifdef SLAVE_CONSOLE
					ret = send(sn, (uint8_t*)"Console\r\n", 9);
					#endif
					#ifdef SLAVE_MATRIX
					ret = send(sn, (uint8_t*)"Matrix\r\n", 8);
					#endif
					ret = send(sn, buf, mysize); // Data send process (User's buffer -> Destination through H/W Tx socket buffer)
								if (ret < 0) // Send Error occurred (sent data length < 0)
								{
									close(sn); // socket close
									return ret;
								}
				}
      //////////////////////////////////////////////////////////////////////////////////////////////
      // Data Transaction Parts; Handle the [data receive and send] process
      //////////////////////////////////////////////////////////////////////////////////////////////
      if ((size = getSn_RX_RSR(sn)) > 0) // Sn_RX_RSR: Socket n Received Size Register, Receiving data length
      {
	if (size > DATA_BUF_SIZE)
          size = DATA_BUF_SIZE; // DATA_BUF_SIZE means user defined buffer size (array)
	ret = recv(sn, buf, size); // Data Receive process (H/W Rx socket buffer -> User's buffer)

	if (ret <= 0)    // If the received data length <= 0, receive failed and process end
          return ret;

	sentsize = 0;
  
	
	// Data sentsize control
	while (size != sentsize)
	{
          ret = send(sn, buf + sentsize, size - sentsize); // Data send process (User's buffer -> Destination through H/W Tx socket buffer)
          if (ret < 0) // Send Error occurred (sent data length < 0)
          {
            close(sn); // socket close
            return ret;
          }
          sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
	}
      }
      //////////////////////////////////////////////////////////////////////////////////////////////
      break;

    case SOCK_CLOSE_WAIT:
#ifdef _LOOPBACK_DEBUG_
      printf("%d:CloseWait\r\n",sn);
#endif
      if ((ret=disconnect(sn)) != SOCK_OK)
        return ret;
#ifdef _LOOPBACK_DEBUG_
      printf("%d:Socket Closed\r\n", sn);
#endif
      break;

    case SOCK_INIT:
#ifdef _LOOPBACK_DEBUG_
      printf("%d:Try to connect to the %d.%d.%d.%d : %d\r\n", sn, destip[0], destip[1], destip[2], destip[3], destport);
#endif
      if ((ret = connect(sn, destip, destport)) != SOCK_OK)   // Try to TCP connect to the TCP server (destination)
        return ret;
      break;

    case SOCK_CLOSED:
      close(sn);
      if ((ret = socket(sn, Sn_MR_TCP, any_port++, 0x00)) != sn)  // TCP socket open with 'any_port' port number
        return ret;
#ifdef _LOOPBACK_DEBUG_
      printf("%d:TCP client loopback start\r\n",sn);
      printf("%d:Socket opened\r\n",sn);
#endif
      break;
    default:
      break;
  }
  
  return 1;
}

#endif

/*Cac ham phu tro ko can phai doc	*/
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


