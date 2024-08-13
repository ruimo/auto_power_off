#include "ch32v003fun.h"
#include <stdio.h>
#include <stdbool.h>

volatile bool requestCounterReset = false;

/*
 This code uses fast interrupts, but be warned, in this mode your hardware stack 
 is only 2 interrupt calls deep!!!

 When you execute code here, from RAM, the latency is between 310 and 480ns.
 From RAM is using  __attribute__((section(".srodata")));

Chart is in Cycles Spent @ 48MHz

|   __attribute__ |  HPE ON  |  HPE OFF |
| ((interrupt))   |   29  |   28  |
| ((section(".srodata"))) and ((interrupt)) | 28 | 23 |
| ((naked)) | 23 | 21 |
| ((section(".srodata"))) and ((naked)) | 15 | 16| 

  These were done with an empty (nop) loop in main.

  HPE ON  = 0x804 = 3
  HPE OFF = 0x804 = 0

  Bog-standard interrupt test with variance. I.e.
	__attribute__((interrupt)) with cursed code in main loop.

  Variance tests: 27-30 cycles
  
  Which will manifest as interurpt jitter.
*/

void EXTI7_0_IRQHandler(void) __attribute__((interrupt));
void EXTI7_0_IRQHandler(void) {
  requestCounterReset = true;

  // Acknowledge the interrupt
  EXTI->INTFR = EXTI_Line2;
}

void initPort(void) {
  // Enable GPIOs
  RCC->APB2PCENR = RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO;

  // GPIO D3 for input pin change.
  //  funPinMode(PD3, GPIO_CFGLR_IN_FLOAT);
  // funPinMode( PD4, GPIO_CFGLR_IN_PUPD ); // Keep SWIO enabled / seems to be unnecessary

  funPinMode(PA1, GPIO_CFGLR_OUT_10Mhz_PP);
  funPinMode(PA2, GPIO_CFGLR_OUT_10Mhz_PP);

  funPinMode(PC1, GPIO_CFGLR_OUT_10Mhz_PP);
  funPinMode(PC2, GPIO_CFGLR_IN_FLOAT);
  funPinMode(PC3, GPIO_CFGLR_IN_PUPD);

  funPinMode(PD0, GPIO_CFGLR_OUT_10Mhz_PP);
  funPinMode(PD1, GPIO_CFGLR_OUT_10Mhz_PP);
  funPinMode(PD2, GPIO_CFGLR_OUT_10Mhz_PP);
  funPinMode(PD3, GPIO_CFGLR_OUT_10Mhz_PP);
  funPinMode(PD4, GPIO_CFGLR_OUT_10Mhz_PP);
  funPinMode(PD5, GPIO_CFGLR_OUT_10Mhz_PP);
  funPinMode(PD6, GPIO_CFGLR_OUT_10Mhz_PP);
  funPinMode(PD7, GPIO_CFGLR_OUT_10Mhz_PP);
}

void setDigitToLed(uint8_t digit) {
  switch (digit) {
  case 0:
    funDigitalWrite(PD0, FUN_LOW);
    funDigitalWrite(PD2, FUN_LOW);
    funDigitalWrite(PD3, FUN_LOW);
    funDigitalWrite(PD4, FUN_LOW);
    funDigitalWrite(PD5, FUN_LOW);
    funDigitalWrite(PD6, FUN_LOW);
    funDigitalWrite(PD7, FUN_HIGH);
    break;

  case 1:
    funDigitalWrite(PD0, FUN_HIGH);
    funDigitalWrite(PD2, FUN_HIGH);
    funDigitalWrite(PD3, FUN_HIGH);
    funDigitalWrite(PD4, FUN_LOW);
    funDigitalWrite(PD5, FUN_LOW);
    funDigitalWrite(PD6, FUN_HIGH);
    funDigitalWrite(PD7, FUN_HIGH);
    break;

  case 2:
    funDigitalWrite(PD0, FUN_LOW);
    funDigitalWrite(PD2, FUN_HIGH);
    funDigitalWrite(PD3, FUN_LOW);
    funDigitalWrite(PD4, FUN_LOW);
    funDigitalWrite(PD5, FUN_HIGH);
    funDigitalWrite(PD6, FUN_LOW);
    funDigitalWrite(PD7, FUN_LOW);
    break;

  case 3:
    funDigitalWrite(PD0, FUN_HIGH);
    funDigitalWrite(PD2, FUN_HIGH);
    funDigitalWrite(PD3, FUN_LOW);
    funDigitalWrite(PD4, FUN_LOW);
    funDigitalWrite(PD5, FUN_LOW);
    funDigitalWrite(PD6, FUN_LOW);
    funDigitalWrite(PD7, FUN_LOW);
    break;

  case 4:
    funDigitalWrite(PD0, FUN_HIGH);
    funDigitalWrite(PD2, FUN_LOW);
    funDigitalWrite(PD3, FUN_HIGH);
    funDigitalWrite(PD4, FUN_LOW);
    funDigitalWrite(PD5, FUN_LOW);
    funDigitalWrite(PD6, FUN_HIGH);
    funDigitalWrite(PD7, FUN_LOW);
    break;

  case 5:
    funDigitalWrite(PD0, FUN_HIGH);
    funDigitalWrite(PD2, FUN_LOW);
    funDigitalWrite(PD3, FUN_LOW);
    funDigitalWrite(PD4, FUN_HIGH);
    funDigitalWrite(PD5, FUN_LOW);
    funDigitalWrite(PD6, FUN_LOW);
    funDigitalWrite(PD7, FUN_LOW);
    break;

  case 6:
    funDigitalWrite(PD0, FUN_LOW);
    funDigitalWrite(PD2, FUN_LOW);
    funDigitalWrite(PD3, FUN_LOW);
    funDigitalWrite(PD4, FUN_HIGH);
    funDigitalWrite(PD5, FUN_LOW);
    funDigitalWrite(PD6, FUN_LOW);
    funDigitalWrite(PD7, FUN_LOW);
    break;

  case 7:
    funDigitalWrite(PD0, FUN_HIGH);
    funDigitalWrite(PD2, FUN_HIGH);
    funDigitalWrite(PD3, FUN_LOW);
    funDigitalWrite(PD4, FUN_LOW);
    funDigitalWrite(PD5, FUN_LOW);
    funDigitalWrite(PD6, FUN_HIGH);
    funDigitalWrite(PD7, FUN_HIGH);
    break;

  case 8:
    funDigitalWrite(PD0, FUN_LOW);
    funDigitalWrite(PD2, FUN_LOW);
    funDigitalWrite(PD3, FUN_LOW);
    funDigitalWrite(PD4, FUN_LOW);
    funDigitalWrite(PD5, FUN_LOW);
    funDigitalWrite(PD6, FUN_LOW);
    funDigitalWrite(PD7, FUN_LOW);
    break;

  case 9:
    funDigitalWrite(PD0, FUN_HIGH);
    funDigitalWrite(PD2, FUN_LOW);
    funDigitalWrite(PD3, FUN_LOW);
    funDigitalWrite(PD4, FUN_LOW);
    funDigitalWrite(PD5, FUN_LOW);
    funDigitalWrite(PD6, FUN_LOW);
    funDigitalWrite(PD7, FUN_LOW);
    break;
  }
}

void initIsr() {
  // Ugh this is tricky.
  // This is how we set (INTSYSCR) to enable hardware interrupt nesting
  // and hardware stack. BUT this means we can only stack intterrupts 2 deep.
  //
  // This feature is called "HPE"
  //
  // Note: If you don't do this, you will need to set your functions to be
  // __attribute__((interrupt)) instead of  __attribute__((naked))  with an
  // mret.
  //
  // PLEASE BE CAREFUL WHEN DOING THIS THOUGH.  There are a number of things
  // you should know with HPE.  The issue is that HPE doesn't preserve s0,
  // and s1. You should review the following material before using HPE.
  //   https://github.com/cnlohr/ch32v003fun/issues/90
  //   https://www.reddit.com/r/RISCV/comments/126262j/notes_on_wch_fast_interrupts/
  //   https://www.eevblog.com/forum/microcontrollers/bizarre-problem-on-ch32v003-with-systick-isr-corrupting-uart-tx-data
  asm volatile(
#if __GNUC__ > 10
    ".option arch, +zicsr\n"
#endif
    "addi t1, x0, 3\n"
    "csrrw x0, 0x804, t1\n"
    : : :  "t1" );

  // Configure the IO as an interrupt.
  AFIO->EXTICR = AFIO_EXTICR_EXTI2_PC;
  EXTI->INTENR = EXTI_INTENR_MR2; // Enable EXT2
  EXTI->RTENR = EXTI_RTENR_TR2;  // Rising edge trigger

  // enable interrupt
  NVIC_EnableIRQ(EXTI7_0_IRQn);
}

void turnOffLed(void) {
  funDigitalWrite(PA1, FUN_LOW);
  funDigitalWrite(PA2, FUN_LOW);
}

void showCountToLed(uint8_t count) {
    uint8_t digit10 = count / 10;
    uint8_t digit1 = count % 10;
    for (uint8_t sec = 0; sec < 60; ++sec) {
      for (uint8_t i = 0; i < 50; ++i) {
        if (digit10 == 0) {
          turnOffLed();
        } else {
          setDigitToLed(digit10);
          funDigitalWrite(PA1, FUN_HIGH);
          funDigitalWrite(PA2, FUN_LOW);
        }
        Delay_Ms(10);
        if (requestCounterReset) return;
        
        setDigitToLed(digit1);
        funDigitalWrite(PA1, FUN_LOW);
        funDigitalWrite(PA2, FUN_HIGH);
        Delay_Ms(10);
        if (requestCounterReset) return;
      }
    }
}

const uint8_t INIT_COUNT = 10;

int main() {
  SystemInit();
  initPort();
  initIsr();

  uint8_t count = INIT_COUNT;
  while (1) {
    if (count == 0) {
      if (funDigitalRead(PC3) == FUN_HIGH) {
        count = INIT_COUNT;
        Delay_Ms(200); // Avoid chattering.
        while (funDigitalRead(PC3) == FUN_HIGH);
      }
      funDigitalWrite(PC1, FUN_LOW);
      turnOffLed();
    } else {
      if (funDigitalRead(PC3) == FUN_HIGH) {
        count = 0;
        Delay_Ms(200); // Avoid chattering.
        while (funDigitalRead(PC3) == FUN_HIGH);
      } else {
        funDigitalWrite(PC1, FUN_HIGH);
        if (requestCounterReset) {
          requestCounterReset = false;
          count = INIT_COUNT;
        }

        showCountToLed(count);
        if (0 < count) --count;
      }
    }
  }
}
