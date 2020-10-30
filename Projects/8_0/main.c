#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_tim.h"
/* Repeat the functionality of 4.0 */
int main()
{
   LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
   LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
   
  while(1)
  {
  }
}
