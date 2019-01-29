/*
 Scan type: wigle
 Networks: 7
 Target lat, lon: 20.62501, -156.437046
 Comment: south east neighbor
 Generated: Oct 27, 2018 12:38:10
 (open + close this sketch to reload changes)
*/

// Copy and paste this to the networks.h file
/*
#include "networks/thiel_south_east.h"
extern const byte NN_NN_THIEL_SOUTH_EAST;
extern char* ssids_thiel_south_east[];
extern byte bssids_[][6];
extern byte rssis_[]
extern byte channels_thiel_south_east[];
extern String name_thiel_south_east;
extern String city_thiel_south_east;
*/

// Number of networks
const byte NN_THIEL_SOUTH_EAST = 7;

// Name and location for OLED
String name_thiel_south_east = "Peter Thiel (SE)";
String city_thiel_south_east = "Wailea-Makena, HI";

// SSIDs for display on OLED
char* ssids_thiel_south_east[NN_THIEL_SOUTH_EAST] = {
	"Mauivi",
	"Ruckus",
	"Makena",
	"Makena",
	"Emeral",
	"Nani M",
	"Makena"
};

// BSSIDs (MAC addresses)
byte bssids_thiel_south_east[NN_THIEL_SOUTH_EAST][6] = {
	{0x14, 0xDD, 0xA9, 0x94, 0x8B, 0x10},
	{0xF0, 0xB0, 0x52, 0x12, 0x8F, 0x28},
	{0x2C, 0xC5, 0xD3, 0x0B, 0xDC, 0x58},
	{0x58, 0xB6, 0x33, 0x03, 0x9C, 0x88},
	{0x2C, 0xB0, 0x5D, 0x29, 0x6E, 0x5E},
	{0x00, 0x26, 0xEC, 0x00, 0x93, 0x90},
	{0xC0, 0xEA, 0xE4, 0xB5, 0x8C, 0xCA}
};

// RSSIs, experimental
byte rssis_thiel_south_east[NN_THIEL_SOUTH_EAST] = {
20, 20, 20, 20, 20, 20, 19
};

byte channels_thiel_south_east[NN_THIEL_SOUTH_EAST] = {
 1, 2, 4, 6, 7, 9, 11  
};