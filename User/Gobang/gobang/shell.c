#include "my_include.h"

Buffer *thisbuf;

uint8_t cmd_buf[128],cmd_len=0;
int8_t err_num = 0;


void printf_help()
{
	printf("\r\n****************帮助列表****************\r\n");
	printf("set_led led_num led_status:\r\n");
	printf("	Set led status!\r\n	led_num:1/2/3	led_status:on/ON/OFF/off/0/1\r\n");
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

FRESULT scan_files (
    char* path        /* Start node to be scanned (also used as work area) */
);

void make_cmd(uint8_t *str)
{
	char *chr;
	char Path[128];
	FIL fil;
	uint8_t led_num,led_status;
	uint32_t tm_date,tm_time;
	err_num = 0;
	if(!strncmp((char *)str,"set_led",7))//set_led 0 on
	{
		chr = strchr((char *)str,' ');
		if(chr > 0)
		{
			led_num = atoi(chr + 1);
			if(led_num >0 && led_num <=3)
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
						err_num = -1;		//参数错误
				}
			}
			else
				err_num = -1;		//参数错误
		}
		if(err_num == 0)
		{
			Set_led(led_num - 1,led_status);
			printf("%s\r\n",str);
		}
	}
	else if(!strncmp((char *)str,"set_tm",6))
	{
		chr = strchr((char *)str,' ');
		if(chr > 0)
		{
			tm_date = atoi(chr + 1);
			if(tm_date > 0)
			{
				chr = strchr(chr+1,' ');
				if(chr > 0)
				{
					tm_time = atoi(chr + 1);
					if(tm_time >0)
					{
						Set_timer(tm_date,tm_time);
						Printf_date(0);
					}
					else
						err_num = -1;
				}
			}
			else 
				err_num = -1;
		}
	}
	else if(!strncmp((char *)str,"set_alarm",9))
	{
		chr = strchr((char *)str,' ');
		if(chr > 0)
		{
			tm_date = atoi(chr + 1);
			if(tm_date > 0)
			{
				chr = strchr(chr+1,' ');
				if(chr > 0)
				{
					tm_time = atoi(chr + 1);
					if(tm_time >0)
					{
						Set_alarm(tm_date,tm_time);
						printf("\r\n");
					}
					else
						err_num = -1;
				}
			}
			else 
				err_num = -1;
		}
	}
	else if(!strncmp((char *)str,"date",4))
	{
		Printf_date(0);
	}
	else if(!strncmp((char *)str,"pwd",3)) //命令行操作：查询当前工作目录
	{
			f_getcwd(Path,sizeof(Path));
	
			printf("\r\n%s\r\n",Path);
	}
	else if(!strncmp((char *)str,"cd",2))  //命令行操作：进入文件、文件夹
	{
			chr = strchr((char *)str,' ');
			if(chr > 0 && *(chr + 1) != '\0')
			{
				f_chdir(chr + 1);
			}
			printf("\r\n");
	}
	else if(!strncmp((char *)str,"ls",2))  //命令行操作：查看当前目录下文件、详细参数
	{
		f_getcwd(Path,sizeof(Path));
		printf("\r\n");
		scan_files(Path);
	}
	else if(!strncmp((char *)str,"mkdir",5)) //命令行操作：新建文件夹
	{
			chr = strchr((char *)str,' ');
			if(chr > 0 && *(chr + 1) != '\0')
			{
				f_mkdir(chr + 1);
			}
			printf("\r\n");
	}
	else if(!strncmp((char *)str,"rm",2))   //命令行操作：删除当前目录下文件
	{
			chr = strchr((char *)str,' ');
			if(chr > 0 && *(chr + 1) != '\0')
			{
				f_unlink(chr + 1);
			}
			printf("\r\n");
	}
	else if(!strncmp((char *)str,"touch",5)) //命令行操作：当前目录下新建一个空文件
	{
			chr = strchr((char *)str,' ');
			if(chr > 0 && *(chr + 1) != '\0')
			{
				if(f_open(&fil,chr + 1,FA_CREATE_NEW) != FR_OK) /* 已经存在就不再新建 */
				{
					printf("\r\n文件已存在!");
				}
				f_close(&fil);
			}
			printf("\r\n");
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
		err_num = -2;			//命令错误
	switch(err_num)
	{
		case -1:
//			printf("The param is err!\r\n");
			Debug("The param is err!	");
			printf_help();
		break;
		case -2:
			Debug("The CMD is err!	");
//			printf("The cmd is err!\r\n");
			printf_help();
		break;
	}
}

uint8_t Cmd_busy_flag;

uint8_t Is_cmdbusy(void)
{
	return Cmd_busy_flag;
}

void Shell_cmd_handle(void)
{
	thisbuf = Get_USART1_buf_handle();
	if(!is_empty(thisbuf))
	{
		Cmd_busy_flag = 1;
		cmd_buf[cmd_len++] = get_data(thisbuf);
		USART1_PutChr(cmd_buf[cmd_len - 1]);
		if(cmd_buf[cmd_len - 1] == '\r')
		{
			cmd_buf[cmd_len] = '\0';
			make_cmd(cmd_buf);
			cmd_len = 0;
			Cmd_busy_flag = 0;
		}
		else if(cmd_buf[cmd_len - 1] == 8)
		{
			cmd_len -= 2;
			printf("\33[K");
		}
	}
}



