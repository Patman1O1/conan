from conan.api.conan_api import ConanAPI
from pathlib import Path
import sys
import shutil
import click

@click.command(name="export")
@click.argument("pathname")
@click.option("--recursive", "-r", is_flag=True, default=False)
@click.option("--force", "-f", is_flag=True, default=False)
def export(pathname: str, recursive: bool, force: bool) -> None:
    # Construct the path using the pathname string
    path: Path = Path(pathname)

    # Construct the source path
    src_path: Path = Path(ConanAPI().home_folder)/path

    # Ensure the source path exists
    if not src_path.exists():
        sys.stderr.write(f"No path named '{src_path}' exists\n")
        sys.exit(1)

    # Construct the destination path
    dst_path: Path = Path(__file__).resolve().parent.parent/path

    # Check if the destination path already exists and
    # override it if the --force flag has been set
    if dst_path.exists() and force == False:
        sys.stderr.write(f"The path '{dst_path}' already exists\n")
        sys.exit(1)

    # Ensure parent directories exist
    dst_path.parent.mkdir(parents=True, exist_ok=True)

    # Copy the command to <conan_home>/extensions/commands/
    if recursive:
        shutil.copytree(src_path, dst_path, symlinks=True, dirs_exist_ok=True)
        return
    shutil.copy2(src_path, dst_path.parent)

if __name__ == "__main__":
    export()
