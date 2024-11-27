#include "main.h"
#include <stdio.h>
#include <string.h>

// Prototypes
void USART_Init();

int main () {
	USART_Init();


	while (1) {
		printf("Hello!\r\n");
		for (volatile int i = 0; i < 10000000; i++);
	}
}

void USART_Init() {
	RCC -> APB1ENR |= (1 << 17);									// USART2 Clock
	RCC -> AHB1ENR |= (1 << 0);										// GPIOA Clock

	GPIOA -> MODER &= ~((3 << (2 * 2)) | (3 << (2 * 3)));			// Initialize PA2 & PA3 to Alternate Function
	GPIOA -> MODER |= (2 << (2 * 2)) | (2 << (2 * 3));

	GPIOA -> AFR[0] |= (7 << (4 * 2)) | (7 << (4 * 3));				// AFR to USART2

	USART2 -> BRR = 0x0683;											// Baud rate = 9600

	USART2 -> CR1 |= (1 << 2) | (1 << 3);							// Receiver and Transmitter Enabled
	USART2 -> CR1 |= (1 << 13);										// USART2 Enabled
}

int __io_putchar(int c) {											// printf() retargetting
	while(!(USART2 -> SR & (1 << 7)));
	USART2 -> DR = c;

	while(!(USART2 -> SR & (1 << 6)));
	USART2 -> SR &= ~(1 << 6);
	return c;
}
