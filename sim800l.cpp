#include "sim800l.h"

void initSIM800L() {
  Serial2.println("AT+CLIP=1"); // Enable caller ID notification
  delay(200);
}

String checkIncomingCall() {
  if (Serial2.available()) {
    String response = Serial2.readString();
    if (response.indexOf("+CLIP:") != -1) {
      int start = response.indexOf("\"") + 1;
      int end = response.indexOf("\"", start);
      if (start > 0 && end > start) {
        return response.substring(start, end); // Caller number
      }
    }
  }
  return "";
}

void sendSMS(String number, String message) {
  Serial2.println("AT+CMGF=1"); // Text mode
  delay(100);
  Serial2.println("AT+CMGS=\"" + number + "\"");
  delay(100);
  Serial2.print(message);
  Serial2.write(26); // CTRL+Z to send
  delay(100);
}
