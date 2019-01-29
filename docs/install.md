# Skylift V0.4 June 2108

- intall oled, gfx, ace button
- install CP2102 driver https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers


SkyLift is a low-cost DIY geolocation spoofing device. It uses the ESP8266 to broadcast WiFi Beacon Frames that exploit a longstanding (2008) vulnerability in WiFi geolocation services. 


Documentation for the previous version can be found [/README_v3.md]


## Parts List

Required

- ESP8266: Wi-Fi Micro-controller
	- NodeMCU 12E
		- Low cost: 3.50
		- USB --> Serial: CP1202
		- Antenna range: OK (not as good as 12F, 12S)

Optional

- OLED
	- ebay:
- Switches
	- x
- PCB
	- 

### Install Arduino

- install driver
- debug <https://developer.apple.com/library/archive/technotes/tn2459/_index.html>

## Setup

Setup the Python parser script to convert WiFi networks

- `cd utils`
- `virtualenv venv -p python3`
- `source venv/bin/activate`
- `pip install -r requirements.txt`
- `pip install --editable .` installs network parser
- `pip install jupyter`
- `jupyter kernelspec install-self --user`
- `parser`

The output should be:

```
(venv) ➜  utils git:(master) ✗ parser
Usage: parser [OPTIONS] COMMAND [ARGS]...

Options:
  --help  Show this message and exit.

Commands:
  ios2json       Convert iOS Airpot Utility scan to
  json2ino       Convert JSON to Arduino .h files
  osx_scan2json  Scan WiFi networks with MacOS Utility scan...
  pcap2json      TODO Convert PCAP to JSON
  wigle2json     Download Wigle API data to JSON
```


`TODO: make setup.sh script`


-----

## Collecting WiFi Data

Collect WiFi data on location or pull from Wigle's API. On-location data is more accurate and includes RSSI information. 


### iOS

- Download ioS "Airport Utility" from AppStore
- General Settings > WiFi Scanner (enable)
- Open Airport Utility and scan WiFi networks for 10-20 seconds
- Save scan to iOS Notes or Email it
- Add `#` in front of SSID, etc... on first line
- Add another line with `# location of scan` to top of file


### MacOS

- Scan and write to file: `parser osx_scan2json output.json` 
- Scan and redirect stdout: `parser osx_scan2json > output.json`


### Wigle

- Register on Wigle for API Name and Token
- Set `WIGLE_API_NAME` and `WIGLE_API_TOKEN` in .env
- `source .env`
- `parser wigle2json output.json --api-name=$WIGLE_API_NAME --api-key=$WIGLE_API_KEY`

Parsing a CSV of filenames, lat, lon
`while IFS=, read -r fname lat lon rad;do parser wigle2json ../data/json/$fname.json --api_name=$WIGLE_API_NAME --api_token=$WIGLE_API_TOKEN --lat=$lat --lon=$lon --radius=$rad;done < ../data/pools/wigle.csv`

## Format Network Data for Arduino

- To file: `parser json2ino input.json arduino_sketch/networks.h`
- Stdout: `parser json2ino input.json > arduino_sketch/networks.h`





## Research on Beacon Frames

- <http://www.rfwireless-world.com/Terminology/WLAN-beacon-frame.html>
- <http://www.rfwireless-world.com/Articles/WLAN-MAC-layer-protocol.html>
- <http://www.sharetechnote.com/html/WLAN_Beacon.html>
- <http://www.sharetechnote.com/html/WLAN_FrameStructure.html#MAC_Header_Ex_01>
- <https://www.saltwaterc.eu/forging-an-802-11-beacon-frame.html>
- <http://www.zytrax.com/tech/wireless/802_mac.htm>
- <https://witestlab.poly.edu/blog/802-11-wireless-lan-2/>
- <https://mrncciew.com/2014/10/08/802-11-mgmt-beacon-frame/>
- <https://en.wikipedia.org/wiki/List_of_WLAN_channels>
- Japan: 12, 13, 14. Most of world is: 12, 13. US is only 1 - 11

------

