# College Bus GPS Tracking System - Complete Setup Guide

## 📋 Project Overview

This is a **production-ready** real-time GPS tracking system for college buses using:
- **ESP32** microcontroller with NEO-6M GPS module
- **Firebase Realtime Database** for cloud storage
- **Leaflet.js + OpenStreetMap** for web-based tracking
- **Zero authentication** setup (API Key only)

**Key Features:**
- Real-time GPS tracking every 3 seconds
- Automatic marker updates on web map
- Detailed GPS metrics (altitude, speed, satellites, HDOP)
- Responsive web interface
- Zero downtime with graceful error handling

---

## 🔧 HARDWARE SETUP

### Components Required:
1. **ESP32 Development Board** (any variant)
2. **NEO-6M GPS Module** (u-blox)
3. **USB Cable** (for programming)
4. **Jumper Wires** (male-to-male and male-to-female)
5. **Power Supply** (5V for GPS module)

### Pin Connections:

| Component | ESP32 Pin | Wire Color |
|-----------|-----------|-----------|
| GPS TX | GPIO17 (RX2) | Yellow |
| GPS RX | GPIO16 (TX2) | Green |
| GPS GND | GND | Black |
| GPS VCC | 3.3V | Red |

**Connection Diagram:**
```
NEO-6M GPS Module
├── VCC → ESP32 3.3V (or 5V with resistor divider for RX)
├── GND → ESP32 GND
├── TX → ESP32 GPIO17 (UART2 RX)
└── RX → ESP32 GPIO16 (UART2 TX)
```

### Hardware Notes:
- NEO-6M operates at 3.3V (check your module voltage)
- Use 220Ω-470Ω resistors in series on GPS RX line if using 5V
- For accurate GPS: Mount antenna outdoors, away from metal
- Initialization time: 30-60 seconds for first fix

---

## 💻 Arduino IDE SETUP

### 1. Install Arduino IDE
- Download from: https://www.arduino.cc/en/software
- Install and launch

### 2. Add ESP32 Board Support
1. **File** → **Preferences**
2. In "Additional Boards Manager URLs", add:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
3. Click OK
4. **Tools** → **Board** → **Boards Manager**
5. Search: `ESP32`
6. Install "ESP32 by Espressif Systems" (latest version)
7. Select **Tools** → **Board** → **ESP32** → **ESP32-DevKit C**

### 3. Install Required Libraries

Open **Sketch** → **Include Library** → **Manage Libraries**, then search and install:

#### Required Libraries:

1. **TinyGPSPlus by Mikal Hart**
   - Search: `TinyGPSPlus`
   - Click "Install"
   - Version: Latest

2. **Firebase Arduino Client Library for ESP32 by Mobizt**
   - Search: `Firebase Arduino Client Library`
   - Click "Install"
   - Version: Latest (v4.x or higher)

3. **ArduinoJson by Benoit Blanchon**
   - Search: `ArduinoJson`
   - Click "Install"
   - Version: Latest (v6.x or higher)

**Verification:**
```cpp
// Test in Arduino IDE: Sketch → Verify/Compile
// Should compile without errors
```

---

## 🔥 FIREBASE SETUP

### 1. Create Firebase Project
1. Go to: https://console.firebase.google.com
2. Click **"Create a project"** or select existing
3. Enter project name: `college-bus-tracker`
4. Skip Google Analytics
5. Click **"Create project"** and wait for completion

### 2. Enable Realtime Database
1. In Firebase Console, navigate to **Realtime Database**
2. Click **"Create Database"**
3. Choose location: Select nearest to your region
4. Start in **"Test Mode"** (for development)
5. Click **"Enable"**

**Database URL** (copy this):
```
https://YOUR_PROJECT-default-rtdb.firebaseio.com
```

### 3. Get API Key
1. Go to **Project Settings** (gear icon)
2. Click **"Service Accounts"** tab
3. You'll see "API keys" section
4. Use the **Web API Key** from your Firebase config
   - OR in Settings → General tab, find "Web API Key"

**Your API Key** will look like:
```
AIzaSyD...your_actual_api_key_here...
```

### 4. Set Database Security Rules
1. In **Realtime Database**, click **"Rules"** tab
2. Replace everything with:

```json
{
  "rules": {
    "bus": {
      "live": {
        ".read": true,
        ".write": true,
        ".validate": true
      }
    }
  }
}
```

3. Click **"Publish"**

⚠️ **IMPORTANT:** These rules allow anyone to read/write. For production:
```json
{
  "rules": {
    "bus": {
      "live": {
        ".read": true,
        ".write": false,
        "latitude": { ".validate": "isNumber()" },
        "longitude": { ".validate": "isNumber()" }
      }
    }
  }
}
```

---

## 📱 ESP32 ARDUINO SKETCH SETUP

### 1. Open the Sketch
1. Copy the full code: `esp32_gps_tracker.ino`
2. Create new sketch in Arduino IDE
3. Paste the entire code
4. Save as: `esp32_gps_tracker.ino`

### 2. Configure Your Credentials (Lines 23-30)

Find and replace these values:

```cpp
// Line 23 - WiFi SSID
#define WIFI_SSID "YOUR_WIFI_SSID"

// Line 24 - WiFi Password  
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Line 27 - Firebase API Key
#define API_KEY "AIzaSyD...your_actual_key..."

// Line 28 - Firebase Database URL
#define DATABASE_URL "https://YOUR_PROJECT-default-rtdb.firebaseio.com"
```

### 3. Upload to ESP32
1. **Tools** → **Port** → Select ESP32 COM port
2. **Tools** → **Board** → **ESP32-DevKit C**
3. **Tools** → **Upload Speed** → `921600` (for faster upload)
4. **Sketch** → **Upload** (or press Ctrl+U)
5. Wait for "Hard resetting via RTS pin" message

### 4. Verify Upload
1. **Tools** → **Serial Monitor**
2. Set baud rate to **115200**
3. Reset ESP32 (press RST button)
4. You should see startup messages

**Expected Serial Output:**
```
╔════════════════════════════════════════╗
║   ESP32 GPS Tracker - Firebase v1.0   ║
╚════════════════════════════════════════╝

[INIT] Initializing GPS Serial on UART2...
[OK] GPS Serial initialized at 9600 baud
[INFO] GPS RX Pin: 17, TX Pin: 16

[INIT] Connecting to WiFi...
[WIFI] Connecting to: YOUR_WIFI_SSID
........
[OK] WiFi connected successfully!
[WIFI] SSID: YOUR_WIFI_SSID
...
```

---

## 🌐 WEB FRONTEND SETUP

### 1. Configure Firebase Credentials
1. Open: `gps_tracking_web.html` in text editor
2. Find lines 179-190:

```javascript
const firebaseConfig = {
    apiKey: "YOUR_API_KEY",
    authDomain: "YOUR_PROJECT.firebaseapp.com",
    databaseURL: "https://YOUR_PROJECT-default-rtdb.firebaseio.com",
    projectId: "YOUR_PROJECT_ID",
    storageBucket: "YOUR_PROJECT.appspot.com",
    messagingSenderId: "YOUR_MESSAGING_SENDER_ID",
    appId: "YOUR_APP_ID"
};
```

3. Replace with your Firebase config from: https://console.firebase.google.com
   - **Settings** → **Project Settings** → **General** tab
   - Scroll to "Your apps" section
   - Find/create "Web" app
   - Copy the Firebase config object

### 2. Open in Browser
1. Save the HTML file locally
2. Double-click to open in browser (Chrome recommended)
3. Or drag & drop into browser window

### 3. Test the Connection
1. Open **Browser DevTools** (F12)
2. Click **Console** tab
3. You should see:
   ```
   Firebase initialized successfully
   Map initialized
   Firebase listener setup complete
   ```

4. Check map loads (shows world map with OpenStreetMap tiles)

---

## ✅ SYSTEM TESTING CHECKLIST

### Phase 1: Hardware & Serial
- [ ] ESP32 uploads successfully
- [ ] Serial Monitor shows startup messages
- [ ] WiFi connects (SSID appears in output)
- [ ] GPS module is powered (red LED on)
- [ ] Serial shows "Initializing GPS Serial" messages

### Phase 2: GPS Signal
- [ ] Place GPS module outdoors or near window
- [ ] Wait 1-2 minutes for initial fix
- [ ] Serial shows increasing satellite count
- [ ] Eventually: "GPS: VALID" appears in status
- [ ] Latitude/Longitude values appear (non-zero)

### Phase 3: Firebase Upload
- [ ] WiFi: CONNECTED status
- [ ] Firebase: CONNECTED status
- [ ] First upload: "DATA UPLOADED SUCCESSFULLY"
- [ ] Data appears every 3 seconds
- [ ] Upload count increments

### Phase 4: Web Dashboard
- [ ] Open HTML file in browser
- [ ] Firebase status shows: Connected
- [ ] GPS status shows: Receiving
- [ ] Map loads without errors
- [ ] Bus marker appears on map
- [ ] Marker updates in real-time (every 3 seconds)
- [ ] Popup shows GPS data
- [ ] Sidebar shows current values

---

## 🐛 TROUBLESHOOTING

### Issue: ESP32 Won't Upload
**Solution:**
1. Check USB cable (use high-quality cable)
2. Install CP210x drivers: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
3. Restart Arduino IDE
4. Try upload speed: 115200

### Issue: GPS Module Not Getting Signal
**Solution:**
1. Ensure antenna is outdoors or near window
2. Wait 2-3 minutes minimum (first fix is slow)
3. Check GPS TX/RX connections
4. Monitor satellites count: should increase over time
5. Cold start can take up to 5 minutes

### Issue: WiFi Connects but Firebase Fails
**Solution:**
1. Verify WiFi has internet access
2. Check API_KEY in code is exactly correct
3. Verify DATABASE_URL format (must end with `/`)
4. Check Firebase Rules allow reads/writes:
   ```json
   ".read": true,
   ".write": true
   ```
5. Restart ESP32

### Issue: Web Dashboard Won't Connect
**Solution:**
1. Open Browser DevTools (F12) → Console
2. Check for JavaScript errors
3. Verify Firebase config in HTML file
4. Ensure authDomain is correct format
5. Test with different browser
6. Clear browser cache (Ctrl+Shift+Del)

### Issue: Firebase Data Not Appearing in Console
**Solution:**
1. Check `/bus/live` path exists in Firebase
2. ESP32 Serial Monitor should show "DATA UPLOADED SUCCESSFULLY"
3. In Firebase Console → Realtime Database, you should see `/bus` folder
4. If missing, manually create structure:
   - Click "+" next to root
   - Key: `bus`
   - Value: empty
   - Confirm

### Issue: Marker Not Moving
**Solution:**
1. Verify GPS data has new coordinates
2. Check browser console for Firebase errors
3. Ensure Leaflet.js loaded (check Sources tab)
4. Try refreshing page
5. Check Firebase real-time listener is active

---

## 📊 MONITORING & LOGGING

### ESP32 Serial Monitor Output
The system provides comprehensive logging:

```
✓ WiFi: CONNECTED
  └─ Signal: -45 dBm | IP: 192.168.1.100

✓ Firebase: CONNECTED

✓ GPS: VALID
  └─ Location: 17.360589, 78.474019

⚙ Uploads: 42 successful | 0 failed

⏱ Uptime: 0h 2m 35s
```

### Key Status Indicators:
- ✓ Green indicator = System ready
- ✗ Red indicator = Connection issue
- ⚠ Yellow indicator = Waiting for data
- ⚙ Configuration/metrics display

### Firebase Console Monitoring
1. Go to **Realtime Database**
2. Click **"Data"** tab
3. Expand `/bus/live` folder
4. Watch values update in real-time
5. Check `timestamp` field shows current Unix time

---

## 🚀 PRODUCTION DEPLOYMENT

### For Actual Bus Deployment:

1. **Secure Firebase**
   ```json
   {
     "rules": {
       "bus": {
         "live": {
           ".read": true,
           ".write": true,
           "latitude": { ".validate": "isNumber()" },
           "longitude": { ".validate": "isNumber()" },
           "speed_kmh": { ".validate": "isNumber()" },
           "satellites": { ".validate": "isNumber()" }
         }
       }
     }
   }
   ```

2. **Add Device Identification**
   ```cpp
   String busID = "BUS_001"; // Add to uploads
   json.set("busId", busID);
   ```

3. **Store Historical Data**
   ```cpp
   // Archive to: /bus/history/{date}/{timestamp}
   String historyPath = "/bus/history/" + dateStr + "/" + String(millis());
   Firebase.RTDB.setJSON(&fbdo, historyPath, &json);
   ```

4. **Web Dashboard Improvements**
   - Add multiple bus tracking
   - Add route history playback
   - Add geofencing alerts
   - Add speed warnings

5. **Deploy Web Frontend**
   - Host on Firebase Hosting, GitHub Pages, or Vercel
   - Use HTTPS only
   - Add SSL certificate

---

## 📝 CODE CUSTOMIZATION

### Modify Upload Interval
Edit line 38:
```cpp
#define UPLOAD_INTERVAL 3000  // milliseconds (3 seconds)
// Change to 5000 for 5 seconds, 10000 for 10 seconds, etc.
```

### Change Firebase Path
Edit line 32:
```cpp
#define FIREBASE_PATH "/bus/live"
// Change to "/bus/tracker1", "/vehicles/bus001", etc.
```

### Add More GPS Data Points
In `uploadToFirebase()` function:
```cpp
// Add battery voltage
json.set("battery_voltage", readBattery());

// Add internal temperature
json.set("chip_temp", temperatureRead());

// Add custom metrics
json.set("driver_id", "DRV_001");
```

### Adjust Serial Output Frequency
Edit line 46:
```cpp
unsigned long serialUpdateInterval = 1000; // milliseconds
// Change to 5000 for less frequent output (quieter)
```

---

## 📚 USEFUL RESOURCES

- **ESP32 Documentation**: https://docs.espressif.com/projects/esp-idf/
- **TinyGPSPlus Examples**: http://arduiniana.org/libraries/tinygpsplus/
- **Firebase Docs**: https://firebase.google.com/docs/database
- **Leaflet.js Documentation**: https://leafletjs.com/
- **OpenStreetMap Project**: https://www.openstreetmap.org/

---

## 🎯 QUICK START SUMMARY

```
1. Hardware: Connect GPS to ESP32 (GPIO17, GPIO16)
2. Arduino: Install libraries + upload sketch with your WiFi/Firebase details
3. Firebase: Create project + get API Key + set Database Rules
4. Web: Open HTML file, update Firebase config
5. Test: Watch Serial Monitor + Web Dashboard for real-time updates
```

**Expected Time to Full Operation: 15-20 minutes**

---

## 📞 SUPPORT

If issues persist:
1. Check all Serial Monitor output carefully
2. Verify every credential is exactly correct (copy-paste)
3. Ensure GPS module antenna is outdoors
4. Check browser console for JavaScript errors
5. Restart both ESP32 and web dashboard
6. Clear browser cache if web dashboard doesn't update

---

**Happy Tracking! 🚌📍**
