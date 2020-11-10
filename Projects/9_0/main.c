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
#define FLASH_SIGN 0x55AA55AA
/* 
64 KBytes for 4th sector.
from 0x08010000 to 0x0801FFFF
*/

/*
Во флешку будет писаться служебная информация для экономии места

1-ые 31 байт зарезервированы. Они будут использоваться как подпись.
При её наличии таблица расположения информации будет считаться действительной.
Подпись: 0x55AA55AA в начале сектора. Остальные 27 байт будут незадействованы.

Потом идут 1985 байт таблицы.
Каждый конкретный бит этого огромного числа будет соответствовать наличию
или отсутствию в данном месте нужной информации.

Затем, 63520 байт полезной информации. 

Если флешка полностью заполнена, то происходит сброс сектора.
Причем последнее 32-битное число перемещается в начало блока с полезной информацией.
*/


int counter;

static inline void unlockFlash() {
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
}

static inline void lockFlash() {
  FLASH->CR |= FLASH_CR_LOCK;
}

static inline void wait4mem() {
  while((FLASH->SR & FLASH_SR_BSY));
}

void eraseSector() {
  unlockFlash();
  wait4mem();
  FLASH->CR |= FLASH_CR_SER; // Enable erase
  FLASH->CR &= ~FLASH_CR_SNB; // Set 4th sector
  FLASH->CR |= (FLASH_CR_SNB & (4 << 3));
  FLASH->CR |= FLASH_CR_STRT; // Start erase
  wait4mem();
  FLASH->CR &= ~FLASH_CR_SER;
  lockFlash();
}

dword readFlash(word ptr) {
  return HWREG((dword)ADDR_FLASH_SECTOR_4 + ptr);
}

bool isTableValid() {
  if (readFlash(0) == FLASH_SIGN)
    return true;
  return false;
}

word searchPtr() {
  //63520 / 4 = 15880
  int i;
  for (i = 0; i < 15880; ++i) {
	  int byte = i / 8;
	  int offset = i % 8;
	  int ptr = 31 + byte;
	  int value = (readFlash(ptr) >> offset) & 0x1;
      if (value == 1)
		break;
      if (i == 15879)
	    return -1; //this is an overflow signal 0xFFFF;
  }
  i = i * 4;
  return i;
}

void writeFlash(word ptr, dword data) {
  //Use a 16-bit variable for ptr.
  //Guarantee that we are now inside the sector
  //pageErase();
  unlockFlash();
  wait4mem();
  FLASH->CR |= FLASH_CR_PG;
  FLASH->CR &= ~FLASH_CR_PSIZE_0;
  FLASH->CR |= FLASH_CR_PSIZE_1;
  wait4mem();
  HWREG((dword)ADDR_FLASH_SECTOR_4 + ptr) = data;
  wait4mem();
  FLASH->CR &= ~FLASH_CR_PG;
  lockFlash();
}

void writeData2Flash(dword);
dword readDataFromFlash();

void clearSector() {
  
}

void writeData2Flash(dword data) {
  if (searchPtr() == -1)
    clearSector(); 
    
}

dword readDataFromFlash() {

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

void eraseMemSignal() {
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
      eraseMemSignal();
      eraseSector();
	  writeFlash(0, FLASH_SIGN);//write a signature
     }
  }
  if (searchPtr() == 0) {
	  
  }
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
