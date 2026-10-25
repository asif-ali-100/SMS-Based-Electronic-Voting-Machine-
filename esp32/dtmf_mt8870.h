#ifndef DTMF_MT8870_H
#define DTMF_MT8870_H

#include <Arduino.h>

void initDTMF();
int readDTMF();
String readDTMFSequence();

#endif
