# I2C Light Sensor with LCD and USART Serial Terminal
Light sensor that returns a value via an I2C interface that displays a value "Light" or "Dark" on an LCD via the same I2C bus, depending on the light value reading. UART terminal display also debugs and prints the when the sensor is initialized an what the light value is in a decimal value.

# Hardware
- STM32-F466RE
- 16x02 LCD
- BH1750 - Light Sensor

# Features
- USART Terminal Communication
- I2C communication between MCU, BH1750 and LCD
- Light Detection
- Clock display (decided against)

# Pins 
- SCL and SDA connected to both LCD and BH1750
- 3.3V power
- RX/TX configured from ST-Link

![Uncovered_Screen](/images/Uncovered_Sensor.jpeg)
![Covered_Screen](/images/Covered_Sensor.jpeg)
![Terminal_Output](/images/Terminal_Output.png)
