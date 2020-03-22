#include "my_include.h"

Buffer *thisbuf;

uint8_t cmd_buf[128],cmd_len=0;
int8_t err_num = 0;


void printf_help()
{
	printf("\r\n****************CMD List****************\r\n");
	printf("set_led led_num led_status:\r\n");
	printf("	Set led status!\r\n	led_num:0/1/2	led_status:on/ON/OFF/off/0/1\r\n");
	printf("help / ? :\r\n");
	printf("	Ask for help?\r\n");
	printf("\r\n****************************************\r\n");
}


static void Set_led(uint8_t num,uint8_t status)
{
	if(num == 0)
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)!status);
	}
	else if(num == 1)
	{
		GPIO_WriteBit(GPIOC,GPIO_Pin_4,(BitAction)!status);
	}
	else if(num == 2)
	{
		GPIO_WriteBit(GPIOC,GPIO_Pin_3,(BitAction)!status);
	}
}

void make_cmd(uint8_t *str)
{
	char *chr;
	uint8_t led_num,led_status;
	if(!strncmp((char *)str,"set_led",7))//set_led 0 on
	{
		chr = strchr((char *)str,' ');
		if(chr > 0)
		{
			led_num = atoi(chr + 1);
			if(led_num >=0 && led_num <=2)
			{
				chr = strchr(chr + 1,' ');
				if(chr > 0)
				{
					if(!strncmp(chr+1,"on",2) || !strncmp(chr+1,"ON",2) || atoi(chr + 1))
					{
						led_status = 1;
					}
					else if(!strncmp(chr+1,"off",3) || !strncmp(chr+1,"OFF",3) || !atoi(chr + 1))
					{
						led_status = 0;
					}
					else
						err_num = -1;		//²ÎÊı´íÎó
				}
			}
			else
				err_num = -1;		//²ÎÊı´íÎó
		}
		if(err_num == 0)
		{
			Set_led(led_num,led_status);
			printf("%s\r\n",str);
		}
	}
	else if(!strncmp((char *)str,"help",4) || !strncmp((char *)str,"?",1))
	{
		printf_help();
	}
	else if(!strncmp((char *)str,"clear",5) || !strncmp((char *)str,"cls",3))
	{
		printf("\33[2J");
	}
	else
		err_num = -2;			//ÃüÁî´íÎó
	switch(err_num)
	{
		case -1:
			printf("The param is err!\r\n");
			printf_help();
		break;
		case -2:
			printf("The cmd is err!\r\n");
			printf_help();
		break;
	}
}

void Shell_cmd_handle(void)
{
	thisbuf = Get_USART1_buf_handle();
	if(!is_empty(thisbuf))
	{
		cmd_buf[cmd_len++] = get_data(thisbuf);
		USART1_PutChr(cmd_buf[cmd_len - 1]);
		if(cmd_buf[cmd_len - 1] == '\r')
		{
			cmd_buf[cmd_len] = '\0';
			make_cmd(cmd_buf);
			cmd_len = 0;
		}
	}
}



