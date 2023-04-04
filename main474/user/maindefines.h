
#define PeriodSaveRTC 96
//do dem 10 giay de do do tre 
//#define counterOf10Second TIM2->CNT
#define fractionOfSecond TIM3->CNT
/* Size of Reception buffer */
#define RX1BUFFERSIZE                      1000
#define RX2BUFFERSIZE                      1000
#define RX3BUFFERSIZE                      100

// hieu chuan gia tri dien ap
#define k12v 		0.0061760164693773
#define k5v 		0.0025138260432378
#define kDCin1 	0.011
#define kDCin2 	0.013

//Thoi gian tu luc bat may toi khi co du lieu GPS co the tin cay
#define GPS_Stable_counter 29

//Time sources
#define RTC  0
#define GPS1 1
#define GPS2 2

#define  use_done    0
#define  JustHigh    1

#define  UNSENT   0
#define  SENT     1


