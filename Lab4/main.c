#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "string.h"



void LED_Control(const char *recv){
	const char* l0_on = "led0 on";
	const char* l0_off = "led0 off";
	const char* l1_on = "led1 on";
	const char* l1_off = "led1 off";
	int _control = 1;
	if(strcmp(recv,l0_on)==0)
		LED0 = 0;
	else if(strcmp(recv,l0_off)==0)
		LED0 = 1;
	else if(strcmp(recv,l1_on)==0)
		LED1 = 0;
	else if(strcmp(recv,l1_off)==0)
		LED1 = 1;
	else
		_control = 0;
	if(_control)
		printf("%s!", recv);
	else
		printf("Hello, %s", recv);
}

int main(void)
{
	u8 t;
	u8 len;
	char recv[20];

	LED_Init();
	KEY_Init();
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(9600);

	while (1)
	{
		t=KEY_Scan(0);		//得到键值
		switch(t)
		{				 
			case KEY0_PRES:
				printf("KEY0 pressed!\r\n");
				break;
			case KEY1_PRES:
				printf("KEY1 pressed!\r\n");
				break;
			case WKUP_PRES:				
				printf("WK_UP pressed!\r\n");
				break;
			default:
				delay_ms(20);
				break;
		}
		if (USART_RX_STA & 0x8000)
		{
			len = USART_RX_STA & 0x3fff;

			for (t = 0; t < len; t++)
			{
				recv[t] = USART_RX_BUF[t]; // 接收到的字符串逐字符存入
				while ((USART1->SR & 0X40) == 0);
			}
			recv[t] = '\0'; // 在字符串末尾拼接 '\0'
			LED_Control(recv);
			USART_RX_STA = 0;
		}

		
		// else
		// {
		// 	times++;
		// 	if (times % 200 == 0)
		// 		printf("Welcome\r\n");
		// 	// if(times%30==0)LED0=!LED0;//闪烁LED,提示系统正在运行.
		// 	delay_ms(10);
		// }
	}
}
