#include <stdbool.h>
#include "stm32f4xx.h"

void delay(int ms) {
  int p = 16000 * ms;
  for (int i = 0; i < p; ++i)
    asm volatile("nop");
}

int main()
{
  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;
   
  /*
  GPIOG -> MODER &= ~ GPIO_MODER_MODER14_1;
  GPIOG -> MODER |= GPIO_MODER_MODER14_0;
  */
  GPIOA -> MODER &= ~ GPIO_MODER_MODER3_0;
  GPIOA -> MODER |=  GPIO_MODER_MODER3_1;
  GPIOA -> AFR[0] |= GPIO_AFRL_AFSEL3_0;
  GPIOA -> AFR[0] &= ~GPIO_AFRL_AFSEL3_1;
  GPIOA -> AFR[0] &= ~GPIO_AFRL_AFSEL3_2;
  GPIOA -> AFR[0] &= ~GPIO_AFRL_AFSEL3_3;
  
  
  //auto reload register
  TIM2 -> ARR = 100;
  //Prescaler
  TIM2 -> PSC = 159;
 
  TIM2 -> CCMR2 &= ~TIM_CCMR2_CC4S_0;
  TIM2 -> CCMR2 &= ~TIM_CCMR2_CC4S_1;
  
  TIM2 -> CCMR2 &= ~TIM_CCMR2_OC4M_0;
  TIM2 -> CCMR2 |= TIM_CCMR2_OC4M_1;
  TIM2 -> CCMR2 |= TIM_CCMR2_OC4M_2;
  TIM2 -> CCR4 = 75;
  TIM2 -> CCER |= TIM_CCER_CC4E;
  TIM2 -> CR1 |= TIM_CR1_CEN;

  while(1) {
    for (int i = 0; i < 100; ++i) {
      TIM2 -> CCR4 = i;
      delay(1);
    }
    for (int i = 99; i >= 0; --i ) {
      TIM2 -> CCR4 = i;
      delay(1);
    }
  }
  //return 0;
}
