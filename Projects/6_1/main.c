#include "stm32f4xx_ll_bus.h"
//#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_rcc.h"


void delay(int ms) {
    for (volatile int i = 0; i < 16000; ++i) {
      for (volatile int j = 0; j < ms; ++j) {
        ;
      }
    }
}


static short c[8];
static int var = 0;
void SPI5_IRQHandler() {
  if (var < 8) {
    c[var] = LL_SPI_ReceiveData8(SPI5);
    //delay(10);
    ++var;
    while (LL_SPI_IsActiveFlag_RXNE(SPI5));
  }
}

int main(){
    /* RCC для GPIOB и SPI2 */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI5);
    /* Альтернативная функция для GPIO */
    //LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_12, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinMode(GPIOF, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOF, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinMode(GPIOF, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinMode(GPIOF, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetOutputPin(GPIOF, (1 << 6));
    //LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_12, LL_GPIO_AF_5);
    LL_GPIO_SetAFPin_0_7(GPIOF, LL_GPIO_PIN_7, LL_GPIO_AF_5);
    LL_GPIO_SetAFPin_8_15(GPIOF, LL_GPIO_PIN_8, LL_GPIO_AF_5);
    LL_GPIO_SetAFPin_8_15(GPIOF, LL_GPIO_PIN_9, LL_GPIO_AF_5);
    LL_SPI_SetMode(SPI5, LL_SPI_MODE_MASTER);
    LL_SPI_SetClockPhase(SPI5,  LL_SPI_PHASE_2EDGE);
    LL_SPI_SetClockPolarity(SPI5,  LL_SPI_POLARITY_HIGH);

    LL_SPI_SetBaudRatePrescaler(SPI5, LL_SPI_BAUDRATEPRESCALER_DIV8);
    LL_SPI_SetTransferBitOrder(SPI5, LL_SPI_MSB_FIRST);
    LL_SPI_SetTransferDirection(SPI5,  LL_SPI_FULL_DUPLEX);
    LL_SPI_SetDataWidth(SPI5, LL_SPI_DATAWIDTH_8BIT);
    LL_SPI_DisableCRC(SPI5);
    LL_SPI_SetNSSMode(SPI5, LL_SPI_NSS_SOFT);
    LL_SPI_EnableIT_RXNE(SPI5);
    LL_SPI_Enable(SPI5);
    /*
    LL_GPIO_ResetOutputPin(GPIOF, (1 << 6));
    LL_SPI_TransmitData16(SPI5, 0x200F);
        //delay(10);
    LL_SPI_ReceiveData16(SPI5);
    while (LL_SPI_IsActiveFlag_TXE(SPI5));
    LL_GPIO_SetOutputPin(GPIOF, (1 << 6));
    */
    NVIC_EnableIRQ(SPI5_IRQn);
    /* prepare packet */
    short pack =  0x8F;
    
    //pack =  1 | (0x0F << 2);
    //LL_GPIO_SetOutputPin(GPIOF, (1 << 6));
    while(1) {
        LL_GPIO_ResetOutputPin(GPIOF, (1 << 6));
        LL_SPI_TransmitData8(SPI5, pack);
        while (LL_SPI_IsActiveFlag_TXE(SPI5));
        delay(10);
        LL_GPIO_SetOutputPin(GPIOF, (1 << 6));
        delay(10);
    }
}

