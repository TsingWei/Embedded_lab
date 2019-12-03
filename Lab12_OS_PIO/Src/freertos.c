/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "string.h"
// #include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern UART_HandleTypeDef huart1;
typedef struct
{
  uint16_t var;
} mailStruct;
osMailQId mail01Handle;

/* USER CODE END Variables */
osThreadId MsgProducerHandle;
osThreadId MsgConsumerHandle;
osMessageQId myQueue01Handle;
osSemaphoreId CountingSem01Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
#ifdef __GNUC__
#define PUTCHAR_PROTO int __io_putchar(int ch)
#else
#define PUTCHAR_PROTO int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTO
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

int _write(int file, char *ptr, int len)
{
  int DataIdx;
  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    __io_putchar(*ptr++);
  }
  return len;
}
/* USER CODE END FunctionPrototypes */

void MsgProducerTask(void const * argument);
void MsgConsumerTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, SET);
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of CountingSem01 */
  osSemaphoreDef(CountingSem01);
  CountingSem01Handle = osSemaphoreCreate(osSemaphore(CountingSem01), 4);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of myQueue01 */
  osMessageQDef(myQueue01, 16, uint16_t);
  myQueue01Handle = osMessageCreate(osMessageQ(myQueue01), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  osMailQDef(mail01, 4, mailStruct);
  mail01Handle = osMailCreate(osMailQ(mail01), NULL);

  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of MsgProducer */
  osThreadDef(MsgProducer, MsgProducerTask, osPriorityNormal, 0, 128);
  MsgProducerHandle = osThreadCreate(osThread(MsgProducer), NULL);

  /* definition and creation of MsgConsumer */
  osThreadDef(MsgConsumer, MsgConsumerTask, osPriorityBelowNormal, 0, 128);
  MsgConsumerHandle = osThreadCreate(osThread(MsgConsumer), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_MsgProducerTask */
/**
  * @brief  Function implementing the MsgProducer thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_MsgProducerTask */
void MsgProducerTask(void const * argument)
{
  /* USER CODE BEGIN MsgProducerTask */
  u_int8_t i=0;
  /* Infinite loop */
  for (;;)
  {
    printf("[Producer>] Try produce %d.\n",i);
    osSemaphoreWait(CountingSem01Handle, osWaitForever);
    printf("[Producer>] %d produced.\n",i++);
  }
  /* USER CODE END MsgProducerTask */
}

/* USER CODE BEGIN Header_MsgConsumerTask */
/**
* @brief Function implementing the MsgConsumer thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MsgConsumerTask */
void MsgConsumerTask(void const * argument)
{
  /* USER CODE BEGIN MsgConsumerTask */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1000);
    printf("[>Consumer] Cosumes.\n");
    osSemaphoreRelease(CountingSem01Handle);
  }
  /* USER CODE END MsgConsumerTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
