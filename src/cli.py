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

class ClickSimple:
  """Wrapper generator for custom Click CLI"""

  def __init__(self):
    pass


  class CustomGroup(click.Group):
    #global dir_plugins  # from CliGenerator init

    # lists commands in plugin directory
    def list_commands(self, ctx):
      global dir_plugins  # from CliGenerator init
      rv = list(self.commands.keys())
      fp_cmds = [Path(x) for x in Path(dir_plugins).iterdir() \
        if str(x).endswith('.py') \
        and '__init__' not in str(x)]
      for fp_cmd in fp_cmds:
        assert fp_cmd.name not in rv, "[-] Error: {} can't exist in cli.py and {}".format(fp_cmd.name)
        rv.append(fp_cmd.stem)
      rv.sort()
      return rv


    def get_command(self, ctx, cmd_name):
      global dir_plugins  # from CliGenerator init
      if cmd_name in self.commands:
       return self.commands[cmd_name]
      ns = {}
      fpp_cmd = Path(dir_plugins, cmd_name + '.py')
      fp_cmd = fpp_cmd.as_posix()
      if not fpp_cmd.exists():
        sys.exit('[-] {} file does not exist'.format(fpp_cmd))
      code = compile(fpp_cmd.read_bytes(), fp_cmd, 'exec')
      try:
        eval(code, ns, ns)
      except Exception as e:
        print(f'exception: {e}')
        @click.command()
        def _fail():
          raise Exception('while loading {}'.format(fpp_cmd.name))
        _fail.short_help = repr(e)
        _fail.help = repr(e)
        return _fail
      if 'cli' not in ns:
        sys.exit('[-] Error: {} does not contain a cli function'.format(fp_cmd))
      return ns['cli']

  @classmethod
  def create(self, dir_plugins_local):
    global dir_plugins
    dir_plugins = dir_plugins_local
    return self.CustomGroup


from pathlib import Path
DP_CMDS = Path(Path(__file__).parent / 'commands').as_posix()
@click.group(cls=ClickSimple.create(DP_CMDS))
@click.pass_context
def cli(ctx):
  """\033[1m\033[94mSkyLift\033[0m"""
  ctx.opts = {}


if __name__ == '__main__':
  cli()
  