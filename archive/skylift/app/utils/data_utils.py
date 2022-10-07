"""
File utilities
"""
import sys
import os
from os.path import join
import stat

import click
from glob import glob
from pprint import pprint
import pathlib
from pathlib import Path
import json
import csv
import logging

from app.settings import app_cfg as cfg

log = logging.getLogger(cfg.LOGGER_NAME)


# def update_val(d, k, v):
#   """Update dict value if v is not None"""
#   if not k in d.keys():
#     d[k] = v
#   else:
#     if v is not None:
#       d[k] = v
#   return d[k]
