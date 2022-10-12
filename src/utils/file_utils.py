#############################################################################
#
# SkyLift
# MIT License
# Copyright (c) 2016-2022 Adam Harvey
# https://github.com/adamhrv/skylift
#
#############################################################################

import sys
import os
from os.path import join
import stat
import dataclasses

import click
from glob import glob
from pprint import pprint
import pathlib
from pathlib import Path
import json
import csv
import logging

from src.settings import app_cfg as cfg

LOG = logging.getLogger(cfg.LOGGER_NAME)

# ---------------------------------------------------------------------
# Filepath utilities
# ---------------------------------------------------------------------

def mkdirs(fp: str):
  """Ensure parent directories exist for a filepath
  :param fp: string, Path, or click.File
  """
  fpp = ensure_posixpath(fp)
  fpp = fpp.parent if fpp.suffix else fpp
  fpp.mkdir(parents=True, exist_ok=True)

def ensure_dir(fp):
  """Alias for mkdirs"""
  mkdirs(fp)


def ensure_posixpath(fp):
  """Ensures filepath is pathlib.Path
  :param fp: a (str, LazyFile, PosixPath)
  :returns: a PosixPath filepath object
  """
  if type(fp) == str:
    fpp = Path(fp)
  elif type(fp) == click.utils.LazyFile:
    fpp = Path(fp.name)
  elif type(fp) == pathlib.PosixPath:
    fpp = fp
  else:
    raise TypeError('{} is not a valid filepath type'.format(type(fp)))
  return fpp

# ------------------------------------------
# File I/O read/write little helpers
# ------------------------------------------

def get_ext(fpp, lower=True):
  """Retuns the file extension w/o dot
  :param fpp: (Pathlib.path) filepath
  :param lower: (bool) force lowercase
  :returns: (str) file extension (ie 'jpg')
  """
  fpp = ensure_posixpath(fpp)
  ext = fpp.suffix.replace('.', '')
  return ext.lower() if lower else ext


def load_csv(fp, as_list=True):
  """Loads CSV and retuns list of items
  :param fp: string filepath to CSV
  :returns: list of all CSV data
  """ 
  LOG.info('loading: {}'.format(fp))
  with open(fp, 'r') as f:
    items = csv.DictReader(f)
    if as_list:
      items = [x for x in items]
    LOG.info('returning {:,} items'.format(len(items)))
    return items

def load_txt(fp, data_class=None, as_list=True):
  with open(fp, 'rt') as f:
    lines = f.read().rstrip('\n')
  if as_list:
    lines = lines.split('\n')
  if data_class:
    lines = data_class(**lines)
  return lines

def load_json(fp, data_class=None):
  """Loads JSON and returns items
  :param fp: (str) filepath
  :returns: (dict) data from JSON
  """
  if not Path(fp).exists():
    LOG.error('file does not exist: {}'.format(fp))
    return {}
  with open(str(fp), 'r') as f:
    data = json.load(f)
  if data_class:
    data = data_class(**data)
  return data


def write_txt(fp, data, ensure_path=True):
  if not data:
    LOG.error('no data')
    return
    
  if ensure_path:
    mkdirs(fp)
  with open(fp, 'w') as f:
    if type(data) == list:
      f.write('\n'.join(data))
    else:
      f.write(data)
    LOG.info('wrote: {}'.format(fp))


class EnhancedJSONEncoder(json.JSONEncoder):
  def default(self, o):
    if dataclasses.is_dataclass(o):
      return dataclasses.asdict(o)
    return super().default(o)


def write_json(fp, data, minify=True, ensure_path=True, sort_keys=True, indent=2, ensure_ascii=True):
  """Writes JSON data
  """
  if ensure_path:
    mkdirs(fp)
  with open(fp, 'w') as f:
    if minify:
      json.dump(data, f, separators=(',',':'), sort_keys=sort_keys, ensure_ascii=ensure_ascii, cls=EnhancedJSONEncoder)
    else:
      json.dump(data, f, indent=2, sort_keys=sort_keys, ensure_ascii=ensure_ascii, cls=EnhancedJSONEncoder)
    LOG.info('wrote: {}'.format(fp))

def write_csv(fp, data, header=None):
  """ """
  with open(fp, 'w') as f:
    writer = csv.DictWriter(f, fieldnames=header)
    writer.writeheader()
    if type(data) is dict:
      for k, v in data.items():
        fp.writerow('{},{}'.format(k, v))
