#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>

void storeVote(String voter, int candidate);
void logVote(String voter, int candidate);

#endif
