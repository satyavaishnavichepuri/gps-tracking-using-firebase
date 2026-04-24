# Arduino Library Installation Guide

## 📚 REQUIRED LIBRARIES

This project requires 3 essential libraries. Follow the instructions below for each one.

---

## Library #1: TinyGPSPlus

**Purpose**: Parse NMEA sentences from GPS module  
**Author**: Mikal Hart  
**Maintained**: Yes (Active)

### Installation Steps:

1. **Open Arduino IDE**
2. Go to **Sketch** → **Include Library** → **Manage Libraries**
3. In the search box, type: `TinyGPSPlus`
4. Find the entry by **Mikal Hart**
5. Click it to select
6. Click **"Install"** button
7. Wait for completion message: "TinyGPSPlus installed"

### Verify Installation:

Create a test sketch:
```cpp
#include <TinyGPSPlus.h>

TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  Serial.println("TinyGPSPlus version: ");
  Serial.println(TinyGPSPlus::libraryVersion());
}

void loop() {}
```

Upload to ESP32. Serial Monitor should show version number.

### Library Files Location:
- **Windows**: `Documents\Arduino\libraries\TinyGPSPlus`
- **macOS**: `~/Documents/Arduino/libraries/TinyGPSPlus`
- **Linux**: `~/Arduino/libraries/TinyGPSPlus`

---

## Library #2: Firebase Arduino Client Library for ESP32

**Purpose**: Firebase Realtime Database communication  
**Author**: Mobizt (K. Suwatchai)  
**Important**: Use latest v4.x (NOT deprecated Firebase_ESP_Client)  
**Maintained**: Yes (Very Active)

### Installation Steps:

1. **Open Arduino IDE**
2. Go to **Sketch** → **Include Library** → **Manage Libraries**
3. In the search box, type: `Firebase Arduino Client`
4. Find the entry by **Mobizt**
   - Full name: "Firebase Arduino Client Library for ESP32"
5. Click it to select
6. **IMPORTANT**: Select version **4.4.0 or higher** (latest)
   - Click dropdown if showing older version
   - Select "Latest" or the highest number
7. Click **"Install"** button
8. **WAIT** - This library is large (5-10MB), may take 1-2 minutes
9. Wait for completion message

### Dependency Notes:

This library may auto-install dependencies:
- ArduinoJson (if not already installed)
- Accept all dependency installations

### Verify Installation:

In Arduino IDE:
```cpp
#include <Firebase_ESP_Client.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Firebase library loaded!");
}

void loop() {}
```

Should compile without errors.

### Library Files Location:
- **Windows**: `Documents\Arduino\libraries\Firebase-ESP-Client`
- **macOS**: `~/Documents/Arduino/libraries/Firebase-ESP-Client`
- **Linux**: `~/Arduino/libraries/Firebase-ESP-Client`

### Common Installation Issues:

**Issue**: "Firebase Arduino Client Library not found in Manage Libraries"
- **Solution**: 
  1. Update Arduino IDE to latest version
  2. Check internet connection
  3. Try searching just: `Firebase`
  4. Manual installation:
     - Download: https://github.com/mobizt/Firebase-ESP-Client
     - Extract to Arduino/libraries folder

**Issue**: "Unknown include file <Firebase_ESP_Client.h>"
- **Solution**:
  1. Close Arduino IDE completely
  2. Go to Arduino libraries folder
  3. Check folder is named exactly: `Firebase-ESP-Client`
  4. Restart Arduino IDE
  5. Verify via Sketch → Include Library (should appear)

---

## Library #3: ArduinoJson

**Purpose**: JSON parsing and generation  
**Author**: Benoit Blanchon  
**Maintained**: Yes (Very Active)

### Installation Steps:

1. **Open Arduino IDE**
2. Go to **Sketch** → **Include Library** → **Manage Libraries**
3. In the search box, type: `ArduinoJson`
4. Find the entry by **Benoit Blanchon**
5. Click it to select
6. Select version **6.19.0 or higher** (latest v6 series)
   - Note: v7.x also available but v6 is more stable for ESP32
7. Click **"Install"** button
8. Wait for completion

### Verify Installation:

```cpp
#include <ArduinoJson.h>

void setup() {
  Serial.begin(115200);
  
  // Create a JSON document
  StaticJsonDocument<200> doc;
  doc["latitude"] = 17.3606;
  doc["longitude"] = 78.4747;
  
  // Serialize to string
  String output;
  serializeJson(doc, output);
  
  Serial.println(output);
  // Should print: {"latitude":17.3606,"longitude":78.4747}
}

void loop() {}
```

### Library Files Location:
- **Windows**: `Documents\Arduino\libraries\ArduinoJson`
- **macOS**: `~/Documents/Arduino/libraries/ArduinoJson`
- **Linux**: `~/Arduino/libraries/ArduinoJson`

---

## ✅ VERIFICATION CHECKLIST

After installing all three libraries, verify by:

### Method 1: Check Library Manager
1. **Sketch** → **Include Library**
2. Look for these under "Contributed libraries":
   - ✓ ArduinoJson
   - ✓ Firebase Arduino Client Library for ESP32
   - ✓ TinyGPSPlus

### Method 2: Compile Test Sketch
```cpp
#include <TinyGPSPlus.h>
#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>

TinyGPSPlus gps;
FirebaseData fbdo;

void setup() {
  Serial.begin(115200);
  Serial.println("All libraries compiled successfully!");
}

void loop() {}
```

**Should compile with NO errors**

### Method 3: Check Library Folders
Windows explorer (or Finder/Files):
- Navigate to: `Documents/Arduino/libraries/`
- Should see folders:
  - ✓ `TinyGPSPlus`
  - ✓ `Firebase-ESP-Client`
  - ✓ `ArduinoJson`

---

## 🔧 ADVANCED SETUP (If Manual Installation Needed)

### Manual Installation Method:

**Step 1: Download Library ZIP**
1. Go to GitHub repository
2. Click green **"Code"** button
3. Select **"Download ZIP"**
4. Note the download location

**Step 2: Install in Arduino IDE**
1. **Sketch** → **Include Library** → **Add .ZIP Library...**
2. Browse to downloaded ZIP file
3. Select and click **Open**
4. Arduino IDE extracts and installs automatically

**Step 3: Restart Arduino IDE**
1. Close Arduino IDE completely
2. Reopen Arduino IDE
3. Library should now appear in Include Library menu

### GitHub URLs for Manual Download:

- TinyGPSPlus: https://github.com/mikalhart/TinyGPSPlus
- Firebase ESP Client: https://github.com/mobizt/Firebase-ESP-Client
- ArduinoJson: https://github.com/bblanchon/ArduinoJson

---

## 🆘 TROUBLESHOOTING LIBRARY ISSUES

### Scenario 1: "Multiple definitions of 'setup()'" Error
- **Cause**: Old Firebase library installed alongside new one
- **Solution**:
  1. Delete: `Documents/Arduino/libraries/Firebase_ESP_Client`
  2. Delete: `Documents/Arduino/libraries/firebase-arduino`
  3. Keep only: `Firebase-ESP-Client`
  4. Restart Arduino IDE

### Scenario 2: "fatal error: Firebase_ESP_Client.h: No such file or directory"
- **Cause**: Library not found in libraries folder
- **Solution**:
  1. Go to: Sketch → Include Library → Manage Libraries
  2. Search for "Firebase"
  3. If not found, install via Manage Libraries
  4. If still not found, try manual ZIP installation
  5. Restart Arduino IDE after manual install

### Scenario 3: "conflicting declaration" for StaticJsonDocument
- **Cause**: Multiple ArduinoJson versions installed
- **Solution**:
  1. Delete all ArduinoJson folders from libraries
  2. Reinstall via Manage Libraries (latest v6 only)
  3. Verify folder name is exactly: `ArduinoJson`

### Scenario 4: Out of Memory or Stack Overflow
- **Cause**: Old versions of libraries using too much SRAM
- **Solution**:
  1. Update all libraries to latest versions
  2. Check library versions in Manage Libraries
  3. For Firebase: must be v4.0+
  4. For ArduinoJson: must be v6.13+
  5. For TinyGPSPlus: use latest version

### Scenario 5: "The library does not support this board"
- **Cause**: Trying to use board manager for unsupported board
- **Solution**:
  1. Ensure ESP32 board is properly installed
  2. **Tools** → **Board Manager** → Search "ESP32"
  3. Install "ESP32 by Espressif Systems"
  4. Restart Arduino IDE
  5. Try library installation again

---

## 📦 LIBRARY VERSION REFERENCE

**Recommended Versions** (as of January 2025):

| Library | Version | URL |
|---------|---------|-----|
| TinyGPSPlus | 1.0.3+ | github.com/mikalhart/TinyGPSPlus |
| Firebase Arduino Client | 4.4.0+ | github.com/mobizt/Firebase-ESP-Client |
| ArduinoJson | 6.19.0+ | github.com/bblanchon/ArduinoJson |

**Version Check in Sketch**:
```cpp
// Add after includes to print versions
Serial.print("ArduinoJson version: ");
Serial.println(ARDUINOJSON_VERSION);  // e.g., "6190001"
```

---

## 🚀 QUICK INSTALLATION SCRIPT (Windows PowerShell)

If you prefer command-line installation:

```powershell
# Requires: Arduino CLI installed

arduino-cli library install "TinyGPSPlus"
arduino-cli library install "Firebase Arduino Client Library for ESP32"
arduino-cli library install "ArduinoJson"

# Verify installation
arduino-cli library list | grep -E "TinyGPS|Firebase|ArduinoJson"
```

---

## 📝 LIBRARY TROUBLESHOOTING QUICK REFERENCE

| Error Message | Solution |
|--------------|----------|
| "fatal error: TinyGPS++.h" | Reinstall TinyGPSPlus library |
| "fatal error: Firebase_ESP_Client.h" | Install Firebase Arduino Client v4.0+ |
| "fatal error: ArduinoJson.h" | Install ArduinoJson v6+ |
| "expected ';' before 'TinyGPSPlus'" | Restart Arduino IDE after install |
| "unknown identifier 'Firebase'" | Close and reopen Arduino IDE |
| "undefined reference to 'firebase'" | Verify library is in libraries folder |

---

## ✨ POST-INSTALLATION TIPS

### 1. Keep Libraries Updated
- **Sketch** → **Include Library** → **Manage Libraries**
- Look for **UPDATE** buttons next to installed libraries
- Update all libraries to latest versions
- Restart Arduino IDE after updates

### 2. Clear Cache If Issues Persist
1. Close Arduino IDE
2. Delete Arduino cache:
   - **Windows**: `%TEMP%\arduino*` folders
   - **macOS**: `~/Library/Arduino15/`
   - **Linux**: `~/.arduino15/`
3. Reopen Arduino IDE (cache rebuilds automatically)

### 3. Check Arduino IDE Version
- **File** → **About Arduino IDE**
- Should be Arduino IDE 2.0+ (recommended)
- Arduino 1.8.x also works but 2.0+ is faster

### 4. Performance Settings
For ESP32, in Arduino IDE:
- **Tools** → **Board** → **ESP32-DevKit C**
- **Tools** → **Upload Speed** → `921600` (faster uploads)
- **Tools** → **CPU Frequency** → `240 MHz` (performance)

---

## 📞 LIBRARY DOCUMENTATION LINKS

- **TinyGPSPlus Guide**: http://arduiniana.org/libraries/tinygpsplus/
- **Firebase Docs**: https://github.com/mobizt/Firebase-ESP-Client/blob/main/README.md
- **ArduinoJson Guide**: https://arduinojson.org/v6/tutorial/

---

**Installation Complete! ✅**

Next steps:
1. Upload the ESP32 sketch (`esp32_gps_tracker.ino`)
2. Configure WiFi and Firebase credentials
3. Open web dashboard HTML file
4. Watch real-time GPS tracking in action!
