# 🌱 OffGrid Soil Monitor

> **Wireless, off-grid environmental monitoring** using LoRa radio — no WiFi, no cell service required.

An Arduino-based sensor node that reads **soil moisture, temperature, and humidity** and transmits the data over **long-range LoRa radio (915 MHz)** to a remote receiver. Built for remote fields, gardens, or any location without reliable connectivity.

---

## 📡 How It Works

```
[Sensor Node]                        [Receiver Node]
 Arduino Uno                          ESP32
 + DHT11 (temp/humidity)   ──LoRa──▶  + Serial Monitor
 + Soil Moisture Sensor                 (displays live readings)
 + RYLR LoRa Module
```

1. **Sensor node** (`finalsoilv1.ino`) reads soil moisture, temperature (°F), and humidity every hour
2. Packages data into a compact string: `T:72.5F H:58.3% S:42%`
3. Transmits via LoRa at 915 MHz to a paired receiver up to ~3 km away
4. **Receiver node** (`esp32lora.ino`) listens and prints readings to Serial

---

## 🛠️ Hardware

| Component | Role |
|-----------|------|
| Arduino Uno | Sensor node microcontroller |
| ESP32 | Receiver microcontroller |
| RYLR896 LoRa Module (×2) | Long-range radio (915 MHz) |
| DHT11 | Temperature & humidity sensor |
| Capacitive Soil Moisture Sensor | Soil moisture (analog) |

**Wiring diagram:** [`Drawing2.pdf`](./Drawing2.pdf)

---

## 📁 Files

| File | Description |
|------|-------------|
| `finalsoilv1.ino` | Sensor node — reads sensors, transmits over LoRa |
| `esp32lora.ino` | Receiver node — listens for LoRa packets, outputs to Serial |
| `Drawing2.pdf` | Circuit/wiring diagram (Autodesk) |

---

## ⚡ Setup

### 1. Install Dependencies
In the Arduino IDE, install via Library Manager:
- `DHT sensor library` by Adafruit
- `SoftwareSerial` (built-in)

### 2. Flash the Sensor Node
Open `finalsoilv1.ino` → upload to **Arduino Uno**

### 3. Flash the Receiver
Open `esp32lora.ino` → upload to **ESP32**

### 4. LoRa Configuration
Both modules are pre-configured with matching settings:

```
Band:       915 MHz (US)
Network ID: 18
SF/BW/CR:   9 / 7 / 1
Preamble:   12
```
> Sensor node = Address `1` → Receiver = Address `2`

### 5. Calibrate Soil Sensor *(optional)*
Adjust these values in `finalsoilv1.ino` to match your specific sensor:
```cpp
const int dryValue = 853;   // raw ADC reading in dry air
const int wetValue  = 350;  // raw ADC reading submerged in water
```

---

## 📊 Sample Output

```
Sent: T:74.3F H:55.0% S:67%
Sent: T:73.1F H:57.2% S:34%
```

---

## 🔧 Customization

- **Transmission interval** — change `delay(3600000)` in `finalsoilv1.ino` (currently 1 hour)
- **Multiple nodes** — assign unique `MY_ADDRESS` values to add more sensor nodes on the same network
- **Add more sensors** — the Arduino has unused analog pins for additional soil probes

---

## 📌 Notes

- Designed for the **US 915 MHz LoRa band** — change `AT+BAND` for EU (868 MHz) or other regions
- Power the sensor node with a small LiPo + solar panel for fully off-grid deployment
- Serial baud: `9600` on sensor node, `115200` on receiver
