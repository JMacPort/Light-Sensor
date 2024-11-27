# I2C Light Sensor with LCD and USART Serial Terminal
Light sensor that when light is detected displays to the LCD screen and maybe lights up LEDs. LCD will use a timer interrupt to keep time and will display a status of "Light" or "No Light". 

# Hardware
- STM32-F466RE
- 16x02 LCD
- BH1750 - Light Sensor

# Features
- UART Terminal Communication
- I2C communication between MCU, BH1750 and LCD
- Light Detection
- Clock display

# Pins 
- SCL and SDA connected to both LCD and BH1750
- 3.3V power
- RX/TX configured from ST-Link