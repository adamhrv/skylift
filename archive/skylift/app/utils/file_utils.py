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
  log.info('loading: {}'.format(fp_in))
  with open(fp_in, 'r') as fp:
    items = csv.DictReader(fp)
    if as_list:
      items = [x for x in items]
    log.info('returning {:,} items'.format(len(items)))
    return items

def load_text(fp_in):
  with open(fp_in, 'rt') as fp:
    lines = fp.read().rstrip('\n').split('\n')
  return lines

def load_json(fp_in):
  """Loads JSON and returns items
  :param fp_in: (str) filepath
  :returns: data from JSON
  """
  if not Path(fp_in).exists():
    log.error('file does not exist: {}'.format(fp_in))
    return {}
  with open(str(fp_in), 'r') as fp:
    log.info('opening: {}'.format(fp_in))
    data = json.load(fp)
  return data


def write_text(data, fp_out, ensure_path=True):
  if not data:
    log.error('no data')
    return
    
  if ensure_path:
    mkdirs(fp_out)
  with open(fp_out, 'w') as fp:
    if type(data) == list:
      fp.write('\n'.join(data))
    else:
      fp.write(data)
    log.info('wrote: {}'.format(fp_out))

def write_json(data, fp_out, minify=True, ensure_path=True, sort_keys=True, indent=2, ensure_ascii=True):
  """
  """
  if ensure_path:
    mkdirs(fp_out)
  with open(fp_out, 'w') as fp:
    if minify:
      json.dump(data, fp, separators=(',',':'), sort_keys=sort_keys, ensure_ascii=ensure_ascii)
    else:
      json.dump(data, fp, indent=2, sort_keys=sort_keys, ensure_ascii=ensure_ascii)
    log.info('wrote: {}'.format(fp_out))

def write_csv(data, fp_out, header=None):
  """ """
  with open(fp_out, 'w') as fp:
    writer = csv.DictWriter(fp, fieldnames=header)
    writer.writeheader()
    if type(data) is dict:
      for k, v in data.items():
        fp.writerow('{},{}'.format(k, v))


# ---------------------------------------------------------------------
# Filepath utilities
# ---------------------------------------------------------------------

def mkdirs(fp):
  """Ensure parent directories exist for a filepath
  :param fp: string, Path, or click.File
  """
  fpp = ensure_posixpath(fp)
  fpp = fpp.parent if fpp.suffix else fpp
  fpp.mkdir(parents=True, exist_ok=True)

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