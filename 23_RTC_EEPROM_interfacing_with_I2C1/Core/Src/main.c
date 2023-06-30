#include "stm32f4xx.h"

#define SLAVE_ADDR 0x68 /* 1101 000. DS1337 */

void I2C1_init(void);
void I2C1_burstRead(char saddr, char maddr, int n, char* data);
void delayMs(int n);

int main(void) {

	char timeDateReadback[15];
	I2C1_init();
	/* configure PA5 for the green LED (LD2) */

	RCC->AHB1ENR |= 1; /* enable GPIOA clock */
	GPIOA->MODER &=~0x00000C00; /* clear pin mode */
	GPIOA->MODER |= 0x00000400; /* setpin to output mode */

	while (1) {
		I2C1_burstRead(SLAVE_ADDR, 0, 7, timeDateReadback);
		if(timeDateReadback[0] & 1)
			GPIOA->ODR |= 0x00000020; /* turn on LED */
		else
			GPIOA->ODR &= ~0x00000020; /* turn off LED */

		delayMs(10);
	}
}
void I2C1_init(void) {

	RCC->AHB1ENR |= 2; /* Enable GPIOB clock */
	RCC->APB1ENR |=0x00200000; /* Enable I2C1 clock */

	/* configure PB8, PB9 pins for I2C1 */
	GPIOB->AFR[1] &= ~0x000000FF; /* PB8, PB9 I2C1 SCL, SDA */
	GPIOB->AFR[1] |= 0x00000044;
	GPIOB->MODER &= ~0x000F0000; /* PB8, PB9 use alternate function */
	GPIOB->MODER |= 0x000A0000;
	GPIOB->OTYPER |= 0x00000300; /* output open-drain */
	GPIOB->PUPDR &=~0x000F0000; /* with pull-ups */
	GPIOB->PUPDR |= 0x00050000;

	I2C1->CR1 = 0x8000; /* software reset I2C1 */
	I2C1->CR1 &= ~0x8000;/* out of reset */
	I2C1->CR2 = 0x0010; /* peripheral clock is 16 MHz*/
	I2C1->CCR = 80; /* standard mode, 100kHz clock */
	I2C1->TRISE = 17; /* maximum rise time */
	I2C1->CR1 |= 0x0001; /* enable I2C1 module */
}

void I2C1_burstRead(char saddr, char maddr, int n, char* data) {

	volatile int tmp;


	while (I2C1->SR2 & 2); /* wait until bus not busy */
	I2C1->CR1 &=~0x800; /* disable POS */
	I2C1->CR1 |= 0x100; /* generate start */

	while (!(I2C1->SR1 & 1)); /* wait until start flag is set */
	I2C1->DR = saddr << 1; /* transmit slave address + Write */

	while (!(I2C1->SR1 & 2)); /* wait until addr flag is set */
	tmp = I2C1->SR2; /* clear addr flag */

	while (!(I2C1->SR1 & 0x80));/* wait until transmitter empty */
	I2C1->DR = maddr; /* send memoryaddress */

	while (!(I2C1->SR1 & 0x80)); /* wait until transmitterempty */
	I2C1->CR1 |= 0x100; /* generate restart */

	while (!(I2C1->SR1 & 1));/* wait until start flag is set */
	I2C1->DR = saddr << 1 | 1;/*ransmit slave address + Read */

	while (!(I2C1->SR1 & 2)); /*until addr flag is set */
	tmp = I2C1->SR2; /* clear addr flag*/
	I2C1->CR1 |= 0x0400; /* Enable Acknowledge */


while(n > 0U)
{
/* One byte left */
	if(n == 1U)
	{
		I2C1->CR1 &= ~(0x400); /* Disable Acknowledge */
		I2C1->CR1 |= 0x200;/* Generate Stop */
		while (!(I2C1->SR1 & 0x40)); /* Wait for RXNEflag set */
		*data++ = I2C1->DR; /* Read data from DR */
		break;
	}
	else
	{
		while (!(I2C1->SR1 & 0x40)); /* Wait until RXNE flag is set */
		(*data++) = I2C1->DR; /* Read data from DR */
		n--;
	}
  }
}
/* 16 MHz SYSCLK */
void delayMs(int n) {
	int i;
	for (; n > 0; n--)
		for (i = 0; i < 3195; i++) ;
}
