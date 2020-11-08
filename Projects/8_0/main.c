#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_tim.h"

/* Repeat the functionality of 4.0 */

void delay(int ms) {
    for (volatile int i = 0; i < 16000; ++i) {
      for (volatile int j = 0; j < ms; ++j) {
        ;
      }
    }
}

int main()
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_1);
  LL_TIM_SetPrescaler(TIM2, 159);
  LL_TIM_SetAutoReload(TIM2, 100);
  LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH4);
  LL_TIM_OC_SetMode(TIM2, LL_TIM_CHANNEL_CH4, LL_TIM_OCMODE_PWM1);
  LL_TIM_OC_SetCompareCH4(TIM2, 75);
  LL_TIM_EnableCounter(TIM2);
  while(1)
  { 
    for (int i = 0; i < 100; ++i) {
      LL_TIM_OC_SetCompareCH4(TIM2, i);
      delay(1);
    }
    for (int i = 99; i >=0; --i) {
      LL_TIM_OC_SetCompareCH4(TIM2, i);
      delay(1);
    }
  }
}
