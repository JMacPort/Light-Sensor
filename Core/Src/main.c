#include "main.h"
#include <stdio.h>
#include <string.h>

// Definitions
#define LCD_ADDR  			0x27			// LCD Address
#define LCD_EN 	  			0x04			// Enable
#define LCD_RW    			0x02			// Read/Write
#define LCD_RS    			0x01			// Register Select
#define LCD_BL	  			0x08			// Back-light
// LCD Commands
#define LCD_CLEAR 			0x01			// Clear Screen
#define LCD_HOME  			0x02			// Cursor Home
#define LCD_ENTRY_MODE  	0x06			// Increments cursor, no shift
#define LCD_DISPLAY_ON  	0x0C			// Display on, cursor off, blink off
#define LCD_FUNCTION_SET 	0x28			// 4-bit mode, 2 lines, 5x8 font


// Prototypes
void USART_Init();

int main () {
	USART_Init();


	while (1) {

	}
}

void I2C_Init() {
	RCC -> APB1ENR |= (1 << 21);									// I2C1 Clock

	I2C1 -> CR1 |= (1 << 15);										// I2C1 Restart
	I2C1 -> CR1 &= ~(1 << 15);

	I2C1 -> CR2 |= (42 << 0);										// Frequency = 42
	I2C1 -> CCR = 210;												// Communication Speed = 210
	I2C1 -> TRISE = 43;												// Rise Time = 43

	I2C1 -> CR1 |= (1 << 0);										// I2C1 Enable
}

// Checks if bus is free
int I2C_Check_Busy() {
	if (I2C1 -> SR2 & (1 << 1)) {
		return 1;
	}
	return 0;
}

// Generates start bit and waits for generation to complete
void I2C_Start() {
	I2C1 -> CR1 |= (1 << 8);
	while (!(I2C1 -> SR1 & (1 << 0)));
}

// Sends device address for communication
void I2C_Send_Address(uint8_t address, uint8_t read) {
	I2C1 -> DR |= (address << 1) | read;
	while(!(I2C1 -> SR1 & (1 << 1)));

	uint8_t temp = I2C1 -> SR1;
	temp = I2C1 -> SR2;
	(void) temp;
}

// Sends a byte of data to the device
void I2C_Send_Data(uint8_t data) {
	while(!(I2C1 -> DR & (1 << 7)));
	I2C1 -> DR = data;
	while(!(I2C1 -> SR1 & (1 << 2)));
}

// Generates stop condition
void I2C_Stop() {
	I2C1 -> CR1 |= (1 << 9);
}

// Complete I2C Cycle; Writes a byte of data to a device
void I2C_Write(uint8_t address, uint8_t data) {
	while(I2C_Check_Busy());
	I2C_Start();
	I2C_Send_Address(address, 0);
	I2C_Send_Data(data);
	I2C_Stop();
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

// printf() retargeting
int __io_putchar(int c) {
	while(!(USART2 -> SR & (1 << 7)));
	USART2 -> DR = c;

	while(!(USART2 -> SR & (1 << 6)));
	USART2 -> SR &= ~(1 << 6);
	return c;
}
