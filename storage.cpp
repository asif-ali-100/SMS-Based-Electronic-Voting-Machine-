#include "storage.h"
#include <Preferences.h>
#include "FS.h"
#include "SPIFFS.h"

extern Preferences prefs;

void storeVote(String voter, int candidate) {
  prefs.begin("votes", false);
  String key = "cand" + String(candidate);
  int count = prefs.getInt(key.c_str(), 0);
  prefs.putInt(key.c_str(), count + 1);
  prefs.end();
}

void logVote(String voter, int candidate) {
  if (!SPIFFS.begin(true)) return;
  File file = SPIFFS.open("/votes.txt", FILE_APPEND);
  if (file) {
    file.println("Mobile: " + voter + ", Candidate: " + String(candidate) + ", Time: " + String(millis()));
    file.close();
  }
}
