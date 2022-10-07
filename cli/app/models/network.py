#############################################################################
#
# SkyLift
# MIT License
# Copyright (c) 2016-2022 Adam Harvey
# https://github.com/adamhrv/skylift
#
#############################################################################

from dataclasses import dataclass
from dataclasses import dataclass, field
from typing import Dict, Tuple, List
from app.utils.misc_utils import map_range


# -------------------------------------------------------------------
# Scans
# -------------------------------------------------------------------

@dataclass
class Network:
  bssid: str
  channel: int
  rssi: int
  latitude: float=0.0
  longitude: float=0.0
  ssid: str=''

  def bssid_as_hex_list_ino(self):
    hex_str = ', '.join([f'0x{x}' for x in self.bssid.split(':')])
    return '{' + hex_str + '}'

  def channel_as_2pt4(self):
    return max(0, min(self.channel, 11))


  # TODO
  # def rssi_as_dbm(self, power_range=(10, 20)):
  #   # map -30 --> 19.5
  # map_range
  #   return self.rssi


@dataclass
class Networks:
  filename: str
  n_wifi: int=0
  n_bt: int=0
  wifi: List[Network]=field(default_factory=lambda: [])
  bt: List[Network]=field(default_factory=lambda: [])

  def __post_init__(self):
    self.n_wifi = len(self.wifi)
    self.n_bt = len(self.bt)
    # sort by descending rssi values
    self.wifi = sorted(self.wifi, key=lambda x: (x.rssi), reverse=True)
    self.bt = sorted(self.bt, key=lambda x: (x.rssi), reverse=True)


  def get_networks(self, min_rssi=None, max_rssi=None, max_networks=None, 
    device_type='wifi'):
    if device_type == 'wifi':
      nets = self.wifi
    elif device_type == 'bt':
      nets = self.bt
    rssis = [x.rssi for x in nets]
    min_rssi = min_rssi if min_rssi else min(rssis)
    max_rssi = max_rssi if max_rssi else max(rssis)
    max_networks = max_networks if max_networks else len(nets)
    nets = [x for x in self.wifi if x.rssi >= min_rssi and x.rssi <= max_rssi]
    nets = nets[:max_networks]
    return nets

  