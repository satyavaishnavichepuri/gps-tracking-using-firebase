/*
 * ESP32 Real-Time GPS Tracker with Firebase
 * 
 * Hardware Setup:
 * - ESP32 UART2: RX = GPIO17, TX = GPIO16
 * - NEO-6M GPS Module: Connect TX to ESP32 GPIO17, RX to ESP32 GPIO16
 * - GND and 3.3V connections required
 * 
 * Required Libraries:
 * - TinyGPSPlus by Mikal Hart (Install via Arduino IDE)
 * - FirebaseClient by Mobizt
 * - ArduinoJson by Benoit Blanchon
 * 
 * Instructions:
 * 1. Install the required libraries
 * 2. Replace WIFI_SSID, WIFI_PASSWORD, API_KEY, and DATABASE_URL
 * 3. Upload to ESP32
 * 4. Open Serial Monitor at 115200 baud
 */

#include <HardwareSerial.h>
#include <TinyGPSPlus.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// ==================== CONFIGURATION ====================
// WiFi Credentials
//check 25 april 2026 nirvana 2 44 am for next lines until host.
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// Firebase Configuration
#define API_KEY ""
#define DATABASE_URL ""
#define FIREBASE_HOST ""
#define FIREBASE_PATH "/bus/live.json"

// GPS Configuration
#define GPS_RX_PIN 16
#define GPS_TX_PIN 17
#define GPS_BAUD_RATE 9600

// Upload Interval (milliseconds)
#define UPLOAD_INTERVAL 3000

// ==================== GLOBAL OBJECTS ====================
HardwareSerial gpsSerial(2); // UART2 on ESP32
TinyGPSPlus gps;

WiFiClientSecure ssl;

// ==================== TIMING VARIABLES ====================
unsigned long lastUploadTime = 0;
unsigned long lastSerialUpdate = 0;
unsigned long serialUpdateInterval = 1000; // Update serial every 1 second

// ==================== STATUS TRACKING ====================
bool wifiConnected = false;
bool firebaseConnected = false;
bool gpsDataValid = false;
int uploadCount = 0;
int failedUploadCount = 0;

// ==================== SETUP FUNCTION ====================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n");
  Serial.println("╔════════════════════════════════════════╗");
  Serial.println("║   ESP32 GPS Tracker - Firebase v1.0   ║");
  Serial.println("╚════════════════════════════════════════╝");
  Serial.println();
  
  Serial.println("[INIT] Initializing GPS Serial on UART2...");
  gpsSerial.begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  delay(500);
  Serial.println("[OK] GPS Serial initialized at 9600 baud");
  Serial.printf("[INFO] GPS RX Pin: %d, TX Pin: %d\n", GPS_RX_PIN, GPS_TX_PIN);
  Serial.println();
  
  Serial.println("[INIT] Connecting to WiFi...");
  connectToWiFi();
  
  Serial.println("[INIT] Configuring Firebase...");
  configureFirebase();
  
  Serial.println();
  Serial.println("╔════════════════════════════════════════╗");
  Serial.println("║       System Initialization Complete   ║");
  Serial.println("╚════════════════════════════════════════╝");
  Serial.println();
}

// ==================== MAIN LOOP ====================
void loop() {
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);
  }
  if (millis() - lastSerialUpdate >= serialUpdateInterval) {
    updateSerialDisplay();
    lastSerialUpdate = millis();
  }
  
  if (millis() - lastUploadTime >= UPLOAD_INTERVAL) {
    if (gps.location.isValid()) {
      uploadToFirebase();
      lastUploadTime = millis();
    } else if (gps.location.isUpdated()) {
      Serial.println("[WARN] GPS location data not yet valid...");
    }
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    if (wifiConnected) {
      Serial.println("[ERROR] WiFi disconnected!");
      wifiConnected = false;
    }
    connectToWiFi();
  }
  
  delay(10);
}

// ==================== WiFi CONNECTION ====================
void connectToWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!wifiConnected) {
      wifiConnected = true;
      Serial.println("[OK] WiFi already connected!");
      printWiFiStatus();
    }
    return;
  }
  
  Serial.printf("[WIFI] Connecting to: %s\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println("[OK] WiFi connected successfully!");
    printWiFiStatus();
  } else {
    wifiConnected = false;
    Serial.println("[ERROR] WiFi connection failed!");
  }
}

void printWiFiStatus() {
  Serial.printf("[WIFI] SSID: %s\n", WiFi.SSID().c_str());
  Serial.printf("[WIFI] Signal Strength: %d dBm\n", WiFi.RSSI());
  Serial.printf("[WIFI] IP Address: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("[WIFI] MAC Address: %s\n", WiFi.macAddress().c_str());
}

// ==================== FIREBASE CONFIGURATION ====================
void configureFirebase() {
  ssl.setInsecure();
  firebaseConnected = true;
  Serial.println("[OK] Firebase configured (direct HTTPS)");
  Serial.printf("[FB] Database URL: %s\n", DATABASE_URL);
}

// ==================== FIREBASE DATA UPLOAD ====================
void uploadToFirebase() {
  if (!wifiConnected) {
    Serial.println("[WARN] WiFi not connected - cannot upload");
    return;
  }
  
  StaticJsonDocument<512> doc;
  
  doc["latitude"] = gps.location.lat();
  doc["longitude"] = gps.location.lng();
  doc["altitude"] = gps.altitude.meters();
  doc["speed_kmh"] = gps.speed.kmph();
  doc["course"] = gps.course.deg();
  doc["satellites"] = gps.satellites.value();
  doc["hdop"] = gps.hdop.hdop();
  
  char dateStr[20];
  char timeStr[20];
  
  if (gps.date.isValid()) {
    sprintf(dateStr, "%04d-%02d-%02d", 
            gps.date.year(), 
            gps.date.month(), 
            gps.date.day());
    doc["date"] = dateStr;
  }
  
  if (gps.time.isValid()) {
    sprintf(timeStr, "%02d:%02d:%02d", 
            gps.time.hour(), 
            gps.time.minute(), 
            gps.time.second());
    doc["time"] = timeStr;
  }
  
  doc["timestamp"] = (unsigned long)time(nullptr);
  doc["millis"] = millis();
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  ssl.setInsecure();
  if (ssl.connect(FIREBASE_HOST, 443)) {
    ssl.println("PUT " + String(FIREBASE_PATH) + "?auth=" + String(API_KEY) + " HTTP/1.1");
    ssl.println("Host: " + String(FIREBASE_HOST));
    ssl.println("Content-Type: application/json");
    ssl.print("Content-Length: ");
    ssl.println(jsonString.length());
    ssl.println("Connection: close");
    ssl.println();
    ssl.println(jsonString);
    
    unsigned long timeout = millis();
    while (ssl.connected() && millis() - timeout < 5000) {
      if (ssl.available()) {
        String line = ssl.readStringUntil('\n');
        if (line.indexOf("HTTP/1.1 200") >= 0) {
          firebaseConnected = true;
          uploadCount++;
          
          Serial.println();
          Serial.println("╔════════════════════════════════════════╗");
          Serial.println("║        DATA UPLOADED SUCCESSFULLY      ║");
          Serial.println("╚════════════════════════════════════════╝");
          
          printGPSData();
          
          Serial.printf("[STATS] Upload #%d | Failed: %d\n", uploadCount, failedUploadCount);
          Serial.printf("[FB] Path: %s\n", FIREBASE_PATH);
          Serial.println();
          
          ssl.stop();
          return;
        }
      }
    }
    
    ssl.stop();
  }
  
  firebaseConnected = false;
  failedUploadCount++;
  Serial.println("[ERROR] Firebase upload failed!");
  Serial.printf("[STATS] Upload #%d | Failed: %d\n", uploadCount, failedUploadCount);
}

// ==================== GPS DATA DISPLAY ====================
void printGPSData() {
  Serial.println("┌─────────────────────────────────────────┐");
  Serial.println("│         CURRENT GPS READING             │");
  Serial.println("├─────────────────────────────────────────┤");
  Serial.printf("│ Location: %.6f, %.6f\n", 
                gps.location.lat(), 
                gps.location.lng());
  Serial.printf("│ Altitude: %.2f meters\n", gps.altitude.meters());
  Serial.printf("│ Speed: %.2f km/h\n", gps.speed.kmph());
  Serial.printf("│ Course: %.2f degrees\n", gps.course.deg());
  Serial.printf("│ Satellites: %d\n", gps.satellites.value());
  Serial.printf("│ HDOP: %.2f\n", gps.hdop.hdop());
  
  if (gps.date.isValid()) {
    Serial.printf("│ Date: %04d-%02d-%02d\n", 
                  gps.date.year(), 
                  gps.date.month(), 
                  gps.date.day());
  }
  
  if (gps.time.isValid()) {
    Serial.printf("│ Time: %02d:%02d:%02d UTC\n", 
                  gps.time.hour(), 
                  gps.time.minute(), 
                  gps.time.second());
  }
  
  Serial.println("└─────────────────────────────────────────┘");
}

// ==================== SERIAL DISPLAY UPDATE ====================
void updateSerialDisplay() {
  Serial.println();
  Serial.println("╔════════════════════════════════════════╗");
  Serial.println("║         SYSTEM STATUS UPDATE           ║");
  Serial.println("╚════════════════════════════════════════╝");
  
  if (wifiConnected && WiFi.status() == WL_CONNECTED) {
    Serial.println("✓ WiFi: CONNECTED");
    Serial.printf("  └─ Signal: %d dBm | IP: %s\n", 
                  WiFi.RSSI(), 
                  WiFi.localIP().toString().c_str());
  } else {
    Serial.println("✗ WiFi: DISCONNECTED");
  }
  
  if (firebaseConnected) {
    Serial.println("✓ Firebase: CONNECTED");
  } else {
    Serial.println("✗ Firebase: DISCONNECTED");
  }
  
  if (gps.location.isValid()) {
    Serial.println("✓ GPS: VALID");
    Serial.printf("  └─ Location: %.6f, %.6f\n", 
                  gps.location.lat(), 
                  gps.location.lng());
  } else if (gps.location.isUpdated()) {
    Serial.println("⚠ GPS: WAITING FOR VALID FIX");
    Serial.printf("  └─ Satellites: %d\n", gps.satellites.value());
  } else {
    Serial.println("✗ GPS: NO DATA");
  }
  
  Serial.printf("⚙ Uploads: %d successful | %d failed\n", uploadCount, failedUploadCount);
  
  unsigned long uptime = millis() / 1000;
  int hours = uptime / 3600;
  int minutes = (uptime % 3600) / 60;
  int seconds = uptime % 60;
  Serial.printf("⏱ Uptime: %dh %dm %ds\n", hours, minutes, seconds);
  
  Serial.println();
}
