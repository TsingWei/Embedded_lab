# 实验11~12：FreeRTOS 与 生产者-消费者模型 实验
11610101 韦青茂
## 实验器材
* 硬件：ARM-STM32开发板，ST-Link。
* 软件：Win10, CubeMX, PlatformIO via VSCode
## 实验要求
1. Finish the practice of the last lab: Using counting semaphore to solve the producer-consumer problem
2. Using mail queues to solve the producer-consumer problem
3. The buffer size of the producer-consumer problem is 4


## 实验过程
### **1.使用`Mail Queue`解决生产者-消费者问题**
#### 配置
1. `Producer`优先级为`Normal`,   
   `Consumer`优先级为`BelowNormal`.
2. `MailQueue`大小为4:
    ```c
    osMailQDef(mail01, 4, mailStruct);
    mail01Handle = osMailCreate(osMailQ(mail01), NULL);
    ```
#### 代码
1. `Producer`部分:
    ```c
    void MsgProducerTask(void const *argument)
    {
    /* USER CODE BEGIN MsgProducerTask */
    mailStruct *mail;
    u_int8_t i = 0;
    /* Infinite loop */
    for (;; ++i)
    {
        while (!(mail = (mailStruct *)osMailAlloc(mail01Handle, osWaitForever)))
        {
        // 如果mail queue已满, 则此时mail为空指针
        // 等待500ms, 此时系统调度会切换线程
        printf("[P]Full! Wait.\n");
        osDelay(500);
        }
        // 向mail queue中发送一则消息
        mail->var = i;
        printf("[P]>>%d\n", mail->var);
        osMailPut(mail01Handle, mail);
    }
    /* USER CODE END MsgProducerTask */
    }
    ```
2. `Consumer`部分:
    ```c
    void MsgConsumerTask(void const *argument)
    {
    /* USER CODE BEGIN MsgConsumerTask */
    osEvent event;
    mailStruct *pMail;

    /* Infinite loop */
    for (;;)
    {
        // 消费者每隔100ms尝试获取一次消息
        osDelay(100);
        event = osMailGet(mail01Handle, osWaitForever);
        if (event.status == osEventMail)
        {
        if (!(pMail = event.value.p))
        {
            // 如果mail queue为空, 则此时pMail为空指针 
            // 等待500ms, 此时系统调度会切换线程 
            printf("[C]Empty!.\n");
            osDelay(500);
            continue;
        }
        printf("[C]%d<<\n", pMail->var);
        osMailFree(mail01Handle, pMail);
        }
    }
    /* USER CODE END MsgConsumerTask */
    }
    ```



#### 结果(串口终端)
1. `Producer`每次塞入四条消息后, 被阻塞, 此时调度器切换到`Consumer`, 接收四条消息.  
2. 当`Consumer`消化完4条信息后,再一次调用`osMailGet(mail01Handle, osWaitForever)`时,调度器会切换线程到`Producer`.
```
...
[C]120<<
[C]121<<
[C]122<<
[C]123<<
[P]>>124
[P]>>125
[P]>>126
[P]>>127
[P]Full! Wait.
[C]124<<
[C]125<<
[C]126<<
[C]127<<
[P]>>128
[P]>>129
[P]>>130
[P]>>131
[P]Full! Wait.
[C]128<<
[C]129<<
[C]130<<
[C]131<<
[P]>>132
[P]>>133
[P]>>134
[P]>>135
[P]Full! Wait.
...
```
  
   
   
### **2.使用信号量解决生产者-消费者问题**
#### 配置


## 遇到的问题及解决方法
### 1. ADC获取到的值异常
>直接输出ADC1的温度传感器读数原始值时,波动较大,换算后的温度值不符合常理,经查阅资料后发现是采样时间过短导致的. 一开始ADC1的时钟是72MHz,采样时间为1.5个周期. 将ADC时钟调整为4MHz,采样时间设为7.5个周期后,终于读数正常.
### 2. EXTI中调用HAL_Delay的优先级问题
>如果按照Cube的默认优先级设置(都为0)是不会有问题的,但是开启WWDG之后如果再在EXTI中调用`HAL_Delay`的话,会导致无法进入`WWDG_Early_Wakeup`中断.但即使是调整了优先级,也没有很好的解决问题.最终只能放弃在EXTI中调用`HAL_Delay`,所以目前按键无法消抖.
