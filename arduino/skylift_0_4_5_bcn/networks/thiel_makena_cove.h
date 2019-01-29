/*
 Scan type: wigle
 Networks: 10
 Target lat, lon: 20.627146, -156.443175
 Comment: Makena Cove
 Generated: Oct 27, 2018 12:38:10
 (open + close this sketch to reload changes)
*/

// Copy and paste this to the networks.h file
/*
#include "networks/thiel_makena_cove.h"
extern const byte NN_NN_THIEL_MAKENA_COVE;
extern char* ssids_thiel_makena_cove[];
extern byte bssids_[][6];
extern byte rssis_[]
extern byte channels_thiel_makena_cove[];
extern String name_thiel_makena_cove;
extern String city_thiel_makena_cove;
*/

// Number of networks
const byte NN_THIEL_MAKENA_COVE = 10;

// Name and location for OLED
String name_thiel_makena_cove = "Peter Thiel (N)";
String city_thiel_makena_cove = "Makena Cove, HI";

// SSIDs for display on OLED
char* ssids_thiel_makena_cove[NN_THIEL_MAKENA_COVE] = {
	"dolphi",
	"NETGEA",
	"mack",
	"mack_2",
	"dolphi",
	"Makena",
	"HP-Pri",
	"HT129",
	"Nani M",
	"Nani M"
};

// BSSIDs (MAC addresses)
byte bssids_thiel_makena_cove[NN_THIEL_MAKENA_COVE][6] = {
	{0x64, 0xA0, 0xE7, 0x5C, 0x47, 0x30},
	{0xA4, 0x2B, 0x8C, 0x82, 0xFE, 0x09},
	{0x38, 0x3B, 0xC8, 0x1E, 0x60, 0x0A},
	{0x04, 0xA1, 0x51, 0x21, 0xD1, 0x0B},
	{0x1C, 0xE6, 0xC7, 0x5A, 0x1F, 0x40},
	{0x90, 0x72, 0x40, 0x14, 0xED, 0x28},
	{0x74, 0x46, 0xA0, 0xDB, 0xE4, 0x6D},
	{0xB8, 0xE6, 0x25, 0x56, 0x5B, 0x52},
	{0x00, 0x02, 0x6F, 0xE9, 0x8E, 0x96},
	{0x00, 0x26, 0xEC, 0x00, 0x93, 0x90}
};

// RSSIs, experimental
byte rssis_thiel_makena_cove[NN_THIEL_MAKENA_COVE] = {
20, 20, 19, 19, 19, 19, 19, 19, 18, 18
};

byte channels_thiel_makena_cove[NN_THIEL_MAKENA_COVE] = {
 1, 2, 3, 4, 5, 6, 7, 8, 9, 11  
};