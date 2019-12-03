# 实验10：使用HAL库进行看门狗实验
11610101 韦青茂
## 实验器材
* 硬件：ARM-STM32开发板，ST-Link。
* 软件：Win7/Win8/Win10, CubeMX, PlatformIO via VSCode
## 实验要求
1. Use the ADC to get the measurement of internal temperature sensor(practice of the last lab)
2. Use KEY1 to refresh the WWDG and output the corresponding data when the early wakeup interrupt is
triggered.



## 实验过程
### 配置
1. 时钟设置. AHB2速度很低, 使得WWDG的窗口长到人类可以操作. ADC的速度也低了, 延长采样时间,使得采样更准确.
    ![mx1](C:/Users/User/Desktop/Embedded/Lab10_WDG_PIO/Img/1.jpg)
2. WWDG配置, 窗口值和重装值都设为最大值(已经和IWDG没什么区别了)  
    ![mx2](C:/Users/User/Desktop/Embedded/Lab10_WDG_PIO/Img/2.jpg)

### 软件代码(main.c)
``` c

/* USER CODE BEGIN PV */
// 引入外部变量
extern UART_HandleTypeDef huart1;
extern WWDG_HandleTypeDef hwwdg;
extern ADC_HandleTypeDef hadc1;
// 在静态区域为变量申请空间
int raw;
float voltage;
float temprature;
/* USER CODE END PV */
```

``` c
/* USER CODE BEGIN PFP */
// 检测stdio.h文件的putchar函数原型,使其能被正确重载
#ifdef __GNUC__
#define PUTCHAR_PROTO int __io_putchar(int ch)
#else
#define PUTCHAR_PROTO int fputc(int ch, FILE *f)
#endif 

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// 重写fputc函数,使其能在串口打印
PUTCHAR_PROTO 
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}
/* USER CODE END 0 */
```

```c
int main(void)
{
  ...
  /* USER CODE BEGIN 2 */
  // 点亮LED0 100ms后熄灭, 用以指示是否刚刚开机(复位)
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
  /* USER CODE END 2 */

  // 开启ADC1
  HAL_ADC_Start(&hadc1);
  while (1)
  {
    HAL_Delay(10);
    // 使用ADC1的连续采样模式
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    raw = HAL_ADC_GetValue(&hadc1);
    voltage = raw * 3300 / 4096.0;
    temprature = (1430 - voltage) / 43.0 + 25.0;
  }
}
```

```c
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
  case KEY1_Pin:
    if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
    {
      // 重设WWDG计数值
      HAL_WWDG_Refresh(&hwwdg);
      // 打印对应的信息
      printf("\r\n=====WWDG refresh.=====\r\n");
    }
    break;
  default:
    break;
  }
}
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
  // 翻转LED1以指示是否进入该中断
  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
  // 串口输出温度值
  printf("T = %f\r\n", temprature);
}
```
### 串口实验：

打印温度值: 
![serial](C:/Users/User/Desktop/Embedded/Lab10_WDG_PIO/Img/3.jpg)
![serial](C:/Users/User/Desktop/Embedded/Lab10_WDG_PIO/Img/4.jpg)

## 遇到的问题及解决方法
### 1. ADC获取到的值异常
>直接输出ADC1的温度传感器读数原始值时,波动较大,换算后的温度值不符合常理,经查阅资料后发现是采样时间过短导致的. 一开始ADC1的时钟是72MHz,采样时间为1.5个周期. 将ADC时钟调整为4MHz,采样时间设为7.5个周期后,终于读数正常.
### 2. EXTI中调用HAL_Delay的优先级问题
>如果按照Cube的默认优先级设置(都为0)是不会有问题的,但是开启WWDG之后如果再在EXTI中调用`HAL_Delay`的话,会导致无法进入`WWDG_Early_Wakeup`中断.但即使是调整了优先级,也没有很好的解决问题.最终只能放弃在EXTI中调用`HAL_Delay`,所以目前按键无法消抖.
