#include "my_include.h"


time_t UNIX_Sec,Alarm_Sec;
SYS_TM Sys_time,Sys_alarm;
uint8_t Alarm_flag=0;

char *week_buf[7]= {
	"日","一","二","三","四","五","六"
};

static void RTC_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void RTC_Configuration(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP , ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	RTC_EnterConfigMode();
	RCC_LSEConfig(RCC_LSE_ON);
	while(RESET == RCC_GetFlagStatus(RCC_FLAG_LSERDY));
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	RTC_SetPrescaler(32767);
	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC | RTC_IT_ALR,ENABLE);
	RTC_WaitForLastTask();
	RTC_NVIC_Configuration();
	
//	Sys_time.tm_year = 2014 - 1900;
//	Sys_time.tm_mon = 11 - 1;
//	Sys_time.tm_mday = 17;
//	Sys_time.tm_hour = 11;
//	Sys_time.tm_min = 32;
//	Sys_time.tm_sec = 0;
//	RTC_SetCounter(mktime(&Sys_time));
//	RTC_WaitForLastTask();
	
	RTC_ExitConfigMode();
	PWR_BackupAccessCmd(DISABLE);
}

void Set_timer(uint32_t date,uint32_t time)		//20141116
{
	Sys_time.tm_year = (date/10000)%10000 - 1900;
	Sys_time.tm_mon = (date/100)%100 - 1;
	Sys_time.tm_mday = date%100;
	Sys_time.tm_hour = (time/10000)%24;
	Sys_time.tm_min = (time/100)%100;
	Sys_time.tm_sec = (time%100)%60;
	PWR_BackupAccessCmd(ENABLE);
	RTC_EnterConfigMode();
	UNIX_Sec = mktime(&Sys_time);
	RTC_SetCounter(UNIX_Sec);
	RTC_WaitForLastTask();
	RTC_ExitConfigMode();
	PWR_BackupAccessCmd(DISABLE);
}

void Set_alarm(uint32_t date,uint32_t time)		//20141116
{
	Sys_alarm.tm_year = (date/10000)%10000 - 1900;
	Sys_alarm.tm_mon = (date/100)%100 - 1;
	Sys_alarm.tm_mday = date%100;
	Sys_alarm.tm_hour = (time/10000)%24;
	Sys_alarm.tm_min = (time/100)%100;
	Sys_alarm.tm_sec = (time%100)%60;
	PWR_BackupAccessCmd(ENABLE);
	RTC_EnterConfigMode();
	Alarm_Sec = mktime(&Sys_alarm);
	RTC_SetAlarm(Alarm_Sec);
	RTC_WaitForLastTask();
	RTC_ExitConfigMode();
	PWR_BackupAccessCmd(DISABLE);
}

void Update_Sec(time_t tm)
{
	UNIX_Sec = tm;
}

time_t time(time_t * tm/*timer*/)
{
	if(tm != NULL)
		*tm = UNIX_Sec;
	return UNIX_Sec;
}

void Printf_date(void)
{
	uint8_t tm_buf[128];
	SYS_TM *now_tm;
	now_tm = localtime(&UNIX_Sec);
	printf("\33[2J");
	printf("公历：%d年%d月%d日 %d-%d-%d \33[34m 星期：%s \33[30m\r\n",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday,now_tm->tm_hour,now_tm->tm_min,now_tm->tm_sec,week_buf[now_tm->tm_wday]);
	GetChinaCalendar(now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday,tm_buf);
	printf("农历：%d%d年%d月%d日\t",tm_buf[0],tm_buf[1],tm_buf[2],tm_buf[3]);
	GetChinaCalendarStr(now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday,tm_buf);
	printf("%s\t",tm_buf);
	GetJieQiStr(now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday,tm_buf);
	printf("%s\r\n",tm_buf);
}

void Update_alarm(uint8_t val)
{
	Alarm_flag = val;
}

uint8_t Is_Alarm(void)
{
	return Alarm_flag;
}

