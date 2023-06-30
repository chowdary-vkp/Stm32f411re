/*p4_2.c UART on USART2 Receive at 115200 Baud
**Receive key strokes from terminal
 emulator(TeraTerm) of the *
host PC to the USART2 of the Nucleo-F446RE board.
* USART2 is connected to the ST-Link virtual COM port.
* Launch Tera Term on a PC and hit any key.
* The LED program from P3_5 of Chapter 3 is used to blink the green
LED * according to the key received.
* You need to wait till the blinking stops before hitting another
key. * Received key is not echoed back to the terminal, so you will
not see * the character displayed.
*
* By default, the clock is running at 16 MHz.
* The UART2 is configured for 115200 Baud.* PA3 - USART2 RX (AF7)
*

*/
#include "stm32f4xx.h"
#include "usart.h"
#include "led.h"



/*------------------------------------------------------------------
---------MAIN function
*-------------------------------------------------------------------
---------*/
int main(void)
{
    char c;

    USART2_init();          /* initialize USART2 */
    LED_init();			    /* initialize PA5 */
    while (1)
    {                      /* Loop forever */
        c = USART2_read(); /* wait for a character received */
        LED_blink(c);
        /* blink the LED */
    }
}




