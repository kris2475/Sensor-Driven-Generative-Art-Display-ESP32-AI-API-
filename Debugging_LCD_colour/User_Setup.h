//                            USER DEFINED SETTINGS
//
//   This is the FINAL, OFFSET-CORRECTED configuration for the DIYMORE_ESP32_ILI9341 2.4-inch display.
//   It uses the alternate driver (ILI9341_2) and the Y offset (TFT_YSTART 80) to fix the 1/4 white noise issue.
//
#define USER_SETUP_INFO "DIYMORE_ESP32_ILI9341_FINAL_CONFIGURED"

// ##################################################################################
// Section 1. Driver and Display Options
// ##################################################################################

// *** CRITICAL FIX: Alternate driver to fix memory window issues. ***
#define ILI9341_2_DRIVER     

// *** CRITICAL COLOR FIX: Restore TFT_BGR. ***
// This tells the TFT_eSPI driver to swap R/B channels to fix psychedelic colors.
#define TFT_BGR 

// Define the pixel width and height in portrait orientation (240x320)
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// ---------------------------------------------------------------------------------
// CRITICAL FIX FOR 1/4 SCREEN CORRUPTION: Vertical Offset
// This shifts the display window to align with the start of the 240x320 panel area.
// ---------------------------------------------------------------------------------
#define TFT_YSTART 80 

// ##################################################################################
// Section 2. Pin Definitions (For ESP32 Dev Board - Confirmed Working)
// ##################################################################################

#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15  // Chip select control pin
#define TFT_DC    2  // Data Command control pin
#define TFT_RST   4  // Reset pin

#define TFT_BL   32            // LED back-light control pin
#define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light

// ##################################################################################
// Section 3. Fonts and Other Options
// ##################################################################################

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

#define SPI_FREQUENCY  27000000 
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000
