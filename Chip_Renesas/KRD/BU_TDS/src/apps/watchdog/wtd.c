#include "wtd.h"
#include "r_cg_userdefine.h"
#include "config.h"
#define WDT_INTERVAL 100 	//ms
uint32_t cntWdt = 0;
void wtd_feed()		//10ms
{
	WDT_PIN = !WDT_PIN;
}

void wtd_feedBeforeMain() //1ms
{
	if(cntWdt++ >= WDT_INTERVAL)
	{
		cntWdt = 0;
		WDT_PIN = !WDT_PIN;
	}
}
