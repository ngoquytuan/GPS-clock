//#include "stm32g4xx_hal.h"
#include <stdint.h>

#define IRIGB_bit0 0
#define IRIGB_bit1 1
#define IRIGB_bitP 2

unsigned char irigb_code[100]={	2,	0,	0,	0,	0,	0,	0,	0,	0,	2,//Second
								0,	0,	0,	0,	0,	0,	0,	0,	0,	2,//Minite
								0,	0,	0,	0,	0,	0,	0,	0,	0,	2,//Hour
								0,	0,	0,	0,	0,	0,	0,	0,	0,	2,//Day
								0,	0,	0,	0,	0,	0,	0,	0,	0,	2,//Day
								0,	0,	0,	0,	0,	0,	0,	0,	0,	2,//Year
								0,	0,	0,	0,	0,	0,	0,	0,	0,	2,//Control
								0,	0,	0,	0,	0,	0,	0,	0,	0,	2,//Control
								0,	0,	0,	0,	0,	0,	0,	0,	0,	2,//TimeOfDay
								0,	0,	0,	0,	0,	0,	0,	0,	0,	2};//TimeOfDay

const uint32_t _Bit0[1280]= {2048 ,2148 ,2249 ,2349 ,2448 ,2546 ,2643 ,2738 ,2832 ,2924 ,3013 ,3101 ,3186 ,3268 ,3347 ,3423 ,3496 ,3565 ,3631 ,3693 ,3751 ,3805 ,3854 ,3899 ,3940 ,3976 ,4008 ,4035 ,4057 ,
	4074 ,4086 ,4094 ,4095 ,4094 ,4086 ,4074 ,4057 ,4035 ,4008 ,3976 ,3940 ,3899 ,3854 ,3805 ,3751 ,3693 ,3631 ,3565 ,3496 ,3423 ,3347 ,3268 ,3186 ,3101 ,3013 ,2924 ,2832 ,2738 ,2643 ,2546 ,2448 ,
	2349 ,2249 ,2148 ,2048 ,1948 ,1847 ,1747 ,1648 ,1550 ,1453 ,1358 ,1264 ,1172 ,1083 ,995 ,910 ,828 ,749 ,673 ,600 ,531 ,465 ,403 ,345 ,291 ,242 ,197 ,156 ,120 ,88 ,61 ,39 ,22 ,10 ,2 ,0 ,2 ,10 ,
	22 ,39 ,61 ,88 ,120 ,156 ,197 ,242 ,291 ,345 ,403 ,465 ,531 ,600 ,673 ,749 ,828 ,910 ,995 ,1083 ,1172 ,1264 ,1358 ,1453 ,1550 ,1648 ,1747 ,1847 ,1948 ,2048 ,2148 ,2249 ,2349 ,2448 ,2546 ,2643 ,
	2738 ,2832 ,2924 ,3013 ,3101 ,3186 ,3268 ,3347 ,3423 ,3496 ,3565 ,3631 ,3693 ,3751 ,3805 ,3854 ,3899 ,3940 ,3976 ,4008 ,4035 ,4057 ,4074 ,4086 ,4094 ,4095 ,4094 ,4086 ,4074 ,4057 ,4035 ,4008 ,
	3976 ,3940 ,3899 ,3854 ,3805 ,3751 ,3693 ,3631 ,3565 ,3496 ,3423 ,3347 ,3268 ,3186 ,3101 ,3013 ,2924 ,2832 ,2738 ,2643 ,2546 ,2448 ,2349 ,2249 ,2148 ,2048 ,1948 ,1847 ,1747 ,1648 ,1550 ,1453 ,
	1358 ,1264 ,1172 ,1083 ,995 ,910 ,828 ,749 ,673 ,600 ,531 ,465 ,403 ,345 ,291 ,242 ,197 ,156 ,120 ,88 ,61 ,39 ,22 ,10 ,2 ,0 ,2 ,10 ,22 ,39 ,61 ,88 ,120 ,156 ,197 ,242 ,291 ,345 ,403 ,465 ,531 ,
	600 ,673 ,749 ,828 ,910 ,995 ,1083 ,1172 ,1264 ,1358 ,1453 ,1550 ,1648 ,1747 ,1847 ,1948 ,2048 ,2081 ,2115 ,2148 ,2181 ,2214 ,2246 ,2278 ,2309 ,2340 ,2370 ,2399 ,2427 ,2455 ,2481 ,2506 ,2531 ,
	2554 ,2576 ,2596 ,2616 ,2634 ,2650 ,2665 ,2679 ,2691 ,2701 ,2710 ,2718 ,2723 ,2727 ,2730 ,2731 ,2730 ,2727 ,2723 ,2718 ,2710 ,2701 ,2691 ,2679 ,2665 ,2650 ,2634 ,2616 ,2596 ,2576 ,2554 ,2531 ,
	2506 ,2481 ,2455 ,2427 ,2399 ,2370 ,2340 ,2309 ,2278 ,2246 ,2214 ,2181 ,2148 ,2115 ,2081 ,2048 ,2015 ,1981 ,1948 ,1915 ,1882 ,1850 ,1818 ,1787 ,1756 ,1726 ,1697 ,1669 ,1641 ,1615 ,1590 ,1565 ,
	1542 ,1520 ,1500 ,1480 ,1462 ,1446 ,1431 ,1417 ,1405 ,1395 ,1386 ,1378 ,1373 ,1369 ,1366 ,1365 ,1366 ,1369 ,1373 ,1378 ,1386 ,1395 ,1405 ,1417 ,1431 ,1446 ,1462 ,1480 ,1500 ,1520 ,1542 ,1565 ,
	1590 ,1615 ,1641 ,1669 ,1697 ,1726 ,1756 ,1787 ,1818 ,1850 ,1882 ,1915 ,1948 ,1981 ,2015 ,2048 ,2081 ,2115 ,2148 ,2181 ,2214 ,2246 ,2278 ,2309 ,2340 ,2370 ,2399 ,2427 ,2455 ,2481 ,2506 ,2531 ,
	2554 ,2576 ,2596 ,2616 ,2634 ,2650 ,2665 ,2679 ,2691 ,2701 ,2710 ,2718 ,2723 ,2727 ,2730 ,2731 ,2730 ,2727 ,2723 ,2718 ,2710 ,2701 ,2691 ,2679 ,2665 ,2650 ,2634 ,2616 ,2596 ,2576 ,2554 ,2531 ,
	2506 ,2481 ,2455 ,2427 ,2399 ,2370 ,2340 ,2309 ,2278 ,2246 ,2214 ,2181 ,2148 ,2115 ,2081 ,2048 ,2015 ,1981 ,1948 ,1915 ,1882 ,1850 ,1818 ,1787 ,1756 ,1726 ,1697 ,1669 ,1641 ,1615 ,1590 ,1565 ,
	1542 ,1520 ,1500 ,1480 ,1462 ,1446 ,1431 ,1417 ,1405 ,1395 ,1386 ,1378 ,1373 ,1369 ,1366 ,1365 ,1366 ,1369 ,1373 ,1378 ,1386 ,1395 ,1405 ,1417 ,1431 ,1446 ,1462 ,1480 ,1500 ,1520 ,1542 ,1565 ,
	1590 ,1615 ,1641 ,1669 ,1697 ,1726 ,1756 ,1787 ,1818 ,1850 ,1882 ,1915 ,1948 ,1981 ,2015 ,2048 ,2081 ,2115 ,2148 ,2181 ,2214 ,2246 ,2278 ,2309 ,2340 ,2370 ,2399 ,2427 ,2455 ,2481 ,2506 ,2531 ,
	2554 ,2576 ,2596 ,2616 ,2634 ,2650 ,2665 ,2679 ,2691 ,2701 ,2710 ,2718 ,2723 ,2727 ,2730 ,2731 ,2730 ,2727 ,2723 ,2718 ,2710 ,2701 ,2691 ,2679 ,2665 ,2650 ,2634 ,2616 ,2596 ,2576 ,2554 ,2531 ,
	2506 ,2481 ,2455 ,2427 ,2399 ,2370 ,2340 ,2309 ,2278 ,2246 ,2214 ,2181 ,2148 ,2115 ,2081 ,2048 ,2015 ,1981 ,1948 ,1915 ,1882 ,1850 ,1818 ,1787 ,1756 ,1726 ,1697 ,1669 ,1641 ,1615 ,1590 ,1565 ,
	1542 ,1520 ,1500 ,1480 ,1462 ,1446 ,1431 ,1417 ,1405 ,1395 ,1386 ,1378 ,1373 ,1369 ,1366 ,1365 ,1366 ,1369 ,1373 ,1378 ,1386 ,1395 ,1405 ,1417 ,1431 ,1446 ,1462 ,1480 ,1500 ,1520 ,1542 ,1565 ,
	1590 ,1615 ,1641 ,1669 ,1697 ,1726 ,1756 ,1787 ,1818 ,1850 ,1882 ,1915 ,1948 ,1981 ,2015 ,2048 ,2081 ,2115 ,2148 ,2181 ,2214 ,2246 ,2278 ,2309 ,2340 ,2370 ,2399 ,2427 ,2455 ,2481 ,2506 ,2531 ,
	2554 ,2576 ,2596 ,2616 ,2634 ,2650 ,2665 ,2679 ,2691 ,2701 ,2710 ,2718 ,2723 ,2727 ,2730 ,2731 ,2730 ,2727 ,2723 ,2718 ,2710 ,2701 ,2691 ,2679 ,2665 ,2650 ,2634 ,2616 ,2596 ,2576 ,2554 ,2531 ,
	2506 ,2481 ,2455 ,2427 ,2399 ,2370 ,2340 ,2309 ,2278 ,2246 ,2214 ,2181 ,2148 ,2115 ,2081 ,2048 ,2015 ,1981 ,1948 ,1915 ,1882 ,1850 ,1818 ,1787 ,1756 ,1726 ,1697 ,1669 ,1641 ,1615 ,1590 ,1565 ,
	1542 ,1520 ,1500 ,1480 ,1462 ,1446 ,1431 ,1417 ,1405 ,1395 ,1386 ,1378 ,1373 ,1369 ,1366 ,1365 ,1366 ,1369 ,1373 ,1378 ,1386 ,1395 ,1405 ,1417 ,1431 ,1446 ,1462 ,1480 ,1500 ,1520 ,1542 ,1565 ,
	1590 ,1615 ,1641 ,1669 ,1697 ,1726 ,1756 ,1787 ,1818 ,1850 ,1882 ,1915 ,1948 ,1981 ,2015 ,2048 ,2081 ,2115 ,2148 ,2181 ,2214 ,2246 ,2278 ,2309 ,2340 ,2370 ,2399 ,2427 ,2455 ,2481 ,2506 ,2531 ,
	2554 ,2576 ,2596 ,2616 ,2634 ,2650 ,2665 ,2679 ,2691 ,2701 ,2710 ,2718 ,2723 ,2727 ,2730 ,2731 ,2730 ,2727 ,2723 ,2718 ,2710 ,2701 ,2691 ,2679 ,2665 ,2650 ,2634 ,2616 ,2596 ,2576 ,2554 ,2531 ,
	2506 ,2481 ,2455 ,2427 ,2399 ,2370 ,2340 ,2309 ,2278 ,2246 ,2214 ,2181 ,2148 ,2115 ,2081 ,2048 ,2015 ,1981 ,1948 ,1915 ,1882 ,1850 ,1818 ,1787 ,1756 ,1726 ,1697 ,1669 ,1641 ,1615 ,1590 ,1565 ,
	1542 ,1520 ,1500 ,1480 ,1462 ,1446 ,1431 ,1417 ,1405 ,1395 ,1386 ,1378 ,1373 ,1369 ,1366 ,1365 ,1366 ,1369 ,1373 ,1378 ,1386 ,1395 ,1405 ,1417 ,1431 ,1446 ,1462 ,1480 ,1500 ,1520 ,1542 ,1565 ,
	1590 ,1615 ,1641 ,1669 ,1697 ,1726 ,1756 ,1787 ,1818 ,1850 ,1882 ,1915 ,1948 ,1981 ,2015 ,2048 ,2081 ,2115 ,2148 ,2181 ,2214 ,2246 ,2278 ,2309 ,2340 ,2370 ,2399 ,2427 ,2455 ,2481 ,2506 ,2531 ,
	2554 ,2576 ,2596 ,2616 ,2634 ,2650 ,2665 ,2679 ,2691 ,2701 ,2710 ,2718 ,2723 ,2727 ,2730 ,2731 ,2730 ,2727 ,2723 ,2718 ,2710 ,2701 ,2691 ,2679 ,2665 ,2650 ,2634 ,2616 ,2596 ,2576 ,2554 ,2531 ,
	2506 ,2481 ,2455 ,2427 ,2399 ,2370 ,2340 ,2309 ,2278 ,2246 ,2214 ,2181 ,2148 ,2115 ,2081 ,2048 ,2015 ,1981 ,1948 ,1915 ,1882 ,1850 ,1818 ,1787 ,1756 ,1726 ,1697 ,1669 ,1641 ,1615 ,1590 ,1565 ,
	1542 ,1520 ,1500 ,1480 ,1462 ,1446 ,1431 ,1417 ,1405 ,1395 ,1386 ,1378 ,1373 ,1369 ,1366 ,1365 ,1366 ,1369 ,1373 ,1378 ,1386 ,1395 ,1405 ,1417 ,1431 ,1446 ,1462 ,1480 ,1500 ,1520 ,1542 ,1565 ,
	1590 ,1615 ,1641 ,1669 ,1697 ,1726 ,1756 ,1787 ,1818 ,1850 ,1882 ,1915 ,1948 ,1981 ,2015 ,2048 ,2081 ,2115 ,2148 ,2181 ,2214 ,2246 ,2278 ,2309 ,2340 ,2370 ,2399 ,2427 ,2455 ,2481 ,2506 ,2531 ,
	2554 ,2576 ,2596 ,2616 ,2634 ,2650 ,2665 ,2679 ,2691 ,2701 ,2710 ,2718 ,2723 ,2727 ,2730 ,2731 ,2730 ,2727 ,2723 ,2718 ,2710 ,2701 ,2691 ,2679 ,2665 ,2650 ,2634 ,2616 ,2596 ,2576 ,2554 ,2531 ,
	2506 ,2481 ,2455 ,2427 ,2399 ,2370 ,2340 ,2309 ,2278 ,2246 ,2214 ,2181 ,2148 ,2115 ,2081 ,2048 ,2015 ,1981 ,1948 ,1915 ,1882 ,1850 ,1818 ,1787 ,1756 ,1726 ,1697 ,1669 ,1641 ,1615 ,1590 ,1565 ,
	1542 ,1520 ,1500 ,1480 ,1462 ,1446 ,1431 ,1417 ,1405 ,1395 ,1386 ,1378 ,1373 ,1369 ,1366 ,1365 ,1366 ,1369 ,1373 ,1378 ,1386 ,1395 ,1405 ,1417 ,1431 ,1446 ,1462 ,1480 ,1500 ,1520 ,1542 ,1565 ,
	1590 ,1615 ,1641 ,1669 ,1697 ,1726 ,1756 ,1787 ,1818 ,1850 ,1882 ,1915 ,1948 ,1981 ,2015 ,2048 ,2081 ,2115 ,2148 ,2181 ,2214 ,2246 ,2278 ,2309 ,2340 ,2370 ,2399 ,2427 ,2455 ,2481 ,2506 ,2531 ,
	2554 ,2576 ,2596 ,2616 ,2634 ,2650 ,2665 ,2679 ,2691 ,2701 ,2710 ,2718 ,2723 ,2727 ,2730 ,2731 ,2730 ,2727 ,2723 ,2718 ,2710 ,2701 ,2691 ,2679 ,2665 ,2650 ,2634 ,2616 ,2596 ,2576 ,2554 ,2531 ,
	2506 ,2481 ,2455 ,2427 ,2399 ,2370 ,2340 ,2309 ,2278 ,2246 ,2214 ,2181 ,2148 ,2115 ,2081 ,2048 ,2015 ,1981 ,1948 ,1915 ,1882 ,1850 ,1818 ,1787 ,1756 ,1726 ,1697 ,1669 ,1641 ,1615 ,1590 ,1565 ,
	1542 ,1520 ,1500 ,1480 ,1462 ,1446 ,1431 ,1417 ,1405 ,1395 ,1386 ,1378 ,1373 ,1369 ,1366 ,1365 ,1366 ,1369 ,1373 ,1378 ,1386 ,1395 ,1405 ,1417 ,1431 ,1446 ,1462 ,1480 ,1500 ,1520 ,1542 ,1565 ,
	1590 ,1615 ,1641 ,1669 ,1697 ,1726 ,1756 ,1787 ,1818 ,1850 ,1882 ,1915 ,1948 ,1981 ,2015
};
const uint32_t _BitREF[1280] = {2048 ,2148 ,2249 ,2349 ,2448 ,2546 ,2643 ,2738 ,2832 ,2924 ,3013 ,3101 ,3186 ,3268 ,3347 ,3423 ,3496 ,3565 ,3631 ,3693 ,3751 ,3805 ,3854 ,3899 ,3940 ,3976 ,4008 ,4035 ,4057 ,4074 ,4086 ,4094 ,4095 ,4094 ,4086 ,4074 ,4057 ,4035 ,4008 ,
3976 ,3940 ,3899 ,3854 ,3805 ,3751 ,3693 ,3631 ,3565 ,3496 ,3423 ,3347 ,3268 ,3186 ,3101 ,3013 ,2924 ,2832 ,2738 ,2643 ,2546 ,2448 ,2349 ,2249 ,2148 ,2048 ,1948 ,1847 ,1747 ,1648 ,1550 ,1453 ,1358 ,1264 ,1172 ,1083 ,995 ,910 ,828 ,749 ,
673 ,600 ,531 ,465 ,403 ,345 ,291 ,242 ,197 ,156 ,120 ,88 ,61 ,39 ,22 ,10 ,2 ,0 ,2 ,10 ,22 ,39 ,61 ,88 ,120 ,156 ,197 ,242 ,291 ,345 ,403 ,465 ,531 ,600 ,673 ,749 ,828 ,910 ,995 ,1083 ,1172 ,1264 ,1358 ,1453 ,1550 ,1648 ,1747 ,1847 ,
1948 ,2048 ,2148 ,2249 ,2349 ,2448 ,2546 ,2643 ,2738 ,2832 ,2924 ,3013 ,3101 ,3186 ,3268 ,3347 ,3423 ,3496 ,3565 ,3631 ,3693 ,3751 ,3805 ,3854 ,3899 ,3940 ,3976 ,4008 ,4035 ,4057 ,4074 ,4086 ,4094 ,4095 ,4094 ,4086 ,4074 ,4057 ,4035 ,
4008 ,3976 ,3940 ,3899 ,3854 ,3805 ,3751 ,3693 ,3631 ,3565 ,3496 ,3423 ,3347 ,3268 ,3186 ,3101 ,3013 ,2924 ,2832 ,2738 ,2643 ,2546 ,2448 ,2349 ,2249 ,2148 ,2048 ,1948 ,1847 ,1747 ,1648 ,1550 ,1453 ,1358 ,1264 ,1172 ,1083 ,995 ,910 ,828 ,
749 ,673 ,600 ,531 ,465 ,403 ,345 ,291 ,242 ,197 ,156 ,120 ,88 ,61 ,39 ,22 ,10 ,2 ,0 ,2 ,10 ,22 ,39 ,61 ,88 ,120 ,156 ,197 ,242 ,291 ,345 ,403 ,465 ,531 ,600 ,673 ,749 ,828 ,910 ,995 ,1083 ,1172 ,1264 ,1358 ,1453 ,1550 ,1648 ,1747 ,1847 ,
1948 ,2048 ,2148 ,2249 ,2349 ,2448 ,2546 ,2643 ,2738 ,2832 ,2924 ,3013 ,3101 ,3186 ,3268 ,3347 ,3423 ,3496 ,3565 ,3631 ,3693 ,3751 ,3805 ,3854 ,3899 ,3940 ,3976 ,4008 ,4035 ,4057 ,4074 ,4086 ,4094 ,4095 ,4094 ,4086 ,4074 ,4057 ,4035 ,
4008 ,3976 ,3940 ,3899 ,3854 ,3805 ,3751 ,3693 ,3631 ,3565 ,3496 ,3423 ,3347 ,3268 ,3186 ,3101 ,3013 ,2924 ,2832 ,2738 ,2643 ,2546 ,2448 ,2349 ,2249 ,2148 ,2048 ,1948 ,1847 ,1747 ,1648 ,1550 ,1453 ,1358 ,1264 ,1172 ,1083 ,995 ,910 ,
828 ,749 ,673 ,600 ,531 ,465 ,403 ,345 ,291 ,242 ,197 ,156 ,120 ,88 ,61 ,39 ,22 ,10 ,2 ,0 ,2 ,10 ,22 ,39 ,61 ,88 ,120 ,156 ,197 ,242 ,291 ,345 ,403 ,465 ,531 ,600 ,673 ,749 ,828 ,910 ,995 ,1083 ,1172 ,1264 ,1358 ,1453 ,1550 ,1648 ,
1747 ,1847 ,1948 ,2048 ,2148 ,2249 ,2349 ,2448 ,2546 ,2643 ,2738 ,2832 ,2924 ,3013 ,3101 ,3186 ,3268 ,3347 ,3423 ,3496 ,3565 ,3631 ,3693 ,3751 ,3805 ,3854 ,3899 ,3940 ,3976 ,4008 ,4035 ,4057 ,4074 ,4086 ,4094 ,4095 ,4094 ,4086 ,4074 ,
4057 ,4035 ,4008 ,3976 ,3940 ,3899 ,3854 ,3805 ,3751 ,3693 ,3631 ,3565 ,3496 ,3423 ,3347 ,3268 ,3186 ,3101 ,3013 ,2924 ,2832 ,2738 ,2643 ,2546 ,2448 ,2349 ,2249 ,2148 ,2048 ,1948 ,1847 ,1747 ,1648 ,1550 ,1453 ,1358 ,1264 ,1172 ,1083 ,
995 ,910 ,828 ,749 ,673 ,600 ,531 ,465 ,403 ,345 ,291 ,242 ,197 ,156 ,120 ,88 ,61 ,39 ,22 ,10 ,2 ,0 ,2 ,10 ,22 ,39 ,61 ,88 ,120 ,156 ,197 ,242 ,291 ,345 ,403 ,465 ,531 ,600 ,673 ,749 ,828 ,910 ,995 ,1083 ,1172 ,1264 ,1358 ,1453 ,1550 ,
1648 ,1747 ,1847 ,1948 ,2048 ,2148 ,2249 ,2349 ,2448 ,2546 ,2643 ,2738 ,2832 ,2924 ,3013 ,3101 ,3186 ,3268 ,3347 ,3423 ,3496 ,3565 ,3631 ,3693 ,3751 ,3805 ,3854 ,3899 ,3940 ,3976 ,4008 ,4035 ,4057 ,4074 ,4086 ,4094 ,4095 ,4094 ,4086 ,
4074 ,4057 ,4035 ,4008 ,3976 ,3940 ,3899 ,3854 ,3805 ,3751 ,3693 ,3631 ,3565 ,3496 ,3423 ,3347 ,3268 ,3186 ,3101 ,3013 ,2924 ,2832 ,2738 ,2643 ,2546 ,2448 ,2349 ,2249 ,2148 ,2048 ,1948 ,1847 ,1747 ,1648 ,1550 ,1453 ,1358 ,1264 ,1172 ,
1083 ,995 ,910 ,828 ,749 ,673 ,600 ,531 ,465 ,403 ,345 ,291 ,242 ,197 ,156 ,120 ,88 ,61 ,39 ,22 ,10 ,2 ,0 ,2 ,10 ,22 ,39 ,61 ,88 ,120 ,156 ,197 ,242 ,291 ,345 ,403 ,465 ,531 ,600 ,673 ,749 ,828 ,910 ,995 ,1083 ,1172 ,1264 ,1358 ,1453 ,
1550 ,1648 ,1747 ,1847 ,1948 ,2048 ,2148 ,2249 ,2349 ,2448 ,2546 ,2643 ,2738 ,2832 ,2924 ,3013 ,3101 ,3186 ,3268 ,3347 ,3423 ,3496 ,3565 ,3631 ,3693 ,3751 ,3805 ,3854 ,3899 ,3940 ,3976 ,4008 ,4035 ,4057 ,4074 ,4086 ,4094 ,4095 ,4094 ,
4086 ,4074 ,4057 ,4035 ,4008 ,3976 ,3940 ,3899 ,3854 ,3805 ,3751 ,3693 ,3631 ,3565 ,3496 ,3423 ,3347 ,3268 ,3186 ,3101 ,3013 ,2924 ,2832 ,2738 ,2643 ,2546 ,2448 ,2349 ,2249 ,2148 ,2048 ,1948 ,1847 ,1747 ,1648 ,1550 ,1453 ,1358 ,1264 ,
1172 ,1083 ,995 ,910 ,828 ,749 ,673 ,600 ,531 ,465 ,403 ,345 ,291 ,242 ,197 ,156 ,120 ,88 ,61 ,39 ,22 ,10 ,2 ,0 ,2 ,10 ,22 ,39 ,61 ,88 ,120 ,156 ,197 ,242 ,291 ,345 ,403 ,465 ,531 ,600 ,673 ,749 ,828 ,910 ,995 ,1083 ,1172 ,1264 ,1358 ,
1453 ,1550 ,1648 ,1747 ,1847 ,1948 ,2048 ,2148 ,2249 ,2349 ,2448 ,2546 ,2643 ,2738 ,2832 ,2924 ,3013 ,3101 ,3186 ,3268 ,3347 ,3423 ,3496 ,3565 ,3631 ,3693 ,3751 ,3805 ,3854 ,3899 ,3940 ,3976 ,4008 ,4035 ,4057 ,4074 ,4086 ,4094 ,4095 ,
4094 ,4086 ,4074 ,4057 ,4035 ,4008 ,3976 ,3940 ,3899 ,3854 ,3805 ,3751 ,3693 ,3631 ,3565 ,3496 ,3423 ,3347 ,3268 ,3186 ,3101 ,3013 ,2924 ,2832 ,2738 ,2643 ,2546 ,2448 ,2349 ,2249 ,2148 ,2048 ,1948 ,1847 ,1747 ,1648 ,1550 ,1453 ,1358 ,
1264 ,1172 ,1083 ,995 ,910 ,828 ,749 ,673 ,600 ,531 ,465 ,403 ,345 ,291 ,242 ,197 ,156 ,120 ,88 ,61 ,39 ,22 ,10 ,2 ,0 ,2 ,10 ,22 ,39 ,61 ,88 ,120 ,156 ,197 ,242 ,291 ,345 ,403 ,465 ,531 ,600 ,673 ,749 ,828 ,910 ,995 ,1083 ,1172 ,1264 ,
1358 ,1453 ,1550 ,1648 ,1747 ,1847 ,1948 ,2048 ,2148 ,2249 ,2349 ,2448 ,2546 ,2643 ,2738 ,2832 ,2924 ,3013 ,3101 ,3186 ,3268 ,3347 ,3423 ,3496 ,3565 ,3631 ,3693 ,3751 ,3805 ,3854 ,3899 ,3940 ,3976 ,4008 ,4035 ,4057 ,4074 ,4086 ,4094 ,
4095 ,4094 ,4086 ,4074 ,4057 ,4035 ,4008 ,3976 ,3940 ,3899 ,3854 ,3805 ,3751 ,3693 ,3631 ,3565 ,3496 ,3423 ,3347 ,3268 ,3186 ,3101 ,3013 ,2924 ,2832 ,2738 ,2643 ,2546 ,2448 ,2349 ,2249 ,2148 ,2048 ,1948 ,1847 ,1747 ,1648 ,1550 ,1453 ,
1358 ,1264 ,1172 ,1083 ,995 ,910 ,828 ,749 ,673 ,600 ,531 ,465 ,403 ,345 ,291 ,242 ,197 ,156 ,120 ,88 ,61 ,39 ,22 ,10 ,2 ,0 ,2 ,10 ,22 ,39 ,61 ,88 ,120 ,156 ,197 ,242 ,291 ,345 ,403 ,465 ,531 ,600 ,673 ,749 ,828 ,910 ,995 ,1083 ,1172 ,
1264 ,1358 ,1453 ,1550 ,1648 ,1747 ,1847 ,1948 ,2048 ,2081 ,2115 ,2148 ,2181 ,2214 ,2246 ,2278 ,2309 ,2340 ,2370 ,2399 ,2427 ,2455 ,2481 ,2506 ,2531 ,2554 ,2576 ,2596 ,2616 ,2634 ,2650 ,2665 ,2679 ,2691 ,2701 ,2710 ,2718 ,2723 ,2727 ,
2730 ,2731 ,2730 ,2727 ,2723 ,2718 ,2710 ,2701 ,2691 ,2679 ,2665 ,2650 ,2634 ,2616 ,2596 ,2576 ,2554 ,2531 ,2506 ,2481 ,2455 ,2427 ,2399 ,2370 ,2340 ,2309 ,2278 ,2246 ,2214 ,2181 ,2148 ,2115 ,2081 ,2048 ,2015 ,1981 ,1948 ,1915 ,1882 ,
1850 ,1818 ,1787 ,1756 ,1726 ,1697 ,1669 ,1641 ,1615 ,1590 ,1565 ,1542 ,1520 ,1500 ,1480 ,1462 ,1446 ,1431 ,1417 ,1405 ,1395 ,1386 ,1378 ,1373 ,1369 ,1366 ,1365 ,1366 ,1369 ,1373 ,1378 ,1386 ,1395 ,1405 ,1417 ,1431 ,1446 ,1462 ,1480 ,
1500 ,1520 ,1542 ,1565 ,1590 ,1615 ,1641 ,1669 ,1697 ,1726 ,1756 ,1787 ,1818 ,1850 ,1882 ,1915 ,1948 ,1981 ,2015 ,2048 ,2081 ,2115 ,2148 ,2181 ,2214 ,2246 ,2278 ,2309 ,2340 ,2370 ,2399 ,2427 ,2455 ,2481 ,2506 ,2531 ,2554 ,2576 ,2596 ,
2616 ,2634 ,2650 ,2665 ,2679 ,2691 ,2701 ,2710 ,2718 ,2723 ,2727 ,2730 ,2731 ,2730 ,2727 ,2723 ,2718 ,2710 ,2701 ,2691 ,2679 ,2665 ,2650 ,2634 ,2616 ,2596 ,2576 ,2554 ,2531 ,2506 ,2481 ,2455 ,2427 ,2399 ,2370 ,2340 ,2309 ,2278 ,2246 ,
2214 ,2181 ,2148 ,2115 ,2081 ,2048 ,2015 ,1981 ,1948 ,1915 ,1882 ,1850 ,1818 ,1787 ,1756 ,1726 ,1697 ,1669 ,1641 ,1615 ,1590 ,1565 ,1542 ,1520 ,1500 ,1480 ,1462 ,1446 ,1431 ,1417 ,1405 ,1395 ,1386 ,1378 ,1373 ,1369 ,1366 ,1365 ,1366 ,
1369 ,1373 ,1378 ,1386 ,1395 ,1405 ,1417 ,1431 ,1446 ,1462 ,1480 ,1500 ,1520 ,1542 ,1565 ,1590 ,1615 ,1641 ,1669 ,1697 ,1726 ,1756 ,1787 ,1818 ,1850 ,1882 ,1915 ,1948 ,1981 ,2015};
const uint32_t _Bit1[1280]= {2048 ,2148 ,2249 ,2349 ,2448 ,2546 ,2643 ,2738 ,2832 ,2924 ,3013 ,3101 ,3186 ,3268 ,3347 ,3423 ,3496 ,3565 ,3631 ,3693 ,3751 ,3805 ,3854 ,3899 ,3940 ,3976 ,4008 ,4035 ,4057 ,4074 ,4086 ,4094 ,4095 ,
4094 ,4086 ,4074 ,4057 ,4035 ,4008 ,3976 ,3940 ,3899 ,3854 ,3805 ,3751 ,3693 ,3631 ,3565 ,3496 ,3423 ,3347 ,3268 ,3186 ,3101 ,3013 ,2924 ,2832 ,2738 ,2643 ,2546 ,2448 ,2349 ,2249 ,2148 ,2048 ,1948 ,1847 ,1747 ,1648 ,1550 ,1453 ,
1358 ,1264 ,1172 ,1083 ,995 ,910 ,828 ,749 ,673 ,600 ,531 ,465 ,403 ,345 ,291 ,242 ,197 ,156 ,120 ,88 ,61 ,39 ,22 ,10 ,2 ,0 ,2 ,10 ,22 ,39 ,61 ,88 ,120 ,156 ,197 ,242 ,291 ,345 ,403 ,465 ,531 ,600 ,673 ,749 ,828 ,910 ,995 ,1083 ,
1172 ,1264 ,1358 ,1453 ,1550 ,1648 ,1747 ,1847 ,1948 ,2048 ,2148 ,2249 ,2349 ,2448 ,2546 ,2643 ,2738 ,2832 ,2924 ,3013 ,3101 ,3186 ,3268 ,3347 ,3423 ,3496 ,3565 ,3631 ,3693 ,3751 ,3805 ,3854 ,3899 ,3940 ,3976 ,4008 ,4035 ,4057 ,
4074 ,4086 ,4094 ,4095 ,4094 ,4086 ,4074 ,4057 ,4035 ,4008 ,3976 ,3940 ,3899 ,3854 ,3805 ,3751 ,3693 ,3631 ,3565 ,3496 ,3423 ,3347 ,3268 ,3186 ,3101 ,3013 ,2924 ,2832 ,2738 ,2643 ,2546 ,2448 ,2349 ,2249 ,2148 ,2048 ,1948 ,1847 ,
1747 ,1648 ,1550 ,1453 ,1358 ,1264 ,1172 ,1083 ,995 ,910 ,828 ,749 ,673 ,600 ,531 ,465 ,403 ,345 ,291 ,242 ,197 ,156 ,120 ,88 ,61 ,39 ,22 ,10 ,2 ,0 ,2 ,10 ,22 ,39 ,61 ,88 ,120 ,156 ,197 ,242 ,291 ,345 ,403 ,465 ,531 ,600 ,673 ,
749 ,828 ,910 ,995 ,1083 ,1172 ,1264 ,1358 ,1453 ,1550 ,1648 ,1747 ,1847 ,1948 ,2048 ,2148 ,2249 ,2349 ,2448 ,2546 ,2643 ,2738 ,2832 ,2924 ,3013 ,3101 ,3186 ,3268 ,3347 ,3423 ,3496 ,3565 ,3631 ,3693 ,3751 ,3805 ,3854 ,3899 ,3940 ,
3976 ,4008 ,4035 ,4057 ,4074 ,4086 ,4094 ,4095 ,4094 ,4086 ,4074 ,4057 ,4035 ,4008 ,3976 ,3940 ,3899 ,3854 ,3805 ,3751 ,3693 ,3631 ,3565 ,3496 ,3423 ,3347 ,3268 ,3186 ,3101 ,3013 ,2924 ,2832 ,2738 ,2643 ,2546 ,2448 ,2349 ,2249 ,
2148 ,2048 ,1948 ,1847 ,1747 ,1648 ,1550 ,1453 ,1358 ,1264 ,1172 ,1083 ,995 ,910 ,828 ,749 ,673 ,600 ,531 ,465 ,403 ,345 ,291 ,242 ,197 ,156 ,120 ,88 ,61 ,39 ,22 ,10 ,2 ,0 ,2 ,10 ,22 ,39 ,61 ,88 ,120 ,156 ,197 ,242 ,291 ,345 ,403 ,
465 ,531 ,600 ,673 ,749 ,828 ,910 ,995 ,1083 ,1172 ,1264 ,1358 ,1453 ,1550 ,1648 ,1747 ,1847 ,1948 ,2048 ,2148 ,2249 ,2349 ,2448 ,2546 ,2643 ,2738 ,2832 ,2924 ,3013 ,3101 ,3186 ,3268 ,3347 ,3423 ,3496 ,3565 ,3631 ,3693 ,3751 ,3805 ,
3854 ,3899 ,3940 ,3976 ,4008 ,4035 ,4057 ,4074 ,4086 ,4094 ,4095 ,4094 ,4086 ,4074 ,4057 ,4035 ,4008 ,3976 ,3940 ,3899 ,3854 ,3805 ,3751 ,3693 ,3631 ,3565 ,3496 ,3423 ,3347 ,3268 ,3186 ,3101 ,3013 ,2924 ,2832 ,2738 ,2643 ,2546 ,2448 ,
2349 ,2249 ,2148 ,2048 ,1948 ,1847 ,1747 ,1648 ,1550 ,1453 ,1358 ,1264 ,1172 ,1083 ,995 ,910 ,828 ,749 ,673 ,600 ,531 ,465 ,403 ,345 ,291 ,242 ,197 ,156 ,120 ,88 ,61 ,39 ,22 ,10 ,2 ,0 ,2 ,10 ,22 ,39 ,61 ,88 ,120 ,156 ,197 ,242 ,291 ,
345 ,403 ,465 ,531 ,600 ,673 ,749 ,828 ,910 ,995 ,1083 ,1172 ,1264 ,1358 ,1453 ,1550 ,1648 ,1747 ,1847 ,1948 ,2048 ,2148 ,2249 ,2349 ,2448 ,2546 ,2643 ,2738 ,2832 ,2924 ,3013 ,3101 ,3186 ,3268 ,3347 ,3423 ,3496 ,3565 ,3631 ,3693 ,3751 ,
3805 ,3854 ,3899 ,3940 ,3976 ,4008 ,4035 ,4057 ,4074 ,4086 ,4094 ,4095 ,4094 ,4086 ,4074 ,4057 ,4035 ,4008 ,3976 ,3940 ,3899 ,3854 ,3805 ,3751 ,3693 ,3631 ,3565 ,3496 ,3423 ,3347 ,3268 ,3186 ,3101 ,3013 ,2924 ,2832 ,2738 ,2643 ,2546 ,
2448 ,2349 ,2249 ,2148 ,2048 ,1948 ,1847 ,1747 ,1648 ,1550 ,1453 ,1358 ,1264 ,1172 ,1083 ,995 ,910 ,828 ,749 ,673 ,600 ,531 ,465 ,403 ,345 ,291 ,242 ,197 ,156 ,120 ,88 ,61 ,39 ,22 ,10 ,2 ,0 ,2 ,10 ,22 ,39 ,61 ,88 ,120 ,156 ,197 ,242 ,
291 ,345 ,403 ,465 ,531 ,600 ,673 ,749 ,828 ,910 ,995 ,1083 ,1172 ,1264 ,1358 ,1453 ,1550 ,1648 ,1747 ,1847 ,1948 ,2048 ,2081 ,2115 ,2148 ,2181 ,2214 ,2246 ,2278 ,2309 ,2340 ,2370 ,2399 ,2427 ,2455 ,2481 ,2506 ,2531 ,2554 ,2576 ,2596 ,
2616 ,2634 ,2650 ,2665 ,2679 ,2691 ,2701 ,2710 ,2718 ,2723 ,2727 ,2730 ,2731 ,2730 ,2727 ,2723 ,2718 ,2710 ,2701 ,2691 ,2679 ,2665 ,2650 ,2634 ,2616 ,2596 ,2576 ,2554 ,2531 ,2506 ,2481 ,2455 ,2427 ,2399 ,2370 ,2340 ,2309 ,2278 ,2246 ,
2214 ,2181 ,2148 ,2115 ,2081 ,2048 ,2015 ,1981 ,1948 ,1915 ,1882 ,1850 ,1818 ,1787 ,1756 ,1726 ,1697 ,1669 ,1641 ,1615 ,1590 ,1565 ,1542 ,1520 ,1500 ,1480 ,1462 ,1446 ,1431 ,1417 ,1405 ,1395 ,1386 ,1378 ,1373 ,1369 ,1366 ,1365 ,1366 ,
1369 ,1373 ,1378 ,1386 ,1395 ,1405 ,1417 ,1431 ,1446 ,1462 ,1480 ,1500 ,1520 ,1542 ,1565 ,1590 ,1615 ,1641 ,1669 ,1697 ,1726 ,1756 ,1787 ,1818 ,1850 ,1882 ,1915 ,1948 ,1981 ,2015 ,2048 ,2081 ,2115 ,2148 ,2181 ,2214 ,2246 ,2278 ,2309 ,
2340 ,2370 ,2399 ,2427 ,2455 ,2481 ,2506 ,2531 ,2554 ,2576 ,2596 ,2616 ,2634 ,2650 ,2665 ,2679 ,2691 ,2701 ,2710 ,2718 ,2723 ,2727 ,2730 ,2731 ,2730 ,2727 ,2723 ,2718 ,2710 ,2701 ,2691 ,2679 ,2665 ,2650 ,2634 ,2616 ,2596 ,2576 ,2554 ,
2531 ,2506 ,2481 ,2455 ,2427 ,2399 ,2370 ,2340 ,2309 ,2278 ,2246 ,2214 ,2181 ,2148 ,2115 ,2081 ,2048 ,2015 ,1981 ,1948 ,1915 ,1882 ,1850 ,1818 ,1787 ,1756 ,1726 ,1697 ,1669 ,1641 ,1615 ,1590 ,1565 ,1542 ,1520 ,1500 ,1480 ,1462 ,1446 ,
1431 ,1417 ,1405 ,1395 ,1386 ,1378 ,1373 ,1369 ,1366 ,1365 ,1366 ,1369 ,1373 ,1378 ,1386 ,1395 ,1405 ,1417 ,1431 ,1446 ,1462 ,1480 ,1500 ,1520 ,1542 ,1565 ,1590 ,1615 ,1641 ,1669 ,1697 ,1726 ,1756 ,1787 ,1818 ,1850 ,1882 ,1915 ,1948 ,
1981 ,2015 ,2048 ,2081 ,2115 ,2148 ,2181 ,2214 ,2246 ,2278 ,2309 ,2340 ,2370 ,2399 ,2427 ,2455 ,2481 ,2506 ,2531 ,2554 ,2576 ,2596 ,2616 ,2634 ,2650 ,2665 ,2679 ,2691 ,2701 ,2710 ,2718 ,2723 ,2727 ,2730 ,2731 ,2730 ,2727 ,2723 ,2718 ,
2710 ,2701 ,2691 ,2679 ,2665 ,2650 ,2634 ,2616 ,2596 ,2576 ,2554 ,2531 ,2506 ,2481 ,2455 ,2427 ,2399 ,2370 ,2340 ,2309 ,2278 ,2246 ,2214 ,2181 ,2148 ,2115 ,2081 ,2048 ,2015 ,1981 ,1948 ,1915 ,1882 ,1850 ,1818 ,1787 ,1756 ,1726 ,1697 ,
1669 ,1641 ,1615 ,1590 ,1565 ,1542 ,1520 ,1500 ,1480 ,1462 ,1446 ,1431 ,1417 ,1405 ,1395 ,1386 ,1378 ,1373 ,1369 ,1366 ,1365 ,1366 ,1369 ,1373 ,1378 ,1386 ,1395 ,1405 ,1417 ,1431 ,1446 ,1462 ,1480 ,1500 ,1520 ,1542 ,1565 ,1590 ,1615 ,
1641 ,1669 ,1697 ,1726 ,1756 ,1787 ,1818 ,1850 ,1882 ,1915 ,1948 ,1981 ,2015 ,2048 ,2081 ,2115 ,2148 ,2181 ,2214 ,2246 ,2278 ,2309 ,2340 ,2370 ,2399 ,2427 ,2455 ,2481 ,2506 ,2531 ,2554 ,2576 ,2596 ,2616 ,2634 ,2650 ,2665 ,2679 ,2691 ,
2701 ,2710 ,2718 ,2723 ,2727 ,2730 ,2731 ,2730 ,2727 ,2723 ,2718 ,2710 ,2701 ,2691 ,2679 ,2665 ,2650 ,2634 ,2616 ,2596 ,2576 ,2554 ,2531 ,2506 ,2481 ,2455 ,2427 ,2399 ,2370 ,2340 ,2309 ,2278 ,2246 ,2214 ,2181 ,2148 ,2115 ,2081 ,2048 ,
2015 ,1981 ,1948 ,1915 ,1882 ,1850 ,1818 ,1787 ,1756 ,1726 ,1697 ,1669 ,1641 ,1615 ,1590 ,1565 ,1542 ,1520 ,1500 ,1480 ,1462 ,1446 ,1431 ,1417 ,1405 ,1395 ,1386 ,1378 ,1373 ,1369 ,1366 ,1365 ,1366 ,1369 ,1373 ,1378 ,1386 ,1395 ,1405 ,
1417 ,1431 ,1446 ,1462 ,1480 ,1500 ,1520 ,1542 ,1565 ,1590 ,1615 ,1641 ,1669 ,1697 ,1726 ,1756 ,1787 ,1818 ,1850 ,1882 ,1915 ,1948 ,1981 ,2015 ,2048 ,2081 ,2115 ,2148 ,2181 ,2214 ,2246 ,2278 ,2309 ,2340 ,2370 ,2399 ,2427 ,2455 ,2481 ,
2506 ,2531 ,2554 ,2576 ,2596 ,2616 ,2634 ,2650 ,2665 ,2679 ,2691 ,2701 ,2710 ,2718 ,2723 ,2727 ,2730 ,2731 ,2730 ,2727 ,2723 ,2718 ,2710 ,2701 ,2691 ,2679 ,2665 ,2650 ,2634 ,2616 ,2596 ,2576 ,2554 ,2531 ,2506 ,2481 ,2455 ,2427 ,2399 ,
2370 ,2340 ,2309 ,2278 ,2246 ,2214 ,2181 ,2148 ,2115 ,2081 ,2048 ,2015 ,1981 ,1948 ,1915 ,1882 ,1850 ,1818 ,1787 ,1756 ,1726 ,1697 ,1669 ,1641 ,1615 ,1590 ,1565 ,1542 ,1520 ,1500 ,1480 ,1462 ,1446 ,1431 ,1417 ,1405 ,1395 ,1386 ,1378 ,
1373 ,1369 ,1366 ,1365 ,1366 ,1369 ,1373 ,1378 ,1386 ,1395 ,1405 ,1417 ,1431 ,1446 ,1462 ,1480 ,1500 ,1520 ,1542 ,1565 ,1590 ,1615 ,1641 ,1669 ,1697 ,1726 ,1756 ,1787 ,1818 ,1850 ,1882 ,1915 ,1948 ,1981 ,2015};
