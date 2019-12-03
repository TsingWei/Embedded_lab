# 实验2：点亮2个LED灯
11610101 韦青茂
## 实验器材
* 硬件：ARM-STM32开发板，J-Link/St-Link。
* 软件：Win7/Win8/Win10, Keil uVision5
## 实验要求
1. 归纳总结创建工程过程中遇到的问题以及解决方法。

2. 例程中 PA8 对应开发板上的 DS0， DS1 对应的 IO 口为 PD2，且 IO 口状态为 0 时灯亮，状态为 1 时灭，亮灭交替可表现为闪烁状态，还可以设置快速、慢速 闪烁等状态，以及两个灯同时亮灭或者交替一亮一灭等状态。要求： 模拟用两个 LED 灯的应用场景下使用方式三种状态，系统正常运行、一般异常 （warning）、严重警告（error） 注意：模拟时可以依次显示多种闪烁状态。

3. 提交 main.c 的内容，提交仿真界面 LogicAnalyzer的截图画面，放 在一个实验报告里，保存为 PDF 文件提交到 SAKAI 系统。 课上会抽查，进行实 际运行、验证。

## 实验过程
### 软件代码(main.c)
``` c
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#define LED_RED 0
#define LED_YELLOW 1

// 开灯
void lightOn(int lightID){
	switch (lightID)
	{
	case LED_RED:
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
		break;
	case LED_YELLOW:
		GPIO_ResetBits(GPIOD, GPIO_Pin_2);
		break;
	default:
		break;
	}
}
//　关灯
void lightOff(int lightID){
	switch (lightID)
	{
	case LED_RED:
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
		break;
	case LED_YELLOW:
		GPIO_SetBits(GPIOD, GPIO_Pin_2);
		break;
	default:
		break;
	}
}

// 对应关系 ： PA8 —— led0      PD2 —— led1        IO状态为低时灯亮，为高时灭
int main()
{
	int normal = 8;
	int warning = 8;
	int error = 8;

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE); //使能PA,PD端口时钟

	delay_init();
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;		  //LED0-->PA.8 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);			  //根据设定参数初始化GPIOA.8

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD, GPIO_Pin_2);


	while (normal--)//正常状态，黄灯常亮，红灯关闭
	{
		lightOn(LED_YELLOW);
		lightOff(LED_RED);
		delay_ms(2000);
	}
	while (warning--)//警告状态，黄灯闪烁，红灯关闭
	{
		delay_ms(2000);
		lightOn(LED_YELLOW);
		lightOff(LED_RED);
		delay_ms(2000);
		lightOff(LED_YELLOW);
	}
	while (error--)//错误状态， 红灯黄灯交替闪烁
	{
		delay_ms(2000);
		lightOn(LED_YELLOW);
		lightOff(LED_RED);
		delay_ms(2000);
		lightOn(LED_RED);
		lightOff(LED_YELLOW);
	}
	lightOff(LED_RED);
}
```
### 仿真截图：
![asd](C:/Users/User/Desktop/Embemded/1.png)
* 正常状态： 黄灯常亮
* 警告状态： 黄灯闪烁
* 错误状态： 黄灯红灯交替闪烁

## 遇到的问题及解决方法
### load按钮灰色，无法上传代码
仿真确认运行如预期后，想要load到板上进行实际运行验证。可是load按钮是灰色的，无法点击。此时需要再回到debug选项中将 use jlink 重新选上，而不是 use simulator。勾选后即可load。
### load成功后板子没有反应
load成功后，板子上的led灯没有按照预期闪烁。此时需要按下RST按钮才可以让板子按照代码运行。
