#include "stm32f429xx.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_exti.h"
#include <stdbool.h>
typedef unsigned long dword;
typedef unsigned char byte;
typedef unsigned short word;

#define HWREG(x) *(volatile dword*)(x)
#define FLASH_KEY1 0x45670123
#define FLASH_KEY2 0xCDEF89AB
#define ADDR_FLASH_SECTOR_4 0x08010000
/* 
64 KBytes for 4th sector.
from 0x08010000 to 0x0801FFFF
*/
int counter;


static inline void unlock_flash() {
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
}

static inline void lock_flash() {
  FLASH->CR |= FLASH_CR_LOCK;
}

static inline void wait4mem() {
  while((FLASH->SR & FLASH_SR_BSY));
}

void pageErase() {
  unlock_flash();
  wait4mem();
  FLASH->CR |= FLASH_CR_SER; // Enable erase
  FLASH->CR &= ~FLASH_CR_SNB; // Set 4th sector
  FLASH->CR |= (FLASH_CR_SNB & (4 << 3));
  FLASH->CR |= FLASH_CR_STRT; // Start erase
  wait4mem();
  FLASH->CR &= ~FLASH_CR_SER;
  lock_flash();
}

void readFlash() {
  counter = HWREG(ADDR_FLASH_SECTOR_4);
}
void writeFlash() {
  pageErase();
  unlock_flash();
  wait4mem();
  FLASH->CR |= FLASH_CR_PG;
  FLASH->CR &= ~FLASH_CR_PSIZE_0;
  FLASH->CR |= FLASH_CR_PSIZE_1;
  wait4mem();
  HWREG(ADDR_FLASH_SECTOR_4) = counter;
  wait4mem();
  FLASH->CR &= ~FLASH_CR_PG;
  lock_flash();
}

void EXTI0_IRQHandler() {
  LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
  if (!LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_0))
    return;
  ++counter;
  writeFlash();
  return;
}

void delay(int counts) {
    for (volatile int i = 0; i < 16000; ++i) {
      for (volatile int j = 0; j < counts; ++j) {
        ;
      }
    }
}

void led() {
  for (int i = 0; i < counter; ++i) {
    LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_13);
    delay(50);
    LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_13);
    delay(50);
  }
}

void eraseMem_signal() {
  for (int i = 0; i < 3; ++i) {
    LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_14);
    delay(50);
    LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_14);
    delay(50);
  }
}

int main()
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinMode(GPIOG, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(GPIOG, LL_GPIO_PIN_14, LL_GPIO_MODE_OUTPUT);
  counter = 0;
  if (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_0)) {
     delay(10);
     if (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_0)) {
       eraseMem_signal();
       pageErase();
     }
  }
  readFlash();
  if (counter == -1)
	counter = 0;
  writeFlash();
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_0);
  NVIC_EnableIRQ(EXTI0_IRQn);
  while(1)
  {
    led();
    delay(100);
  }
}
