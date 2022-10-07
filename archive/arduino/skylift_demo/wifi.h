/*
   Run the WiFi functions
*/

#include <ESP8266WiFi.h>
extern "C" {
#include "user_interface.h"
}

// force all max TX output
#define USE_RSSI_DYNAMIC 0

// Globals
extern const unsigned int NNETS;
extern const unsigned int NPLACES;
extern char* ssids[NNETS];
extern byte bssids[NNETS][6];
extern byte channels[NNETS];
extern byte rssis[NNETS];
extern unsigned int idx_offsets[NPLACES];
extern boolean wifi_tx_status;

// Local
byte nplace_nets;
unsigned int index_start;
unsigned int index_end;
unsigned int counter_offset;

// ---------------------------------------------------------------
// Transmit Beacon packets
// ---------------------------------------------------------------
const byte NUM_TS_PLACES = 8; // number of timestamp places
byte time_packet[NUM_TS_PLACES];
unsigned long ap_epochs[NNETS];
unsigned int counter = 0;
unsigned int sequence_counter = 0;
unsigned int wifi_channel = 6;
unsigned int wifi_channel_pre = 6;
unsigned int wifi_rssi = 20;
unsigned int wifi_rssi_pre = 20;
unsigned long last_mac_ms = 0;
unsigned char pre_channel = 0;
unsigned long last_beacon_us; // milliseconds
unsigned long beacon_interval_us = 102400 / NNETS; // Beacon interval = 0.102400 seconds
const byte npacket_bytes = 57;
/*
  0       Subtype (0x80)
  Management Frame, subtype 8
  ---------------------------------------------
  1       Order Flag (0x00)
  Not leaving DS, last fragment, STA stays up, no data buffered, data not protected, not strictly ordered
  ---------------------------------------------
  2-3     Duration: (0x00, 0x00)
  0 microseconds
  ---------------------------------------------
  4-9     Destination Hardware Address
  Destination address: Broadcast (ff:ff:ff:ff:ff:ff)
  ---------------------------------------------
  10-15   Source Address
  Source address: AB:CD:EF:12:34:56
  ---------------------------------------------
  16-21   Base Station Id
  BSS Id: AB:CD:EF:12:34:56
  ---------------------------------------------
  22-23   Sequence Control
  .... .... .... 0000 = Fragment number: 0
  1001 1001 1100 .... = Sequence number: 2460 (0x99CC)  modulo 4096 increment per AP frame)
  ---------------------------------------------
  24-31   Timestamp
  Timestamp: 0x000000008c7da4ee --> 4,003,757,452 ms
  0xEE, 0xA4, ox7D, 0x8C, 0x00, 0x00, 0x00, 0x00 (eea47d8c00000000 --> 0x00000000eea47d8c)
  ---------------------------------------------
  32-33   Beacon Interval: Time units (104 milliseconds)
  Beacon Interval: 0.102400 [Seconds]
  0x64, 0x00
  ---------------------------------------------
  34-35   Capability Info
  Capabilities Information: 0x1431
  0x31, 0x14
  ---------------------------------------------
  36-41   SSID (eg 6 character length)
  40

*/
uint8_t packet[npacket_bytes] = {
  /*idx     bytes*/
  /*Frame type */
  /*0*/     0x80,
  /* Order Flag */
  /*1*/     0x00,
  /* Duration */
  /*2*/     0x00, 0x00,
  /* Destination address */
  /*4*/     0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  /* VARIABLE:Source address */
  /*10*/    0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
  /* VARIABLE:Source address (same) */
  /*16*/    0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
  /* VARIABLE: Sequence control: incrementing number % 4096 */
  /*22*/    0x00, 0x00,
  /* VARIABLE: Timestamp */
  /*24*/    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* Beacon Interval */
  /*32*/    0x64, 0x00,
  /* Capabilities Information*/
  /*34*/    0x31, 0x14,
  /* VARIABLE: SSID (network name) */
  /*36*/    0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* ? */
  0x01, 0x08, 0x82, 0x84,
  /* ? */
  0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01,
  /* Tag data (end byte) */
  /*56*/    0x04
};

void set_timestamp(byte *p, byte n) {
  // return time in reverse order as byte array pointer
  long t = millis() + ap_epochs[n]; // random start time
  p[0] = (t & 0x00000000000000FF);
  p[1] = (t & 0x000000000000FF00) >> 8;
  p[2] = (t & 0x0000000000FF0000) >> 16;
  p[3] = (t & 0x00000000FF000000) >> 24;
  p[4] = (t & 0x000000FF00000000) >> 32;
  p[5] = (t & 0x0000FF0000000000) >> 40;
  p[6] = (t & 0x00FF000000000000) >> 48;
  p[7] = (t & 0x00FF000000000000) >> 56;
}


void update_wifi() {
  // called when place index is changed by switch press
  index_start = idx_offsets[place_idx_cur];
  if (place_idx_cur == NPLACES - 1) {
    index_end = NNETS;
  } else {
    index_end = idx_offsets[place_idx_cur + 1];
  }
  nplace_nets = index_end - index_start;
  counter = index_start;
  beacon_interval_us = 102400 / nplace_nets;
}


void setup_wifi() {
  // Initialize ESP8266
  WiFi.setOutputPower(20.5); // 0 (min) - 20.5 (max)
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(1);
  for (int i = 0; i < NUM_TS_PLACES; i++) time_packet[i] = 0x00;
  // set random AP timestamp epochs
  for (int i = 0; i < NNETS; i++) ap_epochs[i] = random(10000 * 1000, 1000000 * 1000); // millis
  update_wifi();
}

void run_wifi() {

  if (wifi_tx_status == 0) {
    // Transmit OFF
    return;
  }
  // throttle loop to emulate beacon interval send time
  if (micros() - last_beacon_us < beacon_interval_us) {
    return;
  }

  last_beacon_us = micros();

  counter = (counter + 1) % nplace_nets;
  counter_offset = counter + index_start;

  // BSSID (source address)
  packet[10] = packet[16] = bssids[counter_offset][0];
  packet[11] = packet[17] = bssids[counter_offset][1];
  packet[12] = packet[18] = bssids[counter_offset][2];
  packet[13] = packet[19] = bssids[counter_offset][3];
  packet[14] = packet[20] = bssids[counter_offset][4];
  packet[15] = packet[21] = bssids[counter_offset][5];

  // SSID (Fixed size 6 for now)
  packet[38] = ssids[counter_offset][0];
  packet[39] = ssids[counter_offset][1];
  packet[40] = ssids[counter_offset][2];
  packet[41] = ssids[counter_offset][3];
  packet[42] = ssids[counter_offset][4];
  packet[43] = ssids[counter_offset][5];

  // Convert sequnce to MSB/LSB
  sequence_counter = (sequence_counter + 1) % 4096; // should be randomly initialized
  packet[22] = (sequence_counter & 0x00FF); // LSB
  packet[23] = (sequence_counter & 0xFF00) >> 8; // MSB

  // Update the timestamp
  set_timestamp(time_packet, counter);
  for (int i = 0; i < NUM_TS_PLACES; i++) {
    packet[24 + i] = time_packet[i];
  }

  //packet[38] = '\x00'; // hidden SSID

  // Set channel if different than last
  if (channels[counter_offset] != wifi_channel_pre) {
    wifi_channel_pre = wifi_channel;
    wifi_channel = channels[counter_offset];
    wifi_set_channel(wifi_channel);
  }
  packet[56] = wifi_channel;

  // Experimental: Adjust TX power to simulate AP distance
#ifdef USE_RSSI_DYNAMIC
  if (rssis[counter_offset] != wifi_rssi_pre) {
    wifi_rssi_pre = wifi_rssi;
    wifi_rssi = rssis[counter_offset];
    WiFi.setOutputPower(wifi_rssi);
  }
#endif

  // Broadcast
  wifi_send_pkt_freedom(packet, npacket_bytes, 0);

}
