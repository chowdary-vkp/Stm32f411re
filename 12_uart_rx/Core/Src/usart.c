#include "stm32f4xx.h"
#include "usart.h"


/*------------------------------------------------------------------
---------Initialize UART pins, Baudrate
*-------------------------------------------------------------------
---------*/
void USART1_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    /* Enable GPIOA clock */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 	/* Enable USART1 clock */
    USART1->BRR = 0x008B; 		/* 115200 baud @ 16 MHz */
    USART1->CR1 = 0x0004;     /* enable Rx, 8-bit data */
    USART1->CR2 = 0x0000;     /* 1 stop bit */
    USART1->CR3 = 0x0000;     /* no flow control */
    USART1->CR1 |= USART_CR1_UE; // Enable USART1

    /* Configure PA10 for USART1 RX */
    GPIOA->MODER &= ~(0x3 << (10 * 2));  // clear mode bits for PA10
    GPIOA->MODER |= (0x2 << (10 * 2));   // set PA10 to alternate function mode
    GPIOA->AFR[1] &= ~(0xF << ((10 - 8) * 4));  // clear bits for PA10 in AFRL
    GPIOA->AFR[1] |= (0x7 << ((10 - 8) * 4));   // set AF7 for PA10

}

/* Read a character from USART1 */
char USART1_read(void)
{
    while (!(USART1->SR & 0x0020))
    {
    } // wait until char arrives
    return USART1->DR;
}




