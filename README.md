# I2C Multi-Device Light Detection System

## Project Overview
Developed an embedded system that integrates multiple I2C devices on a shared bus to create a light-detection and display system. The project demonstrates I2C bus management, sensor integration, and multi-interface output capabilities.

## Technical Implementation
- Implemented I2C driver for BH1750 light sensor with 16-bit resolution
- Managed shared I2C bus communication between BH1750 sensor and 16x2 LCD display
- Developed UART debugging interface with real-time sensor value reporting
- Implemented threshold-based light detection algorithm
- Designed robust error handling for I2C communication

## Hardware Components
- STM32F446RE 
- Sensors: BH1750 Light Sensor
- Display: 16x2 I2C LCD Module
- Communication: UART via ST-Link

## Key Features
- Real-time light intensity monitoring
- Dynamic LCD status updates
- Concurrent I2C device management
- UART debug output with formatted data
- Error detection and handling

## Pin Configuration
- I2C: SCL/SDA shared bus configuration
- Power: 3.3V rail management
- Debug: UART TX/RX via ST-Link

## Development Tools
- IDE: STM32CubeIDE
- Debug: ST-Link
- Version Control: Git

## Results
![Uncovered_Screen](/images/Uncovered_Sensor.jpeg)
![Covered_Screen](/images/Covered_Sensor.jpeg)
![Terminal_Output](/images/Terminal_Output.png)
