#ifndef __GPS
#define __GPS


#define	GPS1_USART	&huart1
#define	GPS2_USART	&huart2
#define	GPSBUFSIZE 255

extern char gps1_time[7];
extern char gps2_time[7];

extern char gps1_date[7];
extern char gps2_date[7];

extern char gps1_valid[2];
extern char gps2_valid[2];



void GPS_Init(void);
void GPS_UART_CallBack(void);
void GPS2_UART_CallBack(void);

int GPS_validate(char *nmeastr);


#endif


