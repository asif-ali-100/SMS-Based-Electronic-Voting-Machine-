# ESP32 Firmware

PlatformIO project for the ESP32 firmware. Handles call detection, OTP verification, candidate selection, local vote storage, and syncing votes to the backend server.

## Folder Structure
```
esp32/
├── platformio.ini        # PlatformIO project config
├── src/                   # .cpp source files
│   ├── main.cpp
│   ├── sim800l.cpp
│   ├── dtmf_mt8870.cpp
│   └── storage.cpp
└── include/               # .h header files + config
    ├── sim800l.h
    ├── dtmf_mt8870.h
    ├── storage.h
    ├── config.h.example   # template — copy to config.h
    └── config.h           # your real values (gitignored, not committed)
```

## First-Time Setup
1. **Copy the config template** and fill in your real values:
   ```bash
   cp include/config.h.example include/config.h
   ```
   Edit `include/config.h` and set:
   - `WIFI_SSID` / `WIFI_PASSWORD` — network the ESP32 uses to reach the server
   - `SERVER_URL` — full URL to `server/vote.php` on your XAMPP machine
   - `REGISTERED_VOTER_1` / `REGISTERED_VOTER_2` — demo voter numbers

   `include/config.h` is in `.gitignore`, so your real WiFi credentials never get committed. Only `config.h.example` is tracked.

2. **Wire the hardware** (see table below).

3. **Build and upload:**
   - **VS Code + PlatformIO** (recommended): open the `esp32/` folder in VS Code, install the PlatformIO extension, then click Build → Upload in the PlatformIO toolbar (or run `pio run --target upload` in the terminal).
   - **Arduino IDE**: install the ESP32 board package, create a sketch, and copy the contents of `src/` and `include/` into it (Arduino IDE doesn't use the `src`/`include` split — flatten the files into one sketch folder).

4. Open the Serial Monitor at 115200 baud to see connection logs and vote confirmations.

## Hardware Wiring
| MT8870 Pin | ESP32 Pin |
|------------|-----------|
| Q1         | GPIO 32   |
| Q2         | GPIO 33   |
| Q3         | GPIO 25   |
| Q4         | GPIO 26   |
| STQ        | GPIO 27   |

| SIM800L Pin | ESP32 Pin      |
|-------------|----------------|
| TX          | RX2 (GPIO 16)  |
| RX          | TX2 (GPIO 17)  |
| GND         | GND            |
| VCC         | External 4V regulated supply (not the ESP32's 5V pin — SIM800L needs up to 2A peak) |

## What Each File Does
- `src/main.cpp` — Main loop: call detection → OTP generation/validation → candidate selection → local storage → server sync.
- `src/sim800l.cpp` — AT command handling for the SIM800L (caller ID, sending SMS).
- `src/dtmf_mt8870.cpp` — Reads DTMF tones (OTP digits and candidate number) using the MT8870's strobe (STQ) pin.
- `src/storage.cpp` — Stores vote counts in ESP32 Preferences (NVS) and appends a log line to SPIFFS.
- `include/config.h` — WiFi, server URL, and demo voter list (not committed).

## Notes
- `registeredVoters[]` in `main.cpp` is a placeholder for demo purposes. In a real deployment, validate voters against a secure server-side database, not a hardcoded array.
- If the ESP32 can't reach WiFi, votes are still stored locally (Preferences + SPIFFS) but won't sync to the server until connectivity is restored — you'd want to add a retry/sync-on-reconnect routine for production use.
