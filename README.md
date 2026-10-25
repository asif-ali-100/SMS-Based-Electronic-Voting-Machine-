# Secure Electronic Voting Machine (Prototype)

A call-based voting machine prototype built with an ESP32, a GSM module, and a DTMF decoder. Voters authenticate via OTP over a phone call, then cast their vote by pressing a candidate number on the keypad. Votes are stored on-device and synced to a web dashboard.

## Components
| Part | Role |
|------|------|
| **ESP32** | Main controller, local vote storage, WiFi sync to server |
| **SIM800L** | GSM module — call detection (caller ID) and sending OTP/confirmation SMS |
| **MT8870** | DTMF decoder — reads keypad tones during the call (OTP digits + candidate selection) |
| **XAMPP (PHP + MySQL)** | Backend server storing votes and displaying live results |

## Workflow
1. Voter calls from their registered mobile number.
2. System verifies caller ID against the registered voter list.
3. An OTP is generated and sent via SMS.
4. Voter dials the OTP digits during the call; the MT8870 decodes them.
5. Once the OTP is validated, the voter presses 1, 2, 3, or 4 to select a candidate.
6. ESP32 records the vote locally (Preferences + SPIFFS) **and** POSTs it to the server over WiFi.
7. Voter receives a confirmation SMS; the admin dashboard (`results.php`) shows live results.

## Repository Structure
```
SecureVotingMachine/
├── esp32/                     # PlatformIO project — ESP32 firmware
│   ├── platformio.ini
│   ├── src/                   # main.cpp, sim800l.cpp, dtmf_mt8870.cpp, storage.cpp
│   ├── include/                # headers + config.h.example
│   └── README.md
├── server/                    # XAMPP PHP + MySQL backend
│   ├── db.php, vote.php, results.php
│   ├── config.example.php
│   ├── sql_schema.sql
│   └── README.md
├── LICENSE
├── .gitignore
└── README.md                  # you are here
```

## Prerequisites
- [VS Code](https://code.visualstudio.com/) with the [PlatformIO extension](https://platformio.org/install/ide?install=vscode) (or Arduino IDE)
- [XAMPP](https://www.apachefriends.org/) (Apache + MySQL + PHP)
- ESP32 dev board, SIM800L GSM module, MT8870 DTMF decoder, a SIM card with calling/SMS enabled
- Git

## Quick Start
```bash
git clone https://github.com/<your-username>/SecureVotingMachine.git
cd SecureVotingMachine

# 1. ESP32 config
cp esp32/include/config.h.example esp32/include/config.h
# edit esp32/include/config.h with your WiFi + server URL

# 2. Server config
cp server/config.example.php server/config.php
# edit server/config.php with your MySQL credentials (defaults usually work for XAMPP)
```
Then follow `esp32/README.md` to wire and flash the hardware, and `server/README.md` to set up XAMPP and import the schema.

## Security Features
- Caller ID verification (only registered numbers can vote)
- OTP with timeout (2 minutes) and retry limit (3 attempts)
- Dual storage of votes (ESP32 flash + MySQL server) for redundancy
- Prepared SQL statements to prevent injection on the server side
- Secrets (WiFi credentials, DB password) kept out of version control via `.gitignore`

## Limitations (Prototype, Not Production)
- GSM caller ID and SMS can potentially be spoofed; this is not a substitute for cryptographic authentication.
- ESP32 → server traffic is plain HTTP, not HTTPS/TLS.
- No independent audit trail or paper trail, which real election systems require.
- Not designed to scale to large numbers of concurrent voters.
- The registered-voter list is a hardcoded array for demo purposes, not a real database.

## Future Improvements
- Add TLS encryption for ESP32 → server communication.
- Move voter registry and OTP verification to the server side with proper hashing.
- Add an audit log / paper-trail mechanism.
- Add retry/queueing so votes sync automatically once WiFi reconnects.
- Load-test and redesign for large-scale concurrent use.

## License
[MIT](LICENSE)
