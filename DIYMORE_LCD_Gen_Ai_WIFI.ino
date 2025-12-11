/**
 * @file DIYMORE_LCD_Gen_Ai_WIFI.ino
 * @brief ESP32 Sketch for Closed-Loop AI Image Display System (FIREWALL BYPASS)
 * * The ESP32 acts ONLY as a server for two functions:
 * * 1. Sensor Polling Server (Port 8082): Listens for Python to connect and request sensor data.
 * * 2. Image Reception Server (Port 8080): Listens for Python to connect and stream the image.
 * * The Python script (laptop) now initiates all connections, bypassing corporate firewall restrictions.
 */

#include <WiFi.h>
#include <TFT_eSPI.h> 
#include "driver/temp_sensor.h" 

// --------------------------------------------------------
// --- WIFI & NETWORK CONFIGURATION ---
// --------------------------------------------------------
const char* ssid = "SKYYRMR7"; 
const char* password = "K2xWvDFZkuCh"; 

// Ports for dual server operation on the ESP32
const int image_transfer_port = 8080;   // ESP32 SERVER: receives image
const int sensor_request_port = 8082; // ESP32 SERVER: sends sensor data on request

// No need for python_server_ip here, as the ESP32 never initiates a connection.

// Create two server instances
WiFiServer imageServer(image_transfer_port); 
WiFiServer sensorRequestServer(sensor_request_port); 


// --------------------------------------------------------
// --- TFT & IMAGE CONFIGURATION (Unchanged) ---
// --------------------------------------------------------
TFT_eSPI tft = TFT_eSPI();

#define IMAGE_WIDTH      320  
#define IMAGE_HEIGHT     170  
#define TFT_BL           32
#define TFT_BACKLIGHT_ON HIGH

const size_t EXPECTED_IMAGE_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * 2; 
const size_t LINE_BYTE_COUNT = IMAGE_WIDTH * 2; 
uint8_t lineBuf[LINE_BYTE_COUNT]; 


// --------------------------------------------------------
// --- SENSOR READING FUNCTION (Unchanged) ---
// --------------------------------------------------------
String getSensorReading() {
  float tempC = temperatureRead(); // Returns temperature in Celsius
  return "temp=" + String(tempC, 2) + "C"; 
}

// --------------------------------------------------------
// --- WIFI CONNECTION FUNCTION (Unchanged) ---
// --------------------------------------------------------
void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  tft.fillScreen(TFT_BLACK);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected successfully!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); 
  
  tft.fillScreen(TFT_DARKGREEN);
  delay(500);
  tft.fillScreen(TFT_BLACK);
}

// --------------------------------------------------------
// --- IMAGE DRAWING FUNCTION (Unchanged) ---
// --------------------------------------------------------
void drawImageFromClient(WiFiClient client) {
  Serial.println("\n[SERVER 8080] Receiving image data from Python...");
  tft.setAddrWindow(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);

  size_t bytesReadTotal = 0;
  
  for (int y = 0; y < IMAGE_HEIGHT; y++) {
    size_t bytesRead = client.readBytes((char*)lineBuf, LINE_BYTE_COUNT);
    
    if (bytesRead != LINE_BYTE_COUNT) {
      Serial.printf("FATAL ERROR: Incomplete read at row %d. Expected %u bytes, got %u. Aborting.\n", y, LINE_BYTE_COUNT, bytesRead);
      tft.fillScreen(TFT_RED); 
      return; 
    }
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

// --------------------------------------------------------
// --- SENSOR REQUEST HANDLER (NEW SERVER 8082) ---
// --------------------------------------------------------
void handleSensorRequest(WiFiClient client) {
    // Read any incoming request (optional, but good practice)
    String request = client.readStringUntil('\n'); 
    request.trim();

    Serial.printf("\n[SERVER 8082] Received request: %s. Sending data...\n", request.c_str());

    String data = getSensorReading();
    client.println(data); 
    
    Serial.print("[SERVER 8082] Sent data: ");
    Serial.println(data);
    
    // Disconnect after sending
    client.stop(); 
    Serial.println("[SERVER 8082] Disconnected from Python client.");
}


// --------------------------------------------------------
// --- SETUP and LOOP ---
// --------------------------------------------------------
void setup() {
  Serial.begin(115200); 

  // 1. DISPLAY INITIALIZATION
  tft.init();
  tft.setRotation(3); 
  tft.invertDisplay(true); 
  tft.fillScreen(TFT_BLACK); 
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);

  // 2. WIFI CONNECTION
  connectToWiFi();

  // 3. START BOTH SERVERS
  imageServer.begin();
  sensorRequestServer.begin();
  Serial.print("Image Reception Server started on port: ");
  Serial.println(image_transfer_port);
  Serial.print("Sensor Request Server started on port: ");
  Serial.println(sensor_request_port);
  Serial.println("Loop initialized. Awaiting Python polling request...");
}

void loop() {
  // 1. Check for incoming IMAGE streaming connection (Port 8080)
  WiFiClient imageClient = imageServer.available();
  if (imageClient) {
    drawImageFromClient(imageClient);
  }
  
  // 2. Check for incoming SENSOR DATA REQUEST connection (Port 8082)
  WiFiClient sensorClient = sensorRequestServer.available();
  if (sensorClient) {
    handleSensorRequest(sensorClient);
  }
  
  delay(100); 
}
