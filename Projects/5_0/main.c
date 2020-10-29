#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_rcc.h"

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;

char c;

void USART6_IRQHandler() {
    c = LL_USART_ReceiveData8(USART6);
    while(!LL_USART_IsActiveFlag_TC(USART6));
    LL_USART_TransmitData8(USART6, c);
}

void sendStr(char* str) {
    while (*str != 0)  {
        while(!LL_USART_IsActiveFlag_TC(USART6));
        LL_USART_TransmitData8(USART6, *str);
        ++str;
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
    LL_USART_SetBaudRate(USART6, RCCClocks.PCLK2_Frequency, LL_USART_OVERSAMPLING_16, 9600);
    LL_USART_EnableIT_RXNE(USART6);
    LL_USART_Enable(USART6);
    sendStr("Hello STM32!\r\n");
    NVIC_EnableIRQ(USART6_IRQn);

    while(1) {

    }
}
