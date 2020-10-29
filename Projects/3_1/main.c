#include <stdbool.h>
#include "stm32f4xx.h"

static bool switchVar;
static double f;
static bool mustCalculate;
void TIM1_UP_TIM10_IRQHandler() {
  TIM1 -> SR &= ~TIM_SR_UIF;
  if (!switchVar) {
    //GPIOG -> ODR |= (GPIO_ODR_OD14);
    GPIOG -> ODR &= ~(GPIO_ODR_OD13);
    switchVar = true;
  }
  else {
    //GPIOG -> ODR &= ~(GPIO_ODR_OD14);
    GPIOG -> ODR |= (GPIO_ODR_OD13);
    switchVar = false;
  }
  if (!mustCalculate)
    -- (TIM1 -> ARR);
  
  return;
}

void EXTI0_IRQHandler() {
  EXTI -> PR |= EXTI_PR_PR0;
 if (!((GPIOA -> IDR) & (GPIO_IDR_ID0)))
   return;
 mustCalculate = true;
 NVIC_DisableIRQ(EXTI0_IRQn);
 RCC -> AHB1ENR &= ~RCC_AHB1ENR_GPIOAEN;
 return;
}


int main()
{
  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
  RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN;
  
  GPIOG -> MODER &= ~ GPIO_MODER_MODER13_1;
  GPIOG -> MODER |= GPIO_MODER_MODER13_0;
 
  /*
  GPIOG -> MODER &= ~ GPIO_MODER_MODER14_1;
  GPIOG -> MODER |= GPIO_MODER_MODER14_0;
  */
   //input mode on PA0
  GPIOA -> MODER &= ~ GPIO_MODER_MODER0;
  
  EXTI -> IMR |= EXTI_IMR_MR0;
  EXTI -> RTSR |= EXTI_RTSR_TR0;
  EXTI -> FTSR |= EXTI_FTSR_TR0;

  
  //auto reload register
  TIM1 -> ARR = 1000;
  //Prescaler
  TIM1 -> PSC = 1000;
  //Enable INT in TIM1
  TIM1 -> DIER |= TIM_DIER_UIE;
  TIM1 -> CR1 |= TIM_CR1_CEN;
  
  //GPIOG -> ODR &= ~(GPIO_ODR_OD14);
  //Turn on PG 13
  GPIOG -> ODR |= (GPIO_ODR_OD13);
  switchVar = false;
  mustCalculate = false;
  NVIC_EnableIRQ(EXTI0_IRQn);

  //Enable INT in NVIC
  NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
  
  while(1) {
    if (!(mustCalculate))
      continue;
    
    f = (double)16000000 / ((TIM1 -> ARR) + 1) / ((TIM1 -> PSC) + 1) / 2;
    asm volatile("nop");
  }
  //return 0;
}
