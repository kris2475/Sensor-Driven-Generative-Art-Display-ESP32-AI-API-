#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// --- IMAGE & GEOMETRY SPECIFICATIONS ---
#define IMAGE_WIDTH  320  
#define IMAGE_HEIGHT 170  
#define TFT_BL       32
#define TFT_BACKLIGHT_ON HIGH

// Total size in bytes (320 * 170 * 2 bytes/pixel)
const size_t EXPECTED_IMAGE_SIZE = 320 * 170 * 2; // 108800 bytes
const size_t LINE_BYTE_COUNT = IMAGE_WIDTH * 2; // 640 bytes per line

// Buffer for one line of image data (640 bytes)
uint8_t lineBuf[LINE_BYTE_COUNT]; 

#define STABLE_BAUD_RATE 115200 
const char* START_COMMAND = "START_IMAGE_TRANSFER";

// *** REMOVED: The manual swap_bytes function ***

void drawImageFromSerial() {
  Serial.println("STARTING DRAW: Data stream detected.");
  tft.fillScreen(TFT_BLACK);
  tft.setAddrWindow(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT); 

  size_t bytesReadTotal = 0;
  
  // Read and draw line by line (170 rows exactly)
  for (int y = 0; y < IMAGE_HEIGHT; y++) {
    
    // Read a full line (640 bytes) from the Serial buffer
    size_t bytesRead = Serial.readBytes((char*)lineBuf, LINE_BYTE_COUNT);
    
    if (bytesRead != LINE_BYTE_COUNT) {
      Serial.printf("FATAL ERROR: Incomplete read at row %d. Expected %u bytes, got %u. Aborting.\n", y, LINE_BYTE_COUNT, bytesRead);
      tft.fillScreen(TFT_RED); 
      return; 
    }
    
    // CRITICAL FIX: Push the colors directly without manual byte swap.
    // The 'true' flag in pushColors tells the library to handle any necessary byte swapping.
    tft.pushColors((uint16_t*)lineBuf, IMAGE_WIDTH, true); 
    bytesReadTotal += bytesRead;
  }
  
  if (bytesReadTotal == EXPECTED_IMAGE_SIZE) {
    Serial.println("Image drawn successfully!");
  } else {
    Serial.printf("FATAL ERROR: Final size mismatch. Expected %u, Read %u.\n", EXPECTED_IMAGE_SIZE, bytesReadTotal);
    tft.fillScreen(TFT_MAGENTA);
  }
}

void setup() {
  Serial.begin(STABLE_BAUD_RATE); 
  Serial.setTimeout(1000); 

  // --- DISPLAY INITIALIZATION ---
  tft.init();
  tft.setRotation(3); 
  tft.invertDisplay(true); 
  tft.fillScreen(TFT_BLACK);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);

  Serial.println("System Initialized. Awaiting Command...");
  tft.drawString("Awaiting Command", 10, 10, 2);
  tft.drawString("Baud Rate: 115200", 10, 30, 2);
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n'); 
    command.trim(); 

    if (command.equals(START_COMMAND)) {
      drawImageFromSerial();
      //tft.drawString("Draw Complete!", 10, 50, 2);
    } else {
      Serial.printf("Received unknown command: %s\n", command.c_str());
    }
  }
}