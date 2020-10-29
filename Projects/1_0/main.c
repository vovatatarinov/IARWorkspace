typedef unsigned long dword;
typedef unsigned char byte;
typedef unsigned short word;

#define HWREG(x) *(volatile dword*)(x)
#define GPIOG_BASE 0x40021800
#define GPIOA_BASE 0x40020000
#define RCC_BASE 0x40023800
//#define GPIO_BUS AHB1
//#define RCC_BUS AHB1
#define RCC_AHB1ENR_OFFSET 0x30
#define GPIOx_MODER_OFFSET 0x00
#define GPIOx_ODR_OFFSET 0x14
#define GPIOx_IDR 0x10
/*
void delay() {
    for (volatile int i = 0; i < 1600000; ++i);
}
*/
int main()
{
  //RCC on GPIOG
  HWREG(RCC_BASE + RCC_AHB1ENR_OFFSET) |= (1 << 6);
  //RCC on GPIOA
  HWREG(RCC_BASE + RCC_AHB1ENR_OFFSET) |= (1 << 0);
  //output mode on PG13
  HWREG(GPIOG_BASE + GPIOx_MODER_OFFSET) |= (1 << 26);
  HWREG(GPIOG_BASE + GPIOx_MODER_OFFSET) &= ~(1 << 27);
  //output mode on PG14
  HWREG(GPIOG_BASE + GPIOx_MODER_OFFSET) |= (1 << 28);
  HWREG(GPIOG_BASE + GPIOx_MODER_OFFSET) &= ~(1 << 29);
  //input mode on PA0
  HWREG(GPIOA_BASE + GPIOx_MODER_OFFSET) &= ~( (1 << 0) | (1 << 1) );
  while(1) {
    //delay();
    if (HWREG(GPIOA_BASE + GPIOx_IDR) & (1 << 0)) {
        //Turn off PG 13
        HWREG(GPIOG_BASE + GPIOx_ODR_OFFSET) &= ~(1 << 13);
        //Turn on PG 14
        HWREG(GPIOG_BASE + GPIOx_ODR_OFFSET) |= (1 << 14);
    }
    else {
        //Turn off PG 14
        HWREG(GPIOG_BASE + GPIOx_ODR_OFFSET) &= ~(1 << 14);
        //Turn on PG 13
        HWREG(GPIOG_BASE + GPIOx_ODR_OFFSET) |= (1 << 13);
    }
  }
 
}
