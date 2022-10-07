#!/usr/bin/env python

#############################################################################
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


import click

if __name__ == '__main__':
  from app.utils.click_factory import ClickSimple
  @click.group(cls=ClickSimple.create('commands'))
  @click.pass_context
  def cli(ctx):
    """\033[1m\033[94mSkyLift\033[0m"""
    ctx.opts = {}
    
  cli()