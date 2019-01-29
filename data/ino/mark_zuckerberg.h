/*
 Scan type: wigle
 Networks: 15
 Target lat, lon: 37.45382696, -122.13933349
 Comment: Zucks place
 Generated: Jan 29, 2019 23:30:01
 (open + close this sketch to reload changes)
*/

// Copy and paste this to the networks.h file
/*
#include "networks/mark_zuckerberg.h"
extern const byte NN_NN_MARK_ZUCKERBERG;
extern char* ssids_mark_zuckerberg[];
extern byte bssids_[][6];
extern byte rssis_[]
extern byte channels_mark_zuckerberg[];
extern String name_mark_zuckerberg;
extern String city_mark_zuckerberg;
*/

// Number of networks
const byte NN_MARK_ZUCKERBERG = 15;

// Name and location for OLED
String name_mark_zuckerberg = "Location Name (NW)";
String city_mark_zuckerberg = "Name of City, State";

// SSIDs for display on OLED
char* ssids_mark_zuckerberg[NN_MARK_ZUCKERBERG] = {
	"CPTJ",
	"HOME-6",
	"AER160",
	"Dahls",
	"Croc24",
	"HOME-1",
	"Wester",
	"None",
	"None",
	"HOME-A",
	"None",
	"None",
	"MiloWi",
	"None",
	"None"
};

// BSSIDs (MAC addresses)
byte bssids_mark_zuckerberg[NN_MARK_ZUCKERBERG][6] = {
	{0x00, 0x1D, 0x7E, 0x4E, 0x51, 0xE5},
	{0x00, 0x1D, 0xD4, 0x52, 0x6E, 0xC0},
	{0x00, 0x30, 0x44, 0x26, 0x81, 0x5F},
	{0x00, 0x19, 0xE3, 0xFB, 0x79, 0xF8},
	{0x00, 0x90, 0xA9, 0x10, 0xFD, 0xDB},
	{0x00, 0x71, 0xC2, 0x65, 0xCC, 0x58},
	{0x00, 0x90, 0xA9, 0xCB, 0x43, 0x56},
	{0x00, 0x26, 0xF3, 0x3C, 0xAE, 0xDA},
	{0x00, 0x26, 0xF3, 0x3C, 0xAE, 0xD9},
	{0x00, 0x26, 0xF3, 0x3C, 0xAE, 0xD8},
	{0x00, 0x71, 0xC2, 0x65, 0xCC, 0x5B},
	{0x00, 0x26, 0xF3, 0x72, 0x1C, 0xBB},
	{0x00, 0x1E, 0x52, 0x78, 0x66, 0x86},
	{0x00, 0x71, 0xC2, 0x98, 0xCD, 0x25},
	{0x00, 0x71, 0xC2, 0x98, 0xCD, 0x23}
};

// RSSIs, experimental
byte rssis_mark_zuckerberg[NN_MARK_ZUCKERBERG] = {
20, 20, 20, 20, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19
};

byte channels_mark_zuckerberg[NN_MARK_ZUCKERBERG] = {
 1, 1, 2, 3, 3, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11  
};