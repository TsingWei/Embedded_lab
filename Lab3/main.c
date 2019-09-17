#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
//《嵌入式与微机原理》实验（三）
//按键控制LED灯_IO输入操作		   

void Normal(){
	LED0 = 1;
	LED1 = 0;
}
void Warning(){
	LED0 = 0;
	LED1 = 1;
}
void Error(){
	int i = 8;
	while(i--){
		LED0 = 0;
		LED1 = 1;
		delay_ms(2000);
		LED0 = 1;
		LED1 = 0;
		delay_ms(2000);
	}
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
				Normal();
				break;
			case KEY1_PRES:
				Warning();
				break;
			case WKUP_PRES:				
				Error();
				break;
			default:
				delay_ms(10);	
		} 
	}		 
}
