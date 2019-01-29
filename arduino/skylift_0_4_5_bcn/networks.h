/*
	Auto-generated file
  const unsigned int NNETS = 47; // copy this to main tab
*/

// Globals
extern const unsigned int NNETS;
extern const unsigned int NPLACES;
extern char* ssids[NNETS];
extern byte bssids[NNETS][6];
extern byte channels[NNETS];
extern byte rssis[NNETS];
extern unsigned int idx_offsets[NPLACES];
extern String place_names[NPLACES];
extern String place_cities[NPLACES];
extern unsigned int place_idx_cur;
extern boolean wifi_tx_status;


// --------------------------------------------------------
// Include all networks here
// all networks should be included in the "networks/" subdirectory
// --------------------------------------------------------

#include "networks/thiel_makena_beach.h"
extern const byte NN_THIEL_MAKENA_BEACH;
extern char* ssids_thiel_makena_beach[];
extern byte bssids_thiel_makena_beach[][6];
extern byte rssis_thiel_makena_beach[];
extern byte channels_thiel_makena_beach[];
extern String name_thiel_makena_beach;
extern String city_thiel_makena_beach;

#include "networks/thiel_big_beach.h"
extern const byte NN_THIEL_BIG_BEACH;
extern char* ssids_thiel_big_beach[];
extern byte bssids_thiel_big_beach[][6];
extern byte rssis_thiel_big_beach[];
extern byte channels_thiel_big_beach[];
extern String name_thiel_big_beach;
extern String city_thiel_big_beach;

#include "networks/thiel_makena_cove.h"
extern const byte NN_THIEL_MAKENA_COVE;
extern char* ssids_thiel_makena_cove[];
extern byte bssids_thiel_makena_cove[][6];
extern byte rssis_thiel_makena_cove[];
extern byte channels_thiel_makena_cove[];
extern String name_thiel_makena_cove;
extern String city_thiel_makena_cove;

#include "networks/thiel_main.h"
extern const byte NN_THIEL_MAIN;
extern char* ssids_thiel_main[];
extern byte bssids_thiel_main[][6];
extern byte rssis_thiel_main[];
extern byte channels_thiel_main[];
extern String name_thiel_main;
extern String city_thiel_main;

#include "networks/thiel_kai_organic.h"
extern const byte NN_THIEL_KAI_ORGANIC;
extern char* ssids_thiel_kai_organic[];
extern byte bssids_thiel_kai_organic[][6];
extern byte rssis_thiel_kai_organic[];
extern byte channels_thiel_kai_organic[];
extern String name_thiel_kai_organic;
extern String city_thiel_kai_organic;

#include "networks/thiel_south_east.h"
extern const byte NN_THIEL_SOUTH_EAST;
extern char* ssids_thiel_south_east[];
extern byte bssids_thiel_south_east[][6];
extern byte rssis_thiel_south_east[];
extern byte channels_thiel_south_east[];
extern String name_thiel_south_east;
extern String city_thiel_south_east;


void setup_networks() {

  // Names
  place_names[0] = name_thiel_makena_beach;
  place_names[1] = name_thiel_big_beach;
  place_names[2] = name_thiel_makena_cove;
  place_names[3] = name_thiel_main;
  place_names[4] = name_thiel_kai_organic;
  place_names[5] = name_thiel_south_east;

  // Cities
  place_cities[0] = city_thiel_makena_beach;
  place_cities[1] = city_thiel_big_beach;
  place_cities[2] = city_thiel_makena_cove;
  place_cities[3] = city_thiel_main;
  place_cities[4] = city_thiel_kai_organic;
  place_cities[5] = city_thiel_south_east;

  // concatenate networks into one array for each attribute
  unsigned int idx_offset = 0;
  unsigned int idx = 0;
  idx_offsets[0] = 0;

  // ------------------------------------------------------
  // THIEL_MAKENA_BEACH
  // ------------------------------------------------------

  for (unsigned int i = 0; i < NN_THIEL_MAKENA_BEACH; i++) {
    ssids[i + idx_offset] = ssids_thiel_makena_beach[i];
    for (byte j = 0; j < 6; j++) {
      bssids[i + idx_offset][j] = bssids_thiel_makena_beach[i][j];
    }
    channels[i + idx_offset] = channels_thiel_makena_beach[i];
    rssis[i + idx_offset] = rssis_thiel_makena_beach[i];
  }
  idx_offset += NN_THIEL_MAKENA_BEACH;
  idx++;
  idx_offsets[idx] = idx_offset;


  // ------------------------------------------------------
  // THIEL_BIG_BEACH
  // ------------------------------------------------------

  for (unsigned int i = 0; i < NN_THIEL_BIG_BEACH; i++) {
    ssids[i + idx_offset] = ssids_thiel_big_beach[i];
    for (byte j = 0; j < 6; j++) {
      bssids[i + idx_offset][j] = bssids_thiel_big_beach[i][j];
    }
    channels[i + idx_offset] = channels_thiel_big_beach[i];
    rssis[i + idx_offset] = rssis_thiel_big_beach[i];
  }
  idx_offset += NN_THIEL_BIG_BEACH;
  idx++;
  idx_offsets[idx] = idx_offset;


  // ------------------------------------------------------
  // THIEL_MAKENA_COVE
  // ------------------------------------------------------

  for (unsigned int i = 0; i < NN_THIEL_MAKENA_COVE; i++) {
    ssids[i + idx_offset] = ssids_thiel_makena_cove[i];
    for (byte j = 0; j < 6; j++) {
      bssids[i + idx_offset][j] = bssids_thiel_makena_cove[i][j];
    }
    channels[i + idx_offset] = channels_thiel_makena_cove[i];
    rssis[i + idx_offset] = rssis_thiel_makena_cove[i];
  }
  idx_offset += NN_THIEL_MAKENA_COVE;
  idx++;
  idx_offsets[idx] = idx_offset;


  // ------------------------------------------------------
  // THIEL_MAIN
  // ------------------------------------------------------

  for (unsigned int i = 0; i < NN_THIEL_MAIN; i++) {
    ssids[i + idx_offset] = ssids_thiel_main[i];
    for (byte j = 0; j < 6; j++) {
      bssids[i + idx_offset][j] = bssids_thiel_main[i][j];
    }
    channels[i + idx_offset] = channels_thiel_main[i];
    rssis[i + idx_offset] = rssis_thiel_main[i];
  }
  idx_offset += NN_THIEL_MAIN;
  idx++;
  idx_offsets[idx] = idx_offset;


  // ------------------------------------------------------
  // THIEL_KAI_ORGANIC
  // ------------------------------------------------------

  for (unsigned int i = 0; i < NN_THIEL_KAI_ORGANIC; i++) {
    ssids[i + idx_offset] = ssids_thiel_kai_organic[i];
    for (byte j = 0; j < 6; j++) {
      bssids[i + idx_offset][j] = bssids_thiel_kai_organic[i][j];
    }
    channels[i + idx_offset] = channels_thiel_kai_organic[i];
    rssis[i + idx_offset] = rssis_thiel_kai_organic[i];
  }
  idx_offset += NN_THIEL_KAI_ORGANIC;
  idx++;
  idx_offsets[idx] = idx_offset;


  // ------------------------------------------------------
  // THIEL_SOUTH_EAST
  // ------------------------------------------------------

  for (unsigned int i = 0; i < NN_THIEL_SOUTH_EAST; i++) {
    ssids[i + idx_offset] = ssids_thiel_south_east[i];
    for (byte j = 0; j < 6; j++) {
      bssids[i + idx_offset][j] = bssids_thiel_south_east[i][j];
    }
    channels[i + idx_offset] = channels_thiel_south_east[i];
    rssis[i + idx_offset] = rssis_thiel_south_east[i];
  }
  idx_offset += NN_THIEL_SOUTH_EAST;
  idx++;
  idx_offsets[idx] = idx_offset;


}
