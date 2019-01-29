/*
 Scan type: wigle
 Networks: 2
 Target lat, lon: 20.633785999999997, -156.444242
 Comment: Makena Beach, taco truck
 Generated: Oct 27, 2018 12:38:10
 (open + close this sketch to reload changes)
*/

// Copy and paste this to the networks.h file
/*
#include "networks/thiel_makena_beach.h"
extern const byte NN_NN_THIEL_MAKENA_BEACH;
extern char* ssids_thiel_makena_beach[];
extern byte bssids_[][6];
extern byte rssis_[]
extern byte channels_thiel_makena_beach[];
extern String name_thiel_makena_beach;
extern String city_thiel_makena_beach;
*/

// Number of networks
const byte NN_THIEL_MAKENA_BEACH = 2;

// Name and location for OLED
String name_thiel_makena_beach = "Peter Thiel (N++)";
String city_thiel_makena_beach = "Makena Beach, HI";

// SSIDs for display on OLED
char* ssids_thiel_makena_beach[NN_THIEL_MAKENA_BEACH] = {
	"WiFi H",
	"Barnha"
};

// BSSIDs (MAC addresses)
byte bssids_thiel_makena_beach[NN_THIEL_MAKENA_BEACH][6] = {
	{0xF0, 0xAB, 0x54, 0x29, 0x41, 0x45},
	{0xD4, 0x68, 0x4D, 0x0D, 0x51, 0x38}
};

// RSSIs, experimental
byte rssis_thiel_makena_beach[NN_THIEL_MAKENA_BEACH] = {
20, 18
};

byte channels_thiel_makena_beach[NN_THIEL_MAKENA_BEACH] = {
 1, 11  
};