"""application config
"""
from os.path import join
from pathlib import Path

# ---------------------------------------------------------------------------
# Logger

LOGGER_NAME = 'skylift'
LOGFILE_FORMAT = "%(log_color)s%(levelname)-8s%(reset)s %(cyan)s%(filename)s:%(lineno)s:%(bold_cyan)s%(funcName)s() %(reset)s%(message)s"


# ---------------------------------------------------------------------------
# directories

DIR_DATA = 'data'
DIR_JOBS = join(DIR_DATA, 'jobs')
DIR_JSON = join(DIR_DATA, 'json')
DIR_ARDUINO = join(DIR_DATA, 'ino')
DIR_NETWORKS = join(DIR_DATA, 'networks')


# ---------------------------------------------------------------------------
# Default Job files

FP_JOBS_ARDUINO = join(DIR_JOBS, 'arduino.csv')
FP_JOBS_IOS = join(DIR_JOBS, 'ios.csv')
FP_JOBS_WIGLE_API = join(DIR_JOBS, 'wigle_api.csv')
FP_JOBS_WIGLE_EXPORT = join(DIR_JOBS, 'wigle_export.csv')
FP_LOCATION_SUMMARY = join(DIR_ARDUINO, 'networks.h')


# ---------------------------------------------------------------------------
# OSX

FP_AIRPORT = '/System/Library/PrivateFrameworks/Apple80211.framework/Versions/Current/Resources/airport'


# ---------------------------------------------------------------------------
# ESP8266

ESP_MAX_TX = 20.5  # maximum hardware TX power in dB
ESP_MIN_OP_TX = 15  # minium operational TX power in dB
