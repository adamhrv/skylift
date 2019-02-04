# Skylift V1.0 Jan 2019

SkyLift is a low-cost DIY Wi-Fi geolocation spoofing device. It uses the ESP8266 to broadcast Wi-Fi Beacon Frames that exploit a longstanding (2008) vulnerability in Wi-Fi geolocation services. 


![](docs/images/skylift_angle.jpg)

Version 1.0 includes improvements:

- only two commands are needed to go from GPS coords to Arduino firmware
- job files to process Wigle request, coversion to Arduino, and iOS scan conversion
- removed vendors (not really useful)
- improved timing
- reduced SSID names to 6 characters to improve speed (ESP seems to slow down when handling too many array operations and then broadcast timing accuracy slips)
- added notebook for plotting lat/lon (needs more work)
- Changed to Click CLI interface

Documentation for the previous version can be found [docs/README_v1.md]

## Quick Start

- `git clone https://github.com/adamhrv/skylift`
- `cd skylift`
- Get Wi-Fi data from Wigle: `python skylift/cli_jobs.py wigle_api --wigle_api_name [your_username] --wigle_api_key [your_api_key] --jobs data/jobs/wigle_api.csv`
- Convert networks into Arduino sketch:  `python skylift/cli_jobs.py arduino --jobs data/jobs/arduino.csv`
- open the Arduino sketch and upload to your ESP12E
- get full list of commands by running `python skylift/cli_jobs.py`

```
(venv) ➜  skylift git:(master) ✗ python skylift/cli_jobs.py                                         
Usage: cli_jobs.py [OPTIONS] COMMAND1 [ARGS]... [COMMAND2 [ARGS]...]...

Options:
  -v, --verbose  Verbosity: -v DEBUG, -vv INFO, -vvv WARN, -vvvv ERROR, -vvvvv
                 CRITICAL  [default: 4]
  --help         Show this message and exit.

Commands:
  arduino       Converts JSON to Arduino sketch files
  ios           Process iOS Wi-Fi scans
  wigle_api     Fetches Wigle API data
  wigle_export  Processed Exported Wigle data
```

## Getting Started: Python

 Setup virtual environment

- `git clone https://github.com/adamhrv/skylift`
- `cd skylift`
- `virtualenv --no-site-packages -p python3 venv `
- `source venv/bin/activate`
- `pip install -r requirments`

Download test data from Wigle

- sign up for account on <https://wigle.net>
- `cp env-sample env`
- `nano env/wigle-username.env.env` and add your credentials
  - `WIGLE_API_NAME=your_username`
  - `WIGLE_API_TOKEN=your_token`
- `source env/wigle-username.env`
- run `skylift/python cli_jobs.py` you should see the output below



## Getting Started

- [Arduino](docs/arduino.md)
- [Data job files](docs/jobs.md)
- [Getting Data](docs/scanning.md)
- [Research](docs/research.md)
- BOM (in progress)
- PCB (in progress)


### SkyLift at Exhibitions:

- Cryptorave #8 Transmediale
- [*Dark Technology Dark Web*](spektrumberlin.de/exhibitions/detail/exhibition-10-dark-technology-dark-web.html). Spektrum. Berlin. 2017.
- [How Much of This is Fiction](http://www.fact.co.uk/projects/how-much-of-this-is-fiction.aspx). FACT. Liverpool. 2017.
- [*Lanceurs d'alerte*](https://gaite-lyrique.net/lanceurs-dalerte-0) . La Gaîté Lyrique. Paris. 2017.
- [*Welcome to Ecuador*](http://www.zoogalerie.fr/?p=2059&preview=true). Zoo Galerie. 2016.

### SkyLift Credits:

- Original concept developed in collaboration with [!Mediengruppe Bitnik](https://wwwwwwwwwwwwwwwwwwwwww.bitnik.org/) (DE)
- SkyLift V0.1 developed in collaboration with [Surya Mattu](http://suryamattu.com) (US) 
- V0.1 technical advisement from [Julian Oliver](https://julianoliver.com) (DE)
- Research assistance and Python development contributions: [Leon Eckert](http://leoneckert.com/) (US/DE)
- ["Welcome to Ecuador"](http://www.zoogalerie.fr/?p=2059&preview=true) installation (2016) curated by [Aude Launay](http://launayau.de/) for Zoo Galerie
- ESP8266 proof of concept Beacon Frame code by <https://github.com/kripthor/WiFiBeaconJam>
- [Schloss Solitude Web Residency](https://schloss-post.com/skylift-low-cost-geo-location-spoofing-device/)


### Disclaimer

- Use at your own risk
- This code is for security and privacy research and artistic invention in consensual environments
- After a sustained duration of broadcasting, BSSIDs may be blacklisted or even become associated with another location (this happened at [FACT Liverpool](http://www.fact.co.uk/), which could reveal your location to other users, or be used creatively for a *Bermuda Triangle* effect.
- SkyLift is not a GPS spoofing device. For 1.5GHz GPS spoofing, see [Spoof GPS location with low cost TX SDRs](http://hackaday.com/2016/07/19/pokemon-go-cheat-fools-gps-with-software-defined-radio/) or [Software-Defined GPS Signal Simulator](https://github.com/osqzss/gps-sdr-sim)
- SkyLift is currently a 2.4GHz only device
- In controlled environments where cellular signal is weak and there are few Wi-Fi networks SkyLift may work 100% but in busy urban areas with many Wi-Fi signals it may not work at all
- Check local Wi-Fi regulations before using 


### Further Reading

- [Beacon Frame Overview](https://en.wikipedia.org/wiki/Beacon_frame)
- [Beacon Frame Packet](https://mrncciew.com/2014/10/08/802-11-mgmt-beacon-frame/) in more depth
- [Spoofing WiFi Geolocation services](http://www.senet-int.com/2013/12/wi-fi-geo-location/) on Hackaday
- [Don't Trust Geolocation](http://www.journaldulapin.com/2013/08/26/dont-trust-geolocation/)
- [Device-to-Identity Linking Attack Using Targeted Wi-Fi Geolocation Spoofing](https://hal.inria.fr/hal-01176842/document)
- [SkyLift V0.1 post on using MDK3 + Raspberry Pi](https://ahprojects.com/notebook/2016/skylift-geolocation/)

## History

SkyLift began as a Raspberry Pi with a USB Wi-Fi dongle in 2016, then was prototyped on the Adafruit Huzzah. Several different ESP8266 modules were tried and the NodeMCU 12 was found to provide the lowest cost and comparable performance to options. It can now be built, in functional mode only, for as low as $5. 


![](docs/images/pi_esps.jpg)


SkyLift is open source project maintained by [Adam Harvey](https://ahprojects.com) 
