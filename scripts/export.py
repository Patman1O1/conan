from conan.api.conan_api import ConanAPI
from pathlib import Path
import sys
import shutil
import click

conan_home: Path = Path(ConanAPI().home_folder)
conan_repo: Path = Path(__file__).resolve().parent.parent

@click.group(name="export")
def export() -> None:
    pass

@export.command(name="cmd")
@click.argument("name")
@click.option("--force", "-f", is_flag=True, default=False)
def cmd(name: str, force: bool) -> None:
    # Construct the source path to the command
    src_path: Path = conan_repo/"extensions"/"commands"/f"cmd_{name}.py"

    # Ensure a command matching <name> exists
    if not src_path.exists():
        sys.stderr.write(f"No command named '{name}' at '{src_path.parent}'\n")
        sys.exit(1)

    # Construct the destination path
    dest_path: Path = conan_home/"extensions"/"commands"/f"cmd_{name}.py"

    # Check if a command already exists at the destination path
    # and only override it if the --force flag has been set
    if dest_path.exists() and force == False:
        sys.stderr.write(f"A command named '{name}' already exists at '{dest_path.parent}'\n")
        sys.exit(1)

    # Ensure parent directories exist
    dest_path.parent.mkdir(parents=True, exist_ok=True)

    # Copy the command to <conan_home>/extensions/commands/
    shutil.copy2(src_path, dest_path.parent)

@export.command(name="profile")
@click.argument("name")
@click.option("--force", "-f", is_flag=True, default=False)
def profile(name: str, force: bool) -> None:
    # Construct the source path to the profile
    src_path: Path = conan_repo/"profiles"/f"{name}"

    # Ensure a profile matching <name> exists
    if not src_path.exists():
        sys.stderr.write(f"No profile named '{name}' at '{src_path.parent}'\n")
        sys.exit(1)

    # Construct the destination path
    dest_path: Path = conan_home/"profiles"/f"{name}"

    # Check if a profile already exists at the destination path
    # and only override it if the --force flag has been set
    if dest_path.exists() and force == False:
        sys.stderr.write(f"A profile named '{name}' already exists at '{dest_path.parent}'\n")
        sys.exit(1)

    # Ensure parent directories exist
    dest_path.mkdir(parents=True, exist_ok=True)

    # Copy the profile to <conan_home>/profile/
    shutil.copy2(src_path, dest_path.parent)

@export.command(name="template")
@click.argument("name")
@click.option("--force", "-f", is_flag=True, default=False)
def template(name: str, force: bool) -> None:
    # Construct the source path to the template
    src_path: Path = conan_repo/"templates"/"command"/"new"/f"{name}"

    # Ensure a template matching <name> exists
    if not src_path.exists():
        sys.stderr.write(f"No template named '{name}' at '{src_path.parent}'\n")
        sys.exit(1)

    # Construct the destination path
    dest_path: Path = conan_home/"templates"/"command"/"new"/f"{name}"

    # Check if a template already exists at the destination path
    # and only override it if the --force flag has been set
    if dest_path.exists() and force == False:
        sys.stderr.write(f"A template named '{name}' already exists at '{dest_path.parent}'\n")
        sys.exit(1)

    # Ensure parent directories exist
    dest_path.mkdir(parents=True, exist_ok=True)

    # Copy the template to <conan_home>/templates/command/new/
    shutil.copy2(src_path, dest_path.parent)

if __name__ == "__main__":
    export()
