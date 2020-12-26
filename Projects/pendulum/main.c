#include "stm32f429xx.h"
#include "main.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_gyroscope.h"
#include "stdio.h"
#include "stm32f4xx_ll_rcc.h"

static float xyz[3];
static char str[256];
static void SystemClock_Config(void);
void halt(void);
int sgn(float x);

int main()
{
 //HAL Library initialization
 HAL_Init();
 //Appropriate clock configuration
 SystemClock_Config();

 //Display initialization
 BSP_LCD_Init();
 BSP_GYRO_Init();
 BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER, LCD_FRAME_BUFFER);
 BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
 BSP_LCD_DisplayOn();
 BSP_LCD_Clear(LCD_COLOR_WHITE);//Clear display
 BSP_LCD_SetBackColor(LCD_COLOR_WHITE);//Choose background color
 BSP_LCD_SetTextColor(LCD_COLOR_BLACK);//Set work color, not only for text
 BSP_LCD_DisplayStringAtLine(1, "Pendulum");
 int id = BSP_GYRO_ReadID();
 sprintf(str, "%d", id);
 BSP_LCD_DisplayStringAtLine(2, "Gyro sensor ID");
 BSP_LCD_DisplayStringAtLine(3, str);
 if (id != 212) {
   BSP_LCD_DisplayStringAtLine(4, "Fail");
   halt();
 }
 else
   BSP_LCD_DisplayStringAtLine(4, "OK");
 /*
 LL_RCC_ClocksTypeDef RCCClocks;
 LL_RCC_GetSystemClocksFreq(&RCCClocks);
 int freq = RCCClocks.PCLK1_Frequency;
 SystemCoreClock = freq;
 HAL_InitTick(TICK_INT_PRIORITY);
 */
 //float s = 0;
 int last_t = 0;
 int t = 0;
 float last_z = 0;
 float per;
 while(1)
 {
   float x = 0;
   float y = 0;
   float z = 0;
   for (int i = 0; i < 1000; ++i) {
     BSP_GYRO_GetXYZ(xyz);
     x += xyz[0] / 1000 / 250;
     y += xyz[1] / 1000 / 250;
     z += xyz[2] / 1000 / 250;
   }
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
   t = HAL_GetTick();
   if (sgn(last_z) * sgn(z) < 0) {
     int p = t - last_t;
     last_t = t;
     per = p / 1000.0;
     per *= 2;
   }
   sprintf(str, "T: %f", per);
   BSP_LCD_DisplayStringAtLine(8, str);
   //s += z * ((t - last_t) / 1000.0);
   last_z = z;
   float l = (per / 2 / 3.1415926) * (per / 2 / 3.1415926) * 9.8;
   sprintf(str, "l: %f", l);
   BSP_LCD_DisplayStringAtLine(9, str);
   sprintf(str, "freq: %d", freq);
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

int sgn(float x) {
  if (x > 0)
    return 1;
  if (x < 0)
    return -1;
  return 0;
}

void halt() {
  while(1);
}
