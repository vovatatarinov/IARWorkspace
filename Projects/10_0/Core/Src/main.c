/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal2,
  .stack_size = 128 * 4
};
/* Definitions for PRG1 */
osThreadId_t PRG1Handle;
const osThreadAttr_t PRG1_attributes = {
  .name = "PRG1",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for PRG2 */
osThreadId_t PRG2Handle;
const osThreadAttr_t PRG2_attributes = {
  .name = "PRG2",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for PRG3 */
osThreadId_t PRG3Handle;
const osThreadAttr_t PRG3_attributes = {
  .name = "PRG3",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for PRG4 */
osThreadId_t PRG4Handle;
const osThreadAttr_t PRG4_attributes = {
  .name = "PRG4",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for PRG5 */
osThreadId_t PRG5Handle;
const osThreadAttr_t PRG5_attributes = {
  .name = "PRG5",
  .priority = (osPriority_t) osPriorityNormal1,
  .stack_size = 128 * 4
};
/* Definitions for PRG6 */
osThreadId_t PRG6Handle;
const osThreadAttr_t PRG6_attributes = {
  .name = "PRG6",
  .priority = (osPriority_t) osPriorityNormal1,
  .stack_size = 128 * 4
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartDefaultTask(void *argument);
void StartPRG1(void *argument);
void StartPRG2(void *argument);
void StartPRG3(void *argument);
void StartPRG4(void *argument);
void StartPRG5(void *argument);
void StartPRG6(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void suspendAllTasks() {
  LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_13);
  LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_14);
  vTaskSuspend(PRG1Handle);
  vTaskSuspend(PRG2Handle);
  vTaskSuspend(PRG3Handle);
  vTaskSuspend(PRG4Handle);
  vTaskSuspend(PRG5Handle);
  vTaskSuspend(PRG6Handle);
  
}

void delay(int counts) {
    for (volatile int i = 0; i < 16000; ++i) {
      for (volatile int j = 0; j < counts; ++j) {
        ;
      }
    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of PRG1 */
  PRG1Handle = osThreadNew(StartPRG1, NULL, &PRG1_attributes);

  /* creation of PRG2 */
  PRG2Handle = osThreadNew(StartPRG2, NULL, &PRG2_attributes);

  /* creation of PRG3 */
  PRG3Handle = osThreadNew(StartPRG3, NULL, &PRG3_attributes);

  /* creation of PRG4 */
  PRG4Handle = osThreadNew(StartPRG4, NULL, &PRG4_attributes);

  /* creation of PRG5 */
  PRG5Handle = osThreadNew(StartPRG5, NULL, &PRG5_attributes);

  /* creation of PRG6 */
  PRG6Handle = osThreadNew(StartPRG6, NULL, &PRG6_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */
  suspendAllTasks();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);
  LL_PWR_DisableOverDriveMode();
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_SetSystemCoreClock(16000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_13|LL_GPIO_PIN_14);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_13|LL_GPIO_PIN_14;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */

void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  int mode = 0;
  suspendAllTasks();
  /* Infinite loop */
  for(;;)
  {
    if (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_0)) {
      osDelay(100);
      if (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_0)) {
        ++mode;
        if (mode > 4)
          mode = 0;
        suspendAllTasks();
        switch(mode) {
          case 0:
            suspendAllTasks();
            break;
          case 1:
            suspendAllTasks();
            vTaskResume(PRG1Handle);
            vTaskResume(PRG2Handle);
            break;
          case 2:
            suspendAllTasks();
            vTaskResume(PRG3Handle);
            vTaskResume(PRG4Handle);
            break;
          case 3:
            suspendAllTasks();
            vTaskResume(PRG1Handle);
            vTaskResume(PRG5Handle);
            break;
          case 4:
            suspendAllTasks();
            vTaskResume(PRG3Handle);
            vTaskResume(PRG6Handle);
            break;
          default:
            suspendAllTasks();
            break;
        }
      }
    }
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartPRG1 */
/**
* @brief Function implementing the PRG1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPRG1 */
void StartPRG1(void *argument)
{
  /* USER CODE BEGIN StartPRG1 */
  /* Infinite loop */
  for(;;)
  {
    LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_13);
    osDelay(500);
    LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_13);
    osDelay(500);
  }
  /* USER CODE END StartPRG1 */
}

/* USER CODE BEGIN Header_StartPRG2 */
/**
* @brief Function implementing the PRG2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPRG2 */
void StartPRG2(void *argument)
{
  /* USER CODE BEGIN StartPRG2 */
  /* Infinite loop */
  for(;;)
  {
    LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_14);
    osDelay(1000);
    LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_14);
    osDelay(1000);
  }
  /* USER CODE END StartPRG2 */
}

/* USER CODE BEGIN Header_StartPRG3 */
/**
* @brief Function implementing the PRG3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPRG3 */
void StartPRG3(void *argument)
{
  /* USER CODE BEGIN StartPRG3 */
  /* Infinite loop */
  for(;;)
  {
    LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_13);
    delay(50);
    LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_13);
    delay(50);
  }
  /* USER CODE END StartPRG3 */
}

/* USER CODE BEGIN Header_StartPRG4 */
/**
* @brief Function implementing the PRG4 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPRG4 */
void StartPRG4(void *argument)
{
  /* USER CODE BEGIN StartPRG4 */
  /* Infinite loop */
  for(;;)
  {
    LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_14);
    delay(100);
    LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_14);
    delay(100);
  }
  /* USER CODE END StartPRG4 */
}

/* USER CODE BEGIN Header_StartPRG5 */
/**
* @brief Function implementing the PRG5 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPRG5 */
void StartPRG5(void *argument)
{
  /* USER CODE BEGIN StartPRG5 */
  /* Infinite loop */
  for(;;)
  {
    LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_14);
    osDelay(1000);
    LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_14);
    osDelay(1000);
  }
  /* USER CODE END StartPRG5 */
}

/* USER CODE BEGIN Header_StartPRG6 */
/**
* @brief Function implementing the PRG6 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPRG6 */
void StartPRG6(void *argument)
{
  /* USER CODE BEGIN StartPRG6 */
  /* Infinite loop */
  for(;;)
  {
    LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_14);
    delay(100);
    LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_14);
    delay(100);
  }
  /* USER CODE END StartPRG6 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
