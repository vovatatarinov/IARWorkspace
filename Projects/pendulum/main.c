#include "stm32f429xx.h"
#include "main.h"
#include "BSP/stm32f429i_discovery_lcd.h"
#include "BSP/stm32f429i_discovery_gyroscope.h"
#include "stdio.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_rcc.h"
#include "vector3d/vector3d.h"

static vector3d xyz, last_xyz;
static float diff, last_diff;
static char str[256];
static void SystemClock_Config(void);
static void halt(void);
static inline int sgn(float x);
static void TIM1_Config();
static void LCD_Startup();

static int millis = 0;

void TIM1_UP_TIM10_IRQHandler()
{
  LL_TIM_ClearFlag_UPDATE(TIM1);
  ++millis;
}

int main()
{
  //Appropriate clock configuration
  SystemClock_Config();
  //HAL Library initialization
  HAL_Init();
  //TIM1 Configuration
  TIM1_Config();
  //Display initialization
  LCD_Startup();
  BSP_GYRO_Init();
  vector3dInit(&xyz);
  vector3dInit(&last_xyz);
  last_xyz.x = 0;
  last_xyz.y = 0;
  last_xyz.z = 0;
  last_diff = 0;
  BSP_LCD_DisplayStringAtLine(1, "Pendulum");
  int id = BSP_GYRO_ReadID();
  sprintf(str, "%d", id);
  BSP_LCD_DisplayStringAtLine(2, "Gyro sensor ID");
  BSP_LCD_DisplayStringAtLine(3, str);
  if (id != 212)
  {
    BSP_LCD_DisplayStringAtLine(4, "Fail");
    halt();
  }
  else
    BSP_LCD_DisplayStringAtLine(4, "OK");

  //float s = 0;
  int last_t = 0;
  int t = 0;
  float last_z = 0;
  float per;  
  while (1)
  {
    float x = 0;
    float y = 0;
    float z = 0;
    for (int i = 0; i < 1000; ++i)
    {
      BSP_GYRO_GetXYZ((float*)&xyz);
      x += xyz.x / 1000 / 250;
      y += xyz.y / 1000 / 250;
      z += xyz.z / 1000 / 250;
    }
    
    diff = xyz.abs(&xyz) - last_xyz.abs(&last_xyz);
    last_xyz.copy(&last_xyz, &xyz);
    BSP_LCD_ClearStringLine(5);
    BSP_LCD_ClearStringLine(6);
    BSP_LCD_ClearStringLine(7);
    BSP_LCD_ClearStringLine(8);
    BSP_LCD_ClearStringLine(9);
    BSP_LCD_ClearStringLine(10);
    sprintf(str, "X: %f", x);
    BSP_LCD_DisplayStringAtLine(5, str);
    sprintf(str, "Y: %f", y);
    BSP_LCD_DisplayStringAtLine(6, str);
    sprintf(str, "Z: %f", z);
    BSP_LCD_DisplayStringAtLine(7, str);
    //sprintf(str, "SUM_Z: %f", s);
    //BSP_LCD_DisplayStringAtLine(8, str);
    //last_t = t;
    //t = HAL_GetTick();
    t = millis;
    if (sgn(diff) * sgn(last_diff) < 0)
    {
      int p = t - last_t;
      last_t = t;
      per = p / 1000.0;
      per *= 4;
    }
    last_diff = diff;
    sprintf(str, "T: %f", per);
    BSP_LCD_DisplayStringAtLine(8, str);
    //s += z * ((t - last_t) / 1000.0);
    last_z = z;
    float l = (per / 2 / 3.1415926) * (per / 2 / 3.1415926) * 9.8;
    sprintf(str, "l: %f", l);
    BSP_LCD_DisplayStringAtLine(9, str);
    sprintf(str, "time: %d", t / 1000);
    BSP_LCD_DisplayStringAtLine(10, str);
    HAL_Delay(10);
  }
  //return 0;
}
//Appropriate clock configuration
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
 clocked below the maximum system frequency, to update the voltage scaling value
 regarding system frequency refer to product datasheet. */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Activate the Over-Drive mode */
  HAL_PWREx_EnableOverDrive();

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

static void TIM1_Config()
{
  LL_RCC_ClocksTypeDef RCCClocks;
  LL_RCC_GetSystemClocksFreq(&RCCClocks);
  int freq = RCCClocks.PCLK2_Frequency;
  //SystemCoreClock = freq;
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
  //auto reload register
  LL_TIM_SetAutoReload(TIM1, freq / 1000 / 1000 - 1);
  //Prescaler
  LL_TIM_SetPrescaler(TIM1, 1000 / 3 * 2);
  //Enable INT in TIM1
  LL_TIM_EnableIT_UPDATE(TIM1);
  LL_TIM_EnableCounter(TIM1);
  NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}

static void LCD_Startup()
{
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER, LCD_FRAME_BUFFER);
  BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
  BSP_LCD_DisplayOn();
  BSP_LCD_Clear(LCD_COLOR_WHITE);        //Clear display
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); //Choose background color
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK); //Set work color, not only for text
}

static inline int sgn(float x)
{
  if (x > 0)
    return 1;
  if (x < 0)
    return -1;
  return 0;
}

static void halt()
{
  while (1)
    ;
}
