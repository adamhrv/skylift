/*
 Scan type: wigle
 Networks: 5
 Target lat, lon: 20.625255, -156.441283
 Comment: 
 Generated: Oct 26, 2018 13:49:10
 (open + close this sketch to reload changes)
*/

// Copy and paste this to the networks.h file
/*
#include "networks/thiel_central.h"
extern const byte NN_NN_THIEL_CENTRAL;
extern char* ssids_thiel_central[];
extern byte bssids_[][6];
extern byte rssis_[]
extern byte channels_thiel_central[];
extern String name_thiel_central;
extern String city_thiel_central;
*/

// Number of networks
const byte NN_THIEL_CENTRAL = 5;

// Name and location for OLED
String name_thiel_central = "Peter Thiel (C)";
String city_thiel_central = "Cryptorave, Hawaii";

// SSIDs for display on OLED
char* ssids_thiel_central[NN_THIEL_CENTRAL] = {
	"Makena",
	"Ruckus",
	"Makena",
	"Emeral",
	"Nani M"
};

// BSSIDs (MAC addresses)
byte bssids_thiel_central[NN_THIEL_CENTRAL][6] = {
	{0x2C, 0xC5, 0xD3, 0x0B, 0xDC, 0x58},
	{0xF0, 0xB0, 0x52, 0x12, 0x8F, 0x28},
	{0x58, 0xB6, 0x33, 0x03, 0x9C, 0x88},
	{0x2C, 0xB0, 0x5D, 0x29, 0x6E, 0x5E},
	{0x00, 0x26, 0xEC, 0x00, 0x93, 0x90}
};

// RSSIs, experimental
byte rssis_thiel_central[NN_THIEL_CENTRAL] = {
20, 20, 19, 19, 19
};

byte channels_thiel_central[NN_THIEL_CENTRAL] = {
 1, 3, 6, 8, 11  
};