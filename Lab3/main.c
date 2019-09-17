#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
//《嵌入式与微机原理》实验（三）
//按键控制LED灯_IO输入操作		   

void status_1()
{
	LED0 = 0;
	LED0 = 1;
}

void status_2()
{
	LED0 = 1;
	LED0 = 0;
}
void status_3()
{
	LED0 = 0;
	LED0 = 0;
}

 int main(void)
 {	
	u8 t=0;	  
	delay_init();	      //延时函数初始化	  
	LED_Init();		  	 	//初始化与LED连接的硬件接口
	KEY_Init();        	//初始化与按键连接的硬件接口
											//点亮LED
	while(1)
	{
		t=KEY_Scan(0);		//得到键值
		switch(t)
		{				 
			case KEY0_PRES:
				status_1();
				break;
			case KEY1_PRES:
				status_2();
				break;
			case WKUP_PRES:				
				status_3();
				break;
			default:
				delay_ms(20);
					
		} 
	}		 
}
