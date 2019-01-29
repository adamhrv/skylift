import subprocess
import plistlib

from app.utils import logger_utils, file_utils
from app.settings import app_cfg as cfg

class Scanner:

  def __init__(self):
    self.log = logger_utils.Logger.getLogger()

  @classmethod
  def osx(cls, lat, lon, comment):
    """Use MacOS WiFi scan"""
    print ('[+] Scanning for WiFi networks...')

    t = subprocess.Popen(args=[cfg.FP_AIRPORT,'-s','-x'], stdout=subprocess.PIPE)
    output = t.communicate()[0]
    scan_xml = plistlib.readPlistFromBytes(output)

    networks = {}
    for network in scan_xml:
        # Ensure 2-digit hexidecimal
        bssid = ':'.join(map(lambda x: x if (len(x)==2) else ('0'+x), network['BSSID'].split(':')))
        ssid = network['SSID_STR']
        channel = network['CHANNEL']
        rssi = network['RSSI']
        if not bssid in networks.keys():
          networks[bssid] = WiFiNet(ssid,bssid,channel,rssi,vendor=self.mac2vendor(bssid))
    
    # TODO handle unicode network names
    networks = [n.as_obj() for n in networks]
    json_out = {
      'meta':{
        'source': os.path.basename(fp_input.name),
        'type': ScanType.MACOS.value.lower(),
        'comment': comment,
        'lat_origin': lat,
        'lon_origin': lon,
      },
      'networks': networks
      }

    file_utils.write_json(data, fp_out, minify=False, ensure_ascii=True)

    