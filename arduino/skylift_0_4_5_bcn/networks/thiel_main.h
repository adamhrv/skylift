/*
 Scan type: wigle
 Networks: 10
 Target lat, lon: 20.624365, -156.440561
 Comment: Thiel’s estate
 Generated: Oct 27, 2018 12:38:10
 (open + close this sketch to reload changes)
*/

// Copy and paste this to the networks.h file
/*
#include "networks/thiel_main.h"
extern const byte NN_NN_THIEL_MAIN;
extern char* ssids_thiel_main[];
extern byte bssids_[][6];
extern byte rssis_[]
extern byte channels_thiel_main[];
extern String name_thiel_main;
extern String city_thiel_main;
*/

// Number of networks
const byte NN_THIEL_MAIN = 10;

// Name and location for OLED
String name_thiel_main = "Peter Thiel";
String city_thiel_main = "Thiel's Estate, HI";

// SSIDs for display on OLED
char* ssids_thiel_main[NN_THIEL_MAIN] = {
	"Ruckus",
	"Makena",
	"Makena",
	"Emeral",
	"Nani M",
	"Nani M",
	"Mauivi",
	"HP-Pri",
	"Makena",
	"HT129"
};

// BSSIDs (MAC addresses)
byte bssids_thiel_main[NN_THIEL_MAIN][6] = {
	{0xF0, 0xB0, 0x52, 0x12, 0x8F, 0x28},
	{0x2C, 0xC5, 0xD3, 0x0B, 0xDC, 0x58},
	{0x58, 0xB6, 0x33, 0x03, 0x9C, 0x88},
	{0x2C, 0xB0, 0x5D, 0x29, 0x6E, 0x5E},
	{0x00, 0x26, 0xEC, 0x00, 0x93, 0x90},
	{0x00, 0x02, 0x6F, 0xE9, 0x8E, 0x96},
	{0x14, 0xDD, 0xA9, 0x94, 0x8B, 0x10},
	{0x74, 0x46, 0xA0, 0xDB, 0xE4, 0x6D},
	{0x90, 0x72, 0x40, 0x14, 0xED, 0x28},
	{0xB8, 0xE6, 0x25, 0x56, 0x5B, 0x52}
};

// RSSIs, experimental
byte rssis_thiel_main[NN_THIEL_MAIN] = {
20, 19, 19, 19, 19, 19, 17, 17, 17, 17
};

byte channels_thiel_main[NN_THIEL_MAIN] = {
 1, 2, 3, 4, 5, 6, 7, 8, 9, 11  
};