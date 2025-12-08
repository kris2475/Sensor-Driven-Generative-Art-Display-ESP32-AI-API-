# 💡 Ambient Light-to-Image Generation System

## 🌟 Executive Summary

This project demonstrates a real-time, atmospheric display system that bridges physical sensor data with cloud-based Large Language Models (LLMs) and generative AI. An **ESP32** microcontroller, connected to a triad of environmental sensors (e.g., colour/lux/PIR), detects the ambient mood of a room. This data is **transmitted** via **WiFi** using **JSON** to a cloud service where an LLM interprets the mood and **cues** an **AI-generated image** (e.g., Stable Diffusion or DALL-E). The final image is then pushed back and displayed on the **ST7789 LCD** for a dynamic, ever-changing visualisation of the atmosphere.

## 💻 Hardware and Components

| Component | Description | Amazon Link |
| :--- | :--- | :--- |
| **Microcontroller** | ESP32 Development Board (e.g., Wemos D1, ESP32 DevKitC) | N/A |
| **Display** | 1.9-inch SPI TFT LCD (ST7789 170x320 resolution) | [Product Link](https://www.amazon.co.uk/dp/B0DK399Y9Q) |
| **Sensors** | Triad Sensor Array (e.g., Ambient Light Sensor, Colour Sensor, PIR Sensor) | N/A |

---

## 🔌 Wiring and Pin Connections

This setup uses the standard SPI pins on the ESP32 (VSPI) and the pin assignments confirmed during the successful troubleshooting phase.

| Display Pin (TFT) | Function | ESP32 GPIO Pin | Note |
| :--- | :--- | :--- | :--- |
| **SCK (SCLK)** | SPI Clock | GPIO 18 | Default SCK |
| **MOSI (SDA)** | Master Out Slave In | GPIO 23 | Default MOSI |
| **MISO** | Master In Slave Out | **-1 (Unused)** | Disabled in software setup |
| **CS** | Chip Select | **GPIO 15** | Control Pin |
| **DC** | Data / Command | **GPIO 2** | Control Pin |
| **RST** | Reset | **GPIO 4** | Control Pin |
| **BL** | Backlight LED | **GPIO 32** | For backlight control |
| **VCC** | Power | 3.3V | Connect to 3.3V power rail |
| **GND** | Ground | GND | Connect to Ground |

---

## ⚙️ Library Setup and Troubleshooting (TFT_eSPI)

The most significant hurdle was configuring the **TFT\_eSPI library** to work with the 1.9-inch 170x320 ST7789 display without conflicts.

### 1. The Problem: Compilation and I2C Errors

We encountered two main issues:
1.  **Compilation Errors:** The compiler failed due to missing definitions (`'TFT_SWRST' was not declared in this scope`, etc.) because conflicting display drivers were selected.
2.  **Runtime Errors:** After fixing compilation, the screen remained black due to repeated **I2C NACK errors** (`I2C hardware NACK detected`) caused by the library trying to communicate with a non-existent or conflicting touch controller chip often present on these generic modules.

### 2. The Solution: Custom Configuration File

We resolved both issues by creating a clean, dedicated setup file and applying specific overrides.

#### **A. Clean Setup File**

A new configuration file, **`Setup101_ST7789_170x320.h`**, was created in the `TFT_eSPI/User_Setups` directory.

#### **B. Key Definitions for Correct Operation**

The following critical lines were set in the custom setup file to ensure both compilation and runtime stability:

| Setting | Code | Rationale |
| :--- | :--- | :--- |
| **Driver Type** | `#define ST7789_2_DRIVER` | Selects the correct display controller version. |
| **Resolution** | `#define TFT_WIDTH 170` `#define TFT_HEIGHT 320` | Sets the correct pixel dimensions for the 1.9-inch panel. |
| **I2C/Touch Fix** | `#define NO_TOUCH` | **Crucial:** Disables all touch and I2C initialization, stopping the NACK errors that caused the display to hang. |
| **MISO Disable** | `#define TFT_MISO -1` | The display is write-only SPI, disabling MISO prevents potential hardware conflicts. |
| **Pin Assignments** | `#define TFT_CS 15`, `#define TFT_DC 2`, `#define TFT_RST 4`, `#define TFT_BL 32` | Defines the specific GPIO pins for the ESP32. |

#### **C. Activating the Custom Setup**

The main library file, **`TFT_eSPI/User_Setup_Select.h`**, was modified to only include the new custom setup:

```cpp
// In User_Setup_Select.h
// ... ensure all other #include <User_Setup... lines are commented out
#include <User_Setups/Setup101_ST7789_170x320.h>
