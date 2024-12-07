#include <AT89X52.h>
#define StatusTraffic P1 /*do0, do1, vang0, vang1, xanh0, xanh1*/
sbit SW_chedo = P0^4;
sbit SW = P0^5;
sbit dvi_DT = P0^0;
sbit chuc_DT = P0^1;
sbit dvi_BN = P0^2;
sbit chuc_BN = P0^3;
#define Display P2