#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_adc.h"

int var;
int iamhere = 0;

void TIM1_UP_TIM10_IRQHandler() {
	LL_TIM_ClearFlag_UPDATE(TIM1);
	LL_ADC_REG_StartConversionSWStart(ADC1);
    iamhere = 1;
}

void ADC_IRQHandler() {
	LL_ADC_ClearFlag_EOCS(ADC1);
	var = LL_ADC_REG_ReadConversionData10(ADC1);
}

int main()
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ANALOG);
  LL_TIM_SetPrescaler(TIM1, 15900);
  LL_TIM_SetCounterMode(TIM1, LL_TIM_COUNTERMODE_UP);
  LL_TIM_SetAutoReload(TIM1, 1000);
  LL_TIM_EnableUpdateEvent(TIM1);
  LL_TIM_EnableIT_UPDATE(TIM1);
  LL_TIM_EnableCounter(TIM1);
  LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_10B);
  LL_ADC_SetDataAlignment(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
  LL_ADC_SetSequencersScanMode(ADC1, LL_ADC_SEQ_SCAN_DISABLE);
  LL_ADC_REG_SetTriggerSource(ADC1,  LL_ADC_REG_TRIG_SOFTWARE);
  LL_ADC_REG_SetSequencerRanks(ADC1,  LL_ADC_REG_RANK_1,  LL_ADC_CHANNEL_3);
  LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
  LL_ADC_EnableIT_EOCS(ADC1);
  LL_ADC_Enable(ADC1);
  NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
  NVIC_EnableIRQ(ADC_IRQn);
  while(1)
  {
  }
}
