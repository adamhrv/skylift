/*
	Auto-generated file
const unsigned int NNETS = 75; // copy this to main tab
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

#include "networks/fb_central.h"
extern const byte NN_FB_CENTRAL;
extern char* ssids_fb_central[];
extern byte bssids_fb_central[][6];
extern byte rssis_fb_central[];
extern byte channels_fb_central[];
extern String name_fb_central;
extern String city_fb_central;

#include "networks/fb_nw.h"
extern const byte NN_FB_NW;
extern char* ssids_fb_nw[];
extern byte bssids_fb_nw[][6];
extern byte rssis_fb_nw[];
extern byte channels_fb_nw[];
extern String name_fb_nw;
extern String city_fb_nw;

#include "networks/fb_ne.h"
extern const byte NN_FB_NE;
extern char* ssids_fb_ne[];
extern byte bssids_fb_ne[][6];
extern byte rssis_fb_ne[];
extern byte channels_fb_ne[];
extern String name_fb_ne;
extern String city_fb_ne;

#include "networks/fb_se.h"
extern const byte NN_FB_SE;
extern char* ssids_fb_se[];
extern byte bssids_fb_se[][6];
extern byte rssis_fb_se[];
extern byte channels_fb_se[];
extern String name_fb_se;
extern String city_fb_se;

#include "networks/fb_sw.h"
extern const byte NN_FB_SW;
extern char* ssids_fb_sw[];
extern byte bssids_fb_sw[][6];
extern byte rssis_fb_sw[];
extern byte channels_fb_sw[];
extern String name_fb_sw;
extern String city_fb_sw;


void setup_networks() {

// Names
place_names[0] = name_fb_central;
place_names[1] = name_fb_nw;
place_names[2] = name_fb_ne;
place_names[3] = name_fb_se;
place_names[4] = name_fb_sw;

// Cities
place_cities[0] = city_fb_central;
place_cities[1] = city_fb_nw;
place_cities[2] = city_fb_ne;
place_cities[3] = city_fb_se;
place_cities[4] = city_fb_sw;

// concatenate networks into one array for each attribute
unsigned int idx_offset = 0;
unsigned int idx = 0;
idx_offsets[0] = 0;

// ------------------------------------------------------
// FB_CENTRAL
// ------------------------------------------------------

for(unsigned int i = 0; i < NN_FB_CENTRAL; i++){
	ssids[i + idx_offset] = ssids_fb_central[i];
	for (byte j = 0; j < 6; j++){
		bssids[i + idx_offset][j] = bssids_fb_central[i][j];
	}
	channels[i + idx_offset] = channels_fb_central[i];
	rssis[i + idx_offset] = rssis_fb_central[i];
}
idx_offset += NN_FB_CENTRAL;
idx++;
idx_offsets[idx] = idx_offset;


// ------------------------------------------------------
// FB_NW
// ------------------------------------------------------

for(unsigned int i = 0; i < NN_FB_NW; i++){
	ssids[i + idx_offset] = ssids_fb_nw[i];
	for (byte j = 0; j < 6; j++){
		bssids[i + idx_offset][j] = bssids_fb_nw[i][j];
	}
	channels[i + idx_offset] = channels_fb_nw[i];
	rssis[i + idx_offset] = rssis_fb_nw[i];
}
idx_offset += NN_FB_NW;
idx++;
idx_offsets[idx] = idx_offset;


// ------------------------------------------------------
// FB_NE
// ------------------------------------------------------

for(unsigned int i = 0; i < NN_FB_NE; i++){
	ssids[i + idx_offset] = ssids_fb_ne[i];
	for (byte j = 0; j < 6; j++){
		bssids[i + idx_offset][j] = bssids_fb_ne[i][j];
	}
	channels[i + idx_offset] = channels_fb_ne[i];
	rssis[i + idx_offset] = rssis_fb_ne[i];
}
idx_offset += NN_FB_NE;
idx++;
idx_offsets[idx] = idx_offset;


// ------------------------------------------------------
// FB_SE
// ------------------------------------------------------

for(unsigned int i = 0; i < NN_FB_SE; i++){
	ssids[i + idx_offset] = ssids_fb_se[i];
	for (byte j = 0; j < 6; j++){
		bssids[i + idx_offset][j] = bssids_fb_se[i][j];
	}
	channels[i + idx_offset] = channels_fb_se[i];
	rssis[i + idx_offset] = rssis_fb_se[i];
}
idx_offset += NN_FB_SE;
idx++;
idx_offsets[idx] = idx_offset;


// ------------------------------------------------------
// FB_SW
// ------------------------------------------------------

for(unsigned int i = 0; i < NN_FB_SW; i++){
	ssids[i + idx_offset] = ssids_fb_sw[i];
	for (byte j = 0; j < 6; j++){
		bssids[i + idx_offset][j] = bssids_fb_sw[i][j];
	}
	channels[i + idx_offset] = channels_fb_sw[i];
	rssis[i + idx_offset] = rssis_fb_sw[i];
}
idx_offset += NN_FB_SW;
idx++;
idx_offsets[idx] = idx_offset;


}