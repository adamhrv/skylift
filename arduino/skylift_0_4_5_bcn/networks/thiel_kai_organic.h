/*
 Scan type: wigle
 Networks: 8
 Target lat, lon: 20.620082, -156.438715
 Comment: by great snorkeling spot
 Generated: Oct 27, 2018 12:38:10
 (open + close this sketch to reload changes)
*/

// Copy and paste this to the networks.h file
/*
#include "networks/thiel_kai_organic.h"
extern const byte NN_NN_THIEL_KAI_ORGANIC;
extern char* ssids_thiel_kai_organic[];
extern byte bssids_[][6];
extern byte rssis_[]
extern byte channels_thiel_kai_organic[];
extern String name_thiel_kai_organic;
extern String city_thiel_kai_organic;
*/

// Number of networks
const byte NN_THIEL_KAI_ORGANIC = 8;

// Name and location for OLED
String name_thiel_kai_organic = "Peter Thiel (S)";
String city_thiel_kai_organic = "Kai Oganic, HI";

// SSIDs for display on OLED
char* ssids_thiel_kai_organic[NN_THIEL_KAI_ORGANIC] = {
	"Makena",
	"makena",
	"WIFI-1",
	"MAUI H",
	"Mauivi",
	"HT272",
	"Lorell",
	"HT580"
};

// BSSIDs (MAC addresses)
byte bssids_thiel_kai_organic[NN_THIEL_KAI_ORGANIC][6] = {
	{0xC0, 0xEA, 0xE4, 0xB5, 0x8C, 0xCA},
	{0x2A, 0xA4, 0x3C, 0xE1, 0x17, 0x21},
	{0xAC, 0xCF, 0x23, 0x15, 0xFC, 0xCC},
	{0x58, 0x93, 0x96, 0x11, 0x53, 0x88},
	{0x14, 0xDD, 0xA9, 0x94, 0x8B, 0x10},
	{0xDC, 0x7F, 0xA4, 0x7D, 0xC0, 0xBE},
	{0xBC, 0x0F, 0x2B, 0x8D, 0x0F, 0x45},
	{0xB8, 0xE6, 0x25, 0x5A, 0x53, 0x12}
};

// RSSIs, experimental
byte rssis_thiel_kai_organic[NN_THIEL_KAI_ORGANIC] = {
20, 20, 19, 19, 19, 17, 17, 17
};

byte channels_thiel_kai_organic[NN_THIEL_KAI_ORGANIC] = {
 1, 2, 3, 5, 6, 8, 9, 11  
};