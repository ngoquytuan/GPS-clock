/**
 * @file	httpUtil.c
 * @brief	HTTP Server Utilities	
 * @version 1.0
 * @date	07July2022
 * @par Revision
 *			2014/07/15 - 1.0 Release
 * @author	last edit by Tuannq 07July2022
 * \n\n @par Copyright (C) 1998 - 2014 WIZnet. All rights reserved.
 */

#include "wizchip_conf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "httpUtil.h"
#include "mydefines.h"
//wiz_NetInfo getWIZNETINFO;
//Dung luon bien nay luu cho tiet kiem bo nho!!!
extern wiz_NetInfo myipWIZNETINFO;
extern uint8_t ntpTimeServer_ip[4]; 



/************************************************************************************************/
void stm32g474flashEraseThenSave(void);
//Bien luu gia tri cho webserver

extern uint8_t gps1_stt;
extern uint8_t gps2_stt;
extern uint8_t power1_stt;
extern uint8_t power2_stt;
extern uint8_t days;
extern uint8_t months;
extern uint8_t years;
extern uint8_t hours;
extern uint8_t minutes;
extern uint8_t seconds;
extern uint16_t saved;
extern uint8_t LEDintensity;
extern int8_t lostSignal;
// Pre-defined Get CGI functions

void make_json_netinfo(uint8_t * buf, uint16_t * len);



void make_json_netinfo2(uint8_t * buf, uint16_t * len)
{
	wiz_NetInfo netinfo;
	ctlnetwork(CN_GET_NETINFO, (void*) &netinfo);

	// DHCP: 1 - Static, 2 - DHCP
	*len = sprintf((char *)buf, "NetinfoCallback({\"mac\":\"%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\",\
											\"ip\":\"%d.%d.%d.%d\",\
											\"gw\":\"%d.%d.%d.%d\",\
											\"sn\":\"%d.%d.%d.%d\",\
											\"dns\":\"%d.%d.%d.%d\",\
											\"ntp\":\"%d.%d.%d.%d\",\
											\"dhcp\":\"%d\"\
											});",
											netinfo.mac[0], netinfo.mac[1], netinfo.mac[2], netinfo.mac[3], netinfo.mac[4], netinfo.mac[5],
											netinfo.ip[0], netinfo.ip[1], netinfo.ip[2], netinfo.ip[3],
											netinfo.gw[0], netinfo.gw[1], netinfo.gw[2], netinfo.gw[3],
											netinfo.sn[0], netinfo.sn[1], netinfo.sn[2], netinfo.sn[3],
											netinfo.dns[0], netinfo.dns[1], netinfo.dns[2], netinfo.dns[3],
										  ntpTimeServer_ip[0],ntpTimeServer_ip[1],ntpTimeServer_ip[2],ntpTimeServer_ip[3],
											netinfo.dhcp
											);
}

/************************************************************************************************/


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t http_get_json_handler(uint8_t * uri_name, uint8_t * buf, uint32_t * file_len)
{
	uint8_t ret = HTTP_OK;
	uint16_t len = 0;

//	if(predefined_get_cgi_processor(uri_name, buf, &len))
//	{
//		;
//	}
//	else 
	if(strcmp((const char *)uri_name, "alldata.json") == 0)
	{
		// To do
		//printf("uri_name : %s\r\n",uri_name);
		make_json_netinfo(buf, &len);
		;
	}
//	else if(strcmp((const char *)uri_name, "get_serial_data.cgi") == 0)
//	{
//		//make_json_serial_data(buf, &len);
//	}
	else
	{
		// CGI file not found
		ret = HTTP_FAILED;
	}

	if(ret)	*file_len = len;
	return ret;
}

uint8_t http_get_cgi_handler(uint8_t * uri_name, uint8_t * buf, uint32_t * file_len)
{
	uint8_t ret = HTTP_OK;
	uint16_t len = 0;

//	if(predefined_get_cgi_processor(uri_name, buf, &len))
//	{
//		;
//	}
//	else 
//
	if(strcmp((const char *)uri_name, "get_netinfo.cgi") == 0)
	{
		make_json_netinfo2(buf, &len);
		//printf("get_netinfo.cgi\r\n");
	}	
	else if(strcmp((const char *)uri_name, "example.cgi") == 0)
	{
		// To do
		;
	}
//	else if(strcmp((const char *)uri_name, "get_serial_data.cgi") == 0)
//	{
//		//make_json_serial_data(buf, &len);
//	}
	else
	{
		// CGI file not found
		//printf("CGI file not found\r\n");
		ret = HTTP_FAILED;
	}

	if(ret)	*file_len = len;
	return ret;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Boc tach IP, GW, SN...tu webserver
uint8_t * set_basic_config_setting(uint8_t * uri)
{
	uint8_t * param;

	uint8_t * ip = myipWIZNETINFO.ip;
	//boc tach IP
		if((param = get_http_param_value((char *)uri, "ip")))
		{
			//inet_addr_((uint8_t*)param, value->network_info_common.local_ip);
			inet_addr_((uint8_t*)param, myipWIZNETINFO.ip);
			//printf("IP: %d.%d.%d.%d\r\n",myipWIZNETINFO.ip[0],myipWIZNETINFO.ip[1],myipWIZNETINFO.ip[2],myipWIZNETINFO.ip[3]);
		}
		//boc tach Getway
		if((param = get_http_param_value((char *)uri, "gw")))
		{
			inet_addr_((uint8_t*)param, myipWIZNETINFO.gw);
			//printf("Getway: %d.%d.%d.%d\r\n",myipWIZNETINFO.gw[0],myipWIZNETINFO.gw[1],myipWIZNETINFO.gw[2],myipWIZNETINFO.gw[3]);
		}
		//boc tach Subnet
		if((param = get_http_param_value((char *)uri, "sn")))
		{
			inet_addr_((uint8_t*)param, myipWIZNETINFO.sn);
			//printf("Subnet: %d.%d.%d.%d\r\n",myipWIZNETINFO.sn[0],myipWIZNETINFO.sn[1],myipWIZNETINFO.sn[2],myipWIZNETINFO.sn[3]);
		}
		//boc tach DNS
		if((param = get_http_param_value((char *)uri, "dns")))
		{
			inet_addr_((uint8_t*)param, myipWIZNETINFO.dns);
		}
		//boc tach NTP server IP
		if((param = get_http_param_value((char *)uri, "ntpip")))
		{
			inet_addr_((uint8_t*)param, ntpTimeServer_ip);
			//printf("NTP server IP: %d.%d.%d.%d\r\n",ntpTimeServer_ip[0],ntpTimeServer_ip[1],ntpTimeServer_ip[2],ntpTimeServer_ip[3]);
		}
		//light
		if((param = get_http_param_value((char *)uri, "light")))
		{
			inet_addr_((uint8_t*)param, &LEDintensity);
			if(LEDintensity > 15) LEDintensity = 15;
			if(LEDintensity < 1) LEDintensity = 1;
		}
	return ip;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void make_cgi_basic_config_response_page(uint16_t delay, uint8_t * url, uint8_t * cgi_response_buf, uint16_t * len)
{
	//S2E_Packet *value = get_S2E_Packet_pointer();

	//printf("Open IP: %d.%d.%d.%d\r\n",value->network_info_common.local_ip[0],value->network_info_common.local_ip[1],value->network_info_common.local_ip[2],value->network_info_common.local_ip[3]);
	//printf("Open gateway: %d.%d.%d.%d\r\n",value->network_info_common.gateway[0],value->network_info_common.gateway[1],value->network_info_common.gateway[2],value->network_info_common.gateway[3]);
	//printf("Open subnet: %d.%d.%d.%d\r\n",value->network_info_common.subnet[0],value->network_info_common.subnet[1],value->network_info_common.subnet[2],value->network_info_common.subnet[3]);
	
	*len = sprintf((char *)cgi_response_buf,"<html><head><title>Network Configuration</title><script language=javascript>j=%d;function func(){document.getElementById('delay').innerText=' '+j+' ';if(j>0)j--;setTimeout('func()',1000);if(j<=0)location.href='http://%d.%d.%d.%d';}</script></head><body onload='func()'>Please wait for a while, the module will boot in<span style='color:red;' id='delay'></span> seconds.</body></html>", delay, url[0], url[1], url[2], url[3]);
	//printf("\r\n%s\r\n",cgi_response_buf);
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void saveip()
{		
		//printf("try to store to mem...\r\n");
		stm32g474flashEraseThenSave();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


uint8_t http_post_cgi_handler(uint8_t * uri_name, st_http_request * p_http_request, uint8_t * buf, uint32_t * file_len)
{
	uint8_t ret = HTTP_OK;
	uint16_t len = 0;
	uint8_t val = 0;


//	if(predefined_set_cgi_processor(uri_name, p_http_request->URI, buf, &len))
//	{
//		;
//	}
//	else 
	
	if(strcmp((const char *)uri_name, "config.cgi") == 0)
	{
		
		//newip		= set_basic_config_setting(p_http_request->URI);
    set_basic_config_setting(p_http_request->URI);
		//Kiem tra xem IP con chuan ko thi moi luu?
		saveip();
		make_cgi_basic_config_response_page(5, myipWIZNETINFO.ip, buf, &len);
		ret = HTTP_RESET;
	}
	else
	{
		// CGI file not found
		//printf("CGI file not found\r\n");
		ret = HTTP_FAILED;
	}

	if(ret)	*file_len = len;
	return ret;
}

//Tao file json de phan hoi
void make_json_netinfo(uint8_t * buf, uint16_t * len)
{
	wiz_NetInfo netinfo;
	ctlnetwork(CN_GET_NETINFO, (void*) &netinfo);
	//printf("sec :%d",seconds);
	
//{"mac":"00:08:DC:4F:EB:6E","txtip":"192.168.1.246","gw":"192.168.1.1","txtsn":"255.255.255.1","dns":"8.8.8.8","dhcp":"1","txtdays":"21","txtmonths":"01","txtyears":"2019","txthours":"01","txtminutes":"01","txtseconds":"01","txtgps01":"ON","txtgps02":"OFF","txtpower01":"ON","txtpower02":"OFF"}
	// DHCP: 1 - Static, 2 - DHCP
	if(lostSignal == LOST_RS485_GPS_MASTER)
	{
		*len = sprintf((char *)buf, "{\"mac\":\"%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\",\"txtip\":\"%d.%d.%d.%d\",\"gw\":\"%d.%d.%d.%d\",\"txtsn\":\"%d.%d.%d.%d\",\"dns\":\"%d.%d.%d.%d\",\"dhcp\":\"%d\",\"txtdays\":\"%d\",\"txtmonths\":\"%d\",\"txtyears\":\"%d\",\"txthours\":\"%d\",\"txtminutes\":\"%d\",\"txtseconds\":\"%d\",\"txtgps01\":\"%s\",\"txtgps02\":\"%s\",\"txtpower01\":\"%s\",\"txtpower02\":\"%s\"}",
											netinfo.mac[0], netinfo.mac[1], netinfo.mac[2], netinfo.mac[3], netinfo.mac[4], netinfo.mac[5],
											netinfo.ip[0], netinfo.ip[1], netinfo.ip[2], netinfo.ip[3],
											netinfo.gw[0], netinfo.gw[1], netinfo.gw[2], netinfo.gw[3],
											ntpTimeServer_ip[0], ntpTimeServer_ip[1], ntpTimeServer_ip[2], ntpTimeServer_ip[3],
											netinfo.dns[0], netinfo.dns[1], netinfo.dns[2], netinfo.dns[3],
											netinfo.dhcp,
											days,
											months,
											years,
											hours,
											minutes,
											seconds,
											gps1_stt?"NO SIGNAL":"NO SIGNAL",
											gps2_stt?"NO SIGNAL":"NO SIGNAL",
											power1_stt?"NO SIGNAL":"NO SIGNAL",
											power2_stt?"NO SIGNAL":"NO SIGNAL"
											);
	}
	else
	{
		*len = sprintf((char *)buf, "{\"mac\":\"%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\",\"txtip\":\"%d.%d.%d.%d\",\"gw\":\"%d.%d.%d.%d\",\"txtsn\":\"%d.%d.%d.%d\",\"dns\":\"%d.%d.%d.%d\",\"dhcp\":\"%d\",\"txtdays\":\"%d\",\"txtmonths\":\"%d\",\"txtyears\":\"%d\",\"txthours\":\"%d\",\"txtminutes\":\"%d\",\"txtseconds\":\"%d\",\"txtgps01\":\"%s\",\"txtgps02\":\"%s\",\"txtpower01\":\"%s\",\"txtpower02\":\"%s\"}",
											netinfo.mac[0], netinfo.mac[1], netinfo.mac[2], netinfo.mac[3], netinfo.mac[4], netinfo.mac[5],
											netinfo.ip[0], netinfo.ip[1], netinfo.ip[2], netinfo.ip[3],
											netinfo.gw[0], netinfo.gw[1], netinfo.gw[2], netinfo.gw[3],
//										netinfo.sn[0], netinfo.sn[1], netinfo.sn[2], netinfo.sn[3],
											ntpTimeServer_ip[0], ntpTimeServer_ip[1], ntpTimeServer_ip[2], ntpTimeServer_ip[3],
											netinfo.dns[0], netinfo.dns[1], netinfo.dns[2], netinfo.dns[3],
											netinfo.dhcp,
											days,
											months,
											years,
											hours,
											minutes,
											seconds,
											gps1_stt?"ON":"OFF",
											gps2_stt?"ON":"OFF",
											power1_stt?"ON":"OFF",
											power2_stt?"ON":"OFF"
											);
	}		
	
}

