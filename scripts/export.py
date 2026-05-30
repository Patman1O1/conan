from conan.api.conan_api import ConanAPI
from pathlib import Path
import sys
import shutil
import click
import traceback

conan_home: Path = Path(ConanAPI().home_folder)
conan_repo: Path = Path(__file__).resolve().parent.parent

def _copy(src_path: Path, dst_path: Path, force: bool) -> None:
    try:
        # Ensure the source path leads to pre-existing file/directory
        if not src_path.exists():
            sys.stderr.write(f"No path named '{src_path}' exists\n")
            sys.exit(1)

        if dst_path.exists():
            # Check if the force flag has been set true
            if not force:
                # Don't remove anything if the force flag is set to false
                sys.stderr.write(f"The path '{dst_path}' already exists\n")
                sys.exit(1)

            # Remove the file or directory at the destination path
            if dst_path.is_dir():
                shutil.rmtree(dst_path)
            else:
                dst_path.unlink()

        # Copy the new file/directory into the destination path
        if src_path.is_dir():
            shutil.copytree(src_path, dst_path, symlinks=True, dirs_exist_ok=True)
        else:
            shutil.copy2(src_path, dst_path)

    except Exception as exception:
        sys.stderr.write(f"{traceback.format_exception(exception)}\n")

@click.group(name="export")
def export() -> None:
    return

@export.command("template")
@click.argument("name")
@click.option("--force", "-f", is_flag=True, default=False)
def template(name: str, force: bool) -> None:
    # Ensure <conan_home>/templates/command/new exists
    template_dir: Path = Path("templates/command/new")
    Path.mkdir(conan_home/template_dir, parents=True, exist_ok=True)

    _copy(conan_repo/template_dir/name, conan_home/template_dir/name, force)
    return

@export.command("cmd")
@click.argument("name")
@click.option("--force", "-f", is_flag=True, default=False)
def cmd(name: str, force: bool) -> None:
    # Ensure <conan_home>/extensions/commands exists
    cmd_dir: Path = Path("extensions/commands")
    Path.mkdir(conan_home/cmd_dir, parents=True, exist_ok=True)

    _copy(conan_repo/cmd_dir/name, conan_home/cmd_dir/name, force)
    return

@export.command("profile")
@click.argument("name")
@click.option("--force", "-f", is_flag=True, default=False)
def profile(name: str, force: bool) -> None:
    # Ensure <conan_home>/profiles exists
    Path.mkdir(conan_home/"profiles", parents=True, exist_ok=True)

    _copy(conan_repo/"profiles"/name, conan_home/"profiles"/name, force)
    return

if __name__ == "__main__":
    export()
    exit(0)
