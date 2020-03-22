#ifndef __RTC_H
#define __RTC_H
#include "stm32f10x.h"
#include "time.h"



void RTC_Configuration(void);
void Update_Sec(time_t tm);
void Set_timer(uint32_t date,uint32_t time);
void Printf_date(void);
void Update_alarm(uint8_t val);
uint8_t Is_Alarm(void);
void Set_alarm(uint32_t date,uint32_t time);



#endif
