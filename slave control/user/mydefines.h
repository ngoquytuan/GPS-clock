//Chon 1 trong 3 loai
//#define SLAVE_WALL
//#define SLAVE_CONSOLE
#define SLAVE_MATRIX

#ifdef SLAVE_WALL
	#ifdef SLAVE_CONSOLE
	ERROR: Loi roi
	#endif
	#ifdef SLAVE_MATRIX
	ERROR: Loi roi
	#endif
#endif
#ifdef SLAVE_CONSOLE
	#ifdef SLAVE_WALL
	ERROR: Loi roi
	#endif
	#ifdef SLAVE_MATRIX
	ERROR: Loi roi
	#endif
#endif

#define LOST_RS485_GPS_MASTER 0
#define RS485_GPS_MASTER_OK   1
//#define DebugEnable

#define PeriodSaveRTC   39
#define Time_to_SaveRTC 0

#define SYNC_TIMEOUT  16
#define SYNC_OUTofTIME 0

//#define fractionOfSecond TIM1->CNT
#define fractionOfSecond TIM1->CNT
#define counterOf10Second TIM2->CNT

#define JustHigh 1
#define Done     0

#define PHYStatus_check_enable 1
#define MSEC_PHYSTATUS_CHECK 		1000		// msec
#define UART_IS_HALT 10
//#define OverTheAir



