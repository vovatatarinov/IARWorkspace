#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_rcc.h"
#include <stdbool.h>
//typedef unsigned char byte;
//typedef unsigned short word;
//typedef unsigned long dword;

char buf[65536];
static char* buf_ptr =(char*) &buf;

void free() {
  buf_ptr = (char*) &buf;
}

void delay(int ms) {
    for (volatile int i = 0; i < 16000; ++i) {
      for (volatile int j = 0; j < ms; ++j) {
        ;
      }
    }
}

void USART6_IRQHandler() {
    *buf_ptr = LL_USART_ReceiveData8(USART6);
    ++buf_ptr;
    if (!LL_USART_IsActiveFlag_RXNE(USART6))
      *buf_ptr = '\0';
    //while(!LL_USART_IsActiveFlag_TC(USART6));
    //LL_USART_TransmitData8(USART6, c);
}

void sendStr(const char* str) {
    while (*str != 0)  {
        while(!LL_USART_IsActiveFlag_TC(USART6));
        LL_USART_TransmitData8(USART6, *str);
        ++str;
    }
}

bool cmpStr(const char* str1, const char* str2) {
  while (*str1 != '\0' && str2 != '\0') {
    if (*str1 != *str2)
      return false;
    ++str1;
    ++str2;
  }
  if (*str1 != *str2)
    return false;
  return true;
}

bool sendATwithECHO() {
    const char* cmd = "AT\r\n";
    sendStr(cmd);
    delay(100);
    bool res = cmpStr("AT\r\r\n\r\nOK\r\n", buf);
    free();
    return res;
}

bool sendAT() {
    const char* cmd = "AT\r\n";
    sendStr(cmd);
    delay(100);
    bool res = cmpStr("\r\nOK\r\n", buf);
    free();
    return res;
}

bool sendATCWMODE1() {
    const char* cmd = "AT+CWMODE=1\r\n";
    sendStr(cmd);
    delay(100);
    bool res = cmpStr("\r\nOK\r\n", buf);
    free();
    return res;
}

bool sendATE0() {
  const char* cmd = "ATE0\r\n";
  sendStr(cmd);
  delay(100);
  bool res = cmpStr("ATE0\r\r\n\r\nOK\r\n", buf);
  free();
  return res;
}

void panic() {
  while (1) {
    ;
  }
}

int main(){
    /* RCC для GPIOC и USART6 */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART6);
    /* Альтернативная функция для GPIO */
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOC, LL_GPIO_PIN_6, LL_GPIO_AF_8);
    LL_GPIO_SetAFPin_0_7(GPIOC, LL_GPIO_PIN_7, LL_GPIO_AF_8);
    /* Обмен данными в обоих направлениях */
    LL_USART_EnableDirectionRx(USART6);
    LL_USART_EnableDirectionTx(USART6);
    /* Отключаем проверку четности */
    LL_USART_SetParity(USART6, LL_USART_PARITY_NONE);
    /* Размер слова */
    LL_USART_SetDataWidth(USART6, LL_USART_DATAWIDTH_8B);
    /* Установить количество стоповых битов равным одному. */
    LL_USART_SetStopBitsLength(USART6, LL_USART_STOPBITS_1);
    /* Бауд рейт */
    LL_RCC_ClocksTypeDef RCCClocks;
    LL_RCC_GetSystemClocksFreq(&RCCClocks);
    LL_USART_SetBaudRate(USART6, RCCClocks.PCLK2_Frequency, LL_USART_OVERSAMPLING_16, 115200);
    LL_USART_EnableIT_RXNE(USART6);
    LL_USART_Enable(USART6);
    NVIC_EnableIRQ(USART6_IRQn);
    if (!sendATwithECHO())
      panic();
    if(!sendATE0())
      panic();
    if (!sendAT())
      panic();
    if (!sendATCWMODE1())
      panic();
    while(1) {
      
    }
}
