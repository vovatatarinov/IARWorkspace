#include "stm32f4xx_ll_bus.h"
//#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_rcc.h"

static short c;
void SPI5_IRQHandler() {
    c = LL_SPI_ReceiveData16(SPI5);
}

void delay(int ms) {
    
}

int main(){
    /* RCC для GPIOB и SPI2 */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
    LL_APB1_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI5);
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

    LL_SPI_SetBaudRatePrescaler(SPI5, LL_SPI_BAUDRATEPRESCALER_DIV2);
    LL_SPI_SetTransferBitOrder(SPI5, LL_SPI_MSB_FIRST);
    LL_SPI_SetTransferDirection(SPI5,  LL_SPI_FULL_DUPLEX);
    LL_SPI_SetDataWidth(SPI5, LL_SPI_DATAWIDTH_16BIT);
    LL_SPI_DisableCRC(SPI5);
    LL_SPI_SetNSSMode(SPI5, LL_SPI_NSS_SOFT);
    //LL_SPI_SetNSSMode(SPI2, LL_SPI_NSS_HARD_INPUT);
    LL_SPI_EnableIT_RXNE(SPI5);
    LL_SPI_Enable(SPI5);
    NVIC_EnableIRQ(SPI5_IRQn);
    /* prepare packet */
    short pack =  (0x0F << 8) | 0x0001;
        LL_GPIO_ResetOutputPin(GPIOF, (1 << 6));
    LL_SPI_TransmitData16(SPI5, pack);
    while (LL_SPI_IsActiveFlag_TXE(SPI5));
    //LL_GPIO_SetOutputPin(GPIOF, (1 << 6));
    while(1) {

        
    }
}

