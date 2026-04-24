# GPS Tracking System - Quick Reference Card

## 🔌 PIN CONNECTIONS

```
ESP32 (Front View)
┌─────────────────────┐
│  GND  3.3V  VIN    │
├─────────────────────┤
│ GPIO16 (TX2) ─────→ GPS RX (Green)
│ GPIO17 (RX2) ←───── GPS TX (Yellow)
│ 3.3V ────────────→ GPS VCC (Red)
│ GND ────────────→ GPS GND (Black)
└─────────────────────┘
```

## ✅ CREDENTIALS CHECKLIST

### WiFi Settings
- [ ] **SSID**: ________________
- [ ] **Password**: ________________

### Firebase Project
- [ ] **API Key**: ________________
- [ ] **Database URL**: https://___________-default-rtdb.firebaseio.com
- [ ] **Project ID**: ________________

### Lines to Update in esp32_gps_tracker.ino:
```
Line 23: #define WIFI_SSID "YOUR_SSID"
Line 24: #define WIFI_PASSWORD "YOUR_PASSWORD"
Line 27: #define API_KEY "YOUR_API_KEY"
Line 28: #define DATABASE_URL "YOUR_DB_URL"
```

### Lines to Update in gps_tracking_web.html:
```
Line 185: apiKey: "YOUR_API_KEY"
Line 186: authDomain: "PROJECT.firebaseapp.com"
Line 187: databaseURL: "https://PROJECT-default-rtdb.firebaseio.com"
Line 188: projectId: "YOUR_PROJECT_ID"
Line 189: storageBucket: "PROJECT.appspot.com"
Line 190: messagingSenderId: "YOUR_SENDER_ID"
Line 191: appId: "YOUR_APP_ID"
```

## 📋 REQUIRED ARDUINO LIBRARIES

1. ✓ **TinyGPSPlus** (by Mikal Hart)
2. ✓ **Firebase Arduino Client Library** (by Mobizt)
3. ✓ **ArduinoJson** (by Benoit Blanchon)

**Installation**: Sketch → Include Library → Manage Libraries → Search → Install

## 🔥 FIREBASE SETUP CHECKLIST

- [ ] Project created
- [ ] Realtime Database enabled
- [ ] Database security rules updated
- [ ] API Key obtained
- [ ] Database URL copied
- [ ] `/bus/live` path created

## 🌐 WEB FRONTEND CDN LINKS (Already Included)

```html
<!-- Leaflet CSS -->
https://cdnjs.cloudflare.com/ajax/libs/leaflet/1.9.4/leaflet.min.css

<!-- Leaflet JS -->
https://cdnjs.cloudflare.com/ajax/libs/leaflet/1.9.4/leaflet.min.js

<!-- Firebase v9 compat SDK -->
https://www.gstatic.com/firebaseapps/9.23.0/firebase-app-compat.min.js
https://www.gstatic.com/firebaseapps/9.23.0/firebase-database-compat.min.js
```

## 🔧 ARDUINO IDE BOARD SETTINGS

```
Board: ESP32-DevKit C
Upload Speed: 921600
CPU Frequency: 240 MHz
Flash Size: 4MB
Partition Scheme: Default 4MB
```

## 📡 GPS MODULE SPECIFICATIONS

- **Model**: NEO-6M (u-blox)
- **Baud Rate**: 9600
- **Update Rate**: 1 Hz (can be modified)
- **Accuracy**: 2.5m
- **Satellites**: Up to 22 tracked
- **Warm Start**: ~1 second
- **Cold Start**: 30-60 seconds
- **Hot Start**: ~1 second

## 📊 DATA UPLOADED TO FIREBASE

```json
{
  "latitude": 17.3606,
  "longitude": 78.4747,
  "altitude": 505.2,
  "speed_kmh": 45.67,
  "course": 123.45,
  "satellites": 12,
  "hdop": 1.5,
  "date": "2024-01-15",
  "time": "14:30:45",
  "timestamp": 1705335045
}
```

## 🚀 STARTUP SEQUENCE

```
1. Press RST on ESP32
2. Wait 2 seconds (power-up)
3. Check Serial Monitor (115200 baud)
4. Should see: WiFi connecting → Firebase config → GPS ready
5. Place GPS module outdoors
6. Wait 1-2 minutes for GPS lock
7. First data upload within 3 seconds of valid fix
8. Open web dashboard HTML file
9. Watch marker appear and update in real-time
```

## ⏱️ TIMING REFERENCE

| Event | Time |
|-------|------|
| ESP32 Boot | ~2-3 seconds |
| WiFi Connection | ~5-10 seconds |
| Firebase Authentication | ~2-3 seconds |
| GPS Cold Start (first fix) | 30-60 seconds |
| GPS Warm Start | ~5-10 seconds |
| Data Upload Interval | 3 seconds |
| Web Map Update | Real-time (< 1 second) |

## 🔍 MONITORING COMMANDS

**Check Firebase Data (Console)**:
- Database → Data → Expand `/bus` → View `/bus/live`
- Refresh every 3 seconds to see new data

**Monitor ESP32 Serial Output**:
- Should see "DATA UPLOADED SUCCESSFULLY" every 3 seconds
- Check "✓ WiFi: CONNECTED", "✓ Firebase: CONNECTED", "✓ GPS: VALID"

**Check Web Dashboard**:
- F12 → Console → Should show no errors
- Status badges show green (connected)
- Marker moves on map

## 🆘 QUICK TROUBLESHOOTING

| Problem | Solution |
|---------|----------|
| No Serial Output | Check USB cable, install CP210x drivers |
| GPS No Signal | Move outdoors, wait 2+ minutes |
| WiFi Won't Connect | Check SSID/password exactly |
| Firebase Error | Verify API_KEY and DATABASE_URL |
| Web Dashboard Blank | Check Firebase config, clear cache |
| Marker Not Moving | Refresh page, check GPS has new location |

## 📱 BROWSER REQUIREMENTS

- Modern browser (Chrome, Firefox, Edge, Safari)
- JavaScript enabled
- Location services **not required**
- No VPN needed for Firebase (unless blocked by network)

## 🔐 SECURITY NOTES

⚠️ **Development Settings** (current):
- Database allows open read/write
- No authentication required
- Suitable for testing only

✓ **For Production**:
- Implement authentication
- Restrict write access to ESP32 device
- Encrypt sensitive data
- Use HTTPS for web frontend

## 📞 CONTACT & SUPPORT

- ESP32 Issues: https://github.com/espressif/arduino-esp32
- GPS Module: NEO-6M u-blox documentation
- Firebase Support: https://firebase.google.com/support
- Leaflet Issues: https://github.com/Leaflet/Leaflet

---

**Last Updated**: January 2025
**Compatibility**: ESP32, Arduino IDE 2.0+, Firebase Realtime Database
**Tested On**: Windows, macOS, Linux
