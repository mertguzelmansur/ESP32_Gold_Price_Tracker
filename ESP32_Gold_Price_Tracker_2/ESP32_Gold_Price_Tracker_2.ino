/*
Created by Mert Güzelmansur in 2025 Jan.
ESP32 AHT20 + BMP280 + OLED SCREEN I2C
*/

#include <Adafruit_SSD1306.h> 
#include <WiFi.h> 
#include <Wire.h> 
#include <HTTPClient.h> 
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h> 
#include <time.h>
#include <LittleFS.h>
#include <FS.h>
#include <AsyncJson.h>
#include <Adafruit_AHTX0.h>    // Add AHT20 library
#include <Adafruit_BMP280.h>   // Add BMP280 library
#include <esp_task_wdt.h>  // Add this at the top with other includes

const unsigned char logo [] PROGMEM =                                  // ' Logo', 128x64px
{
// 'financial-profit', 128x64px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x83, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x03, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x07, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x1c, 0x77, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x38, 0xfe, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe3, 0x80, 0x71, 0xde, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc1, 0xc0, 0xe3, 0x8c, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0xe1, 0xc7, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x1c, 0x73, 0x8e, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x3e, 0x3f, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x77, 0x1e, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xe3, 0x80, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x71, 0xc1, 0xc0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe3, 0x80, 0xe1, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc7, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x8e, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x07, 0x1c, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x0e, 0x38, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x03, 0xce, 0x1c, 0x70, 0x00, 0x01, 0xfb, 0xf0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x38, 0xe0, 0x00, 0x03, 0x80, 0x78, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0e, 0x03, 0xf1, 0xc0, 0x00, 0x07, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1c, 0x71, 0xe3, 0x80, 0x00, 0x0e, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x38, 0xf8, 0xc7, 0x00, 0x00, 0x0c, 0x1e, 0x06, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x71, 0xdc, 0x0e, 0x00, 0x00, 0x1c, 0x3f, 0x06, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xe3, 0x8e, 0x1c, 0x00, 0x00, 0x18, 0x33, 0x07, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xc7, 0x07, 0xf8, 0x0f, 0xf0, 0x18, 0x38, 0x03, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xce, 0x03, 0xf0, 0x3f, 0xfc, 0x18, 0x1e, 0x03, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x78, 0x1e, 0x18, 0x0f, 0x03, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0xe0, 0x07, 0x18, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc1, 0x83, 0x98, 0x13, 0x07, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x81, 0x81, 0x9c, 0x1f, 0x06, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0xe0, 0xcc, 0x1e, 0x06, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0xe0, 0xce, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x03, 0x06, 0x00, 0xc7, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x07, 0x07, 0x00, 0xe3, 0x80, 0x78, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x86, 0x03, 0xc0, 0x63, 0xfb, 0xf0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1e, 0x01, 0xc7, 0x01, 0xe0, 0xe7, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0xe3, 0x00, 0x60, 0xcf, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x73, 0x06, 0x60, 0xcc, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x60, 0x78, 0x33, 0x07, 0xe0, 0xcc, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x60, 0xf8, 0x39, 0x81, 0x81, 0x8f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xe0, 0xc8, 0x19, 0xc1, 0x83, 0x8f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x18, 0xe0, 0x07, 0x0c, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xc0, 0xf0, 0x18, 0x78, 0x1e, 0x0c, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xc0, 0x78, 0x18, 0xff, 0xff, 0x0f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xc0, 0x1c, 0x19, 0xff, 0xff, 0x8f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xc0, 0x8c, 0x19, 0x80, 0x01, 0x8c, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x60, 0xfc, 0x3b, 0x80, 0x01, 0xcc, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x60, 0x78, 0x31, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x71, 0xff, 0xff, 0x8f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0xe1, 0xc0, 0x03, 0x8e, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1e, 0x01, 0xc3, 0x80, 0x01, 0xcc, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0f, 0x8f, 0xc1, 0x80, 0x01, 0x8e, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xe1, 0xff, 0xff, 0x8f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xf1, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x33, 0x80, 0x01, 0xcc, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x31, 0x80, 0x01, 0x8c, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xf1, 0xff, 0xff, 0x8f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xe0, 0xff, 0xff, 0x07, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00
};

#define ARDUINO_RUNNING_CORE 1
#define WDT_TIMEOUT_SECONDS 30
#define ASYNC_TCP_PRIORITY 1
#define STACK_SIZE 16384  // Increased stack size

// Create task handles
TaskHandle_t asyncWebServerHandle = NULL;
TaskHandle_t mainTaskHandle = NULL;

// Create a semaphore to protect server operations
SemaphoreHandle_t serverSemaphore = NULL;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
#define BORDER_THICKNESS 1
#define WEATHER_UPDATE_INTERVAL 300000  // 5 minutes in milliseconds
#define SENSOR_UPDATE_INTERVAL 2000     // 2 seconds in milliseconds
#define TIME_UPDATE_INTERVAL 1000     // 1 second in milliseconds

// Display mode constants
#define DISPLAY_MODE_STOCKS_AND_WEATHER "stocks_and_weather"
#define DISPLAY_MODE_WEATHER_ONLY "weather_only"
#define DISPLAY_MODE_STOCKS_ONLY "stocks_only"
#define DISPLAY_MODE_TIME_ONLY "time_only"

// Create sensor objects with proper initialization
Adafruit_AHTX0 aht = {};
Adafruit_BMP280 bmp = {}; // I2C

// Add display mode variable
String displayMode = DISPLAY_MODE_STOCKS_AND_WEATHER;  // Default mode

// Add sensor data variables
float ahtTemperature = 0;
float ahtHumidity = 0;
float bmpTemperature = 0;
float bmpPressure = 0;
float bmpAltitude = 0;

// Local air pressure for accurate altitude calculation (update this for your location)
const float local_air_pressure = 1013.25f;  // Standard sea level pressure in hPa

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);

// Add weather location variables
String weatherLat = "40.2154";  // Default latitude
String weatherLon = "28.9870";  // Default longitude
String weatherLocationName = "Bursa/Nilufer";  // Default location name

// Add these definitions for time zone adjustment
#define TIMEZONE_OFFSET_HOURS +3  // Example: UTC+3 (adjust to your specific time zone)
#define TIMEZONE_OFFSET_MINUTES 0 // Additional minutes offset if needed

// Original definitions and global variables
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const char* ssid = "MGM";
const char* password = "Mert.1256";

// New configuration variables
const int MAX_STOCKS = 10;
String stockApiUrls[MAX_STOCKS];
String stockDisplayNames[MAX_STOCKS];
int currentStockIndex = 0;
int displayInterval = 10000;  // Default 10 seconds

// Add your OpenWeatherMap API key
const char* WEATHER_API_KEY = "e92a8c3f5d6e12f740b9be7b292e89be";  // Replace with your actual API key

// Add these global variables with other declarations
float temperature = 0;
int humidity = 0;
String weatherDescription = "";

// Add time variables
struct tm timeinfo;
time_t now;
char timeStringBuff[50];
bool timeInitialized = false;

String formatTime(const String& isoTime) { 
  struct tm tm; 
  char buf[20]; 
   
  // Parse the ISO time string
  sscanf(isoTime.c_str(), "%d-%d-%dT%d:%d:%d",  
         &tm.tm_year, &tm.tm_mon, &tm.tm_mday,  
         &tm.tm_hour, &tm.tm_min, &tm.tm_sec); 
   
  // Adjust years and months to standard tm struct format
  tm.tm_year -= 1900; 
  tm.tm_mon -= 1; 
   
  // Convert to time_t to allow time zone manipulation
  time_t rawtime = mktime(&tm);
  
  // Apply time zone offset (in seconds)
  rawtime += (TIMEZONE_OFFSET_HOURS * 3600) + (TIMEZONE_OFFSET_MINUTES * 60);
  
  // Convert back to tm struct with adjusted time
  struct tm* adjusted_tm = localtime(&rawtime);
   
  // Format time to HH:MM
  strftime(buf, sizeof(buf), "%H:%M", adjusted_tm); 
  return String(buf); 
} 

float fetchPrice(const char* url) {
  HTTPClient https;
  https.begin(url);
  
  // Add required headers
  https.addHeader("Content-Type", "application/json");
  https.addHeader("User-Agent", "ESP32/1.0");
  
  // Set timeout to prevent hanging
  https.setTimeout(2000); // Reduced to 2 second timeout
  
  int httpCode = https.GET();
  float result = 0;
  
  if (httpCode == HTTP_CODE_OK) {
    String payload = https.getString();
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (!error && doc.containsKey("price")) {
      result = doc["price"].as<float>();
    }
  }
  
  https.end();
  esp_task_wdt_reset();  // Reset watchdog after HTTP request
  delay(20); // Minimal delay
  return result;
}

String fetchTime(const char* url) {
  HTTPClient https;
  https.begin(url);
  
  // Add required headers
  https.addHeader("Content-Type", "application/json");
  https.addHeader("User-Agent", "ESP32/1.0");
  
  // Set timeout to prevent hanging
  https.setTimeout(2000); // Reduced to 2 second timeout
  
  int httpCode = https.GET();
  String result = "";
  
  if (httpCode == HTTP_CODE_OK) {
    String payload = https.getString();
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (!error && doc.containsKey("updatedAt")) {
      String timeStr = doc["updatedAt"].as<String>();
      result = formatTime(timeStr);
    }
  }
  
  https.end();
  esp_task_wdt_reset();  // Reset watchdog after HTTP request
  delay(20); // Minimal delay
  return result;
}

// Function to draw a modern border
void drawBorder() {
  // Draw outer border
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  // Draw inner border
  display.drawRect(BORDER_THICKNESS, BORDER_THICKNESS, 
                  SCREEN_WIDTH - (2 * BORDER_THICKNESS), 
                  SCREEN_HEIGHT - (2 * BORDER_THICKNESS), 
                  SSD1306_WHITE);
}

// Function to draw a horizontal line separator
void drawSeparator(int y) {
  display.drawLine(BORDER_THICKNESS * 2, y, 
                  SCREEN_WIDTH - (BORDER_THICKNESS * 2), y, 
                  SSD1306_WHITE);
}

// Updated printCenter function with y-offset compensation for borders
void printCenter(const String buf, int x, int y)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(buf, x, y, &x1, &y1, &w, &h);
  display.setCursor((x - w / 2) + (SCREEN_WIDTH / 2), y + BORDER_THICKNESS);
  display.print(buf);
}

// Updated displayStockData function for better scaling
void displayStockData(const String& displayName, float stockPrice, const String& stockTime, float lastPrice) {
  display.clearDisplay();
  drawBorder();
  
  // Display name at top - slightly larger
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  printCenter(displayName, 0, 3);
  
  // Draw separator under name
  drawSeparator(14);
  
  // Price with larger text
  display.setTextSize(2);
  String priceStr = "$" + String(stockPrice, 2);
  printCenter(priceStr, 0, 25);
  
  // Draw separator above time
  drawSeparator(50);
  
  // Time at bottom
  display.setTextSize(1);
  printCenter(stockTime, 0, 54);
  
  display.display();
}

// Function to fetch and return the configuration as JSON
void handleConfig(AsyncWebServerRequest *request) {
    if (xSemaphoreTake(serverSemaphore, portMAX_DELAY)) {
        if (!LittleFS.begin()) {
            xSemaphoreGive(serverSemaphore);
            request->send(500, "application/json", "{\"error\":\"Failed to mount filesystem\"}");
            return;
        }

        // Create default config if it doesn't exist
        if (!LittleFS.exists("/config.json")) {
            StaticJsonDocument<1024> defaultConfig;
            defaultConfig["displayInterval"] = 10;
            defaultConfig["displayMode"] = displayMode;
            defaultConfig["weatherLat"] = weatherLat;
            defaultConfig["weatherLon"] = weatherLon;
            defaultConfig["weatherLocationName"] = weatherLocationName;
            JsonArray stocks = defaultConfig.createNestedArray("stocks");

            File configFile = LittleFS.open("/config.json", "w");
            if (configFile) {
                serializeJson(defaultConfig, configFile);
                configFile.close();
            }
        }

        // Read and send the config file
        File configFile = LittleFS.open("/config.json", "r");
        if (configFile) {
            // Read file into a buffer
            String configData = configFile.readString();
            configFile.close();
            xSemaphoreGive(serverSemaphore);
            
            // Send the configuration
            request->send(200, "application/json", configData);
        } else {
            xSemaphoreGive(serverSemaphore);
            request->send(500, "application/json", "{\"error\":\"Failed to open config file\"}");
        }
    } else {
        request->send(503, "text/plain", "Server busy");
    }
}

// Update loadStockConfiguration to create default config if needed
void loadStockConfiguration() {
    if (!LittleFS.begin()) {
        Serial.println("Failed to mount file system");
        return;
    }

    // Create default config if it doesn't exist
    if (!LittleFS.exists("/config.json")) {
        Serial.println("Creating default config...");
        StaticJsonDocument<1024> defaultConfig;
        defaultConfig["displayInterval"] = 10;
        defaultConfig["displayMode"] = displayMode;
        defaultConfig["weatherLat"] = weatherLat;
        defaultConfig["weatherLon"] = weatherLon;
        defaultConfig["weatherLocationName"] = weatherLocationName;
        JsonArray stocks = defaultConfig.createNestedArray("stocks");

        File configFile = LittleFS.open("/config.json", "w");
        if (configFile) {
            serializeJson(defaultConfig, configFile);
            configFile.close();
            Serial.println("Default config created");
        } else {
            Serial.println("Failed to create config file");
            return;
        }
    }

    // Read configuration
    File configFile = LittleFS.open("/config.json", "r");
    if (configFile) {
        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, configFile);
        configFile.close();

        if (!error) {
            // Update display interval
            displayInterval = doc["displayInterval"].as<int>() * 1000; // Convert to milliseconds

            // Update display mode if present
            if (doc.containsKey("displayMode")) {
                displayMode = doc["displayMode"].as<String>();
            }

            // Update weather location if present
            if (doc.containsKey("weatherLat")) {
                weatherLat = doc["weatherLat"].as<String>();
            }
            if (doc.containsKey("weatherLon")) {
                weatherLon = doc["weatherLon"].as<String>();
            }
            if (doc.containsKey("weatherLocationName")) {
                weatherLocationName = doc["weatherLocationName"].as<String>();
            }

            // Update stocks
            JsonArray stocks = doc["stocks"].as<JsonArray>();
            currentStockIndex = 0;

            for (JsonVariant stock : stocks) {
                if (currentStockIndex < MAX_STOCKS) {
                    String apiName = stock["apiName"].as<String>();
                    String displayName = stock["displayName"].as<String>();
                    
                    stockApiUrls[currentStockIndex] = apiName;
                    stockDisplayNames[currentStockIndex] = displayName;
                    
                    currentStockIndex++;
                }
            }

            // Clear remaining slots
            for (int i = currentStockIndex; i < MAX_STOCKS; i++) {
                stockApiUrls[i] = "";
                stockDisplayNames[i] = "";
            }

            Serial.println("Configuration loaded successfully");
        } else {
            Serial.println("Failed to parse config file");
        }
    } else {
        Serial.println("Failed to open config file");
    }
}

// Function to clear all stocks and reset the configuration
void clearAllStocks() {
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS");
    return;
  }

  // Create a new configuration with default values
  StaticJsonDocument<1024> doc;
  doc["displayInterval"] = 10;  // Default interval (10 seconds)
  JsonArray stocksArray = doc.createNestedArray("stocks");  // Empty stocks array

  File configFile = LittleFS.open("/config.json", "w");
  if (configFile) {
    serializeJson(doc, configFile);
    configFile.close();
    Serial.println("All stocks cleared and configuration reset.");
  } else {
    Serial.println("Failed to write updated config file");
  }
}


// Function to handle the 'Add Stock' operation and ensure config file exists
void addStockEntry(String apiName, String displayName) {
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS");
    return;
  }

  StaticJsonDocument<1024> doc;
  
  // Read existing config if it exists
  if (LittleFS.exists("/config.json")) {
    File configFile = LittleFS.open("/config.json", "r");
    DeserializationError error = deserializeJson(doc, configFile);
    configFile.close();
    
    if (error) {
      Serial.println("Failed to parse existing config");
      return;
    }
  } else {
    // Create new config with defaults
    doc["displayInterval"] = 10;
    doc.createNestedArray("stocks");
  }

  // Get the stocks array
  JsonArray stocks = doc["stocks"].as<JsonArray>();
  
  // Add the new stock
  JsonObject newStock = stocks.createNestedObject();
  newStock["apiName"] = apiName;
  newStock["displayName"] = displayName;

  // Save the updated config
  File configFile = LittleFS.open("/config.json", "w");
  if (configFile) {
    serializeJson(doc, configFile);
    configFile.close();
    Serial.println("Stock added successfully!");
  } else {
    Serial.println("Failed to save config file");
  }
}

void removeStockEntry(String apiName) {
  ensureConfigFileExists();

  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS");
    return;
  }

  File configFile = LittleFS.open("/config.json", "r+");
  if (!configFile) {
    Serial.println("Config file not found");
    return;
  }

  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, configFile);
  configFile.close();

  if (error) {
    Serial.println("Failed to parse config file");
    return;
  }

  // Find and remove the stock with the specified apiName
  JsonArray stocksArray = doc["stocks"].as<JsonArray>();
  for (int i = 0; i < stocksArray.size(); i++) {
    if (stocksArray[i]["apiName"] == apiName) {
      stocksArray.remove(i);
      break;
    }
  }

  // Save the updated configuration
  configFile = LittleFS.open("/config.json", "w");
  if (configFile) {
    serializeJson(doc, configFile);
    configFile.close();
    Serial.println("Stock removed successfully");
  } else {
    Serial.println("Failed to write updated config file");
  }
}

void ensureConfigFileExists() {
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS");
    return;
  }

  // Check if config file exists
  if (!LittleFS.exists("/config.json")) {
    Serial.println("No configuration file found. Creating a default one...");

    // Create a new configuration with default values
    StaticJsonDocument<1024> doc;
    doc["displayInterval"] = 10;  // Default interval (10 seconds)
    JsonArray stocksArray = doc.createNestedArray("stocks");  // Empty stocks array

    File configFile = LittleFS.open("/config.json", "w");
    if (configFile) {
      serializeJson(doc, configFile);
      configFile.close();
      Serial.println("Default config file created.");
    } else {
      Serial.println("Failed to create config file.");
    }
  }
}

// Function to create a default JSON configuration file
void createDefaultConfig() {
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS");
    return;
  }

  if (!LittleFS.exists("/config.json")) {
    Serial.println("Creating default config file...");
    StaticJsonDocument<1024> doc;
    doc["displayInterval"] = 10;  // Default interval (10 seconds)
    JsonArray stocksArray = doc.createNestedArray("stocks");  // Empty stocks array

    File configFile = LittleFS.open("/config.json", "w");
    if (configFile) {
      serializeJson(doc, configFile);
      configFile.close();
      Serial.println("Default config file created.");
    } else {
      Serial.println("Failed to create config file.");
    }
  } else {
    Serial.println("Config file already exists.");
  }
}

// Add this new function to fetch weather data
void fetchWeatherData() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  String url = "http://api.openweathermap.org/data/2.5/weather?lat=" + weatherLat + 
               "&lon=" + weatherLon + 
               "&units=metric&appid=" + String(WEATHER_API_KEY);
  
  http.begin(url);
  
  // Set timeout to prevent hanging
  http.setTimeout(2000); // Reduced to 2 second timeout
  
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (!error) {
      temperature = doc["main"]["temp"].as<float>();
      humidity = doc["main"]["humidity"].as<int>();
      weatherDescription = doc["weather"][0]["main"].as<String>();
      if (weatherLocationName.isEmpty() && doc.containsKey("name")) {
        weatherLocationName = doc["name"].as<String>();
      }
    }
  }
  
  http.end();
  esp_task_wdt_reset();  // Reset watchdog after HTTP request
  delay(20); // Minimal delay
}

// Update the readSensorData function
void readSensorData() {
    sensors_event_t humidity = {}, temp = {};
    
    // Read AHT20 sensor
    if (aht.getEvent(&humidity, &temp)) {
        ahtTemperature = temp.temperature;
        ahtHumidity = humidity.relative_humidity;
    }
    
    // Read BMP280 sensor
    bmpTemperature = bmp.readTemperature();
    bmpPressure = bmp.readPressure() / 100.0F; // Convert Pa to hPa
    bmpAltitude = bmp.readAltitude(local_air_pressure);
}

// Updated displayWeatherData function for better scaling
void displayWeatherData() {
    display.clearDisplay();
    drawBorder();
    
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // Display location and online weather with better spacing
    printCenter(weatherLocationName, 0, 3);
    String onlineTemp = String(temperature, 1) + "C";
    printCenter(onlineTemp + " " + weatherDescription, 0, 13);
    
    drawSeparator(23);
    
    // Display local sensor data with better spacing
    display.setCursor(4, 27);
    display.print("AHT20:");
    display.print(ahtTemperature, 1);
    display.print("C ");
    display.print(ahtHumidity, 0);
    display.print("%");
    
    display.setCursor(4, 39);
    display.print("BMP280:");
    display.print(bmpTemperature, 1);
    display.print("C");
    
    display.setCursor(4, 51);
    display.print(bmpPressure, 0);
    display.print("hPa ");
    display.print(bmpAltitude, 0);
    display.print("m");
    
    display.display();
}

// Add function to initialize and configure time
void initTime() {
    configTime(TIMEZONE_OFFSET_HOURS * 3600, TIMEZONE_OFFSET_MINUTES * 60, "pool.ntp.org", "time.nist.gov");
    
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return;
    }
    timeInitialized = true;
    Serial.println("Time initialized!");
}

// Add function to update time
void updateTime() {
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return;
    }
    
    // Format time string
    strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M:%S", &timeinfo);
}

// Updated displayTimeOnly function for better scaling
void displayTimeOnly() {
    display.clearDisplay();
    drawBorder();
    
    display.setTextColor(SSD1306_WHITE);
    
    // Display date with better spacing
    display.setTextSize(1);
    char dateBuff[30];
    strftime(dateBuff, sizeof(dateBuff), "%Y-%m-%d", &timeinfo);
    printCenter(dateBuff, 0, 8);
    
    // Display time with larger size
    display.setTextSize(2);
    printCenter(timeStringBuff, 0, 28);
    
    // Display location name
    display.setTextSize(1);
    printCenter(weatherLocationName, 0, 52);
    
    display.display();
}

// Updated displayError function for better scaling
void displayError(String message) {
  display.clearDisplay();
  drawBorder();
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Draw error icon (X) - slightly repositioned
  int iconX = 8;
  int iconY = 20;
  int iconSize = 8;
  display.drawLine(iconX, iconY, iconX + iconSize, iconY + iconSize, SSD1306_WHITE);
  display.drawLine(iconX, iconY + iconSize, iconX + iconSize, iconY, SSD1306_WHITE);
  
  // Print error message with better spacing
  display.setCursor(iconX + iconSize + 6, iconY);
  display.println("ERROR:");
  display.setCursor(iconX + iconSize + 6, iconY + 12);
  display.println(message);
  
  display.display();
}

// Add this function to handle the main task
void mainTask(void * parameter) {
    static bool wdtRegistered = false;
    static unsigned long lastWeatherUpdate = 0;
    static unsigned long lastSensorUpdate = 0;
    static unsigned long lastTimeUpdate = 0;
    static unsigned long displayStartTime = 0;
    unsigned long currentMillis;

    // Register this task with watchdog only once
    esp_err_t err = esp_task_wdt_add(NULL);
    if (err == ESP_OK) {
        wdtRegistered = true;
        Serial.println("Main task registered with WDT");
    }

    for(;;) {
        currentMillis = millis();
        if (wdtRegistered) {
            esp_task_wdt_reset();  // Only reset if registered
        }

        // Check WiFi connection without blocking
        if (WiFi.status() != WL_CONNECTED) {
            displayError("WiFi Disconnected");
            vTaskDelay(pdMS_TO_TICKS(1000));  // Show error for 1 second
            continue;
        }

        // Update time every second if in time-only mode or if not initialized
        if (!timeInitialized || (currentMillis - lastTimeUpdate >= TIME_UPDATE_INTERVAL)) {
            if (!timeInitialized) {
                initTime();
            } else {
                updateTime();
            }
            lastTimeUpdate = currentMillis;
        }

        // Update weather data every WEATHER_UPDATE_INTERVAL
        if (currentMillis - lastWeatherUpdate >= WEATHER_UPDATE_INTERVAL) {
            if (displayMode != DISPLAY_MODE_STOCKS_ONLY && displayMode != DISPLAY_MODE_TIME_ONLY) {
                fetchWeatherData();
            }
            lastWeatherUpdate = currentMillis;
        }

        // Update sensor data every SENSOR_UPDATE_INTERVAL
        if (currentMillis - lastSensorUpdate >= SENSOR_UPDATE_INTERVAL) {
            if (displayMode != DISPLAY_MODE_STOCKS_ONLY && displayMode != DISPLAY_MODE_TIME_ONLY) {
                readSensorData();
            }
            lastSensorUpdate = currentMillis;
        }

        // Handle different display modes with proper timing
        if (displayMode == DISPLAY_MODE_TIME_ONLY) {
            displayTimeOnly();
            vTaskDelay(pdMS_TO_TICKS(1000));  // Update time display every second
            continue;
        } else if (displayMode == DISPLAY_MODE_WEATHER_ONLY) {
            displayWeatherData();
            vTaskDelay(pdMS_TO_TICKS(displayInterval));  // Use same interval for weather
            continue;
        }

        // Process stocks with better timing
        if (LittleFS.exists("/config.json")) {
            if (xSemaphoreTake(serverSemaphore, portMAX_DELAY)) {
                File configFile = LittleFS.open("/config.json", "r");
                if (!configFile) {
                    xSemaphoreGive(serverSemaphore);
                    displayError("Config Read Error");
                    vTaskDelay(pdMS_TO_TICKS(1000));
                    continue;
                }

                StaticJsonDocument<1024> doc;
                DeserializationError error = deserializeJson(doc, configFile);
                configFile.close();
                xSemaphoreGive(serverSemaphore);

                if (error) {
                    displayError("JSON Parse Error");
                    vTaskDelay(pdMS_TO_TICKS(1000));
                    continue;
                }

                JsonArray stocks = doc["stocks"].as<JsonArray>();
                if (stocks.size() == 0) {
                    if (displayMode != DISPLAY_MODE_STOCKS_ONLY) {
                        displayWeatherData();
                        vTaskDelay(pdMS_TO_TICKS(displayInterval));
                    } else {
                        display.clearDisplay();
                        drawBorder();
                        display.setTextColor(SSD1306_WHITE);
                        display.setTextSize(1);
                        printCenter("No stocks", 0, 20);
                        printCenter("configured", 0, 35);
                        display.display();
                        vTaskDelay(pdMS_TO_TICKS(2000));  // Show message for 2 seconds
                    }
                    continue;
                }

                // Process each stock with proper timing
                static float lastPrice = 0;
                for (JsonVariant stock : stocks) {
                    esp_task_wdt_reset();
                    displayStartTime = millis();  // Track when we start displaying this stock
                    
                    String apiName = stock["apiName"].as<String>();
                    String displayName = stock["displayName"].as<String>();
                    
                    float stockPrice = 0;
                    String stockTime = "";
                    
                    // Fetch stock data with timeout
                    unsigned long fetchStart = millis();
                    bool fetchTimeout = false;
                    
                    while (stockPrice == 0 && !fetchTimeout) {
                        stockPrice = fetchPrice(apiName.c_str());
                        stockTime = fetchTime(apiName.c_str());
                        
                        if (millis() - fetchStart > 3000) {
                            fetchTimeout = true;
                        }
                        esp_task_wdt_reset();
                        vTaskDelay(pdMS_TO_TICKS(50));
                    }

                    // Display stock data for the defined time
                    if (stockPrice > 0) {
                        displayStockData(displayName, stockPrice, stockTime.length() > 0 ? stockTime : "No update time", lastPrice);
                        lastPrice = stockPrice;
                    } else {
                        display.clearDisplay();
                        drawBorder();
                        display.setTextColor(SSD1306_WHITE);
                        display.setTextSize(1);
                        printCenter(displayName, 0, 4);
                        drawSeparator(16);
                        display.setTextSize(2);
                        printCenter("Error", 0, 25);
                        display.setTextSize(1);
                        printCenter(fetchTimeout ? "Timeout" : "Fetch Error", 0, 52);
                        display.display();
                    }
                    
                    // Show each stock for the user-configured time
                    vTaskDelay(pdMS_TO_TICKS(displayInterval));
                }

                // Show weather after stocks if in combined mode
                if (displayMode == DISPLAY_MODE_STOCKS_AND_WEATHER) {
                    displayWeatherData();
                    vTaskDelay(pdMS_TO_TICKS(displayInterval));  // Use same interval for weather
                }
            }
        } else {
            displayError("No Config");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));  // Small delay between cycles
    }
}

// Update the async web server task
void asyncWebServerTask(void * parameter) {
    static bool wdtRegistered = false;

    // Register this task with watchdog only once
    esp_err_t err = esp_task_wdt_add(NULL);
    if (err == ESP_OK) {
        wdtRegistered = true;
        Serial.println("Async web server task registered with WDT");
    }

    for(;;) {
        if (wdtRegistered) {
            esp_task_wdt_reset();  // Only reset if registered
        }
        
        // Check if WiFi is connected
        if (WiFi.status() == WL_CONNECTED) {
            // Handle any pending server operations
            if (xSemaphoreTake(serverSemaphore, pdMS_TO_TICKS(100)) == pdPASS) {
                // Server is available for operations
                xSemaphoreGive(serverSemaphore);
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));  // Give other tasks time to run
    }
}

void setup() 
{
    Serial.begin(115200);
    
    // Initialize display first for startup sequence
    Wire.begin();
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        return;
    }
    
    // Show logo
    display.clearDisplay();
    display.drawBitmap(0, 0, logo, 128, 64, WHITE);                             //Display bitmap from array
    display.display();
    delay(2000); // Show logo for 2 seconds
    
    // Create semaphore before anything else
    serverSemaphore = xSemaphoreCreateMutex();
    if (serverSemaphore == NULL) {
        Serial.println("Error creating semaphore");
        return;
    }

    // Initialize watchdog with better configuration
    esp_task_wdt_deinit(); // First deinitialize if already initialized
    esp_task_wdt_config_t wdt_config = {
        .timeout_ms = WDT_TIMEOUT_SECONDS * 1000,
        .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,
        .trigger_panic = true
    };
    esp_task_wdt_init(&wdt_config);

    // Initialize WiFi and show connection status
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Connecting to WiFi...");
    display.display();
    
    int wifiAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
        delay(500);
        Serial.print(".");
        display.print(".");
        display.display();
        wifiAttempts++;
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nWiFi connection failed!");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("WiFi connection");
        display.println("failed!");
        display.display();
        delay(2000);
        return;
    }

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("WiFi connected!");
    display.println("IP address:");
    display.println(WiFi.localIP().toString());
    display.println("\n\nMERT GUZELMANSUR");
    display.display();
    delay(2000);

    // Initialize LittleFS
    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    // Configure server endpoints
    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
    
    // Add CORS headers to all responses
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

    // Register all handlers
    server.on("/get-config", HTTP_GET, handleConfig);
    
    AsyncCallbackJsonWebHandler* saveHandler = new AsyncCallbackJsonWebHandler(
        "/save-config",
        [](AsyncWebServerRequest *request, JsonVariant &json) {
            if (xSemaphoreTake(serverSemaphore, portMAX_DELAY)) {
                StaticJsonDocument<1024> doc = json.as<JsonObject>();
                File configFile = LittleFS.open("/config.json", "w");
                if (configFile) {
                    serializeJson(doc, configFile);
                    configFile.close();
                    xSemaphoreGive(serverSemaphore);
                    request->send(200, "application/json", "{\"status\":\"success\"}");
                } else {
                    xSemaphoreGive(serverSemaphore);
                    request->send(500, "application/json", "{\"status\":\"error\"}");
                }
            } else {
                request->send(503, "text/plain", "Server busy");
            }
        }
    );
    server.addHandler(saveHandler);

    // Add other handlers
    AsyncCallbackJsonWebHandler* weatherHandler = new AsyncCallbackJsonWebHandler("/update-weather-location", handleWeatherLocation);
    AsyncCallbackJsonWebHandler* displayHandler = new AsyncCallbackJsonWebHandler("/update-display-settings", handleDisplaySettings);
    AsyncCallbackJsonWebHandler* stockHandler = new AsyncCallbackJsonWebHandler("/add-stock", handleAddStock);
    AsyncCallbackJsonWebHandler* intervalHandler = new AsyncCallbackJsonWebHandler("/update-interval", handleUpdateInterval);
    
    server.addHandler(weatherHandler);
    server.addHandler(displayHandler);
    server.addHandler(stockHandler);
    server.addHandler(intervalHandler);
    
    server.on("/get-live-data", HTTP_GET, handleLiveData);
    server.on("/get-stock-data", HTTP_GET, handleStockData);
    server.on("/clear-all-stocks", HTTP_POST, handleClearStocks);

    // Initialize server before creating tasks
    server.begin();
    Serial.println("HTTP server started");

    // Create tasks with proper core assignment
    xTaskCreatePinnedToCore(
        mainTask,
        "mainTask",
        STACK_SIZE,
        NULL,
        1,
        &mainTaskHandle,
        ARDUINO_RUNNING_CORE
    );

    xTaskCreatePinnedToCore(
        asyncWebServerTask,
        "asyncWebServer",
        STACK_SIZE,
        NULL,
        ASYNC_TCP_PRIORITY,
        &asyncWebServerHandle,
        !ARDUINO_RUNNING_CORE  // Run on the other core
    );

    // Initialize rest of the components
    if (!aht.begin()) {
        Serial.println("Could not find AHT20? Check wiring!");
    }
    
    if (!bmp.begin()) {
        Serial.println("Could not find BMP280? Check wiring!");
    } else {
        bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                       Adafruit_BMP280::SAMPLING_X2,
                       Adafruit_BMP280::SAMPLING_X16,
                       Adafruit_BMP280::FILTER_X16,
                       Adafruit_BMP280::STANDBY_MS_500);
    }
    
    initTime();
    loadStockConfiguration();
    fetchWeatherData();
    
    Serial.println("Setup complete!");
}

// Remove the original loop function content and just add a delay
void loop() {
    vTaskDelete(NULL);  // Delete the original loop task
}

// Add server handlers with semaphore protection
void handleWeatherLocation(AsyncWebServerRequest *request, JsonVariant &json) {
    if (xSemaphoreTake(serverSemaphore, portMAX_DELAY)) {
        StaticJsonDocument<200> doc = json.as<JsonObject>();
        
        String newLat = doc["latitude"].as<String>();
        String newLon = doc["longitude"].as<String>();
        String newLocationName = doc["locationName"].as<String>();
        
        weatherLat = newLat;
        weatherLon = newLon;
        weatherLocationName = newLocationName;
        
        StaticJsonDocument<1024> config;
        if (LittleFS.exists("/config.json")) {
            File configFile = LittleFS.open("/config.json", "r");
            deserializeJson(config, configFile);
            configFile.close();
        }
        
        config["weatherLat"] = weatherLat;
        config["weatherLon"] = weatherLon;
        config["weatherLocationName"] = weatherLocationName;
        
        File configFile = LittleFS.open("/config.json", "w");
        if (configFile) {
            serializeJson(config, configFile);
            configFile.close();
            xSemaphoreGive(serverSemaphore);
            request->send(200, "application/json", "{\"status\":\"success\"}");
        } else {
            xSemaphoreGive(serverSemaphore);
            request->send(500, "application/json", "{\"status\":\"error\"}");
        }
    } else {
        request->send(503, "text/plain", "Server busy");
    }
}

void handleDisplaySettings(AsyncWebServerRequest *request, JsonVariant &json) {
    if (xSemaphoreTake(serverSemaphore, portMAX_DELAY)) {
        StaticJsonDocument<200> doc = json.as<JsonObject>();
        String newDisplayMode = doc["displayMode"].as<String>();
        
        displayMode = newDisplayMode;
        
        StaticJsonDocument<1024> config;
        if (LittleFS.exists("/config.json")) {
            File configFile = LittleFS.open("/config.json", "r");
            deserializeJson(config, configFile);
            configFile.close();
        }
        
        config["displayMode"] = displayMode;
        
        File configFile = LittleFS.open("/config.json", "w");
        if (configFile) {
            serializeJson(config, configFile);
            configFile.close();
            xSemaphoreGive(serverSemaphore);
            request->send(200, "application/json", "{\"status\":\"success\"}");
        } else {
            xSemaphoreGive(serverSemaphore);
            request->send(500, "application/json", "{\"status\":\"error\"}");
        }
    } else {
        request->send(503, "text/plain", "Server busy");
    }
}

void handleAddStock(AsyncWebServerRequest *request, JsonVariant &json) {
    if (xSemaphoreTake(serverSemaphore, portMAX_DELAY)) {
        StaticJsonDocument<256> doc = json.as<JsonObject>();
        String apiName = doc["apiName"];
        String displayName = doc["displayName"];
        
        StaticJsonDocument<1024> config;
        if (LittleFS.exists("/config.json")) {
            File configFile = LittleFS.open("/config.json", "r");
            deserializeJson(config, configFile);
            configFile.close();
        } else {
            config["displayInterval"] = 10;
            config.createNestedArray("stocks");
        }
        
        JsonArray stocks = config["stocks"].as<JsonArray>();
        
        if (stocks.size() >= MAX_STOCKS) {
            xSemaphoreGive(serverSemaphore);
            request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Maximum stocks reached\"}");
            return;
        }
        
        JsonObject newStock = stocks.createNestedObject();
        newStock["apiName"] = apiName;
        newStock["displayName"] = displayName;
        
        File configFile = LittleFS.open("/config.json", "w");
        if (configFile) {
            serializeJson(config, configFile);
            configFile.close();
            xSemaphoreGive(serverSemaphore);
            request->send(200, "application/json", "{\"status\":\"success\"}");
        } else {
            xSemaphoreGive(serverSemaphore);
            request->send(500, "application/json", "{\"status\":\"error\"}");
        }
    } else {
        request->send(503, "text/plain", "Server busy");
    }
}

void handleLiveData(AsyncWebServerRequest *request) {
    if (xSemaphoreTake(serverSemaphore, portMAX_DELAY)) {
        StaticJsonDocument<2048> doc;
        
        // Add time data
        char timeBuff[9];
        char dateBuff[11];
        strftime(timeBuff, sizeof(timeBuff), "%H:%M:%S", &timeinfo);
        strftime(dateBuff, sizeof(dateBuff), "%Y-%m-%d", &timeinfo);
        doc["time"] = timeBuff;
        doc["date"] = dateBuff;
        
        // Add weather data
        JsonObject weather = doc.createNestedObject("weather");
        weather["temperature"] = temperature;
        weather["humidity"] = humidity;
        weather["description"] = weatherDescription;
        weather["location"] = weatherLocationName;
        
        // Add sensor data
        JsonObject sensors = doc.createNestedObject("sensors");
        JsonObject aht = sensors.createNestedObject("aht");
        aht["temperature"] = ahtTemperature;
        aht["humidity"] = ahtHumidity;
        
        JsonObject bmp = sensors.createNestedObject("bmp");
        bmp["temperature"] = bmpTemperature;
        bmp["pressure"] = bmpPressure;
        bmp["altitude"] = bmpAltitude;
        
        String response;
        serializeJson(doc, response);
        xSemaphoreGive(serverSemaphore);
        request->send(200, "application/json", response);
    } else {
        request->send(503, "text/plain", "Server busy");
    }
}

void handleStockData(AsyncWebServerRequest *request) {
    if (!request->hasParam("apiName")) {
        request->send(400, "application/json", "{\"error\":\"Missing apiName\"}");
        return;
    }
    
    if (xSemaphoreTake(serverSemaphore, portMAX_DELAY)) {
        String apiName = request->getParam("apiName")->value();
        float price = fetchPrice(apiName.c_str());
        String time = fetchTime(apiName.c_str());
        
        StaticJsonDocument<256> doc;
        doc["price"] = price;
        doc["time"] = time;
        
        String response;
        serializeJson(doc, response);
        xSemaphoreGive(serverSemaphore);
        request->send(200, "application/json", response);
    } else {
        request->send(503, "text/plain", "Server busy");
    }
}

void handleClearStocks(AsyncWebServerRequest *request) {
    if (xSemaphoreTake(serverSemaphore, portMAX_DELAY)) {
        StaticJsonDocument<128> config;
        config["displayInterval"] = 10;
        config.createNestedArray("stocks");
        
        File configFile = LittleFS.open("/config.json", "w");
        if (configFile) {
            serializeJson(config, configFile);
            configFile.close();
            xSemaphoreGive(serverSemaphore);
            request->send(200, "application/json", "{\"status\":\"success\"}");
        } else {
            xSemaphoreGive(serverSemaphore);
            request->send(500, "application/json", "{\"status\":\"error\"}");
        }
    } else {
        request->send(503, "text/plain", "Server busy");
    }
}

// Add handler for updating display interval
void handleUpdateInterval(AsyncWebServerRequest *request, JsonVariant &json) {
    if (xSemaphoreTake(serverSemaphore, portMAX_DELAY)) {
        StaticJsonDocument<128> doc = json.as<JsonObject>();
        int newInterval = doc["interval"].as<int>();
        
        // Ensure the interval is reasonable
        if (newInterval < 1) newInterval = 1;
        if (newInterval > 60) newInterval = 60;
        
        StaticJsonDocument<1024> config;
        if (LittleFS.exists("/config.json")) {
            File configFile = LittleFS.open("/config.json", "r");
            deserializeJson(config, configFile);
            configFile.close();
        }
        
        config["displayInterval"] = newInterval;
        displayInterval = newInterval * 1000; // Convert seconds to milliseconds
        
        File configFile = LittleFS.open("/config.json", "w");
        if (configFile) {
            serializeJson(config, configFile);
            configFile.close();
            xSemaphoreGive(serverSemaphore);
            request->send(200, "application/json", "{\"status\":\"success\"}");
            
            // Load the new configuration immediately
            loadStockConfiguration();
        } else {
            xSemaphoreGive(serverSemaphore);
            request->send(500, "application/json", "{\"status\":\"error\"}");
        }
    } else {
        request->send(503, "text/plain", "Server busy");
    }
}