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
from dataclasses import dataclass

from dacite import from_dict
import click
from glob import glob
from pprint import pprint
import pathlib
from pathlib import Path
import json
import csv
import logging

from app.settings import app_cfg as cfg

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


def load_csv(fp_in, as_list=True):
  """Loads CSV and retuns list of items
  :param fp_in: string filepath to CSV
  :returns: list of all CSV data
  """ 
  LOG.info('loading: {}'.format(fp_in))
  with open(fp_in, 'r') as fp:
    items = csv.DictReader(fp)
    if as_list:
      items = [x for x in items]
    LOG.info('returning {:,} items'.format(len(items)))
    return items

def load_txt(fp_in, data_class=None, as_list=True):
  with open(fp_in, 'rt') as fp:
    lines = fp.read().rstrip('\n')
  if as_list:
    lines = lines.split('\n')
  if data_class:
    lines = from_dict(data_class=data_class, data=lines)
  return lines

def load_json(fp_in, data_class=None):
  """Loads JSON and returns items
  :param fp_in: (str) filepath
  :returns: (dict) data from JSON
  """
  if not Path(fp_in).exists():
    LOG.error('file does not exist: {}'.format(fp_in))
    return {}
  with open(str(fp_in), 'r') as fp:
    data = json.load(fp)
  if data_class:
    data = from_dict(data_class=data_class, data=data)
  return data


def write_txt(fp_out, data, ensure_path=True):
  if not data:
    LOG.error('no data')
    return
    
  if ensure_path:
    mkdirs(fp_out)
  with open(fp_out, 'w') as fp:
    if type(data) == list:
      fp.write('\n'.join(data))
    else:
      fp.write(data)
    LOG.info('wrote: {}'.format(fp_out))


class EnhancedJSONEncoder(json.JSONEncoder):
  def default(self, o):
    if dataclasses.is_dataclass(o):
      return dataclasses.asdict(o)
    return super().default(o)


def write_json(fp_out, data, minify=True, ensure_path=True, sort_keys=True, indent=2, ensure_ascii=True):
  """
  """
  if ensure_path:
    mkdirs(fp_out)
  with open(fp_out, 'w') as fp:
    if minify:
      json.dump(data, fp, separators=(',',':'), sort_keys=sort_keys, ensure_ascii=ensure_ascii, cls=EnhancedJSONEncoder)
    else:
      json.dump(data, fp, indent=2, sort_keys=sort_keys, ensure_ascii=ensure_ascii, cls=EnhancedJSONEncoder)
    LOG.info('wrote: {}'.format(fp_out))

def write_csv(fp_out, data, header=None):
  """ """
  with open(fp_out, 'w') as fp:
    writer = csv.DictWriter(fp, fieldnames=header)
    writer.writeheader()
    if type(data) is dict:
      for k, v in data.items():
        fp.writerow('{},{}'.format(k, v))
