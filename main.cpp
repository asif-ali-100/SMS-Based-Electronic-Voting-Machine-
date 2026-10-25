#include <Preferences.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "FS.h"
#include "SPIFFS.h"
#include "sim800l.h"
#include "dtmf_mt8870.h"
#include "storage.h"
#include "config.h" // copy config.h.example to config.h and fill in your values

Preferences prefs;
String currentOTP = "";
String currentVoter = "";
unsigned long otpTimestamp = 0;
const unsigned long otpTimeout = 120000; // 2 minutes
int retryCount = 0;
const int maxRetries = 3;

// Simple in-memory list of registered voter numbers (replace with real DB lookup)
String registeredVoters[] = {REGISTERED_VOTER_1, REGISTERED_VOTER_2};
const int totalRegistered = 2;

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected: " + WiFi.localIP().toString());
  } else {
    Serial.println("\nWiFi connection failed. Votes will only be stored locally.");
  }
}

// Sends the vote to the PHP backend (server/vote.php)
bool sendVoteToServer(String voter, int candidate) {
  if (WiFi.status() != WL_CONNECTED) return false;

  HTTPClient http;
  http.begin(SERVER_URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String postData = "mobile=" + voter + "&candidate=" + String(candidate);
  int httpCode = http.POST(postData);

  bool success = (httpCode == 200);
  if (success) {
    Serial.println("Vote synced to server: " + http.getString());
  } else {
    Serial.println("Server sync failed, HTTP code: " + String(httpCode));
  }
  http.end();
  return success;
}

bool isRegisteredVoter(String number) {
  for (int i = 0; i < totalRegistered; i++) {
    if (registeredVoters[i] == number) return true;
  }
  return false;
}

String generateOTP() {
  int otp = random(1000, 9999);
  return String(otp);
}

bool verifyOTP(String entered, String actual, unsigned long timestamp, int &retries, int maxR, unsigned long timeout) {
  if (millis() - timestamp > timeout) {
    return false; // expired
  }
  if (entered == actual) {
    return true;
  }
  retries++;
  return false;
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600); // SIM800L UART
  SPIFFS.begin(true);
  prefs.begin("votes", false);
  randomSeed(analogRead(0));
  initSIM800L();
  initDTMF();
  connectWiFi();
}

void loop() {
  // Detect incoming call
  String caller = checkIncomingCall();
  if (caller != "") {
    if (isRegisteredVoter(caller)) {
      currentVoter = caller;
      currentOTP = generateOTP();
      otpTimestamp = millis();
      retryCount = 0;
      sendSMS(caller, "Your OTP is " + currentOTP + ". Dial this code during the call.");
    }
  }

  // Read OTP via DTMF
  String enteredOTP = readDTMFSequence();
  if (enteredOTP.length() > 0) {
    if (verifyOTP(enteredOTP, currentOTP, otpTimestamp, retryCount, maxRetries, otpTimeout)) {
      Serial.println("OTP verified. Awaiting candidate selection...");
      int candidate = readDTMF(); // Candidate number (1-4)
      if (candidate >= 1 && candidate <= 4) {
        storeVote(currentVoter, candidate);
        logVote(currentVoter, candidate);
        sendVoteToServer(currentVoter, candidate);
        sendSMS(currentVoter, "Your vote for Candidate " + String(candidate) + " has been recorded.");
      } else {
        sendSMS(currentVoter, "Invalid candidate selection. Vote not recorded.");
      }
    } else {
      sendSMS(currentVoter, "OTP invalid or expired. Vote not recorded.");
    }
  }
}
