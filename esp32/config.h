#ifndef CONFIG_H
#define CONFIG_H

// Copy this file to config.h and fill in your real values.
// config.h is gitignored so your secrets never get committed.

// WiFi credentials (used to POST votes to the server)
#define WIFI_SSID     "your-wifi-name"
#define WIFI_PASSWORD "your-wifi-password"

// Server endpoint that receives votes (see server/vote.php)
#define SERVER_URL "http://192.168.1.100/SecureVotingMachine/server/vote.php"

// Registered voter numbers (replace with a real lookup in production)
#define REGISTERED_VOTER_1 "+911234567890"
#define REGISTERED_VOTER_2 "+919876543210"

#endif
