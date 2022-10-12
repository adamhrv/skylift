#############################################################################
#
# SkyLift
# MIT License
# Copyright (c) 2016-2022 Adam Harvey
# https://github.com/adamhrv/skylift
#
#############################################################################

def map_range(value, a, b):
  a_min, a_max = a
  b_min, b_max = b
  return b_min + (((value - a_min) / (a_max - a_min)) * (b_max - b_min))