/*
  S K Y L I F T
  CRYPTORAVE BARCELONA EDITION
  Version: 0.5.1
  Adam Harvey 2016-2018
  Code: https://github.com/adamhrv/skylift
  About: https://ahprojects.com/projects/datapools
*/


// Globals
const String DEV_VERSION = "V 0.5.1 BCN";
const unsigned int NPLACES = 6;
const unsigned int NNETS = 47; // EDIT (copy this value from "networks.h")
char* ssids[NNETS];
byte bssids[NNETS][6];
byte channels[NNETS];
byte rssis[NNETS];
unsigned int idx_offsets[NPLACES];
String place_names[NPLACES];
String place_cities[NPLACES];
unsigned int place_idx_cur = 1; // overwritten by EEPROM value
boolean wifi_tx_status = 0; // 1 = ON, 0 = OFF, overwritten by EEPROM value


// include memory, then networks
#include "memory.h" // first
#include "networks.h" // second

// include display, switches, wifi
#include "display.h"
#include "switches.h"
#include "wifi.h"

void setup() {
  setup_memory();
  setup_networks();
  setup_switches();
  setup_display();
  setup_wifi();
}

void loop() {
  run_memory();
  run_switches();
  run_display();
  run_wifi();
}
