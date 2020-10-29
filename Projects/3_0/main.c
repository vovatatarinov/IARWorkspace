#include <stdbool.h>
#include "stm32f4xx.h"
static bool switchVar;
void TIM1_UP_TIM10_IRQHandler() {
  TIM1 -> SR &= ~TIM_SR_UIF;
  if (!switchVar) {
    GPIOG -> ODR |= (GPIO_ODR_OD14);
    GPIOG -> ODR &= ~(GPIO_ODR_OD13);
    switchVar = true;
  }
  else {
    GPIOG -> ODR &= ~(GPIO_ODR_OD14);
    GPIOG -> ODR |= (GPIO_ODR_OD13);
    switchVar = false;
  }
  return;
}
int main()
{
  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
  RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN;
  
  GPIOG -> MODER &= ~ GPIO_MODER_MODER13_1;
  GPIOG -> MODER |= GPIO_MODER_MODER13_0;
  
  GPIOG -> MODER &= ~ GPIO_MODER_MODER14_1;
  GPIOG -> MODER |= GPIO_MODER_MODER14_0;
  
  //auto reload register
  TIM1 -> ARR = 1000;
  //Prescaler
  TIM1 -> PSC = 16000;
  //Enable INT in TIM1
  TIM1 -> DIER |= TIM_DIER_UIE;
  TIM1 -> CR1 |= TIM_CR1_CEN;
  
  GPIOG -> ODR &= ~(GPIO_ODR_OD14);
  //Turn on PG 13
  GPIOG -> ODR |= (GPIO_ODR_OD13);
  switchVar = false;
  //Enable INT in NVIC
  NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
  
  while(1) {
    
  }
  //return 0;
}
