import os
from os.path import join
from pathlib import Path
import logging
import shutil
import sys
from enum import Enum

import colorlog

# ---------------------------------------------------------------------------
# directories
# ---------------------------------------------------------------------------

# Project directory
SELF_CWD = os.path.dirname(os.path.realpath(__file__))  # this file
DP_ROOT = str(Path(SELF_CWD).parent.parent)


# ---------------------------------------------------------------------------
# Logger
# ---------------------------------------------------------------------------

class CustomLogger:

  def __init__(self):
    pass

  @staticmethod
  def create(logger_name, verbosity=4, logfile=None):
    """Configures a logger from click params
    :param verbosity: (int) between 0 and 5
    :param logfile: (str) path to logfile
    :returns: logging root object
    """
    
    loglevel = (5 - (max(0, min(verbosity, 5)))) * 10 # where logging.DEBUG = 10
    date_format = '%Y-%m-%d %H:%M:%S'
    if 'colorlog' in sys.modules and os.isatty(2):
      cformat = '%(log_color)s' + "%(log_color)s%(levelname)-8s%(reset)s %(cyan)s%(filename)s:%(lineno)s:%(bold_cyan)s%(funcName)s() %(reset)s%(message)s"
      f = colorlog.ColoredFormatter(cformat, date_format,
        log_colors = { 'DEBUG'   : 'yellow',       'INFO' : 'white',
        'WARNING' : 'bold_yellow', 'ERROR': 'bold_red',
        'CRITICAL': 'bold_red' })
    else:
      f = logging.Formatter(cfg.LOGFILE_FORMAT, date_format)
    
    logger = logging.getLogger(logger_name)
    logger.setLevel(loglevel)

    if logfile:
      # create file handler which logs even debug messages
      fh = logging.FileHandler(logfile)
      fh.setLevel(loglevel)
      logger.addHandler(fh)

    # add colored handler
    ch = logging.StreamHandler()
    ch.setFormatter(f)
    logger.addHandler(ch)

    if verbosity == 0:
      logger.disabled = True

    return logger

LOGGER_NAME = 'SKYLIFT'
VERBOSITY = int(os.getenv('SKYLIFT_VERBOSITY', 4))
LOG = CustomLogger.create(LOGGER_NAME)



# ---------------------------------------------------------------------------
# Directory paths
# ---------------------------------------------------------------------------

DP_DATA = join(DP_ROOT, 'data')
DP_ARDUINO = join(DP_ROOT, 'arduino')
DP_PROJECTS = join(DP_ROOT, 'projects')
DP_JOBS = join(DP_ROOT, 'jobs')
DP_CONFIGS = join(DP_ROOT, 'configs')
DP_TEMPLATE_ESP32 = join(DP_ROOT, 'arduino/templates/skylift_esp32_esp8266/')
DP_TEMPLATE_ESP8266 = join(DP_ROOT, 'arduino/templates/skylift_esp32_esp8266/')
INO_TEMPLATES = {
  'esp32': DP_TEMPLATE_ESP32,
  'esp8266': DP_TEMPLATE_ESP8266,
  # 'esp32-oled': DP_ARDUINO_TEMPLATE_ESP32_OLED,
  # 'esp8266_oled': DP_ARDUINO_TEMPLATE_ESP8266_OLED,
  }



# ---------------------------------------------------------------------------
# WiFi, BLE Scans
# ---------------------------------------------------------------------------

DTYPES_WIGLE_CSV = {
  'MAC': str,
  'Channel': int,
  'RSSI': int,
  'CurrentLatitude': float,
  'CurrentLongitude': float,
  'Type': str,
  'SSID': str,
}

WIGLE_COL_MAPPINGS = {
  'MAC': 'bssid',
  'SSID': 'ssid',
  'CurrentLatitude': 'latitude',
  'CurrentLongitude': 'longitude',
  'Channel': 'channel',
  'RSSI': 'rssi'
}


DTYPES_IOS_CSV = {
  'SSID': str,
  'BSS': str,
  'RSSI': int,
  'Channel': int
}
IOS_COL_MAPPINGS = {
  'SSID': 'ssid',
  'BSS': 'bssid',
  'RSSI': 'rssi',
  'Channel': 'channel'
}

class ScanType(Enum):
  WIGLE, IOS = range(0, 2)

SCAN_TYPE_LKUP = {
  'wiglewifi': ScanType.WIGLE,
  'ssid': ScanType.IOS
}



# ---------------------------------------------------------------------------
# Arduino templates
# ---------------------------------------------------------------------------


# ---------------------------------------------------------------------------
# Default Job files
# ---------------------------------------------------------------------------

FP_JOBS_ARDUINO = join(DP_JOBS, 'arduino.csv')
FP_JOBS_IOS = join(DP_JOBS, 'ios.csv')
FP_JOBS_WIGLE_API = join(DP_JOBS, 'wigle_api.csv')
FP_JOBS_WIGLE_EXPORT = join(DP_JOBS, 'wigle_export.csv')
FP_LOCATION_SUMMARY = join(DP_ARDUINO, 'networks.h')


# ---------------------------------------------------------------------------
# OSX
# ---------------------------------------------------------------------------

FP_AIRPORT = '/System/Library/PrivateFrameworks/Apple80211.framework/Versions/Current/Resources/airport'


# ---------------------------------------------------------------------------
# ESP32
# ---------------------------------------------------------------------------

ESP32_MAX_TX = "WIFI_POWER_19_5.dBm"

# ---------------------------------------------------------------------------
# ESP8266
# ---------------------------------------------------------------------------

ESP8266_MAX_TX = 20.5  # maximum hardware TX power in dB

# ---------------------------------------------------------------------------
# Admin / Legal
# ---------------------------------------------------------------------------

LICENSE_HEADER = """#############################################################################
#
# SkyLift
# MIT License
# Copyright (c) 2016-2022 Adam Harvey
#
# SkyLift:              https://ahprojects.com/skylift
# DataPools:            https://ahprojects.com/datapools
# Parallel Artifacts:   https://ahprojects.com/parallel-artifacts
# Code:                 https://github.com/adamhrv/skylift
#
#############################################################################
"""

