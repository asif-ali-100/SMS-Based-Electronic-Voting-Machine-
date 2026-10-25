#ifndef SIM800L_H
#define SIM800L_H

#include <Arduino.h>

void initSIM800L();
String checkIncomingCall();
void sendSMS(String number, String message);

#endif
