from os.path import join
import json
from pathlib import Path

import requests

from app.models.data_types import WiFiNet
from app.utils import logger_utils, geo_utils
from app.settings import app_cfg as cfg


class WigleAPI:

  def __init__(self, api_name, api_token):
    self.log = logger_utils.Logger.getLogger()
    self.api_name = api_name
    self.api_token = api_token

  def build_url(self, lat, lon, radius_scale, opt_since):
    """Builds Wigle API URL"""

    #radius_inc_lat = 0.000944 # multiply radius by this amount in lat direction
    #radius_inc_lon = 0.001148 # multiply radius by this amount in lon direction
    radius_inc_lat = 0.00944
    radius_inc_lon = 0.00944
    
    lat_range = (lat - (radius_inc_lat / 2 * radius_scale), lat + (radius_inc_lat / 2 * radius_scale))
    lon_range = (lon - (radius_inc_lon / 2 * radius_scale), lon + (radius_inc_lon / 2 * radius_scale))

    # url builder
    url = 'https://api.wigle.net/api/v2/network/search?'
    url +='onlymine=false&'
    url += 'latrange1=' + str(lat_range[0]) + '&'
    url += 'latrange2=' + str(lat_range[1]) + '&'
    url += 'longrange1=' + str(lon_range[0]) + '&'
    url += 'longrange2=' + str(lon_range[1]) + '&'
    url += 'lastupdt=' + str(int(opt_since)) + '&'
    url += 'freenet=false&'
    url += 'paynet=false'
    return url


  def fetch(self, url, lat, lon):
      """Downloads WiFi data from Wigle API
      Requires API Name/Token
      """
      # https://api.wigle.net/api/v2/network/search?onlymine=false&latrange1=34.09368&latrange2=34.097456&longrange1=-118.380996&longrange2=-118.376404&lastupdt=20160101&freenet=false&paynet=false

      networks = []
      target = (lat, lon)

      # authenticate and get JSON
      # self.log.info('making GET request: {}'.format(url))
      wigle_data = requests.get(url,
        headers={'Authentication':'Basic'},
        auth=(self.api_name, self.api_token))

      # parse wigle results into JSON file
      try:
        wigle_data = wigle_data.json()['results']
      except:
        self.log.error('could not parse data: {}'.format(wigle_data))
        self.log.error(wigle_data)
        self.log.error(f'url: {url}')
        return []

      for n in wigle_data:
        actual = (n['trilat'], n['trilong'])
        rssi_estimated = geo_utils.calc_geo_rssi(actual, target)
        wifi_net = WiFiNet(
          n['ssid'],
          n['netid'],
          n['channel'],
          rssi=rssi_estimated,
          qos=n['qos'],
          lat=n['trilat'],
          lon=n['trilong'],
          lat_target=lat,
          lon_target=lon)
        networks.append(wifi_net)

      # serialize network data
      networks = [n.serialize() for n in networks]
      self.log.debug(f'Found {len(networks)} networks')
      return networks
