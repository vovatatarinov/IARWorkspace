#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_rcc.h"
#include <stdbool.h>

//typedef unsigned char byte;
//typedef unsigned short word;
//typedef unsigned long dword;

char buf[65536];
static char *buf_ptr = (char *)buf;

void free()
{
  buf_ptr = (char *)buf;
}

void delay(int ms)
{
  for (volatile int i = 0; i < 16000; ++i)
  {
    for (volatile int j = 0; j < ms; ++j)
    {
      ;
    }
  }
}

void USART6_IRQHandler()
{
  *buf_ptr = LL_USART_ReceiveData8(USART6);
  ++buf_ptr;
  if (!LL_USART_IsActiveFlag_RXNE(USART6))
    *buf_ptr = '\0';
  //while(!LL_USART_IsActiveFlag_TC(USART6));
  //LL_USART_TransmitData8(USART6, c);
}

void sendStr(const char *str)
{
  while (*str != 0)
  {
    while (!LL_USART_IsActiveFlag_TC(USART6))
      ;
    LL_USART_TransmitData8(USART6, *str);
    ++str;
  }
}

void sendInt(int x) {
  char buf[32];
  int ptr = 30;
  for (int i = 0; i < 32; ++i)
    buf[i] = '\0';
  if (x == 0) {
    sendStr("0");
    return;
  }
  while (x != 0) {
    buf[ptr] = (x % 10) + '0';
    x /= 10;
    --ptr;
  }
  if (x < 0)
    buf[ptr] = '-';
  else
    ++ptr;
  sendStr(&buf[ptr]);
}

bool cmpStr(const char *str1, const char *str2)
{
  while (*str1 != '\0' && str2 != '\0')
  {
    if (*str1 != *str2)
      return false;
    ++str1;
    ++str2;
  }
  if (*str1 != *str2)
    return false;
  return true;
}

int findLength(const char *str)
{
  int res = 0;
  while (*str != '\0')
  {
    ++res;
    ++str;
  }
  return res;
}

bool findOK(const char *str)
{
  int l = findLength(str);
  char b[7];
  b[6] = str[l];
  b[5] = str[l - 1];
  b[4] = str[l - 2];
  b[3] = str[l - 3];
  b[2] = str[l - 4];
  b[1] = str[l - 5];
  b[0] = str[l - 6];
  return cmpStr("\r\nOK\r\n", b);
}

bool sendAT()
{
  const char *cmd = "AT\r\n";
  sendStr(cmd);
  delay(100);
  bool res = findOK(buf);
  free();
  return res;
}

bool sendATCWMODE1()
{
  const char *cmd = "AT+CWMODE=1\r\n";
  sendStr(cmd);
  delay(100);
  bool res = findOK(buf);
  free();
  return res;
}

bool sendATE0()
{
  const char *cmd = "ATE0\r\n";
  sendStr(cmd);
  delay(100);
  bool res = findOK(buf);;
  free();
  return res;
}

bool sendATCWLAP()
{
  const char *cmd = "AT+CWLAP\r\n";
  sendStr(cmd);
  delay(1000);
  bool res1 = findOK(buf);
  free();
  return res1;
}

bool sendATCWJAP() {
  const char *cmd = "AT+CWJAP=\"N8\",\"J2KhEPDc\"\r\n";
  sendStr(cmd);
  delay(1000);
  bool res = findOK(buf);
  free();
  return res;
}

bool sendATCIFSR() {
  const char *cmd = "AT+CIFSR\r\n";
  sendStr(cmd);
  delay(250);
  bool res = findOK(buf);
  free();
  return res; 
}

bool connect2server() {
  const char *cmd = "AT+CIPSTART=\"TCP\",\"www.worldtimeapi.org\",80\r\n";
  sendStr(cmd);
  delay(250);
  bool res = findOK(buf);
  free();
  return res; 
}

void getHTTP() {
  const char *get = "GET /api/timezone/Europe/Moscow.txt HTTP/1.1\r\nHost: www.worldtimeapi.org\r\nConnection: close\r\n\r\n";
  int l = findLength(get);
  const char *cmd = "AT+CIPSEND=";
  const char *rn = "\r\n";
  sendStr(cmd);
  sendInt(l);
  sendStr(rn);
  delay(10);
  sendStr(get);
  delay(1000);
}

void panic()
{
  while (1)
  {
    ;
  }
}

int main()
{
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
  if (!sendAT())
    panic();
  if (!sendATE0())
    panic();
  if (!sendAT())
    panic();
  if (!sendATCWMODE1())
    panic();
  if (!sendATCWLAP())
    panic();
  if (!sendATCWJAP())
    panic(); 
  if (!sendATCIFSR())
    panic();
  if (!connect2server())
    panic();
  getHTTP();
  while (1)
  {
  }
}
