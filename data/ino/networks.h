/*
	Auto-generated file
const unsigned int NNETS = 15; // copy this to main tab
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

#include "networks/mark_zuckerberg.h"
extern const byte NN_MARK_ZUCKERBERG;
extern char* ssids_mark_zuckerberg[];
extern byte bssids_mark_zuckerberg[][6];
extern byte rssis_mark_zuckerberg[];
extern byte channels_mark_zuckerberg[];
extern String name_mark_zuckerberg;
extern String city_mark_zuckerberg;


void setup_networks() {

// Names
place_names[0] = name_mark_zuckerberg;

// Cities
place_cities[0] = city_mark_zuckerberg;

// concatenate networks into one array for each attribute
unsigned int idx_offset = 0;
unsigned int idx = 0;
idx_offsets[0] = 0;

// ------------------------------------------------------
// MARK_ZUCKERBERG
// ------------------------------------------------------

for(unsigned int i = 0; i < NN_MARK_ZUCKERBERG; i++){
	ssids[i + idx_offset] = ssids_mark_zuckerberg[i];
	for (byte j = 0; j < 6; j++){
		bssids[i + idx_offset][j] = bssids_mark_zuckerberg[i][j];
	}
	channels[i + idx_offset] = channels_mark_zuckerberg[i];
	rssis[i + idx_offset] = rssis_mark_zuckerberg[i];
}
idx_offset += NN_MARK_ZUCKERBERG;
idx++;
idx_offsets[idx] = idx_offset;


}