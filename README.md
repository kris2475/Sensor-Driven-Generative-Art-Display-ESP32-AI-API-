# Sensor-Driven Generative Art Display (ESP32 + AI API)

## 1. Executive Summary
This project establishes a real-time, closed-loop system that links physical environmental data to cloud-based generative AI. An ESP32 microcontroller reads a dynamic sensor value, which is then pulled by a Python polling client and used to prompt an LLM (e.g., Stability AI) to generate a unique image. The image is transmitted back to the ESP32 and displayed on a TFT LCD.

The system uses a firewall-bypass architecture in which the Python application initiates all network connections to the ESP32. This converts blocked inbound connections into permitted outbound ones, enabling deployment on restricted corporate networks.

## 2. Sensor and Data
**Metric Type:**  
PoC Sensor Used: Internal ESP32 Temperature  
General Applicability: Ambient Light (Lux), Color, Proximity, Humidity, Sound Level, PIR Motion.

The sensor value provides a continuously changing, quantifiable measure of the environment. This data is converted into natural language and incorporated into the initial prompt, enabling the physical environment to directly influence the generated artwork.

## 3. Hardware and Components
| Component        | Description                                              |
|------------------|----------------------------------------------------------|
| Microcontroller  | ESP32 Development Board (e.g., Wemos D1, DevKitC)        |
| Display          | 1.9-inch SPI TFT LCD (ST7789, 170x320 resolution)        |
| Sensor           | ESP32 Internal Temperature Sensor (PoC)                  |

## 4. System Architecture (Firewall Bypass)
The system uses a *pull-and-push*, client-driven model to bypass firewall restrictions that block inbound connections.

**Connection Roles**  
- **ESP32:** Two dedicated servers  
- **Python Script (Laptop):** Client for all connections

| Step | Connection Flow                   | Purpose                                            | Protocol / Port |
|------|-----------------------------------|----------------------------------------------------|------------------|
| 1    | Python → ESP32 (Server 1)         | Pulls current sensor data                          | TCP / 8082       |
| 2    | Python → ESP32 (Server 2)         | Pushes generated raw image data for display        | TCP / 8080       |

By ensuring the laptop initiates all connections, the system operates without inbound firewall exceptions.

## 5. Wiring and Pin Connections
Standard ESP32 VSPI pins are used.

| Display Pin (TFT) | Function             | ESP32 GPIO Pin | Note                        |
|--------------------|----------------------|-----------------|-----------------------------|
| SCK (SCLK)         | SPI Clock            | 18              | Default SCK                 |
| MOSI (SDA)         | Master Out Slave In  | 23              | Default MOSI                |
| MISO               | Master In Slave Out  | -1 (Unused)     | Disabled in software        |
| CS                 | Chip Select          | 15              | Control Pin                 |
| DC                 | Data / Command       | 2               | Control Pin                 |
| RST                | Reset                | 4               | Control Pin                 |
| BL                 | Backlight LED        | 32              | Backlight control           |
| VCC                | Power                | 3.3V            | —                           |
| GND                | Ground               | GND             | —                           |

## 6. Library Setup and Troubleshooting (TFT_eSPI)
### 1. The Problem: Compilation and I2C Errors
- **Compilation errors:** Missing definitions due to conflicting display drivers.
- **Runtime errors:** Black screen caused by repeated I2C NACK errors from attempted communication with a non-existent touch controller.

### 2. The Solution: Custom Configuration File
A dedicated setup file `Setup101_ST7789_170x320.h` was created in `TFT_eSPI/User_Setups`.

#### A. Clean Setup File
A minimal, conflict-free configuration tailored to the 170x320 ST7789 display.

#### B. Key Definitions
| Setting        | Code Snippet                            | Rationale                                    |
|----------------|------------------------------------------|-----------------------------------------------|
| Driver Type    | `#define ST7789_2_DRIVER`                | Correct controller version                    |
| Resolution     | `#define TFT_WIDTH 170`<br>`#define TFT_HEIGHT 320` | Matches actual panel size           |
| I2C/Touch Fix  | `#define NO_TOUCH`                       | Prevents I2C NACK from nonexistent controller |
| MISO Disable   | `#define TFT_MISO -1`                    | Write-only display                            |
| Pin Assignments| `#define TFT_CS 15`<br>`#define TFT_DC 2`<br>`#define TFT_RST 4`<br>`#define TFT_BL 32` | ESP32 wiring |




## 7. Code Implementation Details

### A. ESP32 Sketch (DIYMORE_LCD_Gen_Ai_WIFI.ino)

The ESP32 operates as two independent TCP servers:

| Server               | Port | Function                                   |
|----------------------|------|---------------------------------------------|
| sensorRequestServer  | 8082 | Reads temperature and returns raw string    |
| imageServer          | 8080 | Receives and displays raw RGB565 image data |

### B. Python Script (firewall_bypass_loop.py)

The Python client orchestrates the following workflow:

- **Polling Loop:** Runs every 30 seconds  
- **Pull Data (8082):** Retrieves sensor value from ESP32  
- **Prompt Generation:** Converts the sensor reading into a natural-language prompt  
- **AI Generation:** Invokes Stability AI to generate a JPEG image  
- **Conversion:** Converts the JPEG image into RGB565 raw format  
- **Push Image (8080):** Streams the RGB565 data to the ESP32 for immediate display

#### C. Activating the Custom Setup
```cpp
// In User_Setup_Select.h
// ... ensure all other includes are commented out
#include <User_Setups/Setup101_ST7789_170x320.h>

