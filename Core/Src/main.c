#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Definitions
// LCD
#define LCD_ADDR  			0x27			// LCD Address
#define LCD_EN 	  			0x04			// Enable
#define LCD_RS    			0x01			// Register Select
#define LCD_BL	  			0x08			// Back-light
// LCD Commands
#define LCD_CLEAR 			0x01			// Clear Screen
#define LCD_HOME  			0x02			// Cursor Home
#define LCD_ENTRY_MODE  	0x06			// Increments cursor, no shift
#define LCD_DISPLAY_ON  	0x0C			// Display on, cursor off, blink off
#define LCD_FUNCTION_SET 	0x28			// 4-bit mode, 2 lines, 5x8 font
// Light Sensor
#define Light_ADDR			0x23			// Light Sensor Address


// Prototypes
void USART_Init();
void I2C_Init();
void LCD_Init();
void LCD_Send_Command(uint8_t);
void LCD_Send_Data(uint8_t);
void LCD_Send_String(char*);
//void Find_Addr();


int main () {
	USART_Init();
	I2C_Init();
	LCD_Init();

	LCD_Send_String("Hello");
	while (1) {

	}
}

void I2C_Init() {
	RCC -> APB1ENR |= (1 << 21);									// I2C1 Clock
	RCC -> AHB1ENR |= (1 << 1);										// GPIOB Clock

	GPIOB -> MODER &= ~((3 << (2 * 8)) | (3 << (2 * 9)));			// PB8 & 9 to Alternate Function
	GPIOB -> MODER |= (2 << (2 * 8)) | (2 << (2 * 9));

	GPIOB -> OTYPER |= (1 << 8) | (1 << 9);							// Open-Drain on PB8 & 9

	GPIOB -> PUPDR |= (1 << (2 * 8)) | (1 << (2 * 9));				// Pull-Up Resistors

	GPIOB -> AFR[1] |= (4 << 0) | (4 << 4);							// Alternate Function to I2C1

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
int I2C_Send_Address(uint8_t address, uint8_t read) {
	I2C1 -> DR = (address << 1) | read;
	for (volatile int i = 0; i < 5000; i++);
	if (I2C1 -> SR1 & (1 << 10)) {
		return 0;
	}
	while(!(I2C1 -> SR1 & (1 << 1)));

	uint8_t temp = I2C1 -> SR1;
	temp = I2C1 -> SR2;
	(void) temp;
	return 1;
}

// Sends a byte of data to the device
void I2C_Send_Data(uint8_t data) {
	while(!(I2C1 -> SR1 & (1 << 7)));
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

void LCD_Send_Command(uint8_t cmd) {
	uint8_t upper = (cmd & 0xF0) | LCD_BL;
	uint8_t lower = ((cmd << 4) & 0xF0) | LCD_BL;

	I2C_Write(LCD_ADDR, upper | LCD_EN);
	I2C_Write(LCD_ADDR, upper);

	I2C_Write(LCD_ADDR, lower | LCD_EN);
	I2C_Write(LCD_ADDR, lower);
}

void LCD_Init() {
	LCD_Send_Command(0x33);
	LCD_Send_Command(0x32);
	LCD_Send_Command(LCD_FUNCTION_SET);
	LCD_Send_Command(LCD_ENTRY_MODE);
	LCD_Send_Command(LCD_DISPLAY_ON);
	LCD_Send_Command(LCD_CLEAR);
}

void LCD_Send_Data(uint8_t data) {
	uint8_t upper = (data & 0xF0) | LCD_BL | LCD_RS;
	uint8_t lower = ((data << 4) & 0xF0) | LCD_BL | LCD_RS;

	I2C_Write(LCD_ADDR, upper | LCD_EN);
	I2C_Write(LCD_ADDR, upper);

	I2C_Write(LCD_ADDR, lower | LCD_EN);
	I2C_Write(LCD_ADDR, lower);
}

void LCD_Send_String(char* str) {
	while (*str != '\0' ) {
		LCD_Send_Data(*str);
		str++;
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

// printf() retargeting
int __io_putchar(int c) {
	while(!(USART2 -> SR & (1 << 7)));
	USART2 -> DR = c;

	while(!(USART2 -> SR & (1 << 6)));
	USART2 -> SR &= ~(1 << 6);
	return c;
}

// Used to discover device addresses - No longer necessary
//void Find_Addr() {
//    printf("Starting scan...\r\n");
//
//    for (int i = 0; i < 128; i++) {
//        I2C_Start();
//
//        I2C1->DR = (i << 1) | 0;
//
//        for (volatile int j = 0; j < 50000; j++);
//
//        if (!(I2C1->SR1 & (1 << 10))) {
//            printf("Device found at: 0x%02X\r\n", i);
//        }
//
//        I2C1->SR1 &= ~(1 << 10);
//        I2C_Stop();
//
//        for (volatile int j = 0; j < 10000; j++);
//    }
//}
