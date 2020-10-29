#include <stdbool.h>
#include "stm32f4xx.h"

static bool LEDisGreen;
static bool PA0Status;
void EXTI0_IRQHandler() {
  EXTI -> PR |= EXTI_PR_PR0;
  if (PA0Status) {
    if (!((GPIOA -> IDR) & (GPIO_IDR_ID0))) {
      PA0Status = false;
    }
    else
      return;
  }
  else {
    if ((GPIOA -> IDR) & (GPIO_IDR_ID0)) {
      PA0Status = true;
    }
    else
      return;
  }
  if (LEDisGreen) {
      //Turn off PG 13
        GPIOG -> ODR &= ~(GPIO_ODR_OD13);
        //Turn on PG 14
        GPIOG -> ODR |= (GPIO_ODR_OD14);
        LEDisGreen = false;
        return;
  }
   GPIOG -> ODR &= ~(GPIO_ODR_OD14);
  //Turn on PG 13
  GPIOG -> ODR |= (GPIO_ODR_OD13);
  LEDisGreen = true;
  return;
}

int main()
{
  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  
  GPIOG -> MODER &= ~ GPIO_MODER_MODER13_1;
  GPIOG -> MODER |= GPIO_MODER_MODER13_0;
  
  GPIOG -> MODER &= ~ GPIO_MODER_MODER14_1;
  GPIOG -> MODER |= GPIO_MODER_MODER14_0;
  //input mode on PA0
  GPIOA -> MODER &= ~ GPIO_MODER_MODER0;
  EXTI -> IMR |= EXTI_IMR_MR0;
  EXTI -> RTSR |= EXTI_RTSR_TR0;
  EXTI -> FTSR |= EXTI_FTSR_TR0;
  
  GPIOG -> ODR &= ~(GPIO_ODR_OD14);
  //Turn on PG 13
  GPIOG -> ODR |= (GPIO_ODR_OD13);
  LEDisGreen = true;
  PA0Status = false;
  NVIC_EnableIRQ(EXTI0_IRQn);
  
  while(1) {
    
  }
  //return 0;
}
