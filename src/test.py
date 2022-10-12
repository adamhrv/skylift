import click

@click.group()
@click.option(
    "-v",
    "--verbose",
    count=True,
    default=0,
    help="-v for DEBUG",
)
def cli(verbose):
    from src.settings.app_cfg import LOG
    LOG.debug('ok')
    print(verbose)

@cli.command("list")
@click.option(
    "--option1"
)
def my_list_command(option1):
    print(option1)


@cli.command("find")
@click.option(
    "--option2"
)
def my_find_command(option2):
     print(option2)


if __name__ == '__main__':
   cli()
