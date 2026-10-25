#include "dtmf_mt8870.h"

// MT8870 output pins (Q1-Q4) connected to ESP32 GPIOs
#define DTMF_Q1 32
#define DTMF_Q2 33
#define DTMF_Q3 25
#define DTMF_Q4 26
#define DTMF_STQ 27 // Data valid / strobe pin

void initDTMF() {
  pinMode(DTMF_Q1, INPUT);
  pinMode(DTMF_Q2, INPUT);
  pinMode(DTMF_Q3, INPUT);
  pinMode(DTMF_Q4, INPUT);
  pinMode(DTMF_STQ, INPUT);
}

int readDTMF() {
  // Wait for the STQ pin to go high, indicating a valid digit is ready
  if (digitalRead(DTMF_STQ) == HIGH) {
    int d0 = digitalRead(DTMF_Q1);
    int d1 = digitalRead(DTMF_Q2);
    int d2 = digitalRead(DTMF_Q3);
    int d3 = digitalRead(DTMF_Q4);
    int digit = (d3 << 3) | (d2 << 2) | (d1 << 1) | d0;
    delay(300); // debounce, avoid re-reading same tone
    return digit;
  }
  return -1;
}

String readDTMFSequence() {
  String seq = "";
  unsigned long lastDigitTime = millis();

  while (millis() - lastDigitTime < 3000) { // 3s pause ends sequence
    int digit = readDTMF();
    if (digit >= 0 && digit <= 9) {
      seq += String(digit);
      lastDigitTime = millis();
    }
  }
  return seq;
}
