#############################################################################
#
# SkyLift
# MIT License
# Copyright (c) 2016-2022 Adam Harvey
# https://github.com/adamhrv/skylift
#
#############################################################################


import click

from src.settings import app_cfg as cfg
from src.settings import types


# --------------------------------------------------------
# Click command helpers
# --------------------------------------------------------
def enum_to_names(enum_type):
  return {x.name.lower(): x for x in enum_type}
  
def show_help(enum_type):
  names = enum_to_names(enum_type)
  return 'Options: "{}"'.format(', '.join(list(names.keys())))

def get_default(opt):
  return opt.name.lower()


# --------------------------------------------------------
# Custom Click parameter class
# --------------------------------------------------------


class ParamVar(click.ParamType):

  name = 'default_type'

  def __init__(self, param_type):
    # self.name = '{}'.format(param_type.name.lower())
    # sealf.
    self.ops =  {x.name.lower(): x for x in param_type}
  
  def convert(self, value, param, ctx):
    """converts (str) repr to Enum hash"""
    try:
      return self.ops[value.lower()]
    except:
      self.fail('{} is not a valid option'.format(value, param, ctx))



















