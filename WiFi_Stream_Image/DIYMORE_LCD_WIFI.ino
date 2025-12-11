#include <WiFi.h>
#include <HTTPClient.h>
#include <TFT_eSPI.h> 
#include <ArduinoJson.h> // REQUIRED for JSON parsing

// --------------------------------------------------------
// --- TFT_eSPI & Image Configuration (From User Input) ---
// --------------------------------------------------------
TFT_eSPI tft = TFT_eSPI();

#define IMAGE_WIDTH      320  
#define IMAGE_HEIGHT     170  
#define TFT_BL           32
#define TFT_BACKLIGHT_ON HIGH

// ... (other TFT definitions remain unchanged) ...

#define STABLE_BAUD_RATE 115200 
const char* START_COMMAND = "START_IMAGE_TRANSFER";

// --------------------------------------------------------
// --- WIFI Configuration (Confirmed Working) ---
// --------------------------------------------------------
const char* ssid = ""; 
const char* password = ""; 

// --------------------------------------------------------
// --- HTTP Test Configuration (NEW RELIABLE URL) ---
// --------------------------------------------------------
// Fetches a JSON object containing a sample blog post title
const char* test_url = "http://jsonplaceholder.typicode.com/posts/1"; 
const size_t JSON_DOC_SIZE = 1024; // Allocate enough memory for the JSON document

// --------------------------------------------------------
// --- DISPLAY FUNCTION ---
// --------------------------------------------------------
void displayStatus(String title, String message, uint16_t color = TFT_WHITE) {
  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(color, TFT_BLACK);
  tft.drawString(title, 10, 10, 2); 
  
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  // Display the first part of the message on the second line
  tft.drawString(message.substring(0, 30), 10, 30, 2); 

  // If the message is long, display the rest on the third line
  if (message.length() > 30) {
    tft.drawString(message.substring(30, 60), 10, 50, 2); 
  }
}

// --------------------------------------------------------
// --- WIFI CONNECTION FUNCTION (Unchanged) ---
// --------------------------------------------------------
void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  displayStatus("Connecting...", ssid);
  
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
  
  displayStatus("Connected!", "IP: " + WiFi.localIP().toString(), TFT_GREEN);
}

// --------------------------------------------------------
// --- HTTP READ & JSON PARSE FUNCTION ---
// --------------------------------------------------------
void readAndDisplayWebData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    http.begin(test_url); 
    displayStatus("HTTP JSON Test:", "Requesting post title...", TFT_CYAN);
    Serial.println("\n[HTTP] Starting request...");
    
    int httpCode = http.GET();
    
    if (httpCode > 0) {
      Serial.printf("[HTTP] GET Code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        
        // --- JSON Parsing Start ---
        StaticJsonDocument<JSON_DOC_SIZE> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
          Serial.print(F("JSON Deserialization failed: "));
          Serial.println(error.f_str());
          displayStatus("JSON Error!", error.f_str(), TFT_RED);
        } else {
          // Extract the title field from the JSON object
          const char* title = doc["title"]; 
          String display_title = "Title: ";
          display_title += title;
          
          Serial.println("\n--- RECEIVED TITLE ---");
          Serial.println(display_title);
          
          // Display the extracted title on the TFT Screen
          displayStatus("JSON SUCCESS!", display_title, TFT_GREEN); 
        }
        // --- JSON Parsing End ---

      } else {
        String error_msg = "HTTP Fail: Code " + String(httpCode);
        displayStatus("ERROR!", error_msg, TFT_RED);
      }
    } else {
      String error_msg = "GET Error: " + http.errorToString(httpCode);
      displayStatus("ERROR!", error_msg, TFT_RED);
    }

    http.end(); 
  } else {
    connectToWiFi();
  }
}


// --------------------------------------------------------
// --- IMAGE DRAWING FUNCTION (Unchanged) ---
// --------------------------------------------------------
void drawImageFromSerial() {
  // ... (drawImageFromSerial logic remains the same) ...
}

// --------------------------------------------------------
// --- SETUP and LOOP (Unchanged) ---
// --------------------------------------------------------
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
  
  connectToWiFi();
  readAndDisplayWebData(); // Run the HTTP test
}

void loop() {
  // Check for the serial command to start image transfer
  // ... (loop logic remains the same) ...
}
