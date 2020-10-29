#include <stm32f4xx.h>

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
  while(1) {
    //delay();
    if ((GPIOA -> IDR) & (GPIO_IDR_ID0)) {
        //Turn off PG 13
        GPIOG -> ODR &= ~(GPIO_ODR_OD13);
        //Turn on PG 14
        GPIOG -> ODR |= (GPIO_ODR_OD14);
    }
    else {
        //Turn off PG 14
        GPIOG -> ODR &= ~(GPIO_ODR_OD14);
        //Turn on PG 13
        GPIOG -> ODR |= (GPIO_ODR_OD13);
    }
  }

  //return 0;
}
